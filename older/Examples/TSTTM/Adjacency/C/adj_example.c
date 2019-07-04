//*******************************************************
// A simple example that shows how to call and use the 
// information through the TSTTM interface. 
//********************************************************

// The user must include TSTTB.h and TSTTM.h include files;
#include <stdio.h>
#include "TSTTB.h"
#include "TSTTM.h"
#include "sidl_Exception.h"

int main(int argv, char** argc) 
{
  sidl_BaseInterface err = NULL;

  /* 
   * Set up the mesh by calling the factory and getting the root set 
   */
  TSTTM_Factory factory = TSTTM_Factory__create(); /* creates a mesh factory */
  TSTTM_Mesh mesh = TSTTM_Factory_newMesh("Simple",&err);/* creates a mesh from the factory */

  void *root_set = TSTTM_Mesh_getRootSet(mesh,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") )  {
    printf("Retrieving the root set failed\n");
    SIDL_CLEAR(err);
    exit(0);
  }

  /* Load data into the root set */
  TSTTM_Mesh_load(mesh,root_set,"../../MeshFiles/square10.1",&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") )  {
    printf("Loading the mesh failed\n");
    SIDL_CLEAR(err);
    exit(0);
  }

  /* Get the geometric dimension */
  int dim = TSTTM_Mesh_getGeometricDim(mesh,&err);
  printf("The geometric dimension = %d\n",dim);
  
  /* Get the number of vertices */
  int num_vtx = TSTTM_Mesh_getNumOfType(mesh,root_set,TSTTM_EntityType_VERTEX,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") ) {
    printf("FAILED to get number of vertices!\n");
  }
  printf("The number of vertices = %d\n",num_vtx);

  int num_faces = TSTTM_Mesh_getNumOfType(mesh,root_set,TSTTM_EntityType_FACE,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") ) {
    printf("FAILED to get number of faces!\n");
  }
  printf("The number of faces = %d\n",num_faces);

  struct sidl_double__array* coords = NULL; 
  struct sidl_int__array* index = NULL; /* a sidl integer array*/
  struct sidl_int__array* adj_index = NULL; /* a sidl integer array*/
  struct sidl_int__array* offset = NULL; /* a sidl integer array*/
  struct TSTTM_EntityTopology__array* entity_topologies = NULL;
  int coords_size, index_size, adj_index_size;
  int offset_size, entity_topologies_size;

  /* Let the implementation use its prefered storage order */
  enum TSTTM_StorageOrder__enum stor = TSTTM_StorageOrder_UNDETERMINED;
 
  /* Get the vertex coordinates */
  TSTTM_Mesh_getAllVtxCoords(mesh,root_set,&coords,&coords_size,&index,&index_size,&stor,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") ) {
    printf("FAILED to get vertex coords!\n");
  }

  /* get the adjacency table */
  struct TSTTM_AdjacencyInfo__array* adj_info = NULL;
  int adj_info_size;
  TSTTM_Mesh_getAdjTable(mesh,&adj_info,&adj_info_size,&err);

  /* printf the adjacency table */
  int i, j;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      printf(" %d ",TSTTM_AdjacencyInfo__array_get1(adj_info,i*4+j));
    }
    printf("\n");
  }


  /* Get the vertex coordinate indices */
  TSTTM_Mesh_getVtxCoordIndex(mesh,root_set,TSTTM_EntityType_VERTEX,
                              TSTTM_EntityTopology_POINT,
                              TSTTM_EntityType_FACE,
                              &offset, &offset_size,
                              &adj_index, &adj_index_size,
                              &entity_topologies, &entity_topologies_size,
                              &err);
  if ( SIDL_CATCH(err,"TSTTB.Error") ) {
    printf("FAILED to get coord index information!\n");
  }

  char yesorno[100];
  printf("\n \nType y to see the adjacency info:");
  scanf("%s",&yesorno);
  if((strncmp(yesorno,"Y",1)==0)||(strncmp(yesorno,"y",1)==0))
  {
    for (i=0;i<num_faces;i++) {
      printf("Elem %d:  ");
      for (j=sidl_int__array_get1(offset,i);j<sidl_int__array_get1(offset,i+1);j++) {
        printf(" %d ",sidl_int__array_get1(adj_index,j));
      }
      printf("\n");
    }
  }

  /*
  printf("\n \nType y to see the entire array:");
  scanf("%s",&yesorno);
  if((strncmp(yesorno,"Y",1)==0)||(strncmp(yesorno,"y",1)==0))
  {
  	for(i=0;i<num_vtx;i++)
  	{
		  if(stor == TSTTM_StorageOrder_BLOCKED)
	  	  {
		    for (j=0;j<dim;j++) {
		      printf("%f ",sidl_double__array_get1(coords,i+j*num_vtx));
		    }
                    printf("\n");
	  	  }
  	  	  if(stor == TSTTM_StorageOrder_INTERLEAVED)
	  	  {
		    for (j=0;j<dim;j++) {
		      printf("%f ",sidl_double__array_get1(coords,i*dim+j));
		    }; 
                    printf("\n");
	  	  }
  	}
  }
  */
  exit(0);
}
