#define getArrayPointer(array_) (array_._get_ior()->d_firstElement)

TSTT_Swap::Swap3D_impl::eFaceCat TSTT_Swap::Swap3D_impl::eCategorizeFace
(opaque oFace, opaque aoVerts[6], opaque aoTets[4],
 opaque& oPivot0, opaque& oPivot1, opaque& oOther)
{
  // Make a general SIDL array of opaques, for calls to connectivity.
  // This should never need to be bigger than four entries.
  static sidl::array<opaque> saEnts = sidl::array<opaque>::create1d(4);
  static opaque *aoEntities = getArrayPointer(saEnts);
  int iSize;

  //@@ Case: one or both cells is not a tet, including boundaries.
  // Boundary triangles are lumped in with those that are between a
  // prism and a pyramid, for instance. 
  EntMesh.getEntAdj(oFace, TSTTM::EntityType_REGION, saEnts, iSize);
  assert(iSize <= 2);
  if (iSize < 2 ||
      EntMesh.getEntTopo(aoEntities[0]) != TSTTM::EntityTopology_TETRAHEDRON ||
      EntMesh.getEntTopo(aoEntities[1]) != TSTTM::EntityTopology_TETRAHEDRON)
    return (eOther);

  //@@ List all tets formed from five verts defining two tets sharing aoFace
  aoTets[0] = aoEntities[0];
  aoTets[1] = aoEntities[1];

  EntMesh.getEntAdj(oFace, TSTTM::EntityType_VERTEX, saEnts, iSize);
  assert(iSize == 3);
  aoVerts[0] = aoEntities[0];
  aoVerts[1] = aoEntities[1];
  aoVerts[2] = aoEntities[2];

  // The last two come from the two tets incident on the face.  Exactly
  // one vert for each tet isn't part of the original face. 
  aoVerts[3] = aoVerts[4] = oNullOpaque;
  EntMesh.getEntAdj(aoTets[0], TSTTM::EntityType_VERTEX, saEnts, iSize);
  assert(iSize == 4);
  for (int ii = 0; ii < 4; ii++) {
    if (aoEntities[ii] != aoVerts[0] &&
	aoEntities[ii] != aoVerts[1] &&
	aoEntities[ii] != aoVerts[2]) {
      aoVerts[3] = aoEntities[ii];
      break;
    }
  }
      
  EntMesh.getEntAdj(aoTets[1], TSTTM::EntityType_VERTEX, saEnts, iSize);
  assert(iSize == 4);
  for (int ii = 0; ii < 4; ii++) {
    assert(aoEntities[ii] != aoVerts[3]);
    if (aoEntities[ii] != aoVerts[0] &&
	aoEntities[ii] != aoVerts[1] &&
	aoEntities[ii] != aoVerts[2]) {
      aoVerts[4] = aoEntities[ii];
      break;
    }
  }

  opaque& oVertA = aoVerts[0];
  opaque& oVertB = aoVerts[1];
  opaque& oVertC = aoVerts[2];
  opaque& oVertD = aoVerts[3];
  opaque& oVertE = aoVerts[4];

  // Orientation check.  Basically, ABCE and ACBD need to both be
  // positively oriented, or we're in real trouble.  The most typical
  // trouble, of course, is that the tets are on opposite sides from
  // where I expected, in which case some switching is in order.
#ifndef NDEBUG
  int iOrientABCE = iOrient3D(oVertA, oVertB, oVertC, oVertE);
  assert(iOrientABCE == 1);
  int iOrientACBD = iOrient3D(oVertA, oVertC, oVertB, oVertD);
  assert(iOrientACBD == 1);
#endif

  // Any add'l cell must be a neighbor of both of the first two.  So
  // make a list of all the tets that share a face with the first two
  // tets.  Awkward, but not difficult.
  int iNumTets = 2;
  {
    opaque aoFaces[4];
    opaque aoAdjTets0[4], aoAdjTets1[4];

    EntMesh.getEntAdj(aoTets[0], TSTTM::EntityType_FACE,
		      saEnts, iSize);
    assert(iSize == 4);
    aoFaces[0] = aoEntities[0];
    aoFaces[1] = aoEntities[1];
    aoFaces[2] = aoEntities[2];
    aoFaces[3] = aoEntities[3];

    for (int ii = 0; ii < 4; ii++) {
      EntMesh.getEntAdj(aoFaces[ii], TSTTM::EntityType_REGION,
			saEnts, iSize);
      if (iSize == 1) aoAdjTets0[ii] = oNullOpaque;
      else {
	if (aoEntities[0] == aoTets[0]) {
	  aoAdjTets0[ii] = aoEntities[1];
	}
	else {
	  assert(aoEntities[1] == aoTets[0]);
	  aoAdjTets0[ii] = aoEntities[0];
	}
      }
    }

    EntMesh.getEntAdj(aoTets[1], TSTTM::EntityType_FACE,
		      saEnts, iSize);
    assert(iSize == 4);
    aoFaces[0] = aoEntities[0];
    aoFaces[1] = aoEntities[1];
    aoFaces[2] = aoEntities[2];
    aoFaces[3] = aoEntities[3];

    for (int ii = 0; ii < 4; ii++) {
      EntMesh.getEntAdj(aoFaces[ii], TSTTM::EntityType_REGION,
			saEnts, iSize);
      if (iSize == 1) aoAdjTets1[ii] = oNullOpaque;
      else {
	if (aoEntities[0] == aoTets[1]) {
	  aoAdjTets1[ii] = aoEntities[1];
	}
	else {
	  assert(aoEntities[1] == aoTets[1]);
	  aoAdjTets1[ii] = aoEntities[0];
	}
      }
    }

    // Could be zero, one, or two tets in common.
    for (int i0 = 0; i0 < 4; i0 ++) {
      opaque oTet0 = aoAdjTets0[i0];
      for (int i1 = 0; i1 < 4; i1 ++) {
	opaque oTet1 = aoAdjTets1[i1];
	if (oTet0 == oTet1 && oTet0 != oNullOpaque) {
	  aoTets[iNumTets++] = oTet0;
	}
      }
    }
    assert(iNumTets >= 2 && iNumTets <= 4);
#warning If tets aren't in entity set, don't do anything.
  }
	
  switch (iNumTets) {
  case 2:
    {
      //@@ Two tets
      // Orientation has already been checked.

      // The following code for eN32 face identification uses only as
      // many orientation primitives as required to determine the actual
      // face type.  This is very useful, as this is a critical code
      // path. 

      // Initialize these to ludicrous values.
      int iOrientA = 100, iOrientB = 100, iOrientC = 100;
      bool qApproxFlatA = false, qApproxFlatB = false, qApproxFlatC = false;
      bool qBadOrientation = false;

      // There is always the possibility that the configuration has one
      // or two correctly-oriented but nearly flat (within the tolerance
      // set by VolMesh::dMaxAngleForSurfSwap) faces.  So if the
      // orientation is positive, check whether it should perhaps be
      // tagged as zero instead.

      iOrientA = iGenOrient3D(oVertB, oVertC, oVertD, oVertE,
			      qApproxFlatA);
      if (iOrientA == -1) {
        oPivot0 = oVertB;
        oPivot1 = oVertC;
        oOther  = oVertA;
	qBadOrientation = true;
      }
      else {
	iOrientB = iGenOrient3D(oVertC, oVertA, oVertD, oVertE, qApproxFlatB);
	if (iOrientB == -1) {
	  oPivot0 = oVertC;
	  oPivot1 = oVertA;
	  oOther  = oVertB;
	  qBadOrientation = true;
	}
	if (iOrientA == 0 && iOrientB == 0) return eN20;
	if (!qBadOrientation) {
	  iOrientC = iGenOrient3D(oVertA, oVertB, oVertD, oVertE,
				  qApproxFlatC);
	  if (iOrientC == -1) {
	    oPivot0 = oVertA;
	    oPivot1 = oVertB;
	    oOther  = oVertC;
	    qBadOrientation = true;
	  }
	}
      }
      if (qBadOrientation) {
	// May eventually need to distinguish between genuine N32 cases
	// and cases where the bad orientation is caused by a reflex
	// edge on the boundary.   But not yet.
	return eN32;
      }	    

      switch (iOrientA + iOrientB + iOrientC) {
      case 0: // Impossible, but included for completeness.
	return eOther;
      case 1: // Two orientations are zero (three points co-linear).
	      // Hopelessly unswappable.  Bail out.
	return eN20;
      case 2: {
	//@@@ Four points are coplanar; T22, T44, N44
	// One orientation must be 0; verts are re-labeled to make it
	// iOrientC.  This implies that AB is the coplanar edge.	  
	assert(! (iOrientA && iOrientB && iOrientC));
	if (iOrientA == 0) {
	  opaque oTemp;
	  oTemp = oVertA;
	  oVertA = oVertB;
	  oVertB = oVertC;
	  oVertC = oTemp;
	}
	else if (iOrientB == 0) {
	  opaque oTemp;
	  oTemp = oVertA;
	  oVertA = oVertC;
	  oVertC = oVertB;
	  oVertB = oTemp;
	}
	else if (iOrientC == 0) {}
	// The following assertion has been removed deliberately, to
	// accomodate the near-flat cases tested for with dot products
	// above.  Prior to adding support for near-flat swapping, the
	// orientation was in fact re-aligned correctly, so there's no
	// cause for concern.
	// Verify that the relabeling was done correctly
	//	assert( iOrient3D(pVVertA, pVVertB, pVVertD, pVVertE) ==
	//	0 );
	

	// The configuration of these two tets is classified based on the
	// properties of the coplanar faces.
	// 1 If both are BFaces with the same boundary condition,
	//   swappable two tets to two.
	// 2 If both are BFaces with different bdry cond, not swappable.
	// 3 If one is a BFace and the other not, not swappable.
	// 4 If neither is a BFace, both opposite cells are tets, and the
	//   tets have the same fourth vert, swappable four to four.
	// 5 If neither is a BFace, both opposite cells are tets, and the
	//   tets do not have the same fourth vert, not swappable,
	//   although some non-local transformations might make it so.
	// 6 If neither is a BFace and one or both opposite cells is not a
	//   tet, not swappable.
	opaque oTetA = aoTets[0], oTetB = aoTets[1];

	// These are the faces that are coplanar
	opaque oFaceA = getFaceOpposite(oTetA, oVertC);
	opaque oFaceB = getFaceOpposite(oTetB, oVertC);
	opaque oOppCellA = getCellOpposite(oFaceA, oTetA);
	opaque oOppCellB = getCellOpposite(oFaceB, oTetB);

  	if (oOppCellA == oNullOpaque && oOppCellB == oNullOpaque) {
	  // Both faces are on the boundary.  These are already -known-
	  // to be co-planar, so checking whether they classify onto the
	  // same geometric face is sufficient.
	  opaque oClassA, oClassB;
	  ClassMesh.getClas(oFaceA, oClassA);
	  ClassMesh.getClas(oFaceB, oClassB);
	  if (oClassA == oClassB)
	    return eT22; // case 1
	  else
	    return eOther;  // case 2
	}
	else if (oOppCellA == oNullOpaque || oOppCellB == oNullOpaque ||
		 (EntMesh.getEntTopo(oOppCellA) !=
		  TSTTM::EntityTopology_TETRAHEDRON) ||
		 (EntMesh.getEntTopo(oOppCellB) !=
		  TSTTM::EntityTopology_TETRAHEDRON)) {
	  // Exactly one face on the boundary or internal faces with
	  // cells other than tets
	  return eOther; // cases 3,6
	}
	else {
	  // Both faces are internal to the mesh and separate pairs of tets
	  opaque oVertOppA = getVertOpposite(oOppCellA, oFaceA);
	  opaque oVertOppB = getVertOpposite(oOppCellB, oFaceB);
	  if (oVertOppA == oVertOppB) {
	    aoVerts[5] = oVertOppA;
	    aoTets[2] = oOppCellA;
	    aoTets[3] = oOppCellB;
	    return eT44; // case 4
	  }
	  else {
            oPivot0 = oVertA;
            oPivot1 = oVertB;
            oOther  = oVertC;
	    return eN44; // case 5
          }
	}
      } // End two-tet cases with four verts coplanar
      case 3:
	{
	  // This is probably a T23 case, but it might be a perturbed T22
	  // case, which should be tagged so that removal of surface verts
	  // will work better.  (Actually, it could even be a perturbed
	  // N20, which is a disaster during removal if you try to run
	  // with it as a T23! so nuke this case.)
	  if ((qApproxFlatA && qApproxFlatB) ||
	      (qApproxFlatB && qApproxFlatC) ||
	      (qApproxFlatC && qApproxFlatA))
	    return eN20;
	  if (qApproxFlatA || qApproxFlatB || qApproxFlatC) {
	    // It is a perturbed T22
	    if (qApproxFlatA) {
	      opaque oTemp;
	      oTemp = oVertA;
	      oVertA = oVertB;
	      oVertB = oVertC;
	      oVertC = oTemp;
	    }
	    else if (qApproxFlatB) {
	      opaque oTemp;
	      oTemp = oVertA;
	      oVertA = oVertC;
	      oVertC = oVertB;
	      oVertB = oTemp;
	    }
	    else if (qApproxFlatC) {}
	    
	    opaque oTetA = aoTets[0], oTetB = aoTets[1];
	    
	    // These are the faces that are coplanar
	    opaque oFaceA = getFaceOpposite(oTetA, oVertC);
	    opaque oFaceB = getFaceOpposite(oTetB, oVertC);
	    opaque oOppCellA = getCellOpposite(oFaceA, oTetA);
	    opaque oOppCellB = getCellOpposite(oFaceB, oTetB);
	    
	    if (oOppCellA == oNullOpaque && oOppCellB == oNullOpaque) {
	      // Both faces are on the boundary.  These are already -known-
	      // to be co-planar, so checking whether they classify onto the
	      // same geometric face is sufficient.
	      opaque oClassA, oClassB;
	      ClassMesh.getClas(oFaceA, oClassA);
	      ClassMesh.getClas(oFaceB, oClassB);
	      if (oClassA == oClassB)
		return eT22; // case 1
	      else
		return eOther;  // case 2
	    }
	  } // If one is nearly flat
	  // Otherwise, the configuration is convex; swappable two for three.
	  return eT23;
	}
      default:
	// No other cases should be possible
	assert(0);
	return eOther;
      }
    } // End of two tet cases
  case 3:
    {
      //@@ Three tet cases
      {
	// Shuffle things around, so that verts A, B, and C are on the
	// beltline of the three-tet configuration.

	opaque aoVertsTmp[] = {aoVerts[0], aoVerts[1], aoVerts[2],
			       aoVerts[3], aoVerts[4]};
	aoVerts[0] = aoVertsTmp[4];
	aoVerts[1] = aoVertsTmp[3];
	aoVerts[2] = aoVertsTmp[0];
	aoVerts[3] = aoVertsTmp[1];
	aoVerts[4] = aoVertsTmp[2];

	// Now a bit more shuffling so that aoTets[2] contains verts 0,
	// 1, 3, and 4.  Meanwhile, aoTets[0] will contain 1, 2, 3, 4,
	// and aoTets[1] will contain 0, 2, 3, 4.

	EntMesh.getEntAdj(aoTets[2], TSTTM::EntityType_VERTEX,
			  saEnts, iSize);
	assert(iSize == 4);
	if (aoEntities[0] != oVertC && aoEntities[1] != oVertC &&
	    aoEntities[2] != oVertC && aoEntities[3] != oVertC) {}
	else if (aoEntities[0] != oVertE && aoEntities[1] != oVertE &&
		 aoEntities[2] != oVertE && aoEntities[3] != oVertE) {
	  opaque oTemp = oVertC;
	  oVertC = oVertE;
	  oVertE = oVertD;
	  oVertD = oTemp;
	}
	else {
	  opaque oTemp = oVertC;
	  oVertC = oVertD;
	  oVertD = oVertE;
	  oVertE = oTemp;
	}
      }

    
      // The first of these returns will save a few orientation
      // primitive evaluations
      int iOrient1 = iOrient3D(oVertA, oVertC, oVertB, oVertD);
      if (iOrient1 == -1) return eOther;
      else if (iOrient1 == 0) return eN30;
      
      int iOrient2 = iOrient3D(oVertA, oVertB, oVertC, oVertE);
      if (iOrient2 == -1) return eOther;
      else if (iOrient2 == 0) return eN30;
      
      assert(iOrient1 == 1);
      assert(iOrient2 == 1);
      
      //@@@@ T32
      // These assertions will probably have to be reinstated for testing
      // later. 
      //     assert(! LpTCTets[0]->qHasVert(pVVertA) );
      //     assert(! LpTCTets[1]->qHasVert(pVVertB) );
      //     assert(! LpTCTets[2]->qHasVert(pVVertC) );
      return eT32;
    } // End of three tet cases
  case 4:
    //@@@ Four tets => N40
    return eN40; 
    // End of four tet cases
  } // End of classification/weeding
  // Should never reach here, but just in case...
  return eOther;
}

