// 
// File:          TSTT_Swap_Swap3D_Impl.cc
// Symbol:        TSTT_Swap.Swap3D-v0.7
// Symbol Type:   class
// Babel Version: 0.10.10
// Description:   Server-side implementation for TSTT_Swap.Swap3D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.10
// 
#include "TSTT_Swap_Swap3D_Impl.hh"

// DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D._includes)
#ifdef NO_BABEL_QUAL
#include "TSTT_Swap_Delaunay3D_Impl.hh"
#include "TSTT_Swap_MaxMinSine3D_Impl.hh"
#include "TSTT_Swap_MinMaxAngle3D_Impl.hh"
#else
#include "TSTT_Swap_Delaunay3D.hh"
#include "TSTT_Swap_MinMaxAngle3D.hh"
#include "TSTT_Swap_MaxMinSine3D.hh"
#endif

using namespace TSTT_Swap;

#include <math.h>
#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;

#include "TSTT_Swap_InitCanon.cc"

typedef void* opaque;
#define oNullOpaque NULL

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MIN3
#define MIN3(a, b, c) ((a) < (b) ? MIN((a), (c)) : MIN((b), (c)))
#endif

#define MINMAX4(da, db, dc, dd, dMin, dMax) {	\
    double dMax1, dMax2, dMin1, dMin2;		\
    if (da > db) {dMax1 = da; dMin1 = db;}	\
    else         {dMax1 = db; dMin1 = da;}	\
    if (dc > dd) {dMax2 = dc; dMin2 = dd;}	\
    else         {dMax2 = dd; dMin2 = dc;}	\
    dMax = std::max(dMax1, dMax2);		\
    dMin = std::min(dMin1, dMin2);		\
  }
//   if (dMax1 > dMax2) dMax = dMax1; \
//   else               dMax = dMax2; \
//   if (dMin1 < dMin2) dMin = dMin1; \
//   else               dMin = dMin2; \

#define dMAG3D(a) sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2])

#define dDIST3D(a, b) sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) +  (a[2]-b[2])*(a[2]-b[2]))
#define dDIST_SQ_3D(a, b) ((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) +  (a[2]-b[2])*(a[2]-b[2]))

#define adDIFF3D(a, b) {a[0]-b[0], a[1]-b[1], a[2]-b[2]}

#define vSCALE3D(a,d) {a[0]*=d; a[1]*=d; a[2]*=d;}

#define vNORMALIZE3D(a) do {double dInvMag = 1./dMAG3D(a); \
a[0]*=dInvMag; a[1]*=dInvMag; a[2]*=dInvMag;} while(0)

#define dDOT3D(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])

#define vCROSS3D(a,b,res) do { \
res[0] = a[1]*b[2] - a[2]*b[1]; \
res[1] = a[2]*b[0] - a[0]*b[2]; \
res[2] = a[0]*b[1] - a[1]*b[0]; \
} while(0)

#define getArrayPointer(array_) (array_._get_ior()->d_firstElement)

#define my_throw(a, b) do { \
    TSTTB::Error TErr; \
    TErr.set((TSTTB::ErrorType_ ## a), (b)); \
    throw(TErr); \
} while(0)

static int iNum23 = 0;
static int iNum32 = 0;
static int iNum44 = 0;
static int iNum22 = 0;
static int iNumEdge = 0;
static int iNumBdryEdge = 0;
static int aiEdgeReq[10], aiEdgeDone[10];
static int aiBdryEdgeReq[10], aiBdryEdgeDone[10];

// static void vGetCoords(TSTTM::Entity& EntMesh,
// 		       opaque oVert, double adCoords[3])
// {
//   sidl::array<double> sadCoords;
//   int iSize, aiLo[] = {0}, aiHi[] = {2}, aiStride[] = {1};
//   sadCoords.borrow(adCoords, 1, aiLo, aiHi, aiStride);
//   EntMesh.getVtxCoord(oVert, sadCoords, iSize);
//   assert(iSize == 3);
// }
// DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D._includes)

// user-defined constructor.
void TSTT_Swap::Swap3D_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D._ctor)
#ifndef NO_BABEL
  sidl::BaseInterface BI;
  EntMesh = BI;
  ModMesh = BI;
  Assoc = BI;
#ifdef OLD_TSTTR
  GModel = BI;
  assert(GModel._is_nil());
#endif
  assert(EntMesh._is_nil());
  assert(ModMesh._is_nil());
  assert(Assoc._is_nil());
#ifdef HAVE_ARR_INTERFACE
  ArrMesh = BI;
  assert(ArrMesh._is_nil());
#endif
#endif

  setSwapType(SwapType_DELAUNAY); // This sets ST and QM.
  qAllowRecursion = true;
  qAllowEdgeSwapping = true;
  qAllowBdryChanges = true;
  dMaxAngle = 0;
  eFC = eOther;

  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D._ctor)
}

// user-defined destructor.
void TSTT_Swap::Swap3D_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D._dtor)
  // Nothing requires explicit destruction.
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D._dtor)
}

// static class initializer.
void TSTT_Swap::Swap3D_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D._load)
  // Insert-Code-Here {TSTT_Swap.Swap3D._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D._load)
}

// user-defined static methods: (none)

// user-defined non-static methods:
/**
 * Method:  setEdgeSwapping[]
 */
void
TSTT_Swap::Swap3D_impl::setEdgeSwapping (
  /* in */ bool allow_edge_swapping ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setEdgeSwapping)
  qAllowEdgeSwapping = allow_edge_swapping;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setEdgeSwapping)
}

/**
 * Method:  isEdgeSwappingAllowed[]
 */
bool
TSTT_Swap::Swap3D_impl::isEdgeSwappingAllowed ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.isEdgeSwappingAllowed)
  return qAllowEdgeSwapping;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.isEdgeSwappingAllowed)
}

/**
 * Method:  setBdryMutable[]
 */
void
TSTT_Swap::Swap3D_impl::setBdryMutable (
  /* in */ bool allow_bdry_changes ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setBdryMutable)
  qAllowBdryChanges = allow_bdry_changes;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setBdryMutable)
}

/**
 * Method:  isBdryMutable[]
 */
bool
TSTT_Swap::Swap3D_impl::isBdryMutable ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.isBdryMutable)
  return qAllowBdryChanges;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.isBdryMutable)
}

/**
 * Method:  setSwapType[]
 */
void
TSTT_Swap::Swap3D_impl::setSwapType (
  /* in */ ::TSTT_Swap::SwapType ST ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setSwapType)
  if (ST_int != ST) {
    ST_int = ST;
    switch (ST) {
#ifdef NO_BABEL_QUAL
    case SwapType_DELAUNAY:
      pQM = new TSTT_Swap::Delaunay3D_impl();
      break;
    case SwapType_MAX_MIN_SINE:
      pQM = new TSTT_Swap::MaxMinSine3D_impl();
      break;
    case SwapType_MIN_MAX_ANGLE:
      pQM = new TSTT_Swap::MinMaxAngle3D_impl();
      break;
#else
    case SwapType_DELAUNAY:
      QM_int = TSTT_Swap::Delaunay3D::_create();
      break;
    case SwapType_MAX_MIN_SINE:
      QM_int = TSTT_Swap::MaxMinSine3D::_create();
      break;
    case SwapType_MIN_MAX_ANGLE:
      QM_int = TSTT_Swap::MinMaxAngle3D::_create();
      break;
#endif
    default:
      // Do nothing.
      break;
    }
#ifdef NO_BABEL_QUAL
    assert(pQM);
#else
    assert(QM_int._not_nil());
#endif
  }
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setSwapType)
}

/**
 * Method:  setUserQualMeasure[]
 */
void
TSTT_Swap::Swap3D_impl::setUserQualMeasure (
  /* in */ ::TSTT_Swap::QualMeasure QM ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setUserQualMeasure)
#ifndef NO_BABEL_QUAL
  QM_int = QM;
#endif
  setSwapType(SwapType_USER_DEFINED);
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setUserQualMeasure)
}

/**
 * Method:  getSwapType[]
 */
::TSTT_Swap::SwapType
TSTT_Swap::Swap3D_impl::getSwapType ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.getSwapType)
  return ST_int;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.getSwapType)
}

/**
 * Method:  setSwapRecursion[]
 */
void
TSTT_Swap::Swap3D_impl::setSwapRecursion (
  /* in */ bool allow_recursion ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setSwapRecursion)
  qAllowRecursion = allow_recursion;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setSwapRecursion)
}

/**
 * Method:  isRecursionAllowed[]
 */
bool
TSTT_Swap::Swap3D_impl::isRecursionAllowed ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.isRecursionAllowed)
  return qAllowRecursion;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.isRecursionAllowed)
}

/**
 * Method:  setMeshData[]
 */
void
TSTT_Swap::Swap3D_impl::setMeshData (
  /* in */ ::TSTTM::Modify Mod_Mesh ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setMeshData)
  // Won't compile if Mod isn't a TSTTM::Modify object, so this check
  // should never fail.
#ifndef NO_BABEL
  ModMesh = Mod_Mesh;
  if (ModMesh._is_nil())
    my_throw(NOT_SUPPORTED,
	     "Support for Modify interface is mandatory");
  
  EntMesh = Mod_Mesh;
  if (EntMesh._is_nil())
    my_throw(NOT_SUPPORTED,
	     "Support for Entity interface is mandatory");

#ifdef HAVE_ARR_INTERFACE
  ArrMesh = Mod_Mesh;
  if (ArrMesh._is_nil())
    my_throw(NOT_SUPPORTED,
	     "Support for EntArr interface is mandatory");
#endif
#endif
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setMeshData)
}

/**
 * Method:  setModelData[]
 */
void
TSTT_Swap::Swap3D_impl::setModelData (
  /* in */ ::TSTTG::Model Geom_Model ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setModelData)
  GModel = Geom_Model;
  if (GModel._is_nil())
    my_throw(INVALID_ARGUMENT, "Bad model data");
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setModelData)
}

/**
 * Method:  setAssocData[]
 */
void
TSTT_Swap::Swap3D_impl::setAssocData (
  /* in */ ::TSTTR::Associate Assoc_Data ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setAssocData)
  Assoc = Assoc_Data;
  if (Assoc._is_nil())
    my_throw(INVALID_ARGUMENT, "Bad association data");
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setAssocData)
}

/**
 * Method:  swap[]
 */
int32_t
TSTT_Swap::Swap3D_impl::swap (
  /* inout */ void*& entity_set,
  /* inout */ void*& entity_handle ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.swap)

  if (EntMesh.getEntTopo(entity_handle) != TSTTM::EntityTopology_TRIANGLE) {
    my_throw(INVALID_ENTITY_HANDLE,
	     "Can only swap triangular faces");
  }
    
//   cout << "Trying to swap face with handle " << entity_handle << endl;
  opaque aoVerts[6], oPivot0, oPivot1, oOther;
  opaque aoTets[4];
  eFC = eCategorizeFace(entity_handle, aoVerts, aoTets,
			oPivot0, oPivot1, oOther);
  
  //@@ Determine the preferable configuration and swap if necessary
  switch (eFC) {
  // These cases are handled by edge swapping.
  case eN44:
  case eN32:
    if (ST_int != TSTT_Swap::SwapType_DELAUNAY && qAllowEdgeSwapping)
      return (iEdgeSwap3D(entity_set, entity_handle,
			  oPivot0, oPivot1, oOther));
    else
      return 0;
    // These cases are definitely unswappable
  case eT44:
//     cout << "swap T44? ";
    // In this case, you should swap if and only if -both- sides of the
    // plane benefit from the reconfiguration.
    if (qDoSwap(aoVerts[0], aoVerts[1], aoVerts[2], aoVerts[3], aoVerts[4]) &&
	qDoSwap(aoVerts[1], aoVerts[0], aoVerts[5], aoVerts[3], aoVerts[4])) {
//       cout << "yes" << endl;
      return (iReconfTet44(entity_set, aoTets, entity_handle, aoVerts));
    }
    else {
//       cout << "no" << endl;
      return 0;
    }
  case eT22:
//     cout << "swap T22? ";
    if (qDoSwap(aoVerts[0], aoVerts[1], aoVerts[2], aoVerts[3],	aoVerts[4])) {
//       cout << "yes" << endl;
      return (iReconfTet22(entity_set, aoTets, entity_handle, aoVerts));
    }
    else {
//       cout << "no" << endl;
      return 0;
    }
  case eT23:
//     cout << "swap T23? ";
    if (qDoSwap(aoVerts[0], aoVerts[1], aoVerts[2], aoVerts[3],	aoVerts[4])) {
//       cout << "yes" << endl;
      return (iReconfTet23(entity_set, aoTets, entity_handle, aoVerts));
    }
    else {
//       cout << "no" << endl;
      return 0;
    }
  case eT32:
//     cout << "swap T32? ";
    if (qDoSwap(aoVerts[0], aoVerts[1], aoVerts[2], aoVerts[3],	aoVerts[4])) {
//       cout << "yes" << endl;
      return (iReconfTet32(entity_set, aoTets, entity_handle, aoVerts));
    }
    else {
//       cout << "no" << endl;
      return 0;
    }
  case eBdry:
  case eN40:
  case eN30:
  case eN20:
  case eBdryReflex:
  case eOther:
    return 0;
  default: // Catch-all for bad cases
    assert(0);
    return 0;
  }
  // 
  return 0; 
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.swap)
}

/**
 * Method:  swapAll[]
 */
int32_t
TSTT_Swap::Swap3D_impl::swapAll (
  /* inout */ void*& entity_set ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.swapAll)
  void * iter;
  EntMesh.initEntIter(entity_set, TSTTM::EntityType_FACE,
		      TSTTM::EntityTopology_TRIANGLE, iter);

  int iTotal = 0, iPasses = 0, iSwaps = 1, iMaxPasses = 15;

  if (qAllowRecursion) iMaxPasses = 5;
  while (iSwaps && iPasses < iMaxPasses) {
    soRecursionQueue.clear();
    ++iPasses;
    if (iMaxPasses > 1)
      cout << "   Pass " << iPasses << ": ";
    iSwaps = 0;
    // Iterate over all faces.
    void *entity_handle;
    bool qMoreData = EntMesh.getNextEntIter(iter, entity_handle);
//     while (qMoreData) {
//       soRecursionQueue.insert(entity_handle);
//       qMoreData = EntMesh.getNextEntIter(iter, entity_handle);
//     }
//     while (!soRecursionQueue.empty()) {
//       std::set<opaque>::iterator iter = soRecursionQueue.begin();
//       entity_handle = *iter;
//       soRecursionQueue.erase(iter);
//       iSwaps += swap(entity_set, entity_handle);
//     }
    while (qMoreData) {
      iSwaps += swap(entity_set, entity_handle);
      // Now clean out the recursion queue.
      if (qAllowRecursion) {
	while (!soRecursionQueue.empty()) {
	  std::set<opaque>::iterator iter = soRecursionQueue.begin();
	  entity_handle = *iter;
	  soRecursionQueue.erase(iter);
	  iSwaps += swap(entity_set, entity_handle);
	}
	assert(soRecursionQueue.empty());
      }
      qMoreData = EntMesh.getNextEntIter(iter, entity_handle);
    }
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

  cout << "  Swapped 2 tets for 3: " << iNum23 << endl;
  cout << "  Swapped 3 tets for 2: " << iNum32 << endl;
  cout << "  Swapped 2 tets for 2: " << iNum22 << endl;
  cout << "  Swapped 4 tets for 4: " << iNum44 << endl;
  if (isEdgeSwappingAllowed()) {
    cout << "  Swapped interior edge: " << iNumEdge << endl;
    cout << "    Swaps from 3 tets: " << aiEdgeDone[3]
	 << " out of " << aiEdgeReq[3] << endl;
    cout << "    Swaps from 4 tets: " << aiEdgeDone[4]
	 << " out of " << aiEdgeReq[4] << endl;
    cout << "    Swaps from 5 tets: " << aiEdgeDone[5]
	 << " out of " << aiEdgeReq[5] << endl;
    cout << "    Swaps from 6 tets: " << aiEdgeDone[6]
	 << " out of " << aiEdgeReq[6] << endl;
    cout << "    Swaps from 7 tets: " << aiEdgeDone[7]
	 << " out of " << aiEdgeReq[7] << endl;
    if (isBdryMutable()) {
      cout << "  Swapped boundary edge: " << iNumBdryEdge << endl;
      cout << "    Swaps from 3 tets: " << aiBdryEdgeDone[3]
	   << " out of " << aiBdryEdgeReq[3] << endl;
      cout << "    Swaps from 4 tets: " << aiBdryEdgeDone[4]
	   << " out of " << aiBdryEdgeReq[4] << endl;
      cout << "    Swaps from 5 tets: " << aiBdryEdgeDone[5]
	   << " out of " << aiBdryEdgeReq[5] << endl;
      cout << "    Swaps from 6 tets: " << aiBdryEdgeDone[6]
	   << " out of " << aiBdryEdgeReq[6] << endl;
      cout << "    Swaps from 7 tets: " << aiBdryEdgeDone[7]
	   << " out of " << aiBdryEdgeReq[7] << endl;
    }
  }

//   // Entity handles may have gotten cooked in the process; this is
//   // implementation dependent.
//   EntMesh.areEHValid(true);
  return iTotal;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.swapAll)
}


// DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D._misc)
int TSTT_Swap::Swap3D_impl::iOrient3D
(const double adA[3], const double adB[3],
 const double adC[3], const double adD[3]) 
     // Computes the orientation of four verts in 3D.
{
  double dXa = adA[0];
  double dYa = adA[1];
  double dZa = adA[2];
  
  double dXb = adB[0];
  double dYb = adB[1];
  double dZb = adB[2];
  
  double dXc = adC[0];
  double dYc = adC[1];
  double dZc = adC[2];
  
  double dXd = adD[0];
  double dYd = adD[1];
  double dZd = adD[2];
  
  double dMaxX, dMinX, dMaxY, dMinY, dMaxZ, dMinZ;
  
  double dDX2 = dXb - dXa;
  double dDX3 = dXc - dXa;
  double dDX4 = dXd - dXa;
  MINMAX4(dXa, dXb, dXc, dXd, dMinX, dMaxX);
  
  double dDY2 = dYb - dYa;
  double dDY3 = dYc - dYa;
  double dDY4 = dYd - dYa;
  MINMAX4(dYa, dYb, dYc, dYd, dMinY, dMaxY);
  
  double dDZ2 = dZb - dZa;
  double dDZ3 = dZc - dZa;
  double dDZ4 = dZd - dZa;
  MINMAX4(dZa, dZb, dZc, dZd, dMinZ, dMaxZ);
  double dMax = MAX( MAX(dMaxX-dMinX, dMaxY-dMinY), dMaxZ-dMinZ);
  
  // dDet is proportional to the cell volume
  double dDet = (dDX2*(dDY3*dDZ4 - dDY4*dDZ3) +
		 dDX3*(dDY4*dDZ2 - dDY2*dDZ4) +
		 dDX4*(dDY2*dDZ3 - dDY3*dDZ2));

  //   // Compute a length scale based on edge lengths.
  //   double dScale = ( dDIST3D(adA, adB) + dDIST3D(adA, adC) +
  // 		    dDIST3D(adA, adD) + dDIST3D(adB, adC) +
  // 		    dDIST3D(adB, adD) + dDIST3D(adC, adD) ) / 6.;
  
  //   dDet /= (dScale*dScale*dScale);
  
  //   double dError = 1.e-13;
  
  // Compute an upper bound on the error bound.
  
  const double dMachEps = 2.22044605e-13; // about 2^-52 * 1000;
  
  double dError = dMachEps * dMax*dMax*dMax;
  
  if (dDet > dError)
    return (1);
  else if (dDet < -dError) 
    return (-1);
  else
    return(0);
}

int TSTT_Swap::Swap3D_impl::iOrient3D(opaque oTet)
{
  assert(EntMesh.getEntTopo(oTet) == TSTTM::EntityTopology_TETRAHEDRON);
  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(4);

  int iNV;
  EntMesh.getEntAdj(oTet, TSTTM::EntityType_VERTEX, saoVerts, iNV);
  assert(iNV == 4);

  static sidl::array<double> sadCoords = sidl::array<double>::create1d(12);
  static double *adCoords = getArrayPointer(sadCoords);

  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  int iSize;
  EntMesh.getVtxArrCoords(saoVerts, 4, SO, sadCoords, iSize);
  assert(iSize == 12);

  return iOrient3D(adCoords, adCoords+3, adCoords+6, adCoords+9);
}

int TSTT_Swap::Swap3D_impl::iOrient3D
(opaque oVert0, opaque oVert1, opaque oVert2, opaque oVert3)
{
  assert(EntMesh.getEntTopo(oVert0) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVert1) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVert2) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVert3) == TSTTM::EntityTopology_POINT);

  // Using a single call to getVtxArrCoords is much more efficient than
  // multiple calls to getVtxCoords. 
  static sidl::array<double> sadCoords = sidl::array<double>::create1d(12);
  static double *adCoords = getArrayPointer(sadCoords);

  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(4);
  static opaque *aoVerts = getArrayPointer(saoVerts);
  aoVerts[0] = oVert0;
  aoVerts[1] = oVert1;
  aoVerts[2] = oVert2;
  aoVerts[3] = oVert3;

  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  int iSize;
  EntMesh.getVtxArrCoords(saoVerts, 4, SO, sadCoords, iSize);
  assert(iSize == 12);

  return iOrient3D(adCoords, adCoords+3, adCoords+6, adCoords+9);
}

double TSTT_Swap::Swap3D_impl::dDet4By4(const double a2dMat4[4][4]) const
{
  return (  a2dMat4[0][0] * (  a2dMat4[1][1]*a2dMat4[2][2]*a2dMat4[3][3]
                             + a2dMat4[1][2]*a2dMat4[2][3]*a2dMat4[3][1]
                             + a2dMat4[1][3]*a2dMat4[2][1]*a2dMat4[3][2]
                             - a2dMat4[3][1]*a2dMat4[2][2]*a2dMat4[1][3]
                             - a2dMat4[3][2]*a2dMat4[2][3]*a2dMat4[1][1]
                             - a2dMat4[3][3]*a2dMat4[2][1]*a2dMat4[1][2] )
          - a2dMat4[0][1] * (  a2dMat4[1][0]*a2dMat4[2][2]*a2dMat4[3][3]
                             + a2dMat4[1][2]*a2dMat4[2][3]*a2dMat4[3][0]
                             + a2dMat4[1][3]*a2dMat4[2][0]*a2dMat4[3][2]
                             - a2dMat4[3][0]*a2dMat4[2][2]*a2dMat4[1][3]
                             - a2dMat4[3][2]*a2dMat4[2][3]*a2dMat4[1][0]
                             - a2dMat4[3][3]*a2dMat4[2][0]*a2dMat4[1][2] )
          + a2dMat4[0][2] * (  a2dMat4[1][0]*a2dMat4[2][1]*a2dMat4[3][3]
                             + a2dMat4[1][1]*a2dMat4[2][3]*a2dMat4[3][0]
                             + a2dMat4[1][3]*a2dMat4[2][0]*a2dMat4[3][1]
                             - a2dMat4[3][0]*a2dMat4[2][1]*a2dMat4[1][3]
                             - a2dMat4[3][1]*a2dMat4[2][3]*a2dMat4[1][0]
                             - a2dMat4[3][3]*a2dMat4[2][0]*a2dMat4[1][1] )
          - a2dMat4[0][3] * (  a2dMat4[1][0]*a2dMat4[2][1]*a2dMat4[3][2]
                             + a2dMat4[1][1]*a2dMat4[2][2]*a2dMat4[3][0]
                             + a2dMat4[1][2]*a2dMat4[2][0]*a2dMat4[3][1]
                             - a2dMat4[3][0]*a2dMat4[2][1]*a2dMat4[1][2]
                             - a2dMat4[3][1]*a2dMat4[2][2]*a2dMat4[1][0]
                             - a2dMat4[3][2]*a2dMat4[2][0]*a2dMat4[1][1] ) );
}

int TSTT_Swap::Swap3D_impl::iInsphere
(const double adA[3], const double adB[3], const double adC[3],
 const double adD[3], const double adE[3])
{    
  double dXa = adA[0];
  double dYa = adA[1];
  double dZa = adA[2];
  
  double dXb = adB[0];
  double dYb = adB[1];
  double dZb = adB[2];
  
  double dXc = adC[0];
  double dYc = adC[1];
  double dZc = adC[2];
  
  double dXd = adD[0];
  double dYd = adD[1];
  double dZd = adD[2];
  
  double dXe = adE[0];
  double dYe = adE[1];
  double dZe = adE[2];
  
  double a2dInSphMat[4][4], dWa;
  dWa = dXa*dXa + dYa*dYa + dZa*dZa;
  
  a2dInSphMat[0][0] = dXb - dXa;
  a2dInSphMat[0][1] = dYb - dYa;
  a2dInSphMat[0][2] = dZb - dZa;
  a2dInSphMat[0][3] = dXb*dXb + dYb*dYb + dZb*dZb - dWa;
  
  a2dInSphMat[1][0] = dXc - dXa;
  a2dInSphMat[1][1] = dYc - dYa;
  a2dInSphMat[1][2] = dZc - dZa;
  a2dInSphMat[1][3] = dXc*dXc + dYc*dYc + dZc*dZc - dWa;
  
  a2dInSphMat[2][0] = dXd - dXa;
  a2dInSphMat[2][1] = dYd - dYa;
  a2dInSphMat[2][2] = dZd - dZa;
  a2dInSphMat[2][3] = dXd*dXd + dYd*dYd + dZd*dZd - dWa;
  
  a2dInSphMat[3][0] = dXe - dXa;
  a2dInSphMat[3][1] = dYe - dYa;
  a2dInSphMat[3][2] = dZe - dZa;
  a2dInSphMat[3][3] = dXe*dXe + dYe*dYe + dZe*dZe - dWa;
  
  // Set up a scale that is the average of the distance from A to each
  // of the other verts.  Use some trickery to take advantage of
  // already knowing what the differences in coordinates are.
  double dAveLen = 0.25 * (dMAG3D(a2dInSphMat[0]) +
			   dMAG3D(a2dInSphMat[1]) +
			   dMAG3D(a2dInSphMat[2]) +
			   dMAG3D(a2dInSphMat[3]));
  double dScale = pow(dAveLen, 5);
  double dDet = dDet4By4(a2dInSphMat) / dScale *
    iOrient3D(adA, adB, adC, adD);
  double dEps = 1.e-12;
  if      (dDet >  dEps) return (-1);
  else if (dDet < -dEps) return ( 1);
  else                   return ( 0);
}

void TSTT_Swap::Swap3D_impl::vUnitNormal
(const double adA[3], const double adB[3],
 const double adC[3], double adRes[3])
{
  double adDiffBA[] = adDIFF3D(adB, adA);
  double adDiffCB[] = adDIFF3D(adC, adB);

  adRes[0] = adDiffCB[1]*adDiffBA[2] - adDiffCB[2]*adDiffBA[1];
  adRes[1] = adDiffCB[2]*adDiffBA[0] - adDiffCB[0]*adDiffBA[2];
  adRes[2] = adDiffCB[0]*adDiffBA[1] - adDiffCB[1]*adDiffBA[0];

  double dInvMag = 1./sqrt(adRes[0]*adRes[0] + adRes[1]*adRes[1] +
			   adRes[2]*adRes[2]);
  adRes[0] *= dInvMag;
  adRes[1] *= dInvMag;
  adRes[2] *= dInvMag;
}

int TSTT_Swap::Swap3D_impl::iGenOrient3D
(const opaque oVert0, const opaque oVert1, const opaque oVert2,
 const opaque oVert3, bool& qApproxFlat)
{
  // Using a single call to getVtxArrCoords is much more efficient than
  // multiple calls to getVtxCoords. 
  static sidl::array<double> sadCoords = sidl::array<double>::create1d(12);
  static double *adCoords = getArrayPointer(sadCoords);

  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(4);
  static opaque *aoVerts = getArrayPointer(saoVerts);
  aoVerts[0] = oVert0;
  aoVerts[1] = oVert1;
  aoVerts[2] = oVert2;
  aoVerts[3] = oVert3;
    
  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  int iSize;
  EntMesh.getVtxArrCoords(saoVerts, 4, SO, sadCoords, iSize);
  assert(iSize == 12);

  double * adCoords0 = adCoords;
  double * adCoords1 = adCoords + 3;
  double * adCoords2 = adCoords + 6;
  double * adCoords3 = adCoords + 9;
  
  int iOrient = iOrient3D(adCoords0, adCoords1, adCoords2, adCoords3);

  qApproxFlat = false;
  if (iOrient == 1) {
    // Check whether it's nearly flat
    double adNorm0[3], adNorm1[3];
    vUnitNormal(adCoords0, adCoords1, adCoords2, adNorm0);
    vUnitNormal(adCoords2, adCoords1, adCoords3, adNorm1);
    double dDot = dDOT3D(adNorm0, adNorm1);
    if (dDot > cos(dMaxAngle * M_PI / 180)) {
      qApproxFlat = true;
    }
  }
  return iOrient;
}

opaque TSTT_Swap::Swap3D_impl::getFaceOpposite
(const opaque oCell, const opaque oVert)
{
  assert(EntMesh.getEntTopo(oCell) == TSTTM::EntityTopology_TETRAHEDRON);
  assert(EntMesh.getEntTopo(oVert) == TSTTM::EntityTopology_POINT);
  static sidl::array<opaque> saoFaces = sidl::array<opaque>::create1d(4);
  static opaque *aoFaces = getArrayPointer(saoFaces);

  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(4);
  static opaque *aoVerts = getArrayPointer(saoVerts);

  int iSize;
  EntMesh.getEntAdj(oCell, TSTTM::EntityType_FACE, saoFaces, iSize);
  assert(iSize == 4);

  // Maybe use block call here (block of 4)
  for (int iF = 0; iF < 4; iF++) {
    EntMesh.getEntAdj(aoFaces[iF], TSTTM::EntityType_VERTEX, saoVerts, iSize);
    assert(iSize == 3);
    if (aoVerts[0] != oVert && aoVerts[1] != oVert && aoVerts[2] != oVert)
      return aoFaces[iF];
  }
  // Should never get here.
  assert(0);
  return oNullOpaque;
}

void TSTT_Swap::Swap3D_impl::getFacesOpposite
(const opaque oCell, const opaque aoVerts[3], opaque aoFacesOut[3])
{
  assert(EntMesh.getEntTopo(oCell) == TSTTM::EntityTopology_TETRAHEDRON);
  assert(EntMesh.getEntTopo(aoVerts[0]) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(aoVerts[1]) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(aoVerts[2]) == TSTTM::EntityTopology_POINT);
  static sidl::array<opaque> saoFaces = sidl::array<opaque>::create1d(4);
  static opaque *aoFaces = getArrayPointer(saoFaces);

  int iSize;
  EntMesh.getEntAdj(oCell, TSTTM::EntityType_FACE, saoFaces, iSize);
  assert(iSize == 4);

  aoFacesOut[0] = aoFacesOut[1] = aoFacesOut[2] = oNullOpaque;

#ifdef HAVE_ARR_INTERFACE
  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(12);
  static sidl::array<int> saiOffset;
  int offset_size;
  ArrMesh.getEntArrAdj(saoFaces, 4, TSTTM::EntityType_VERTEX, saoVerts, iSize,
		       saiOffset, offset_size);
  assert(iSize == 12);
  assert(offset_size == 5);
#else
  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(3);
#endif
  static opaque *aoVertsTmp = getArrayPointer(saoVerts);

  for (int iF = 0; iF < 4; iF++) {
#ifdef HAVE_ARR_INTERFACE
    for (int iV = 0; iV < 3; iV++) {
      if (aoVertsTmp[0 + iF*3] != aoVerts[iV] &&

	  aoVertsTmp[1 + iF*3] != aoVerts[iV] &&
	  aoVertsTmp[2 + iF*3] != aoVerts[iV])
	aoFacesOut[iV] = aoFaces[iF];
    }
#else
    EntMesh.getEntAdj(aoFaces[iF], TSTTM::EntityType_VERTEX, saoVerts, iSize);
    assert(iSize == 3);
    for (int iV = 0; iV < 3; iV++) {
      if (aoVertsTmp[0] != aoVerts[iV] &&
	  aoVertsTmp[1] != aoVerts[iV] &&
	  aoVertsTmp[2] != aoVerts[iV])
	aoFacesOut[iV] = aoFaces[iF];
    }
#endif
  }
  assert(aoFacesOut[0] != oNullOpaque);
  assert(aoFacesOut[1] != oNullOpaque);
  assert(aoFacesOut[2] != oNullOpaque);
}

