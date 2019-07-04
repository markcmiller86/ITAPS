// 
// File:          TSTT_Swap_Swap2D_Impl.cc
// Symbol:        TSTT_Swap.Swap2D-v0.7
// Symbol Type:   class
// Babel Version: 0.10.10
// Description:   Server-side implementation for TSTT_Swap.Swap2D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.10
// 
#include "TSTT_Swap_Swap2D_Impl.hh"

// DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._includes)
using namespace TSTT_Swap;
#include <math.h>
#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define dDIST2D(a, b) sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]))
#define dDOT2D(a,b) (a[0]*b[0] + a[1]*b[1])
#define dMAG2D(a) sqrt(a[0]*a[0] + a[1]*a[1])
#define vNORMALIZE2D(a) do {double dInvMag = 1./dMAG2D(a); \
a[0]*=dInvMag; a[1]*=dInvMag;} while(0)

#define getArrayPointer(array_) (array_._get_ior()->d_firstElement)

#define my_throw(a, b) do { \
    TSTTB::Error TErr; \
    TErr.set((TSTTB::ErrorType_ ## a), (b)); \
    throw(TErr); \
} while(0)

typedef void* opaque;
// DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._includes)

// user-defined constructor.
void TSTT_Swap::Swap2D_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._ctor)
  ST_int = SwapType_DELAUNAY;
  qAllowRecursion = true;
  dMaxAngle = 0;

  sidl::BaseInterface BI;
  QM_int = BI;
  assert(QM_int._is_nil());
#ifdef NO_BABEL
#else
  EntMesh = BI;
  ModMesh = BI;
  Assoc = BI;
#ifdef OLD_TSTTR
  GModel = BI;
  assert(GModel._is_nil());
#endif
  assert(EntMesh._is_nil());
  assert(ModMesh._is_nil());
#endif
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._ctor)
}

// user-defined destructor.
void TSTT_Swap::Swap2D_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._dtor)
  // Nothing needed here.
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._dtor)
}

// static class initializer.
void TSTT_Swap::Swap2D_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._load)
  // Insert-Code-Here {TSTT_Swap.Swap2D._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._load)
}

// user-defined static methods: (none)

// user-defined non-static methods:
/**
 * Method:  setSwapType[]
 */
void
TSTT_Swap::Swap2D_impl::setSwapType (
  /* in */ ::TSTT_Swap::SwapType ST ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.setSwapType)
  ST_int = ST;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.setSwapType)
}

/**
 * Method:  setUserQualMeasure[]
 */
void
TSTT_Swap::Swap2D_impl::setUserQualMeasure (
  /* in */ ::TSTT_Swap::QualMeasure QM ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.setUserQualMeasure)
  QM_int = QM;
  setSwapType(SwapType_USER_DEFINED);
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.setUserQualMeasure)
}

/**
 * Method:  getSwapType[]
 */
::TSTT_Swap::SwapType
TSTT_Swap::Swap2D_impl::getSwapType ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.getSwapType)
  return ST_int;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.getSwapType)
}

/**
 * Method:  setSwapRecursion[]
 */
void
TSTT_Swap::Swap2D_impl::setSwapRecursion (
  /* in */ bool allow_recursion ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.setSwapRecursion)
  qAllowRecursion = allow_recursion;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.setSwapRecursion)
}

/**
 * Method:  isRecursionAllowed[]
 */
bool
TSTT_Swap::Swap2D_impl::isRecursionAllowed ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.isRecursionAllowed)
  return qAllowRecursion;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.isRecursionAllowed)
}

/**
 * Method:  setMeshData[]
 */
void
TSTT_Swap::Swap2D_impl::setMeshData (
  /* in */ ::TSTTM::Modify Mod_Mesh ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.setMeshData)
#ifndef NO_BABEL
  // Won't compile if Mod isn't a TSTTM::Modify object, so this check
  // should never fail.
  ModMesh = Mod_Mesh;
  if (ModMesh._is_nil())
    my_throw(NOT_SUPPORTED,
	     "Support for Modify interface is mandatory");
  
  EntMesh = Mod_Mesh;
  if (EntMesh._is_nil())
    my_throw(NOT_SUPPORTED,
	     "Support for Entity interface is mandatory");
#endif
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.setMeshData)
}

/**
 * Method:  setModelData[]
 */
void
TSTT_Swap::Swap2D_impl::setModelData (
  /* in */ ::TSTTG::Model Geom_Model ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.setModelData)
