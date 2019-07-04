#include <stdlib.h>
#include <sstream>

#include "iMeshTester.hh"

// getRootSet error codes:  Should never fail (always iBase_SUCCESS).
void iMeshTester::test_getRootSet() {
  // Can only test if the function exists.
  if (m_pMW_test->getRootSet) {
    startTest(MU_getRootSet);
    m_testRootSet = reinterpret_cast<iBase_EntitySetHandle>(0xffffffff);
    m_pMW_test->getRootSet(m_testInstance, &m_testRootSet, &m_err);
    CHECK_ERROR(m_err);
    if (m_testRootSet == reinterpret_cast<iBase_EntitySetHandle>(0xffffffff)) {
      // It's possible that this is the value that the implementation
      // actually uses.  If so, we should get the same answer from a
      // second call with different input.
      m_testRootSet = reinterpret_cast<iBase_EntitySetHandle>(0x0);
      m_pMW_test->getRootSet(m_testInstance, &m_testRootSet, &m_err);
      test(m_testRootSet ==
	   reinterpret_cast<iBase_EntitySetHandle>(0xffffffff),
	   "rootSet is not consistently defined");
    }
    finishTest();
  }
}

// getGeometricDimension error codes:  Should never fail (always iBase_SUCCESS).
void iMeshTester::test_getGeometricDimension() {
  if (m_pMW_test->getGeometricDimension) {
    startTest(MU_getGeometricDimension);
    int testDim = -1, refDim = -1;
    m_pMW_test->getGeometricDimension(m_testInstance, &testDim, &m_err);
    CHECK_ERROR(m_err);
    m_pMW_ref->getGeometricDimension(m_refInstance, &refDim, &m_refErr);
    test(testDim == refDim, "Wrong geometric dimension");
    test(testDim > 0 && testDim <= 3, "Geometric dimension out of range");
    finishTest();
  }
}

// getNumOfTopo error codes:
//
// iBase_INVALID_ENTITY_TOPOLOGY: iff topo < 0 or > iMesh_ALL_TOPOLOGIES
void iMeshTester::test_getNumOfTopo() {
  if (m_pMW_test->getNumOfTopo) {
    startTest(MU_getNumOfTopo);
    int testNumByTopo[iMesh_ALL_TOPOLOGIES+1], testSum = 0;
    int refNumByTopo[iMesh_ALL_TOPOLOGIES+1], refSum = 0;
    for (int i = 0; i < iMesh_ALL_TOPOLOGIES; i++) {
      m_pMW_test->getNumOfTopo(m_testInstance, m_testRootSet,
			       i, &testNumByTopo[i], &m_err);
      CHECK_ERROR(m_err);
      m_pMW_ref->getNumOfTopo(m_refInstance, m_refRootSet, i,
			      &refNumByTopo[i], &m_refErr);
      testSum += testNumByTopo[i];
      refSum += refNumByTopo[i];
      std::stringstream ss;
      ss << "Incorrect number of entities with topology " << i << std::endl
	 << "  Correct number is " << refNumByTopo[i] << "; got "
	 << testNumByTopo[i] << std::endl;
      std::string message = ss.str();
      // An implementation may not represent all entities directly.
      test(testNumByTopo[i] == refNumByTopo[i] || testNumByTopo[i] == 0,
	   message);
      if (testNumByTopo[i] == 0 && refNumByTopo[i] != 0) {
	std::cout << "  Not representing " << topoNames[i]
		  << "s in the mesh."
		  << std::endl;
      }
      else if (testNumByTopo[i] == 0) {
	std::cout << "  Not representing " << topoNames[i]
		  << "s in the mesh or none were present."
		  << std::endl;
      }
      else  if (testNumByTopo[i] == refNumByTopo[i]) {
	std::cout << "  Representing all " << topoNames[i] << "s in the mesh."
		  << std::endl;
      }
      else {
	std::cout << "  Representing only some " << topoNames[i]
		  << "s in the mesh." << std::endl;
      }
    }

    // Now for the total number, which should match the cumulative total.
    int testTotal = -1;
    m_pMW_test->getNumOfTopo(m_testInstance, m_testRootSet,
			     iMesh_ALL_TOPOLOGIES, &testTotal, &m_err);
    CHECK_ERROR(m_err);
    std::stringstream ss;
    ss << "Incorrect total number of entities " << std::endl
       << "  Correct number for this implementation is " << testSum
       << "; got " << testTotal << std::endl;
    std::string message = ss.str();
    // An implementation may not represent all entities directly.
    test(testSum == testTotal, message);
    
    // Check for error throwing.
    int junk;
    m_pMW_test->getNumOfTopo(m_testInstance, m_testRootSet,
			     -1, &junk, &m_err);
    CHECK_ERROR2(m_err, iBase_INVALID_ENTITY_TOPOLOGY);

    m_pMW_test->getNumOfTopo(m_testInstance, m_testRootSet,
			     iMesh_ALL_TOPOLOGIES+1, &junk, &m_err);
    CHECK_ERROR2(m_err, iBase_INVALID_ENTITY_TOPOLOGY);

    finishTest();
  }
} // Testing getNumOfTopo

