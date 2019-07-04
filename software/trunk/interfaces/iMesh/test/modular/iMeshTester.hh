#ifndef iMeshTester_hh_
#define iMeshTester_hh_

#define NULL_HANDLE reinterpret_cast<iBase_EntityHandle>(INT_MIN+1)

// Automatically provide args for the checkError function.
#define CHECK_ERROR(a) checkError(__FILE__, __LINE__, a)
#define CHECK_ERROR2(a, b) checkError(__FILE__, __LINE__, a, b)

#include <map>

#include "iMeshWrapper.hh"

static char topoNames[][32] =
  {"point", "line segment", "polygon", "triangle", "quadrilateral",
   "polyhedron", "tetrahedron", "hexahedron", "prism", "pyramid",
   "septahedron", "all topologies"};

static char typeNames[][32] =
  {"vertex", "edge", "face", "region", "all types"};

class iMeshTester {
private:
  iMeshWrapper *m_pMW_test;
  iMesh_Instance m_testInstance;
  iBase_EntitySetHandle m_testRootSet;

  iMeshWrapper *m_pMW_ref;
  iMesh_Instance m_refInstance;
  iBase_EntitySetHandle m_refRootSet;

  std::map<iBase_EntityHandle, iBase_EntityHandle> m_refToTestVertMap;
  std::map<iBase_EntityHandle, iBase_EntityHandle> m_testToRefEntMap,
     m_refToTestEntMap;

  int m_err, m_refErr;
  int m_numPassed, m_numFailed, m_numSubtestsPassed, m_numSubtestsFailed;
  char m_testID[32];

  enum ModUnit_funcs
    { // Testing functions that don't appear in iMesh.
      MU_setup, 
      // Database functions (16)
      MU_getErrorType, MU_getDescription, MU_newMesh, MU_dtor,
      MU_load, MU_save, MU_getRootSet,
      MU_getGeometricDimension, MU_setGeometricDimension,
      MU_getDfltStorage, MU_getAdjTable, MU_getNumOfType, MU_getNumOfTopo,
      MU_areEHValid, MU_getEntities, MU_getAdjEntIndices,
      // Entity functions (13)
      MU_initEntIter, MU_getNextEntIter, MU_resetEntIter, MU_endEntIter,
      MU_getEntTopo, MU_getEntType, MU_getEntAdj, MU_getEnt2ndAdj,
      MU_getVtxCoord, MU_setVtxCoord, MU_createVtx, MU_createEnt, MU_deleteEnt,
      // Entity array functions (13)
      MU_initEntArrIter, MU_getNextEntArrIter,
      MU_resetEntArrIter, MU_endEntArrIter,
      MU_getEntArrTopo, MU_getEntArrType, MU_getEntArrAdj, MU_getEntArr2ndAdj,
      MU_getVtxArrCoords, MU_setVtxArrCoords, MU_createVtxArr,
      MU_createEntArr, MU_deleteEntArr,
      // Set functions (24)
      MU_createEntSet, MU_destroyEntSet, MU_isList,
      MU_getNumEntSets, MU_getEntSets, MU_addEntToSet, MU_rmvEntFromSet,
      MU_addEntArrToSet, MU_rmvEntArrFromSet, MU_addEntSet, MU_rmvEntSet,
      MU_isEntContained, MU_isEntArrContained, MU_isEntSetContained,
      MU_addPrntChld, MU_rmvPrntChld, MU_isChildOf, MU_getNumChld,
      MU_getNumPrnt, MU_getChldn, MU_getPrnts,
      MU_subtract, MU_intersect, MU_unite,
      // Basic tag functions (7)
      MU_createTag, MU_destroyTag, MU_getTagName, MU_getTagSizeValues,
      MU_getTagSizeBytes, MU_getTagHandle, MU_getTagType, 
      // Entity tag data manipulation (10)
      MU_getData, MU_getIntData, MU_getDblData, MU_getEHData,
      MU_setData, MU_setIntData, MU_setDblData, MU_setEHData, 
      MU_rmvTag, MU_getAllTags,
      // Entity array tag data manipulation (9)
      MU_getArrData, MU_getIntArrData, MU_getDblArrData, MU_getEHArrData,
      MU_setArrData, MU_setIntArrData, MU_setDblArrData, MU_setEHArrData,
      MU_rmvArrTag,
      // Set tag data manipulation (10)
      MU_setEntSetData, MU_setEntSetIntData,
      MU_setEntSetDblData, MU_setEntSetEHData,
      MU_getEntSetData, MU_getEntSetIntData,
      MU_getEntSetDblData, MU_getEntSetEHData,
      MU_getAllEntSetTags, MU_rmvEntSetTag,
      MU_NUM_FUNCS
    } m_currentFunc;
  bool m_passedAllTests[MU_NUM_FUNCS];

  iMeshTester(const iMeshTester&) {}
  void setupEntityMaps();
  bool equivAdjacency(const int sourceType, const int targetType,
		      iBase_EntityHandle testEnt,
		      iBase_EntityHandle testResults[],
		      const int testEnts_size,
		      iBase_EntityHandle refEnt,
		      iBase_EntityHandle refResults[],
		      const int refEnts_size);
public:
  iMeshTester(iMeshWrapper *pMW_test, iMeshWrapper *pMW_ref);
  ~iMeshTester();

  /// Begin testing a function
  /// \param func Enumerated value telling which function is being tested.
  void startTest(const enum ModUnit_funcs func);

  /// Finish testing a function.  Among other things, print a diagnostic
  /// for the function just tested and update the count of functions
  /// that have passed and failed.
  void finishTest();

  /// Check an iMesh function's error return.  Getting the wrong value
  /// is grounds for ruling that this function fails its test.
  void checkError(const char* fileName, int line,
		  int& err, const int errExpected = iBase_SUCCESS);

  /// Check something about data returned by the tested implementation.
  /// If expr is false, then this function is deemed incorrect and fails.
  void test(bool expr, std::string message);
  
  // No functions below here that aren't just test wrappers for
  // individual iMesh functions.

  // Each of the following functions tests the corresponding iMesh
  // function.

  // Full-database tests
  void test_newMesh_dtor(std::string optionPrefix);
  void test_getDescription();
  void test_getRootSet();
  void test_getGeometricDimension();
  void test_getDfltStorage();
  void test_getAdjTable();
  void test_getNumOfTopo();
  void test_getNumOfType();

  // Entity tests
  void test_getEntAdj();
};
#endif
