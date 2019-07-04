//*******************************************************
// A simple example that shows the creation of a TSTT Mesh factory
// and the corresponding TSTT::Mesh interface object.  The particular
// implementation used is determined by the included libraries in the
// makefile.  All supported TSTT implementations read a triangle,
// vtk file, and this is used to show the simple loading of a mesh
// and the retrieval of the geometric dimension.
//********************************************************

// The user must include TSTTB.h and TSTTM.h include files;
// their location is given by XXXXXXXX
#include <stdio.h>
#include "TSTTB.h"
#include "TSTTM.h"
#include "sidl_Exception.h"

int main(int argv, char** argc) 
{
  sidl_BaseInterface err = NULL;

  /* create a TSTT mesh factory */
  TSTTM_Factory factory = TSTTM_Factory__create();

  /* create a new mesh interface instance from the factory */
  TSTTM_Mesh mesh = TSTTM_Factory_newMesh("GRUMMP_3D",&err);

  /* retrieve the root set.  This argument is often used in
     the interfaces to retrieve all the information associated with a
     mesh interface instance.    */
  void *root_set = TSTTM_Mesh_getRootSet(mesh,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") ) {
    TSTTB_Error tstt_err = TSTTB_Error__cast(err);
    /* do something with TSTTB error */
    SIDL_CLEAR(err);
    exit(0);
  }

  /* Load the vtk mesh from a file.  The only elements that are guaranteed
     to be supported by all implementations are triangles.  Others may be
     supported, but this is implementation dependent. */
  TSTTM_Mesh_load(mesh,root_set,"../../MeshFiles/tire",&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") ) {
    /*    TSTTB_ErrorType__enum err_type;*/
    enum TSTTB_ErrorType__enum err_type;
    char* desc;
    TSTTB_Error tstt_err = TSTTB_Error__cast(err);
    printf("Catching error\n");
    TSTTB_Error_get(tstt_err,&err_type,&desc);
    printf("printing error %s\n",desc);
    SIDL_CLEAR(err);
    exit(0);
  }


  /* Get the geometric dimension of the mesh to show that the we have
     actually loaded a mesh */
  {
    int dim = TSTTM_Mesh_getGeometricDim(mesh,&err);
    printf("dimension = %d\n",dim);
  }

  /* Get the number of vertices and elements in the mesh to show the use of
     SIDL enumerated types in C++.   */
  {
    int num_vtx = TSTTM_Mesh_getNumOfType(mesh,root_set,TSTTM_EntityType_VERTEX,&err);
    printf("number of vertices = %d\n",num_vtx);
  }

  exit(0);
}

