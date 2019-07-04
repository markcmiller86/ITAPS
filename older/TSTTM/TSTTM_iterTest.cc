#include "TSTTM_unitTest.hh"

void vCreateIterTestVerts(const int iDim, TSTTM::Mesh& icMesh)
  // Creates and returns a new mesh containing a flock of vertices.
  // This function is used repeatedly by the code that tests iterators
  // in the presence of mesh modification.
  //
  // This call throws an error is something goes so wrong that the
  // initial mesh can't be created.
{
  icMesh = TSTTM::Factory::newMesh(CREATE_ARG);
  TSTTM::Modify ModMesh(icMesh);
  if (ModMesh._is_nil()) {
    // Complete train wreck
    TEST(data_preparation);
  }

  if (iDim == 2) {
    // Need to be able to create five quads, so there need to be at
    // least eleven points.
    try {
      opaque new_vert;
      // The third arg doesn't matter in 2D.
      ModMesh.createVtx(0, 0, -100, new_vert);
      ModMesh.createVtx(0, 1, -100, new_vert);
      ModMesh.createVtx(0, 2, -100, new_vert);
      ModMesh.createVtx(1, 0, -100, new_vert);
      ModMesh.createVtx(1, 1, -100, new_vert);
      ModMesh.createVtx(1, 2, -100, new_vert);
      ModMesh.createVtx(2, 0, -100, new_vert);
      ModMesh.createVtx(2, 1, -100, new_vert);
      ModMesh.createVtx(2, 2, -100, new_vert);
      ModMesh.createVtx(3, 0, -100, new_vert);
      ModMesh.createVtx(3, 1, -100, new_vert);
    } catch (...) {
      // Again, a complete train wreck: failed to create eleven vertices.
      TEST(data_preparation);
    }
  }
  else if (iDim == 3) {
    // Need to be able to create five hexes, so there need to be at
    // least twenty-two points.
    try {
      opaque new_vert;
      ModMesh.createVtx(0, 0, 0, new_vert);
      ModMesh.createVtx(0, 1, 0, new_vert);
      ModMesh.createVtx(0, 2, 0, new_vert);
      ModMesh.createVtx(1, 0, 0, new_vert);
      ModMesh.createVtx(1, 1, 0, new_vert);
      ModMesh.createVtx(1, 2, 0, new_vert);
      ModMesh.createVtx(2, 0, 0, new_vert);
      ModMesh.createVtx(2, 1, 0, new_vert);
      ModMesh.createVtx(2, 2, 0, new_vert);
      ModMesh.createVtx(0, 0, 1, new_vert);
      ModMesh.createVtx(0, 1, 1, new_vert);
      ModMesh.createVtx(0, 2, 1, new_vert);
      ModMesh.createVtx(1, 0, 1, new_vert);
      ModMesh.createVtx(1, 1, 1, new_vert);
      ModMesh.createVtx(1, 2, 1, new_vert);
      ModMesh.createVtx(2, 0, 1, new_vert);
      ModMesh.createVtx(2, 1, 1, new_vert);
      ModMesh.createVtx(2, 2, 1, new_vert);
      ModMesh.createVtx(0, 0, 2, new_vert);
      ModMesh.createVtx(0, 1, 2, new_vert);
      ModMesh.createVtx(1, 0, 2, new_vert);
      ModMesh.createVtx(1, 1, 2, new_vert);
    } catch (...) {
      // Again, a complete train wreck: failed to create 22 vertices.
      TEST(data_preparation);
    }
  }
  // No other possibilities exist; the code can't get to this point with
  // other numbers of spatial dimensions.
}

// For each entity type, there are two creation tests and half a dozen
// or so deletion tests.  Each test begins by creating a standard mesh
// with a known collection of entities.
//
// Creation tests:
//   1. With the iterator pointing to an entity other than the last,
//      create a new entity.  Expect: iterator should reach new entity,
//      too.
//   2. Repeat creation test 1 with iterator pointing to the last
//      entity.
//
// Deletion tests:
//   1. With the iterator pointing to the last entity, delete it.  When
//      the iterator is incremented, it should return "no more data".
//   2. With the iterator pointing to some entity other than the last one,
//      delete that entity.  Iterator should continue and traverse all
//      entities.
//   3. With the iterator pointing to the first entity,
//      delete that entity.  Iterator should continue and traverse all
//      entities.
//   4. With the iterator pointing to some entity other than the last
//      one, delete another entity other than the last one, but later in
//      the sequence.  Iterator should continue, stepping over the
//      deleted entity.
//   5. With the iterator pointing to some entity other than the last
//      one, delete the last one.  Iterator should continue, but never
//      return the deleted last entity.
//   6. Repeat deletion test 5, but with the iterator pointing to the
//      second-last entity.  Iterator should return "no more data" when
//      advanced.
//   7. Repeat deletion test 4, deleting an entity -before- the current
//      iterator.  Should still touch every entity. 

