#include "TetTestData.hh"

TetTestData::TetTestData(const char fileName[]) :
  numVerts(0), numEdges(0), numTris(0), numTets(0), coordData(NULL)
{
  // Actually read this data from a file in the near future...
  numVerts = 4;
  numEdges = 6;
  numTris = 4;
  numTets = 1;
  coordData = new double[numVerts][3];
}

TetTestData::~TetTestData()
{
  delete [] coordData;
}

int TetTestData::geomDim()
{
  return 3;
}

int TetTestData::numOfType(const int type)
{
  switch (type) {
  case iBase_VERTEX:
    return numVerts;
  case iBase_EDGE:
    return numEdges;
  case iBase_FACE:
    return numTris;
  case iBase_REGION:
    return numTets;
  case iBase_ALL_TYPES:
    return numVerts + numEdges + numTris + numTets;
  default:
    assert(0);
    return 0;
  }
}

int TetTestData::numOfTopo(const int type)
{
  switch (type) {
  case iMesh_POINT:
    return numVerts;
  case iMesh_LINE_SEGMENT:
    return numEdges;
  case iMesh_TRIANGLE:
    return numTris;
  case iMesh_TETRAHEDRON:
    return numTets;
  case iMesh_ALL_TOPOLOGIES:
    return numVerts + numEdges + numTris + numTets;
  default:
    return 0;
  }
}

double TetTestData::coords(int vert, int dim)
{
  assert(vert >= 0 && vert < numVerts);
  assert(dim >= 0 && dim < 3);
  return coordData[vert][dim];
}
