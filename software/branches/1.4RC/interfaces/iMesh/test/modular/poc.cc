#include <iostream>
#include <string>
#include "iMeshWrapper.hh"

using namespace std;
int main() {
  std::string refImplFiles[4];
  refImplFiles[0] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libiMesh_RefImpl.so";
  refImplFiles[1] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libRI_3D.so";
  refImplFiles[2] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libRI_2D.so";
  refImplFiles[3] = "/home/cfog/ITAPS/iMesh-refImpl/lib/libg/linux-gnu/libRI_base.so";
  iMeshWrapper IMW(refImplFiles, 4, true);
  IMW.report();
  iMesh_Instance instance;
  int err;

  IMW.newMesh("", &instance, &err, 0);
  iBase_EntitySetHandle rootSet;
  IMW.getRootSet(instance, &rootSet, &err);
  IMW.load(instance, rootSet, "../globe_mats.vtk", "", &err, 17, 0);

//   std::string MOABFiles[10];
//   MOABFiles[0] = "/tmp/BAP/moab-3.99/lib/libiMesh.so";
//   MOABFiles[1] = "/tmp/BAP/moab-3.99/lib/libMOAB.so";
//   iMeshWrapper IMW1(MOABFiles, 2, false);
//   IMW1.report();

//   std::string GRUMMPFiles[10];
//   GRUMMPFiles[0] = "/home/cfog/GRUMMP/branches/database-cleanup-2/lib/libO/linux-gnu/libiMesh_GRUMMP.so";
//   GRUMMPFiles[1] = "/home/cfog/GRUMMP/branches/database-cleanup-2/lib/libO/linux-gnu/libGR_3D.so";
//   GRUMMPFiles[2] = "/home/cfog/GRUMMP/branches/database-cleanup-2/lib/libO/linux-gnu/libGR_surf.so";
//   GRUMMPFiles[3] = "/home/cfog/GRUMMP/branches/database-cleanup-2/lib/libO/linux-gnu/libGR_2D.so";
//   GRUMMPFiles[4] = "/home/cfog/GRUMMP/branches/database-cleanup-2/lib/libO/linux-gnu/libGR_base.so";
//   GRUMMPFiles[5] = "/home/cfog/GRUMMP/branches/database-cleanup-2/lib/libO/linux-gnu/libGR_geom.so";
//   GRUMMPFiles[6] = "/home/cfog/cgm/lib/libcgm.so";
//   iMeshWrapper IMW2(GRUMMPFiles, 7, false);
//   IMW2.report();

#define QUOTE(a) #a
#define QUOTED_NAME(a) QUOTE(a)

  cout << "iMesh_getErrorType:  " <<QUOTED_NAME(iMesh_getErrorType)  << endl;
}
