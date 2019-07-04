#ifndef TETTESTDATA_HH_
#define TETTESTDATA_HH_

#include <assert.h>
#include "TestData.hh"

class TetTestData : public TestData {
private:
  int numVerts, numEdges, numTris, numTets;
  double (*coordData)[3];
public:
  TetTestData(const char fileName[]);
  virtual ~TetTestData();
  virtual int geomDim();
  virtual int numOfType(const int type);
  virtual int numOfTopo(const int topo);
  virtual double coords(const int vert, const int dim);
};

#endif /*TETTESTDATA_HH_*/
