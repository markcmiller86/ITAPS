#include <iostream>
#include <set>
#include "TSTTM.hh"
#include "TSTTB.hh"

typedef void* EntityHandle;
typedef void* EntitySetHandle;
typedef void* TagHandle;

int main( int argc, char *argv[] )
{
  std::string filename = argv[1];
  TSTTM::Mesh mesh = TSTTM::Factory::newMesh("");
  EntitySetHandle rootSet = mesh.getRootSet();
  mesh.load(rootSet, filename);

  sidl::array<EntitySetHandle> sets;
  int sets_size;
  TSTTB::EntSet mesh_eset = mesh;
  mesh_eset.getEntSets(rootSet, 1, sets, sets_size);

  TSTTB::SetTag mesh_stag = mesh;
  std::set<TagHandle> tag_handles;
  for (int i = 0; i < sets_size; i++) {
      // get connectivity
    sidl::array<TagHandle> tags;
    int tags_size;

    mesh_stag.getAllEntSetTags(sets[i], tags, tags_size);
    
    for (int j = 0; j < tags_size; j++) {
      std::string tname = mesh_stag.getTagName(tags[j]);
      tag_handles.insert(tags[j]);
    }
  }

  for (std::set<TagHandle>::iterator sit = tag_handles.begin(); 
       sit != tag_handles.end(); sit++) {
    std::string tag_name = mesh_stag.getTagName(*sit);
    int tag_size = mesh_stag.getTagSizeBytes(*sit);
    std::cout << "Tag name = '" << tag_name 
              << "', size = " << tag_size << " bytes." << std::endl;
  }
  
  return true;
}
