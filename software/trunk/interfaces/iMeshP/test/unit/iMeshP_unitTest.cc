// This file is intended to test functionality of a iMeshP implementation.
// Functions to manage the parallel aspects of a discrete mesh that has been decomposed
// into a partition consisting of parts. Functions to manage the parts and partition,
// copies of entities on different processors, entity migration, data exchange across
// processor boundaries including communication requirements including collective, 
// point-to-point or none. (Note: subject to change)
// 
// It is worth noting that, although efforts have been made to make each
// test as independent as possible, it is unavoidable that a failure
// during one test may cause failures in subsequent tests, sometimes
// even across different interface tests.  In light of this, it is best
// to try to correct errors in the order they occur, or at least to
// exclude interface tests that cause errors to see if later errors are
// avoided.
//
// Each iMeshP specification function is tested in two ways
//
// 1- Basic Functionality tests: This test checks if the iMeshP function is successful or
//    not. If the function is not successful, it displays the iBase error code. 
//
// 2- Validation tests: This checks if the function results are accurate by cross checking
//    them with combinations of other iMeshP/iMesh functions.
//
// For all tests, the output message displays which of the two tests does the function
// pass/fail.

#include "iMeshP_unitTest.h"
#include <iostream>

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &mysize);  

  // Create a file for each processor's output so that the output doesn't
  // get jumbled on the screen.

  char filename[23];
  char* cMeshFile;

  sprintf(filename, "Output_%02d.txt", myrank);
  fp = fopen(filename, "w");

  /// the mesh file to be loaded 
  cMeshFile = argv[1];

  if (!myrank) 
  {
    std::cout<<"*****************************************************\n";
    std::cout<<"[INPUT]\n";
    std::cout<<"\tdistributed mesh? "; std::cout<<"YES\n";
    std::cout<<"\tinitial # parts per proc: 1"<<endl;
    std::cout<<"\tdesired # parts per proc: "<<MAX_NUM_PARTS<<endl;
    std::cout<<"*****************************************************\n\n";  
  }

   /// If the file handle is invalid, exit.
  if (fp == NULL )
  {
    std::cout << "Error opening file!" << endl;
    exit(1);
  }

    /// Create a new mesh using mesh instance and partition handle.
  iMesh_Instance instance;
  iMeshP_PartitionHandle partition;

  iMesh_newMesh(options, &instance, &ierr, options_len);
    
  /// If the mesh is not created successfully, we can't continue so exit.
     
  CHECK_ERR_CRITICAL_MAIN(ierr, "iMesh_newMesh");

   /// Set up legal type-topology combination, ref: iMesh unit test.
  vSetupImplStatus();
  vSetupTypeAndTopo();
  vSetupNumTypeAndTopo();

  {
    /// get root set from the mesh instance, if doesn't succeed we can't proceed so throw
    ///  error and exit
    iBase_EntitySetHandle root;
    iMesh_getRootSet(instance, &root, &ierr);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMesh_getRootSet");

    /// create a partition --- mesh instance contains the partition
    /// if doesn't succeed we can't proceed so throw error and exit
  
    iMeshP_createPartitionAll(instance, MPI_COMM_WORLD, &partition, &ierr);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_createPartitionAll");

    /// Now load the mesh into the partition, if load doesn't succeed throw an error and
    ///  exit.   
    iMeshP_loadAll(instance, partition, root, cMeshFile, options, &ierr, 
                 strlen(cMeshFile), options_len);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_loadAll");

    iMeshP_PartitionHandle* pPtnHandles;
    int pPtnSize = 0, pPtnAllocated = 0;

    iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_getPartitions");

    iMeshP_PartHandle* pLocalParts = NULL;
    int pLocalPartAllocated=0, pLocalPartSize=0;
    iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated, &pLocalPartSize, &ierr);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_getLocalParts");

    if (pLocalPartSize!=INIT_NUM_PARTS) // INIT_NUM_PARTS=1 by default
      INIT_NUM_PARTS = pLocalPartSize;

    /// set the desired number of parts per process
    if (argc>2 && atoi(argv[2])>INIT_NUM_PARTS)
      MAX_NUM_PARTS = atoi(argv[2]);
    else 
      MAX_NUM_PARTS = 2*INIT_NUM_PARTS;

    UT_FREE(pPtnHandles);
    UT_FREE(pLocalParts);
    /// All steps have passed upto this point
    bAllPassed = true;
  }

  {
    fprintf(fp, "\n*********************************************");
    fprintf(fp, "\n 1- Testing IMESHP PARTITION FUNCTIONALITY");
    fprintf(fp, "\n*******************************************");
#ifdef DEBUG
    if (!myrank)
    {
    fprintf(stdout, "\n*********************************************");
    fprintf(stdout, "\n 1- Testing IMESHP PARTITION FUNCTIONALITY");
    fprintf(stdout, "\n*******************************************");
    }
#endif
    iCurNFailed=iNFailed;   
    RUN("vGetPartitionTest", vGetPartitionTest(instance), iCurNFailed);

    iCurNFailed=iNFailed;   
    RUN("vGetPartitionInfoTest", vGetPartitionInfoTest(instance), iCurNFailed);

  }
  /**  \brief Testing stuff related to part functionality. 
   
    Functions include createPart, destroyPart, getPartIdFromPartHandle,
    getPartIdsFromPartHandlesArr, getPartHandleFromPartId and 
    getPartHandlesFromPartsIdsArr */
  
  {
    fprintf(fp, "\n*******************************************");
    fprintf(fp, "\n 2- Testing IMESHP PART FUNCTIONALITY");
    fprintf(fp, "\n*****************************************");
#ifdef DEBUG
    if (!myrank)
    {
    fprintf(stdout, "\n*******************************************");
    fprintf(stdout, "\n 2- Testing IMESHP PART FUNCTIONALITY");
    fprintf(stdout, "\n*****************************************");
    }
#endif

    iCurNFailed=iNFailed;   
    RUN("vGetPartHandleIdRankTest", vGetPartHandleIdRankTest(instance), iCurNFailed);

    iCurNFailed=iNFailed;   
    RUN("vGetPartHandleIdRankArrTest", vGetPartHandleIdRankArrTest(instance), iCurNFailed);

    iCurNFailed=iNFailed;   
    RUN("vGetPartTypeTopoTest", vGetPartTypeTopoTest(instance), iCurNFailed);

    iCurNFailed=iNFailed;   
    RUN("vGetPartNonResIteratorTest", vGetPartNonResIteratorTest(instance), iCurNFailed);

/* -- turned off temporarily
    iCurNFailed=iNFailed;   
    RUN("vGetPartResIteratorTest", vGetPartResIteratorTest(instance), iCurNFailed);
*/
    iCurNFailed=iNFailed;
    RUN("vGetPartNborTest", vGetPartNborTest(instance), iCurNFailed);

    iCurNFailed=iNFailed;   
    RUN("vGetPartNborArrTest", vGetPartNborArrTest(instance), iCurNFailed);
  }
  
  {
    fprintf(fp, "\n************************************************");
    fprintf(fp, "\n 3- Testing IMESHP PART BOUNDARY FUNCTIONALITY");
    fprintf(fp, "\n************************************************");
#ifdef DEBUG    
    if (!myrank)
    {
    fprintf(stdout, "\n************************************************");
    fprintf(stdout, "\n 3- Testing IMESHP PART BOUNDARY FUNCTIONALITY");
    fprintf(stdout, "\n************************************************");
    }   
#endif

    iCurNFailed=iNFailed; 
    RUN("vGetPartBdryEntsTest", vGetPartBdryEntsTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vBdryEntsIteratorTest", vBdryEntsIteratorTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vBdryEntsArrIteratorTest", vBdryEntsArrIteratorTest(instance), iCurNFailed);
  } 

  {
    fprintf(fp, "\n******************************************************");
    fprintf(fp, "\n 4- Testing IMESHP ENTITY OWNERSHIP FUNCTIONALITY");
    fprintf(fp, "\n******************************************************");
#ifdef DEBUG    
    if (!myrank)
    {
    fprintf(stdout, "\n******************************************************");
    fprintf(stdout, "\n 4- Testing IMESHP ENTITY OWNERSHIP FUNCTIONALITY");
    fprintf(stdout, "\n******************************************************");      
    }
#endif

    iCurNFailed=iNFailed; 
    RUN("vGetEntOwnerPartTest", vGetEntOwnerPartTest(instance), iCurNFailed);
   
    iCurNFailed=iNFailed; 
    RUN("vGetEntOwnerPartArrTest", vGetEntOwnerPartArrTest(instance), iCurNFailed);
 
    iCurNFailed=iNFailed; 
    RUN("vIsEntOwnerTest", vIsEntOwnerTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vIsEntOwnerArrTest", vIsEntOwnerArrTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vGetEntCopiesTest", vGetEntCopiesTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vGetEntCopyOnPartTest", vGetEntCopyOnPartTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vGetEntOwnerCopyTest", vGetEntOwnerCopyTest(instance), iCurNFailed);

/*
    iCurNFailed=iNFailed; 
    RUN("vGetEntStatusTest", vGetEntStatusTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vGetEntStatusArrTest", vGetEntStatusArrTest(instance), iCurNFailed);
*/
  }
  {
    fprintf(fp, "\n*********************************************");
    fprintf(fp, "\n 5- Testing IMESHP MODIFICATION FUNCTIONALITY");
    fprintf(fp, "\n*********************************************");
#ifdef DEBUG
    if (!myrank)
    {      
    fprintf(stdout, "\n*********************************************");
    fprintf(stdout, "\n 5- Testing IMESHP MODIFICATION FUNCTIONALITY");
    fprintf(stdout, "\n*********************************************");
    }
#endif     
    iCurNFailed=iNFailed;
    RUN("vExchangeEntArrTest", vExchangeEntArrTest(instance), iCurNFailed);

    iCurNFailed=iNFailed;
    RUN("vMigrateEntTest", vMigrateEntTest(instance), iCurNFailed);

    iCurNFailed=iNFailed; 
    RUN("vCreateDeletePartTest", vCreateDeletePartTest(instance), iCurNFailed);
  }

  {
/*
    fprintf(fp, "\n*********************************************");
    fprintf(fp, "\n 6- Testing IMESHP GHOSTING FUNCTIONALITY");
    fprintf(fp, "\n*******************************************");
#ifdef DEBUG
    if (!myrank)
    {
    fprintf(stdout, "\n*********************************************");
    fprintf(stdout, "\n 6- Testing IMESHP GHOSTING FUNCTIONALITY");
    fprintf(stdout, "\n*******************************************");
    }
#endif     
   // individual ghost entity creation turned off
   // iCurNFailed=iNFailed; 
   // RUN("vAddRmvGhostTest", vAddRmvGhostTest(instance), iCurNFailed);
    iCurNFailed=iNFailed; 
    RUN("vCreateDeleteGhostEntsAllTest", vCreateDeleteGhostEntsAllTest(instance), iCurNFailed);
  // iCurNFailed=iNFailed;    
  //  RUN("vGetGhostInfoTest", vGetGhostInfoTest(instance), iCurNFailed);
  //  iCurNFailed=iNFailed; 
  //  RUN("vDeleteGhostEntsAllTest", vDeleteGhostEntsAllTest(instance), iCurNFailed);
*/
  }

  iMeshP_destroyPartitionAll(instance, partition, &ierr);
  CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_destroyPartitionAll");

  // Free up the iMesh instance
  iMesh_dtor(instance, &ierr);

  int iNumTotalTest=0, iNumTested=0;
  for (std::map<std::string, bool>::iterator it=test_map.begin(); it!=test_map.end(); ++it)
  {
    ++iNumTotalTest;
    if (it->second) ++iNumTested; 
  }
  
  /// Print the total number of tests passed or failed
  fprintf(fp, "\033[0;%dm---------------------------------------------------------------------\033[0m\n", 37);
  if (!myrank) fprintf(stdout, "\033[0;%dm\n---------------------------------------------------------------------\033[0m\n", 37);

  fprintf (fp, "\tFINAL RESULTS:  #Passed %d,  #Failed %d,  #Total %d \n", 
           iNPassed, iNFailed, iNPassed+iNFailed);
  if (!myrank)
    fprintf (stdout, "\tFINAL RESULTS:  #Passed %d,  #Failed %d,  #Total %d \n",      
           iNPassed, iNFailed, iNPassed+iNFailed);

  if (iNPassed > 500)
  {
    fprintf(fp, "\t\tPass Rate %d%,  Test Coverage %d% \n", (int)100*iNPassed/(iNPassed+iNFailed), (int)100*iNumTested/iNumTotalTest);
    if (!myrank)  fprintf(stdout, "\t\tPass Rate %d%,  Test Coverage %d% \n", (int)100*iNPassed/(iNPassed+iNFailed), (int)100*iNumTested/iNumTotalTest);
  }
  else
  {
    fprintf(fp, "\t\tPass Rate N/A,  Test Coverage %d% \n", (int)100*iNumTested/iNumTotalTest);
    if (!myrank)  fprintf(stdout, "\t\tPass Rate N/A,  Test Coverage %d% \n", (int)100*iNumTested/iNumTotalTest);

    fprintf(fp, "\033[0;%dm\nVerify the mesh and/or implement iMeshP_getPartBdryEnts and \niMeshP_getEntities on which most test routines rely.\033[0m\n", 31);
    if (!myrank) fprintf(stdout, "\033[0;%dm\nVerify the mesh and/or implement iMeshP_getPartBdryEnts and \niMeshP_getEntities on which most test routines rely.\033[0m\n", 31);
  }

  fprintf(fp, "\033[0;%dm---------------------------------------------------------------------\033[0m\n", 37);
  if (!myrank) fprintf(stdout, "\033[0;%dm---------------------------------------------------------------------\033[0m\n", 37);
  fclose(fp);

  if (!myrank)
    std::cout<<"\nPlease See Output_ProcId.txt for Test Result of Each Process\n\n";
  
  MPI_Finalize();
  return 1;
}

////////////////////////////////////////
//         TYPE/TOPOLOGY SETUP        //
////////////////////////////////////////

