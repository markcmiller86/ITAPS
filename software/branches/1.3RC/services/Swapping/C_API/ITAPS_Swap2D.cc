#include "ITAPS_Swap.hh"

using namespace ITAPS_Swap;
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

#define dDIST2D(a, b) sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]))
#define dDOT2D(a,b) (a[0]*b[0] + a[1]*b[1])
#define dMAG2D(a) sqrt(a[0]*a[0] + a[1]*a[1])
#define vNORMALIZE2D(a) do {double dInvMag = 1./dMAG2D(a); \
a[0]*=dInvMag; a[1]*=dInvMag;} while(0)

/**
 * Method:  swap[]
 */
int
ITAPS_Swap::Swap2D::swap (
  /* inout */ iBase_EntitySetHandle entity_set,
  /* inout */ iBase_EntityHandle entity_handle ) 
  throw (iBase_ErrorType)
{
  int err = iBase_SUCCESS;
  if (entity_set != shRootSet) {
    shSet = entity_set;
    qUsingSet = true;
  }
  int iRetVal = swap(entity_handle);
  qUsingSet = false;
  shSet = NULL;
  return iRetVal;
}

/**
 * Method:  swap[]
 */
int
ITAPS_Swap::Swap2D::swap (
  /* inout */ iBase_EntityHandle entity_handle ) 
  throw (iBase_ErrorType)
{
  int err = iBase_SUCCESS;
  static iBase_EntityHandle *entity_handles = NULL;
  // Can't declare this next one const, because its address has be
  // passable as an int*.
  static int entity_handles_allocated = 4;
  if (entity_handles == NULL) entity_handles =
    reinterpret_cast<iBase_EntityHandle*>(calloc(4, sizeof(void*)));

  int iNumFaces;
  iMesh_getEntAdj(pMeshInst, entity_handle, iBase_FACE,
		  &entity_handles, &entity_handles_allocated, &iNumFaces,
		  &err);
  CHECK_ERR(err, pMeshInst);

  // Can't do anything unless there are exactly two faces adjacent to
  // this edge.
  if (iNumFaces != 2) return 0;

  // Now identify them explicitly.
  iBase_EntityHandle ehCell1 = entity_handles[0];
  iBase_EntityHandle ehCell2 = entity_handles[1];
  
  // Make sure both faces are triangles.
  int iTopo1, iTopo2;
  iMesh_getEntTopo(pMeshInst, ehCell1, &iTopo1, &err);
  iMesh_getEntTopo(pMeshInst, ehCell2, &iTopo2, &err);
  CHECK_ERR(err, pMeshInst);

  if ((iTopo1 != iMesh_TRIANGLE) || (iTopo2 != iMesh_TRIANGLE)) return 0;

  // Make sure both faces have the same classification.
#ifdef CLASSIFY
#ifdef OLD_TSTTR
#warning Classification still uses old SIDL/Babel interface
  opaque oClass1, oClass2;
  Assoc.getEntEntAssociation(ModMesh, GModel, ehCell1, oClass1);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehCell2, oClass2);
#else
#warning Classification still uses my SIDL/Babel interface hack
  Assoc.getEntEntAssociation(ehCell1, oClass1);
  Assoc.getEntEntAssociation(ehCell2, oClass2);
#endif
  if (oClass1 != oClass2) return 0;
