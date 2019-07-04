#include <iostream>
#include <string>
#include "iMeshWrapper.hh"
#include "iMeshTester.hh"

using namespace std;
int main() {
  std::string refImplFiles[4];
  refImplFiles[0] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libiMesh_RefImpl.so";
  refImplFiles[1] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libRI_3D.so";
  refImplFiles[2] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libRI_2D.so";
  refImplFiles[3] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libRI_base.so";
  iMeshWrapper IMW_RI(refImplFiles, 4, true);

  {
    iMesh_Instance instance;
    int err;
    IMW_RI.newMesh("", &instance, &err, 0);
    iBase_EntitySetHandle rootSet;
    IMW_RI.getRootSet(instance, &rootSet, &err);
    IMW_RI.load(instance, rootSet, "../globe_mats.vtk", "", &err, 17, 0);
    IMW_RI.initWrapper(instance, rootSet);
  }

  std::string MOABFiles[10];
  MOABFiles[0] = "/usr/local/itaps/lib/libiMesh.so.0.0.0";
  MOABFiles[1] = "/usr/local/itaps/lib/libMOAB.so.0.0.0";
  iMeshWrapper IMW_test(MOABFiles, 2, false);

  {
    iMesh_Instance instance;
    int err;
    IMW_test.newMesh("", &instance, &err, 0);
    iBase_EntitySetHandle rootSet;
    IMW_test.getRootSet(instance, &rootSet, &err);
    IMW_test.load(instance, rootSet, "../globe_mats.vtk", "", &err, 17, 0);
    IMW_test.initWrapper(instance, rootSet);
  }

  iMeshTester MT(&IMW_test, &IMW_RI);
  MT.test_newMesh_dtor("moab");
  MT.test_getDescription();
  MT.test_getRootSet();
  MT.test_getGeometricDimension();
  MT.test_getDfltStorage();
  MT.test_getAdjTable();
  MT.test_getNumOfTopo();
  MT.test_getNumOfType();

  MT.test_getEntAdj();
}
