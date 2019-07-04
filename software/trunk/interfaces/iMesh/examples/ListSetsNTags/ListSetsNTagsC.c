/** ListSetsNTags: list sets & tags from a mesh
 * 
 * This program shows how to read and list sets and tags from a mesh
 *
 * Usage: SetsNTags <mesh_file_name>
 *
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "iMesh.h"

#define ERRORR(a) {if (iBase_SUCCESS != err) {printf("%s", a); return err;}}

int main( int argc, char *argv[] )
{
  char *filename;
  iMesh_Instance mesh;
  int err;
  iBase_EntitySetHandle root_set;
  iBase_EntitySetHandle *sets;
  int sets_alloc, sets_size;
  iBase_TagHandle *tags = NULL;
  int tags_alloc = 0, tags_size;
  int i, j;
  char tname[128];
  int int_val, tname_size;
  double dbl_val;
  int tag_type;

/****
  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
 ****/
    /* Check command line arg */
 /****  filename = argv[1];  ****/

    /* set input file name */
  filename="../MeshFiles/125hex.vtk";

    /* status info */
  printf("Running ListSetsNTags with %s\n", filename);

    /* create the Mesh instance */
  iMesh_newMesh(NULL, &mesh, &err, 0);
  ERRORR("Error creating new mesh.\n");
  
  
  iMesh_getRootSet(mesh, &root_set, &err);
  ERRORR("Couldn't get root set.");
  
    /* load the mesh */
  iMesh_load(mesh, root_set, filename, NULL, &err, strlen(filename), 0);
  ERRORR("Couldn't load mesh.");

    /* get all sets */
  sets = NULL;
  sets_alloc = 0;
  iMesh_getEntSets(mesh, root_set, 1, &sets, &sets_alloc, &sets_size, &err);
  ERRORR("Couldn't get all sets.");

    /* iterate through them, checking whether they have tags */
  tags = NULL;
  tags_alloc = 0;
  for (i = 0; i < sets_size; i++) {
      /* get connectivity */
    iMesh_getAllEntSetTags(mesh, sets[i], &tags, &tags_alloc, &tags_size, &err);
    ERRORR("Failed to get ent set tags.");

    if (0 != tags_size) {
      printf("Set 0x%lx: Tags:\n", (long unsigned int) sets[i]);
      
        /* list tag names on this set */
      for (j = 0; j < tags_size; j++) {
        tname_size = 128;
        iMesh_getTagName(mesh, tags[j], tname, &err, tname_size);
        tname[tname_size-1] = '\0';
        printf("%s", tname);
        iMesh_getTagType(mesh, tags[j], &tag_type, &err);
        ERRORR("Failed to get tag type.");
        if (iBase_INTEGER == tag_type) {
          iMesh_getEntSetIntData(mesh, sets[i], tags[j], &int_val, &err);
          ERRORR("Failed to get int data type.");
          printf("(val = %d); ", int_val);
        }
        else if (iBase_DOUBLE == tag_type) {
          iMesh_getEntSetDblData(mesh, sets[i], tags[j], &dbl_val, &err);
          printf("(val = %f); ", dbl_val);
        }
        else printf("; ");
      }
    }
    printf("\n");
    free(tags);
    tags = NULL;
    tags_alloc = 0;
  }
  
  free(sets);

  iMesh_dtor(mesh, &err);
  ERRORR("Failed to destruct interface.");

    /* status info */
  printf("Done running ListSetsNTags\n");
  
  return 0;
}
