/*
Header file for iMeshP Compliance test.
Include function definitions, macros for debugging & global variables.
*/


#include "iBase.h"
#include "iMesh.h"
#include "iMeshP.h"

//-------------------------------------------------------

//@@ Standard include files

#include <set>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>
#include <map>
#include <cstdio>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::set;
using std::multimap;

bool bAllPassed = true, bErrTests = false;

  /// counter for failed & passed tests
   int iNFailed = 0;
   int iNPassed = 0;

  // rank of the MPI process, error code
   int me, ierr, size;
   
  // usually we keep the options null and the length is going to be zero
  char* options = NULL;
  int options_len = 0;

  // File for recording the output of each process
   FILE *fp;
  
  // Keeping track of the number of parts created in each process. By default, parts per process are 1.
   int iNumPrts = 1;

  // Number of layers in creating ghost entities in the partition.
   int iNumLayers = 1;

  /// Keeping track of the number of partitions created.
   int iNumPrtn = 0;

  // desired number of parts per process. 
   int iPrtsPerProcess = 4;
  
  // For legal types and topologies for e.g. vertex-point type and topology is valid
   bool a2qLegalTypeAndTopo[5][12];

   /// Number of layers for ghosting
   int iUpperNum = 1;
 
   // Names of the types and topologies
   char astrTypes[][30] = {"vertex",
			    "edge",
			    "face",
			    "region",
			    "all types"};
char astrTopologies[][30] = {"point",
				 "line segment",
				 "polygon",
				 "triangle",
				 "quadrilateral",
				 "polyhedron",
				 "tetrahedron",
				 "pyramid",
				 "prism",
				 "hexahedron",
				 "septahedron",
				 "all topologies"};

char strEntStatus[][30] = {"internal",
			   "boundary",
			    "ghost"};

/** Function declaration for testing part functionality**/
void vCreateDeletePartTest(iMesh_Instance);

void vGetPrtIdFromPrtHandleArrTest(iMesh_Instance);

void vGetPrtIdFromPrtHandleTest(iMesh_Instance);

void vGetPrtHandleFromPrtIdTest(iMesh_Instance);

void vGetPrtHandleArrFromPrtIdsTest(iMesh_Instance);

static void vSetupTypeAndTopo();

void vBdryEntsIteratorTest(iMesh_Instance);

/** Function declaration for testing part boundary functionality **/
void vGetPrtBdryEntsTest(iMesh_Instance);

void vGetPrtNborsTest(iMesh_Instance);

/// Now test getting part neighbors
void vGetPrtNborsArrTest(iMesh_Instance);
  
void vGetNumPrtNborsTest(iMesh_Instance);   
  
void vGetNumPrtNborsArrTest(iMesh_Instance);  

void vBdryEntsArrIteratorTest(iMesh_Instance);

/*** Function declaration for testing entity ownership */

void vGetEntOwnerPrtTest(iMesh_Instance);

void vGetEntOwnerPrtArrTest(iMesh_Instance);

void vIsEntOwnerArrTest(iMesh_Instance);

void vIsEntOwnerTest(iMesh_Instance);

void vGetNumCopies(iMesh_Instance);

void vGetCopyParts(iMesh_Instance);

void vGetCopyOnPartTest(iMesh_Instance);

void vGetOwnerCopyTest(iMesh_Instance);

void vGetEntStatus(iMesh_Instance);

void vGetEntCopies(iMesh_Instance);

void vGetEntStatusArr(iMesh_Instance);

/*** Function declaration for testing off-processor modification functionality ***/
void vAddRmvGhostTest(iMesh_Instance);

/*** Function declaration for testing ghosting functionality ***/
void vDeleteGhostEntsAll(iMesh_Instance);

void vCreateGhostEntsAll(iMesh_Instance);

void vGetGhostInfo(iMesh_Instance);

/*** Partition Functionality testing ***/
void vCreatePartitionTest(iMesh_Instance);

/**** Error handling (Followed from iMesh unit Test) **/

/**** Error handling (Followed from iMesh unit Test) **/