opaque TSTT_Swap::Swap3D_impl::getVertOpposite
(const opaque oCell, const opaque oFace)
{
  assert(EntMesh.getEntTopo(oCell) == TSTTM::EntityTopology_TETRAHEDRON);
  assert(EntMesh.getEntTopo(oFace) == TSTTM::EntityTopology_TRIANGLE);
  static sidl::array<opaque> saoCVerts = sidl::array<opaque>::create1d(4);
  static opaque *aoCellVerts = getArrayPointer(saoCVerts);

  static sidl::array<opaque> saoFVerts = sidl::array<opaque>::create1d(3);
  static opaque *aoFaceVerts = getArrayPointer(saoFVerts);

  // Probably use block call here. (block of 2)
  int iSize;
  EntMesh.getEntAdj(oCell, TSTTM::EntityType_VERTEX, saoCVerts, iSize);
  assert(iSize == 4);

  EntMesh.getEntAdj(oFace, TSTTM::EntityType_VERTEX, saoFVerts, iSize);
  assert(iSize == 3);

  for (int iV = 0; iV < 4; iV++) {
    opaque oVert = aoCellVerts[iV];
    if (aoFaceVerts[0] != oVert && aoFaceVerts[1] != oVert &&
	aoFaceVerts[2] != oVert)
      return oVert;
  }
  // Should never get here.
  assert(0);
  return oNullOpaque;
}

opaque TSTT_Swap::Swap3D_impl::getCellOpposite
(const opaque oFace, const opaque oCell)
{
  assert(EntMesh.getEntType(oCell) == TSTTM::EntityType_REGION);
  assert(EntMesh.getEntType(oFace) == TSTTM::EntityType_FACE);
  static sidl::array<opaque> saoCells = sidl::array<opaque>::create1d(2);
  static opaque *aoCells = getArrayPointer(saoCells);
  
  int iSize;
  EntMesh.getEntAdj(oFace, TSTTM::EntityType_REGION, saoCells, iSize);
  if (iSize == 1) return oNullOpaque;
  else assert(iSize == 2);

  if (aoCells[0] == oCell)
    return aoCells[1];
  else {
    assert(aoCells[1] == oCell);
    return aoCells[0];
  }
}

//@ Decide which type of triangular face this is, anyway.
TSTT_Swap::Swap3D_impl::eFaceCat TSTT_Swap::Swap3D_impl::eCategorizeFace
(opaque oFace, opaque aoVerts[6], opaque aoTets[4],
 opaque& oPivot0, opaque& oPivot1, opaque& oOther)
{
  // Make a general SIDL array of opaques, for calls to connectivity.
  // This has a size of 8 because there a block call for face-tet
  // connectivity that needs that many.
  static sidl::array<opaque> saEnts = sidl::array<opaque>::create1d(8);
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

#ifdef HAVE_ARR_INTERFACE
  // Now use a block call for what was once three vertex connectivity
  // calls.  Create these arrays to handle a call that returns two cells
  // for each of eight faces.
  static sidl::array<opaque> saBigEnts = sidl::array<opaque>::create1d(16);
  static sidl::array<int> saiOffset = sidl::array<int>::create1d(9);
  int offset_size;
  static opaque *aoBigEnts = getArrayPointer(saBigEnts);
  static int *aiOffset = getArrayPointer(saiOffset);

  aoEntities[2] = oFace;
//   cout << aoTets[0] << " " << aoTets[1] << " " << oFace << endl;
//   cout << aoEntities[0] << " " << aoEntities[1] << " " << aoEntities[2]
//        << endl << endl;

  ArrMesh.getEntArrAdj(saEnts, 3, TSTTM::EntityType_VERTEX, saBigEnts, iSize,
		       saiOffset, offset_size);
  assert(offset_size == 4);
  assert(iSize == 11);
  aoVerts[0] = aoBigEnts[8];
  aoVerts[1] = aoBigEnts[9];
  aoVerts[2] = aoBigEnts[10];
  aoVerts[3] = aoVerts[4] = oNullOpaque;

  for (int ii = 0; ii < 4; ii++) {
    if (aoBigEnts[ii] != aoVerts[0] &&
	aoBigEnts[ii] != aoVerts[1] &&
	aoBigEnts[ii] != aoVerts[2]) {
      aoVerts[3] = aoBigEnts[ii];
      break;
    }
  }

  for (int ii = 4; ii < 8; ii++) {
    assert(aoBigEnts[ii] != aoVerts[3]);
    if (aoBigEnts[ii] != aoVerts[0] &&
	aoBigEnts[ii] != aoVerts[1] &&
	aoBigEnts[ii] != aoVerts[2]) {
      aoVerts[4] = aoBigEnts[ii];
      break;
    }
  }
#else
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
#endif
  assert(aoVerts[3] != oNullOpaque);
  assert(aoVerts[4] != oNullOpaque);

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
#ifdef HAVE_ARR_INTERFACE
    // This collection of block calls actually seem to slow things down!

    // First, do a tet->face call, using saEnts to hold the (output) faces.
    EntMesh.getEntAdj(aoTets[0], TSTTM::EntityType_FACE, saEnts, iSize);
    assert(iSize == 4);

    // Now do a block face-tet call, using saEnts to hold the (input)
    // faces and saBigEnts to hold the (output) tets.
    ArrMesh.getEntArrAdj(saEnts, 4, TSTTM::EntityType_REGION,
			 saBigEnts, iSize, saiOffset, offset_size);
    assert(iSize <= 8);
    assert(offset_size == 5);

    // Identify the adjacent tets
    for (int ii = 0; ii < 4; ii++) {
      int iOff = aiOffset[ii];
      if (aiOffset[ii+1] == iOff + 1) aoAdjTets0[ii] = oNullOpaque;
      else {
	if (aoBigEnts[iOff] == aoTets[0]) {
	  aoAdjTets0[ii] = aoBigEnts[iOff + 1];
	}
	else {
	  assert(aoBigEnts[iOff + 1] == aoTets[0]);
	  aoAdjTets0[ii] = aoBigEnts[iOff];
	}
      }
    }

    // First, do a tet->face call, using saEnts to hold the (output) faces.
    EntMesh.getEntAdj(aoTets[1], TSTTM::EntityType_FACE, saEnts, iSize);
    assert(iSize == 4);

    // Now do a block face-tet call, using saEnts to hold the (input)
    // faces and saBigEnts to hold the (output) tets.
    ArrMesh.getEntArrAdj(saEnts, 4, TSTTM::EntityType_REGION,
			 saBigEnts, iSize, saiOffset, offset_size);
    assert(iSize <= 8);

    // Identify the adjacent tets
    for (int ii = 0; ii < 4; ii++) {
      int iOff = aiOffset[ii];
      if (aiOffset[ii+1] == iOff + 1) aoAdjTets1[ii] = oNullOpaque;
      else {
	if (aoBigEnts[iOff] == aoTets[1]) {
	  aoAdjTets1[ii] = aoBigEnts[iOff + 1];
	}
	else {
	  assert(aoBigEnts[iOff + 1] == aoTets[1]);
	  aoAdjTets1[ii] = aoBigEnts[iOff];
	}
      }
    }
#ifndef NDEBUG
    {
      
      EntMesh.getEntAdj(aoTets[0], TSTTM::EntityType_FACE,
			saEnts, iSize);
      assert(iSize == 4);
      aoFaces[0] = aoEntities[0];
      aoFaces[1] = aoEntities[1];
      aoFaces[2] = aoEntities[2];
      aoFaces[3] = aoEntities[3];

      opaque aoAdjTets0Copy[4], aoAdjTets1Copy[4];
      // Can use block call here (four entities)
      for (int ii = 0; ii < 4; ii++) {
	EntMesh.getEntAdj(aoFaces[ii], TSTTM::EntityType_REGION,
			  saEnts, iSize);
	if (iSize == 1) aoAdjTets0Copy[ii] = oNullOpaque;
	else {
	  if (aoEntities[0] == aoTets[0]) {
	    aoAdjTets0Copy[ii] = aoEntities[1];
	  }
	  else {
	    assert(aoEntities[1] == aoTets[0]);
	    aoAdjTets0Copy[ii] = aoEntities[0];
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
      
      // Can use block call here (four entities) (or merge w/ above for 8?)
      for (int ii = 0; ii < 4; ii++) {
	EntMesh.getEntAdj(aoFaces[ii], TSTTM::EntityType_REGION,
			  saEnts, iSize);
	if (iSize == 1) aoAdjTets1Copy[ii] = oNullOpaque;
	else {
	  if (aoEntities[0] == aoTets[1]) {
	    aoAdjTets1Copy[ii] = aoEntities[1];
	  }
	  else {
	    assert(aoEntities[1] == aoTets[1]);
	    aoAdjTets1Copy[ii] = aoEntities[0];
	  }
	}
      }
      assert(aoAdjTets0[0] == aoAdjTets0Copy[0]);
      assert(aoAdjTets0[1] == aoAdjTets0Copy[1]);
      assert(aoAdjTets0[2] == aoAdjTets0Copy[2]);
      assert(aoAdjTets0[3] == aoAdjTets0Copy[3]);
      assert(aoAdjTets1[0] == aoAdjTets1Copy[0]);
      assert(aoAdjTets1[1] == aoAdjTets1Copy[1]);
      assert(aoAdjTets1[2] == aoAdjTets1Copy[2]);
      assert(aoAdjTets1[3] == aoAdjTets1Copy[3]);
    } // End the debugging block
#endif
#else
    EntMesh.getEntAdj(aoTets[0], TSTTM::EntityType_FACE,
		      saEnts, iSize);
    assert(iSize == 4);
    aoFaces[0] = aoEntities[0];
    aoFaces[1] = aoEntities[1];
    aoFaces[2] = aoEntities[2];
    aoFaces[3] = aoEntities[3];

    // Can use block call here (four entities)
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

    // Can use block call here (four entities) (or merge w/ above for 8?)
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
#endif

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
	  opaque oClassA = oNullOpaque, oClassB = oNullOpaque;
#ifdef CLASSIFY
#ifdef OLD_TSTTR
	  Assoc.getEntEntAssociation(ModMesh, GModel, oFaceA, oClassA);
	  Assoc.getEntEntAssociation(ModMesh, GModel, oFaceB, oClassB);
#else
	  Assoc.getEntEntAssociation(oFaceA, oClassA);
	  Assoc.getEntEntAssociation(oFaceB, oClassB);
#endif
#endif
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
	      opaque oClassA = oNullOpaque, oClassB = oNullOpaque;
#ifdef CLASSIFY
#ifdef OLD_TSTTR
	      Assoc.getEntEntAssociation(ModMesh, GModel, oFaceA, oClassA);
	      Assoc.getEntEntAssociation(ModMesh, GModel, oFaceB, oClassB);
#else
	      Assoc.getEntEntAssociation(oFaceA, oClassA);
	      Assoc.getEntEntAssociation(oFaceB, oClassB);
#endif
#endif
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

//@@ Determines whether swapping is needed for local Delaunay-ness.
// Returns true to swap, false to remain the same.
bool TSTT_Swap::Swap3D_impl::qDoSwapDelaunay
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3], const double adLocE[3])
{
  int iInSphereRes = iInsphere(adLocA, adLocB, adLocC, adLocD, adLocE);

  switch (eFC) {
  case eT32:
    // Current tets are ABDE, BCDE, CADE.  Swap if pVVertE is on (0) or
    // outside (-1) the circumsphere of ABCD.  This is an arbitrary
    // tie-break; configurations which are co-spherical go to the two
    // tet configuration.
    return (iInSphereRes != 1);
  case eT23:
  case eT22:
  case eT44: // This one is the same as eT22 for all situations.
    // Current tets are ABCD, CBAE.  Swap if pVVertE is inside (1) the
    // circumsphere of ABCD.  Again, configurations which are
    // co-spherical go to the two-tet configuration for T23.
    return (iInSphereRes == 1);
  default:
    return(false);
  }
}

//@@ Determines whether swapping will improve the maximum face angle.
// Returns true to swap, false to remain the same.
bool TSTT_Swap::Swap3D_impl::qDoSwapMaxDihed
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3], const double adLocE[3])
{
  // This algorithm finds face angles between adjacent faces by dotting
  // their unit normals.  The largest magnitude loses.
  //
  // To prevent pairs from flopping back and forth, a swap is only
  // authorized if the inequality is bigger than eps.

  double dEps = 1.e-10;
  switch (eFC) {
  case eT23:
  case eT32:
    {
      //@@@ The case where the five pVVerts form a genuinely convex set
      double adNormABD[3], adNormABE[3], adNormADE[3];
      double adNormBCD[3], adNormBCE[3], adNormBDE[3];
      double adNormCAD[3], adNormCAE[3], adNormCDE[3];
      vUnitNormal(adLocA, adLocB, adLocC, adNormABD);
      vUnitNormal(adLocA, adLocB, adLocE, adNormABE);
      vUnitNormal(adLocA, adLocD, adLocE, adNormADE);

      vUnitNormal(adLocB, adLocC, adLocD, adNormBCD);
      vUnitNormal(adLocB, adLocC, adLocE, adNormBCE);
      vUnitNormal(adLocB, adLocD, adLocE, adNormBDE);

      vUnitNormal(adLocC, adLocA, adLocD, adNormCAD);
      vUnitNormal(adLocC, adLocA, adLocE, adNormCAE);
      vUnitNormal(adLocC, adLocD, adLocE, adNormCDE);

      double dLastDot, dWorst2 = -1., dWorst3 = -1.;

      // Care must be taken to be certain that I'm taking the dot
      // product of two vectors that both point into the enclosed angle
      // or both point out, or correct the sign.

      dLastDot = dDOT3D(adNormABD, adNormBCD);   /* Both in */
      dWorst2 = MAX(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormBCD, adNormCAD);   /* Both in */
      dWorst2 = MAX(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormCAD, adNormABD);   /* Both in */
      dWorst2 = MAX(dWorst2, dLastDot);

      dLastDot = dDOT3D(adNormABE, adNormBCE);   /* Both out */
      dWorst2 = MAX(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormBCE, adNormCAE);   /* Both out */
      dWorst2 = MAX(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormCAE, adNormABE);   /* Both out */
      dWorst2 = MAX(dWorst2, dLastDot);

      dLastDot = - dDOT3D(adNormABD, adNormABE); /* One in, one out */
      dWorst3 = MAX(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormBCD, adNormBCE); /* One in, one out */
      dWorst3 = MAX(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormCAD, adNormCAE); /* One in, one out */
      dWorst3 = MAX(dWorst3, dLastDot);

      dLastDot = - dDOT3D(adNormADE, adNormBDE); /* One in, one out */
      dWorst3 = MAX(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormBDE, adNormCDE); /* One in, one out */
      dWorst3 = MAX(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormCDE, adNormADE); /* One in, one out */
      dWorst3 = MAX(dWorst3, dLastDot);

      if      (dWorst3 > dWorst2 + dEps) return(eFC == eT32);
      else if (dWorst2 > dWorst3 + dEps) return(eFC == eT23);
      else                               return(false);
    }
  case eT22:
    {
      double adNormACE[3], adNormECB[3], adNormBCD[3], adNormDCA[3];
      double adNormABC[3], adNormCDE[3], adNormABE[3];
      vUnitNormal(adLocA, adLocC, adLocE, adNormACE);
      vUnitNormal(adLocE, adLocC, adLocB, adNormECB);
      vUnitNormal(adLocB, adLocC, adLocD, adNormBCD);
      vUnitNormal(adLocD, adLocC, adLocA, adNormDCA);

      vUnitNormal(adLocA, adLocB, adLocC, adNormABC);
      vUnitNormal(adLocC, adLocD, adLocE, adNormCDE);
      vUnitNormal(adLocA, adLocB, adLocE, adNormABE);

      double dLastDot, dWorstNow = -1., dWorstSwapped = -1.;

      dLastDot = dDOT3D(adNormACE, adNormECB);
      dWorstNow = MAX(dWorstNow, dLastDot);
      dLastDot = dDOT3D(adNormBCD, adNormDCA);
      dWorstNow = MAX(dWorstNow, dLastDot);
      dLastDot = fabs(dDOT3D(adNormABC, adNormABE));
      dWorstNow = MAX(dWorstNow, dLastDot);

      dLastDot = dDOT3D(adNormACE, adNormDCA);
      dWorstSwapped = MAX(dWorstSwapped, dLastDot);
      dLastDot = dDOT3D(adNormBCD, adNormECB);
      dWorstSwapped = MAX(dWorstSwapped, dLastDot);
      dLastDot = fabs(dDOT3D(adNormCDE, adNormABE));
      dWorstSwapped = MAX(dWorstSwapped, dLastDot);

      if   (dWorstNow > dWorstSwapped + dEps) return(true);
      else                                    return(false);
    }
  case eT44:
    return(false);
  default:
    // Bad face type in finding max dihedral angle
    assert(0);
    return(false);
  }
}

