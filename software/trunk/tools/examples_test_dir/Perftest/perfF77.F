c ITAPS Mesh Interface brick mesh performance test
c 
c This program tests ITAPS mesh interface functions used to 
c create a square structured mesh.  Boilerplate taken from 
c ITAPS mesh interface test in MOAB and performance test in MOAB

#define TFLAG %VAL(0)
#define CHECK(a) \
      if (err .ne. 0) print *, a

      program perfF

#include "iBase_f.h"
#include "iMesh_f.h"
      integer nelem
      character*8 elem_ch
      character argv0*128, ctest*1
      IBASE_HANDLE_T rpcoords, rpconnect
      real*8 coords
      integer coords_size, coords_alloc
      integer connect
      integer connect_siz, connect_alloc
      pointer (rpcoords, coords(0:*))
      pointer (rpconnect, connect(0:*))
      iMesh_Instance mesh
      inargs = iargc()
      if (inargs .lt. 2) then
         call getarg(0, argv0)
         write(*,'(A)') 'Usage: ', argv0, '<ints_per_side> <B|I>'
         write(*,'(A)') '   B - Bulk interface'
         write(*,'(A)') '   I - Individual entity interface'
         stop
      end if
  
c     print *, 'Input nelem and test (B=Blocked, I=Indiv)'
c     read *, nelem, ctest

c
c get values of nelem and ctest from argument list; convert from 
c character to integer for nelem
      call getarg(1, elem_ch)
      read (elem_ch,*) nelem

      call getarg(2, ctest)

      write(*,'(a,i4)') 'number of elements: ', nelem

c pre-build the coords array
      rpcoords = 0
      call build_coords(%VAL(nelem), rpcoords)

      call build_connect(%VAL(nelem), %VAL(1), rpconnect);

c create an implementation
      call iMesh_newMesh("", mesh, ierr)

      if (ctest .eq. "B") then
         call testBulk(mesh, nelem, coords, connect)
      else if (ctest .eq. 'I') then
         call testIndiv(mesh, nelem, coords)
      else 
         write(*,*) "Last argument must be either 'B' or 'I'"
         stop
      end if

      call iMesh_dtor(%VAL(mesh), ierr)
      call free(rpconnect)
      call free(rpcoords)
      stop
      end

      subroutine testBulk(mesh, nelem, coords, connect) 

#include "iBase_f.h"
#include "iMesh_f.h"      

      iMesh_Instance mesh
      real*8 coords(0:*)
      integer connect(0:*)
      IBASE_HANDLE_T sidl_verts, sidl_connect
      real*8 utime, stime, ttime0, ttime1, ttime2, ttime3
      IBASE_HANDLE_T ipsidl_verts, ipsidl_connect
      IBASE_HANDLE_T ipconnect, rpcoords
      pointer (ipsidl_verts, sidl_verts(0:*))
      integer sidl_verts_alloc, sidl_verts_size
      pointer (ipsidl_connect, sidl_connect(0:*))
      integer sidl_connect_alloc, sidl_connect_size

      IBASE_HANDLE_T ipnew_hexes, new_hexes
      pointer (ipnew_hexes, new_hexes(0:*))
      integer new_hexes_alloc, new_hexes_size

      IBASE_HANDLE_T ipstatus
      integer status
      pointer (ipstatus, status(0:*))
      integer status_alloc, status_size
      integer storage_order

      rpcoords = loc(coords)
      ipconnect = loc(connect)

      call print_time(TFLAG, ttime0, utime, stime)
      num_verts = (nelem + 1)*(nelem + 1)*(nelem + 1)
      num_elems = nelem*nelem*nelem
      storage_order = iBase_BLOCKED
  
c create vertices as a block
      sidl_verts_alloc = 0
      sidl_verts_size = 0
      ipsidl_verts = 0
      call iMesh_createVtxArr(%VAL(mesh), %VAL(num_verts), 
     1     %VAL(storage_order), %VAL(rpcoords), %VAL(3*num_verts),
     1     ipsidl_verts, sidl_verts_alloc, sidl_verts_size, 
     1     ierr)
      if (iBase_SUCCESS .ne. ierr) then
         print *,"Couldn't create vertices in bulk call"
         stop
      end if

c need to explicitly fill connectivity array, since we don''t know
c the format of entity handles
c     ipsidl_connect = malloc(8*num_elems*POINTER_SIZE);
      ipsidl_connect = malloc(8*num_elems*8);
      do 10 i = 0, 7
c use connect[i]-1 because we used starting vertex index (vstart) of 1
         sidl_connect(i) = sidl_verts(connect(i)-1);
   10 continue

