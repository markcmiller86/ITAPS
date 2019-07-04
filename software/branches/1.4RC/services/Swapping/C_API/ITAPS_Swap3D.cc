#include "ITAPS_Swap.hh"

using namespace ITAPS_Swap;

#include <stdlib.h>

#include <math.h>
#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;

#include "ITAPS_InitCanon.cc"

static void * const nullHandle = NULL;

#ifndef MIN3
#define MIN3(a, b, c) ((a) < (b) ? std::min((a), (c)) : std::min((b), (c)))
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

#define vNORMALIZE3D(a) do {double dInvMag = 1./dMAG3D(a);	\
    a[0]*=dInvMag; a[1]*=dInvMag; a[2]*=dInvMag;} while(0)

#define dDOT3D(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])

#define vCROSS3D(a,b,res) do {			\
    res[0] = a[1]*b[2] - a[2]*b[1];		\
    res[1] = a[2]*b[0] - a[0]*b[2];		\
    res[2] = a[0]*b[1] - a[1]*b[0];		\
  } while(0)

static int iNum23 = 0;
static int iNum32 = 0;
static int iNum44 = 0;
static int iNum22 = 0;
static int iNumEdge = 0;
static int iNumBdryEdge = 0;
static int aiEdgeReq[10], aiEdgeDone[10];
static int aiBdryEdgeReq[10], aiBdryEdgeDone[10];

static bool qConfirmTopo(iMesh_Instance inst, iBase_EntityHandle ent,
			 iMesh_EntityTopology topo)
{
  int err = iBase_SUCCESS;
  int iTopo;
  iMesh_getEntTopo(inst, ent, &iTopo, &err);
  CHECK_ERR(err, inst);
  return (topo == iTopo);
}

// This function exists to make it easy for configure scripts to
// identify the library.  No real point in ever calling the thing,
// obviously.
extern "C" {
  void swapStub() {}
};

/**
 * Method:  swap[]
 */
int
ITAPS_Swap::Swap3D::swap (/* inout */ iBase_EntitySetHandle entity_set,
			  /* inout */ iBase_EntityHandle entity_handle )
{
  int err = iBase_SUCCESS;
  if (entity_set != shRootSet) {
    shSet = entity_set;
    qUsingSet = true;
  }
  int iRetVal = swap(entity_handle);
  //
  qUsingSet = false;
  shSet = NULL;
  return iRetVal;
  // DO-NOT-DELETE splicer.end(ITAPS_Swap.Swap3D.swap)
}

/**
 * Method:  swap[]
 */
int
ITAPS_Swap::Swap3D::swap (/* inout */ iBase_EntityHandle entity_handle )
{
  int err = iBase_SUCCESS;

  int iTopoIn;
  iMesh_getEntTopo(pMeshInst, entity_handle, &iTopoIn, &err);
  CHECK_ERR(err, pMeshInst);
  // Can only swap triangles.
  if (iTopoIn != iMesh_TRIANGLE) throw(iBase_INVALID_ENTITY_HANDLE);

  //   cout << "Trying to swap face with handle " << entity_handle << endl;
  iBase_EntityHandle aehVerts[6], ehPivot0, ehPivot1, ehOther;
  iBase_EntityHandle aehTets[4];
  eFC = eCategorizeFace(entity_handle, aehVerts, aehTets,
			ehPivot0, ehPivot1, ehOther);

  int iRetVal = 0;
  //@@ Determine the preferable configuration and swap if necessary
  switch (eFC) {
    // These cases are handled by edge swapping.
  case eN44:
  case eN32:
    if (ST_int != ITAPS_Swap::SwapType_DELAUNAY && qAllowEdgeSwapping) {
      iRetVal = iEdgeSwap3D(entity_handle, ehPivot0, ehPivot1, ehOther);
    }
    break;
    // These cases are definitely unswappable
  case eT44:
    // In this case, you should swap if and only if -both- sides of the
    // plane benefit from the reconfiguration.
    if (qDoSwap(aehVerts[0], aehVerts[1], aehVerts[2], aehVerts[3], aehVerts[4]) &&
	qDoSwap(aehVerts[1], aehVerts[0], aehVerts[5], aehVerts[3], aehVerts[4])) {
      //      iRetVal = 0;
      //#warning Ignoring 44 swaps
       iRetVal = iReconfTet44(aehTets, entity_handle, aehVerts);
    }
    break;
  case eT22:
    if (qDoSwap(aehVerts[0], aehVerts[1], aehVerts[2], aehVerts[3], aehVerts[4])) {
//       iRetVal = 0;
// #warning Ignoring 22 swaps
      iRetVal = iReconfTet22(aehTets, entity_handle, aehVerts);
    }
    break;
  case eT23:
    if (qDoSwap(aehVerts[0], aehVerts[1], aehVerts[2], aehVerts[3], aehVerts[4])) {
//       iRetVal = 0;
// #warning Ignoring 23 swaps
      iRetVal = iReconfTet23(aehTets, entity_handle, aehVerts);
    }
    break;
  case eT32:
    if (qDoSwap(aehVerts[0], aehVerts[1], aehVerts[2], aehVerts[3], aehVerts[4])) {
//       iRetVal = 0;
// #warning Ignoring 32 swaps
      iRetVal = iReconfTet32(aehTets, entity_handle, aehVerts);
    }
    break;
  case eBdry:
  case eN40:
  case eN30:
  case eN20:
  case eBdryReflex:
  case eOther:
    break;
  default: // Catch-all for bad cases
    assert(0);
    break;
  }
  //
  return iRetVal;
  // DO-NOT-DELETE splicer.end(ITAPS_Swap.Swap3D.swap)
}

/**
 * Method:  swapAll[]
 */
int
ITAPS_Swap::Swap3D::swapAll (iBase_EntitySetHandle entity_set,
			     iBase_TagHandle* pTag)
{
  int err = iBase_SUCCESS;
  iNum23 = 0;
  iNum32 = 0;
  iNum44 = 0;
  iNum22 = 0;
  iNumEdge = 0;
  iNumBdryEdge = 0;
  {
    int ii;
    for (ii = 0 ; ii < 10 ; ii++) {
      aiEdgeReq[ii] = aiEdgeDone[ii] = aiBdryEdgeReq[ii] =
	aiBdryEdgeDone[ii] = 0;
    }
  }

  bool qUsingTag = false;
  if (pTag != NULL) {
    qUsingTag = true;
  }
    
  if (entity_set != shRootSet) {
    shSet = entity_set;
    qUsingSet = true;
  }
  iBase_EntityIterator iter;
  iMesh_initEntIter(pMeshInst, shRootSet, iBase_FACE,
		    iMesh_TRIANGLE, 1, &iter, &err);
  CHECK_ERR(err, pMeshInst);

  int iTotal = 0, iPasses = 0, iSwaps = 1, iMaxPasses = 15;

  if (qAllowRecursion) iMaxPasses = 5;
  while (iSwaps && iPasses < iMaxPasses) {
    soRecursionQueue.clear();
    ++iPasses;
    if (iMaxPasses > 1)
      cout << "   Pass " << iPasses << ": ";
    iSwaps = 0;
    // Iterate over all faces.
    iBase_EntityHandle entity_handle;
    int qMoreData;
    iMesh_getNextEntIter(pMeshInst, iter, &entity_handle, &qMoreData, &err);
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
	    std::set<iBase_EntityHandle>::iterator STL_iter = soRecursionQueue.begin();
	    entity_handle = *STL_iter;
	    soRecursionQueue.erase(STL_iter);
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
  //   iMesh_areEHValid(pMeshInst, true, &err);
  qUsingSet = false;
  shSet = NULL;
  return iTotal;
}


// DO-NOT-DELETE splicer.begin(ITAPS_Swap.Swap3D._misc)
int ITAPS_Swap::Swap3D::iOrient3D
(const double adA[3], const double adB[3],
 const double adC[3], const double adD[3])
// Computes the orientation of four verts in 3D.
{
  double dMaxX, dMinX, dMaxY, dMinY, dMaxZ, dMinZ;

  register double dXa = adA[0];
  register double dXb = adB[0];
  register double dXc = adC[0];
  register double dXd = adD[0];
  double dDX2 = dXb - dXa;
  double dDX3 = dXc - dXa;
  double dDX4 = dXd - dXa;
  MINMAX4(dXa, dXb, dXc, dXd, dMinX, dMaxX);

  register double dYa = adA[1];
  register double dYb = adB[1];
  register double dYc = adC[1];
  register double dYd = adD[1];
  double dDY2 = dYb - dYa;
  double dDY3 = dYc - dYa;
  double dDY4 = dYd - dYa;
  MINMAX4(dYa, dYb, dYc, dYd, dMinY, dMaxY);

  register double dZa = adA[2];
  register double dZb = adB[2];
  register double dZc = adC[2];
  register double dZd = adD[2];
  double dDZ2 = dZb - dZa;
  double dDZ3 = dZc - dZa;
  double dDZ4 = dZd - dZa;
  MINMAX4(dZa, dZb, dZc, dZd, dMinZ, dMaxZ);

  double dMax = std::max( std::max(dMaxX-dMinX, dMaxY-dMinY), dMaxZ-dMinZ);

  // dDet is proportional to the cell volume
  double dDet = (dDX2*(dDY3*dDZ4 - dDY4*dDZ3) +
		 dDX3*(dDY4*dDZ2 - dDY2*dDZ4) +
		 dDX4*(dDY2*dDZ3 - dDY3*dDZ2));

  //   // Compute a length scale based on edge lengths.
  //   double dScale = ( dDIST3D(adA, adB) + dDIST3D(adA, adC) +
  //		    dDIST3D(adA, adD) + dDIST3D(adB, adC) +
  //		    dDIST3D(adB, adD) + dDIST3D(adC, adD) ) / 6.;

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

int ITAPS_Swap::Swap3D::iOrient3D(iBase_EntityHandle ehTet)
{
  int err;
  assert(qConfirmTopo(pMeshInst, ehTet, iMesh_TETRAHEDRON));
  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[4];
  static int aehVerts_allocated = 4;

  int iNV;
  iMesh_getEntAdj(pMeshInst, ehTet, iBase_VERTEX, &aehVerts,
		  &aehVerts_allocated, &iNV, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iNV == 4);

  static double *adCoords = new double[12];
  static int adCoords_allocated = 12;

  // Must be an int so that we can pass it by address.
  int SO = iBase_INTERLEAVED;
  int iSize;
  iMesh_getVtxArrCoords(pMeshInst, aehVerts, 4, SO, &adCoords,
			&adCoords_allocated, &iSize, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iSize == 12);

  return iOrient3D(adCoords, adCoords+3, adCoords+6, adCoords+9);
}

int ITAPS_Swap::Swap3D::iOrient3D
(iBase_EntityHandle ehVert0, iBase_EntityHandle ehVert1,
 iBase_EntityHandle ehVert2, iBase_EntityHandle ehVert3)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehVert0, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVert1, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVert2, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVert3, iMesh_POINT));

  // Using a single call to getVtxArrCoords is much more efficient than
  // multiple calls to getVtxCoords.
  static double *adCoords = new double[12];
  int iCoords_allocated = 12, iCoords_size;

  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[4];
  int iVerts_allocated = 4, iVerts_size;
  aehVerts[0] = ehVert0;
  aehVerts[1] = ehVert1;
  aehVerts[2] = ehVert2;
  aehVerts[3] = ehVert3;

  int SO = iBase_INTERLEAVED;
  int iSize;
  iMesh_getVtxArrCoords(pMeshInst, aehVerts, 4, SO, &adCoords,
			&iCoords_allocated, &iCoords_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iCoords_size == 12);

  return iOrient3D(adCoords, adCoords+3, adCoords+6, adCoords+9);
}

double ITAPS_Swap::Swap3D::dDet4By4(const double a2dMat4[4][4]) const
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

int ITAPS_Swap::Swap3D::iInsphere
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

void ITAPS_Swap::Swap3D::vUnitNormal
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

int ITAPS_Swap::Swap3D::iGenOrient3D
(const iBase_EntityHandle ehVert0, const iBase_EntityHandle ehVert1, const iBase_EntityHandle ehVert2,
 const iBase_EntityHandle ehVert3, bool& qApproxFlat)
{
  int err = iBase_SUCCESS;
  // Using a single call to getVtxArrCoords is much more efficient than
  // multiple calls to getVtxCoords.
  static double *adCoords = new double[12];
  int iCoords_allocated = 12, iCoords_size;

  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[4];
  int iVerts_allocated = 4, iVerts_size;
  aehVerts[0] = ehVert0;
  aehVerts[1] = ehVert1;
  aehVerts[2] = ehVert2;
  aehVerts[3] = ehVert3;

  int SO = iBase_INTERLEAVED;
  iMesh_getVtxArrCoords(pMeshInst, aehVerts, 4, SO, &adCoords,
			&iCoords_allocated, &iCoords_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iCoords_size == 12);

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

iBase_EntityHandle ITAPS_Swap::Swap3D::getFaceOpposite
(const iBase_EntityHandle ehCell, const iBase_EntityHandle ehVert)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehCell, iMesh_TETRAHEDRON));
  assert(qConfirmTopo(pMeshInst, ehVert, iMesh_POINT));
  static iBase_EntityHandle *aehFaces = new iBase_EntityHandle[4];
  static int iFaces_allocated = 4, iFaces_size;

  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[4];
  static int iVerts_allocated = 4, iVerts_size;

  iMesh_getEntAdj(pMeshInst, ehCell, iBase_FACE, &aehFaces, &iFaces_allocated,
		  &iFaces_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iFaces_size == 4);

  // Maybe use block call here (block of 4)
  for (int iF = 0; iF < 4; iF++) {
    iMesh_getEntAdj(pMeshInst, aehFaces[iF], iBase_VERTEX, &aehVerts,
		    &iVerts_allocated, &iVerts_size, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iVerts_size == 3);
    if (aehVerts[0] != ehVert && aehVerts[1] != ehVert && aehVerts[2] != ehVert)
      return aehFaces[iF];
  }
  // Should never get here.
  assert(0);
  return reinterpret_cast<iBase_EntityHandle>(nullHandle);
}