// getNumOfType error codes:
//
// iBase_INVALID_ENTITY_TYPE: iff type < 0 or > iBase_ALL_TYPES
void iMeshTester::test_getNumOfType() {
  if (m_pMW_test->getNumOfType) {
    startTest(MU_getNumOfType);
    int testNumByType[iBase_ALL_TYPES+1], testSum = 0;
    int refNumByType[iBase_ALL_TYPES+1], refSum = 0;
    for (int i = 0; i < iBase_ALL_TYPES; i++) {
      m_pMW_test->getNumOfType(m_testInstance, m_testRootSet,
			       i, &testNumByType[i], &m_err);
      CHECK_ERROR(m_err);
      m_pMW_ref->getNumOfType(m_refInstance, m_refRootSet, i,
			      &refNumByType[i], &m_refErr);
      testSum += testNumByType[i];
      refSum += refNumByType[i];
      std::stringstream ss;
      ss << "Incorrect number of entities with type " << i << std::endl
	 << "  Correct number is " << refNumByType[i] << "; got "
	 << testNumByType[i] << std::endl;
      std::string message = ss.str();
      // An implementation may not represent all entities directly.
      test(testNumByType[i] == refNumByType[i] || testNumByType[i] == 0,
	   message);
      if (testNumByType[i] == 0 && refNumByType[i] != 0) {
	std::cout << "  Not representing " << typeNames[i]
		  << "s in the mesh."
		  << std::endl;
      }
      else if (testNumByType[i] == 0) {
	std::cout << "  Not representing " << typeNames[i]
		  << "s in the mesh or none were present."
		  << std::endl;
      }
      else  if (testNumByType[i] == refNumByType[i]) {
	std::cout << "  Representing all " << typeNames[i] << "s in the mesh."
		  << std::endl;
      }
      else {
	std::cout << "  Representing only some " << typeNames[i]
		  << "s in the mesh." << std::endl;
      }
    }

    // Now for the total number, which should match the cumulative total.
    int testTotal = -1;
    m_pMW_test->getNumOfType(m_testInstance, m_testRootSet,
			     iBase_ALL_TYPES, &testTotal, &m_err);
    CHECK_ERROR(m_err);
    std::stringstream ss;
    ss << "Incorrect total number of entities " << std::endl
       << "  Correct number for this implementation is " << testSum
       << "; got " << testTotal << std::endl;
    std::string message = ss.str();
    // An implementation may not represent all entities directly.
    test(testSum == testTotal, message);
    
    // Check for error throwing.
    int junk;
    m_pMW_test->getNumOfType(m_testInstance, m_testRootSet,
			     -1, &junk, &m_err);
    CHECK_ERROR2(m_err, iBase_INVALID_ENTITY_TYPE);

    m_pMW_test->getNumOfType(m_testInstance, m_testRootSet,
			     iBase_ALL_TYPES+1, &junk, &m_err);
    CHECK_ERROR2(m_err, iBase_INVALID_ENTITY_TYPE);

    finishTest();
  }
} // Testing getNumOfType

// getDescription error codes:  Should never fail (always iBase_SUCCESS).
void iMeshTester::test_getDescription()
{
  // This function takes an error code that was returned by some other
  // function and returns a description.  Error codes that are out of
  // range should be mapped by the implementation to iBase_FAILURE.
  // (It's possible that an implementation could return a non-ITAPS
  // error code and be able to describe it accurately; it's not clear
  // that this is a good idea, really, but it's possible.)

  // No real way to test this, as there's no standard result.  So the
  // test is only for existence.
  if (m_pMW_test->getDescription) {
    startTest(MU_getDescription);
    test(true, "Null test");
    finishTest();
  }
}

// getDfltStorage error codes:  Should never fail (always iBase_SUCCESS).
void iMeshTester::test_getDfltStorage()
{
  if (m_pMW_test->getDfltStorage) {
    startTest(MU_getDfltStorage);
    int order;
    m_pMW_test->getDfltStorage(m_testInstance, &order, &m_err);
    CHECK_ERROR(m_err);
    test(order == iBase_BLOCKED ||
	 order == iBase_INTERLEAVED, "Invalid default storage order");
    finishTest();
  }
}