C create the entities
      new_hexes_alloc = 0
      status_alloc = 0
  
      call iMesh_createEntArr(%VAL(mesh), %VAL(iMesh_HEXAHEDRON), 
     1     %VAL(ipsidl_connect), %VAL(8), 
     1     ipnew_hexes, new_hexes_alloc, new_hexes_size,
     1     ipstatus, status_alloc, status_size, ierr)
      if (iBase_SUCCESS .ne. ierr) then
         print *, "Couldn't create hex elements in bulk call"
         stop
      end if

      call print_time(TFLAG, ttime1, utime, stime)

c query the mesh 2 ways
      call query_elem_to_vert(mesh)

      call print_time(TFLAG, ttime2, utime, stime)

      call query_vert_to_elem(mesh)
  
      call print_time(TFLAG, ttime3, utime, stime)

      write(*,'(a, i4, g12.6, g12.6, g12.6, a)') 
     1  'SIDL Bulk ucd: nelem, constr, e_to_v query, v_to_e query = ', 
     1  nelem, ttime1-ttime0, ttime2-ttime1, ttime3-ttime2
      call free(ipsidl_verts)
      call free(ipsidl_connect)
      call free(ipnew_hexes)
      call free(ipstatus)
      return
      end

C######################################################################
C testIndiv
C######################################################################
      subroutine testIndiv(mesh, nelem, coords)

#include "iBase_f.h"
#include "iMesh_f.h"      

      iMesh_Instance mesh
      real*8 coords(0:*)

      real*8 utime, stime, ttime0, ttime1, ttime2, ttime3

      IBASE_HANDLE_T ipsidl_verts
      IBASE_HANDLE_T sidl_verts
      pointer (ipsidl_verts, sidl_verts(0:*))
      integer sidl_verts_alloc, sidl_verts_size

      real*8 tmp_coords(0:2)
      IBASE_HANDLE_T tmp_conn(0:7), new_hex
      integer status

      call print_time(TFLAG, ttime0, utime, stime)

c need some dimensions
      numv = nelem + 1
      numv_sq = numv*numv
      num_verts = numv*numv*numv
#define VINDEX(i,j,k) (i + (j*numv) + (k*numv_sq))

c array to hold vertices created individually
c     ipsidl_verts = malloc(POINTER_SIZE*num_verts)
      ipsidl_verts = malloc(8*num_verts)

c create the vertex
      do 20 i = 0, num_verts-1
         call iMesh_createVtx(%VAL(mesh), 
     1        %VAL(coords(i)), %VAL(coords(num_verts+i)), 
     1        %VAL(coords(2*num_verts+i)),
     1        sidl_verts(i), ierr)
         if (iBase_SUCCESS .ne. ierr) then
            print *, "Couldn't create vertex in individual call"
            stop
         end if
   20 continue
  
      do 30 i=0, nelem-1
         do 40 j=0, nelem-1
            do 50 k=0, nelem-1
               ijkv = VINDEX(i,j,k)
               tmp_conn(0) = sidl_verts(ijkv)
               tmp_conn(1) = sidl_verts(ijkv+1)
               tmp_conn(2) = sidl_verts(ijkv+1+numv)
               tmp_conn(3) = sidl_verts(ijkv+numv)
               tmp_conn(4) = sidl_verts(ijkv+numv*numv)
               tmp_conn(5) = sidl_verts(ijkv+1+numv*numv)
               tmp_conn(6) = sidl_verts(ijkv+1+numv+numv*numv)
               tmp_conn(7) = sidl_verts(ijkv+numv+numv*numv)
        
c create the entity
               call iMesh_createEnt(%VAL(mesh), %VAL(iMesh_HEXAHEDRON), 
     1              tmp_conn, %VAL(8), new_hex, status, ierr)
               if (iBase_SUCCESS .ne. ierr) then
                  print *, "Couldn't create hex element in indiv call"
                  stop
               end if
  50        continue
  40     continue
  30  continue
      call free(ipsidl_verts)

      call print_time(TFLAG, ttime1, utime, stime)

c query the mesh 2 ways
      call query_elem_to_vert(mesh)

      call print_time(TFLAG, ttime2, utime, stime)

      call query_vert_to_elem(mesh)
  
      call print_time(TFLAG, ttime3, utime, stime)

      write(*,'(a, i4, g12.6, g12.6, g12.6, a)') 
     1  'Fort Indiv ucd: nelem, constr, e_to_v query, v_to_e query = ', 
     1  nelem, ttime1-ttime0, ttime2-ttime1, ttime3-ttime2
      
      return
      end

      subroutine query_elem_to_vert(mesh)