#ifdef OLD_TSTTR
  GModel = Geom_Model;
  if (GModel._is_nil())
    my_throw(INVALID_ARGUMENT, "Bad model data");
#endif
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.setModelData)
}

/**
 * Method:  setAssocData[]
 */
void
TSTT_Swap::Swap2D_impl::setAssocData (
  /* in */ ::TSTTR::Associate Assoc_Data ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.setAssocData)
  Assoc = Assoc_Data;
  if (Assoc._is_nil())
    my_throw(INVALID_ARGUMENT, "Bad association data");
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.setAssocData)
}

/**
 * Method:  swap[]
 */
int32_t
TSTT_Swap::Swap2D_impl::swap (
  /* inout */ void*& entity_set,
  /* inout */ void*& entity_handle ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.swap)

  opaque oCell1, oCell2;
  static sidl::array<opaque> saoEnt = sidl::array<opaque>::create1d(4);
  static opaque *aoEnt = getArrayPointer(saoEnt);

  int iNumFaces;
  EntMesh.getEntAdj(entity_handle, TSTTM::EntityType_FACE,
		    saoEnt, iNumFaces);

  // Can't do anything unless there are exactly two faces adjacent to
  // this edge.
  if (iNumFaces != 2) return 0;

  // Now identify them explicitly.
  oCell1 = aoEnt[0];
  oCell2 = aoEnt[1];
  
  // Make sure both faces are triangles.
  if ((EntMesh.getEntTopo(oCell1) != TSTTM::EntityTopology_TRIANGLE) ||
      (EntMesh.getEntTopo(oCell2) != TSTTM::EntityTopology_TRIANGLE))
    return 0;

  // Make sure both faces have the same classification.
  opaque oClass1, oClass2;
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oCell1, oClass1);
  Assoc.getEntEntAssociation(ModMesh, GModel, oCell2, oClass2);
#else
  Assoc.getEntEntAssociation(oCell1, oClass1);
  Assoc.getEntEntAssociation(oCell2, oClass2);
