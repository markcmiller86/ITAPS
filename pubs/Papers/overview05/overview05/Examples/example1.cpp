#include <iostream>
#include "TSTTM.hh"

typedef void* EntityHandle;
typedef void* EntitySetHandle;
typedef void* IteratorHandle;

int main( int argc, char *argv[] )
{
  std::string filename = argv[1];
  TSTTM::Mesh mesh = TSTTM::Factory::newMesh("");
  EntitySetHandle rootSet = mesh.getRootSet();
  mesh.load(rootSet, filename);

  int vert_uses = 0;
  TSTTM::Entity mesh_ent = mesh;
  IteratorHandle iter;
  mesh_ent.initEntIter(rootSet, TSTTM::EntityType_REGION, 
		       TSTTM::EntityTopology_ALL_TOPOLOGIES, iter);
  EntityHandle ent;
  bool moreData = mesh_ent.getNextEntIter(iter, ent);
  while (moreData) {
    sidl::array<EntityHandle> verts;
    int verts_size;
    mesh_ent.getEntAdj(ent, TSTTM::EntityType_VERTEX,
                       verts, verts_size);
    vert_uses += verts_size;
    moreData = mesh_ent.getNextEntIter(iter, ent);
  }

  // now get adjacencies in one big block
  sidl::array<EntityHandle> ents;
  int ents_size;
  mesh.getEntities(0, TSTTM::EntityType_REGION, 
                   TSTTM::EntityTopology_ALL_TOPOLOGIES,
                   ents, ents_size);
  sidl::array<EntityHandle> allverts;
  sidl::array<int> offsets;
  int allverts_size, offsets_size;
  TSTTM::Arr mesh_arr = mesh;
  mesh_arr.getEntArrAdj(ents, ents_size, TSTTM::EntityType_VERTEX,
                        allverts, allverts_size,
                        offsets, offsets_size);
  
  std::cout << "Sizes did ";
  if (allverts_size != vert_uses) std::cout << "not";
  std::cout << " agree!" << std::endl;
  
  return true;
}
