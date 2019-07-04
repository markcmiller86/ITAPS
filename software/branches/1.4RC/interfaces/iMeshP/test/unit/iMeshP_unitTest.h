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
std::map <std::string, bool> impl_map;
std::map <std::string, bool> test_map;


  /// counter for failed & passed tests
   int iNFailed = 0, iCurNFailed=0 ;
   int iNPassed = 0;

  // rank of the MPI process, error code
   int mysize, myrank, ierr, size;
   int eEType, eETopo;
   char msg[256];
  // usually we keep the options null and the length is going to be zero
  char* options = NULL;
  int options_len = 0;
  
  // File for recording the output of each process
   FILE *fp;
  
  // Keeping track of the number of parts created in each process. By default, parts per process are 1.
   int INIT_NUM_PARTS = 1;

  // Number of layers in creating ghost entities in the partition.
   int MAX_NUM_LAYERS = 1;

  /// Keeping track of the number of partitions created.
   int iNumPartn = 0;

  // desired number of parts per process. 
   int MAX_NUM_PARTS=2;
  
  // For legal types and topologies for e.g. vertex-point type and topology is valid
   bool a2qLegalTypeAndTopo[5][12];

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
				 "hexahedron",
				 "prism",
				 "pyramid",
				 "septahedron",
				 "all topologies"};

char strEntStatus[][30] = {"internal",
			   "boundary",
			    "ghost"};

int a2iEntCount[5][12];
int a2iPartBdryEntCount[5][12];

static int vSetupTypeAndTopo();
static int vSetupNumTypeAndTopo();
static int vSetupImplStatus();

/** Function declaration for testing part functionality**/
int vCreateDeletePartTest(iMesh_Instance);

int vGetPartHandleIdRankTest(iMesh_Instance);

int vGetPartHandleIdRankArrTest(iMesh_Instance);
int vGetPartTypeTopoTest(iMesh_Instance);
int vGetPartNonResIteratorTest(iMesh_Instance instance);
int vGetPartResIteratorTest(iMesh_Instance instance);

int vGetPartNborTest(iMesh_Instance);

int vGetPartNborArrTest(iMesh_Instance);
  
/** Function declaration for testing part boundary functionality **/
int vGetPartBdryEntsTest(iMesh_Instance);

int vBdryEntsIteratorTest(iMesh_Instance);

int vBdryEntsArrIteratorTest(iMesh_Instance);

/*** Function declaration for testing entity ownership */

int vGetEntOwnerPartTest(iMesh_Instance);

int vGetEntOwnerPartArrTest(iMesh_Instance);

int vIsEntOwnerArrTest(iMesh_Instance);

int vIsEntOwnerTest(iMesh_Instance);

int vGetEntCopiesTest(iMesh_Instance);

int vGetEntCopyOnPartTest(iMesh_Instance);

int vGetEntOwnerCopyTest(iMesh_Instance);

int vGetEntStatusTest(iMesh_Instance);

int vGetEntStatusArrTest(iMesh_Instance);

/** Function declaration for testing migration **/

int vExchangeEntArrTest(iMesh_Instance);

/** Function declaration for testing ghosting **/

int vAddRmvGhostTest(iMesh_Instance);
int vDeleteGhostEntsAllTest(iMesh_Instance);
int vCreateDeleteGhostEntsAllTest(iMesh_Instance);
int vGetGhostInfoTest(iMesh_Instance);

/*** Partition Functionality testing ***/
int vGetPartitionTest(iMesh_Instance);
int vGetPartitionInfoTest(iMesh_Instance instance);

/**** Error handling (Followed from iMesh unit Test) **/
#define _FUNC_ 0
#define _VALID_ 1

#define UT_FREE(a) do {if (a) free(a); a = NULL;} while(0)

#define CHECK_ERR_CRITICAL(err_,str) do {			       \
  if( err_ != iBase_SUCCESS)					\
  {								\
    fprintf(fp, "\n CRITICAL ERROR in %s Function Called: %s \n",__FUNCTION__, str);\
    if (!myrank) cout<< "\n CRITICAL ERROR in "<<__FUNCTION__<<" Function Called: "<<str<<"\n"; \
    PRINT_ERR_TYPE(err_);						\
    iNFailed++; 	  						\
    fprintf(fp, "Cannot continue interface test, exiting...\n");     \
    if (!myrank) cout<<"Cannot continue interface test, exiting...\n"; \
    return 1;								\
  }									\
} while (0)	
 
