c FindAdjacency: Interacting with iMesh
c 
c This program shows how to get more information about a mesh, by
c getting connectivity two different ways (as connectivity and as
c adjacent 0-dimensional entities).

c Usage: FindAdjacency

#define CHECK(a) \
      if (ierr .ne. 0) print *, a

      program findconnect
      implicit none
#include "iMesh_f.h"

c declarations
      iMesh_Instance mesh
      iBase_EntitySetHandle root_set
      integer*POINTER_SIZE rpents, rpverts, rpallverts, ipoffsets
      integer ioffsets
      integer*POINTER_SIZE ents, verts, allverts
      pointer (rpents, ents(0:*))
      pointer (rpverts, verts(0:*))
      pointer (rpallverts, allverts(0:*))
      pointer (ipoffsets, ioffsets(0,*))
      integer ierr, ents_alloc, ents_size
      integer iverts_alloc, iverts_size
      integer allverts_alloc, allverts_size
      integer offsets_alloc, offsets_size
      integer vert_uses, i

c create the Mesh instance
      call iMesh_newMesh("", mesh, ierr)
      CHECK("Problems instantiating interface.")

c get root set
      call iMesh_getRootSet(%VAL(mesh), root_set, ierr)
      CHECK("Problems getting root set")

c load the mesh
      call iMesh_load(%VAL(mesh), %VAL(root_set), 
     1     "../MeshFiles/125hex.vtk", "", ierr)
      CHECK("Load failed")

c get all 3d elements
      ents_alloc = 0
      call iMesh_getEntities(%VAL(mesh), %VAL(root_set),
     $     %VAL(iBase_REGION),%VAL(iMesh_ALL_TOPOLOGIES), rpents,
     $     ents_alloc, ents_size,ierr)
      CHECK("Couldn't get entities")

      vert_uses = 0

c iterate through them; 
      do i = 0, ents_size-1
c get connectivity
         iverts_alloc = 0
         call iMesh_getEntAdj(%VAL(mesh), %VAL(ents(i)), 
     1        %VAL(iBase_VERTEX), rpverts, iverts_alloc, iverts_size,
     1        ierr)
         CHECK("Failure in getEntAdj")

c sum number of vertex uses
         vert_uses = vert_uses + iverts_size

         if (iverts_size .ne. 0) call free(rpverts)
      end do

c now get adjacencies in one big block
      allverts_alloc = 0
      offsets_alloc = 0
      call iMesh_getEntArrAdj(%VAL(mesh), %VAL(rpents), 
     1     %VAL(ents_size), %VAL(iBase_VERTEX), rpallverts, 
     1     allverts_alloc, allverts_size, ipoffsets, offsets_alloc, 
     1     offsets_size, ierr)
      CHECK("Failure in getEntArrAdj")

      if (allverts_size .ne. 0) call free(rpallverts)
      if (offsets_size .ne. 0) call free(ipoffsets)
      if (ents_size .ne. 0) call free(rpents)

c compare results of two calling methods
      if (allverts_size .ne. vert_uses) then
         write(*,'("FAILED: Sizes did NOT agree!")')
      else 
         write(*,'("SUCCEEDED: Sizes did agree!")')
      endif

      call iMesh_dtor(%VAL(mesh), ierr)
      CHECK("Failed to destroy interface")

      end
