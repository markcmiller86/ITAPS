//@ File header

#ifndef CREATE_ARG
#define CREATE_ARG ""
#endif

// This file is intended to test functionality of a TSTT implementation.
// Mesh query, modification, iterators, entity sets, and tags are all
// tested.  In cases where it's practical to define a priori a correct
// result for a query, that is done.  In other cases, consistency is the
// only thing checked.  In still others, the only possible correctness
// check is that the function took correct input and didn't choke
// (things like getAdjTable or getGeometricDim, for instance).

// It is worth noting that, although efforts have been made to make each
// test as independent as possible, it is unavoidable that a failure
// during one test may cause failures in subsequent tests, sometimes
// even across different interface tests.  In light of this, it is best
// to try to correct errors in the order they occur, or at least to
// exclude interface tests that cause errors to see if later errors are
// avoided.

// Correct argument handling, including both pre- and post-conditions,
// is checked elsewhere (TSTTM_precondTest.cc and
// TSTTM_postcondTest.cc).

#define MAIN_PROGRAM
#include "TSTTM_unitTest.hh"

static void vSetupTypeAndTopo()
{
  // Which combinations are okay doesn't depend on whether the mesh
  // database is 2D, 3D, etc.  Meshes are required to returned nothing
  // if they don't have something, that's all.

  int eEType;
  int eETopo;

  //@@ Setup data
  for (eETopo = TSTTM::EntityTopology_POINT;
       eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
       eETopo++) {
    for (eEType = TSTTM::EntityType_VERTEX;
	 eEType < TSTTM::EntityType_ALL_TYPES;
	 eEType++) {
      a2qLegalTypeAndTopo[eEType][eETopo] = false;
    }
    a2qLegalTypeAndTopo[TSTTM::EntityType_ALL_TYPES][eETopo] = true;
  }

  a2qLegalTypeAndTopo[TSTTM::EntityType_VERTEX]
    [TSTTM::EntityTopology_POINT] = true;

  a2qLegalTypeAndTopo[TSTTM::EntityType_EDGE]
    [TSTTM::EntityTopology_LINE_SEGMENT] = true;

  a2qLegalTypeAndTopo[TSTTM::EntityType_FACE]
    [TSTTM::EntityTopology_TRIANGLE] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_FACE]
    [TSTTM::EntityTopology_QUADRILATERAL] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_FACE]
    [TSTTM::EntityTopology_POLYGON] = true;

  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_TETRAHEDRON] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_PYRAMID] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_PRISM] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_HEXAHEDRON] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_SEPTAHEDRON] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_POLYHEDRON] = true;

  a2qLegalTypeAndTopo[TSTTM::EntityType_VERTEX]
    [TSTTM::EntityTopology_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_EDGE]
    [TSTTM::EntityTopology_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_FACE]
    [TSTTM::EntityTopology_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_REGION]
    [TSTTM::EntityTopology_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[TSTTM::EntityType_ALL_TYPES]
    [TSTTM::EntityTopology_ALL_TOPOLOGIES] = true;
}

//@ Adjacency reciprocity test kernel

void vEntityReciprocityTest(TSTTM::Entity& EntMesh,
			    TSTTM::EntityType eET1, TSTTM::EntityType eET2)
{
  // Iterate over entities of type eET1
  void* entIter;
  EntMesh.initEntIter(pvRootSet, eET1,
		      TSTTM::EntityTopology_ALL_TOPOLOGIES, entIter);
  bool qMoreData, qOK = true;
  do {
    void* entity;
    qMoreData = EntMesh.getNextEntIter(entIter, entity);
    // Grab the entities of type eET2 adjacent to entity.
    sidl::array<void*> saAdjEnt1;
    int iNAdjEnt1;
    EntMesh.getEntAdj(entity, eET2, saAdjEnt1, iNAdjEnt1);
    // Loop over those entities
    int iAdj1Lo = saAdjEnt1.lower(0);
    for (int iAdj1 = iAdj1Lo; iAdj1 < iAdj1Lo + iNAdjEnt1 && qOK; iAdj1++) {
      void* adj_entity = saAdjEnt1.get(iAdj1);
      // Now get their adjacencies.
      sidl::array<void*> saAdjEnt2;
      int iNAdjEnt2;
      EntMesh.getEntAdj(adj_entity, eET1, saAdjEnt2, iNAdjEnt2);
      // Loop over -those- to make sure the original entity is one of
      // them.
      int iAdj2Lo = saAdjEnt2.lower(0);
      qOK = false;
      for (int iAdj2 = iAdj2Lo; iAdj2 < iAdj2Lo + iNAdjEnt2; iAdj2++) {
	void* adj_ent2 = saAdjEnt2.get(iAdj2);
	if (adj_ent2 == entity) {
	  qOK = true;
	  break;
	}
      } // Loop over entities of type eET1 adj to eET2's adj to the
	// original ent.
    } // Loop over entities of type eET2 adj to original ent
  } while (qMoreData && qOK); // Loop over entities of type eET1.
  EntMesh.endEntIter(entIter);
  if (qOK) {
    cout << " passed";
    TEST(true);
  }
  else {
    cout << " failed";
    TEST(false);
  }
} // Done with entity reciprocity test

void vArrayReciprocityTest(TSTTM::Arr& ArrMesh,
		      TSTTM::EntityType eET1, TSTTM::EntityType eET2)
{
  try {
    // First, get all eET1 -> eET2 adjacency info.
    ::sidl::array<void*> saHandles1, saHandles2, saAdjHandles1;
    ::sidl::array<int32_t> saiFirstOffset, saiSecondOffset;
    int iNumEntities;
    ArrMesh.getEntities(pvRootSet, eET1, TSTTM::EntityTopology_ALL_TOPOLOGIES,
			saHandles1, iNumEntities);
    int iNum1st, iOffset1st, iNum2nd, iOffset2nd;
    ArrMesh.getEntArrAdj(saHandles1, iNumEntities, eET2,
			 saHandles2, iNum1st, saiFirstOffset, iOffset1st);
    ArrMesh.getEntArrAdj(saHandles2, iNum1st, eET1,
			 saAdjHandles1, iNum2nd, saiSecondOffset, iOffset2nd);
    
    int iEH1Lo = saHandles1.lower(0);
    int iEH1Hi = iEH1Lo + iNumEntities - 1;
    int iEH2Lo = saHandles2.lower(0);
    int iAEHLo = saAdjHandles1.lower(0);
    int iO1Lo = saiFirstOffset.lower(0);
    int iO2Lo = saiSecondOffset.lower(0);
    bool qOK = (iNumEntities == iOffset1st - 1) && (iNum1st == iOffset2nd - 1)
      && (iNum1st == saiFirstOffset.get(iO1Lo + iNumEntities))
      && (iNum2nd == saiSecondOffset.get(iO2Lo + iOffset2nd - 1))
      && (saiFirstOffset.get(iO1Lo) == 0)
      && (saiSecondOffset.get(iO2Lo) == 0);
    
    // For every entity, check that every adjacent entity also thinks
    // the first entity is adjacent to it.
    int iEH1, iEH2;
    for (iEH1 = iEH1Lo; qOK && iEH1 <= iEH1Hi; iEH1++) {
      void * Handle1 = saHandles1.get(iEH1);
      int iBegin = saiFirstOffset.get(iEH1) + iEH2Lo;
      int iEnd = saiFirstOffset.get(iEH1+1) + iEH2Lo - 1;
      for (iEH2 = iBegin; iEH2 <= iEnd; iEH2++) {
	bool qFoundIt = false;
	int iBegin2 = saiSecondOffset.get(iEH2) + iAEHLo;
	int iEnd2 = saiSecondOffset.get(iEH2+1) + iAEHLo - 1;
	for (int iAEH = iBegin2; !qFoundIt && iAEH <= iEnd2; iAEH++) {
	  void * AdjHandle = saAdjHandles1.get(iAEH);
	  if (Handle1 == AdjHandle) qFoundIt = true;
	}
	qOK = qFoundIt;
      } // Done with inner loop
    } // Done with outer loop
    if (qOK) {
      cout << " passed";
      TEST(true);
    }
    else {
      cout << " failed";
      TEST(false);
    }
  } SHOULD_NOT_THROW;
} // Done with array reciprocity test

//@@ Test the core Mesh interface.

// Test stuff specific to interface Mesh.  Those functions are:
//    load, save, getRootSet, getGeometricDim, getDfltStorage,
//    getAdjTable, getNumOfType, getNumOfTopo, getAllVtxCoords,
//    getVtxCoordIndex, getEntities, getVtxArrCoords, and
//    getAdjEntities


static void vTestMeshInterface(TSTTM::Mesh& Mesh,
			       string strFilename)
{
  try {
    pvRootSet = Mesh.getRootSet();
  } SHOULD_NOT_THROW;

  try {
    Mesh.load(pvRootSet, strFilename);
  } catch (TSTTB::Error TErr) {
    TErr.echo(" Couldn't load mesh data!");
    // Better crash out; nothing will work from here.
    throw;
  }

  try {
    Mesh.save(pvRootSet, SAVE_NAME);
  } catch (TSTTB::Error TErr) {
    TErr.echo(" Couldn't save mesh data!");
  }

  try {
    pvRootSet = Mesh.getRootSet();
  } SHOULD_NOT_THROW;

  // Verify that the implementation returns a dimension.
  cout << " Getting geometric dimension..." << endl;
  int iGeomDim;
  try {
    iGeomDim = Mesh.getGeometricDim();
    TEST(iGeomDim > 0);
  } catch (TSTTB::Error& TErr) {
    TErr.echo("unitTest hit an unknown error! ");
    TEST(false);
  }

  // Verify that the implementation returns a storage order preference.
  cout << "Getting default storage order..." << endl;
  TSTTM::StorageOrder SODef;
  try {
    SODef = Mesh.getDfltStorage();
    TEST(SODef == TSTTM::StorageOrder_BLOCKED ||
	 SODef == TSTTM::StorageOrder_INTERLEAVED);
  }
  catch (TSTTB::Error TErr) {
    TErr.echo("Implementation must specify a storage order.");
    TEST(false);
  }

  // Verify that the implementation returns a valid adjacency table.
  cout << " Getting adjacency table." << endl;
  ::sidl::array<TSTTM::AdjacencyInfo> saAdjTable;
  try {
    int iSize;
    Mesh.getAdjTable(saAdjTable, iSize);
    TEST(iSize == 16);
    TEST(saAdjTable.dimen() == 1);
    int iLo = saAdjTable.lower(0);
    a2iAdjTable[0][0] = saAdjTable.get(iLo + 0);
    a2iAdjTable[0][1] = saAdjTable.get(iLo + 1);
    a2iAdjTable[0][2] = saAdjTable.get(iLo + 2);
    a2iAdjTable[0][3] = saAdjTable.get(iLo + 3);

    a2iAdjTable[1][0] = saAdjTable.get(iLo + 4);
    a2iAdjTable[1][1] = saAdjTable.get(iLo + 5);
    a2iAdjTable[1][2] = saAdjTable.get(iLo + 6);
    a2iAdjTable[1][3] = saAdjTable.get(iLo + 7);

    a2iAdjTable[2][0] = saAdjTable.get(iLo + 8);
    a2iAdjTable[2][1] = saAdjTable.get(iLo + 9);
    a2iAdjTable[2][2] = saAdjTable.get(iLo + 10);
    a2iAdjTable[2][3] = saAdjTable.get(iLo + 11);

    a2iAdjTable[3][0] = saAdjTable.get(iLo + 12);
    a2iAdjTable[3][1] = saAdjTable.get(iLo + 13);
    a2iAdjTable[3][2] = saAdjTable.get(iLo + 14);
    a2iAdjTable[3][3] = saAdjTable.get(iLo + 15);
  }
  catch (TSTTB::Error TErr) {
    TErr.echo("Couldn't get adjacency table. ");
    TEST(false);
  }

  //@@ getEntities; entity type, topo
  bool qOK;
  int eEType;
  int eETopo;

  //@@@ Setup of legal argument combinations and expected return sizes.
  for (eETopo = TSTTM::EntityTopology_POINT;
       eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
       eETopo++) {
    for (eEType = TSTTM::EntityType_VERTEX;
	 eEType <= TSTTM::EntityType_ALL_TYPES;
	 eEType++) {
      a2iEntCount[eEType][eETopo] = 0;
    }
  }

  a2iEntCount[TSTTM::EntityType_VERTEX][TSTTM::EntityTopology_POINT] =
    a2iEntCount[TSTTM::EntityType_VERTEX]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES] =
    a2iEntCount[TSTTM::EntityType_ALL_TYPES][TSTTM::EntityTopology_POINT] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_POINT);

  a2iEntCount[TSTTM::EntityType_EDGE][TSTTM::EntityTopology_LINE_SEGMENT] =
    a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_LINE_SEGMENT] =
    a2iEntCount[TSTTM::EntityType_EDGE]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_LINE_SEGMENT);

  a2iEntCount[TSTTM::EntityType_FACE][TSTTM::EntityTopology_TRIANGLE] =
    a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_TRIANGLE] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_TRIANGLE);

  a2iEntCount[TSTTM::EntityType_FACE][TSTTM::EntityTopology_QUADRILATERAL] =
    a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_QUADRILATERAL] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_QUADRILATERAL);

  a2iEntCount[TSTTM::EntityType_ALL_TYPES][TSTTM::EntityTopology_POLYGON] =
    a2iEntCount[TSTTM::EntityType_FACE]
      [TSTTM::EntityTopology_POLYGON] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_POLYGON);

  a2iEntCount[TSTTM::EntityType_FACE][TSTTM::EntityTopology_ALL_TOPOLOGIES] =
    a2iEntCount[TSTTM::EntityType_FACE]
      [TSTTM::EntityTopology_TRIANGLE] +
    a2iEntCount[TSTTM::EntityType_FACE]
      [TSTTM::EntityTopology_QUADRILATERAL] +
    a2iEntCount[TSTTM::EntityType_FACE]
      [TSTTM::EntityTopology_POLYGON];

  a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_TETRAHEDRON] =
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_TETRAHEDRON] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_TETRAHEDRON);

  a2iEntCount[TSTTM::EntityType_ALL_TYPES][TSTTM::EntityTopology_PYRAMID] =
    a2iEntCount[TSTTM::EntityType_REGION][TSTTM::EntityTopology_PYRAMID] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_PYRAMID);

  a2iEntCount[TSTTM::EntityType_ALL_TYPES][TSTTM::EntityTopology_PRISM] =
    a2iEntCount[TSTTM::EntityType_REGION][TSTTM::EntityTopology_PRISM] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_PRISM);

  a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_HEXAHEDRON] =
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_HEXAHEDRON] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_HEXAHEDRON);

  a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_SEPTAHEDRON] =
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_SEPTAHEDRON] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_SEPTAHEDRON);

  a2iEntCount[TSTTM::EntityType_ALL_TYPES]
      [TSTTM::EntityTopology_POLYHEDRON] =
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_POLYHEDRON] =
    Mesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_POLYHEDRON);

  a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES] =
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_TETRAHEDRON] +
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_PYRAMID] +
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_PRISM] +
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_HEXAHEDRON] +
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_SEPTAHEDRON] +
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_POLYHEDRON];

  a2iEntCount[TSTTM::EntityType_ALL_TYPES]
    [TSTTM::EntityTopology_ALL_TOPOLOGIES] =
    a2iEntCount[TSTTM::EntityType_VERTEX]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES] +
    a2iEntCount[TSTTM::EntityType_EDGE]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES] +
    a2iEntCount[TSTTM::EntityType_FACE]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES] +
    a2iEntCount[TSTTM::EntityType_REGION]
      [TSTTM::EntityTopology_ALL_TOPOLOGIES];

  //@@@ Test loop
  cout << " Global entity query tests" << endl;
  for (eEType = TSTTM::EntityType_VERTEX;
       eEType <= TSTTM::EntityType_ALL_TYPES;
       eEType++) {
    for (eETopo = TSTTM::EntityTopology_POINT;
	 eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
	 eETopo++) {
      try {
	cout << "  Type: ";
	cout.width(9);
	cout << astrTypes[eEType] << " Topology: ";
	cout.width(14);
	cout << astrTopologies[eETopo];
	//@@@@ Get entities, type, topo, dim
	::sidl::array<void*> saEntHandles;
	int iNumEnt;
	Mesh.getEntities(pvRootSet, static_cast<TSTTM::EntityType>(eEType),
			 static_cast<TSTTM::EntityTopology>(eETopo),
			 saEntHandles, iNumEnt);
	qOK = (iNumEnt == a2iEntCount[eEType][eETopo]);
	cout << " OK" << endl;
      }
      catch (TSTTB::Error TErr) {
	switch (TErr.getErrorType()) {
	case TSTTB::ErrorType_BAD_TYPE_AND_TOPO:
	  if (a2qLegalTypeAndTopo[eEType][eETopo]) {
	    // This should only occur if a precondition test failed.
	    cout << endl;
	    TErr.echo("Threw instead of returning! ");
	    TEST(false);
	  }
	  else {
	    cout << " threw an error, as expected." << endl;
	    iNPassed++;
	  }
	  break;
	case TSTTB::ErrorType_NOT_SUPPORTED:
	  // This should never occur, according to the spec.
	  cout << "; feasible though unsupported combination" << endl;
	  iNNotImpl++;
	  break;
	default:
	  cout << endl;
	  TErr.echo("unitTest hit an unknown error! ");
	  TEST(false);
	}
      }
    }
  } // Done with double loop to verify consistency of number of entities
    // fetched and number of entities that the mesh claims to have.


  //@@ getVtxArrCoords should insist on vertex handles
  cout << " Verifying that getVtxArrCoords requires vertex handles as input..."
       << endl;
  try {
    ::sidl::array<void*> saEntHandles;
    int iNumEnt;
    Mesh.getEntities(pvRootSet, TSTTM::EntityType_ALL_TYPES,
		     TSTTM::EntityTopology_ALL_TOPOLOGIES,
		     saEntHandles, iNumEnt);
    ::sidl::array<double> sadCoords;
    TSTTM::StorageOrder SO = TSTTM::StorageOrder_UNDETERMINED;
    int iCSize;
    Mesh.getVtxArrCoords(saEntHandles, iNumEnt, SO, sadCoords, iCSize);
    // Should only get here if all the entities in the entire mesh
    // happened to be vertices.
    int iNumVerts = Mesh.getNumOfType(pvRootSet, TSTTM::EntityType_VERTEX);
    TEST(iNumVerts == iNumEnt);
    TEST(iCSize == iNumVerts * Mesh.getGeometricDim());
  } catch (TSTTB::Error TErr) {
    // Should have thrown InvalidEntityType.
    TEST(TErr.getErrorType() == TSTTB::ErrorType_INVALID_ENTITY_TYPE);
  }

  //@@ Coordinate tests
  //@@@ Two approaches, default order, coords should match
  cout << " Checking vertex coordinate retrieval consistency "
       << "(default order) ..." << endl;
  ::sidl::array<void*> saVertHandles;
  int iNV;
  Mesh.getEntities(pvRootSet, TSTTM::EntityType_VERTEX,
		   TSTTM::EntityTopology_POINT, saVertHandles, iNV);
  int iLoH = saVertHandles.lower(0);

  ::sidl::array<double> sadCoords1, sadCoords2;
  ::sidl::array<int32_t> saiInEntitySet;
  TSTTM::StorageOrder SO1 = TSTTM::StorageOrder_UNDETERMINED;
  TSTTM::StorageOrder SO2 = TSTTM::StorageOrder_UNDETERMINED;

  int iCSize1, iCSize2, iIES2;
  try {
    Mesh.getVtxArrCoords(saVertHandles, iNV, SO1, sadCoords1, iCSize1);
    Mesh.getAllVtxCoords(pvRootSet, sadCoords2, iCSize2, saiInEntitySet, iIES2, SO2);
  } SHOULD_NOT_THROW_CRITICAL;

  int iLo1 = sadCoords1.lower(0);
  int iLo2 = sadCoords2.lower(0);
  int iLoEnt = saiInEntitySet.lower(0);

  qOK = ((SO1 == SO2) && (SO1 != TSTTM::StorageOrder_UNDETERMINED) &&
	      (iCSize1 == iCSize2) && (iCSize1 == iGeomDim*iNV));

  // Now let's get serious!  Are the coordinates the same?
  int i1 = iLo1, i2 = iLo2, iLast = iLo1 + iCSize1 - 1;
  for ( ; qOK && i1 <= iLast; i1++, i2++) {
    if (sadCoords1.get(i1) != sadCoords2.get(i2)) qOK = false;
  }
  assert(i1 == iLo1 + iCSize1);
  assert(i2 == iLo2 + iCSize2);
  int iIES = iLoEnt;
  for ( ; qOK && iIES < iLoEnt + iIES2; iIES++, iIES++) {
    if (saiInEntitySet.get(iIES) != 1) qOK = false;
  }
  TEST(qOK);

  //@@@ Using other order, check again with each call.
  // Now check the other ordering.
  cout << " Checking vertex coordinate retrieval consistency "
       << "(non-default order) ..." << endl;
  TSTTM::StorageOrder SO3 = TSTTM::StorageOrder_BLOCKED;
  if (SO2 == SO3) SO3 = TSTTM::StorageOrder_INTERLEAVED;
  ::sidl::array<double> sadCoords3, sadCoords4;
  int iCSize3, iCSize4, iIES4;
  Mesh.getVtxArrCoords(saVertHandles, iNV, SO3, sadCoords3, iCSize3);
  Mesh.getAllVtxCoords(pvRootSet, sadCoords4, iCSize4, saiInEntitySet, iIES4, SO3);

  iLoEnt = saiInEntitySet.lower(0);
  int iLo3 = sadCoords3.lower(0);
  int iLo4 = sadCoords4.lower(0);

  qOK = (SO3 != SO1) && (iCSize3 == iCSize2) && (iCSize4 == iCSize2) &&
    (iIES4*iGeomDim == iCSize4);
  TEST(qOK);
  int i;
  for (iIES = iLoEnt, i = iLoH; i <= iLoH + iNV - 1; i++, iIES++) {
    if (SO1 == TSTTM::StorageOrder_BLOCKED) {
      for (int j = 0; j < iGeomDim; j++) {
	if (sadCoords1.get(iLo1 + (i - iLoH) +    iNV*j) !=
	    sadCoords3.get(iLo3 + (i - iLoH)*iGeomDim + j) ||
	    sadCoords1.get(iLo1 + (i - iLoH) +    iNV*j) !=
	    sadCoords4.get(iLo4 + (i - iLoH)*iGeomDim + j))
	  qOK = false;
      }
    }
    else {
      for (int j = 0; j < iGeomDim; j++) {
	if (sadCoords3.get(iLo3 + (i - iLoH) +    iNV*j) !=
	    sadCoords1.get(iLo1 + (i - iLoH)*iGeomDim + j) ||
	    sadCoords4.get(iLo4 + (i - iLoH) +    iNV*j) !=
	    sadCoords1.get(iLo1 + (i - iLoH)*iGeomDim + j))
	  qOK = false;
      }
    }
    if (saiInEntitySet.get(iIES) != 1) qOK = false;
  }
  TEST(qOK);

  // Now check the consistency of getAdjEntities and getVtxCoordIndex.
  // Basically, get verts adjacent to entities and vertex indices
  // for all entities, and make sure the coords for the verts match up
  // in the two instances.
  {
    cout << " Checking that getAdjEntities and getVtxCoordIndex "
	 << "give consistent results for adjacent vertices." << endl;
    sidl::array<void*> saHVert;
    sidl::array<int> saiOffset, saiInSet1, saiInSet2, saiOffset2, saiIndex;
    sidl::array<double> sadCoords1, sadCoords2;
    sidl::array<TSTTM::EntityTopology> saEntTopo;
    int iVertSize, iOffsetSize, iInSet1Size, iCoords1Size, iCoords2Size;
    int iInSet2Size, iOffset2Size, iIndexSize, iEntTopoSize;
    TSTTM::StorageOrder SO1, SO2;
    SO1 = SO2 = TSTTM::StorageOrder_INTERLEAVED;
    try {
      Mesh.getAdjEntities(pvRootSet, TSTTM::EntityType_ALL_TYPES,
			  TSTTM::EntityTopology_ALL_TOPOLOGIES,
			  TSTTM::EntityType_VERTEX,
			  saHVert, iVertSize,
			  saiOffset, iOffsetSize,
			  saiInSet1, iInSet1Size);
      Mesh.getVtxArrCoords(saHVert, iVertSize, SO1,
			   sadCoords1, iCoords1Size);

      Mesh.getAllVtxCoords(pvRootSet, sadCoords2, iCoords2Size,
			   saiInSet2, iInSet2Size, SO2);
      Mesh.getVtxCoordIndex(pvRootSet, TSTTM::EntityType_ALL_TYPES,
			    TSTTM::EntityTopology_ALL_TOPOLOGIES,
			    TSTTM::EntityType_VERTEX,
			    saiOffset2, iOffset2Size,
			    saiIndex, iIndexSize,
			    saEntTopo, iEntTopoSize);
    } SHOULD_NOT_THROW;
    // All args are valid, so none of the calls should throw.

    // Now check the results.  Basically, the array returned from
    // getVtxArrCoords should match the array you'd get by stringing
    // together coordinates from getAllVtxCoords in the order defined by
    // getVtxCoordIndex.  So here goes:
    int iIndexLo = saiIndex.lower(0);
    int iCoords1Lo = sadCoords1.lower(0);
    int iCoords2Lo = sadCoords2.lower(0);

    int iInd, iC1;
    qOK = true;
    for (iInd = iIndexLo, iC1 = iCoords1Lo;
	 iInd <= iIndexLo + iIndexSize - 1;
	 iInd++, iC1+=iGeomDim) {
      int iIndex = saiIndex.get(iInd);
      int iC2 = iCoords2Lo + iIndex*iGeomDim;
      qOK = (qOK &&
	     sadCoords1.get(iC1) == sadCoords2.get(iC2) &&
	     sadCoords1.get(iC1+1) == sadCoords2.get(iC2+1));
      if (iGeomDim == 3) {
	qOK = (qOK &&
	       sadCoords1.get(iC1+2) == sadCoords2.get(iC2+2));
      }
    } // Done checking all coords
    TEST(qOK);
    // Much more thorough testing of adjacency will be done elsewhere,
    // especially confirming that adjacency info is reciprocal.
  }
}

// Test stuff specific to interface Entity.  Those functions are:
//    initEntIter, getNextEntIter, resetEntIter, endEntIter,
//    getEntTopo, getEntType, getVtxCoord, getEntAdj