//@@ Determines whether swapping will improve the maximum face angle.
// Returns true to swap, false to remain the same.
bool TSTT_Swap::Swap3D_impl::qDoSwapMinSine
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3], const double adLocE[3])
{
  // This algorithm finds sine of face angles between adjacent faces by
  // crossing their unit normals.  The idea is to pick the configuration
  // which maximizes this value.
  //
  // To prevent pairs from flopping back and forth, a swap is only
  // authorized if the inequality is bigger than eps.

  double dEps = 1.e-10;
  switch (eFC) {
  case eT23:
  case eT32:
    {
      double adNormABD[3], adNormABE[3], adNormADE[3], adNormABC[3];
      double adNormBCD[3], adNormBCE[3], adNormBDE[3];
      double adNormCAD[3], adNormCAE[3], adNormCDE[3];
      //@@@ The case where the five pVVerts form a genuinely convex set
      vUnitNormal(adLocA, adLocB, adLocD, adNormABD);
      vUnitNormal(adLocA, adLocB, adLocE, adNormABE);
      vUnitNormal(adLocA, adLocD, adLocE, adNormADE);

      vUnitNormal(adLocA, adLocB, adLocC, adNormABC); // Points to E

      vUnitNormal(adLocB, adLocC, adLocD, adNormBCD);
      vUnitNormal(adLocB, adLocC, adLocE, adNormBCE);
      vUnitNormal(adLocB, adLocD, adLocE, adNormBDE);

      vUnitNormal(adLocC, adLocA, adLocD, adNormCAD);
      vUnitNormal(adLocC, adLocA, adLocE, adNormCAE);
      vUnitNormal(adLocC, adLocD, adLocE, adNormCDE);

      double dWorst2 = 1., dWorst3 = 1.;

      // Because I'm taking the MAGNITUDE of the cross product, the
      // actual sign of the result doesn't matter (vector can be
      // reversed).  This means that normal orientation doesn't matter.

      // Sines of dihedral angles of tet ABCD
      double adTemp[3], dMag;

      vCROSS3D(adNormABD, adNormBCD, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormBCD, adNormCAD, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormCAD, adNormABD, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormABD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormBCD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormCAD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      // Sines of dihedral angles of tet ABCE
      vCROSS3D(adNormABE, adNormBCE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormBCE, adNormCAE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormCAE, adNormABE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormABE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormBCE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      vCROSS3D(adNormCAE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = MIN(dWorst2, dMag);

      // Now have the smallest sine of dihedral angle in tet ABCD and ABCE

      //Sines of dihedral angles of tet ABDE
      vCROSS3D(adNormABD, adNormABE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormABD, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormABD, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormABE, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormABE, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormADE, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      //Sines of dihedral angles of tet BCDE
      vCROSS3D(adNormBCD, adNormBCE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormBCD, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormBCD, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormBCE, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormBCE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormBDE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      //Sines of dihedral angles of tet CADE
      vCROSS3D(adNormCAD, adNormCAE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormCAD, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormCAD, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormCAE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormCAE, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);

      vCROSS3D(adNormCDE, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = MIN(dWorst3, dMag);
      // Now have the smallest sine of dihedral in tets ABDE, BCDE, CADE

      if      (dWorst3 > dWorst2 + dEps) return(eFC == eT23);
      else if (dWorst2 > dWorst3 + dEps) return(eFC == eT32);
      else                               return(false);
    }
  case eT22:
    {
      double adNormACE[3], adNormECB[3], adNormBCD[3], adNormDCA[3];
      double adNormABC[3], adNormCDE[3], adNormABE[3];
      vUnitNormal(adLocA, adLocC, adLocE, adNormACE);
      vUnitNormal(adLocE, adLocC, adLocB, adNormECB);
      vUnitNormal(adLocB, adLocC, adLocD, adNormBCD);
      vUnitNormal(adLocD, adLocC, adLocA, adNormDCA);

      vUnitNormal(adLocA, adLocB, adLocC, adNormABC);
      vUnitNormal(adLocC, adLocD, adLocE, adNormCDE);
      vUnitNormal(adLocA, adLocB, adLocE, adNormABE);

      double dWorstNow = 1., dWorstSwap = 1.;
      double adTemp[3], dMag;

      // Sines of dihedral angles of tet ABCD, except those that are the
      // same both ways
      vCROSS3D(adNormBCD, adNormDCA, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);

      vCROSS3D(adNormBCD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);

      vCROSS3D(adNormDCA, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);


      // Sines of dihedral angles of tet ABCE, except those that are the
      // same both ways
      vCROSS3D(adNormECB, adNormACE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);

      vCROSS3D(adNormECB, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);

      vCROSS3D(adNormACE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);


      // Only check the center face once; the sine is the same on both
      // sides
      vCROSS3D(adNormABE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = MIN(dWorstNow, dMag);

      // Now have the smallest sine of dihedral angle in tet ABCD and ABCE

      // Sines of dihedral angles of tet DECA, except those that are the
      // same both ways
      vCROSS3D(adNormDCA, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);

      vCROSS3D(adNormACE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);

      vCROSS3D(adNormDCA, adNormACE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);


      // Sines of dihedral angles of tet DECB, except those that are the
      // same both ways
      vCROSS3D(adNormBCD, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);

      vCROSS3D(adNormECB, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);

      vCROSS3D(adNormBCD, adNormECB, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);


      // Only check the center face once; the sine is the same on both
      // sides
      vCROSS3D(adNormABE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = MIN(dWorstSwap, dMag);

      // Now have the smallest sine of dihedral angle in tet DECA and DECB

      if   (dWorstNow + dEps < dWorstSwap) return(true);
      else                                 return(false);
    }
  case eT44:
    return(false);
  default:
    // Bad face type in finding max dihedral angle
    assert(0);
    return(false);
  }
}

double TSTT_Swap::Swap3D_impl::dEvaluateQual(opaque oTet)
{
  assert(EntMesh.getEntTopo(oTet) == TSTTM::EntityTopology_TETRAHEDRON);
  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(4);
  int iNVerts;
  EntMesh.getEntAdj(oTet, TSTTM::EntityType_VERTEX, saoVerts, iNVerts);
  assert(iNVerts == 4);

  static sidl::array<double> sadCoords = sidl::array<double>::create1d(12);
  int iNCoords;
  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  EntMesh.getVtxArrCoords(saoVerts, iNVerts, SO, sadCoords, iNCoords);
  assert(iNCoords == 12);

  assert(QM_int._not_nil());
  return (QM_int.calcQuality(sadCoords, 12) *
	  ((QM_int.shouldMinimize()) ? -1 : 1));
}

double TSTT_Swap::Swap3D_impl::dEvaluateQual(opaque oVertA, opaque oVertB,
					      opaque oVertC, opaque oVertD)
{
  assert(EntMesh.getEntTopo(oVertA) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVertB) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVertC) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVertD) == TSTTM::EntityTopology_POINT);
  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(4);
  static opaque *aoVerts  = getArrayPointer(saoVerts);
  aoVerts[0] = oVertA;
  aoVerts[1] = oVertB;
  aoVerts[2] = oVertC;
  aoVerts[3] = oVertD;

  static sidl::array<double> sadCoords = sidl::array<double>::create1d(12);
  int iNCoords;
  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  EntMesh.getVtxArrCoords(saoVerts, 4, SO, sadCoords, iNCoords);
  assert(iNCoords == 12);

  assert(QM_int._not_nil());
  return (QM_int.calcQuality(sadCoords, 12) *
	  ((QM_int.shouldMinimize()) ? -1 : 1));
}

double TSTT_Swap::Swap3D_impl::dEvaluateQual
(const double adLocA[3], const double adLocB[3],
 const double adLocC[3], const double adLocD[3])
{
  static sidl::array<double> sadCoords = sidl::array<double>::create1d(12);
  static double *adAllCoords = getArrayPointer(sadCoords);

  adAllCoords[0] = adLocA[0];
  adAllCoords[1] = adLocA[1];
  adAllCoords[2] = adLocA[2];
  adAllCoords[3] = adLocB[0];
  adAllCoords[4] = adLocB[1];
  adAllCoords[5] = adLocB[2];
  adAllCoords[6] = adLocC[0];
  adAllCoords[7] = adLocC[1];
  adAllCoords[8] = adLocC[2];
  adAllCoords[9] = adLocD[0];
  adAllCoords[10] = adLocD[1];
  adAllCoords[11] = adLocD[2];
  
  assert(QM_int._not_nil());
  return (QM_int.calcQuality(sadCoords, 12) *
	  ((QM_int.shouldMinimize()) ? -1 : 1));
}

bool TSTT_Swap::Swap3D_impl::qDoSwapUser
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3], const double adLocE[3])
{
  double dEps = 1.e-10;
  switch (eFC) {
  case eT23:
    {
      // Old configuration
      double dQualABCD = dEvaluateQual(adLocA, adLocB, adLocC, adLocD);
      double dQualACBE = dEvaluateQual(adLocA, adLocC, adLocB, adLocE);

      // New configuration
      double dQualDEBA = dEvaluateQual(adLocD, adLocE, adLocB, adLocA);
      double dQualDECB = dEvaluateQual(adLocD, adLocE, adLocC, adLocB);
      double dQualDEAC = dEvaluateQual(adLocD, adLocE, adLocA, adLocC);

      return (MIN3(dQualDEBA, dQualDECB, dQualDEAC) >
	      dEps + MIN(dQualABCD, dQualACBE));
    }
  case eT32:
    {
      // New configuration
      double dQualABCD = dEvaluateQual(adLocA, adLocB, adLocC, adLocD);
      double dQualACBE = dEvaluateQual(adLocA, adLocC, adLocB, adLocE);

      // Old configuration
      double dQualDEBA = dEvaluateQual(adLocD, adLocE, adLocB, adLocA);
      double dQualDECB = dEvaluateQual(adLocD, adLocE, adLocC, adLocB);
      double dQualDEAC = dEvaluateQual(adLocD, adLocE, adLocA, adLocC);

      return (MIN(dQualABCD, dQualACBE) >
	      MIN3(dQualDEBA, dQualDECB, dQualDEAC) + dEps);
    }      
  case eT22:
    {
      // Old configuration
      double dQualABCD = dEvaluateQual(adLocA, adLocB, adLocC, adLocD);
      double dQualACBE = dEvaluateQual(adLocA, adLocC, adLocB, adLocE);

      // New configuration
      double dQualDECB = dEvaluateQual(adLocD, adLocE, adLocC, adLocB);
      double dQualDEAC = dEvaluateQual(adLocD, adLocE, adLocA, adLocC);

      return (MIN(dQualDECB, dQualDEAC) >
	      dEps + MIN(dQualABCD, dQualACBE));
    }
  case eT44:
    // Would also need the location of a vert F (on the other side of
    // the coplanar slab).  Since this isn't currently available, don't
    // swap this.
    return false;
  default:
    return false;
  }
}

bool TSTT_Swap::Swap3D_impl::qDoSwap
(opaque oVertA, opaque oVertB, opaque oVertC, opaque oVertD, opaque oVertE)
{
  assert(EntMesh.getEntType(oVertA) == TSTTM::EntityType_VERTEX);
  assert(EntMesh.getEntType(oVertB) == TSTTM::EntityType_VERTEX);
  assert(EntMesh.getEntType(oVertC) == TSTTM::EntityType_VERTEX);
  assert(EntMesh.getEntType(oVertD) == TSTTM::EntityType_VERTEX);
  assert(EntMesh.getEntType(oVertE) == TSTTM::EntityType_VERTEX);

  static sidl::array<double> sadCoords = sidl::array<double>::create1d(15);
  static double *adCoords = getArrayPointer(sadCoords);
  static double * const &adLocA = adCoords;
  static double * const &adLocB = adCoords+3;
  static double * const &adLocC = adCoords+6;
  static double * const &adLocD = adCoords+9;
  static double * const &adLocE = adCoords+12;
    
  static sidl::array<opaque> saoVerts = sidl::array<opaque>::create1d(5);
  static opaque *aoVerts  = getArrayPointer(saoVerts);
  aoVerts[0] = oVertA;
  aoVerts[1] = oVertB;
  aoVerts[2] = oVertC;
  aoVerts[3] = oVertD;
  aoVerts[4] = oVertE;
    
  TSTTM::StorageOrder SO = TSTTM::StorageOrder_INTERLEAVED;
  int iSize;
  EntMesh.getVtxArrCoords(saoVerts, 5, SO, sadCoords, iSize);
  assert(iSize == 15);

  switch (ST_int) {
  case TSTT_Swap::SwapType_DELAUNAY:
    return qDoSwapDelaunay(adLocA, adLocB, adLocC, adLocD, adLocE);
  case TSTT_Swap::SwapType_MIN_MAX_ANGLE:
    return qDoSwapMaxDihed(adLocA, adLocB, adLocC, adLocD, adLocE);
  case TSTT_Swap::SwapType_MAX_MIN_SINE:
    return qDoSwapMinSine(adLocA, adLocB, adLocC, adLocD, adLocE);
  case TSTT_Swap::SwapType_USER_DEFINED:
    return qDoSwapUser(adLocA, adLocB, adLocC, adLocD, adLocE);
  default:
    // Bad swap measure specified
    assert(0);
    return(false);
  }
}

//@@ Swap two tets for three.
int TSTT_Swap::Swap3D_impl::iReconfTet23
(opaque entity_set, opaque aoTets[], opaque oFace, opaque aoVerts[])
  throw(TSTTB::Error)
{
  // Identify both cells and all seven faces in the two-tet config.
  opaque & oTetD = aoTets[0];
  opaque & oTetE = aoTets[1];

  opaque oClassD = oNullOpaque, oClassE = oNullOpaque;
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetD, oClassD);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetE, oClassE);
#else
  Assoc.getEntEntAssociation(oTetD, oClassD);
  Assoc.getEntEntAssociation(oTetE, oClassE);
#endif
#endif
  if (oClassD != oClassE) return 0;
  
  opaque & oVertA = aoVerts[0];
  opaque & oVertB = aoVerts[1];
  opaque & oVertC = aoVerts[2];
  opaque & oVertD = aoVerts[3];
  opaque & oVertE = aoVerts[4];

  assert(getVertOpposite(oTetD, oFace) == oVertD);
  assert(getVertOpposite(oTetE, oFace) == oVertE);

  // Top half
#ifdef SLOW_BUT_SURE
  opaque oFaceBCE = getFaceOpposite(oTetE, oVertA);
  opaque oFaceCAE = getFaceOpposite(oTetE, oVertB);
  opaque oFaceABE = getFaceOpposite(oTetE, oVertC);
#else
  opaque oFaceBCE, oFaceCAE, oFaceABE;
  {
    opaque aoFaces[3], aoVertsTmp[] = {oVertA, oVertB, oVertC};
    getFacesOpposite(oTetE, aoVertsTmp, aoFaces);
    oFaceBCE = aoFaces[0];
    oFaceCAE = aoFaces[1];
    oFaceABE = aoFaces[2];
  }
#endif

  // Bottom half
#ifdef SLOW_BUT_SURE
  opaque oFaceCBD = getFaceOpposite(oTetD, oVertA);
  opaque oFaceACD = getFaceOpposite(oTetD, oVertB);
  opaque oFaceBAD = getFaceOpposite(oTetD, oVertC);
#else
  opaque oFaceCBD, oFaceACD, oFaceBAD;
  {
    opaque aoFaces[3], aoVertsTmp[] = {oVertA, oVertB, oVertC};
    getFacesOpposite(oTetD, aoVertsTmp, aoFaces);
    oFaceCBD = aoFaces[0];
    oFaceACD = aoFaces[1];
    oFaceBAD = aoFaces[2];
  }
#endif

  // To switch configurations, first delete the old tets (D and E) and
  // interior face (oFace).  Remove tets before faces.
  ModMesh.deleteEnt(oTetD);
  ModMesh.deleteEnt(oTetE);
  ModMesh.deleteEnt(oFace);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetD, oClassD);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetE, oClassD);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFace, oClassD);
#else
  Assoc.rmvEntEntAssociation(oTetD, oClassD);
  Assoc.rmvEntEntAssociation(oTetE, oClassD);
  Assoc.rmvEntEntAssociation(oFace, oClassD);
#endif
#endif

  soRecursionQueue.erase(oFace);

  // Now create three new faces (DEA, DEB, DEC).
  opaque oFaceDEA = createFace(oVertD, oVertE, oVertA);
  opaque oFaceDEB = createFace(oVertD, oVertE, oVertB);
  opaque oFaceDEC = createFace(oVertD, oVertE, oVertC);
  
  // Finally, create three new tets (DEAB, DEBC, DECA).
  opaque oTetDEAB = createTet(oFaceDEA, oFaceDEB, oFaceABE, oFaceBAD);
  opaque oTetDEBC = createTet(oFaceDEB, oFaceDEC, oFaceBCE, oFaceCBD);
  opaque oTetDECA = createTet(oFaceDEC, oFaceDEA, oFaceCAE, oFaceACD);