#endif
#endif

  if (oClass1 != oClass2) return 0;

  int iNV;
  EntMesh.getEntAdj(entity_handle, TSTTM::EntityType_VERTEX,
		    saoEnt, iNV);
  
  assert(iNV == 2);
  opaque oVertA = aoEnt[0];
  opaque oVertB = aoEnt[1];

  // Now get verts for the faces.  These are guaranteed to be in CW
  // order, although they may be circularly permuted.  Two will match
  // the edge, one won't.  
  EntMesh.getEntAdj(oCell1, TSTTM::EntityType_VERTEX,
		    saoEnt, iNV);

  assert(iNV == 3);
  opaque oVertC = NULL;
  {
    opaque oVert0 = aoEnt[0];
    opaque oVert1 = aoEnt[1];
    opaque oVert2 = aoEnt[2];

    if (oVert0 != oVertA && oVert0 != oVertB) {
      assert((oVert1 == oVertA && oVert2 == oVertB) ||
	     (oVert2 == oVertA && oVert1 == oVertB));
      oVertC = oVert0;
    }
    else if (oVert1 != oVertA && oVert1 != oVertB) {
      assert((oVert0 == oVertA && oVert2 == oVertB) ||
	     (oVert2 == oVertA && oVert0 == oVertB));
      oVertC = oVert1;
    }
    else {
      assert(oVert2 != oVertA && oVert2 != oVertB);
      assert((oVert0 == oVertA && oVert1 == oVertB) ||
	     (oVert1 == oVertA && oVert0 == oVertB));
      oVertC = oVert2;
    }
  }
  
  EntMesh.getEntAdj(oCell2, TSTTM::EntityType_VERTEX,
		    saoEnt, iNV);
  
  assert(iNV == 3);
  opaque oVertD = NULL;
  {
    opaque oVert0 = aoEnt[0];
    opaque oVert1 = aoEnt[1];
    opaque oVert2 = aoEnt[2];

    if (oVert0 != oVertA && oVert0 != oVertB) {
      assert((oVert1 == oVertA && oVert2 == oVertB) ||
	     (oVert2 == oVertA && oVert1 == oVertB));
      oVertD = oVert0;
    }
    else if (oVert1 != oVertA && oVert1 != oVertB) {
      assert((oVert0 == oVertA && oVert2 == oVertB) ||
	     (oVert2 == oVertA && oVert0 == oVertB));
      oVertD = oVert1;
    }
    else {
      assert(oVert2 != oVertA && oVert2 != oVertB);
      assert((oVert0 == oVertA && oVert1 == oVertB) ||
	     (oVert1 == oVertA && oVert0 == oVertB));
      oVertD = oVert2;
    }
  }

  // Grab coords of all the vertices.
  double adCoordsA[2], adCoordsB[2], adCoordsC[2], adCoordsD[2];
  {
    static sidl::array<double> sadCoords = sidl::array<double>::create1d(8);
    static double *adAllCoords = getArrayPointer(sadCoords);

    aoEnt[0] = oVertA;
    aoEnt[1] = oVertB;
    aoEnt[2] = oVertC;
    aoEnt[3] = oVertD;

    TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
    int iCSize;
    EntMesh.getVtxArrCoords(saoEnt, 4, SO, sadCoords, iCSize);
    assert(iCSize == 8);

    adCoordsA[0] = adAllCoords[0];
    adCoordsA[1] = adAllCoords[1];
    adCoordsB[0] = adAllCoords[2];
    adCoordsB[1] = adAllCoords[3];
    adCoordsC[0] = adAllCoords[4];
    adCoordsC[1] = adAllCoords[5];
    adCoordsD[0] = adAllCoords[6];
    adCoordsD[1] = adAllCoords[7];
  }
    
  
  int iCorrectOrient = iOrient2D(adCoordsA, adCoordsB, adCoordsC);
  assert(iOrient2D(adCoordsB, adCoordsA, adCoordsD) == iCorrectOrient);

  int iOrientA = iOrient2D (adCoordsA, adCoordsD, adCoordsC);
  if (iOrientA != iCorrectOrient) return 0;
  int iOrientB = iOrient2D (adCoordsB, adCoordsC, adCoordsD);
  // Confirm correct orientation for the candidate triangles.
  if (iOrientB != iCorrectOrient) return 0;

  if (qDoSwap (adCoordsA, adCoordsB, adCoordsC, adCoordsD)) {
    // Create a new edge handle.
    opaque oNewEdge;
    TSTTM::CreationStatus CStat;
    // These verts are put into the borrowed array for saEHVerts.
    aoEnt[0] = oVertC;
    aoEnt[1] = oVertD;
    ModMesh.createEnt(TSTTM::EntityTopology_LINE_SEGMENT,
		      saoEnt, 2, oNewEdge, CStat);
    assert(CStat == TSTTM::CreationStatus_NEW);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, oNewEdge, oClass1);
#else
    Assoc.setEntEntAssociation(oNewEdge, oClass1);
#endif
#endif
    
    // Find the as-yet-unidentified edges of oCell1 and 2.
    int iNE;
    EntMesh.getEntAdj(oCell1, TSTTM::EntityType_EDGE, saoEnt, iNE);
    assert(iNE == 3);

    // Need to get things ID'd: edge A1 is incident on vert 1 in cell1,
    // and so on.  The first if/else clause depends on canonical
    // ordering of edges; the second fixes things in case the order is
    // not canonical.
    opaque oEdgeA1, oEdgeB1;
    if (aoEnt[0] == entity_handle) {
      oEdgeA1 = aoEnt[1];
      oEdgeB1 = aoEnt[2];
    }
    else if (aoEnt[1] == entity_handle) {
      oEdgeA1 = aoEnt[2];
      oEdgeB1 = aoEnt[0];
    }
    else {
      assert(aoEnt[2] == entity_handle);
      oEdgeA1 = aoEnt[0];
      oEdgeB1 = aoEnt[1];
    }
    EntMesh.getEntAdj(oEdgeA1, TSTTM::EntityType_VERTEX,
		      saoEnt, iNV);
    
    assert(iNV == 2);
    if (aoEnt[0] != oVertA && aoEnt[1] != oVertA) {
      opaque pvTmp = oEdgeA1;
      oEdgeA1 = oEdgeB1;
      oEdgeB1 = pvTmp;
    }    

    EntMesh.getEntAdj(oCell2, TSTTM::EntityType_EDGE, saoEnt, iNE);
    assert(iNE == 3);

    // And repeat for the other cell.
    opaque oEdgeA2, oEdgeB2;
    if (aoEnt[0] == entity_handle) {
      oEdgeA2 = aoEnt[1];
      oEdgeB2 = aoEnt[2];
    }
    else if (aoEnt[1] == entity_handle) {
      oEdgeA2 = aoEnt[2];
      oEdgeB2 = aoEnt[0];
    }
    else {
      assert(aoEnt[2] == entity_handle);
      oEdgeA2 = aoEnt[0];
      oEdgeB2 = aoEnt[1];
    }
    EntMesh.getEntAdj(oEdgeA2, TSTTM::EntityType_VERTEX,
		      saoEnt, iNV);
    assert(iNV == 2);
    if (aoEnt[0] != oVertA && aoEnt[1] != oVertA) {
      opaque pvTmp = oEdgeA2;
      oEdgeA2 = oEdgeB2;
      oEdgeB2 = pvTmp;
    }    
    
    // Must delete entity_handle and oCell1 and 2.  Order matters.
    ModMesh.deleteEnt(oCell1);
    ModMesh.deleteEnt(oCell2);
    ModMesh.deleteEnt(entity_handle);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.rmvEntEntAssociation(ModMesh, GModel, oCell1, oClass1);
    Assoc.rmvEntEntAssociation(ModMesh, GModel, oCell2, oClass1);
    Assoc.rmvEntEntAssociation(ModMesh, GModel, entity_handle, oClass1);
