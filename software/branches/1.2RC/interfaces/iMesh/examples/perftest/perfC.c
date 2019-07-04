/** ITAPS Mesh Interface brick mesh performance test
 * 
 * This program tests ITAPS mesh interface functions used to 
 * create a square structured mesh.  Boilerplate taken from 
 * ITAPS mesh interface test in MOAB and performance test in MOAB
 *
 */

/* Different platforms follow different conventions for usage */
#ifndef NT
#include <sys/resource.h>
#endif
#ifdef SOLARIS
extern "C" int getrusage(int, struct rusage *);
#ifndef RUSAGE_SELF
#include </usr/ucbinclude/sys/rusage.h>
#endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "iMesh.h"


double LENGTH = 1.0;

/* forward declare some functions */
void query_vert_to_elem(iMesh_Instance mesh);
void query_elem_to_vert(iMesh_Instance mesh);
void print_time(const int print_em, double *tot_time, 
                double *utime, double *stime);
void testBulk(iMesh_Instance mesh, 
              const int nelem, double *coords,
              const int *connect);
void testIndiv(iMesh_Instance mesh, const int nelem, 
               double *coords);
void compute_edge(double *start, const int nelem,  const double xint,
                  const int stride);
void compute_face(double *a, const int nelem,  const double xint,
                  const int stride1, const int stride2);
void build_coords(const int nelem, double **coords);
void build_connect(const int nelem, const int vstart, int **connect);
#define CHECK_SIZE(array, type, size)                                 \
    if (array == NULL || array ## _alloc == 0) {                      \
      array = (type*) malloc(size*sizeof(type));}                     \
    else if (array ## _alloc < size) {                                \
      printf("Array passed in is non-zero but too short.\n");         \
      assert(0); }

int main( int argc, char *argv[] )
{
  double *coords;
  int *connect;
  int nelem;
  int ierr;
  iMesh_Instance mesh;

  nelem = 20;
  if (argc < 3) {
    printf("Usage: %s <ints_per_side> <B|I>\n", argv[0]);
    printf("   B - Bulk interface\n");
    printf("   I - Individual entity interface\n");
    
    return 1;
  }
  
  sscanf(argv[1], "%d", &nelem);

  printf("number of elements: %d\n", nelem);

    /* pre-build the coords array */
  coords = NULL;
  build_coords(nelem, &coords);
  assert(NULL != coords);

  connect = NULL;
  build_connect(nelem, 1, &connect);

    /* create an implementation */
  iMesh_newMesh("", &mesh, &ierr, 0);

  if (argv[2][0] == 'B') 
    testBulk(mesh, nelem, coords, connect);
  else if (argv[2][0] == 'I') 
    testIndiv(mesh, nelem, coords);
  else {
    printf("Last argument must be either 'B' or 'I'\n");
    return 1;
  }
  
  free(coords);
  free(connect);

  iMesh_dtor(mesh, &ierr);
  
  return 0;
}