#include "iBase_f.h"
#include "iMesh_f.h"      

      iMesh_Instance mesh

      IBASE_HANDLE_T ipall_hexes
      IBASE_HANDLE_T all_hexes
      integer all_hexes_size, all_hexes_alloc
      pointer (ipall_hexes, all_hexes(0:*))

      IBASE_HANDLE_T ipdum_connect
      IBASE_HANDLE_T dum_connect
      integer dum_connect_size, dum_connect_alloc
      pointer (ipdum_connect, dum_connect(0:*))
      
      IBASE_HANDLE_T rpdum_coords
      integer dum_coords_alloc, dum_coords_size
      real*8 dum_coords
      pointer (rpdum_coords, dum_coords(0:*))

      real*8 centroid(0:2)
      iBase_EntitySetHandle root_set

      call iMesh_getRootSet(%VAL(imesh), root_set, err)
      CHECK("Failed to get root set.")

c get all the hex elements
      all_hexes_alloc = 0
      call iMesh_getEntities(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_REGION), %VAL(iMesh_HEXAHEDRON), 
     1     ipall_hexes, all_hexes_alloc, all_hexes_size, ierr)
      if (iBase_SUCCESS .ne. ierr) then
         print *, "Couldn't get all hex elements in query_mesh"
         stop
      end if         

c loop over elements
      do 60 i = 0, all_hexes_size-1
         dum_connect_alloc = 0;
c get the connectivity of this element; will allocate space on 1st iteration,
c but will have correct size on subsequent ones
         call iMesh_getEntAdj(%VAL(mesh), %VAL(all_hexes(i)), 
     1        %VAL(iBase_VERTEX),
     1        ipdum_connect, dum_connect_alloc, dum_connect_size,
     1        ierr)

c get vertex coordinates; ; will allocate space on 1st iteration,
c but will have correct size on subsequent ones
         dum_coords_alloc = 0
         iorder = iBase_BLOCKED
         call iMesh_getVtxArrCoords(%VAL(mesh), %VAL(ipdum_connect), 
     1        %VAL(dum_connect_size), iorder,
     1        rpdum_coords, dum_coords_alloc, dum_coords_size, ierr)

         centroid = (3*0.0)
         if (iorder .eq. iBase_BLOCKED) then
            do 70 j = 0, 7
               centroid(0) = centroid(0) + dum_coords(j)
               centroid(1) = centroid(1) + dum_coords(8+j)
               centroid(2) = centroid(2) + dum_coords(16+j)
  70        continue
         else 
            do 80 j = 0, 7
               centroid(0) = centroid(0) + dum_coords(3*j)
               centroid(1) = centroid(1) + dum_coords(3*j+1)
               centroid(2) = centroid(2) + dum_coords(3*j+2)
  80        continue
         end if
         call free(rpdum_coords)
         call free(ipdum_connect)
  60  continue

      call free(ipall_hexes)
      if (iBase_SUCCESS .ne. ierr) then
         print *, "Problem getting connectivity or vertex coords"
         stop
      end if
      return
      end

      subroutine query_vert_to_elem(mesh)

#include "iBase_f.h"
#include "iMesh_f.h"      

      iMesh_Instance mesh

      IBASE_HANDLE_T ipall_verts
      IBASE_HANDLE_T all_verts
      integer all_verts_size, all_verts_alloc
      pointer (ipall_verts, all_verts(0:*))

      IBASE_HANDLE_T ipdum_hexes
      IBASE_HANDLE_T dum_hexes
      integer dum_hexes_size, dum_hexes_alloc
      pointer (ipdum_hexes, dum_hexes(0:*))

      iBase_EntitySetHandle root_set

      call iMesh_getRootSet(%VAL(imesh), root_set, err)
      CHECK("Failed to get root set.")

      all_verts_size = 0
      all_verts_alloc = 0
      ierr = iBase_SUCCESS

c get all the vertices elements
      call iMesh_getEntities(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_VERTEX), %VAL(iMesh_POINT), 
     1     ipall_verts, all_verts_alloc, all_verts_size, ierr)

      if (iBase_SUCCESS .ne. ierr) then
         print *, "Couldn't get all vertices in query_vert_to_elem"
         stop
      end if

c loop over vertices
      do 90 i = 0, all_verts_size-1

c get the connectivity of this element; will have to allocate space on every
c iteration, since size can vary
         dum_hexes_alloc = 0
         call iMesh_getEntAdj(%VAL(mesh), %VAL(all_verts(i)), 
     1        %VAL(iBase_REGION),
     1        ipdum_hexes, dum_hexes_alloc, dum_hexes_size, ierr)
         if (iBase_SUCCESS .ne. ierr) then
            print *, "Problem getting connectivity or vertex coords."
            stop
         end if

         if (dum_hexes_alloc .ne. 0) call free(ipdum_hexes)
  90  continue
      call free(ipall_verts)

      return
      end