// getAdjTable error codes:
//
// Can fail in the standard array ways:
//   allocated size too small for pre-allocated array
//   out of memory (tough to survive this test, even if it were done)
//
// Otherwise, should return SUCCESS.
void iMeshTester::test_getAdjTable()
{
  if (m_pMW_test->getAdjTable) {
    startTest(MU_getAdjTable);
    {
      int *adjTable, adjTable_alloc(0), adjTable_size(-1);
      // Should work with an unallocated array.
      m_pMW_test->getAdjTable(m_testInstance, &adjTable,
			      &adjTable_alloc, &adjTable_size, &m_err);
      CHECK_ERROR(m_err); // Should always succeed on this call.
      test(adjTable_alloc == 16 && adjTable_size == 16,
	   "Adjacency table has the wrong size");
      bool qOK = true;
      for (int ii = 0; ii < 16; ii++) {
	qOK = qOK && (adjTable[ii] >= iBase_UNAVAILABLE &&
		      adjTable[ii] <= iBase_SOME_ORDER_N);
      }
      test(qOK, "Adjacency cost value out of range");
      free(adjTable);
    }

    // Should also work with a pre-allocated array.
    {
      int *adjTable = new int[16];
      int adjTable_alloc(16), adjTable_size(-1);
      // Should work with an unallocated array.
      m_pMW_test->getAdjTable(m_testInstance, &adjTable,
			      &adjTable_alloc, &adjTable_size, &m_err);
      CHECK_ERROR(m_err); // Should always succeed on this call.
      test(adjTable_alloc == 16 && adjTable_size == 16,
	   "Adjacency table has the wrong size");
      free(adjTable);
    }
    
    // Test for error conditions.
    {
      int *adjTable = new int[16];
      int adjTable_alloc(5), adjTable_size(-1);
      // Should work with an unallocated array.
      m_pMW_test->getAdjTable(m_testInstance, &adjTable,
			      &adjTable_alloc, &adjTable_size, &m_err);
      CHECK_ERROR2(m_err, iBase_BAD_ARRAY_SIZE); 

      adjTable_alloc = 16;
      adjTable_size = 5;
      // This one should work, since size is supposed to be an output parameter.
      m_pMW_test->getAdjTable(m_testInstance, &adjTable,
			      &adjTable_alloc, &adjTable_size, &m_err);
      CHECK_ERROR(m_err); 
      test(adjTable_alloc == 16 && adjTable_size == 16,
	   "Adjacency table has the wrong size");

      free(adjTable);
    }
    finishTest();
  }
}

// newMesh error codes:
//
// - Can fail because of lack of memory.
//
// - Can also get hit with an option that it should recognize (as
//   determined by examining the option prefix) but doesn't (because of
//   a typo, for instance). (iBase_INVALID_ARGUMENT)
//
// - Any others?
//
// dtor error codes:
//
// Can fail because the instance has already been destroyed.  First,
// should this be an error (I vote yes).  Second, what error code should
// this be? Right now, I'm expecting iBase_INVALID_ARGUMENT.
void iMeshTester::test_newMesh_dtor(std::string optionPrefix)
{
  if (m_pMW_test->newMesh) {
    startTest(MU_newMesh);
    // First check creation
    iMesh_Instance newInstance;
    m_pMW_test->newMesh("", &newInstance, &m_err, 0);
    CHECK_ERROR(m_err);

    // Now destroy it.
    m_pMW_test->dtor(newInstance, &m_err);
    CHECK_ERROR(m_err);

    // Now try it again; this should fail.
    m_pMW_test->dtor(newInstance, &m_err);
    CHECK_ERROR2(m_err, iBase_INVALID_ARGUMENT);

    // Now try creating a mesh with a bogus argument.  This should
    // error, but still return an instance.
    {
      iMesh_Instance *instance = NULL;
      std::string fakeOption = optionPrefix + ":asdfsadf";
      m_pMW_test->newMesh(fakeOption.c_str(), instance, &m_err,
			  fakeOption.size());
      CHECK_ERROR2(m_err, iBase_INVALID_ARGUMENT);
      test(instance == NULL, "Didn't create an instance with no non-fake args");
    }
    finishTest();
  }

}

//       MU_getErrorType, MU_getDescription, MU_newMesh, MU_dtor,
//       MU_load, MU_save, MU_setGeometricDimension,
//       MU_areEHValid, MU_getEntities, MU_getAdjEntIndices,
