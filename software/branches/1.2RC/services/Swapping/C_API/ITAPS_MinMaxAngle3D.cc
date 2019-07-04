#include "ITAPS_QualMeasure.hh"

#include <math.h>
#include <assert.h>
#include <algorithm>

#define dDOT3D(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])

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

double
ITAPS_Swap::MinMaxAngle3D::calcQuality (
  /* in */ const double coords[],
  /* in */ const int coords_size ) const
  throw (iBase_ErrorType)
{
  assert(coords_size == 12);
  // Create aliases for all the vert coordinates
  const double * const &  adA = coords;
  const double * const &  adB = adA + 3;
  const double * const &  adC = adA + 6;
  const double * const &  adD = adA + 9;
  
  double adNorm0[3], adNorm1[3], adNorm2[3], adNorm3[3];
  vUnitNormal(adC, adB, adD, adNorm0);
  vUnitNormal(adA, adC, adD, adNorm1);
  vUnitNormal(adB, adA, adD, adNorm2);
  vUnitNormal(adA, adB, adC, adNorm3);

  double dDot, adDiheds[6];

  dDot = -dDOT3D(adNorm0, adNorm1);
  adDiheds[0] = my_acos(dDot); // Edge CD

  dDot = -dDOT3D(adNorm0, adNorm2);
  adDiheds[1] = my_acos(dDot); // Edge BD

  dDot = -dDOT3D(adNorm0, adNorm3);
  adDiheds[2] = my_acos(dDot); // Edge BC

  dDot = -dDOT3D(adNorm1, adNorm2);
  adDiheds[3] = my_acos(dDot); // Edge AD

  dDot = -dDOT3D(adNorm1, adNorm3);
  adDiheds[4] = my_acos(dDot); // Edge AC

  dDot = -dDOT3D(adNorm2, adNorm3);
  adDiheds[5] = my_acos(dDot); // Edge AB
  
  return *(std::max_element(adDiheds, adDiheds + 6));
}