#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEA, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEB, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEC, oClassD);

  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDEAB, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDEBC, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDECA, oClassD);
#else
  Assoc.setEntEntAssociation(oFaceDEA, oClassD);
  Assoc.setEntEntAssociation(oFaceDEB, oClassD);
  Assoc.setEntEntAssociation(oFaceDEC, oClassD);

  Assoc.setEntEntAssociation(oTetDEAB, oClassD);
  Assoc.setEntEntAssociation(oTetDEBC, oClassD);
  Assoc.setEntEntAssociation(oTetDECA, oClassD);
#endif
#endif

  assert(iOrient3D(oVertA, oVertB, oVertD, oVertE) == 1);
  assert(iOrient3D(oVertB, oVertC, oVertD, oVertE) == 1);
  assert(iOrient3D(oVertC, oVertA, oVertD, oVertE) == 1);

  assert(qTetCheck(oTetDEAB));
  assert(qTetCheck(oTetDEBC));
  assert(qTetCheck(oTetDECA));

  assert(qFaceCheck(oFaceDEA));
  assert(qFaceCheck(oFaceDEB));
  assert(qFaceCheck(oFaceDEC));
  assert(qFaceCheck(oFaceABE));
  assert(qFaceCheck(oFaceBAD));
  assert(qFaceCheck(oFaceBCE));
  assert(qFaceCheck(oFaceCBD));
  assert(qFaceCheck(oFaceCAE));
  assert(qFaceCheck(oFaceACD));

  int iRetVal = 1;
  iNum23++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(oFaceBCE);
    soRecursionQueue.insert(oFaceCAE);
    soRecursionQueue.insert(oFaceABE);
    soRecursionQueue.insert(oFaceCBD);
    soRecursionQueue.insert(oFaceACD);
    soRecursionQueue.insert(oFaceBAD);
  }
  return(iRetVal);
}
//@@ Swap three tets for two.
int TSTT_Swap::Swap3D_impl::iReconfTet32
(opaque entity_set, opaque aoTets[], opaque oFace, opaque aoVerts[])
  throw(TSTTB::Error)
{
  // Identify the three cells of the three-tet config.  The cell ID's
  // require that the cell NOT containing oVertA is oTetA, etc.
  opaque &oTetA = aoTets[0]; // BCDE
  opaque &oTetB = aoTets[1]; // CADE
  opaque &oTetC = aoTets[2]; // ABDE

  opaque &oVertA = aoVerts[0];
  opaque &oVertB = aoVerts[1];
  opaque &oVertC = aoVerts[2];
  opaque &oVertD = aoVerts[3];
  opaque &oVertE = aoVerts[4];

  assert(! qHasVert(oTetA, oVertA));
  assert(! qHasVert(oTetB, oVertB));
  assert(! qHasVert(oTetC, oVertC));

#ifdef CLASSIFY
  opaque oClassA, oClassB, oClassC;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetA, oClassA);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetB, oClassB);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetC, oClassC);
#else
  Assoc.getEntEntAssociation(oTetA, oClassA);
  Assoc.getEntEntAssociation(oTetB, oClassB);
  Assoc.getEntEntAssociation(oTetC, oClassC);
#endif
  if (oClassA != oClassB || oClassA != oClassC) return 0;
#endif
  
  assert(qHasVert(oFace, oVertA) || qHasVert(oFace, oVertB)
	 || qHasVert(oFace, oVertC));
  assert(qHasVert(oFace, oVertD));
  assert(qHasVert(oFace, oVertE));

  // Identify the nine faces in the three-tet configuration. oFaceA and
  // oFaceA1 are faces in the shell not incident on oVertA. oFaceA2 IS
  // incident on oVertA, because that's the easiest way to define it
  // uniquely.
#ifdef SLOW_BUT_SURE
  opaque oFaceC  = getFaceOpposite( oTetC, oVertE ); // ABD
  opaque oFaceC1 = getFaceOpposite( oTetC, oVertD ); // EAB
  opaque oFaceA2 = getFaceOpposite( oTetC, oVertB ); // ADE

  opaque oFaceA  = getFaceOpposite( oTetA, oVertE ); // BCD
  opaque oFaceA1 = getFaceOpposite( oTetA, oVertD ); // EBC
  opaque oFaceB2 = getFaceOpposite( oTetA, oVertC ); // BDE

  opaque oFaceB  = getFaceOpposite( oTetB, oVertE ); // CAD
  opaque oFaceB1 = getFaceOpposite( oTetB, oVertD ); // ECA
  opaque oFaceC2 = getFaceOpposite( oTetB, oVertA ); // CDE
#else
  opaque oFaceA, oFaceA1, oFaceA2;
  opaque oFaceB, oFaceB1, oFaceB2;
  opaque oFaceC, oFaceC1, oFaceC2;

  {
    opaque aoFaces[3], aoVertsTmp[] = {oVertE, oVertD, oVertB};

    getFacesOpposite(oTetC, aoVertsTmp, aoFaces);
    oFaceC  = aoFaces[0];
    oFaceC1 = aoFaces[1];
    oFaceA2 = aoFaces[2];

    aoVertsTmp[2] = oVertC;
    getFacesOpposite(oTetA, aoVertsTmp, aoFaces);
    oFaceA  = aoFaces[0];
    oFaceA1 = aoFaces[1];
    oFaceB2 = aoFaces[2];

    aoVertsTmp[2] = oVertA;
    getFacesOpposite(oTetB, aoVertsTmp, aoFaces);
    oFaceB  = aoFaces[0];
    oFaceB1 = aoFaces[1];
    oFaceC2 = aoFaces[2];
  }    
#endif

  // Need to delete the three old interior faces (A2, B2, C2) and three
  // tets.
  ModMesh.deleteEnt(oTetA);
  ModMesh.deleteEnt(oTetB);
  ModMesh.deleteEnt(oTetC);

  ModMesh.deleteEnt(oFaceA2);
  ModMesh.deleteEnt(oFaceB2);
  ModMesh.deleteEnt(oFaceC2);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetA, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetB, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetC, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceA2, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceB2, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceC2, oClassA);
#else
  Assoc.rmvEntEntAssociation(oTetA, oClassA);
  Assoc.rmvEntEntAssociation(oTetB, oClassA);
  Assoc.rmvEntEntAssociation(oTetC, oClassA);
  Assoc.rmvEntEntAssociation(oFaceA2, oClassA);
  Assoc.rmvEntEntAssociation(oFaceB2, oClassA);
  Assoc.rmvEntEntAssociation(oFaceC2, oClassA);
#endif
#endif

  soRecursionQueue.erase(oFaceA2);
  soRecursionQueue.erase(oFaceB2);
  soRecursionQueue.erase(oFaceC2);

  // Now create a new face (ABC)
  opaque oFaceABC = createFace(oVertA, oVertB, oVertC);

  // Finally, create two new tets (ACBD, ABCE)
  opaque oTetABCD = createTet(oFaceABC, oFaceA, oFaceC, oFaceB);
  opaque oTetACBE = createTet(oFaceABC, oFaceA1, oFaceB1, oFaceC1);

#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceABC, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetABCD, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetACBE, oClassA);
#else
  Assoc.setEntEntAssociation(oFaceABC, oClassA);
  Assoc.setEntEntAssociation(oTetABCD, oClassA);
  Assoc.setEntEntAssociation(oTetACBE, oClassA);
#endif
#endif
  
  assert(qHasVert(oTetABCD, oVertD));
  assert(qHasVert(oTetACBE, oVertE));
  assert(iOrient3D(oVertA, oVertC, oVertB, oVertD) == 1);
  assert(iOrient3D(oVertA, oVertB, oVertC, oVertE) == 1);

  assert(qTetCheck(oTetABCD));
  assert(qTetCheck(oTetACBE));

  assert(qFaceCheck(oFaceA));
  assert(qFaceCheck(oFaceB));
  assert(qFaceCheck(oFaceC));
  assert(qFaceCheck(oFaceA1));
  assert(qFaceCheck(oFaceB1));
  assert(qFaceCheck(oFaceC1));
  assert(qFaceCheck(oFaceABC));

  int iRetVal = 1;
  iNum32 ++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(oFaceA);
    soRecursionQueue.insert(oFaceB);
    soRecursionQueue.insert(oFaceC);
    soRecursionQueue.insert(oFaceA1);
    soRecursionQueue.insert(oFaceB1);
    soRecursionQueue.insert(oFaceC1);
  }
  return(iRetVal);
}

//@@ Swap two tets for two, in the case where two faces are coplanar.
int TSTT_Swap::Swap3D_impl::iReconfTet22
(opaque entity_set, opaque aoTets[], opaque oFace, opaque aoVerts[])
  throw(TSTTB::Error)
{
  // This routine does only the 2-to-2 case.  4-to-4 is done elsewhere.
  // The cells and verts are assumed to have been set up properly already.

/*      	        C
        	       /\
        	      /||\
        	     / |  \                 A,D are on the base;
       	       	    /  | | \                B,E are front, C,F are back.
  		   /   |    \               Cell A is on the left, B on
        	  /    |  |  \	            the right.
        	 /     |      \
  		/      |   |   \
  	       /     C |     F  \
  	      /	       |  E |    \
  	     /   B     | _ - B    \
  	    /  _ _ - - |    /  - _ \
  	   D -         |           -E
  	    -__	    A  |  /  D  __--
  	       -__     |     __-
  	     	  -___ |/__--
  	     	      -A-

*/
  if (!qAllowBdryChanges) return 0;
  opaque & oTetA = aoTets[0]; // ACBD
  opaque & oTetB = aoTets[1]; // ABCE

  opaque &oVertA = aoVerts[0];
  opaque &oVertB = aoVerts[1];
  opaque &oVertC = aoVerts[2];
  opaque &oVertD = aoVerts[3];
  opaque &oVertE = aoVerts[4];

  assert(qHasVert(oTetA, oVertD));
  assert(qHasVert(oTetB, oVertE));

#ifdef CLASSIFY
  opaque oClassA, oClassB;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetA, oClassA);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetB, oClassB);
#else
  Assoc.getEntEntAssociation(oTetA, oClassA);
  Assoc.getEntEntAssociation(oTetB, oClassB);
#endif
  if (oClassA != oClassB) return 0;
#endif

#ifdef SLOW_BUT_SURE
  opaque oFaceA = getFaceOpposite(oTetA, oVertC);
  opaque oFaceB = getFaceOpposite(oTetA, oVertB);
  opaque oFaceC = getFaceOpposite(oTetA, oVertA);

  opaque oFaceD = getFaceOpposite(oTetB, oVertC);
  opaque oFaceE = getFaceOpposite(oTetB, oVertB);
  opaque oFaceF = getFaceOpposite(oTetB, oVertA);
#else
  opaque oFaceA, oFaceB, oFaceC, oFaceD, oFaceE, oFaceF;
  {
    opaque aoFaces[3], aoVertsTmp[] = {oVertC, oVertB, oVertA};

    getFacesOpposite(oTetA, aoVertsTmp, aoFaces);
    oFaceA = aoFaces[0];
    oFaceB = aoFaces[1];
    oFaceC = aoFaces[2];

    getFacesOpposite(oTetB, aoVertsTmp, aoFaces);
    oFaceD = aoFaces[0];
    oFaceE = aoFaces[1];
    oFaceF = aoFaces[2];
  }
#endif

  // Check that faces A and D are bdry faces.  Then, if they belong to
  // different patches, bail out, so that we won't end up losing a
  // subsegment (from A to B, in this case) when we swap.
#ifndef NDEBUG
  opaque oTetOppA = getCellOpposite(oFaceA, oTetA);
  opaque oTetOppD = getCellOpposite(oFaceD, oTetB);
  assert(oTetOppA == oNullOpaque && oTetOppD == oNullOpaque);
#endif
#ifdef CLASSIFY
  opaque oClassBdry,  oClassBdryD;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oFaceA, oClassBdry);
  Assoc.getEntEntAssociation(ModMesh, GModel, oFaceD, oClassBdryD);
#else
  Assoc.getEntEntAssociation(oFaceA, oClassBdry);
  Assoc.getEntEntAssociation(oFaceD, oClassBdryD);
#endif
  if (oClassBdry != oClassBdryD) return 0;
#endif

  // Need to delete the two tets, the dividing face, and the two
  // coplanar faces.
  ModMesh.deleteEnt(oTetA);
  ModMesh.deleteEnt(oTetB);
  ModMesh.deleteEnt(oFace);
  ModMesh.deleteEnt(oFaceA);
  ModMesh.deleteEnt(oFaceD);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetA, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetB, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFace, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceA, oClassBdry);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceD, oClassBdry);
#else
  Assoc.rmvEntEntAssociation(oTetA, oClassA);
  Assoc.rmvEntEntAssociation(oTetB, oClassA);
  Assoc.rmvEntEntAssociation(oFace, oClassA);
  Assoc.rmvEntEntAssociation(oFaceA, oClassBdry);
  Assoc.rmvEntEntAssociation(oFaceD, oClassBdry);
#endif
#endif

  soRecursionQueue.erase(oFace);
  soRecursionQueue.erase(oFaceA);
  soRecursionQueue.erase(oFaceD);

  // Create new faces (CDE, BDE, AED)
  opaque oFaceAED = createFace(oVertA, oVertE, oVertD);
  opaque oFaceBDE = createFace(oVertB, oVertD, oVertE);
  opaque oFaceCDE = createFace(oVertC, oVertD, oVertE);

  // Create two new tets
  opaque oTetCDEA = createTet(oFaceCDE, oFaceB, oFaceAED, oFaceE);
  opaque oTetCEDB = createTet(oFaceCDE, oFaceF, oFaceBDE, oFaceC);
  
#ifdef CLASSIFY
  // Classify the interior face (CDE) and the two tets as interior.
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceCDE, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetCDEA, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetCEDB, oClassA);
#else
  Assoc.setEntEntAssociation(oFaceCDE, oClassA);
  Assoc.setEntEntAssociation(oTetCDEA, oClassA);
  Assoc.setEntEntAssociation(oTetCEDB, oClassA);
#endif
  // Classify the new boundary faces (AED, BDE) on the bdry.
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceAED, oClassBdry);
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceBDE, oClassBdry);
#else
  Assoc.setEntEntAssociation(oFaceAED, oClassBdry);
  Assoc.setEntEntAssociation(oFaceBDE, oClassBdry);
#endif
#endif

  assert(qTetCheck(oTetCDEA));
  assert(qTetCheck(oTetCEDB));

  assert(qFaceCheck(oFaceAED));
  assert(qFaceCheck(oFaceBDE));
  assert(qFaceCheck(oFaceCDE));
  assert(qFaceCheck(oFaceB));
  assert(qFaceCheck(oFaceC));
  assert(qFaceCheck(oFaceE));
  assert(qFaceCheck(oFaceF));

  assert(getVertOpposite(oTetCEDB, oFaceCDE) == oVertB);
  assert(getVertOpposite(oTetCDEA, oFaceCDE) == oVertA);
  assert(iOrient3D(oVertC, oVertD, oVertE, oVertB) == -1);
  assert(iOrient3D(oVertC, oVertD, oVertE, oVertA) ==  1);

  int iRetVal = 1;
  iNum22 ++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(oFaceB);
    soRecursionQueue.insert(oFaceC);
    soRecursionQueue.insert(oFaceE);
    soRecursionQueue.insert(oFaceF);
  }
  return(iRetVal);
}

//@@ Swap four tets for four, in the case where two faces are coplanar.
int TSTT_Swap::Swap3D_impl::iReconfTet44
(opaque entity_set, opaque aoTets[], opaque oFace, opaque aoVerts[])
  throw(TSTTB::Error)
{
  // The cells and verts are assumed to have been set up properly already.

  // Here's the before picture:
/*      	        C
        	       /\
        	      /||\
        	     / |  \                 A,D are on the base;
       	       	    /  | | \                B,E are front, C,F are back.
  		   /   |    \               Cell A is on the left, B on
        	  /    |  |  \	            the right.
        	 /     |      \
  		/      |   |   \
  	       /     C |     F  \
  	      /	       |  E |    \
  	     /   B     | _ - B    \
  	    /  _ _ - - |    /  - _ \
  	   D -         |           -E
  	    -__	    A  |  /  D  __--
  	       -__     |     __-
  	     	  -___ |/__--
  	     	      -A-

  	               ___---B_
  	       ____----	    /  --__
  	   D---            /       -E
  	    - _	    A     / |D  _ -
  	    \  - _       /   _ -   /
             \     -_   / _-      /
  	      \       -A-  |     /
  	       \     C1|     F1 /
  		\      |  |    /
        	 \ B1  |  E1  /
        	  \    |     /	            A,D are on the base;
  		   \   | |  /               B1,E1 are front, C1,F1 are back.
       	       	    \  |   /                Cell A1 is on the left, B1 on
        	     \ || /                 the right.
        	      \| /                  Face G divides these two cells.
        	       \/
                       C1

*/
  // Identify both cells and all seven faces in the two-tet config.
  opaque & oTetA = aoTets[0];
  opaque & oTetB = aoTets[1];
  opaque & oTetA1 = aoTets[2];
  opaque & oTetB1 = aoTets[3];

#ifdef CLASSIFY
  opaque oClass;
  opaque &oClassA = oClass, oClassB, oClassA1, oClassB1;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetA, oClass);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetB, oClassB);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetA1, oClassA1);
  Assoc.getEntEntAssociation(ModMesh, GModel, oTetB1, oClassB1);
