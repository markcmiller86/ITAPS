/*****************************************************************************
 * Zoltan Library for Parallel Applications                                  *
 * Copyright (c) 2000,2001,2002, Sandia National Laboratories                *
 * For more info, see the README file in the top-level Zoltan directory      *
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile: ITAPSZoltanTest.cpp,v $
 *    $Author: vjleung $
 *    $Date: 2011/04/01 22:24:32 $
 *    $Revision: 1.26 $
 *****************************************************************************/

//
//  C++ example of Zoltan library use with ITAPS by Vitus Leung
//

#include "iMesh.h"
#include "ITAPSZoltan.hpp"

#include <iostream>

#define RR if (0 != result) return result

static iBase_EntitySetHandle root_set;

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (argc < 5) {
    cout << "[" << rank << "] Usage: " << argv[0]
	 << " <# partitions> <mesh_file> "
	 << "<entity type (VERTEX/EDGE/FACE/REGION)> <output_mesh_file> "
	 << "[<method(Block/Random/RCB/RIB/HSFC/Graph/Hypergraph)>] "
	 << "[<(Graph/Hypergraph)_approach(Partition/Repartion/Refine)>] "
	 << "<Graph_package(PHG/Parmetis)>"
	 << endl;
    return 1;
  }

  int etype;
  if (!strcmp(argv[3], "REGION")) etype = iBase_REGION;
  else if (!strcmp(argv[3], "FACE")) etype = iBase_FACE;
  else if (argv[5] != NULL && !strcmp(argv[5], "Hypergraph")) {
    cout<<"["<<rank
	<<"] Argument 3 must be FACE or REGION when argument 5 is Hypergraph,"
	<<" not "<<argv[3]<<endl;
    return 1;
  }
  else if (!strcmp(argv[3], "EDGE")) etype = iBase_EDGE;
  else if (!strcmp(argv[3], "VERTEX")) etype = iBase_VERTEX;
  else {
    cout<<"["<<rank<<"] Argument 3 must be VERTEX, EDGE, FACE, or REGION, not "
	<< argv[3] << endl;
    return 1;
  }

  // initialize the Mesh
  int result;
  iMesh_Instance mesh = NULL;
  iMesh_newMesh(NULL, &mesh, &result, 0);
  if (iBase_SUCCESS != result) {
    cout << "Failed to create a mesh instance." << endl;
    return result;
  }
  iMesh_getRootSet(mesh, &root_set,&result);
  if (iBase_SUCCESS != result) {
    cout << "Failed to create a root set." << endl;
    return result;
  }

  if (rank == 0) {
    // load the mesh
    iMesh_load(mesh, root_set, argv[2], NULL, &result, strlen(argv[2]), 0);
    if (iBase_SUCCESS != result) {
      cout << "ERROR : can not load the mesh from file " << argv[2] << endl;
      return result;
    }
  }

  ITAPSZoltan *itapsz = new ITAPSZoltan(&mesh, &root_set, argc, argv);

  const char *approach = NULL, *method = NULL, *package = NULL;
  if (argc > 5) method = argv[5];
  if (argc > 6) approach = argv[6];
  if (argc > 7) package = argv[7];
  
  int nparts = atoi(argv[1]);
  
  result=itapsz->parallel_partition(nparts,etype,method,approach,package); RR;
  
  if (rank == 0) {
    iMesh_save(mesh, root_set, argv[4], NULL, &result, strlen(argv[4])+1, 0);
    if (iBase_SUCCESS != result) {
      cout << "ERROR : can not save the mesh to file " << argv[4] << endl;
      return result;
    }
  }

  MPI_Finalize();
  return 0;
}