#else
    Assoc.rmvEntEntAssociation(oCell1, oClass1);
    Assoc.rmvEntEntAssociation(oCell2, oClass1);
    Assoc.rmvEntEntAssociation(entity_handle, oClass1);
#endif
#endif
    soRecursionQueue.erase(entity_handle);
    
    // Now create new cells using surviving edges of oCell1 and 2.
    // That's one cell based on oEdgeA1, oEdgeA2, and oNewEdge, and
    // another with the B edges.
    aoEnt[0] = oEdgeA1;
    aoEnt[1] = oEdgeA2;
    aoEnt[2] = oNewEdge;
    opaque oNewCellA;
    ModMesh.createEnt(TSTTM::EntityTopology_TRIANGLE,
		      saoEnt, 3, oNewCellA, CStat);
    assert(CStat == TSTTM::CreationStatus_NEW);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, oNewCellA, oClass1);
#else
    Assoc.setEntEntAssociation(oNewCellA, oClass1);
#endif
#endif
    
    aoEnt[0] = oEdgeB2;
    aoEnt[1] = oEdgeB1;
    aoEnt[2] = oNewEdge;
    opaque oNewCellB;
    ModMesh.createEnt(TSTTM::EntityTopology_TRIANGLE,
		      saoEnt, 3, oNewCellB, CStat);
    assert(CStat == TSTTM::CreationStatus_NEW);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, oNewCellB, oClass1);
#else
    Assoc.setEntEntAssociation(oNewCellB, oClass1);
#endif
#endif
    
    // This is where one would, eventually, swap recursively.
    int iRetVal = 1;
    if (qAllowRecursion) {
      soRecursionQueue.insert(oEdgeA1);
      soRecursionQueue.insert(oEdgeA2);
      soRecursionQueue.insert(oEdgeB1);
      soRecursionQueue.insert(oEdgeB2);
    }
    return iRetVal;
  }
  else
    return (0);

  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.swap)
}

/**
 * Method:  swapAll[]
 */
int32_t
TSTT_Swap::Swap2D_impl::swapAll (
  /* inout */ void*& entity_set ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D.swapAll)

  void * iter;
  EntMesh.initEntIter(entity_set, TSTTM::EntityType_EDGE,
		      TSTTM::EntityTopology_LINE_SEGMENT, iter);

  int iTotal = 0, iPasses = 0, iSwaps = 1, iMaxPasses = 15;

  if (qAllowRecursion) iMaxPasses = 1;
  while (iSwaps && iPasses < iMaxPasses) {
    soRecursionQueue.clear();
    ++iPasses;
    if (iMaxPasses > 1)
      cout << "   Pass " << iPasses << ": ";
    iSwaps = 0;
    // Iterate over all edges.
    bool qMoreData;
    void *entity_handle;
    qMoreData = EntMesh.getNextEntIter(iter, entity_handle);
    while (qMoreData) {
      soRecursionQueue.insert(entity_handle);
      qMoreData = EntMesh.getNextEntIter(iter, entity_handle);
    }
    while (!soRecursionQueue.empty()) {
      std::set<opaque>::iterator iter = soRecursionQueue.begin();
      entity_handle = *iter;
      soRecursionQueue.erase(iter);
      iSwaps += swap(entity_set, entity_handle);
    }
//     while (qMoreData) {
//       iSwaps += swap(entity_set, entity_handle);
//       // Now clean out the recursion queue.
//       if (qAllowRecursion) {
// 	while (!soRecursionQueue.empty()) {
// 	  std::set<opaque>::iterator iter = soRecursionQueue.begin();
// 	  entity_handle = *iter;
// 	  soRecursionQueue.erase(iter);
// 	  iSwaps += swap(entity_set, entity_handle);
// 	}
// 	assert(soRecursionQueue.empty());
//       }
//       qMoreData = EntMesh.getNextEntIter(iter, entity_handle);
//     }
    // Reset in case we need to go again.
    EntMesh.resetEntIter(iter);

    iTotal += iSwaps;
    cout << iSwaps << " swaps";
    if (iMaxPasses > 1)
      cout << ", " << iTotal << " total";
    cout << endl;
  }
  // Don't need the iterator anymore.
  EntMesh.endEntIter(iter);
  if (iMaxPasses > 1)
    cout << "Face swapping complete." << endl;

  return iTotal;

  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D.swapAll)
}


// DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._misc)
//@ Compute the vector normal for a set of two verts
// This is always rotated 90 degrees counterclockwise from the vector
// running from vertex A to vertex B.
void TSTT_Swap::Swap2D_impl::vNormal
(const double adA[2], const double adB[2], double adRes[2])
{
  adRes[0] = adA[1] - adB[1];
  adRes[1] = adB[0] - adA[0];
}

int TSTT_Swap::Swap2D_impl::iOrient2D
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
int TSTT_Swap::Swap2D_impl::iIncircle
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
bool TSTT_Swap::Swap2D_impl::qTieBreak
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
bool TSTT_Swap::Swap2D_impl::qDoSwapDelaunay
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
bool TSTT_Swap::Swap2D_impl::qDoSwapMaxDihed
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

  double dMaxThis = MAX (dDotBDA, dDotACB);
  double dMaxOther = MAX (dDotDAC, dDotCBD);

  if (dMaxThis > dMaxOther + dEps)
    return true;
  else if (dMaxThis + dEps < dMaxOther)
    return false;
  else
    return qTieBreak (adCoordsA, adCoordsB, adCoordsC, adCoordsD);
}

static double dQual(TSTT_Swap::QualMeasure QM,
		    const double adLocA[2], const double adLocB[2],
		    const double adLocC[2])
{
  static sidl::array<double> sadCoords = sidl::array<double>::create1d(6);
  static double* adAllCoords = getArrayPointer(sadCoords);

  adAllCoords[0] = adLocA[0];
  adAllCoords[1] = adLocA[1];
  adAllCoords[2] = adLocB[0];
  adAllCoords[3] = adLocB[1];
  adAllCoords[4] = adLocC[0];
  adAllCoords[5] = adLocC[1];

  return (QM.calcQuality(sadCoords, 6));
}

bool TSTT_Swap::Swap2D_impl::qDoSwapUser
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3])
{
  double dEps = 1.e-10;
  int iSign = (QM_int.shouldMinimize()) ? -1 : 1;

  // Old configuration
  double dQualABC = iSign*dQual(QM_int, adLocA, adLocB, adLocC);
  double dQualBAD = iSign*dQual(QM_int, adLocB, adLocA, adLocD);

  // New configuration
  double dQualCDB = iSign*dQual(QM_int, adLocC, adLocD, adLocB);
  double dQualDCA = iSign*dQual(QM_int, adLocD, adLocC, adLocA);

  return (MIN(dQualDCA, dQualCDB) > dEps + MIN(dQualABC, dQualBAD));
}

bool TSTT_Swap::Swap2D_impl::qDoSwap
(const double adCoordsA[2], const double adCoordsB[2],
 const double adCoordsL[2], const double adCoordsR[2])
{
  switch (ST_int) {
  case SwapType_MAX_MIN_SINE:
  case SwapType_DELAUNAY:
    return qDoSwapDelaunay (adCoordsA, adCoordsB, adCoordsL, adCoordsR);
  case SwapType_MIN_MAX_ANGLE:
    return qDoSwapMaxDihed (adCoordsA, adCoordsB, adCoordsL, adCoordsR);
  case SwapType_USER_DEFINED:
    // Once I figure out how to register a user callback, I'll rewrite
    // this.
    return qDoSwapUser (adCoordsA, adCoordsB, adCoordsL, adCoordsR);
  default:
    return (false);
  }
}

// DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._misc)