void testBulk(iMesh_Instance mesh, 
              const int nelem, double *coords,
              const int *connect) 
{
  double utime, stime, ttime0, ttime1, ttime2, ttime3;
  int num_verts;
  int num_elems;
  iBase_EntityHandle *sidl_vertices;
  int sidl_vertices_alloc, sidl_vertices_size;
  int ierr;
  iBase_EntityHandle *sidl_connect;
  int sidl_connect_size, sidl_connect_alloc;
  iBase_EntityHandle *new_hexes;
  int new_hexes_size, new_hexes_alloc;
  int *status;
  int status_size, status_alloc;
  int i;

  print_time(0, &ttime0, &utime, &stime);
  num_verts = (nelem + 1)*(nelem + 1)*(nelem + 1);
  num_elems = nelem*nelem*nelem;
  
    /* create vertices as a block */
  sidl_vertices = NULL;
  sidl_vertices_alloc = sidl_vertices_size = 0;
  CHECK_SIZE(sidl_vertices, iBase_EntityHandle, num_verts);
  iMesh_createVtxArr(mesh, num_verts, iBase_BLOCKED,
                     coords, 3*num_verts,
                     &sidl_vertices, &sidl_vertices_alloc, 
                     &sidl_vertices_size, &ierr);
  if (iBase_SUCCESS != ierr) {
    printf("Couldn't create vertices in bulk call\n");
    return;
  }

    /* need to explicitly fill connectivity array, since we don't know */
    /* the format of entity handles */
  sidl_connect = NULL;
  sidl_connect_size = 8 * num_elems;
  sidl_connect_alloc = 0;
  CHECK_SIZE(sidl_connect, iBase_EntityHandle, 8*num_elems);
  for (i = 0; i < sidl_connect_size; i++) {
      /* use connect[i]-1 because we used starting vertex index (vstart) of 1 */
    assert(connect[i]-1 < num_verts);
    sidl_connect[i] = sidl_vertices[connect[i]-1];
  }
  
    /* create the entities */
  new_hexes = NULL;
  new_hexes_alloc = 0;
  status = NULL;
  status_alloc = 0;
  iMesh_createEntArr(mesh, iMesh_HEXAHEDRON, sidl_connect, 
                     sidl_connect_size, 
                     &new_hexes, &new_hexes_alloc, &new_hexes_size,
                     &status, &status_alloc, &status_size, &ierr);
  if (iBase_SUCCESS != ierr) {
    printf("Couldn't create hex elements in bulk call\n");
    return;
  }

  free(new_hexes);
  free(status);
  free(sidl_vertices);
  free(sidl_connect);

  print_time(0, &ttime1, &utime, &stime);

    /* query the mesh 2 ways */
  query_elem_to_vert(mesh);

  print_time(0, &ttime2, &utime, &stime);

  query_vert_to_elem(mesh);
  
  print_time(0, &ttime3, &utime, &stime);

  printf("C Bulk ucd: nelem, construct, e_to_v query, v_to_e query = " 
         "%d, %g, %g, %g seconds\n",
         nelem, ttime1-ttime0, ttime2-ttime1, ttime3-ttime2);
}

void testIndiv(iMesh_Instance mesh, 
               const int nelem, double *coords) 
{
  double utime, stime, ttime0, ttime1, ttime2, ttime3;
  int numv;
  int numv_sq;
  int num_verts;
  iBase_EntityHandle *sidl_vertices;
  int sidl_vertices_size, sidl_vertices_alloc;
  int ierr, i;
  iBase_EntityHandle tmp_conn[8];
  iBase_EntityHandle new_hex;
  int status;
  int j, k, vijk;

  print_time(0, &ttime0, &utime, &stime);

    /* need some dimensions */
  numv = nelem + 1;
  numv_sq = numv*numv;
  num_verts = numv*numv*numv;
#define VINDEX(i,j,k) (i + (j*numv) + (k*numv_sq))

    /* array to hold vertices created individually */
  sidl_vertices_size = num_verts;
  sidl_vertices_alloc = 0;
  sidl_vertices = NULL;
  CHECK_SIZE(sidl_vertices, iBase_EntityHandle, num_verts);

    /* temporary array to hold vertex positions for single vertex */
  for (i = 0; i < num_verts; i++) {

      /* create the vertex */
    iMesh_createVtx(mesh, 
                    coords[i], coords[num_verts+i], coords[2*num_verts+i],
                    sidl_vertices+i, &ierr);
    if (iBase_SUCCESS != ierr) {
      printf("Couldn't create vertex in individual call\n");
      return;
    }
  }
  
  for (i=0; i < nelem; i++) {
    for (j=0; j < nelem; j++) {
      for (k=0; k < nelem; k++) {

        vijk = VINDEX(i,j,k);
        tmp_conn[0] = sidl_vertices[vijk];
        tmp_conn[1] = sidl_vertices[vijk+1];
        tmp_conn[2] = sidl_vertices[vijk+1+numv];
        tmp_conn[3] = sidl_vertices[vijk+numv];
        tmp_conn[4] = sidl_vertices[vijk+numv*numv];
        tmp_conn[5] = sidl_vertices[vijk+1+numv*numv];
        tmp_conn[6] = sidl_vertices[vijk+1+numv+numv*numv];
        tmp_conn[7] = sidl_vertices[vijk+numv+numv*numv];
        
          /* create the entity */
        iMesh_createEnt(mesh, iMesh_HEXAHEDRON, 
                        tmp_conn, 8, &new_hex, &status, &ierr);
        if (iBase_SUCCESS != ierr) {
          printf("Couldn't create hex element in individual call\n");
          return;
        }
      }
    }
  }

  free(sidl_vertices);

  print_time(0, &ttime1, &utime, &stime);

    /* query the mesh 2 ways */
  query_elem_to_vert(mesh);

  print_time(0, &ttime2, &utime, &stime);

  query_vert_to_elem(mesh);
  
  print_time(0, &ttime3, &utime, &stime);

  printf("C Indiv ucd: nelem, construct, e_to_v query, v_to_e query = "
         "%d, %g, %g, %g seconds.\n",
         nelem, ttime1-ttime0, ttime2-ttime1, ttime3-ttime2);
}