static void vTestEntityInterface(TSTTM::Entity& EntMesh)
{
  // getEntTopo, getEntType, getVtxCoord, getEntAdj can all be tested
  // via consistency with the global calls.  For instance, all entities
  // retrieved by the global getEntities(FACE, TRIANGLE) call should be
  // of type face and topology triangle.

  bool qOK;
  //@@@ Test loop for getEntTopo and getEntType.
  cout << " Verifying retrieved entity type and topology" << endl;
  int eEType, eETopo;
  for (eEType = TSTTM::EntityType_VERTEX;
       eEType <= TSTTM::EntityType_ALL_TYPES;
       eEType++) {
    for (eETopo = TSTTM::EntityTopology_POINT;
	 eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
	 eETopo++) {
      qOK = true;
      if (a2qLegalTypeAndTopo[eEType][eETopo]) {
	cout << "  Type: ";
	cout.width(9);
	cout << astrTypes[eEType] << " Topology: ";
	cout.width(14);
	cout << astrTopologies[eETopo] << endl;
	::sidl::array<void*> saEntHandles;
	int iNumEnt;
	// This call must succeed, or the Mesh tests wouldn't all pass.
	EntMesh.getEntities(pvRootSet, static_cast<TSTTM::EntityType>(eEType),
			    static_cast<TSTTM::EntityTopology>(eETopo),
			    saEntHandles, iNumEnt);
	int iEntLo = saEntHandles.lower(0);
	for (int iEnt = iEntLo; iEnt <= iEntLo + iNumEnt - 1; iEnt++) {
	  void * pEnt = saEntHandles.get(iEnt);
	  TSTTM::EntityType ETy = EntMesh.getEntType(pEnt);
	  TSTTM::EntityTopology ETo = EntMesh.getEntTopo(pEnt);
	  qOK = qOK &&
	    ((ETy == eEType || eEType == TSTTM::EntityType_ALL_TYPES)
	     &&
	     (ETo == eETopo ||
	      eETopo == TSTTM::EntityTopology_ALL_TOPOLOGIES));
	}
	TEST(qOK);
      }
    }
  } // Done with double loop to verify consistency of entity type and
    // topology.

  // Now test getVtxCoord for consistency with the getEntities(VERTEX)
  // -> getVtxArrCoord chain.
  cout << " Checking single vertex coordinate retrieval" << endl;
  {
    sidl::array<void*> saVtxHandles;
    sidl::array<double> sadAllCoords;
    double adCoords[3];

    int iGeomDim = EntMesh.getGeometricDim();

    // Get all the vertex coordinates.
    int iNVerts, iNCoords;
    EntMesh.getEntities(pvRootSet, TSTTM::EntityType_VERTEX,
			TSTTM::EntityTopology_POINT, saVtxHandles, iNVerts);
    TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
    EntMesh.getVtxArrCoords(saVtxHandles, iNVerts, SO, sadAllCoords, iNCoords);
    assert(iNCoords == iGeomDim*iNVerts);

    // Now get them vertex by vertex and compare.
    int iCLo = sadAllCoords.lower(0);
    int iVLo = saVtxHandles.lower(0);
    qOK = true;
    for (int iV = iVLo, iC = iCLo; iV < iVLo + iNVerts; iV++) {
      void* vert_handle = saVtxHandles.get(iV);
      EntMesh.getVtxCoord(vert_handle, adCoords[0], adCoords[1], adCoords[2]);
      if (iGeomDim == 2)
	qOK = qOK && (adCoords[2] == 0);
      for (int ii = 0; ii < iGeomDim; iC++,ii++) {
	qOK = qOK && (sadAllCoords.get(iC) == adCoords[ii]);
      }
    }
    TEST(qOK);
  }

  //@@ Test single entity adjacency call for consistency with the global
  // call.

  //@@@ Test loop
  cout << " Checking single entity adjacency calls" << endl;
  for (eEType = TSTTM::EntityType_VERTEX;
       eEType < TSTTM::EntityType_ALL_TYPES;
       eEType++) {
    for (eETopo = TSTTM::EntityTopology_POINT;
	 eETopo < TSTTM::EntityTopology_ALL_TOPOLOGIES;
	 eETopo++) {
      if (a2qLegalTypeAndTopo[eEType][eETopo]) {
	qOK = true;
	cout << "  Type: ";
	cout.width(9);
	cout << astrTypes[eEType] << " Topology: ";
	cout.width(14);
	cout << astrTopologies[eETopo] << endl;
	// Get entities
	::sidl::array<void*> saEntHandles;
	int iNumEnt;
	EntMesh.getEntities(pvRootSet, static_cast<TSTTM::EntityType>(eEType),
			    static_cast<TSTTM::EntityTopology>(eETopo),
			    saEntHandles, iNumEnt);
	int iEntLo = saEntHandles.lower(0);

	// Now test for equivalence in adjacency operations.
	int eAdjType, eLastType = TSTTM::EntityType_REGION;
	for (eAdjType = TSTTM::EntityType_VERTEX;
	     eAdjType <= eLastType;
	     eAdjType++) {
	  // Skip any that are identities or unsupported.
	  if (eAdjType == eEType ||
	      (a2iAdjTable[eEType][eAdjType] ==
	       TSTTM::AdjacencyInfo_UNAVAILABLE))
	    continue;
	  cout << "   Adjacent entity type:  ";
	  cout.width(9);
	  cout << astrTypes[eAdjType] << endl;

	  sidl::array<void*> saAdjEnt;
	  sidl::array<int> saiOffset, saiInSet;
	  int iNumAdj, iOffsetSize, iInSetSize;
	  // Get adjacencies using the entityset call.

	  EntMesh.getAdjEntities
	    (pvRootSet, static_cast<TSTTM::EntityType>(eEType),
	     static_cast<TSTTM::EntityTopology>(eETopo),
	     static_cast<TSTTM::EntityType>(eAdjType),
	     saAdjEnt, iNumAdj,
	     saiOffset, iOffsetSize, saiInSet, iInSetSize);

	  // Verify that the size of the output arrays are correct.
	  int iAdjLo = saAdjEnt.lower(0);
	  int iOffLo = saiOffset.lower(0);
	  int iIESLo = saiInSet.lower(0);

	  qOK = (iNumAdj == iInSetSize) && (iNumEnt == iOffsetSize - 1);

	  // Verify that the contents are the same, and that everything
	  // is labeled as being in the entity set.
	  int iEnt, iOff;
	  for (iEnt = iEntLo, iOff = iOffLo;
	       iEnt < iEntLo + iNumEnt - 1 && qOK;
	       iEnt++, iOff++) {
	    void * handle = saEntHandles.get(iEnt);
	    int iNLocalAdj;
	    sidl::array<void*> saAdjEntHandles;
	    EntMesh.getEntAdj(handle,
			      static_cast<TSTTM::EntityType>(eAdjType),
			      saAdjEntHandles, iNLocalAdj);
	    // Number of adjacencies must match.
	    qOK = (iNLocalAdj ==
		   (saiOffset.get(iOff+1) - saiOffset.get(iOff)));
	    // Now check that (a) the adjacencies match and (b) all the
	    // entities are listed as in the entity set.
	    int iOffset = saiOffset.get(iOff);
	    int iLocalLo = saAdjEntHandles.lower(0);
	    for (int iAdj = 0; iAdj < iNLocalAdj; iAdj++) {
	      void * global_handle = saAdjEnt.get(iAdjLo + iOffset + iAdj);
	      void * local_handle = saAdjEntHandles.get(iLocalLo + iAdj);
	      qOK = qOK && (global_handle == local_handle) &&
		(saiInSet.get(iIESLo + iOffset + iAdj) == 1);
	    } // Loop over local adjacencies
	  } // Loop over entities
	} // Loop over adjacency types
	TEST(qOK);
      }
    }
  } // Done with double loop to verify consistency of adjacencies

  // Iterators should be tested by iterating over every legal type and
  // topology combination to be sure that all the entities retrieved by
  // the global call are traversed by the iterator.  To make sure reset
  // works properly, run through each collection twice.
  cout << " Testing single entity iterators" << endl;
  for (eEType = TSTTM::EntityType_VERTEX;
       eEType <= TSTTM::EntityType_ALL_TYPES;
       eEType++) {
    for (eETopo = TSTTM::EntityTopology_POINT;
	 eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
	 eETopo++) {
      // First, grab everything and put it in a set.
      int iMaxCount = a2iEntCount[eEType][eETopo];
      void* aEnt[iMaxCount];
      sidl::array<void*> saoEnt;
      int iLo = 0, iHi = iMaxCount-1, iStride = 1;
      saoEnt.borrow(aEnt, 1, &iLo, &iHi, &iStride);
      int iActualCount;
      if (a2qLegalTypeAndTopo[eEType][eETopo]) {
	cout << "  Type: ";
	cout.width(9);
	cout << astrTypes[eEType] << " Topology: ";
	cout.width(14);
	cout << astrTopologies[eETopo] << endl;
	EntMesh.getEntities(pvRootSet,
			    static_cast<TSTTM::EntityType>(eEType),
			    static_cast<TSTTM::EntityTopology>(eETopo),
			    saoEnt, iActualCount);

	// Should see what happens with zero-sized iterators.
	// if (iActualCount == 0) continue;

	// Put all these entities into a big set.  This will need to be a
	// multiset when testing entity set queries, because the entity
	// set could have the same entity in it twice.
	std::set<void*> sEnt;
	sEnt.insert(aEnt, aEnt+iActualCount);
	std::set<void*> sEntCopy(sEnt);
	qOK = ((int)(sEnt.size()) == iActualCount);

	// Now iterate over these entities, and remove them one by one
	// from the set, hopefully succeeding every time.
	void* entIter;
	bool qMoreData = true;

	try {
	  EntMesh.initEntIter(pvRootSet,
			      static_cast<TSTTM::EntityType>(eEType),
			      static_cast<TSTTM::EntityTopology>(eETopo),
			      entIter);
	  int iMissedCount = 0;
	  int iExtraCount = 0;
	  do {
	    void* entHandle;
	    qMoreData = EntMesh.getNextEntIter(entIter, entHandle);
	    int iRes = sEnt.erase(entHandle);
	    if (iRes > 1) iExtraCount++;
	    else if (iRes == 0) iMissedCount++;
	  } while (qMoreData);
	  qOK = qOK && (iMissedCount == 0) && (iExtraCount == 0)
	    && (sEnt.size() == 0);

	  try {
	    EntMesh.resetEntIter(entIter);
	  } SHOULD_NOT_THROW;

	  // Now repeat to be sure the reset worked.
	  iMissedCount = 0;
	  iExtraCount = 0;
	  do {
	    void* entHandle;
	    qMoreData = EntMesh.getNextEntIter(entIter, entHandle);
	    int iRes = sEnt.erase(entHandle);
	    if (iRes > 1) iExtraCount++;
	    else if (iRes == 0) iMissedCount++;
	  } while (qMoreData);
	  qOK = qOK && (iMissedCount == 0) && (iExtraCount == 0)
	    && (sEnt.size() == 0);

	  try {
	    EntMesh.endEntIter(entIter);
	  } SHOULD_NOT_THROW;
	} catch (TSTTB::Error TErr) { // Big try block beginning with initIter
	  switch (TErr.getErrorType()) {
	  case TSTTB::ErrorType_FAILURE:
	    // Could mean there aren't any of these; check for this.
	    if (iMaxCount == 0)
	      iNPassed++;
	    else {
	      cout << "Max count should be zero; is " << iMaxCount << endl;
	      qAllPassed = false;
	      iNFailed++;
	    }
	    break;
	  case TSTTB::ErrorType_INVALID_ARGUMENT:
	    // Can happen for 2D meshes requested to iterate over
	    // regions.
	    if (EntMesh.getGeometricDim() == 2 &&
		((eEType == TSTTM::EntityType_REGION) ||
		 ((eEType == TSTTM::EntityType_ALL_TYPES) &&
		  ((eETopo == TSTTM::EntityTopology_POLYHEDRON) ||
		   (eETopo == TSTTM::EntityTopology_TETRAHEDRON) ||
		   (eETopo == TSTTM::EntityTopology_PYRAMID) ||
		   (eETopo == TSTTM::EntityTopology_PRISM) ||
		   (eETopo == TSTTM::EntityTopology_HEXAHEDRON) ||
		   (eETopo == TSTTM::EntityTopology_SEPTAHEDRON)))))
	      iNPassed++;
	    else {
	      cout << "Valid arguments incorrectly diagnosed." << endl;
	      qAllPassed = false;
	      iNFailed++;
	    }
	    break;
	  default:
	    TErr.echo("Unexpected error type!");
	    TEST(false);
	    break;
	  }
	} // Done with catching exceptions
      } // This was a legal type/topo combination
    }
  } // Done with double loop to test entity iterators.

  // Test reciprocity of entity adjacency calls.
  // Edge->vertex
  cout << " Testing single entity adjacency reciprocity" << endl;
  if (a2iAdjTable[1][0] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[0][1] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Edges and verts:";
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_EDGE,
			   TSTTM::EntityType_VERTEX);
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityType_EDGE);
    cout << endl;
  }

  // Face->vertex
  if (a2iAdjTable[2][0] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[0][2] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Faces and verts:";
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_FACE,
			   TSTTM::EntityType_VERTEX);
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityType_FACE);
    cout << endl;
  }

  // Region->vertex
  if (a2iAdjTable[3][0] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[0][3] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Regions and verts:";
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_REGION,
			   TSTTM::EntityType_VERTEX);
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityType_REGION);
    cout << endl;
  }

  // Face->edge
  if (a2iAdjTable[2][1] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[1][2] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Faces and edges:";
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_FACE,
			   TSTTM::EntityType_EDGE);
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_EDGE,
			   TSTTM::EntityType_FACE);
    cout << endl;
  }

  // Region->edge
  if (a2iAdjTable[3][1] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[1][3] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Regions and edges:";
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_REGION,
			   TSTTM::EntityType_EDGE);
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_EDGE,
			   TSTTM::EntityType_REGION);
    cout << endl;
  }

  // Region->face
  if (a2iAdjTable[3][2] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[2][3] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Regions and faces:";
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_REGION,
			   TSTTM::EntityType_FACE);
    vEntityReciprocityTest(EntMesh, TSTTM::EntityType_FACE,
			   TSTTM::EntityType_REGION);
    cout << endl;
  }

  cout << "Entity adjacency identity test..." << endl;
  //@@ Ensure that identity adjacency calls return nothing.
  // Test to be sure that no VERTEX has an adjacent VERTEX, etc.  The
  // TSTT data model specifies that entities of the same type are never
  // adjacent, but are connected instead through entities of different
  // type incident on both.
  for (int iType = 0; iType <= 3; iType++) {
    if (a2iAdjTable[iType][iType] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
      cout << "  Type: ";
      cout.width(9);
      cout << astrTypes[iType] << endl;
      TSTTM::EntityType eType = static_cast<TSTTM::EntityType>(iType);

      ::sidl::array<void*> saEntHand;
      int iNumEnt;
      EntMesh.getEntities(pvRootSet, eType,
			  TSTTM::EntityTopology_ALL_TOPOLOGIES,
			  saEntHand, iNumEnt);

      ::sidl::array<void*> saAdjHand;
      int iNAdj, iBase = saEntHand.lower(0);
      qOK = true;
      for (int i = 0; i < iNumEnt; i++) {
	void * entity = saEntHand.get(iBase + i);
	EntMesh.getEntAdj(entity, eType, saAdjHand, iNAdj);
	qOK = qOK && (iNAdj == 0);
      }
      TEST(qOK);
    }
  }
}

static void vTestArrInterface(TSTTM::Arr& ArrMesh) {
  bool qOK;
  int eEType;
  int eETopo;

  //@@@ Test loop
  cout << " Verifying retrieved entity type and topology (array version)"
       << endl << "    and global entity adjacency"
       << endl;
  for (eEType = TSTTM::EntityType_VERTEX;
       eEType <= TSTTM::EntityType_ALL_TYPES;
       eEType++) {
    for (eETopo = TSTTM::EntityTopology_POINT;
	 eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
	 eETopo++) {
      if (a2qLegalTypeAndTopo[eEType][eETopo]) {
	cout << "  Type: ";
	cout.width(9);
	cout << astrTypes[eEType] << " Topology: ";
	cout.width(14);
	cout << astrTopologies[eETopo] << endl;
	//@@@@ Get entities, type, topo
	::sidl::array<void*> saEntHandles;
	int iNumEnt;
	ArrMesh.getEntities(pvRootSet, static_cast<TSTTM::EntityType>(eEType),
			 static_cast<TSTTM::EntityTopology>(eETopo),
			 saEntHandles, iNumEnt);
	::sidl::array<TSTTM::EntityType> saEType;
	::sidl::array<TSTTM::EntityTopology> saETopo;

	int topo_size, type_size;
	ArrMesh.getEntArrTopo(saEntHandles, iNumEnt, saETopo, topo_size);
	ArrMesh.getEntArrType(saEntHandles, iNumEnt, saEType, type_size);

	int iTopoLo = saETopo.lower(0);
	int iTypeLo = saEType.lower(0);

	qOK = (topo_size == iNumEnt) && (type_size == iNumEnt);

	int iTopo, iType;
	for (iTopo = iTopoLo, iType = iTypeLo;
	     iTopo < iTopoLo + topo_size;
	     iTopo++, iType++) {
	  TSTTM::EntityTopology eTo = saETopo.get(iType);
	  TSTTM::EntityType eTy = saEType.get(iType);
	  qOK = qOK && ((eTo == eETopo ||
			 eETopo == TSTTM::EntityTopology_ALL_TOPOLOGIES) &&
			(eTy == eEType ||
			 eEType == TSTTM::EntityType_ALL_TYPES));
	}
	assert(iTopo <= iTopoLo + topo_size);
	assert(iType <= iTypeLo + type_size);
	if (!qOK) {
	  cout << endl << "Inconsistency in entity type or topology.\n\n";
	  qAllPassed = false;
	  iNFailed++;
	}

	TEST(qOK);
	if (eEType == TSTTM::EntityType_ALL_TYPES)
	  continue;

	qOK = true;
	//@@@@ Equivalence tests for entityGetAdj and entitysetGetAdj
	// Now test for equivalence in adjacency operations.
	int eAdjType, eLastType = TSTTM::EntityType_REGION;
	for (eAdjType = TSTTM::EntityType_VERTEX;
	     eAdjType <= eLastType;
	     eAdjType++) {
	  // Skip any that are identities or unsupported.
	  if (eAdjType == eEType || a2iAdjTable[eEType][eAdjType] == 0)
	    continue;
	  cout << "   Adjacent type: ";
	  cout.width(9);
	  cout << astrTypes[eAdjType] << endl;

	  ::sidl::array<int32_t> saiOffset1, saiOffset2, saiInSet;
	  ::sidl::array<void*> saAdjEnt1, saAdjEnt2;
	  // Get adjacencies using entityGetAdjacencies.
	  int iNumAdj1, iOffset1Size;
	  ArrMesh.getEntArrAdj(saEntHandles, iNumEnt,
			    static_cast<TSTTM::EntityType>(eAdjType),
			    saAdjEnt1, iNumAdj1, saiOffset1, iOffset1Size);

	  // Get adjacencies using the entityset call.
	  int iNumAdj2, iOffset2Size, iInSetSize;
	  ArrMesh.getAdjEntities
	    (pvRootSet, static_cast<TSTTM::EntityType>(eEType),
	     static_cast<TSTTM::EntityTopology>(eETopo),
	     static_cast<TSTTM::EntityType>(eAdjType),
	     saAdjEnt2, iNumAdj2,
	     saiOffset2, iOffset2Size, saiInSet, iInSetSize);

	  // Verify that the size of the output arrays are correct.
	  int iAdj1Lo = saAdjEnt1.lower(0);
	  int iAdj2Lo = saAdjEnt2.lower(0);
	  int iOff1Lo = saiOffset1.lower(0);
	  int iOff2Lo = saiOffset2.lower(0);
	  int iIESLo = saiInSet.lower(0);

	  qOK = (iNumAdj1 == iNumAdj2) && (iNumAdj2 == iInSetSize)
	    && (iNumEnt == iOffset1Size - 1) && (iNumEnt == iOffset2Size - 1);

	  // Verify that the contents are the same, and that everything
	  // is labeled as being in the entity set.
	  int iAdj1, iAdj2, iIES;
	  for (iAdj1 = iAdj1Lo, iAdj2 = iAdj2Lo, iIES = iIESLo;
	       iAdj1 < iAdj1Lo + iNumAdj1 && qOK;
	       iAdj1++, iAdj2++, iIES++) {
	    qOK = ((saAdjEnt1.get(iAdj1) == saAdjEnt2.get(iAdj2)) &&
		   (saiInSet.get(iIES) == 1));
	  }
	  assert(!qOK || iAdj1 == iAdj1Lo + iNumAdj1);
	  assert(!qOK || iAdj2 == iAdj2Lo + iNumAdj2);
	  assert(!qOK || iIES  == iIESLo + iInSetSize);

	  int iOff1, iOff2;
	  for (iOff1 = iOff1Lo, iOff2 = iOff2Lo;
	       iOff1 < iOff1Lo + iOffset1Size && qOK;
	       iOff1++, iOff2++) {
	    qOK = (saiOffset1.get(iOff1) == saiOffset2.get(iOff2));
	  }
	  assert(!qOK || iOff1 == iOff1Lo + iOffset1Size);
	  assert(!qOK || iOff2 == iOff2Lo + iOffset2Size);
	}
	TEST(qOK);
      }
    }
  } // Done with double loop to verify consistency of bulk adjacency calls.

  // Iterators should be tested by iterating over every legal type and
  // topology combination to be sure that all the entities retrieved by
  // the global call are traversed by the iterator.  To make sure reset
  // works properly, run through each collection twice.
  cout << " Testing array iterators" << endl;
  for (eEType = TSTTM::EntityType_VERTEX;
       eEType <= TSTTM::EntityType_ALL_TYPES;
       eEType++) {
    for (eETopo = TSTTM::EntityTopology_POINT;
	 eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES;
	 eETopo++) {
      cout << "  Type: ";
      cout.width(9);
      cout << astrTypes[eEType] << " Topology: ";
      cout.width(14);
      cout << astrTopologies[eETopo] << endl;
      // First, grab everything and put it in a set.
      int iMaxCount = ArrMesh.getNumOfTopo
	(pvRootSet, static_cast<TSTTM::EntityTopology>(eETopo));
      void* aEnt[iMaxCount];
      sidl::array<void*> saoEnt;
      int iLo = 0, iHi = iMaxCount-1, iStride = 1;
      saoEnt.borrow(aEnt, 1, &iLo, &iHi, &iStride);
      int iActualCount;
      if (a2qLegalTypeAndTopo[eEType][eETopo]) {
	ArrMesh.getEntities(pvRootSet,
			    static_cast<TSTTM::EntityType>(eEType),
			    static_cast<TSTTM::EntityTopology>(eETopo),
			    saoEnt, iActualCount);

	// Should see what happens with zero-sized iterators.
	// if (iActualCount == 0) continue;

	// Put all these entities into a big set.  This will need to be a
	// multiset when testing entity set queries, because the entity
	// set could have the same entity in it twice.
	std::set<void*> sEnt;
	sEnt.insert(aEnt, aEnt+iActualCount);
	std::set<void*> sEntCopy(sEnt);
	qOK = ((int)(sEnt.size()) == iActualCount);

	// Now iterate over these entities, and remove them one by one
	// from the set, hopefully succeeding every time.
	void* arrIter;
	void* aEntHandles[100];
	int iLo = 0, iHi = 99, iStride = 1;
	sidl::array<void*> saEntHandles;
	saEntHandles.borrow(aEntHandles, 1, &iLo, &iHi, &iStride);
	bool qMoreData = true;

	try {
	  ArrMesh.initEntArrIter(pvRootSet,
				 static_cast<TSTTM::EntityType>(eEType),
				 static_cast<TSTTM::EntityTopology>(eETopo),
				 100, arrIter);
	  int iMissedCount = 0;
	  int iExtraCount = 0;
	  do {
	    int iNumHandles;
	    qMoreData = ArrMesh.getNextEntArrIter(arrIter, saEntHandles,
						  iNumHandles);
	    for (int ii = 0; ii < iNumHandles; ii++) {
	      int iRes = sEnt.erase(aEntHandles[ii]);
	      if (iRes > 1) iExtraCount++;
	      else if (iRes == 0) iMissedCount++;
	    }
	  } while (qMoreData);
	  qOK = qOK && (iMissedCount == 0) && (iExtraCount == 0)
	    && (sEnt.size() == 0);

	  try {
	    ArrMesh.resetEntArrIter(arrIter);
	  } SHOULD_NOT_THROW;

	  // Now repeat to be sure the reset worked.
	  iMissedCount = 0;
	  iExtraCount = 0;
	  do {
	    int iNumHandles;
	    qMoreData = ArrMesh.getNextEntArrIter(arrIter, saEntHandles,
						  iNumHandles);
	    for (int ii = 0; ii < iNumHandles; ii++) {
	      int iRes = sEnt.erase(aEntHandles[ii]);
	      if (iRes > 1) iExtraCount++;
	      else if (iRes == 0) iMissedCount++;
	    }
	  } while (qMoreData);
	  qOK = qOK && (iMissedCount == 0) && (iExtraCount == 0)
	    && (sEnt.size() == 0);

	  try {
	    ArrMesh.endEntArrIter(arrIter);
	  } SHOULD_NOT_THROW;
	} catch (TSTTB::Error TErr) { // Big try block beginning with initIter
	  switch (TErr.getErrorType()) {
	  case TSTTB::ErrorType_FAILURE:
	    // Could mean there aren't any of these; check for this.
	    if (iMaxCount == 0)
	      iNPassed++;
	    else {
	      cout << "Max count should be zero; is " << iMaxCount << endl;
	      qAllPassed = false;
	      iNFailed++;
	    }
	    break;
	  case TSTTB::ErrorType_INVALID_ARGUMENT:
	    // Can happen for 2D meshes requested to iterate over
	    // regions.
	    if (ArrMesh.getGeometricDim() == 2 &&
		((eEType == TSTTM::EntityType_REGION) ||
		 ((eEType == TSTTM::EntityType_ALL_TYPES) &&
		  ((eETopo == TSTTM::EntityTopology_POLYHEDRON) ||
		   (eETopo == TSTTM::EntityTopology_TETRAHEDRON) ||
		   (eETopo == TSTTM::EntityTopology_PYRAMID) ||
		   (eETopo == TSTTM::EntityTopology_PRISM) ||
		   (eETopo == TSTTM::EntityTopology_HEXAHEDRON) ||
		   (eETopo == TSTTM::EntityTopology_SEPTAHEDRON)))))
	      iNPassed++;
	    else {
	      cout << "Valid arguments incorrectly diagnosed." << endl;
	      qAllPassed = false;
	      iNFailed++;
	    }
	    break;
	  default:
	    TErr.echo("Unexpected error type!");
	    TEST(false);
	    break;
	  }
	} // Done with catching exceptions
      } // This was a legal type/topo combination
    }
  } // Done with double loop to test entity iterators.

  // Test reciprocity of array adjacency calls.
  // Edge->vertex
  cout << " Testing entity array adjacency reciprocity" << endl;
  if (a2iAdjTable[1][0] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[0][1] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Edges and verts:";
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_EDGE,
			   TSTTM::EntityType_VERTEX);
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityType_EDGE);
    cout << endl;
  }

  // Face->vertex
  if (a2iAdjTable[2][0] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[0][2] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Faces and verts:";
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_FACE,
			   TSTTM::EntityType_VERTEX);
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityType_FACE);
    cout << endl;
  }

  // Region->vertex
  if (a2iAdjTable[3][0] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[0][3] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Regions and verts:";
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_REGION,
			   TSTTM::EntityType_VERTEX);
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityType_REGION);
    cout << endl;
  }

  // Face->edge
  if (a2iAdjTable[2][1] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[1][2] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Faces and edges:";
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_FACE,
			   TSTTM::EntityType_EDGE);
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_EDGE,
			   TSTTM::EntityType_FACE);
    cout << endl;
  }

  // Region->edge
  if (a2iAdjTable[3][1] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[1][3] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Regions and edges:";
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_REGION,
			   TSTTM::EntityType_EDGE);
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_EDGE,
			   TSTTM::EntityType_REGION);
    cout << endl;
  }

  // Region->face
  if (a2iAdjTable[3][2] != TSTTM::AdjacencyInfo_UNAVAILABLE &&
      a2iAdjTable[2][3] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
    cout << "  Regions and faces:";
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_REGION,
			   TSTTM::EntityType_FACE);
    vArrayReciprocityTest(ArrMesh, TSTTM::EntityType_FACE,
			   TSTTM::EntityType_REGION);
    cout << endl;
  }

  cout << " Array adjacency identity test..." << endl;
  //@@ Ensure that identity adjacency calls return nothing.
  // Test to be sure that no VERTEX has an adjacent VERTEX, etc.  The
  // TSTT data model specifies that entities of the same type are never
  // adjacent, but are connected instead through entities of different
  // type incident on both.
  for (int iType = 0; iType <= 3; iType++) {
    if (a2iAdjTable[iType][iType] != TSTTM::AdjacencyInfo_UNAVAILABLE) {
      cout << "  Type: ";
      cout.width(9);
      cout << astrTypes[iType] << endl;
      TSTTM::EntityType eType = static_cast<TSTTM::EntityType>(iType);

      ::sidl::array<void*> saEntHand;
      int iNumEnt;
      ArrMesh.getEntities(pvRootSet, eType,
			  TSTTM::EntityTopology_ALL_TOPOLOGIES,
			  saEntHand, iNumEnt);

      ::sidl::array<void*> saAdjHand;
      ::sidl::array<int32_t> saiOffset;
      int iNAdj, iNOff;
      ArrMesh.getEntArrAdj(saEntHand, iNumEnt, eType, saAdjHand, iNAdj,
			   saiOffset, iNOff);
      TEST(iNAdj == 0 && iNOff == iNumEnt+1);
    }
  }
}