#else
  Assoc.getEntEntAssociation(oTetA, oClass);
  Assoc.getEntEntAssociation(oTetB, oClassB);
  Assoc.getEntEntAssociation(oTetA1, oClassA1);
  Assoc.getEntEntAssociation(oTetB1, oClassB1);
#endif
  if ((oClassA != oClassB) ||
      (oClassA != oClassA1) ||
      (oClassA != oClassB1))
    return 0;
#endif
  
  opaque & oVertA = aoVerts[0];
  opaque & oVertB = aoVerts[1];
  opaque & oVertC = aoVerts[2];
  opaque & oVertD = aoVerts[3];
  opaque & oVertE = aoVerts[4];
  opaque & oVertC1 = aoVerts[5];

  assert(qHasVert(oTetA, oVertD));
  assert(qHasVert(oTetB, oVertE));
  assert(qHasVert(oTetA1, oVertD));
  assert(qHasVert(oTetB1, oVertE));

#ifdef SLOW_BUT_SURE
  opaque oFaceA = getFaceOpposite( oTetA, oVertC);
  opaque oFaceB = getFaceOpposite( oTetA, oVertB);
  opaque oFaceC = getFaceOpposite( oTetA, oVertA);

  opaque oFaceD = getFaceOpposite( oTetB, oVertC);
  opaque oFaceE = getFaceOpposite( oTetB, oVertB);
  opaque oFaceF = getFaceOpposite( oTetB, oVertA);

  opaque oFaceB1 = getFaceOpposite( oTetA1, oVertB);
  opaque oFaceC1 = getFaceOpposite( oTetA1, oVertA);
  opaque oFaceG1 = getFaceOpposite( oTetA1, oVertD);

  opaque oFaceE1 = getFaceOpposite( oTetB1, oVertB);
  opaque oFaceF1 = getFaceOpposite( oTetB1, oVertA);
  assert(oFaceG1 == getFaceOpposite( oTetB1, oVertE));
#else
  opaque oFaceA, oFaceB, oFaceC, oFaceD, oFaceE, oFaceF;
  opaque oFaceB1, oFaceC1, oFaceG1, oFaceE1, oFaceF1;
  {
    opaque aoFaces[3], aoVertsTmp[] = {oVertC, oVertB, oVertA};

    getFacesOpposite(oTetA, aoVertsTmp, aoFaces);
    oFaceA = aoFaces[0];
    oFaceB = aoFaces[1];
    oFaceC = aoFaces[2];

    getFacesOpposite(oTetB, aoVertsTmp, aoFaces);
    oFaceD = aoFaces[0];
    oFaceE = aoFaces[1];
    oFaceF = aoFaces[2];

    aoVertsTmp[0] = oVertB;
    aoVertsTmp[1] = oVertA;
    aoVertsTmp[2] = oVertD;
    getFacesOpposite(oTetA1, aoVertsTmp, aoFaces);
    oFaceB1 = aoFaces[0];
    oFaceC1 = aoFaces[1];
    oFaceG1 = aoFaces[2];

    aoVertsTmp[2] = oVertE;
    getFacesOpposite(oTetB1, aoVertsTmp, aoFaces);
    oFaceE1 = aoFaces[0];
    oFaceF1 = aoFaces[1];
    assert(oFaceG1 == aoFaces[2]);    
  }
#endif
    

  assert(getVertOpposite(oTetB1, oFaceD) == oVertC1);

  // Must get rid of all four tets and all four interior faces.
  ModMesh.deleteEnt(oTetA);
  ModMesh.deleteEnt(oTetB);
  ModMesh.deleteEnt(oTetA1);
  ModMesh.deleteEnt(oTetB1);

  ModMesh.deleteEnt(oFaceA);
  ModMesh.deleteEnt(oFaceD);
  ModMesh.deleteEnt(oFace);
  ModMesh.deleteEnt(oFaceG1);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetA,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetB,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetA1, oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oTetB1, oClass);

  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceA,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceD,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFace,   oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, oFaceG1, oClass);
#else
  Assoc.rmvEntEntAssociation(oTetA,  oClass);
  Assoc.rmvEntEntAssociation(oTetB,  oClass);
  Assoc.rmvEntEntAssociation(oTetA1, oClass);
  Assoc.rmvEntEntAssociation(oTetB1, oClass);

  Assoc.rmvEntEntAssociation(oFaceA,  oClass);
  Assoc.rmvEntEntAssociation(oFaceD,  oClass);
  Assoc.rmvEntEntAssociation(oFace,   oClass);
  Assoc.rmvEntEntAssociation(oFaceG1, oClass);
#endif
#endif

  soRecursionQueue.erase(oFace);
  soRecursionQueue.erase(oFaceA);
  soRecursionQueue.erase(oFaceD);
  soRecursionQueue.erase(oFaceG1);

  // Now create four new faces: DEB, DEC, DEA, DEC1
  opaque oFaceDEB = createFace(oVertD, oVertE, oVertB);
  opaque oFaceDEC = createFace(oVertD, oVertE, oVertC);
  opaque oFaceDEA = createFace(oVertD, oVertE, oVertA);
  opaque oFaceDEC1 = createFace(oVertD, oVertE, oVertC1);

  // Finally, create four new tets: DEBC, DECA, DEAC1, DEC1B
  opaque oTetDEBC = createTet(oFaceDEB, oFaceDEC, oFaceF, oFaceC);
  opaque oTetDECA = createTet(oFaceDEC, oFaceDEA, oFaceE, oFaceB);
  opaque oTetDEAC1 = createTet(oFaceDEA, oFaceDEC1, oFaceE1, oFaceB1);
  opaque oTetDEC1B = createTet(oFaceDEC1, oFaceDEB, oFaceF1, oFaceC1);

#ifdef CLASSIFY
  // All of these get the same classification.
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEB, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEC, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEA, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, oFaceDEC1, oClass);
  
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDEBC, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDECA, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDEAC1, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, oTetDEC1B, oClass);
#else
  Assoc.setEntEntAssociation(oFaceDEB, oClass);
  Assoc.setEntEntAssociation(oFaceDEC, oClass);
  Assoc.setEntEntAssociation(oFaceDEA, oClass);
  Assoc.setEntEntAssociation(oFaceDEC1, oClass);
  
  Assoc.setEntEntAssociation(oTetDEBC, oClass);
  Assoc.setEntEntAssociation(oTetDECA, oClass);
  Assoc.setEntEntAssociation(oTetDEAC1, oClass);
  Assoc.setEntEntAssociation(oTetDEC1B, oClass);
#endif
#endif
  
  assert(qTetCheck(oTetDEBC));
  assert(qTetCheck(oTetDECA));
  assert(qTetCheck(oTetDEAC1));
  assert(qTetCheck(oTetDEC1B));

  assert(qFaceCheck(oFaceB));
  assert(qFaceCheck(oFaceC));
  assert(qFaceCheck(oFaceE));
  assert(qFaceCheck(oFaceF));

  assert(qFaceCheck(oFaceB1));
  assert(qFaceCheck(oFaceC1));
  assert(qFaceCheck(oFaceE1));
  assert(qFaceCheck(oFaceF1));

  assert(iOrient3D(oVertC, oVertD, oVertE, oVertB) == -1);
  assert(iOrient3D(oVertC, oVertD, oVertE, oVertA) ==  1);

  assert(iOrient3D(oVertC1, oVertD, oVertE, oVertB) ==  1);
  assert(iOrient3D(oVertC1, oVertD, oVertE, oVertA) == -1);

  int iRetVal = 1;
  iNum44 ++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(oFaceB);
    soRecursionQueue.insert(oFaceC);
    soRecursionQueue.insert(oFaceE);
    soRecursionQueue.insert(oFaceF);
    soRecursionQueue.insert(oFaceB1);
    soRecursionQueue.insert(oFaceC1);
    soRecursionQueue.insert(oFaceE1);
    soRecursionQueue.insert(oFaceF1);
  }
  return(iRetVal);
}

#ifndef NDEBUG
bool TSTT_Swap::Swap3D_impl::qTetCheck(opaque oTet)
{
  // Confirm the proper number of faces.
  opaque aoFaces[4];
  static sidl::array<opaque> saoFaces;
  int iLo = 0, iHi = 3, iStride = 1, iNFaces;
  saoFaces.borrow(aoFaces, 1, &iLo, &iHi, &iStride);

  EntMesh.getEntAdj(oTet, TSTTM::EntityType_FACE, saoFaces, iNFaces);

  if (iNFaces != 4) return false;

  // Can use block call here (block of 4)
  // Confirm reciprocal face connectivity.
  opaque aoRegions[2];
  static sidl::array<opaque> saoRegions;
  int iNRegs;
  iHi = 2;

  saoRegions.borrow(aoRegions, 1, &iLo, &iHi, &iStride);
  for (int i = 0; i < 4; i++) {
    EntMesh.getEntAdj(aoFaces[i], TSTTM::EntityType_REGION, saoRegions,
		      iNRegs);
    if (iNRegs > 2 || iNRegs < 1 ||
	(aoRegions[0] != oTet && aoRegions[1] != oTet))
      return false;
  }
  return true;
}

bool TSTT_Swap::Swap3D_impl::qFaceCheck(opaque oFace)
{
  // Must have either one or two regions incident.
  opaque aoRegs[2];
  static sidl::array<opaque> saoRegs;
  int iLo = 0, iHi = 1, iStride = 1, iNRegs;
  saoRegs.borrow(aoRegs, 1, &iLo, &iHi, &iStride);

  EntMesh.getEntAdj(oFace, TSTTM::EntityType_REGION, saoRegs, iNRegs);

  if (iNRegs < 1 || iNRegs > 2) return false;

  // Check reciprocal connectivity.
  opaque aoFaces[6];
  static sidl::array<opaque> saoFaces;
  int iNFaces;
  iHi = 5;

  saoFaces.borrow(aoFaces, 1, &iLo, &iHi, &iStride);

  bool qRetVal = false;
  for (int i = 0; i < iNRegs; i++) {
    EntMesh.getEntAdj(aoRegs[i], TSTTM::EntityType_FACE, saoFaces, iNFaces);
    if (iNFaces < 4 || iNFaces > 6) return false; // Bad region!
    for (int ii = 0; ii < iNFaces; ii++) {
      if (aoFaces[ii] == oFace) {
	qRetVal = true;
	break;
      }
    }
  }
  return qRetVal;
}
#endif

bool TSTT_Swap::Swap3D_impl::qHasVert(opaque oEntity, opaque oVert)
{
  // Slog through the verts and see if you find the one you're after.
  opaque aoVerts[8];
  static sidl::array<opaque> saoVerts;
  int iLo = 0, iHi = 8, iStride = 1, iNVerts;
  saoVerts.borrow(aoVerts, 1, &iLo, &iHi, &iStride);

  EntMesh.getEntAdj(oEntity, TSTTM::EntityType_VERTEX, saoVerts, iNVerts);
  assert(iNVerts >= 1 && iNVerts <= 8);

  for (int i = 0; i < iNVerts; i++) {
    if (aoVerts[i] == oVert) return true;
  }
  return false;
}

//@ Locally reconfigure tets incident on an edge to improve mesh quality
// The edge given by pVNorth, pVSouth is a candidate for removal.  If
// the edge doesn't lie on a boundary and a reconfiguration which
// improves mesh quality exists, then a swap is done, replacing 4 tets
// with 4, 5 with 6, 6 with 8, or 7 with 10.  Generally N tets are
// replaced by 2(N-2).  At this point, the largest transformation
// considered is 7 tets to 10.  The number of possible configurations
// for 8 to 12 swapping is somewhere around 150, so it's not
// considered, at least for now.
//
// After the edge swap is complete, all new faces are face swapped.
// The return value is the total number of face swaps done, with edge
// swaps counting as N-2 face swaps.


int TSTT_Swap::Swap3D_impl::iEdgeSwap3D
(void* entity_set, void* oFace, void* oVNorth, void* oVSouth, void* oVOther)
  throw(TSTTB::Error)
{
  //
  assert(qFaceCheck(oFace));
  assert(qHasVert(oFace, oVNorth));
  assert(qHasVert(oFace, oVSouth));
  assert(qHasVert(oFace, oVOther));
  // This call only needs to appear once, but that's okay, because
  // subsequent calls short-circuit and exit immediately.
  vInitCanonConfigs();

  const double dInvalidPenalty = -10000;

  static const int iMaxFaces = 10;
  opaque aoVOuter[iMaxFaces]; // Verts and tets
  opaque aoFNorth[iMaxFaces], aoFSouth[iMaxFaces], aoFOrig[iMaxFaces]; // Faces

  // Determine the size of the orbit around the pivot edge and list
  // all the verts, faces, and cells involved.
  aoFOrig[0] = oFace;
  aoVOuter[0] = oVOther;
#ifdef CLASSIFY
  opaque oClass;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oFace, oClass);
#else
  Assoc.getEntEntAssociation(oFace, oClass);
#endif
#endif

  static sidl::array<opaque> saoCells =
    sidl::array<opaque>::create1d(iMaxFaces);
  static opaque *aoCOrig = getArrayPointer(saoCells);

  int iNC;
  EntMesh.getEntAdj(oFace, TSTTM::EntityType_REGION, saoCells, iNC);
  // If there's only one of these, then we've got a bdry face in our hand.
  assert(iNC == 1 || iNC == 2);
  if (iNC == 1) {
    return iBdryEdgeSwap3D(entity_set, oFace, oVNorth, oVSouth, oVOther);
  }

  // aoCOrig now contains a cell incident on oFace.  Not necessarily
  // on the "left" side, so there might be orientation problems
  // later...

  assert(EntMesh.getEntTopo(aoCOrig[0]) == TSTTM::EntityTopology_TETRAHEDRON);
#ifdef CLASSIFY
  {
    opaque oClassTmp;
#ifdef OLD_TSTTR
    Assoc.getEntEntAssociation(ModMesh, GModel, aoCOrig[0], oClassTmp);
#else
    Assoc.getEntEntAssociation(aoCOrig[0], oClassTmp);
#endif
    if (oClassTmp != oClass) return 0;
  }
#endif
#ifdef SLOW_BUT_SURE
  aoFNorth[0] = getFaceOpposite(aoCOrig[0], oVSouth);
  aoFSouth[0] = getFaceOpposite(aoCOrig[0], oVNorth);
  opaque oFNew = getFaceOpposite(aoCOrig[0], aoVOuter[0]);
#else
  opaque oFNew;
  {
    opaque aoFaces[3], aoVerts[] = {oVSouth, oVNorth, aoVOuter[0]};
    getFacesOpposite(aoCOrig[0], aoVerts, aoFaces);
    aoFNorth[0] = aoFaces[0];
    aoFSouth[0] = aoFaces[1];
    oFNew = aoFaces[2];
  }
