// 
// File:          TSTT_Swap_MinMaxAngle3D_Impl.cc
// Symbol:        TSTT_Swap.MinMaxAngle3D-v0.7
// Symbol Type:   class
// Babel Version: 0.10.10
// Description:   Server-side implementation for TSTT_Swap.MinMaxAngle3D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.10
// 
#include "TSTT_Swap_MinMaxAngle3D_Impl.hh"

// DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D._includes)
#include <math.h>
#include <assert.h>

#define dDOT3D(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#define dMAG3D(a) sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2])

#define adDIFF3D(a, b) {a[0]-b[0], a[1]-b[1], a[2]-b[2]}

#define vCROSS3D(a,b,res) do { \
res[0] = a[1]*b[2] - a[2]*b[1]; \
res[1] = a[2]*b[0] - a[0]*b[2]; \
res[2] = a[0]*b[1] - a[1]*b[0]; \
} while(0)

#define vNORMALIZE3D(a) do {double dInvMag = 1./dMAG3D(a); \
a[0]*=dInvMag; a[1]*=dInvMag; a[2]*=dInvMag;} while(0)

static void vUnitNormal(const double adA[3], const double adB[3],
			const double adC[3], double adRes[3])
{
  double adDiffBA[3] = adDIFF3D(adB, adA);
  double adDiffCB[3] = adDIFF3D(adC, adB);
  vCROSS3D(adDiffCB, adDiffBA, adRes);
  vNORMALIZE3D(adRes);
}

static double my_acos(const double dArg)
  // Result is in degrees.
{
  static const double dScale = 180/M_PI;
  if (dArg > 1) return 90;
  else if (dArg < -1) return -90;
  else return acos(dArg)*dScale;
}

// DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D._includes)

// user-defined constructor.
void TSTT_Swap::MinMaxAngle3D_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D._ctor)
  // Insert-Code-Here {TSTT_Swap.MinMaxAngle3D._ctor} (constructor)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D._ctor)
}

// user-defined destructor.
void TSTT_Swap::MinMaxAngle3D_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D._dtor)
  // Insert-Code-Here {TSTT_Swap.MinMaxAngle3D._dtor} (destructor)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D._dtor)
}

// static class initializer.
void TSTT_Swap::MinMaxAngle3D_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D._load)
  // Insert-Code-Here {TSTT_Swap.MinMaxAngle3D._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D._load)
}

// user-defined static methods: (none)

// user-defined non-static methods:
/**
 * Method:  shouldMinimize[]
 */
bool
TSTT_Swap::MinMaxAngle3D_impl::shouldMinimize ()
throw ( 
  ::TSTTB::Error
)
{
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D.shouldMinimize)
  return true;
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D.shouldMinimize)
}

/**
 * Method:  calcQuality[]
 */
double
TSTT_Swap::MinMaxAngle3D_impl::calcQuality (
  /* in */ ::sidl::array<double> coords,
  /* in */ int32_t coords_size ) 
throw ( 
  ::TSTTB::Error
){
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D.calcQuality)
  assert(coords_size == 12);
  assert(sidlArrayDim(coords._get_ior()) == 1);
  assert(sidlStride(coords._get_ior(), 0) == 1);
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

  double dDot = -dDOT3D(adNorm0, adNorm1);
  double dDihedCD = my_acos(dDot); // Edge CD

  dDot = -dDOT3D(adNorm0, adNorm2);
  double dDihedBD = my_acos(dDot); // Edge BD

  dDot = -dDOT3D(adNorm0, adNorm3);
  double dDihedBC = my_acos(dDot); // Edge BC

  dDot = -dDOT3D(adNorm1, adNorm2);
  double dDihedAD = my_acos(dDot); // Edge AD

  dDot = -dDOT3D(adNorm1, adNorm3);
  double dDihedAC = my_acos(dDot); // Edge AC

  dDot = -dDOT3D(adNorm2, adNorm3);
  double dDihedAB = my_acos(dDot); // Edge AB
  
  return MAX(MAX(MAX(dDihedAB, dDihedAC), MAX(dDihedAD, dDihedBC)),
	     MAX(dDihedBD, dDihedCD));
  // DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D.calcQuality)
}


// DO-NOT-DELETE splicer.begin(TSTT_Swap.MinMaxAngle3D._misc)
// Insert-Code-Here {TSTT_Swap.MinMaxAngle3D._misc} (miscellaneous code)
// DO-NOT-DELETE splicer.end(TSTT_Swap.MinMaxAngle3D._misc)