static int vSetupTypeAndTopo()
{
  // Which combinations are okay doesn't depend on whether the mesh
  // database is 2D, 3D, etc.  Meshes are required to returned nothing
  // if they don't have something, that's all.
  //@@ Setup data
  for (eETopo = iMesh_POINT; eETopo <= iMesh_ALL_TOPOLOGIES; ++eETopo)
  {
    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
      a2qLegalTypeAndTopo[eEType][eETopo] = false;
    a2qLegalTypeAndTopo[iBase_ALL_TYPES][eETopo] = true;
  }

  a2qLegalTypeAndTopo[iBase_VERTEX][iMesh_POINT] = true;

  a2qLegalTypeAndTopo[iBase_EDGE][iMesh_LINE_SEGMENT] = true;

  a2qLegalTypeAndTopo[iBase_FACE][iMesh_TRIANGLE] = true;
  a2qLegalTypeAndTopo[iBase_FACE][iMesh_QUADRILATERAL] = true;
  a2qLegalTypeAndTopo[iBase_FACE][iMesh_POLYGON] = true;

  a2qLegalTypeAndTopo[iBase_REGION][iMesh_TETRAHEDRON] = true;
  a2qLegalTypeAndTopo[iBase_REGION][iMesh_PYRAMID] = true;
  a2qLegalTypeAndTopo[iBase_REGION][iMesh_PRISM] = true;
  a2qLegalTypeAndTopo[iBase_REGION][iMesh_HEXAHEDRON] = true;
  a2qLegalTypeAndTopo[iBase_REGION][iMesh_SEPTAHEDRON] = true;
  a2qLegalTypeAndTopo[iBase_REGION][iMesh_POLYHEDRON] = true;

  a2qLegalTypeAndTopo[iBase_VERTEX][iMesh_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[iBase_EDGE][iMesh_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[iBase_FACE][iMesh_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[iBase_REGION][iMesh_ALL_TOPOLOGIES] = true;
  a2qLegalTypeAndTopo[iBase_ALL_TYPES][iMesh_ALL_TOPOLOGIES] = true;
}

static int vSetupNumTypeAndTopo()
{
  //@@@ Setup of legal argument combinations and expected return sizes.
  for (eETopo = iMesh_POINT;
       eETopo <= iMesh_ALL_TOPOLOGIES;
       eETopo++) {
    for (eEType = iBase_VERTEX;
	 eEType <= iBase_ALL_TYPES;
	 eEType++) {
      a2iEntCount[eEType][eETopo] = 0;
      a2iPartBdryEntCount[eEType][eETopo] = 0;
    }
  }
}

////////////////////////////////////////
//         TYPE/TOPOLOGY SETUP        //
////////////////////////////////////////

static int vSetupImplStatus()
{
  test_map["iMeshP_createPartitionAll"]=true; impl_map["iMeshP_createPartitionAll"]=false;
  test_map["iMeshP_destroyPartitionAll"]=true; impl_map["iMeshP_destroyPartitionAll"]=false;
  test_map["iMeshP_getPartitionComm"]=true; impl_map["iMeshP_getPartitionComm"]=false;
  test_map["iMeshP_getNumPartitions"]=true;impl_map["iMeshP_getNumPartitions"]=false;
  test_map["iMeshP_getPartitions"]=true;impl_map["iMeshP_getPartitions"]=false;
  test_map["iMeshP_createPart"]=true;impl_map["iMeshP_createPart"]=false;
  test_map["iMeshP_destroyPart"]=true;impl_map["iMeshP_destroyPart"]=false;
  test_map["iMeshP_syncPartitionAll"]=true;impl_map["iMeshP_syncPartitionAll"]=false;
  test_map["iMeshP_getNumGlobalParts"]=true;impl_map["iMeshP_getNumGlobalParts"]=false;
  test_map["iMeshP_getNumLocalParts"]=true;impl_map["iMeshP_getNumLocalParts"]=false;
  test_map["iMeshP_getLocalParts"]=true;impl_map["iMeshP_getLocalParts"]=false;
  test_map["iMeshP_getRankOfPart"]=true;impl_map["iMeshP_getRankOfPart"]=false;
  test_map["iMeshP_getRankOfPartArr"]=true;impl_map["iMeshP_getRankOfPartArr"]=false;
  test_map["iMeshP_getNumOfTypeAll"]=true;impl_map["iMeshP_getNumOfTypeAll"]=false;
  test_map["iMeshP_getNumOfTopoAll"]=true;impl_map["iMeshP_getNumOfTopoAll"]=false;
  test_map["iMeshP_getPartIdFromPartHandle"]=true;impl_map["iMeshP_getPartIdFromPartHandle"]=false;
  test_map["iMeshP_getPartIdsFromPartHandlesArr"]=true;impl_map["iMeshP_getPartIdsFromPartHandlesArr"]=false;
  test_map["iMeshP_getPartHandleFromPartId"]=true;impl_map["iMeshP_getPartHandleFromPartId"]=false;
  test_map["iMeshP_getPartHandlesFromPartsIdsArr"]=true;impl_map["iMeshP_getPartHandlesFromPartsIdsArr"]=false;
  test_map["iMeshP_getNumPartNbors"]=true;impl_map["iMeshP_getNumPartNbors"]=false;
  test_map["iMeshP_getNumPartNborsArr"]=true;impl_map["iMeshP_getNumPartNborsArr"]=false;
  test_map["iMeshP_getPartNbors"]=true;impl_map["iMeshP_getPartNbors"]=false;
  test_map["iMeshP_getPartNborsArr"]=true;impl_map["iMeshP_getPartNborsArr"]=false;
  test_map["iMeshP_getNumPartBdryEnts"]=true;impl_map["iMeshP_getNumPartBdryEnts"]=false;
  test_map["iMeshP_getPartBdryEnts"]=true;impl_map["iMeshP_getPartBdryEnts"]=false;
  test_map["iMeshP_initPartBdryEntIter"]=true;impl_map["iMeshP_initPartBdryEntIter"]=false;
  test_map["iMesh_getNextEntIter"]=true; impl_map["iMesh_getNextEntIter"]=false;
  test_map["iMesh_resetEntIter"]=true; impl_map["iMesh_resetEntIter"]=false;
  test_map["iMesh_endEntIter"]=true; impl_map["iMesh_endEntIter"]=false;
  test_map["iMeshP_initPartBdryEntArrIter"]=true;impl_map["iMeshP_initPartBdryEntArrIter"]=false;
  test_map["iMesh_getNextEntArrIter"]=true; impl_map["iMesh_getNextEntArrIter"]=false;
  test_map["iMesh_resetEntArrIter"]=true; impl_map["iMesh_resetEntArrIter"]=false;
  test_map["iMesh_endEntArrIter"]=true; impl_map["iMesh_endEntArrIter"]=false;
  test_map["iMeshP_getNumOfType"]=true;impl_map["iMeshP_getNumOfType"]=false;
  test_map["iMeshP_getNumOfTopo"]=true;impl_map["iMeshP_getNumOfTopo"]=false;
  test_map["iMeshP_getAdjEntIndices"]=false;impl_map["iMeshP_getAdjEntIndices"]=false;
  test_map["iMeshP_getEntities"]=true;impl_map["iMeshP_getEntities"]=false;
  test_map["iMeshP_initEntIter"]=true;impl_map["iMeshP_initEntIter"]=false;
  test_map["iMeshP_initEntArrIter"]=true;impl_map["iMeshP_initEntArrIter"]=false;
  test_map["iMeshP_getEntOwnerPart"]=true;impl_map["iMeshP_getEntOwnerPart"]=false;
  test_map["iMeshP_getEntOwnerPartArr"]=true;impl_map["iMeshP_getEntOwnerPartArr"]=false;
  test_map["iMeshP_isEntOwner"]=true;impl_map["iMeshP_isEntOwner"]=false;
  test_map["iMeshP_isEntOwnerArr"]=true;impl_map["iMeshP_isEntOwnerArr"]=false;
  test_map["iMeshP_getEntStatus"]=true;impl_map["iMeshP_getEntStatus"]=false;
  test_map["iMeshP_getEntStatusArr"]=true;impl_map["iMeshP_getEntStatusArr"]=false;
  test_map["iMeshP_getNumCopies"]=true;impl_map["iMeshP_getNumCopies"]=false;
  test_map["iMeshP_getCopyParts"]=true;impl_map["iMeshP_getCopyParts"]=false;
  test_map["iMeshP_getCopies"]=true;impl_map["iMeshP_getCopies"]=false;
  test_map["iMeshP_getCopyOnPart"]=true;impl_map["iMeshP_getCopyOnPart"]=false;
  test_map["iMeshP_getOwnerCopy"]=true;impl_map["iMeshP_getOwnerCopy"]=false;
  test_map["iMeshP_waitForRequest"]=false;impl_map["iMeshP_waitForRequest"]=false;
  test_map["iMeshP_waitForAnyRequest"]=false;impl_map["iMeshP_waitForAnyRequest"]=false;
  test_map["iMeshP_waitForAllRequests"]=false;impl_map["iMeshP_waitForAllRequests"]=false;
  test_map["iMeshP_waitForRequestEnt"]=true;impl_map["iMeshP_waitForRequestEnt"]=true;
  test_map["iMeshP_testRequest"]=false;impl_map["iMeshP_testRequest"]=false;
  test_map["iMeshP_pollForRequests"]=false;impl_map["iMeshP_pollForRequests"]=false;  
  test_map["iMeshP_exchEntArrToPartsAll"]=true;impl_map["iMeshP_exchEntArrToPartsAll"]=false;
  test_map["iMeshP_migrateEntity"]=true;impl_map["iMeshP_migrateEntity"]=true;
  test_map["iMeshP_updateVtxCoords"]=false;impl_map["iMeshP_updateVtxCoords"]=false;
  test_map["iMeshP_replaceOnPartBdry"]=false;impl_map["iMeshP_replaceOnPartBdry"]=false;
  test_map["iMeshP_rmvGhostOf"]=false;impl_map["iMeshP_rmvGhostOf"]=false;
  test_map["iMeshP_syncMeshAll"]=false;impl_map["iMeshP_syncMeshAll"]=false;
  test_map["iMeshP_pushTags"]=false; impl_map["iMeshP_pushTags"]=false;
  test_map["iMeshP_pushTagsEnt"]=false;impl_map["iMeshP_pushTagsEnt"]=false;
  test_map["iMeshP_iPushTags"]=false;impl_map["iMeshP_iPushTags"]=false;  
  test_map["iMeshP_iPushTagsEnt"]=false;impl_map["iMeshP_iPushTagsEnt"]=false;
  test_map["iMeshP_createGhostEntsAll"]=false;impl_map["iMeshP_createGhostEntsAll"]=false;
  test_map["iMeshP_deleteGhostEntsAll"]=false;impl_map["iMeshP_deleteGhostEntsAll"]=false;
  test_map["iMeshP_ghostEntInfo"]=false;impl_map["iMeshP_ghostEntInfo"]=false;
  test_map["iMeshP_loadAll"]=true; impl_map["iMeshP_loadAll"]=false;
  test_map["iMeshP_saveAll"]=false; impl_map["iMeshP_saveAll"]=false;
}
  
////////////////////////////////////////
//      PARTITION FUNCTIONALITY       //
////////////////////////////////////////

int vGetPartitionTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  int iNumPtn;
  iMeshP_getNumPartitions(instance, &iNumPtn, &ierr);
  TEST(!ierr, _FUNC_, "MeshP_getNumPartitions"); 
  if (ierr==iBase_NOT_SUPPORTED) 
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }
  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  for (int num = 0; num < pPtnSize; ++num)
  {
    MPI_Comm communicator;
    iMeshP_getPartitionComm(instance, pPtnHandles[num], &communicator, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getPartitionComm");
  }

  TEST(pPtnSize==iNumPtn, _VALID_, "iPtnSize==iNumPtn");  
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

int vGetPartitionInfoTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  int iNumTypeAll, iNumTopoAll;
  iMeshP_getNumOfTypeAll (instance, pPtnHandles[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumTypeAll, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumOfTypeAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_getNumOfTopoAll (instance, pPtnHandles[0], root, static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &iNumTopoAll, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumOfTopoeAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }
  
  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  sprintf(msg,"iNumTypeAll(%d)==iNumTopoAll(%d)", iNumTypeAll, iNumTopoAll);
  TEST(iNumTypeAll==iNumTopoAll, _VALID_, msg);

    for (int num = 0; num < pPtnSize; ++num)
  {
    iMeshP_PartitionHandle pPtn = pPtnHandles[num];
    int iNumSumType=0, iNumType, iNumSumTopo, iNumTopo;
    for (eEType = iBase_VERTEX; eEType < iBase_ALL_TYPES; ++eEType)
    {
      iMeshP_getNumOfTypeAll (instance, pPtn, root, eEType, &iNumType, &ierr);
      sprintf(msg,"iMeshP_getNumOfTypeAll(%s)", astrTypes[eEType]);
      TEST(!ierr, _FUNC_, msg);

      iNumSumType+=iNumType;
      iNumSumTopo=0;
      for (eETopo = iMesh_POINT; eETopo < iMesh_ALL_TOPOLOGIES; ++eETopo)
      {
        if (!a2qLegalTypeAndTopo[eEType][eETopo]) continue;
        iMeshP_getNumOfTopoAll (instance, pPtn, root, static_cast<iMesh_EntityTopology>(eETopo), &iNumTopo, &ierr);
        sprintf(msg,"iMeshP_getNumOfTopoAll(%s)", astrTopologies[eETopo]);
        TEST(!ierr, _FUNC_, msg);
        iNumSumTopo += iNumTopo;
      }  // for each eETopo
      sprintf(msg,"iNumType(%d)==iNumSumTopo(%d) (%s)", iNumType, iNumSumTopo, astrTypes[eEType]);
      TEST(iNumType==iNumSumTopo, _VALID_, msg);
    } // for each eEType
    sprintf(msg,"iNumSumType(%d)==iNumTypeAll(%d)", iNumSumType, iNumTypeAll);
    TEST(iNumSumType==iNumTypeAll, _VALID_, msg);
  } // for each partition
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

////////////////////////////////////////
//         PART FUNCTIONALITY         //
////////////////////////////////////////

/*/////////////////////////////////////////////// 
 Test: iMeshP_createPart and iMeshP_destroyPart.
  * Dependencies: iMeshP_getNumLocalParts, iMeshP_getNumParts, iMeshP_getPartIdFromPartHandle, 
                  iMeshP_getPartitions, iMeshP_getNumOfType.
  * Test Cases: 
    1- Should create fixed number of parts in the given partition
    2- Get the new part's handle and part id
    3- Check if the on-process parts have incremented.
    4- Delete created parts.
    5- Check if the on-process parts have decremented.
/////////////////////////////////////////////// */
int vCreateDeletePartTest(iMesh_Instance instance)
{
  /// Get all the partitions, parts and rootset
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated=0, pLocalPartSize=0;
  iMeshP_getLocalParts(instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated, &pLocalPartSize, &ierr);

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
  int iNumLocalParts, iNumGlobParts, iNumType;
  iMeshP_getNumLocalParts(instance, pPtnHandles[0], &iNumLocalParts, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumLocalParts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  
  iMeshP_getNumGlobalParts(instance, pPtnHandles[0], &iNumGlobParts, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumGlobalParts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_getNumOfType(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumType, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_PartHandle pPartHdl;
  /// Step 2: Create a part in the given partition. 
  iMeshP_createPart(instance, pPtnHandles[0], &pPartHdl, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  else 
  {
    if (ierr==iBase_SUCCESS) 
      iMeshP_destroyPart(instance, pPtnHandles[0], pPartHdl, &ierr);
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  iMeshP_syncPartitionAll(instance, pPtnHandles[0], &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); 

  if (!bOK_func)
  {
    UT_FREE(pPtnHandles); 
    return iBase_FAILURE; 
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  int iNumLocalPartsInit, iNumGlobPartsInit, iCurNumParts;
  bool bOK_valid=true;
  /* Loop through all partitions on the process to create a part in each of them*/
  for (int p = 0; p < pPtnSize; ++p)
  {
    /* Step 1: Get the total number of on-process parts using iMeshP_getNumLocalParts. */
    iMeshP_getNumLocalParts(instance, pPtnHandles[p], &iNumLocalPartsInit, &ierr);
    TEST(iNumLocalPartsInit==INIT_NUM_PARTS, _VALID_, "iNumLocalPartsInit==INIT_NUM_PARTS");

    iMeshP_getNumGlobalParts(instance, pPtnHandles[p], &iNumGlobPartsInit, &ierr);
    TEST(iNumGlobPartsInit==INIT_NUM_PARTS*mysize, _VALID_, "iNumGlobPartsInit==INIT_NUM_PARTS*mysize");

    iMeshP_PartHandle* pLocalPartsInit = NULL;
    int pLocalPartAllocatedInit=0, pLocalPartSizeInit=0;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalPartsInit, &pLocalPartAllocatedInit,
          &pLocalPartSizeInit, &ierr);
    TEST( pLocalPartAllocatedInit==iNumLocalPartsInit, _VALID_, "pLocalPartAllocatedInit==iInitNumParts");
    TEST( pLocalPartSizeInit==iNumLocalPartsInit, _VALID_, "pLocalPartSizeInit==iInitNumParts");

    iCurNumParts=iNumLocalPartsInit;
    for (int pr = 0; pr < MAX_NUM_PARTS-INIT_NUM_PARTS; ++pr)
    {
      iMeshP_PartHandle pPartHdl;
      /// Step 2: Create a part in the given partition. 
      iMeshP_createPart(instance, pPtnHandles[p], &pPartHdl, &ierr);
      iCurNumParts++;
    } 
    
    TEST( iCurNumParts==MAX_NUM_PARTS, _VALID_, "iCurNumParts==MAX_NUM_PARTS");

    iMeshP_syncPartitionAll(instance, pPtnHandles[p], &ierr);

    iMeshP_getNumLocalParts(instance, pPtnHandles[p], &iNumLocalParts, &ierr);
    TEST(iNumLocalParts==MAX_NUM_PARTS, _VALID_, "iNumLocalParts==MAX_NUM_PARTS");

    iMeshP_getNumGlobalParts(instance, pPtnHandles[p], &iNumGlobParts, &ierr);
    TEST(iNumGlobParts==MAX_NUM_PARTS*mysize, _VALID_, "iNumGlobParts==MAX_NUM_PARTS*mysize");

    iMeshP_PartHandle* pLocalParts = NULL;
    int pLocalPartAllocated=0, pLocalPartSize=0;
    ///  Step 1: Get all local part handles in the partition
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated, &pLocalPartSize, &ierr);
    TEST(pLocalPartAllocated==MAX_NUM_PARTS, _VALID_, "pLocalPartAllocated==MAX_NUM_PARTS");
    TEST(pLocalPartSize==MAX_NUM_PARTS, _VALID_, "pLocalPartSize==MAX_NUM_PARTS");
    
    bOK_valid=true;
    for (int i=0; i<  pLocalPartSizeInit; ++i)
      if (pLocalPartsInit[i]!=pLocalParts[i]) bOK_valid=false;
    TEST( bOK_valid, _VALID_, "comparing initial part handles before/after iMeshP_createPart");

    bOK_valid=true;
    for (int i=pLocalPartSizeInit; i<pLocalPartSize; ++i)
    {
      iMeshP_getNumOfType(instance, pPtnHandles[p], pLocalParts[i], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumType, &ierr);
      if (iNumType) bOK_valid=false;
    }
    TEST(bOK_valid, _VALID_, "checking emptiness of new part");
  
    /// Now delete parts
    for (int dty = MAX_NUM_PARTS-1; dty >= iNumLocalPartsInit; --dty)
      iMeshP_destroyPart(instance, pPtnHandles[p], pLocalParts[dty], &ierr);	
    iMeshP_syncPartitionAll(instance, pPtnHandles[p], &ierr);

    /// Now get local parts again; the number of parts must have dropped down by
    /// MAX_NUM_PARTS.
    iNumLocalParts=0;
    iMeshP_getNumLocalParts(instance, pPtnHandles[p], &iNumLocalParts, &ierr);
    TEST(iNumLocalParts==iNumLocalPartsInit, _VALID_, "iNumLocalParts==iNumLocalPartsInit");

    UT_FREE (pLocalPartsInit);
    UT_FREE (pLocalParts);
  } // for each partition
  /// Free the memory
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 


/*/////////////////////////////////////////////// 
 Test: vGetPartHandleIdRankTest
    * Dependencies:iMeshP_getPartitions,  iMeshP_getLocalParts
    * Tested Functions iMeshP_getPartIdFromPartHandle, iMeshP_getRankOfPart, 
                     iMeshP_getPartHandleFromPartID
    * Test Cases: 
      1- Return valid part id when a valid part handle array is given
      2- Give a valid rank of the part using the returned part id. 
      3- When an invalid/remote part handle is given, it should throw an error.
/////////////////////////////////////////////// */
int vGetPartHandleIdRankTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iMeshP_Part pPartId;
  iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[0], pLocalParts[0], &pPartId, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iRank;
  iMeshP_getRankOfPart(instance, pPtnHandles[0], pPartId, &iRank, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getRankOfPart");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_PartHandle pPartHandle; 
  iMeshP_getPartHandleFromPartId(instance, pPtnHandles[0], pPartId, &pPartHandle, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartHandleFromPartId");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts);

  if (!bOK_func)
  {
    UT_FREE(pPtnHandles); 
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************

  /// For every partition, get its local parts and perform the test.
  for (int p = 0; p < pPtnSize; ++p)
  { 
    ///  Step 1: Get all local part handles in the partition
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);
    /* Step 1: For all part handles, get part id using the method of
       getPartIdsFromPartHandlesArr. It takes a part handle and returns a part id */
    for (int ph=0; ph<pLocalPartSize; ++ph)
    {
      /// variable for Part id of the local part handle.
      iMeshP_Part pPartIdTmp;
      iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[p], pLocalParts[ph], &pPartIdTmp, &ierr);
      
     /// Step 2: Get the rank of the part id to check if the rank is valid 
      int iRank;
      iMeshP_getRankOfPart(instance, pPtnHandles[p], pPartIdTmp, &iRank, &ierr);
      TEST(iRank == myrank, _VALID_, "checking iMeshP_getRankOfPart");
    } // for ph
    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

int vGetPartHandleIdRankArrTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
  iMeshP_PartitionHandle* pPtnHandles=NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iMeshP_Part pPartId;
  iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[0], pLocalParts[0], &pPartId, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iRank;
  iMeshP_getRankOfPart(instance, pPtnHandles[0], pPartId, &iRank, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getRankOfPart");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_PartHandle pPartHandle; 
  iMeshP_getPartHandleFromPartId(instance, pPtnHandles[0], pPartId, &pPartHandle, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartHandleFromPartId");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_Part* pPartIdArr = NULL;
  int iPartIdArrAllocated=0, iPartIdArrSize=0;

  iMeshP_getPartIdsFromPartHandlesArr (instance, pPtnHandles[0], pLocalParts, pLocalPartSize,
        &pPartIdArr, &iPartIdArrAllocated, &iPartIdArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdsFromPartHandlesArr");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_PartHandle* pPartArr = NULL; 
  int pPartArrSize=0, pPartArrAllocated=0;

  iMeshP_getPartHandlesFromPartsIdsArr (instance, pPtnHandles[0], pPartIdArr, iPartIdArrSize,
        &pPartArr, &pPartArrAllocated, &pPartArrSize, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int* iRankArr=NULL;
  int iRankArrSize=0, iRankArrAllocated=0;

  iMeshP_getRankOfPartArr(instance, pPtnHandles[0], pPartIdArr, iPartIdArrSize, &iRankArr, 
                          &iRankArrAllocated, &iRankArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getRankOfPartArr");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts);  UT_FREE(pPartIdArr);  UT_FREE(pPartArr); UT_FREE(iRankArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************

  /// For every partition, get its local parts and perform the test.
  bool bOK_valid=true;
  for (int p = 0; p < pPtnSize; ++p)
  { 
    /// retrieve an array of handle, id, rank
    ///  Step 1: Get all local part handles in the partition
    iMeshP_PartHandle* pLocalParts = NULL;
    pLocalPartAllocated=0; pLocalPartSize=0;
    iMeshP_getLocalParts (instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

    iMeshP_Part* pPartIdArr = NULL;
    iPartIdArrAllocated=0, iPartIdArrSize=0;

    iMeshP_getPartIdsFromPartHandlesArr (instance, pPtnHandles[p], pLocalParts, pLocalPartSize,
        &pPartIdArr, &iPartIdArrAllocated, &iPartIdArrSize, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getPartIdsFromPartHandlesArr");
    TEST(iPartIdArrAllocated==pLocalPartSize, _VALID_, "iPartIdArrAllocated==pLocalPartSize");
    TEST(iPartIdArrSize==pLocalPartSize, _VALID_, "iPartIdArrSize==pLocalPartSize");

    iMeshP_PartHandle* pPartArr = NULL; 
    pPartArrAllocated=0, pPartArrAllocated=0;
    iMeshP_getPartHandlesFromPartsIdsArr (instance, pPtnHandles[p], pPartIdArr, iPartIdArrSize,
        &pPartArr, &pPartArrAllocated, &pPartArrSize, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getPartHandlesFromPartsIdsArr");
    TEST(pPartArrAllocated==iPartIdArrSize, _VALID_, "pPartArrAllocated==iPartIdArrSize");
    TEST(pPartArrSize==iPartIdArrSize, _VALID_, "pPartArrSize==iPartIdArrSize");
    bOK_valid=true;
    for (int i=0; i<pPartArrSize; ++i)
      if (pPartArr[i]!=pLocalParts[i]) bOK_valid=false;
    TEST(bOK_valid, _VALID_, "pPartArr==pLocalParts");  

    int* iRankArr=NULL;
    iRankArrAllocated=0, iRankArrSize=0;
    iMeshP_getRankOfPartArr(instance, pPtnHandles[p], pPartIdArr, iPartIdArrSize, &iRankArr, 
                          &iRankArrAllocated, &iRankArrSize, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getRankOfPartArr");
    TEST(iRankArrAllocated==iPartIdArrSize, _VALID_, "iRankArrAllocated==iPartIdArrSize");
    TEST(iRankArrSize==iPartIdArrSize, _VALID_, "iRankArrSize==iPartIdArrSize");
    bOK_valid=true;
    for (int i=0; i<iRankArrSize; ++i)
      if (iRankArr[i]!=myrank) bOK_valid=false;
    TEST(bOK_valid, _VALID_, "iMeshP_getRankOfPartArr"); 

    /* Step 1: For all part handles, get part id using the method of
       getPartIdsFromPartHandlesArr. It takes a part handle and returns a part id */
    for (int ph = 0; ph < pLocalPartSize; ++ph)
    {
      /// variable for Part id of the local part handle.
      iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[p], pLocalParts[ph], &pPartId, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
      TEST(pPartId==pPartIdArr[ph], _VALID_, "pPartId==pPartIdArr[ph]");

     /// Step 2: Get the rank of the part id to check if the rank is valid 
      iMeshP_getRankOfPart(instance, pPtnHandles[p], pPartId, &iRank, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getRankOfPart");
      TEST(iRank==myrank, _VALID_, "iRank==myrank");

      iMeshP_getPartHandleFromPartId(instance, pPtnHandles[0], pPartId, &pPartHandle, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getPartHandleFromPartId");
      TEST(pPartHandle==pLocalParts[ph], _VALID_, "pPartHandle==pLocalParts[ph]");
    } // for each part handle
    UT_FREE(pLocalParts);
    UT_FREE(pPartArr);
    UT_FREE(pPartIdArr);
    UT_FREE(iRankArr);
  } // for each partition
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

/*/////////////////////////////////////////////// 
 Test: vGetPartTypeTopoTest
    * Dependencies:iMeshP_getPartitions,  iMeshP_getLocalParts
    * Tested Functions iMeshP_getEntities, iMeshP_getRankOfPart, 
                     iMeshP_getPartHandleFromPartID
    * Test Cases: 
      1- Return valid part id when a valid part handle array is given
      2- Give a valid rank of the part using the returned part id. 
      3- When an invalid/remote part handle is given, it should throw an error.
/////////////////////////////////////////////// */
int vGetPartTypeTopoTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  // Check all possible combinations of boundary entities.
  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrAllocated=0, pEntArrSize=0;

  iMeshP_getEntities (instance, pPtnHandles[0], pLocalParts[0], root,
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),  &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iNumType, iNumTopo;
  iMeshP_getNumOfType(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumType, &ierr);
  TEST(!ierr, _FUNC_, " iMeshP_getNumOfType");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_getNumOfTopo(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &iNumTopo, &ierr);
  TEST(!ierr, _FUNC_, " iMeshP_getNumOfTopo");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iNumTypeAll, iNumTopoAll;
  iMeshP_getNumOfTypeAll (instance, pPtnHandles[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumTypeAll, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumOfTypeAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_getNumOfTopoAll (instance, pPtnHandles[0], root, static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &iNumTopoAll, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumOfTopoAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pEntArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// For every partition, get its local parts and perform the test.
  for (int p = 0; p < pPtnSize; ++p)
  { 
    // 
    //  TO BE COMPLETED
    // 
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

int vGetPartNonResIteratorTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  // Check all possible combinations of boundary entities.
  iBase_EntityIterator pEntIter;
  iMeshP_initEntIter (instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                       0 /*non-resilient*/, &pEntIter, &ierr);
  
  TEST(!ierr, _FUNC_, "Non-Resilient iMeshP_initEntIter");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iBase_EntityArrIterator pEntArrIter;
  iMeshP_initEntArrIter (instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                      10, 0 /*non-resilient*/, &pEntArrIter, &ierr);
  
  TEST(!ierr, _FUNC_, "Non-Resilient iMeshP_initEntArrIter");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrAllocated=0, pEntArrSize=0;

  iMeshP_getEntities (instance, pPtnHandles[0], pLocalParts[0], root,
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),  &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iNumType, iNumTopo;
  iMeshP_getNumOfType(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumType, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumOfType");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_getNumOfTopo(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &iNumTopo, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumOfTopo");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pEntArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }
  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// For every partition, get its local parts and perform the test.
  for (int p = 0; p < pPtnSize; ++p)
  { 
    ///  Step 1: Get all local part handles in the partition
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

int vGetPartResIteratorTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated = 0, pLocalPartSize = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  // Check all possible combinations of boundary entities.
  iBase_EntityIterator pEntIter;
  iMeshP_initEntIter (instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                       1 /*resilient*/, &pEntIter, &ierr);
  
  TEST(!ierr, _FUNC_, "Resilient iMeshP_initEntIter");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iBase_EntityArrIterator pEntArrIter;
  iMeshP_initEntArrIter (instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                      10, 1 /*resilient*/, &pEntArrIter, &ierr);
  
  TEST(!ierr, _FUNC_, "Resilient iMeshP_initEntArrIter");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrAllocated=0, pEntArrSize=0;

  iMeshP_getEntities (instance, pPtnHandles[0], pLocalParts[0], root,
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),  &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iNumType, iNumTopo;
  iMeshP_getNumOfType(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumType, &ierr);
  TEST(!ierr, _FUNC_, "getNumOfType");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_getNumOfTopo(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &iNumTopo, &ierr);
  TEST(!ierr, _FUNC_, "getNumOfTopo");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pEntArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }
  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// For every partition, get its local parts and perform the test.
  for (int p = 0; p < pPtnSize; ++p)
  { 
    ///  Step 1: Get all local part handles in the partition
    iMeshP_PartHandle* pLocalParts = NULL;
    pLocalPartAllocated=0; pLocalPartSize=0;
    iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);
 
    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 


////////////////////////////////////////
//    PART BOUNDARY FUNCTIONALITY     //
////////////////////////////////////////

/*/////////////////////////////////////////////// 
 Test: iMeshP_getPartBdryEnts, iMeshP_getNumPartBdryEnts
    * Dependencies:iMeshP_getEntStatus, iMeshP_getLocalParts, iMeshP_getPartitions
    * Test Cases:
      1- Should get the part boundary entities of all combinations of types and
         topologies. For each type and topology, cross check if the entity is actually a
         boundary entity w.r.t the given part. This can be done by checking entity status.
      2- Compare the number of boundary entities got with the number got through
         getBdryEnts. If it is different then fail.
/////////////////////////////////////////////// */

int vGetPartBdryEntsTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated = 0, pLocalPartSize = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  // Check all possible combinations of boundary entities.
  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
                   static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
                   iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int iNumPartBdryEnt = 0;
  iMeshP_getNumPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
                  static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), iMeshP_ALL_PARTS,
                  &iNumPartBdryEnt, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getNumPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************

  bool bOK_valid=true;
  int iGeomDim;
  iMesh_getGeometricDimension(instance, &iGeomDim, &ierr);

  /// For every partition, get its local parts and perform the test.
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition
    iMeshP_PartHandle* pLocalParts = NULL;
    int pLocalPartAllocated = 0, pLocalPartSize = 0;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    for (int pr = 0; pr < pLocalPartSize; ++pr)
    {
      // Check all possible combinations of boundary entities.
      for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
      {
        for (eETopo = iMesh_POINT; eETopo <= iMesh_ALL_TOPOLOGIES; ++eETopo) 
        {
          /// Now check if the combination is legal
          if (!a2qLegalTypeAndTopo[eEType][eETopo])  continue;
          iBase_EntityHandle* pPartBdryEntArr = NULL;
          /// pEntities stores the boundary entities
          pPartBdryEntArrAllocated=0;
          pPartBdryEntArrSize=0;
          /// Step 1: Get part boundary entities for all possible combinations.
 	  iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[pr],
                  static_cast<iBase_EntityType>(eEType), 
                  static_cast<iMesh_EntityTopology>(eETopo), iMeshP_ALL_PARTS, &pPartBdryEntArr,
                  &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr); 
          TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
          a2iPartBdryEntCount[eEType][eETopo] = pPartBdryEntArrSize;

          iMeshP_getNumPartBdryEnts (instance, pPtnHandles[p], pLocalParts[pr],
                  static_cast<iBase_EntityType>(eEType), static_cast<iMesh_EntityTopology>(eETopo), iMeshP_ALL_PARTS,
                  &iNumPartBdryEnt, &ierr); 
          TEST(!ierr, _FUNC_, "iMeshP_getNumPartBdryEnts");
          sprintf(msg,"matching #part bdry entity in iMeshP_getPartBdryEnts and iMeshP_getNumPartBdryEnts(%s,%s) for %d'th part", 
                  astrTypes[eEType], astrTopologies[eETopo], pr);
          TEST(pPartBdryEntArrSize==iNumPartBdryEnt, _VALID_, msg);
          UT_FREE(pPartBdryEntArr);
        } // for eTopo
      } // for eType
    } // for each part
    UT_FREE(pLocalParts);
  } // for each partition
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 


/*/////////////////////////////////////////////// 
 Test: iMeshP_getPartNbors
    * Dependencies: iMeshP_getLocalParts, iMeshP_getPartitions
    * Test Cases:
      1- Check if the function returns the correct neighboring parts w.r.t a given part.
         By definition of neighbors in iMeshP spec, part A is a neighbor of another part
         B if part A has copies of entities owned by Part B or Part B has copies of
         entities owned by part A. Cross check by getting the number of boundary entities
         shared between Part A and Part B. 
      2- If the boundary entities & ghosts shared between parts are zero and the parts
         given by function are neighbors then throw error.
/////////////////////////////////////////////// */

int vGetPartNborTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles=NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated = 0, pLocalPartSize = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  int iNumPartNbor;
  iMeshP_Part* pPartNborIdArr=NULL;
  int pPartNborIdArrAllocated=0, pPartNborIdArrSize=0;
  iMeshP_getPartNbors (instance, pPtnHandles[0], pLocalParts[0], 
              static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumPartNbor, &pPartNborIdArr,
              &pPartNborIdArrAllocated, &pPartNborIdArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartNbors");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  
  iMeshP_getNumPartNbors (instance, pPtnHandles[0], pLocalParts[0], 
              static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumPartNbor, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumPartNbors");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  else 
  {
    TEST(pPartNborIdArrSize==iNumPartNbor, _VALID_, "pPartNborIdArrSize==iNumPartNbors");
    if (pPartNborIdArrSize!=iNumPartNbor) 
    {
#ifdef DEBUG
      if (!myrank) cout<<"[proc"<<myrank
        <<"] iMeshP_getPartNbors (iBase_ALL_TYPES): #parts="<< pPartNborIdArrSize
        <<"\n    iMeshP_getNumPartNbors(iBase_ALL_TYPES): #parts="<<iNumPartNbor<<endl;
#endif
      bOK_func=false;
    }
  }

  int iNumPartBdryEnt = 0;
  iMeshP_getNumPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
                static_cast<iBase_EntityType>(iBase_ALL_TYPES),
                static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                iMeshP_ALL_PARTS, &iNumPartBdryEnt, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getNumPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pPartNborIdArr); UT_FREE(pLocalParts);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }


  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  // Test case 1 : Get the neighboring parts of a given part for all entity types

  int iGeomDim;
  iMesh_getGeometricDimension(instance, &iGeomDim, &ierr);

  for (int p = 0; p < pPtnSize; ++p) 
  {   
    ///  Get all local part handles in the partition to perform tests
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
    for (int ph = 0; ph < pLocalPartSize; ++ph) 
    {
      for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
      {
        if (eEType==iGeomDim || eEType==iBase_REGION) continue;
        iMeshP_Part* pPartNborIdArr=NULL;
        pPartNborIdArrAllocated=0, pPartNborIdArrSize=0;
        iMeshP_getPartNbors (instance, pPtnHandles[p], pLocalParts[ph], 
              static_cast<iBase_EntityType>(eEType), &iNumPartNbor, &pPartNborIdArr,
              &pPartNborIdArrAllocated, &pPartNborIdArrSize, &ierr);
        sprintf(msg,"iMeshP_getPartNbors(%s)", astrTypes[eEType]); 
        TEST(!ierr, _FUNC_, msg);
        TEST(iNumPartNbor==pPartNborIdArrSize, _VALID_, "iNumPartNbor==pPartNborIdArrSize");
 
        iMeshP_getNumPartNbors (instance, pPtnHandles[p], pLocalParts[ph], 
              static_cast<iBase_EntityType>(eEType), &iNumPartNbor, &ierr);
        sprintf(msg,"iMeshP_getNumPartNbors(%s)", astrTypes[eEType]);
        TEST(!ierr, _FUNC_, msg);  // invalid_entity_type
        TEST(iNumPartNbor==pPartNborIdArrSize, _VALID_, "iNumPartNbor==pPartNborIdArrSize");

        for (int nbor = 0; nbor < pPartNborIdArrSize; ++nbor)
        {
          /// Step 3: Get the number of boundary entity between the neighbor part and Part A. 
          iMeshP_getNumPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
                static_cast<iBase_EntityType>(eEType),
                static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                pPartNborIdArr[nbor], &iNumPartBdryEnt, &ierr);
          sprintf(msg,"iMeshP_getNumPartBdryEnts(%s, %s, nbor_part %d)", 
                astrTypes[eEType], astrTopologies[iMesh_ALL_TOPOLOGIES], pPartNborIdArr[nbor]); 
          TEST(!ierr, _FUNC_, msg);
          sprintf(msg,"#part bdry %s in n'boring part %d > 0", astrTypes[eEType], nbor);
          TEST(iNumPartBdryEnt>0, _VALID_, msg);
        } // for nbor
        UT_FREE(pPartNborIdArr);
      } // for eEType
    } // for each part
    UT_FREE(pLocalParts);
  } // for each partition
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}

/*/////////////////////////////////////////////// 
 Test: iMeshP_getNumPartNborsArr
    * Dependencies: iMeshP_getLocalParts, iMeshP_getPartitions
    * Test Cases:
      1- Check if the function returns the correct number of neighbors of given parts for
         all entity types (using iMeshP_getPartNbors). Cross check with getNumPartNbors. 
/////////////////////////////////////////////// */

int vGetPartNborArrTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles=NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated = 0, pLocalPartSize = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  int* pNumPartNborArr=NULL;
  int pNumPartNborArrAllocated=0, pNumPartNborArrSize=0;
  iMeshP_Part* pPartNborIdArr=NULL;
  int pPartNborIdArrAllocated=0, pPartNborIdArrSize=0;
  iMeshP_getPartNborsArr (instance, pPtnHandles[0], pLocalParts, pLocalPartSize, 
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), 
            &pNumPartNborArr, &pNumPartNborArrAllocated, &pNumPartNborArrSize, 
            &pPartNborIdArr, &pPartNborIdArrAllocated, &pPartNborIdArrSize, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getPartNborsArr");  
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int* pNumPartNborArr2=NULL;
  int pNumPartNborArrAllocated2=0, pNumPartNborArrSize2=0;
  int iNumPartNbor, iNumPartNbor2; 
  iMeshP_getNumPartNborsArr (instance, pPtnHandles[0], pLocalParts, pLocalPartSize, 
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), &pNumPartNborArr2,
            &pNumPartNborArrAllocated2, &pNumPartNborArrSize2, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getNumPartNborsArr");  
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  else 
  {
    iNumPartNbor=0;
    for (int i=0; i<pNumPartNborArrSize2; ++i)
      iNumPartNbor+=pNumPartNborArr2[i];
    sprintf(msg,"pPartNborIdArrSize(%d)==iNumPartNbor(%d)",pPartNborIdArrSize, iNumPartNbor);
    TEST(pPartNborIdArrSize==iNumPartNbor, _VALID_, msg);
    if (pPartNborIdArrSize!=iNumPartNbor)  bOK_func=false;
  }
  iMeshP_Part* pPartNborId=NULL;
  int pPartNborIdAllocated=0, pPartNborIdSize=0;
  iMeshP_getPartNbors (instance, pPtnHandles[0], pLocalParts[0], 
              static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumPartNbor, &pPartNborId,
              &pPartNborIdAllocated, &pPartNborIdSize, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  
  iMeshP_getNumPartNbors (instance, pPtnHandles[0], pLocalParts[0], 
              static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumPartNbor2, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  else 
  {
    sprintf(msg,"iNumPartNbor(%d)==iNumPartNbor2(%d)",iNumPartNbor, iNumPartNbor2);
    TEST(iNumPartNbor==iNumPartNbor2, _VALID_, msg);
    if (iNumPartNbor!=iNumPartNbor2) 
      bOK_func=false;
  }

  int iNumPartBdryEnt = 0;
  iMeshP_getNumPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
                static_cast<iBase_EntityType>(iBase_ALL_TYPES),
                static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
                iMeshP_ALL_PARTS, &iNumPartBdryEnt, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pNumPartNborArr); UT_FREE(pNumPartNborArr2); UT_FREE(pPartNborIdArr); UT_FREE(pPartNborId); 
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }


  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  bool bOK_valid=true;
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Step 1: Get all local part handles in the partition to perform tests
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      int* pNumPartNborArr=NULL;
      pNumPartNborArrAllocated=0; pNumPartNborArrSize=0;
      iMeshP_Part* pPartNborIdArr=NULL;
      pPartNborIdArrAllocated=0; pPartNborIdArrSize=0;
      iMeshP_getPartNborsArr (instance, pPtnHandles[p], pLocalParts, pLocalPartSize, 
            static_cast<iBase_EntityType>(eEType), 
            &pNumPartNborArr, &pNumPartNborArrAllocated, &pNumPartNborArrSize, 
            &pPartNborIdArr, &pPartNborIdArrAllocated, &pPartNborIdArrSize, &ierr); 
      sprintf(msg,"iMeshP_getPartNborsArr(%s)", astrTypes[eEType]); 
      TEST(!ierr, _FUNC_, msg);
      if (!ierr)
      {
        sprintf(msg,"pLocalPartSize(%d)==pNumPartNborArrAllocated(%d) (%s)", pLocalPartSize, pNumPartNborArrAllocated, astrTypes[eEType]);
        TEST(pLocalPartSize==pNumPartNborArrAllocated, _VALID_, msg);
        sprintf(msg,"pNumPartNborArrAllocated(%d)==pNumPartNborArrSize(%d) (%s)", pNumPartNborArrAllocated, 
                    pNumPartNborArrSize, astrTypes[eEType]);
        TEST(pNumPartNborArrAllocated==pNumPartNborArrSize, _VALID_,msg);
        sprintf(msg,"pPartNborIdArrAllocated(%d)==pPartNborIdArrSize(%d) (%s)", pPartNborIdArrAllocated, pPartNborIdArrSize, astrTypes[eEType]);
        TEST(pPartNborIdArrAllocated==pPartNborIdArrSize, _VALID_,msg);
      }

      int* pNumPartNborArr2=NULL;
      pNumPartNborArrAllocated2=0; pNumPartNborArrSize2=0;  
      iMeshP_getNumPartNborsArr (instance, pPtnHandles[p], pLocalParts, pLocalPartSize, 
            static_cast<iBase_EntityType>(eEType), &pNumPartNborArr2,
            &pNumPartNborArrAllocated2, &pNumPartNborArrSize2, &ierr); 
      sprintf(msg,"iMeshP_getNumPartNborsArr(%s)", astrTypes[eEType]);  
      TEST(!ierr, _FUNC_, msg);
      if (!ierr)
      {
        TEST(pLocalPartSize==pNumPartNborArrAllocated2, _VALID_, "pLocalPartSize==pNumPartNborArrAllocated2");
        TEST(pNumPartNborArrAllocated2==pNumPartNborArrSize2, _VALID_, "pNumPartNborArrAllocated2==pNumPartNborArrSize2");
        iNumPartNbor=0; 
        for (int i=0; i<pNumPartNborArrSize2; ++i)
          iNumPartNbor+=pNumPartNborArr2[i];
        sprintf(msg,"# total part_nbor in iMeshP_getPartNborsArr(%d) and iMeshP_getNumPartNborsArr(%d) (%s)",
                    pPartNborIdArrSize, iNumPartNbor, astrTypes[eEType]);
        TEST(pPartNborIdArrSize==iNumPartNbor, _VALID_, msg);
        bOK_valid=true;
        for (int i=0; i<pNumPartNborArrSize2; ++i)
          if (pNumPartNborArr2[i]!=pNumPartNborArr[i]) bOK_valid=false;
        sprintf(msg,"matching # individual part_nbor in iMeshP_getPartNborsArr and iMeshP_getNumPartNborsArr (%s)", astrTypes[eEType]);
        TEST(bOK_valid, _VALID_, msg);
      }

      int count=0;
      for (int ph = 0; ph < pLocalPartSize; ++ph)
      {
        iMeshP_Part* pPartNborId=NULL;
        pPartNborIdAllocated=0; pPartNborIdSize=0;
        iMeshP_getPartNbors (instance, pPtnHandles[p], pLocalParts[ph], 
              static_cast<iBase_EntityType>(eEType), &iNumPartNbor, &pPartNborId,
              &pPartNborIdAllocated, &pPartNborIdSize, &ierr);
        sprintf(msg,"iMeshP_getPartNbors(%s) for %d'th part", astrTypes[eEType], ph);
        TEST(!ierr, _FUNC_, "iMeshP_getPartNbors");
        if (!ierr)
        {
          TEST(iNumPartNbor==pPartNborIdAllocated, _VALID_, "iNumPartNbor==pPartNborIdAllocated");
          TEST(pPartNborIdAllocated==pPartNborIdSize, _VALID_, "pPartNborIdAllocated==pPartNborIdSize");
          TEST(iNumPartNbor==pNumPartNborArr[ph], _VALID_, "iNumPartNbor==pNumPartNborArr2[ph]");
          bOK_valid=true;
          for (int i=0; i<pNumPartNborArr[ph]; ++i)
            if (pPartNborIdArr[i+count]!=pPartNborId[i])
              bOK_valid=false;
          sprintf(msg,"matching part_nbor id in iMeshP_getPartNborsArr and iMeshP_getPartNbors (%s) for %d'th part", astrTypes[eEType], ph);
          TEST(bOK_valid, _VALID_, msg);
        }
        count+=iNumPartNbor;
        UT_FREE(pPartNborId);
      }
      UT_FREE(pNumPartNborArr);
      UT_FREE(pPartNborIdArr);
      UT_FREE(pNumPartNborArr2);
    } // for eEType
    UT_FREE(pLocalParts);
  } 
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

/*/////////////////////////////////////////////// 
 Test: initPartBdryEntsIterator
    * Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions
    * Test Cases:
      1- Check if the iterator is successfully returned for all possible combinations of
         types and topos.
      2- Check if the iterator performs the iMesh iterator functions like
         getNextEntIter,endEntIter and resetEntIter. 
      3- For every returned entity, check it's status w.r.t the given part. Should be
         boundary. 
/////////////////////////////////////////////// */

int vBdryEntsIteratorTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
            
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
              
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
                
  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);
            
  // Check all possible combinations of boundary entities.
  iBase_EntityIterator pPartBdryEntIter;
  iMeshP_initPartBdryEntIter (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, 0 /*non-resilient*/, &pPartBdryEntIter, &ierr);
            
  TEST(!ierr, _FUNC_, "Non-Resilient iMeshP_initPartBdryEntIter");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int iEntStatus;
  if (pPartBdryEntArrSize) 
  {
    iMeshP_getEntStatus (instance, pPtnHandles[0], pLocalParts[0], pPartBdryEntArr[0], &iEntStatus, &ierr);
    TEST(!ierr, _FUNC_, "MeshP_getEntStatus");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }
  int iNumEnts;
  iMeshP_getNumPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &iNumEnts, &ierr);
  TEST(!ierr, _FUNC_, "MeshP_getNumPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  bool bOK_valid1=true, bOK_valid2=true, bOK_valid3=true;
  iBase_EntityHandle pEntity;
  int iHas_data;

  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    int pLocalPartSize = 0, pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    // Check all possible combinations of boundary entities.
    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (eETopo = iMesh_POINT; eETopo <= iMesh_ALL_TOPOLOGIES; ++eETopo) 
      {
        /// Now check if the combination is legal
        if (!a2qLegalTypeAndTopo[eEType][eETopo]) continue;
        for (int ph = 0; ph < pLocalPartSize; ++ph)
        {   
          iBase_EntityHandle* pPartBdryEntArr = NULL;
          pPartBdryEntArrAllocated=0; pPartBdryEntArrSize=0;
          
          iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
                 static_cast<iBase_EntityType>(eEType), static_cast<iMesh_EntityTopology>(eETopo),
                 iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
          TEST(!ierr, _FUNC_, "MeshP_getPartBdryEnts");
          sprintf(msg,"MeshP_getPartBdryEnts (%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph);
          TEST(!ierr, _FUNC_, msg);
          if (!pPartBdryEntArrSize) continue;

          iBase_EntityIterator pPartBdryIter;
          iMeshP_getNumPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph], 
                 static_cast<iBase_EntityType>(eEType), static_cast<iMesh_EntityTopology>(eETopo), 
                 iMeshP_ALL_PARTS, &iNumEnts, &ierr);
          sprintf(msg,"iMeshP_getNumPartBdryEnts (%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph);
          TEST(!ierr, _FUNC_, msg);
          TEST(iNumEnts==pPartBdryEntArrSize, _VALID_, "matching #part bdry ents in iMeshP_getPartBdryEnts and iMeshP_getNumPartBdryEnts");

 	  /// Step 2: Now start the iterator
          iMeshP_initPartBdryEntIter (instance, pPtnHandles[p], pLocalParts[ph],
                 static_cast<iBase_EntityType>(eEType), static_cast<iMesh_EntityTopology>(eETopo),
                 iMeshP_ALL_PARTS,0 /* non-resilient*/, &pPartBdryIter, &ierr); 
          sprintf(msg,"MeshP_initPartBdryEntIter (%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType], 
                      astrTopologies[eETopo], ph);
          TEST(!ierr, _FUNC_, msg);

          iMesh_getNextEntIter(instance, pPartBdryIter, &pEntity, &iHas_data, &ierr);
          TEST(!ierr, _FUNC_, "iMesh_getNextEntIter");
          TEST(iHas_data, _VALID_, "iMesh_getNextEntIter returned an entity");
          bOK_func=true;
          int count=0;
	  while (iHas_data)
          {
            iMeshP_getEntStatus (instance, pPtnHandles[p], pLocalParts[ph],
                    pEntity, &iEntStatus, &ierr); 
            if (ierr) bOK_func=false;
	    if (iEntStatus != 1) bOK_valid1 = false;
	    if (pPartBdryEntArr[count]!=pEntity) bOK_valid2=false;
	    iMesh_getNextEntIter (instance, pPartBdryIter, &pEntity, &iHas_data, &ierr);
            if (ierr) bOK_func=false;
            ++count;
	  } // while (iHas_data)
          TEST(bOK_func, _FUNC_, "iMesh_getNextEntIter");
          sprintf(msg,"checking entity status(%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph); 
          TEST(bOK_valid1, _VALID_, msg);
          sprintf(msg,"matching entity in iMeshP_initPartBdryEntIter and iMeshP_getPartBdryEnts(%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph);
          TEST(bOK_valid2, _VALID_,msg);
          sprintf(msg,"matching #entities in iMeshP_initPartBdryEntIter and iMeshP_getNumPartBdryEnts(%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph);
          TEST(count==iNumEnts, _VALID_,msg);

          iMesh_resetEntIter(instance, pPartBdryIter, &ierr);
          TEST(!ierr, _FUNC_, "iMesh_resetEntIter");
          iMesh_endEntIter(instance, pPartBdryIter, &ierr);
          TEST(!ierr, _FUNC_, "iMesh_endEntIter");

          UT_FREE(pPartBdryEntArr);
        } // for each part
      }  // for eETopo
    } // for eEType
    UT_FREE(pLocalParts);
  }
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: initPartBdryArrIterator
    * Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions
    * Test Cases:
      1- Check if the iterator is returned for all possible combinations of types and
         topos.
      2- Check if the iterator performs the iMesh iterator functions like getNextEntIter
         and endEntIter. 
      3- Check the reset operation of the iterator // to be done for array iterator. 
      4- For every returned entity, check it's status w.r.t the given part. Should be
         boundary. 
/////////////////////////////////////////////// */

int vBdryEntsArrIteratorTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iBase_EntityArrIterator pPartBdryEntArrIter;
  iMeshP_initPartBdryEntArrIter (instance, pPtnHandles[0], pLocalParts[0], 
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         10, iMeshP_ALL_PARTS, 0 /*non-resilient*/, &pPartBdryEntArrIter, &ierr);
  
  TEST(!ierr, _FUNC_, "Non-Resilient iMeshP_initPartBdryEntArrIter");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int* pEntStatusArr=NULL;
  int pEntStatusArrAllocated=0, pEntStatusArrSize=0;
  if (pPartBdryEntArrSize) 
  {
    iMeshP_getEntStatusArr (instance, pPtnHandles[0], pLocalParts[0], pPartBdryEntArr, pPartBdryEntArrSize, 
           &pEntStatusArr, &pEntStatusArrAllocated, &pPartBdryEntArrSize, &ierr);
    TEST(!ierr, _FUNC_, "MeshP_getEntStatusArr");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  int iNumEnts;
  iMeshP_getNumPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &iNumEnts, &ierr);
  TEST(!ierr, _FUNC_, "MeshP_getNumPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr); 
  UT_FREE(pEntStatusArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  bool bOK_valid1=true, bOK_valid2=true, bOK_valid3=true;
  iBase_EntityHandle pEntity;
  int iHas_data;
  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrSize=0, pEntArrAllocated=0;
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    int pLocalPartSize = 0, pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    // Check all possible combinations of boundary entities.
    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (eETopo = iMesh_POINT; eETopo <= iMesh_ALL_TOPOLOGIES; ++eETopo) 
      {
        /// Now check if the combination is legal
        if (!a2qLegalTypeAndTopo[eEType][eETopo]) continue;
        for (int ph = 0; ph < pLocalPartSize; ++ph)
        {   
          iBase_EntityArrIterator pPartBdryArrIter;
          iMeshP_initPartBdryEntArrIter (instance, pPtnHandles[p], pLocalParts[ph],
                  eEType, eETopo, 10, iMeshP_ALL_PARTS, 0 /*resilient*/, 
                  &pPartBdryArrIter, &ierr);

          sprintf(msg,"MeshP_initPartBdryEntArrIter (%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType], 
                      astrTopologies[eETopo], ph);
          TEST(!ierr, _FUNC_, msg);
          if (ierr) bOK_func=false;

          iBase_EntityHandle* pPartBdryEntArr = NULL;
          pPartBdryEntArrAllocated=0; pPartBdryEntArrSize=0;      
          iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
                 static_cast<iBase_EntityType>(eEType), static_cast<iMesh_EntityTopology>(eETopo),
                 iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
          sprintf(msg,"iMeshP_getPartBdryEnts (%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph);
          TEST(!ierr, _FUNC_, msg);
          sprintf(msg,"pPartBdryEntArrAllocated(%d)==pPartBdryEntArrSize(%d) (%s, %s)", pPartBdryEntArrAllocated, 
                       pPartBdryEntArrSize, astrTypes[eEType], astrTopologies[eETopo]);
          TEST(pPartBdryEntArrAllocated==pPartBdryEntArrSize, _VALID_, msg);

          iMeshP_getNumPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph], 
                 static_cast<iBase_EntityType>(eEType), static_cast<iMesh_EntityTopology>(eETopo), 
                 iMeshP_ALL_PARTS, &iNumEnts, &ierr);
          sprintf(msg,"iMeshP_getNumPartBdryEnts (%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph);
          TEST(!ierr, _FUNC_, msg);
          sprintf(msg,"iNumEnts(%d)==pPartBdryEntArrSize(%d) (%s, %s)", iNumEnts, 
                       pPartBdryEntArrSize, astrTypes[eEType], astrTopologies[eETopo]);
          TEST(iNumEnts==pPartBdryEntArrSize, _VALID_, msg);
          if (iNumEnts!=pPartBdryEntArrSize)  bOK_func=false;
          TEST(bOK_func, _FUNC_, "checking iMeshP_initPartBdryEntArrIter and iMeshP_getPartBdryEnts");
          if (!bOK_func) 
          {
            UT_FREE(pPartBdryEntArr);
            continue;
          }
          pEntArr=NULL;
          pEntArrSize=0; pEntArrAllocated=0; 
          iMesh_getNextEntArrIter(instance, pPartBdryArrIter, &pEntArr, 
                 &pEntArrAllocated, &pEntArrSize, &iHas_data, &ierr);
         TEST(!ierr, _FUNC_, "iMesh_getNextEntArrIter");
         int count=0;
         while (iHas_data)
         {
           pEntStatusArr=NULL;
           pEntStatusArrAllocated=0; pEntStatusArrSize=0;
          // if (!myrank) cout<<"iMesh_getNextEntArrIter returned "<<pEntArrSize<<" entities\n";
           iMeshP_getEntStatusArr(instance, pPtnHandles[p], pLocalParts[ph], pEntArr, pEntArrSize, 
                  &pEntStatusArr, &pEntStatusArrAllocated, &pEntStatusArrSize, &ierr);
           if (ierr) bOK_func=false;
           bOK_valid1=true;
           for (int i=0; i<pEntArrSize; ++i)
           {
             if (pEntStatusArr[i] != iMeshP_BOUNDARY) bOK_valid1 = false;
  	     if (pPartBdryEntArr[count+i]!=pEntArr[i]) 
               bOK_valid2=false;
           }
           count+=pEntArrSize;

           UT_FREE(pEntArr);
           pEntArr=NULL;
           pEntArrAllocated=0; pEntArrSize=0;
 	   iMesh_getNextEntArrIter (instance, pPartBdryArrIter, &pEntArr, &pEntArrAllocated, &pEntArrSize,
                 &iHas_data, &ierr);
           UT_FREE(pEntStatusArr);
	  } // while (iHas_data)

          TEST(bOK_func, _FUNC_, "iMesh_getEntStatusArr");
          sprintf(msg,"checking entity status(%s,%s,iMeshP_ALL_PARTS) for %d'th part", astrTypes[eEType],
                       astrTopologies[eETopo], ph); 
          TEST(bOK_valid1, _VALID_, msg);

          sprintf(msg,"entity mismatch in iMeshP_initPartBdryEntArrIter and iMeshP_getPartBdryEnts (%s,%s,iMeshP_ALL_PARTS) for %d'th part",  
                      astrTypes[eEType], astrTopologies[eETopo], ph);
          TEST(bOK_valid2, _VALID_,msg);

          sprintf(msg,"#entities mismatch in iMeshP_initPartBdryEntArrIter(%d) and iMeshP_getNumPartBdryEnts(%d) (%s,%s,iMeshP_ALL_PARTS) for %d'th part",count, iNumEnts, astrTypes[eEType], astrTopologies[eETopo], ph);
          TEST(count==iNumEnts, _VALID_,msg);

          iMesh_resetEntArrIter(instance, pPartBdryArrIter, &ierr);
          TEST(!ierr, _FUNC_, "iMesh_resetEntArrIter");
          iMesh_endEntArrIter(instance, pPartBdryArrIter, &ierr);
          TEST(!ierr, _FUNC_, "iMesh_endEntArrIter");

          UT_FREE(pEntArr);
          UT_FREE(pPartBdryEntArr);
        } // for each part
      }  // for eETopo
    } // for eEType
    UT_FREE(pLocalParts);
  }
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}

////////////////////////////////////////
//  ENTITY OWNERSHIP FUNCTIONALITY    //
////////////////////////////////////////

/*/////////////////////////////////////////////// 
 Test: iMeshP_getEntOwnerPart
    * Pre-req: Get boundary entities between parts.
    * Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    iMeshP_getPartNbors, iMeshP_getPartIdFromPartHandle, iMesh_getRootSet.
    * Test Cases:
      1- Get the boundary entities and get their owning parts. 
      2- Check against the local part if it is the owner of the boundary entity. If not,
         then the owner should be among the neighbors.

    According to the iMeshP specification, Part A "neighbors" Part B if Part A has copies 
    of entities owned by Part B and/or if Part B has copies of entities owned by Part A. 
    Therefore, the owning part should be among the neighbors of the given part A. If not 
    then fail. 
/////////////////////////////////////////////// */

int vGetEntOwnerPartTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  // Check all possible combinations of boundary entities.
  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  iMeshP_Part pOwnerPartId;
  if (pPartBdryEntArrSize)
  { 
    iMeshP_getEntOwnerPart (instance, pPtnHandles[0], pPartBdryEntArr[0], &pOwnerPartId, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getEntOwnerPart");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  iMeshP_Part pPartId;
  iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[0], pLocalParts[0], &pPartId, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iNumPartNbor;
  iMeshP_Part* pPartNborIdArr=NULL;
  int pPartNborIdArrAllocated=0, pPartNborIdArrSize=0;
  iMeshP_getPartNbors (instance, pPtnHandles[0], pLocalParts[0], 
              static_cast<iBase_EntityType>(iBase_ALL_TYPES), &iNumPartNbor, &pPartNborIdArr,
              &pPartNborIdArrAllocated, &pPartNborIdArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartNbors");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr); 
  UT_FREE(pPartNborIdArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize = 0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;

    iMeshP_getLocalParts (instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
    
    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (eETopo = iMesh_POINT; eETopo <= iMesh_ALL_TOPOLOGIES; ++eETopo) 
      {
        /// Now check if the type-topo combination is legal
        if (a2qLegalTypeAndTopo[eEType][eETopo]) 
        { 
	  // bOK_funcis test case value for getEntOwnerPart success/failure.
          // bOK_valid is test case value for checking if the owner is self or among the
          // neighbor. 
	  bool bOK_func= true, bOK_valid = true;
          for (int pr = 0; pr < pLocalPartSize; ++pr)
      	  {   
            iBase_EntityHandle* pEntities = NULL;
            int iEntsAllocated=0, iEntsSize=0;
 
            /// Return boundary entities between the local part and all other parts

            iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[pr], 
                  eEType, eETopo, iMeshP_ALL_PARTS, &pEntities, &iEntsAllocated, 
                  &iEntsSize, &ierr); 

            /// If we fail in getting boundary entities, we fail both test cases and
            /// break.
            CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");

            if (ierr!=iBase_SUCCESS)
            {
              fprintf(fp, "\niMeshP_getPartBdryEnts Failed: Type %s, Topology %s \n", astrTypes[eEType], astrTopologies[eETopo]);
#ifdef DEBUG
            if (!myrank)
              fprintf(stdout, "\niMeshP_getPartBdryEnts Failed: Type %s, Topology %s \n", astrTypes[eEType], astrTopologies[eETopo]);
#endif
              bOK_func= false; bOK_valid = false;
              break;
            }

            /// Step 2: For every boundary entity, get its owner part. 
            /// If owner is not equal to the current part handle, then the owner should
            /// be among the neighbors.

            for (int ii = 0; ii < iEntsSize; ++ii)
            {
              /// Get entity's owning part id.
              iMeshP_Part pPartId;

              iMeshP_getEntOwnerPart (instance, pPtnHandles[p], pEntities[ii],
		    &pPartId, &ierr);     
              CHECK_ERR(ierr, "iMeshP_getEntOwnerPart");

              /// bOK_valid depends on test case for bOK_funcso we fail both.
              if (ierr != iBase_SUCCESS)
              {
                bOK_func= false; bOK_valid = false;
	        break;
              }

              /// Step 3: Get the unique part id of local part handle.
              iMeshP_Part pPartIdLoc;

              iMeshP_getPartIdFromPartHandle (instance, pPtnHandles[p], pLocalParts[pr],
                    &pPartIdLoc, &ierr);

              CHECK_ERR(ierr, "getPartIdFromPartHandle");
              if (ierr!=iBase_SUCCESS)
              {
                bOK_valid = false; 
         	break;
              }
 	
              /// Step 4: Check if the owning part is the same as the current part used
              /// in getEntities. If not then the owner should be among the neighbors.

              if (pPartId != pPartIdLoc)
              {
                /* According to the iMeshP specification, Part A "neighbors" Part B if
                   Part A has copies of entities owned by Part B and/or if Part B has
                   copies of entities owned by Part A. If part A(current part) has a
                   boundary or ghost entity owned by some other part B then part B must
                   be A's neighbor*/

                /// Step 6: Get neighbors of the current part pLocalParts[pr], the owning
                /// part should be one of them.

                int iNumPartNbors, iNborIdsAllocated=0, iNborIdsSize=0;
                iMeshP_Part* iNborIds = NULL;

                iMeshP_getPartNbors(instance, pPtnHandles[p], pLocalParts[pr], eEType,
                      &iNumPartNbors, &iNborIds, &iNborIdsAllocated, &iNborIdsSize, &ierr);
 
                CHECK_ERR(ierr, "iMeshP_getPartNbors");

     	        if (ierr!=iBase_SUCCESS)
  	        {
	          bOK_valid = false;
                  break;
            	}

                bool bIsThere = false;

                /// Step 7: Loop through neighbor ids and search for the owning part. 
                /// If not found then fail.
                for (int jj = 0; jj < iNborIdsSize; ++jj)
                {
           	  if (iNborIds[jj] == pPartId)
           	  {
                    bIsThere = true;	
             	    break;
           	  }
                } // for jj
                if (!bIsThere)
	        {
           	  CHECK_VALIDATION_ERR("iMeshP_getEntOwnerPart");
           	  bOK_valid = false;
                  break;
         	} // if (!bIsThere)
                UT_FREE(iNborIds);
	      } // if (pPartId != pPartIdLoc)     
            } // for ii
            UT_FREE(pEntities);  
	  } // for pr
   	  TEST(bOK_func, 0, ""); 
          TEST(bOK_valid, 1, "");
        } // if (a2qLegalTypeAndTopo[eEType][eETopo])
      } // for eTopo
    } // for eType
    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: iMeshP_getEntOwnerPartArr
    * Pre-req: Get the entities w.r.t a given local part A and set.
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions,
                    iMeshP_getPartHandleFromPartId
    * Test Cases:
      1- Get owning parts of given entities. Check if function succeeds.
      2- Check the owning parts of the given entities.
         - If the owning parts are the same as Part A then pass.
         - If the owning parts are different, check the entity status w.r.t the given
           part A. The owning status should be boundary or ghost. In that case, check
           neighbors like we did in the last test case. If owning part is among neighbors
           then pass else fail.
/////////////////////////////////////////////// */

int vGetEntOwnerPartArrTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrAllocated=0, pEntArrSize=0;

  iMeshP_getEntities (instance, pPtnHandles[0], pLocalParts[0], root,
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),  &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_Part* pOwnerPartIdArr = NULL;
  int pOwnerPartIdArrAllocated=0, pOwnerPartIdArrSize=0;

  iMeshP_getEntOwnerPartArr (instance, pPtnHandles[0], pEntArr, pEntArrSize,
                 &pOwnerPartIdArr, &pOwnerPartIdArrAllocated, &pOwnerPartIdArrSize, &ierr);
           CHECK_ERR(ierr, "iMeshP_getEntOwnerPartArr");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_Part pOwnerPartId;
  if (pEntArrSize)
  { 
    iMeshP_getEntOwnerPart (instance, pPtnHandles[0], pEntArr[0], &pOwnerPartId, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getEntOwnerPart");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  iMeshP_Part pPartId;
  iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[0], pLocalParts[0], &pPartId, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pEntArr); UT_FREE(pOwnerPartIdArr); 
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }


  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  bool bOK_valid=true;
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize = 0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
  
      // Check all possible combinations of boundary entities.
    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (eETopo = iMesh_POINT; eETopo <= iMesh_ALL_TOPOLOGIES; ++eETopo) 
      {
        /// Now check if the combination is legal
        if (!a2qLegalTypeAndTopo[eEType][eETopo]) continue;
        
        for (int ph = 0; ph < pLocalPartSize; ++ph)
        {  
          iBase_EntityHandle* pEntArr= NULL;
          pEntArrAllocated=0, pEntArrSize=0;

          iMeshP_getEntities (instance, pPtnHandles[p], pLocalParts[ph], root,
                 eEType, eETopo, &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
          sprintf(msg,"iMeshP_getEntities(%s, %s)", astrTypes[eEType], astrTopologies[eETopo]); 
          TEST(!ierr, _FUNC_, msg);
          TEST(pEntArrAllocated==pEntArrSize, _VALID_, "iEntArrAllocated==iEntArrSize");

          iMeshP_Part* pOwnerPartIdArr = NULL;
          pOwnerPartIdArrAllocated=0; pOwnerPartIdArrSize=0;

          iMeshP_getEntOwnerPartArr (instance, pPtnHandles[p], pEntArr, pEntArrSize,
   	         &pOwnerPartIdArr, &pOwnerPartIdArrAllocated, &pOwnerPartIdArrSize, &ierr);
          sprintf(msg,"iMeshP_getEntOwnerPartArr(%s, %s)", astrTypes[eEType], astrTopologies[eETopo]);
          TEST(!ierr, _FUNC_, msg);
           sprintf(msg,"pEntArrSize(%d)==pOwnerPartIdArrAllocated(%d) (%s, %s)", pEntArrSize, pOwnerPartIdArrAllocated, astrTypes[eEType], astrTopologies[eETopo]);
          TEST(pEntArrSize==pOwnerPartIdArrAllocated, _VALID_, msg);
          sprintf(msg,"pOwnerPartIdArrAllocated(%d)==pOwnerPartIdArrSize(%d) (%s, %s)", pOwnerPartIdArrAllocated, pOwnerPartIdArrSize, astrTypes[eEType], astrTopologies[eETopo]);
          TEST(pOwnerPartIdArrAllocated==pOwnerPartIdArrSize, _VALID_,msg);

          bOK_valid=true;
          for (int j=0; j<pEntArrSize; ++j)
          {
            iMeshP_getEntOwnerPart (instance, pPtnHandles[p], pEntArr[j], &pOwnerPartId, &ierr);
            if (pOwnerPartIdArr[j]!=pOwnerPartId) bOK_valid=false;
          }
          sprintf(msg,"matching iMeshP_getEntOwnerPartArr and iMeshP_getEntOwnerPart (%s, %s) for %d'th part", astrTypes[eEType], astrTopologies[eETopo], ph);
          TEST(bOK_valid, _VALID_, msg);
          UT_FREE(pEntArr);
          UT_FREE(pOwnerPartIdArr);
        } // for each part
      } // for eETopo
    } // for eEType
    UT_FREE(pLocalParts);
  }// for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}

/*/////////////////////////////////////////////// 
 Test: isEntOwner
    * Pre-req: Get the entities w.r.t a given part A and set. 
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    iMeshP_getEntStatus
    * Test Cases:
      1- check isEntOwner function and its parameters. 
      2- Check the entity ownership status w.r.t the given part. If it is owned, then the entity status w.r.t the given part can be internal or boundary.  
/////////////////////////////////////////////// */

int vIsEntOwnerTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int iIsOwner;
  iMeshP_Part pOwnerPartId;
  if (pPartBdryEntArrSize)
  { 
    iMeshP_isEntOwner(instance, pPtnHandles[0], pLocalParts[0], pPartBdryEntArr[0], &iIsOwner, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_isEntOwner");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

    iMeshP_getEntOwnerPart (instance, pPtnHandles[0], pPartBdryEntArr[0], &pOwnerPartId, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getEntOwnerPart");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  iMeshP_Part pPartId;
  iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[0], pLocalParts[0], &pPartId, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize = 0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    /// Loop through all local parts of a partition.
    bool bOK_func= true, bOK_valid = true;

    for (int ph = 0; ph < pLocalPartSize; ++ph)
    { 
      iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[p], pLocalParts[ph], &pPartId, &ierr);

      iBase_EntityHandle* pPartBdryEntArr = NULL;
      pPartBdryEntArrAllocated=0; pPartBdryEntArrSize=0;

      iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
             static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),        
             iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
      TEST(pPartBdryEntArrAllocated==pPartBdryEntArrSize, _VALID_, "pPartBdryEntArrAllocated==pPartBdryEntArrSize");

      for (int i=0; i<pPartBdryEntArrSize; ++i)
      {
        iMeshP_isEntOwner(instance, pPtnHandles[p], pLocalParts[ph], pPartBdryEntArr[i], &iIsOwner, &ierr);
        if (ierr) bOK_func=false;

        iMeshP_getEntOwnerPart (instance, pPtnHandles[p], pPartBdryEntArr[i], &pOwnerPartId, &ierr);
        if (ierr) bOK_func=false;

        if (iIsOwner && pOwnerPartId!=pPartId) bOK_valid=false;
      }
      TEST(bOK_func, _FUNC_, "checking iMeshP_isEntOwner and iMeshP_getEntOwnerPart");
      TEST(bOK_valid, _VALID_, "matching iMeshP_isEntOwner and iMeshP_getEntOwnerPart");
      UT_FREE(pPartBdryEntArr);
    }
    UT_FREE(pLocalParts);
  } // for ptn
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}

/*/////////////////////////////////////////////// 
 Test: iMeshP_isEntOwnerArr
    * Pre-req: et the entities w.r.t a given part A and set. 
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    iMeshP_getEntStatus
    * Test Cases:
      1- Test if isEntOwnerArr function call throws error. Check output parameters.
      2- Check if the entity is owned by the given part.
         - If yes, then get the status of the entity. Should be internal or boundary.
         - If no, get the status of the entity. Should be boundary or ghost.
/////////////////////////////////////////////// */

int vIsEntOwnerArrTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
    
  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0; 
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);
      
  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrAllocated=0, pEntArrSize=0;
      
  iMeshP_getEntities (instance, pPtnHandles[0], pLocalParts[0], root,
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),  
         &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");
  TEST(pEntArrSize, _VALID_, "no entity in iMeshP_getEntities");
  if (ierr==iBase_NOT_SUPPORTED||!pEntArrSize) bOK_func=false;

  int* pIsOwnerArr = NULL;
  int pIsOwnerArrAllocated=0, pIsOwnerArrSize=0;

  iMeshP_isEntOwnerArr(instance, pPtnHandles[0], pLocalParts[0], pEntArr, 
            pEntArrSize, &pIsOwnerArr, &pIsOwnerArrAllocated, &pIsOwnerArrSize, &ierr );
  TEST(!ierr, _FUNC_, "iMeshP_isEntOwnerArr");
  TEST(pEntArrSize==pIsOwnerArrSize, _VALID_, "checking output size in iMeshP_isEntOwnerArr");
  if (ierr==iBase_NOT_SUPPORTED||pEntArrSize!=pIsOwnerArrSize) bOK_func=false;
        
  iMeshP_Part* pOwnerPartIdArr = NULL;
  int pOwnerPartIdArrAllocated=0, pOwnerPartIdArrSize=0;

  iMeshP_getEntOwnerPartArr (instance, pPtnHandles[0], pEntArr, pEntArrSize,
                 &pOwnerPartIdArr, &pOwnerPartIdArrAllocated, &pOwnerPartIdArrSize, &ierr);
           CHECK_ERR(ierr, "iMeshP_getEntOwnerPartArr");
  TEST(!ierr, _FUNC_, "iMeshP_getEntOwnerPartArr");
  sprintf(msg,"pEntArrSize(%d)==pOwnerPartIdArrSize(%d)", pEntArrSize, pOwnerPartIdArrSize);
  TEST(pEntArrSize==pOwnerPartIdArrSize, _VALID_,msg);
  if (ierr==iBase_NOT_SUPPORTED||pEntArrSize!=pOwnerPartIdArrSize) bOK_func=false;
           
  iMeshP_Part pPartId;  
  iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[0], pLocalParts[0], &pPartId, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartIdFromPartHandle");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pEntArr); UT_FREE(pOwnerPartIdArr); UT_FREE(pIsOwnerArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// Loop through all partitions
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize =0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    for (int ph = 0; ph < pLocalPartSize; ++ph)
    { 
      iBase_EntityHandle* pEntArr= NULL;
      pEntArrAllocated=0, pEntArrSize=0;

      iMeshP_getEntities (instance, pPtnHandles[p], pLocalParts[ph], root,
             static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), 
             &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getEntities");
      TEST(pEntArrAllocated==pEntArrSize, _VALID_, "pEntArrAllocated==pEntArrSize");

      iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[p], pLocalParts[ph], &pPartId, &ierr);

      int* pIsOwnerArr = NULL;
      pIsOwnerArrAllocated=0; pIsOwnerArrSize=0;

      iMeshP_isEntOwnerArr(instance, pPtnHandles[p], pLocalParts[ph], pEntArr, 
            pEntArrSize, &pIsOwnerArr, &pIsOwnerArrAllocated, &pIsOwnerArrSize, &ierr );
      TEST(!ierr, _FUNC_, "iMeshP_isEntOwnerArr");
      sprintf(msg,"pEntArrSize(%d)==pIsOwnerArrAllocated(%d)", pEntArrSize, pIsOwnerArrAllocated); 
      TEST(pEntArrSize==pIsOwnerArrAllocated, _VALID_, msg);
      sprintf(msg,"pIsOwnerArrAllocated(%d)==pIsOwnerArrSize(%d)", pIsOwnerArrAllocated, pIsOwnerArrSize);
      TEST(pIsOwnerArrAllocated==pIsOwnerArrSize, _VALID_, msg);

      iMeshP_Part* pOwnerPartIdArr = NULL;
      pOwnerPartIdArrAllocated=0; pOwnerPartIdArrSize=0;

      iMeshP_getEntOwnerPartArr (instance, pPtnHandles[p], pEntArr, pEntArrSize,
                 &pOwnerPartIdArr, &pOwnerPartIdArrAllocated, &pOwnerPartIdArrSize, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getEntOwnerPartArr");
      sprintf(msg,"pEntArrSize(%d)==pOwnerPartIdArrAllocated(%d)", pEntArrSize, pOwnerPartIdArrAllocated);
      TEST(pEntArrSize==pOwnerPartIdArrAllocated, _VALID_, msg);
      sprintf(msg,"pOwnerPartIdArrAllocated(%d)==pOwnerPartIdArrSize(%d)", pOwnerPartIdArrAllocated, pOwnerPartIdArrSize);
      TEST(pOwnerPartIdArrAllocated==pOwnerPartIdArrSize, _VALID_, msg);

      bool bOK_valid=true;
      for (int i=0; i<pEntArrSize; ++i)
        if (pIsOwnerArr[i] && pOwnerPartIdArr[i]!=pPartId)
          bOK_valid=false;
      TEST(bOK_valid, _VALID_, "matching iMeshP_isEntOwnerArr and iMeshP_getEntOwnerPartArr");
      
      UT_FREE(pEntArr);
      UT_FREE(pIsOwnerArr);
      UT_FREE(pOwnerPartIdArr);
    }
    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: iMeshP_getNumCopies
    * Pre-req: Get the total number of entities of ALL_TYPES in the partition, Get the number of entities common in the root set and part.
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    iMeshP_getNumOfTypeAll
    * Test Cases:
      1- Check the number of copies in the partition.
      2- entities are copies on the neighbors. For every entity, do getNumCopies.  
/////////////////////////////////////////////// */

int vGetEntCopiesTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int iNumCopy;
  iMeshP_Part* pPartIdArr=NULL; 
  int pPartIdArrAllocated=0, pPartIdArrSize=0;
  iBase_EntityHandle* pEntCopyArr=NULL;
  int pEntCopyArrAllocated=0, pEntCopyArrSize=0;
  if (pPartBdryEntArrSize)
  {
    iMeshP_getCopies(instance, pPtnHandles[0], pPartBdryEntArr[0], &pPartIdArr, &pPartIdArrAllocated, 
           &pPartIdArrSize, &pEntCopyArr, &pEntCopyArrAllocated, &pEntCopyArrSize, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getCopies");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

    iMeshP_getNumCopies(instance, pPtnHandles[0], pPartBdryEntArr[0], &iNumCopy, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getNumCopies");
    TEST(iNumCopy==pEntCopyArrSize, _VALID_, "matching iMeshP_getCopies and iMeshP_getNumCopies");
    if (ierr==iBase_NOT_SUPPORTED || iNumCopy!=pEntCopyArrSize) bOK_func=false;
  } 
  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr);
  UT_FREE(pPartIdArr); 
  UT_FREE(pEntCopyArr);

  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize = 0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    /// Loop through all local parts of a partition.
    bool bOK_func1=true, bOK_func2=true;
    bool bOK_valid1 = true, bOK_valid2 = true, bOK_valid3 = true, bOK_valid4 = true;

    for (int ph = 0; ph < pLocalPartSize; ++ph)
    { 
      iBase_EntityHandle*  pPartBdryEntArr = NULL;
      pPartBdryEntArrAllocated=0; pPartBdryEntArrSize=0;

      iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
             static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),        
             iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
      TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
      TEST(pPartBdryEntArrAllocated==pPartBdryEntArrSize, _VALID_, "pPartBdryEntArrAllocated==pPartBdryEntArrSize");

      for (int i=0; i<pPartBdryEntArrSize; ++i)
      {
        iMeshP_Part* pPartIdArr=NULL; 
        pPartIdArrAllocated=0; pPartIdArrSize=0;
        iBase_EntityHandle* pEntCopyArr=NULL;
        pEntCopyArrAllocated=0; pEntCopyArrSize=0;
        iMeshP_getCopies(instance, pPtnHandles[p], pPartBdryEntArr[i], &pPartIdArr, &pPartIdArrAllocated, 
           &pPartIdArrSize, &pEntCopyArr, &pEntCopyArrAllocated, &pEntCopyArrSize, &ierr);
        if (ierr) bOK_func1=false;

        if (pPartIdArrAllocated!=pPartIdArrSize) bOK_valid1=false;
        if (pEntCopyArrAllocated!=pEntCopyArrSize) bOK_valid2=false;
        if (pPartIdArrSize!=pEntCopyArrSize) bOK_valid3=false;

        iMeshP_getNumCopies(instance, pPtnHandles[p], pPartBdryEntArr[i], &iNumCopy, &ierr);
        if (ierr) bOK_func2=false;
        if (pEntCopyArrSize!=iNumCopy) bOK_valid4=false;
        UT_FREE(pPartIdArr);
        UT_FREE(pEntCopyArr);
      } // for (int i=0; i<pPartBdryEntArrSize; ++i)
      TEST(bOK_func1, _FUNC_, "iMeshP_getCopies");
      TEST(bOK_func2, _FUNC_, "iMeshP_getNumCopies");
      TEST(bOK_valid1, _VALID_, "checking partid allocated and size in iMeshP_getCopies");
      TEST(bOK_valid2, _VALID_, "checking entity copy allocated and size in iMeshP_getCopies");
      TEST(bOK_valid3, _VALID_, "checking partid size and entity copy size in iMeshP_getCopies");
      TEST(bOK_valid4, _VALID_, "checking iMeshP_getCopies and iMeshP_getNumCopies");

      UT_FREE(pPartBdryEntArr);
    } // for each part
    UT_FREE(pLocalParts);
  } // for ptn
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}

/*/////////////////////////////////////////////// 
 Test: iMeshP_getCopyOnPart
    * Pre-req: Get the entities w.r.t a given part A and set. 
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    iMeshP_getPartIdFromPartHandle, iMeshP_isEntOwner, iMeshP_getEntStatus
    * Test Cases:
      1- For every owned boundary entity on part A and some other part call getCopyOnPart on all neighbors. If it fails for all the neighbors then fail test case.  
/////////////////////////////////////////////// */

int vGetEntCopyOnPartTest(iMesh_Instance instance)
{
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int iNumCopy;
  iMeshP_Part* pPartIdArr1=NULL; 
  int pPartIdArrAllocated1=0, pPartIdArrSize1=0;
  iMeshP_Part* pPartIdArr2=NULL; 
  int pPartIdArrAllocated2=0, pPartIdArrSize2=0;
  iBase_EntityHandle* pEntCopyArr=NULL;
  int pEntCopyArrAllocated=0, pEntCopyArrSize=0;
  iBase_EntityHandle pEntCopy;
  if (pPartBdryEntArrSize)
  {
    iMeshP_getCopies(instance, pPtnHandles[0], pPartBdryEntArr[0], &pPartIdArr1, &pPartIdArrAllocated1, 
           &pPartIdArrSize1, &pEntCopyArr, &pEntCopyArrAllocated, &pEntCopyArrSize, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getCopies");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

    iMeshP_getNumCopies(instance, pPtnHandles[0], pPartBdryEntArr[0], &iNumCopy, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_getNumCopies");
    sprintf(msg, "iNumCopy(%d)==pEntCopyArrSize(%d)", iNumCopy, pEntCopyArrSize); 
    TEST(iNumCopy==pEntCopyArrSize, _VALID_, msg);
    if (ierr==iBase_NOT_SUPPORTED || iNumCopy!=pEntCopyArrSize) bOK_func=false;

    if (pEntCopyArrSize)
    {
      iMeshP_getCopyOnPart(instance, pPtnHandles[0], pPartBdryEntArr[0],pPartIdArr1[0], &pEntCopy, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getCopyOnPart");
      if (ierr==iBase_NOT_SUPPORTED);

      iMeshP_getCopyParts(instance, pPtnHandles[0], pPartBdryEntArr[0], &pPartIdArr2, &pPartIdArrAllocated2, 
           &pPartIdArrSize2, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getCopyParts");
      if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
    } 
  }

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr);
  UT_FREE(pPartIdArr1); 
  UT_FREE(pPartIdArr2); 
  UT_FREE(pEntCopyArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize = 0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);

    /// Loop through all local parts of a partition.
    bool bOK_func1=true, bOK_func2=true;
    bool bOK_valid1 = true, bOK_valid2 = true, bOK_valid3 = true, bOK_valid4 = true;

    for (int ph = 0; ph < pLocalPartSize; ++ph)
    { 
      iBase_EntityHandle* pPartBdryEntArr = NULL;
      pPartBdryEntArrAllocated=0; pPartBdryEntArrSize=0;

      iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
             static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),        
             iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
      TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
      TEST(pPartBdryEntArrAllocated==pPartBdryEntArrSize, _VALID_, "pPartBdryEntArrAllocated==pPartBdryEntArrSize");

      for (int i=0; i<pPartBdryEntArrSize; ++i)
      {
        iMeshP_Part* pPartIdArr1=NULL; 
        pPartIdArrAllocated1=0; pPartIdArrSize1=0;
        iBase_EntityHandle* pEntCopyArr=NULL;
        pEntCopyArrAllocated=0; pEntCopyArrSize=0;
        iMeshP_getCopies(instance, pPtnHandles[p], pPartBdryEntArr[i], &pPartIdArr1, &pPartIdArrAllocated1, 
           &pPartIdArrSize1, &pEntCopyArr, &pEntCopyArrAllocated, &pEntCopyArrSize, &ierr);
        if (ierr) bOK_func1=false;

        if (pPartIdArrAllocated1!=pPartIdArrSize1) bOK_valid1=false;
        if (pEntCopyArrAllocated!=pEntCopyArrSize) bOK_valid2=false;
        if (pPartIdArrSize1!=pEntCopyArrSize) bOK_valid3=false;

        iMeshP_getNumCopies(instance, pPtnHandles[p], pPartBdryEntArr[i], &iNumCopy, &ierr);
        if (ierr) bOK_func2=false;
        if (pEntCopyArrSize!=iNumCopy) bOK_valid4=false;
        UT_FREE(pPartIdArr1);
        UT_FREE(pEntCopyArr);
      } // for (int i=0; i<pPartBdryEntArrSize; ++i)
      TEST(bOK_func1, _FUNC_, "iMeshP_getCopies");
      TEST(bOK_func2, _FUNC_, "iMeshP_getNumCopies");
      TEST(bOK_valid1, _VALID_, "checking partid allocated and size in iMeshP_getCopies");
      TEST(bOK_valid2, _VALID_, "checking entity copy allocated and size in iMeshP_getCopies");
      TEST(bOK_valid3, _VALID_, "checking partid size and entity copy size in iMeshP_getCopies");
      TEST(bOK_valid4, _VALID_, "checking iMeshP_getCopies and iMeshP_getNumCopies");

      UT_FREE(pPartBdryEntArr);
    } // for each part
    UT_FREE(pLocalParts);
  } // for ptn
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: iMeshP_getOwnerCopy
    * Pre-req: Get the entities w.r.t a given part A and set. 
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    getPartIdFromPartHandle, isEntOwner, getEntStatus
    * Test Cases:
      1- Get the entities ownership status. if it is boundary & owner OR internal then get the owner copy. 
      2- Compare the returned entity copy with original one. If different, fail. TO BE REVISED
      3- Compare the returned owner part id with what we determined using isEntOwner. if different, fail.  
/////////////////////////////////////////////// */

int vGetEntOwnerCopyTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;

  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  iMeshP_Part pPartId;
  iMeshP_getPartIdFromPartHandle (instance, pPtnHandles[0], pLocalParts[0],
         &pPartId, &ierr);
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int iNumCopy, iIsOwner;
  iMeshP_Part* pPartIdArr1=NULL; 
  int pPartIdArrAllocated1=0, pPartIdArrSize1=0;
  iMeshP_Part* pPartIdArr2=NULL; 
  int pPartIdArrAllocated2=0, pPartIdArrSize2=0;
  iBase_EntityHandle* pEntCopyArr=NULL;
  int pEntCopyArrAllocated=0, pEntCopyArrSize=0;
  iBase_EntityHandle pOwnerEnt;
  iMeshP_Part pOwnerPartId;

  if (pPartBdryEntArrSize)
  {
    iMeshP_isEntOwner(instance, pPtnHandles[0], pLocalParts[0], pPartBdryEntArr[0], &iIsOwner, &ierr); 
    TEST(!ierr, _FUNC_, "iMeshP_isEntOwner");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
    iMeshP_getOwnerCopy(instance, pPtnHandles[0], pPartBdryEntArr[0], &pOwnerPartId,
                &pOwnerEnt, &ierr); 
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  UT_FREE(pLocalParts); 
  UT_FREE(pPartBdryEntArr);
  UT_FREE(pPartIdArr1); 
  UT_FREE(pPartIdArr2); 
  UT_FREE(pEntCopyArr);

  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    pLocalPartSize = 0; pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
    bool bOK_func1=true, bOK_func2=true;
    bool bOK_valid1 = true,bOK_valid2 = true;
    for (int ph = 0; ph < pLocalPartSize; ++ph)
    { 
      iBase_EntityHandle* pPartBdryEntArr = NULL;
      pPartBdryEntArrAllocated=0; pPartBdryEntArrSize=0;

      iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[ph],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
         iMeshP_ALL_PARTS, &pPartBdryEntArr, &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr);
      TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
      TEST(pPartBdryEntArrAllocated==pPartBdryEntArrSize, _VALID_, "pPartBdryEntArrAllocated==pPartBdryEntArrSize");

      iMeshP_getPartIdFromPartHandle (instance, pPtnHandles[p], pLocalParts[ph], &pPartId, &ierr);
      
      iMeshP_Part* pPartIdArr1=NULL; 
      pPartIdArrAllocated1=0; pPartIdArrSize1=0;
      iMeshP_Part* pPartIdArr2=NULL; 
      pPartIdArrAllocated2=0; pPartIdArrSize2=0;
      iBase_EntityHandle* pEntCopyArr=NULL;
      pEntCopyArrAllocated=0; pEntCopyArrSize=0;
      bOK_func1=true;
      for (int i=0; i<pPartBdryEntArrSize; ++i)
      {
        iMeshP_isEntOwner(instance, pPtnHandles[p], pLocalParts[ph], pPartBdryEntArr[i], &iIsOwner, &ierr); 
        if (ierr) bOK_func1=false;

        iMeshP_getOwnerCopy(instance, pPtnHandles[p], pPartBdryEntArr[i], &pOwnerPartId, &pOwnerEnt, &ierr); 
        if (ierr) bOK_func2=false;

        if (bOK_func1&&bOK_func2&&iIsOwner)
        {
          if (pOwnerPartId!=pPartId) bOK_valid1=false;
          if (pPartBdryEntArr[i]!=pOwnerEnt) bOK_valid2=false;
        }
      }
      TEST(bOK_func1, _FUNC_, "iMeshP_isEntOwner");
      TEST(bOK_func2, _FUNC_, "iMeshP_getOwnerCopy");
      TEST(bOK_valid1, _VALID_, "checkine owner part id for iMeshP_isEntOwner and iMeshP_getOwnerCopy");
      TEST(bOK_valid2, _VALID_, "checkine owner entity copy for iMeshP_isEntOwner and iMeshP_getOwnerCopy");
      UT_FREE(pPartBdryEntArr);
      UT_FREE(pPartIdArr1);
      UT_FREE(pPartIdArr2);
      UT_FREE(pEntCopyArr);
    }
    UT_FREE(pLocalParts);
  }
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: Gets the status (internal, ghost or boundary) of a given entity.
    * Pre-req: Get the entities w.r.t a given part A and set. 
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
                    getPartIdFromPartHandle, isEntOwner, getEntStatus
    * Test Cases:
      1- Get the entity status w.r.t the given part. For vertex entity type, get the upward adjacency; for rest of the types, get the downward adjacencies. Entity status should follow the following table.

             Vertex             Edge          Face               Region
Vertex I      x                 I              I,B		  I, (G?)
       B      x                 B,I	       B,I		  I
       G      x                 G,I,B          G,(I?)	          G, (I?)

Edge   I     I,B                x	       x                  x
       B     B                  x	       x                  x
       G     G                  x              x                  x

Face   I    I,B,G               I,B (G?)        x                  x
       B    B                  B               x                  x
       G    I,B,G              I,B,G           x                  x

Region I     I,B,G             I,B,G           B,G,I              x  
       B     x			x	       x                  x
       G     G			G	       G		  x

/////////////////////////////////////////////// */

int vGetEntStatusTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting entity status w.r.t a part: getEntStatus------------\n");
#ifdef DEBUG
  if (!myrank)
  fprintf(stdout, "\n \n ------------- Getting entity status w.r.t a part: getEntStatus------------\n");
#endif 

  /// get root set from the mesh instance, if doesn't succeed we can't proceed so throw
  /// error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

  /// Loop through all partitions
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    int pLocalPartSize = 0, pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
    CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

    if (pLocalPartSize != INIT_NUM_PARTS)
    {
      CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
      iNFailed++;
      return iBase_FAILURE;
    }

    // Check all possible combinations of boundary entities.
    for (eEType = iBase_VERTEX; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (eETopo = iMesh_POINT; eETopo <= static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES); ++eETopo) 
      {
        /// Now check if the combination is legal
        if (a2qLegalTypeAndTopo[eEType][eETopo]) 
        {
	  bool bOK_func= true, bOK_valid = true;

	  for (int pr = 0; pr < pLocalPartSize; ++pr)
    	  {  
            iBase_EntityHandle* pEntities = NULL;
            int iEntsAllocated=0, iEntsSize;
            /// Step 1: Get all entities common to the given part and set
            iMeshP_getEntities (instance, pPtnHandles[p], pLocalParts[pr], root, eEType,
                  eETopo, &pEntities, &iEntsAllocated, &iEntsSize, &ierr);
            CHECK_ERR(ierr, "getEntities"); 

            if (ierr)
            {
              fprintf(fp, "\niMeshP_getEntities Failed: Type %s, Topology %s \n", astrTypes[eEType], astrTopologies[eETopo]);
#ifdef DEBUG
              if (!myrank)
                fprintf(stdout, "\niMeshP_getEntities Failed: Type %s, Topology %s \n", astrTypes[eEType], astrTopologies[eETopo]);
#endif
              bOK_func= false; bOK_valid = false;
              break;
            }

            for (int ii = 0; ii < iEntsSize ; ++ii)
            {
              iMeshP_Part* pPart_ids = NULL;
              int iParStatus;
   
              // Step 2: For every entity, get status of the entity w.r.t the part
              iMeshP_getEntStatus (instance, pPtnHandles[p], pLocalParts[pr],       
                    pEntities[ii], &iParStatus, &ierr); 
              CHECK_ERR(ierr, "iMeshP_getEntStatus");

              /// If entity status is not success then fail first test
              if (ierr != iBase_SUCCESS)
              {
              	bOK_func= false; bOK_valid = false;
            	break;
              }

              iBase_EntityHandle* pEntityHandles = NULL;
              int iEntAllocated = 0, iEntSize = 0;

              /// Step 3: Get downward adjacency, if eEType is not vertex.
              /// If entity type is a vertex then get upward adjacency.
              if (eEType == iBase_VERTEX)
                iMesh_getEntAdj (instance, pEntities[ii], eEType + 1, &pEntityHandles,
      	 	     &iEntAllocated, &iEntSize, &ierr);
              else
                iMesh_getEntAdj (instance, pEntities[ii], eEType - 1, &pEntityHandles,
      		     &iEntAllocated, &iEntSize, &ierr);

              CHECK_ERR(ierr, "iMesh_getEntAdj");
	
              if (ierr != iBase_SUCCESS)
              {
             	bOK_func= false; bOK_valid = false;
            	break;
              }

              /// Step 4: Now check the status of adjacent entities w.r.t the given part.
              for (int adj = 0; adj < iEntSize; ++adj)
              {
                int iAdjStatus;

	        iMeshP_getEntStatus (instance, pPtnHandles[p], pLocalParts[pr],
		      pEntityHandles[adj], &iAdjStatus, &ierr);
      
                /// If an error is thrown to get the status of adjacent entity w.r.t the
                /// part then fail third test. 
              	CHECK_ERR(ierr, "getEntStatus");
        	if (ierr != iBase_SUCCESS)
	        {
                  bOK_func= false; bOK_valid = false;
                  break;
            	}

                /// Also check the iAdjStatus, if it's not internal, ghost or boundary
                /// then fail third test.

                /// Special Case for a vertex
                if (eEType == iBase_VERTEX)
                {
                  /// Case 1: Vertex: Internal Edge: Internal.
                  if (iParStatus == 0 && iAdjStatus != 0)
	          {
	            CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
                    bOK_valid = false;
                  }
                  /// Case 2: Vertex: Boundary Edge: Ghost Allowed? CHECKIT
                  if (iParStatus == 1 && iAdjStatus == 2)
                  {
                    CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
              	    bOK_valid = false;
                  }
                } // if (eEType == iBase_VERTEX)
                else if (eEType == iBase_EDGE)
                {
                  /// Case 3: Edge: Boundary, Vertex: Not Boundary
                  if (iParStatus == 1 && iAdjStatus != 1)
               	  {
                    CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
                    bOK_valid = false;
                  }
 
                  /// Case 4: Edge: Ghost Vertex: Not ghost
                  if (iParStatus == 2 && iAdjStatus != 2)
                  {
                    CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
                    bOK_valid = false;
                  }
                } // else if (eEType == iBase_EDGE)
                else if (eEType == iBase_FACE)
                {
                  /// Case 5: Face: Boundary Edge: Not Boundary
                  if (iParStatus == 1 && iAdjStatus != 1)
                  {
                    CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
             	    bOK_valid = false;
                  }
                } // else if (eEType == iBase_FACE)
                else if (eEType == iBase_REGION)
                {
                  /// Case 6: Region: Ghost Face: Not ghost
                  if (iParStatus == 2 && iAdjStatus != 2)
                  {
                    CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
                    bOK_valid = false;
                  }
                } // else if (eEType == iBase_REGION)
              } // for adj
              UT_FREE(pPart_ids);
              UT_FREE(pEntityHandles);
            } // for ii
            UT_FREE(pEntities);
          } // for pr
          TEST(bOK_func, 0, ""); 
          TEST(bOK_valid, 1, ""); 
        } // if (a2qLegalTypeAndTopo[eEType][eETopo]) 
      } // for eType
    } // for eTopo
    UT_FREE(pLocalParts);
  } // for ptn
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 




/*/////////////////////////////////////////////// 
 Test: getEntStatusArr
    * Pre-req: Get Boundary entities w.r.t the given local part and other parts.
    * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions
    * Test Cases:
      1- Get part boundary entities of a part w.r.t all other parts. Check the entity
         status of the part boundary entities. If status is not 1 then fail.
      2- For every part, get entities common to the part and root set. Get status of
         entities w.r.t the part. For boundary entities, compare them with step 1. If all
         do not match with step 1 then fail. Sizes of both arrays of boundary entities in
         Step1 and 2 should be same. 
/////////////////////////////////////////////// */

int vGetEntStatusArrTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting entity array status : getEntStatusArr------------\n");
#ifdef DEBUG
  if (!myrank)
  fprintf(stdout, "\n \n ------------- Getting entity array status : getEntStatusArr------------\n");
#endif
  /// get root set from the mesh instance, if doesn't succeed we can't proceed so throw
  /// error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

  /// Loop through all partitions
  for (int p = 0; p < pPtnSize; ++p) 
  {   
    ///  Get all local on-process part handles in the partition 
    int pLocalPartSize = 0, pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated, 
          &pLocalPartSize, &ierr);
    CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

    if (pLocalPartSize != INIT_NUM_PARTS)
    {
      CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
      iNFailed++;
      return iBase_FAILURE;
    }

    /// Test 1: First get part boundary entities and check their status
    bool bOK_func= true, bOK_valid = true;

    for (int pr = 0; pr < pLocalPartSize; ++pr)
    { 
      iBase_EntityHandle* pEnts = NULL;
      int iEntAllocated=0, iEntSize;

      /// Step 1: Get part boundary entities between the given part and all other parts.

      iMeshP_getPartBdryEnts (instance, pPtnHandles[p], pLocalParts[pr], static_cast<iBase_EntityType>(iBase_ALL_TYPES), 
            static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), iMeshP_ALL_PARTS, &pEnts, &iEntAllocated, &iEntSize,
            &ierr);

      CHECK_ERR(ierr, "getPartBdryEnts");
	
      /// we can't continue if we don't get the boundary entities so fail.
      if (ierr != iBase_SUCCESS)
      {
	bOK_func= false; bOK_valid = false;
	break;
      }
 
      int* iParStatusArr = NULL;
      int iParStatusSize, iParStatusAlloc=0;

      /// Step 2: Check the status of the array of part boundary entities.

      iMeshP_getEntStatusArr (instance, pPtnHandles[p], pLocalParts[pr], pEnts, iEntSize,
            &iParStatusArr, &iParStatusAlloc, &iParStatusSize, &ierr); 

      CHECK_ERR(ierr, "getEntStatusArr");
  
      if (ierr != iBase_SUCCESS)
      {
        bOK_func= false; bOK_valid = false;
        break;
      }

      /// Step 3: If the output array size of getEntStatusArr is not the same as the
      /// input size of entities then fail. CHECK IT: Why partStatusALloc? Why not
      /// ParStatusSize?
      if (iParStatusAlloc != iEntSize)
      {
        CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
        bOK_func= false;
      }

      /// Step 4: Check status of the entities, if it is not 1 (boundary) then fail.
      for (int ent = 0; ent < iParStatusAlloc; ++ent)
      {
        if (iParStatusArr[ent] != 1)
        {
          CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
          bOK_valid = false;
        }
      } // for ent
  
      /// Test 2: Now get the entities common to the part and set, check if the boundary
      /// entities match for both
      iBase_EntityHandle* pEntities = NULL;
      int iEntityAllocated=0, iEntitySize, iParEntSize, iParEntAlloc;
      int* iParStatusEnt = NULL;
 	
      /// Step 1: Get all entities both in the given part and set
      iMeshP_getEntities (instance, pPtnHandles[p], pLocalParts[pr], root,
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &pEntities, &iEntityAllocated,
            &iEntitySize, &ierr);

      CHECK_ERR(ierr, "getEntities");
	
      if (ierr != iBase_SUCCESS)
      {
	bOK_valid = false; 
	break;
      }
      /// Step 2: Get the array of status for all entities (boundary, internal or ghost)
      iMeshP_getEntStatusArr (instance, pPtnHandles[p], pLocalParts[pr], pEntities, 
            iEntitySize, &iParStatusEnt, &iParEntAlloc, &iParEntSize, &ierr); 

      CHECK_ERR(ierr, "getEntStatusArr");
	
      if (ierr != iBase_SUCCESS)
      {
	bOK_func= false; 
	break;
      }

      /// Step 3: If o/p array size not the same as input then fail.
      /// CHECK IT: Why do we use iPartEntAlloc instead of iParEntSize?
      if (iParEntAlloc != iEntityAllocated)
      {
        CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
        bOK_func= false;
      }

      /// Step 4: Extract boundary entities and compare them with the previous array.
      int iBdryEntSize = 0;

      for (int ent = 0; ent < iParEntAlloc; ++ent)
      {
        if (iParStatusEnt[ent] == 1)
        {
          ++iBdryEntSize;
          bool match = false;
          /// Compare with the previous array of boundary ents
          for (int ent2 = 0; ent2 < iParStatusAlloc; ++ent2)
          {
  	    if (pEnts[ent2] == pEntities[ent])
            {
              match = true;
              break;
	    }
          }
          if (!match)
          {
            CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
            bOK_valid = false;
            break;
          }
        } // if (iParStatusEnt[ent] == 1)
      } // for ent

      if (iBdryEntSize != iEntAllocated)
      {
        CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
        bOK_func= false;
      }
      UT_FREE(pEntities);
      UT_FREE(pEnts);
      UT_FREE(iParStatusArr);
    }// for pr
    TEST(bOK_func, 0, "");
    TEST(bOK_valid, 1, "");
    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}

////////////////////////////////////////
//             GHOSTING               //
////////////////////////////////////////

/*/////////////////////////////////////////////// 
 Test: Adds and removes entity ghosts to part.
    * Pre-req:  Get the boundary entities of the part w.r.t all other parts. 
    * Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions,
                    iMesh_getRootSet, iMeshP_getEntStatusArr, iMeshP_getPartNbors,
                    iMeshP_getCopyOnPart, iMeshP_getNumCopies
    * Test Cases:
      1- a- Get vertices in a part then get all parts having copies of that vertex. 
         b- For every vertex, get the upward adjacent entities.Get the number of copies
            of the adjacent entities in the partition. Check if the copy of the adjacent
            entity exists on the copy parts of the vertices. If not, add ghost there.
         c- Count the ghosts being added on other parts.
         d- Now for each adjacent entity, get the number of copies in the partition
            again. Check if it has increased by the number of ghosts added. If not, we
            fail the validation test.
      2- Now remove the ghosts of adjacent entities from the parts. 
/////////////////////////////////////////////// */

int vAddRmvGhostTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// Get all the partitions
  char *cFuncName = "iMeshP_addGhostOf, iMeshP_rmvGhostOf";
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Adding/Removing ghost to a part : addGhost, rmvGhost------------\n");
#ifdef DEBUG
  if (!myrank)
    fprintf(stdout, "\n \n ------------- Adding/Removing ghost to a part : addGhost, rmvGhost------------\n");
#endif
  /// get root set from the mesh instance, if doesn't succeed we can't proceed so throw
  /// error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

  /// Loop through all partitions
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    int pLocalPartSize = 0, pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
    CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

    bool bOK_func= true, bOK_valid = true, bOK_valid2 = true;

    for (int pr = 0; pr < pLocalPartSize; ++pr)
    {     
      int iEntityAllocated=0, iEntitySize;
      iBase_EntityHandle* pEntities = NULL;

       /// Step 1: Get all entities of type vertex & point on the part boundary.
      iMeshP_getPartBdryEnts(instance, pPtnHandles[p], pLocalParts[pr], iBase_VERTEX, 
            iMesh_POINT, iMeshP_ALL_PARTS, &pEntities, &iEntityAllocated,
            &iEntitySize, &ierr); 

      /// if failure occurs, we cannot continue the test so exit. 
      CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");
     
      if (ierr!=iBase_SUCCESS)
      {
        bOK_func= false; bOK_valid = false;bOK_valid2 = false;
        break;
      }
      for (int ent = 0; ent < iEntitySize; ++ent)
      {
        /// Step 2: Get the parts having copy of the given vertex
        iMeshP_Part* pPartIds = NULL;
        int iCopyPartsSize, iCopyPartsAllocated=0;

        iMeshP_getCopyParts(instance, pPtnHandles[p], pEntities[ent], &pPartIds, 
              &iCopyPartsAllocated, &iCopyPartsSize, &ierr); 

        CHECK_ERR(ierr, "iMeshP_getCopyParts");

        if (ierr!=iBase_SUCCESS)
        {
          bOK_func= false; bOK_valid = false;bOK_valid2 = false;
          break;
        }

        /// Step 3: Get all entities adjacent to the part boundary vertices. 
        for (eEType = iBase_EDGE; eEType <= iBase_ALL_TYPES; ++eEType)
        { 
          /// Now get adjacencies of the vertex
          iBase_EntityHandle* pAdjEnts = NULL;
          int iAdjAllocated=0, iAdjSize;
 
          iMesh_getEntAdj (instance, pEntities[ent], eEType, &pAdjEnts, &iAdjAllocated,
               &iAdjSize, &ierr);

          CHECK_ERR(ierr, "iMeshP_getEntAdj");

          if (ierr!=iBase_SUCCESS)
          {
            bOK_func= false; bOK_valid = false;bOK_valid2 = false;
            break;
          }

          for (int adj = 0; adj < iAdjSize; ++adj)
          {
            /// Step 4: First get total number of copies of entities before adding ghost.
            int iNumCopiesEnt;

            /// Set for containing the parts on which entities have been ghosted
            std::set<iMeshP_Part> copyPartsSet;

            iMeshP_getNumCopies(instance, pPtnHandles[p], pAdjEnts[adj], &iNumCopiesEnt,
                  &ierr); 

            CHECK_ERR(ierr, "iMeshP_getNumCopies");
     
            if (ierr!=iBase_SUCCESS)
            {
              bOK_valid = false;
              break;
            }

            /// Make ghost copies of entities owned by local part only.
            int iIsOwner;
            iMeshP_isEntOwner (instance, pPtnHandles[p], pLocalParts[pr], pAdjEnts[adj],
                  &iIsOwner, &ierr);

            CHECK_ERR(ierr, "iMeshP_isEntOwner");
            if (ierr != iBase_SUCCESS)
              continue;

            if (iIsOwner == 1)
            {
              /// Get all part ids having copies of the entity
              iMeshP_Part* pCopyIds = NULL;
              int iPartIdsAlloc=0, iPartIdsSize;

              iMeshP_getCopyParts (instance, pPtnHandles[p], pAdjEnts[adj], &pCopyIds, 
                    &iPartIdsAlloc, &iPartIdsSize, &ierr); 

              int iCopies = 0;

              /// Now find the part ids having copies of vertices but no copy of adjacent
              /// entities. Place such part ids in a set
              for (int cp = 0; cp < iCopyPartsSize; ++cp)
              {
                bool bNotThere = true;
                for (int acp = 0; acp < iPartIdsSize; ++acp)
          	{
            	  if (pCopyIds[acp] == pPartIds[cp])
          	  {
	            bNotThere = false;
                    break;
          	  }
	        } // for acp
                if (bNotThere)
                {
                  copyPartsSet.insert(pPartIds[cp]); 
                }
              } // for cp

              /// Step 5: Now we can ghost the entity on the part ids in the set.
              std::set<iMeshP_Part>::iterator it; 

              for (it = copyPartsSet.begin(); it!= copyPartsSet.end(); ++it) 
              {   
                /// Now add the ghost.
                iMeshP_RequestHandle rq;
                iMeshP_addGhostOf (instance, pPtnHandles[p], (iMeshP_Part)(*it),
                      pAdjEnts[adj], &rq, &ierr);

                CHECK_ERR(ierr, "iMeshP_addGhostOf");
                if (ierr != iBase_SUCCESS)
                {
                  bOK_func= false; bOK_valid = false; bOK_valid2 = false;
                  break;
                }
  
                /// Wait for the ghost to be added
                iMeshP_waitForRequest(instance, pPtnHandles[p], rq, &ierr);
                CHECK_ERR(ierr, "iMeshP_waitForRequest");
                if (ierr != iBase_SUCCESS)
                {
                  bOK_func= false; bOK_valid = false; bOK_valid2 = false;
                  break;
                }
                ++iCopies;
              } // for it

              /// If number of copies has increased check iCopies
              if (iCopies > 0)
              {
                int iNumCopiesEntAftr;
  
                iMeshP_getNumCopies (instance, pPtnHandles[p], pAdjEnts[adj], 
                      &iNumCopiesEntAftr, &ierr); 
 
                CHECK_ERR(ierr, "iMeshP_getNumCopies");
                if (ierr != iBase_SUCCESS)
                {
                  bOK_valid = false;
              	  break;
                }

                if (iNumCopiesEntAftr != (iNumCopiesEnt + iCopies))
                {      
                  CHECK_VALIDATION_ERR("iMeshP_AddGhostOf, iMeshP_rmvGhostOf");
                  bOK_valid = false;
                  break;
                }
                /// Step 6: Once verified that the ghosts have been added, we need to
                /// remove them.
                std::set<iMeshP_Part>::iterator it; 
  
                for (it = copyPartsSet.begin(); it!=copyPartsSet.end(); ++it) 
                {
                  iMeshP_rmvGhostOf (instance, pPtnHandles[p], (iMeshP_Part)(*it),
                        pAdjEnts[adj], &ierr);

                  CHECK_ERR(ierr, "iMeshP_rmvGhostOf");

                  if (ierr != iBase_SUCCESS)
                  {
                    bOK_valid2 = false;
                    break;
                  } 
                } // for it
                int iNumCopiesAftrRmv;

                iMeshP_getNumCopies(instance, pPtnHandles[p], pAdjEnts[adj], 
                      &iNumCopiesAftrRmv, &ierr);

                if (iNumCopiesAftrRmv != iNumCopiesEnt)
                {
                  CHECK_VALIDATION_ERR("iMeshP_AddGhostOf, iMeshP_rmvGhostOf");
            	  bOK_valid = false;
            	  break;
                }
              } // if (iCopies > 0)
            } //  if (iIsOwner == 1)
          } // for adj 
          UT_FREE(pAdjEnts);
        } // for eEType 
        UT_FREE(pPartIds);
      } // for ent
      UT_FREE(pEntities);
    } // for pr
    TEST(bOK_func,_FUNC_, cFuncName); 
    TEST(bOK_valid, _VALID_, cFuncName); 
    TEST(bOK_valid2,_VALID_, cFuncName);
    UT_FREE(pLocalParts);
  } // for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 


/*/////////////////////////////////////////////// 
 Test: iMeshP_deleteGhostEntsAll
  * Pre-req: Get the entities w.r.t a given part A and set using getEntities 
  * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, 
		  iMesh_getRootSet, iMeshP_getEntStatusArr
  * Test Cases: 
    1- Remove all ghosts in the partitions, then check if any entity of status 'ghost' exists.
/////////////////////////////////////////////// */

int vDeleteGhostEntsAllTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Removing ghost between two part : deleteGhostEntsAll------------\n");
#ifdef DEBUG
  if (!myrank)
  fprintf(stdout, "\n \n ------------- Removing ghost between two part : deleteGhostEntsAll------------\n");
#endif

  /// get root set from the mesh instance, if doesn't succeed we can't proceed so throw
  /// error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

  /// Loop through all partitions
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    ///  Get all local part handles in the partition 
    int pLocalPartSize = 0, pLocalPartAllocated = 0;
    iMeshP_PartHandle* pLocalParts = NULL;
    iMeshP_getLocalParts (instance, pPtnHandles[p], &pLocalParts, &pLocalPartAllocated,
          &pLocalPartSize, &ierr);
    CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

    bool bOK_func= true, bOK_valid = true;

    /// Step 1: Delete all ghost entities between parts 
    iMeshP_deleteGhostEntsAll(instance, pPtnHandles[p], &ierr);

    CHECK_ERR(ierr, "iMeshP_deleteGhostEntsAll");
    if (ierr != iBase_SUCCESS)
    {
      bOK_func= false; bOK_valid = false;
      break;
    }
    /// Step 2: Get all entities for all parts and check if any of them is of ghost type.
    int iEntityAllocated=0, iEntitySize;
    iBase_EntityHandle* pEntities = NULL;

    /// Get all entities common to the given part and set
    for (int pr = 0; pr < pLocalPartSize; ++pr)
    { 
      iMeshP_getEntities (instance, pPtnHandles[p], pLocalParts[pr], root,
            static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &pEntities, &iEntityAllocated,
            &iEntitySize, &ierr);

      CHECK_ERR(ierr, "iMeshP_getEntities");
        
      if (ierr != iBase_SUCCESS)
      {
	bOK_valid = false;
	break;
      }

      /// Get the status of the above entities
      int* iParStatus = NULL;
      int iParAlloc=0, iParSize;

      iMeshP_getEntStatusArr(instance, pPtnHandles[p], pLocalParts[pr], pEntities,
            iEntitySize, &iParStatus, &iParAlloc, &iParSize, &ierr); 

      CHECK_ERR(ierr, "iMeshP_getEntStatusArr");

      if (ierr != iBase_SUCCESS)
      {
	bOK_valid = false;
	break;
      }

      /// Step 3: Loop through all entities and check their status. 
      /// If status is ghost then deleteGhostAll didn't work so fail.
      for (int ii = 0; ii < iParAlloc; ++ii)
      {
        if (iParStatus[ii] == iMeshP_GHOST)
        {
          CHECK_VALIDATION_ERR("iMeshP_deleteGhostEntsAll");
          bOK_valid = false;
          break;
        }
      } // for ii
      UT_FREE(iParStatus);
    } // for pr
    TEST(bOK_func, 0, "");
    TEST(bOK_valid, 1, "");
    UT_FREE(pEntities);
    UT_FREE(pLocalParts);
  } /// for p
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: create ghosts in the partition.
  * Pre-req: get the entities w.r.t a given part A and set. 
  * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getRootSet, 
		  iMeshP_deleteGhostEntsAll, iMeshP_isEntOwner, iMeshP_getEntStatusArr, 
 		  iMeshP_getPartNbors, iMeshP_getPartHandleFromPartId
  * Test Cases: 
    1- Remove all ghosts in the partition. 
/////////////////////////////////////////////// */

int vCreateDeleteGhostEntsAllTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************
  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

/*
  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated = 0, pLocalPartSize = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);
*/
  int iGeomDim, iNumLayers=1;
  int pGhostRulesAllocated=0, pGhostSize=0;

  int* pGhostTypeArr = NULL;
  int* pBridgeTypeArr = NULL;
  int* pNumLayerArr = NULL;
  int pGhostRuleArrAllocated=0, pGhostRuleArrSize=0;

  iMesh_getGeometricDimension(instance, &iGeomDim, &ierr);
  iMeshP_createGhostEntsAll (instance, pPtnHandles[0], iGeomDim, iGeomDim-1, iNumLayers,
              0, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_createGhostEntsAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  else if (!ierr)
  {
    iMeshP_ghostEntInfo(instance, pPtnHandles[0], &pGhostRuleArrAllocated, &pGhostRuleArrSize, 
          &pGhostTypeArr, &pBridgeTypeArr, &pNumLayerArr, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_ghostEntInfo");
    if (ierr) bOK_func=false;

    iMeshP_deleteGhostEntsAll(instance, pPtnHandles[0], &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_deleteGhostEntsAll");
    if (ierr) bOK_func=false;
  }
  UT_FREE(pGhostTypeArr);
  UT_FREE(pBridgeTypeArr);
  UT_FREE(pNumLayerArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// Get all the partitions
  bool bOK_func1=true, bOK_func2=true, bOK_valid1=true, bOK_valid2=true, bOK_valid3=true;
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    // Check all possible combinations of boundary entities.
    for (eEType = iBase_EDGE; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (int iBridge = iBase_VERTEX; iBridge < eEType; ++iBridge) 
      {  
        for (iNumLayers=1; iNumLayers<=MAX_NUM_LAYERS; ++iNumLayers)
        {
          iMeshP_createGhostEntsAll (instance, pPtnHandles[p], eEType, iBridge, iNumLayers,
              0/*include_copy*/, &ierr);
          
          if (ierr) bOK_func=false;
        }
        sprintf(msg,"iMeshP_createGhostEntsAll (ghost=%s, bridge=%s, numLayer=1..%d, include_copy=%d",
                    astrTypes[eEType], astrTypes[iBridge], MAX_NUM_LAYERS, 0); 
        TEST(bOK_func, _FUNC_, msg);
        if (!ierr)
        {
          // test ghost rule info
          int* pGhostTypeArr=NULL;
          int* pBridgeTypeArr=NULL;
          int* pNumLayerArr=NULL;
          pGhostRuleArrAllocated=0; pGhostRuleArrSize=0;
          iMeshP_ghostEntInfo(instance, pPtnHandles[p], &pGhostRuleArrAllocated, &pGhostRuleArrSize,
                 &pGhostTypeArr, &pBridgeTypeArr, &pNumLayerArr, &ierr);
          sprintf(msg,"iMeshP_ghostEntInfo (ghost=%s, bridge=%s)", astrTypes[eEType], astrTypes[iBridge]);
          TEST(!ierr, _FUNC_, msg);
          if (!ierr)
          {
            TEST(pGhostRuleArrAllocated==MAX_NUM_LAYERS, _VALID_, "pGhostRuleArrAllocated==MAX_NUM_LAYERS");
            TEST(pGhostRuleArrAllocated==pGhostRuleArrSize, _VALID_, "pGhostRuleArrAllocated==pGhostRuleArrSize");
            bOK_valid1=true; bOK_valid2=true; bOK_valid3=true;
            for (int i=0; i<MAX_NUM_LAYERS; ++i)
            {
              if (pGhostTypeArr[i]!=eEType) bOK_valid1=false;
              if (pBridgeTypeArr[i]!=iBridge) bOK_valid2=false;
              if (pNumLayerArr[i]!=i+1) bOK_valid3=false;
            }
            TEST(bOK_valid1, _VALID_,  "checking pGhostTypeArr in iMeshP_ghostEntInfo");
            TEST(bOK_valid2, _VALID_, "checking pBridgeTypeArr in iMeshP_ghostEntInfo");
            TEST(bOK_valid3, _VALID_, "checking pNumLayerArr in iMeshP_ghostEntInfo"); 
          } // if success with iMeshP_ghostEntInfo
          UT_FREE(pGhostTypeArr);
          UT_FREE(pBridgeTypeArr);
          UT_FREE(pNumLayerArr);

          iMeshP_deleteGhostEntsAll(instance, pPtnHandles[p], &ierr);
          sprintf(msg,"iMeshP_iMeshP_createGhostEntsAll(%s)", astrTypes[eEType]);
          TEST(!ierr, _FUNC_, msg);
        } // if success with iMeshP_createGhostEntsAll
      } // for iBridge
    } // for eEType
  } // for each partition
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

/*/////////////////////////////////////////////// 
 Test: getGhostInfo
  * Pre-req: Get the entities w.r.t a given part A and set. 
  * Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getRootSet,
		  iMeshP_deleteGhostEntsAll, iMeshP_createGhostEntsAll
  * Test Cases: 
    1- For every partition, 
       a- Delete all ghosting rules. 
       b- For each entity type, bridge, include-copy combination, create ghost entities and check 
          if the getGhostInfo function returns the newly created ghosting entity type, bridge, include copy information. 
       c-If for any combination it does not return correct information then fail. 
/////////////////////////////////////////////// */

int vGetGhostInfoTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartAllocated = 0, pLocalPartSize = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  int iGeomDim, iNumLayers=1;
  int pGhostRulesAllocated=0, pGhostSize=0;

  int* pGhostTypeArr = NULL;
  int* pBridgeTypeArr = NULL;
  int* pNumLayerArr = NULL;
  int pGhostRuleArrAllocated=0, pGhostRuleArrSize=0;

  iMesh_getGeometricDimension(instance, &iGeomDim, &ierr);
  iMeshP_createGhostEntsAll (instance, pPtnHandles[0], iGeomDim, iGeomDim-1, iNumLayers,
              0, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_createGhostEntsAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  else if (!ierr)
  {
    iMeshP_ghostEntInfo(instance, pPtnHandles[0], &pGhostRuleArrAllocated, &pGhostRuleArrSize, 
          &pGhostTypeArr, &pBridgeTypeArr, &pNumLayerArr, &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_ghostEntInfo");
    if (ierr) bOK_func=false;

    iMeshP_deleteGhostEntsAll(instance, pPtnHandles[0], &ierr);
    TEST(!ierr, _FUNC_, "iMeshP_deleteGhostEntsAll");
    if (ierr) bOK_func=false;
  }

  iBase_EntityHandle* pPartBdryEntArr = NULL;
  int pPartBdryEntArrAllocated=0, pPartBdryEntArrSize=0;

  iMeshP_getPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
                   static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), iMeshP_ALL_PARTS, &pPartBdryEntArr,
                  &pPartBdryEntArrAllocated, &pPartBdryEntArrSize, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");
  TEST(pPartBdryEntArrSize, _VALID_, "no part boundary entity in iMeshP_getPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED||!pPartBdryEntArrSize) bOK_func=false;

  int iNumPartBdryEnt = 0;
  iMeshP_getNumPartBdryEnts (instance, pPtnHandles[0], pLocalParts[0],
         static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), iMeshP_ALL_PARTS,
                  &iNumPartBdryEnt, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getNumPartBdryEnts");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  int* pEntStatusArr=NULL;
  int pEntStatusArrAllocated=0, pEntStatusArrSize=0;
  if (pPartBdryEntArrSize) 
  {
    iMeshP_getEntStatusArr (instance, pPtnHandles[0], pLocalParts[0], pPartBdryEntArr, pPartBdryEntArrSize, 
           &pEntStatusArr, &pEntStatusArrAllocated, &pPartBdryEntArrSize, &ierr);
    TEST(!ierr, _FUNC_, "MeshP_getEntStatusArr");
    if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;
  }

  UT_FREE(pLocalParts);
  UT_FREE(pPartBdryEntArr);
  UT_FREE(pEntStatusArr);
  UT_FREE(pGhostTypeArr);
  UT_FREE(pBridgeTypeArr);
  UT_FREE(pNumLayerArr);
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }

  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  /// Get all the partitions
  bool bOK_func1=true, bOK_func2=true, bOK_valid1=true, bOK_valid2=true, bOK_valid3=true;
  for (int p = 0; p < pPtnSize; ++p) 
  { 
    // Check all possible combinations of boundary entities.
    for (eEType = iBase_EDGE; eEType <= iBase_ALL_TYPES; ++eEType) 
    {
      for (int iBridge = iBase_VERTEX; iBridge < eEType; ++iBridge) 
      {  
        for (iNumLayers=1; iNumLayers<=MAX_NUM_LAYERS; ++iNumLayers)
        {
          iMeshP_createGhostEntsAll (instance, pPtnHandles[p], eEType, iBridge, iNumLayers,
              0/*include_copy*/, &ierr);
          
          if (ierr) bOK_func=false;
        }
        sprintf(msg,"iMeshP_createGhostEntsAll (ghost=%s, bridge=%s, numLayer=1..%d, include_copy=%d",
                    astrTypes[eEType], astrTypes[iBridge], MAX_NUM_LAYERS, 0); 
        TEST(bOK_func, _FUNC_, msg);
        if (!ierr)
        {
          // test ghost rule info
          pGhostTypeArr=NULL;
          pBridgeTypeArr=NULL;
          pNumLayerArr=NULL;
          pGhostRuleArrAllocated=0; pGhostRuleArrSize=0;
          iMeshP_ghostEntInfo(instance, pPtnHandles[p], &pGhostRuleArrAllocated, &pGhostRuleArrSize,
                 &pGhostTypeArr, &pBridgeTypeArr, &pNumLayerArr, &ierr);
          sprintf(msg,"iMeshP_ghostEntInfo (ghost=%s, bridge=%s)", astrTypes[eEType], astrTypes[iBridge]);
          TEST(!ierr, _FUNC_, msg);
          if (!ierr)
          {
            TEST(pGhostRuleArrAllocated==MAX_NUM_LAYERS, _VALID_, "pGhostRuleArrAllocated==MAX_NUM_LAYERS");
            TEST(pGhostRuleArrAllocated==pGhostRuleArrSize, _VALID_, "pGhostRuleArrAllocated==pGhostRuleArrSize");
            bOK_valid1=true; bOK_valid2=true; bOK_valid3=true;
            for (int i=0; i<MAX_NUM_LAYERS; ++i)
            {
              if (pGhostTypeArr[i]!=eEType) bOK_valid1=false;
              if (pBridgeTypeArr[i]!=iBridge) bOK_valid2=false;
              if (pNumLayerArr[i]!=i+1) bOK_valid3=false;
            }
            TEST(bOK_valid1, _VALID_,  "checking pGhostTypeArr in iMeshP_ghostEntInfo");
            TEST(bOK_valid2, _VALID_, "checking pBridgeTypeArr in iMeshP_ghostEntInfo");
            TEST(bOK_valid3, _VALID_, "checking pNumLayerArr in iMeshP_ghostEntInfo"); 
          } // if success with iMeshP_ghostEntInfo

          iMeshP_deleteGhostEntsAll(instance, pPtnHandles[p], &ierr);
          sprintf(msg,"iMeshP_iMeshP_createGhostEntsAll(%s)", astrTypes[eEType]);
          TEST(!ierr, _FUNC_, msg);
        } // if success with iMeshP_createGhostEntsAll
      } // for iBridge
    } // for eEType
  } // for each partition
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
} 

/*/////////////////////////////////////////////// 
 Test: vExchangeEntArrTest
  * Pre-req: Get the entities w.r.t a given part A and set. 
  * Dependencies: iMeshP_getPartitions, iMeshP_getLocalParts, iMeshP_getEntities
                  iMeshP_exchEntArrToPartsAll
  * Test Cases: 
    1- 
/////////////////////////////////////////////// */
int vExchangeEntArrTest(iMesh_Instance instance)
{
  /// *****************************************
  /// 		DEPENDENCY CHECK
  /// *****************************************

  bool bOK_func=true;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles=NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated,
                         &pLocalPartSize, &ierr);

  int iGeomDim;
  iMesh_getGeometricDimension(instance, &iGeomDim, &ierr);

  iBase_EntityHandle* pEntArr = NULL;
  int pEntArrAllocated=0, pEntArrSize=0;

  iMeshP_getEntities (instance, pPtnHandles[0], pLocalParts[0], root,
            static_cast<iBase_EntityType>(iGeomDim), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),  &pEntArr, &pEntArrAllocated, &pEntArrSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  iMeshP_Part target_part_ids[pEntArrSize];
  for (int i=0; i<pEntArrSize; ++i)
    target_part_ids[i] = (myrank+1)%mysize;
  iMeshP_RequestHandle rq;
  iMeshP_exchEntArrToPartsAll(instance, pPtnHandles[0], pEntArr, pEntArrSize, 
         target_part_ids, 0, 0, &rq, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_exchEntArrToPartsAll");
  if (ierr==iBase_NOT_SUPPORTED) bOK_func=false;

  UT_FREE(pLocalParts); UT_FREE(pEntArr); 
  if (!bOK_func)
  {
    UT_FREE(pPtnHandles);
    return iBase_FAILURE;
  }
  
  /// *****************************************
  /// 		VALIDITY CHECK
  /// *****************************************
  UT_FREE(pPtnHandles);
  return iBase_SUCCESS;
}


/*/////////////////////////////////////////////// 
 Test: vExchangeEntArrTest
  * Pre-req: Get the entities w.r.t a given part A and set. 
  * Dependencies: iMeshP_getPartitions, iMeshP_getLocalParts, iMeshP_getEntities, iMeshP_getPartNbors
                  iMeshP_getPartBdryEnts, iMeshP_migrateEntity, iMeshP_waitForRequestEnt
  * Test Cases: 
    1- 
/////////////////////////////////////////////// */
int vMigrateEntTest(iMesh_Instance instance)
{
  bool bOK_func = true, bOK_valid1 = true, bOK_valid2 = true, isFound;
  int ierr, numEnts = 0, entId = 0, numNbrs = 0, numNbrsAlloc = 0, numNbrsSize = 0, partId = 0, entsAlloc = 0, entStatus = 0;
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  iMeshP_PartitionHandle* pPtnHandles=NULL;
  int pPtnSize = 0, pPtnAllocated = 0;
  iMeshP_getPartitions(instance, &pPtnHandles, &pPtnAllocated, &pPtnSize, &ierr);

  iMeshP_PartHandle* pLocalParts = NULL;
  int pLocalPartSize = 0, pLocalPartAllocated = 0;
  iMeshP_getLocalParts (instance, pPtnHandles[0], &pLocalParts, &pLocalPartAllocated, &pLocalPartSize, &ierr);

  // choose a boundary part
  iMeshP_Part* nbrParts=NULL;
  iMeshP_getPartNbors(instance, pPtnHandles[0], pLocalParts[0], iBase_ALL_TYPES, &numNbrs, &nbrParts, &numNbrsAlloc, &numNbrsSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getPartNbors");

  partId = rand() % numNbrs;

  // choose an entity on part boundary to migrate
  iBase_EntityHandle *bdryEnts=NULL;
  iMeshP_getPartBdryEnts(instance, pPtnHandles[0], pLocalParts[0], iBase_ALL_TYPES, iMesh_ALL_TOPOLOGIES, (int) nbrParts[partId], &bdryEnts, &entsAlloc, &numEnts, &ierr); 
  TEST(!ierr, _FUNC_, "iMeshP_getPartBdryEnts");

  entId = rand() % numEnts;

  // migrate
  iMeshP_RequestHandle rq;
  iMeshP_migrateEntity(instance, pPtnHandles[0], pLocalParts[0], bdryEnts[entId], &rq, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_migrateEntity");
  
  if (ierr != iBase_SUCCESS) 
  {
    UT_FREE(pLocalParts); 
    UT_FREE(pPtnHandles); 
    UT_FREE(nbrParts);
    UT_FREE(bdryEnts);
    return iBase_FAILURE;
  }

  // wait for ent
  iBase_EntityHandle *newEnts=NULL;
  int entSizeAlloc=0, entSize=0;
  iMeshP_waitForRequestEnt(instance, pPtnHandles[0], rq, &newEnts, &entSizeAlloc, &entSize, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_waitForRequestEnt");

  // check if local entity is a internal ent
  iMeshP_getEntStatus(instance, pPtnHandles[0], pLocalParts[0], bdryEnts[entId], &entStatus, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntStatus");

  if (entStatus != iMeshP_INTERNAL)
    bOK_valid1 = false;

  TEST(bOK_valid1, _VALID_,  "checking local_entity status for iMeshP_migrateEntity");

  // check ents are on the part
  int numAllEnts = 0, numEntsAlloc = 0;
  iBase_EntityHandle *entities = NULL;

  iMeshP_getEntities(instance, pPtnHandles[0], pLocalParts[0], root, static_cast<iBase_EntityType>(iBase_ALL_TYPES), static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES), &entities, &numEntsAlloc, &numAllEnts, &ierr);
  TEST(!ierr, _FUNC_, "iMeshP_getEntities");

  for (int i = 0; i < entSize; i++)
  {
    isFound = false;
    for (int j = 0; j < numAllEnts; j++)
    {
      if (newEnts[i] == entities[j])
      {
        isFound = true;
        break;
      }
    }
    if (isFound == false)
    {
      bOK_valid2 = false;
      break;
    }
  }
  
  TEST(bOK_valid2, _VALID_,  "checking newEnts for iMeshP_migrateEntity");

  UT_FREE(nbrParts);
  UT_FREE(bdryEnts);
  UT_FREE(newEnts);
  UT_FREE(entities);
  UT_FREE(pLocalParts);
  UT_FREE(pPtnHandles);

  return iBase_SUCCESS;
}