void query_elem_to_vert(iMesh_Instance mesh)
{
  iBase_EntityHandle *all_hexes;
  int all_hexes_size, all_hexes_alloc;
  int ierr;
  int i, j;
  iBase_EntityHandle *dum_connect;
  int dum_connect_size, dum_connect_alloc;
  double *dum_coords;
  int dum_coords_size, dum_coords_alloc;
  int order;
  double centroid[3];

    /* get all the hex elements */
  all_hexes_alloc = 0;
  iMesh_getEntities(mesh, 0, iBase_REGION, 
                    iMesh_HEXAHEDRON, 
                    &all_hexes, &all_hexes_alloc, &all_hexes_size,
                    &ierr);
  if (iBase_SUCCESS != ierr) {
    printf("Couldn't get all hex elements in query_mesh\n");
    return;
  }

    /* loop over elements */
  dum_coords = NULL;
  dum_connect = NULL;
  dum_coords_alloc = dum_connect_alloc = 0;
  for (i = 0; i < all_hexes_size; i++) {
      /* get the connectivity of this element; will allocate space on 1st iteration, */
      /* but will have correct size on subsequent ones */
    iMesh_getEntAdj(mesh, all_hexes[i], iBase_VERTEX,
                    &dum_connect, &dum_connect_alloc, &dum_connect_size,
                    &ierr);

      /* get vertex coordinates; ; will allocate space on 1st iteration, */
      /* but will have correct size on subsequent ones */
    order = iBase_UNDETERMINED;
    iMesh_getVtxArrCoords(mesh, dum_connect, dum_connect_size, &order,
                          &dum_coords, &dum_coords_alloc, &dum_coords_size,
                          &ierr);

    assert(24 == dum_coords_size && dum_coords_alloc == 24);
    centroid[0] = centroid[1] = centroid[2] = 0.0;
    if (order == iBase_BLOCKED) {
      for (j = 0; j < 8; j++) {
        centroid[0] += dum_coords[j];
        centroid[1] += dum_coords[8+j];
        centroid[2] += dum_coords[16+j];
      }
    }
    else {
      for (j = 0; j < 8; j++) {
        centroid[0] += dum_coords[3*j];
        centroid[1] += dum_coords[3*j+1];
        centroid[2] += dum_coords[3*j+2];
      }
    }
  }
  if (iBase_SUCCESS != ierr) {
    printf("Problem getting connectivity or vertex coords.\n");
    return;
  }
  
  free(dum_coords);
  free(dum_connect);
  free(all_hexes);
}