void ITAPS_Swap::Swap3D::getFacesOpposite
(const iBase_EntityHandle ehCell, const iBase_EntityHandle aehVerts_In[3], iBase_EntityHandle aehFacesOut[3])
{
  int err;
  assert(qConfirmTopo(pMeshInst, ehCell, iMesh_TETRAHEDRON));
  assert(qConfirmTopo(pMeshInst, aehVerts_In[0], iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, aehVerts_In[1], iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, aehVerts_In[2], iMesh_POINT));
  static iBase_EntityHandle *aehFaces = new iBase_EntityHandle[4];
  static int iFaces_allocated = 4, iFaces_size;

  iMesh_getEntAdj(pMeshInst, ehCell, iBase_FACE, &aehFaces, &iFaces_allocated,
		  &iFaces_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iFaces_size == 4);

  aehFacesOut[0] = aehFacesOut[1] = aehFacesOut[2] =
    reinterpret_cast<iBase_EntityHandle>(nullHandle);

  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[12];
  static int iVerts_allocated = 12, iVerts_size;
  static int *aiOffset = new int[5];
  static int iOffset_allocated = 5, iOffset_size;

  iMesh_getEntArrAdj(pMeshInst, aehFaces, 4, iBase_VERTEX,
		     &aehVerts, &iVerts_allocated, &iVerts_size,
		     &aiOffset, &iOffset_allocated, &iOffset_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iVerts_size == 12);
  assert(iOffset_size == 5);

  for (int iF = 0; iF < 4; iF++) {
    for (int iV = 0; iV < 3; iV++) {
      if (aehVerts[0 + iF*3] != aehVerts_In[iV] &&
	  aehVerts[1 + iF*3] != aehVerts_In[iV] &&
	  aehVerts[2 + iF*3] != aehVerts_In[iV])
	aehFacesOut[iV] = aehFaces[iF];
    }
  }
  assert(aehFacesOut[0] != nullHandle);
  assert(aehFacesOut[1] != nullHandle);
  assert(aehFacesOut[2] != nullHandle);
}

iBase_EntityHandle ITAPS_Swap::Swap3D::getVertOpposite
(const iBase_EntityHandle ehCell, const iBase_EntityHandle ehFace)
{
  int err;
  assert(qConfirmTopo(pMeshInst, ehCell, iMesh_TETRAHEDRON));
  assert(qConfirmTopo(pMeshInst, ehFace, iMesh_TRIANGLE));
  static iBase_EntityHandle *aehCellVerts = new iBase_EntityHandle[4];
  static int iCellVerts_allocated = 4, iCellVerts_size;

  static iBase_EntityHandle *aehFaceVerts = new iBase_EntityHandle[3];
  static int iFaceVerts_allocated = 4, iFaceVerts_size;

  // Probably use block call here. (block of 2)
  int iSize;
  iMesh_getEntAdj(pMeshInst, ehCell, iBase_VERTEX, &aehCellVerts,
		  &iCellVerts_allocated, &iCellVerts_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iCellVerts_size == 4);

  iMesh_getEntAdj(pMeshInst, ehFace, iBase_VERTEX, &aehFaceVerts,
		  &iFaceVerts_allocated, &iFaceVerts_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iFaceVerts_size == 3);

  for (int iV = 0; iV < 4; iV++) {
    iBase_EntityHandle ehVert = aehCellVerts[iV];
    if (aehFaceVerts[0] != ehVert && aehFaceVerts[1] != ehVert &&
	aehFaceVerts[2] != ehVert)
      return ehVert;
  }
  // Should never get here.
  assert(0);
  return reinterpret_cast<iBase_EntityHandle>(nullHandle);
}

iBase_EntityHandle ITAPS_Swap::Swap3D::getCellOpposite
(const iBase_EntityHandle ehFace, const iBase_EntityHandle ehCell)
{
  int err = iBase_SUCCESS;
  {
    int iType;
    iMesh_getEntType(pMeshInst, ehCell, &iType, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iType == iBase_REGION);
    iMesh_getEntType(pMeshInst, ehFace, &iType, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iType == iBase_FACE);
  }
  static iBase_EntityHandle *aehCells = new iBase_EntityHandle[2];
  static int iCells_allocated = 2, iCells_size;

  iMesh_getEntAdj(pMeshInst, ehFace, iBase_REGION, &aehCells,
		  &iCells_allocated, &iCells_size, &err);
  CHECK_ERR(err, pMeshInst);
  if (iCells_size == 1) return reinterpret_cast<iBase_EntityHandle>(nullHandle);
  else {
    assert(iCells_size == 2);
  }

  if (aehCells[0] == ehCell)
    return aehCells[1];
  else {
    assert(aehCells[1] == ehCell);
    return aehCells[0];
  }
}

