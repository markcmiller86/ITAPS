#include "ITAPS_QualMeasure.hh"

#include <math.h>
#include <assert.h>
#include <algorithm>

#define dDOT3D(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])
#define dDIST3D(a, b) sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) +  (a[2]-b[2])*(a[2]-b[2]))
#define dDIST_SQ_3D(a, b) ((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) +  (a[2]-b[2])*(a[2]-b[2]))

#define vSWAP3D(a, b) do {register double adTemp__; \
adTemp__ = b[0]; b[0] = a[0]; a[0] = adTemp__; \
adTemp__ = b[1]; b[1] = a[1]; a[1] = adTemp__; \
adTemp__ = b[2]; b[2] = a[2]; a[2] = adTemp__; \
} while(0)

static void vSolve3By3(const double adRow1_in[3], const double adRow2_in[3],
		       const double adRow3_in[3],
		       double dRHS1, double dRHS2, double dRHS3,
		       double adRes[3])
{
  double adRow1[3];
  double adRow2[3];
  double adRow3[3];

  adRow1[0] = adRow1_in[0];
  adRow1[1] = adRow1_in[1];
  adRow1[2] = adRow1_in[2];
  adRow2[0] = adRow2_in[0];
  adRow2[1] = adRow2_in[1];
  adRow2[2] = adRow2_in[2];
  adRow3[0] = adRow3_in[0];
  adRow3[1] = adRow3_in[1];
  adRow3[2] = adRow3_in[2];

  /* Pivot first column */
  if (fabs(adRow1[0]) >= fabs(adRow2[0]) &&
      fabs(adRow1[0]) >= fabs(adRow3[0])) {}
  else if (fabs(adRow2[0]) >= fabs(adRow3[0])) {
    double dTemp = dRHS1;
    dRHS1 = dRHS2;
    dRHS2 = dTemp;
    vSWAP3D(adRow1, adRow2);
  }
  else {
    double dTemp = dRHS1;
    dRHS1 = dRHS3;
    dRHS3 = dTemp;
    vSWAP3D(adRow1, adRow3);
  }
    
  /* Eliminate first column */
  dRHS2 -= dRHS1 * adRow2[0]/adRow1[0];
  dRHS3 -= dRHS1 * adRow3[0]/adRow1[0];
  adRow2[1] = adRow2[1] - adRow1[1] * (adRow2[0]/adRow1[0]);
  adRow2[2] = adRow2[2] - adRow1[2] * (adRow2[0]/adRow1[0]);
  adRow3[1] = adRow3[1] - adRow1[1] * (adRow3[0]/adRow1[0]);
  adRow3[2] = adRow3[2] - adRow1[2] * (adRow3[0]/adRow1[0]);

  /* Pivot second column */
  if (fabs(adRow2[1]) >= fabs(adRow3[1])) {}
  else {
    double dTemp = dRHS2;
    dRHS2 = dRHS3;
    dRHS3 = dTemp;
    vSWAP3D(adRow2, adRow3);
  }

  /* Eliminate second column */
  dRHS3 -= dRHS2 * adRow3[1]/adRow2[1];
  adRow3[2] = adRow3[2] - adRow2[2] * (adRow3[1]/adRow2[1]);

  /* Solve for dRHS3 and back-substitute */
  adRes[2] = dRHS3 /= adRow3[2];
  dRHS2 -= adRow2[2] * dRHS3;
  dRHS1 -= adRow1[2] * dRHS3;

  /* Solve for dRHS2 and back-substitute */
  adRes[1] = dRHS2 /= adRow2[1];
  dRHS1 -= adRow1[1] * dRHS2;
  
  /* Solve for dRHS1 */
  adRes[0] = dRHS1 /= adRow1[0];
}

double
ITAPS_Swap::Delaunay3D::calcQuality (
  /* in */ const double coords[],
  /* in */ const int coords_size ) const
  throw (iBase_ErrorType)
{
  // Actually going to calculate ratio of shortest edge length to
  // circumradius.  This isn't exactly equivalent to Delaunay, but it's
  // not a -bad- approximation.

  assert(coords_size == 12);
  // Create aliases for all the vert coordinates
  const double * const &  adA = coords;
  const double * const &  adB = adA + 3;
  const double * const &  adC = adA + 6;
  const double * const &  adD = adA + 9;
  
  double adCircCent[3];
  {
    double adRow1[] = {adA[0] - adB[0],
		       adA[1] - adB[1],
		       adA[2] - adB[2]};
    double adRow2[] = {adA[0] - adC[0],
		       adA[1] - adC[1],
		       adA[2] - adC[2]};
    double adRow3[] = {adA[0] - adD[0],
		       adA[1] - adD[1],
		       adA[2] - adD[2]};
    
    double dRHS1 = 0.5 * (dDOT3D(adA, adA) - dDOT3D(adB, adB));
    double dRHS2 = 0.5 * (dDOT3D(adA, adA) - dDOT3D(adC, adC));
    double dRHS3 = 0.5 * (dDOT3D(adA, adA) - dDOT3D(adD, adD));
    
    vSolve3By3(adRow1, adRow2, adRow3, dRHS1, dRHS2, dRHS3, adCircCent);
  }
  
  double dShortestLen;
  {
    double adLenSq[] = {dDIST_SQ_3D(adA, adB), dDIST_SQ_3D(adA, adC),
			dDIST_SQ_3D(adA, adD), dDIST_SQ_3D(adB, adC),
			dDIST_SQ_3D(adB, adD), dDIST_SQ_3D(adC, adD)};
    
    double dShortestSq = *(std::min_element(adLenSq, adLenSq + 6));
    dShortestLen = sqrt(dShortestSq);
  }
  double dRadius = dDIST3D(adCircCent, adA);
  
  return(sqrt(0.375) * dShortestLen / dRadius);
}
