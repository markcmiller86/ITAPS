#include "iMeshTest.hh"
#include "iMesh.h"
#include "TetTestData.hh"

const char *tetDataFile = "meshData";
const char *tetMeshFile = "meshFile.vtk";

int main() 
{
  iMeshTest Tester;
  TetTestData Data(tetDataFile);
  
  Tester.provideTestData(&Data);
  Tester.newMesh("3D");
  Tester.getRootSet();
  Tester.load(tetMeshFile, "3D");
  Tester.getDfltStorage();
  Tester.getAdjTable();
  Tester.getGeometricDimension();
  for (int i = 0; i < iMesh_ALL_TOPOLOGIES; i++) {
    Tester.getNumOfTopo(i);
  }
  for (int i = 0; i < iBase_ALL_TYPES; i++) {
    Tester.getNumOfType(i);
  }
  Tester.getAllVtxCoords(iBase_INTERLEAVED);
  Tester.printSummary();
}
