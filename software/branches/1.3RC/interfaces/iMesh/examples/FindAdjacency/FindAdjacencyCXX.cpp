/** FindAdjacency: Interacting with TSTTM mesh
 * 
 * This program shows how to get more information about a mesh, by
 * getting connectivity two different ways (as connectivity and as
 * adjacent 0-dimensional entities).
 *
 * Usage: FindAdjacency <mesh_file_name>
 *
 */
#include <iostream>
#include <stdlib.h>
#include "iMesh.h"

#define CHECK(a) if (iBase_SUCCESS != ierr) std::cout << a << std::endl

int main( int argc, char *argv[] )
{
  int i, ierr;
  iMesh_Instance mesh;
  iBase_EntityHandle *ents, *verts, *allverts;
  int ents_alloc = 0, ents_size;
  int verts_alloc = 0, verts_size;
  int allverts_alloc = 0, allverts_size;
  int *offsets, offsets_alloc = 0, offsets_size;
  int vert_uses = 0;
  iBase_EntitySetHandle root_set;

    // create the Mesh instance
  iMesh_newMesh("", &mesh, &ierr, 0);
  CHECK("Problems instantiating interface.");
  
    // get root set
  iMesh_getRootSet(mesh, &root_set, &ierr);
  CHECK("Problems getting root set");

    // load the mesh
  iMesh_load(mesh, root_set, "../MeshFiles/125hex.vtk", "", &ierr, 26, 0);
  CHECK("Load failed");

    // get all 3d elements
  iMesh_getEntities(mesh, root_set, iBase_REGION, iMesh_ALL_TOPOLOGIES,
                    &ents, &ents_alloc, &ents_size, &ierr);
  CHECK("Couldn't get entities");

    // iterate through them; 
    // first have to get an Entity interface instance
  for (i = 0; i < ents_size; i++) {
      // get connectivity
    verts_alloc = 0;
    iMesh_getEntAdj(mesh, ents[i], iBase_VERTEX,
                    &verts, &verts_alloc, &verts_size,
                    &ierr);
    CHECK("Failure in getEntAdj");

      // sum number of vertex uses
    vert_uses += verts_size;
    free(verts);
  }

    // now get adjacencies in one big block
  iMesh_getEntArrAdj(mesh, ents, ents_size, iBase_VERTEX,
                     &allverts, &allverts_alloc, &allverts_size,
                     &offsets, &offsets_alloc, &offsets_size,
                     &ierr);
  CHECK("Failure in getEntArrAdj");
  
    // compare results of two calling methods
  if (allverts_size != vert_uses)
    std::cout << "Sizes didn't agree" << std::endl;
  else 
    std::cout << "Sizes did agree" << std::endl;
  
  iMesh_dtor(mesh, &ierr);
  CHECK("Failed to destroy interface");
  
  return 0;
}
