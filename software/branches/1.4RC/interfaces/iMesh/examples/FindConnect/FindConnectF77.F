c FindConnect: Interacting with iMesh
c 
c This program shows how to get more information about a mesh, by
c getting connectivity two different ways (as connectivity and as
c adjacent 0-dimensional entities).

c Usage: FindConnect

#define CHECK(a) \
      if (ierr .ne. 0) print *, a

      program findconnect
#include "iBase_f.h"
#include "iMesh_f.h"

c declarations
      iMesh_Instance mesh
      iBase_EntitySetHandle root_set
      IBASE_HANDLE_T ents, verts, allverts
      IBASE_HANDLE_T rpverts, rpallverts, ipoffsets
      pointer (rpents, ents(0:*))
      pointer (rpverts, verts(0:*))
      pointer (rpallverts, allverts(0:*))
      pointer (ipoffsets, ioffsets(0,*))
      integer ierr, ents_alloc, ents_size
      integer iverts_alloc, iverts_size
      integer allverts_alloc, allverts_size
      integer offsets_alloc, offsets_size

c create the Mesh instance
      call iMesh_newMesh("", mesh, ierr)

c get root set
      call iMesh_getRootSet(%VAL(mesh), root_set, ierr)
      CHECK("Problems getting root set")

c load the mesh
      call iMesh_load(%VAL(mesh), %VAL(root_set), 
     1  "../MeshFiles/125hex.vtk", "", ierr)

c get all 3d elements
      ents_alloc = 0
      call iMesh_getEntities(%VAL(mesh), %VAL(root_set), 
     1  %VAL(iBase_REGION), %VAL(iMesh_ALL_TOPOLOGIES), 
     1  rpents, ents_alloc, ents_size, ierr)

      ivert_uses = 0

c iterate through them; 
      do i = 0, ents_size-1
c get connectivity
         iverts_alloc = 0
         call iMesh_getEntAdj(%VAL(mesh), %VAL(ents(i)), 
     1        %VAL(iBase_VERTEX), rpverts, iverts_alloc, iverts_size,
     1        ierr)
c sum number of vertex uses

         ivert_uses = ivert_uses + iverts_size

         call free(rpverts)
      end do

c now get adjacencies in one big block
      allverts_alloc = 0
      offsets_alloc = 0
      call iMesh_getEntArrAdj(%VAL(mesh), %VAL(rpents), 
     1     %VAL(ents_size), %VAL(iBase_VERTEX), rpallverts, 
     1     allverts_alloc, allverts_size, ipoffsets, offsets_alloc, 
     1     offsets_size, ierr)

      call free(rpallverts);
      call free(ipoffsets);
      call free(rpents);

c compare results of two calling methods
      if (allverts_size .ne. ivert_uses) then
         write(*,'("Sizes didn''t agree!")')
      else 
         write(*,'("Sizes did agree!")')
      endif

      call iMesh_dtor(%VAL(mesh), ierr)

      end
