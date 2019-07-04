#include "iMesh_unitTest.hh"

// Test stuff specific to interface Modify.  Those functions are:
//     setVtxCoords, createVtx, createEnt, deleteEnt
void vTestModifyInterface(iMesh_Instance& instance)
{
  int err;
  cout << "Modify interface is currently only tested thoroughly in 2D" << endl;
  int iGeomDim;
  iMesh_getGeometricDimension(instance, &iGeomDim, &err);
  CHECK_ERR(err);
  bool qOK;
  int iCoords1Size, iNVerts;
  int SO = iBase_INTERLEAVED;

  {
    // setVtxCoords is trivial to test:  get a vertex, change its coords,
    // and verify that the change worked.
    iBase_EntityHandle *aVerts = NULL;
    int iVertsAlloc = 0;
    iMesh_getEntities(instance, pvRootSet, iBase_VERTEX, iMesh_POINT,
		      &aVerts, &iVertsAlloc, &iNVerts, &err);
    CHECK_ERR(err);

    double *adCoords1 = NULL;
    int iCoords1Alloc = 0;
    iMesh_getVtxArrCoords(instance, aVerts, iNVerts,
			  SO, &adCoords1, &iCoords1Alloc, &iCoords1Size,
			  &err);
    CHECK_ERR(err);
    iBase_EntityHandle vertex = aVerts[(0)];

    int iVert = 0;
    {
      cout << " Setting vertex coordinates..." << endl;
      double adCoords[iGeomDim];
      for (int i = 0; i < iGeomDim; i++) {
	adCoords[i] = i + adCoords1[(i)];
      }

      if (iGeomDim == 2) {
	// In 2D, should throw an error here.
	iMesh_setVtxCoord(instance, vertex, adCoords[0], adCoords[1], 0, &err);
	CHECK_ERR(err);
      }
      else if (iGeomDim == 3) {
	iMesh_setVtxCoord(instance, vertex,
			  adCoords[0], adCoords[1], adCoords[2], &err);
	CHECK_ERR(err);
      }

      double * adCoords2 = NULL;
      int iCoords2Size;
      int iCoords2Alloc = 0;
      iMesh_getVtxArrCoords(instance, aVerts, iNVerts, SO,
			    &adCoords2, &iCoords2Alloc, &iCoords2Size, &err);
      CHECK_ERR(err);

      qOK = (iCoords2Size == iCoords1Size);
      for (int i = 0; i < iGeomDim; i++) {
	qOK = qOK && (APPROX_EQUAL(adCoords1[(i)],
				   adCoords2[(i)] - i));
      }
      TEST(qOK);
      UT_FREE(adCoords2);
      UT_FREE(aVerts);
      
      // Now set them back.
      if (iGeomDim == 3)
	iMesh_setVtxCoord(instance, vertex, adCoords1[0], adCoords1[1],
			  adCoords1[2], &err);
      else 
	iMesh_setVtxCoord(instance, vertex, adCoords1[0], adCoords1[1],
			  0, &err);
      CHECK_ERR(err);
      UT_FREE(adCoords1);
    }
  }
  vDoSaveLoad(&instance, &pvRootSet);


  // Now create a new vertex.  Then verify that its handle and
  // coordinates are returned properly.
  {
    cout << " Creating a new vertex..." << endl;
    double adCoords[] = {0,0,0};

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

    iBase_EntityHandle new_vert;
    if (iGeomDim == 2) {
      iMesh_createVtx(instance, adCoords[0], adCoords[1], 0, &new_vert, &err);
      CHECK_ERR(err);
    }
    else if (iGeomDim == 3) {
      iMesh_createVtx(instance, adCoords[0], adCoords[1], adCoords[2],
		      &new_vert, &err);
      CHECK_ERR(err);
    }

    double adCoords2[] = {0,0,0};
    iMesh_getVtxCoord(instance, new_vert,
		      &adCoords2[0], &adCoords2[1], &adCoords2[2], &err);
    CHECK_ERR(err);

    for (int i = 0; i < iGeomDim; i++) {
      qOK = qOK && (APPROX_EQUAL(adCoords2[i], adCoords[i]));
    }
    TEST(qOK);
    iMesh_deleteEnt(instance, new_vert, &err);
    CHECK_ERR(err);
    // Make sure it actually got deleted.
    {
      int iNTopo;
      iMesh_getNumOfTopo(instance, pvRootSet, iMesh_POINT, &iNTopo, &err);
      CHECK_ERR(err);
      TEST(iNTopo * iGeomDim == iCoords1Size);
    }
  }

  // Test entity creation.  All created entities go into the following
  // containers for orderly deletion later.   Note that
  // iMesh must support Modify, or we wouldn't even be in
  // this subroutine.
  iMesh_Instance new_instance;
  iMesh_newMesh("", &new_instance, &err, 0);

  if (iGeomDim == 2) {
    iMesh_setGeometricDimension(new_instance, 2, &err);
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

    iBase_EntityHandle aNewVerts[8];
    for (int iV = 0; iV < 8; iV++) {
      iMesh_createVtx(new_instance, adAllCoords[2*iV], adAllCoords[2*iV+1], 0,
		      &aNewVerts[iV], &err);
      CHECK_ERR(err);
    }
    iBase_EntitySetHandle pvLocalRootSet;
    iMesh_getRootSet(new_instance, &pvLocalRootSet, &err);
    CHECK_ERR(err);

    cout << "   Attempting to create eleven edges...";
    iBase_EntityHandle aEdgeVerts[] = {aNewVerts[0], aNewVerts[1],
			  aNewVerts[1], aNewVerts[2],
			  aNewVerts[2], aNewVerts[3],
			  aNewVerts[3], aNewVerts[0],
			  aNewVerts[2], aNewVerts[4],
			  aNewVerts[4], aNewVerts[5],
			  aNewVerts[3], aNewVerts[5],
			  aNewVerts[1], aNewVerts[6],
			  aNewVerts[6], aNewVerts[7],
			  aNewVerts[7], aNewVerts[2],
			  aNewVerts[1], aNewVerts[7]};

    iBase_EntityHandle aNewEdges[11], aNewTris[2], aNewQuads[2];
    bool qEdgesOK = false, qTriOK = false, qQuadOK = false;

    int CStat;

    for (int iE = 0; iE < 11; iE++) {
      // This will throw an error if the implementation can't build
      // edges from verts.
      iMesh_createEnt(new_instance, iMesh_LINE_SEGMENT,
		      aEdgeVerts + 2*iE, 2, &aNewEdges[iE], &CStat, &err);
      CHECK_ERR(err);
      qOK = (err == iBase_SUCCESS && CStat == iBase_NEW);
      // Check that the edge has the right verts
      {
	iBase_EntityHandle *aVertsCheck = NULL;
	int iNV;
	int iaVertsCheckAlloc = 0;
	iMesh_getEntAdj(new_instance, aNewEdges[iE], iBase_VERTEX,
			&aVertsCheck, &iaVertsCheckAlloc, &iNV, &err);
	CHECK_ERR(err);
	qOK = qOK && ((iNV == 2) &&
		      ((aVertsCheck[0] == aEdgeVerts[2*iE] &&
			aVertsCheck[1] == aEdgeVerts[2*iE+1]) ||
		       (aVertsCheck[1] == aEdgeVerts[2*iE] &&
			aVertsCheck[0] == aEdgeVerts[2*iE+1])));
	UT_FREE(aVertsCheck);
      }
    }
    qEdgesOK = qOK;

    if (qEdgesOK) {
      // Now make sure that trying this again doesn't cause problems.
      for (int iE = 0; iE < 11; iE++) {
	iMesh_createEnt(new_instance, iMesh_LINE_SEGMENT,
			aEdgeVerts + 2*iE, 2, &aNewEdges[iE], &CStat, &err);
	qOK = qOK && ((CStat == iBase_ALREADY_EXISTED) ||
		      (CStat == iBase_CREATED_DUPLICATE));
	CHECK_ERR(err);
      }
      cout << endl;
      TEST(qOK);

      qOK = true;
    }

    if (qEdgesOK) {
      // Now try to create two triangles from edges.
      cout << "   Attempting to create two triangles from edges..." << endl;
      // These triangles are built from edges (1, 10, 9) and (7, 8, 10).
      iBase_EntityHandle aTriEdges[] = {aNewEdges[1], aNewEdges[10], aNewEdges[9],
			   aNewEdges[7], aNewEdges[8], aNewEdges[10]};
      
      int CStat;

      for (int iTri = 0; iTri < 2; iTri++) {
	iMesh_createEnt(new_instance, iMesh_TRIANGLE, aTriEdges + 3*iTri, 3,
			&aNewTris[iTri], &CStat, &err);
	CHECK_ERR(err);
	qOK = qOK && (CStat == iBase_NEW);

	// Check that the tri has the right edges
	{
	  iBase_EntityHandle *aEdgesCheck = NULL;
	  int iNE;
	  int iEdgesCheckAlloc = 0;
	  iMesh_getEntAdj(new_instance, aNewTris[iTri], iBase_EDGE,
			  &aEdgesCheck, &iEdgesCheckAlloc, &iNE, &err);
	  CHECK_ERR(err);
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
	  UT_FREE(aEdgesCheck);
	}
      }

      // Verify edge-to-tri connectivity.
      {
	// There's only one edge that has two adjacent triangles; the
	// others have a triangle and something undefined adjacent to
	// them.  So check just that one edge, which is edge 10.
	iBase_EntityHandle *aTri = NULL;
	int iNumTri;
	int iTriAlloc = 0;
	iMesh_getEntAdj(new_instance, aNewEdges[10], iBase_FACE,
			&aTri, &iTriAlloc, &iNumTri, &err);
	CHECK_ERR(err);
	qOK = qOK && (iNumTri == 2)
	  && ((aTri[0] == aNewTris[0] && aTri[1] == aNewTris[1]) ||
	      (aTri[0] == aNewTris[1] && aTri[1] == aNewTris[0]));
	UT_FREE(aTri);
      }
      qTriOK = qOK;

      if (qTriOK) {
	// Now make sure that trying this again doesn't cause problems.
	for (int iTri = 0; iTri < 2; iTri++) {
	  iBase_EntityHandle new_tri;
	  iMesh_createEnt(new_instance, iMesh_TRIANGLE, aTriEdges + 3*iTri, 3,
			  &new_tri, &CStat, &err);
	  CHECK_ERR(err);
	  qOK = qOK && ((CStat == iBase_ALREADY_EXISTED) ||
			CStat == iBase_CREATED_DUPLICATE);
	}
      }

      if (!qTriOK) {
	cout << "    no support for building triangles from edges" << endl;
      }

      TEST(qOK);
      qOK = true;

      // Now try to create two quadrilaterals from edges.
      cout << "   Attempting to create two quadrilaterals from edges..."
	   << endl;
      // These quadrilaterals are built from edges (0, 1, 2, 3) and
      // (2, 4, 5, 6).
      iBase_EntityHandle aQuadEdges[] =
	{aNewEdges[0], aNewEdges[1], aNewEdges[2], aNewEdges[3],
	 aNewEdges[2], aNewEdges[4], aNewEdges[5], aNewEdges[6]};

      for (int iQuad = 0; iQuad < 2; iQuad++) {
	iMesh_createEnt(new_instance, iMesh_QUADRILATERAL, aQuadEdges + 4*iQuad, 4,
			&aNewQuads[iQuad], &CStat, &err);
	CHECK_ERR(err);
	qOK = qOK && (CStat == iBase_NEW);

	// Check that the quad has the right edges
	{
	  iBase_EntityHandle *aEdgesCheck = NULL;
	  int iNE;
	  int iEdgesCheckAlloc = 0;
	  iMesh_getEntAdj(new_instance, aNewQuads[iQuad], iBase_EDGE,
			  &aEdgesCheck, &iEdgesCheckAlloc, &iNE, &err);
	  CHECK_ERR(err);
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
	  UT_FREE(aEdgesCheck);
	}
      }

      // Verify edge-to-quad connectivity.
      {
	// There's only one edge that has two adjacent quadrilaterals; the
	// others have a quad and something undefined adjacent to
	// them.  So check just that one edge, which is edge 2.
	iBase_EntityHandle *aQuad = NULL;
	int iNumQuad;
	int iQuadAlloc = 0;
	iMesh_getEntAdj(new_instance, aNewEdges[2], iBase_FACE,
			&aQuad, &iQuadAlloc, &iNumQuad, &err);
	CHECK_ERR(err);
	qOK = qOK && (iNumQuad == 2)
	  && ((aQuad[0] == aNewQuads[0] && aQuad[1] == aNewQuads[1]) ||
	      (aQuad[0] == aNewQuads[1] && aQuad[1] == aNewQuads[0]));
	UT_FREE(aQuad);
      }
      qQuadOK = qOK;

      // Now make sure that trying this again doesn't cause problems.
      if (qQuadOK) {
	// If tris were also built successfully, verify edge-tri/quad
	// connectivity.
	if (qTriOK) {
	  // There's only one edge that is between a tri and a quad:
	  // edge 1.
	  iBase_EntityHandle *aFace = NULL;
	  int iNumFace;
	  int iFaceAlloc = 0;
	  iMesh_getEntAdj(new_instance, aNewEdges[1], iBase_FACE,
			  &aFace, &iFaceAlloc, &iNumFace, &err);
	  CHECK_ERR(err);
	  qOK = qOK && (iNumFace == 2)
	    && ((aFace[0] == aNewQuads[0] && aFace[1] == aNewTris[0]) ||
		(aFace[0] == aNewTris[0] && aFace[1] == aNewQuads[0]));
	  UT_FREE(aFace);
	}

	for (int iQuad = 0; iQuad < 2; iQuad++) {
	  iBase_EntityHandle new_quad;
	  iMesh_createEnt(new_instance, iMesh_QUADRILATERAL,
			  aQuadEdges + 4*iQuad, 4, &new_quad, &CStat, &err);
	  CHECK_ERR(err);
	  qOK = qOK && ((CStat == iBase_ALREADY_EXISTED) ||
			(CStat == iBase_CREATED_DUPLICATE));
	}
      }
      if (!qQuadOK) {
	cout << "    no support for building quads from edges" << endl;
      }

      TEST(qOK);
      qOK = true;
    }

    // Now test entity deletion for all entities recorded.  Along the
    // way, check that entity counts and returned entities are done
    // correctly.

    // First, verify that low-dimension entities can't be deleted
    // without deleting the entities that depend on them.
    int iNVerts, iNEdges, iNTris, iNQuads;
    iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_POINT,
		       &iNVerts, &err);
    CHECK_ERR(err);
    iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_LINE_SEGMENT,
		       &iNEdges, &err);
    CHECK_ERR(err);
    iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_TRIANGLE,
		       &iNTris, &err);
    CHECK_ERR(err);
    iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_QUADRILATERAL,
		       &iNQuads, &err);
    CHECK_ERR(err);

    TESTEQ(iNVerts,8);
    TEST((iNEdges == 0 && !qEdgesOK) || iNEdges == 11);
    TEST((iNTris == 0 && !qTriOK) || iNTris == 2);
    TEST((iNQuads == 0 && !qQuadOK) || iNQuads == 2);

    iMesh_deleteEnt(new_instance, aNewVerts[0], &err);
    CHECK_ERR2(err, iBase_INVALID_ENTITY_HANDLE);
    // Should not succeed in deleting.

    {
      int iNewNVerts;
      iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_POINT,
			 &iNewNVerts, &err);
      CHECK_ERR(err);
      TESTEQ(iNewNVerts,iNVerts);

      if (qTriOK || qQuadOK) {
	// Try to remove the edge that is shared between by a tri and a
	// quad.  Can only get here if edges were created in the first
	// place.  This removal attempt should fail.
	iMesh_deleteEnt(new_instance, aNewEdges[1], &err);
	CHECK_ERR2(err, iBase_INVALID_ENTITY_HANDLE);
	int iNewNEdges;
	iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_LINE_SEGMENT,
			   &iNewNEdges, &err);
	CHECK_ERR(err);
	TESTEQ(iNewNEdges,iNEdges);
      }

      // Now delete one quad (if any exist) and verify that you only
      // get one quad for subsequent requests.
      if (qQuadOK) {
	iMesh_deleteEnt(new_instance, aNewQuads[1], &err);
	CHECK_ERR(err);
	iBase_EntityHandle *aQuads = NULL;
	int iNQuadAfter;
	int iQuadsAlloc = 0;
	iMesh_getEntities(new_instance, pvLocalRootSet, iBase_FACE,
			  iMesh_QUADRILATERAL,
			  &aQuads, &iQuadsAlloc, &iNQuadAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNQuadAfter,1);
	iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_QUADRILATERAL,
			   &iNQuadAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNQuadAfter,1);
	UT_FREE(aQuads);
      }

      // Repeat the previous test for triangles.
      if (qTriOK) {
	iMesh_deleteEnt(new_instance, aNewTris[1], &err);
	CHECK_ERR(err);
	iBase_EntityHandle *aTris = NULL;
	int iNTriAfter;
	int iTrisAlloc = 0;
	iMesh_getEntities(new_instance, pvLocalRootSet, iBase_FACE,
			   iMesh_TRIANGLE,
			  &aTris, &iTrisAlloc, &iNTriAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNTriAfter,1);
	iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_TRIANGLE,
			   &iNTriAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNTriAfter,1);
	UT_FREE(aTris);
      }

      // Now delete the rest of the faces.
      if (qQuadOK) iMesh_deleteEnt(new_instance, aNewQuads[0], &err);
      CHECK_ERR(err);
      if (qTriOK)  iMesh_deleteEnt(new_instance, aNewTris[0], &err);
      CHECK_ERR(err);
      int iNewNTris, iNewNQuads, iNewNFaces;
      iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_QUADRILATERAL,
			 &iNewNQuads, &err);
      CHECK_ERR(err);
      iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_TRIANGLE,
			 &iNewNTris, &err);
      CHECK_ERR(err);
      iMesh_getNumOfType(new_instance, pvLocalRootSet, iBase_FACE,
			 &iNewNFaces, &err);
      CHECK_ERR(err);
      TEST(iNewNQuads == 0 && iNewNTris == 0 && iNewNFaces == 0);

      // Repeat the single-deletion test for edges.
      {
	iMesh_deleteEnt(new_instance, aNewEdges[10], &err);
	CHECK_ERR(err);
	iBase_EntityHandle *aEdges = NULL;
	int iNEdgeAfter;
	int iaEdgesAlloc = 0;
	iMesh_getEntities(new_instance, pvLocalRootSet, iBase_EDGE,
			   iMesh_LINE_SEGMENT,
			  &aEdges, &iaEdgesAlloc, &iNEdgeAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNEdgeAfter,10);
	iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_LINE_SEGMENT,
			   &iNEdgeAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNEdgeAfter,10);
	iMesh_getNumOfType(new_instance, pvLocalRootSet, iBase_EDGE,
			   &iNEdgeAfter, &err);
	CHECK_ERR(err);
	TESTEQ(iNEdgeAfter,10);
	UT_FREE(aEdges);
      }

      // Delete the remaining edges.
      int i;
      for (i = 0; i < 10; i++) {
	iMesh_deleteEnt(new_instance, aNewEdges[i], &err);
	CHECK_ERR(err);
      }
      int iNewNSegs, iNewNEdges;
      iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_LINE_SEGMENT,
			 &iNewNSegs, &err);
      CHECK_ERR(err);
      iMesh_getNumOfType(new_instance, pvLocalRootSet, iBase_EDGE,
			 &iNewNEdges, &err);
      CHECK_ERR(err);
      TEST(iNewNSegs == 0 && iNewNEdges == 0);

      // Delete all the verts.
      for (i = 0; i < 8; i++) {
	iMesh_deleteEnt(new_instance, aNewVerts[i], &err);
	CHECK_ERR(err);
      }
      int iNewNPts;
      iMesh_getNumOfTopo(new_instance, pvLocalRootSet, iMesh_POINT,
			 &iNewNPts, &err);
      CHECK_ERR(err);
      iMesh_getNumOfType(new_instance, pvLocalRootSet, iBase_VERTEX,
			 &iNewNVerts, &err);
      CHECK_ERR(err);
    }
    // Done with creating stuff via edges.

    // Now test again, creating tris and quads directly from verts.

    // Create the verts again, because you just deleted them.
    for (int iV = 0; iV < 8; iV++) {
      iMesh_createVtx(new_instance, adAllCoords[2*iV], adAllCoords[2*iV+1], 0,
		      &aNewVerts[iV], &err);
      CHECK_ERR(err);
    }
    cout << "   Attempting to create two triangles from verts..." << endl;
    // First, try to create two triangles.
    iBase_EntityHandle aTriVerts[] = {aNewVerts[1], aNewVerts[6], aNewVerts[7],
			 aNewVerts[7], aNewVerts[2], aNewVerts[1]};

    qTriOK = true;

    for (int iTri = 0; iTri < 2; iTri++) {
      iMesh_createEnt(new_instance, iMesh_TRIANGLE, aTriVerts + 3*iTri, 3,
		      &aNewTris[iTri], &CStat, &err);
      CHECK_ERR(err);
      qOK = qOK && (CStat == iBase_NEW);

      // Check that the tri has the right verts
      {
	iBase_EntityHandle *aVertsCheck = NULL;
	int iNE;
	int iVertsCheckAlloc = 0;
	iMesh_getEntAdj(new_instance, aNewTris[iTri], iBase_VERTEX,
			&aVertsCheck, &iVertsCheckAlloc, &iNE, &err);
	CHECK_ERR(err);
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
	UT_FREE(aVertsCheck);
      }
    } // Done making and checking all tris.
    qTriOK = qOK;

    if (qTriOK) {
      // Now make sure that trying this again doesn't cause problems.
      for (int iTri = 0; iTri < 2; iTri++) {
	iBase_EntityHandle new_tri;
	iMesh_createEnt(new_instance, iMesh_TRIANGLE,
			aTriVerts + 3*iTri, 3, &new_tri, &CStat, &err);
	CHECK_ERR(err);
	qOK = qOK && ((CStat == iBase_ALREADY_EXISTED) ||
		      (CStat == iBase_CREATED_DUPLICATE));
      }
    }
    TEST(qOK);
    qOK = true;

    // Should have five edges.
    iMesh_getNumOfType(new_instance, pvLocalRootSet, iBase_EDGE,
		       &iNEdges, &err);
    CHECK_ERR(err);
    TESTEQ(5,iNEdges);

    if (!qTriOK) {
      cout << "    no support for building triangles from vertices" << endl;
    }

    // Done trying to create triangles from vertices.

    cout << "   Attempting to create two quadrilaterals from verts..." << endl;
    // First, try to create two quads.
    iBase_EntityHandle aQuadVerts[] =
      {aNewVerts[0], aNewVerts[1], aNewVerts[2], aNewVerts[3],
       aNewVerts[3], aNewVerts[2], aNewVerts[4], aNewVerts[5]};

    qQuadOK = true;

    for (int iQuad = 0; iQuad < 2; iQuad++) {
      iMesh_createEnt(new_instance, iMesh_QUADRILATERAL, aQuadVerts + 4*iQuad,
		      4, &aNewQuads[iQuad], &CStat, &err);
      CHECK_ERR(err);
      qOK = qOK && (CStat == iBase_NEW);

      // Check that the quad has the right verts
      {
	iBase_EntityHandle *aVertsCheck = NULL;
	int iNE;
	int iVertsCheckAlloc = 0;
	iMesh_getEntAdj(new_instance, aNewQuads[iQuad], iBase_VERTEX,
			&aVertsCheck, &iVertsCheckAlloc, &iNE, &err);
	CHECK_ERR(err);
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
	UT_FREE(aVertsCheck);
      }
    } // Done making and checking all quads.
    qQuadOK = qOK;

    if (qQuadOK) {
	// Now make sure that trying this again doesn't cause problems.
      for (int iQuad = 0; iQuad < 2; iQuad++) {
	iBase_EntityHandle new_quad;
	iMesh_createEnt(new_instance, iMesh_QUADRILATERAL,
			aQuadVerts + 4*iQuad, 4, &new_quad, &CStat, &err);
	CHECK_ERR(err);
	qOK = qOK && ((CStat == iBase_ALREADY_EXISTED) ||
		      (CStat == iBase_CREATED_DUPLICATE));
      }
    }
    TEST(qOK);
    qOK = true;

    if (qQuadOK) {
      // Should have seven edges, or 11 if the triangles also built okay.
      iMesh_getNumOfType(new_instance, pvLocalRootSet, iBase_EDGE,
			 &iNEdges, &err);
      CHECK_ERR(err);
      TEST( (qTriOK && 11 == iNEdges) || (!qTriOK && 7 == iNEdges) );
    }
    else {
      cout << "    no support for building quadrilaterals from vertices"
	   << endl;
    }
    // Done trying to create quadrilaterals from vertices.
    vDoSaveLoad(&new_instance, &pvLocalRootSet);

    // No need to re-test delete.
  } // Done with testing create/deleteEnt for 2D.
  else if (iGeomDim == 3) {
    iMesh_setGeometricDimension(new_instance, 3, &err);
    // Now repeat all of the above for the more complex case of testing
    // 3D mesh creation, in (at least most of) the permutations.
    qOK = true;
    cout << " Creating mini 3D mesh..." << endl;
    // First, create some verts.  
    //
    double adAllCoords[] = {0, 0, 0,
			    1, 0, 0,
			    1, 1, 0,
			    0, 1, 0,
			    0, 0, 0,
			    1, 0, 0,
			    1, 1, 0,
			    0, 1, 0};
    cout << "   Creating eight vertices..." << endl;

    iBase_EntityHandle aNewVerts[8];
    for (int iV = 0; iV < 8; iV++) {
      iMesh_createVtx(new_instance, adAllCoords[iGeomDim*iV],
		      adAllCoords[iGeomDim*iV+1], adAllCoords[iGeomDim*iV+2], 
		      &aNewVerts[iV], &err);
      CHECK_ERR(err);
    }
  }
  iMesh_dtor(new_instance, &err);
  CHECK_ERR(err);
}
