// 
// File:          TSTT_Swap_MaxMinSine3D_Impl.cc
// Symbol:        TSTT_Swap.MaxMinSine3D-v0.7
// Symbol Type:   class
// Babel Version: 0.10.10
// Description:   Server-side implementation for TSTT_Swap.MaxMinSine3D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.10
// 
#include "TSTT_Swap_MaxMinSine3D_Impl.hh"

// DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D._includes)
#include <math.h>
#include <assert.h>

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define vCROSS3D(a,b,res) do { \
res[0] = a[1]*b[2] - a[2]*b[1]; \
res[1] = a[2]*b[0] - a[0]*b[2]; \
res[2] = a[0]*b[1] - a[1]*b[0]; \
} while(0)

#define vNORMALIZE3D(a) do {double dInvMag = 1./dMAG3D(a); \
a[0]*=dInvMag; a[1]*=dInvMag; a[2]*=dInvMag;} while(0)

#define dMAG3D(a) sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2])

#define adDIFF3D(a, b) {a[0]-b[0], a[1]-b[1], a[2]-b[2]}

static double dCrossMag3D(const double adA[3], const double adB[3])
{
  double adTmp[3];
  adTmp[0] = adA[1]*adB[2] - adA[2]*adB[1];
  adTmp[1] = adA[2]*adB[0] - adA[0]*adB[2];
  adTmp[2] = adA[0]*adB[1] - adA[1]*adB[0];

  return sqrt(adTmp[0]*adTmp[0] + adTmp[1]*adTmp[1] + adTmp[2]*adTmp[2]);
}

static void vUnitNormal(const double adA[3], const double adB[3],
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
// DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D._includes)

// user-defined constructor.
void TSTT_Swap::MaxMinSine3D_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D._ctor)
  // Insert-Code-Here {TSTT_Swap.MaxMinSine3D._ctor} (constructor)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D._ctor)
}

// user-defined destructor.
void TSTT_Swap::MaxMinSine3D_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D._dtor)
  // Insert-Code-Here {TSTT_Swap.MaxMinSine3D._dtor} (destructor)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D._dtor)
}

// static class initializer.
void TSTT_Swap::MaxMinSine3D_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D._load)
  // Insert-Code-Here {TSTT_Swap.MaxMinSine3D._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D._load)
}

// user-defined static methods: (none)

// user-defined non-static methods:
/**
 * Method:  shouldMinimize[]
 */
bool
TSTT_Swap::MaxMinSine3D_impl::shouldMinimize ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D.shouldMinimize)
  return false;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D.shouldMinimize)
}

/**
 * Method:  calcQuality[]
 */
double
TSTT_Swap::MaxMinSine3D_impl::calcQuality (
  /* in */ ::sidl::array<double> coords,
  /* in */ int32_t coords_size ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D.calcQuality)
  assert(coords_size == 12);
#ifndef NDEBUG
  assert(sidlArrayDim(coords._get_ior()) == 1);
  assert(sidlStride(coords._get_ior(), 0) == 1);
#endif
  // Create aliases for all the vert coordinates
  const double * const &  adA = sidlArrayAddr1(coords._get_ior(), 0);
  const double * const &  adB = adA + 3;
  const double * const &  adC = adA + 6;
  const double * const &  adD = adA + 9;

  double adNorm0[3], adNorm1[3], adNorm2[3], adNorm3[3];
  vUnitNormal(adC, adB, adD, adNorm0);
  vUnitNormal(adA, adC, adD, adNorm1);
  vUnitNormal(adB, adA, adD, adNorm2);
  vUnitNormal(adA, adB, adC, adNorm3);

  double dSineCD = dCrossMag3D(adNorm0, adNorm1);
  double dSineBD = dCrossMag3D(adNorm0, adNorm2);
  double dSineBC = dCrossMag3D(adNorm0, adNorm3);
  double dSineAD = dCrossMag3D(adNorm1, adNorm2);
  double dSineAC = dCrossMag3D(adNorm1, adNorm3);
  double dSineAB = dCrossMag3D(adNorm2, adNorm3);
  assert(dSineCD > 0);
  assert(dSineBD > 0);
  assert(dSineBC > 0);
  assert(dSineAD > 0);
  assert(dSineAC > 0);
  assert(dSineAB > 0);

  return MIN(MIN(MIN(dSineCD, dSineBD), MIN(dSineBC, dSineAD)),
	     MIN(dSineAC, dSineAB));
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D.calcQuality)
}


// DO-NOT-DELETE splicer.begin(TSTT_Swap.MaxMinSine3D._misc)
// Insert-Code-Here {TSTT_Swap.MaxMinSine3D._misc} (miscellaneous code)
// DO-NOT-DELETE splicer.end(TSTT_Swap.MaxMinSine3D._misc)

