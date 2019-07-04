//*******************************************************
// A simple example that shows the creation of a TSTT Mesh factory
// and the corresponding TSTT::Mesh interface object.  The particular
// implementation used is determined by the included libraries in the
// makefile.  All supported TSTT implementations read a triangle,
// vtk file, and this is used to show the simple loading of a mesh
// and the retrieval of the geometric dimension.
//********************************************************

// The user must include TSTTB.hh and TSTTM.hh include files;
// their location is given by XXXXXXXX
#include <iostream>
//#include "TSTTMeshFactory_MeshFactory.hh"
#include "TSTTB.hh"
#include "TSTTM.hh"

using std::cout;
using std::endl;

int main(int argv, char** argc) 
{

  // create a TSTT mesh factory
  TSTTM::Factory factory = TSTTM::Factory::_create();

  // create a new mesh interface instance from the factory
  TSTTM::Mesh mesh;
  try {
    mesh = factory.newMesh("GRUMMP_3D");
  } catch (TSTTB::Error err) {
    err.echo("Failure in getting the mesh instance from the factory");
    return(0);
  }

  // retrieve the root set.  This argument is often used in
  // the interfaces to retrieve all the information associated with a
  // mesh interface instance.   
  void *root_set;
  try{
    root_set = mesh.getRootSet();
  } catch (TSTTB::Error err) {
    err.echo("Failure to get the root set");
    return(0);
  }

  // Load the vtk mesh from a file.  The only elements that are guaranteed
  // to be supported by all implementations are triangles.  Others may be
  // supported, but this is implementation dependent. 
  try {
    mesh.load(root_set,"../../MeshFiles/tire");
  } catch (TSTTB::Error err) {
    err.echo("Failure loading the mesh");
    return(0);
  }

  // Get the geometric dimension of the mesh to show that the we have
  // actually loaded a mesh 
  try {
    int dim = mesh.getGeometricDim();
    cout << "Geometric Dimension:  " << dim << endl;
  } catch (TSTTB::Error err) {
    err.echo("Failure to get the geometric dimension");
    return(0);
  }

  // Get the number of vertices and elements in the mesh to show the use of
  // SIDL enumerated types in C++.  
  try {
    int num_vtx = mesh.getNumOfType(root_set, TSTTM::EntityType_VERTEX);
    cout << "Number of Vertices:  " << num_vtx << endl;
  } catch (TSTTB::Error err) {
    err.echo("Failure to get the number of vertices");
    return(0);
  }

  exit(0);
}
