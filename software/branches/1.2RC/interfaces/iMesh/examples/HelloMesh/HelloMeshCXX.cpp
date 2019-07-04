/*
 * A simple example that shows the creation of an
 * iMesh interface object.  The particular
 * implementation used is determined by the included libraries in the
 * makefile.  All supported ITAPS implementations read a triangle,
 * vtk file, and this is used to show the simple loading of a mesh
 * and the retrieval of the geometric dimension.
 */


#include "iMesh.h"
#include <iostream>

#define CHECK(a) if (err != 0) std::cout << a << std::endl

int main(int argc, char **argv) 
{
  iMesh_Instance imesh;
  iBase_EntitySetHandle root_set;
  int err, dim, num_ent;

// create the imesh instance and get the root set
  iMesh_newMesh("", &imesh, &err, 0);
  CHECK("Failed to create imesh instance.");

  iMesh_getRootSet(imesh, &root_set, &err);
  CHECK("Failed to get root set.");

// Load the vtk mesh from a file.  The only elements that are guaranteed
//     to be supported by all implementations are triangles.  Others may be
//     supported, but this is implementation dependent.
  const char *fname = "125hex.vtk";
  if (argc > 1) fname = argv[1];
  iMesh_load(imesh, root_set, fname, "", &err, 26, 0);
  CHECK("Failed to load mesh file.");

// Get the geometric dimension of the mesh to show that the we have
// actually loaded a mesh
  iMesh_getGeometricDimension(imesh, &dim, &err);
  CHECK("Failed in getGeometricDimension");
  
  std::cout << "dimension = " << dim << std::endl;

// Get the number of vertices and elements in the mesh to show the use of
// enumerated types
  for (dim = iBase_VERTEX; dim <= iBase_REGION; dim++) {
    iMesh_getNumOfType(imesh, root_set, dim, &num_ent, &err);
    CHECK("Failed in getNumOfType.\n");
    std::cout << "number of " << dim << "-D entities = " << num_ent 
              << std::endl;
  }

// Call destructor on interface
  iMesh_dtor(imesh, &err);
  CHECK("Failed to destruct imesh instance.");

  return 0;
}

