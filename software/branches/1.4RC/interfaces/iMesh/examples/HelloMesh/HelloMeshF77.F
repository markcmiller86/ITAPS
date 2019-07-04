c *******************************************************
c A simple example that shows the creation of an
c iMesh interface object.  The particular
c implementation used is determined by the included libraries in the
c makefile.  All supported ITAPS implementations read a triangle,
c vtk file, and this is used to show the simple loading of a mesh
c and the retrieval of the geometric dimension.
c********************************************************
#define CHECK(a) \
      if (err .ne. 0) print *, a



      subroutine load_mesh (msh, rset)
      implicit none
#include "iBase_f.h"
#include "iMesh_f.h"
      iMesh_Instance msh
      iBase_EntitySetHandle rset
      integer err

c create the imesh instance and get the root set
      call iMesh_newMesh("", msh, err)
      CHECK("Failed to create imesh instance.")

      call iMesh_getRootSet(%VAL(msh), rset, err)
      CHECK("Failed to get root set.")

c Load the vtk mesh from a file.  The only elements that are guaranteed
c     to be supported by all implementations are triangles.  Others may be
c     supported, but this is implementation dependent.
c
c     next to last argument is number of characters in filename (3rd argument)
      call iMesh_load(%VAL(msh), %VAL(rset),
     1                "../MeshFiles/125hex.vtk", "", err)
      CHECK("Failed to load mesh file.")

      return
      end



      subroutine check_mesh (msh, rset)
      implicit none
#include "iBase_f.h"
#include "iMesh_f.h"
      iMesh_Instance msh 
      iBase_EntitySetHandle rset
      integer err, dims, num_ent

c Get the geometric dimension of the mesh to show that the we have
c actually loaded a mesh
      call iMesh_getGeometricDimension(%VAL(msh), dims, err)
      CHECK("Failed in getGeometricDimension")
      print *, "dimension = ", dims

c Get the number of vertices and elements in the mesh to show the use of
c enumerated types
      do 10 dims = iBase_VERTEX, iBase_REGION
         call iMesh_getNumOfType(%VAL(msh), %VAL(rset), %VAL(dims), 
     1        num_ent, err)
         CHECK("Failed in getNumOfType.\n")
         print *, "number of ", dims, "-D entities = ", num_ent 
 10   continue

      return
      end


      program hello_mesh
      implicit none
#include "iBase_f.h"
#include "iMesh_f.h"

      iMesh_Instance imesh
      iBase_EntitySetHandle root_set
      integer err

      call load_mesh(imesh, root_set)

      call check_mesh(imesh, root_set)

c Call destructor on interface
      call iMesh_dtor(%VAL(imesh), err)
      CHECK("Failed to destruct imesh instance.")

      return
      end