#endif
  if (qUsingSet && !(qIsEntContained(ehCell1) &&
		     qIsEntContained(ehCell2))) {
    return 0;
  }

  int iNV;
  iMesh_getEntAdj(pMeshInst, entity_handle, iBase_VERTEX,
		  &entity_handles, &entity_handles_allocated,
		  &iNV, &err);
  CHECK_ERR(err, pMeshInst);

  assert(iNV == 2);
  iBase_EntityHandle ehVertA = entity_handles[0];
  iBase_EntityHandle ehVertB = entity_handles[1];

  // Now get verts for the faces.  These are guaranteed to be in CW
  // order, although they may be circularly permuted.  Two will match
  // the edge, one won't.  
  iMesh_getEntAdj(pMeshInst, ehCell1, iBase_VERTEX, &entity_handles,
		  &entity_handles_allocated, &iNV, &err);
  CHECK_ERR(err, pMeshInst);

  assert(iNV == 3);
  iBase_EntityHandle ehVertC = NULL;
  {
    iBase_EntityHandle ehVert0 = entity_handles[0];
    iBase_EntityHandle ehVert1 = entity_handles[1];
    iBase_EntityHandle ehVert2 = entity_handles[2];

    if (ehVert0 != ehVertA && ehVert0 != ehVertB) {
      assert((ehVert1 == ehVertA && ehVert2 == ehVertB) ||
	     (ehVert2 == ehVertA && ehVert1 == ehVertB));
      ehVertC = ehVert0;
    }
    else if (ehVert1 != ehVertA && ehVert1 != ehVertB) {
      assert((ehVert0 == ehVertA && ehVert2 == ehVertB) ||
	     (ehVert2 == ehVertA && ehVert0 == ehVertB));
      ehVertC = ehVert1;
    }
    else {
      assert(ehVert2 != ehVertA && ehVert2 != ehVertB);
      assert((ehVert0 == ehVertA && ehVert1 == ehVertB) ||
	     (ehVert1 == ehVertA && ehVert0 == ehVertB));
      ehVertC = ehVert2;
    }
  }
  
  iMesh_getEntAdj(pMeshInst, ehCell2, iBase_VERTEX, &entity_handles,
		  &entity_handles_allocated, &iNV, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iNV == 3);
  iBase_EntityHandle ehVertD = NULL;
  {
    iBase_EntityHandle ehVert0 = entity_handles[0];
    iBase_EntityHandle ehVert1 = entity_handles[1];
    iBase_EntityHandle ehVert2 = entity_handles[2];

    if (ehVert0 != ehVertA && ehVert0 != ehVertB) {
      assert((ehVert1 == ehVertA && ehVert2 == ehVertB) ||
	     (ehVert2 == ehVertA && ehVert1 == ehVertB));
      ehVertD = ehVert0;
    }
    else if (ehVert1 != ehVertA && ehVert1 != ehVertB) {
      assert((ehVert0 == ehVertA && ehVert2 == ehVertB) ||
	     (ehVert2 == ehVertA && ehVert0 == ehVertB));
      ehVertD = ehVert1;
    }
    else {
      assert(ehVert2 != ehVertA && ehVert2 != ehVertB);
      assert((ehVert0 == ehVertA && ehVert1 == ehVertB) ||
	     (ehVert1 == ehVertA && ehVert0 == ehVertB));
      ehVertD = ehVert2;
    }
  }

  // Grab coords of all the vertices.
  const double *adCoordsA, *adCoordsB, *adCoordsC, *adCoordsD;
  {
    static double *adAllCoords = NULL;
    static int iAlloc = 12;
    if (adAllCoords == NULL) adAllCoords =
      (double*)(calloc(12, sizeof(double)));

    entity_handles[0] = ehVertA;
    entity_handles[1] = ehVertB;
    entity_handles[2] = ehVertC;
    entity_handles[3] = ehVertD;

    // This really should be an iBase_StorageOrder, but then it can't be
    // passed by address as an int*.
    int SO = iBase_INTERLEAVED;
    int iCSize;
    iMesh_getVtxArrCoords(pMeshInst, entity_handles, 4,
			  SO, &adAllCoords, &iAlloc, &iCSize, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iCSize == 12);

    adCoordsA = adAllCoords;
    adCoordsB = adAllCoords + 3;
    adCoordsC = adAllCoords + 6;
    adCoordsD = adAllCoords + 9;
  }
    
  int iCorrectOrient = iOrient2D(adCoordsA, adCoordsB, adCoordsC);
  assert(iOrient2D(adCoordsB, adCoordsA, adCoordsD) == iCorrectOrient);

  int iOrientA = iOrient2D (adCoordsA, adCoordsD, adCoordsC);
  int iOrientB = iOrient2D (adCoordsB, adCoordsC, adCoordsD);
  // Confirm correct orientation for the candidate triangles.
  if ((iOrientA != iCorrectOrient) ||
      (iOrientB != iCorrectOrient)) {
    return 0;
  }

  if (qDoSwap (adCoordsA, adCoordsB, adCoordsC, adCoordsD)) {
#define OLD_MANIP
#ifdef OLD_MANIP    
    // Create a new edge handle.
    iBase_EntityHandle ehNewEdge;
    // Should really be an iBase_CreationStatus, but then you can't pass
    // it by address as an int*, which the spec requires.
    int CStat;
    // These verts are put into the borrowed array for saEHVerts.
    entity_handles[0] = ehVertC;
    entity_handles[1] = ehVertD;
    iMesh_createEnt(pMeshInst, iMesh_LINE_SEGMENT, entity_handles, 2,
		    &ehNewEdge, &CStat, &err);
    CHECK_ERR(err, pMeshInst);
    assert(CStat == iBase_NEW);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, oNewEdge, oClass1);