//@ Decide which type of triangular face this is, anyway.
ITAPS_Swap::Swap3D::eFaceCat ITAPS_Swap::Swap3D::eCategorizeFace
(iBase_EntityHandle ehFace, iBase_EntityHandle aehVerts[6], iBase_EntityHandle aehTets[4],
 iBase_EntityHandle& ehPivot0, iBase_EntityHandle& ehPivot1, iBase_EntityHandle& ehOther)
{
  int err = iBase_SUCCESS;
  // Make a general array of iBase_EntityHandles, for calls to connectivity.
  // This has a size of 8 because there a block call for face-tet
  // connectivity that needs that many.
  static iBase_EntityHandle *aehEntities = new iBase_EntityHandle[8];
  static int iEntities_allocated = 8, iSize;

  //@@ Case: one or both cells is not a tet, including boundaries.
  // Boundary triangles are lumped in with those that are between a
  // prism and a pyramid, for instance.
  iMesh_getEntAdj(pMeshInst, ehFace, iBase_REGION, &aehEntities,
		  &iEntities_allocated, &iSize, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iSize <= 2);
  if (iSize < 2 ||
      !qConfirmTopo(pMeshInst, aehEntities[0], iMesh_TETRAHEDRON) ||
      !qConfirmTopo(pMeshInst, aehEntities[1], iMesh_TETRAHEDRON))
    return (eOther);

  //@@ List all tets formed from five verts defining two tets sharing ehFace
  aehTets[0] = aehEntities[0];
  aehTets[1] = aehEntities[1];

  // Now use a block call for what was once three vertex connectivity
  // calls.  Create these arrays to handle a call that returns two cells
  // for each of eight faces.
  static iBase_EntityHandle *aehBigEnts = new iBase_EntityHandle[16];
  static int iBigEnts_allocated = 16, iBigEnts_size;

  static int *aiOffset = new int[9];
  static int iOffset_allocated = 9, iOffset_size;

  iMesh_getEntArrAdj(pMeshInst, aehEntities, 2, iBase_VERTEX,
                     &aehBigEnts, &iBigEnts_allocated, &iBigEnts_size,
                     &aiOffset, &iOffset_allocated, &iOffset_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iOffset_size == 3);
  assert(iBigEnts_size == 8);
  
  int iVerts_allocated = 5, iVerts_size;
  iMesh_getEntAdj(pMeshInst, ehFace, iBase_VERTEX, &aehVerts, 
                  &iVerts_allocated, &iVerts_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iVerts_size == 3);
  aehVerts[3] = aehVerts[4] = reinterpret_cast<iBase_EntityHandle>(nullHandle);

  for (int ii = 0; ii < 4; ii++) {
    if (aehBigEnts[ii] != aehVerts[0] &&
	aehBigEnts[ii] != aehVerts[1] &&
	aehBigEnts[ii] != aehVerts[2]) {
      aehVerts[3] = aehBigEnts[ii];
      break;
    }
  }

  for (int ii = 4; ii < 8; ii++) {
    assert(aehBigEnts[ii] != aehVerts[3]);
    if (aehBigEnts[ii] != aehVerts[0] &&
	aehBigEnts[ii] != aehVerts[1] &&
	aehBigEnts[ii] != aehVerts[2]) {
      aehVerts[4] = aehBigEnts[ii];
      break;
    }
  }
  assert(aehVerts[3] != nullHandle);
  assert(aehVerts[4] != nullHandle);

  iBase_EntityHandle& ehVertA = aehVerts[0];
  iBase_EntityHandle& ehVertB = aehVerts[1];
  iBase_EntityHandle& ehVertC = aehVerts[2];
  iBase_EntityHandle& ehVertD = aehVerts[3];
  iBase_EntityHandle& ehVertE = aehVerts[4];

  // Orientation check.  Basically, ABCE and ACBD need to both be
  // positively oriented, or we're in real trouble.  The most typical
  // trouble, of course, is that the tets are on opposite sides from
  // where I expected, in which case some switching is in order.
  int iOrientABCE = iOrient3D(ehVertA, ehVertB, ehVertC, ehVertE);
  if (iOrientABCE == -1) {
    iBase_EntityHandle ehTemp = ehVertD;
    ehVertD = ehVertE;
    ehVertE = ehTemp;

    ehTemp = aehTets[0];
    aehTets[0] = aehTets[1];
    aehTets[1] = ehTemp;
  }
  assert(iOrient3D(ehVertA, ehVertB, ehVertC, ehVertE) == 1);
  assert(iOrient3D(ehVertA, ehVertC, ehVertB, ehVertD) == 1);

  // Any add'l cell must be a neighbor of both of the first two.  So
  // make a list of all the tets that share a face with the first two
  // tets.  Awkward, but not difficult.
  int iNumTets = 2;
  {
    iBase_EntityHandle aehFaces[4];
    iBase_EntityHandle aehAdjTets0[4], aehAdjTets1[4];

    // First, do a tet->face call, using &aehEntities to hold the (output) faces.
    iMesh_getEntAdj(pMeshInst, aehTets[0], iBase_FACE,
		    &aehEntities, &iEntities_allocated, &iSize, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iSize == 4);

    // Now do a block face-tet call, using &aehEntities to hold the (input)
    // faces and &aehBigEnts to hold the (output) tets.
    iMesh_getEntArrAdj(pMeshInst, aehEntities, 4, iBase_REGION,
		       &aehBigEnts, &iBigEnts_allocated, &iBigEnts_size,
		       &aiOffset, &iOffset_allocated, &iOffset_size, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iBigEnts_size <= 8);
    assert(iOffset_size == 5);

    // Identify the adjacent tets
    for (int ii = 0; ii < 4; ii++) {
      int iOff = aiOffset[ii];
      if (aiOffset[ii+1] == iOff + 1) aehAdjTets0[ii] = reinterpret_cast<iBase_EntityHandle>(nullHandle);
      else {
	if (aehBigEnts[iOff] == aehTets[0]) {
	  aehAdjTets0[ii] = aehBigEnts[iOff + 1];
	}
	else {
	  assert(aehBigEnts[iOff + 1] == aehTets[0]);
	  aehAdjTets0[ii] = aehBigEnts[iOff];
	}
      }
    }

    // First, do a tet->face call, using &aehEntities to hold the (output) faces.
    iMesh_getEntAdj(pMeshInst, aehTets[1], iBase_FACE,
		    &aehEntities, &iEntities_allocated, &iSize, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iSize == 4);

    // Now do a block face-tet call, using &aehEntities to hold the (input)
    // faces and &aehBigEnts to hold the (output) tets.
    iMesh_getEntArrAdj(pMeshInst, aehEntities, 4, iBase_REGION,
		       &aehBigEnts, &iBigEnts_allocated, &iBigEnts_size,
		       &aiOffset, &iOffset_allocated, &iOffset_size, &err);
    CHECK_ERR(err, pMeshInst);
    assert(iBigEnts_size <= 8);
    assert(iOffset_size == 5);

    // Identify the adjacent tets
    for (int ii = 0; ii < 4; ii++) {
      int iOff = aiOffset[ii];
      if (aiOffset[ii+1] == iOff + 1) aehAdjTets1[ii] = reinterpret_cast<iBase_EntityHandle>(nullHandle);
      else {
	if (aehBigEnts[iOff] == aehTets[1]) {
	  aehAdjTets1[ii] = aehBigEnts[iOff + 1];
	}
	else {
	  assert(aehBigEnts[iOff + 1] == aehTets[1]);
	  aehAdjTets1[ii] = aehBigEnts[iOff];
	}
      }
    }

    // Could be zero, one, or two tets in common.
    for (int i0 = 0; i0 < 4; i0 ++) {
      iBase_EntityHandle ehTet0 = aehAdjTets0[i0];
      for (int i1 = 0; i1 < 4; i1 ++) {
	iBase_EntityHandle ehTet1 = aehAdjTets1[i1];
	if (ehTet0 == ehTet1 && ehTet0 != reinterpret_cast<iBase_EntityHandle>(nullHandle)) {
	  aehTets[iNumTets++] = ehTet0;
	}
      }
    }
    assert(iNumTets >= 2 && iNumTets <= 4);
    if (qUsingSet) {
      for (int ii = 0; ii < iNumTets; ii++) {
	if (!qIsEntContained(aehTets[ii])) return eOther;
      }
    }
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

      iOrientA = iGenOrient3D(ehVertB, ehVertC, ehVertD, ehVertE,
			      qApproxFlatA);
      if (iOrientA == -1) {
	ehPivot0 = ehVertB;
	ehPivot1 = ehVertC;
	ehOther  = ehVertA;
	qBadOrientation = true;
      }
      else {
	iOrientB = iGenOrient3D(ehVertC, ehVertA, ehVertD, ehVertE, qApproxFlatB);
	if (iOrientB == -1) {
	  ehPivot0 = ehVertC;
	  ehPivot1 = ehVertA;
	  ehOther  = ehVertB;
	  qBadOrientation = true;
	}
	if (iOrientA == 0 && iOrientB == 0) return eN20;
	if (!qBadOrientation) {
	  iOrientC = iGenOrient3D(ehVertA, ehVertB, ehVertD, ehVertE,
				  qApproxFlatC);
	  if (iOrientC == -1) {
	    ehPivot0 = ehVertA;
	    ehPivot1 = ehVertB;
	    ehOther  = ehVertC;
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
	  iBase_EntityHandle oTemp;
	  oTemp = ehVertA;
	  ehVertA = ehVertB;
	  ehVertB = ehVertC;
	  ehVertC = oTemp;
	}
	else if (iOrientB == 0) {
	  iBase_EntityHandle oTemp;
	  oTemp = ehVertA;
	  ehVertA = ehVertC;
	  ehVertC = ehVertB;
	  ehVertB = oTemp;
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
	iBase_EntityHandle ehTetA = aehTets[0], ehTetB = aehTets[1];

	// These are the faces that are coplanar
	iBase_EntityHandle ehFaceA = getFaceOpposite(ehTetA, ehVertC);
	iBase_EntityHandle ehFaceB = getFaceOpposite(ehTetB, ehVertC);
	iBase_EntityHandle ehOppCellA = getCellOpposite(ehFaceA, ehTetA);
	iBase_EntityHandle ehOppCellB = getCellOpposite(ehFaceB, ehTetB);

	if (ehOppCellA == reinterpret_cast<iBase_EntityHandle>(nullHandle) && ehOppCellB == reinterpret_cast<iBase_EntityHandle>(nullHandle)) {
	  // Both faces are on the boundary.  These are already -known-
	  // to be co-planar, so checking whether they classify onto the
	  // same geometric face is sufficient.
	  iBase_EntityHandle oClassA = reinterpret_cast<iBase_EntityHandle>(nullHandle), oClassB = reinterpret_cast<iBase_EntityHandle>(nullHandle);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
	  Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceA, oClassA);
	  Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceB, oClassB);
#else
	  Assoc.getEntEntAssociation(ehFaceA, oClassA);
	  Assoc.getEntEntAssociation(ehFaceB, oClassB);
#endif
#endif
	  if (oClassA == oClassB)
	    return eT22; // case 1
	  else
	    return eOther;  // case 2
	}
	else if (ehOppCellA == reinterpret_cast<iBase_EntityHandle>(nullHandle) || ehOppCellB == reinterpret_cast<iBase_EntityHandle>(nullHandle) ||
		 !qConfirmTopo(pMeshInst, ehOppCellA, iMesh_TETRAHEDRON) ||
		 !qConfirmTopo(pMeshInst, ehOppCellB, iMesh_TETRAHEDRON)) {
	  // Exactly one face on the boundary or internal faces with
	  // cells other than tets
	  return eOther; // cases 3,6
	}
	else {
	  // Both faces are internal to the mesh and separate pairs of tets
	  iBase_EntityHandle ehVertOppA = getVertOpposite(ehOppCellA, ehFaceA);
	  iBase_EntityHandle ehVertOppB = getVertOpposite(ehOppCellB, ehFaceB);
	  if (qUsingSet && !(qIsEntContained(ehOppCellA) &&
			     qIsEntContained(ehOppCellB))) {
	    return eOther; // Set violation
	  }
	  if (ehVertOppA == ehVertOppB) {
	    aehVerts[5] = ehVertOppA;
	    aehTets[2] = ehOppCellA;
	    aehTets[3] = ehOppCellB;
	    return eT44; // case 4
	  }
	  else {
	    ehPivot0 = ehVertA;
	    ehPivot1 = ehVertB;
	    ehOther  = ehVertC;
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
	      iBase_EntityHandle ehTemp;
	      ehTemp = ehVertA;
	      ehVertA = ehVertB;
	      ehVertB = ehVertC;
	      ehVertC = ehTemp;
	    }
	    else if (qApproxFlatB) {
	      iBase_EntityHandle ehTemp;
	      ehTemp = ehVertA;
	      ehVertA = ehVertC;
	      ehVertC = ehVertB;
	      ehVertB = ehTemp;
	    }
	    else if (qApproxFlatC) {}

	    iBase_EntityHandle ehTetA = aehTets[0], ehTetB = aehTets[1];

	    // These are the faces that are coplanar
	    iBase_EntityHandle ehFaceA = getFaceOpposite(ehTetA, ehVertC);
	    iBase_EntityHandle ehFaceB = getFaceOpposite(ehTetB, ehVertC);
	    iBase_EntityHandle ehOppCellA = getCellOpposite(ehFaceA, ehTetA);
	    iBase_EntityHandle ehOppCellB = getCellOpposite(ehFaceB, ehTetB);

	    if (ehOppCellA == reinterpret_cast<iBase_EntityHandle>(nullHandle) && ehOppCellB == reinterpret_cast<iBase_EntityHandle>(nullHandle)) {
	      // Both faces are on the boundary.  These are already -known-
	      // to be co-planar, so checking whether they classify onto the
	      // same geometric face is sufficient.
	      iBase_EntityHandle oClassA = reinterpret_cast<iBase_EntityHandle>(nullHandle), oClassB = reinterpret_cast<iBase_EntityHandle>(nullHandle);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
	      Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceA, oClassA);
	      Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceB, oClassB);
#else
	      Assoc.getEntEntAssociation(ehFaceA, oClassA);
	      Assoc.getEntEntAssociation(ehFaceB, oClassB);
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

	iBase_EntityHandle aehVertsTmp[] = {aehVerts[0], aehVerts[1], aehVerts[2],
					    aehVerts[3], aehVerts[4]};
	aehVerts[0] = aehVertsTmp[4];
	aehVerts[1] = aehVertsTmp[3];
	aehVerts[2] = aehVertsTmp[0];
	aehVerts[3] = aehVertsTmp[1];
	aehVerts[4] = aehVertsTmp[2];

	// Now a bit more shuffling so that aehTets[2] contains verts 0,
	// 1, 3, and 4.  Meanwhile, aehTets[0] will contain 1, 2, 3, 4,
	// and aehTets[1] will contain 0, 2, 3, 4.

	iMesh_getEntAdj(pMeshInst, aehTets[2], iBase_VERTEX,
			&aehEntities, &iEntities_allocated,
			&iSize, &err);
	CHECK_ERR(err, pMeshInst);
	assert(iSize == 4);
	if (aehEntities[0] != ehVertC && aehEntities[1] != ehVertC &&
	    aehEntities[2] != ehVertC && aehEntities[3] != ehVertC) {}
	else if (aehEntities[0] != ehVertE && aehEntities[1] != ehVertE &&
		 aehEntities[2] != ehVertE && aehEntities[3] != ehVertE) {
	  iBase_EntityHandle ehTemp = ehVertC;
	  ehVertC = ehVertE;
	  ehVertE = ehVertD;
	  ehVertD = ehTemp;
	}
	else {
	  iBase_EntityHandle ehTemp = ehVertC;
	  ehVertC = ehVertD;
	  ehVertD = ehVertE;
	  ehVertE = ehTemp;
	}
      }


      // The first of these returns will save a few orientation
      // primitive evaluations
      int iOrient1 = iOrient3D(ehVertA, ehVertC, ehVertB, ehVertD);
      if (iOrient1 == -1) return eOther;
      else if (iOrient1 == 0) return eN30;

      int iOrient2 = iOrient3D(ehVertA, ehVertB, ehVertC, ehVertE);
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
bool ITAPS_Swap::Swap3D::qDoSwapDelaunay
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
bool ITAPS_Swap::Swap3D::qDoSwapMaxDihed
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
      dWorst2 = std::max(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormBCD, adNormCAD);   /* Both in */
      dWorst2 = std::max(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormCAD, adNormABD);   /* Both in */
      dWorst2 = std::max(dWorst2, dLastDot);

      dLastDot = dDOT3D(adNormABE, adNormBCE);   /* Both out */
      dWorst2 = std::max(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormBCE, adNormCAE);   /* Both out */
      dWorst2 = std::max(dWorst2, dLastDot);
      dLastDot = dDOT3D(adNormCAE, adNormABE);   /* Both out */
      dWorst2 = std::max(dWorst2, dLastDot);

      dLastDot = - dDOT3D(adNormABD, adNormABE); /* One in, one out */
      dWorst3 = std::max(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormBCD, adNormBCE); /* One in, one out */
      dWorst3 = std::max(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormCAD, adNormCAE); /* One in, one out */
      dWorst3 = std::max(dWorst3, dLastDot);

      dLastDot = - dDOT3D(adNormADE, adNormBDE); /* One in, one out */
      dWorst3 = std::max(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormBDE, adNormCDE); /* One in, one out */
      dWorst3 = std::max(dWorst3, dLastDot);
      dLastDot = - dDOT3D(adNormCDE, adNormADE); /* One in, one out */
      dWorst3 = std::max(dWorst3, dLastDot);

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
      dWorstNow = std::max(dWorstNow, dLastDot);
      dLastDot = dDOT3D(adNormBCD, adNormDCA);
      dWorstNow = std::max(dWorstNow, dLastDot);
      dLastDot = fabs(dDOT3D(adNormABC, adNormABE));
      dWorstNow = std::max(dWorstNow, dLastDot);

      dLastDot = dDOT3D(adNormACE, adNormDCA);
      dWorstSwapped = std::max(dWorstSwapped, dLastDot);
      dLastDot = dDOT3D(adNormBCD, adNormECB);
      dWorstSwapped = std::max(dWorstSwapped, dLastDot);
      dLastDot = fabs(dDOT3D(adNormCDE, adNormABE));
      dWorstSwapped = std::max(dWorstSwapped, dLastDot);

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
bool ITAPS_Swap::Swap3D::qDoSwapMinSine
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
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormBCD, adNormCAD, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormCAD, adNormABD, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormABD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormBCD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormCAD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      // Sines of dihedral angles of tet ABCE
      vCROSS3D(adNormABE, adNormBCE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormBCE, adNormCAE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormCAE, adNormABE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormABE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormBCE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      vCROSS3D(adNormCAE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst2 = std::min(dWorst2, dMag);

      // Now have the smallest sine of dihedral angle in tet ABCD and ABCE

      //Sines of dihedral angles of tet ABDE
      vCROSS3D(adNormABD, adNormABE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormABD, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormABD, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormABE, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormABE, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormADE, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      //Sines of dihedral angles of tet BCDE
      vCROSS3D(adNormBCD, adNormBCE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormBCD, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormBCD, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormBCE, adNormBDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormBCE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormBDE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      //Sines of dihedral angles of tet CADE
      vCROSS3D(adNormCAD, adNormCAE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormCAD, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormCAD, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormCAE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormCAE, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);

      vCROSS3D(adNormCDE, adNormADE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorst3 = std::min(dWorst3, dMag);
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
      dWorstNow = std::min(dWorstNow, dMag);

      vCROSS3D(adNormBCD, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = std::min(dWorstNow, dMag);

      vCROSS3D(adNormDCA, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = std::min(dWorstNow, dMag);


      // Sines of dihedral angles of tet ABCE, except those that are the
      // same both ways
      vCROSS3D(adNormECB, adNormACE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = std::min(dWorstNow, dMag);

      vCROSS3D(adNormECB, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = std::min(dWorstNow, dMag);

      vCROSS3D(adNormACE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = std::min(dWorstNow, dMag);


      // Only check the center face once; the sine is the same on both
      // sides
      vCROSS3D(adNormABE, adNormABC, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstNow = std::min(dWorstNow, dMag);

      // Now have the smallest sine of dihedral angle in tet ABCD and ABCE

      // Sines of dihedral angles of tet DECA, except those that are the
      // same both ways
      vCROSS3D(adNormDCA, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);

      vCROSS3D(adNormACE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);

      vCROSS3D(adNormDCA, adNormACE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);


      // Sines of dihedral angles of tet DECB, except those that are the
      // same both ways
      vCROSS3D(adNormBCD, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);

      vCROSS3D(adNormECB, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);

      vCROSS3D(adNormBCD, adNormECB, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);


      // Only check the center face once; the sine is the same on both
      // sides
      vCROSS3D(adNormABE, adNormCDE, adTemp);
      dMag = dMAG3D(adTemp);
      dWorstSwap = std::min(dWorstSwap, dMag);

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

double ITAPS_Swap::Swap3D::dEvaluateQual(iBase_EntityHandle ehTet)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehTet, iMesh_TETRAHEDRON));

  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[4];
  static int iVerts_allocated = 4, iVerts_size;
  iMesh_getEntAdj(pMeshInst, ehTet, iBase_VERTEX,
		  &aehVerts, &iVerts_allocated, &iVerts_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iVerts_size == 4);

  static double *adCoords = new double[12];
  static int iCoords_allocated = 12, iCoords_size;
  int SO = iBase_INTERLEAVED;
  iMesh_getVtxArrCoords(pMeshInst, aehVerts, iVerts_size, SO,
			&adCoords, &iCoords_allocated, &iCoords_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iCoords_size == 12);

  assert(pQM_int);
  return (pQM_int->calcQuality(adCoords, 12) *
	  ((pQM_int->shouldMinimize()) ? -1 : 1));
}

double ITAPS_Swap::Swap3D::dEvaluateQual(iBase_EntityHandle ehVertA, iBase_EntityHandle ehVertB,
					 iBase_EntityHandle ehVertC, iBase_EntityHandle ehVertD)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehVertA, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertB, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertC, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertD, iMesh_POINT));
  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[4];
  aehVerts[0] = ehVertA;
  aehVerts[1] = ehVertB;
  aehVerts[2] = ehVertC;
  aehVerts[3] = ehVertD;

  static double *adCoords = new double[12];
  static int iCoords_allocated = 12, iCoords_size;
  int SO = iBase_INTERLEAVED;
  iMesh_getVtxArrCoords(pMeshInst, aehVerts, 4, SO,
			&adCoords, &iCoords_allocated, &iCoords_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iCoords_size == 12);

  assert(pQM_int);
  return (pQM_int->calcQuality(adCoords, 12) *
	  ((pQM_int->shouldMinimize()) ? -1 : 1));
}

double ITAPS_Swap::Swap3D::dEvaluateQual
(const double adLocA[3], const double adLocB[3],
 const double adLocC[3], const double adLocD[3])
{
  double adCoords[] = {adLocA[0], adLocA[1], adLocA[2],
		       adLocB[0], adLocB[1], adLocB[2],
		       adLocC[0], adLocC[1], adLocC[2],
		       adLocD[0], adLocD[1], adLocD[2]};

  assert(pQM_int);
  return (pQM_int->calcQuality(adCoords, 12) *
	  ((pQM_int->shouldMinimize()) ? -1 : 1));
}

bool ITAPS_Swap::Swap3D::qDoSwapExternal
(const double adLocA[3], const double adLocB[3], const double adLocC[3],
 const double adLocD[3], const double adLocE[3])
{
  double dEps = 1.e-10;
  switch (eFC) {
  case eT23:
    {
      // Old configuration
      double dQualACBD = dEvaluateQual(adLocA, adLocC, adLocB, adLocD);
      double dQualABCE = dEvaluateQual(adLocA, adLocB, adLocC, adLocE);

      // New configuration
      double dQualDEAB = dEvaluateQual(adLocD, adLocE, adLocA, adLocB);
      double dQualDEBC = dEvaluateQual(adLocD, adLocE, adLocB, adLocC);
      double dQualDECA = dEvaluateQual(adLocD, adLocE, adLocC, adLocA);

      return (MIN3(dQualDEAB, dQualDEBC, dQualDECA) >
	      dEps + std::min(dQualACBD, dQualABCE));
    }
  case eT32:
    {
      // New configuration
      double dQualACBD = dEvaluateQual(adLocA, adLocC, adLocB, adLocD);
      double dQualABCE = dEvaluateQual(adLocA, adLocB, adLocC, adLocE);

      // Old configuration
      double dQualDEAB = dEvaluateQual(adLocD, adLocE, adLocA, adLocB);
      double dQualDEBC = dEvaluateQual(adLocD, adLocE, adLocB, adLocC);
      double dQualDECA = dEvaluateQual(adLocD, adLocE, adLocC, adLocA);

      return (std::min(dQualACBD, dQualABCE) >
	      MIN3(dQualDEAB, dQualDEBC, dQualDECA) + dEps);
    }
  case eT22:
    {
      // Old configuration
      double dQualACBD = dEvaluateQual(adLocA, adLocC, adLocB, adLocD);
      double dQualABCE = dEvaluateQual(adLocA, adLocB, adLocC, adLocE);

      // New configuration
      double dQualDEBC = dEvaluateQual(adLocD, adLocE, adLocB, adLocC);
      double dQualDECA = dEvaluateQual(adLocD, adLocE, adLocC, adLocA);

      return (std::min(dQualDEBC, dQualDECA) >
	      dEps + std::min(dQualACBD, dQualABCE));
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

bool ITAPS_Swap::Swap3D::qDoSwap
(iBase_EntityHandle ehVertA, iBase_EntityHandle ehVertB, iBase_EntityHandle ehVertC, iBase_EntityHandle ehVertD, iBase_EntityHandle ehVertE)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehVertA, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertB, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertC, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertD, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertE, iMesh_POINT));
  static double *adCoords = new double[15];
  static int iCoords_allocated = 15, iCoords_size;
  static double * const &adLocA = adCoords;
  static double * const &adLocB = adCoords+3;
  static double * const &adLocC = adCoords+6;
  static double * const &adLocD = adCoords+9;
  static double * const &adLocE = adCoords+12;

  static iBase_EntityHandle *aehVerts  = new iBase_EntityHandle[5];
  aehVerts[0] = ehVertA;
  aehVerts[1] = ehVertB;
  aehVerts[2] = ehVertC;
  aehVerts[3] = ehVertD;
  aehVerts[4] = ehVertE;

  int SO = iBase_INTERLEAVED;
  int iSize;
  iMesh_getVtxArrCoords(pMeshInst, aehVerts, 5, SO,
			&adCoords, &iCoords_allocated, &iCoords_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iCoords_size == 15);

  switch (ST_int) {
  case ITAPS_Swap::SwapType_DELAUNAY:
    return qDoSwapDelaunay(adLocA, adLocB, adLocC, adLocD, adLocE);
  case ITAPS_Swap::SwapType_MIN_MAX_ANGLE:
    return qDoSwapMaxDihed(adLocA, adLocB, adLocC, adLocD, adLocE);
  case ITAPS_Swap::SwapType_MAX_MIN_SINE:
    return qDoSwapMinSine(adLocA, adLocB, adLocC, adLocD, adLocE);
  case ITAPS_Swap::SwapType_USER_DEFINED:
  case ITAPS_Swap::SwapType_MESQUITE:
    return qDoSwapExternal(adLocA, adLocB, adLocC, adLocD, adLocE);
  default:
    // Bad swap measure specified
    assert(0);
    return(false);
  }
}

//@@ Swap two tets for three.
int ITAPS_Swap::Swap3D::iReconfTet23
(iBase_EntityHandle aehTets[], iBase_EntityHandle ehFace, iBase_EntityHandle aehVerts[])
{
  int err = iBase_SUCCESS;
  // Identify both cells and all seven faces in the two-tet config.
  iBase_EntityHandle & ehTetD = aehTets[0];
  iBase_EntityHandle & ehTetE = aehTets[1];

  iBase_EntityHandle oClassD = reinterpret_cast<iBase_EntityHandle>(nullHandle), oClassE = reinterpret_cast<iBase_EntityHandle>(nullHandle);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetD, oClassD);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetE, oClassE);
#else
  Assoc.getEntEntAssociation(ehTetD, oClassD);
  Assoc.getEntEntAssociation(ehTetE, oClassE);
#endif
#endif
  if (oClassD != oClassE) return 0;

  iBase_EntityHandle & ehVertA = aehVerts[0];
  iBase_EntityHandle & ehVertB = aehVerts[1];
  iBase_EntityHandle & ehVertC = aehVerts[2];
  iBase_EntityHandle & ehVertD = aehVerts[3];
  iBase_EntityHandle & ehVertE = aehVerts[4];

  assert(getVertOpposite(ehTetD, ehFace) == ehVertD);
  assert(getVertOpposite(ehTetE, ehFace) == ehVertE);

  // Top half
  iBase_EntityHandle ehFaceBCE, ehFaceCAE, ehFaceABE;
  {
    iBase_EntityHandle aehFaces[3], aehVertsTmp[] = {ehVertA, ehVertB, ehVertC};
    getFacesOpposite(ehTetE, aehVertsTmp, aehFaces);
    ehFaceBCE = aehFaces[0];
    ehFaceCAE = aehFaces[1];
    ehFaceABE = aehFaces[2];
  }

  // Bottom half
  iBase_EntityHandle ehFaceCBD, ehFaceACD, ehFaceBAD;
  {
    iBase_EntityHandle aehFaces[3], aehVertsTmp[] = {ehVertA, ehVertB, ehVertC};
    getFacesOpposite(ehTetD, aehVertsTmp, aehFaces);
    ehFaceCBD = aehFaces[0];
    ehFaceACD = aehFaces[1];
    ehFaceBAD = aehFaces[2];
  }

  // To switch configurations, first delete the old tets (D and E) and
  // interior face (ehFace).  Remove tets before faces.
  iMesh_deleteEnt(pMeshInst, ehTetD, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetE, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFace, &err);
  CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetD, oClassD);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetE, oClassD);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFace, oClassD);
#else
  Assoc.rmvEntEntAssociation(ehTetD, oClassD);
  Assoc.rmvEntEntAssociation(ehTetE, oClassD);
  Assoc.rmvEntEntAssociation(ehFace, oClassD);
#endif
#endif

  soRecursionQueue.erase(ehFace);

  assert(iOrient3D(ehVertA, ehVertB, ehVertD, ehVertE) == 1);
  assert(iOrient3D(ehVertB, ehVertC, ehVertD, ehVertE) == 1);
  assert(iOrient3D(ehVertC, ehVertA, ehVertD, ehVertE) == 1);

  iBase_EntityHandle ehTetDEAB, ehTetDEBC, ehTetDECA;
  if (qCreateFromVerts) {
    ehTetDEAB = createTetFromVerts(ehVertD, ehVertE, ehVertA, ehVertB);
    ehTetDEBC = createTetFromVerts(ehVertD, ehVertE, ehVertB, ehVertC);
    ehTetDECA = createTetFromVerts(ehVertD, ehVertE, ehVertC, ehVertA);
  }
  else {
    // Now create three new faces (DEA, DEB, DEC).
    iBase_EntityHandle ehFaceDEA = createFace(ehVertD, ehVertE, ehVertA);
    iBase_EntityHandle ehFaceDEB = createFace(ehVertD, ehVertE, ehVertB);
    iBase_EntityHandle ehFaceDEC = createFace(ehVertD, ehVertE, ehVertC);
    
    // Finally, create three new tets (DEAB, DEBC, DECA).
    ehTetDEAB = createTet(ehFaceDEB, ehFaceABE, ehFaceBAD, ehFaceDEA);
    ehTetDEBC = createTet(ehFaceDEC, ehFaceBCE, ehFaceCBD, ehFaceDEB);
    ehTetDECA = createTet(ehFaceDEA, ehFaceCAE, ehFaceACD, ehFaceDEC);
    
    assert(qFaceCheck(ehFaceDEA));
    assert(qFaceCheck(ehFaceDEB));
    assert(qFaceCheck(ehFaceDEC));
    assert(qFaceCheck(ehFaceABE));
    assert(qFaceCheck(ehFaceBAD));
    assert(qFaceCheck(ehFaceBCE));
    assert(qFaceCheck(ehFaceCBD));
    assert(qFaceCheck(ehFaceCAE));
    assert(qFaceCheck(ehFaceACD));
  }

  assert(qTetCheck(ehTetDEAB));
  assert(qTetCheck(ehTetDEBC));
  assert(qTetCheck(ehTetDECA));

#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEA, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEB, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEC, oClassD);

  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDEAB, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDEBC, oClassD);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDECA, oClassD);
#else
  Assoc.setEntEntAssociation(ehFaceDEA, oClassD);
  Assoc.setEntEntAssociation(ehFaceDEB, oClassD);
  Assoc.setEntEntAssociation(ehFaceDEC, oClassD);

  Assoc.setEntEntAssociation(ehTetDEAB, oClassD);
  Assoc.setEntEntAssociation(ehTetDEBC, oClassD);
  Assoc.setEntEntAssociation(ehTetDECA, oClassD);
#endif
#endif

  int iRetVal = 1;
  iNum23++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(ehFaceBCE);
    soRecursionQueue.insert(ehFaceCAE);
    soRecursionQueue.insert(ehFaceABE);
    soRecursionQueue.insert(ehFaceCBD);
    soRecursionQueue.insert(ehFaceACD);
    soRecursionQueue.insert(ehFaceBAD);
  } // Recursing

  return(iRetVal);
}
//@@ Swap three tets for two.
int ITAPS_Swap::Swap3D::iReconfTet32
(iBase_EntityHandle aehTets[], iBase_EntityHandle ehFace, iBase_EntityHandle aehVerts[])
{
  int err = iBase_SUCCESS;
  // Identify the three cells of the three-tet config.  The cell ID's
  // require that the cell NOT containing ehVertA is ehTetA, etc.
  iBase_EntityHandle &ehTetA = aehTets[0]; // BCDE
  iBase_EntityHandle &ehTetB = aehTets[1]; // CADE
  iBase_EntityHandle &ehTetC = aehTets[2]; // ABDE

  iBase_EntityHandle &ehVertA = aehVerts[0];
  iBase_EntityHandle &ehVertB = aehVerts[1];
  iBase_EntityHandle &ehVertC = aehVerts[2];
  iBase_EntityHandle &ehVertD = aehVerts[3];
  iBase_EntityHandle &ehVertE = aehVerts[4];

  assert(! qHasVert(ehTetA, ehVertA));
  assert(! qHasVert(ehTetB, ehVertB));
  assert(! qHasVert(ehTetC, ehVertC));

#ifdef CLASSIFY
  iBase_EntityHandle oClassA, oClassB, oClassC;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetA, oClassA);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetB, oClassB);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetC, oClassC);
