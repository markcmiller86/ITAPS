! ITAPS Mesh Interface brick mesh performance test
! 
! This program tests ITAPS mesh interface functions used to 
! create a square structured mesh.  Boilerplate taken from 
! ITAPS mesh interface test in MOAB and performance test in MOAB

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

! print *, 'Input nelem and test (B=Blocked, I=Indiv)'
! read *, nelem, ctest

!
! get nelem and ctest values from argument list; convert from
! character to integer for nelem

  call getarg(1, elem_ch)
  read(elem_ch,*) nelem

  call getarg(2, ctest)  

  write(*,'(a,i4)') 'number of elements: ', nelem

  ! pre-build the coords array
  rpcoords = 0
  call build_coords(%VAL(nelem), rpcoords)

  call build_connect(%VAL(nelem), %VAL(1), rpconnect)

  ! create an implementation
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
end program perfF

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

  rpcoords = loc(coords)

  call print_time(TFLAG, ttime0, utime, stime)
  num_verts = (nelem + 1)*(nelem + 1)*(nelem + 1)
  num_elems = nelem*nelem*nelem

  ! create vertices as a block
  sidl_verts_alloc = num_verts
  sidl_verts_size = num_verts
  ! ipsidl_verts = malloc(POINTER_SIZE*num_verts)
  ipsidl_verts = malloc(8*num_verts)
  call iMesh_createVtxArr(%VAL(mesh), %VAL(num_verts), &
       %VAL(iBase_BLOCKED), %VAL(rpcoords), %VAL(3*num_verts), &
       ipsidl_verts, sidl_verts_alloc, sidl_verts_size, &
       ierr)
  if (iBase_SUCCESS .ne. ierr) then
     print *,"Couldn't create vertices in bulk call"
     stop
  end if

  ! need to explicitly fill connectivity array, since we don't know
  ! the format of entity handles
  ! ipsidl_connect = malloc(8*num_elems*POINTER_SIZE);
  ipsidl_connect = malloc(8*num_elems*8);
  do i = 0, 7
     ! use connect[i]-1 because we used starting vertex index (vstart) of 1
     sidl_connect(i) = sidl_verts(connect(i)-1);
  end do

  ! create the entities
  new_hexes_alloc = 0
  status_alloc = 0

  call iMesh_createEntArr(%VAL(mesh), %VAL(iMesh_HEXAHEDRON), &
       %VAL(ipsidl_connect), %VAL(8), &
       ipnew_hexes, new_hexes_alloc, new_hexes_size, &
       ipstatus, status_alloc, status_size, ierr)
  if (iBase_SUCCESS .ne. ierr) then
     print *, "Couldn't create hex elements in bulk call"
     stop
  end if

  call print_time(TFLAG, ttime1, utime, stime)

  ! query the mesh 2 ways
  call query_elem_to_vert(mesh)

  call print_time(TFLAG, ttime2, utime, stime)

  call query_vert_to_elem(mesh)

  call print_time(TFLAG, ttime3, utime, stime)

  write(*,'(a, i4, g12.6, g12.6, g12.6, a)') &
       'SIDL Bulk ucd: nelem, constr, e_to_v query, v_to_e query = ', &
       nelem, ttime1-ttime0, ttime2-ttime1, ttime3-ttime2
  call free(ipnew_hexes)
  call free(ipstatus)
  call free(ipsidl_connect)
  call free(ipsidl_verts)
  return
end subroutine testBulk

subroutine testIndiv(mesh, nelem, coords)

#include "iBase_f.h"
#include "iMesh_f.h"      

  iMesh_Instance mesh
  real*8 coords(0:*)

  real*8 utime, stime, ttime0, ttime1, ttime2, ttime3

  IBASE_HANDLE_T ipsidl_verts, sidl_verts
  pointer (ipsidl_verts, sidl_verts(0:*))
  integer sidl_verts_alloc, sidl_verts_size

  real*8 tmp_coords(0:2)
  IBASE_HANDLE_T tmp_conn(0:7), new_hex
  integer status

  call print_time(TFLAG, ttime0, utime, stime)

  ! need some dimensions
  numv = nelem + 1
  numv_sq = numv*numv
  num_verts = numv*numv*numv
#define VINDEX(i,j,k) (i + (j*numv) + (k*numv_sq))

  ! array to hold vertices created individually
  !ipsidl_verts = malloc(POINTER_SIZE*num_verts)
  ipsidl_verts = malloc(8*num_verts)

  ! create the vertex
  do i = 0, num_verts-1
     call iMesh_createVtx(%VAL(mesh), &
          %VAL(coords(i)), %VAL(coords(num_verts+i)), &
          %VAL(coords(2*num_verts+i)), &
          sidl_verts(i), ierr)
     if (iBase_SUCCESS .ne. ierr) then
        print *, "Couldn't create vertex in individual call"
        stop
     end if
  end do

  do i=0, nelem-1
     do j=0, nelem-1
        do k=0, nelem-1
           ijkv = VINDEX(i,j,k)
           tmp_conn(0) = sidl_verts(ijkv)
           tmp_conn(1) = sidl_verts(ijkv+1)
           tmp_conn(2) = sidl_verts(ijkv+1+numv)
           tmp_conn(3) = sidl_verts(ijkv+numv)
           tmp_conn(4) = sidl_verts(ijkv+numv*numv)
           tmp_conn(5) = sidl_verts(ijkv+1+numv*numv)
           tmp_conn(6) = sidl_verts(ijkv+1+numv+numv*numv)
           tmp_conn(7) = sidl_verts(ijkv+numv+numv*numv)

           ! create the entity
           call iMesh_createEnt(%VAL(mesh), %VAL(iMesh_HEXAHEDRON), &
                tmp_conn, %VAL(8), new_hex, status, ierr)
           if (iBase_SUCCESS .ne. ierr) then
              print *, "Couldn't create hex element in indiv call"
              stop
           end if
        end do
     end do
  end do

  call free(ipsidl_verts)
  call print_time(TFLAG, ttime1, utime, stime)

  ! query the mesh 2 ways
  call query_elem_to_vert(mesh)

  call print_time(TFLAG, ttime2, utime, stime)

  call query_vert_to_elem(mesh)

  call print_time(TFLAG, ttime3, utime, stime)

  write(*,'(a, i4, g12.6, g12.6, g12.6, a)') &
       'Fort Indiv ucd: nelem, constr, e_to_v query, v_to_e query = ', &
       nelem, ttime1-ttime0, ttime2-ttime1, ttime3-ttime2

  return
