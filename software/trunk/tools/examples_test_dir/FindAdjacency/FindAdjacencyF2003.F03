! FindConnect: Interacting with iMesh
!
! This program shows how to get more information about a mesh, by
! getting connectivity two different ways (as connectivity and as
! adjacent 0-dimensional entities).

! Usage: FindConnect

#include "iMesh_iso.F03"

program findconnect

use iBase
use iMesh
use iso_c_binding

implicit none

! declarations
type(c_ptr) :: mesh, root_set
integer iverts_alloc, iverts_size
integer, dimension(:), pointer :: offsets
integer ierr, ents_alloc, ents_size
integer allverts_alloc, allverts_size
integer offsets_alloc, offsets_size
type(c_ptr) ents_C, verts_C, allverts_C, offsets_C
type(c_ptr), pointer, dimension(:) :: ents
integer i, ivert_uses
character(len=100) :: fileName

fileName = "125hex.vtk  "

! create the Mesh instance
call iMesh_newMesh("", mesh, ierr, 0)

! identify the root set
call iMesh_getRootSet(mesh, root_set, ierr)

! load the mesh
call iMesh_load(mesh, root_set, fileName, "", ierr, 10, 0)

! get all 3d elements
ents_alloc = 0
call iMesh_getEntities(mesh, root_set, iBase_REGION, iMesh_ALL_TOPOLOGIES, &
     ents_C, ents_alloc, ents_size, ierr)
call C_F_POINTER(ents_C, ents, [ents_alloc])

ivert_uses = 0

! iterate through them;
iverts_alloc = 0
do i = 1, ents_size
   ! get connectivity; verts_C is allocated in the first call, but not thereafter
  call iMesh_getEntAdj(mesh, ents(i), iBase_VERTEX, &
       verts_C, iverts_alloc, iverts_size, ierr)
  ! sum number of vertex uses
  ivert_uses = ivert_uses + iverts_size
end do

! now get adjacencies in one big block
allverts_alloc = 0
offsets_alloc = 0
call iMesh_getEntArrAdj(mesh, ents_C, ents_size, &
     iBase_VERTEX, allverts_C, allverts_alloc, allverts_size, &
     offsets_C, offsets_alloc, offsets_size, ierr)

! compare results of two calling methods
write(*,*) allverts_size, ivert_uses
if (allverts_size .ne. ivert_uses) then
   write(*,'("Sizes didn''t agree!")')
else
   write(*,'("Sizes did agree!")')
endif

call iMesh_dtor(mesh, ierr)

end program findconnect