#else
  Assoc.getEntEntAssociation(ehTetA, oClassA);
  Assoc.getEntEntAssociation(ehTetB, oClassB);
  Assoc.getEntEntAssociation(ehTetC, oClassC);
#endif
  if (oClassA != oClassB || oClassA != oClassC) return 0;
#endif

  assert(qHasVert(ehFace, ehVertA) || qHasVert(ehFace, ehVertB)
	 || qHasVert(ehFace, ehVertC));
  assert(qHasVert(ehFace, ehVertD));
  assert(qHasVert(ehFace, ehVertE));

  // Identify the nine faces in the three-tet configuration. ehFaceA and
  // ehFaceA1 are faces in the shell not incident on ehVertA. ehFaceA2 IS
  // incident on ehVertA, because that's the easiest way to define it
  // uniquely.
#ifdef SLOW_BUT_SURE
  iBase_EntityHandle ehFaceC  = getFaceOpposite( ehTetC, ehVertE ); // ABD
  iBase_EntityHandle ehFaceC1 = getFaceOpposite( ehTetC, ehVertD ); // EAB
  iBase_EntityHandle ehFaceA2 = getFaceOpposite( ehTetC, ehVertB ); // ADE

  iBase_EntityHandle ehFaceA  = getFaceOpposite( ehTetA, ehVertE ); // BCD
  iBase_EntityHandle ehFaceA1 = getFaceOpposite( ehTetA, ehVertD ); // EBC
  iBase_EntityHandle ehFaceB2 = getFaceOpposite( ehTetA, ehVertC ); // BDE

  iBase_EntityHandle ehFaceB  = getFaceOpposite( ehTetB, ehVertE ); // CAD
  iBase_EntityHandle ehFaceB1 = getFaceOpposite( ehTetB, ehVertD ); // ECA
  iBase_EntityHandle ehFaceC2 = getFaceOpposite( ehTetB, ehVertA ); // CDE
#else
  iBase_EntityHandle ehFaceA, ehFaceA1, ehFaceA2;
  iBase_EntityHandle ehFaceB, ehFaceB1, ehFaceB2;
  iBase_EntityHandle ehFaceC, ehFaceC1, ehFaceC2;

  {
    iBase_EntityHandle aehFaces[3], aehVertsTmp[] = {ehVertE, ehVertD, ehVertB};

    getFacesOpposite(ehTetC, aehVertsTmp, aehFaces);
    ehFaceC  = aehFaces[0];
    ehFaceC1 = aehFaces[1];
    ehFaceA2 = aehFaces[2];

    aehVertsTmp[2] = ehVertC;
    getFacesOpposite(ehTetA, aehVertsTmp, aehFaces);
    ehFaceA  = aehFaces[0];
    ehFaceA1 = aehFaces[1];
    ehFaceB2 = aehFaces[2];

    aehVertsTmp[2] = ehVertA;
    getFacesOpposite(ehTetB, aehVertsTmp, aehFaces);
    ehFaceB  = aehFaces[0];
    ehFaceB1 = aehFaces[1];
    ehFaceC2 = aehFaces[2];
  }
#endif

  // Need to delete the three old interior faces (A2, B2, C2) and three
  // tets.
  iMesh_deleteEnt(pMeshInst, ehTetA, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetB, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetC, &err);
  CHECK_ERR(err, pMeshInst);

  iMesh_deleteEnt(pMeshInst, ehFaceA2, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFaceB2, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFaceC2, &err);
  CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetA, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetB, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetC, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceA2, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceB2, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceC2, oClassA);
#else
  Assoc.rmvEntEntAssociation(ehTetA, oClassA);
  Assoc.rmvEntEntAssociation(ehTetB, oClassA);
  Assoc.rmvEntEntAssociation(ehTetC, oClassA);
  Assoc.rmvEntEntAssociation(ehFaceA2, oClassA);
  Assoc.rmvEntEntAssociation(ehFaceB2, oClassA);
  Assoc.rmvEntEntAssociation(ehFaceC2, oClassA);
#endif
#endif

  soRecursionQueue.erase(ehFaceA2);
  soRecursionQueue.erase(ehFaceB2);
  soRecursionQueue.erase(ehFaceC2);

  iBase_EntityHandle ehTetACBD, ehTetABCE;
  if (qCreateFromVerts) {
    ehTetACBD = createTetFromVerts(ehVertA, ehVertC, ehVertB, ehVertD);
    ehTetABCE = createTetFromVerts(ehVertA, ehVertB, ehVertC, ehVertE);
  }
  else {
    // Now create a new face (ABC)
    iBase_EntityHandle ehFaceABC = createFace(ehVertA, ehVertB, ehVertC);

    // Finally, create two new tets (ACBD, ABCE)
    // FaceA = BCD  FaceA1 = EBC
    // FaceB = CAD  FaceB1 = ECA
    // FaceC = ABD  FaceC1 = EAB
    ehTetACBD = createTet(ehFaceB, ehFaceA, ehFaceC, ehFaceABC);
    ehTetABCE = createTet(ehFaceC1, ehFaceA1, ehFaceB1, ehFaceABC);
    assert(qFaceCheck(ehFaceABC));
  }