#endif
  int i = 1;
  while ((i < iMaxFaces) && (oFNew != oFace)) {
    aoFOrig[i] = oFNew;
    aoVOuter[i] = getVertOpposite(aoCOrig[i-1], aoFOrig[i-1]);
    aoCOrig[i] = getCellOpposite(oFNew, aoCOrig[i-1]);
    // Hit a boundary or non-tet cell.
    if (aoCOrig[i] == oNullOpaque) {
      return iBdryEdgeSwap3D(entity_set, oFNew, oVNorth, oVSouth, aoVOuter[i]);
    }
    assert(EntMesh.getEntTopo(aoCOrig[i]) ==
	   TSTTM::EntityTopology_TETRAHEDRON);
#ifdef CLASSIFY
    // Give up if more than one region is incident on the edge. 
    {
      opaque oClassTmp;
#ifdef OLD_TSTTR
      Assoc.getEntEntAssociation(ModMesh, GModel, aoCOrig[i], oClassTmp);
#else
      Assoc.getEntEntAssociation(aoCOrig[i], oClassTmp);
#endif
      if (oClassTmp != oClass) return 0;
    }
#endif

#ifdef SLOW_BUT_SURE
    aoFNorth[i] = getFaceOpposite(aoCOrig[i], oVSouth);
    aoFSouth[i] = getFaceOpposite(aoCOrig[i], oVNorth);
    oFNew = getFaceOpposite(aoCOrig[i], aoVOuter[i]);
#else
    {
      opaque aoFaces[3], aoVerts[] = {oVSouth, oVNorth, aoVOuter[i]};
      getFacesOpposite(aoCOrig[i], aoVerts, aoFaces);
      aoFNorth[i] = aoFaces[0];
      aoFSouth[i] = aoFaces[1];
      oFNew = aoFaces[2];
    }
#endif
    i++;
  }
  if (oFNew != oFace) return 0; // Ran out of space
  int iNOrig = i;
  aiEdgeReq[iNOrig]++;
  if (iNOrig > 7) {
    return 0;
  }

  // Compute the quality of the current configuration.  This is the
  // smallest quality for any of the existing cells.
  double dOrigQual = 1000;
    
  for (i = 0; i < iNOrig; i++) {
    // Evaluate quality, depending on the swap criterion in use.
    double dQual = dEvaluateQual(aoCOrig[i]);
    dOrigQual = MIN(dQual, dOrigQual);
  }
  
  // Set up info about the possible new configurations.

  // Compute quality for each possible tet pair (one pair per possible
  // triangle). 
  double a3dNewQual[iMaxFaces][iMaxFaces][iMaxFaces];
  int iSign = iOrient3D(aoVOuter[0], aoVOuter[1], oVSouth, oVNorth);
  int iV0, iV1, iV2;
#ifndef NDEBUG
  for (iV0 = 0; iV0 < iMaxFaces; iV0++)
    for (iV1 = 0; iV1 < iMaxFaces; iV1++)
      for (iV2 = 0; iV2 < iMaxFaces; iV2++)
	a3dNewQual[iV0][iV1][iV2] = 0;
  for (int iDum = 0; iDum < iNOrig; iDum++) 
    assert(iSign == iOrient3D(aoVOuter[iDum], aoVOuter[(iDum+1)%iNOrig],
			      oVSouth, oVNorth));
#endif
  for (iV0 = 0; iV0 < iNOrig; iV0++)
    for (iV1 = iV0+1; iV1 < iNOrig; iV1++)
      for (iV2 = iV1+1; iV2 < iNOrig; iV2++) {
        if (iOrient3D(aoVOuter[iV0], aoVOuter[iV1], aoVOuter[iV2],
		      oVNorth) == iSign) 
	  // Evaluate quality
	  a3dNewQual[iV0][iV1][iV2] = dEvaluateQual(aoVOuter[iV0],
						    aoVOuter[iV1],
						    aoVOuter[iV2],
						    oVNorth);
	else
          a3dNewQual[iV0][iV1][iV2] = dInvalidPenalty; 

	// Don't bother evaluating for the other tet if this one is
	// worse that the worst in the original config, because this tet
	// pair can never be in a final config anyway.
	if (a3dNewQual[iV0][iV1][iV2] >= dOrigQual) {
	  double dDummyQual;
	  if (iOrient3D(aoVOuter[iV0], aoVOuter[iV2], aoVOuter[iV1],
			oVSouth) == iSign)
	    dDummyQual = dEvaluateQual(aoVOuter[iV0], aoVOuter[iV2],
				       aoVOuter[iV1],  oVSouth);
	  else
	    dDummyQual = dInvalidPenalty;
	  a3dNewQual[iV0][iV1][iV2] = MIN(a3dNewQual[iV0][iV1][iV2],
					  dDummyQual);
	}

	// Copy things around symmetrically
	a3dNewQual[iV2][iV1][iV0] = a3dNewQual[iV2][iV0][iV1] =
	  a3dNewQual[iV1][iV2][iV0] = a3dNewQual[iV1][iV0][iV2] =
	  a3dNewQual[iV0][iV2][iV1] = a3dNewQual[iV0][iV1][iV2];
	
      }
  
  // Find the new configuration with the highest quality.
  static const double dEps = 1.e-10;
  double dMaxQual = dOrigQual;
  int iCanon, iPerm;
  int iBestCanon = -1, iBestPerm = -1;
  for (iCanon = 0; iCanon < aCS[iNOrig].iNCanon; iCanon++)
    for (iPerm = 0; iPerm < aCS[iNOrig].aConf[iCanon].iNPerm; iPerm++) {
      double dConfQual = 1000;
      // Once this config is known to be worse than the best one we know
      // about, we can give up on it.
      for (int iTri = 0;
	   iTri < iNOrig-2 && (dConfQual >= dMaxQual);
	   iTri++) {
        iV0 = (aCS[iNOrig].aConf[iCanon].a2iFaceVert[iTri][0] + iPerm)
	  % iNOrig;
        iV1 = (aCS[iNOrig].aConf[iCanon].a2iFaceVert[iTri][1] + iPerm)
	  % iNOrig;
        iV2 = (aCS[iNOrig].aConf[iCanon].a2iFaceVert[iTri][2] + iPerm)
	  % iNOrig;
	dConfQual = MIN(dConfQual, a3dNewQual[iV0][iV1][iV2]);
      }
      if (dConfQual > dMaxQual + dEps) {
	dMaxQual = dConfQual;
	iBestCanon = iCanon;
	iBestPerm = iPerm;
      }   
    }
  
  // If the highest-quality alternative is no better than the current
  // configuration, do nothing.  
  if (dOrigQual + dEps > dMaxQual) {
    return 0;
  }
  else {
    // If the highest-quality alternative is better than the current
    // configuration:
    aiEdgeDone[iNOrig]++;
    
    // Delete all of the interior tets and faces.  These must be
    // separate loops, because TSTTM won't delete a face that still has
    // incident regions, so regions must be done first.
    for (i = 0; i < iNOrig; i++) {
      ModMesh.deleteEnt(aoCOrig[i]);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aoCOrig[i], oClass);
#else
      Assoc.rmvEntEntAssociation(aoCOrig[i], oClass);
#endif
#endif
    }

    for (i = 0; i < iNOrig; i++) {
      ModMesh.deleteEnt(aoFOrig[i]);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aoFOrig[i], oClass);
#else
      Assoc.rmvEntEntAssociation(aoFOrig[i], oClass);
#endif
#endif
      soRecursionQueue.erase(aoFOrig[i]);
    }      

    // Renumber all the old face, vertex and cell data, offsetting it
    // by iBestPerm to make the desired configuration match the stored
    // canonical configuration.  
    if (iBestPerm != 0) {
      opaque aoVOuterTemp[iMaxFaces];
      opaque aoFNorthTemp[iMaxFaces], aoFSouthTemp[iMaxFaces];
      // Copy data to temporary arrays.
      for (i = 0; i < iNOrig; i++) {
        aoFNorthTemp[i] = aoFNorth[i];
        aoFSouthTemp[i] = aoFSouth[i];
        aoVOuterTemp[i] = aoVOuter[i];
      }
      // Copy back with an offset.
      for (i = 0; i < iNOrig; i++) {
        aoFNorth[i] = aoFNorthTemp[(i+iBestPerm)%iNOrig];
        aoFSouth[i] = aoFSouthTemp[(i+iBestPerm)%iNOrig];
        aoVOuter[i] = aoVOuterTemp[(i+iBestPerm)%iNOrig];
      }
    }

    opaque aoFNew[iNOrig*3 - 8];
    // Re-connect the mesh locally
    Config *pConf = &(aCS[iNOrig].aConf[iBestCanon]);
    // Assign data to new faces
    for (i = 0; i < 3*iNOrig-8; i++) {
      opaque oV0, oV1, oV2;
      iV0 = pConf->a2iFaceVert[i][0];
      switch (iV0) {
      case NORTH_POLE: oV0 = oVNorth; break;
      case SOUTH_POLE: oV0 = oVSouth; break;
      default:         oV0 = aoVOuter[iV0]; break;
      }
      iV1 = pConf->a2iFaceVert[i][1];
      switch (iV1) {
      case NORTH_POLE: oV1 = oVNorth; break;
      case SOUTH_POLE: oV1 = oVSouth; break;
      default:         oV1 = aoVOuter[iV1]; break;
      }
      iV2 = pConf->a2iFaceVert[i][2];
      switch (iV2) {
      case NORTH_POLE: oV2 = oVNorth; break;
      case SOUTH_POLE: oV2 = oVSouth; break;
      default:         oV2 = aoVOuter[iV2]; break;
      }
      try {
	aoFNew[i] = createFace(oV0, oV1, oV2);
      } catch (TSTTB::Error TErr) {
	TErr.echo("Creating face in edge swap loop.");
      }
    }

    opaque aoCNew[2*iNOrig-4];
    // Cell setup, including re-connection to outer faces
    for (i = 0; i < 2*iNOrig-4; i++) {
      opaque aoF[4];
      for (int ii = 0; ii < 4; ii++) {
	int iFace = pConf->a2iCellFace[i][ii];
	if (iFace >= SOUTH_FACE) {
	  aoF[ii] = aoFSouth[iFace - SOUTH_FACE];
	}
	else if (iFace >= NORTH_FACE) {
	  aoF[ii] = aoFNorth[iFace - NORTH_FACE];
	}
	else
	  aoF[ii] = aoFNew[iFace];
      }
      try {
	aoCNew[i] = createTet(aoF[0], aoF[1], aoF[2], aoF[3]);
      } catch (TSTTB::Error TErr) {
	TErr.echo("Creating tet in edge swap loop.");
      }
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.setEntEntAssociation(ModMesh, GModel, aoCNew[i], oClass);
#else
      Assoc.setEntEntAssociation(aoCNew[i], oClass);
#endif
#endif
    }

#ifdef CLASSIFY
    for (i = 0; i < 3*iNOrig-8; i++) {
      // Set face classification; some implementations will want tets
      // classified first.
#ifdef OLD_TSTTR
      Assoc.setEntEntAssociation(ModMesh, GModel, aoFNew[i], oClass);
#else
      Assoc.setEntEntAssociation(aoFNew[i], oClass);
#endif
    }
#endif

    // Validate the result
    for (i = 0; i < iNOrig*2 - 4; i++) {
      // Check everything about the tet
      assert(qTetCheck(aoCNew[i]));
      // Every tet has to hit one pole or the other
      assert(qHasVert(aoCNew[i], oVNorth) ||
	     qHasVert(aoCNew[i], oVSouth));
      // Assert tetrahedron orientation
      assert(iOrient3D(aoCNew[i]) == 1);
    }

    for (i = 0; i < 3*iNOrig - 8 ; i++) {
      assert(qFaceCheck(aoFNew[i]));
    }

    for (i = 0; i < iNOrig; i++) {    
      assert(qFaceCheck(aoFSouth[i]));
      assert(qFaceCheck(aoFNorth[i]));
    }

    // If recursion is permitted, check all the new faces for possible
    // swaps, as well as all of the faces on the outside of the orbit.
    int iRetVal = iNOrig-2;
    iNumEdge ++;
    if (qAllowRecursion) {
      for (i = 0; i < 3*iNOrig-8; i++) {
	soRecursionQueue.insert(aoFNew[i]);
      }
      for (i = 0; i < iNOrig; i++) {
	soRecursionQueue.insert(aoFNorth[i]);
	soRecursionQueue.insert(aoFSouth[i]);
      }
    }

    // Return the total (equivalent) number of swaps performed
    return iRetVal;
  }
}

//@ Locally reconfigure tets incident on a bdry edge to improve mesh quality
// The bdry edge given by oVNorth, oVSouth is a candidate for removal.
// pF is required to be a boundary face, and pVOther is the third vertex
// on that face.
// 
// If the bdry edge has two incident bdry faces that are coplanar and
// have the same boundary condition, and if a reconfiguration which
// improves mesh quality exists, then a swap is done, replacing 2 tets
// with 2, 3 with 4, ... or 6 with 10.  Generally N tets are replaced by
// 2(N-1).  At this point, the largest transformation considered is 6
// tets to 10.  The number of possible configurations for 7 to 12
// swapping is somewhere around 150, so it's not considered, at least
// for now. 
//
// All of these reconfigurations are close cousins to interior edge
// swaps.  A 2-to-2 boundary swap is just a 3-to-2 swap with one tet
// flattened into oblivion.  This means that all of the canonical
// configuration info from interior can be re-used here.
//
// After the edge swap is complete, all new interior faces are face
// swapped. The return value is the total number of face swaps done,
// with boundary edge swaps counting as N-3 face swaps. 
int TSTT_Swap::Swap3D_impl::iBdryEdgeSwap3D
(opaque entity_set, opaque oFace, opaque oVNorth, opaque oVSouth,
 opaque oVOther)
  throw(TSTTB::Error)
{
  assert(EntMesh.getEntTopo(oFace) == TSTTM::EntityTopology_TRIANGLE);
  assert(qFaceCheck(oFace));
  assert(qHasVert(oFace, oVNorth));
  assert(qHasVert(oFace, oVSouth));
  assert(qHasVert(oFace, oVOther));

#ifdef CLASSIFY
  // The face must be on the boundary.
  opaque oGeomFaceStart, oGeomFaceEnd;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oFace, oGeomFaceStart);
#else
  Assoc.getEntEntAssociation(oFace, oGeomFaceStart);
#endif
#endif

  // This call only needs to appear once, but that's okay, because
  // subsequent calls short-circuit and exit immediately.
  vInitCanonConfigs();
  if (!qAllowBdryChanges) return 0;  

  const double dInvalidPenalty = -10000;

  //@@ Identify all cells and faces incident on the bad edge
  static const int iMaxSize = 10;
  opaque aoVOuter[iMaxSize+1]; // Verts and tets
  opaque aoFNorth[iMaxSize+1], aoFSouth[iMaxSize+1], aoFOrig[iMaxSize+1]; // Faces

  static sidl::array<opaque> saoCells =
    sidl::array<opaque>::create1d(iMaxSize+1);
  static opaque *aoCOrig = getArrayPointer(saoCells);

  int iNC;
  EntMesh.getEntAdj(oFace, TSTTM::EntityType_REGION, saoCells, iNC);
  // If there's only one of these, then we've got a bdry face in our hand.
  assert(iNC == 1);

  // aoCOrig now contains a cell incident on oFace.  Not necessarily
  // on the "left" side, so there might be orientation problems
  // later...

  // Determine the size of the orbit around the pivot edge and list
  // all the verts, faces, and cells involved.
  aoFOrig[0] = oFace;
  aoVOuter[0] = oVOther;
  aoFNorth[0] = getFaceOpposite(aoCOrig[0], oVSouth);
  aoFSouth[0] = getFaceOpposite(aoCOrig[0], oVNorth);
  opaque oFaceNew = getFaceOpposite(aoCOrig[0], aoVOuter[0]);
#ifdef CLASSIFY
  opaque oClassInt;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, aoCOrig[0], oClassInt);
#else
  Assoc.getEntEntAssociation(aoCOrig[0], oClassInt);
#endif
#endif
  int i = 1;
  while (i <= iMaxSize) {
    aoFOrig[i] = oFaceNew;
    aoVOuter[i] = getVertOpposite(aoCOrig[i-1], aoFOrig[i-1]);
    aoCOrig[i] = getCellOpposite(oFaceNew, aoCOrig[i-1]);
    // Hit a boundary or non-tet cell.
    if (aoCOrig[i] == oNullOpaque) break;
    if (EntMesh.getEntTopo(aoCOrig[i]) !=
	TSTTM::EntityTopology_TETRAHEDRON)
      my_throw(NOT_SUPPORTED, "Can't swap non-tetrahedra.");
#ifdef CLASSIFY
    // Give up now if there is more than one region incident on the edge.
    {
      opaque oClassTmp;
#ifdef OLD_TSTTR
      Assoc.getEntEntAssociation(ModMesh, GModel, aoCOrig[i], oClassTmp);
#else
      Assoc.getEntEntAssociation(aoCOrig[i], oClassTmp);
#endif
      if (oClassTmp != oClassInt) return 0;
    }
#endif

    aoFNorth[i] = getFaceOpposite(aoCOrig[i], oVSouth);
    aoFSouth[i] = getFaceOpposite(aoCOrig[i], oVNorth);
    oFaceNew = getFaceOpposite(aoCOrig[i], aoVOuter[i]);
    i++;
  }
#ifdef CLASSIFY
  // Assign data for the second boundary face
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, oFaceNew, oGeomFaceEnd);
#else
  Assoc.getEntEntAssociation(oFaceNew, oGeomFaceEnd);
#endif
  if (oGeomFaceStart != oGeomFaceEnd) return 0;