#define PRINT_ERR_TYPE(err_) do {					\
  switch(err_)						 	\
  {								\
    case iBase_MESH_ALREADY_LOADED: \
            fprintf(fp, "\n iBase error Type: Mesh Already loaded \n");\
	    if (!myrank) cout<<"\n iBase error Type: Mesh Already loaded \n"; \
	    break;							\
    case iBase_FILE_NOT_FOUND: \
            fprintf(fp, "\n iBase error Type: File not found error \n");\
	    if (!myrank) cout<<"\n iBase error Type: File not found error \n"; \
	    break;							\
    case iBase_FILE_WRITE_ERROR: \
            fprintf(fp, "\n iBase error Type: File write error \n ");	\
	    if (!myrank) cout<<"\n iBase error Type: File write error \n "; \
	    break;							\
    case iBase_NIL_ARRAY: \
            fprintf(fp, "\n iBase error Type: Nil Array error \n ");\
            if (!myrank) cout<<"\n iBase error Type: Nil Array error \n"; \
    	    break;							\
    case iBase_BAD_ARRAY_SIZE: \
            fprintf(fp, "\n iBase error Type: Bad Array Size \n");	\
            if (!myrank) cout<<"\n iBase error Type: Bad Array Size \n"; \
   	    break;							\
    case iBase_BAD_ARRAY_DIMENSION: \
            fprintf(fp, "\n iBase error Type: Bad Array Dimension\n");	\
            if (!myrank) cout<<"\n iBase error Type: Bad Array Dimension\n"; \
    	    break;							\
    case iBase_INVALID_ENTITY_HANDLE: \
            fprintf(fp, "\n iBase error Type: Invalid entity handle \n");\
            if (!myrank) cout<<"\n iBase error Type: Invalid entity handle \n"; \
   	    break;							\
    case iBase_INVALID_ENTITY_COUNT: \
            fprintf(fp, "\n iBase error Type: Invalid entity count error\n");\
            if (!myrank) cout<<"\n iBase error Type: Invalid entity count error\n"; \
   	    break;							\
    case iBase_INVALID_ENTITY_TYPE: \
             fprintf(fp, "\n iBase error Type: Invalid entity type error \n");\
             if (!myrank) cout<<"\n iBase error Type: Invalid entity type error \n"; \
    	     break;							\
    case iBase_INVALID_ENTITY_TOPOLOGY: \
             fprintf(fp, "\n iBase error Type: Invalid entity topology error \n ");\
             if (!myrank) cout<<"\n iBase error Type: Invalid entity topology error \n"; \
    	     break;							\
    case iBase_BAD_TYPE_AND_TOPO: \
             fprintf(fp, "\n iBase error Type: Bad type and topology \n ");		\
             if (!myrank) cout<<"\n iBase error Type: Bad type and topology \n"; \
    	     break;							\
    case iBase_ENTITY_CREATION_ERROR: \
             fprintf(fp, "\n iBase error Type: Entity creation error \n");		\
	     if (!myrank) cout<<"\n iBase error Type: Entity creation error \n"; \
   	     break;							\
    case iBase_INVALID_TAG_HANDLE: \
             fprintf(fp, "\n iBase error Type: Invalid Tag Handle error \n ");		\
             if (!myrank) cout<<"\n iBase error Type: Invalid Tag Handle error \n"; \
   	     break;							\
    case iBase_TAG_NOT_FOUND: \
             fprintf(fp, "\n iBase error Type: Tag not found error \n");		\
             if (!myrank) cout<<"\n iBase error Type: Tag not found error \n"; \
    	     break;							\
    case iBase_TAG_ALREADY_EXISTS: \
              fprintf(fp, "\n iBase error Type: Tag already exists error \n");\
              if (!myrank) cout<<"\n iBase error Type: Tag already exists error \n"; \
              break;							\
    case iBase_TAG_IN_USE: \
             fprintf(fp, "\n iBase error Type: Tag in use error \n");\
             if (!myrank) cout<<"\n iBase error Type: Tag in use error \n"; \
     	     break;							\
    case iBase_INVALID_ENTITYSET_HANDLE: \
             fprintf(fp, "\n iBase error Type: Invalid entity set handle \n");\
	     if (!myrank) \
	       cout<<"\n iBase error Type: Invalid entity set handle \n"; \
	     break;							\
    case iBase_INVALID_ITERATOR_HANDLE: \
             fprintf(fp, "\n iBase error Type: Invalid iterator handle \n");\
	     if (!myrank) cout<<"\n iBase error Type: Invalid iterator handle \n"; \
	     break;							\
    case iBase_INVALID_ARGUMENT: \
             fprintf(fp, "\n iBase error Type: Invalid argument error \n");\
	     if (!myrank) cout<<"\n iBase error Type: Invalid argument error \n"; \
	     break;							\
    case iBase_MEMORY_ALLOCATION_FAILED: \
             fprintf(fp, "\n iBase error Type: Memory allocation failed \n");\
	     if (!myrank) \
	       cout<<"\n iBase error Type: Memory allocation failed \n"; \
     	     break;							\
    case iBase_NOT_SUPPORTED: \
             fprintf(fp, "\n iBase error Type: Not supported \n");\
             if (!myrank) cout<<"\n iBase error Type: Not supported \n"; \
	     break;							\
    case iBase_FAILURE: \
             fprintf(fp, "\n iBase error Type: Failure \n");\
	     if (!myrank) cout<<"\n iBase error Type: Failure \n"; \
	     break;							\
    default: fprintf(fp, "\n Unidentified error \n");\
             if (!myrank) cout<<"\n Unidentified error \n"; \
  }								\
} while(0)	
			
