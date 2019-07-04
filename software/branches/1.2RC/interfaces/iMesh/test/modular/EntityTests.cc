#include "stdlib.h"

#include <sstream>

#include "iMeshTester.hh"

void iMeshTester::test_getEntAdj() {
  if (m_pMW_test->getEntAdj) {
    startTest(MU_getEntAdj);

    iBase_AdjacencyCost measuredAdj[4][4];
    for (int ii = 0; ii < 4; ii++) {
      for (int jj = 0; jj < 4; jj++) {
	measuredAdj[ii][jj] = iBase_UNAVAILABLE;
      }
    }

    // Unlikely to ever return 10000 entities from a single adjacency call.
    iBase_EntityHandle *testEnts, *refEnts;
    int testEnts_alloc = 10000, testEnts_size = -1;
    int refEnts_alloc = 10000, refEnts_size = -1;
    testEnts = new iBase_EntityHandle[testEnts_alloc];
    refEnts = new iBase_EntityHandle[refEnts_alloc];

    for (int sourceType = iBase_VERTEX; sourceType <= iBase_REGION;
	 sourceType++) {
      iBase_EntityHandle *refSource;
      int refSource_alloc = 0, refSource_size = -1;

      m_pMW_ref->getEntities(m_refInstance, m_refRootSet, sourceType,
			     iMesh_ALL_TOPOLOGIES,
			     &refSource, &refSource_alloc, &refSource_size,
			     &m_refErr);
      CHECK_ERROR(m_refErr);
      // Can't do anything without some entities.
      if (refSource_size <= 0) continue;

      // Map these to test entities.
      iBase_EntityHandle *testSource = new iBase_EntityHandle[refSource_size];
      for (int ii = 0; ii < refSource_size; ii++) {
	std::map<iBase_EntityHandle, iBase_EntityHandle>::iterator
	  iter = m_refToTestEntMap.find(refSource[ii]);
	if (iter != m_refToTestEntMap.end()) {
	  testSource[ii] = iter->second;
	}
	else {
	  testSource[ii] = NULL_HANDLE;
	}
      }

      for (int targetType = iBase_VERTEX; targetType <= iBase_ALL_TYPES;
	   targetType++) {
	int attempted = 0, succeeded = 0;
	for (int ent = 0; ent < refSource_size; ent++) {
	  // Don't try this with handles that didn't map.
	  if (testSource[ent] == NULL_HANDLE) continue;

	  attempted++;
	  m_pMW_test->getEntAdj(m_testInstance, testSource[ent], targetType,
				&testEnts, &testEnts_alloc, &testEnts_size,
				&m_err);
	  CHECK_ERROR(m_err);
				
	  m_pMW_ref->getEntAdj(m_refInstance, refSource[ent], targetType,
			       &refEnts, &refEnts_alloc, &refEnts_size,
			       &m_refErr);
	  CHECK_ERROR(m_refErr);

	  bool result = equivAdjacency(sourceType, targetType, 
				       testSource[ent], testEnts, testEnts_size,
				       refSource[ent], refEnts, refEnts_size);
	  if (result) succeeded++;
	}
	std::cout << "  Source type: " << typeNames[sourceType] << std::endl
		  << "  Target type: " << typeNames[targetType] << std::endl;
	std::cout << "Attempted " << attempted << " adjacency checks; "
		  << succeeded << " of these succeeded." << std::endl;
	std::stringstream ss;
	ss << "Error with adjacency retrieval." << std::endl;
	std::string message = ss.str();
	test(succeeded == attempted, message);

	if (succeeded == attempted) {
	  if (attempted == refSource_size)
	    measuredAdj[sourceType][targetType] = iBase_ALL_ORDER_1;
	  else
	    measuredAdj[sourceType][targetType] = iBase_SOME_ORDER_1;
	}
      }
      delete [] testSource;
      free(refSource);
    }
    delete [] testEnts;
    delete [] refEnts;

    // Possible error conditions:
    // Invalid handle, invalid type, pre-allocated too small

    // QUESTION
    // Does "SOME" mean all the adjacencies for some entities, or some
    // adjacencies for some/all entities?  Right now, the code assumes
    // all for some. 

    

    finishTest();
  }
}