#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceABC, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetACBD, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetABCE, oClassA);
#else
  Assoc.setEntEntAssociation(ehFaceABC, oClassA);
  Assoc.setEntEntAssociation(ehTetACBD, oClassA);
  Assoc.setEntEntAssociation(ehTetABCE, oClassA);
#endif
#endif

  assert(qHasVert(ehTetACBD, ehVertD));
  assert(qHasVert(ehTetABCE, ehVertE));
  assert(iOrient3D(ehVertA, ehVertC, ehVertB, ehVertD) == 1);
  assert(iOrient3D(ehVertA, ehVertB, ehVertC, ehVertE) == 1);

  assert(qTetCheck(ehTetACBD));
  assert(qTetCheck(ehTetABCE));

  assert(qFaceCheck(ehFaceA));
  assert(qFaceCheck(ehFaceB));
  assert(qFaceCheck(ehFaceC));
  assert(qFaceCheck(ehFaceA1));
  assert(qFaceCheck(ehFaceB1));
  assert(qFaceCheck(ehFaceC1));

  int iRetVal = 1;
  iNum32 ++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(ehFaceA);
    soRecursionQueue.insert(ehFaceB);
    soRecursionQueue.insert(ehFaceC);
    soRecursionQueue.insert(ehFaceA1);
    soRecursionQueue.insert(ehFaceB1);
    soRecursionQueue.insert(ehFaceC1);
  }
  return(iRetVal);
}

//@@ Swap two tets for two, in the case where two faces are coplanar.
int ITAPS_Swap::Swap3D::iReconfTet22
(iBase_EntityHandle aehTets[], iBase_EntityHandle ehFace, iBase_EntityHandle aehVerts[])
{
  int err = iBase_SUCCESS;
  // This routine does only the 2-to-2 case.  4-to-4 is done elsewhere.
  // The cells and verts are assumed to have been set up properly already.

  /*	        C
   *            /\ 
   *           /||\ 
   *          / |  \                 A,D are on the base;
   *         /  | | \                B,E are front, C,F are back.
   *        /   |    \               Cell A is on the left, B on
   *       /    |  |  \	            the right.
   *      /     |      \ 
   *     /      |   |   \ 
   *    /     C |     F	 \ 
   *   /        |  E |    \ 
   *  /   B     | _ - B	   \ 
   * /  _ _ - - |    /  - _ \ 
   *D -         |           -E
   * -__     A  |  /  D  __--
   *    -__     |     __-
   *       -___ |/__--
   *           -A-

  */
  if (!qAllowBdryChanges) return 0;
  iBase_EntityHandle & ehTetA = aehTets[0]; // ACBD
  iBase_EntityHandle & ehTetB = aehTets[1]; // ABCE

  iBase_EntityHandle &ehVertA = aehVerts[0];
  iBase_EntityHandle &ehVertB = aehVerts[1];
  iBase_EntityHandle &ehVertC = aehVerts[2];
  iBase_EntityHandle &ehVertD = aehVerts[3];
  iBase_EntityHandle &ehVertE = aehVerts[4];

  assert(qHasVert(ehTetA, ehVertD));
  assert(qHasVert(ehTetB, ehVertE));

#ifdef CLASSIFY
  iBase_EntityHandle oClassA, oClassB;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetA, oClassA);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetB, oClassB);
#else
  Assoc.getEntEntAssociation(ehTetA, oClassA);
  Assoc.getEntEntAssociation(ehTetB, oClassB);
#endif
  if (oClassA != oClassB) return 0;
#endif

#ifdef SLOW_BUT_SURE
  iBase_EntityHandle ehFaceA = getFaceOpposite(ehTetA, ehVertC);
  iBase_EntityHandle ehFaceB = getFaceOpposite(ehTetA, ehVertB);
  iBase_EntityHandle ehFaceC = getFaceOpposite(ehTetA, ehVertA);

  iBase_EntityHandle ehFaceD = getFaceOpposite(ehTetB, ehVertC);
  iBase_EntityHandle ehFaceE = getFaceOpposite(ehTetB, ehVertB);
  iBase_EntityHandle ehFaceF = getFaceOpposite(ehTetB, ehVertA);
#else
  iBase_EntityHandle ehFaceA, ehFaceB, ehFaceC, ehFaceD, ehFaceE, ehFaceF;
  {
    iBase_EntityHandle aehFaces[3], aehVertsTmp[] = {ehVertC, ehVertB, ehVertA};

    getFacesOpposite(ehTetA, aehVertsTmp, aehFaces);
    ehFaceA = aehFaces[0];
    ehFaceB = aehFaces[1];
    ehFaceC = aehFaces[2];

    getFacesOpposite(ehTetB, aehVertsTmp, aehFaces);
    ehFaceD = aehFaces[0];
    ehFaceE = aehFaces[1];
    ehFaceF = aehFaces[2];
  }
#endif

  // Check that faces A and D are bdry faces.  Then, if they belong to
  // different patches, bail out, so that we won't end up losing a
  // subsegment (from A to B, in this case) when we swap.
#ifndef NDEBUG
  iBase_EntityHandle ehTetOppA = getCellOpposite(ehFaceA, ehTetA);
  iBase_EntityHandle ehTetOppD = getCellOpposite(ehFaceD, ehTetB);
  assert(ehTetOppA == reinterpret_cast<iBase_EntityHandle>(nullHandle) && ehTetOppD == reinterpret_cast<iBase_EntityHandle>(nullHandle));
#endif
#ifdef CLASSIFY
  iBase_EntityHandle oClassBdry,  oClassBdryD;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceA, oClassBdry);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceD, oClassBdryD);
#else
  Assoc.getEntEntAssociation(ehFaceA, oClassBdry);
  Assoc.getEntEntAssociation(ehFaceD, oClassBdryD);
#endif
  if (oClassBdry != oClassBdryD) return 0;
#endif

  // Need to delete the two tets, the dividing face, and the two
  // coplanar faces.
  iMesh_deleteEnt(pMeshInst, ehTetA, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetB, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFace, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFaceA, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFaceD, &err);
  CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetA, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetB, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFace, oClassA);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceA, oClassBdry);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceD, oClassBdry);
#else
  Assoc.rmvEntEntAssociation(ehTetA, oClassA);
  Assoc.rmvEntEntAssociation(ehTetB, oClassA);
  Assoc.rmvEntEntAssociation(ehFace, oClassA);
  Assoc.rmvEntEntAssociation(ehFaceA, oClassBdry);
  Assoc.rmvEntEntAssociation(ehFaceD, oClassBdry);
#endif
#endif

  soRecursionQueue.erase(ehFace);
  soRecursionQueue.erase(ehFaceA);
  soRecursionQueue.erase(ehFaceD);

  iBase_EntityHandle ehTetCDEA, ehTetCEDB;
  if (qCreateFromVerts) {
    ehTetCDEA = createTetFromVerts(ehVertC, ehVertD, ehVertE, ehVertA);
    ehTetCEDB = createTetFromVerts(ehVertC, ehVertE, ehVertD, ehVertB);
  }
  else {
    // Create new faces (CDE, BDE, AED)
    iBase_EntityHandle ehFaceAED = createFace(ehVertA, ehVertE, ehVertD);
    iBase_EntityHandle ehFaceBDE = createFace(ehVertB, ehVertD, ehVertE);
    iBase_EntityHandle ehFaceCDE = createFace(ehVertC, ehVertD, ehVertE);
    
    // Create two new tets
    // FaceB = ACD   FaceC = BCD
    // FaceE = ACE   FaceF = BCE
    ehTetCDEA = createTet(ehFaceB, ehFaceAED, ehFaceE, ehFaceCDE);
    ehTetCEDB = createTet(ehFaceF, ehFaceBDE, ehFaceC, ehFaceCDE);
    assert(qFaceCheck(ehFaceAED));
    assert(qFaceCheck(ehFaceBDE));
    assert(qFaceCheck(ehFaceCDE));

    assert(getVertOpposite(ehTetCEDB, ehFaceCDE) == ehVertB);
    assert(getVertOpposite(ehTetCDEA, ehFaceCDE) == ehVertA);
  }
#ifdef CLASSIFY
  // Classify the interior face (CDE) and the two tets as interior.
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceCDE, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetCDEA, oClassA);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetCEDB, oClassA);
#else
  Assoc.setEntEntAssociation(ehFaceCDE, oClassA);
  Assoc.setEntEntAssociation(ehTetCDEA, oClassA);
  Assoc.setEntEntAssociation(ehTetCEDB, oClassA);
#endif
  // Classify the new boundary faces (AED, BDE) on the bdry.
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceAED, oClassBdry);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceBDE, oClassBdry);
#else
  Assoc.setEntEntAssociation(ehFaceAED, oClassBdry);
  Assoc.setEntEntAssociation(ehFaceBDE, oClassBdry);
#endif
#endif

  assert(qTetCheck(ehTetCDEA));
  assert(qTetCheck(ehTetCEDB));

  assert(qFaceCheck(ehFaceB));
  assert(qFaceCheck(ehFaceC));
  assert(qFaceCheck(ehFaceE));
  assert(qFaceCheck(ehFaceF));

  assert(iOrient3D(ehVertC, ehVertD, ehVertE, ehVertB) == -1);
  assert(iOrient3D(ehVertC, ehVertD, ehVertE, ehVertA) ==  1);

  int iRetVal = 1;
  iNum22 ++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(ehFaceB);
    soRecursionQueue.insert(ehFaceC);
    soRecursionQueue.insert(ehFaceE);
    soRecursionQueue.insert(ehFaceF);
  }
  return(iRetVal);
}

//@@ Swap four tets for four, in the case where two faces are coplanar.
int ITAPS_Swap::Swap3D::iReconfTet44
(iBase_EntityHandle aehTets[], iBase_EntityHandle ehFace, iBase_EntityHandle aehVerts[])
{
  int err = iBase_SUCCESS;
  // The cells and verts are assumed to have been set up properly already.

  // Here's the before picture:
  /*	        C
   *            /\ .
   *           /||\ .
   *          / |  \                 A,D are on the base;
   *         /  | | \                B,E are front, C,F are back.
   *        /   |    \               Cell A is on the left, B on
   *       /    |  |  \	            the right.
   *      /     |      \ .
   *     /      |   |   \ .
   *    /     C |     F  \ .
   *   /        |  E |    \ .
   *  /   B     | _ - B    \ .
   * /  _ _ - - |    /  - _ \ .
   *D -         |           -E
   * -__     A  |  /  D  __--
   *    -__     |     __-
   *       -___ |/__--
   *           -A-
   *
   *             ___---B_
   *     ____----     /  --__
   *D---             /       -E
   * - _      A     / |D  _ -
   *  \  - _       /   _ -   /
   *   \     -_   / _-      /
   *    \       -A-  |     /
   *     \     C1|     F1 /
   *      \      |  |    /
   *       \ B1  |  E1  /
   *        \    |     /	            A,D are on the base;
   *         \   | |  /               B1,E1 are front, C1,F1 are back.
   *          \  |   /                Cell A1 is on the left, B1 on
   *           \ || /                 the right.
   *            \| /                  Face G divides these two cells.
   *             \/
   *             C1

  */
  // Identify both cells and all seven faces in the two-tet config.
  iBase_EntityHandle & ehTetA = aehTets[0];
  iBase_EntityHandle & ehTetB = aehTets[1];
  iBase_EntityHandle & ehTetA1 = aehTets[2];
  iBase_EntityHandle & ehTetB1 = aehTets[3];

#ifdef CLASSIFY
  iBase_EntityHandle oClass;
  iBase_EntityHandle &oClassA = oClass, oClassB, oClassA1, oClassB1;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetA, oClass);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetB, oClassB);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetA1, oClassA1);
  Assoc.getEntEntAssociation(ModMesh, GModel, ehTetB1, oClassB1);
#else
  Assoc.getEntEntAssociation(ehTetA, oClass);
  Assoc.getEntEntAssociation(ehTetB, oClassB);
  Assoc.getEntEntAssociation(ehTetA1, oClassA1);
  Assoc.getEntEntAssociation(ehTetB1, oClassB1);
#endif
  if ((oClassA != oClassB) ||
      (oClassA != oClassA1) ||
      (oClassA != oClassB1))
    return 0;
#endif

  iBase_EntityHandle & ehVertA = aehVerts[0];
  iBase_EntityHandle & ehVertB = aehVerts[1];
  iBase_EntityHandle & ehVertC = aehVerts[2];
  iBase_EntityHandle & ehVertD = aehVerts[3];
  iBase_EntityHandle & ehVertE = aehVerts[4];
  iBase_EntityHandle & ehVertC1 = aehVerts[5];

  assert(qHasVert(ehTetA, ehVertD));
  assert(qHasVert(ehTetB, ehVertE));
  assert(qHasVert(ehTetA1, ehVertD));
  assert(qHasVert(ehTetB1, ehVertE));

#ifdef SLOW_BUT_SURE
  iBase_EntityHandle ehFaceA = getFaceOpposite( ehTetA, ehVertC);
  iBase_EntityHandle ehFaceB = getFaceOpposite( ehTetA, ehVertB);
  iBase_EntityHandle ehFaceC = getFaceOpposite( ehTetA, ehVertA);

  iBase_EntityHandle ehFaceD = getFaceOpposite( ehTetB, ehVertC);
  iBase_EntityHandle ehFaceE = getFaceOpposite( ehTetB, ehVertB);
  iBase_EntityHandle ehFaceF = getFaceOpposite( ehTetB, ehVertA);

  iBase_EntityHandle ehFaceB1 = getFaceOpposite( ehTetA1, ehVertB);
  iBase_EntityHandle ehFaceC1 = getFaceOpposite( ehTetA1, ehVertA);
  iBase_EntityHandle ehFaceG1 = getFaceOpposite( ehTetA1, ehVertD);

  iBase_EntityHandle ehFaceE1 = getFaceOpposite( ehTetB1, ehVertB);
  iBase_EntityHandle ehFaceF1 = getFaceOpposite( ehTetB1, ehVertA);
  assert(ehFaceG1 == getFaceOpposite( ehTetB1, ehVertE));
