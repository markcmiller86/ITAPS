#include <stdlib.h>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

#include "iMesh.h"
#include "iBase.h"
#include "ITAPS_Swap.hh"

static ITAPS_Swap::Swap3D S3D;
static iMesh_Instance Mesh;
static int iTotalSwaps = 0;
static double dTotalTime = 0;

/// @cond ShowInternals
struct VertTuple {
  iBase_EntityHandle vert0, vert1, vert2, vert3;
  VertTuple(iBase_EntityHandle v0,
	     iBase_EntityHandle v1,
	     iBase_EntityHandle v2,
	     iBase_EntityHandle v3 = NULL)
  { vert0 = v0; vert1 = v1; vert2 = v2; vert3 = v3;}
  friend bool operator<(VertTuple VT1, VertTuple VT2)
  {
    return (VT1.vert0 < VT2.vert0 ||
	    (VT1.vert0 == VT2.vert0 && VT1.vert1 < VT2.vert1) ||
	    (VT1.vert0 == VT2.vert0 && VT1.vert1 == VT2.vert1 &&
	     VT1.vert2 < VT2.vert2) ||
	    (VT1.vert0 == VT2.vert0 && VT1.vert1 == VT2.vert1 &&
	     VT1.vert2 == VT2.vert2 && VT1.vert3 < VT2.vert3));
  }
};
/// @endcond

static void vSetupTest(iBase_EntitySetHandle *ptr_root_set,
		       std::string strFilename)
{
  S3D.setSwapRecursion(true);
  S3D.setEdgeSwapping(true);
  S3D.setBdryMutable(true);
  
  int iErr = iBase_SUCCESS;
  iMesh_newMesh("3D", &Mesh, &iErr, 2);
  
  S3D.setMeshData(Mesh);
  
  // This is guaranteed to work now.
  iMesh_getRootSet(Mesh, ptr_root_set, &iErr);
  CHECK_ERR(iErr, Mesh);
  
#ifndef CREATE_FROM_FACES
  S3D.createFromVertsOnly(true);
#endif

  int adj_table[] = {iBase_AVAILABLE, 0, iBase_ALL_ORDER_1, iBase_ALL_ORDER_1,
		     0, 0, 0, 0,
		     iBase_ALL_ORDER_1, 0, iBase_AVAILABLE, iBase_ALL_ORDER_1,
		     iBase_ALL_ORDER_1, 0, iBase_ALL_ORDER_1, iBase_AVAILABLE};
  iMesh_setGeometricDimension(Mesh, 3, &iErr);
  CHECK_ERR(iErr, Mesh);
  iMesh_setAdjTable(Mesh, adj_table, 16, &iErr);
  CHECK_ERR(iErr, Mesh);

  iMesh_load(Mesh, *ptr_root_set, strFilename.data(), "", &iErr, strFilename.size(), 0);
  CHECK_ERR(iErr, Mesh);
  
#ifdef CLASSIFY
  TSTTG::Model Model = TSTTG::Factory::newModel("");
  TSTTR::Infer Infer = TSTTR::Factory::newAssoc("");
  
  Infer.createAssociation(Mesh, Model);
#ifdef OLD_TSTTR
  Infer.inferAllAssociations(Mesh, Model);
#else
  Infer.inferAllAssociations();
#endif
  
  S3D.setModelData(Model);
  S3D.setAssocData(Infer);
#endif
  
}

static void vRunTest(iBase_EntitySetHandle entity_set,
		     ITAPS_Swap::SwapType ST,
		     iBase_TagHandle *ptr_tag = NULL)
{
  S3D.setSwapType(ST);
  dTotalTime -= clock();
  try {
    int iRes = S3D.swapAll(entity_set, ptr_tag);
    iTotalSwaps += iRes;
    dTotalTime += clock();
    
    cout << "Performed " << iRes << " swaps." << endl;
  } catch (iBase_ErrorType err) {
    exit(err);
  }
#if 0
  catch (...) {
    cout << "Caught some non-iBase_ErrorType error!" << endl;
    exit(-1);
  }
#endif 
}

static void vFinish()
{
  int err;
  iMesh_dtor(Mesh, &err);
}

