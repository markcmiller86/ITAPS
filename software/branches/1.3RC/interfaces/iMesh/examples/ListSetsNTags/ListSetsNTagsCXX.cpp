/** ListSetsNTags: list sets & tags from a mesh
 * 
 * This program shows how to read and list sets and tags from a mesh
 *
 * Usage: SetsNTags <mesh_file_name>
 *
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "iMesh.h"

#define ERRORR(a) {if (iBase_SUCCESS != err) {std::cout << a << std::endl; return err;}}

int main( int argc, char *argv[] )
{
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }
  
  // Check command line arg
  char *filename = argv[1];

    // create the Mesh instance
  iMesh_Instance mesh;
  int err;
  iMesh_newMesh(NULL, &mesh, &err, 0);
  ERRORR("Error creating new mesh.\n");
  
  
  iBase_EntitySetHandle root_set;
  iMesh_getRootSet(mesh, &root_set, &err);
  ERRORR("Couldn't get root set.");
  
    // load the mesh
  iMesh_load(mesh, root_set, filename, NULL, &err, strlen(filename), 0);
  ERRORR("Couldn't load mesh.");

    // get all sets
  iBase_EntitySetHandle *sets = NULL;
  int sets_alloc = 0, sets_size;
  iMesh_getEntSets(mesh, root_set, 1, &sets, &sets_alloc, &sets_size, &err);
  ERRORR("Couldn't get all sets.");

    // iterate through them, checking whether they have tags
  iBase_TagHandle *tags = NULL;
  int tags_alloc = 0, tags_size;
  int i, j;
  for (i = 0; i < sets_size; i++) {
      // get connectivity
    iMesh_getAllEntSetTags(mesh, sets[i], &tags, &tags_alloc, &tags_size, &err);
    ERRORR("Failed to get ent set tags.");

    if (0 != tags_size) {
      std::cout << "Set " << sets[i] << "; Tags:" << std::endl;
      
        // list tag names on this set
      for (j = 0; j < tags_size; j++) {
        char tname[128];
        int int_val, tname_size = 128;
        double dbl_val;
        iMesh_getTagName(mesh, tags[j], tname, &err, tname_size);
        tname[tname_size] = '\0';
        std::cout << tname;
        int tag_type;
        iMesh_getTagType(mesh, tags[j], &tag_type, &err);
        ERRORR("Failed to get tag type.");
        if (iBase_INTEGER == tag_type) {
          iMesh_getEntSetIntData(mesh, sets[i], tags[j], &int_val, &err);
          ERRORR("Failed to get int data type.");
          std::cout << "(val = " << int_val << "); ";
        }
        else if (iBase_DOUBLE == tag_type) {
          iMesh_getEntSetDblData(mesh, sets[i], tags[j], &dbl_val, &err);
          std::cout << "(val = " << dbl_val << "); ";
        }
        else std::cout << "; ";
      }
    }
    std::cout << std::endl;
    
    free(tags);
    tags = NULL;
    tags_alloc = 0;
  }
  
  free(sets);

  iMesh_dtor(mesh, &err);
  ERRORR("Failed to destruct interface.");
  
  return 0;
}