end subroutine testIndiv

subroutine query_elem_to_vert(mesh)

#include "iBase_f.h"
#include "iMesh_f.h"      

  iMesh_Instance mesh

  IBASE_HANDLE_T ipall_hexes, all_hexes
  integer all_hexes_size, all_hexes_alloc
  pointer (ipall_hexes, all_hexes(0:*))

  IBASE_HANDLE_T ipdum_connect, dum_connect
  integer dum_connect_size
  integer dum_connect_alloc
  pointer (ipdum_connect, dum_connect(0:*))

  IBASE_HANDLE_T rpdum_coords
  integer dum_coords_alloc, dum_coords_size
  real*8 dum_coords
  pointer (rpdum_coords, dum_coords(0:*))

  real*8 centroid(0:2)

  iBase_EntitySetHandle root_set

  call iMesh_getRootSet(%VAL(imesh), root_set, err)
  CHECK("Failed to get root set.")

  ! get all the hex elements
  all_hexes_alloc = 0
  call iMesh_getEntities(%VAL(mesh), %VAL(root_set), %VAL(iBase_REGION), &
       %VAL(iMesh_HEXAHEDRON), &
       ipall_hexes, all_hexes_alloc, all_hexes_size, ierr)
  if (iBase_SUCCESS .ne. ierr) then
     print *, "Couldn't get all hex elements in query_mesh"
     stop
  end if

  ! loop over elements
  do i = 0, all_hexes_size-1
     dum_connect_alloc = 0;
     ! get the connectivity of this element; will allocate space on 1st iteration,
     ! but will have correct size on subsequent ones
     call iMesh_getEntAdj(%VAL(mesh), %VAL(all_hexes(i)), &
          %VAL(iBase_VERTEX), &
          ipdum_connect, dum_connect_alloc, dum_connect_size, &
          ierr)

     ! get vertex coordinates; ; will allocate space on 1st iteration,
     ! but will have correct size on subsequent ones
     dum_coords_alloc = 0
     iorder = iBase_BLOCKED
     call iMesh_getVtxArrCoords(%VAL(mesh), %VAL(ipdum_connect), &
          %VAL(dum_connect_size), iorder, &
          rpdum_coords, dum_coords_alloc, dum_coords_size, ierr)

     centroid = (3*0.0)
     if (iorder .eq. iBase_BLOCKED) then
        do j = 0, 7
           centroid(0) = centroid(0) + dum_coords(j)
           centroid(1) = centroid(1) + dum_coords(8+j)
           centroid(2) = centroid(2) + dum_coords(16+j)
        end do
     else 
        do j = 0, 7
           centroid(0) = centroid(0) + dum_coords(3*j)
           centroid(1) = centroid(1) + dum_coords(3*j+1)
           centroid(2) = centroid(2) + dum_coords(3*j+2)
        end do
     end if
     call free(rpdum_coords)
     call free(ipdum_connect)
  end do

  call free(ipall_hexes)

  if (iBase_SUCCESS .ne. ierr) then
     print *, "Problem getting connectivity or vertex coords"
     stop
  end if
  return
end subroutine query_elem_to_vert

subroutine query_vert_to_elem(mesh)

#include "iBase_f.h"
#include "iMesh_f.h"      

  iMesh_Instance mesh

  IBASE_HANDLE_T ipall_verts, all_verts
  integer all_verts_size, all_verts_alloc
  pointer (ipall_verts, all_verts(0:*))

  IBASE_HANDLE_T ipdum_hexes, dum_hexes
  integer dum_hexes_size, dum_hexes_alloc
  pointer (ipdum_hexes, dum_hexes(0:*))

  iBase_EntitySetHandle root_set

  call iMesh_getRootSet(%VAL(imesh), root_set, err)
  CHECK("Failed to get root set.")

  ! get all the vertices elements
  all_verts_alloc = 0
  all_verts_size = 0
  call iMesh_getEntities(%VAL(mesh), %VAL(root_set), %VAL(iBase_VERTEX), &
       %VAL(iMesh_POINT), &
       ipall_verts, all_verts_alloc, all_verts_size, ierr)

  if (iBase_SUCCESS .ne. ierr) then
     print *, "Couldn't get all vertices in query_vert_to_elem"
     stop
  end if

  ! loop over vertices
  do i = 0, all_verts_size-1

     ! get the connectivity of this element; will have to allocate space on every
     ! iteration, since size can vary
     dum_hexes_alloc = 0
     call iMesh_getEntAdj(%VAL(mesh), %VAL(all_verts(i)), &
          %VAL(iBase_REGION), &
          ipdum_hexes, dum_hexes_alloc, dum_hexes_size, ierr)
     if (iBase_SUCCESS .ne. ierr) then
        print *, "Problem getting connectivity or vertex coords."
        stop
     end if
     if (dum_hexes_alloc .ne. 0) then
        call free(ipdum_hexes)
     end if
  end do
  call free(ipall_verts)
  return
end subroutine query_vert_to_elem