int main(int argc, char *argv[] )
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <mesh_filename>" << endl;
    return 1;
  }
  std::string strFilename(argv[1]);
  iBase_EntitySetHandle root_set;
  
  int iNumTrips = 1;
  if (argc > 2) iNumTrips = 10;
  double dElapsed = 0;
  int iTrips = 0;
  while (iTrips < iNumTrips) {
    iTrips++;
    vSetupTest(&root_set, strFilename);
    vRunTest(root_set, ITAPS_Swap::SwapType_DELAUNAY);
    vRunTest(root_set, ITAPS_Swap::SwapType_MAX_MIN_SINE);
    vFinish();
  }
  
  cout << "Ave. elapsed time was " << dTotalTime / CLOCKS_PER_SEC / iTrips
       << " sec" << endl;
  cout << "Swapping rate was " << iTotalSwaps / (dTotalTime / CLOCKS_PER_SEC)
       << " swaps per second" << endl;

  if (argc == 3) {
    std::string strTests(argv[2]);
    if (strTests.find("t") != std::string::npos) {
      std::cout << std::endl << std::endl
		<< "==================================================="
		<< std::endl 
		<< "============= Swapping test with tags ============="
		<< std::endl 
		<< "==================================================="
		<< std::endl << std::endl;

      // Do tag test.

      // A handful of tagged faces disappear, because edge swapping
      // sometimes sucks them up.  For rand1, it's something like 9 out
      // of 9034, which is miniscule.

      // Set up the vanilla test again...
      vSetupTest(&root_set, strFilename);

      // Set up tag
      int err;
      int iTagged = 0, iUntagged = 0;
      iBase_TagHandle tag;
      iMesh_createTag(Mesh, "swappable faces", 1, iBase_INTEGER,
		      &tag, &err, 15);
      CHECK_ERR(err, Mesh);
      
      // Tag all faces in the range [0..0.5]^2
      // Create a set of all faces outside that range (as vertex triples).
      std::set<struct VertTuple> sVT;

      iBase_EntityIterator iter;
      iBase_EntityHandle entity_handle;
      int qMoreData;
      iMesh_initEntIter(Mesh, root_set, iBase_FACE, iMesh_ALL_TOPOLOGIES, 0,
			&iter, &err);
      CHECK_ERR(err, Mesh);

      iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
      CHECK_ERR(err, Mesh);
      while (qMoreData) {
	static iBase_EntityHandle *vert_handles = new iBase_EntityHandle[4];
	static double *vertCoords = new double[9];
	int vert_handles_size = 0, vert_handles_allocated = 4;
	int vertCoords_size = 0, vertCoords_allocated = 9;
	iMesh_getEntAdj(Mesh, entity_handle, iBase_VERTEX,
			&vert_handles, &vert_handles_allocated,
			&vert_handles_size, &err);
	CHECK_ERR(err, Mesh);
	if (vert_handles_size == 3) {
	  int SO = iBase_INTERLEAVED;
	  iMesh_getVtxArrCoords(Mesh, vert_handles, vert_handles_size,
				SO, &vertCoords, &vertCoords_allocated,
				&vertCoords_size, &err);
	  CHECK_ERR(err, Mesh);
	  
	  double centroidCoords[3];
	  centroidCoords[0] = (vertCoords[0] + vertCoords[3] + vertCoords[6])/3;
	  centroidCoords[1] = (vertCoords[1] + vertCoords[4] + vertCoords[7])/3;
	  centroidCoords[2] = (vertCoords[2] + vertCoords[5] + vertCoords[8])/3;
	  
	  if (centroidCoords[0] < 0 && centroidCoords[1] < 0 &&
	      centroidCoords[2] < 0) {
	    iMesh_setIntData(Mesh, entity_handle, tag, 1, &err);
	    CHECK_ERR(err, Mesh);
	    iTagged++;
	  }
	  else {
	    sVT.insert(VertTuple(vert_handles[0],
				 vert_handles[1],
				 vert_handles[2]));
	    iUntagged++;
	  }
	}
	iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
	CHECK_ERR(err, Mesh);
      }
      iMesh_resetEntIter(Mesh, iter, &err);
      CHECK_ERR(err, Mesh);

      std::cout << iTagged << " faces were tagged.  "
		<< iUntagged << " weren't." << std::endl;

      // Turn off recursion
      S3D.setSwapRecursion(false);
      
      // Swap
      vRunTest(root_set, ITAPS_Swap::SwapType_DELAUNAY, &tag);
      
      // Check that all faces outside the original region still exist
      iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
      CHECK_ERR(err, Mesh);
      while (qMoreData) {
	iBase_EntityHandle *vert_handles = new iBase_EntityHandle[4];
	double *vertCoords = new double[9];
	int vert_handles_size = 0, vert_handles_allocated = 4;
	int vertCoords_size = 0, vertCoords_allocated = 9;
	iMesh_getEntAdj(Mesh, entity_handle, iBase_VERTEX,
			&vert_handles, &vert_handles_allocated,
			&vert_handles_size, &err);
	CHECK_ERR(err, Mesh);
	if (vert_handles_size == 3) {
	  int SO = iBase_INTERLEAVED;
	  iMesh_getVtxArrCoords(Mesh, vert_handles, vert_handles_size,
				SO, &vertCoords, &vertCoords_allocated,
				&vertCoords_size, &err);
	  CHECK_ERR(err, Mesh);
	  
	  double centroidCoords[3];
	  centroidCoords[0] = (vertCoords[0] + vertCoords[3] + vertCoords[6])/3;
	  centroidCoords[1] = (vertCoords[1] + vertCoords[4] + vertCoords[7])/3;
	  centroidCoords[2] = (vertCoords[2] + vertCoords[5] + vertCoords[8])/3;
	  
	  if (centroidCoords[0] < 0 && centroidCoords[1] < 0 &&
	      centroidCoords[2] < 0) {
	    // Do nothing
	  }
	  else {
	    VertTuple VT(vert_handles[0], vert_handles[1], vert_handles[2]);
	    if (sVT.find(VT) == sVT.end()) {
	      // Not found!
	      std::cout << "New face that wasn't found: " << vert_handles[0] << " "
			<< vert_handles[1] << " "<< vert_handles[2] << std::endl;
	    }
	    else {
	      sVT.erase(VT);
	    }
	  }
	}
	iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
	CHECK_ERR(err, Mesh);
      }
      std::cout << "Number of untagged faces that vanished: "
		<< sVT.size() << std::endl;
      std::set<VertTuple>::iterator setIter = sVT.begin();
      for ( ; setIter != sVT.end() ; setIter++) {
	VertTuple VT = *setIter;
	std::cout << "Old face that wasn't found: " << VT.vert0 << " "
		  << VT.vert1 << " "<< VT.vert2 << std::endl;
      }
    }
    else if (strTests.find("s") != std::string::npos) {
      // Do set test.
      std::cout << std::endl << std::endl
		<< "==================================================="
		<< std::endl 
		<< "============= Swapping test with sets ============="
		<< std::endl 
		<< "==================================================="
		<< std::endl << std::endl;
      // Set up the vanilla test again...
      vSetupTest(&root_set, strFilename);

      // Set up set
      int err;
      int iInSet = 0, iNotInSet = 0;
      iBase_EntitySetHandle entity_set;
      iMesh_createEntSet(Mesh, false, &entity_set, &err);
      
      // All tets with centroids in the range [0..0.5]^2 go in a set.
      // Create a std::set of all faces outside that range (as vertex
      // quadruples).
      std::set<struct VertTuple> sVT;

      iBase_EntityIterator iter;
      iBase_EntityHandle entity_handle;
      int qMoreData;
      iMesh_initEntIter(Mesh, root_set, iBase_REGION, iMesh_ALL_TOPOLOGIES, 0,
			&iter, &err);
      CHECK_ERR(err, Mesh);

      iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
      CHECK_ERR(err, Mesh);
      while (qMoreData) {
	static iBase_EntityHandle *vert_handles = new iBase_EntityHandle[8];
	static double *vertCoords = new double[12];
	int vert_handles_size = 0, vert_handles_allocated = 8;
	int vertCoords_size = 0, vertCoords_allocated = 12;
	iMesh_getEntAdj(Mesh, entity_handle, iBase_VERTEX,
			&vert_handles, &vert_handles_allocated,
			&vert_handles_size, &err);
	CHECK_ERR(err, Mesh);
	if (vert_handles_size == 4) {
	  int SO = iBase_INTERLEAVED;
	  iMesh_getVtxArrCoords(Mesh, vert_handles, vert_handles_size,
				SO, &vertCoords, &vertCoords_allocated,
				&vertCoords_size, &err);
	  CHECK_ERR(err, Mesh);
	  
	  double centroidCoords[3];
	  centroidCoords[0] = (vertCoords[0] + vertCoords[3] +
			       vertCoords[6] + vertCoords[9])/4;
	  centroidCoords[1] = (vertCoords[1] + vertCoords[4] +
			       vertCoords[7] + vertCoords[10])/4;
	  centroidCoords[2] = (vertCoords[2] + vertCoords[5] +
			       vertCoords[8] + vertCoords[11])/4;
	  
	  if (centroidCoords[0] < 0 && centroidCoords[1] < 0 &&
	      centroidCoords[2] < 0) {
	    iMesh_addEntToSet(Mesh, entity_handle, entity_set, &err);
	    CHECK_ERR(err, Mesh);
	    iInSet++;
	  }
	  else {
	    sVT.insert(VertTuple(vert_handles[0],
				 vert_handles[1],
				 vert_handles[2],
				 vert_handles[3]));
	    iNotInSet++;
	  }
	}
	iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
	CHECK_ERR(err, Mesh);
      }
      iMesh_resetEntIter(Mesh, iter, &err);
      CHECK_ERR(err, Mesh);

      std::cout << iInSet << " tets in the set.  "
		<< iNotInSet << " aren't." << std::endl;

      // Turn on recursion
      S3D.setSwapRecursion(true);

      // Swap
      vRunTest(entity_set, ITAPS_Swap::SwapType_DELAUNAY);

      // Check that all tets outside the original region still exist
      iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
      CHECK_ERR(err, Mesh);
      while (qMoreData) {
	iBase_EntityHandle *vert_handles = new iBase_EntityHandle[8];
	double *vertCoords = new double[12];
	int vert_handles_size = 0, vert_handles_allocated = 8;
	int vertCoords_size = 0, vertCoords_allocated = 12;
	iMesh_getEntAdj(Mesh, entity_handle, iBase_VERTEX,
			&vert_handles, &vert_handles_allocated,
			&vert_handles_size, &err);
	CHECK_ERR(err, Mesh);
	if (vert_handles_size == 4) {
	  int SO = iBase_INTERLEAVED;
	  iMesh_getVtxArrCoords(Mesh, vert_handles, vert_handles_size,
				SO, &vertCoords, &vertCoords_allocated,
				&vertCoords_size, &err);
	  CHECK_ERR(err, Mesh);
	  
	  double centroidCoords[3];
	  centroidCoords[0] = (vertCoords[0] + vertCoords[3] +
			       vertCoords[6] + vertCoords[9])/4;
	  centroidCoords[1] = (vertCoords[1] + vertCoords[4] +
			       vertCoords[7] + vertCoords[10])/4;
	  centroidCoords[2] = (vertCoords[2] + vertCoords[5] +
			       vertCoords[8] + vertCoords[11])/4;
	  
	  if (centroidCoords[0] < 0 && centroidCoords[1] < 0 &&
	      centroidCoords[2] < 0) {
	    // Do nothing
	  }
	  else {
	    VertTuple VT(vert_handles[0], vert_handles[1], vert_handles[2],
			 vert_handles[3]);
	    if (sVT.find(VT) == sVT.end()) {
	      // Not found!
	      std::cout << "New tet that wasn't found: " << VT.vert0 << " "
			<< VT.vert1 << " "<< VT.vert2 << " " << VT.vert3
			<< std::endl;
	    }
	    else {
	      sVT.erase(VT);
	    }
	  }
	}
	iMesh_getNextEntIter(Mesh, iter, &entity_handle, &qMoreData, &err);
	CHECK_ERR(err, Mesh);
      }
      std::cout << "Number of tets not in the set that vanished: "
		<< sVT.size() << std::endl;
      std::set<VertTuple>::iterator setIter = sVT.begin();
      for ( ; setIter != sVT.end() ; setIter++) {
	VertTuple VT = *setIter;
	std::cout << "Old tet that wasn't found: " << VT.vert0 << " "
		  << VT.vert1 << " "<< VT.vert2 << " " << VT.vert3
		  << std::endl;
      }
    }
  }
  return 0;
}