#else
  iBase_EntityHandle ehFaceA, ehFaceB, ehFaceC, ehFaceD, ehFaceE, ehFaceF;
  iBase_EntityHandle ehFaceB1, ehFaceC1, ehFaceG1, ehFaceE1, ehFaceF1;
  {
    iBase_EntityHandle aehFaces[3], aehVertsTmp[] = {ehVertC, ehVertB, ehVertA};

    getFacesOpposite(ehTetA, aehVertsTmp, aehFaces);
    ehFaceA = aehFaces[0];
    ehFaceB = aehFaces[1];
    ehFaceC = aehFaces[2];

    getFacesOpposite(ehTetB, aehVertsTmp, aehFaces);
    ehFaceD = aehFaces[0];
    ehFaceE = aehFaces[1];
    ehFaceF = aehFaces[2];

    aehVertsTmp[0] = ehVertB;
    aehVertsTmp[1] = ehVertA;
    aehVertsTmp[2] = ehVertD;
    getFacesOpposite(ehTetA1, aehVertsTmp, aehFaces);
    ehFaceB1 = aehFaces[0];
    ehFaceC1 = aehFaces[1];
    ehFaceG1 = aehFaces[2];

    aehVertsTmp[2] = ehVertE;
    getFacesOpposite(ehTetB1, aehVertsTmp, aehFaces);
    ehFaceE1 = aehFaces[0];
    ehFaceF1 = aehFaces[1];
    assert(ehFaceG1 == aehFaces[2]);
  }
#endif


  assert(getVertOpposite(ehTetB1, ehFaceD) == ehVertC1);

  // Must get rid of all four tets and all four interior faces.
  iMesh_deleteEnt(pMeshInst, ehTetA, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetB, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetA1, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehTetB1, &err);
  CHECK_ERR(err, pMeshInst);

  iMesh_deleteEnt(pMeshInst, ehFaceA, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFaceD, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFace, &err);
  CHECK_ERR(err, pMeshInst);
  iMesh_deleteEnt(pMeshInst, ehFaceG1, &err);
  CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetA,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetB,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetA1, oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehTetB1, oClass);

  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceA,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceD,  oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFace,   oClass);
  Assoc.rmvEntEntAssociation(ModMesh, GModel, ehFaceG1, oClass);
#else
  Assoc.rmvEntEntAssociation(ehTetA,  oClass);
  Assoc.rmvEntEntAssociation(ehTetB,  oClass);
  Assoc.rmvEntEntAssociation(ehTetA1, oClass);
  Assoc.rmvEntEntAssociation(ehTetB1, oClass);

  Assoc.rmvEntEntAssociation(ehFaceA,  oClass);
  Assoc.rmvEntEntAssociation(ehFaceD,  oClass);
  Assoc.rmvEntEntAssociation(ehFace,   oClass);
  Assoc.rmvEntEntAssociation(ehFaceG1, oClass);
#endif
#endif

  soRecursionQueue.erase(ehFace);
  soRecursionQueue.erase(ehFaceA);
  soRecursionQueue.erase(ehFaceD);
  soRecursionQueue.erase(ehFaceG1);

  iBase_EntityHandle ehTetDEBC, ehTetDECA, ehTetDEAC1, ehTetDEC1B;
  if (qCreateFromVerts) {
    ehTetDEBC = createTetFromVerts(ehVertD, ehVertE, ehVertB, ehVertC);
    ehTetDECA = createTetFromVerts(ehVertD, ehVertE, ehVertC, ehVertA);
    ehTetDEAC1 = createTetFromVerts(ehVertD, ehVertE, ehVertA, ehVertC1);
    ehTetDEC1B = createTetFromVerts(ehVertD, ehVertE, ehVertC1, ehVertB);
  }
  else {
    // Now create four new faces: DEB, DEC, DEA, DEC1
    iBase_EntityHandle ehFaceDEB = createFace(ehVertD, ehVertE, ehVertB);
    iBase_EntityHandle ehFaceDEC = createFace(ehVertD, ehVertE, ehVertC);
    iBase_EntityHandle ehFaceDEA = createFace(ehVertD, ehVertE, ehVertA);
    iBase_EntityHandle ehFaceDEC1 = createFace(ehVertD, ehVertE, ehVertC1);
    
    // Finally, create four new tets: DEBC, DECA, DEAC1, DEC1B
    // Face B: ACD    Face B1: AC1D
    // Face C: BCD    Face C1: BC1D
    // Face E: ACE    Face E1: AC1E
    // Face F: BCE    Face F1: BC1E
    ehTetDEBC = createTet(ehFaceDEC, ehFaceF, ehFaceC, ehFaceDEB);
    ehTetDECA = createTet(ehFaceDEA, ehFaceE, ehFaceB, ehFaceDEC);
    ehTetDEAC1 = createTet(ehFaceDEC1, ehFaceE1, ehFaceB1, ehFaceDEA);
    ehTetDEC1B = createTet(ehFaceDEB, ehFaceF1, ehFaceC1, ehFaceDEC1);

    assert(qFaceCheck(ehFaceDEB));
    assert(qFaceCheck(ehFaceDEC));
    assert(qFaceCheck(ehFaceDEA));
    assert(qFaceCheck(ehFaceDEC1));
  }
      
    
#ifdef CLASSIFY
  // All of these get the same classification.
#ifdef OLD_TSTTR
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEB, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEC, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEA, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehFaceDEC1, oClass);

  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDEBC, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDECA, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDEAC1, oClass);
  Assoc.setEntEntAssociation(ModMesh, GModel, ehTetDEC1B, oClass);
#else
  Assoc.setEntEntAssociation(ehFaceDEB, oClass);
  Assoc.setEntEntAssociation(ehFaceDEC, oClass);
  Assoc.setEntEntAssociation(ehFaceDEA, oClass);
  Assoc.setEntEntAssociation(ehFaceDEC1, oClass);

  Assoc.setEntEntAssociation(ehTetDEBC, oClass);
  Assoc.setEntEntAssociation(ehTetDECA, oClass);
  Assoc.setEntEntAssociation(ehTetDEAC1, oClass);
  Assoc.setEntEntAssociation(ehTetDEC1B, oClass);
#endif
#endif

  assert(qTetCheck(ehTetDEBC));
  assert(qTetCheck(ehTetDECA));
  assert(qTetCheck(ehTetDEAC1));
  assert(qTetCheck(ehTetDEC1B));

  assert(qFaceCheck(ehFaceB));
  assert(qFaceCheck(ehFaceC));
  assert(qFaceCheck(ehFaceE));
  assert(qFaceCheck(ehFaceF));

  assert(qFaceCheck(ehFaceB1));
  assert(qFaceCheck(ehFaceC1));
  assert(qFaceCheck(ehFaceE1));
  assert(qFaceCheck(ehFaceF1));

  assert(iOrient3D(ehVertC, ehVertD, ehVertE, ehVertB) == -1);
  assert(iOrient3D(ehVertC, ehVertD, ehVertE, ehVertA) ==  1);

  assert(iOrient3D(ehVertC1, ehVertD, ehVertE, ehVertB) ==  1);
  assert(iOrient3D(ehVertC1, ehVertD, ehVertE, ehVertA) == -1);

  int iRetVal = 1;
  iNum44 ++;
  if (qAllowRecursion) {
    soRecursionQueue.insert(ehFaceB);
    soRecursionQueue.insert(ehFaceC);
    soRecursionQueue.insert(ehFaceE);
    soRecursionQueue.insert(ehFaceF);
    soRecursionQueue.insert(ehFaceB1);
    soRecursionQueue.insert(ehFaceC1);
    soRecursionQueue.insert(ehFaceE1);
    soRecursionQueue.insert(ehFaceF1);
  }
  return(iRetVal);
}

#ifndef NDEBUG
bool ITAPS_Swap::Swap3D::qTetCheck(iBase_EntityHandle ehTet)
{
  int err = iBase_SUCCESS;
  // Confirm the proper number of faces.
  iBase_EntityHandle *aehFaces = new iBase_EntityHandle[4];
  int iFaces_allocated = 4, iFaces_size;

  iMesh_getEntAdj(pMeshInst, ehTet, iBase_FACE,
		  &aehFaces, &iFaces_allocated, &iFaces_size, &err);
  CHECK_ERR(err, pMeshInst);

  if (iFaces_size != 4) return false;

  // Confirm reciprocal face connectivity.
  iBase_EntityHandle *aehRegions = new iBase_EntityHandle[2];
  int iRegions_allocated = 2, iRegions_size;

  for (int i = 0; i < 4; i++) {
    iMesh_getEntAdj(pMeshInst, aehFaces[i], iBase_REGION,
		    &aehRegions, &iRegions_allocated, &iRegions_size, &err);
    CHECK_ERR(err, pMeshInst);
    if (iRegions_size > 2 || iRegions_size < 1 ||
	(aehRegions[0] != ehTet && aehRegions[1] != ehTet))
      return false;
  }
  return true;
}

bool ITAPS_Swap::Swap3D::qFaceCheck(iBase_EntityHandle ehFace)
{
  int err = iBase_SUCCESS;
  // Must have either one or two regions incident.
  static iBase_EntityHandle *aehRegs = new iBase_EntityHandle[2];
  static int iRegs_allocated = 2, iRegs_size;

  iMesh_getEntAdj(pMeshInst, ehFace, iBase_REGION,
		  &aehRegs, &iRegs_allocated, &iRegs_size, &err);
  CHECK_ERR(err, pMeshInst);

  if (iRegs_size < 1 || iRegs_size > 2) return false;

  // Check reciprocal connectivity.
  static iBase_EntityHandle *aehFaces = new iBase_EntityHandle[6];
  static int iFaces_allocated = 6, iFaces_size;

  bool qRetVal = false;
  for (int i = 0; i < iRegs_size; i++) {
    iMesh_getEntAdj(pMeshInst, aehRegs[i], iBase_FACE,
		    &aehFaces, &iFaces_allocated, &iFaces_size, &err);
    CHECK_ERR(err, pMeshInst);
    if (iFaces_size < 4 || iFaces_size > 6) return false; // Bad region!
    for (int ii = 0; ii < iFaces_size; ii++) {
      if (aehFaces[ii] == ehFace) {
	qRetVal = true;
	break;
      }
    }
  }
  return qRetVal;
}
#endif