#define CHECK_ERR_CRITICAL(err_,str) do {			       \
	if( err_ != iBase_SUCCESS)					\
	{								\
	fprintf(fp, "\n CRITICAL ERROR in %s Function Called: %s \n",__FUNCTION__, str);\
	PRINT_ERR_TYPE(err_);						\
	iNFailed++; 	  						\
       fprintf(fp, "Cannot continue interface test, exiting...\n");     \
      return;								\
    }									\
  } while (0)							
 
  #define PRINT_ERR_TYPE(err_) do {					\
    switch(err_)						 	\
	{								\
	case 1:								\
		fprintf(fp, "\n iBase error Type: Mesh Already loaded \n");\
		break;							\
	case 2:								\
		fprintf(fp, "\n iBase error Type: No Mesh Data \n");	\
		break;							\
	case 3: 							\
		fprintf(fp, "\n iBase error Type: File not found error \n");\
		break;							\
	case 4:								\
	       fprintf(fp, "\n iBase error Type: File write error \n ");	\
		break;							\
	case 5: 							\
		fprintf(fp, "\n iBase error Type: Nil Array error \n ");\
		break;							\
	case 6: 							\
		fprintf(fp, "\n iBase error Type: Bad Array Size \n");	\
		break;							\
	case 7:								\
		fprintf(fp, "\n iBase error Type: Bad Array Dimension\n");	\
		break;							\
	case 8:								\
		fprintf(fp, "\n iBase error Type: Invalid entity handle \n");\
		break;							\
	case 9:								\
		fprintf(fp, "\n iBase error Type: Invalid entity count error\n");\
		break;							\
	case 10:							\
		fprintf(fp, "\n iBase error Type: Invalid entity type error \n");\
		break;							\
	case 11:							\
		fprintf(fp, "\n iBase error Type: Invalid entity topology error \n ");\
		break;							\
	case 12:							\
		fprintf(fp, "\n iBase error Type: Bad type and topology \n ");		\
		break;							\
	case 13:							\
		fprintf(fp, "\n iBase error Type: Entity creation error \n");		\
		break;							\
	case 14: 							\
		fprintf(fp, "\n iBase error Type: Invalid Tag Handle error \n ");		\
		break;							\
	case 15:							\
		fprintf(fp, "\n iBase error Type: Tag not found error \n");		\
		break;							\
	case 16:							\
		fprintf(fp, "\n iBase error Type: Tag already exists error \n");\
		break;							\
	case 17:							\
		fprintf(fp, "\n iBase error Type: Tag in use error \n");\
		break;							\
	case 18:							\
		fprintf(fp, "\n iBase error Type: Invalid entity set handle \n");\
		break;							\
	case 19:							\
		fprintf(fp, "\n iBase error Type: Invalid iterator handle \n");\
		break;							\
	case 20:							\
		fprintf(fp, "\n iBase error Type: Invalid argument error \n");\
		break;							\
	case 21:							\
		fprintf(fp, "\n iBase error Type: Memory allocation failed \n");\
		break;							\
	case 22:							\
		fprintf(fp, "\n iBase error Type: Not supported \n");\
		break;							\
	case 23:							\
		fprintf(fp, "\n iBase error Type: Failure \n");\
		break;							\
	default:							\
		fprintf(fp, "\n Unidentified error \n");\
	}								\
} while(0)	
			
#define CHECK_ERR(err_,str) do {					\
    if (err_ != iBase_SUCCESS) {					\
         fprintf(fp, "\n ERROR: Function name: %s Function called: %s Line: %d \n", __FUNCTION__,str, __LINE__);	\
	PRINT_ERR_TYPE(err_); \
	writeErrorDescr(err_);\
       }\
 } while (0)	
		
#define CHECK_VALIDATION_ERR(str) do {	\
         fprintf(fp, "\n Validation ERROR: Function name: %s Function called: %s LINE: %d \n", __FUNCTION__, str, __LINE__);	\
 } while (0)			
			
#define CHECK_ERR_CRITICAL_MAIN(err_,str) do {					\
    if ( err_ != iBase_SUCCESS) {					\
      fprintf(fp, "\n CRITICAL ERROR: Function name: %s Function called: %s LINE: %d", __FUNCTION__, str, __LINE__);	\
      PRINT_ERR_TYPE(err_);						\
      fprintf(fp, "%s","Cannot continue test, exiting...\n");			 \
      iNFailed++;							\
      return 1;								\
    }								\
  } while (0)

#define CHECK_ERR2(err_,expected_err) do {			\
    if ( err_ != expected_err) {				\
      fprintf(fp, "At line %d in %s should have hit error ", __LINE__, __FILE__);	\
      fprintf(fp, "\n Instead, hit error:  ");				\
     }								\
  } while (0)

// The following is currently not counted toward the pass total, as a
// successful return doesn't mean anything --- an empty function body
// can do that!
#define writeErrorDescr(aa_) do {			      \
    char descr[200];					      \
    int int_err_ = aa_;					      \
    iMesh_getDescription(instance, descr, 200);               \
    fprintf(fp, "%s", descr);				      \
  } while (0)

/// Borrowed from iMesh unit test. 

#define TEST(a_,w_) do {					\
    if (a_) 							\
   {					\
 	if(w_ == 0)	\
	fprintf(fp, "\n Passed basic functionality test. \n");\
	else		\
	fprintf(fp, "\n Passed validation test \n");				\
    iNPassed++;							\
     }								\
    else {								\
      bAllPassed = false;						\
      iNFailed++;	\
      if(w_ == 0)								\
      fprintf(fp, "\n Failure basic functionality tests at %d \n",__LINE__);       \
	else	\
	fprintf(fp, "\n Failure validation tests at %d \n", __LINE__);\
    }									\
  } while (0)



