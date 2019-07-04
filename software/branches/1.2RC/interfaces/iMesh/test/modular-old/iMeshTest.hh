#ifndef _iMeshTest_hh_
#define _iMeshTest_hh_

#include <assert.h>
#include <stddef.h>
#include <iostream>

using std::cout;
using std::endl;

#include "iMesh.h"
#include "TestData.hh"

#warning Version __STDC_VERSION__

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  warning FUNCTION definition of __ func __ macro
#  define __func__ __FUNCTION__
# else
#  warning UNKNOWN definition of __ func __ macro
#  define __func__ "<unknown>"
# endif
#endif

// A couple of convenience macros to make error handling easier to code.  
// These would be member functions, except that having them as macros makes it easier to abort
// in mid-test.
#define BEGIN_FUNC(test) do { \
	assert(testInProgress == NUM_FUNCTIONS); \
 	testInProgress = test; \
 	testStatus[testInProgress] = NOT_TESTED; \
} while (0)

#define END_FUNC do { \
 	assert(testInProgress != NUM_FUNCTIONS); \
	testStatus[testInProgress] = (isThisFuncOK) ? PASSED : FAILED; \
	testInProgress = NUM_FUNCTIONS; \
	return isThisFuncOK; \
} while (0)

#define TEST(a) do { \
	if (a) numPassed++; \
	else { \
		numFailed++; \
		isThisFuncOK = false; \
		cout << __FILE__ ":" << __LINE__ << ": Test failed: " << #a << "(function " << \
				 __func__ << ")" << endl; \
		} \
} while (0)

#define HANDLE_ERROR(a) do { \
 	if (a != iBase_SUCCESS) { \
 		numFailed++; \
 		isThisFuncOK = false; \
		cout << __FILE__ ":" << __LINE__ << " Hit error code " << a  << endl; \
 	} \
 	END_FUNC; \
} while (0)

class iMeshTest {
public:
  enum status
    { NOT_TESTED = -1, FAILED = 0, PASSED = 1};
  enum functionEnum
    {
      GET_DESCRIPTION,
      NEW_MESH,
      DTOR,
      // iMesh core
      LOAD, SAVE, GET_ROOT_SET, GET_GEOMETRIC_DIMENSION, GET_DFLT_STORAGE,
      GET_ADJ_TABLE, ARE_EH_VALID, GET_NUM_OF_TYPE, GET_NUM_OF_TOPO,
      GET_ALL_VTX_COORDS, GET_VTX_COORD_INDEX, GET_ENTITIES, GET_VTX_ARR_COORDS, GET_ADJ_ENTITIES,
      // Single entity
      INIT_ENT_ITER, GET_NEXT_ENT_ITER, RESET_ENT_ITER, END_ENT_ITER,
      GET_ENT_TOPO, GET_ENT_TYPE, GET_VTX_COORD, GET_ENT_ADJ,
      // Entity array
      INIT_ENT_ARR_ITER, GET_NEXT_ENT_ARR_ITER, RESET_ENT_ARR_ITER, END_ENT_ARR_ITER,
      GET_ENT_ARR_TOPO, GET_ENT_ARR_TYPE, GET_ENT_ARR_ADJ,
      // Single entity modify
      SET_VTX_COORDS, CREATE_VTX, CREATE_ENT, DELETE_ENT,
      SET_VTX_ARR_COORDS, CREATE_VTX_ARR, CREATE_ENT_ARR, DELETE_ENT_ARR,
      // Basic tags
      CREATE_TAG, DESTROY_TAG, GET_TAG_NAME, GET_TAG_SIZE_VALUES, GET_TAG_SIZE_BYTES,
      GET_TAG_HANDLE, GET_TAGTYPE,
      // Single entity tagging
      GET_DATA, GET_INT_DATA, GET_DBL_DATA, GET_EH_DATA,
      SET_DATA, SET_INT_DATA, SET_DBL_DATA, SET_EH_DATA, GET_ALL_TAGS, RMV_TAG,
      // Entity array tagging
      GET_ARR_DATA, GET_INT_ARR_DATA, GET_DBL_ARR_DATA, GET_EH_ARR_DATA,
      SET_ARR_DATA, SET_INT_ARR_DATA, SET_DBL_ARR_DATA, SET_EH_ARR_DATA,
      RMV_ARR_TAG,
      // Entity set tagging
      SET_ENTSET_DATA, SET_ENTSET_INT_DATA, SET_ENTSET_DBL_DATA, SET_ENTSET_EH_DATA,
      GET_ENTSET_DATA, GET_ENTSET_INT_DATA, GET_ENTSET_DBL_DATA, GET_ENTSET_EH_DATA,
      GET_ALL_ENTSET_TAGS, RMV_ENTSET_TAG,
      // Entity set basics
      CREATE_ENTSET, DESTROY_ENTSET, IS_LIST, GET_NUM_ENTSETS, GET_ENTSETS,
      ADD_ENT_TO_SET, RMV_ENT_FROM_SET, ADD_ENT_ARR_TO_SET, RMV_ENT_ARR_FROM_SET,
      ADD_ENTSET, RMV_ENTSET, IS_ENT_CONTAINED, IS_ENTSET_CONTAINED,
      // Heirarchical relationships between sets
      ADD_PRNT_CHLD, RMV_PRNT_CHLD, IS_CHILD_OF, GET_NUM_CHLDN, GET_NUM_PRNT, GET_CHLDN, GET_PRNTS,
      // Set Boolean ops
      SUBTRACT, INTERSECT, UNITE,
      // This one must be last
      NUM_FUNCTIONS};
private:
  TestData *data;
  iMesh_Instance instance;
  iBase_EntitySetHandle rootSet;
  int reportedAdjTable[4][4];
  int storageOrder;
  int numPassed, numFailed;
  enum status testStatus[NUM_FUNCTIONS], thisTestStatus;
  enum functionEnum testInProgress;
  bool isThisFuncOK, gotRootSet, checkErrorHandling;
  void requireRootSet();
public:
  iMeshTest();
  virtual ~iMeshTest();
  // These functions are for data setup prior to testing.
  void provideTestData(TestData * const ptrData);
  void provideMeshInstance(const iMesh_Instance inst);
  void provideRootSet(const iBase_EntitySetHandle rootSetIn);
  // Some output at the end would be nice...
  void printSummary();
  // The following test specific iMesh functions with the same 
  // names. 
  bool dtor();
  bool save(const char *name, const char *options);
  bool areEHValid(const int doReset);
  bool getRootSet();
  bool load(const char *name, const char *options);
  bool newMesh(const char *options);
  bool getDfltStorage();
  bool getAdjTable();
  bool getGeometricDimension();
  bool getNumOfTopo(const int entTopo);
  bool getNumOfType(const int entType);
  bool getAllVtxCoords(const int order);
};

#endif
