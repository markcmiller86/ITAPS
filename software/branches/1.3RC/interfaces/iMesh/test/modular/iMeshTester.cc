#include <stdlib.h>

#include <cstdio>
#include <iostream>
#include <sstream>

#include "iMeshTester.hh"

static char s_testNames[][32] =
  {"setup",
   "getErrorType", "getDescription", "newMesh", "dtor",
   "load", "save", "getRootSet",
   "getGeometricDimension", "setGeometricDimension",
   "getDfltStorage", "getAdjTable", "getNumOfType", "getNumOfTopo",
   "areEHValid", "getEntities", "getAdjEntIndices", 
   "initEntIter", "getNextEntIter", "resetEntIter", "endEntIter",
   "getEntTopo", "getEntType", "getEntAdj", "getEnt2ndAdj",
   "getVtxCoord", "setVtxCoord", "createVtx", "createEnt", "deleteEnt", 
   "initEntArrIter", "getNextEntArrIter",
   "resetEntArrIter", "endEntArrIter",
   "getEntArrTopo", "getEntArrType", "getEntArrAdj", "getEntArr2ndAdj",
   "getVtxArrCoords", "setVtxArrCoords", "createVtxArr",
   "createEntArr", "deleteEntArr", 
   "createEntSet", "destroyEntSet", "isList",
   "getNumEntSets", "getEntSets", "addEntToSet", "rmvEntFromSet",
   "addEntArrToSet", "rmvEntArrFromSet", "addEntSet", "rmvEntSet",
   "isEntContained", "isEntArrContained", "isEntSetContained",
   "addPrntChld", "rmvPrntChld", "isChildOf", "getNumChld",
   "getNumPrnt", "getChldn", "getPrnts",
   "subtract", "intersect", "unite", 
   "createTag", "destroyTag", "getTagName", "getTagSizeValues",
   "getTagSizeBytes", "getTagHandle", "getTagType",  
   "getData", "getIntData", "getDblData", "getEHData",
   "setData", "setIntData", "setDblData", "setEHData",
   "rmvTag", "getAllTags", 
   "getArrData", "getIntArrData", "getDblArrData", "getEHArrData",
   "setArrData", "setIntArrData", "setDblArrData", "setEHArrData",
   "rmvArrTag", 
   "setEntSetData", "setEntSetIntData",
   "setEntSetDblData", "setEntSetEHData",
   "getEntSetData", "getEntSetIntData",
   "getEntSetDblData", "getEntSetEHData",
   "getAllEntSetTags", "rmvEntSetTag"};

iMeshTester::iMeshTester(iMeshWrapper *pMW_test, iMeshWrapper *pMW_ref) :
  m_pMW_test(pMW_test), m_testInstance(pMW_test->getInstance()),
  m_testRootSet(pMW_test->getRootSetValue()),
  m_pMW_ref(pMW_ref), m_refInstance(pMW_ref->getInstance()),
  m_refRootSet(pMW_ref->getRootSetValue()),
  m_err(INT_MIN), m_refErr(INT_MIN), m_numPassed(0), m_numFailed(0),
  m_numSubtestsPassed(0), m_numSubtestsFailed(0)
{
  setupEntityMaps();
}

iMeshTester::~iMeshTester()
{
  // Print some parting diagnostics, etc.
  std::cout << std::endl;
  std::cout << "Overall, passed " << m_numPassed << " out of "
	    << m_numPassed + m_numFailed << " tests performed." << std::endl;
}

void iMeshTester::startTest(const enum ModUnit_funcs func)
{
  m_currentFunc = func;
  snprintf(m_testID, 32, "%s", s_testNames[func]);
  m_err = INT_MIN;
  m_numSubtestsPassed = 0;
  m_numSubtestsFailed = 0;
  std::cout << "Testing " << m_testID << "..." << std::endl;
  m_passedAllTests[m_currentFunc] = false;
}

void iMeshTester::finishTest()
{
  if (m_numSubtestsFailed > 0) {
    std::cout << " passed " << m_numSubtestsPassed << "; failed "
	      << m_numSubtestsFailed << " sub-tests for "
	      << m_testID << "." << std::endl;
    m_numFailed++;
  }
  else if (m_numSubtestsPassed > 0) {
    std::cout << " passed all " << m_numSubtestsPassed << " sub-tests for "
	      << m_testID << "."
	      << std::endl;
    m_numPassed++;
    m_passedAllTests[m_currentFunc] = true;
  }
  else {
    std::cout << " no subtests for " << m_testID << "!" << std::endl;
    m_numFailed++;
  }
}

void iMeshTester::checkError(const char* fileName, int line,
			     int& err, const int errExpected)
{
  if (err == errExpected) {
    m_numSubtestsPassed++;
  }
  else if (err == INT_MIN) {
    std::cerr << "Function " << m_testID << " failed to reset error code."
	      << std::endl;
    m_numSubtestsFailed++;
  }
  else {
    m_numSubtestsFailed++;
    std::cerr << "While testing " << m_testID
	      << ", a function returned error code " << err
	      << " when code " << errExpected << " was expected."
	      << std::endl
	      << "Source file: " << fileName 
	      << " line " << line << std::endl;
  }
  err = INT_MIN;
}

void iMeshTester::test(const bool expr, std::string message)
{
  if (expr) {
    m_numSubtestsPassed++;
  }
  else {
    m_numSubtestsFailed++;
    std::cerr << "Function " << m_testID << " has an error: "
	      << std::endl << "  " << message << std::endl;
  }
}

bool iMeshTester::equivAdjacency(const int sourceType, const int targetType,
				 iBase_EntityHandle testEnt,
				 iBase_EntityHandle testResults[],
				 const int testEnts_size,
				 iBase_EntityHandle refEnt,
				 iBase_EntityHandle refResults[],
				 const int refEnts_size)
{
  // This test is hopelessly inadequate.
  return (testEnts_size == refEnts_size);
}