// Test stuff specific to interface Modify.  Those functions are:
//     setVtxCoords, createVtx, createEnt, deleteEnt
static void vTestModifyInterface(TSTTM::Modify& ModMesh)
{
  cout << "Modify interface is currently only tested thoroughly in 2D" << endl;
  int iGeomDim = ModMesh.getGeometricDim();
  bool qOK;
  int iCoords1Size, iNVerts;
  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;

  {
    // setVtxCoords is trivial to test:  get a vertex, change its coords,
    // and verify that the change worked.
    sidl::array<void*> saVerts;
    ModMesh.getEntities(pvRootSet, TSTTM::EntityType_VERTEX,
			TSTTM::EntityTopology_POINT, saVerts, iNVerts);

    sidl::array<double> sadCoords1;
    ModMesh.getVtxArrCoords(saVerts, iNVerts, SO, sadCoords1, iCoords1Size);
    int iCoords1Lo = sadCoords1.lower(0);
    int iVertsLo = saVerts.lower(0);
    void* vertex = saVerts.get(iVertsLo);

    {
      cout << " Setting vertex coordinates..." << endl;
      sidl::array<double> sadCoordsTmp;
      double adCoords[iGeomDim];
      int iLo = 0, iHi = iGeomDim - 1, iStride = 1;
      sadCoordsTmp.borrow(adCoords, 1, &iLo, &iHi, &iStride);
      for (int i = 0; i < iGeomDim; i++) {
	adCoords[i] = i + sadCoords1.get(iCoords1Lo + i);
      }

      if (iGeomDim == 2) {
	// In 2D, should throw an error here.
	try {
	  ModMesh.setVtxCoords(vertex, adCoords[0], adCoords[1], 3);
	} catch (TSTTB::Error& TErr) {
	  TEST(TErr.getErrorType() == TSTTB::ErrorType_INVALID_ARGUMENT);
	}

	ModMesh.setVtxCoords(vertex, adCoords[0], adCoords[1], 0);
      }
      else if (iGeomDim == 3) {
	ModMesh.setVtxCoords(vertex, adCoords[0], adCoords[1], adCoords[2]);
      }

      sidl::array<double> sadCoords2;
      int iCoords2Size;
      ModMesh.getVtxArrCoords(saVerts, iNVerts, SO, sadCoords2, iCoords2Size);

      qOK = (iCoords2Size == iCoords1Size);
      int iCoords2Lo = sadCoords2.lower(0);
      for (int i = 0; i < iGeomDim; i++) {
	qOK = qOK && (APPROX_EQUAL(sadCoords1.get(iCoords1Lo + i),
				   sadCoords2.get(iCoords2Lo + i) - i));
      }
      TEST(qOK);
    }
  }

  // Now create a new vertex.  Then verify that its handle and
  // coordinates are returned properly.
  {
    cout << " Creating a new vertex..." << endl;
    sidl::array<double> sadCoordsTmp;
    double adCoords[] = {0,0,0};
    int iLo = 0, iHi = iGeomDim - 1, iStride = 1;
    sadCoordsTmp.borrow(adCoords, 1, &iLo, &iHi, &iStride);

    switch (iGeomDim) {
    case 3:
      adCoords[2] = 1.414; // approx sqrt(2)
    case 2:
      adCoords[1] = 2.8128; // approx e
    case 1:
      adCoords[0] = 3.14159; // approx pi
      break;
    default:
      assert(0);
    }

    void* new_vert;
    if (iGeomDim == 2) {
      try {
	ModMesh.createVtx(adCoords[0], adCoords[1], 0, new_vert);
      } SHOULD_NOT_THROW;
    }
    else if (iGeomDim == 3) {
      try {
	ModMesh.createVtx(adCoords[0], adCoords[1], adCoords[2], new_vert);
      } SHOULD_NOT_THROW;
    }

    sidl::array<void*> saVerts2;
    int iNVerts2;
    ModMesh.getEntities(pvRootSet, TSTTM::EntityType_VERTEX,
			TSTTM::EntityTopology_POINT, saVerts2, iNVerts2);
    qOK = ((iNVerts2 == iNVerts + 1) &&
	   (saVerts2.get(saVerts2.lower(0) + iNVerts2 - 1) == new_vert));

    // Now verify the coords.
    sidl::array<double> sadCoords2;
    int iCoords2Size;
    ModMesh.getVtxArrCoords(saVerts2, iNVerts2, SO, sadCoords2, iCoords2Size);
    qOK = qOK && (iCoords2Size == iCoords1Size + iGeomDim);
    int iCoords2Lo = sadCoords2.lower(0);

    for (int i = 0; i < iGeomDim; i++) {
      qOK = qOK &&
	(APPROX_EQUAL(sadCoords2.get(iCoords2Lo + (iNVerts2-1)*iGeomDim + i),
	 adCoords[i]));
    }
    TEST(qOK);
    try {
      ModMesh.deleteEnt(new_vert);
    } SHOULD_NOT_THROW;
    // Make sure it actually got deleted.
    TEST(ModMesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_POINT)
	 * iGeomDim == iCoords1Size);
  }

  // Test entity creation.  All created entities go into the following
  // containers for orderly deletion later.   Note that
  // TSTTM::Mesh must support Modify, or we wouldn't even be in
  // this subroutine.
  TSTTM::Mesh icMesh = TSTTM::Factory::newMesh(CREATE_ARG);
  TSTTM::Modify modMesh(icMesh);
  TSTTM::Entity entMesh(icMesh);

  if (iGeomDim == 2) {
    qOK = true;
    cout << " Creating mini 2D mesh..." << endl;
    // First, create some verts.  The first six can be used to create
    // two quads; verts 1, 2, 6, 7 to create two independent triangles.
    // Sketch:
    //
    //
    //
    //
    //       5 --5-- 4
    //       |       |
    //       6       4
    //       |       |
    //       3 --2-- 2 --9-- 7
    //       |       |     / |
    //       3       1   10  8
    //       |       | /     |
    //       0 --0-- 1 --7-- 6
    //
    double adAllCoords[] = {0, 0,
			    1, 0,
			    1, 1,
			    0, 1,
			    1, 2,
			    0, 2,
			    2, 0,
			    2, 1};
    cout << "   Creating eight vertices..." << endl;

    void* aNewVerts[8];
    for (int iV = 0; iV < 8; iV++) {
      modMesh.createVtx(adAllCoords[2*iV], adAllCoords[2*iV+1], 0,
		       aNewVerts[iV]);
    }
    void *pvLocalRootSet = icMesh.getRootSet();

    cout << "   Attempting to create eleven edges...";
    void* aEdgeVerts[] = {aNewVerts[0], aNewVerts[1],
			  aNewVerts[1], aNewVerts[2],
			  aNewVerts[2], aNewVerts[3],
			  aNewVerts[3], aNewVerts[0],
			  aNewVerts[2], aNewVerts[4],
			  aNewVerts[3], aNewVerts[5],
			  aNewVerts[4], aNewVerts[5],
			  aNewVerts[1], aNewVerts[6],
			  aNewVerts[6], aNewVerts[7],
			  aNewVerts[7], aNewVerts[2],
			  aNewVerts[1], aNewVerts[7]};

    void *aNewEdges[11], *aNewTris[2], *aNewQuads[2];
    bool qTriOK = true, qQuadOK = true;
    sidl::array<void*> saVerts;

    TSTTM::CreationStatus CStat;
    int iLo = 0, iHi = 1, iStride = 1;
    try {
      for (int iE = 0; iE < 11; iE++) {
	saVerts.borrow(aEdgeVerts + 2*iE, 1, &iLo, &iHi, &iStride);
	// This will throw an error if the implementation can't build
	// edges from verts.
	modMesh.createEnt(TSTTM::EntityTopology_LINE_SEGMENT,
			 saVerts, 2, aNewEdges[iE], CStat);
	qOK = qOK && (CStat == TSTTM::CreationStatus_NEW);
	// Check that the edge has the right verts
	{
	  sidl::array<void*> saVertsCheck;
	  void* aVertsCheck[2];
	  int iNV;
	  saVertsCheck.borrow(aVertsCheck, 1, &iLo, &iHi, &iStride);
	  entMesh.getEntAdj(aNewEdges[iE], TSTTM::EntityType_VERTEX, saVertsCheck, iNV);
	  qOK = qOK && ((iNV == 2) &&
			((aVertsCheck[0] == aEdgeVerts[2*iE] &&
			  aVertsCheck[1] == aEdgeVerts[2*iE+1]) ||
			 (aVertsCheck[1] == aEdgeVerts[2*iE] &&
			  aVertsCheck[0] == aEdgeVerts[2*iE+1])));
	}
      }
      // Now make sure that trying this again doesn't cause problems.
      for (int iE = 0; iE < 11; iE++) {
	saVerts.borrow(aEdgeVerts + 2*iE, 1, &iLo, &iHi, &iStride);
	void *new_edge;
	try {
	  modMesh.createEnt(TSTTM::EntityTopology_LINE_SEGMENT,
			   saVerts, 2, new_edge, CStat);
	  qOK = qOK && ((CStat == TSTTM::CreationStatus_ALREADY_EXISTED) ||
			(CStat == TSTTM::CreationStatus_CREATED_DUPLICATE));
	} SHOULD_NOT_THROW;
      }
      cout << endl;
      TEST(qOK);

      qOK = true;
      // Now try to create two triangles from edges.
      cout << "   Attempting to create two triangles from edges...";
      // These triangles are built from edges (1, 10, 9) and (7, 8, 10).
      void* aTriEdges[] = {aNewEdges[1], aNewEdges[10], aNewEdges[9],
			   aNewEdges[7], aNewEdges[8], aNewEdges[10]};
      sidl::array<void*> saEdges;
      iHi = 2;

      TSTTM::CreationStatus CStat;
      try {
	for (int iTri = 0; iTri < 2; iTri++) {
	  saEdges.borrow(aTriEdges + 3*iTri, 1, &iLo, &iHi, &iStride);
	  modMesh.createEnt(TSTTM::EntityTopology_TRIANGLE,
			   saEdges, 3, aNewTris[iTri], CStat);
	  qOK = qOK && (CStat == TSTTM::CreationStatus_NEW);

	  // Check that the tri has the right edges
	  {
	    sidl::array<void*> saEdgesCheck;
	    void* aEdgesCheck[3];
	    int iNE;
	    saEdgesCheck.borrow(aEdgesCheck, 1, &iLo, &iHi, &iStride);
	    entMesh.getEntAdj(aNewTris[iTri], TSTTM::EntityType_EDGE, saEdgesCheck, iNE);
	    // Must be in cyclic order, so only three possibilities exist.
	    qOK = qOK && ((iNE == 3) &&
			  ((aEdgesCheck[0] == aTriEdges[3*iTri] &&
			    aEdgesCheck[1] == aTriEdges[3*iTri+1] &&
			    aEdgesCheck[2] == aTriEdges[3*iTri+2]) ||
			   (aEdgesCheck[1] == aTriEdges[3*iTri] &&
			    aEdgesCheck[2] == aTriEdges[3*iTri+1] &&
			    aEdgesCheck[0] == aTriEdges[3*iTri+2]) ||
			   (aEdgesCheck[2] == aTriEdges[3*iTri] &&
			    aEdgesCheck[0] == aTriEdges[3*iTri+1] &&
			    aEdgesCheck[1] == aTriEdges[3*iTri+2])));
	  }

	}

	// Verify edge-to-tri connectivity.
	{
	  // There's only one edge that has two adjacent triangles; the
	  // others have a triangle and something undefined adjacent to
	  // them.  So check just that one edge, which is edge 10.
	  sidl::array<void*> saTri;
	  void * aTri[10];
	  int iLo = 0, iHi = 9, iStride = 1;
	  saTri.borrow(aTri, 1, &iLo, &iHi, &iStride);
	  int iNumTri;
	  entMesh.getEntAdj(aNewEdges[10], TSTTM::EntityType_FACE, saTri, iNumTri);
	  qOK = qOK && (iNumTri == 2)
	    && ((aTri[0] == aNewTris[0] && aTri[1] == aNewTris[1]) ||
		(aTri[0] == aNewTris[1] && aTri[1] == aNewTris[0]));
	}

	// Now make sure that trying this again doesn't cause problems.
	for (int iTri = 0; iTri < 2; iTri++) {
	  saVerts.borrow(aEdgeVerts + 3*iTri, 1, &iLo, &iHi, &iStride);
	  void *new_tri;
	  try {
	    modMesh.createEnt(TSTTM::EntityTopology_TRIANGLE,
			     saEdges, 3, new_tri, CStat);
	    qOK = qOK && ((CStat == TSTTM::CreationStatus_ALREADY_EXISTED) ||
			  (CStat == TSTTM::CreationStatus_CREATED_DUPLICATE));
	  } SHOULD_NOT_THROW;
	}
	cout << endl;
      } catch (TSTTB::Error& TErr) {
	qTriOK = false;
	if (TErr.getErrorType() == TSTTB::ErrorType_NOT_SUPPORTED) {
	  cout << endl
	       << "    no support for building triangles from edges" << endl;
	}
	else {
	  TErr.echo("Unexpected error type!");
	  TEST(false);
	}
      }
      TEST(qOK);
      qOK = true;

      // Now try to create two quadrilaterals from edges.
      cout << "   Attempting to create two quadrilaterals from edges...";
      // These quadrilaterals are built from edges (0, 1, 2, 3) and
      // (2, 4, 5, 6).
      void* aQuadEdges[] =
	{aNewEdges[0], aNewEdges[1], aNewEdges[2], aNewEdges[3],
	 aNewEdges[2], aNewEdges[4], aNewEdges[5], aNewEdges[6]};
      iHi = 3;

      try {
	for (int iQuad = 0; iQuad < 2; iQuad++) {
	  saEdges.borrow(aQuadEdges + 4*iQuad, 1, &iLo, &iHi, &iStride);
	  modMesh.createEnt(TSTTM::EntityTopology_QUADRILATERAL,
			   saEdges, 4, aNewQuads[iQuad], CStat);
	  qOK = qOK && (CStat == TSTTM::CreationStatus_NEW);

	  // Check that the quad has the right edges
	  {
	    sidl::array<void*> saEdgesCheck;
	    void* aEdgesCheck[4];
	    int iNE;
	    saEdgesCheck.borrow(aEdgesCheck, 1, &iLo, &iHi, &iStride);
	    entMesh.getEntAdj(aNewQuads[iQuad], TSTTM::EntityType_EDGE, saEdgesCheck, iNE);
	    // Must be in cyclic order, so only four possibilities exist.
	    qOK = qOK && ((iNE == 4) &&
			  ((aEdgesCheck[0] == aQuadEdges[4*iQuad] &&
			    aEdgesCheck[1] == aQuadEdges[4*iQuad+1] &&
			    aEdgesCheck[2] == aQuadEdges[4*iQuad+2] &&
			    aEdgesCheck[3] == aQuadEdges[4*iQuad+3]) ||
			   (aEdgesCheck[1] == aQuadEdges[4*iQuad] &&
			    aEdgesCheck[2] == aQuadEdges[4*iQuad+1] &&
			    aEdgesCheck[3] == aQuadEdges[4*iQuad+2] &&
			    aEdgesCheck[0] == aQuadEdges[4*iQuad+3]) ||
			   (aEdgesCheck[2] == aQuadEdges[4*iQuad] &&
			    aEdgesCheck[3] == aQuadEdges[4*iQuad+1] &&
			    aEdgesCheck[0] == aQuadEdges[4*iQuad+2] &&
			    aEdgesCheck[1] == aQuadEdges[4*iQuad+3]) ||
			   (aEdgesCheck[3] == aQuadEdges[4*iQuad] &&
			    aEdgesCheck[0] == aQuadEdges[4*iQuad+1] &&
			    aEdgesCheck[1] == aQuadEdges[4*iQuad+2] &&
			    aEdgesCheck[2] == aQuadEdges[4*iQuad+3])));
	  }
	}

	// Verify edge-to-quad connectivity.
	{
	  // There's only one edge that has two adjacent quadrilaterals; the
	  // others have a quad and something undefined adjacent to
	  // them.  So check just that one edge, which is edge 2.
	  sidl::array<void*> saQuad;
	  void * aQuad[10];
	  int iLo = 0, iHi = 9, iStride = 1;
	  saQuad.borrow(aQuad, 1, &iLo, &iHi, &iStride);
	  int iNumQuad;
	  entMesh.getEntAdj(aNewEdges[2], TSTTM::EntityType_FACE, saQuad, iNumQuad);
	  qOK = qOK && (iNumQuad == 2)
	    && ((aQuad[0] == aNewQuads[0] && aQuad[1] == aNewQuads[1]) ||
		(aQuad[0] == aNewQuads[1] && aQuad[1] == aNewQuads[0]));
	}

	// If tris were also built successfully, verify edge-tri/quad
	// connectivity.
	if (qTriOK) {
	  // There's only one edge that is between a tri and a quad:
	  // edge 1.
	  sidl::array<void*> saFace;
	  void * aFace[10];
	  int iLo = 0, iHi = 9, iStride = 1;
	  saFace.borrow(aFace, 1, &iLo, &iHi, &iStride);
	  int iNumFace;
	  entMesh.getEntAdj(aNewEdges[1], TSTTM::EntityType_FACE, saFace, iNumFace);
	  qOK = qOK && (iNumFace == 2)
	    && ((aFace[0] == aNewQuads[0] && aFace[1] == aNewTris[0]) ||
		(aFace[0] == aNewTris[0] && aFace[1] == aNewQuads[0]));
	}

	// Now make sure that trying this again doesn't cause problems.
	for (int iQuad = 0; iQuad < 2; iQuad++) {
	  saVerts.borrow(aEdgeVerts + 4*iQuad, 1, &iLo, &iHi, &iStride);
	  void *new_quad;
	  try {
	    modMesh.createEnt(TSTTM::EntityTopology_QUADRILATERAL,
			     saEdges, 4, new_quad, CStat);
	    qOK = qOK && ((CStat == TSTTM::CreationStatus_ALREADY_EXISTED) ||
			  (CStat == TSTTM::CreationStatus_CREATED_DUPLICATE));
	  } SHOULD_NOT_THROW;
	}
	cout << endl;
      } catch (TSTTB::Error& TErr) {
	qQuadOK = false;
	if (TErr.getErrorType() == TSTTB::ErrorType_NOT_SUPPORTED) {
	  cout << endl
	       << "    no support for building quads from edges" << endl;
	}
	else {
	  TErr.echo("Unexpected error type!");
	  TEST(false);
	}
      }
      TEST(qOK);
      qOK = true;

      // Now test entity deletion for all entities recorded.  Along the
      // way, check that entity counts and returned entities are done
      // correctly.

      // First, verify that low-dimension entities can't be deleted
      // without deleting the entities that depend on them.
      int iNVerts = icMesh.getNumOfTopo
	(pvLocalRootSet, TSTTM::EntityTopology_POINT);
      int iNEdges = icMesh.getNumOfTopo
	(pvLocalRootSet, TSTTM::EntityTopology_LINE_SEGMENT);
      int iNTris = icMesh.getNumOfTopo
	(pvLocalRootSet, TSTTM::EntityTopology_TRIANGLE);
      int iNQuads = icMesh.getNumOfTopo
	(pvLocalRootSet, TSTTM::EntityTopology_QUADRILATERAL);

      TEST(iNVerts == 8);
      TEST(iNEdges == 11);
      TEST((iNTris == 0 && !qTriOK) || iNTris == 2);
      TEST((iNQuads == 0 && !qQuadOK) || iNQuads == 2);

      modMesh.deleteEnt(aNewVerts[0]);
      // Should not succeed in deleting.
      TEST(icMesh.getNumOfTopo(pvLocalRootSet,
				   TSTTM::EntityTopology_POINT) == 8);

      if (qTriOK || qQuadOK) {
	// Try to remove the edge that is shared between by a tri and
	// a quad.
	modMesh.deleteEnt(aNewEdges[1]);
	TEST(icMesh.getNumOfTopo(pvLocalRootSet,
				     TSTTM::EntityTopology_LINE_SEGMENT)
	     == 11);
      }

      // Now delete one quad (if any exist) and verify that you only
      // get one quad one request.
      if (qQuadOK) {
	modMesh.deleteEnt(aNewQuads[1]);
	sidl::array<void*> saQuads;
	int iNQuadAfter;
	entMesh.getEntities(pvLocalRootSet, TSTTM::EntityType_FACE,
			   TSTTM::EntityTopology_QUADRILATERAL,
			     saQuads, iNQuadAfter);
	TEST(iNQuadAfter == 1 &&
	     icMesh.getNumOfTopo(pvLocalRootSet,
				     TSTTM::EntityTopology_QUADRILATERAL)
	     == 1);
      }

      // Repeat the previous test for triangles.
      if (qTriOK) {
	modMesh.deleteEnt(aNewTris[1]);
	sidl::array<void*> saTris;
	int iNTriAfter;
	entMesh.getEntities(pvLocalRootSet, TSTTM::EntityType_FACE,
			   TSTTM::EntityTopology_TRIANGLE,
			   saTris, iNTriAfter);
	TEST(iNTriAfter == 1 &&
	     icMesh.getNumOfTopo(pvLocalRootSet,
				     TSTTM::EntityTopology_TRIANGLE)
	     == 1);
      }

      // Now delete the rest of the faces.
      if (qQuadOK) modMesh.deleteEnt(aNewQuads[0]);
      if (qTriOK)  modMesh.deleteEnt(aNewTris[0]);
      TEST(icMesh.getNumOfTopo(pvLocalRootSet,
				   TSTTM::EntityTopology_QUADRILATERAL) == 0
	   && icMesh.getNumOfTopo(pvLocalRootSet,
				      TSTTM::EntityTopology_TRIANGLE) == 0
	   && icMesh.getNumOfType(pvLocalRootSet,
				  TSTTM::EntityType_FACE) == 0);

      // Repeat the single-deletion test for edges.
      {
	modMesh.deleteEnt(aNewEdges[10]);
	sidl::array<void*> saEdges;
	int iNEdgeAfter;
	entMesh.getEntities(pvLocalRootSet, TSTTM::EntityType_EDGE,
			   TSTTM::EntityTopology_LINE_SEGMENT,
			   saEdges, iNEdgeAfter);
	TEST(iNEdgeAfter == 10 &&
	     icMesh.getNumOfTopo(pvLocalRootSet,
				     TSTTM::EntityTopology_LINE_SEGMENT)
	     == 10 &&
	     icMesh.getNumOfType(pvLocalRootSet,
				 TSTTM::EntityType_EDGE) == 10);

      }

      // Delete the remaining edges.
      int i;
      for (i = 0; i < 10; i++) {
	modMesh.deleteEnt(aNewEdges[i]);
      }
      TEST(icMesh.getNumOfTopo(pvLocalRootSet,
				   TSTTM::EntityTopology_LINE_SEGMENT) == 0 &&
	   icMesh.getNumOfType(pvLocalRootSet,
			       TSTTM::EntityType_EDGE) == 0);

      // Delete all the verts.
      for (i = 0; i < 8; i++) {
	modMesh.deleteEnt(aNewVerts[i]);
      }
      TEST(icMesh.getNumOfTopo(pvLocalRootSet,
				   TSTTM::EntityTopology_POINT) == 0 &&
	   icMesh.getNumOfType(pvLocalRootSet,
			       TSTTM::EntityType_VERTEX) == 0);


    } catch (TSTTB::Error& TErr) {
      if (TErr.getErrorType() == TSTTB::ErrorType_NOT_SUPPORTED) {
	cout << " no support for edges" << endl;
      }
      else {
	TErr.echo("Unexpected error type!");
	TEST(false);
      }
    } // Done with trying to create edges, and stuff from edges.

    // Now test again, creating tris and quads directly from verts.

    // Create the verts again, because you just deleted them.
    for (int iV = 0; iV < 8; iV++) {
      modMesh.createVtx(adAllCoords[2*iV], adAllCoords[2*iV+1], 0,
		       aNewVerts[iV]);
    }
    cout << "   Attempting to create two triangles from verts...";
    // First, try to create two triangles.
    void* aTriVerts[] = {aNewVerts[1], aNewVerts[6], aNewVerts[7],
			 aNewVerts[7], aNewVerts[2], aNewVerts[1]};

    qTriOK = true;
    iHi = 2;
    try {
      for (int iTri = 0; iTri < 2; iTri++) {
	saVerts.borrow(aTriVerts + 3*iTri, 1, &iLo, &iHi, &iStride);
	modMesh.createEnt(TSTTM::EntityTopology_TRIANGLE,
			 saVerts, 3, aNewTris[iTri], CStat);
	qOK = qOK && (CStat == TSTTM::CreationStatus_NEW);

	// Check that the tri has the right verts
	{
	  sidl::array<void*> saVertsCheck;
	  void* aVertsCheck[3];
	  int iNE;
	  saVertsCheck.borrow(aVertsCheck, 1, &iLo, &iHi, &iStride);
	  entMesh.getEntAdj(aNewTris[iTri], TSTTM::EntityType_VERTEX,
			   saVertsCheck, iNE);
	  // Must be in cyclic order, so only three possibilities exist.
	  qOK = qOK && ((iNE == 3) &&
			((aVertsCheck[0] == aTriVerts[3*iTri] &&
			  aVertsCheck[1] == aTriVerts[3*iTri+1] &&
			  aVertsCheck[2] == aTriVerts[3*iTri+2]) ||
			 (aVertsCheck[1] == aTriVerts[3*iTri] &&
			  aVertsCheck[2] == aTriVerts[3*iTri+1] &&
			  aVertsCheck[0] == aTriVerts[3*iTri+2]) ||
			 (aVertsCheck[2] == aTriVerts[3*iTri] &&
			  aVertsCheck[0] == aTriVerts[3*iTri+1] &&
			  aVertsCheck[1] == aTriVerts[3*iTri+2])));
	}
      } // Done making and checking all tris.

      // Now make sure that trying this again doesn't cause problems.
      for (int iTri = 0; iTri < 2; iTri++) {
	saVerts.borrow(aTriVerts + 3*iTri, 1, &iLo, &iHi, &iStride);
	void *new_tri;
	try {
	  modMesh.createEnt(TSTTM::EntityTopology_TRIANGLE,
			   saVerts, 3, new_tri, CStat);
	  qOK = qOK && ((CStat == TSTTM::CreationStatus_ALREADY_EXISTED) ||
			(CStat == TSTTM::CreationStatus_CREATED_DUPLICATE));
	} SHOULD_NOT_THROW;
      }
      cout << endl;
      TEST(qOK);
      qOK = true;

      // Should have five edges.
      TEST(5 == icMesh.getNumOfType(pvLocalRootSet, TSTTM::EntityType_EDGE));
    } catch (TSTTB::Error& TErr) {
      qTriOK = false;
      if (TErr.getErrorType() == TSTTB::ErrorType_NOT_SUPPORTED) {
	cout << endl
	     << "    no support for building triangles from vertices" << endl;
      }
      else {
	TErr.echo("Unexpected error type!");
	TEST(false);
      }
    } // Done trying to create triangles from vertices.

    cout << "   Attempting to create two quadrilaterals from verts...";
    // First, try to create two quads.
    void* aQuadVerts[] =
      {aNewVerts[0], aNewVerts[1], aNewVerts[2], aNewVerts[3],
       aNewVerts[3], aNewVerts[2], aNewVerts[4], aNewVerts[5]};

    qQuadOK = true;
    iHi = 3;
    try {
      for (int iQuad = 0; iQuad < 2; iQuad++) {
	saVerts.borrow(aQuadVerts + 4*iQuad, 1, &iLo, &iHi, &iStride);
	modMesh.createEnt(TSTTM::EntityTopology_QUADRILATERAL,
			 saVerts, 4, aNewQuads[iQuad], CStat);
	qOK = qOK && (CStat == TSTTM::CreationStatus_NEW);

	// Check that the quad has the right verts
	{
	  sidl::array<void*> saVertsCheck;
	  void* aVertsCheck[4];
	  int iNE;
	  saVertsCheck.borrow(aVertsCheck, 1, &iLo, &iHi, &iStride);
	  entMesh.getEntAdj(aNewQuads[iQuad], TSTTM::EntityType_VERTEX,
			   saVertsCheck, iNE);
	  // Must be in cyclic order, so only three possibilities exist.
	  qOK = qOK && ((iNE == 4) &&
			((aVertsCheck[0] == aQuadVerts[4*iQuad] &&
			  aVertsCheck[1] == aQuadVerts[4*iQuad+1] &&
			  aVertsCheck[2] == aQuadVerts[4*iQuad+2] &&
			  aVertsCheck[3] == aQuadVerts[4*iQuad+3]) ||
			 (aVertsCheck[1] == aQuadVerts[4*iQuad] &&
			  aVertsCheck[2] == aQuadVerts[4*iQuad+1] &&
			  aVertsCheck[3] == aQuadVerts[4*iQuad+2] &&
			  aVertsCheck[0] == aQuadVerts[4*iQuad+3]) ||
			 (aVertsCheck[2] == aQuadVerts[4*iQuad] &&
			  aVertsCheck[3] == aQuadVerts[4*iQuad+1] &&
			  aVertsCheck[0] == aQuadVerts[4*iQuad+2] &&
			  aVertsCheck[1] == aQuadVerts[4*iQuad+3]) ||
			 (aVertsCheck[3] == aQuadVerts[4*iQuad] &&
			  aVertsCheck[0] == aQuadVerts[4*iQuad+1] &&
			  aVertsCheck[1] == aQuadVerts[4*iQuad+2] &&
			  aVertsCheck[2] == aQuadVerts[4*iQuad+3])));
	}
      } // Done making and checking all quads.

      // Now make sure that trying this again doesn't cause problems.
      for (int iQuad = 0; iQuad < 2; iQuad++) {
	saVerts.borrow(aQuadVerts + 4*iQuad, 1, &iLo, &iHi, &iStride);
	void *new_quad;
	try {
	  modMesh.createEnt(TSTTM::EntityTopology_QUADRILATERAL,
			   saVerts, 4, new_quad, CStat);
	  qOK = qOK && ((CStat == TSTTM::CreationStatus_ALREADY_EXISTED) ||
			(CStat == TSTTM::CreationStatus_CREATED_DUPLICATE));
	} SHOULD_NOT_THROW;
      }
      cout << endl;
      TEST(qOK);
      qOK = true;

      // Should have seven edges, or 11 if the triangles also built okay.
      TEST((qTriOK &&
	    11 == icMesh.getNumOfType(pvLocalRootSet, TSTTM::EntityType_EDGE))
	   ||
	   (!qTriOK &&
	    7 == icMesh.getNumOfType(pvLocalRootSet, TSTTM::EntityType_EDGE))
	   );
    } catch (TSTTB::Error& TErr) {
      qQuadOK = false;
      if (TErr.getErrorType() == TSTTB::ErrorType_NOT_SUPPORTED) {
	cout << endl
	     << "    no support for building quadrilaterals from vertices"
	     << endl;
      }
      else {
	TErr.echo("Unexpected error type!");
	TEST(false);
      }
    } // Done trying to create quadrilaterals from vertices.

    // No need to re-test delete.
    // icMesh goes out of scope with the following }
  } // Done with testing create/deleteEnt for 2D.
  else if (iGeomDim == 3) {
    // Now repeat all of the above for the more complex case of testing
    // 3D mesh creation, in (at least most of) the permutations.
  }
}

static void vTestArrModInterface(TSTTM::ArrMod& ArrModMesh)
{
  // setVtxArrCoords is trivial to test:  get vertices, change their coords,
  // and verify that the change worked.
  sidl::array<void*> saVerts;
  int iNVerts;
  ArrModMesh.getEntities(pvRootSet, TSTTM::EntityType_VERTEX,
			 TSTTM::EntityTopology_POINT, saVerts, iNVerts);
  TEST(iNVerts == ArrModMesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_POINT));
  
  sidl::array<double> sadCoords1;
  int iCoords1Size;
  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  ArrModMesh.getVtxArrCoords(saVerts, iNVerts, SO, sadCoords1, iCoords1Size);
  int iCoords1Lo = sadCoords1.lower(0);
  int iGeomDim = ArrModMesh.getGeometricDim();

  bool qOK;
  {
    cout << " Setting vertex coordinates..." << endl;
    sidl::array<double> sadCoordsTmp =
      sidl::array<double>::create1d(iNVerts*iGeomDim);
    for (int iV = 0; iV < iNVerts; iV++) {
      int iOff = iGeomDim * iV;
      for (int i = 0; i < iGeomDim; i++) {
	sadCoordsTmp.set(iCoords1Lo + iOff + i,
			 sadCoords1.get(iCoords1Lo + iOff + i) + iV + i + 1);
      }
    }

    ArrModMesh.setVtxArrCoords(saVerts, iNVerts, SO, sadCoordsTmp,
			       iCoords1Size);

    sidl::array<double> sadCoords2;
    int iCoords2Size;
    ArrModMesh.getVtxArrCoords(saVerts, iNVerts, SO, sadCoords2, iCoords2Size);

    qOK = (iCoords2Size == iCoords1Size);
    int iCoords2Lo = sadCoords2.lower(0);
    for (int iV = 0; iV < iNVerts; iV++) {
      int iOff = iGeomDim * iV;
      for (int i = 0; i < iGeomDim; i++) {
	qOK = qOK && (APPROX_EQUAL(sadCoords1.get(iCoords1Lo + iOff + i),
				   sadCoords2.get(iCoords2Lo + iOff + i)
				   - iV - i - 1));
      }
    }
    TEST(qOK);
  }

  // Now create a flock of new vertices.  Then verify that their handles
  // and coordinates are returned properly .
  {
    qOK = true;
    cout << " Creating new vertices..." << endl;
    sidl::array<double> sadCoords2;
    sadCoords2 = sidl::array<double>::create1d(10*iGeomDim);
    int iCoords2Lo = sadCoords2.lower(0);
    for (int i = 0; i < 10; i++) {
      switch (iGeomDim) {
      case 3:
	sadCoords2.set(iCoords2Lo + i*iGeomDim + 2, 1.414 * i); // i*sqrt(2)
      case 2:
	sadCoords2.set(iCoords2Lo + i*iGeomDim + 1, 2.81828 * i); // i*e
      case 1:
	sadCoords2.set(iCoords2Lo + i*iGeomDim + 0, 3.14159 * i); // i*pi
	break;
      default:
	assert(0);
      }
    }

    sidl::array<void*> saNewVerts;
    int iNumNewVerts;
    TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
    ArrModMesh.createVtxArr(10, TSTTM::StorageOrder_INTERLEAVED, sadCoords2,
			    10*iGeomDim, saNewVerts, iNumNewVerts);
    qOK = (iNumNewVerts == 10);

    sidl::array<void*> saVerts2;
    int iNVerts2;
    ArrModMesh.getEntities(pvRootSet, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityTopology_POINT, saVerts2, iNVerts2);
    qOK = qOK && (iNVerts2 == iNVerts + 10);
    int iNewVertLo = saNewVerts.lower(0);
    int iV2Lo = saVerts2.lower(0);
    for (int ii = 0; ii < 10; ii ++) {
      qOK = qOK && (saVerts2.get(iV2Lo + iNVerts + ii) ==
		    saNewVerts.get(iNewVertLo + ii));
    }

    // Now verify the coords.
    sidl::array<double> sadCoords3;
    int iCoords3Size;
    ArrModMesh.getVtxArrCoords(saVerts2, iNVerts2, SO,
			       sadCoords3, iCoords3Size);
    qOK = qOK && (iCoords3Size == iCoords1Size + iGeomDim*10);
    int iCoords3Lo = sadCoords3.lower(0);

    for (int iV = 0; iV < 10; iV++) {
      for (int i = 0; i < iGeomDim; i++) {
	qOK = qOK && (sadCoords3.get(iCoords3Lo + (iNVerts+iV)*iGeomDim + i)
		      == sadCoords2.get(iCoords2Lo + iV*iGeomDim + i));
      }
    }
    TEST(qOK);
    try {
      ArrModMesh.deleteEntArr(saNewVerts, iNumNewVerts);
    } SHOULD_NOT_THROW;
    // Check that they were really deleted.
    TEST(ArrModMesh.getNumOfTopo(pvRootSet, TSTTM::EntityTopology_POINT)
	 == iNVerts);
  } // Done checking array vertex creation
}


