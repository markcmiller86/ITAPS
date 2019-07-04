! *******************************************************
! A simple example that shows the creation of an
! iMesh interface object.  The particular
! implementation used is determined by the included libraries in the
! makefile.  All supported ITAPS implementations read a triangle,
! vtk file, and this is used to show the simple loading of a mesh
! and the retrieval of the geometric dimension.
!********************************************************

#define CHECK(a) if (err .ne. 0) print *, a

program hello_mesh
implicit none

#include "iBase_f.h"
#include "iMesh_f.h"

iMesh_Instance imesh
iBase_EntitySetHandle root_set
integer err, dim, num_ent

! create the imesh instance and get the root set
call iMesh_NewMesh("", imesh, err)
CHECK("Failed to create imesh instance.")

call iMesh_getRootSet(%VAL(imesh), root_set, err)
CHECK("Failed to get root set.")

! Load the vtk mesh from a file.  The only elements that are guaranteed
!     to be supported by all implementations are triangles.  Others may be
!     supported, but this is implementation dependent.
!   next to last argument is number of characters in filename (3rd argument)
call iMesh_load(%VAL(imesh), %VAL(root_set), "../MeshFiles/125hex.vtk", &
                "", err)
CHECK("Failed to load mesh file.")

! Get the geometric dimension of the mesh to show that the we have
! actually loaded a mesh
call iMesh_getGeometricDimension(%VAL(imesh), dim, err)
CHECK("Failed in getGeometricDimension")
print *, "dimension = ", dim

! Get the number of vertices and elements in the mesh to show the use of
! enumerated types
do dim = iBase_VERTEX, iBase_REGION
   call iMesh_getNumOfType(%VAL(imesh), %VAL(root_set), %VAL(dim), &
        num_ent, err)
   CHECK("Failed in getNumOfType.\n")
   print *, "number of ", dim, "-D entities = ", num_ent 
end do

! Call destructor on interface
call iMesh_dtor(%VAL(imesh), err)
CHECK("Failed to destruct imesh instance.")

return
end

