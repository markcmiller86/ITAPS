//*******************************************************
// A simple example that shows the creation of sidl arrays
// and how to retreave information from TSTTM using sidl 
// arrays. Some best practice techniques for using sidl 
// arrays with TSTTM are include.
//********************************************************

// The user must include TSTTB.h and TSTTM.h include files;
#include <stdio.h>
#include "TSTTB.h"
#include "TSTTM.h"
#include "sidl_Exception.h"

int main(int argv, char** argc) 
{
  sidl_BaseInterface err = NULL;
  struct sidl_double__array* COORDS = NULL; /* a sidl double array */
  struct sidl_double__array* COORDS2 = NULL;
  struct sidl_double__array* COORDS3 = NULL; 
  struct sidl_int__array* INDEX = NULL; /* a sidl integer array*/
  struct sidl_int__array* INDEX2 = NULL; 
  struct sidl_int__array* INDEX3 = NULL; 
  enum TSTTM_StorageOrder__enum stor; /* a TSTTM storage order enumerator*/
  char yesorno[100];
  int i,upper,lower, index_size,coords_size;
  double x,y,z;  

   /* 
    * Set up the mesh by calling the factor and getting the root set 
   */
  TSTTM_Factory factory = TSTTM_Factory__create(); /* creates a mesh factory */
  TSTTM_Mesh mesh = TSTTM_Factory_newMesh("Simple",&err);/* creates a mesh from the factory */

  void *root_set = TSTTM_Mesh_getRootSet(mesh,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") )
  {
         TSTTB_Error tstt_err = TSTTB_Error__cast(err);
         SIDL_CLEAR(err);
         exit(0);
  }

  TSTTM_Mesh_load(mesh,root_set,"../../MeshFiles/square10.1",&err);

  if ( SIDL_CATCH(err,"TSTTB.Error") )
  {
          enum TSTTB_ErrorType__enum err_type;
          char* desc;
          TSTTB_Error tstt_err = TSTTB_Error__cast(err);
          printf("Catching error\n");
          TSTTB_Error_get(tstt_err,&err_type,&desc);
          printf("printing error %s\n",desc);
          SIDL_CLEAR(err);
          exit(0);
  }

  /* get the geometric dimension */
  int dim = TSTTM_Mesh_getGeometricDim(mesh,&err);
  printf("dimension = %d\n",dim);
  
  /* get the number of vertices */
  int num_vtx = TSTTM_Mesh_getNumOfType(mesh,root_set,TSTTM_EntityType_VERTEX,&err);
  printf("number of vertices = %d\n",num_vtx);

   /* 
   * An implementation should be able to store data either way
   * in an array, but one way might be faster. The function 
   * getDfltStorage retrieves the storage order that implementation 
   * prefers.
   */
  stor=TSTTM_Mesh_getDfltStorage(mesh,&err);
  if(stor == TSTTM_StorageOrder_INTERLEAVED)
       printf("The implementation prefers the data order to be INTERLEAVED\n"); 
       /*This means the data is stored xyzxyzxyzxyz.. */
  if(stor == TSTTM_StorageOrder_BLOCKED)
       printf("The implementation prefers the data order to be BLOCKED\n"); 
       /*This means the data is stored xxx.. yyy.. zzz..*/
  if(stor == TSTTM_StorageOrder_UNDETERMINED)
       printf("The implementation has no preference for the data order\n");


/********************start example array tests****************************************/
  /*        unallocated  arrays in TSTT 
   * If a TSTTM implementation is given an unallocated
   * array it can allocate the array size itself. Here
   * we are letting the implementation allocate the array
   * it needs, and observing the size of the arrays before and after.
   * */
  printf("\n************performing unallocated getAllVtxCoords test ***********\n");
  
  upper=sidl_int__array_upper(INDEX,0); /*returns the upper index of the INDEX array */
  lower=sidl_int__array_lower(INDEX,0); /*returns the lower index of the INDEX array */
  index_size=(upper-lower)+1;
  printf("size of unallocated sidl_integer__array INDEX before getAllVtxCoords= %i\n",index_size);  
  
  
  upper=sidl_double__array_upper(COORDS,0); /*returns the upper index of the COORDS */
  lower=sidl_double__array_lower(COORDS,0); /*returns the lower index of the COORDS */
  coords_size=(upper-lower)+1;
  printf("size of unallocated sidl_double__array COORDS before getAllVtxCoords= %i\n",coords_size);  
 
  TSTTM_Mesh_getAllVtxCoords(mesh,root_set,&COORDS,&coords_size,&INDEX,&index_size,&stor,&err);
  
  if ( SIDL_CATCH(err,"TSTTB.Error") )
  printf("FAILED!\n");
  else
  printf("PASSED!\n");

  upper=sidl_int__array_upper(INDEX,0); /*returns the upper index of the INDEX array */
  lower=sidl_int__array_lower(INDEX,0); /*returns the lower index of the INDEX array */
  index_size=(upper-lower)+1;
  printf("size of unallocated sidl_integer__array INDEX after getAllVtxCoords= %i\n",index_size);  
  
  upper=sidl_double__array_upper(COORDS,0); /*returns the upper index of the COORDS */
  lower=sidl_double__array_lower(COORDS,0); /*returns the lower index of the COORDS */
  coords_size=(upper-lower)+1;
  printf("size of unallocated sidl_double__array COORDS after getAllVtxCoords= %i\n",coords_size);  

  /*          allocated  arrays in TSTT
   * If it is known that multiple calls will be made
   * to the same array then it should be preallocated
   * to save computing time. Here we are preallocating 
   * the coordinate and index arrays. This is considered 
   * the best practice.
   */
  
  printf("\n************performing allocated getAllVtxCoords test ***********\n");
  COORDS2=sidl_double__array_create1d(num_vtx*dim);
  INDEX2=sidl_int__array_create1d(num_vtx);
  
  upper=sidl_int__array_upper(INDEX2,0); /*returns the upper index of the INDEX array */
  lower=sidl_int__array_lower(INDEX2,0); /*returns the lower index of the INDEX array */
  index_size=(upper-lower)+1;
  printf("size of preallocated sidl_integer__array INDEX before getAllVtxCoords= %i\n",index_size);  
  
  upper=sidl_double__array_upper(COORDS2,0); /*returns the upper index of the COORDS */
  lower=sidl_double__array_lower(COORDS2,0); /*returns the lower index of the COORDS */
  coords_size=(upper-lower)+1;
  printf("size of preallocated sidl_double__array COORDS before getAllVtxCoords= %i\n",coords_size);  
  
  
  TSTTM_Mesh_getAllVtxCoords(mesh,root_set,&COORDS2,&coords_size,&INDEX2,&index_size,&stor,&err);
  
  if ( SIDL_CATCH(err,"TSTTB.Error") )
  printf("FAILED!\n");
  else
  printf("PASSED!\n");
  
  /*          wrongly allocated  arrays in TSTT
   * If a TSTTM implementation is given an allocated
   * array of size smaller then it needs for the desired 
   * task, the implementation will throw an error.
   */

  printf("\n************performing wrongly allocated getAllVtxCoords test***********\n");
  COORDS3=sidl_double__array_create1d(num_vtx*dim-3);
  INDEX3=sidl_int__array_create1d(num_vtx-3);
  
  upper=sidl_int__array_upper(INDEX3,0); /*returns the upper index of the INDEX array */
  lower=sidl_int__array_lower(INDEX3,0); /*returns the lower index of the INDEX array */
  index_size=(upper-lower)+1;
  printf("size of preallocated sidl_integer__array INDEX before getAllVtxCoords= %i\n",index_size);  
  
  upper=sidl_double__array_upper(COORDS3,0); /*returns the upper index of the COORDS */
  lower=sidl_double__array_lower(COORDS3,0); /*returns the lower index of the COORDS */
  coords_size=(upper-lower)+1;
  printf("size of preallocated sidl_double__array COORDS before getAllVtxCoords= %i\n",coords_size);  
  
  
  TSTTM_Mesh_getAllVtxCoords(mesh,root_set,&COORDS3,&coords_size,&INDEX3,&index_size,&stor,&err);
  if ( SIDL_CATCH(err,"TSTTB.Error") )
    printf("FAILED!\n");
  else
    printf("PASSED!\n");
 
/********************end example array tests****************************************/
  
  if(stor == TSTTM_StorageOrder_UNDETERMINED)
  {
     printf("After the call to getAllVtxCoords your implementation should not \n"
            "return an UNDETERMINED storage order, this TSTTM implementation is faulty.\n");
     exit(0);
   }

  printf("\n \nType y to see the entire array:");
  int j;
  scanf("%s",&yesorno);
  if((strncmp(yesorno,"Y",1)==0)||(strncmp(yesorno,"y",1)==0))
  {
  	for(i=0;i<num_vtx;i++)
  	{
		  if(stor == TSTTM_StorageOrder_BLOCKED)
	  	  {
	   	  /*printing the coordinate pare from a BLOCKED array */
		    for (j=0;j<dim;j++) {
		      printf("%f ",sidl_double__array_get1(COORDS,i+j*num_vtx));
		    }
                    printf("\n");
	  	  }
  	  	  if(stor == TSTTM_StorageOrder_INTERLEAVED)
	  	  {
		    for (j=0;j<dim;j++) {
		      printf("%f ",sidl_double__array_get1(COORDS,i*dim+j));
		    }; 
                    printf("\n");
	  	  }
  	}
  }

  exit(0);
}