static void vTestEntSetInterface(TSTTM::Mesh& M)
{
  //     createEntSet, destroyEntSet, isList, getNumEntSets, getEntSets,
  //     addEntToSet, rmvEntFromSet, addEntArrToSet, rmvEntArrFromSet,
  //     addEntSet, rmvEntSet, isContainedIn
  TSTTB::EntSet ES(M);
  TSTTM::Entity EntMesh(M);
  void* apvEntSets[11];
  sidl::array<void*> saSets;
  int iNumSets;
  bool qGotAllSets;
  int i;

  cout << "Testing creation, basic methods, deletion...              ";
  // Try creating a number of sets and lists and see if they show up as members
  // of the root set...
  try {
    for ( i=0; i<5; i++ ) {
      ES.createEntSet(true, apvEntSets[i]);
      TEST ( ES.isList(apvEntSets[i]) );
    }
    for ( i=5; i<10; i++ ) {
      ES.createEntSet(false, apvEntSets[i]);
      TEST ( !ES.isList(apvEntSets[i]) );
    }

    // Whether the number of hops is -1, 0, 1 is irrelevant for the root set,
    // especially since none of the sets contain each other
    qGotAllSets = true;
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(pvRootSet, -1) == 10 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(pvRootSet,  0) == 10 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(pvRootSet,  1) == 10 );
    TEST ( qGotAllSets );

    // Try to get all the entity sets from the root set
    ES.getEntSets( pvRootSet, 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 10 )
      GOT_ALL_ITEMS ( apvEntSets, saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    // Try to destroy all the sets
    for ( i=0; i<10; i++ ) {
      ES.destroyEntSet( apvEntSets[i] );
      iNPassed++;
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing addition and removal for sets in sets...          ";
  try {

    // Create 10 sets, the first five of which are lists ( irrelevant as far
    // as sets containing one another, but good for thoroughness )
    for ( i=0; i<10; i++ )
      ES.createEntSet(i<5, apvEntSets[i]);

    // Create a hierarchy of sets where             [0]
    //     set 0 contains sets 1-4,           _______|_______
    //     set 1 contains sets 5-7,           [1],[2],[3],[4]
    // and set 5 contains sets 8-9        _____|_____
    //                                    [5],[6],[7]
    //                                  ___|___
    // (and test the contain query)     [8],[9]

    qGotAllSets = true;
    for ( i=1; i<5; i++ ) {
      ES.addEntSet( apvEntSets[i], apvEntSets[0] );
      qGotAllSets = qGotAllSets && ( ES.isEntSetContained(apvEntSets[0], apvEntSets[i]) );
    }
    for ( i=5; i<8; i++ ) {
      ES.addEntSet( apvEntSets[i], apvEntSets[1] );
      qGotAllSets = qGotAllSets && ( ES.isEntSetContained(apvEntSets[1], apvEntSets[i]) );
    }
    for ( i=8; i<10; i++ ) {
      ES.addEntSet( apvEntSets[i], apvEntSets[5] );
      qGotAllSets = qGotAllSets && ( ES.isEntSetContained(apvEntSets[5], apvEntSets[i]) );
    }
    TEST ( qGotAllSets );

    // The numbers expected should be obvious from the diagram above, keeping in
    // mind that -1 hops really means infinite
    qGotAllSets = true;
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[8],  0) == 0 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[8],  1) == 0 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[8], -1) == 0 );
    TEST ( qGotAllSets );

    qGotAllSets = true;
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[5],  0) == 2 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[5],  1) == 2 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[5], -1) == 2 );
    TEST ( qGotAllSets );

    qGotAllSets = true;
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[1],  0) == 3 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[1],  1) == 5 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[1], -1) == 5 );
    TEST ( qGotAllSets );

    qGotAllSets = true;
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[0],  0) == 4 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[0],  1) == 7 );
    qGotAllSets = qGotAllSets && ( ES.getNumEntSets(apvEntSets[0], -1) == 9 );
    TEST ( qGotAllSets );

    // Now test whether the sets counted in the methods above can actually be
    // retrived
    ES.getEntSets( apvEntSets[8], -1, saSets, iNumSets );
    TEST ( iNumSets == 0 );

    ES.getEntSets( apvEntSets[5], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvEntSets+8), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    ES.getEntSets( apvEntSets[5], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvEntSets+8), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    ES.getEntSets( apvEntSets[5], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvEntSets+8), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    ES.getEntSets( apvEntSets[1], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 3 )
      GOT_ALL_ITEMS ( (apvEntSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    ES.getEntSets( apvEntSets[1], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvEntSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    ES.getEntSets( apvEntSets[1], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvEntSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    ES.getEntSets( apvEntSets[0], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 4 )
      GOT_ALL_ITEMS ( (apvEntSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    ES.getEntSets( apvEntSets[0], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 7 )
      GOT_ALL_ITEMS ( (apvEntSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    ES.getEntSets( apvEntSets[0], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 9 )
      GOT_ALL_ITEMS ( (apvEntSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    // Test the ability to remove sets from sets
    qGotAllSets = true;
    for ( i=1; i<5; i++ ) {
      ES.rmvEntSet( apvEntSets[i], apvEntSets[0] );
      qGotAllSets = qGotAllSets && ( !ES.isEntSetContained(apvEntSets[0], apvEntSets[i]) );
    }
    for ( i=5; i<8; i++ ) {
      ES.rmvEntSet( apvEntSets[i], apvEntSets[1] );
      qGotAllSets = qGotAllSets && ( !ES.isEntSetContained(apvEntSets[1], apvEntSets[i]) );
    }
    for ( i=8; i<10; i++ ) {
      ES.rmvEntSet( apvEntSets[i], apvEntSets[5] );
      qGotAllSets = qGotAllSets && ( !ES.isEntSetContained(apvEntSets[5], apvEntSets[i]) );
    }
    TEST ( qGotAllSets );

    // Clean up the sets
    for ( i=0; i<10; i++ ) {
      ES.destroyEntSet( apvEntSets[i] );
    }

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing addition and removal for entities in sets...      ";
  void* apvEnts[10];
  void* pvIter;
  sidl::array<void*> saEnts = saEnts.create1d(10),
		     saBadEnts = saBadEnts.create1d(10);
  try {
    // Get some faces to work with (any generic entities will do)
    if ( !EntMesh.initEntIter( pvRootSet, TSTTM::EntityType_FACE,
			  TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    for ( i=0; i<10; i++ ) {
      if ( !EntMesh.getNextEntIter(pvIter, apvEnts[i]) ) {
	cout << "\nCould not get enough mesh data to work with... exiting...\n";
	TEST ( data_preparation );
	return;
      }
      // Add the entities to a sidl array for later tests
      saEnts.set(i, apvEnts[i]);
    }
    EntMesh.endEntIter( pvIter );
    for ( i=0; i<10; i++ ) {
      ES.createEntSet(i<5, apvEntSets[i]);
      // Create a sidl array of invalid entities (in this case they are sets)
      // (used for error testing)
      saBadEnts.set( i, apvEntSets[i] );
    }
    // SHOULD_NOW_THROW_CRITICAL means this test cannot continue if this part fails
    // (need entities to work with for everything else)
  } SHOULD_NOT_THROW_CRITICAL;

  try {
    // Create a hierarchy of sets where             [0]
    //     set 0 contains sets 1-4,           _______|_______
    //     set 1 contains sets 5-7,           [1],[2],[3],[4]
    // and set 5 contains sets 8-9        _____|_____
    //                                    [5],[6],[7]
    //                                  ___|___
    //                                  [8],[9]
    for ( i=1; i<5; i++ ) {
      ES.addEntSet( apvEntSets[i], apvEntSets[0] );
    }
    for ( i=5; i<8; i++ ) {
      ES.addEntSet( apvEntSets[i], apvEntSets[1] );
    }
    for ( i=8; i<10; i++ ) {
      ES.addEntSet( apvEntSets[i], apvEntSets[5] );
    }

    // Ensure that all none of the entities are already in the newly created sets
    bool qEmpty = true;
    for ( i=0; i<10; i++ )
      for ( int j=0; j<10; j++ )
	qEmpty = qEmpty && !ES.isEntContained( apvEntSets[i], apvEnts[j] );
    TEST ( qEmpty );

    // Add all the entities to each of the sets in two different ways
    // Since the first five sets are lists, they should end up with
    // two copies of each entity
    for ( i=0; i<10; i++ ) {
      ES.addEntArrToSet( saEnts, 10, apvEntSets[i] );
      for ( int j=0; j<10; j++ )
	ES.addEntToSet( apvEnts[j], apvEntSets[i] );
    }

    // Test that all of the entities are in each of the sets
    bool qAllIn = true;
    for ( i=0; i<10; i++ )
      for ( int j=0; j<10; j++ )
	qAllIn = qAllIn && ES.isEntContained(apvEntSets[i], apvEnts[j]);
    TEST ( qAllIn );

    // Remove one copy of the entities using two different function calls
    for ( i=0; i<5; i++ ) {
      ES.rmvEntArrFromSet( saEnts, 10, apvEntSets[i] );
      for ( int j=0; j<10; j++ )
	ES.rmvEntFromSet( apvEnts[j], apvEntSets[i+5] );
    }

    // Check that the lists still contain a copy of each entity
    qAllIn = true;
    for ( i=0; i<5; i++ )
      for ( int j=0; j<10; j++ )
	qAllIn = qAllIn && ES.isEntContained(apvEntSets[i], apvEnts[j]);
    TEST ( qAllIn );

    // Check that the sets no longer have any of the entities
    qEmpty = true;
    for ( i=5; i<10; i++ )
      for ( int j=0; j<10; j++ )
	qEmpty = qEmpty && !ES.isEntContained(apvEntSets[i], apvEnts[j]);
    TEST ( qEmpty );

    // Remove the entities from the lists...
    for ( i=0; i<5; i++ )
      ES.rmvEntArrFromSet( saEnts, 10, apvEntSets[i] );

    // ...and ensure that the lists and sets are indeed empty
    qEmpty = true;
    for ( i=0; i<10; i++ )
      for ( int j=0; j<10; j++ )
	qEmpty = qEmpty && !ES.isEntContained(apvEntSets[i], apvEnts[j]);
    TEST ( qEmpty );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  if ( qErrTests ) {
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    /*
    TSTTM::Mesh icMesh = TSTTM::Factory::newMesh(CREATE_ARG);
    try {
      icMesh.createEntSet( true, apvEntSets[10] );
      cout << "\nERROR: Exception not thrown when trying to create an "
	   << "entity set for a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      icMesh.createEntSet( false, apvEntSets[10] );
      cout << "\nERROR: Exception not thrown when trying to create an "
	   << "entity set for a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      icMesh.destroyEntSet( apvEntSets[10] );
      cout << "\nERROR: Exception not thrown when trying to create an "
	   << "entity set for a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      icMesh.getNumEntSets( pvNMDRoot, 0 );
      cout << "\nERROR: Exception not thrown when trying to create an "
	   << "entity set for a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      icMesh.getEntSets( pvNMDRoot, 0, saSets, iNumSets );
      cout << "\nERROR: Exception not thrown when trying to create an "
	   << "entity set for a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    */
    try {
      ES.destroyEntSet( apvEnts[0] );
      cout << "\nERROR: Exception not thrown when destroying an invalid "
	   << "entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.isList( apvEnts[0] );
      cout << "\nERROR: Exception not thrown when querying whether an invalid "
	   << "entity set is a list! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.getNumEntSets( apvEnts[0], 0 );
      cout << "\nERROR: Exception not thrown when getting the number of sets "
	   << "within an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.getEntSets( apvEnts[0], 0, saSets, iNumSets );
      cout << "\nERROR: Exception not thrown when getting the sets "
	   << "contained within an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.addEntToSet( apvEnts[0], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when adding an entity "
	   << "to an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.addEntToSet( apvEntSets[0], apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when adding an invalid entity "
	   << "to an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      ES.rmvEntFromSet( apvEnts[0], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when removing an entity "
	   << "from an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.rmvEntFromSet( apvEntSets[0], apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when removing an invalid entity "
	   << "from an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      ES.addEntArrToSet( saEnts, 10, apvEnts[0] );
      cout << "\nERROR: Exception not thrown when adding an entity "
	   << "array to an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.addEntArrToSet( saBadEnts, 10, apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when adding an invalid entity "
	   << "array to an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      ES.rmvEntArrFromSet( saEnts, 10, apvEnts[0] );
      cout << "\nERROR: Exception not thrown when removing an entity "
	   << "array from an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.rmvEntArrFromSet( saBadEnts, 10, apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when removing an invalid entity "
	   << "array from an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      ES.addEntSet( apvEntSets[0], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when adding an entity set "
	   << "to an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.addEntSet( apvEnts[0], apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when adding an invalid entity "
	   << "set to an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.addEntSet( apvEntSets[0], pvRootSet );
      cout << "\nERROR: Exception not thrown when adding an entity set "
	   << "to the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ARGUMENT );
    try {
      ES.addEntSet( pvRootSet, apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when adding the root "
	   << "set to an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ARGUMENT );
    try {
      ES.rmvEntSet( apvEntSets[0], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when removing an entity set "
	   << "from an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.rmvEntSet( apvEnts[0], apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when removing an invalid entity "
	   << "set from an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ES.rmvEntSet( apvEntSets[0], pvRootSet );
      cout << "\nERROR: Exception not thrown when removing an entity set "
	   << "from the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ARGUMENT );
    try {
      ES.rmvEntSet( pvRootSet, apvEntSets[0] );
      cout << "\nERROR: Exception not thrown when removing the root "
	   << "set from an entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ARGUMENT );
    cout << "DONE\n";
  }
  try {
    for ( i=0; i<10; i++ ) {
      ES.destroyEntSet( apvEntSets[i] );
    }
  } SHOULD_NOT_THROW;
}

static void vTestSetRelationInterface(TSTTB::SetRelation& SR)
{
  //     addPrntChld, rmvPrntChld, isChildOf, getNumChld, getNumPrnt,
  //     getChldn, getPrnts
  cout << "Testing relations and parent/child queries...             ";
  void* apvSets[10];
  sidl::array<void*> saSets = saSets.create1d(10);
  int i, iNumSets;
  bool qIsChild;
  try {
    // Create ten sets, the first five of which are lists
    for ( i=0; i<10; i++ )
      SR.createEntSet( i<5, apvSets[i] );

    // Create a hierarchy of sets where                     [0]
    //     set 0 is the parent of sets 1-4,           _______|_______
    //     set 1 is the parent of sets 5-7,           [1],[2],[3],[4]
    // and set 5 is the parent of sets 8-9        _____|_____
    //                                           [5],[6],[7]
    //                                         ___|___
    //                                         [8],[9]
    // Also test the isChildOf query
    qIsChild = true;
    for ( i=1; i<5; i++ ) {
      SR.addPrntChld( apvSets[0], apvSets[i] );
      qIsChild = ( qIsChild && SR.isChildOf(apvSets[0], apvSets[i]) );
    }
    for ( i=1; i<5; i++ )
      for ( int j=5; j<8; j++ ) {
	SR.addPrntChld( apvSets[i], apvSets[j] );
	qIsChild = ( qIsChild && SR.isChildOf(apvSets[i], apvSets[j]) );
      }
    for ( i=5; i<8; i++ )
      for ( int j=8; j<10; j++ ) {
	SR.addPrntChld( apvSets[i], apvSets[j] );
	qIsChild = ( qIsChild && SR.isChildOf(apvSets[i], apvSets[j]) );
      }
    TEST ( qIsChild );

  } SHOULD_NOT_THROW_CRITICAL;
  cout << "DONE\n";

  cout << "Testing ability to get children/parents...                ";
  bool qNumsOK;
  try {

    // Test that the number of children and parents for each set is correct
    // for no hops, 1 hop, and -1 (infinite) hops
    // If the numbers do not seem obvious, refer to the diagram above
    qNumsOK = true;
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[0],  0) == 4 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[0],  1) == 7 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[0], -1) == 9 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[0],  0) == 0 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[0],  1) == 0 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[0], -1) == 0 );
    TEST ( qNumsOK );

    qNumsOK = true;
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[1],  0) == 3 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[1],  1) == 5 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[1], -1) == 5 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[1],  0) == 1 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[1],  1) == 1 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[1], -1) == 1 );
    TEST ( qNumsOK );

    qNumsOK = true;
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[5],  0) == 2 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[5],  1) == 2 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[5], -1) == 2 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[5],  0) == 4 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[5],  1) == 5 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[5], -1) == 5 );
    TEST ( qNumsOK );

    qNumsOK = true;
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[8],  0) == 0 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[8],  1) == 0 );
    qNumsOK = qNumsOK && ( SR.getNumChld(apvSets[8], -1) == 0 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[8],  0) == 3 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[8],  1) == 7 );
    qNumsOK = qNumsOK && ( SR.getNumPrnt(apvSets[8], -1) == 8 );
    TEST ( qNumsOK );

    // Check that the sets can return the parents and children
    // counted above
    ///
    SR.getChldn( apvSets[0], 0, saSets, iNumSets );
    bool qGotAllSets = false;
    if ( iNumSets == 4 )
      GOT_ALL_ITEMS ( (apvSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getChldn( apvSets[0], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 7 )
      GOT_ALL_ITEMS ( (apvSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getChldn( apvSets[0], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 9 )
      GOT_ALL_ITEMS ( (apvSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    SR.getPrnts( apvSets[0], 0, saSets, iNumSets );
    TEST ( iNumSets == 0 );
    SR.getPrnts( apvSets[0], 1, saSets, iNumSets );
    TEST ( iNumSets == 0 );
    SR.getPrnts( apvSets[0], -1, saSets, iNumSets );
    TEST ( iNumSets == 0 );

    ///
    SR.getChldn( apvSets[1], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 3 )
      GOT_ALL_ITEMS ( (apvSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getChldn( apvSets[1], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getChldn( apvSets[1], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    void** ppvGotSets;
    SR.getPrnts( apvSets[1], 0, saSets, iNumSets );
    ppvGotSets = saSets.first();
    TEST ( iNumSets == 1 && ppvGotSets[0] == apvSets[0] );
    SR.getPrnts( apvSets[1], 1, saSets, iNumSets );
    ppvGotSets = saSets.first();
    TEST ( iNumSets == 1 && ppvGotSets[0] == apvSets[0] );
    SR.getPrnts( apvSets[1], -1, saSets, iNumSets );
    ppvGotSets = saSets.first();
    TEST ( iNumSets == 1 && ppvGotSets[0] == apvSets[0] );

    ///
    SR.getChldn( apvSets[5], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+8), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getChldn( apvSets[5], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+8), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getChldn( apvSets[5], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+8), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    SR.getPrnts( apvSets[5], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 4 )
      GOT_ALL_ITEMS ( (apvSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getPrnts( apvSets[5], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( apvSets, saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getPrnts( apvSets[5], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( apvSets, saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

    ///
    SR.getChldn( apvSets[8], 0, saSets, iNumSets );
    TEST ( iNumSets == 0 );
    SR.getChldn( apvSets[8], 1, saSets, iNumSets );
    TEST ( iNumSets == 0 );
    SR.getChldn( apvSets[8], -1, saSets, iNumSets );
    TEST ( iNumSets == 0 );

    SR.getPrnts( apvSets[8], 0, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 3 )
      GOT_ALL_ITEMS ( (apvSets+5), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getPrnts( apvSets[8], 1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 7 )
      GOT_ALL_ITEMS ( (apvSets+1), saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );
    SR.getPrnts( apvSets[8], -1, saSets, iNumSets );
    qGotAllSets = false;
    if ( iNumSets == 8 )
      GOT_ALL_ITEMS ( apvSets, saSets, iNumSets, qGotAllSets );
    TEST ( qGotAllSets );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  double dData;
  void* pvData = &dData;
  if ( qErrTests ) {
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      SR.addPrntChld( pvRootSet, apvSets[0] );
      cout << "\nERROR: Exception not thrown when establishing a parent-"
	   << "child relationship with the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( apvSets[0], pvRootSet );
      cout << "\nERROR: Exception not thrown when establishing a parent-"
	   << "child relationship with the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( pvData, apvSets[0] );
      cout << "\nERROR: Exception not thrown when establishing a parent-"
	   << "child relationship with an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( apvSets[0], pvData );
      cout << "\nERROR: Exception not thrown when establishing a parent-"
	   << "child relationship with an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( apvSets[0], apvSets[0] );
      cout << "\nERROR: Exception not thrown when establishing a cyclic "
	   << "parent-child relationship between a set and itself! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( apvSets[1], apvSets[0] );
      cout << "\nERROR: Exception not thrown when establishing a cyclic "
	   << "parent-child relationship! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( apvSets[5], apvSets[0] );
      cout << "\nERROR: Exception not thrown when establishing a cyclic "
	   << "parent-child relationship! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.addPrntChld( apvSets[8], apvSets[0] );
      cout << "\nERROR: Exception not thrown when establishing a cyclic "
	   << "parent-child relationship! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.rmvPrntChld( pvRootSet, apvSets[0] );
      cout << "\nERROR: Exception not thrown when removing a parent-"
	   << "child relationship with the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.rmvPrntChld( apvSets[0], pvRootSet );
      cout << "\nERROR: Exception not thrown when removing a parent-"
	   << "child relationship with the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.rmvPrntChld( pvData, apvSets[0] );
      cout << "\nERROR: Exception not thrown when removing a parent-"
	   << "child relationship with an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.rmvPrntChld( apvSets[0], pvData );
      cout << "\nERROR: Exception not thrown when removing a parent-"
	   << "child relationship with an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.isChildOf( pvRootSet, apvSets[0] );
      cout << "\nERROR: Exception not thrown when checking a parent-"
	   << "child relationship with the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.isChildOf( apvSets[0], pvRootSet );
      cout << "\nERROR: Exception not thrown when checking a parent-"
	   << "child relationship with the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.isChildOf( pvData, apvSets[0] );
      cout << "\nERROR: Exception not thrown when checking a parent-"
	   << "child relationship with an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.isChildOf( apvSets[0], pvData );
      cout << "\nERROR: Exception not thrown when checking a parent-"
	   << "child relationship with an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getNumChld( pvRootSet, -1 );
      cout << "\nERROR: Exception not thrown when getting number of "
	   << "children for the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getNumChld( pvData, -1 );
      cout << "\nERROR: Exception not thrown when getting number of "
	   << "children for an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getNumPrnt( pvRootSet, -1 );
      cout << "\nERROR: Exception not thrown when getting number of "
	   << "parents for the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getNumPrnt( pvData, -1 );
      cout << "\nERROR: Exception not thrown when getting number of "
	   << "parents for an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getChldn( pvRootSet, -1, saSets, iNumSets );
      cout << "\nERROR: Exception not thrown when getting "
	   << "children for the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getChldn( pvData, -1, saSets, iNumSets );
      cout << "\nERROR: Exception not thrown when getting "
	   << "children for an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getPrnts( pvRootSet, -1, saSets, iNumSets );
      cout << "\nERROR: Exception not thrown when getting "
	   << "parents for the root set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      SR.getPrnts( pvData, -1, saSets, iNumSets );
      cout << "\nERROR: Exception not thrown when getting "
	   << "parents for an invalid entity set! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    cout << "DONE\n";
  }

  cout << "Testing relation removal...                               ";
  try {
    // Ensure that the parent-child relations can be removed without throwing
    // an error an that the query reflects this removal
    for ( i=1; i<5; i++ ) {
      SR.rmvPrntChld( apvSets[0], apvSets[i] );
      TEST ( !SR.isChildOf(apvSets[0], apvSets[i]) );
    }
    for ( i=5; i<8; i++ ) {
      SR.rmvPrntChld( apvSets[1], apvSets[i] );
      TEST ( !SR.isChildOf(apvSets[1], apvSets[i]) );
    }
    for ( i=8; i<10; i++ ) {
      SR.rmvPrntChld( apvSets[5], apvSets[i] );
      TEST ( !SR.isChildOf(apvSets[5], apvSets[i]) );
    }

    // Clean up the sets
    for ( i=0; i<10; i++ ) {
      SR.destroyEntSet( apvSets[i] );
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";
}

static void vTestBoolOpsInterface(TSTTM::Mesh& M)
{
  cout << "Testing methods with regular sets and lists...\n";
  TSTTB::SetBoolOps BO(M);
  TSTTM::Entity EntMesh(M);
  void* apvSets[10];
  void* apvEnts[10];
  void* pvIter;
  sidl::array<void*> saEnts = saEnts.create1d(10),
		     saBadEnts = saBadEnts.create1d(10);
  int i;
  try {
    // Get some entities to work with
    if ( !EntMesh.initEntIter( pvRootSet, TSTTM::EntityType_FACE,
			  TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    for ( i=0; i<10; i++ ) {
      if ( !EntMesh.getNextEntIter(pvIter, apvEnts[i]) ) {
	cout << "\nCould not get enough mesh data to work with... exiting...\n";
	TEST ( data_preparation );
	return;
      }
      saEnts.set(i, apvEnts[i]);
    }
    EntMesh.endEntIter( pvIter );

    // Create a number of entity sets for testing
    // Call the entities in use a,b,c,d,e,f,g,h,i,j (indices 0-9)
    // Entity set 0 is a list and contains      : a,b,c,d,e,a,b,c,d,e,E1,E2,E3,E4,E5
    // Entity set 1 is a list and contains      : g,f,e,d,E2,E3,E4,E5
    // Entity set 2 is a list and contains      : f,f,g,g,h,h,i,i,j,j
    // Entity set 3 is not a list and contains  : a,b,c,d,e,E1
    // Entity set 4 is not a list and contains  : d,e,f,g,E1,E2
    // Entity set 5 is not a list and contains  : f,g,h,i,j
    for ( i=0; i<6; i++ ) {
      BO.createEntSet(i<3, apvSets[i]);
    }
    for ( i=0; i<5; i++ ) {
      BO.addEntToSet( apvEnts[i], apvSets[0] );
      BO.addEntSet( apvSets[i+1], apvSets[0] );
      BO.addEntToSet( apvEnts[i], apvSets[3] );
    }
    for ( i=0; i<5; i++ )
      BO.addEntToSet( apvEnts[i], apvSets[0] );
    for ( i=3; i<7; i++ ) {
      BO.addEntToSet( apvEnts[9-i], apvSets[1] );
      BO.addEntSet( apvSets[i-1], apvSets[1] );
      BO.addEntToSet( apvEnts[i], apvSets[4] );
      BO.addEntSet( apvSets[(i+1)/3], apvSets[4] );
    }
    for ( i=5; i<10; i++ ) {
      BO.addEntToSet( apvEnts[i], apvSets[2] );
      BO.addEntToSet( apvEnts[i], apvSets[2] );
      BO.addEntToSet( apvEnts[i], apvSets[5] );
    }
    BO.addEntSet( apvSets[1], apvSets[3] );
  } SHOULD_NOT_THROW_CRITICAL;

  cout << "Testing intersect with regular lists...                   ";
  try {
    // E0 n E2; expect empty list
    BO.intersect( apvSets[0], apvSets[2], apvSets[6] );
    TEST ( BO.isList(apvSets[6]) );
    TEST ( BO.getNumEntSets(apvSets[6], 0) == 0 );
    bool qIsEmpty = true;
    for ( i=0; i<10; i++ )
      qIsEmpty = ( qIsEmpty && !BO.isEntContained(apvSets[6], apvEnts[i]) );
    TEST ( qIsEmpty );

    // E0 n E1; expect d,e,E2,E3,E4,E5
    BO.intersect( apvSets[0], apvSets[1], apvSets[7] );
    TEST ( BO.isList(apvSets[7]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 4 )
      GOT_ALL_ITEMS ( (apvSets+2), saGotSets, 4, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    void **ppvGotEnts;
    if ( iNumEnts == 2 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      qGotAllItems = qGotAllItems && ( ppvGotEnts[0] == apvEnts[3] );
      qGotAllItems = qGotAllItems && ( ppvGotEnts[1] == apvEnts[4] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E1 n E0; expect e,d,E2,E3,E4,E5
    BO.intersect( apvSets[1], apvSets[0], apvSets[8] );
    TEST ( BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 4 )
      GOT_ALL_ITEMS ( (apvSets+2), saGotSets, 4, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 2 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      qGotAllItems = qGotAllItems && ( ppvGotEnts[0] == apvEnts[4] );
      qGotAllItems = qGotAllItems && ( ppvGotEnts[1] == apvEnts[3] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E1 n E2; expect g,f
    BO.intersect( apvSets[1], apvSets[2], apvSets[9] );
    TEST ( BO.isList(apvSets[9]) );
    TEST ( BO.getNumEntSets(apvSets[9], 0) == 0 );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 2 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      qGotAllItems = qGotAllItems && ( ppvGotEnts[0] == apvEnts[6] );
      qGotAllItems = qGotAllItems && ( ppvGotEnts[1] == apvEnts[5] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing intersect with regular sets...                    ";
  try {
    // E3 n E5; expect empty set
    BO.intersect( apvSets[3], apvSets[5], apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    TEST ( BO.getNumEntSets(apvSets[6], 0) == 0 );
    bool qIsEmpty = true;
    for ( i=0; i<10; i++ )
      qIsEmpty = ( qIsEmpty && !BO.isEntContained(apvSets[6], apvEnts[i]) );
    TEST ( qIsEmpty );

    // E3 n E4; expect d,e,E1 (unordered)
    BO.intersect( apvSets[3], apvSets[4], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    if ( iNumSets == 1 )
      TEST ( *saGotSets.first() == apvSets[1] );
    else
      TEST ( false );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    bool qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 n E3; expect d,e,E1 (unordered)
    BO.intersect( apvSets[4], apvSets[3], apvSets[8] );
    TEST ( !BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    if ( iNumSets == 1 )
      TEST ( *saGotSets.first() == apvSets[1] );
    else
      TEST ( false );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 n E5; expect f,g (unordered)
    BO.intersect( apvSets[4], apvSets[5], apvSets[9] );
    TEST ( !BO.isList(apvSets[9]) );
    TEST ( BO.getNumEntSets(apvSets[9], 0) == 0 );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+5), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing intersect with sets and lists...                  ";
  try {
    // E0 n E5; expect empty set
    BO.intersect( apvSets[0], apvSets[5], apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    TEST ( BO.getNumEntSets(apvSets[6], 0) == 0 );
    bool qIsEmpty = true;
    for ( i=0; i<10; i++ )
      qIsEmpty = ( qIsEmpty && !BO.isEntContained(apvSets[6], apvEnts[i]) );
    TEST ( qIsEmpty );

    // E0 n E4; expect d,e,E1,E2 (unordered)
    BO.intersect( apvSets[0], apvSets[4], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 n E0; expect d,e,E1,E2 (unordered)
    BO.intersect( apvSets[4], apvSets[0], apvSets[8] );
    TEST ( !BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 n E2; expect f,g (unordered)
    BO.intersect( apvSets[4], apvSets[2], apvSets[9] );
    TEST ( !BO.isList(apvSets[9]) );
    TEST ( BO.getNumEntSets(apvSets[9], 0) == 0 );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+5), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing subtract with regular lists...                    ";
  try {
    // E0 - E2; expect E0 = a,b,c,d,e,a,b,c,d,e,E1,E2,E3,E4,E5
    BO.subtract( apvSets[0], apvSets[2], apvSets[6] );
    TEST ( BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, 5, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    void **ppvGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 10 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      for ( i=0; i<10; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[i%5] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E0 - E1; expect a,b,c,d,e,a,b,c,E1
    BO.subtract( apvSets[0], apvSets[1], apvSets[7] );
    TEST ( BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    if ( iNumSets == 1 )
      TEST ( *saGotSets.first() == apvSets[1] );
    else
      TEST ( false );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 8 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      for ( i=0; i<8; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[i%5] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E1 - E0; expect g,f
    BO.subtract( apvSets[1], apvSets[0], apvSets[8] );
    TEST ( BO.isList(apvSets[8]) );
    TEST ( BO.getNumEntSets(apvSets[8], 0) == 0 );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 2 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      qGotAllItems = qGotAllItems && ( ppvGotEnts[0] == apvEnts[6] );
      qGotAllItems = qGotAllItems && ( ppvGotEnts[1] == apvEnts[5] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E2 - E1; expect f,g,h,h,i,i,j,j
    BO.subtract( apvSets[2], apvSets[1], apvSets[9] );
    TEST ( BO.isList(apvSets[9]) );
    TEST ( BO.getNumEntSets(apvSets[9], 0) == 0 );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 8 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      qGotAllItems = qGotAllItems && ( ppvGotEnts[0] == apvEnts[5] );
      qGotAllItems = qGotAllItems && ( ppvGotEnts[1] == apvEnts[6] );
      for ( i=2; i<8; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[(i/2)+6] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing subtract with regular sets...                     ";
  try {
    // E3 - E5; expect E3 = a,b,c,d,e,E1 (unordered)
    BO.subtract( apvSets[3], apvSets[5], apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets = saGotSets.create1d(2);
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    TEST ( iNumSets == 1 );
    if ( iNumSets == 1 )
      TEST ( *saGotSets.first() == apvSets[1] );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    bool qGotAllItems = false;
    if ( iNumEnts == 5 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, 5, qGotAllItems );
    TEST ( qGotAllItems );

    // E3 - E4; expect a,b,c (unordered)
    BO.subtract( apvSets[3], apvSets[4], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    TEST ( BO.getNumEntSets(apvSets[7], 0) == 0 );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 3 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, 3, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 - E3; expect f,g,E2 (unordered)
    BO.subtract( apvSets[4], apvSets[3], apvSets[8] );
    TEST ( !BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    TEST ( iNumSets == 1 );
    if ( iNumSets == 1 )
      TEST ( *saGotSets.first() == apvSets[2] );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+5), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 - E5; expect d,e,E1,E2 (unordered)
    BO.subtract( apvSets[4], apvSets[5], apvSets[9] );
    TEST ( !BO.isList(apvSets[9]) );
    BO.getEntSets( apvSets[9], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, 2, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing subtract with lists and sets...                   ";
  try {
    // E0 - E5; expect a,b,c,d,e,E1,E2,E3,E4,E5 (unordered)
    BO.subtract( apvSets[0], apvSets[5], apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 5 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E0 - E4; expect a,b,c,E3,E4,E5 (unordered)
    BO.subtract( apvSets[0], apvSets[4], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 3 )
      GOT_ALL_ITEMS ( (apvSets+3), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 3 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, 3, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 - E0; expect f,g (unordered)
    BO.subtract( apvSets[4], apvSets[0], apvSets[8] );
    TEST ( !BO.isList(apvSets[8]) );
    TEST ( BO.getNumEntSets(apvSets[8], 0) == 0 );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+5), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 - E2; expect d,e,E1,E2 (unordered)
    BO.subtract( apvSets[4], apvSets[2], apvSets[9] );
    TEST ( !BO.isList(apvSets[9]) );
    BO.getEntSets( apvSets[9], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, 2, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 2 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, 2, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing unite with regular lists...                       ";
  try {
    // E0 U E2; expect a,b,c,d,e,a,b,c,d,e,f,f,g,g,h,h,i,i,j,j,E1,E2,E3,E4,E5
    BO.unite( apvSets[0], apvSets[2], apvSets[6] );
    TEST ( BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, 5, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    void **ppvGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 20 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      for ( i=0; i<10; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[i%5] );
      for ( i=10; i<20; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[i/2] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E0 U E1; expect a,b,c,d,e,a,b,c,d,e,g,f,e,d,E1,E2,E3,E4,E5
    BO.unite( apvSets[0], apvSets[1], apvSets[7] );
    TEST ( BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, 5, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 14 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      for ( i=0; i<10; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[i%5] );
      for ( i=10; i<14; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[16-i] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E1 U E0; expect g,f,e,d,a,b,c,d,e,a,b,c,d,e,E1,E2,E3,E4,E5
    BO.unite( apvSets[1], apvSets[0], apvSets[8] );
    TEST ( BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, 5, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 14 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      for ( i=0; i<4; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[6-i] );
      for ( i=4; i<14; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[(i-4)%5] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    // E1 U E2; expect g,f,e,d,f,f,g,g,h,h,i,i,j,j,E2,E3,E4,E5
    BO.unite( apvSets[1], apvSets[2], apvSets[9] );
    TEST ( BO.isList(apvSets[9]) );
    BO.getEntSets( apvSets[9], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 4 )
      GOT_ALL_ITEMS ( (apvSets+2), saGotSets, 4, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    TEST ( iNumEnts == 14 );
    if ( iNumEnts == 14 ) {
      qGotAllItems = true;
      ppvGotEnts = saGotEnts.first();
      for ( i=0; i<4; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[6-i] );
      for ( i=4; i<14; i++ )
	qGotAllItems = qGotAllItems && ( ppvGotEnts[i] == apvEnts[(i/2)+3] );
    }
    else
      qGotAllItems = false;
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing unite with regular sets...                        ";
  try {
    // E3 U E5; expect a,b,c,d,e,f,g,h,i,j,E1 (unordered)
    BO.unite( apvSets[3], apvSets[5], apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets = saGotSets.create1d(2);
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    TEST ( iNumSets == 1 );
    if ( iNumSets == 1 )
      TEST ( *saGotSets.first() == apvSets[1] );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    bool qGotAllItems = false;
    if ( iNumEnts == 10 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E3 U E4; expect a,b,c,d,e,f,g,E1,E2 (unordered)
    BO.unite( apvSets[3], apvSets[4], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 7 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 U E3; expect a,b,c,d,e,f,g,E1,E2 (unordered)
    BO.unite( apvSets[4], apvSets[3], apvSets[8] );
    TEST ( !BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 7 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 U E5; expect d,e,f,g,h,i,j,E1,E2 (unordered)
    BO.unite( apvSets[4], apvSets[5], apvSets[9] );
    TEST ( !BO.isList(apvSets[9]) );
    BO.getEntSets( apvSets[9], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 7 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing unite with lists and sets...                      ";
  try {
    // E0 U E5; expect a,b,c,d,e,f,g,h,i,j,E1,E2,E3,E4,E5 (unordered)
    BO.unite( apvSets[0], apvSets[5], apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 10 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E0 U E4; expect a,b,c,d,e,f,g,E1,E2,E3,E4,E5 (unordered)
    BO.unite( apvSets[0], apvSets[4], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 7 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 U E0; expect a,b,c,d,e,f,g,E1,E2,E3,E4,E5 (unordered)
    BO.unite( apvSets[4], apvSets[0], apvSets[8] );
    TEST ( !BO.isList(apvSets[8]) );
    BO.getEntSets( apvSets[8], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[8], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 7 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // E4 U E2; expect d,e,f,g,h,i,j,E1,E2 (unordered)
    BO.unite( apvSets[4], apvSets[2], apvSets[9] );
    TEST ( !BO.isList(apvSets[9]) );
    BO.getEntSets( apvSets[9], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 2 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[9], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 7 )
      GOT_ALL_ITEMS ( (apvEnts+3), saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<10; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing methods with the root set... may take a while...\n";

  cout << "Testing intersect with the root set...                    ";
  try {
    // E0 n R; expect a,b,c,d,e,E1,E2,E3,E4,E5 (unordered)
    BO.intersect( apvSets[0], pvRootSet, apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 5 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // R n E0; expect a,b,c,d,e,E1,E2,E3,E4,E5 (unordered)
    BO.intersect( pvRootSet, apvSets[0], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 5 )
      GOT_ALL_ITEMS ( (apvSets+1), saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == 5 )
      GOT_ALL_ITEMS ( apvEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<8; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing subtract with the root set...                     ";
  try {
    // E0 - R; expect empty set
    BO.subtract( apvSets[0], pvRootSet, apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    TEST ( BO.getNumEntSets(apvSets[6], 0) == 0 );
    bool qIsEmpty = true;
    for ( i=0; i<10; i++ )
      qIsEmpty = ( qIsEmpty && !BO.isEntContained(apvSets[6], apvEnts[i]) );
    TEST ( qIsEmpty );

    // R - E0; expect the complement of E0 (unordered; includes E0 and E6)
    BO.subtract( pvRootSet, apvSets[0], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    sidl::array<void*> saGotSets;
    int iNumSets;
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 2 ) {
      void** ppvGotSets = saGotSets.first();
      qGotAllItems = ( (ppvGotSets[0] == apvSets[0] ||
			ppvGotSets[1] == apvSets[0]) &&
		       (ppvGotSets[0] == apvSets[6] ||
			ppvGotSets[1] == apvSets[6]) );
    }
    TEST ( qGotAllItems );
    int iNumEnts;
    sidl::array<void*> saGotEnts;
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    int iNumAllEnts = M.getNumOfType(pvRootSet, TSTTM::EntityType_VERTEX) +
      M.getNumOfType(pvRootSet, TSTTM::EntityType_EDGE) +
      M.getNumOfType(pvRootSet, TSTTM::EntityType_FACE) +
      M.getNumOfType(pvRootSet, TSTTM::EntityType_REGION);
    void** ppvGotEnts;
    if ( iNumEnts == iNumAllEnts-5 ) {
      qIsEmpty = true;
      ppvGotEnts = saGotEnts.first();
      for ( i = 0; i < iNumEnts; i++ )
	qIsEmpty = ( qIsEmpty && !BO.isEntContained(apvSets[0], ppvGotEnts[i]) );
    }
    else
      qIsEmpty = false;
    TEST ( qIsEmpty );

    for ( i=6; i<8; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing unite with the root set...                        ";
  try {
    // E0 U R; expect a copy of R (unordered)
    BO.unite( apvSets[0], pvRootSet, apvSets[6] );
    TEST ( !BO.isList(apvSets[6]) );
    sidl::array<void*> saGotSets = saGotSets.create1d(7);
    int iNumSets;
    BO.getEntSets( apvSets[6], 0, saGotSets, iNumSets );
    bool qGotAllItems = false;
    if ( iNumSets == 6 )
      GOT_ALL_ITEMS ( apvSets, saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    int iNumEnts;
    int iNumAllEnts = M.getNumOfType(pvRootSet, TSTTM::EntityType_VERTEX) +
      M.getNumOfType(pvRootSet, TSTTM::EntityType_EDGE) +
      M.getNumOfType(pvRootSet, TSTTM::EntityType_FACE) +
      M.getNumOfType(pvRootSet, TSTTM::EntityType_REGION);
    sidl::array<void*> saGotEnts, saAllEnts;
    M.getEntities( apvSets[6], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    TEST ( iNumEnts == iNumAllEnts );
    M.getEntities( pvRootSet, TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saAllEnts, iNumAllEnts );
    qGotAllItems = false;
    void** ppvAllEnts = saAllEnts.first();
    if ( iNumEnts == iNumAllEnts )
      GOT_ALL_ITEMS ( ppvAllEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    // R U E0; expect a copy of R (unordered)
    BO.unite( pvRootSet, apvSets[0], apvSets[7] );
    TEST ( !BO.isList(apvSets[7]) );
    BO.getEntSets( apvSets[7], 0, saGotSets, iNumSets );
    qGotAllItems = false;
    if ( iNumSets == 7 )
      GOT_ALL_ITEMS ( apvSets, saGotSets, iNumSets, qGotAllItems );
    TEST ( qGotAllItems );
    M.getEntities( apvSets[7], TSTTM::EntityType_ALL_TYPES,
		    TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    qGotAllItems = false;
    if ( iNumEnts == iNumAllEnts )
      GOT_ALL_ITEMS ( ppvAllEnts, saGotEnts, iNumEnts, qGotAllItems );
    TEST ( qGotAllItems );

    for ( i=6; i<8; i++ )
      BO.destroyEntSet( apvSets[i] );

  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  if ( qErrTests ) {
    void* pvBadSet = pvIter;
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      BO.subtract( apvSets[0], apvEnts[0], pvBadSet  );
      cout << "\nERROR: Exception not thrown when performing subtraction for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      BO.destroyEntSet( pvBadSet );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      BO.subtract( apvEnts[0], apvSets[0], pvBadSet  );
      cout << "\nERROR: Exception not thrown when performing subtraction for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      BO.destroyEntSet( pvBadSet );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      BO.intersect( apvSets[0], apvEnts[0], pvBadSet  );
      cout << "\nERROR: Exception not thrown when performing intersection for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      BO.destroyEntSet( pvBadSet );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      BO.intersect( apvEnts[0], apvSets[0], pvBadSet  );
      cout << "\nERROR: Exception not thrown when performing intersection for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      BO.destroyEntSet( pvBadSet );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      BO.unite( apvSets[0], apvEnts[0], pvBadSet  );
      cout << "\nERROR: Exception not thrown when performing union for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      BO.destroyEntSet( pvBadSet );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      BO.unite( apvEnts[0], apvSets[0], pvBadSet  );
      cout << "\nERROR: Exception not thrown when performing union for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      BO.destroyEntSet( pvBadSet );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    cout << "DONE\n";
  }

  // Clean up the sets
  try {
    for ( i=0; i<6; i++ )
      BO.destroyEntSet( apvSets[i] );
  } SHOULD_NOT_THROW;

}

static void vTestMeshWithSets(TSTTM::Mesh& M)
{
  //    getNumOfType, getNumOfTopo, getAllVtxCoords,
  //    getVtxCoordIndex, getEntities, and getAdjEntities
  TSTTM::Entity EntMesh(M);
  TSTTM::Arr ArrMesh(M);
  TSTTB::EntSet ES(M);
  const int iNumType = TSTTM::EntityType_ALL_TYPES - TSTTM::EntityType_VERTEX;
  const int iNumTopo = TSTTM::EntityTopology_ALL_TOPOLOGIES - TSTTM::EntityTopology_POINT;
  void* apvSets[5];
  void* a2pvTypeEnts[iNumType][iNumType*5];
  void* pvIter;
  void* a2pvTopoEnts[iNumTopo][iNumTopo*5];
  int aiNumTypeEnts[iNumType] = {0};
  int aiNumTopoEnts[iNumTopo] = {0};
  int i, iGeoDim;
  bool qGotData;

  try {
    iGeoDim = M.getGeometricDim();
    // Get a different number of entities for each type and record how many were retrieved
    // --a2pvTypeEnts stores the entities, aiNumTypeEnts stores how many for each type
    qGotData = false;
    for ( i = 0; i < iNumType; i++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityType_FACE) )
	break;
      if ( EntMesh.initEntIter(pvRootSet, static_cast<TSTTM::EntityType>(i+TSTTM::EntityType_VERTEX),
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
	int &j = aiNumTypeEnts[i];
	while ( j < 5*(iNumType-i) && EntMesh.getNextEntIter(pvIter, a2pvTypeEnts[i][j]) )
	  j++;
	EntMesh.endEntIter( pvIter );
	if ( i > 0 )
	  qGotData = qGotData || (j > 0);
      }
    }
    // If there aren't the required number of vertices (20 to be exact) or there aren't any
    // of any of the other types of entities, bail out because the subsequent tests would be
    // useless and might not even work
    if ( !qGotData || (aiNumTypeEnts[0] <= 0) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }

    // Get a different number of entities for each topology and record how many were retrieved
    // --a2pvTopoEnts stores the entities, aiNumTopoEnts stores how many for each topology
    qGotData = false;
    for ( i = 0; i < iNumTopo; i++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      if ( EntMesh.initEntIter(pvRootSet, TSTTM::EntityType_ALL_TYPES,
			      static_cast<TSTTM::EntityTopology>(i+TSTTM::EntityTopology_POINT), pvIter) ) {
	int &j = aiNumTopoEnts[i];
	while ( j < 5*(iNumTopo-i) && EntMesh.getNextEntIter(pvIter, a2pvTopoEnts[i][j]) )
	  j++;
	EntMesh.endEntIter( pvIter );
	if ( i > 0 )
	  qGotData = qGotData || (j > 0);
      }
    }
    // If there aren't the required number of points (55 to be exact) or there aren't any
    // of any of the other topologies of entities, bail out because the subsequent tests would be
    // useless and might not even work
    if ( !qGotData || (aiNumTopoEnts[0] <= 0) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }

    // Create two lists and two sets, one for types and one for topologies each,
    // as well as an empty list
    for ( i=0; i<5; i++ )
      ES.createEntSet( i<3, apvSets[i] );

    // Add all of the type entities to list 1 and set 3
    for ( i=0; i<iNumType; i++ )
      for ( int j=0; j<aiNumTypeEnts[i]; j++ ) {
	ES.addEntToSet( a2pvTypeEnts[i][j], apvSets[1] );
	ES.addEntToSet( a2pvTypeEnts[i][j], apvSets[3] );
      }
    // Double the number of vertices for the list
    for ( i=0; i<aiNumTypeEnts[0]; i++ )
      ES.addEntToSet( a2pvTypeEnts[0][i], apvSets[1] );

    // Add all of the topology entities to list 2 and set 4
    for ( i=0; i<iNumTopo; i++ )
      for ( int j=0; j<aiNumTopoEnts[i]; j++ ) {
	ES.addEntToSet( a2pvTopoEnts[i][j], apvSets[2] );
	ES.addEntToSet( a2pvTopoEnts[i][j], apvSets[4] );
      }
    // Double the number of points for the list
    for ( i=0; i<aiNumTopoEnts[0]; i++ )
      ES.addEntToSet( a2pvTopoEnts[0][i], apvSets[2] );

  } SHOULD_NOT_THROW_CRITICAL;

  cout << "Testing basic queries...                                  ";
  try {
    // Ensure that the correct number of each type/topology of entities is returned
    bool qNumOK0 = true, qNumOK1 = true, qNumOK2 = true, qNumOK3 = true, qNumOK4 = true;
    // The vertices have to be handled separately because they were added twice to the list
    qNumOK0 = qNumOK0 && ( M.getNumOfType(apvSets[0], TSTTM::EntityType_VERTEX) == 0 );
    qNumOK1 = qNumOK1 && ( M.getNumOfType(apvSets[1], TSTTM::EntityType_VERTEX)
			   == 2*aiNumTypeEnts[0] );
    qNumOK3 = qNumOK3 && ( M.getNumOfType(apvSets[3], TSTTM::EntityType_VERTEX)
			   == aiNumTypeEnts[0] );
    for ( int iType = TSTTM::EntityType_VERTEX+1;
	  iType < TSTTM::EntityType_ALL_TYPES; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      qNumOK0 = qNumOK0 && ( M.getNumOfType(apvSets[0], static_cast<TSTTM::EntityType>(iType))
			     == 0 );
      qNumOK1 = qNumOK1 && ( M.getNumOfType(apvSets[1], static_cast<TSTTM::EntityType>(iType))
			     == aiNumTypeEnts[iType-TSTTM::EntityType_VERTEX] );
      qNumOK3 = qNumOK3 && ( M.getNumOfType(apvSets[3], static_cast<TSTTM::EntityType>(iType))
			     == aiNumTypeEnts[iType-TSTTM::EntityType_VERTEX] );
    }
    // The points have to be handled separately because they were added twice to the list
    qNumOK0 = qNumOK0 && ( M.getNumOfTopo(apvSets[0], TSTTM::EntityTopology_POINT) == 0 );
    qNumOK2 = qNumOK2 && ( M.getNumOfTopo(apvSets[2], TSTTM::EntityTopology_POINT)
			   == 2*aiNumTopoEnts[0] );
    qNumOK4 = qNumOK4 && ( M.getNumOfTopo(apvSets[4], TSTTM::EntityTopology_POINT)
			   == aiNumTopoEnts[0] );
    for ( int iTopo = TSTTM::EntityTopology_POINT+1;
	  iTopo < TSTTM::EntityTopology_ALL_TOPOLOGIES; iTopo++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      qNumOK0 = qNumOK0 && ( M.getNumOfTopo(apvSets[0], static_cast<TSTTM::EntityTopology>(iTopo))
			     == 0 );
      qNumOK2 = qNumOK2 && ( M.getNumOfTopo(apvSets[2], static_cast<TSTTM::EntityTopology>(iTopo))
			     == aiNumTopoEnts[iTopo-TSTTM::EntityTopology_POINT] );
      qNumOK4 = qNumOK4 && ( M.getNumOfTopo(apvSets[4], static_cast<TSTTM::EntityTopology>(iTopo))
			     == aiNumTopoEnts[iTopo-TSTTM::EntityTopology_POINT] );
    }
    TEST ( qNumOK0 );
    TEST ( qNumOK1 );
    TEST ( qNumOK2 );
    TEST ( qNumOK3 );
    TEST ( qNumOK4 );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  sidl::array<void*> saGotEnts;
  void** ppvGotEnts;
  int iNumEnts;
  cout << "Testing entity retrieval...                               ";
  try {
    // Try to retrieve the entities for each type and topology
    // Set #0 is empty
    M.getEntities( apvSets[0], TSTTM::EntityType_VERTEX,
			TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    TEST ( iNumEnts == 0 );
    // List #1 has double the vertices
    saGotEnts = saGotEnts.create1d( 2*aiNumTypeEnts[0] );
    M.getEntities( apvSets[1], TSTTM::EntityType_VERTEX,
			TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == 2*aiNumTypeEnts[0] ) {
      qGotData = true;
      ppvGotEnts = saGotEnts.first();
      // They should come back in the same order, two times over
      for ( i = 0; i < iNumEnts; i++ )
	qGotData = qGotData && ( ppvGotEnts[i] == a2pvTypeEnts[0][(i%aiNumTypeEnts[0])] );
    }
    else
      qGotData = false;
    TEST ( qGotData );
    // Set #3 has the vertices once, but the order is not guaranteed
    qGotData = false;
    M.getEntities( apvSets[3], TSTTM::EntityType_VERTEX,
			TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
    if ( iNumEnts == aiNumTypeEnts[0] )
      GOT_ALL_ITEMS ( a2pvTypeEnts[0], saGotEnts, iNumEnts, qGotData );
    TEST ( qGotData );

    // Repeat similar tests for each type
    for ( int iType = 1; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityType_FACE) )
	break;
      M.getEntities( apvSets[0], static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
			  TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
      TEST ( iNumEnts == 0 );
      saGotEnts = saGotEnts.create1d( aiNumTypeEnts[iType] );
      M.getEntities( apvSets[1], static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
			  TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
      if ( iNumEnts == aiNumTypeEnts[iType] ) {
	qGotData = true;
	ppvGotEnts = saGotEnts.first();
	for ( i = 0; i < iNumEnts; i++ )
	  qGotData = qGotData && ( ppvGotEnts[i] == a2pvTypeEnts[iType][i] );
      }
      else
	qGotData = false;
      TEST ( qGotData );
      qGotData = false;
      M.getEntities( apvSets[3], static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
			  TSTTM::EntityTopology_ALL_TOPOLOGIES, saGotEnts, iNumEnts );
      if ( iNumEnts == aiNumTypeEnts[iType] )
	GOT_ALL_ITEMS ( a2pvTypeEnts[iType], saGotEnts, iNumEnts, qGotData );
      TEST ( qGotData );
    }

    // Essentially the same thing for topologies
    M.getEntities( apvSets[0], TSTTM::EntityType_ALL_TYPES,
			TSTTM::EntityTopology_POINT, saGotEnts, iNumEnts );
    TEST ( iNumEnts == 0 );
    saGotEnts = saGotEnts.create1d( 2*aiNumTopoEnts[0] );
    M.getEntities( apvSets[2], TSTTM::EntityType_ALL_TYPES ,
			TSTTM::EntityTopology_POINT, saGotEnts, iNumEnts );
    if ( iNumEnts == 2*aiNumTopoEnts[0] ) {
      qGotData = true;
      ppvGotEnts = saGotEnts.first();
      for ( i = 0; i < iNumEnts; i++ )
	qGotData = qGotData && ( ppvGotEnts[i] == a2pvTopoEnts[0][(i%aiNumTopoEnts[0])] );
    }
    else
      qGotData = false;
    TEST ( qGotData );
    qGotData = false;
    M.getEntities( apvSets[4], TSTTM::EntityType_ALL_TYPES,
			TSTTM::EntityTopology_POINT, saGotEnts, iNumEnts );
    if ( iNumEnts == aiNumTopoEnts[0] )
      GOT_ALL_ITEMS ( a2pvTopoEnts[0], saGotEnts, iNumEnts, qGotData );
    TEST ( qGotData );

    for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      M.getEntities( apvSets[0], TSTTM::EntityType_ALL_TYPES,
			  static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
			  saGotEnts, iNumEnts );
      TEST ( iNumEnts == 0 );
      saGotEnts = saGotEnts.create1d( aiNumTopoEnts[iTopo] );
      M.getEntities( apvSets[2], TSTTM::EntityType_ALL_TYPES,
			  static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
			  saGotEnts, iNumEnts );
      if ( iNumEnts == aiNumTopoEnts[iTopo] ) {
	qGotData = true;
	ppvGotEnts = saGotEnts.first();
	for ( i = 0; i < iNumEnts; i++ )
	  qGotData = qGotData && ( ppvGotEnts[i] == a2pvTopoEnts[iTopo][i] );
      }
      else
	qGotData = false;
      TEST ( qGotData );
      qGotData = false;
      M.getEntities( apvSets[4], TSTTM::EntityType_ALL_TYPES,
			  static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
			  saGotEnts, iNumEnts );
      if ( iNumEnts == aiNumTopoEnts[iTopo] )
	GOT_ALL_ITEMS ( a2pvTopoEnts[iTopo], saGotEnts, iNumEnts, qGotData );
      TEST ( qGotData );
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing entity adjacency consistency...\n";
  bool qAdjCons, qInCons;
  try {
    for ( int eEType = TSTTM::EntityType_VERTEX;
	  eEType < TSTTM::EntityType_ALL_TYPES; eEType++ ) {
      for ( int eETopo = TSTTM::EntityTopology_POINT;
	    eETopo < TSTTM::EntityTopology_ALL_TOPOLOGIES; eETopo++ ) {
	if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
	  if ( (iGeoDim == 2) && (eEType > TSTTM::EntityType_FACE ||
				  eETopo > TSTTM::EntityTopology_QUADRILATERAL) )
	    continue;
	  // Get entities of the current type and topology
	  ::sidl::array<void*> saEntHandles;
	  int iNumEnt;
	  M.getEntities( apvSets[2], static_cast<TSTTM::EntityType>(eEType),
			      static_cast<TSTTM::EntityTopology>(eETopo),
			      saEntHandles, iNumEnt );
	  if ( iNumEnt == 0 )
	    continue;
	  void** apvEnts = saEntHandles.first();

	  cout << "  Type: ";
	  cout.width(9);
	  cout << astrTypes[eEType] << "      with topology: ";
	  cout.width(14);
	  cout << astrTopologies[eETopo] << endl;

	  // Now test for equivalence in adjacency operations.
	  int eAdjType, eLastType = TSTTM::EntityType_REGION;
	  for ( eAdjType = TSTTM::EntityType_VERTEX; eAdjType <= eLastType; eAdjType++ ) {
	    // Skip any that are unsupported.
	    if ( a2iAdjTable[eEType][eAdjType] == TSTTM::AdjacencyInfo_UNAVAILABLE )
	      continue;
	    qAdjCons = true;
	    qInCons = true;

	    cout << "   Checking 'contains' for adjacent type:  ";
	    cout.width(9);
	    cout << astrTypes[eAdjType] << "   ";

	    sidl::array<void*> saAdjEnt;
	    sidl::array<int> saiOffset, saiInSet;
	    int iNumAdj, iOffsetSize, iInSetSize;
	    // Get adjacencies using the entityset call.

	    M.getAdjEntities( apvSets[2], static_cast<TSTTM::EntityType>(eEType),
				   static_cast<TSTTM::EntityTopology>(eETopo),
				   static_cast<TSTTM::EntityType>(eAdjType), saAdjEnt,
				   iNumAdj, saiOffset, iOffsetSize, saiInSet, iInSetSize );
	    void** apvAdjEnts = saAdjEnt.first();

	    // Now check that (a) the adjacencies match and (b) all the
	    // entities are listed as in the entity set.

	    // Check (a)
	    if ( iNumAdj != iInSetSize ) {
	      qInCons = false;
	    } else {
	      int* aiInSet = saiInSet.first();
	      for ( int iAdj = 0; iAdj < iNumAdj && qInCons; iAdj++ ) {
		qInCons = qInCons && ( aiInSet[iAdj] ==
				       ES.isEntContained(apvSets[2], apvAdjEnts[iAdj]) );
	      }
	      cout << "   DONE\n";
	    }

	    // Check (b)
	    cout << "   Checking adjacency for entity type:     ";
	    cout.width(9);
	    cout << astrTypes[eAdjType] << "   ";
	    if ( iNumEnt != iOffsetSize-1 ) {
	      qAdjCons = false;
	    }
	    else {
	      int*   aiOffsets  = saiOffset.first();

	      for ( int iEnt = 0; iEnt < iNumEnt && qAdjCons; iEnt++ ) {
		int iNLocalAdj;
		sidl::array<void*> saAdjEntHandles;
		EntMesh.getEntAdj( apvEnts[iEnt], static_cast<TSTTM::EntityType>(eAdjType),
				  saAdjEntHandles, iNLocalAdj );
		// Number of adjacencies must match.
		if ( iNLocalAdj != (aiOffsets[iEnt+1] - aiOffsets[iEnt]) ) {
		  qAdjCons = false;
		  continue;
		}
		int iOffset = aiOffsets[iEnt];
		void** apvLocalAdj = saAdjEntHandles.first();
		for (int iAdj = 0; iAdj < iNLocalAdj; iAdj++) {
		  void * global_handle = apvAdjEnts[iOffset + iAdj];
		  void * local_handle = apvLocalAdj[iAdj];
		  qAdjCons = qAdjCons && (global_handle == local_handle);
		} // Loop over local adjacencies
	      } // Loop over entities
	      cout << "   DONE\n";
	    }
	  } // Loop over adjacency types
	  TEST(qAdjCons);
	  TEST(qInCons);
	}
      }
    } // Done with double loop to verify consistency of adjacencies
  } SHOULD_NOT_THROW;

  // getAllVtxCoords, getVtxCoordIndex
  cout << "Checking consistency of vertex coordinate retrieval...    ";
  int iNumAdjEnts, iNumVerts, iNumIndices, iNumTopologies, iNumOffsets1, iNumOffsets2,
    iNumOffsets3, iNumAllCoords, iNumCoords, iNumInSet1, iNumInSet2;
  bool qCoordsCons, qStorCons, qToposCons;
  TSTTM::StorageOrder SOTemp;
  try {
    qStorCons = true;
    for ( int iSet = 0; iSet < 5 && qStorCons; iSet++ ) {
      sidl::array<int> saiInSet1;
      sidl::array<double> sadAllCoords;
      SOTemp = TSTTM::StorageOrder_UNDETERMINED;
      M.getAllVtxCoords( apvSets[iSet], sadAllCoords, iNumAllCoords,
			      saiInSet1, iNumInSet1, SOTemp );
      TEST ( iNumAllCoords == iNumInSet1*iGeoDim );
      qStorCons = qStorCons && ( SOTemp == M.getDfltStorage() );
      int* aiInSet1 = saiInSet1.first();
      double* adAllCoords = sadAllCoords.first();

      qInCons = ( iNumAllCoords == iNumInSet1*iGeoDim );
      qCoordsCons = ( iNumAllCoords == iNumInSet1*iGeoDim );
      qToposCons = true;
      for ( int eEType = TSTTM::EntityType_VERTEX;
	    eEType <= TSTTM::EntityType_ALL_TYPES; eEType++ ) {
	for ( int eETopo = TSTTM::EntityTopology_POINT;
	      eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES
		&& qInCons && qToposCons && qCoordsCons
		; eETopo++ ) {
	  // Skip any illegal type-topology combinations
	  if ( !a2qLegalTypeAndTopo[eEType][eETopo] )
	    continue;
	  if ( (iGeoDim == 2) && (eEType == TSTTM::EntityType_REGION ||
				  (eETopo > TSTTM::EntityTopology_QUADRILATERAL &&
				   eETopo != TSTTM::EntityTopology_ALL_TOPOLOGIES)) )
	    continue;

	  for ( int eAdjType = TSTTM::EntityType_VERTEX;
		eAdjType < TSTTM::EntityType_ALL_TYPES; eAdjType++ ) {
	    // Skip any that are identities or unsupported.
	    if ( eEType == eAdjType ||
		 a2iAdjTable[eEType][eAdjType] == TSTTM::AdjacencyInfo_UNAVAILABLE )
	      continue;

	    sidl::array<void*> saAdjEnts;
	    sidl::array<int> saiOffsets1, saiInSet2;
	    M.getAdjEntities( apvSets[iSet], static_cast<TSTTM::EntityType>(eEType),
				   static_cast<TSTTM::EntityTopology>(eETopo),
				   static_cast<TSTTM::EntityType>(eAdjType), saAdjEnts,
				   iNumAdjEnts, saiOffsets1, iNumOffsets1, saiInSet2,
				   iNumInSet2 );
	    ppvGotEnts = saAdjEnts.first();
	    int* aiInSet2 = saiInSet2.first();

	    sidl::array<void*> saVerts;
	    sidl::array<int> saiOffsets2;
	    void** apvAdjEnts;
	    apvAdjEnts = saAdjEnts.first();
	    if ( eAdjType == TSTTM::EntityType_VERTEX ) {
	      saVerts = saAdjEnts;
	      iNumVerts = iNumAdjEnts;
	    }
	    else {
	      ArrMesh.getEntArrAdj( saAdjEnts, iNumAdjEnts, TSTTM::EntityType_VERTEX,
				   saVerts, iNumVerts, saiOffsets2, iNumOffsets2 );
	    }
	    void** apvVerts = saVerts.first();
	    sidl::array<double> sadCoords;
	    SOTemp = TSTTM::StorageOrder_UNDETERMINED;
	    M.getVtxArrCoords( saVerts, iNumVerts, SOTemp, sadCoords, iNumCoords );
	    qStorCons = qStorCons && ( SOTemp == M.getDfltStorage() );

	    sidl::array<int> saiOffsets3, saiIndices;
	    sidl::array<TSTTM::EntityTopology> saTopologies;
	    M.getVtxCoordIndex( apvSets[iSet], static_cast<TSTTM::EntityType>(eEType),
				     static_cast<TSTTM::EntityTopology>(eETopo),
				     static_cast<TSTTM::EntityType>(eAdjType), saiOffsets3,
				     iNumOffsets3, saiIndices, iNumIndices, saTopologies,
				     iNumTopologies );
	    int iTopoLo = saTopologies.lower(0);
	    if ( iNumTopologies == iNumAdjEnts ) {
	      for ( i = 0; i < iNumAdjEnts; i++ ) {
		qToposCons = qToposCons && ( saTopologies.get(iTopoLo+i) ==
					     EntMesh.getEntTopo(ppvGotEnts[i]) );
	      }
	    }
	    else {
	      qToposCons = false;
	    }
	    if ( (iNumOffsets3 == iNumAdjEnts+1 ) && (iNumVerts == iNumIndices) &&
		 (iNumCoords == iGeoDim*iNumVerts) ) {
	      int* aiIndices = saiIndices.first();
	      double* adCoords = sadCoords.first();
	      int* aiOffsets = saiOffsets3.first();
	      if ( SOTemp == TSTTM::StorageOrder_BLOCKED )
		cout << "\nUsing blocked storage order...";
	      int iBegin = 0;
	      for ( i = 0; i < iNumAdjEnts && qCoordsCons; i++ ) {
		if ( aiInSet2[i] == 1 ) {
		  for ( int j = aiOffsets[i]; j < aiOffsets[i+1]; j++ ) {
		    int iCurrInd = iGeoDim*aiIndices[j];
		    iBegin = iGeoDim*j;
		    qCoordsCons = qCoordsCons && ( adCoords[iBegin] == adAllCoords[iCurrInd] )
		      && ( adCoords[iBegin+1] == adAllCoords[iCurrInd+1] );
		    if ( iGeoDim == 3 )
		      qCoordsCons = qCoordsCons && ( adCoords[iBegin+2] == adAllCoords[iCurrInd+2] );
		    qInCons = qInCons && ( aiInSet1[iCurrInd/iGeoDim] ==
					   ES.isEntContained(apvSets[iSet], apvVerts[j]) );
		  }
		}
		else {
		  qInCons = qInCons && ( !ES.isEntContained(apvSets[iSet], apvAdjEnts[i]) );
		}
	      }
	    }
	    else {
	      qCoordsCons = false;
	    }
	  }
	}
      }
      TEST ( qCoordsCons );
      TEST ( qInCons );
      TEST ( qToposCons );
    }
    TEST ( qStorCons );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  if ( qErrTests ) {
    void* pvBadSet = pvIter;
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      M.getNumOfType( pvBadSet, TSTTM::EntityType_VERTEX  );
      cout << "\nERROR: Exception not thrown when getting number of a type for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      M.getNumOfType( apvSets[0], TSTTM::EntityType_ALL_TYPES );
      cout << "\nERROR: Exception not thrown when getting number of a type for "
	   << "all entity types! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TYPE );
    try {
      M.getNumOfType( apvSets[0], static_cast<TSTTM::EntityType>(TSTTM::EntityType_ALL_TYPES+1) );
      cout << "\nERROR: Exception not thrown when getting number of a type for "
	   << "an invalid entity topology! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TYPE );
    try {
      M.getNumOfTopo( pvBadSet, TSTTM::EntityTopology_POINT  );
      cout << "\nERROR: Exception not thrown when getting number of a topology for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      M.getNumOfTopo( apvSets[0], TSTTM::EntityTopology_ALL_TOPOLOGIES );
      cout << "\nERROR: Exception not thrown when getting number of a topolgy for "
	   << "all entity topologies! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TOPOLOGY );
    try {
      M.getNumOfTopo( apvSets[0],
			   static_cast<TSTTM::EntityTopology>(TSTTM::EntityTopology_ALL_TOPOLOGIES+1) );
      cout << "\nERROR: Exception not thrown when getting number of a topolgy for "
	   << "an invalid entity topology! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TOPOLOGY );

    sidl::array<double> saCoords;
    sidl::array<int> saIndices;
    int iNumCoords, iNumIndices;
    TSTTM::StorageOrder SOTemp = TSTTM::StorageOrder_INTERLEAVED;
    try {
      M.getAllVtxCoords( pvBadSet, saCoords, iNumCoords, saIndices, iNumIndices, SOTemp );
      cout << "\nERROR: Exception not thrown when getting vertex coordinates for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );

    sidl::array<int> saOffsets;
    sidl::array<TSTTM::EntityTopology> saGotTopo;
    int iNumOffsets, iNumGotTopo;
    try {
      EntMesh.getVtxCoordIndex( pvBadSet, TSTTM::EntityType_VERTEX, TSTTM::EntityTopology_POINT,
			       TSTTM::EntityType_VERTEX, saOffsets, iNumOffsets, saIndices,
			       iNumIndices, saGotTopo, iNumGotTopo );
      cout << "\nERROR: Exception not thrown when getting vertex coordinates for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );

    for ( int eEType = TSTTM::EntityType_VERTEX;
	  eEType <= TSTTM::EntityType_ALL_TYPES; eEType++ ) {
      for ( int eETopo = TSTTM::EntityTopology_POINT;
	    eETopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES; eETopo++ ) {
	if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
	  /*
	  int eAdjType, eLastType = TSTTM::EntityType_REGION;
	  for ( eAdjType = TSTTM::EntityType_VERTEX; eAdjType <= eLastType; eAdjType++ ) {
	    // Skip any that are supported.
	    if ( a2iAdjTable[eEType][eAdjType] != TSTTM::AdjacencyInfo_UNAVAILABLE )
	      continue;
	    try {
	      sidl::array<void*> saAdjEnt;
	      sidl::array<int> saiOffset, saiInSet;
	      int iNumAdj, iOffsetSize, iInSetSize;
	      M.getAdjEntities( apvSets[2], static_cast<TSTTM::EntityType>(eEType),
				     static_cast<TSTTM::EntityTopology>(eETopo),
				     static_cast<TSTTM::EntityType>(eAdjType), saAdjEnt,
				     iNumAdj, saiOffset, iOffsetSize, saiInSet, iInSetSize );
	      cout << "\nERROR: Exception not thrown when getting adjacent entities for "
		   << "an invalid entity type declared invalid! ";
	      TEST ( error_throwing );
	    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TYPE );
	  }
	  */
	} else {
	  try {
	    sidl::array<void*> saAdjEnt;
	    sidl::array<int> saiOffset, saiInSet;
	    int iNumAdj, iOffsetSize, iInSetSize;
	    M.getAdjEntities( apvSets[2], static_cast<TSTTM::EntityType>(eEType),
				   static_cast<TSTTM::EntityTopology>(eETopo),
				   TSTTM::EntityType_VERTEX, saAdjEnt,
				   iNumAdj, saiOffset, iOffsetSize, saiInSet, iInSetSize );
	    cout << "\nERROR: Exception not thrown when getting adjacent entities for "
		 << "an invalid entity \n       type-topology combination ("
		 << astrTypes[eEType] << " with " << astrTopologies[eETopo] << ")! ";
	    TEST ( error_throwing );
	  } MUST_THROW ( TSTTB::ErrorType_BAD_TYPE_AND_TOPO );

	  sidl::array<int> saOffsets, saIndices;
	  int iNumOffsets, iNumIndices, iNumGotTopo;
	  sidl::array<TSTTM::EntityTopology> saGotTopo;

	  try {
	    M.getVtxCoordIndex( apvSets[2], static_cast<TSTTM::EntityType>(eEType),
				     static_cast<TSTTM::EntityTopology>(eETopo),
				     TSTTM::EntityType_VERTEX, saOffsets, iNumOffsets, saIndices,
				     iNumIndices, saGotTopo, iNumGotTopo );
	    cout << "\nERROR: Exception not thrown when getting vertex coordinate indices for "
		 << "an invalid entity \n       type-topology combination ("
		 << astrTypes[eEType] << " with " << astrTopologies[eETopo] << ")! ";
	    TEST ( error_throwing );
	  } MUST_THROW ( TSTTB::ErrorType_BAD_TYPE_AND_TOPO );
	}
      }
    }
    cout << "DONE\n";
  }

  try {
    for ( i=0; i<5; i++ )
      ES.destroyEntSet( apvSets[i] );
  } SHOULD_NOT_THROW;

}

static void vTestEntityWithSets(TSTTM::Mesh& M)
{
  //    initEntIter, getNextEntIter, resetEntIter, endEntIter
  TSTTM::Entity EntMesh(M);
  TSTTB::EntSet ES(M);
  const int iNumType = TSTTM::EntityType_ALL_TYPES - TSTTM::EntityType_VERTEX;
  const int iNumTopo = TSTTM::EntityTopology_ALL_TOPOLOGIES - TSTTM::EntityTopology_POINT;
  void* apvSets[10];
  void* a2pvTypeEnts[iNumType][iNumType*5];
  void* pvIter;
  void* a2pvTopoEnts[iNumTopo][iNumTopo*5];
  int aiNumTypeEnts[iNumType] = {0};
  int aiNumTopoEnts[iNumTopo] = {0};
  int i, iGeoDim, iNumTypeVerts, iNumTopoVerts;
  bool qGotData = false;

  try {
    iGeoDim = EntMesh.getGeometricDim();
    // Get a different number of entities for each type and record how many were retrieved
    // --a2pvTypeEnts stores the entities, aiNumTypeEnts stores how many for each type
    for ( i = 0; i < iNumType; i++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityType_FACE) )
	break;
      if ( EntMesh.initEntIter(pvRootSet, static_cast<TSTTM::EntityType>(i+TSTTM::EntityType_VERTEX),
			       TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
	int &j = aiNumTypeEnts[i];
	while ( j < 5*(iNumType-i) && EntMesh.getNextEntIter(pvIter, a2pvTypeEnts[i][j]) )
	  j++;
	EntMesh.endEntIter( pvIter );
	if ( i > 0 )
	  qGotData = qGotData || (j > 0);
      }
    }
    // If there aren't the required number of vertices (20 to be exact) or there aren't any
    // of any of the other topologies of entities, bail out because the subsequent tests would be
    // useless and might not even work
    if ( !qGotData || aiNumTypeEnts[0] < 5*iNumType ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    // Get a different number of entities for each topology and record how many were retrieved
    // --a2pvTopoEnts stores the entities, aiNumTopoEnts stores how many for each topology
    qGotData = false;
    for ( i = 0; i < iNumTopo; i++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      if ( EntMesh.initEntIter(pvRootSet, TSTTM::EntityType_ALL_TYPES,
			       static_cast<TSTTM::EntityTopology>(i+TSTTM::EntityTopology_POINT), pvIter) ) {
	int &j = aiNumTopoEnts[i];
	while ( j < 5*(iNumTopo-i) && EntMesh.getNextEntIter(pvIter, a2pvTopoEnts[i][j]) )
	  j++;
	EntMesh.endEntIter( pvIter );
	if ( i > 0 )
	  qGotData = qGotData || (j > 0);
      }
    }
    // If there aren't the required number of points (55 to be exact) or there aren't any
    // of any of the other topologies of entities, bail out because the subsequent tests would be
    // useless and might not even work
    if ( !qGotData || aiNumTopoEnts[0] < 5*iNumTopo ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }

    // Create two lists and two sets, one for types and one for topologies each,
    // as well as an empty list
    for ( i=0; i<5; i++ )
      ES.createEntSet( i<3, apvSets[i] );

    // Add the entities of each type to list #1 and set #3, interspersing them with
    // vertices, like this:
    //        V0,V0,V1,V1,V2,V2,...,E0,V0,E1,V1,E2,V2,....R0,V0,R1,V1,R2,V2
    // for the set the order makes no difference, but it does make the list traversal
    // slightly more interesting
    for ( int j = 0; j < aiNumTypeEnts[0]; j++ )
      for ( i = 0; i < iNumType; i++ ) {
	if ( j < aiNumTypeEnts[i] ) {
	  ES.addEntToSet( a2pvTypeEnts[i][j], apvSets[1] );
	  ES.addEntToSet( a2pvTypeEnts[0][j], apvSets[1] );
	  ES.addEntToSet( a2pvTypeEnts[i][j], apvSets[3] );
	}
      }

    // Calculate the number of vertices used in the list above
    iNumTypeVerts = aiNumTypeEnts[0];
    for ( i=0; i<iNumType; i++ )
      iNumTypeVerts += aiNumTypeEnts[i];

    // Do the same thing except for topologies
    for ( int j = 0; j < aiNumTopoEnts[0]; j++ )
      for ( i = 0; i < iNumTopo; i++ ) {
	if ( j < aiNumTopoEnts[i] ) {
	  ES.addEntToSet( a2pvTopoEnts[i][j], apvSets[2] );
	  ES.addEntToSet( a2pvTopoEnts[0][j], apvSets[2] );
	  ES.addEntToSet( a2pvTopoEnts[i][j], apvSets[4] );
	}
      }

    // Calculate the number of points used in the list above
    iNumTopoVerts = aiNumTopoEnts[0];
    for ( i=0; i<iNumTopo; i++ )
      iNumTopoVerts += aiNumTopoEnts[i];

  } SHOULD_NOT_THROW_CRITICAL;

  cout << "Testing set iterator initialization...                    ";
  void* apvTypeIter0[iNumType];
  void* apvTopoIter0[iNumTopo];
  void* apvTypeIter1[iNumType];
  void* apvTypeIter3[iNumType];
  void* apvTopoIter2[iNumTopo];
  void* apvTopoIter4[iNumTopo];
  bool aqInitType1[iNumType] = {false};
  bool aqInitType3[iNumType] = {false};
  bool aqInitTopo2[iNumTopo] = {false};
  bool aqInitTopo4[iNumTopo] = {false};
  try {
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      TEST ( !EntMesh.initEntIter(apvSets[0], static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
				  TSTTM::EntityTopology_ALL_TOPOLOGIES, apvTypeIter0[iType]) );
      if ( aiNumTypeEnts[iType] > 0 ) {
	aqInitType1[iType] =
	  EntMesh.initEntIter( apvSets[1],
			       static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
			       TSTTM::EntityTopology_ALL_TOPOLOGIES, apvTypeIter1[iType] );
	TEST ( aqInitType1[iType] );
	aqInitType3[iType] =
	  EntMesh.initEntIter( apvSets[3],
			       static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
			       TSTTM::EntityTopology_ALL_TOPOLOGIES, apvTypeIter3[iType] );
	TEST ( aqInitType3[iType] );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      TEST ( !EntMesh.initEntIter(apvSets[0], TSTTM::EntityType_ALL_TYPES,
				  static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
				  apvTopoIter0[iTopo]) );
      if ( aiNumTopoEnts[iTopo] > 0 ) {
	aqInitTopo2[iTopo] =
	  EntMesh.initEntIter( apvSets[2], TSTTM::EntityType_ALL_TYPES,
			       static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
			       apvTopoIter2[iTopo] );
	TEST ( aqInitTopo2[iTopo] );
	aqInitTopo4[iTopo] =
	  EntMesh.initEntIter( apvSets[4], TSTTM::EntityType_ALL_TYPES,
			       static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
			       apvTopoIter4[iTopo] );
	TEST ( aqInitTopo4[iTopo] );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing set iterator entity retrieval...                  ";
  void* pvEntTemp;
  void** ppvGotEnts;
  sidl::array<void*> saGotEnts = saGotEnts.create1d( iNumTopoVerts+1 );
  try {
    if ( aqInitType1[0] ) {
      i = 0;
      while ( i < iNumTypeVerts+1 && EntMesh.getNextEntIter(apvTypeIter1[0], pvEntTemp) ) {
	saGotEnts.set( i, pvEntTemp );
	i++;
      }
      qGotData = ( i == iNumTypeVerts );
      if ( qGotData ) {
	i = 0;
	ppvGotEnts = saGotEnts.first();
	for ( int j=0; j < aiNumTypeEnts[0]; j++ ) {
	  qGotData = qGotData && ( a2pvTypeEnts[0][j] == ppvGotEnts[i++] );
	  for ( int iType = 0; iType < iNumType; iType++ )
	    if ( j < aiNumTypeEnts[iType] )
	      qGotData = qGotData && ( a2pvTypeEnts[0][j] == ppvGotEnts[i++] );
	}
      }
      TEST ( qGotData );
    }
    for ( int iType = 1; iType < iNumType; iType++ ) {
      if ( aqInitType1[iType] ) {
	i = 0;
	while ( i < aiNumTypeEnts[iType]+1 && EntMesh.getNextEntIter(apvTypeIter1[iType], pvEntTemp) ) {
	  saGotEnts.set( i, pvEntTemp );
	  i++;
	}
	qGotData = ( i == aiNumTypeEnts[iType] );
	if ( qGotData ) {
	  ppvGotEnts = saGotEnts.first();
	  for ( int j=0; j < aiNumTypeEnts[iType]; j++ )
	    qGotData = qGotData && ( a2pvTypeEnts[iType][j] == ppvGotEnts[j] );
	}
	TEST ( qGotData );
      }
    }
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      TEST ( !EntMesh.getNextEntIter(apvTypeIter0[iType], pvEntTemp) );
      if ( aqInitType3[iType] ) {
	i = 0;
	while ( i < aiNumTypeEnts[iType]+1 && EntMesh.getNextEntIter(apvTypeIter3[iType], pvEntTemp) ) {
	  saGotEnts.set( i, pvEntTemp );
	  i++;
	}
	qGotData = false;
	if ( i == aiNumTypeEnts[iType] )
	  GOT_ALL_ITEMS ( a2pvTypeEnts[iType], saGotEnts, i, qGotData );
	TEST ( qGotData );
      }
    }
    if ( aqInitTopo2[0] ) {
      i = 0;
      while ( i < iNumTopoVerts+1 && EntMesh.getNextEntIter(apvTopoIter2[0], pvEntTemp) ) {
	saGotEnts.set( i, pvEntTemp );
	i++;
      }
      qGotData = ( i == iNumTopoVerts );
      if ( qGotData ) {
	ppvGotEnts = saGotEnts.first();
	i = 0;
	for ( int j=0; j < aiNumTopoEnts[0]; j++ ) {
	  qGotData = qGotData && ( a2pvTopoEnts[0][j] == ppvGotEnts[i++] );
	  for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ )
	    if ( j < aiNumTopoEnts[iTopo] )
	      qGotData = qGotData && ( a2pvTopoEnts[0][j] == ppvGotEnts[i++] );
	}
      }
      TEST ( qGotData );
    }
    for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
      if ( aqInitTopo2[iTopo] ) {
	i = 0;
	while ( i < aiNumTopoEnts[iTopo]+1 && EntMesh.getNextEntIter(apvTopoIter2[iTopo], pvEntTemp) ) {
	  saGotEnts.set( i, pvEntTemp );
	  i++;
	}
	qGotData = ( i == aiNumTopoEnts[iTopo] );
	if ( qGotData ) {
	  ppvGotEnts = saGotEnts.first();
	  for ( int j=0; j < aiNumTopoEnts[iTopo]; j++ )
	    qGotData = qGotData && ( a2pvTopoEnts[iTopo][j] == ppvGotEnts[j] );
	}
	TEST ( qGotData );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      TEST ( !EntMesh.getNextEntIter(apvTopoIter0[iTopo], pvEntTemp) );
      if ( aqInitTopo4[iTopo] ) {
	i = 0;
	while ( i < aiNumTopoEnts[iTopo]+1 && EntMesh.getNextEntIter(apvTopoIter4[iTopo], pvEntTemp) ) {
	  saGotEnts.set( i, pvEntTemp );
	  i++;
	}
	qGotData = false;
	if ( i == aiNumTopoEnts[iTopo] )
	  GOT_ALL_ITEMS ( a2pvTopoEnts[iTopo], saGotEnts, i, qGotData );
	TEST ( qGotData );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing reset, with removal and addition mid-traversal... ";
  void* pvEntOut;
  void* pvEntBack;
  try {
    if ( aqInitType1[0]  ) {
      EntMesh.resetEntIter( apvTypeIter1[0] );
      i = 0;
      while ( i < (iNumTypeVerts/3) && EntMesh.getNextEntIter(apvTypeIter1[0], pvEntTemp) )
	i++;
      pvEntOut = pvEntTemp;
      while ( i < (iNumTypeVerts/2) && EntMesh.getNextEntIter(apvTypeIter1[0], pvEntTemp) )
	i++;
      ES.rmvEntFromSet( pvEntOut, apvSets[1] );
      ES.rmvEntFromSet( pvEntTemp, apvSets[1] );
      ES.addEntToSet( pvEntTemp, apvSets[1] );
      while ( i < iNumTypeVerts+1 && EntMesh.getNextEntIter(apvTypeIter1[0], pvEntTemp) )
	i++;
      ES.addEntToSet( pvEntOut, apvSets[1] );
      if ( EntMesh.getNextEntIter(apvTypeIter1[0], pvEntBack) )
	TEST ( pvEntOut == pvEntBack );
      else
	TEST ( false );
      TEST ( !EntMesh.getNextEntIter(apvTypeIter1[0], pvEntTemp) );
    }
    for ( int iType = 1; iType < iNumType; iType++ ) {
      if ( aqInitType1[iType] ) {
	EntMesh.resetEntIter( apvTypeIter1[iType] );
	i = 0;
	while ( i < (aiNumTypeEnts[iType]/3) && EntMesh.getNextEntIter(apvTypeIter1[iType], pvEntTemp) )
	  i++;
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTypeEnts[iType]/2) && EntMesh.getNextEntIter(apvTypeIter1[iType], pvEntTemp) )
	  i++;
	ES.rmvEntFromSet( pvEntOut, apvSets[1] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[1] );
	ES.addEntToSet( pvEntTemp, apvSets[1] );
	while ( i < aiNumTypeEnts[iType]+1 && EntMesh.getNextEntIter(apvTypeIter1[iType], pvEntTemp) )
	  i++;
	ES.addEntToSet( pvEntOut, apvSets[1] );
	if ( EntMesh.getNextEntIter(apvTypeIter1[iType], pvEntBack) )
	  TEST ( pvEntOut == pvEntBack );
	else
	  TEST ( false );
	TEST ( !EntMesh.getNextEntIter(apvTypeIter1[iType], pvEntTemp) );
      }
    }
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      EntMesh.resetEntIter( apvTypeIter0[iType] );
      TEST ( !EntMesh.getNextEntIter(apvTypeIter0[iType], pvEntTemp) );
      if ( aqInitType3[iType] ) {
	EntMesh.resetEntIter( apvTypeIter3[iType] );
	i = 0;
	while ( i < (aiNumTypeEnts[iType]/3) && EntMesh.getNextEntIter(apvTypeIter3[iType], pvEntTemp) )
	  i++;
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTypeEnts[iType]/2) && EntMesh.getNextEntIter(apvTypeIter3[iType], pvEntTemp) )
	  i++;
	ES.rmvEntFromSet( pvEntOut, apvSets[3] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[3] );
	while ( EntMesh.getNextEntIter(apvTypeIter3[iType], pvEntTemp) ) {
	  i++;
	}
	TEST ( i >= aiNumTypeEnts[iType]/2 );
      }
    }
    if ( aqInitTopo2[0]  ) {
      EntMesh.resetEntIter( apvTopoIter2[0] );
      i = 0;
      while ( i < (iNumTopoVerts/3) && EntMesh.getNextEntIter(apvTopoIter2[0], pvEntTemp) )
	i++;
      pvEntOut = pvEntTemp;
      while ( i < (iNumTopoVerts/2) && EntMesh.getNextEntIter(apvTopoIter2[0], pvEntTemp) )
	i++;
      ES.rmvEntFromSet( pvEntOut, apvSets[2] );
      ES.rmvEntFromSet( pvEntTemp, apvSets[2] );
      ES.addEntToSet( pvEntTemp, apvSets[2] );
      while ( i < iNumTopoVerts+1 && EntMesh.getNextEntIter(apvTopoIter2[0], pvEntTemp) )
	i++;
      ES.addEntToSet( pvEntOut, apvSets[2] );
      if ( EntMesh.getNextEntIter(apvTopoIter2[0], pvEntBack) )
	TEST ( pvEntOut == pvEntBack );
      else
	TEST ( false );
      TEST ( !EntMesh.getNextEntIter(apvTopoIter2[0], pvEntTemp) );
    }
    for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
      if ( aqInitTopo2[iTopo] ) {
	EntMesh.resetEntIter( apvTopoIter2[iTopo] );
	i = 0;
	while ( i < (aiNumTopoEnts[iTopo]/3) && EntMesh.getNextEntIter(apvTopoIter2[iTopo], pvEntTemp) )
	  i++;
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTopoEnts[iTopo]/2) && EntMesh.getNextEntIter(apvTopoIter2[iTopo], pvEntTemp) )
	  i++;
	ES.rmvEntFromSet( pvEntOut, apvSets[2] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[2] );
	ES.addEntToSet( pvEntTemp, apvSets[2] );
	while ( i < aiNumTopoEnts[iTopo]+1 && EntMesh.getNextEntIter(apvTopoIter2[iTopo], pvEntTemp) )
	  i++;
	ES.addEntToSet( pvEntOut, apvSets[2] );
	if ( EntMesh.getNextEntIter(apvTopoIter2[iTopo], pvEntBack) )
	  TEST ( pvEntOut == pvEntBack );
	else
	  TEST ( false );
	TEST ( !EntMesh.getNextEntIter(apvTopoIter2[iTopo], pvEntTemp) );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      EntMesh.resetEntIter( apvTopoIter0[iTopo] );
      TEST ( !EntMesh.getNextEntIter(apvTopoIter0[iTopo], pvEntTemp) );
      if ( aqInitTopo4[iTopo] ) {
	EntMesh.resetEntIter( apvTopoIter4[iTopo] );
	i = 0;
	while ( i < (aiNumTopoEnts[iTopo]/3) && EntMesh.getNextEntIter(apvTopoIter4[iTopo], pvEntTemp) )
	  i++;
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTopoEnts[iTopo]/2) && EntMesh.getNextEntIter(apvTopoIter4[iTopo], pvEntTemp) )
	  i++;
	ES.rmvEntFromSet( pvEntOut, apvSets[4] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[4] );
	while ( EntMesh.getNextEntIter(apvTopoIter4[iTopo], pvEntTemp) ) {
	  i++;
	}
	TEST ( i >= aiNumTopoEnts[iTopo]/2 );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing set iterator termination...                       ";
  try {
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      EntMesh.endEntIter( apvTypeIter0[iType] );
      if ( aiNumTypeEnts[iType] > 0 ) {
	EntMesh.endEntIter( apvTypeIter1[iType] );
	EntMesh.endEntIter( apvTypeIter3[iType] );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      EntMesh.endEntIter( apvTopoIter0[iTopo] );
      if ( aiNumTopoEnts[iTopo] > 0 ) {
	EntMesh.endEntIter( apvTopoIter2[iTopo] );
	EntMesh.endEntIter( apvTopoIter4[iTopo] );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  if ( qErrTests ) {
    void* pvBadSet = pvIter;
    void* pvBadIter;
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      EntMesh.initEntIter( pvBadSet, TSTTM::EntityType_VERTEX,
			   TSTTM::EntityTopology_ALL_TOPOLOGIES, pvBadIter );
      cout << "\nERROR: Exception not thrown when initializing an iterator for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      EntMesh.endEntIter( pvBadIter );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      EntMesh.initEntIter( apvSets[0], static_cast<TSTTM::EntityType>(TSTTM::EntityType_ALL_TYPES+1),
			   TSTTM::EntityTopology_ALL_TOPOLOGIES, pvBadIter );
      cout << "\nERROR: Exception not thrown when initializing an iterator for "
	   << "an invalid entity type! ";
      TEST ( error_throwing );
      EntMesh.endEntIter( pvBadIter );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TYPE );
    try {
      EntMesh.initEntIter( apvSets[0], TSTTM::EntityType_ALL_TYPES,
			   static_cast<TSTTM::EntityTopology>(TSTTM::EntityTopology_ALL_TOPOLOGIES+1),
			   pvBadIter );
      cout << "\nERROR: Exception not thrown when initializing an iterator for "
	   << "an invalid entity topology! ";
      TEST ( error_throwing );
      EntMesh.endEntIter( pvBadIter );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TOPOLOGY );
    for ( int iType = TSTTM::EntityType_VERTEX; iType <= TSTTM::EntityType_ALL_TYPES; iType++ )
      for ( int iTopo = TSTTM::EntityTopology_POINT;
	    iTopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES; iTopo++ ) {
	if ( !a2qLegalTypeAndTopo[iType][iTopo] ) {
	  if ( (iGeoDim == 2) && (iType == TSTTM::EntityType_REGION ||
				  (iTopo > TSTTM::EntityTopology_QUADRILATERAL &&
				   iTopo != TSTTM::EntityTopology_ALL_TOPOLOGIES)) )
	    continue;
	  try {
	    EntMesh.initEntIter( apvSets[0], static_cast<TSTTM::EntityType>(iType),
				 static_cast<TSTTM::EntityTopology>(iTopo), pvBadIter );
	    cout << "\nERROR: Exception not thrown when initializing an iterator for "
		 << "an invalid entity \n       type-topology combination ("
		 << astrTypes[iType] << " with " << astrTopologies[iTopo] << ")! ";
	    TEST ( error_throwing );
	    EntMesh.endEntIter( pvBadIter );
	  } MUST_THROW ( TSTTB::ErrorType_BAD_TYPE_AND_TOPO );
	} // if ( !a2q.. )
      } // for ( int iTopo... )

    pvBadIter = pvEntTemp;
    try {
      EntMesh.getNextEntIter( pvBadIter, pvEntTemp );
      cout << "\nERROR: Exception not thrown when getting next entity for "
	   << "an invalid iterator handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ITERATOR_HANDLE );
    try {
      EntMesh.resetEntIter( pvBadIter );
      cout << "\nERROR: Exception not thrown when getting next entity for "
	   << "an invalid iterator handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ITERATOR_HANDLE );
    try {
      EntMesh.endEntIter( pvBadIter );
      cout << "\nERROR: Exception not thrown when getting next entity for "
	   << "an invalid iterator handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ITERATOR_HANDLE );
    cout << "DONE\n";
  }

  try {
    for ( i=0; i<5; i++ )
      ES.destroyEntSet( apvSets[i] );
  } SHOULD_NOT_THROW;

}

static void vTestArrayWithSets(TSTTM::Mesh& M)
{
  TSTTM::Entity EntMesh(M);
  TSTTM::Arr    ArrMesh(M);
  TSTTB::EntSet ES(M);
  //     initEntArrIter, getEntArrNextIter, resetEntArrIter,
  //     endEntArrIter
  const int iNumType = TSTTM::EntityType_ALL_TYPES - TSTTM::EntityType_VERTEX;
  const int iNumTopo = TSTTM::EntityTopology_ALL_TOPOLOGIES - TSTTM::EntityTopology_POINT;
  void* apvSets[10];
  void* a2pvTypeEnts[iNumType][iNumType*5];
  void* pvIter;
  void* a2pvTopoEnts[iNumTopo][iNumTopo*5];
  int aiNumTypeEnts[iNumType] = {0};
  int aiNumTopoEnts[iNumTopo] = {0};
  int i, iGeoDim, iNumTypeVerts, iNumTopoVerts;
  bool qGotData = false;

  try {
    iGeoDim = M.getGeometricDim();
    // Get a different number of entities for each type and record how many were retrieved
    // --a2pvTypeEnts stores the entities, aiNumTypeEnts stores how many for each type
    for ( i = 0; i < iNumType; i++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityType_FACE) )
	break;
      if ( EntMesh.initEntIter(pvRootSet, static_cast<TSTTM::EntityType>(i+TSTTM::EntityType_VERTEX),
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
	int &j = aiNumTypeEnts[i];
	while ( j < 5*(iNumType-i) && EntMesh.getNextEntIter(pvIter, a2pvTypeEnts[i][j]) )
	  j++;
	EntMesh.endEntIter( pvIter );
	if ( i > 0 )
	  qGotData = qGotData || (j > 0);
      }
    }
    // If there aren't the required number of vertices (20 to be exact) or there aren't any
    // of any of the other types of entities, bail out because the subsequent tests would be
    // useless and might not even work
    if ( !qGotData || aiNumTypeEnts[0] < 5*iNumType ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    // Get a different number of entities for each topology and record how many were retrieved
    // --a2pvTopoEnts stores the entities, aiNumTopoEnts stores how many for each topology
    qGotData = false;
    for ( i = 0; i < iNumTopo; i++ ) {
      if ( (iGeoDim == 2) && (i > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      if ( EntMesh.initEntIter(pvRootSet, TSTTM::EntityType_ALL_TYPES,
			      static_cast<TSTTM::EntityTopology>(i+TSTTM::EntityTopology_POINT), pvIter) ) {
	int &j = aiNumTopoEnts[i];
	while ( j < 5*(iNumTopo-i) && EntMesh.getNextEntIter(pvIter, a2pvTopoEnts[i][j]) )
	  j++;
	EntMesh.endEntIter( pvIter );
	if ( i > 0 )
	  qGotData = qGotData || (j > 0);
      }
    }
    // If there aren't the required number of points (55 to be exact) or there aren't any
    // of any of the other topologies of entities, bail out because the subsequent tests would be
    // useless and might not even work
    if ( !qGotData || aiNumTopoEnts[0] < 5*iNumTopo ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }

    // Create two lists and two sets, one for types and one for topologies each,
    // as well as an empty list
    for ( i=0; i<5; i++ )
      ES.createEntSet( i<3, apvSets[i] );

    // Add the entities of each type to list #1 and set #3, interspersing them with
    // vertices, like this:
    //        V0,V0,V1,V1,V2,V2,...,E0,V0,E1,V1,E2,V2,....R0,V0,R1,V1,R2,V2
    // for the set the order makes no difference, but it does make the list traversal
    // slightly more interesting
    for ( int j = 0; j < aiNumTypeEnts[0]; j++ )
      for ( i = 0; i < iNumType; i++ ) {
	if ( j < aiNumTypeEnts[i] ) {
	  ES.addEntToSet( a2pvTypeEnts[i][j], apvSets[1] );
	  ES.addEntToSet( a2pvTypeEnts[0][j], apvSets[1] );
	  ES.addEntToSet( a2pvTypeEnts[i][j], apvSets[3] );
	}
      }

    // Calculate the number of vertices used in the list above
    iNumTypeVerts = aiNumTypeEnts[0];
    for ( i=0; i<iNumType; i++ )
      iNumTypeVerts += aiNumTypeEnts[i];

    // Do the same thing for topologies
    for ( int j = 0; j < aiNumTopoEnts[0]; j++ )
      for ( i = 0; i < iNumTopo; i++ ) {
	if ( j < aiNumTopoEnts[i] ) {
	  ES.addEntToSet( a2pvTopoEnts[i][j], apvSets[2] );
	  ES.addEntToSet( a2pvTopoEnts[0][j], apvSets[2] );
	  ES.addEntToSet( a2pvTopoEnts[i][j], apvSets[4] );
	}
      }

    // Calculate the number of points used in the list above
    iNumTopoVerts = aiNumTopoEnts[0];
    for ( i=0; i<iNumTopo; i++ )
      iNumTopoVerts += aiNumTopoEnts[i];

  } SHOULD_NOT_THROW_CRITICAL;

  cout << "Testing set array iterator initialization...              ";
  void* apvTypeIter0[iNumType];
  void* apvTopoIter0[iNumTopo];
  void* apvTypeIter1[iNumType];
  void* apvTypeIter3[iNumType];
  void* apvTopoIter2[iNumTopo];
  void* apvTopoIter4[iNumTopo];
  bool aqInitType1[iNumType] = {false};
  bool aqInitType3[iNumType] = {false};
  bool aqInitTopo2[iNumTopo] = {false};
  bool aqInitTopo4[iNumTopo] = {false};
  try {
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      TEST ( !ArrMesh.initEntArrIter(apvSets[0],
				     static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
				     TSTTM::EntityTopology_ALL_TOPOLOGIES, 2, apvTypeIter0[iType]) );
      if ( aiNumTypeEnts[iType] > 0 ) {
	aqInitType1[iType] =
	  ArrMesh.initEntArrIter( apvSets[1],
				  static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
				  TSTTM::EntityTopology_ALL_TOPOLOGIES, 2, apvTypeIter1[iType] );
	TEST ( aqInitType1[iType] );
	aqInitType3[iType] =
	  ArrMesh.initEntArrIter( apvSets[3],
				  static_cast<TSTTM::EntityType>(iType+TSTTM::EntityType_VERTEX),
				  TSTTM::EntityTopology_ALL_TOPOLOGIES, 2, apvTypeIter3[iType] );
	TEST ( aqInitType3[iType] );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      TEST ( !ArrMesh.initEntArrIter(apvSets[0], TSTTM::EntityType_ALL_TYPES,
				     static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
				     2, apvTopoIter0[iTopo]) );
      if ( aiNumTopoEnts[iTopo] > 0 ) {
	aqInitTopo2[iTopo] =
	  ArrMesh.initEntArrIter( apvSets[2], TSTTM::EntityType_ALL_TYPES,
				  static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
				  2, apvTopoIter2[iTopo] );
	TEST ( aqInitTopo2[iTopo] );
	aqInitTopo4[iTopo] =
	  ArrMesh.initEntArrIter( apvSets[4], TSTTM::EntityType_ALL_TYPES,
				  static_cast<TSTTM::EntityTopology>(iTopo+TSTTM::EntityTopology_POINT),
				  2, apvTopoIter4[iTopo] );
	TEST ( aqInitTopo4[iTopo] );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing set array iterator entity retrieval...            ";
  void* pvEntTemp;
  void** ppvGotEnts;
  sidl::array<void*> saGotEnts = saGotEnts.create1d( iNumTopoVerts+1 );
  sidl::array<void*> saEntTemp = saEntTemp.create1d( 2 );
  int iNumEnts;
  try {
    if ( aqInitType1[0] ) {
      i = 0;
      while ( i < iNumTypeVerts+1 &&
	      ArrMesh.getNextEntArrIter(apvTypeIter1[0], saEntTemp, iNumEnts) ) {
	ppvGotEnts = saEntTemp.first();
	for ( int j=0; j<iNumEnts; j++ ) {
	  saGotEnts.set( i, ppvGotEnts[j] );
	  i++;
	}
      }
      qGotData = ( i == iNumTypeVerts );
      if ( qGotData ) {
	i = 0;
	ppvGotEnts = saGotEnts.first();
	for ( int j=0; j < aiNumTypeEnts[0]; j++ ) {
	  qGotData = qGotData && ( a2pvTypeEnts[0][j] == ppvGotEnts[i++] );
	  for ( int iType = 0; iType < iNumType; iType++ )
	    if ( j < aiNumTypeEnts[iType] )
	      qGotData = qGotData && ( a2pvTypeEnts[0][j] == ppvGotEnts[i++] );
	}
      }
      TEST ( qGotData );
    }
    for ( int iType = 1; iType < iNumType; iType++ ) {
      if ( aqInitType1[iType] ) {
	i = 0;
	while ( i < aiNumTypeEnts[iType]+1 &&
		ArrMesh.getNextEntArrIter(apvTypeIter1[iType], saEntTemp, iNumEnts) ) {
	  ppvGotEnts = saEntTemp.first();
	  for ( int j=0; j<iNumEnts; j++ ) {
	    saGotEnts.set( i, ppvGotEnts[j] );
	    i++;
	  }
	}
	qGotData = ( i == aiNumTypeEnts[iType] );
	if ( qGotData ) {
	  ppvGotEnts = saGotEnts.first();
	  for ( int j=0; j < aiNumTypeEnts[iType]; j++ )
	    qGotData = qGotData && ( a2pvTypeEnts[iType][j] == ppvGotEnts[j] );
	}
	TEST ( qGotData );
      }
    }
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      TEST ( !ArrMesh.getNextEntArrIter(apvTypeIter0[iType], saEntTemp, iNumEnts) );
      if ( aqInitType3[iType] ) {
	i = 0;
	while ( i < aiNumTypeEnts[iType]+1 &&
		ArrMesh.getNextEntArrIter(apvTypeIter3[iType], saEntTemp, iNumEnts) ) {
	  ppvGotEnts = saEntTemp.first();
	  for ( int j=0; j<iNumEnts; j++ ) {
	    saGotEnts.set( i, ppvGotEnts[j] );
	    i++;
	  }
	}
	qGotData = false;
	if ( i == aiNumTypeEnts[iType] )
	  GOT_ALL_ITEMS ( a2pvTypeEnts[iType], saGotEnts, i, qGotData );
	TEST ( qGotData );
      }
    }
    if ( aqInitTopo2[0] ) {
      i = 0;
      while ( i < iNumTopoVerts+1 &&
	      ArrMesh.getNextEntArrIter(apvTopoIter2[0], saEntTemp, iNumEnts) ) {
	ppvGotEnts = saEntTemp.first();
	for ( int j=0; j<iNumEnts; j++ ) {
	  saGotEnts.set( i, ppvGotEnts[j] );
	  i++;
	}
      }
      qGotData = ( i == iNumTopoVerts );
      if ( qGotData ) {
	ppvGotEnts = saGotEnts.first();
	i = 0;
	for ( int j=0; j < aiNumTopoEnts[0]; j++ ) {
	  qGotData = qGotData && ( a2pvTopoEnts[0][j] == ppvGotEnts[i++] );
	  for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ )
	    if ( j < aiNumTopoEnts[iTopo] )
	      qGotData = qGotData && ( a2pvTopoEnts[0][j] == ppvGotEnts[i++] );
	}
      }
      TEST ( qGotData );
    }
    for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
      if ( aqInitTopo2[iTopo] ) {
	i = 0;
	while ( i < aiNumTopoEnts[iTopo]+1 &&
		ArrMesh.getNextEntArrIter(apvTopoIter2[iTopo], saEntTemp, iNumEnts) ) {
	  ppvGotEnts = saEntTemp.first();
	  for ( int j=0; j<iNumEnts; j++ ) {
	    saGotEnts.set( i, ppvGotEnts[j] );
	    i++;
	  }
	}
	qGotData = ( i == aiNumTopoEnts[iTopo] );
	if ( qGotData ) {
	  ppvGotEnts = saGotEnts.first();
	  for ( int j=0; j < aiNumTopoEnts[iTopo]; j++ )
	    qGotData = qGotData && ( a2pvTopoEnts[iTopo][j] == ppvGotEnts[j] );
	}
	TEST ( qGotData );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      TEST ( !ArrMesh.getNextEntArrIter(apvTopoIter0[iTopo], saEntTemp, iNumEnts) );
      if ( aqInitTopo4[iTopo] ) {
	i = 0;
	while ( i < aiNumTopoEnts[iTopo]+1 &&
		ArrMesh.getNextEntArrIter(apvTopoIter4[iTopo], saEntTemp, iNumEnts) ) {
	  ppvGotEnts = saEntTemp.first();
	  for ( int j=0; j<iNumEnts; j++ ) {
	    saGotEnts.set( i, ppvGotEnts[j] );
	    i++;
	  }
	}
	qGotData = false;
	if ( i == aiNumTopoEnts[iTopo] )
	  GOT_ALL_ITEMS ( a2pvTopoEnts[iTopo], saGotEnts, i, qGotData );
	TEST ( qGotData );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing reset, with removal and addition mid-traversal... ";
  void* pvEntOut;
  int iRemain;
  try {
    if ( aqInitType1[0]  ) {
      ArrMesh.resetEntArrIter( apvTypeIter1[0] );
      i = 0;
      iRemain = iNumTypeVerts%2;
      while ( i < (iNumTypeVerts/3) &&
	      ArrMesh.getNextEntArrIter(apvTypeIter1[0], saEntTemp, iNumEnts) ) {
	for ( int j=0; j<iNumEnts; j++ ) {
	  pvEntTemp = saEntTemp.first()[j];
	  i++;
	}
      }
      pvEntOut = pvEntTemp;
      while ( i < (iNumTypeVerts/2) &&
	      ArrMesh.getNextEntArrIter(apvTypeIter1[0], saEntTemp, iNumEnts) ) {
	for ( int j=0; j<iNumEnts; j++ ) {
	  pvEntTemp = saEntTemp.first()[j];
	  i++;
	}
      }
      ES.rmvEntFromSet( pvEntOut, apvSets[1] );
      ES.rmvEntFromSet( pvEntTemp, apvSets[1] );
      ES.addEntToSet( pvEntTemp, apvSets[1] );
      while ( i < iNumTypeVerts+1-iRemain &&
	      ArrMesh.getNextEntArrIter(apvTypeIter1[0], saEntTemp, iNumEnts) ) {
	for ( int j=0; j<iNumEnts; j++ ) {
	  pvEntTemp = saEntTemp.first()[j];
	  i++;
	}
      }
      ES.addEntToSet( pvEntOut, apvSets[1] );
      if ( ArrMesh.getNextEntArrIter(apvTypeIter1[0], saEntTemp, iNumEnts) ) {
	qGotData = ( (iNumEnts == 1 && saEntTemp.first()[0] == pvEntOut) ||
		     (iNumEnts == 2 && saEntTemp.first()[1] == pvEntOut) );
	TEST ( qGotData );
      }
      else
	TEST ( false );
      TEST ( !ArrMesh.getNextEntArrIter(apvTypeIter1[0], saEntTemp, iNumEnts) );
    }
    for ( int iType = 1; iType < iNumType; iType++ ) {
      if ( aqInitType1[iType] ) {
	ArrMesh.resetEntArrIter( apvTypeIter1[iType] );
	i = 0;
	iRemain = aiNumTypeEnts[iType]%2;
	while ( i < (aiNumTypeEnts[iType]/3) &&
		ArrMesh.getNextEntArrIter(apvTypeIter1[iType], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTypeEnts[iType]/2) &&
		ArrMesh.getNextEntArrIter(apvTypeIter1[iType], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	ES.rmvEntFromSet( pvEntOut, apvSets[1] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[1] );
	ES.addEntToSet( pvEntTemp, apvSets[1] );
	while ( i < aiNumTypeEnts[iType]+1-iRemain &&
		ArrMesh.getNextEntArrIter(apvTypeIter1[iType], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	ES.addEntToSet( pvEntOut, apvSets[1] );
	if ( ArrMesh.getNextEntArrIter(apvTypeIter1[iType], saEntTemp, iNumEnts) ) {
	  qGotData = ( (iNumEnts == 1 && saEntTemp.first()[0] == pvEntOut) ||
		       (iNumEnts == 2 && saEntTemp.first()[1] == pvEntOut) );
	  TEST ( qGotData );
	}
	else
	  TEST ( false );
	TEST ( !ArrMesh.getNextEntArrIter(apvTypeIter1[iType], saEntTemp, iNumEnts) );
      }
    }
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      ArrMesh.resetEntArrIter( apvTypeIter0[iType] );
      TEST ( !ArrMesh.getNextEntArrIter(apvTypeIter0[iType], saEntTemp, iNumEnts) );
      if ( aqInitType3[iType] ) {
	ArrMesh.resetEntArrIter( apvTypeIter3[iType] );
	i = 0;
	while ( i < (aiNumTypeEnts[iType]/3) &&
		ArrMesh.getNextEntArrIter(apvTypeIter3[iType], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTypeEnts[iType]/2) &&
		ArrMesh.getNextEntArrIter(apvTypeIter3[iType], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	ES.rmvEntFromSet( pvEntOut, apvSets[3] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[3] );
	while ( ArrMesh.getNextEntArrIter(apvTypeIter3[iType], saEntTemp, iNumEnts) ) {
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	}
	TEST ( i >= aiNumTypeEnts[iType]/2 );
      }
    }
    if ( aqInitTopo2[0]  ) {
      ArrMesh.resetEntArrIter( apvTopoIter2[0] );
      i = 0;
      iRemain = iNumTopoVerts%2;
      while ( i < (iNumTopoVerts/3) &&
	      ArrMesh.getNextEntArrIter(apvTopoIter2[0], saEntTemp, iNumEnts) ) {
	for ( int j=0; j<iNumEnts; j++ ) {
	  pvEntTemp = saEntTemp.first()[j];
	  i++;
	}
      }
      pvEntOut = pvEntTemp;
      while ( i < (iNumTopoVerts/2) &&
	      ArrMesh.getNextEntArrIter(apvTopoIter2[0], saEntTemp, iNumEnts) ) {
	for ( int j=0; j<iNumEnts; j++ ) {
	  pvEntTemp = saEntTemp.first()[j];
	  i++;
	}
      }
      ES.rmvEntFromSet( pvEntOut, apvSets[2] );
      ES.rmvEntFromSet( pvEntTemp, apvSets[2] );
      ES.addEntToSet( pvEntTemp, apvSets[2] );
      while ( i < iNumTopoVerts+1-iRemain &&
	      ArrMesh.getNextEntArrIter(apvTopoIter2[0], saEntTemp, iNumEnts) ) {
	for ( int j=0; j<iNumEnts; j++ ) {
	  pvEntTemp = saEntTemp.first()[j];
	  i++;
	}
      }
      ES.addEntToSet( pvEntOut, apvSets[2] );
      if ( ArrMesh.getNextEntArrIter(apvTopoIter2[0], saEntTemp, iNumEnts) ) {
	qGotData = ( (iNumEnts == 1 && saEntTemp.first()[0] == pvEntOut) ||
		     (iNumEnts == 2 && saEntTemp.first()[1] == pvEntOut) );
	TEST ( qGotData );
      }
      else
	TEST ( false );
      TEST ( !ArrMesh.getNextEntArrIter(apvTopoIter2[0], saEntTemp, iNumEnts) );
    }
    for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
      if ( aqInitTopo2[iTopo] ) {
	ArrMesh.resetEntArrIter( apvTopoIter2[iTopo] );
	i = 0;
	iRemain = aiNumTopoEnts[iTopo]%2;
	while ( i < (aiNumTopoEnts[iTopo]/3) &&
		ArrMesh.getNextEntArrIter(apvTopoIter2[iTopo], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTopoEnts[iTopo]/2) &&
		ArrMesh.getNextEntArrIter(apvTopoIter2[iTopo], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	ES.rmvEntFromSet( pvEntOut, apvSets[2] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[2] );
	ES.addEntToSet( pvEntTemp, apvSets[2] );
	while ( i < aiNumTopoEnts[iTopo]+1-iRemain &&
		ArrMesh.getNextEntArrIter(apvTopoIter2[iTopo], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	ES.addEntToSet( pvEntOut, apvSets[2] );
	if ( ArrMesh.getNextEntArrIter(apvTopoIter2[iTopo], saEntTemp, iNumEnts) ) {
	  qGotData = ( (iNumEnts == 1 && saEntTemp.first()[0] == pvEntOut) ||
		       (iNumEnts == 2 && saEntTemp.first()[1] == pvEntOut) );
	  TEST ( qGotData );
	}
	else
	  TEST ( false );
	TEST ( !ArrMesh.getNextEntArrIter(apvTopoIter2[iTopo], saEntTemp, iNumEnts) );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      ArrMesh.resetEntArrIter( apvTopoIter0[iTopo] );
      TEST ( !ArrMesh.getNextEntArrIter(apvTopoIter0[iTopo], saEntTemp, iNumEnts) );
      if ( aqInitTopo4[iTopo] ) {
	ArrMesh.resetEntArrIter( apvTopoIter4[iTopo] );
	i = 0;
	while ( i < (aiNumTopoEnts[iTopo]/3) &&
		ArrMesh.getNextEntArrIter(apvTopoIter4[iTopo], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	pvEntOut = pvEntTemp;
	while ( i < (aiNumTopoEnts[iTopo]/2) &&
		ArrMesh.getNextEntArrIter(apvTopoIter4[iTopo], saEntTemp, iNumEnts) )
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	ES.rmvEntFromSet( pvEntOut, apvSets[4] );
	ES.rmvEntFromSet( pvEntTemp, apvSets[4] );
	while ( ArrMesh.getNextEntArrIter(apvTopoIter4[iTopo], saEntTemp, iNumEnts) ) {
	  for ( int j=0; j<iNumEnts; j++ ) {
	    pvEntTemp = saEntTemp.first()[j];
	    i++;
	  }
	}
	TEST ( i >= aiNumTopoEnts[iTopo]/2 );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing set array iterator termination...                 ";
  try {
    for ( int iType = 0; iType < iNumType; iType++ ) {
      if ( (iGeoDim == 2) && (iType > TSTTM::EntityType_FACE) )
	break;
      ArrMesh.endEntArrIter( apvTypeIter0[iType] );
      if ( aiNumTypeEnts[iType] > 0 ) {
	ArrMesh.endEntArrIter( apvTypeIter1[iType] );
	ArrMesh.endEntArrIter( apvTypeIter3[iType] );
      }
    }
    for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
      if ( (iGeoDim == 2) && (iTopo > TSTTM::EntityTopology_QUADRILATERAL) )
	break;
      ArrMesh.endEntArrIter( apvTopoIter0[iTopo] );
      if ( aiNumTopoEnts[iTopo] > 0 ) {
	ArrMesh.endEntArrIter( apvTopoIter2[iTopo] );
	ArrMesh.endEntArrIter( apvTopoIter4[iTopo] );
      }
    }
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  if ( qErrTests ) {
    void* pvBadSet = pvIter;
    void* pvBadIter;
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      ArrMesh.initEntArrIter( pvBadSet, TSTTM::EntityType_VERTEX,
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, 3, pvBadIter );
      cout << "\nERROR: Exception not thrown when initializing an iterator for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      ArrMesh.endEntArrIter( pvBadIter );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ArrMesh.initEntArrIter( apvSets[0], static_cast<TSTTM::EntityType>(TSTTM::EntityType_ALL_TYPES+1),
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, 3, pvBadIter );
      cout << "\nERROR: Exception not thrown when initializing an iterator for "
	   << "an invalid entity type! ";
      TEST ( error_throwing );
      ArrMesh.endEntArrIter( pvBadIter );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TYPE );
    try {
      ArrMesh.initEntArrIter( apvSets[0], TSTTM::EntityType_ALL_TYPES,
			      static_cast<TSTTM::EntityTopology>(TSTTM::EntityTopology_ALL_TOPOLOGIES+1),
			      3, pvBadIter );
      cout << "\nERROR: Exception not thrown when initializing an iterator for "
	   << "an invalid entity topology! ";
      TEST ( error_throwing );
      ArrMesh.endEntArrIter( pvBadIter );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_TOPOLOGY );
    for ( int iType = TSTTM::EntityType_VERTEX; iType <= TSTTM::EntityType_ALL_TYPES; iType++ )
      for ( int iTopo = TSTTM::EntityTopology_POINT;
	    iTopo <= TSTTM::EntityTopology_ALL_TOPOLOGIES; iTopo++ ) {
	if ( !a2qLegalTypeAndTopo[iType][iTopo] ) {
	  try {
	    ArrMesh.initEntArrIter( apvSets[0], static_cast<TSTTM::EntityType>(iType),
				    static_cast<TSTTM::EntityTopology>(iTopo), 3, pvBadIter );
	    cout << "\nERROR: Exception not thrown when initializing an iterator for "
		 << "an invalid entity \n       type-topology combination ("
		 << astrTypes[iType] << " with " << astrTopologies[iTopo] << ")! ";
	    TEST ( error_throwing );
	    ArrMesh.endEntArrIter( pvBadIter );
	  } MUST_THROW ( TSTTB::ErrorType_BAD_TYPE_AND_TOPO );
	} // if ( !a2q.. )
      } // for ( int iTopo... )

    pvBadIter = pvEntTemp;
    try {
      ArrMesh.getNextEntArrIter( pvBadIter, saEntTemp, iNumEnts );
      cout << "\nERROR: Exception not thrown when getting next entity for "
	   << "an invalid iterator handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ITERATOR_HANDLE );
    try {
      ArrMesh.resetEntArrIter( pvBadIter );
      cout << "\nERROR: Exception not thrown when getting next entity for "
	   << "an invalid iterator handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ITERATOR_HANDLE );
    try {
      ArrMesh.endEntArrIter( pvBadIter );
      cout << "\nERROR: Exception not thrown when getting next entity for "
	   << "an invalid iterator handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ITERATOR_HANDLE );
    cout << "DONE\n";
  }

  try {
    for ( i=0; i<5; i++ )
      ES.destroyEntSet( apvSets[i] );
  } SHOULD_NOT_THROW;

}

static void vTestTagInterface(TSTTB::Tag& TagMesh)
{
  //     createTag, destroyTag, getTagName, getTagSizeValues,
  //     getTagSizeBytes, getTagHandle, getTagType
  void* apvTags[4];
  void* pvBadTag;
  cout << "Creating tags of each type...                             ";
  try {
    bool qNamesOK = true, qTypesOK = true, qSizeBytesOK = true,
      qSizeValuesOK = true, qRetrieved = true;
    // Create a tag of each type and check that the queries for each of the various
    // parameters are working properly
    for ( int i=0; i<4; i++ ) {
      TagMesh.createTag( astrTagNames[i], aiTagSizes[i], aTagTypes[i],
			 apvTags[i] );
      qNamesOK = qNamesOK && ( TagMesh.getTagName(apvTags[i]) == astrTagNames[i] );
      qTypesOK = qTypesOK && ( TagMesh.getTagType(apvTags[i]) == aTagTypes[i] );
      qSizeBytesOK = qSizeBytesOK && ( TagMesh.getTagSizeBytes(apvTags[i])
				       == aiTagBytes[i]*aiTagSizes[i] );
      qSizeValuesOK = qSizeValuesOK && ( TagMesh.getTagSizeValues(apvTags[i]) == aiTagSizes[i] );
      qRetrieved = qRetrieved && ( TagMesh.getTagHandle(astrTagNames[i]) == apvTags[i] );
    }
    TEST ( qNamesOK );
    TEST ( qTypesOK );
    TEST ( qSizeBytesOK );
    TEST ( qSizeValuesOK );
    TEST ( qRetrieved );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  if ( qErrTests ) {
    cout << "Testing error throwing for Tag methods...                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      TagMesh.createTag( astrTagNames[0], 2, TSTTB::TagValueType_DOUBLE,
			 pvBadTag );
      cout << "\nERROR: Duplicate tag name not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_TAG_ALREADY_EXISTS );
    try {
      TagMesh.createTag( "Bad", -1, TSTTB::TagValueType_ENTITY_HANDLE,
			 pvBadTag );
      cout << "\nERROR: Invalid tag value size not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ARGUMENT );
    try {
      TagMesh.createTag( "Bad", 1, TSTTB::TagValueType(5), pvBadTag );
      cout << "\nERROR: Invalid tag value type not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ARGUMENT );
    try {
      TagMesh.getTagName( pvBadTag );
      cout << "\nERROR: Invalid tag handle not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      TagMesh.getTagType( pvBadTag );
      cout << "\nERROR: Invalid tag handle not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      TagMesh.getTagSizeValues( pvBadTag );
      cout << "\nERROR: Invalid tag handle not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      TagMesh.getTagSizeBytes( pvBadTag );
      cout << "\nERROR: Invalid tag handle not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      TagMesh.destroyTag( pvBadTag, false );
      cout << "\nERROR: Invalid tag handle not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      TagMesh.getTagHandle( "Garbage" );
      cout << "\nERROR: Invalid tag name not caught! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_TAG_NOT_FOUND );
    cout << "DONE\n";
  }

  cout << "Testing tag destruction...                                ";
  // Try to destroy tags... can't test forced/unforced destruction just yet
  try {
    for ( int i=0; i<4; i++ )
      TagMesh.destroyTag(apvTags[i], false);
  } SHOULD_NOT_THROW;
  cout << "DONE\n";
}

static void vTestEntTagInterface(TSTTM::Mesh& M)
{
  void* pvIter;
  void* apvTags[7];
  void* apvEnts[3];
  char* pcData;
  sidl::array<char> saOutput;
  sidl::array<void*> saGotTags;
  int iNumChars;

  TSTTB::Tag    TagInt(M);
  TSTTB::EntTag EntTag(M);
  TSTTM::Entity EntMesh(M);
  //     getData, getIntData, getDblData, getEHData, setData,
  //     setIntData, setDblData, setEHData, getAllTags, rmvTag
  cout << "Testing basic tagging functionality...\n";
  try {
    // Create tags of each type that store a single value
    for ( int i=0; i<3; i++ )
      TagInt.createTag( astrTagNames[i], 1, aTagTypes[i], apvTags[i] );
    // ... and a generic tag for completeness
    TagInt.createTag( astrTagNames[3], 6, aTagTypes[3], apvTags[3] );

    // Get at least 3 entities to work with
    if ( !EntMesh.initEntIter(pvRootSet, TSTTM::EntityType_FACE,
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( (!EntMesh.getNextEntIter(pvIter, apvEnts[0])) ||
	 (!EntMesh.getNextEntIter(pvIter, apvEnts[1])) ||
	 (!EntMesh.getNextEntIter(pvIter, apvEnts[2])) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
  } SHOULD_NOT_THROW_CRITICAL;

  cout << "Testing get and set methods with integers...              ";
  sidl::array<char> saChars;
  try {
    // Try setting the data using the generic function...
    iNumChars = sizeof(int);
    saChars = saChars.create1d(iNumChars);
    int* piData = new int(25);
    pcData = (char*)piData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    delete piData;
    EntTag.setData( apvEnts[0], apvTags[0], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    EntTag.getData( apvEnts[0], apvTags[0], saOutput, iNumChars );
    TEST ( iNumChars == (int)sizeof(int) );
    TEST ( *(int*)saOutput.first() == 25 );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    EntTag.setIntData( apvEnts[0], apvTags[0], 30 );
    TEST ( EntTag.getIntData(apvEnts[0], apvTags[0]) == 30 );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with doubles...               ";
  try {
    // Try setting the data using the generic function...
    iNumChars = sizeof(double);
    saChars = saChars.create1d(iNumChars);
    double* pdData = new double(1.8);
    pcData = (char*)pdData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    delete pdData;
    EntTag.setData( apvEnts[0], apvTags[1], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    EntTag.getData( apvEnts[0], apvTags[1], saOutput, iNumChars );
    TEST ( iNumChars == (int)sizeof(double) );
    TEST ( *(double*)saOutput.first() == 1.8 );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    EntTag.setDblData( apvEnts[0], apvTags[1], 1.9 );
    TEST ( EntTag.getDblData(apvEnts[0], apvTags[1]) == 1.9 );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with entity handles...        ";
  try {
    // Try setting the data using the generic function...
    iNumChars = sizeof(void*);
    saChars = saChars.create1d(iNumChars);
    void** ppvData = new void*(apvEnts[1]);
    pcData = (char*)ppvData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    delete ppvData;
    EntTag.setData( apvEnts[0], apvTags[2], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    EntTag.getData( apvEnts[0], apvTags[2], saOutput, iNumChars );
    TEST ( iNumChars == (int)sizeof(void*) );
    TEST ( *(void**)saOutput.first() == apvEnts[1] );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    EntTag.setEHData( apvEnts[0], apvTags[2], apvEnts[2] );
    TEST ( EntTag.getEHData(apvEnts[0], apvTags[2]) == apvEnts[2] );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with generic data (bytes)...  ";
  try {
    // Try setting the data using the generic function...
    iNumChars = 6;
    saChars = saChars.create1d(iNumChars);
    char acData[] = "abcdef";
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, acData[i] );
    EntTag.setData( apvEnts[0], apvTags[3], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    EntTag.getData( apvEnts[0], apvTags[3], saOutput, iNumChars );
    bool qGotData = ( iNumChars == 6 );
    if ( qGotData ) {
      pcData = saOutput.first();
      for ( int i=0; i<iNumChars; i++ )
	qGotData = qGotData && ( pcData[i] == acData[i] );
    }
    TEST ( qGotData );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing tag finding...                                    ";
  try {
    int iNumTags;
    EntTag.getAllTags( apvEnts[0], saGotTags, iNumTags );
    TEST ( iNumTags == 4 );
    bool qTagsPresent = false;
    if ( iNumTags == 4 )
      GOT_ALL_ITEMS ( apvTags, saGotTags, iNumTags, qTagsPresent );
    TEST ( qTagsPresent );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  saOutput = saOutput.create1d( sizeof(int) );
  iNumChars = sizeof(int);
  sidl::array<char> sa2DArray =
    sa2DArray.create2dRow(sizeof(int), sizeof(int));
  sidl::array<char> saNilArray;
  if ( qErrTests ) {
    TSTTM::Mesh icMesh = TSTTM::Factory::newMesh(CREATE_ARG);
    TSTTB::Tag  Tag2 = icMesh;
    TSTTB::EntTag ET2 = icMesh;
    try {
      for ( int i=0; i<3; i++ )
	Tag2.createTag( astrTagNames[i], 1, aTagTypes[i], apvTags[i+4] );
    } SHOULD_NOT_THROW;
    cout << "Testing error throwing for EntTag methods...              ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      ET2.setData( apvEnts[0], apvTags[4], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.setIntData( apvEnts[0], apvTags[4], 15 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.setDblData( apvEnts[0], apvTags[5], 1.5 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.setEHData( apvEnts[0], apvTags[6], apvEnts[1] );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.getData( apvEnts[0], apvTags[4], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.getIntData( apvEnts[0], apvTags[4] );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.getDblData( apvEnts[0], apvTags[5] );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.getEHData( apvEnts[0], apvTags[6] );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ET2.rmvTag( apvEnts[0], apvTags[4] );
      cout << "\nERROR: Exception not thrown when removing tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      for (int i = 0; i < 3; i++) {
	Tag2.destroyTag( apvTags[i+4], true );
      }
    } SHOULD_NOT_THROW;

    // Back to using the original tags on the original mesh...
    try {
      EntTag.getData( pcData, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getData( apvTags[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getData( apvEnts[0], pcData, saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    saOutput = saOutput.create1d(1);
    try {
      EntTag.getData( apvEnts[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid output array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000) {
      try {
	EntTag.getData( apvEnts[0], apvTags[0], sa2DArray, iNumChars );
	cout << "\nERROR: Exception not thrown when getting tag data for "
	     << "an invalid output array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );
    }

    saOutput = saOutput.create1d( sizeof(int) );
    iNumChars = sizeof(int);
    try {
      EntTag.setData( pcData, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( pcData, apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setData( apvTags[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( apvTags[0], apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setData( apvEnts[0], pcData, saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    saOutput = saOutput.create1d(1);
    try {
      EntTag.setData( apvEnts[0], apvTags[0], saNilArray, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an empty input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );
    try {
      EntTag.setData( apvEnts[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000) {
      try {
	EntTag.setData( apvEnts[0], apvTags[0], sa2DArray, iNumChars );
	cout << "\nERROR: Exception not thrown when setting tag data for "
	     << "an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );
    }

    try {
      EntTag.setIntData( apvEnts[0], apvTags[1], 15 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for a double tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.setDblData( apvEnts[0], apvTags[0], 1.5 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.setEHData( apvEnts[0], apvTags[0], apvEnts[1] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.getIntData( apvEnts[0], apvTags[1] );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for a double tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.getDblData( apvEnts[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.getEHData( apvEnts[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.setIntData( pcData, apvTags[0], 15 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( pcData, apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setIntData( apvTags[0], apvTags[0], 15 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( apvTags[0], apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setDblData( pcData, apvTags[1], 1.5 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( pcData, apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setDblData( apvTags[0], apvTags[1], 1.5 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( apvTags[0], apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setEHData( pcData, apvTags[2], apvEnts[1] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( pcData, apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setEHData( apvEnts[0], apvTags[2], pcData );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( apvEnts[0], apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setEHData( apvTags[0], apvTags[2], apvEnts[1] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( apvTags[0], apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.setEHData( apvEnts[0], apvTags[2], apvTags[0] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
      EntTag.rmvTag( apvEnts[0], apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getIntData( pcData, apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getIntData( apvTags[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getDblData( pcData, apvTags[1] );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getDblData( apvTags[0], apvTags[1] );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getEHData( pcData, apvTags[2] );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getEHData( apvTags[0], apvTags[2] );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.rmvTag( apvEnts[0], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      EntTag.rmvTag( pcData, apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.rmvTag( apvTags[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    saGotTags = saGotTags.create1d(4);
    try {
      EntTag.getAllTags( pcData, saGotTags, iNumChars );
      cout << "\nERROR: Exception not thrown when getting all tags "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      EntTag.getAllTags( apvTags[0], saGotTags, iNumChars );
      cout << "\nERROR: Exception not thrown when getting all tags "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    saGotTags = saGotTags.create1d(1);
    try {
      EntTag.getAllTags( apvEnts[0], saGotTags, iNumChars );
      cout << "\nERROR: Exception not thrown when getting all tags "
	   << "into an array of the wrong size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000) {
      saGotTags = saGotTags.create2dRow(5, 5);
      try {
	EntTag.getAllTags( apvEnts[0], saGotTags, iNumChars );
	cout << "\nERROR: Exception not thrown when getting all tags "
	     << "into an array of the wrong dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );
    }
    cout << "DONE\n";
  }

  cout << "Testing tag removal and forced/unforced destruction...    ";
  try {
    // Untag the entity for the first two tags...
    EntTag.rmvTag( apvEnts[0], apvTags[0] );
    EntTag.rmvTag( apvEnts[0], apvTags[1] );
    // ... and see if they can be destroyed without an error
    TagInt.destroyTag( apvTags[0], false );
    TagInt.destroyTag( apvTags[1], false );
    iNPassed++;
  } SHOULD_NOT_THROW;
  // Try to destroy the other tags without first untagging the entity
  try {
    EntTag.destroyTag( apvTags[2], false );
    cout << "\nERROR: Exception not thrown when trying to destroy tag "
	 << "with data without using 'forced'! ";
    TEST ( error_throwing );
  } MUST_THROW ( TSTTB::ErrorType_TAG_IN_USE );
  try {
    EntTag.destroyTag( apvTags[3], false );
    cout << "\nERROR: Exception not thrown when trying to destroy tag "
	 << "with data without using 'forced'! ";
    TEST ( error_throwing );
  } MUST_THROW ( TSTTB::ErrorType_TAG_IN_USE );
  try {
    // Force the destruction of the tags
    TagInt.destroyTag( apvTags[2], true );
    TagInt.destroyTag( apvTags[3], true );
    iNPassed++;
  } SHOULD_NOT_THROW;

  cout << "DONE\n";
}

static void vTestArrTagInterface(TSTTM::Mesh& M)
{
  //     getArrData, getIntArrData, getDblArrData, getEHArrData,
  //     setArrData, setIntArrData, setDblArrData, setEHArrData,
  //     rmvArrTag
  cout << "Testing basic array tagging functionality...\n";
  void* pvIter;
  void* apvTags[7];
  void* apvEnts[3];
  char* pcData;
  sidl::array<char> saOutput;
  sidl::array<void*> saEnts = saEnts.create1d(3);
  sidl::array<void*> saBadEnts = saBadEnts.create1d(9);
  sidl::array<char> saChars;
  sidl::array<int> saInts = saInts.create1d(9);
  sidl::array<double> saDoubles = saDoubles.create1d(9);
  sidl::array<void*> saHandles = saHandles.create1d(9);

  int iNumChars;
  int iNumVals;

  // Begin the "normal" operation testing.
  TSTTB::ArrTag AT(M);
  TSTTM::Entity EntMesh(M);
  TSTTB::Tag    TagInt(M);
  try {
    if ( !EntMesh.initEntIter(pvRootSet, TSTTM::EntityType_FACE,
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( !EntMesh.getNextEntIter(pvIter, apvEnts[0]) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( !EntMesh.getNextEntIter(pvIter, apvEnts[1]) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( !EntMesh.getNextEntIter(pvIter, apvEnts[2]) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    // Create tags of each type that store 3 values...
    for ( int i=0; i<3; i++ ) {
      TagInt.createTag( astrTagNames[i], 3, aTagTypes[i], apvTags[i] );
      saEnts.set( i, apvEnts[i] );
      saBadEnts.set( 3*i, apvTags[i] );
      saBadEnts.set( 3*i+1, apvTags[i] );
      saBadEnts.set( 3*i+2, apvTags[i] );
    }
    // ... and a generic tag for completeness
    TagInt.createTag( astrTagNames[3], 2, aTagTypes[3], apvTags[3] );
  } SHOULD_NOT_THROW_CRITICAL;

  cout << "Testing get and set methods with integers...              ";
  bool qGotData;
  try {
    // Try setting the data using the generic function...
    iNumChars = 3*3*sizeof(int);
    saChars = saChars.create1d(iNumChars);
    int aiData[] = {4,8,12,16,20,24,28,32,36};
    pcData = (char*)&aiData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    AT.setArrData( saEnts, 3, apvTags[0], saChars, iNumChars );
    // ... then seeing if it comes back intact
    AT.getArrData( saEnts, 3, apvTags[0], saOutput, iNumChars );
    int* piOutput;
    qGotData = ( iNumChars == 3*3*(int)sizeof(int) );
    if ( qGotData ) {
      piOutput = (int*)saOutput.first();
      for ( int i=0; i<9; i++ )
	qGotData = qGotData && ( piOutput[i] == 4*(i+1) );
    }
    TEST ( qGotData );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    for ( int i=0; i<9; i++ )
      saInts.set( i, 5*(i+1) );
    AT.setIntArrData( saEnts, 3, apvTags[0], saInts, 9 );
    sidl::array<int> saIntsBack;
    AT.getIntArrData( saEnts, 3, apvTags[0], saIntsBack, iNumVals );
    qGotData = ( iNumVals == 9 );
    if ( qGotData ) {
      piOutput = saIntsBack.first();
      for ( int i=0; i<9; i++ )
	qGotData = qGotData && ( piOutput[i] == 5*(i+1) );
    }
    TEST ( qGotData );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with doubles...               ";
  try {
    // Try setting the data using the generic function...
    iNumChars = 9*sizeof(double);
    saChars = saChars.create1d(iNumChars);
    double adData[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };
    pcData = (char*)adData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    AT.setArrData( saEnts, 3, apvTags[1], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    iNumChars = 0;
    AT.getArrData( saEnts, 3, apvTags[1], saOutput, iNumChars );
    double* pdOutput;
    qGotData = ( iNumChars == 9*(int)sizeof(double) );
    if ( qGotData ) {
      pdOutput = (double*)saOutput.first();
      for ( int i=0; i<9; i++ )
	qGotData = qGotData && APPROX_EQUAL(pdOutput[i], ((double)(i+1))/10);
    }
    TEST ( qGotData );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    for ( int i=0; i<9; i++ )
      saDoubles.set(i, ((double)(i+1))/5);
    AT.setDblArrData( saEnts, 3, apvTags[1], saDoubles, 9 );
    sidl::array<double> saDoublesBack;
    AT.getDblArrData( saEnts, 3, apvTags[1], saDoublesBack, iNumVals );
    qGotData = ( iNumVals == 9 );
    if ( qGotData ) {
      pdOutput = saDoublesBack.first();
      for ( int i=0; i<9; i++ )
	qGotData = qGotData && APPROX_EQUAL(pdOutput[i],((double)(i+1))/5);
    }
    TEST ( qGotData );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with entity handles...        ";
  try {
    // Try setting the data using the generic function...
    iNumChars = 9*sizeof(void*);
    saChars = saChars.create1d(iNumChars);
    void* apvEntTemp[9];
    for ( int i=0; i<9; i++ )
      apvEntTemp[i] = apvEnts[i%3];
    pcData = (char*)apvEntTemp;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    AT.setArrData( saEnts, 3, apvTags[2], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    AT.getArrData( saEnts, 3, apvTags[2], saOutput, iNumChars );
    void** ppvOutput;
    qGotData = ( iNumChars == 9*(int)sizeof(void*) );
    if ( qGotData ) {
      ppvOutput = (void**)saOutput.first();
      for ( int i=0; i<9; i++ )
	qGotData = qGotData && ( ppvOutput[i] == apvEntTemp[i] );
    }
    TEST ( qGotData );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    for ( int i=0; i<9; i++ )
      saHandles.set(i, apvEnts[(i+1)%3]);
    AT.setEHArrData( saEnts, 3, apvTags[2], saHandles, 9 );
    sidl::array<void*> saHandlesBack;
    AT.getEHArrData( saEnts, 3, apvTags[2], saHandlesBack, iNumVals );
    qGotData = ( iNumVals == 9 );
    if ( qGotData ) {
      ppvOutput = saHandlesBack.first();
      for ( int i=0; i<9; i++ )
	qGotData = qGotData && ( ppvOutput[i] == apvEnts[(i+1)%3] );
    }
    TEST ( qGotData );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with generic data (bytes)...  ";
  try {
    // Try setting the data using the generic function...
    iNumChars = 6;
    saChars = saChars.create1d(iNumChars);
    char acData[] = "abcdef";
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, acData[i] );
    AT.setArrData( saEnts, 3, apvTags[3], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    AT.getArrData( saEnts, 3, apvTags[3], saOutput, iNumChars );
    qGotData = ( iNumChars == 6 );
    if ( qGotData ) {
      pcData = (char*)saOutput.first();
      for ( int i=0; i<iNumChars; i++ )
	qGotData = qGotData && ( pcData[i] == acData[i] );
    }
    TEST ( qGotData );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";


  // Testing error throwing
  sidl::array<void*> saNilArray;
  sidl::array<void*> sa2DArray = sa2DArray.create2dRow( 9, 9 );
  for ( int i=0; i<9; i++ )
    for ( int j=0; j<9; j++ )
      sa2DArray.set( i, j, apvEnts[j%3]);
  if ( qErrTests ) {
    TSTTM::Mesh icMesh = TSTTM::Factory::newMesh(CREATE_ARG);
    TSTTB::ArrTag AT2(icMesh);
    TSTTB::Tag  Tag2(icMesh);
    iNumChars = 9*sizeof(int);
    saOutput = saOutput.create1d( iNumChars );
    cout << "Testing error throwing for ArrTag methods...              ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      for ( int i=0; i<3; i++ )
	Tag2.createTag( astrTagNames[i], 3, aTagTypes[i], apvTags[i+4] );
    } SHOULD_NOT_THROW;
    try {
      AT2.setArrData( saEnts, 3, apvTags[4], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.setIntArrData( saEnts, 3, apvTags[4], saInts, 9 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.setDblArrData( saEnts, 3, apvTags[5], saDoubles, 9 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.setEHArrData( saEnts, 3, apvTags[6], saHandles, 9 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.getArrData( saEnts, 3, apvTags[4], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.getIntArrData( saEnts, 3, apvTags[4], saInts, iNumVals );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.getDblArrData( saEnts, 3, apvTags[5], saDoubles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      AT2.getEHArrData( saEnts, 3, apvTags[6], saHandles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );

    try {
      AT.setArrData( saBadEnts, 3, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid entity handle! ";
      TEST ( error_throwing );
      AT.rmvArrTag( saBadEnts, 3, apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.setArrData( saEnts, 3, pcData, saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.setArrData( saNilArray, 3, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.setArrData( sa2DArray, 3, apvTags[0], saOutput, iNumChars );
	cout << "\nERROR: Exception not thrown when setting tag data for "
	     << "an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    iNumChars = 12*sizeof(int);
    saOutput = saOutput.create1d(iNumChars);
    try {
      AT.setArrData( saEnts, 4, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    try {
      AT.setIntArrData( saEnts, 3, apvTags[1], saInts, 9 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for a double tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.setDblArrData( saEnts, 3, apvTags[0], saDoubles, 9 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.setEHArrData( saEnts, 3, apvTags[0], saHandles, 9 );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.setIntArrData( saBadEnts, 3, apvTags[0], saInts, 9 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
      AT.rmvArrTag( saBadEnts, 3, apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.setIntArrData( saNilArray, 3, apvTags[0], saInts, 9 );
      cout << "\nERROR: Exception not thrown when setting integer tag data for "
	   << "a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.setIntArrData( sa2DArray, 3, apvTags[0], saInts, 9 );
	cout << "\nERROR: Exception not thrown when setting integer tag data for "
	     << "an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    saInts = saInts.create1d(12);
    try {
      AT.setIntArrData( saEnts, 4, apvTags[0], saInts, 12 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    try {
      AT.setDblArrData( saBadEnts, 3, apvTags[1], saDoubles, 9 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
      AT.rmvArrTag( saBadEnts, 3, apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.setDblArrData( saNilArray, 3, apvTags[1], saDoubles, 9 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.setDblArrData( sa2DArray, 3, apvTags[1], saDoubles, 9 );
	cout << "\nERROR: Exception not thrown when setting double tag data "
	     << "for an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    saDoubles = saDoubles.create1d(12);
    try {
      AT.setDblArrData( saEnts, 4, apvTags[1], saDoubles, 12 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    try {
      AT.setEHArrData( saBadEnts, 3, apvTags[2], saHandles, 9 );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.setEHArrData( saEnts, 3, apvTags[2], saBadEnts, 9 );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.setEHArrData( saNilArray, 3, apvTags[2], saHandles, 9 );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.setEHArrData( sa2DArray, 3, apvTags[2], saHandles, 9 );
	cout << "\nERROR: Exception not thrown when setting entity handle tag "
	     << "data for an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    saHandles = saHandles.create1d(12);
    try {
      AT.setEHArrData( saEnts, 4, apvTags[2], saHandles, 12 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    try {
      AT.getArrData( saBadEnts, 3, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.getArrData( saEnts, 3, pcData, saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.getArrData( saNilArray, 3, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.getArrData( sa2DArray, 3, apvTags[0], saOutput, iNumChars );
	cout << "\nERROR: Exception not thrown when getting tag data for "
	     << "an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.getArrData( saEnts, 4, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saOutput = saOutput.create1d(1);
    try {
      AT.getArrData( saEnts, 3, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid output array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    try {
      AT.getIntArrData( saEnts, 3, apvTags[1], saInts, iNumVals );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for a double tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.getDblArrData( saEnts, 3, apvTags[0], saDoubles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.getEHArrData( saEnts, 3, apvTags[0], saHandles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.getIntArrData( saBadEnts, 3, apvTags[0], saInts, iNumVals );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.getIntArrData( saNilArray, 3, apvTags[0], saInts, iNumVals );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.getIntArrData( sa2DArray, 3, apvTags[0], saInts, iNumVals );
	cout << "\nERROR: Exception not thrown when getting integer tag data "
	     << "for an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.getIntArrData( saEnts, 4, apvTags[0], saInts, iNumVals );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saInts = saInts.create1d(1);
    try {
      AT.getIntArrData( saEnts, 3, apvTags[0], saInts, iNumVals );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid output array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000) {
      saInts = saInts.create2dRow(3, 3);
      try {
	AT.getIntArrData( saEnts, 3, apvTags[0], saInts, iNumVals );
	cout << "\nERROR: Exception not thrown when getting integer tag data "
	     << "for an invalid output array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );
    }

    try {
      AT.getDblArrData( saBadEnts, 3, apvTags[1], saDoubles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.getDblArrData( saNilArray, 3, apvTags[1], saDoubles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.getDblArrData( sa2DArray, 3, apvTags[1], saDoubles, iNumVals );
	cout << "\nERROR: Exception not thrown when getting double tag data "
	     << "for an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.getDblArrData( saEnts, 4, apvTags[1], saDoubles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saDoubles = saDoubles.create1d(1);
    try {
      AT.getDblArrData( saEnts, 3, apvTags[1], saDoubles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid output array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saDoubles = saDoubles.create2dRow(3, 3);

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.getDblArrData( saEnts, 3, apvTags[1], saDoubles, iNumVals );
	cout << "\nERROR: Exception not thrown when getting double tag data "
	     << "for an invalid output array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.getEHArrData( saBadEnts, 3, apvTags[2], saHandles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.getEHArrData( saNilArray, 3, apvTags[2], saHandles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.getEHArrData( sa2DArray, 3, apvTags[2], saHandles, iNumVals );
	cout << "\nERROR: Exception not thrown when getting entity handle tag "
	     << "data for an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.getEHArrData( saEnts, 4, apvTags[2], saHandles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saHandles = saHandles.create1d(1);
    try {
      AT.getEHArrData( saEnts, 3, apvTags[2], saHandles, iNumVals );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid output array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saHandles = saHandles.create2dRow(3, 3);

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.getEHArrData( saEnts, 3, apvTags[2], saHandles, iNumVals );
	cout << "\nERROR: Exception not thrown when getting entity handle tag "
	     << "data for an invalid output array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.rmvArrTag( saEnts, 3, saEnts[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      AT.rmvArrTag( saBadEnts, 3, apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid entity handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      AT.rmvArrTag( saNilArray, 3, apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for a nil input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	AT.rmvArrTag( sa2DArray, 3, apvTags[0] );
	cout << "\nERROR: Exception not thrown when removing tag "
	     << "data for an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      AT.rmvArrTag( saEnts, 4, apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag "
	   << "data for an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    cout << "DONE\n";

  }
  cout << "Testing tag removal and forced/unforced destruction...    ";
  try {
    AT.rmvArrTag( saEnts, 3, apvTags[0] );
    AT.rmvArrTag( saEnts, 3, apvTags[1] );
    TagInt.destroyTag( apvTags[0], false );
    TagInt.destroyTag( apvTags[1], false );
    iNPassed++;
  } SHOULD_NOT_THROW;
  try {
    TagInt.destroyTag( apvTags[2], false );
    cout << "\nERROR: Exception not thrown when trying to destroy tag "
	 << "with data without using 'forced'! ";
    TEST ( error_throwing );
  } MUST_THROW ( TSTTB::ErrorType_TAG_IN_USE );
  try {
    TagInt.destroyTag( apvTags[3], false );
    cout << "\nERROR: Exception not thrown when trying to destroy tag "
	 << "with data without using 'forced'! ";
    TEST ( error_throwing );
  } MUST_THROW ( TSTTB::ErrorType_TAG_IN_USE );
  try {
    TagInt.destroyTag( apvTags[2], true );
    TagInt.destroyTag( apvTags[3], true );
    iNPassed++;
  } SHOULD_NOT_THROW;
  cout << "DONE\n";
}

static void vTestSetTagInterface(TSTTM::Mesh& stMesh)
{
  sidl::array<char> saOutput;
  sidl::array<void*> saGotTags;
  void* apvTags[7];
  void* apvSets[6] = {NULL};
  char* pcData;
  void* apvEnts[3];
  int iNumChars;
  // Testing normal operation of SetTag
  TSTTB::Tag    TagInt(stMesh);
  TSTTB::SetTag ST(stMesh);
  TSTTB::EntSet ES(stMesh);
  TSTTM::Entity EntMesh(stMesh);
  //     getEntSetData, getEntSetIntData, getEntSetDblData,
  //     getEntSetEHData, setEntSetData, setEntSetIntData,
  //     setEntSetDblData, setEntSetEHData, getAllEntSetTags,
  //     rmvEntSetTag
  cout << "Testing entity set tagging functionality...\n";
  pvRootSet = EntMesh.getRootSet();
  void* pvIter;
  try {
    // Get at least 3 entities to work with
    if ( !EntMesh.initEntIter(pvRootSet, TSTTM::EntityType_FACE,
			      TSTTM::EntityTopology_ALL_TOPOLOGIES, pvIter) ) {
      cout << "\nCould not get mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( !EntMesh.getNextEntIter(pvIter, apvEnts[0]) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( !EntMesh.getNextEntIter(pvIter, apvEnts[1]) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    if ( !EntMesh.getNextEntIter(pvIter, apvEnts[2]) ) {
      cout << "\nCould not get enough mesh data to work with... exiting...\n";
      TEST ( data_preparation );
      return;
    }
    // Create a tag of each type and 3 entity sets to be tagged
    for ( int i=0; i<3; i++ ) {
      TagInt.createTag( astrTagNames[i], 1, aTagTypes[i], apvTags[i] );
      ES.createEntSet( false, apvSets[i] );
      if ( apvSets[i] == NULL ) {
	cout << "Could not create entity sets to work with... "
	     << "exiting...\n";
	TEST ( data_preparation );
	return;
      }
    }
    // Be sure to test tagging of the root set (mesh) as well.
    apvSets[0] = pvRootSet;
    TagInt.createTag( astrTagNames[3], 6, aTagTypes[3], apvTags[3] );
  } SHOULD_NOT_THROW;

  cout << "Testing get and set methods with integers...              ";
  sidl::array<char> saChars;
  try {
    // Try setting the data using the generic function...
    iNumChars = sizeof(int);
    saChars = saChars.create1d(iNumChars);
    int* piData = new int(25);
    pcData = (char*)piData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    delete piData;
    ST.setEntSetData( apvSets[0], apvTags[0], saChars, iNumChars );
    // ... then seeing if it comes back intact
    ST.getEntSetData( apvSets[0], apvTags[0], saOutput, iNumChars );
    TEST ( iNumChars == (int)sizeof(int) );
    TEST ( *(int*)saOutput.first() == 25 );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    ST.setEntSetIntData( apvSets[0], apvTags[0], 30 );
    TEST ( ST.getEntSetIntData(apvSets[0], apvTags[0]) == 30 );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with doubles...               ";
  try {
    // Try setting the data using the generic function...
    iNumChars = sizeof(double);
    saChars = saChars.create1d(iNumChars);
    double* pdData = new double(1.8);
    pcData = (char*)pdData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    delete pdData;
    ST.setEntSetData( apvSets[0], apvTags[1], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    ST.getEntSetData( apvSets[0], apvTags[1], saOutput, iNumChars );
    TEST ( iNumChars == (int)sizeof(double) );
    TEST ( *(double*)saOutput.first() == 1.8 );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    ST.setEntSetDblData( apvSets[0], apvTags[1], 1.9 );
    TEST ( ST.getEntSetDblData(apvSets[0], apvTags[1]) == 1.9 );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with entity handles...        ";
  try {
    // Try setting the data using the generic function...
    iNumChars = sizeof(void*);
    saChars = saChars.create1d(iNumChars);
    void** ppvData = new void*(apvEnts[0]);
    pcData = (char*)ppvData;
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, pcData[i] );
    delete ppvData;
    ST.setEntSetData( apvSets[0], apvTags[2], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    ST.getEntSetData( apvSets[0], apvTags[2], saOutput, iNumChars );
    TEST ( iNumChars == (int)sizeof(void*) );
    TEST ( *(void**)saOutput.first() == apvEnts[0] );
    // Now try to change the data using the specific function
    // and ensure it comes back intact
    ST.setEntSetEHData( apvSets[0], apvTags[2], apvEnts[1] );
    TEST ( ST.getEntSetEHData(apvSets[0], apvTags[2]) == apvEnts[1] );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing get and set methods with generic data (bytes)...  ";
  try {
    // Try setting the data using the generic function...
    iNumChars = 6;
    saChars = saChars.create1d(iNumChars);
    char acData[] = "abcdef";
    for ( int i=0; i<iNumChars; i++ )
      saChars.set( i, acData[i] );
    ST.setEntSetData( apvSets[0], apvTags[3], saChars, iNumChars );
    // ... then seeing if it comes back intact
    saOutput = saOutput.create1d(iNumChars);
    ST.getEntSetData( apvSets[0], apvTags[3], saOutput, iNumChars );
    bool qGotData = ( iNumChars == 6 );
    if ( qGotData ) {
      pcData = (char*)saOutput.first();
      for ( int i=0; i<iNumChars; i++ )
	qGotData = qGotData && ( pcData[i] == acData[i] );
    }
    TEST ( qGotData );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";

  cout << "Testing tag finding...                                    ";
  try {
    int iNumTags;
    ST.getAllEntSetTags( apvSets[0], saGotTags, iNumTags );
    bool qTagsPresent = false;
    if ( iNumTags == 4 )
      GOT_ALL_ITEMS ( apvTags, saGotTags, iNumTags, qTagsPresent );
    TEST ( qTagsPresent );
  } SHOULD_NOT_THROW;
  cout << "DONE\n";


  // Testing error behavior
  sidl::array<char> saNilArray;
  sidl::array<char> sa2DArray =
    sa2DArray.create2dRow(sizeof(int), sizeof(int));
  if ( qErrTests ) {
    TSTTM::Mesh icMesh = TSTTM::Factory::newMesh(CREATE_ARG);
    TSTTB::EntSet ES2(icMesh);
    TSTTB::Tag Tag2(icMesh);
    TSTTB::SetTag ST2(icMesh);
    cout << "Testing error throwing for SetTag methods...              ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    try {
      saOutput = saOutput.create1d( sizeof(int) );
      iNumChars = sizeof(int);
      for ( int i=0; i<3; i++ ) {
	Tag2.createTag( astrTagNames[i], 1, aTagTypes[i], apvTags[i+4] );
	ES2.createEntSet( false, apvSets[i+3] );
	if ( apvSets[i+3] == NULL ) {
	  cout << "Could not create entity sets to work with... "
	       << "exiting...\n";
	  TEST ( data_preparation );
	  return;
	}
      }
    } SHOULD_NOT_THROW;
    try {
      ST2.setEntSetData( apvSets[3], apvTags[4], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.setEntSetIntData( apvSets[3], apvTags[4], 15 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.setEntSetDblData( apvSets[3], apvTags[5], 1.5 );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.setEntSetEHData( apvSets[3], apvTags[6], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.getEntSetData( apvSets[3], apvTags[4], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.getEntSetIntData( apvSets[3], apvTags[4] );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.getEntSetDblData( apvSets[3], apvTags[5] );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.getEntSetEHData( apvSets[3], apvTags[6] );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );
    try {
      ST2.rmvEntSetTag( apvSets[3], apvTags[6] );
      cout << "\nERROR: Exception not thrown when removing tag data for "
	   << "a mesh with no data! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NO_DATA );

    try {
      ST.getEntSetData( pcData, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetData( apvEnts[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetData( apvSets[0], pcData, saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    saOutput = saOutput.create1d(1);
    try {
      ST.getEntSetData( apvSets[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when getting tag data for "
	   << "an invalid output array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	ST.getEntSetData( apvSets[0], apvTags[0], sa2DArray, iNumChars );
	cout << "\nERROR: Exception not thrown when getting tag data for "
	     << "an invalid output array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    saOutput = saOutput.create1d( sizeof(int) );
    iNumChars = sizeof(int);
    try {
      ST.setEntSetData( pcData, apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( pcData, apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetData( apvEnts[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( apvEnts[0], apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetData( apvSets[0], pcData, saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    saOutput = saOutput.create1d(1);
    try {
      ST.setEntSetData( apvSets[0], apvTags[0], saNilArray, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an empty input array! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_NIL_ARRAY );
    try {
      ST.setEntSetData( apvSets[0], apvTags[0], saOutput, iNumChars );
      cout << "\nERROR: Exception not thrown when setting tag data for "
	   << "an invalid input array size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	ST.setEntSetData( apvSets[0], apvTags[0], sa2DArray, iNumChars );
	cout << "\nERROR: Exception not thrown when setting tag data for "
	     << "an invalid input array dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    try {
      ST.setEntSetIntData( apvSets[0], apvTags[1], 15 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for a double tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.setEntSetDblData( apvSets[0], apvTags[0], 1.5 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.setEntSetEHData( apvSets[0], apvTags[0], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.getEntSetIntData( apvSets[0], apvTags[1] );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for a double tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.getEntSetDblData( apvSets[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.getEntSetEHData( apvSets[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an integer tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.setEntSetIntData( pcData, apvTags[0], 15 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( pcData, apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetIntData( apvTags[0], apvTags[0], 15 );
      cout << "\nERROR: Exception not thrown when setting integer tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( apvTags[0], apvTags[0] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetDblData( pcData, apvTags[1], 1.5 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( pcData, apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetDblData( apvTags[0], apvTags[1], 1.5 );
      cout << "\nERROR: Exception not thrown when setting double tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( apvTags[0], apvTags[1] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetEHData( pcData, apvTags[2], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( pcData, apvTags[2] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetEHData( apvTags[0], apvTags[2], apvEnts[0] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity set handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( apvTags[0], apvTags[2] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.setEntSetEHData( apvSets[0], apvTags[2], pcData );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( apvSets[0], apvTags[2] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      ST.setEntSetEHData( apvSets[0], apvTags[2], apvSets[0] );
      cout << "\nERROR: Exception not thrown when setting entity handle tag "
	   << "data for an invalid entity handle! ";
      TEST ( error_throwing );
      ST.rmvEntSetTag( apvSets[0], apvTags[2] );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITY_HANDLE );
    try {
      ST.getEntSetIntData( pcData, apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetIntData( apvTags[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when getting integer tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetDblData( pcData, apvTags[1] );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetDblData( apvTags[0], apvTags[1] );
      cout << "\nERROR: Exception not thrown when getting double tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetEHData( pcData, apvTags[2] );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.getEntSetEHData( apvTags[0], apvTags[2] );
      cout << "\nERROR: Exception not thrown when getting entity handle tag "
	   << "data for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.rmvEntSetTag( apvSets[0], apvSets[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid tag handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_TAG_HANDLE );
    try {
      ST.rmvEntSetTag( pcData, apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    try {
      ST.rmvEntSetTag( apvTags[0], apvTags[0] );
      cout << "\nERROR: Exception not thrown when removing tag data "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    saGotTags = saGotTags.create1d(4);
    try {
      ST.getAllEntSetTags( apvTags[0], saGotTags, iNumChars );
      cout << "\nERROR: Exception not thrown when getting all tags "
	   << "for an invalid entity set handle! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_INVALID_ENTITYSET_HANDLE );
    saGotTags = saGotTags.create1d(1);
    try {
      ST.getAllEntSetTags( apvSets[0], saGotTags, iNumChars );
      cout << "\nERROR: Exception not thrown when getting all tags "
	   << "into an array of the wrong size! ";
      TEST ( error_throwing );
    } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_SIZE );
    saGotTags = saGotTags.create2dRow(5, 5);

    // Babel 0.9.x flattens arrays to 1D automagically, so this test isn't
    // correct.
    if (BABEL_SERIAL >= 1001000)
      try {
	ST.getAllEntSetTags( apvSets[0], saGotTags, iNumChars );
	cout << "\nERROR: Exception not thrown when getting all tags "
	     << "into an array of the wrong dimension! ";
	TEST ( error_throwing );
      } MUST_THROW ( TSTTB::ErrorType_BAD_ARRAY_DIMENSION );

    cout << "DONE\n";
  }
  cout << "Testing tag removal and forced/unforced destruction...    ";
  try {
    ST.rmvEntSetTag( apvSets[0], apvTags[0] );
    ST.rmvEntSetTag( apvSets[0], apvTags[1] );
    TagInt.destroyTag( apvTags[0], false );
    TagInt.destroyTag( apvTags[1], false );
    iNPassed++;
  } SHOULD_NOT_THROW;
  try {
    TagInt.destroyTag( apvTags[2], false );
    cout << "\nERROR: Exception not thrown when trying to destroy tag "
	 << "with data without using 'forced'! ";
    TEST ( error_throwing );
  } MUST_THROW ( TSTTB::ErrorType_TAG_IN_USE );
  try {
    TagInt.destroyTag( apvTags[3], false );
    cout << "\nERROR: Exception not thrown when trying to destroy tag "
	 << "with data without using 'forced'! ";
    TEST ( error_throwing );
  } MUST_THROW ( TSTTB::ErrorType_TAG_IN_USE );
  try {
    TagInt.destroyTag( apvTags[2], true );
    TagInt.destroyTag( apvTags[3], true );
    iNPassed++;
  } SHOULD_NOT_THROW;
  cout << "DONE\n";
}

//@ Test iterators in the presence of deletion and addition of entities.
// Basically, getting iterators to always behave properly when entities
// are added or deleted during iteration is a non-trivial exercise.  The
// current test checks only iterators for edges and triangles (for 2D
// meshes), or triangles and tetrahedra (for 3D meshes), as those are
// what's needed for the swapping service.  Other iterators should be
// bomb-proofed in the same way, and should be checked eve

//@ Main program

int main(int argc, char *argv[] )
{
  // Check command line arg
  int iFile = 1;
  string sArg;
  string filename = "";
  const char* sOutFile;
  while ( iFile < argc ) {
    sArg = argv[iFile];
    if ( sArg == "-h" || sArg == "--help" ) {
      TYPE_HELP;
      TYPE_USAGE;
      TYPE_USAGE_HELP;
      return 0;
    } else if ( sArg == "-e" || sArg == "--error" ) {
      qErrTests = true;
    } else if ( sArg == "-s" || sArg == "--silent" ) {
      qSilent = true;
    } else if (argv[iFile][0] == '-' && argv[iFile][1] == 'o') {
      if ( argv[iFile][2] == '\0' ) {
	sOutFile = "out_test.txt";
      } else {
	sOutFile = ( argv[iFile]+2 );
      }
      qOutFile = true;
    } else if (argv[iFile][0] == '-' && argv[iFile][1] == '-' &&
	       argv[iFile][2] == 'o' && argv[iFile][3] == 'u' &&
	       argv[iFile][4] == 't' && argv[iFile][5] == 'p' &&
	       argv[iFile][6] == 'u' && argv[iFile][7] == 't') {
      if ( argv[iFile][8] != '\0' && argv[iFile][8] != '=' ) {
	sArg = ( argv[iFile]+2 );
	TYPE_USAGE;
	std::cerr << "Invalid option: '" << sArg << "'.  Try '"
		  << argv[0] << " --help'." << endl << endl;
	return 1;
      } else if ( argv[iFile][8] == '=' ) {
	if ( argv[iFile][9] == '\0' ) {
	  TYPE_USAGE;
	  std::cerr << "No output file given!  Try '" << argv[0]
		    << " --help'." << endl << endl;
	  return 1;
	}
	sOutFile = (argv[iFile]+9);
      } else {
	sOutFile = "out_test.txt";
      }
      qOutFile = true;
    } else if ( argv[iFile][0] == '-' ) {
      if ( argv[iFile][1] == '-' )
	sArg = (argv[iFile]+2);
      else
	sArg = (argv[iFile]+1);
      TYPE_USAGE;
      std::cerr << "Invalid option: '" << sArg << "'.  Try '"
		<< argv[0] << " --help'." << endl << endl;
      return 1;
    } else {
      filename = argv[iFile];
    }
    iFile++;
  }
  if ( filename == "" ) {
    TYPE_USAGE;
    std::cerr << "Need an input file!  Try '" << argv[0]
	      << " --help'." << endl << endl;
    return 1;
  }

#ifdef SETVBUF_REVERSED
  setvbuf(stdout, _IONBF, NULL, 1);
#else
  setvbuf(stdout, NULL, _IONBF, 1);
#endif

  std::streambuf* outbuf = cout.rdbuf();
  std::streambuf* errbuf = std::cerr.rdbuf();
  std::ofstream file, nullFile;
  if ( qOutFile ) {
    file.open(sOutFile);
    if ( !file.is_open() ) {
      std::cerr << endl << "Error: Could not open output file '"
		<< sOutFile << "'!" << endl << endl;
      return 1;
    }
    cout.rdbuf( file.rdbuf() );
    std::cerr.rdbuf( file.rdbuf() );
  }
  if ( qSilent ) {
    nullFile.open("/dev/null/");
    cout.rdbuf( nullFile.rdbuf() );
    std::cerr.rdbuf( nullFile.rdbuf() );
  }

  TSTTM::Mesh icMesh = TSTTM::Factory::newMesh(CREATE_ARG);

  vSetupTypeAndTopo();

  {
    // EVERYTHING in the TSTTM interface extends TSTTM::Mesh, so this had
    // better work!  Otherwise, this implementation has nothing at all.
    TSTTM::Mesh mesh = icMesh;
    TEST(mesh._not_nil());
    if (mesh._is_nil()) {
      cout << "This appears not to be a TSTTM implementation.\n";
      return 1;
    }

    // Test stuff specific to interface Mesh.  Those functions are:
    //    load, save, getRootSet, getGeometricDim, getDfltStorage,
    //    getAdjTable, getNumOfType, getNumOfTopo, getAllVtxCoords,
    //    getVtxCoordIndex, getEntities, getVtxArrCoords, and
    //    getAdjEntities

    cout << "Testing mesh interface..." << endl;
    qAllPassed = true;
    vTestMeshInterface(mesh, filename);
    if ( qAllPassed )
      iNIntPassed++;
    else
      iNIntFailed++;
    TEST_STATS;
  }

  // Test stuff specific to interface Entity.  Those functions are:
  //    initEntIter, getNextEntIter, resetEntIter, endEntIter,
  //    getEntTopo, getEntType, getVtxCoord, getEntAdj
  bool qEntityPassed = false;
  {
    TSTTM::Entity teMesh = icMesh;
    if (teMesh._not_nil()) {
      cout << "Testing single entity interface..." << endl;
      qAllPassed = true;
      vTestEntityInterface(teMesh);
      if ( qAllPassed ) {
	qEntityPassed = true;
	iNIntPassed++;
      }
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Entity-based query and traversal not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface Arr.  Those functions are:
  //     initEntArrIter, getEntArrNextIter, resetEntArrIter,
  //     endEntArrIter, getEntArrTopo, getEntArrType, getEntArrAdj
  {
    TSTTM::Arr taMesh = icMesh;
    if (taMesh._not_nil()) {
      cout << "Testing entity array interface..." << endl;
      qAllPassed = true;
      vTestArrInterface(taMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Array-based query and traversal not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface Modify.  Those functions are:
  //     setVtxCoords, createVtx, createEnt, deleteEnt
  bool qModifyPassed = false;
  {
    TSTTM::Modify tmMesh = icMesh;
    if (tmMesh._not_nil()) {
      cout << "Testing single entity modification interface..." << endl;
      qAllPassed = true;
      vTestModifyInterface(tmMesh);
      if ( qAllPassed ) {
	qModifyPassed = true;
	iNIntPassed++;
      }
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Mesh modification not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface ModArr.  Those functions are:
  //     setVtxArrCoords, createVtxArr, createEntArr, deleteEntArr
  {
    TSTTM::ArrMod tmaMesh = icMesh;
    if (tmaMesh._not_nil()) {
      cout << "Testing entity array modification interface..." << endl;
      qAllPassed = true;
      vTestArrModInterface(tmaMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Array-based mesh modification not implemented.\n" << endl;
    }
  }

  // Tag and entity set testing, appended June 2005 by Ben Kerby

  // Test stuff specific to interface EntSet.  Those functions are:
  //     createEntSet, destroyEntSet, isList, getNumEntSets, getEntSets,
  //     addEntToSet, rmvEntFromSet, addEntArrToSet, rmvEntArrFromSet,
  //     addEntSet, rmvEntSet, isContainedIn
  {
    TSTTB::EntSet tesMesh = icMesh;
    TSTTM::Entity teMesh = icMesh;
    TSTTM::Arr taMesh = icMesh;
    if (tesMesh._not_nil() && teMesh._not_nil() && taMesh._not_nil()) {
    cout << "Testing entity set interface..." << endl;
      qAllPassed = true;
      vTestEntSetInterface(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Entity sets not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface SetRelation.  Those functions are:
  //     addPrntChld, rmvPrntChld, isChildOf, getNumChld, getNumPrnt,
  //     getChldn, getPrnts
  {
    TSTTB::SetRelation tsrMesh = icMesh;
    if (tsrMesh._not_nil()) {
      cout << "Testing set relation interface..." << endl;
      qAllPassed = true;
      vTestSetRelationInterface(tsrMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Set relations not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface SetBoolOps.  Those functions are:
  //     subtract, intersect, unite
  {
    TSTTB::SetBoolOps tsboMesh = icMesh;
    TSTTM::Entity teMesh = icMesh;
    if (tsboMesh._not_nil() && teMesh._not_nil()) {
      cout << "Testing set boolean operations interface..." << endl;
      qAllPassed = true;
      vTestBoolOpsInterface(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Set boolean operations not implemented.\n" << endl;
    }
  }

  // Now that entity sets have been tested, go back and re-test query
  // using entity sets.

  // Retest stuff specific to interface Mesh that can take entity_set
  // args.  Those functions are:
  //    getNumOfType, getNumOfTopo, getAllVtxCoords,
  //    getVtxCoordIndex, getEntities, and getAdjEntities

  {
    TSTTB::EntSet tesMesh = icMesh;
    TSTTM::Mesh tMesh = icMesh;
    if (tesMesh._not_nil() && tMesh._not_nil()) {
      cout << "Testing mesh interface with entity set args..." << endl;
      qAllPassed = true;
      vTestMeshWithSets(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Mesh interface with entity set args not implemented.\n" << endl;
    }
  }

  // Test single-entity iterators over EntitySets. Those functions are:
  //    initEntIter, getNextEntIter, resetEntIter, endEntIter
  {
    TSTTM::Entity teMesh = icMesh;
    TSTTB::EntSet tesMesh = icMesh;
    if (teMesh._not_nil() && tesMesh._not_nil()) {
      cout << "Testing single entity iterators with entity sets..." << endl;
      qAllPassed = true;
      vTestEntityWithSets(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Single entity iterators with entity sets not implemented.\n" << endl;
    }
  }

  // Test array iterators over EntitySets. Those functions are:
  //     initEntArrIter, getEntArrNextIter, resetEntArrIter,
  //     endEntArrIter
  {
    TSTTM::Arr taMesh = icMesh;
    TSTTB::EntSet tesMesh = icMesh;
    if (taMesh._not_nil() && tesMesh._not_nil()) {
      cout << "Testing entity array iterators over entity sets..." << endl;
      qAllPassed = true;
      vTestArrayWithSets(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Entity array iterators over entity sets not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface Tag.  Those functions are:
  //     createTag, destroyTag, getTagName, getTagSizeValues,
  //     getTagSizeBytes, getTagHandle, getTagType
  {
    TSTTB::Tag ttMesh = icMesh;
    if (ttMesh._not_nil()) {
      cout << "Testing tag interface..." << endl;
      qAllPassed = true;
      vTestTagInterface(ttMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Tags not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface EntTag.  Those functions are:
  //     getData, getIntData, getDblData, getEHData, setData,
  //     setIntData, setDblData, setEHData, getAllTags, rmvTag
  {
    TSTTB::EntTag tetMesh = icMesh;
    TSTTM::Entity teMesh = icMesh;
    if (tetMesh._not_nil() && teMesh._not_nil()) {
      cout << "Testing entity tag interface..." << endl;
      qAllPassed = true;
      vTestEntTagInterface(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Entity tags not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface ArrTag.  Those functions are:
  //     getArrData, getIntArrData, getDblArrData, getEHArrData,
  //     setArrData, setIntArrData, setDblArrData, setEHArrData,
  //     rmvArrTag
  {
    TSTTB::ArrTag tatMesh = icMesh;
    TSTTM::Entity teMesh = icMesh;
    if (tatMesh._not_nil() && teMesh._not_nil()) {
      cout << "Testing array tag interface..." << endl;
      qAllPassed = true;
      vTestArrTagInterface(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Array tags not implemented.\n" << endl;
    }
  }

  // Test stuff specific to interface SetTag.  Those functions are:
  //     getEntSetData, getEntSetIntData, getEntSetDblData,
  //     getEntSetEHData, setEntSetData, setEntSetIntData,
  //     setEntSetDblData, setEntSetEHData, getAllEntSetTags,
  //     rmvEntSetTag
  {
    TSTTB::SetTag tstMesh = icMesh;
    TSTTB::EntSet tesMesh = icMesh;
    if (tstMesh._not_nil() && tesMesh._not_nil()) {
      cout << "Testing entity set tag interface..." << endl;
      qAllPassed = true;
      vTestSetTagInterface(icMesh);
      if ( qAllPassed )
	iNIntPassed++;
      else
	iNIntFailed++;
      TEST_STATS;
    }
    else {
      iNNotImpl++;
      cout << "Entity set tags not implemented.\n" << endl;
    }
  }

  // These tests aren't yet ready for primetime, so they're commented out.  
  
  // Now test the behavior of iterators in the presence of mesh
  // modification, provided that both Entity and Modify interfaces
  // passed their tests.
//   if (qEntityPassed && qModifyPassed) {
//     qAllPassed = true;
//     vTestIterWithModify(icMesh.getGeometricDim());
//     vTestSetTagInterface(icMesh);
//     if ( qAllPassed )
//       iNIntPassed++;
//     else
//       iNIntFailed++;
//     TEST_STATS;
//   }
//   else {
//     iNNotImpl++;
//     cout << "Can't test iterators with Modify; either Entity or Modify failed a test.\n" << endl;
//   }

  if ( qSilent || qOutFile ) {
    cout.rdbuf( outbuf );
    std::cerr.rdbuf( errbuf );
  }

  std::stringstream stst;

  stst << endl << "FINAL RESULTS: " << std::setw(4) << iNPassed
       << " tests passed, " << endl << "               "
       << std::setw(4) << iNFailed << " tests failed in a total of "
       << endl << "               " << std::setw(4)
       << iNPassed+iNFailed << " tests, a pass rate of "
       << (int)100*iNPassed/(iNPassed+iNFailed) << "%." << endl
       << endl << "               " << std::setw(4)
       << iNIntPassed+iNIntFailed+iNNotImpl
       << " interfaces/interface combinations were tested: " << endl
       << "               " << std::setw(4) << iNIntPassed
       << " passed all interface-specific tests"
       << ( iNNotImpl == 0 ? " and" : ", " ) << endl
       << "               " << std::setw(4) << iNIntFailed
       << " failed at least one test";
  if ( iNNotImpl != 0 ) {
    stst << ", and " << endl << "               " << std::setw(4)
	 << iNNotImpl << ( iNNotImpl==1 ? " was " : " were ")
	 << "not implemented. " << endl << endl;
  } else
    stst << "." << endl << endl;

  cout << stst.str();

  if ( qOutFile ) {
    file << stst.str();
    file.close();
  }

  exit(iNFailed);
}
