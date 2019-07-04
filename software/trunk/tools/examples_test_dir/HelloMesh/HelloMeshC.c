/*
 * A simple example that shows the creation of an
 * iMesh interface object.  The particular
 * implementation used is determined by the included libraries in the
 * makefile.  All supported ITAPS implementations read a triangle,
 * vtk file, and this is used to show the simple loading of a mesh
 * and the retrieval of the geometric dimension.
 */


#include "iMesh.h"
#include <stdio.h>
#include <string.h>

#define CHECK(a) if (err != 0) printf(a)

int main(int argc, char **argv) 
{
  iMesh_Instance imesh;
  iBase_EntitySetHandle root_set;
  int err, dim, num_ent;
  const char *fname = "../MeshFiles/125hex.vtk";

/* create the imesh instance and get the root set */
  iMesh_newMesh("", &imesh, &err, 0);
  CHECK("Failed to create imesh instance.\n");

  iMesh_getRootSet(imesh, &root_set, &err);
  CHECK("Failed to get root set.\n");

/* Load the vtk mesh from a file.  The only elements that are guaranteed
     to be supported by all implementations are triangles.  Others may be
     supported, but this is implementation dependent. */
  if (argc > 1) fname = argv[1];
  iMesh_load(imesh, root_set, fname, "", &err, strlen(fname), 0);
  CHECK("Failed to load mesh file.\n");

/* Get the geometric dimension of the mesh to show that the we have
   actually loaded a mesh */
  iMesh_getGeometricDimension(imesh, &dim, &err);
  CHECK("Failed in getGeometricDimension\n");
  
  printf("dimension = %d\n", dim);

/* Get the number of vertices and elements in the mesh to show the use of
   enumerated types */
  for (dim = iBase_VERTEX; dim <= iBase_REGION; dim++) {
    iMesh_getNumOfType(imesh, root_set, dim, &num_ent, &err);
    CHECK("Failed in getNumOfType.\n");
    printf("number of %d-D entities = %d\n", dim, num_ent);
  }

/* Call destructor on interface */
  iMesh_dtor(imesh, &err);
  CHECK("Failed to destruct imesh instance.\n");

  return 0;
}