#else
    Assoc.setEntEntAssociation(oNewEdge, oClass1);
#endif
#endif
    
    // Find the as-yet-unidentified edges of ehCell1 and 2.
    int iNE;
    iMesh_getEntAdj(pMeshInst, ehCell1, iBase_EDGE,
		    &entity_handles, &entity_handles_allocated, &iNE, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iNE == 3);

    // Need to get things ID'd: edge A1 is incident on vert 1 in cell1,
    // and so on.  The first if/else clause depends on canonical
    // ordering of edges; the second fixes things in case the order is
    // not canonical.
    iBase_EntityHandle ehEdgeA1, ehEdgeB1;
    if (entity_handles[0] == entity_handle) {
      ehEdgeA1 = entity_handles[1];
      ehEdgeB1 = entity_handles[2];
    }
    else if (entity_handles[1] == entity_handle) {
      ehEdgeA1 = entity_handles[2];
      ehEdgeB1 = entity_handles[0];
    }
    else {
      assert(entity_handles[2] == entity_handle);
      ehEdgeA1 = entity_handles[0];
      ehEdgeB1 = entity_handles[1];
    }
    iMesh_getEntAdj(pMeshInst, ehEdgeA1, iBase_VERTEX,
		    &entity_handles, &entity_handles_allocated, &iNV, &err);
    CHECK_ERR(err, pMeshInst);
    
    assert(iNV == 2);
    if (entity_handles[0] != ehVertA && entity_handles[1] != ehVertA) {
      iBase_EntityHandle ehTmp = ehEdgeA1;
      ehEdgeA1 = ehEdgeB1;
      ehEdgeB1 = ehTmp;
    }    

    // And repeat for the other cell.
    iMesh_getEntAdj(pMeshInst, ehCell2, iBase_EDGE,
		    &entity_handles, &entity_handles_allocated, &iNE, &err);
    CHECK_ERR(err, pMeshInst);

    assert(iNE == 3);

    iBase_EntityHandle ehEdgeA2, ehEdgeB2;
    if (entity_handles[0] == entity_handle) {
      ehEdgeA2 = entity_handles[1];
      ehEdgeB2 = entity_handles[2];
    }
    else if (entity_handles[1] == entity_handle) {
      ehEdgeA2 = entity_handles[2];
      ehEdgeB2 = entity_handles[0];
    }
    else {
      assert(entity_handles[2] == entity_handle);
      ehEdgeA2 = entity_handles[0];
      ehEdgeB2 = entity_handles[1];
    }
    iMesh_getEntAdj(pMeshInst, ehEdgeA2, iBase_VERTEX,
		    &entity_handles, &entity_handles_allocated, &iNV, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iNV == 2);
    if (entity_handles[0] != ehVertA && entity_handles[1] != ehVertA) {
      iBase_EntityHandle ehTmp = ehEdgeA2;
      ehEdgeA2 = ehEdgeB2;
      ehEdgeB2 = ehTmp;
    }    
    
    // Must delete entity_handle and ehCell1 and 2.  Order matters.
    iMesh_deleteEnt(pMeshInst, ehCell1, &err);
    CHECK_ERR(err, pMeshInst);
    iMesh_deleteEnt(pMeshInst, ehCell2, &err);
    CHECK_ERR(err, pMeshInst);
    iMesh_deleteEnt(pMeshInst, entity_handle, &err);
    CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.rmvEntEntAssociation(ModMesh, GModel, ehCell1, oClass1);
    Assoc.rmvEntEntAssociation(ModMesh, GModel, ehCell2, oClass1);
    Assoc.rmvEntEntAssociation(ModMesh, GModel, entity_handle, oClass1);
