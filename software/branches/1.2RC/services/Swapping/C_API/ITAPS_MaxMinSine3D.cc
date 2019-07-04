#include "ITAPS_QualMeasure.hh"

#include <math.h>
#include <assert.h>
#include <algorithm>

#define vCROSS3D(a,b,res) do { \
res[0] = a[1]*b[2] - a[2]*b[1]; \
res[1] = a[2]*b[0] - a[0]*b[2]; \
res[2] = a[0]*b[1] - a[1]*b[0]; \
} while(0)

#define vNORMALIZE3D(a) do {double dInvMag = 1./dMAG3D(a); \
a[0]*=dInvMag; a[1]*=dInvMag; a[2]*=dInvMag;} while(0)

#define dMAG3D(a) sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2])

#define adDIFF3D(a, b) {a[0]-b[0], a[1]-b[1], a[2]-b[2]}

static double dCrossMagSq3D(const double adA[3], const double adB[3])
{
  double adTmp[3];
  adTmp[0] = adA[1]*adB[2] - adA[2]*adB[1];
  adTmp[1] = adA[2]*adB[0] - adA[0]*adB[2];
  adTmp[2] = adA[0]*adB[1] - adA[1]*adB[0];

  return (adTmp[0]*adTmp[0] + adTmp[1]*adTmp[1] + adTmp[2]*adTmp[2]);
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
double
ITAPS_Swap::MaxMinSine3D::calcQuality (
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

  double adSineSq[] = {dCrossMagSq3D(adNorm0, adNorm1),
		      dCrossMagSq3D(adNorm0, adNorm2),
		      dCrossMagSq3D(adNorm0, adNorm3),
		      dCrossMagSq3D(adNorm1, adNorm2),
		      dCrossMagSq3D(adNorm1, adNorm3),
		      dCrossMagSq3D(adNorm2, adNorm3)};
  
  double dMinVal = std::min(std::min(adSineSq[0], adSineSq[1]),
      std::min(std::min(adSineSq[2], adSineSq[3]),
               std::min(adSineSq[4], adSineSq[5])));
  return (sqrt(dMinVal));
  // return (sqrt(*(std::min_element(adSineSq, adSineSq + 6)));
}