static void vGetSelectedVertHandles(TSTTM::Entity& EntMesh,
				    opaque& oSecond,
				    opaque& oThird,
				    opaque& oFourth,
				    opaque& oSecondLast,
				    opaque& oLast)
{
  // This code assumes at least six vertices exist, and returns handles
  // for verts 2, 3, 4, N-1, and N.
  opaque iter;
  bool qValidData = EntMesh.initEntIter(EntMesh.getRootSet(),
					TSTTM::EntityType_VERTEX,
					TSTTM::EntityTopology_POINT, iter);

  assert(qValidData);
  qValidData = EntMesh.getNextEntIter(iter, oSecond);
  assert(qValidData);
  qValidData = EntMesh.getNextEntIter(iter, oThird);
  assert(qValidData);
  qValidData = EntMesh.getNextEntIter(iter, oFourth);
  assert(qValidData);

  oLast = oThird;
  opaque oTmp = oFourth;
  do {
    oSecondLast = oLast;
    oLast = oTmp;
    qValidData = EntMesh.getNextEntIter(iter, oTmp);
  } while (qValidData);
}

static void vTestVertIterWithModify(const int iDim)
{
  { // Creation test 1
    TSTTM::Mesh TMesh;
    vCreateIterTestVerts(iDim, TMesh);
    // This mesh supports Modify and Entity, or we wouldn't be here, but
    // check the result anyway. 
    TSTTM::Modify ModMesh(TMesh);
    TSTTM::Entity EntMesh(ModMesh);
    if (EntMesh._not_nil()) {
      opaque oRootSet = EntMesh.getRootSet(), oIter;
      bool qValidData = EntMesh.initEntIter(oRootSet,
					    TSTTM::EntityType_VERTEX,
					    TSTTM::EntityTopology_POINT,
					    oIter);
      if (!qValidData) {
	cout << "Could not create iterator for vertices, even though we "
	     << "should be able to!" << endl;
	TEST(data_preparation);
      }
      // Okay, now we know we can get a valid iterator.

      // Creation test 1.
      opaque entity, new_vert, lastHandle;
      qValidData = EntMesh.getNextEntIter(oIter, entity);
      bool qOK = qValidData;
      // These will only throw if creation fails completely, in which
      // case we should never have gotten here.
      if (iDim == 2) {
	ModMesh.createVtx(3.14159, 2.81828, -100, new_vert);
      }
      else {
	// iDim == 3
	ModMesh.createVtx(3.14159, 2.81828, 0.69315, new_vert);
      }
      int iCount = 1;
      while(qValidData) {
	iCount++;
	opaque tmp_ent;
	qValidData = EntMesh.getNextEntIter(oIter, tmp_ent);
	if (qValidData) lastHandle = tmp_ent;
      }
      bool creation_test_1 =
	(iCount == EntMesh.getNumOfType(oRootSet, TSTTM::EntityType_VERTEX)
	   && lastHandle == new_vert);
      TEST(creation_test_1);
    } // Created and tested an EntMesh
    else {
      // No EntMesh cast possible
      bool not_an_Entity_Mesh = false;
      TEST(not_an_Entity_Mesh);
    }
  }

  { // Creation test 2
    TSTTM::Mesh TMesh;
    vCreateIterTestVerts(iDim, TMesh);
    // This mesh supports Modify and Entity, or we wouldn't be here, but
    // check the result anyway. 
    TSTTM::Modify ModMesh(TMesh);
    TSTTM::Entity EntMesh(ModMesh);
    if (EntMesh._not_nil()) {
      opaque oRootSet = EntMesh.getRootSet(), oIter;
      bool qValidData = EntMesh.initEntIter(oRootSet, TSTTM::EntityType_VERTEX,
					    TSTTM::EntityTopology_POINT,
					    oIter);
      if (!qValidData) {
	cout << "Could not create iterator for vertices, even though we "
	     << "should be able to!" << endl;
	TEST(data_preparation);
      }
      // Okay, now we know we can get a valid iterator.

      // Creation test 2.
      opaque lastHandle, new_vert;
      int iNEnts = EntMesh.getNumOfType(oRootSet, TSTTM::EntityType_VERTEX);
      for (int iEnt = 1; qValidData && iEnt < iNEnts; iEnt++) {
	qValidData = EntMesh.getNextEntIter(oIter, lastHandle);
      }	
      if (!qValidData) TEST(data_preparation);

      // Creation should always succeed, since it did when testing
      // Modify, but just to be sure...
      try {
	if (iDim == 2) {
	  ModMesh.createVtx(3.14159, 2.81828, -100, new_vert);
	}
	else {
	  // iDim == 3
	  ModMesh.createVtx(3.14159, 2.81828, 0.69315, new_vert);
	}
      }
      catch (...) {
	const bool creation_error = false;
	TEST(creation_error);
      }
      qValidData = EntMesh.getNextEntIter(oIter, lastHandle);
      bool creation_test_2 = (qValidData && lastHandle == new_vert);
      TEST(creation_test_2);
    } // Created and tested an EntMesh
    else {
      // No EntMesh cast possible
      bool not_an_Entity_Mesh = false;
      TEST(not_an_Entity_Mesh);
    }
  }
}

void vTestIterWithModify(const int iDim)
{
  vTestVertIterWithModify(iDim);
}