#else
    Assoc.rmvEntEntAssociation(ehCell1, oClass1);
    Assoc.rmvEntEntAssociation(ehCell2, oClass1);
    Assoc.rmvEntEntAssociation(entity_handle, oClass1);
#endif
#endif
    soRecursionQueue.erase(entity_handle);
    
    // Now create new cells using surviving edges of ehCell1 and 2.
    // That's one cell based on ehEdgeA1, ehEdgeA2, and ehNewEdge, and
    // another with the B edges.
    entity_handles[0] = ehEdgeA1;
    entity_handles[1] = ehEdgeA2;
    entity_handles[2] = ehNewEdge;
    iBase_EntityHandle ehNewCellA;
    iMesh_createEnt(pMeshInst, iMesh_TRIANGLE, entity_handles, 3, &ehNewCellA, &CStat, &err);
    CHECK_ERR(err, pMeshInst);
    assert(CStat == iBase_NEW);
    if (qUsingSet) vAddToSet(ehNewCellA);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, ehNewCellA, oClass1);
#else
    Assoc.setEntEntAssociation(ehNewCellA, oClass1);
#endif
#endif
    
    entity_handles[0] = ehEdgeB2;
    entity_handles[1] = ehEdgeB1;
    entity_handles[2] = ehNewEdge;
    iBase_EntityHandle ehNewCellB;
    iMesh_createEnt(pMeshInst, iMesh_TRIANGLE, entity_handles, 3, &ehNewCellB, &CStat, &err);
    CHECK_ERR(err, pMeshInst);
    assert(CStat == iBase_NEW);
    if (qUsingSet) vAddToSet(ehNewCellB);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, ehNewCellB, oClass1);
#else
    Assoc.setEntEntAssociation(ehNewCellB, oClass1);
#endif
#endif
#else
    // Delete two old tris and an old edge.
    // Must delete entity_handle and ehCell1 and 2.  Order matters.
    iMesh_deleteEnt(pMeshInst, ehCell1, &err);
    CHECK_ERR(err, pMeshInst);
    iMesh_deleteEnt(pMeshInst, ehCell2, &err);
    CHECK_ERR(err, pMeshInst);
    iMesh_deleteEnt(pMeshInst, entity_handle, &err);
    CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.rmvEntEntAssociation(ModMesh, GModel, ehCell1, oClass1);
    Assoc.rmvEntEntAssociation(ModMesh, GModel, ehCell2, oClass1);
    Assoc.rmvEntEntAssociation(ModMesh, GModel, entity_handle, oClass1);
#else
    Assoc.rmvEntEntAssociation(ehCell1, oClass1);
    Assoc.rmvEntEntAssociation(ehCell2, oClass1);
    Assoc.rmvEntEntAssociation(entity_handle, oClass1);
#endif
#endif
    soRecursionQueue.erase(entity_handle);

    // Add two new tris, built from verts.
    entity_handles[0] = ehVertA;
    entity_handles[1] = ehVertB;
    entity_handles[2] = ehVertD;
    iBase_EntityHandle ehNewCellA;
    iMesh_createEnt(pMeshInst, iMesh_TRIANGLE, entity_handles, 3, &ehNewCellA, &CStat, &err);
    CHECK_ERR(err, pMeshInst);
    assert(CStat == iBase_NEW);
    if (qUsingSet) vAddToSet(ehNewCellA);

    entity_handles[0] = ehVertB;
    entity_handles[1] = ehVertD;
    entity_handles[2] = ehVertC;
    iBase_EntityHandle ehNewCellB;
    iMesh_createEnt(pMeshInst, iMesh_TRIANGLE, entity_handles, 3, &ehNewCellB, &CStat, &err);
    CHECK_ERR(err, pMeshInst);
    assert(CStat == iBase_NEW);
    if (qUsingSet) vAddToSet(ehNewCellB);

