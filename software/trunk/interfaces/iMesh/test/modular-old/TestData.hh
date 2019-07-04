#ifndef TESTDATA_HH_
#define TESTDATA_HH_

#include <stddef.h>

#include "iBase.h"
#include "iMesh.h"

class TestData {
public:
  TestData()
  {
  }
  virtual ~TestData()
  {
  }
  virtual int geomDim() = 0;
  virtual int numOfType(const int type) = 0;
  virtual int numOfTopo(const int topo) = 0;
  virtual double coords(const int vert, const int dim) = 0;
};

#endif /*TESTDATA_HH_*/