void query_vert_to_elem(iMesh_Instance mesh)
{
  iBase_EntityHandle *all_verts;
  int all_verts_size, all_verts_alloc;
  int ierr;
  iBase_EntityHandle *dum_hexes;
  int dum_hexes_size, dum_hexes_alloc;
  int i;

  all_verts = NULL;
  all_verts_alloc = 0;

    /* get all the vertices elements */
  iMesh_getEntities(mesh, 0, iBase_VERTEX, iMesh_POINT, 
                    &all_verts, &all_verts_alloc, &all_verts_size,
                    &ierr);
  if (iBase_SUCCESS != ierr) {
    printf("Couldn't get all vertices in query_vert_to_elem\n");
    return;
  }

    /* loop over vertices */
  for (i = 0; i < all_verts_size; i++) {
    dum_hexes = NULL;
    dum_hexes_alloc = 0;

      /* get the connectivity of this element; will have to allocate space on every */
      /* iteration, since size can vary */
    iMesh_getEntAdj(mesh, all_verts[i], iBase_REGION,
                    &dum_hexes, &dum_hexes_alloc, &dum_hexes_size,
                    &ierr);
    if (iBase_SUCCESS != ierr) {
      printf("Problem getting connectivity or vertex coords.\n");
      return;
    }

    free(dum_hexes);
  }
  free(all_verts);
}

void print_time(const int print_em, double *tot_time, 
                double *utime, double *stime) 
{
  struct rusage r_usage;
  getrusage(RUSAGE_SELF, &r_usage);
  *utime = (double)r_usage.ru_utime.tv_sec +
      ((double)r_usage.ru_utime.tv_usec/1.e6);
  *stime = (double)r_usage.ru_stime.tv_sec +
      ((double)r_usage.ru_stime.tv_usec/1.e6);
  *tot_time = *utime + *stime;
  if (print_em)
    printf("User, system, total time = %g, %g, %g\n", *utime, *stime,
           *tot_time);
#ifndef LINUX
  printf("Max resident set size = %ld bytes\n", r_usage.ru_maxrss*4096);
  printf("Int resident set size = %ld bytes\n", r_usage.ru_idrss);
#else
  system("ps o args,sz,rss,vsz | grep perf | grep -v grep");  /* RedHat 9.0 doesnt fill in actual memory data  */
#endif
}

void compute_edge(double *start, const int nelem,  const double xint,
                  const int stride) 
{
  int i;
  for (i = 1; i < nelem; i++) {
    start[i*stride] = start[0]+i*xint;
    start[nelem+1+i*stride] = start[nelem+1]+i*xint;
    start[2*(nelem+1)+i*stride] = start[2*(nelem+1)]+i*xint;
  }
}

void compute_face(double *a, const int nelem,  const double xint,
                  const int stride1, const int stride2) 
{
  int i, j;
  double tse, ada;
  
    /* 2D TFI on a face starting at a, with strides stride1 in ada and stride2 in tse */
  for (j = 1; j < nelem; j++) {
    tse = j * xint;
    for (i = 1; i < nelem; i++) {
      ada = i * xint;
      
      a[i*stride1+j*stride2] = (1.0 - ada)*a[i*stride1]
          + ada*a[i*stride1+nelem*stride2]
          + (1.0 - tse)*a[j*stride2]
          + tse*a[j*stride2+nelem*stride1]
          - (1.0 - tse)*(1.0 - ada)*a[0]
          - (1.0 - tse)*ada*a[nelem*stride1]
          - tse*(1.0 - ada)*a[nelem*stride2]
          - tse*ada*a[nelem*(stride1+stride2)];
      a[nelem+1+i*stride1+j*stride2] = (1.0 - ada)*a[nelem+1+i*stride1]
          + ada*a[nelem+1+i*stride1+nelem*stride2]
          + (1.0 - tse)*a[nelem+1+j*stride2]
          + tse*a[nelem+1+j*stride2+nelem*stride1]
          - (1.0 - tse)*(1.0 - ada)*a[nelem+1+0]
          - (1.0 - tse)*ada*a[nelem+1+nelem*stride1]
          - tse*(1.0 - ada)*a[nelem+1+nelem*stride2]
          - tse*ada*a[nelem+1+nelem*(stride1+stride2)];
      a[2*(nelem+1)+i*stride1+j*stride2] = (1.0 - ada)*a[2*(nelem+1)+i*stride1]
          + ada*a[2*(nelem+1)+i*stride1+nelem*stride2]
          + (1.0 - tse)*a[2*(nelem+1)+j*stride2]
          + tse*a[2*(nelem+1)+j*stride2+nelem*stride1]
          - (1.0 - tse)*(1.0 - ada)*a[2*(nelem+1)+0]
          - (1.0 - tse)*ada*a[2*(nelem+1)+nelem*stride1]
          - tse*(1.0 - ada)*a[2*(nelem+1)+nelem*stride2]
          - tse*ada*a[2*(nelem+1)+nelem*(stride1+stride2)];
    }
  }
}