#endif // OLD_MANIP
    // This is where one would, eventually, swap recursively.
    int iRetVal = 1;
    if (qAllowRecursion) {
      soRecursionQueue.insert(ehEdgeA1);
      soRecursionQueue.insert(ehEdgeA2);
      soRecursionQueue.insert(ehEdgeB1);
      soRecursionQueue.insert(ehEdgeB2);
    }
    return iRetVal;
  }
  else {
    return (0);
  }
}

/**
 * Method:  swapAll[]
 */
int
ITAPS_Swap::Swap2D::swapAll (iBase_EntitySetHandle entity_set,
			     iBase_TagHandle *pTag) 
throw ( 
  iBase_ErrorType
){
  int err = iBase_SUCCESS;
  bool qUsingTag = false;
  if (pTag != NULL) {
    qUsingTag = true;
  }
    
  if (entity_set != shRootSet) {
    shSet = entity_set;
    qUsingSet = true;
  }
  iBase_EntityIterator iter;
  iMesh_initEntIter(pMeshInst, entity_set, iBase_EDGE,
		    iMesh_LINE_SEGMENT, &iter, &err);
  CHECK_ERR(err, pMeshInst);

  int iTotal = 0, iPasses = 0, iSwaps = 1, iMaxPasses = 15;

  if (qAllowRecursion) iMaxPasses = 1;
  while (iSwaps && iPasses < iMaxPasses) {
    soRecursionQueue.clear();
    ++iPasses;
    if (iMaxPasses > 1)
      cout << "   Pass " << iPasses << ": ";
    iSwaps = 0;
    // Iterate over all edges.
    int qMoreData;
    iBase_EntityHandle entity_handle;
    iMesh_getNextEntIter(pMeshInst, iter, &entity_handle,
			 &qMoreData, &err);
    CHECK_ERR(err, pMeshInst); 
    while (qMoreData) {
      int tagData = 1;
      if (qUsingTag) {
	iMesh_getIntData(pMeshInst, entity_handle, *pTag, &tagData, &err);
	if (err != iBase_SUCCESS) tagData = 0;
      }
      if (tagData != 0) {
	iSwaps += swap(entity_handle);
	// Now clean out the recursion queue.
	if (qAllowRecursion) {
	  while (!soRecursionQueue.empty()) {
	    std::set<iBase_EntityHandle>::iterator iter = soRecursionQueue.begin();
	    entity_handle = *iter;
	    soRecursionQueue.erase(iter);
	    iSwaps += swap(entity_handle);
	  }
	  assert(soRecursionQueue.empty());
	}
      }
      iMesh_getNextEntIter(pMeshInst, iter, &entity_handle, &qMoreData, &err);
      CHECK_ERR(err, pMeshInst);
    }
    // Reset in case we need to go again.
    iMesh_resetEntIter(pMeshInst, iter, &err);
    CHECK_ERR(err, pMeshInst);

    iTotal += iSwaps;
    cout << iSwaps << " swaps";
    if (iMaxPasses > 1)
      cout << ", " << iTotal << " total";
    cout << endl;
  }
  // Don't need the iterator anymore.
  iMesh_endEntIter(pMeshInst, iter, &err);
  CHECK_ERR(err, pMeshInst);
  if (iMaxPasses > 1)
    cout << "Face swapping complete." << endl;

  qUsingSet = false;
  shSet = NULL;
  return iTotal;
}


//@ Compute the vector normal for a set of two verts
// This is always rotated 90 degrees counterclockwise from the vector
// running from vertex A to vertex B.
void ITAPS_Swap::Swap2D::vNormal
(const double adA[2], const double adB[2], double adRes[2])
{
  adRes[0] = adA[1] - adB[1];
  adRes[1] = adB[0] - adA[0];
}