#define CHECK_ERR(err_,str) do {					\
  if (err_ != iBase_SUCCESS)  \
  {	\
    fprintf(fp, "\n ERROR: Function name: %s Function called: %s Line: %d \n", __FUNCTION__,str, __LINE__);	\
    if (!myrank) fprintf(stdout, "\n ERROR: Function name: %s Function called: %s Line: %d \n", __FUNCTION__,str, __LINE__); \
    PRINT_ERR_TYPE(err_); \
  }\
} while (0)	
		
#define CHECK_VALIDATION_ERR(str) do {	\
  fprintf(fp, "\n Validation ERROR: Function %s called %s LINE: %d \n", __FUNCTION__, str, __LINE__);	\
  if (!myrank) cout<<"\n Validation ERROR: Function "<< __FUNCTION__<<" called "<<str<<" LINE: "<<__LINE__<<"\n"; \
} while (0)			
			
#define CHECK_ERR_CRITICAL_MAIN(err_,str) do {					\
  if ( err_ != iBase_SUCCESS) {					\
    fprintf(fp, "\n CRITICAL ERROR: Function name: %s Function called: %s LINE: %d", __FUNCTION__, str, __LINE__);	\
    if (!myrank) cout<<"\n CRITICAL ERROR: Function name: "<<__FUNCTION__<<" Function called: "<<str<<" LINE: "<<__LINE__<<"\n";	\
    PRINT_ERR_TYPE(err_);						\
    fprintf(fp, "%s","Cannot continue test, exiting...\n");			 \
    if (!myrank) cout<<"Cannot continue test, exiting...\n"; \
    iNFailed++;							\
    return 1;								\
  }								\
} while (0)

#define CHECK_ERR2(err_,expected_err) do {			\
  if ( err_ != expected_err) {				\
    fprintf(fp, "At line %d in %s should have hit error ", __LINE__, __FILE__);	\
    fprintf(fp, "\n Instead, hit error:  ");				\
    if (!myrank) { \
      cout<<"At line "<<__LINE__<<" in "<<__FILE__<<" should have hit error ";	\
      cout<<"\n Instead, hit error:  ";				\
    } \
  } \
} while (0)

// The following is currently not counted toward the pass total, as a
// successful return doesn't mean anything --- an empty function body
// can do that!
#define writeErrorDescr(aa_) do {			      \
  char descr[200];					      \
  iMesh_getDescription(instance, descr, 200);               \
  fprintf(fp, "%s", descr);				      \
  if (!myrank) cout<<descr; \
} while (0)

#define RUN(f_, a_, p_) do { \
  if (a_) \
  { if (!myrank) fprintf(stdout, "\n\033[0;%dm* %s ... SKIPPED DUE TO DEPENDENCY ERROR\033[0m\n", 32, f_); }\
  else { \
    if (p_ == iNFailed) \
    {  if (!myrank) fprintf(stdout, "\n\033[0;%dm* %s ... PASSED\033[0m\n", 34, f_); } \
    else \
    {  if (!myrank) fprintf(stdout, "\n\033[0;%dm* %s ... FAILED (#failures: %d)\033[0m\n", 31, f_, iNFailed-p_); } \
  } \
} while (0)

/// Borrowed from iMesh unit test. 
#define TEST(a_,w_,s_) do {					\
  if (a_) 							\
  {					\
    iNPassed++;	\
  }  \
  else {								\
    bAllPassed = false;						\
    iNFailed++;	\
    if(w_ == 0)	{							\
      char descr[200];					      \
      iMesh_getDescription(instance, descr, 200);               \
      fprintf(fp, "\n Failed functionality test in line %d: %s (%s)\n",__LINE__, s_, descr);       \
      if (!myrank) fprintf(stdout, "\n Failed functionality test in line %d: %s (%s)\n", __LINE__, s_, descr);\
    } \
    else {	\
      fprintf(fp, "\n Failed validation test in line %d: %s \n",__LINE__, s_); \
      if (!myrank) fprintf(stdout, "\n Failed validation test in line %d: %s \n",__LINE__, s_); \
    } \
  } \
} while (0)