#endif

  int iNOrigCells = i; 
  int iNOrigVerts = iNOrigCells + 1;
  aiBdryEdgeReq[iNOrigCells]++;

  // A set of easy exits for cases where surface edge swapping is not
  // legal.

  // Did we run out of space while identifying the entities around the
  // edge we'd like to swap?
  if (aoCOrig[i] != oNullOpaque) return 0;

  // Do we have more cells incident on the edge than we're prepared to
  // handle?
  if (iNOrigCells > 6) {
    // vMessage(4, "Too many tets around bdry edge: %d\n", iNOrigCells);
    return 0;
  }

#ifdef CLASSIFY
  // Do the boundary faces have the same classification?
  if (oGeomFaceStart != oGeomFaceEnd) 
    return 0;
#endif

  // Do the two boundary triangles form a convex quad?  If so, the
  // orientation of the tets formed by the post-swap triangles and any
  // non-coplanar point (i.e., any other member of aoVOuter) will be the
  // same.  This exit is redundant (no swapping would occur for this
  // case anyway), but saves a lot of quality computations.
  if (iOrient3D(oVOther, aoVOuter[iNOrigVerts-1], oVNorth, aoVOuter[1])
      !=
      iOrient3D(oVOther, oVSouth, aoVOuter[iNOrigVerts-1], aoVOuter[1])
      ) return 0;

  double dOrigQual;
  //@@ Compute the quality of the current configuration.
  // This is the smallest quality for any of the existing cells.
  dOrigQual = 1000;
  for (i = 0; i < iNOrigCells; i++) {
    // Evaluate quality, depending on the swap criterion in use.
    double dQual = dEvaluateQual(aoCOrig[i]);
    dOrigQual = MIN(dQual, dOrigQual);
  }
  
  // Set up info about the possible new configurations.

  // Compute quality for each possible tet pair (one pair per possible
  // triangle). 
  double a3dNewQual[iMaxSize][iMaxSize][iMaxSize];
  int iSign = iOrient3D(aoVOuter[0], aoVOuter[1], oVSouth, oVNorth);
  int iV0, iV1, iV2;
#ifndef NDEBUG
  for (iV0 = 0; iV0 < iMaxSize; iV0++)
    for (iV1 = 0; iV1 < iMaxSize; iV1++)
      for (iV2 = 0; iV2 < iMaxSize; iV2++)
	a3dNewQual[iV0][iV1][iV2] = 0;
  for (int iDum = 0; iDum < iNOrigCells; iDum++) 
    assert(iSign == iOrient3D(aoVOuter[iDum], aoVOuter[iDum+1],
			       oVSouth, oVNorth));
#endif
  for (iV0 = 0; iV0 < iNOrigVerts; iV0++)
    for (iV1 = iV0+1; iV1 < iNOrigVerts; iV1++)
      for (iV2 = iV1+1; iV2 < iNOrigVerts; iV2++) {
        if (iOrient3D(aoVOuter[iV0], aoVOuter[iV1], aoVOuter[iV2],
		      oVNorth) == iSign) 
	  // Evaluate quality
	  a3dNewQual[iV0][iV1][iV2] = dEvaluateQual(aoVOuter[iV0],
						    aoVOuter[iV1],
						    aoVOuter[iV2],
						    oVNorth);
	else
          a3dNewQual[iV0][iV1][iV2] = dInvalidPenalty; 

	// Don't bother evaluating for the other tet if this one is
	// worse that the worst in the original config, because this tet
	// pair can never be in a final config anyway.
	if (a3dNewQual[iV0][iV1][iV2] >= dOrigQual) {
	  double dDummyQual;
	  if (iOrient3D(aoVOuter[iV0], aoVOuter[iV2], aoVOuter[iV1],
			oVSouth) == iSign)
	    dDummyQual = dEvaluateQual(aoVOuter[iV0], aoVOuter[iV2],
				       aoVOuter[iV1],  oVSouth);
	  else
	    dDummyQual = dInvalidPenalty;
	  a3dNewQual[iV0][iV1][iV2] = MIN(a3dNewQual[iV0][iV1][iV2],
					  dDummyQual);
	}

	// Copy things around symmetrically
	a3dNewQual[iV2][iV1][iV0] = a3dNewQual[iV2][iV0][iV1] =
	  a3dNewQual[iV1][iV2][iV0] = a3dNewQual[iV1][iV0][iV2] =
	  a3dNewQual[iV0][iV2][iV1] = a3dNewQual[iV0][iV1][iV2];
	
      }
  
  // Find the new configuration with the highest quality.
  static const double dEps = 1.e-10;
  double dMaxQual = dOrigQual;
  int iCanon, iPerm;
  int iBestCanon = -1, iBestPerm = -1;
  for (iCanon = 0; iCanon < aCS[iNOrigVerts].iNCanon; iCanon++)
    for (iPerm = 0; iPerm < aCS[iNOrigVerts].aConf[iCanon].iNPerm; iPerm++) {
      double dConfQual = 1000;
      // Once this config is known to be worse than the best one we know
      // about, we can give up on it.
      for (int iTri = 0;
	   iTri < iNOrigVerts-2 && (dConfQual >= dMaxQual);
	   iTri++) {
        iV0 = (aCS[iNOrigVerts].aConf[iCanon].a2iFaceVert[iTri][0] + iPerm)
	  % iNOrigVerts;
        iV1 = (aCS[iNOrigVerts].aConf[iCanon].a2iFaceVert[iTri][1] + iPerm)
	  % iNOrigVerts;
        iV2 = (aCS[iNOrigVerts].aConf[iCanon].a2iFaceVert[iTri][2] + iPerm)
	  % iNOrigVerts;
	dConfQual = MIN(dConfQual, a3dNewQual[iV0][iV1][iV2]);
      }
      if (dConfQual > dMaxQual + dEps) {
	dMaxQual = dConfQual;
	iBestCanon = iCanon;
	iBestPerm = iPerm;
      }   
    }
  
  // If the highest-quality alternative is worse than the current
  // configuration, do nothing.  The check for iBestCanon is there so
  // that some stupid roundoff in comparison of dMaxQual and dOrigQual
  // won't cause a reconfiguration when there shouldn't be one.
  if (dMaxQual < dOrigQual + dEps) {
    return 0;
  }
  else {
    // If the highest-quality alternative is better than the current
    // configuration:
    aiBdryEdgeDone[iNOrigCells]++;

    // Delete all of the interior tets and faces.  These must be
    // separate loops, because TSTTM won't delete a face that still has
    // incident regions, so regions must be done first.  Because these
    // lists begin and end with faces that must be kept, (a) there's one
    // more face than there are tets and (b) both the first and last
    // faces must be kept.  Hence the odd-looking bounds for the loops
    // below. 
    for (i = 0; i < iNOrigCells; i++) {
      ModMesh.deleteEnt(aoCOrig[i]);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aoCOrig[i], oClassInt);
#else
      Assoc.rmvEntEntAssociation(aoCOrig[i], oClassInt);
#endif
#endif
    }
    for (i = 1; i < iNOrigCells; i++) {
      ModMesh.deleteEnt(aoFOrig[i]);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aoFOrig[i], oClassInt);
#else
      Assoc.rmvEntEntAssociation(aoFOrig[i], oClassInt);
#endif
#endif
      soRecursionQueue.erase(aoFOrig[i]);
    }      
    
    // Create a new North face and a new South face to hold the
    // reconfigured boundary faces.  Also, connect them to the existing
    // boundary data.

    aoFNorth[iNOrigVerts-1] = createFace(aoVOuter[0],
					 aoVOuter[iNOrigVerts-1],
					 oVNorth);
    aoFSouth[iNOrigVerts-1] = createFace(aoVOuter[iNOrigVerts-1],
					 aoVOuter[0],
					 oVSouth);

    // Renumber all the old face, vertex and cell data, offsetting it
    // by iBestPerm to make the desired configuration match the stored
    // canonical configuration.  
    if (iBestPerm != 0) {
      opaque aoFNorthTemp[iNOrigVerts], aoFSouthTemp[iNOrigVerts];
      opaque aoVOuterTemp[iNOrigVerts];
      // Copy data to temporary arrays.
      for (i = 0; i < iNOrigVerts; i++) {
        aoFNorthTemp[i] = aoFNorth[i];
        aoFSouthTemp[i] = aoFSouth[i];
        aoVOuterTemp[i] = aoVOuter[i];
      }
      // Copy back with an offset.
      for (i = 0; i < iNOrigVerts; i++) {
        aoFNorth[i] = aoFNorthTemp[(i+iBestPerm)%iNOrigVerts];
        aoFSouth[i] = aoFSouthTemp[(i+iBestPerm)%iNOrigVerts];
        aoVOuter[i] = aoVOuterTemp[(i+iBestPerm)%iNOrigVerts];
      }
    }

    // Re-connect the mesh locally
    Config *pConf = &(aCS[iNOrigVerts].aConf[iBestCanon]);
    opaque aoFNew[3*iNOrigVerts - 8];
    // Assign data to new faces
    for (i = 0; i < 3*iNOrigVerts-8; i++) {
      opaque oV0, oV1, oV2;
      iV0 = pConf->a2iFaceVert[i][0];
      switch (iV0) {
      case NORTH_POLE: oV0 = oVNorth; break;
      case SOUTH_POLE: oV0 = oVSouth; break;
      default:         oV0 = aoVOuter[iV0]; break;
      }
      iV1 = pConf->a2iFaceVert[i][1];
      switch (iV1) {
      case NORTH_POLE: oV1 = oVNorth; break;
      case SOUTH_POLE: oV1 = oVSouth; break;
      default:         oV1 = aoVOuter[iV1]; break;
      }
      iV2 = pConf->a2iFaceVert[i][2];
      switch (iV2) {
      case NORTH_POLE: oV2 = oVNorth; break;
      case SOUTH_POLE: oV2 = oVSouth; break;
      default:         oV2 = aoVOuter[iV2]; break;
      }

      aoFNew[i] = createFace(oV0, oV1, oV2);
    }

    // Cell setup, including re-connection to outer faces
    opaque aoCNew[2*iNOrigVerts - 4];
    for (i = 0; i < 2*iNOrigVerts-4; i++) {
      opaque aoF[4];
      for (int ii = 0; ii < 4; ii++) {
	int iFace = pConf->a2iCellFace[i][ii];
	if (iFace >= SOUTH_FACE) {
	  aoF[ii] = aoFSouth[iFace - SOUTH_FACE];
	}
	else if (iFace >= NORTH_FACE) {
	  aoF[ii] = aoFNorth[iFace - NORTH_FACE];
	}
	else
	  aoF[ii] = aoFNew[iFace];
      }
      aoCNew[i] = createTet(aoF[0], aoF[1], aoF[2], aoF[3]);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.setEntEntAssociation(ModMesh, GModel, aoCNew[i], oClassInt);
#else
      Assoc.setEntEntAssociation(aoCNew[i], oClassInt);
#endif
#endif
    }

#ifdef CLASSIFY
    // Now set face classifications
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, aoFNorth[iNOrigVerts-1], oGeomFaceStart);
    Assoc.setEntEntAssociation(ModMesh, GModel, aoFSouth[iNOrigVerts-1], oGeomFaceStart);
    for (i = 0; i < 3*iNOrigVerts-8; i++) {
      Assoc.setEntEntAssociation(ModMesh, GModel, aoFNew[i], oClassInt);
    }
#else
    Assoc.setEntEntAssociation(aoFNorth[iNOrigVerts-1], oGeomFaceStart);
    Assoc.setEntEntAssociation(aoFSouth[iNOrigVerts-1], oGeomFaceStart);
    for (i = 0; i < 3*iNOrigVerts-8; i++) {
      Assoc.setEntEntAssociation(aoFNew[i], oClassInt);
    }
#endif
#endif
    
    // Validate the result
    for (i = 0; i < iNOrigVerts*2 - 4; i++) {
      // Check everything about the tet
      assert(qTetCheck(aoCNew[i]));
      // Every tet has to hit one pole or the other
      assert(qHasVert(aoCNew[i], oVNorth) ||
	     qHasVert(aoCNew[i], oVSouth));
      // Assert tetrahedron orientation
      assert(iOrient3D(aoCNew[i]) == 1);
    }

    for (i = 0; i < 3*iNOrigVerts - 8 ; i++)
      assert(qFaceCheck(aoFNew[i]));

    for (i = 0; i < iNOrigVerts; i++) {
      assert(qFaceCheck(aoFSouth[i]));
      assert(qFaceCheck(aoFNorth[i]));
    }

    // If recursion is permitted, check all the new faces for possible
    // swaps, as well as all of the faces on the outside of the orbit.
    int iRetVal = iNOrigCells-1;
    iNumBdryEdge ++;
    if (qAllowRecursion) {
      for (i = 0; i < 3*iNOrigCells-8; i++) {
	soRecursionQueue.insert(aoFNew[i]);
      }
      for (i = 0; i < iNOrigCells; i++) {
	soRecursionQueue.insert(aoFNorth[i]);
	soRecursionQueue.insert(aoFSouth[i]);
      }
    }
    return iRetVal;
  }
}

opaque TSTT_Swap::Swap3D_impl::createFace(opaque oVertA, opaque oVertB,
					   opaque oVertC)
  throw(TSTTB::Error)
{
  assert(EntMesh.getEntTopo(oVertA) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVertB) == TSTTM::EntityTopology_POINT);
  assert(EntMesh.getEntTopo(oVertC) == TSTTM::EntityTopology_POINT);
  opaque aoVerts[] = {oVertA, oVertB, oVertC};
  static sidl::array<opaque> saoVerts;
  int iLo = 0, iHi = 2, iStride = 1;
  saoVerts.borrow(aoVerts, 1, &iLo, &iHi, &iStride);

  opaque oNewFace;
  TSTTM::CreationStatus CS;
  ModMesh.createEnt(TSTTM::EntityTopology_TRIANGLE, saoVerts, 3, oNewFace,
		    CS);
  // This is actually a testable post-condition for the TSTTM
  // implementation, so this assertion should never fire anyway. 
  assert(EntMesh.getEntTopo(oNewFace) == TSTTM::EntityTopology_TRIANGLE);
  if (CS == TSTTM::CreationStatus_CREATION_FAILED)
    my_throw(ENTITY_CREATION_ERROR,
	     "Couldn't create face!");

  return oNewFace;
}

opaque TSTT_Swap::Swap3D_impl::createTet(opaque oFaceA, opaque oFaceB,
					  opaque oFaceC, opaque oFaceD)
  throw(TSTTB::Error)
{
  assert(EntMesh.getEntTopo(oFaceA) == TSTTM::EntityTopology_TRIANGLE);
  assert(EntMesh.getEntTopo(oFaceB) == TSTTM::EntityTopology_TRIANGLE);
  assert(EntMesh.getEntTopo(oFaceC) == TSTTM::EntityTopology_TRIANGLE);
  assert(EntMesh.getEntTopo(oFaceD) == TSTTM::EntityTopology_TRIANGLE);
  opaque aoFaces[] = {oFaceA, oFaceB, oFaceC, oFaceD};
  sidl::array<opaque> saoFaces;
  int iLo = 0, iHi = 3, iStride = 1;
  saoFaces.borrow(aoFaces, 1, &iLo, &iHi, &iStride);

  opaque oNewTet;
  TSTTM::CreationStatus CS;
  ModMesh.createEnt(TSTTM::EntityTopology_TETRAHEDRON, saoFaces, 4, oNewTet,
		    CS);
  // This is actually a testable post-condition for the TSTTM
  // implementation, so this assertion should never fire anyway. 
  assert(EntMesh.getEntTopo(oNewTet) == TSTTM::EntityTopology_TETRAHEDRON);
//   cout << "Created tet with handle " << oNewTet
//        << "; status is " << CS << endl;
//   cout << "  Faces were: "  << oFaceA << " " << oFaceB << " "
//        << oFaceC << " " << oFaceD << endl << endl;
  if (CS == TSTTM::CreationStatus_CREATION_FAILED)
    my_throw(ENTITY_CREATION_ERROR,
	     "Couldn't create tet!");

  return oNewTet;
}

/**
 * Method:  setMeshData[]
 */
#ifdef NO_BABEL
void
TSTT_Swap::Swap3D_impl::setMeshData (
  /* in */ MESH_IMPLEMENTATION_CLASS pMesh_in )
throw ( 
  ::TSTTB::Error
){
  pMesh = pMesh_in;
}
#endif

#ifdef NO_BABEL_QUAL
/**
 * Method:  setUserQualMeasure[]
 */
void
TSTT_Swap::Swap3D_impl::setUserQualMeasure (
  /* in */ QUALITY_MEASURE pQM_in ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap3D.setUserQualMeasure)
  assert(pQM_in);
  pQM = pQM_in;
  setSwapType(SwapType_USER_DEFINED);
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D.setUserQualMeasure)
}
#endif

// DO-NOT-DELETE splicer.end(TSTT_Swap.Swap3D._misc)