int ITAPS_Swap::Swap2D::iOrient2D
(const double adA[2], const double adB[2], const double adC[2])
{
  double xa = adA[0];
  double ya = adA[1];
  
  double xb = adB[0];
  double yb = adB[1];
  
  double xc = adC[0];
  double yc = adC[1];
  
  // Currently no exact arithmetic
  double dDet = ((xb - xa) * (yc - ya) - (xc - xa) * (yb - ya));
  // Scale the determinant by the mean of the magnitudes of vectors:
  double dScale = (dDIST2D (adA, adB) + dDIST2D (adB, adC)
		   + dDIST2D (adC, adA)) / 3;
  dDet /= (dScale * dScale);
  if (dDet > 1.e-10)
    return 1;
  else if (dDet < -1.e-10)
    return -1;
  else
    return 0;
}

//@ Incircle test
int ITAPS_Swap::Swap2D::iIncircle
(const double adA[2], const double adB[2],
 const double adC[2], const double adD[2])
  // Determines whether VertD is inside or outside the circumcircle
  // of the tri formed by VertA-C, using as nearly exact arithmetic
  // as required.  Returns 1 if VertD is inside the
  // circumcircle of the tri, -1 if outside, 0 if on. 
{
  double xa = adA[0];
  double ya = adA[1];
  double wa2 = xa * xa + ya * ya;
  
  double xb = adB[0];
  double yb = adB[1];
  double wb2 = xb * xb + yb * yb;
  
  double xc = adC[0];
  double yc = adC[1];
  double wc2 = xc * xc + yc * yc;
  
  double xd = adD[0];
  double yd = adD[1];
  double wd2 = xd * xd + yd * yd;
  
  // Currently no exact arithmetic
  double dDet1 = ((xb - xa) * ((yc - ya) * (wd2 - wa2) - (yd - ya) * (wc2 - wa2))
		  + (xc - xa) * ((yd - ya) * (wb2 - wa2) - (yb - ya) * (wd2 - wa2))
		  + (xd - xa) * ((yb - ya) * (wc2 - wa2) - (yc - ya) * (wb2 - wa2))
		  );
  int iOrient = iOrient2D(adA, adB, adC);
  double dProd = dDet1 * iOrient;
  // Scale by the mean of the magnitudes of vectors:
  double dScale = (dDIST2D (adB, adA) +
		   dDIST2D (adC, adA) +
		   dDIST2D (adB, adC)) / 3;
  // Unclear exactly how to scale this...
  dProd /= (dScale * dScale * dScale * dScale);
  if (dProd > 1.e-10)
    return -1;
  else if (dProd < -1.e-10)
    return 1;
  else
    return 0;
}

// This tie breaker is used only to pick a unique configuration in the
// event that a tie occurs trying to decide  how to swap.  The only
// thing which this choice of tiebreaker has to recommend it is that it
// gives a unique configuration for any four verts ABCD.
bool ITAPS_Swap::Swap2D::qTieBreak
(const double adCoordsA[2], const double adCoordsB[2],
 const double adCoordsC[2], const double adCoordsD[2])
{
  static const double dEps = 1.e-12;
  // Choose the diagonal whose midpoint has the lexicographically
  // smallest location.  In case of ties, don't swap.
  double dXAC = adCoordsA[0] + adCoordsC[0];
  double dXBD = adCoordsB[0] + adCoordsD[0];

  if (dXAC > dXBD + dEps) {
    // Keep diagonal AC
    return false;
  }
  else if (dXBD > dXAC + dEps) {
    // Switch to diagonal BD
    return true;
  }
  else {
    double dYAC = adCoordsA[1] + adCoordsC[1];
    double dYBD = adCoordsB[1] + adCoordsD[1];
    
    if (dYBD > dYAC + dEps) {
      // Switch to diagonal BD
      return true;
    }
    else {
      return false;
    }
  }
}

//@@ Determines whether swapping is needed for local Delaunay-ness.
bool ITAPS_Swap::Swap2D::qDoSwapDelaunay
(const double adCoordsA[2], const double adCoordsB[2],
 const double adCoordsC[2], const double adCoordsD[2])
{
  switch (iIncircle (adCoordsA, adCoordsB, adCoordsC, adCoordsD)) {
  case (-1):
    return false;		// VertD is outside

  case 1:
    return true;		// VertD is inside

  case 0:			// Need some sort of tie-breaker for uniqueness
    return qTieBreak(adCoordsA, adCoordsB, adCoordsC, adCoordsD);

  default:
    assert (0);
    return false;
  }
}