bool ITAPS_Swap::Swap3D::qHasVert(iBase_EntityHandle ehEntity,
				  iBase_EntityHandle ehVert)
{
  int err = iBase_SUCCESS;
  // Slog through the verts and see if you find the one you're after.
  static iBase_EntityHandle *aehVerts = new iBase_EntityHandle[8];
  static int iVerts_allocated = 8, iVerts_size;

  iMesh_getEntAdj(pMeshInst, ehEntity, iBase_VERTEX,
		  &aehVerts, &iVerts_allocated, &iVerts_size, &err);
  CHECK_ERR(err, pMeshInst);
  assert(iVerts_size >= 1 && iVerts_size <= 8);

  for (int i = 0; i < iVerts_size; i++) {
    if (aehVerts[i] == ehVert) return true;
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


int ITAPS_Swap::Swap3D::iEdgeSwap3D
(iBase_EntityHandle ehFace, iBase_EntityHandle ehVNorth,
 iBase_EntityHandle ehVSouth, iBase_EntityHandle ehVOther)
{
  int err = iBase_SUCCESS;
  //
  assert(qFaceCheck(ehFace));
  assert(qHasVert(ehFace, ehVNorth));
  assert(qHasVert(ehFace, ehVSouth));
  assert(qHasVert(ehFace, ehVOther));
  // This call only needs to appear once, but that's okay, because
  // subsequent calls short-circuit and exit immediately.
  vInitCanonConfigs();

  const double dInvalidPenalty = -10000;

  static const int iMaxFaces = 10;
  iBase_EntityHandle aehVOuter[iMaxFaces]; // Verts and tets
  iBase_EntityHandle aehFNorth[iMaxFaces], aehFSouth[iMaxFaces], aehFOrig[iMaxFaces]; // Faces

  // Determine the size of the orbit around the pivot edge and list
  // all the verts, faces, and cells involved.
  aehFOrig[0] = ehFace;
  aehVOuter[0] = ehVOther;
#ifdef CLASSIFY
  opaque oClass;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehFace, oClass);
#else
  Assoc.getEntEntAssociation(ehFace, oClass);
#endif
#endif

  static iBase_EntityHandle *aehCOrig = new iBase_EntityHandle[iMaxFaces];
  static int iCOrig_allocated = iMaxFaces, iCOrig_size;

  iMesh_getEntAdj(pMeshInst, ehFace, iBase_REGION,
		  &aehCOrig, &iCOrig_allocated, &iCOrig_size, &err);
  CHECK_ERR(err, pMeshInst);
  // If there's only one of these, then we've got a bdry face in our hand.
  assert(iCOrig_size == 1 || iCOrig_size == 2);
  if (iCOrig_size == 1) {
    return iBdryEdgeSwap3D(ehFace, ehVNorth, ehVSouth, ehVOther);
  }

  // aehCOrig now contains a cell incident on ehFace.  Not necessarily
  // on the "left" side, so there might be orientation problems
  // later...

  assert(qConfirmTopo(pMeshInst, aehCOrig[0], iMesh_TETRAHEDRON));
  if (qUsingSet && !qIsEntContained(aehCOrig[0])) return 0;

#ifdef CLASSIFY
  {
    opaque oClassTmp;
#ifdef OLD_TSTTR
    Assoc.getEntEntAssociation(ModMesh, GModel, aehCOrig[0], oClassTmp);
#else
    Assoc.getEntEntAssociation(aehCOrig[0], oClassTmp);
#endif
    if (oClassTmp != oClass) return 0;
  }
#endif
  iBase_EntityHandle ehFNew;
  {
    iBase_EntityHandle aehFaces[3], aehVerts[] = {ehVSouth, ehVNorth, aehVOuter[0]};
    getFacesOpposite(aehCOrig[0], aehVerts, aehFaces);
    aehFNorth[0] = aehFaces[0];
    aehFSouth[0] = aehFaces[1];
    ehFNew = aehFaces[2];
  }

  int i = 1;
  while ((i < iMaxFaces) && (ehFNew != ehFace)) {
    aehFOrig[i] = ehFNew;
    aehVOuter[i] = getVertOpposite(aehCOrig[i-1], aehFOrig[i-1]);
    aehCOrig[i] = getCellOpposite(ehFNew, aehCOrig[i-1]);
    // Hit a boundary or non-tet cell.
    if (aehCOrig[i] == reinterpret_cast<iBase_EntityHandle>(nullHandle)) {
      return iBdryEdgeSwap3D(ehFNew, ehVNorth, ehVSouth, aehVOuter[i]);
    }
    assert(qConfirmTopo(pMeshInst, aehCOrig[i], iMesh_TETRAHEDRON));

    if (qUsingSet && !qIsEntContained(aehCOrig[i])) return 0;
    // Give up if more than one region is incident on the edge, or the
    // new tet isn't in the required set.
#ifdef CLASSIFY
    {
      opaque oClassTmp;
#ifdef OLD_TSTTR
      Assoc.getEntEntAssociation(ModMesh, GModel, aehCOrig[i], oClassTmp);
#else
      Assoc.getEntEntAssociation(aehCOrig[i], oClassTmp);
#endif
      if (oClassTmp != oClass) return 0;
    }
#endif

    {
      iBase_EntityHandle aehFaces[3],
	aehVerts[] = {ehVSouth, ehVNorth, aehVOuter[i]};
      getFacesOpposite(aehCOrig[i], aehVerts, aehFaces);
      aehFNorth[i] = aehFaces[0];
      aehFSouth[i] = aehFaces[1];
      ehFNew = aehFaces[2];
    }
    i++;
  }
  if (ehFNew != ehFace) return 0; // Ran out of space
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
    double dQual = dEvaluateQual(aehCOrig[i]);
    dOrigQual = std::min(dQual, dOrigQual);
  }

  // Set up info about the possible new configurations.

  // Compute quality for each possible tet pair (one pair per possible
  // triangle).
  double a3dNewQual[iMaxFaces][iMaxFaces][iMaxFaces];
  int iSign = iOrient3D(aehVOuter[0], aehVOuter[1], ehVSouth, ehVNorth);
  int iV0, iV1, iV2;
  for (iV0 = 0; iV0 < iMaxFaces; iV0++)
    for (iV1 = 0; iV1 < iMaxFaces; iV1++)
      for (iV2 = 0; iV2 < iMaxFaces; iV2++)
	a3dNewQual[iV0][iV1][iV2] = 0;
#ifndef NDEBUG
  for (int iDum = 0; iDum < iNOrig; iDum++)
    assert(iSign == iOrient3D(aehVOuter[iDum], aehVOuter[(iDum+1)%iNOrig],
			      ehVSouth, ehVNorth));
#endif
  for (iV0 = 0; iV0 < iNOrig; iV0++)
    for (iV1 = iV0+1; iV1 < iNOrig; iV1++)
      for (iV2 = iV1+1; iV2 < iNOrig; iV2++) {
	if (iOrient3D(aehVOuter[iV0], aehVOuter[iV1], aehVOuter[iV2],
		      ehVNorth) == iSign)
	  // Evaluate quality
	  a3dNewQual[iV0][iV1][iV2] = dEvaluateQual(aehVOuter[iV0],
						    aehVOuter[iV1],
						    aehVOuter[iV2],
						    ehVNorth);
	else
	  a3dNewQual[iV0][iV1][iV2] = dInvalidPenalty;

	// Don't bother evaluating for the other tet if this one is
	// worse that the worst in the original config, because this tet
	// pair can never be in a final config anyway.
	if (a3dNewQual[iV0][iV1][iV2] >= dOrigQual) {
	  double dDummyQual;
	  if (iOrient3D(aehVOuter[iV0], aehVOuter[iV2], aehVOuter[iV1],
			ehVSouth) == iSign)
	    dDummyQual = dEvaluateQual(aehVOuter[iV0], aehVOuter[iV2],
				       aehVOuter[iV1],  ehVSouth);
	  else
	    dDummyQual = dInvalidPenalty;
	  a3dNewQual[iV0][iV1][iV2] = std::min(a3dNewQual[iV0][iV1][iV2],
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
	dConfQual = std::min(dConfQual, a3dNewQual[iV0][iV1][iV2]);
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
      iMesh_deleteEnt(pMeshInst, aehCOrig[i], &err);
      CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aehCOrig[i], oClass);
#else
      Assoc.rmvEntEntAssociation(aehCOrig[i], oClass);
#endif
#endif
    }

    for (i = 0; i < iNOrig; i++) {
      iMesh_deleteEnt(pMeshInst, aehFOrig[i], &err);
      CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aehFOrig[i], oClass);
#else
      Assoc.rmvEntEntAssociation(aehFOrig[i], oClass);
#endif
#endif
      soRecursionQueue.erase(aehFOrig[i]);
    }

    // Renumber all the old face, vertex and cell data, offsetting it
    // by iBestPerm to make the desired configuration match the stored
    // canonical configuration.
    if (iBestPerm != 0) {
      iBase_EntityHandle aehVOuterTemp[iMaxFaces];
      iBase_EntityHandle aehFNorthTemp[iMaxFaces], aehFSouthTemp[iMaxFaces];
      // Copy data to temporary arrays.
      for (i = 0; i < iNOrig; i++) {
	aehFNorthTemp[i] = aehFNorth[i];
	aehFSouthTemp[i] = aehFSouth[i];
	aehVOuterTemp[i] = aehVOuter[i];
      }
      // Copy back with an offset.
      for (i = 0; i < iNOrig; i++) {
	aehFNorth[i] = aehFNorthTemp[(i+iBestPerm)%iNOrig];
	aehFSouth[i] = aehFSouthTemp[(i+iBestPerm)%iNOrig];
	aehVOuter[i] = aehVOuterTemp[(i+iBestPerm)%iNOrig];
      }
    }

    // Re-connect the mesh locally
    Config *pConf = &(aCS[iNOrig].aConf[iBestCanon]);
    iBase_EntityHandle aehCNew[2*iNOrig-4];
    int iOrient = 0;
    if (qCreateFromVerts) {
      // There are iNOrig - 2 triangles on the equator, two tets apiece,
      // four verts per tet.
      for (i = 0; i < iNOrig -2; i++) {
	// These are the equatorial tris, so all these indices should be
	// positive. 
	assert(pConf->a2iFaceVert[i][0] >= 0);
	assert(pConf->a2iFaceVert[i][1] >= 0);
	assert(pConf->a2iFaceVert[i][2] >= 0);
	iBase_EntityHandle ehVert0 = aehVOuter[pConf->a2iFaceVert[i][0]];
	iBase_EntityHandle ehVert1 = aehVOuter[pConf->a2iFaceVert[i][1]];
	iBase_EntityHandle ehVert2 = aehVOuter[pConf->a2iFaceVert[i][2]];

	if (iOrient == 0)
	  iOrient = iOrient3D(ehVert0, ehVert1, ehVert2, ehVNorth);
	
	if (iOrient == 1) {
	  assert(iOrient3D(ehVert0, ehVert1, ehVert2, ehVNorth) == 1);
	  assert(iOrient3D(ehVert0, ehVert2, ehVert1, ehVSouth) == 1);
	  aehCNew[2*i  ] = createTetFromVerts(ehVert0, ehVert1,
					      ehVert2, ehVNorth);
	  aehCNew[2*i+1] = createTetFromVerts(ehVert0, ehVert2,
					      ehVert1, ehVSouth);
	}
	else {
	  assert(iOrient3D(ehVert0, ehVert2, ehVert1, ehVNorth) == 1);
	  assert(iOrient3D(ehVert0, ehVert1, ehVert2, ehVSouth) == 1);
	  aehCNew[2*i  ] = createTetFromVerts(ehVert0, ehVert2,
					      ehVert1, ehVNorth);
	  aehCNew[2*i+1] = createTetFromVerts(ehVert0, ehVert1,
					      ehVert2, ehVSouth);
	}
      }
    }
    else {
      iBase_EntityHandle aehFNew[iNOrig*3 - 8];
      // Assign data to new faces
      for (i = 0; i < 3*iNOrig-8; i++) {
	iBase_EntityHandle ehV0, ehV1, ehV2;
	iV0 = pConf->a2iFaceVert[i][0];
	switch (iV0) {
	case NORTH_POLE: ehV0 = ehVNorth; break;
	case SOUTH_POLE: ehV0 = ehVSouth; break;
	default:         ehV0 = aehVOuter[iV0]; break;
	}
	iV1 = pConf->a2iFaceVert[i][1];
	switch (iV1) {
	case NORTH_POLE: ehV1 = ehVNorth; break;
	case SOUTH_POLE: ehV1 = ehVSouth; break;
	default:         ehV1 = aehVOuter[iV1]; break;
	}
	iV2 = pConf->a2iFaceVert[i][2];
	switch (iV2) {
	case NORTH_POLE: ehV2 = ehVNorth; break;
	case SOUTH_POLE: ehV2 = ehVSouth; break;
	default:         ehV2 = aehVOuter[iV2]; break;
	}
	aehFNew[i] = createFace(ehV0, ehV1, ehV2);
	if (iOrient == 0)
	  iOrient = iOrient3D(ehV0, ehV1, ehV2, ehVNorth);
	assert(iOrient3D(ehV0, ehV1, ehV2, ehVNorth) * iOrient != -1);
      }
      
	
      // Cell setup, including re-connection to outer faces
      for (i = 0; i < 2*iNOrig-4; i++) {
	iBase_EntityHandle aehF[4];
	for (int ii = 0; ii < 4; ii++) {
	  int iFace = pConf->a2iCellFace[i][ii];
	  if (iFace >= SOUTH_FACE) {
	    aehF[ii] = aehFSouth[iFace - SOUTH_FACE];
	  }
	  else if (iFace >= NORTH_FACE) {
	    aehF[ii] = aehFNorth[iFace - NORTH_FACE];
	  }
	  else
	    aehF[ii] = aehFNew[iFace];
	}
	aehCNew[i] = createTet(aehF[1], aehF[2], aehF[3], aehF[0]);
      }
      for (i = 0; i < 3*iNOrig - 8 ; i++) {
	assert(qFaceCheck(aehFNew[i]));
	if (qAllowRecursion) {
	  soRecursionQueue.insert(aehFNew[i]);
	}
#ifdef CLASSIFY
	// Set face classification; some implementations will want tets
	// classified first.
#ifdef OLD_TSTTR
	Assoc.setEntEntAssociation(ModMesh, GModel, aehFNew[i], oClass);
#else
	Assoc.setEntEntAssociation(aehFNew[i], oClass);
#endif
#endif
      }
    }
#ifdef CLASSIFY
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, aehCNew[i], oClass);
#else
    Assoc.setEntEntAssociation(aehCNew[i], oClass);
#endif
#endif
  
    // Validate the result
    for (i = 0; i < iNOrig*2 - 4; i++) {
      // Check everything about the tet
      assert(qTetCheck(aehCNew[i]));
      // Every tet has to hit one pole or the other
      assert(qHasVert(aehCNew[i], ehVNorth) ||
	     qHasVert(aehCNew[i], ehVSouth));
      // Assert tetrahedron orientation
      assert(iOrient3D(aehCNew[i]) == 1);
    }

    for (i = 0; i < iNOrig; i++) {
      assert(qFaceCheck(aehFSouth[i]));
      assert(qFaceCheck(aehFNorth[i]));
    }

    // If recursion is permitted, check all the new faces for possible
    // swaps, as well as all of the faces on the outside of the orbit.
    int iRetVal = iNOrig-2;
    iNumEdge ++;
    if (qAllowRecursion) {
      // Interior faces already added, if they're conveniently available.
      for (i = 0; i < iNOrig; i++) {
	soRecursionQueue.insert(aehFNorth[i]);
	soRecursionQueue.insert(aehFSouth[i]);
      }
    }

    // Return the total (equivalent) number of swaps performed
    return iRetVal;
  }
}

//@ Locally reconfigure tets incident on a bdry edge to improve mesh quality
// The bdry edge given by ehVNorth, ehVSouth is a candidate for removal.
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
int ITAPS_Swap::Swap3D::iBdryEdgeSwap3D
(iBase_EntityHandle ehFace, iBase_EntityHandle ehVNorth, iBase_EntityHandle ehVSouth,
 iBase_EntityHandle ehVOther)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehFace, iMesh_TRIANGLE));
  assert(qFaceCheck(ehFace));
  assert(qHasVert(ehFace, ehVNorth));
  assert(qHasVert(ehFace, ehVSouth));
  assert(qHasVert(ehFace, ehVOther));

#ifdef CLASSIFY
  // The face must be on the boundary.
  opaque oGeomFaceStart, oGeomFaceEnd;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehFace, oGeomFaceStart);
#else
  Assoc.getEntEntAssociation(ehFace, oGeomFaceStart);
#endif
#endif

  // This call only needs to appear once, but that's okay, because
  // subsequent calls short-circuit and exit immediately.
  vInitCanonConfigs();
  if (!qAllowBdryChanges) return 0;

  const double dInvalidPenalty = -10000;

  //@@ Identify all cells and faces incident on the bad edge
  static const int iMaxSize = 10;
  iBase_EntityHandle aehVOuter[iMaxSize+1]; // Verts and tets
  iBase_EntityHandle aehFNorth[iMaxSize+1], aehFSouth[iMaxSize+1],
    aehFOrig[iMaxSize+1]; // Faces

  static iBase_EntityHandle *aehCOrig = new iBase_EntityHandle[iMaxSize+1];
  static int iCOrig_allocated = iMaxSize+1, iCOrig_size;

  iMesh_getEntAdj(pMeshInst, ehFace, iBase_REGION,
		  &aehCOrig, &iCOrig_allocated, &iCOrig_size, &err);
  CHECK_ERR(err, pMeshInst);
  // If there's only one of these, then we've got a bdry face in our hand.
  assert(iCOrig_size == 1);

  // aehCOrig now contains a cell incident on ehFace.  Not necessarily
  // on the "left" side, so there might be orientation problems
  // later...
  if (qUsingSet && !qIsEntContained(aehCOrig[0])) return 0;

  // Determine the size of the orbit around the pivot edge and list
  // all the verts, faces, and cells involved.
  aehFOrig[0] = ehFace;
  aehVOuter[0] = ehVOther;
  aehFNorth[0] = getFaceOpposite(aehCOrig[0], ehVSouth);
  aehFSouth[0] = getFaceOpposite(aehCOrig[0], ehVNorth);
  iBase_EntityHandle ehFaceNew = getFaceOpposite(aehCOrig[0], aehVOuter[0]);
#ifdef CLASSIFY
  opaque oClassInt;
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, aehCOrig[0], oClassInt);
#else
  Assoc.getEntEntAssociation(aehCOrig[0], oClassInt);
#endif
#endif
  int i = 1;
  while (i <= iMaxSize) {
    aehFOrig[i] = ehFaceNew;
    aehVOuter[i] = getVertOpposite(aehCOrig[i-1], aehFOrig[i-1]);
    aehCOrig[i] = getCellOpposite(ehFaceNew, aehCOrig[i-1]);
    // Hit a boundary or non-tet cell.
    if (aehCOrig[i] == reinterpret_cast<iBase_EntityHandle>(nullHandle)) break;
    if (!qConfirmTopo(pMeshInst, aehCOrig[i], iMesh_TETRAHEDRON)) {
      throw(iBase_NOT_SUPPORTED);
    }
    // Give up if more than one region is incident on the edge, or the
    // new tet isn't in the required set.
    if (qUsingSet && !qIsEntContained(aehCOrig[i])) return 0;
#ifdef CLASSIFY
    {
      opaque oClassTmp;
#ifdef OLD_TSTTR
      Assoc.getEntEntAssociation(ModMesh, GModel, aehCOrig[i], oClassTmp);
#else
      Assoc.getEntEntAssociation(aehCOrig[i], oClassTmp);
#endif
      if (oClassTmp != oClassInt) return 0;
    }
#endif

    aehFNorth[i] = getFaceOpposite(aehCOrig[i], ehVSouth);
    aehFSouth[i] = getFaceOpposite(aehCOrig[i], ehVNorth);
    ehFaceNew = getFaceOpposite(aehCOrig[i], aehVOuter[i]);
    i++;
  }