void build_coords(const int nelem, double **coords) 
{
  double ttime0, ttime1, utime1, stime1;
  int numv, numv_sq, tot_numv;
  int coords_ptr_alloc;
  double *coords_ptr;
  int idx;
  double scale1, scale2, scale3;
  int i000, ia00, i0t0, iat0, i00g, ia0g, i0tg, iatg;
  double cX, cY, cZ;
  int adaInts, tseInts, gammaInts;
  double tse, ada, gamma, tm1, am1, gm1,
      *ai0k, *aiak, *a0jk, *atjk, *aij0, *aijg;
  int i, j, k;

  print_time(0, &ttime0, &utime1, &stime1);

    /* allocate the memory */
  numv = nelem+1;
  numv_sq = numv*numv;
  tot_numv = numv*numv*numv;
  coords_ptr_alloc = 0;
  coords_ptr = *coords;
  CHECK_SIZE(coords_ptr, double, 3*tot_numv);
  *coords = coords_ptr;

/* use FORTRAN-like indexing */
#define VINDEX(i,j,k) (i + (j*numv) + (k*numv_sq))
    /* use these to prevent optimization on 1-scale, etc (real map wouldn't have */
    /* all these equal) */
  scale1 = LENGTH/nelem;
  scale2 = LENGTH/nelem;
  scale3 = LENGTH/nelem;

#ifdef REALTFI
    /* use a real TFI xform to compute coordinates */
    /* initialize positions of 8 corners */
  coords_ptr[VINDEX(0,0,0)] = coords_ptr[VINDEX(0,0,0)+nelem+1] = coords_ptr[VINDEX(0,0,0)+2*(nelem+1)] = 0.0;
  coords_ptr[VINDEX(0,nelem,0)] = coords_ptr[VINDEX(0,nelem,0)+2*(nelem+1)] = 0.0; coords_ptr[VINDEX(0,nelem,0)+nelem+1] = LENGTH;
  coords_ptr[VINDEX(0,0,nelem)] = coords_ptr[VINDEX(0,0,nelem)+nelem+1] = 0.0; coords_ptr[VINDEX(0,0,nelem)+2*(nelem+1)] = LENGTH;
  coords_ptr[VINDEX(0,nelem,nelem)] = 0.0; coords_ptr[VINDEX(0,nelem,nelem)+nelem+1] = coords_ptr[VINDEX(0,nelem,nelem)+2*(nelem+1)] = LENGTH;
  coords_ptr[VINDEX(nelem,0,0)] = LENGTH; coords_ptr[VINDEX(nelem,0,0)+nelem+1] = coords_ptr[VINDEX(nelem,0,0)+2*(nelem+1)] = 0.0;
  coords_ptr[VINDEX(nelem,0,nelem)] = coords_ptr[VINDEX(nelem,0,nelem)+2*(nelem+1)] = LENGTH; coords_ptr[VINDEX(nelem,0,nelem)+nelem+1] = 0.0;
  coords_ptr[VINDEX(nelem,nelem,0)] = coords_ptr[VINDEX(nelem,nelem,0)+nelem+1] = LENGTH; coords_ptr[VINDEX(nelem,nelem,0)+2*(nelem+1)] = 0.0;
  coords_ptr[VINDEX(nelem,nelem,nelem)] = coords_ptr[VINDEX(nelem,nelem,nelem)+nelem+1] = coords_ptr[VINDEX(nelem,nelem,nelem)+2*(nelem+1)] = LENGTH;

    /* compute edges */
    /* i (stride=1) */
  compute_edge(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, 1);
  compute_edge(&coords_ptr[VINDEX(0,nelem,0)], nelem, scale1, 1);
  compute_edge(&coords_ptr[VINDEX(0,0,nelem)], nelem, scale1, 1);
  compute_edge(&coords_ptr[VINDEX(0,nelem,nelem)], nelem, scale1, 1);
    /* j (stride=numv) */
  compute_edge(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, numv);
  compute_edge(&coords_ptr[VINDEX(nelem,0,0)], nelem, scale1, numv);
  compute_edge(&coords_ptr[VINDEX(0,0,nelem)], nelem, scale1, numv);
  compute_edge(&coords_ptr[VINDEX(nelem,0,nelem)], nelem, scale1, numv);
    /* k (stride=numv^2) */
  compute_edge(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, numv_sq);
  compute_edge(&coords_ptr[VINDEX(nelem,0,0)], nelem, scale1, numv_sq);
  compute_edge(&coords_ptr[VINDEX(0,nelem,0)], nelem, scale1, numv_sq);
  compute_edge(&coords_ptr[VINDEX(nelem,nelem,0)], nelem, scale1, numv_sq);

    /* compute faces */
    /* i=0, nelem */
  compute_face(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, numv, numv_sq);
  compute_face(&coords_ptr[VINDEX(nelem,0,0)], nelem, scale1, numv, numv_sq);
    /* j=0, nelem */
  compute_face(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, 1, numv_sq);
  compute_face(&coords_ptr[VINDEX(0,nelem,0)], nelem, scale1, 1, numv_sq);
    /* k=0, nelem */
  compute_face(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, 1, numv);
  compute_face(&coords_ptr[VINDEX(0,0,nelem)], nelem, scale1, 1, numv);

    /* initialize corner indices */
  i000 = VINDEX(0,0,0);
  ia00 = VINDEX(nelem,0,0);
  i0t0 = VINDEX(0,nelem,0);
  iat0 = VINDEX(nelem,nelem,0);
  i00g = VINDEX(0,0,nelem);
  ia0g = VINDEX(nelem,0,nelem);
  i0tg = VINDEX(0,nelem,nelem);
  iatg = VINDEX(nelem,nelem,nelem);
  adaInts = nelem;
  tseInts = nelem;
  gammaInts = nelem;
  
  
  for (i=1; i < nelem; i++) {
    for (j=1; j < nelem; j++) {
      for (k=1; k < nelem; k++) {
        idx = VINDEX(i,j,k);
        tse = i*scale1;
        ada = j*scale2;
        gamma = k*scale3;
        tm1 = 1.0 - tse;
        am1 = 1.0 - ada;
        gm1 = 1.0 - gamma;

        cX = gm1 *   (am1*(tm1*coords_ptr[i000] + tse*coords_ptr[i0t0])  +
                      ada*(tm1*coords_ptr[ia00] + tse*coords_ptr[iat0])) +
            gamma * (am1*(tm1*coords_ptr[i00g] + tse*coords_ptr[i0tg])  +
                     ada*(tm1*coords_ptr[ia0g] + tse*coords_ptr[iatg]));

        cY = gm1 *   (am1*(tm1*coords_ptr[i000] + tse*coords_ptr[i0t0])  +
                      ada*(tm1*coords_ptr[ia00] + tse*coords_ptr[iat0])) +
            gamma * (am1*(tm1*coords_ptr[i00g] + tse*coords_ptr[i0tg])  +
                     ada*(tm1*coords_ptr[ia0g] + tse*coords_ptr[iatg]));

        cZ = gm1 *   (am1*(tm1*coords_ptr[i000] + tse*coords_ptr[i0t0])  +
                      ada*(tm1*coords_ptr[ia00] + tse*coords_ptr[iat0])) +
            gamma * (am1*(tm1*coords_ptr[i00g] + tse*coords_ptr[i0tg])  +
                     ada*(tm1*coords_ptr[ia0g] + tse*coords_ptr[iatg]));

        ai0k = &coords_ptr[VINDEX(k,0,i)];
        aiak = &coords_ptr[VINDEX(k,adaInts,i)];
        a0jk = &coords_ptr[VINDEX(k,j,0)];
        atjk = &coords_ptr[VINDEX(k,j,tseInts)];
        aij0 = &coords_ptr[VINDEX(0,j,i)];
        aijg = &coords_ptr[VINDEX(gammaInts,j,i)];
  
        coords_ptr[VINDEX(i,j,k)] = (   am1*ai0k[0] 
                                        + ada*aiak[0] 
                                        + tm1*a0jk[0] 
                                        + tse*atjk[0]
                                        + gm1*aij0[0] 
                                        + gamma*aijg[0] )/2.0 - cX/2.0;

        coords_ptr[nelem+1+VINDEX(i,j,k)] = (   am1*ai0k[nelem+1] 
                                                + ada*aiak[nelem+1] 
                                                + tm1*a0jk[nelem+1] 
                                                + tse*atjk[nelem+1]
                                                + gm1*aij0[nelem+1] 
                                                + gamma*aijg[nelem+1] )/2.0 - cY/2.0;

        coords_ptr[2*(nelem+1)+VINDEX(i,j,k)] = (   am1*ai0k[2*(nelem+1)] 
                                                    + ada*aiak[2*(nelem+1)] 
                                                    + tm1*a0jk[2*(nelem+1)] 
                                                    + tse*atjk[2*(nelem+1)]
                                                    + gm1*aij0[2*(nelem+1)] 
                                                    + gamma*aijg[2*(nelem+1)] )/2.0 - cZ/2.0;
      }
    }
  }
  

#else
  for (i=0; i < numv; i++) {
    for (j=0; j < numv; j++) {
      for (k=0; k < numv; k++) {
        idx = VINDEX(i,j,k);
          /* blocked coordinate ordering */
        coords_ptr[idx] = i*scale1;
        coords_ptr[tot_numv+idx] = j*scale2;
        coords_ptr[2*tot_numv+idx] = k*scale3;
      }
    }
  }
#endif
  print_time(0, &ttime1, &utime1, &stime1);
  printf("iBase/MOAB: TFI time = %g sec\n", ttime1-ttime0);
}

void build_connect(const int nelem, const int vstart, int **connect) 
{
  int nume_tot;
  int *connect_ptr;
  int vijk, numv, numv_sq, idx, i, j, k;

    /* allocate the memory */
  nume_tot = nelem*nelem*nelem;
  *connect = (int*) malloc(8*nume_tot*sizeof(int));
  connect_ptr = *connect;

  numv = nelem + 1;
  numv_sq = numv*numv;
  idx = 0;
  for (i=0; i < nelem; i++) {
    for (j=0; j < nelem; j++) {
      for (k=0; k < nelem; k++) {
        vijk = vstart+VINDEX(i,j,k);
        connect_ptr[idx++] = vijk;
        connect_ptr[idx++] = vijk+1;
        connect_ptr[idx++] = vijk+1+numv;
        connect_ptr[idx++] = vijk+numv;
        connect_ptr[idx++] = vijk+numv*numv;
        connect_ptr[idx++] = vijk+1+numv*numv;
        connect_ptr[idx++] = vijk+1+numv+numv*numv;
        connect_ptr[idx++] = vijk+numv+numv*numv;
        assert(i <= numv*numv*numv);
      }
    }
  }
}