//@@ Determines whether swapping will improve the maximum face angle.
bool ITAPS_Swap::Swap2D::qDoSwapMaxDihed
(const double adCoordsA[2], const double adCoordsB[2],
 const double adCoordsC[2], const double adCoordsD[2])
{
  // This algorithm finds face angles between adjacent faces by dotting
  // their unit normals.  The largest magnitude loses.
  //
  // To prevent pairs from flopping back and forth, the tie-breaker is
  // invoked if the inequality is small.

  double dEps = 1.e-12;
  // All of these normals are intended to point inwards.
  double adNormDA[2], adNormBD[2], adNormCB[2], adNormAC[2];
  vNormal (adCoordsA, adCoordsD, adNormDA);
  vNormal (adCoordsD, adCoordsB, adNormBD);
  vNormal (adCoordsB, adCoordsC, adNormCB);
  vNormal (adCoordsC, adCoordsA, adNormAC);

  vNORMALIZE2D (adNormDA);
  vNORMALIZE2D (adNormBD);
  vNORMALIZE2D (adNormCB);
  vNORMALIZE2D (adNormAC);

  double dDotBDA = dDOT2D (adNormBD, adNormDA);
  double dDotACB = dDOT2D (adNormAC, adNormCB);
  double dDotDAC = dDOT2D (adNormDA, adNormAC);
  double dDotCBD = dDOT2D (adNormCB, adNormBD);

  double dMaxThis  = std::max (dDotBDA, dDotACB);
  double dMaxOther = std::max (dDotDAC, dDotCBD);

  if (dMaxThis > dMaxOther + dEps)
    return true;
  else if (dMaxThis + dEps < dMaxOther)
    return false;
  else
    return qTieBreak (adCoordsA, adCoordsB, adCoordsC, adCoordsD);
}

static double dQual(ITAPS_Swap::QM_Base* pQM,
		    const double adLocA[2], const double adLocB[2],
		    const double adLocC[2])
{
  static double adAllCoords[6];

  adAllCoords[0] = adLocA[0];
  adAllCoords[1] = adLocA[1];
  adAllCoords[2] = adLocB[0];
  adAllCoords[3] = adLocB[1];
  adAllCoords[4] = adLocC[0];
  adAllCoords[5] = adLocC[1];

  return (pQM->calcQuality(adAllCoords, 6));
}

bool ITAPS_Swap::Swap2D::qDoSwapExternal
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3])
{
  double dEps = 1.e-10;
  int iSign = (pQM_int->shouldMinimize()) ? -1 : 1;

  // Old configuration
  double dQualABC = iSign*dQual(pQM_int, adLocA, adLocB, adLocC);
  double dQualBAD = iSign*dQual(pQM_int, adLocB, adLocA, adLocD);

  // New configuration
  double dQualCDB = iSign*dQual(pQM_int, adLocC, adLocD, adLocB);
  double dQualDCA = iSign*dQual(pQM_int, adLocD, adLocC, adLocA);

  return (std::min(dQualDCA, dQualCDB) > dEps + std::min(dQualABC, dQualBAD));
}

bool ITAPS_Swap::Swap2D::qDoSwap
(const double adCoordsA[2], const double adCoordsB[2],
 const double adCoordsL[2], const double adCoordsR[2])
{
  switch (ST_int) {
  case SwapType_MAX_MIN_SINE:
  case SwapType_DELAUNAY:
    return qDoSwapDelaunay (adCoordsA, adCoordsB, adCoordsL, adCoordsR);
  case SwapType_MIN_MAX_ANGLE:
    return qDoSwapMaxDihed (adCoordsA, adCoordsB, adCoordsL, adCoordsR);
  case SwapType_MESQUITE:
  case SwapType_USER_DEFINED:
    // Once I figure out how to register a user callback, I'll rewrite
    // this.
    return qDoSwapExternal (adCoordsA, adCoordsB, adCoordsL, adCoordsR);
  default:
    return (false);
  }
}