#ifdef CLASSIFY
  // Assign data for the second boundary face
#ifdef OLD_TSTTR
  Assoc.getEntEntAssociation(ModMesh, GModel, ehFaceNew, oGeomFaceEnd);
#else
  Assoc.getEntEntAssociation(ehFaceNew, oGeomFaceEnd);
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
  if (aehCOrig[i] != reinterpret_cast<iBase_EntityHandle>(nullHandle)) return 0;

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
  // non-coplanar point (i.e., any other member of aehVOuter) will be the
  // same.  This exit is redundant (no swapping would occur for this
  // case anyway), but saves a lot of quality computations.
  if (iOrient3D(ehVOther, aehVOuter[iNOrigVerts-1], ehVNorth, aehVOuter[1])
      !=
      iOrient3D(ehVOther, ehVSouth, aehVOuter[iNOrigVerts-1], aehVOuter[1])
      ) return 0;

  double dOrigQual;
  //@@ Compute the quality of the current configuration.
  // This is the smallest quality for any of the existing cells.
  dOrigQual = 1000;
  for (i = 0; i < iNOrigCells; i++) {
    // Evaluate quality, depending on the swap criterion in use.
    double dQual = dEvaluateQual(aehCOrig[i]);
    dOrigQual = std::min(dQual, dOrigQual);
  }

  // Set up info about the possible new configurations.

  // Compute quality for each possible tet pair (one pair per possible
  // triangle).
  double a3dNewQual[iMaxSize][iMaxSize][iMaxSize];
  int iSign = iOrient3D(aehVOuter[0], aehVOuter[1], ehVSouth, ehVNorth);
  int iV0, iV1, iV2;
#ifndef NDEBUG
  for (iV0 = 0; iV0 < iMaxSize; iV0++)
    for (iV1 = 0; iV1 < iMaxSize; iV1++)
      for (iV2 = 0; iV2 < iMaxSize; iV2++)
	a3dNewQual[iV0][iV1][iV2] = 0;
  for (int iDum = 0; iDum < iNOrigCells; iDum++)
    assert(iSign == iOrient3D(aehVOuter[iDum], aehVOuter[iDum+1],
			      ehVSouth, ehVNorth));
#endif
  for (iV0 = 0; iV0 < iNOrigVerts; iV0++)
    for (iV1 = iV0+1; iV1 < iNOrigVerts; iV1++)
      for (iV2 = iV1+1; iV2 < iNOrigVerts; iV2++) {
	if (iOrient3D(aehVOuter[iV0], aehVOuter[iV1], aehVOuter[iV2],
		      ehVNorth) == iSign)
	  // Evaluate quality
	  a3dNewQual[iV0][iV1][iV2] = dEvaluateQual(aehVOuter[iV0],
						    aehVOuter[iV1],
						    aehVOuter[iV2],
						    ehVNorth);
	else
	  a3dNewQual[iV0][iV1][iV2] = dInvalidPenalty;

	// Don't bother evaluating for the other tet if this one is
	// worse that the worst in the original config, because this tet
	// pair can never be in a final config anyway.
	if (a3dNewQual[iV0][iV1][iV2] >= dOrigQual) {
	  double dDummyQual;
	  if (iOrient3D(aehVOuter[iV0], aehVOuter[iV2], aehVOuter[iV1],
			ehVSouth) == iSign)
	    dDummyQual = dEvaluateQual(aehVOuter[iV0], aehVOuter[iV2],
				       aehVOuter[iV1],  ehVSouth);
	  else
	    dDummyQual = dInvalidPenalty;
	  a3dNewQual[iV0][iV1][iV2] = std::min(a3dNewQual[iV0][iV1][iV2],
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
	dConfQual = std::min(dConfQual, a3dNewQual[iV0][iV1][iV2]);
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
    if (qCreateFromVerts) return 0;
    
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
      iMesh_deleteEnt(pMeshInst, aehCOrig[i], &err);
      CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aehCOrig[i], oClassInt);
#else
      Assoc.rmvEntEntAssociation(aehCOrig[i], oClassInt);
#endif
#endif
    }
    for (i = 1; i < iNOrigCells; i++) {
      iMesh_deleteEnt(pMeshInst, aehFOrig[i], &err);
      CHECK_ERR(err, pMeshInst);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.rmvEntEntAssociation(ModMesh, GModel, aehFOrig[i], oClassInt);
#else
      Assoc.rmvEntEntAssociation(aehFOrig[i], oClassInt);
#endif
#endif
      soRecursionQueue.erase(aehFOrig[i]);
    }

    // Create a new North face and a new South face to hold the
    // reconfigured boundary faces.  Also, connect them to the existing
    // boundary data.

    aehFNorth[iNOrigVerts-1] = createFace(aehVOuter[0],
					  aehVOuter[iNOrigVerts-1],
					  ehVNorth);
    aehFSouth[iNOrigVerts-1] = createFace(aehVOuter[iNOrigVerts-1],
					  aehVOuter[0],
					  ehVSouth);

    // Renumber all the old face, vertex and cell data, offsetting it
    // by iBestPerm to make the desired configuration match the stored
    // canonical configuration.
    if (iBestPerm != 0) {
      iBase_EntityHandle aehFNorthTemp[iNOrigVerts], aehFSouthTemp[iNOrigVerts];
      iBase_EntityHandle aehVOuterTemp[iNOrigVerts];
      // Copy data to temporary arrays.
      for (i = 0; i < iNOrigVerts; i++) {
	aehFNorthTemp[i] = aehFNorth[i];
	aehFSouthTemp[i] = aehFSouth[i];
	aehVOuterTemp[i] = aehVOuter[i];
      }
      // Copy back with an offset.
      for (i = 0; i < iNOrigVerts; i++) {
	aehFNorth[i] = aehFNorthTemp[(i+iBestPerm)%iNOrigVerts];
	aehFSouth[i] = aehFSouthTemp[(i+iBestPerm)%iNOrigVerts];
	aehVOuter[i] = aehVOuterTemp[(i+iBestPerm)%iNOrigVerts];
      }
    }

    // Re-connect the mesh locally
    Config *pConf = &(aCS[iNOrigVerts].aConf[iBestCanon]);
    iBase_EntityHandle aehFNew[3*iNOrigVerts - 8];
    // Assign data to new faces
    for (i = 0; i < 3*iNOrigVerts-8; i++) {
      iBase_EntityHandle ehV0, ehV1, ehV2;
      iV0 = pConf->a2iFaceVert[i][0];
      switch (iV0) {
      case NORTH_POLE: ehV0 = ehVNorth; break;
      case SOUTH_POLE: ehV0 = ehVSouth; break;
      default:         ehV0 = aehVOuter[iV0]; break;
      }
      iV1 = pConf->a2iFaceVert[i][1];
      switch (iV1) {
      case NORTH_POLE: ehV1 = ehVNorth; break;
      case SOUTH_POLE: ehV1 = ehVSouth; break;
      default:         ehV1 = aehVOuter[iV1]; break;
      }
      iV2 = pConf->a2iFaceVert[i][2];
      switch (iV2) {
      case NORTH_POLE: ehV2 = ehVNorth; break;
      case SOUTH_POLE: ehV2 = ehVSouth; break;
      default:         ehV2 = aehVOuter[iV2]; break;
      }

      aehFNew[i] = createFace(ehV0, ehV1, ehV2);
    }

    // Cell setup, including re-connection to outer faces
    iBase_EntityHandle aehCNew[2*iNOrigVerts - 4];
    for (i = 0; i < 2*iNOrigVerts-4; i++) {
      iBase_EntityHandle aehF[4];
      for (int ii = 0; ii < 4; ii++) {
	int iFace = pConf->a2iCellFace[i][ii];
	if (iFace >= SOUTH_FACE) {
	  aehF[ii] = aehFSouth[iFace - SOUTH_FACE];
	}
	else if (iFace >= NORTH_FACE) {
	  aehF[ii] = aehFNorth[iFace - NORTH_FACE];
	}
	else
	  aehF[ii] = aehFNew[iFace];
      }
      aehCNew[i] = createTet(aehF[1], aehF[2], aehF[3], aehF[0]);
#ifdef CLASSIFY
#ifdef OLD_TSTTR
      Assoc.setEntEntAssociation(ModMesh, GModel, aehCNew[i], oClassInt);
#else
      Assoc.setEntEntAssociation(aehCNew[i], oClassInt);
#endif
#endif
    }

#ifdef CLASSIFY
    // Now set face classifications
#ifdef OLD_TSTTR
    Assoc.setEntEntAssociation(ModMesh, GModel, aehFNorth[iNOrigVerts-1], oGeomFaceStart);
    Assoc.setEntEntAssociation(ModMesh, GModel, aehFSouth[iNOrigVerts-1], oGeomFaceStart);
    for (i = 0; i < 3*iNOrigVerts-8; i++) {
      Assoc.setEntEntAssociation(ModMesh, GModel, aehFNew[i], oClassInt);
    }
#else
    Assoc.setEntEntAssociation(aehFNorth[iNOrigVerts-1], oGeomFaceStart);
    Assoc.setEntEntAssociation(aehFSouth[iNOrigVerts-1], oGeomFaceStart);
    for (i = 0; i < 3*iNOrigVerts-8; i++) {
      Assoc.setEntEntAssociation(aehFNew[i], oClassInt);
    }
#endif
#endif

    // Validate the result
    for (i = 0; i < iNOrigVerts*2 - 4; i++) {
      // Check everything about the tet
      assert(qTetCheck(aehCNew[i]));
      // Every tet has to hit one pole or the other
      assert(qHasVert(aehCNew[i], ehVNorth) ||
	     qHasVert(aehCNew[i], ehVSouth));
      // Assert tetrahedron orientation
      assert(iOrient3D(aehCNew[i]) == 1);
    }

    for (i = 0; i < 3*iNOrigVerts - 8 ; i++)
      assert(qFaceCheck(aehFNew[i]));

    for (i = 0; i < iNOrigVerts; i++) {
      assert(qFaceCheck(aehFSouth[i]));
      assert(qFaceCheck(aehFNorth[i]));
    }

    // If recursion is permitted, check all the new faces for possible
    // swaps, as well as all of the faces on the outside of the orbit.
    int iRetVal = iNOrigCells-1;
    iNumBdryEdge ++;
    if (qAllowRecursion) {
      for (i = 0; i < 3*iNOrigCells-8; i++) {
	soRecursionQueue.insert(aehFNew[i]);
      }
      for (i = 0; i < iNOrigCells; i++) {
	soRecursionQueue.insert(aehFNorth[i]);
	soRecursionQueue.insert(aehFSouth[i]);
      }
    }
    return iRetVal;
  }
}

iBase_EntityHandle ITAPS_Swap::Swap3D::createFace(iBase_EntityHandle ehVertA,
						  iBase_EntityHandle ehVertB,
						  iBase_EntityHandle ehVertC)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehVertA, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertB, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertC, iMesh_POINT));
  iBase_EntityHandle aehVerts[] = {ehVertA, ehVertB, ehVertC};

  iBase_EntityHandle ehNewFace;
  int CS;
  iMesh_createEnt(pMeshInst, iMesh_TRIANGLE, aehVerts, 3, &ehNewFace,
		  &CS, &err);
  CHECK_ERR(err, pMeshInst);
  if (CS == iBase_CREATION_FAILED)
    throw(iBase_ENTITY_CREATION_ERROR);

  // This is actually a testable post-condition for the TSTTM
  // implementation, so this assertion should never fire anyway.
  assert(qConfirmTopo(pMeshInst, ehNewFace, iMesh_TRIANGLE));

  return ehNewFace;
}

iBase_EntityHandle ITAPS_Swap::Swap3D::createTet(iBase_EntityHandle ehFaceA,
						 iBase_EntityHandle ehFaceB,
						 iBase_EntityHandle ehFaceC,
						 iBase_EntityHandle ehFaceD)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehFaceA, iMesh_TRIANGLE));
  assert(qConfirmTopo(pMeshInst, ehFaceB, iMesh_TRIANGLE));
  assert(qConfirmTopo(pMeshInst, ehFaceC, iMesh_TRIANGLE));
  assert(qConfirmTopo(pMeshInst, ehFaceD, iMesh_TRIANGLE));
  iBase_EntityHandle aehFaces[] = {ehFaceA, ehFaceB, ehFaceC, ehFaceD};

  iBase_EntityHandle ehNewTet;
  int CS;
  iMesh_createEnt(pMeshInst, iMesh_TETRAHEDRON, aehFaces, 4, &ehNewTet,
		  &CS, &err);
  CHECK_ERR(err, pMeshInst);
  if (CS == iBase_CREATION_FAILED)
    throw(iBase_ENTITY_CREATION_ERROR);
  // This is actually a testable post-condition for the TSTTM
  // implementation, so this assertion should never fire anyway.
  assert(qConfirmTopo(pMeshInst, ehNewTet, iMesh_TETRAHEDRON));

  if (qUsingSet) vAddToSet(ehNewTet);  

  return ehNewTet;
}

iBase_EntityHandle ITAPS_Swap::Swap3D::createTetFromVerts
(iBase_EntityHandle ehVertA, iBase_EntityHandle ehVertB,
 iBase_EntityHandle ehVertC, iBase_EntityHandle ehVertD)
{
  int err = iBase_SUCCESS;
  assert(qConfirmTopo(pMeshInst, ehVertA, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertB, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertC, iMesh_POINT));
  assert(qConfirmTopo(pMeshInst, ehVertD, iMesh_POINT));
  iBase_EntityHandle aehVerts[] = {ehVertA, ehVertB, ehVertC, ehVertD};

  iBase_EntityHandle ehNewTet;
  int CS;
  iMesh_createEnt(pMeshInst, iMesh_TETRAHEDRON, aehVerts, 4, &ehNewTet,
		  &CS, &err);
  CHECK_ERR(err, pMeshInst);
  if (CS == iBase_CREATION_FAILED)
    throw(iBase_ENTITY_CREATION_ERROR);
  // This is actually a testable post-condition for the TSTTM
  // implementation, so this assertion should never fire anyway.
  assert(qConfirmTopo(pMeshInst, ehNewTet, iMesh_TETRAHEDRON));

  if (qUsingSet) vAddToSet(ehNewTet);  

  return ehNewTet;
}
