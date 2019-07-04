#ifndef NT
#include <sys/resource.h>
#endif
#ifdef SOLARIS
extern "C" int getrusage(int, struct rusage *);
#ifndef RUSAGE_SELF
#include </usr/ucbinclude/sys/rusage.h>
#endif
#endif

#include "stdio.h"
#include "stdlib.h"

double LENGTH = 1.0;

#define CHECK_SIZE(array, type, size)  \
          if (array == 0 || array ## _alloc == 0) {\
            array = (type*) malloc(size*sizeof(type));\
            array ## _alloc = size; \
          } \
          else if (array ## _alloc < size) { \
            printf("Array passed in is non-zero but too short.\n"); \
            exit(0); }

void print_time_(const int print_em, double *tot_time, 
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
 system("ps o args,sz,rss,vsz | grep perf | grep -v grep");  /* RedHat 9.0 doesnt fill in actual memory data */
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
    /* 2D TFI on a face starting at a, with strides stride1 in ada and stride2 in tse*/
  int i, j;
  for (j = 1; j < nelem; j++) {
    double tse = j * xint;
    for (i = 1; i < nelem; i++) {
      double ada = i * xint;
      
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

void build_coords_(const int nelem, double **coords) 
{
  double ttime0, ttime1, utime1, stime1;
  int numv;
  int numv_sq;
  int tot_numv;
  int coords_ptr_alloc;
  double *coords_ptr;
  int idx;
  double scale1, scale2, scale3;
  int i000, ia00, i0t0, iat0, i00g, ia0g, i0tg, iatg;
  int adaInts, tseInts, gammaInts;
  double cX, cY, cZ;
  int i, j, k;
  double tse, ada, gamma, tm1, am1, gm1;
  double *ai0k, *aiak, *a0jk, *atjk, *aij0, *aijg;

  print_time_(0, &ttime0, &utime1, &stime1);

    /* allocate the memory*/
  numv = nelem+1;
  numv_sq = numv*numv;
  tot_numv = numv*numv*numv;
  coords_ptr_alloc = 0;
  coords_ptr  = *coords;
  CHECK_SIZE(coords_ptr, double, 3*tot_numv);
  *coords = coords_ptr;

/* use FORTRAN-like indexing*/
#define VINDEX(i,j,k) (i + (j*numv) + (k*numv_sq))
    /* use these to prevent optimization on 1-scale, etc (real map wouldn't have*/
    /* all these equal)*/
  scale1 = LENGTH/nelem;
  scale2 = LENGTH/nelem;
  scale3 = LENGTH/nelem;

#ifdef REALTFI
    /* use a real TFI xform to compute coordinates*/
    /* initialize positions of 8 corners*/
  coords_ptr[VINDEX(0,0,0)] = coords_ptr[VINDEX(0,0,0)+nelem+1] = coords_ptr[VINDEX(0,0,0)+2*(nelem+1)] = 0.0;
  coords_ptr[VINDEX(0,nelem,0)] = coords_ptr[VINDEX(0,nelem,0)+2*(nelem+1)] = 0.0; coords_ptr[VINDEX(0,nelem,0)+nelem+1] = LENGTH;
  coords_ptr[VINDEX(0,0,nelem)] = coords_ptr[VINDEX(0,0,nelem)+nelem+1] = 0.0; coords_ptr[VINDEX(0,0,nelem)+2*(nelem+1)] = LENGTH;
  coords_ptr[VINDEX(0,nelem,nelem)] = 0.0; coords_ptr[VINDEX(0,nelem,nelem)+nelem+1] = coords_ptr[VINDEX(0,nelem,nelem)+2*(nelem+1)] = LENGTH;
  coords_ptr[VINDEX(nelem,0,0)] = LENGTH; coords_ptr[VINDEX(nelem,0,0)+nelem+1] = coords_ptr[VINDEX(nelem,0,0)+2*(nelem+1)] = 0.0;
  coords_ptr[VINDEX(nelem,0,nelem)] = coords_ptr[VINDEX(nelem,0,nelem)+2*(nelem+1)] = LENGTH; coords_ptr[VINDEX(nelem,0,nelem)+nelem+1] = 0.0;
  coords_ptr[VINDEX(nelem,nelem,0)] = coords_ptr[VINDEX(nelem,nelem,0)+nelem+1] = LENGTH; coords_ptr[VINDEX(nelem,nelem,0)+2*(nelem+1)] = 0.0;
  coords_ptr[VINDEX(nelem,nelem,nelem)] = coords_ptr[VINDEX(nelem,nelem,nelem)+nelem+1] = coords_ptr[VINDEX(nelem,nelem,nelem)+2*(nelem+1)] = LENGTH;

    /* compute edges*/
    /* i (stride=1)*/
  compute_edge(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, 1);
  compute_edge(&coords_ptr[VINDEX(0,nelem,0)], nelem, scale1, 1);
  compute_edge(&coords_ptr[VINDEX(0,0,nelem)], nelem, scale1, 1);
  compute_edge(&coords_ptr[VINDEX(0,nelem,nelem)], nelem, scale1, 1);
    /* j (stride=numv)*/
  compute_edge(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, numv);
  compute_edge(&coords_ptr[VINDEX(nelem,0,0)], nelem, scale1, numv);
  compute_edge(&coords_ptr[VINDEX(0,0,nelem)], nelem, scale1, numv);
  compute_edge(&coords_ptr[VINDEX(nelem,0,nelem)], nelem, scale1, numv);
    /* k (stride=numv^2)*/
  compute_edge(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, numv_sq);
  compute_edge(&coords_ptr[VINDEX(nelem,0,0)], nelem, scale1, numv_sq);
  compute_edge(&coords_ptr[VINDEX(0,nelem,0)], nelem, scale1, numv_sq);
  compute_edge(&coords_ptr[VINDEX(nelem,nelem,0)], nelem, scale1, numv_sq);

    /* compute faces*/
    /* i=0, nelem*/
  compute_face(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, numv, numv_sq);
  compute_face(&coords_ptr[VINDEX(nelem,0,0)], nelem, scale1, numv, numv_sq);
    /* j=0, nelem*/
  compute_face(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, 1, numv_sq);
  compute_face(&coords_ptr[VINDEX(0,nelem,0)], nelem, scale1, 1, numv_sq);
    /* k=0, nelem*/
  compute_face(&coords_ptr[VINDEX(0,0,0)], nelem, scale1, 1, numv);
  compute_face(&coords_ptr[VINDEX(0,0,nelem)], nelem, scale1, 1, numv);

    /* initialize corner indices*/
  
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
          /* blocked coordinate ordering*/
        coords_ptr[idx] = i*scale1;
        coords_ptr[tot_numv+idx] = j*scale2;
        coords_ptr[2*tot_numv+idx] = k*scale3;
      }
    }
  }
#endif
  print_time_(0, &ttime1, &utime1, &stime1);
  printf("iBase/MOAB: TFI time = %g sec\n", ttime1-ttime0);
}

void build_connect_(const int nelem, const int vstart, int **connect) 
{
    /* allocate the memory*/
  int nume_tot;
  int *connect_ptr;
  int vijk;
  int numv;
  int numv_sq;
  int idx;
  int i, j, k;

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
        if (i > numv*numv*numv) exit(0);
      }
    }
  }
}
