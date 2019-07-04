/*
iMeshP Compliance test source code.
Each iMeshP specification function is tested in two ways

1- Basic Functionality tests: This test checks if the iMeshP function is successful or not. If the function is not successful, it displays the iBase error code. 

2- Validation tests: This checks if the function results are accurate by cross checking them with combinations of other iMeshP/iMesh functions.

For all tests, the output message displays which of the two tests does the function pass/fail.
*/


#include "iMeshP_unitTest.h"

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  // Create a file for each processor's output so that the output doesn't
  // get jumbled on the screen.

  char filename[23];
  char* cMeshFile;

  sprintf(filename, "Output_%02d.txt", me);
  fp = fopen(filename, "w");

  /// the mesh file to be loaded 
  cMeshFile = argv[1];

  /// If the file handle is invalid, exit.
  if( fp == NULL )
  {
    cout << "Error opening file!" << endl;
    exit(1);
  }

    /// Create a new mesh using mesh instance and partition handle.
  iMesh_Instance instance;
  iMeshP_PartitionHandle partition;

  iMesh_newMesh(options, &instance, &ierr, options_len);
    
  /// If the mesh is not created successfully, we can't continue so exit.
     
  CHECK_ERR_CRITICAL_MAIN(ierr, "iMesh_newMesh");

   /// Set up legal type-topology combination, ref: iMesh unit test.
  vSetupTypeAndTopo();
  {
    //fprintf(fp, "*****************************");
    //fprintf(fp, "\n 1-Testing IMESHP File IO");
    //fprintf(fp, "\n*****************************");

    /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
    iBase_EntitySetHandle root;
    iMesh_getRootSet(instance, &root, &ierr);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMesh_getRootSet");

    /// create a partition --- mesh instance contains the partition
    /// if does'nt succeed we can't proceed so throw error and exit
  
    iMeshP_createPartitionAll(instance, MPI_COMM_WORLD, &partition, &ierr);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_createPartitionAll");

    /// Now load the mesh into the partition, if load does'nt succeed throw an error and exit.   
    iMeshP_loadAll(instance, partition, root, cMeshFile, options, &ierr, 
                 strlen(cMeshFile), options_len);
    CHECK_ERR_CRITICAL_MAIN(ierr, "iMeshP_loadAll");
 
    /// All steps have passed upto this point
    bAllPassed = true;
  }
  
  /**  \brief Testing stuff related to part functionality. 
   
    Functions include createPart, destroyPart, getPartIdFromPartHandle,getPartIdsFromPartHandlesArr, getPartHandleFromPartId and getPartHandlesFromPartsIdsArr */
  
  {
    fprintf(fp, "*******************************************");
    fprintf(fp, "\n 1- Testing IMESHP PART FUNCTIONALITY");
    fprintf(fp, "\n*****************************************");

    // test: create empty parts in partitions
    /// works but can't keep the created parts because we have SPPP.
    vCreateDeletePartTest(instance);

    /// test: get a given part id, get its part handle.
    vGetPrtIdFromPrtHandleArrTest(instance); 
   
    /// test: get part id array from part handle array.
    vGetPrtIdFromPrtHandleTest( instance); 

    /// test: get array of part handles from array of part ids
    vGetPrtHandleArrFromPrtIdsTest(instance); 
  
    /// test: get the part handle given a part id.
    // vGetPrtHandleFromPrtIdTest(instance); 
  }
  
  {
    fprintf(fp, "************************************************");
    fprintf(fp, "\n 2- Testing IMESHP PART BOUNDARY FUNCTIONALITY");
    fprintf(fp, "\n************************************************");
    
    /// test: get the boundary entities among parts
    vGetPrtBdryEntsTest(instance); 

    /// test: get part neighbors
    vGetPrtNborsTest(instance); 
   
    /// test: get part neighbors array
    vGetPrtNborsArrTest(instance); 
  
    /// test: get number of part neighbors
    vGetNumPrtNborsTest(instance); 
  
    /// test: get array of number of part neighbors
    vGetNumPrtNborsArrTest(instance);   
 
    /// test: get entity boundary iterator functionality
    vBdryEntsIteratorTest(instance); 

    /// test: get entity boundary array iterator functionality
    vBdryEntsArrIteratorTest(instance); 
  } 

  {
    fprintf(fp, "\n******************************************************");
    fprintf(fp, "\n 3- Testing IMESHP ENTITY OWNERSHIP FUNCTIONALITY");
    fprintf(fp, "\n****************************************************");
    
    /// test: get owner part of an entity
    vGetEntOwnerPrtTest(instance); 
   
    /// test: get owner parts of array of entities
    vGetEntOwnerPrtArrTest(instance); 
 
    /// test: check for ownership of an array of entities w.r.t part.
    vIsEntOwnerArrTest(instance); 

    /// test: check for ownership of an entity w.r.t. a part.
    vIsEntOwnerTest(instance); 
    
    /// test: get number of copies of entities on a partition.
    vGetNumCopies(instance); 

    /// test: get part ids having copies of entities.
    vGetCopyParts(instance);

    /// test: get entity handle of copy of a given entity on part.
    vGetCopyOnPartTest(instance);
 
    /// test: get entity handle of a copy of an entity in it's owner part.
    vGetOwnerCopyTest(instance);

    /// test: get entity status w.r.t a given part.
    vGetEntStatus(instance);

    /// test: get part ids having entity copies and handles of the remote copies.
    vGetEntCopies(instance);

    /// test: getting entity status of an array of entities
    vGetEntStatusArr(instance);
  }

  {
    fprintf(fp, "*********************************************");
    fprintf(fp, "\n 4- Testing IMESHP GHOSTING FUNCTIONALITY");
    fprintf(fp, "\n*******************************************");
     
    /// test: adding ghost entities to a part
    //vAddRmvGhostTest(instance);

    /// test: remove all ghost entities between parts
    vDeleteGhostEntsAll(instance);

    /// test: create all ghost entities between parts (implementation on-going)
    //vCreateGhostEntsAll(instance);

    /// test: get all ghost rules and information in the partition
    vGetGhostInfo(instance);
  }

  /// Print the total number of tests passed or failed
  fprintf(fp, "\n FINAL RESULTS: \t %d tests passed \t %d tests failed \t Total tests %d \t",   
          iNPassed, iNFailed, iNPassed+iNFailed);

  /// To avoid floating point exception in case all tests fail.
  if(iNPassed > 0)
    fprintf(fp, " pass rate of: %d \n", (int)100*iNPassed/(iNPassed+iNFailed));

  fclose(fp);

  int myrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  if (myrank==0) printf("\nPlease See Output_ProcId.txt for Test Result of Each Process\n\n");
  
  MPI_Finalize();
  return 1;
}

/*/////////////////////////////////////////////// 
          Type and Topology Setup 
/////////////////////////////////////////////////*/
static void vSetupTypeAndTopo()
{
  // Which combinations are okay doesn't depend on whether the mesh
  // database is 2D, 3D, etc.  Meshes are required to returned nothing
  // if they don't have something, that's all.

  int eEType;
  int eETopo;

  //@@ Setup data
  for (eETopo = iMesh_POINT;
       eETopo <= iMesh_ALL_TOPOLOGIES;
       eETopo++) {
    for (eEType = iBase_VERTEX;
	 eEType < iBase_ALL_TYPES;
	 eEType++) {
      a2qLegalTypeAndTopo[eEType][eETopo] = false;
    }
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

/*/////////////////////////////////////////////// 
 PART FUNCTIONALITY 
 /* Tests iMeshP_createPart and iMeshP_destroyPart.
  * Dependencies: iMeshP_getNumLocalParts,iMeshP_getPartIdFromPartHandle, iMeshP_getPartitions.
  * Test Cases: 
    1- Should create fixed number of parts in the given partition
    2- Get the new part's handle and part id
    3- Check if the on-process parts have incremented.
    4- Delete created parts.
    5- Check if the on-process parts have decremented.
/////////////////////////////////////////////// */
void vCreateDeletePartTest(iMesh_Instance instance)
{
  //  char *cFuncName = "iMeshP_createPart, iMeshP_destroyPart";
  /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles;
  int ptnSize = 0, ptnAllocated = 0;

  iMeshP_getPartitions(instance, &pPtnHandles, &ptnAllocated, &ptnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  int iNumLocalParts = 0, iNumPartsAftrCreate = 0;

  int iPrtSize = 0, iPrtAllocated = 0;
  
  /// For keeping track of tests passed
  /// bOK for functionality testing.
  /// bOK1 for validation testing.

  bool bOK = true, bOK1 = true;

  fprintf(fp, "\n \n ------------- Create part : createPart, Delete Part deletePart test ------------");

  /* Loop through all partitions on the process to create a part in each of them*/
  for(int p = 0; p < ptnSize; p++ )
  {
  /* Step 1: Get the total number of on-process parts using iMeshP_getNumLocalParts. */
  iMeshP_getNumLocalParts(instance, pPtnHandles[p], &iNumLocalParts, &ierr);
   
  CHECK_ERR(ierr, "iMeshP_getNumLocalParts");

  if(ierr!=iBase_SUCCESS)
   bOK1 = false; /// test of correctness not possible
   
  fprintf(fp, "\n Total number of on-process parts before carrying out tests: %d \n", iNumLocalParts);

  if(iNumLocalParts != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

  /* Part handles of newly created parts*/
  iMeshP_PartHandle p_Part[iPrtsPerProcess];
 
  for(int pr = 0; pr < iPrtsPerProcess; pr++)
   {
  /// Step 2: Create a part in the given partition. 
  iMeshP_createPart(instance, pPtnHandles[p], &p_Part[pr], &ierr);
  
  fprintf(fp , "\n Created part %d \n", pr);

  CHECK_ERR(ierr, "iMeshP_createPart");

  /* If the result is'nt success then exit the loop.*/
  if(ierr != iBase_SUCCESS)
     {
     bOK = false;
     break;
     }
    
  /* increment parts per process counter, it keeps track what is the total number of parts on each process right now. We can get this count using iMeshP function too but we need to keep our own track */
   iNumPrts++;
  }

  TEST(bOK, 0);

  iMeshP_syncPartitionAll(instance, pPtnHandles[p], &ierr);

  CHECK_ERR(ierr, "iMeshP_syncPartitionAll");

  if(ierr != iBase_SUCCESS)
   bOK1 = false;

  /* Getting the part id of the newly created part.*/
  iMeshP_Part p_PartId;

  /// Step 4: Get back the newly created part ids. If error is thrown then something is wrong so fail. 
  for(int pr = 0; pr < iPrtsPerProcess; pr++)
   {
    iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[p], p_Part[pr], &p_PartId, &ierr);
    CHECK_ERR(ierr, "iMeshP_PartIdFromPartHandle");

    if(ierr != iBase_SUCCESS)
    {
    bOK1 = false;
    break;
   }
  } // end parts loop

  /// Step 5: Check if the total number of parts has increased by iPrtsPerProcess. If no then fail.
  iMeshP_getNumLocalParts(instance, pPtnHandles[p], &iNumPartsAftrCreate, &ierr);
 
   if(iNumPartsAftrCreate != (iNumLocalParts + iPrtsPerProcess ))
     {
     CHECK_VALIDATION_ERR("iMeshP_createPart");
     bOK1 = false;
     }

   /// Now deleting parts
    for(int dty = 0; dty < iPrtsPerProcess; dty++)
	{
        iMeshP_destroyPart(instance, pPtnHandles[p], p_Part[dty], &ierr);	
	--iNumPrts;
	CHECK_ERR(ierr, "iMeshP_destroyPart");
	if(ierr != iBase_SUCCESS)
	{
	bOK = false;
	break;
	}
        fprintf(fp, "\n Destroying part %d \n", dty);
        }

  TEST(bOK, 0);

 /// Now get local parts again; the number of parts must have dropped down by iPrtsPerProcess.
 int iNumLocalPartDtry;
 iMeshP_getNumLocalParts(instance, pPtnHandles[p], &iNumLocalPartDtry, &ierr);
 CHECK_ERR(ierr, "iMeshP_getNumLocalParts");

  if(ierr!=iBase_SUCCESS)
   bOK1 = false;

 /* Now test if the number of parts has decreased by one*/
  if(iNumLocalPartDtry != iNumLocalParts )
  {
    CHECK_VALIDATION_ERR("iMeshP_destroyPart");
   //fprintf(fp, "\n ERROR: Number of part did'nt decrease by %d Before: %d . Exiting.\n",iPrtsPerProcess, iNumLocalParts);
   bOK1 = false;
  }

  iMeshP_syncPartitionAll(instance, pPtnHandles[p], &ierr);
  CHECK_ERR(ierr, "syncPartitionAll");

   if(ierr != iBase_SUCCESS)
   bOK1 = false;

  TEST(bOK1, 1);
  } // end partition loop
  /// Free the memory
  free(pPtnHandles);
}// end function createPart

/* Tests iMeshP_getPartIdFromPartHandleArr
*
* Dependencies:iMeshP_getLocalParts, iMeshP_getRankOfPart, iMeshP_destroyPart
iMeshP_isEntOwner, iMeshP_getPartBdryEnts, iMeshP_getEntOwnerPart.
* Test cases: 
* 1- Should return valid part ids when valid part handle array is given
* 2- using the returned valid part id, it should get a valid rank of the part.
* 3- Should return an iBase failure when a remote (off-process) part id is given.
*/

void vGetPrtIdFromPrtHandleArrTest(iMesh_Instance instance)
{
   //  char *cFuncName = "iMeshP_getPartIdFromPartHandleArr";
   /// Get all the partitions 
  iMeshP_PartitionHandle* pPtnHandles;
  int ptnSize = 0, ptnAllocated = 0;
 
  iMeshP_getPartitions(instance, &pPtnHandles, &ptnAllocated, &ptnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "getPartitions");

  /// For the on-process local parts
  int iPrtSize = 0, iPrtAllocated = 0;

   /// For keeping track of tests passed
  /// bOK for functionality testing.
  /// bOK1 for validation testing.

  bool bOK = true, bOK1 = true;

for(int p = 0; p < ptnSize; p++ )
  {

   fprintf(fp, "\n \n ------------- Get Part ids from part handle array : getPartIdsFromPartHandlesArr test ------------");
 
   /// Container for the local parts
   iMeshP_PartHandle* pLocalParts = NULL;

   ///  Step 1: Get all local part handles in the partition
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   
   /// If getting the local parts operation doesn't succeed then we can't proceed so quit.
    CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

  if(iPrtSize != iNumPrts)
   {
    fprintf(fp, "\n iPrtSize %d iNumPrts %d \n", iPrtSize, iNumPrts);
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   // fprintf(fp, "\n Local part handles of process %d : %d \n", me, iPrtSize);

  /// Step 2: For all of the local part handles, get the part ids using getPartIdsFromPartHandleArr.

  int iPrtSizeOut = 0, iPrtAllocatedOut = 0;
  iMeshP_Part* pPartIds = NULL;
  iMeshP_getPartIdsFromPartHandlesArr(instance, 
                                      pPtnHandles[p], 
                                      pLocalParts, 
                                      iPrtSize,  
				      &pPartIds,
			              &iPrtAllocatedOut,
				      &iPrtSizeOut, 
				      &ierr);

  /* Fail the test if the above operation fails */
  CHECK_ERR(ierr, "iMeshP_getPartIdsFromPartHandlesArr");

  if(ierr != iBase_SUCCESS)
	{
  	bOK = false; bOK1 = false;
	break;
	}
    
/// CHECKIT iPrtAllocatedOut
  if(iPrtAllocatedOut != iPrtSize)
       {
        CHECK_VALIDATION_ERR("iMeshP_getPartIdsFromPartHandlesArr");
	//fprintf(fp, "\n ERROR: Size of resulting array is different frm input. Continuing with next partition.\n");
	bOK = false; bOK1 = false;
	break;
	}

  // fprintf(fp, "\n Got part ids from the array of part handles, Number: %d \n", iPrtSizeOut);
   
/*  Step 3: For every part id returned, get the rank of the part to check if the part id is valid. If rank less than zero or greater than total local parts then fail */ 

 for(int ph = 0; ph < iPrtSize; ph++ )
   {
	int iRank;
        iMeshP_getRankOfPart(instance, pPtnHandles[p], pPartIds[ph], &iRank, &ierr);

	CHECK_ERR(ierr, "iMeshP_getRankOfPart");
        if(ierr!=iBase_SUCCESS)
	{
        bOK1 = false;
	break;
	}
	if(iRank != me )
		{	
		bOK1 = false;
		break;
		}

  // fprintf(fp, "\n Rank of part %d : %d \n", ph, iRank);
   } // end for loop

/// Step 4: Now get boundary entities; check if the local part is owner of the boundary entities. 
/// If yes, get entity's owner part id and compare with the part id of local parts
/// that we got using the getPartIdsArrFromPartHandles.
/// This compares if we have consistency between ids returned by different functions like getEntOwnerPart and getPartIdsFromPartHandlesArr.

  for(int pr = 0; pr < iPrtSize; pr++ )
  {
   iBase_EntityHandle* pEntities = NULL;
   int iEntsAllocated, iEntsSize;

   iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],  
            iBase_ALL_TYPES, 
            iMesh_ALL_TOPOLOGIES, 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntsAllocated,
            &iEntsSize, 
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");
   if(ierr!=iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}
   
  for(int ent = 0; ent <iEntsSize; ent++)
  {
   int iIsOwner;
   iMeshP_isEntOwner(instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            pEntities[ent], 
            &iIsOwner, 
            &ierr);

   if(ierr!=iBase_SUCCESS)
	{
        CHECK_ERR(ierr, "iMeshP_isEntOwner");
	bOK1 = false;
	break;
	}

   if(iIsOwner == 1)
    {
     /// Now get owning part id.
     iMeshP_Part pId;
     iMeshP_getEntOwnerPart(instance,
            pPtnHandles[p],
            pEntities[ent], 
            &pId,
            &ierr);

    if(ierr!=iBase_SUCCESS)
	{
        CHECK_ERR(ierr, "iMeshP_getEntOwnerPart");
	bOK1 = false;
	break;
	}

    bool bIdThere = false;
    for(int id = 0; id < iPrtSizeOut; id++)
	{
	if(pId == pPartIds[id])
	  {
	    bIdThere =true;	
	    break;
	   }
	} 
   if(!bIdThere)
	{
	//fprintf(fp, "\n Id returned from getPartIdsFromPartHandlesArr is not the same as getEntOwnerPart \n");
        CHECK_VALIDATION_ERR("iMeshP_getPartIdsFromPartHandlesArr");
	bOK1 = false;
	break;
	}
   } // end if isowner
   
   } // end ent loop
  free(pEntities);
  } // end parts loop
  TEST(bOK, 0);
  TEST(bOK1, 1);
  free(pLocalParts);
  } // end partitions loop

free(pPtnHandles);
}


/* Tests iMeshP_getPartIdFromPartHandle
* Dependencies:iMeshP_getLocalParts, iMeshP_getRankOfPart, iMeshP_getPartIdsFromPartHandlesArr
* Test Cases: 
* 1- Return valid part id when a valid part handle array is given
* 2- Give a valid rank of the part using the returned part id. 
* 3- When an invalid/remote part handle is given, it should throw an error.
*/
void vGetPrtIdFromPrtHandleTest(iMesh_Instance instance)
{
  //  char *cFuncName = "iMeshP_getPartIdFromPartHandle";
 /// Get all the partitions 
  iMeshP_PartitionHandle* pPtnHandles;
  int ptnSize = 0, ptnAllocated = 0;
  int iPrtSize = 0, iPrtAllocated = 0;

  iMeshP_getPartitions(instance, &pPtnHandles, &ptnAllocated, &ptnSize, &ierr);

  /// If the function fails then throw error and exit.
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

// Loop through all the partitions, get local part handles and get their part ids
for(int p = 0; p < ptnSize; p++ )
  {
   fprintf(fp, "\n \n ------------- Get part id from part handle test: getPartIdFromPartHandle ------------");
   iMeshP_PartHandle* pLocalParts = NULL;
   ///  Get all local part handles in the partition
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   // fprintf(fp, "\n Got local parts on process %d, Number: %d \n", me, iPrtSize);

  /* Step 1: For all part handles, get part id using the method of getPartIdsFromPartHandlesArr. It takes a part handle and returns a part id */
  bool bOK = true;
  bool bOK1 = true;

  for(int ph = 0; ph < iPrtSize; ph++)
    {
	/// variable for Part id of the local part handle.
        iMeshP_Part pPartIdTmp;
	
	iMeshP_getPartIdFromPartHandle(instance, pPtnHandles[p], pLocalParts[ph], &pPartIdTmp, &ierr);
	 CHECK_ERR(ierr, "iMeshP_getPartIdFromPartHandle");
	 
	if(ierr != iBase_SUCCESS)
	{
	  bOK = false;
	  break;
	}
           /// Step 2: Get the rank of the part id to check if the rank is valid 
 	int iRank;
        iMeshP_getRankOfPart(instance, pPtnHandles[p], pPartIdTmp, &iRank, &ierr);
	CHECK_ERR(ierr, "iMeshP_getRankOfPart");
	  
        if(ierr!=iBase_SUCCESS)
	{
          bOK1 = false;
	  break;
	}

	//fprintf(fp, "\n Rank of part no. %d in process %d is %d \n", ph, me, iRank);
 
        /// Step 3: If wrong rank is returned then fail. Rank returned should be equal to the rank of the process
        if(iRank != me)
        {
	//fprintf(fp, "\n ERROR: Unable to get part id from part handle. Exiting.\n");
	CHECK_VALIDATION_ERR("iMeshP_getPartIdFromPartHandle");
	bOK1 = false;
	break;
	}
             
   } // end part loop
  TEST(bOK, 0);
  TEST(bOK1, 1);
  free(pLocalParts);
  } // end partitions loop
free(pPtnHandles);
}// end function

/* Tests getPartHandleFromPartId
*  Dependencies: iMeshP_getPartitions,iMeshP_getLocalParts, iMeshP_getPartNbors.
* Test cases: 
   Get boundary entities between a part and all other parts. For every entity, get its owner part id. 
    a- If the owner part id is of a local part, the operation getPartHandleFromPartId should succeed. In that case, compare the returned part handle with local part handles to check if it is there.
    b- If the owner part id is for a off-process/remote part then the operation should always return iBase failure (I assume so).
*/

void vGetPrtHandleFromPrtIdTest(iMesh_Instance instance)
{
   // char *cFuncName = "iMeshP_getPartHandleFromPartId";
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles;
  int ptnSize = 0, ptnAllocated = 0;
  int iPrtSize = 0, iPrtAllocated = 0;

  iMeshP_getPartitions(instance, &pPtnHandles, &ptnAllocated, &ptnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

fprintf(fp, "\n \n ------------- Get part handle from part id: getPartHandleFromPartId ------------");

/// For every partition, get its local parts and perform the test.
for(int p = 0; p < ptnSize; p++ )
  { 
   ///  Step 1: Get all local part handles in the partition
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

    CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

  // fprintf(fp, "\n Got all Local Parts %d \n ", iPrtSize);

    /// bOK for basic functionality testing
    /// bOK1 for testing if the results are valid.

    bool bOK = true, bOK1 = true;

   /// Get the local part ids from the part handles.
    iMeshP_Part* pLocalIds = NULL;
    int iLocalIdsAlloc, iLocalIdsSize;

    iMeshP_getPartIdsFromPartHandlesArr(instance,
            pPtnHandles[p],
            pLocalParts,
            iPrtSize,
            &pLocalIds,
            &iLocalIdsAlloc,
            &iLocalIdsSize,
            &ierr);

   CHECK_ERR(ierr, "iMeshP_getPartIdsFromPartHandlesArr");

    if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}

 /// Now test for correctness

 for(int pr = 0; pr < iPrtSize; pr++ )
    {
  /// Step 2: For every part in the partition, get its boundary entities w.r.t all other parts.
   iBase_EntityHandle* pEntities = NULL;
   int iEntsAllocated, iEntsSize;

   iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],  
            iBase_ALL_TYPES, 
            iMesh_ALL_TOPOLOGIES, 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntsAllocated,
            &iEntsSize, 
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");

   if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}

 /// Step 3: For every entity, get its owner part id. Try to transform the owner part id into a part handle. 
 /// If the owner part is a local one, the operation getPartHandleFromPartId should succeed. If the owner part is
 /// an off-process part (remote part) then the operation should fail. 

 for(int ent = 0; ent <iEntsSize; ent++)
  {
   iMeshP_Part pPrtId;

   iMeshP_getEntOwnerPart(instance,
            pPtnHandles[p], 
            pEntities[ent],
            &pPrtId,
            &ierr);

    if(ierr != iBase_SUCCESS)
	{
	//fprintf(fp, "\n ERROR: Dependency failed: getting owner part id: getEntOwnerPart\n");
	CHECK_ERR(ierr, "iMeshP_getEntOwnerPart");
	bOK1 = false;
	break;
	}
 /// Check if the owning part id is among the local part ids or not.
   bool isLocalId = false;
   for(int id = 0; id < iLocalIdsSize; id++)
     {
       if(pPrtId == pLocalIds[id])
	{
         isLocalId = true;
	 break;
	}
     }
 /// Now try to get local part handle from the part id.
   iMeshP_PartHandle pPrtHandle;

   iMeshP_getPartHandleFromPartId(instance,
            pPtnHandles[p], 
            pPrtId,
            &pPrtHandle,
            &ierr);

  /// If not success then remote part id is given; try with next entity owner.
  if(ierr != iBase_SUCCESS && !isLocalId)
  {
    //fprintf(fp, "\n Remote part Id: Checking the error returned. \n");
    CHECK_ERR2(ierr, iBase_FAILURE);

    /// If the part id is for a remote part & error is not iBase_failure then we fail.
    if(ierr != iBase_FAILURE)
	{
	CHECK_VALIDATION_ERR("iMeshP_getPartHandleFromPartId");
	bOK = false;
   	bOK1 = false;
	}
  }

  if(ierr != iBase_SUCCESS && isLocalId)
   {
    //fprintf(fp, "\n Unable to get part handle for local part id \n");
    CHECK_VALIDATION_ERR("iMeshP_getPartHandleFromPartId");
    bOK = false;
     bOK1 = false;
    break;
   }
  /// Step 4: If the part id is for a local part handle, check the handle against the local ones.
   bool bIsThere = false;

   for(int prt = 0; prt < iPrtSize; prt++ )
    if(pLocalParts[prt] == pPrtHandle)
     {
     bIsThere = true;
      break;
     }
   if(!bIsThere)
    {
     //fprintf(fp, "\n ERROR: The returned part handle does not match any of the local parts. \n");
     CHECK_VALIDATION_ERR("iMeshP_getPartHandleFromPartId");
     bOK1 = false;
     break;
    }
    
 } // end ents loop
free(pEntities);

/// Step 4: Now for the local part, get its neighbors. If the neighboring id is not among the local parts, try to get the part id from the part handle. It should fail because we are trying to get part handle for off-process part.
   int iNumPrtNbors, iNborIdsAlloc, iNborIdsSize;
   iMeshP_Part* pNborIds = NULL;

   iMeshP_getPartNbors(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            iBase_ALL_TYPES,
            &iNumPrtNbors,
            &pNborIds,
            &iNborIdsAlloc,
            &iNborIdsSize,
            &ierr);
  CHECK_ERR(ierr, "iMeshP_getPartNbors");

  if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}

/// For each neighboring part id, check if it is among the on-process parts. If not try to get its part handle. Should fail.
  for(int nbor = 0; nbor < iNborIdsSize; nbor++ )
    {
    bool bIsThere = false;
   for(int lcl = 0; lcl < iPrtSize; lcl++)
    {  
    if(pLocalIds[nbor] == pNborIds[nbor])
      {
	bIsThere = true;
	break;
      }
    } // end lcl loop
    if(!bIsThere)
    {
     iMeshP_PartHandle pPrtHandle;
     iMeshP_getPartHandleFromPartId(instance,
            pPtnHandles[p], 
            pNborIds[nbor],
            &pPrtHandle,
            &ierr);

    if(ierr == iBase_SUCCESS)
    {
    //fprintf(fp, "\n ERROR: Converted remote part id into local part handle. Exiting. \n");
    CHECK_VALIDATION_ERR("iMeshP_getPartHandleFromPartId");
    bOK1 = false;
    break;
   }
    } // end if bIsThere
    } // end nbor
   free(pNborIds);
   free(pLocalParts);
   } // end parts loop
TEST(bOK, 0);
TEST(bOK1, 1);

free(pPtnHandles);
}// end partition loop 

}
/* Test iMeshP_getPartHandleArrFromPartsIds
  * Dependencies: iMeshP_getPartitions, iMeshP_getLocalParts, iMeshP_getPartNborsArr, iMeshP_getPartIdsFromPartHandlesArr
* Test Cases:
1- Get an array of local part ids. Transform it into part handles. Given that we have local part ids only, the operation should succeed.
2- Get an array of neighboring part ids. In case of single part/process, the operation should fail because all neighboring parts are going to be remote. 
*/
void vGetPrtHandleArrFromPrtIdsTest(iMesh_Instance instance)
{
  //  char *cFuncName = "iMeshP_getPartHandleArrFromPartsIds";
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int ptnSize = 0, ptnAllocated = 0;
  int iPrtSize = 0, iPrtAllocated = 0;

  iMeshP_getPartitions(instance, &pPtnHandles, &ptnAllocated, &ptnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");


fprintf(fp, "\n \n ------------- Get part handle array from part ids: getPartHandleArrFromPartIds test ------------");

/// bOK for basic functionality testing
    /// bOK1 for testing if the results are valid.

   bool bOK = true, bOK1 = true;

/// For every partition, get its local parts and perform the test.
for(int p = 0; p < ptnSize; p++ )
  { 
   ///  Step 1: Get all local part handles in the partition. Local part handles will be in form of array.
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   //fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);

        /// Step 2: Now get unique part ids array for the local part handles.

   iMeshP_Part* pPrtIdsArr = NULL;
   int iPrtAllocated2, iPrtSize2;

   iMeshP_getPartIdsFromPartHandlesArr(instance,
            pPtnHandles[p], 
            pLocalParts,
            iPrtSize,
            &pPrtIdsArr,
            &iPrtAllocated2,
            &iPrtSize2,
            &ierr);

  CHECK_ERR(ierr, "iMeshP_getPartIdsFromPartHandles");

  if(ierr != iBase_SUCCESS)
   {
    //fprintf(fp, "\n ERROR: Dependency failed: getting part ids from part handles: getPartIdsFromPartHandles\n");
    bOK1 = false;
     }
  
  /// Step 3: Convert the array of unique part ids back to local part handles. This should always work since we all part ids correspond to local parts.

    iMeshP_PartHandle* pPrtHandles = NULL;
    int iPrtSize3, iPrtAllocated3;

    iMeshP_getPartHandlesFromPartsIdsArr(instance,
            pPtnHandles[p], 
            pPrtIdsArr,
            iPrtSize2,
            &pPrtHandles,
            &iPrtAllocated3,
            &iPrtSize3,
            &ierr);

  CHECK_ERR(ierr, "iMeshP_getPartHandlesFromPartsIdsArr");
/// Step 4: Check returned error code. Also check if the array size and allocation is correct.
   if(iPrtAllocated3 != iPrtAllocated || iPrtSize3 != iPrtSize || ierr != iBase_SUCCESS)
     {
     //fprintf(fp, "\n ERROR: Failed getting part handle array from parts ids. \n");
     CHECK_VALIDATION_ERR("iMeshP_getPartHandlesFromPartsIdsArr");
     bOK = false;bOK1 = false;
     break;
     }

/// Step 5: Compare the returned local part handles with the ones we got using getLocalParts. they should be the same.  
   for(int ii = 0; ii < iPrtSize; ii++ )
    {
    if(pPrtHandles[ii] != pLocalParts[ii])
       {
       //fprintf(fp, "\n ERROR: failed getting array of local part handles from unique ids \n");
        CHECK_VALIDATION_ERR("iMeshP_getPartHandlesFromPartsIdsArr");
       bOK1 = false;
	break;
       }
    }

/// Step 6: Get array of neighboring part id and try to get the local part handles. For SPPP, this should fail.
  if(iPrtSize == 1)
   {
   int iNumPrtNbors, iNborIdsAlloc, iNborIdsSize;
   iMeshP_Part* pPrtIds = NULL;

   iMeshP_getPartNbors(instance,
            pPtnHandles[p], 
            pLocalParts[0],
            iBase_ALL_TYPES,
            &iNumPrtNbors,
            &pPrtIds,
            &iNborIdsAlloc,
            &iNborIdsSize,
            &ierr);

  CHECK_ERR(ierr, "iMeshP_getPartNbors");

   if(ierr != iBase_SUCCESS)
	{
	//fprintf(fp, "ERROR: Dependency failed: getting part neighbors: getPartNbors ");
	bOK1 = false;	
	break;
	}

   iMeshP_PartHandle* pnborHandles = NULL;
   int iNborAlloc, iNborSize;

   iMeshP_getPartHandlesFromPartsIdsArr(instance,
            pPtnHandles[p], 
            pPrtIds,
            iNborIdsSize,
            &pnborHandles,
            &iNborAlloc,
            &iNborSize,
            &ierr);

  /// this should not succeed for SPPP.
  if(ierr != iBase_FAILURE)
   {
    //fprintf(fp, "\n ERROR: Got part handle for remote part id. \n");
    CHECK_VALIDATION_ERR("iMeshP_getPartHandlesFromPartsIdsArr");
    bOK1 = false;
    break;
   }
   free(pPrtIds);
   } // end if PrtSize condition

   free(pPrtHandles);
   free(pLocalParts);
   free(pPrtIdsArr);
   } // end partitions loop
   TEST(bOK, 0);
   TEST(bOK1, 1);

free(pPtnHandles);
} // end function

//////////////////////////////////////////PART BOUNDARY FUNCTIONALITY/////////////////////////////////////////////

/* Test: iMeshP_getPartBdryEnts, iMeshP_getNumPartBdryEnts
Dependencies: iMeshP_getEntStatus, iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Should get the part boundary entities of all combinations of types and topologies. For each type and topology, cross check if the entity is actually a boundary entity w.r.t the given part. This can be done by checking entity status.

2- Compare the number of boundary entities got with the number got through getBdryEnts. If it is different then fail.
*/
void vGetPrtBdryEntsTest(iMesh_Instance instance)
{
  //   char *cFuncName = "iMeshP_getPartBdryEnts, iMeshP_getNumPartBdryEnts";
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int ptnSize = 0, ptnAllocated = 0;
  int iPrtSize = 0, iPrtAllocated = 0;

  iMeshP_getPartitions(instance, &pPtnHandles, &ptnAllocated, &ptnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, " iMeshP_getPartitions");
  fprintf(fp, "\n \n ------------- Getting part boundary entities: getPartBdryEnts, getNumPartBdryEnts test ------------");

/// For every partition, get its local parts and perform the test.
for(int p = 0; p < ptnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   //fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);
   
     // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
	 /// bOK for checking getPartBdryEnts does not throw error.
	/// bOK1 for checking getNumPartBdryEnts does not throw error.
	/// bOK2 for checking if getPartBdryEnts returns accurate results and 
	/// for checking if getPartBdryEnts and getNumPartBdryEnts return same number of entities

         bool bOK = true, bOK1 = true, bOK2 = true;
	 fprintf(fp, "---------------------------------------------------------");
         fprintf(fp, "\n Testing: Type: %s Topology: %s \n", astrTypes[eEType], astrTopologies[eETopo]);

       /// for every part, check all combinations.
   for(int pr = 0; pr < iPrtSize; pr++ )
    {
      iBase_EntityHandle* pEntities = NULL;
      /// pEntities stores the boundary entities
       int iEntities_allocated, iEntities_size;

     /// Step 1: Get part boundary entities for all possible combinations.
	iMeshP_getPartBdryEnts(
	    instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            static_cast<iBase_EntityType>(eEType), 
            static_cast<iMesh_EntityTopology>(eETopo), 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntities_allocated,
            &iEntities_size, 
            &ierr); 

      CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");

      int prterr = ierr;
      
     /// Now using the same combination of types and topo, try iMeshP_getNumPartBdryEnts
     int iEntities_Num = 0;
      iMeshP_getNumPartBdryEnts(
	    instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            static_cast<iBase_EntityType>(eEType), 
            eETopo, 
            iMeshP_ALL_PARTS, 
            &iEntities_Num, 
            &ierr); 

     CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");

      if(ierr!=iBase_SUCCESS)
	{
          fprintf(fp, "\n Error getting boundary entitites. %d\n", ierr);
	  bOK1 = false;
	  bOK2 = false;
	  break;
	}
    /// prterr holds error code for getPartBdryEnts.
    /// if any entity type and topo combination is unsupported, throw a warning.

	if(prterr == iBase_BAD_TYPE_AND_TOPO) 
            fprintf(fp, "\n Warning: Type %s and Topology %s unsupported \n", astrTypes[eEType], astrTopologies[eETopo] );
		
	if(prterr != iBase_SUCCESS && prterr != iBase_BAD_TYPE_AND_TOPO)
	  {
	   fprintf(fp, "\n ERROR: getting part boundary entities \n");
	   bOK = false; bOK2 = false;
	   break;
	   }
      
  /// Step 2: Cross Check: Get the entity status of all the entities w.r.t the part, it should be boundary, otherwise fail.
      int* iEntStatusArr = NULL;
      int iParStatusSize, iParStatusAlloc;

      iMeshP_getEntStatusArr(instance,
            pPtnHandles[p],
            pLocalParts[pr],  
            pEntities,
            iEntities_size,
            &iEntStatusArr, 
            &iParStatusSize, 
            &iParStatusAlloc, 
            &ierr); 

    CHECK_ERR(ierr, "iMeshP_getEntStatusArr");

       if(ierr!=iBase_SUCCESS)
	{
	  fprintf(fp, "\n Dependency failed: iMeshP_getEntStatusArr \n");
	  bOK2 = false;
	  break;
	}

     // 1 corresponds to boundary entities.
     for(int ii = 0; ii < iParStatusAlloc; ii++ )
          if(iEntStatusArr[ii] != 1)
           {	
	   bOK2 = false;
	   fprintf(fp, "\n ERROR: Entity %d is not a boundary entity .Status: %d\n", ii, iEntStatusArr[ii]);
	   CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
	   break;
           }

      fprintf(fp, "\n # of boundary entities : %d \t part id: %d \t type: %s \t topology: %s\n", iEntities_Num,pr,astrTypes[eEType], astrTopologies[eETopo]);

      /// Step 3: Now compare the number of boundary entities with the size returned by getBdryEnts.If not same then fail.
      if(iEntities_size != iEntities_Num)
	{
fprintf(fp, "\n getPartBdryEnts and getNumPartBdryEnts do not return the same result for type %s topo %s. \n", astrTypes[eEType], astrTopologies[eETopo]);
      CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
      bOK2 = false;
	}
  
      free(iEntStatusArr);
      free(pEntities);
      }// end parts loop
	TEST(bOK, 0);
	TEST(bOK1, 0);
	TEST(bOK2, 1);
  	} // end if statement
     }// end inner for loop
    } //end outer for loop
free(pLocalParts);
} // end partitions loop
free(pPtnHandles);
} // end function

/* Test: iMeshP_getPartNbors
Dependencies: iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Check if the function returns the correct neighboring parts w.r.t a given part. By definition of neighbors in iMeshP spec, part A is a neighbor of another part B if part A has copies of entities owned by Part B or Part B has copies of entities owned by part A. Cross check by getting the number of boundary entities shared between Part A and Part B. 

- If the boundary entities & ghosts shared between parts are zero and the parts given by function are neighbors then throw error.

*/
void vGetPrtNborsTest(iMesh_Instance instance)
{
    // char *cFuncName = "iMeshP_getPartNbors";
    /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;

  /// Partition size and allocation
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting neighbors of a given part: getPartNbors test ------------\n");

    /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

// ***** Test case 1 : Get the neighboring parts of a given part for all entity types ********

  /// Step 1: First loop through all parts and for every Part A get it's neighbors. 

for(int p = 0; p < iPtnSize; p++ ) 
  { 
    
   ///  Get all local part handles in the partition to perform tests
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   /// No local parts found then exit
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }
     /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

     for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {

     /// Loop through all parts
     bool bOK = true;
     bool bOK1 = true;
     fprintf(fp, "--------------------------------------------");
     fprintf(fp, "\n Testing Type: %s \n", astrTypes[eEType]);

     for(int pr = 0; pr < iPrtSize; pr++ )
       {
    //fprintf(fp, "\n Getting neighbors for part no. : %d \n ", pr);

   int iNborPrtSize = 0, iNborPrtAllocated = 0, iNumPrtNbor = 0;

   /// PNborPrtIds stores the neigboring part ids
   iMeshP_Part* pNborPrtIds = NULL;
  
   iMeshP_getPartNbors(instance,
		       pPtnHandles[p], 
		       pLocalParts[pr], 
		       static_cast<iBase_EntityType>(eEType), 
		       &iNumPrtNbor,
		       &pNborPrtIds, 
                       &iNborPrtAllocated,
		       &iNborPrtSize,
		       &ierr);

   /// If a failure error is thrown then exit because we can't continue with rest of the tests.
  CHECK_ERR(ierr, "iMeshP_getPartNbors");
   if(ierr != iBase_SUCCESS)
     {
     //fprintf(fp,"\n Unable to get part neighbors with entity type %s .\n", astrTypes[eEType]);
     bOK = false;bOK1 = false;
     break;
      }

   fprintf(fp, "\n # of neighbors of the given part: %d \t type: %s \n", iNborPrtSize, astrTypes[eEType]);
  
  /// Step 2: Loop through all the neighbors to check if they actually share entities with the Part A. 

  for(int nbor = 0; nbor < iNborPrtSize; nbor++ )
   {
 
  /// Step 3: Get the number of boundary entity between the neighbor part and Part A. 
   int iNumEntities = 0;
   iMeshP_getNumPartBdryEnts(instance,
			  pPtnHandles[p], 
			  pLocalParts[pr], 
			  static_cast<iBase_EntityType>(eEType),
			  static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
			  pNborPrtIds[nbor],
			  &iNumEntities,
			  &ierr);
   CHECK_ERR(ierr, "iMeshP_getNumPartBdryEnts");
   if(ierr != iBase_SUCCESS)
    {
    //fprintf(fp,"\n Dependency failed: getNumPartBdryEnts %s .\n", astrTypes[eEType]);
     bOK1 = false;
     break;
    }

  /// If no entities are shared, check for ghost entities. 
   if(iNumEntities <= 0)
	{
        //fprintf(fp, "\n Checking for ghosts \n");
   	bool bIsThere = false;

    /// Check ghost entities using getCopyOnPart.
	int iEntSize, iEntAlloc;
	iBase_EntityHandle* pEnts = NULL;
        
        iMeshP_getEntities(instance,
            pPtnHandles[p], 
            pLocalParts[pr], 
            root,
            static_cast<iBase_EntityType>(eEType),
	    static_cast<iMesh_EntityTopology>(iMesh_ALL_TOPOLOGIES),
            &pEnts,
            &iEntAlloc,
            &iEntSize,
            &ierr);

      CHECK_ERR(ierr, "iMeshP_getEntities");
      if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}
	/// Now for each entity, do getCopyOnPart
	/// If there exists a single copy shared between parts, then break.
	for(int ii = 0; ii < iEntSize; ii++ )
	{
	iBase_EntityHandle pCopy;
	iMeshP_getCopyOnPart(instance,
            pPtnHandles[p], 
            pEnts[ii], 
            pNborPrtIds[nbor], 
            &pCopy, 
            &ierr);
	if(ierr == iBase_SUCCESS) 
	{
	bIsThere = true;
	break;
	}
	} // end ii
	if(!bIsThere)
	{
	//fprintf(fp, "\n ERROR: No copies found between parts. Wrong neighbor returned! \n");
	CHECK_VALIDATION_ERR("iMeshP_getPartNbors");
	bOK1 = false;
	break;
	}
	}

   /// Print the number of boundary entities     
   fprintf(fp, "\n Neighbor: %d : \t # of Boundary Ents %d \n", nbor, iNumEntities);
   }// end neighbors loop

   free(pNborPrtIds);
  } // end parts loop
  TEST(bOK, 0);
  TEST(bOK1, 1);
 } // end types loop

  free(pLocalParts);
}// end partitions loop
 free(pPtnHandles);
}// end function call

/* Test: iMeshP_getNumPartNbors
Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Check if the function returns the correct number of neighbors for a given part for different entity types (using iMeshP_getPartNbors). Cross check with getPartNbors,

*/
void vGetNumPrtNborsTest(iMesh_Instance instance)
{
    // char *cFuncName = "iMeshP_getNumPartNbors";
  ///***** Check for iMeshP_getNumPartNbors ********

/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;

  /// Partition size and allocation
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Get Number of neighboring parts: getNumPartNbors test ------------\n");
  
// ***** Test case 1 : Get the neighboring parts of a given part for all entity types ********

  /// Step 1: First loop through all parts and for every Part A get it's neighbors. 

/// Loop through all partitions and perform tests

for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Step 1: Get all local part handles in the partition to perform tests
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   /// No local parts found then exit
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   //fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);
	
     for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
	fprintf(fp, "-----------------------------------------");
       fprintf(fp, "\n Testing type: %s \n", astrTypes[eEType]);
       bool bOK = true;
       bool bOK1 = true;
     /// Loop through all parts
     for(int pr = 0; pr < iPrtSize; pr++ )
       {
 /* Step 2: We have cross checked now that the parts are actually neighbors of each other because they share entities. This implies that getPartNbors function is correct (based on dependency of getPartBdryEnts). Now we can check if the getNumPartNbors function gives the right number by comparing its result with the size of getPartNbors */

   int iNumPrtNbors;

   iMeshP_getNumPartNbors(instance,
            pPtnHandles[p],
	    pLocalParts[pr], 
            static_cast<iBase_EntityType>(eEType),
            &iNumPrtNbors,
            &ierr);
    CHECK_ERR(ierr, "iMeshP_getNumPartNbors");
   if(ierr != iBase_SUCCESS)
    {
    bOK = false;bOK1 = false;
    break;
    }
    
   /// Now check if the neighbors are actually of the same size.

   /// PNborPrtIds stores the neigboring part ids
   bOK = true;
   iMeshP_Part* pNborPrtIds = NULL;
   int iNumnborGiven,iNborPrtAllocated, iNborPrtSize;

   iMeshP_getPartNbors(instance,
		       pPtnHandles[p], 
		       pLocalParts[pr], 
		       static_cast<iBase_EntityType>(eEType), 
		       &iNumnborGiven,
		       &pNborPrtIds, 
                       &iNborPrtAllocated,
		       &iNborPrtSize,
		       &ierr);

  if(iNumPrtNbors != iNborPrtSize)
    {
    //fprintf(fp, "\n Number of neighbors got by getPartNbors and getNumPartNbors not equal ! Exiting function \n");
    CHECK_VALIDATION_ERR("iMeshP_getNumPartNbors");
    bOK1 = false;
    break;
    }
   
   free(pNborPrtIds);
   } //end parts loop
  TEST(bOK, 0);
  TEST(bOK1, 1);
 } // end types loop
  free(pLocalParts);
}// end partitions loop
free(pPtnHandles);
} // end function

/* Test: iMeshP_getNumPartNborsArr
Dependencies: iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Check if the function returns the correct number of neighbors of given parts for all entity types (using iMeshP_getPartNbors). Cross check with getNumPartNbors. 

*/
void vGetNumPrtNborsArrTest(iMesh_Instance instance)
{
    // char *cFuncName = "iMeshP_getNumPartNborsArr";
   /// ******* Check for iMeshP_getNumPartNborsArr *********
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;

  /// Partition size and allocation
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

fprintf(fp, "\n \n -------------  Get number of neighbors for array of parts: getNumPartNborsArr test ------------\n");

/// Loop through all partitions and perform tests

for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Step 1: Get all local part handles in the partition to perform tests
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   /// No local parts found then exit
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);

     for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {

        bool bOK = true, bOK1 = true;
	fprintf(fp,"-------------------------------------------");
	fprintf(fp, "\n Testing type: %s \n", astrTypes[eEType]);
        int* num_part_nbors = NULL;
        int num_part_nbors_allocated = 0, num_part_nbors_size = 0;

    /// Step 2: Get number of part neighbors for an array of local parts. 
    iMeshP_getNumPartNborsArr(
            instance,
            pPtnHandles[p],
            pLocalParts,
            iPrtSize,
            static_cast<iBase_EntityType>(eEType),
            &num_part_nbors,
            &num_part_nbors_allocated,
            &num_part_nbors_size,
            &ierr); 
    /// Some entity types may be unsupported so for now we don't fail on unsupported entity types
   /// since this is not an iMeshP issue.
     CHECK_ERR(ierr, "iMeshP_getNumPartNborsArr");

     if(ierr == iBase_INVALID_ENTITY_TYPE)
     {
      fprintf(fp, "\n Warning: Entity type %s not supported \n", astrTypes[eEType]);
      continue;
     }
      else
        if(ierr != iBase_SUCCESS && ierr!= iBase_INVALID_ENTITY_TYPE)
         {
	   bOK = false;bOK1 = false;
	   break;
         }

 /// Step 3: Now compare if the getNumPartNborsArr and getNumPartNbors returned the same result. We already verified getNumPartNbors
  int iNumNbors = 0;
  for(int prt = 0; prt < iPrtSize; prt++ )
    {
     iMeshP_getNumPartNbors(instance,
            pPtnHandles[p],
            pLocalParts[prt],
            static_cast<iBase_EntityType>(eEType),
            &iNumNbors,
            &ierr);

   CHECK_ERR(ierr, "iMeshP_getNumPartNbors");
   if(ierr != iBase_SUCCESS)
   {
     bOK1 = false;
    //fprintf(fp, "\n Dependency failed: getting count of neighboring parts: getNumPartNbors \n "); 
   }

     if(iNumNbors != num_part_nbors[prt])
      {
      //fprintf(fp, "\n ERROR: Number of part neighbors returned by getNumPartNbors is different. %d %d\n", iNumPrts, num_part_nbors[prt]);
      bOK1 = false;
	break;
      }
    } // end parts loop
    TEST(bOK, 0);
    TEST(bOK1, 1);
   free(num_part_nbors);
    } // end types loop
   free(pLocalParts);
  }// end partitions loop
free(pPtnHandles);
} // end function call

/* Test: iMeshP_getPartNborsArr

Dependencies: iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Check if the function returns the correct number of neighbors for given parts with different entity types (using iMeshP_getPartNbors). Cross check with getPartNbors. 

*/
void vGetPrtNborsArrTest(iMesh_Instance instance)
{
   // char *cFuncName = "iMeshP_getPartNborsArr";
  ///***** Check for iMeshP_getPartNborsArr ********

/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;

  /// Partition size and allocation
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

fprintf(fp, "\n \n -------------  Getting part neighbors for array of parts: getPartNborsArr test ------------\n");

for(int p = 0; p < iPtnSize; p++ ) 
  { 

   ///  Step 1: Get all local part handles in the partition to perform tests
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   /// No local parts found then exit
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

  if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

 //  fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);

     for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
  fprintf(fp, "------------------------------------------");
  fprintf(fp, "\n Testing type: %s \n", astrTypes[eEType]);
  
  bool bOK = true, bOK1 = true;

  int iNum_part_nbors_size = 0, iNum_part_nbors_allocated = 0, iNbor_part_ids_allocated = 0, iNbor_part_ids_size = 0;

  int* iNum_part_nbors = NULL;
  iMeshP_Part* nbor_part_ids = NULL;

   /// Step 2: Make a set for storing neighboring part ids got from getPartNbors
    std::set<iMeshP_Part> prtSet2;
    std::set<iMeshP_Part> prtSet;

   for(int pr = 0; pr < iPrtSize; pr++ )
   {
     int iNumPrtNbors, iNborIdsAlloc, iNborIdsSize;
     iMeshP_Part* pNborPrts = NULL;

     iMeshP_getPartNbors(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            static_cast<iBase_EntityType>(eEType),
            &iNumPrtNbors,
            &pNborPrts,
            &iNborIdsAlloc,
            &iNborIdsSize,
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getPartNbors");
    if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
        iNborIdsSize = 0; // avoids breaking code
       }

   for(int ii = 0; ii < iNborIdsSize; ii++ )
    prtSet.insert(pNborPrts[ii]);

    free(pNborPrts);
   } // end parts loop

  iMeshP_getPartNborsArr(instance,
            pPtnHandles[p],
            pLocalParts,
            iPrtSize,
            static_cast<iBase_EntityType>(eEType),
            &iNum_part_nbors,
            &iNum_part_nbors_allocated,
            &iNum_part_nbors_size,
            &nbor_part_ids,
            &iNbor_part_ids_allocated,
            &iNbor_part_ids_size,
            &ierr);

  CHECK_ERR(ierr, "iMeshP_getPartNborsArr");

   if(ierr == iBase_INVALID_ENTITY_TYPE)
     {
      fprintf(fp, "\n Warning: Entity type %s not supported \n", astrTypes[eEType]);
      continue; // Go on with next entity type
     }
      else
        if(ierr != iBase_SUCCESS && ierr != iBase_INVALID_ENTITY_TYPE)
         {
           bOK = false;bOK1 = false;
           break;
         }

  for(int st = 0; st < iNbor_part_ids_size; st++ )
    {
     prtSet2.insert(nbor_part_ids[st]);
    }
   // Compare the set returned by getPartNbors and getPartNborArr and check if they have the same result.
   if(prtSet != prtSet2)
    {
    //fprintf(fp, "\n Get number of part neighbors from array of part handles test failed \n");
    CHECK_VALIDATION_ERR("iMeshP_getPartNborsArr");
    bOK1 = false;
    }
  
   free(nbor_part_ids);
   TEST(bOK, 0);
   TEST(bOK1, 1);
 } // end types loop
} // end partitions loop

   free(pPtnHandles);

} // end function call

/* Test: initPartBdryEntsIterator
Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Check if the iterator is successfully returned for all possible combinations of types and topos.
2- Check if the iterator performs the iMesh iterator functions like getNextEntIter,endEntIter and resetEntIter. 
4- For every returned entity, check it's status w.r.t the given part. Should be boundary. 
*/
void vBdryEntsIteratorTest(iMesh_Instance instance)
{
 //   char *cFuncName = "iMeshP_initPartBdryEntsIterator";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Testing boundary entity iterators ------------\n");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    fprintf(fp, "\n iPrtSize %d iNumPrts %d \n", iPrtSize, iNumPrts);
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }
     // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
       
     bool bOK = true, bOK1 = true;
     fprintf(fp, "----------------------------------------------------------------------------");
     fprintf(fp, "\n Testing Type: %s Topology: %s \n", astrTypes[eEType], astrTopologies[eETopo]);
      for(int pr = 0; pr < iPrtSize; pr++ )
       {   
     iBase_EntityIterator entity_iterator;
      int iNumEnts;
     /// Step 1: Get the number of boundary ents w.r.t parts
     /// Use this to skip iterator if the number is zero
     iMeshP_getNumPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            eEType, 
            eETopo, 
            iMeshP_ALL_PARTS,
            &iNumEnts,
            &ierr);

	/// I think getting number of boundary entities helps to test if the iterator should be null in case of
	/// no entities. 
	CHECK_ERR(ierr, "iMeshP_getNumPartBdryEnts");
	
       if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;bOK = false;
	break;
 	}
	   fprintf(fp, "\n # of boundary entities %d \t type %s \t topo %s \n", iNumEnts, astrTypes[eEType], astrTopologies[eETopo]);

 	/// Step 2: Now start the iterator
          iMeshP_initPartBdryEntIter(
            instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            eEType, 
            eETopo, 
            iMeshP_ALL_PARTS, 
            &entity_iterator, 
            &ierr); 

	 CHECK_ERR(ierr, "iMeshP_initPartBdryEntIter");

           
          if(ierr != iBase_SUCCESS)
		{
		bOK = false, bOK1 = false;
		break;
		}

   		  if(iNumEnts <= 0)
			{
			if(entity_iterator != NULL)
			 {
			 // fprintf(fp, "\n Not a null pointer for empty array iterator \n");
			  CHECK_VALIDATION_ERR("iMeshP_initPartBdryEntIter");
			  bOK = false; bOK1 =false;
			  break;
			}
			}
      /// Step 3: Check iMesh functions of getNextEntIter. Keep on getting next entity until the iterator has data.
              iBase_EntityHandle pEntity_handle;
              int iHas_data;
              iMesh_getNextEntIter(instance,
                            entity_iterator,
                            &pEntity_handle,
                            &iHas_data,
                            &ierr);
		CHECK_ERR(ierr, "iMeshP_getNextEntIter");
	     if(ierr != iBase_SUCCESS && iNumEnts > 0)
		{
		CHECK_VALIDATION_ERR("iMeshP_initPartBdryEntIter");
		bOK1 = false;
		bOK = false;
		}
		
	     while(iHas_data)
                {
		int iEntStatus;
		 /*Now get the entity status w.r.t the given part. The status should be boundary (1 is for boundary) */
                 iMeshP_getEntStatus(
                     instance,
                     pPtnHandles[p],
                     pLocalParts[pr], 
                     pEntity_handle, 
                     &iEntStatus,
                     &ierr); 

		CHECK_ERR(ierr, "iMeshP_getEntStatus");

		/* If the returned entity was not a boundary entity then break */
		if(iEntStatus != 1)
			{
			//fprintf(fp, "\n ERROR: The iterator returned an entity which did not have boundary status \n");
			CHECK_VALIDATION_ERR("iMeshP_initPartBdryEntsIterator");
			bOK1 = false;
			break;
			} 
		
		iMesh_getNextEntIter(instance,
                            entity_iterator,
                            &pEntity_handle,
                            &iHas_data,
                            &ierr);
		CHECK_ERR(ierr, "getNextEntIter");

		if(ierr != iBase_SUCCESS)
		  {
		  bOK1 = false;
		  break;
		  }
		} // end while iHas_Data

	     /// Now reset to make sure it works.
	     iMesh_resetEntIter(instance, entity_iterator, &ierr); 	   
	     CHECK_ERR(ierr,"iMesh_resetEntIter");

	     if(ierr != iBase_SUCCESS)
		{
		bOK1 = false;
		break;
		}

	    if(iNumEnts <= 0)
			{
			if(entity_iterator != NULL)
			 {
			  //fprintf(fp, "\n ERROR: Reset failed: Not a null pointer for empty array iterator \n");
			  CHECK_VALIDATION_ERR("iMeshP_initBdryIterator");
			  bOK1 = false;
			  break;
			}
			}
	    /// get the next entity just to check if reset works
  	     iMesh_getNextEntIter(instance,
                            entity_iterator,
                            &pEntity_handle,
                            &iHas_data,
                            &ierr);
	    CHECK_ERR(ierr, "iMeshP_getNextEntIter");

		if(ierr != iBase_SUCCESS)
		  {	
		bOK1 = false;
		break;
		}

              while(iHas_data)
                {
		iMesh_getNextEntIter(instance,
                            entity_iterator,
                            &pEntity_handle,
                            &iHas_data,
                            &ierr);
		CHECK_ERR(ierr, "iMeshP_getNextEntIter");

		if(ierr != iBase_SUCCESS)
		{
		bOK1 = false;		
		break;
		}
		}
             iMesh_endEntIter(instance, entity_iterator, &ierr);
	     if(ierr != iBase_SUCCESS)
		{
		bOK1 = false;
		CHECK_ERR(ierr, "iMeshP_endEntIter");
		break;
		}
   // Note: iMeshP_ALL_PARTS option supported?
           
       }//end parts loop
          TEST(bOK, 0);
          TEST(bOK1, 1);

      }//end topo loop
      } //end type loop
     
     } // end if condition
  free(pLocalParts);
 } //end partitions loop
  free(pPtnHandles);
} // end function

/* Test: initPartBdryArrIterator
Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases: 
1- Check if the iterator is returned for all possible combinations of types and topos.
2- Check if the iterator performs the iMesh iterator functions like getNextEntIter and endEntIter. 
3- Check the reset operation of the iterator // to be done for array iterator. 
4- For every returned entity, check it's status w.r.t the given part. Should be boundary. 
*/
void vBdryEntsArrIteratorTest(iMesh_Instance instance)
{
  //char *cFuncName = "iMeshP_initPartBdryArrIterator";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Testing boundary entity array iterators ------------\n");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);
  
     // Step 1: Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
         bool bOK = true, bOK1 = true;
 	fprintf(fp, "------------------------------------------------------------------");
 	fprintf(fp, "\n Testing Type: %s Topology: %s \n", astrTypes[eEType], astrTopologies[eETopo]);

     for(int pr = 0; pr < iPrtSize; pr++ )
    	   { 
	    int iNumEnts;

            /// Step 1: Get the number of boundary ents w.r.t parts
           /// Use this to skip iterator if the number is zero
           iMeshP_getNumPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            eEType, 
            eETopo, 
            iMeshP_ALL_PARTS,
            &iNumEnts,
            &ierr);

	   iBase_EntityArrIterator entity_array_iterator;

          iMeshP_initPartBdryEntArrIter(
            instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            eEType, 
            eETopo, 
            20,
            iMeshP_ALL_PARTS, 
            &entity_array_iterator, 
            &ierr);

          /// If iterator returns error then fail and exit
	  CHECK_ERR(ierr, "initPartBdryEntArrIter");

           if(ierr != iBase_SUCCESS)
 		{
		bOK = false; bOK1 = false;
	     	break;
		}
  	   if(iNumEnts == 0)
		{
		if(entity_array_iterator == NULL)
		{
		//fprintf(fp, "\n ERROR: the iterator is not null for zero number of elements \n");
		CHECK_VALIDATION_ERR("iMeshP_initPartBdryEntArrIter");
		bOK = false;bOK1 = false;
		break;
		}
		}
	   iBase_EntityHandle* entity_handles = NULL;
	   int iEntity_handles_size = 0, iEntity_handles_allocated = 0, iHas_data = 0;
	   
           /// Get the next set of entities and check them for ownership.
           iMesh_getNextEntArrIter(instance,
                               entity_array_iterator,
                               &entity_handles,
                               &iEntity_handles_allocated,
                               &iEntity_handles_size,
                               &iHas_data, 
                               &ierr);

	  CHECK_ERR(ierr, "iMesh_getNextEntArrIter");
            if(ierr != iBase_SUCCESS)
		{
		bOK1 = false;
		break;
		}
	   while(iHas_data)
		{
		for(int ii = 0; ii < iEntity_handles_size; ii++ )
		  {
		    iBase_EntityHandle pEntity_handle = entity_handles[ii];
		    
		int iEntStatus;
		/* Now get the entity status w.r.t the given part. The status should be boundary (1 is for boundary) */
                 iMeshP_getEntStatus(
                     instance,
                     pPtnHandles[p],
                     pLocalParts[pr], 
                     pEntity_handle, 
                     &iEntStatus,
                     &ierr); 
		CHECK_ERR(ierr, "iMeshP_getEntStatus");
		/* If the returned entity was not a boundary entity then break */
		if(iEntStatus != 1)
			{
			CHECK_VALIDATION_ERR("iMeshP_initPartBdryArrIterator");
			bOK1 = false;
			break;
			}
		} // end ii for loop
		 iMesh_getNextEntArrIter(instance,
                               entity_array_iterator,
                               &entity_handles,
                               &iEntity_handles_allocated,
                               &iEntity_handles_size,
                               &iHas_data, 
                               &ierr);
		CHECK_ERR(ierr, "iMesh_getNextEntArrIter");
		if(ierr != iBase_SUCCESS)
			{
			bOK1 = false;
			break;
			}
	    }// end while loop
           free(entity_handles);
	   iMesh_endEntArrIter(instance, entity_array_iterator, &ierr);

	   CHECK_ERR(ierr, "iMesh_endEntArrIter");
	   if(ierr != iBase_SUCCESS)
		{
		bOK1 = false;
		break;
		}
	    } // end parts loop 
	  TEST(bOK, 0);
	  TEST(bOK1, 1);
          }// end if condition for legal types and topologies
       }// end topo loop
     }// end type loop
   
  free(pLocalParts);
  }//end partitions loop
free(pPtnHandles);
 }// end function

/***************************************** Entity Ownership Functionality ***************************/

/* Test: iMeshP_getEntOwnerPart

Pre-req: Get boundary entities between parts.

Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getPartNbors, iMeshP_getPartIdFromPartHandle, iMesh_getRootSet.

Test Cases: 
1- Get the boundary entities and get their owning parts. 
2- Check against the local part if it is the owner of the boundary entity. If not, then the owner should be among the neighbors.

According to the iMeshP specification, Part A "neighbors" Part B if Part A has copies of entities owned by Part B
and/or if Part B has copies of entities owned by Part A. Therefore, the owning part should be among the neighbors of the given part A. If not then fail.
*/

void vGetEntOwnerPrtTest(iMesh_Instance instance)
{
 // char* cFuncName = "iMeshP_getEntOwnerPart";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Testing entity owner part test: getEntOwnerPrt ------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions to perform the tests
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;

   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
    
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);

 /// Step 1: Try each type and topology for all local part handles.

      // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the type-topo combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) { 
	  // bOK is test case value for getEntOwnerPart success/failure.
          // bOK1 is test case value for checking if the owner is self or among the neighbor. 
	  bool bOK = true, bOK1 = true;
       for(int pr = 0; pr < iPrtSize; pr++ )
    	{  
     fprintf(fp, "----------------------------------------------------------");	
     fprintf(fp, "\n Testing Type: %s Topology: %s \n", astrTypes[eEType], astrTopologies[eETopo]);

     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;
 
   /// Return boundary entities between the local part and all other parts

    iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            eEType, 
            eETopo, 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntsAllocated,
            &iEntsSize, 
            &ierr); 

    /// If we fail in getting boundary entities, we fail both test cases and break.
   CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");

    if(ierr!=iBase_SUCCESS)
     {
     bOK = false; bOK1 = false;
     break;
     }

   /// Step 2: For every boundary entity, get its owner part. 
   /// If owner is not equal to the current part handle, then the owner should be among the neighbors.

   for(int ii = 0; ii < iEntsSize; ii++ )
    {
    /// Get entity's owning part id.
     iMeshP_Part pPrtId;

     iMeshP_getEntOwnerPart(instance,
			   pPtnHandles[p],
			   pEntities[ii],
			   &pPrtId,
			   &ierr
			   );     
    CHECK_ERR(ierr, "iMeshP_getEntOwnerPart");

    /// bOK1 depends on test case for bOK so we fail both.
     if(ierr != iBase_SUCCESS)
         {
         bOK = false; bOK1 = false;
	 break;
	}


    /// Step 3: Get the unique part id of local part handle.
     iMeshP_Part pPrtIdLoc;

     iMeshP_getPartIdFromPartHandle(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            &pPrtIdLoc,
            &ierr);

    CHECK_ERR(ierr, "getPartIdFromPartHandle");
    if(ierr!=iBase_SUCCESS)
	{
	bOK1 = false; 
	break;
         }
 	
   /// Step 4: Check if the owning part is the same as the current part used in getEntities. 
   /// If not then the owner should be among the neighbors.

     if(pPrtId != pPrtIdLoc)
	{
      /*According to the iMeshP specification, Part A "neighbors" Part B if Part A has copies of entities owned by Part B
and/or if Part B has copies of entities owned by Part A. 
      If part A(current part) has a boundary or ghost entity owned by some other part B then part B must be A's neighbor*/

       /// Step 6: Get neighbors of the current part pLocalParts[pr], the owning part should be one of them.

       
       int iNumPrtNbors, iNborIdsAllocated, iNborIdsSize;
       iMeshP_Part* iNborIds = NULL;

       iMeshP_getPartNbors(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            eEType,
            &iNumPrtNbors,
            &iNborIds,
            &iNborIdsAllocated,
            &iNborIdsSize,
            &ierr);
 
     CHECK_ERR(ierr, "iMeshP_getPartNbors");

	if(ierr!=iBase_SUCCESS)
 	{
	bOK1 = false;
        break;
   	}

     bool bIsThere = false;

   /// Step 7: Loop through neighbor ids and search for the owning part. if not found then fail.
     for(int jj = 0; jj < iNborIdsSize; jj++ )
      {
	if(iNborIds[jj] == pPrtId)
	{
        bIsThere = true;	
	break;
	}
      } // end jj
      if(!bIsThere)
	{
	//fprintf(fp, "\n Owning part id is not among the neighbors. Exiting function. \n");
	CHECK_VALIDATION_ERR("iMeshP_getEntOwnerPart");
	bOK1 = false;
        break;
	}
      free(iNborIds);
	} // end if condition
      
       }  // end ii loop
       free(pEntities);  
	}// end parts loop   
	TEST(bOK, 0); TEST(bOK1, 1);
        } // end if type-topo condition
       } // end topologies loop
      } // end types loop
    free(pLocalParts);
   }// end partitions loop
   free(pPtnHandles);
  }// end function

/* Test: iMeshP_getEntOwnerPartArr

Pre-req: Get the entities w.r.t a given local part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getPartHandleFromPartId

Test Cases: 
1- Get owning parts of given entities. Check if function succeeds.
2- Check the owning parts of the given entities.
   - If the owning parts are the same as Part A then pass.
   - If the owning parts are different, check the entity status w.r.t the given part A. The owning status should be boundary or ghost. In that case, check neighbors like we did in the last test case. If owning part is among neighbors then pass else fail.

*/

void vGetEntOwnerPrtArrTest(iMesh_Instance instance)
{
  // char *cFuncName = "iMeshP_getEntOwnerPartArr";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Testing entity array owner parts: getEntOwnerPrtArr  ------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");
   
   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);
  
      // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) { 
     /// bOK for checking if the function call to getEntOwnerPartArr is successful.
    /// bOK1 for checking if it returns correct owners.
     fprintf(fp, "---------------------------------------------------------------");
     fprintf(fp, "\n Testing Type: %s Topology: %s \n", astrTypes[eEType], astrTopologies[eETopo]);

     bool bOK = true, bOK1 = true;

     for(int pr = 0; pr < iPrtSize; pr++ )
    {  
     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;
 	
     /// Step 1: Get all entities common to the given part and set
     /// According to iMeshP spec, Functions that return entity information for a part, set or mesh 
    ///instance return the information for all entities (including copies and
    /// ghosts) in that part.

     iMeshP_getEntities(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            eEType,
            eETopo,
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);

     CHECK_ERR(ierr, "iMeshP_getEntities"); 

     if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false; 
	break; 
	}

   /* Step 2: For the array of entities, get their owner parts. If it's not equal to the local part handle, then check if the  entity is ghost or boundary w.r.t the local part. If entity has no status w.r.t the part, throw an error. */

     iMeshP_Part* pPrtIds = NULL;
     int iPrtIdAllocated, iPrtIdSize;

     iMeshP_getEntOwnerPartArr(instance,
			   pPtnHandles[p],
			   pEntities,	
			   iEntsSize,
			   &pPrtIds,
			   &iPrtIdAllocated,
			   &iPrtIdSize,
			   &ierr
			   );
     CHECK_ERR(ierr, "iMeshP_getEntOwnerPartArr");

    // If the function throws error then all test cases fail.
    if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break; 
	}

/// Step 3: If the entity owner status array's size is not the same as the input entities array size then fail.
    if(iPrtIdAllocated != iEntsSize)
      {
	CHECK_VALIDATION_ERR("iMeshP_getEntOwnerPartArr");
        bOK = false;
	}
 
  /// Step 4: get the part id of the local part handle.
     iMeshP_Part pPrt;

     iMeshP_getPartIdFromPartHandle(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            &pPrt,
            &ierr);

    CHECK_ERR(ierr, "iMeshP_getPartIdFromPartHandle");

    if(ierr!=iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}

for(int ii = 0; ii < iEntsSize; ii++ )
    {
    
   /// Step 5: Check if the owning part is the same as the current part used in getEntities. 
   /// If not then the entity status should be boundary or ghost w.r.t the current part.
   /// If yes, then the entity status should be internal or boundary w.r.t the current part.

     if(pPrt != pPrtIds[ii])
	{
	//fprintf(fp, "\n Entity not owned by part %d \n", pr);

	/// Now check status of the entity w.r.t the local part.
	/// If it is internal, then fail test.

        int iEntOwner;
        iMeshP_getEntStatus(instance,
			    pPtnHandles[p],
			    pLocalParts[pr],
			    pEntities[ii],
			    &iEntOwner,	
			    &ierr);

	
       CHECK_ERR(ierr, "iMeshP_getEntStatus");

       if(ierr!=iBase_SUCCESS)
	{
        bOK1 = false;
       break;
 	}

        if(iEntOwner == 0)
		{
		fprintf(fp, "\n Validation ERROR in %s: Function called: %s \n", "iMeshP_getEntStatus", __FUNCTION__);
 		bOK1 = false;
		}

      /*According to the iMeshP specification, Part A "neighbors" Part B if Part A has copies of entities owned by Part B
and/or if Part B has copies of entities owned by Part A. 
      If part A(current part) has a boundary or ghost entity owned by some other part B then part B must be A's neighbor*/

       /// Step 6: Get neighbors of the current part pLocalParts[pr], the owning part should be one of them.

       int iNumPrtNbors, iNborIdsAllocated, iNborIdsSize;
       iMeshP_Part* iNborIds = NULL;

       iMeshP_getPartNbors(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            eEType,
            &iNumPrtNbors,
            &iNborIds,
            &iNborIdsAllocated,
            &iNborIdsSize,
            &ierr);

      CHECK_ERR(ierr, "iMeshP_getPartNbors");

	if(ierr!=iBase_SUCCESS)
	{
	bOK1 = false; 
	break;
	}


     bool bIsThere = false;

     for(int jj = 0; jj < iNborIdsSize; jj++ )
      {
       if(iNborIds[jj] == pPrtIds[ii])
        bIsThere = true;
      }
      if(!bIsThere)
	{
	CHECK_VALIDATION_ERR("iMeshP_getEntOwnerPartArr");
	bOK1 = false;
	}
     free(iNborIds);
	} // end if condition
      
       }  // end ii 
      free(pPrtIds);
      free(pEntities);
	}// end parts loop
	TEST(bOK, 0); TEST(bOK1, 1); 
        } // end if condition
       } // end topologies loop
      } // end types loop
    free(pLocalParts);
   }// end partitions loop
    free(pPtnHandles);
}// end function


/*Test: iMeshP_isEntOwnerArr

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getEntStatus

Test Cases: 
1- Test if isEntOwnerArr function call throws error. Check output parameters.
2- Check if the entities are owned by the given part.
  -  If yes, then get the status of the entity. Should be internal or boundary.
  - If no, get the status of the entity. Should be boundary or ghost.

*/

void vIsEntOwnerArrTest(iMesh_Instance instance)
{
 // char *cFuncName = "iMeshP_isEntOwnerArr";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Testing an array of entities is owned by a part or not:isEntOwnerArr  ------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

  bool bOK = true, bOK1 = true;

for(int pr = 0; pr < iPrtSize; pr++ )
    { 
     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;
 	
     /// Step 1: For every part, get all entities common to the given part and set
     iMeshP_getEntities(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);

    CHECK_ERR(ierr, "iMeshP_getEntities");
    
     if(ierr!=iBase_SUCCESS)
	{
        bOK = false; bOK1 = false;
	break;
	} 
   //  fprintf(fp, "\n Number of Entities: %d \n", iEntsSize);

   /* Step 2: For an array of entities again go through the status, call isEntOwner. Once the result is true, get the status of the entity and cross check if the result is valid. */

     iMeshP_Part* pPrtIds = NULL;
     int* iIsOwnerArr = NULL;
     int iIsOwnerAllocated, iIsOwnerSize;

     iMeshP_isEntOwnerArr(instance,
			   pPtnHandles[p],
			   pLocalParts[pr],
			   pEntities,
			   iEntsSize,
			   &iIsOwnerArr,
			   &iIsOwnerAllocated,
			   &iIsOwnerSize,
			   &ierr );
    
    CHECK_ERR(ierr, "iMeshP_isEntOwnerArr");
     if(ierr != iBase_SUCCESS)
	{
	bOK = false;
	bOK1 = false;
	break;
	}
  
/// Step 3: if the returned size of entity status array is not the same as input entity array then fail.
    if(iIsOwnerAllocated != iEntsSize)
      {
	//fprintf(fp, "\n Validation ERROR in %s: Function called: %s \n", "iMeshP_getEntOwnerPartArr", __FUNCTION__);
	//fprintf(fp, "\n Description: Output array size error: I/P Entities size: %d Owner array size: %d \n", iEntsSize, iIsOwnerAllocated);
	CHECK_VALIDATION_ERR("iMeshP_isEntOwnerArr");
	bOK = false; 
	}
/// Step 4: Loop through all entities, check if the entities are owned by given part. 
   for(int ii = 0; ii < iEntsSize; ii++ )
    {
     int iParStatus;
/// Step 5: Check entity status;
/// Internal and owned --- pass.
/// Internal and non-owned --- fail.
/// Boundary and owned --- pass (tentative)
/// Boundary and non-owned --- pass (tentative).
/// Ghost and owned -fail
/// Ghost and non-owned- pass

     if(iIsOwnerArr[ii] == 1)
       {
	iMeshP_getEntStatus(
            instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            pEntities[ii],
            &iParStatus,
            &ierr); 

       if(ierr!=iBase_SUCCESS)
	{
        // fprintf(fp, "\n Entity does not exist in the part : getEntStatus failure . \n");
	CHECK_VALIDATION_ERR("iMeshP_isEntOwnerArr");
	bOK1 = false;
	break;
	}

         /// Ghost entities are not owned by the part.
        if(iParStatus == 2)
           {
	 // fprintf(fp, "\n The owned entity status cannot be ghost. Failed test. Exiting function \n");
	   CHECK_VALIDATION_ERR("iMeshP_isEntOwnerArr");
	   bOK1 = false;
	   break;
		}

         /// Entities owned by a part are either boundary or internal
         if(iParStatus != 0 && iParStatus != 1)
		{
	  //fprintf(fp, "\n The owned entity status is niether internal nor boundary. Failed test. Exiting function \n");
	   CHECK_VALIDATION_ERR("iMeshP_isEntOwnerArr");
	   bOK1 = false;
	   break;
		}
      /// Step 5: If entity ownership status is 1 then we can always get the owner copy & part id.
      iMeshP_Part pOwnerPart;
      iBase_EntityHandle pOwnerEnt;

      iMeshP_getOwnerCopy(instance,
            pPtnHandles[p],
            pEntities[ii],
            &pOwnerPart, 
            &pOwnerEnt, 
            &ierr);

    CHECK_ERR(ierr, "iMeshP_getOwnerCopy");

     if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}
    
     /// Should point to the same space ?
     if(pEntities[ii] != pOwnerEnt)	
	{
	//fprintf(fp, "\n ERROR: Owner entity is not the same as getOwnerCopy. Exiting function. \n");	
	CHECK_VALIDATION_ERR("iMeshP_isEntOwnerArr");
	bOK1 = false;
	break;
	}

    /// Now check if the owner part id is the same.
     
    /// Step 6: get the part id of the local part handle.
     iMeshP_Part pPrt;

     iMeshP_getPartIdFromPartHandle(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            &pPrt,
            &ierr);

    CHECK_ERR(ierr, "iMeshP_getPartIdFromPartHandle");
    if(ierr!=iBase_SUCCESS)
	{
       bOK1 = false;
	break;
	}

    if(pPrt != pOwnerPart)
	{
	//fprintf(fp, "\n ERROR: Owner part is not the same as getOwnerCopy. Exiting function. \n");	
	CHECK_VALIDATION_ERR("iMeshP_isEntOwnerArr");
	bOK1 = false;
	break;
	}
	}// end if condition
      } // end for loop       
    }// end parts loop
   TEST(bOK, 0); 
   TEST(bOK1, 1);
   }// end partitions loop
   free(pPtnHandles);
  }// end function

/*Test: isEntOwner

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getEntStatus

Test Cases:
1- check isEntOwner function and its parameters. 
2- Check the entity ownership status w.r.t the given part. If it is owned, then the entity status w.r.t the given part can be internal or boundary.  

*/

void vIsEntOwnerTest(iMesh_Instance instance)
{
  //char *cFuncName = "iMeshP_isEntOwner";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Testing if a part is entity owner or not: isEntOwner  ------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");
   
  if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);

  /// Loop through all local parts of a partition.
  bool bOK = true, bOK1 = true;

  for(int pr = 0; pr < iPrtSize; pr++ )
    { 
     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;
 	
     /// Step 1: Get all entities common to the given part and set
     iMeshP_getEntities(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);

    CHECK_ERR(ierr, "iMeshP_getEntities");

   if(ierr!=iBase_SUCCESS)
	{
        bOK = false; bOK1 = false;
	break;
	} 
     
    // fprintf(fp, "\n Number of Entities: %d \n", iEntsSize);

   for(int ii = 0; ii < iEntsSize; ii++ )
    {
   /* Step 2: For all entities check their ownership status */

     int iIsOwner;
     iMeshP_isEntOwner(instance,
			   pPtnHandles[p],
			   pLocalParts[pr],
			   pEntities[ii],
			   &iIsOwner,
			   &ierr);

	CHECK_ERR(ierr, "iMeshP_isEntOwner");

     if(ierr != iBase_SUCCESS)
	{
        bOK = false; bOK1 = false;
	break;
	} 
        
   /// Step 3: If the ownership status is true, get the entity status w.r.t the owner part. 
   /// boundary --- owned : pass
   /// boundary --- non-owned : pass
   /// internal -- owned: pass
   /// internal --- non-owned : fail.
   /// ghost--- owned: fail.
   /// ghost --- non-owned: pass.
   /// error (entity does not exist on given part) : fail.
     int iParStatus;
     if(iIsOwner == 1)
       {
	iMeshP_getEntStatus(
            instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            pEntities[ii],
            &iParStatus,
            &ierr); 

	CHECK_ERR(ierr, "iMeshP_getEntStatus");
        if(ierr != iBase_SUCCESS)
	  {
	   //fprintf(fp, "\n ERROR: Entity status is invalid w.r.t the given part \n");
	   CHECK_VALIDATION_ERR("iMeshP_isEntOwner");
           bOK1 = false;
	   break;
	  }
        /// Ghost entities are not owned by the part.
        if(iParStatus == 2)
           {
	  // fprintf(fp, "\n The owned entity status cannot be ghost. Failed test. Exiting function \n");
	   CHECK_VALIDATION_ERR("iMeshP_isEntOwner");
	   bOK1 = false;
	   break;
		}
         /// If it is not internal or boundary, throw an error.
         if(iParStatus != 0 && iParStatus != 1)
		{
	   //fprintf(fp, "\n The entity status is niether internal nor boundary. Failed test. Exiting function \n");
	   CHECK_VALIDATION_ERR("iMeshP_isEntOwner");
	   bOK1 = false;
	   break;
		}

       /// Step 4: If entity ownership status is 1 then we can always get the owner copy & part id.
      iMeshP_Part pOwnerPart;
      iBase_EntityHandle pOwnerEnt;

      iMeshP_getOwnerCopy(instance,
            pPtnHandles[p],
            pEntities[ii],
            &pOwnerPart, 
            &pOwnerEnt, 
            &ierr);

    if(ierr!=iBase_SUCCESS)
	{
	CHECK_ERR(ierr, "iMeshP_getOwnerCopy");
        bOK1 = false;
	break;
	} 
     
    /// Now check if the owner part id is the same.
     
    /// Step 6: get the part id of the local part handle CHECKIT.
     iMeshP_Part pPrt;

     iMeshP_getPartIdFromPartHandle(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            &pPrt,
            &ierr);

    if(ierr!=iBase_SUCCESS)
	{
	CHECK_ERR(ierr, "iMeshP_getPartIdFromPartHandle");
        bOK1 = false;
	break;
	}

    if(pPrt != pOwnerPart)
	{
	//fprintf(fp, "\n ERROR: Owner part is not the same as getOwnerCopy. Exiting function. \n");	
	CHECK_VALIDATION_ERR("iMeshP_isEntOwner");
	bOK1 = false;
	break;
	}
        
	}// end if condition
      } // end ii loop
    }// end parts loop
    TEST(bOK, 0);
   TEST(bOK1, 1);
    free(pLocalParts);
   }// end partitions loop
   free(pPtnHandles);
  }// end function

/*
test: iMeshP_getNumCopies

Pre-req: Get the total number of entities of ALL_TYPES in the partition, Get the number of entities common in the root set and part. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getNumOfTypeAll

Test Cases: 
1- Check the number of copies in the partition.
2- entities are copies on the neighbors. For every entity, do getNumCopies. */

void vGetNumCopies(iMesh_Instance instance)
{
 // char *cFuncName = "iMeshP_getNumCopies";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting number of copies for a given entity: getNumCopies  ------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  {   
  ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

 //  fprintf(fp, "\n Got all Local Parts, Count: %d \n ", iPrtSize);

      // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
     
       for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {  
       bool bOK = true, bOK1 = true;
        for(int pr = 0; pr < iPrtSize; pr++ )
          {    
/// We know that when Part A is a neighbor of Part B when it has copies of entities owned by Part B OR Part B has copies of entities owned by Part A.
     int iNumPrtNbors, iNborIdsAllocated, iNborIdsSize;
     iMeshP_Part* pNborIds = NULL;

    /// Step 1: Get neighbors of the part w.r.t the specific entity type.
    iMeshP_getPartNbors(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            eEType,
            &iNumPrtNbors,
            &pNborIds,
            &iNborIdsAllocated,
            &iNborIdsSize,
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getPartNbors");
    /// We make bOk1 false if we get error in getting neighbors
    if(ierr != iBase_SUCCESS)
	{
	fprintf(fp, "\n Dependency failed: error getting part neighbors \n");
	bOK1 = false;
	}
     int iNumSub = 0;
 
   /// Step 2: Get the part boundary entities
    iBase_EntityHandle* pBdryEntities = NULL;
     int iBdryEntsAllocated, iBdryEntsSize;

    iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            iMeshP_ALL_PARTS, 
            &pBdryEntities,
            &iBdryEntsAllocated,
            &iBdryEntsSize, 
            &ierr); 
    /// we can't continue if we don't get boundary entities.
    CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");
    if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}

   for(int ii = 0; ii < iBdryEntsSize; ii++ )
    {
      int iIsOwner;

      iMeshP_isEntOwner(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            pBdryEntities[ii], 
            &iIsOwner, 
            &ierr); 
     /// Step 3: Check if the given part is the owner of the entity. 
    CHECK_ERR(ierr, "iMeshP_isEntOwner");

    if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
   /// Step 4: If the part is owner of the given entity then get the number of copies of entities on the partition.
   if(iIsOwner == 1)
   {
    /// Maintain our own count of num copies of the entity that is actually shared with another part.
    int iMyNumCopies = 0;
   /* Step 3: For every entity, figure out the number of copies, now get a total of that number*/
     int iNumCopiesEnt;

     iMeshP_getNumCopies(
            instance,
            pPtnHandles[p], 
            pBdryEntities[ii], 
            &iNumCopiesEnt,
            &ierr);
     CHECK_ERR(ierr, "iMeshP_getNumCopies");

   /// Both test cases fail if we don't succeed getting number of copies.
    if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
 
  /* Step 4: Now for the given entity, check if its copy exists on the neighboring part */
  for(int nbor = 0; nbor < iNborIdsSize; nbor++ )
  {
   iBase_EntityHandle pCopyEnt;

   iMeshP_getCopyOnPart(instance,
            pPtnHandles[p], 
            pBdryEntities[ii],
            pNborIds[nbor], 
            &pCopyEnt, 
            &ierr); 

   /// Step 5: If the entity has no status w.r.t the neighboring part, go on with next neighboring part.
   if(ierr != iBase_SUCCESS)
     continue;

   /// Step 6: If getting the copy on part is successful, increment the counter.
        ++iMyNumCopies;

        } // end nbor loop 

  /// Step 7: We have counted the number of copies on parts. If it is not the same as getNumCopies then fail.
  if(iNumCopiesEnt != iMyNumCopies)
    {
    //fprintf(fp, "\n Number of copies of entity %d, number of copies on neighbor %d \n", iNumCopiesEnt, iMyNumCopies);
    //fprintf(fp, "\n Error. Number of copies of entities does not match with our own. Exiting function. \n");
    CHECK_VALIDATION_ERR("iMeshP_getNumCopies");
    bOK1 = false;
    }
     }// end if iIsOwner condition
      } // end ii loop
     free(pNborIds);
     free(pBdryEntities);
       }// end parts loop
     TEST(bOK, 0);TEST(bOK1, 1);
     } // end if condition
    } // end topo loop
   } // end types loop       
   free(pLocalParts);
 } // end partitions loop
   free(pPtnHandles);
}// end function

/*
iMeshP_getCopyParts

Pre-req: Get part boundary entities. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getPartHandleFromPartId, iMeshP_getEntStatus,iMesh_getRootSet

Test Cases: 
1- For all owned part boundary entities, get their copy part ids. The copy part id must be among the neighbors. Cross check if it exists in the neighbors. 
*/

void vGetCopyParts(iMesh_Instance instance)
{
  //char *cFuncName = "iMeshP_getCopyParts";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting copy parts of a given entity: getCopyParts  ------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
     
 for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
      /// Now check if the combination is legal
   if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
     bool bOK = true, bOK1 = true;
    for(int pr = 0; pr < iPrtSize; pr++ )
         { 
	fprintf(fp, "-----------------------------------------------");
	fprintf(fp, "\n Testing: Type %s and Topology %s \n", astrTypes[eEType], astrTopologies[eETopo] );
      /// We know that when Part A is a neighbor of Part B when it has copies of entities owned by Part B OR Part B has copies of entities owned by Part A.
     int iNumPrtNbors, iNborIdsAllocated, iNborIdsSize;
     iMeshP_Part* pNborIds = NULL;

    /// Step 1: Get neighbors of the part w.r.t the specific entity type.
    iMeshP_getPartNbors(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            static_cast<iBase_EntityType>(eEType),
            &iNumPrtNbors,
            &pNborIds,
            &iNborIdsAllocated,
            &iNborIdsSize,
            &ierr); 

    CHECK_ERR(ierr, "iMeshP_getPartNbors");

	if(ierr != iBase_SUCCESS)
	{
	fprintf(fp, "\n ERROR: getting neighboring parts \n");
	bOK1 = false;
	}

         iBase_EntityHandle* pEntities = NULL;
         int iEntsAllocated, iEntsSize;

         /// Step 2: Get all part boundary entities.
         iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            static_cast<iBase_EntityType>(eEType),
            static_cast<iMesh_EntityTopology>(eETopo), 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);
        CHECK_ERR(ierr, "getPartBdryEnts");

      /// if any entity type and topo combination is unsupported, throw a warning.
	if(ierr == iBase_BAD_TYPE_AND_TOPO) 
		{
        fprintf(fp, "\n Warning: Type %s and Topology %s unsupported \n", astrTypes[eEType], astrTopologies[eETopo] );
        continue;
		}
	/// we can't continue if we don't get boundary entities because then will will have nothing to test.
       if(ierr != iBase_SUCCESS)
	  {
	  bOK = false;bOK1 = false;
	  break;
          }
     
    // fprintf(fp, "\n Number of Entities: %d Error Code %d \n", iEntsSize, ierr);

   /// Step 3: For every owned boundary entity, get it's copy parts.
   for(int ii = 0; ii < iEntsSize; ii++ )
    {
     int iIsOwner;
     iMeshP_isEntOwner(instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            pEntities[ii],
            &iIsOwner, 
            &ierr); 
     CHECK_ERR(ierr, "isEntOwner");

    if(ierr != iBase_SUCCESS)
	{
	bOK = false;
	bOK1 = false;
	break;
	}

    if(iIsOwner == 1)
     {
     iMeshP_Part* pPart_ids = NULL;
     int iPrtIdsAllocated, iPrtIdsSize;

     iMeshP_getCopyParts(instance,
            pPtnHandles[p],
            pEntities[ii], 
            &pPart_ids, 
            &iPrtIdsAllocated, 
            &iPrtIdsSize, 
            &ierr);
    CHECK_ERR(ierr, "iMeshP_getCopyParts");
    /// If getCopyParts fail, we fail both test cases.
     if(ierr != iBase_SUCCESS)
	  {
	  bOK = false;bOK1 = false;
	  break;
          }

     /// Step 4: Now check if the returned copy part id is among the neighboring parts. Should be there.
  if(iPrtIdsSize > 0 && iNborIdsSize > 0)
  {
   bool bAllThere = false;

 for(int jj = 0; jj < iPrtIdsSize; jj++ )
   {
    for(int nbor = 0; nbor < iNborIdsSize; nbor++ )
     {
  	if(pNborIds[nbor] == pPart_ids[jj])
 	{
          bAllThere = true;
	  break;
	}
    } // end nbor loop
   if(!bAllThere)
    {
    //fprintf(fp, "\n ERROR: Part having copy of the entity is not among the neighbors! \n");	 
   CHECK_VALIDATION_ERR("iMeshP_getCopyParts");
    bOK1 = false;
    break;
   }
    } //end jj loop 
  } // end if condition 

 } // end if iIsOwner

   } // end ii loop
 free(pNborIds);
 free(pEntities);
  }// end parts loop
  TEST(bOK, 0); TEST(bOK1, 1);
   } // end if condition
  }//end topo loop
 } // end type loop
   free(pLocalParts);
   }// end partitions loop
   free(pPtnHandles);
  }// end function

/*iMeshP_getCopyOnPart

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMeshP_getPartIdFromPartHandle, iMeshP_isEntOwner, iMeshP_getEntStatus

Test Cases: 
1- For every owned boundary entity on part A and some other part call getCopyOnPart on all neighbors. If it fails for all the neighbors then fail test case. 
*/

void vGetCopyOnPartTest(iMesh_Instance instance)
{
 // char *cFuncName = "iMeshP_getCopyOnPart";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting copy of the entity from part: getCopyOnPart------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

 for ( int eEType = iBase_VERTEX;
	eEType < iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
   bool bOK = true;
   fprintf(fp, "----------------------------------------------------");
   fprintf(fp, "\n Type: %s Topology: %s \n", astrTypes[eEType], astrTopologies[eETopo]);

  for(int pr = 0; pr < iPrtSize; pr++ )
    { 
     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;

     /// Step 1: Get all entities common to the given part and set.
    iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            eEType, 
            eETopo, 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntsAllocated,
            &iEntsSize, 
            &ierr); 
    CHECK_ERR(ierr, "getPartBdryEnts");
    if(ierr != iBase_SUCCESS)
	  {
	  bOK = false;
	  break;
          }
     
    /// if partbdryents function fails, we can't continue testing so fail.
    
    // fprintf(fp, "\n Number of Entities: %d \n", iEntsSize);

     /// Step 2: Get neighbors of the given part.
   int iNumnbors, iNborIdSize, iNborIdAlloc;
   iMeshP_Part* pnborPrtIds = NULL;

   iMeshP_getPartNbors(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            eEType,
            &iNumnbors,
            &pnborPrtIds,
            &iNborIdAlloc,
            &iNborIdSize,
            &ierr); 
   CHECK_ERR(ierr, "iMeshP_getPartNbors");

     /// if getPartNbors function fails, we skip to next iteration so fail test case.
   if(ierr != iBase_SUCCESS)
	  {
	  bOK = false;
	  break;
          }
     
   /// Step 3: For every owned boundary entity, get it's copy parts on the neighbors.
   for(int ii = 0; ii < iEntsSize; ii++ )
    {
     int iIsOwner;

     iMeshP_isEntOwner(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            pEntities[ii], 
            &iIsOwner, 
            &ierr); 
    CHECK_ERR(ierr, "iMeshP_isEntOwner");
	  
   if(ierr != iBase_SUCCESS)
	  {
	  bOK = false;
	  break;
          }
     if(iIsOwner == 1)
      {
       int iPrtIdsAllocated, iPrtIdsSize;
       bool bIsThere = false;
    for(int nbor = 0; nbor < iNborIdSize; nbor++)
      {
      iBase_EntityHandle pCopy_Entity;
	
	/// Step 3: This should give a copy of the non-owned entity on the given part.

	iMeshP_getCopyOnPart(
            instance, 
	    pPtnHandles[p],
            pEntities[ii], 
            pnborPrtIds[nbor],
            &pCopy_Entity, 
            &ierr); 

 	if(ierr == iBase_SUCCESS)
	  bIsThere = true;
    } // end nbor loop
     if(!bIsThere)
     {
 	//fprintf(fp, "\n ERROR: Failed getting copy on any of the neighbors. \n");
	CHECK_VALIDATION_ERR("iMeshP_getCopyOnPart");
	bOK = false;
     }
     } // end if condition

    }// end ii loop 
    free(pEntities);
    free(pnborPrtIds);
    }// end parts loop
    TEST(bOK, 0);
     } // end if condition
    } // end topo loop
   } // end types loop
   free(pLocalParts);
   }// end partitions loop
   free(pPtnHandles);
  }// end function


/*
Tests iMeshP_getOwnerCopy

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, getPartIdFromPartHandle, isEntOwner, getEntStatus

Test Cases: 
1- Get the entities ownership status. if it is boundary & owner OR internal then get the owner copy. 
2- Compare the returned entity copy with original one. If different, fail. TO BE REVISED
3- Compare the returned owner part id with what we determined using isEntOwner. if different, fail.
*/

void vGetOwnerCopyTest(iMesh_Instance instance)
{
 //  char *cFuncName = "iMeshP_getOwnerCopy";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting entity handle from the owner part: getOwnerCopy------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

   bool bOK = true, bOK1 = true;
for(int pr = 0; pr < iPrtSize; pr++ )
    { 
     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated = 0, iEntsSize = 0;
 	
     /// Step 1: Get all entities common to the given part and set
     iMeshP_getEntities(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);
     CHECK_ERR(ierr, "iMeshP_getEntities");

    if(ierr != iBase_SUCCESS)
	{
	bOK=false; bOK1 = false;
	break;
	}
   
   /// Step 2: For every entity, check if given part is the owner. If yes
   /// then get the owner entity handle from the part.
   for(int ii = 0; ii < iEntsSize; ii++ )
    {
     iMeshP_Part* pPart_ids = NULL;
     int iPrtIdsAllocated, iPrtIdsSize;

    /// if the status is boundary, check the owner. 
    int iIsOwner;

    iMeshP_isEntOwner(instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            pEntities[ii], 
            &iIsOwner, 
            &ierr); 

    CHECK_ERR(ierr, "iMeshP_isEntOwner");
	
    if(ierr != iBase_SUCCESS)
	{
	bOK=false; bOK1 = false;
	break;
	}

/// Step 3: When the local part handle is owner of the entity then get entity's owner copy.
    if(iIsOwner == 1 )
	{
        iBase_EntityHandle pCopy_Entity;
	iMeshP_Part pPrtId;

        iMeshP_getPartIdFromPartHandle(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            &pPrtId,
            &ierr);
	CHECK_ERR(ierr, "iMeshP_getPartIdFromPartHandle");
    
       if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}
   
	iBase_EntityHandle pOwnerEntHandle;
	iMeshP_Part pOwnerPartId;

	iMeshP_getOwnerCopy(
            instance,
            pPtnHandles[p],
            pEntities[ii], 
            &pOwnerPartId, 
            &pOwnerEntHandle, 
            &ierr); 
       CHECK_ERR(ierr, "iMeshP_getOwnerCopy");
	
    if(ierr != iBase_SUCCESS)
	{
	bOK=false; bOK1 = false;
	break;
	}
   
         if(pOwnerPartId != pPrtId)	
             {
 		//fprintf(fp,"\n ERROR: Owner part id returned is different \n");
	        CHECK_VALIDATION_ERR("iMeshP_getOwnerCopy");
  		bOK1 = false;
		break;
            }
	} // end if iIsOwner 
     free(pPart_ids);
    }// end ii loop
    free(pEntities);
    }// end parts loop
   free(pLocalParts);
   TEST(bOK, 0);
   TEST(bOK1, 1);
   }// end partitions loop
   free(pPtnHandles);
  }// end function

/*
Gets the status (internal, ghost or boundary) of a given entity.

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getEntAdj

Test Cases: 
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

*/

void vGetEntStatus(iMesh_Instance instance)
{
   //char *cFuncName = "iMeshP_getEntStatus";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting entity status w.r.t a part: getEntStatus------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
  
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    //fprintf(fp, "\n PrtSize: %d iNumPrts: %d \n", iPrtSize, iNumPrts);
    iNFailed++;
    return;
   }

   // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_VERTEX;
	eEType <= iBase_REGION; eEType++ ) {
    for ( int eETopo = iMesh_POINT;
	  eETopo < iMesh_ALL_TOPOLOGIES; eETopo++ ) {
     /// Now check if the combination is legal
      if ( a2qLegalTypeAndTopo[eEType][eETopo] ) {
	  bool bOK = true, bOK1 = true;
    fprintf(fp, "-----------------------------------------------");	  
    fprintf(fp, "\n Testing: type %s and topology %s \n", astrTypes[eEType], astrTopologies[eETopo]);

	  for(int pr = 0; pr < iPrtSize; pr++ )
    		{  
       iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;
     /// Step 1: Get all entities common to the given part and set
     iMeshP_getEntities(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            eEType,
            eETopo,
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);

     CHECK_ERR(ierr, "getEntities"); 

     if(ierr!=iBase_SUCCESS)
      {
       bOK = false; bOK1 = false;
       break;
      }
     //fprintf(fp, "\n Number of Entities: %d \n", iEntsSize);

   for(int ii = 0; ii < iEntsSize ; ii++ )
    {
     iMeshP_Part* pPart_ids = NULL;
     int iPrtIdsAllocated, iPrtIdsSize;
     int iParStatus;
   
     // Step 2: For every entity, get status of the entity w.r.t the part
     iMeshP_getEntStatus(instance,
            pPtnHandles[p],
            pLocalParts[pr], 
            pEntities[ii], 
            &iParStatus,
            &ierr); 
     CHECK_ERR(ierr, "iMeshP_getEntStatus");

    /// If entity status is not success then fail first test
    if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}

   // fprintf(fp, "\n Entity status is %s w.r.t given part , Type: %s \n", strEntStatus[iParStatus], astrTypes[eEType]);
    iBase_EntityHandle* pEntityHandles = NULL;
    int iEntAllocated = 0, iEntSize = 0;

   /// Step 3: Get downward adjacency, if eEType is not vertex.
   /// If entity type is a vertex then get upward adjacency.
   if(eEType == iBase_VERTEX)
    iMesh_getEntAdj(instance,
		    pEntities[ii], 
		    eEType + 1,
		    &pEntityHandles,
		    &iEntAllocated,
		    &iEntSize,
		    &ierr);
   else
     iMesh_getEntAdj(instance,
		    pEntities[ii], 
		    eEType - 1,
		    &pEntityHandles,
		    &iEntAllocated,
		    &iEntSize,
		    &ierr);

   CHECK_ERR(ierr, "iMesh_getEntAdj");
	
   if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}

  /// Step 4: Now check the status of adjacent entities w.r.t the given part.
   for(int adj = 0; adj < iEntSize; adj++ )
    {
        int iAdjStatus;

	iMeshP_getEntStatus(instance,
			    pPtnHandles[p],
			    pLocalParts[pr],
			    pEntityHandles[adj],
                            &iAdjStatus,
                            &ierr);
      
      /// If an error is thrown to get the status of adjacent entity w.r.t the part then fail third test. 
	CHECK_ERR(ierr, "getEntStatus");
 	if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}

      //fprintf(fp, "\n Neighboring entity status is %s \n", strEntStatus[iAdjStatus]);
      /// Also check the iAdjStatus, if it's not internal, ghost or boundary then fail third test.

     /// Special Case for a vertex
    if(eEType == iBase_VERTEX)
    {
     /// Case 1: Vertex: Internal Edge: Internal.

     if(iParStatus == 0 && iAdjStatus != 0)
	{
	//fprintf(fp, "\n Error: Wrong entity status: Vertex: Internal, Edge : Not internal \n");
	CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
	bOK1 = false;
         }
   /// Case 2: Vertex: Boundary Edge: Ghost Allowed? CHECKIT
    if(iParStatus == 1 && iAdjStatus == 2)
    {
	//fprintf(fp, "\n Error: Wrong entity status, Vertex: Boundary Edge: Ghost \n");
        CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
	bOK1 = false;
         }
   
    }
   
    else if(eEType == iBase_EDGE)
   {
    /// Case 3: Edge: Boundary, Vertex: Not Boundary
     if(iParStatus == 1 && iAdjStatus != 1)
	{
	//fprintf(fp, "\n Error: Wrong downward adjacency, Vertex: Boundary Edge: Not Boundary \n");
        CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
	bOK1 = false;
         }
 
   /// Case 4: Edge: Ghost Vertex: Not ghost
     if(iParStatus == 2 && iAdjStatus != 2)
	{
	//fprintf(fp, "\n Error: Wrong downward adjacency, Vertex: Ghost Edge: Not Ghost \n");
       CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
	bOK1 = false;
         }
   }
   else if(eEType == iBase_FACE)
   {
     /// Case 5: Face: Boundary Edge: Not Boundary
     if(iParStatus == 1 && iAdjStatus != 1)
	{
	//fprintf(fp, "\n Error: Wrong downward adjacency, Face: Boundary Edge: Not Boundary \n");
        CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
	bOK1 = false;
         }
   }
  else if(eEType == iBase_REGION)
   {
    /// Case 6: Region: Ghost Face: Not ghost
     if(iParStatus == 2 && iAdjStatus != 2)
	{
	//fprintf(fp, "\n Error: Wrong downward adjacency, Region: Ghost Face: Not Ghost \n");
	CHECK_VALIDATION_ERR("iMeshP_getEntStatus");
	bOK1 = false;
         }
   }
   }//end adj loop
   free(pPart_ids);
   free(pEntityHandles);
   } // end ii for loop
   free(pEntities);
      } // end parts loop
   TEST(bOK, 0); 
   TEST(bOK1, 1); 
   
   } // end if condition
  }// end types loop
  }// end topo loop
  free(pLocalParts);
 }// end partitions loop
  free(pPtnHandles);
}// end function call.



/*
Test iMeshP_getCopies

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases:
1- Get copies of the given entities w.r.t the part. Should succeed.
2- If an invalid status comes up then fail. 
*/

void vGetEntCopies(iMesh_Instance instance)
{
  // char *cFuncName = "iMeshP_getCopies";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting entity copies: getEntCopies------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Perform tests on all partitions.
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local on-process part handles in the partition.
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

     bool bOK = true, bOK1 = true;
   /// Loop through the local part handles.
   for(int pr = 0; pr < iPrtSize; pr++ )
    { 
     iBase_EntityHandle* pEntities = NULL;
     int iEntsAllocated, iEntsSize;
 	
     /// Step 1: Get all entities common to both the given on-process part and entity set.
     iMeshP_getEntities(
            instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEntities,
            &iEntsAllocated,
            &iEntsSize,
            &ierr);
   CHECK_ERR(ierr, "iMeshP_getEntities");
   if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	} 
   // fprintf(fp, "\n Number of Entities: %d \n", iEntsSize);
 
   for(int ii = 0; ii < iEntsSize ; ii++ )
    {
    /// Step 2: For every entity, first check its ownership w.r.t the part then get its number of copies on the partition
     int iIsOwner;

     iMeshP_isEntOwner(instance,
            pPtnHandles[p], 
            pLocalParts[pr], 
            pEntities[ii], 
            &iIsOwner, 
            &ierr); 

    CHECK_ERR(ierr, "iMeshP_isEntOwner");
	
   if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
     /// Step 3: Check the status of the entity. If it is ghost w.r.t the given part. It should return 2 copies only.

   int iEntStatus;
   iMeshP_getEntStatus(instance,
            pPtnHandles[p], 
            pLocalParts[pr], 
            pEntities[ii], 
            &iEntStatus,
            &ierr);
   CHECK_ERR(ierr, "iMeshP_getEntStatus");

  if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
         /// For part ids having the remote copy
       iMeshP_Part* pPartCopies = NULL;
       int iPrtCopyIdsAll = 0, iPrtCopyIdsSize = 0;    
    
      /// Step 4: Get entity copies on the part ids.
       iBase_EntityHandle* pEntCopies = NULL;
      int iCopiesAlloc = 0, iCopiesSize = 0;

// We want to test for ghost entities and owned ones only.
if(iIsOwner == 1 || iEntStatus == 2)
   {
     iMeshP_Part* pPart_ids = NULL;
    
     int iNumCopies;

     iMeshP_getNumCopies(instance,
            pPtnHandles[p], 
            pEntities[ii], 
            &iNumCopies,
            &ierr); 

    CHECK_ERR(ierr, "iMeshP_getNumCopies");
	
     if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
 if(iNumCopies > 0) /// CHECKIT: conflicting views: Is it 0 or 1?
    {
     iMeshP_getCopies(instance,
            pPtnHandles[p], 
            pEntities[ii], 
            &pPartCopies, 
            &iPrtCopyIdsAll, 
            &iPrtCopyIdsSize, 
            &pEntCopies, 
            &iCopiesAlloc, 
            &iCopiesSize,
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getCopies");
	
   if(ierr != iBase_SUCCESS)
	{
	bOK = false; 
	break;
	}
 /// Check: Number of copies returned by getCopies and getNumCopies must match.
   if(iCopiesAlloc != iNumCopies)
    {
    //fprintf(fp, "\n Number of copies returned do not match with getNumCopies \n");
    CHECK_VALIDATION_ERR("iMeshP_getCopies");
    bOK = false;
   }
 } 
  
  /// if the entity is ghost, the number of copies should be 2.
   if(iEntStatus == 2)
    {   

     iMeshP_getCopies(instance,
            pPtnHandles[p], 
            pEntities[ii], 
            &pPartCopies, 
            &iPrtCopyIdsAll, 
            &iPrtCopyIdsSize, 
            &pEntCopies, 
            &iCopiesAlloc, 
            &iCopiesSize,
            &ierr); 
    CHECK_ERR(ierr, "getCopies");

   if(ierr != iBase_SUCCESS)
	{
	bOK = false;
	break;
	}    
  if(iCopiesSize != 2)
     {
    //fprintf(fp, "\n Number of copies should be 2 for a ghost entity \n");
    CHECK_VALIDATION_ERR("iMeshP_getCopies");
    bOK1 = false;
    break;
   }
  free(pPart_ids);
   } 

   /// Step 5: Check if the part ids returned by getCopies are among the neighbors

    /// We know that when Part A is a neighbor of Part B when it has copies of entities owned by Part B OR Part B has copies of entities owned by Part A.
     int iNumPrtNbors, iNborIdsAllocated, iNborIdsSize;
     iMeshP_Part* pNborIds = NULL;

    ///  Get neighbors of the part w.r.t the specific entity type.
    iMeshP_getPartNbors(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            iBase_ALL_TYPES,
            &iNumPrtNbors,
            &pNborIds,
            &iNborIdsAllocated,
            &iNborIdsSize,
            &ierr); 

  bool bIsThere = false;
    for(int cp = 0; cp < iCopiesSize; cp++)
    {
   for(int cp2 = 0; cp2 < iNborIdsSize; cp2++ )
    {
    if(pNborIds[cp2] == pPartCopies[cp])
 	{
        bIsThere = true;
	break;
	}
   if(!bIsThere) 
	{
	//fprintf(fp, "\n Part having entity copy is not among the neighbors \n");
	CHECK_VALIDATION_ERR("iMeshP_getCopies");
	bOK1 = false;
	}
    } // end cp2 loop
    } // end cp loop
   } // end if check
   free(pPartCopies);
   } // end ii for loop
  free(pEntities);
  } // end pr loop
  TEST(bOK, 0);
  TEST(bOK1, 1);
   
  free(pLocalParts);
 }// end partitions loop
  free(pPtnHandles);
}// end function call.


/* test: getEntStatusArr

Pre-req: Get Boundary entities w.r.t the given local part and other parts. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions

Test Cases:
1- Get part boundary entities of a part w.r.t all other parts. Check the entity status of the part boundary entities. If status is not 1 then fail.
2- For every part, get entities common to the part and root set. Get status of entities w.r.t the part. For boundary entities, compare them with step 1. If all do not match with step 1 then fail. Sizes of both arrays of boundary entities in Step1 and 2 should be same.
*/

void vGetEntStatusArr(iMesh_Instance instance)
{
 // char *cFuncName = "iMeshP_getEntStatusArr";
 /// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Getting entity array status : getEntStatusArr------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
  
   ///  Get all local on-process part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   if(iPrtSize != iNumPrts)
   {
    CHECK_VALIDATION_ERR("iMeshP_getLocalParts");
    iNFailed++;
    return;
   }

  /// Test 1: First get part boundary entities and check their status
  bool bOK = true, bOK1 = true;

for(int pr = 0; pr < iPrtSize; pr++ )
    { 
   iBase_EntityHandle* pEnts = NULL;
   int iEntAllocated, iEntSize;

  /// Step 1: Get part boundary entities between the given part and all other parts.

   iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            iBase_ALL_TYPES, 
            iMesh_ALL_TOPOLOGIES, 
            iMeshP_ALL_PARTS, 
            &pEnts,
            &iEntAllocated,
            &iEntSize, 
            &ierr);

   CHECK_ERR(ierr, "getPartBdryEnts");
	
  /// we can't continue if we don't get the boundary entities so fail.
   if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
 
  int* iParStatusArr = NULL;
  int iParStatusSize, iParStatusAlloc;

  /// Step 2: Check the status of the array of part boundary entities.

  iMeshP_getEntStatusArr(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            pEnts, 
            iEntSize,
            &iParStatusArr,
            &iParStatusAlloc, 
            &iParStatusSize, 
            &ierr); 

  CHECK_ERR(ierr, "getEntStatusArr");
  
  if(ierr != iBase_SUCCESS)
   {
   bOK = false; bOK1 = false;
   break;
   }

 /// Step 3: If the output array size of getEntStatusArr is not the same as the input size of entities then fail.
 /// CHECK IT: Why partStatusALloc? Why not ParStatusSize?
  if(iParStatusAlloc != iEntSize)
  {
   //fprintf(fp, "\n Size mismatch error. Input array of boundary entity does not match output array of ent status \n");
   CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
   bOK = false;
   }

 /// Step 4: Check status of the entities, if it is not 1 (boundary) then fail.
  for(int ent = 0; ent < iParStatusAlloc; ent++ )
   {
   if(iParStatusArr[ent] != 1)
    {
    //fprintf(fp, "\n Functionality error: getEntStatusArr gives wrong entity status for boundary entities \n");
    CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
    bOK1 = false;
   }
  }
/// Test 2: Now get the entities common to the part and set, check if the boundary entities match for both
     iBase_EntityHandle* pEntities = NULL;
     int iEntityAllocated, iEntitySize, iParEntSize, iParEntAlloc;
     int* iParStatusEnt = NULL;
 	
     /// Step 1: Get all entities both in the given part and set
     iMeshP_getEntities(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEntities,
            &iEntityAllocated,
            &iEntitySize,
            &ierr);

     CHECK_ERR(ierr, "getEntities");
	
    if(ierr != iBase_SUCCESS)
	{
	bOK1 = false; 
	break;
	}
   /// Step 2: Get the array of status for all entities (boundary, internal or ghost)
     iMeshP_getEntStatusArr(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            pEntities, 
            iEntitySize,
            &iParStatusEnt,
            &iParEntAlloc, 
            &iParEntSize, 
            &ierr); 

    CHECK_ERR(ierr, "getEntStatusArr");
	
    if(ierr != iBase_SUCCESS)
	{
	bOK = false; 
	break;
	}

   /// Step 3: If o/p array size not the same as input then fail.
   /// CHECK IT: Why do we use iPartEntAlloc instead of iParEntSize?
    if(iParEntAlloc != iEntityAllocated)
     {
     //fprintf(fp, "\n Size mismatch error. Input array of root set- entity does not match output array of ent status \n");
     CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
     bOK = false;
     }
   /// Step 4: Extract boundary entities and compare them with the previous array.
  int iBdryEntSize = 0;

  for(int ent = 0; ent < iParEntAlloc; ent++ )
   {
   if(iParStatusEnt[ent] == 1)
    {
   ++iBdryEntSize;
    bool match = false;
    /// Compare with the previous array of boundary ents
     for(int ent2 = 0; ent2 < iParStatusAlloc; ent2++ )
         {
 	   if(pEnts[ent2] == pEntities[ent] )
              {
                match = true;
                break;
	      }
         }
    if(!match)
     {
    //fprintf(fp, "\n Boundary entity given by Test 1 and Test 2 do not match. Exiting. \n");
    CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
    bOK1 = false;
    break;
     }
   }
  } // end ent loop
   if(iBdryEntSize != iEntAllocated)
    {
    //fprintf(fp, "\n Number of boundary entities in step 1 and 2 do not match. Exiting function.\n");
    CHECK_VALIDATION_ERR("iMeshP_getEntStatusArr");
    bOK = false;
    }
   free(pEntities);
   free(pEnts);
   free(iParStatusArr);
   }// end parts loops
  TEST(bOK, 0);
  TEST(bOK1, 1);

  free(pLocalParts);
  } // end partitions loop
  free(pPtnHandles);

}

//////////////////////////////////////////////GHOSTING/////////////////////////////////////////
/*
Adds and removes entity ghosts to part.

Pre-req: Get the boundary entities of the part w.r.t all other parts. 

Dependencies: iMeshP_getPartBdryEnts, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getRootSet, iMeshP_getEntStatusArr, iMeshP_getPartNbors, iMeshP_getCopyOnPart, iMeshP_getNumCopies

Test Cases:
1- a- Get vertices in a part then get all parts having copies of that vertex. 
   b- For every vertex, get the upward adjacent entities.Get the number of copies of the adjacent entities in the partition. Check if the copy of the adjacent entity exists on the copy parts of the vertices. If not, add ghost there.
  c- Count the ghosts being added on other parts.
  d- Now for each adjacent entity, get the number of copies in the partition again. Check if it has increased by the number of ghosts added. If not, we fail the validation test.
2- Now remove the ghosts of adjacent entities from the parts. 

*/

/*void vAddRmvGhostTest(iMesh_Instance instance)
{
/// Get all the partitions
  char *cFuncName = "iMeshP_addGhostOf, iMeshP_rmvGhostOf";
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Adding/Removing ghost to a part : addGhost, rmvGhost------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

  bool bOK = true, bOK1 = true, bOK2 = true;

  for(int pr = 0; pr < iPrtSize; pr++ )
    {     
    int iEntityAllocated, iEntitySize;
    iBase_EntityHandle* pEntities = NULL;

     /// Step 1: Get all entities of type vertex & point on the part boundary.
    iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            iBase_VERTEX, 
            iMesh_POINT, 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntityAllocated,
            &iEntitySize, 
            &ierr); 

   /// if failure occurs, we cannot continue the test so exit. 
    CHECK_ERR(ierr, "iMeshP_getPartBdryEnts");
     
    if(ierr!=iBase_SUCCESS)
     {
     bOK = false; bOK1 = false;bOK2 = false;
     break;
     }
  for(int ent = 0; ent < iEntitySize; ent++ )
   {
 /// Step 2: Get the parts having copy of the given vertex
  iMeshP_Part* pPrtIds = NULL;
  int iCopyPrtsSize, iCopyPrtsAllocated;

   iMeshP_getCopyParts(instance,
            pPtnHandles[p], 
            pEntities[ent],
            &pPrtIds, 
            &iCopyPrtsAllocated, 
            &iCopyPrtsSize, 
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getCopyParts");

   if(ierr!=iBase_SUCCESS)
     {
     bOK = false; bOK1 = false;bOK2 = false;
     break;
     }

 /// Step 3: Get all entities adjacent to the part boundary vertices. 

 for(int eEType = iBase_EDGE; eEType < iBase_REGION; eEType++ )
   { 
 /// Now get adjacencies of the vertex
   iBase_EntityHandle* pAdjEnts = NULL;
   int iAdjAllocated, iAdjSize;
 
   iMesh_getEntAdj(instance,
                   pEntities[ent],
                   eEType,
                   &pAdjEnts,
                   &iAdjAllocated,
                   &iAdjSize,
                   &ierr);

  CHECK_ERR(ierr, "iMeshP_getEntAdj");


   if(ierr!=iBase_SUCCESS)
     {
     bOK = false; bOK1 = false;bOK2 = false;
     break;
     }

 for(int adj = 0; adj < iAdjSize; adj++ )
   {
     /// Step 4: First get total number of copies of entities before adding ghost.
    int iNumCopiesEnt;

   /// Set for containing the parts on which entities have been ghosted
   std::set<iMeshP_Part> copyPrtsSet;

    iMeshP_getNumCopies(instance,
            pPtnHandles[p], 
            pAdjEnts[adj], 
            &iNumCopiesEnt,
            &ierr); 

   CHECK_ERR(ierr, "iMeshP_getNumCopies");
     
    if(ierr!=iBase_SUCCESS)
     {
     bOK1 = false;
     break;
     }

   /// Make ghost copies of entities owned by local part only.
    int iIsOwner;

    iMeshP_isEntOwner(instance,
            pPtnHandles[p], 
            pLocalParts[pr], 
            pAdjEnts[adj],
            &iIsOwner, 
            &ierr);

   CHECK_ERR(ierr, "iMeshP_isEntOwner");

   if(ierr != iBase_SUCCESS)
	   continue;

 if(iIsOwner == 1)
  {

 /// Get all part ids having copies of the entity
    iMeshP_Part* pCopyIds = NULL;
    int iPrtIdsAlloc, iPrtIdsSize;

    iMeshP_getCopyParts(instance,
            pPtnHandles[p], 
            pAdjEnts[adj],  
            &pCopyIds, 
            &iPrtIdsAlloc, 
            &iPrtIdsSize, 
            &ierr); 

 int iCopies = 0;
 
/// Now find the part ids having copies of vertices but no copy of adjacent entities.
/// Place such part ids in a set
 
 for(int cp = 0; cp < iCopyPrtsSize; cp++)
  {
   bool bNotThere = true;
  // fprintf(fp, "\n Part Id for vertex: %d \n", pPrtIds[cp]);
   for(int acp = 0; acp < iPrtIdsSize; acp++ )
	{
    //	fprintf(fp, "\n Part Id for adjacent entities: %d \n", pCopyIds[acp]);
	if(pCopyIds[acp] == pPrtIds[cp])
	  {
	   bNotThere = false;
	   break;
	  }
	}
   if(bNotThere)
    {
    // fprintf(fp, "\n Inserting: %d \n", pPrtIds[cp]);
     copyPrtsSet.insert(pPrtIds[cp]); 
    }
  }
/// Step 5: Now we can ghost the entity on the part ids in the set.
  std::set<iMeshP_Part>::iterator it; 

  for( it = copyPrtsSet.begin(); it!= copyPrtsSet.end(); it++) {   

   //fprintf(fp, "\n Part id for ghost: %d \n", (iMeshP_Part)(*it));
   /// Now add the ghost.
    iMeshP_RequestHandle rq;

    iMeshP_addGhostOf(instance, 
            pPtnHandles[p], 
            (iMeshP_Part)(*it),
            pAdjEnts[adj], 
            &rq,
            &ierr);

   CHECK_ERR(ierr, "iMeshP_addGhostOf");
   if(ierr != iBase_SUCCESS)
    {
    bOK = false; bOK1 = false; bOK2 = false;
    break;
    }

   /// Wait for the ghost to be added

   iMeshP_waitForRequest(instance,
            pPtnHandles[p],
            rq,
            &ierr);

  CHECK_ERR(ierr, "iMeshP_waitForRequest");
  if(ierr != iBase_SUCCESS)
    {
    bOK = false; bOK1 = false; bOK2 = false;
    break;
    }

   ++iCopies;
   
   } // end it

/// If number of copies has increased check iCopies
 if(iCopies > 0)
   {
     int iNumCopiesEntAftr;

    iMeshP_getNumCopies(instance,
            pPtnHandles[p], 
            pAdjEnts[adj], 
            &iNumCopiesEntAftr,
            &ierr); 
 
   CHECK_ERR(ierr, "iMeshP_getNumCopies");
   if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}

   if(iNumCopiesEntAftr != (iNumCopiesEnt + iCopies))
    {
   //fprintf(fp, "\n Number of ghost entities added is not correct \n ");
   CHECK_VALIDATION_ERR("iMeshP_AddGhostOf, iMeshP_rmvGhostOf");
   bOK1 = false;
   break;
   }
/// Step 6: Once verified that the ghosts have been added, we need to remove them.
  std::set<iMeshP_Part>::iterator it; 

  for( it = copyPrtsSet.begin(); it!= copyPrtsSet.end(); it++) {

  iMeshP_rmvGhostOf(instance, 
            pPtnHandles[p],
            (iMeshP_Part)(*it),
            pAdjEnts[adj],
            &ierr);

  CHECK_ERR(ierr, "iMeshP_rmvGhostOf");

  if(ierr != iBase_SUCCESS)
   {
   bOK2 = false;
   break;
   } 
     } // end it
    int iNumCopiesAftrRmv;

    iMeshP_getNumCopies(instance,
            pPtnHandles[p], 
            pAdjEnts[adj], 
            &iNumCopiesAftrRmv,
            &ierr);

  if(iNumCopiesAftrRmv != iNumCopiesEnt)
	{
	//fprintf(fp, "\n ERROR: # of copies after removing all ghosts != # of copies before removing ghosts \n");
        CHECK_VALIDATION_ERR("iMeshP_AddGhostOf, iMeshP_rmvGhostOf");
	bOK1 = false;
	break;
	}
   } // end if icopies

    } // end if iIsOwner
   } // end adj 
   free(pAdjEnts);
   } // end eEType 
   free(pPrtIds);
   } // end ent

    free(pEntities);
   } // end pr loop.
   TEST(bOK,cFuncName); TEST(bOK1,cFuncName); TEST(bOK2,cFuncName);
   free(pLocalParts);
  } // end partitions loop.

free(pPtnHandles);
} */

/*
iMeshP_deleteGhostEntsAll

Get the entities w.r.t a given part A and set using getEntities. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getRootSet, iMeshP_getEntStatusArr

Test Cases:
1- Remove all ghosts in the partitions, then check if any entity of status 'ghost' exists.
*/
void vDeleteGhostEntsAll(iMesh_Instance instance)
{
  //char *cFuncName = "iMeshP_deleteGhostEntsAll";
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- Removing ghost between two part : deleteGhostEntsAll------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");

/// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);
   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");

   bool bOK = true, bOK1 = true;

    /// Step 1: Delete all ghost entities between parts 
   iMeshP_deleteGhostEntsAll(instance,
            pPtnHandles[p],
            &ierr);

   CHECK_ERR(ierr, "iMeshP_deleteGhostEntsAll");
   if(ierr != iBase_SUCCESS)
	{
	bOK = false; bOK1 = false;
	break;
	}
   /// Step 2: Get all entities for all parts and check if any of them is of ghost type.
  int iEntityAllocated, iEntitySize;
    iBase_EntityHandle* pEntities = NULL;

     /// Get all entities common to the given part and set
for(int pr = 0; pr < iPrtSize; pr++ )
    { 
     iMeshP_getEntities(instance,
            pPtnHandles[p],
            pLocalParts[pr],
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEntities,
            &iEntityAllocated,
            &iEntitySize,
            &ierr);

    CHECK_ERR(ierr, "iMeshP_getEntities");
        
	if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}
    /// Get the status of the above entities

   int* iParStatus = NULL;
    int iParAlloc, iParSize;

    iMeshP_getEntStatusArr(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            pEntities, 
            iEntitySize, 
            &iParStatus,
            &iParAlloc, 
            &iParSize, 
            &ierr); 

  CHECK_ERR(ierr, "iMeshP_getEntStatusArr");

   if(ierr != iBase_SUCCESS)
	{
	bOK1 = false;
	break;
	}

   /// Step 3: Loop through all entities and check their status. if status is ghost then deleteGhostAll did'nt work so fail.
   for(int ii = 0; ii < iParAlloc; ii++)
    {
     if(iParStatus[ii] == 2)
      {
     // fprintf(fp, "\n Ghost entity found after deletion. Error! \n");
      CHECK_VALIDATION_ERR("iMeshP_deleteGhostEntsAll");
      bOK1 = false;
      break;
      }
    }
   free(iParStatus);
    } // end parts loop
  TEST(bOK, 0);
  TEST(bOK1, 1);
  free(pEntities);
  free(pLocalParts);
  } /// end partitions loop
  free(pPtnHandles);
}

/*
create ghosts in the partition.

Pre-req: get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getRootSet, iMeshP_deleteGhostEntsAll, iMeshP_isEntOwner, iMeshP_getEntStatusArr, iMeshP_getPartNbors, iMeshP_getPartHandleFromPartId

Test Cases:
1- Remove all ghosts in the partition. For each part, 
 */
/* 
void vCreateGhostEntsAll(iMesh_Instance instance)
{
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0, iIncludeCopies;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);
  CHECK_ERR_CRITICAL(ierr);

  fprintf(fp, "\n \n ------------- Creating ghosts in partition : createGhostEntsAll------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);
  if(ierr!=iBase_SUCCESS)
     fprintf(fp, "\n Dependency failed: iMeshP_getRootSet \n");

  CHECK_ERR_CRITICAL(ierr);
  
  /// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
   ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;

    // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_REGION;
	eEType <= iBase_REGION; eEType++ ) {
       for(int iBridge = iBase_VERTEX; 
          iBridge <= iBase_VERTEX; iBridge++ ) 
        { 
     //  cout << "\n Combination type " << astrTypes[eEType] << " Bridge: " << astrTypes[iBridge];
     /// Step 1: Delete all ghost entities in the partition
      iMeshP_deleteGhostEntsAll(instance,
            pPtnHandles[p],
            &ierr);
       CHECK_ERR(ierr);
      
   /// Step 2: Get the number of ghosts to be created. 

   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   if(ierr!=iBase_SUCCESS)
     fprintf(fp, "\n Dependency failed: iMeshP_getLocalParts \n");

   CHECK_ERR_CRITICAL(ierr);

   typedef multimap<iBase_EntityHandle, iMeshP_Part> prtMap;
   prtMap ghostContainer;

   int iCopies;
   for(int pr = 0; pr < iPrtSize; pr++ )
    { 
    int iEntityAllocated, iEntitySize;
    iBase_EntityHandle* pEntities = NULL;

     /// Step 3-1 : Get all entities on the part boundary.
    iMeshP_getPartBdryEnts(instance,
            pPtnHandles[p], 
            pLocalParts[pr],
            iBridge, 
            iMesh_POINT, 
            iMeshP_ALL_PARTS, 
            &pEntities,
            &iEntityAllocated,
            &iEntitySize, 
            &ierr); 

   /// if failure occurs, we cannot continue the test so exit. 
    if(ierr!=iBase_SUCCESS)
     fprintf(fp, "\n Failure in getting entities common to part & set. Exiting function \n");

    CHECK_ERR_CRITICAL(ierr);
   /// Step 2: Get all entities adjacent to the part boundary vertices. 

  for(int ent = 0; ent < iEntitySize; ent++ )
   {
 /// Step 3: Get the parts having copy of the given vertex
  int iIsOwnerVtx;

   iMeshP_isEntOwner(instance,
            pPtnHandles[p], 
            pLocalParts[pr], 
            pEntities[ent], 
            &iIsOwnerVtx, 
            &ierr); 

 if(iIsOwnerVtx == 1)
  {
  iMeshP_Part* pPrtIds = NULL;
  int iCopyPrtsSize, iCopyPrtsAllocated;

   iMeshP_getCopyParts(instance,
            pPtnHandles[p], 
            pEntities[ent],
            &pPrtIds, 
            &iCopyPrtsAllocated, 
            &iCopyPrtsSize, 
            &ierr); 
   CHECK_ERR_CRITICAL(ierr);

   /// Now get adjacencies of the vertex
   iBase_EntityHandle* pAdjEnts = NULL;
   int iAdjAllocated, iAdjSize;
 
   iMesh_getEntAdj(instance,
                   pEntities[ent],
                   eEType,
                   &pAdjEnts,
                   &iAdjAllocated,
                   &iAdjSize,
                   &ierr);
  fprintf(fp, "\n Adjacent entities %d \n", iAdjSize);
  CHECK_ERR(ierr);
  /// For each adjacent entity of eEType, check if its copy exists on the copy parts of the vertices.
  for(int adj = 0; adj < iAdjSize; adj++ )
   {

 for(int cp = 0; cp < iCopyPrtsSize; cp++)
  {
/// Step 5: Now check if copy of the face already exists on the copy parts of vertices. If not, we can ghost the entity.
   /// First check for ownership; if the part owns the face/entity then get its copy on other parts.
   iBase_EntityHandle pEntHandle = NULL;

   iMeshP_getCopyOnPart(instance,
            pPtnHandles[p], 
            pAdjEnts[adj],
            pPrtIds[cp], 
            &pEntHandle, 
            &ierr);

  /// If the face does not exist on the copy part of vertex then we can ghost it there. 
  if(ierr != iBase_SUCCESS)
  {
   fprintf(fp, "\n Incrementing counter \n");
   ++iCopies;
  // ghostContainer.insert(prtMap::value_type(pEntities[ent],pPrtIds[cp]));
   }   
  } // end cp loop 
   } // end adj loop
  free(pAdjEnts);
   } // end if condition
   } // end ent loop
 
     } /// end parts loop
fprintf(fp, "\n Ghost entities created with type are %d \n",iCopies);

 /// Now create the ghost entities
   iMeshP_createGhostEntsAll(instance,
            pPtnHandles[p], 
            eEType,
            iBridge,
            iNumLayers,
            0,
            &ierr);
/// Now loop through all parts and check ghost entities; for every ghost entity, find the respective element in multimap

 int iGhostCopies;

   for(int pr = 0; pr < iPrtSize; pr++ )
    { 
    int iEntAlloc, iEntSize;
    iBase_EntityHandle* pEnts = NULL;
    
    iMeshP_getEntities(instance,
            pPtnHandles[p], 
            pLocalParts[pr], 
            root,
            iBase_ALL_TYPES,
            iMesh_ALL_TOPOLOGIES,
            &pEnts,
            &iEntAlloc,
            &iEntSize,
            &ierr);
   
   CHECK_ERR_CRITICAL(ierr);
   
  for(int st = 0; st < iEntSize; st++)
   {
   /// Check status of entity w.r.t the part; if it is ghost, find it in the multimap
    int iEntStatus;
    iMeshP_getEntStatus(instance,
                     pPtnHandles[p],
                     pLocalParts[pr], 
                     pEnts[st], 
                     &iEntStatus,
                     &ierr); 

   /// Find in the multimap
   if(iEntStatus == 2)
   ++iGhostCopies;
   } // end st loop
    } // end parts loop 
    //fprintf(fp, "\n Ghost entities created with type are %d \n",iGhostCopies);
   } // end bridge loop
  } // end types loop
   } // end partitions loop 
  free(pPtnHandles);
 } // end function loop
*/

/* Test: getGhostInfo
Get the ghost information in the partition.

Pre-req: Get the entities w.r.t a given part A and set. 

Dependencies: iMeshP_getEntities, iMeshP_getLocalParts, iMeshP_getPartitions, iMesh_getRootSet, iMeshP_deleteGhostEntsAll, iMeshP_createGhostEntsAll

Test Cases:
1- For every partition, 
  a- Delete all ghosting rules. 
  b- For each entity type, bridge, include-copy combination, create ghost entities and check if the getGhostInfo function returns the newly created ghosting entity type, bridge, include copy information. 
  c-If for any combination it does not return correct information then fail.
*/
void vGetGhostInfo(iMesh_Instance instance)
{
 // char *cFuncName = "iMeshP_getGhostInfo";
/// Get all the partitions
  iMeshP_PartitionHandle* pPtnHandles = NULL;
  int iPtnSize = 0, iPtnAllocated = 0;
  
  iMeshP_getPartitions(instance, &pPtnHandles, &iPtnAllocated, &iPtnSize, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMeshP_getPartitions");

  fprintf(fp, "\n \n ------------- getting ghosting info : iMeshP_getGhostInfo------------\n");

  /// get root set from the mesh instance, if does'nt succeed we can't proceed so throw error and exit
  iBase_EntitySetHandle root;
  iMesh_getRootSet(instance, &root, &ierr);

  CHECK_ERR_CRITICAL(ierr, "iMesh_getRootSet");
  
  /// Loop through all partitions
for(int p = 0; p < iPtnSize; p++ ) 
  { 
    ///  Get all local part handles in the partition 
   int iPrtSize = 0, iPrtAllocated = 0;
   iMeshP_PartHandle* pLocalParts = NULL;
   iMeshP_getLocalParts(instance, pPtnHandles[p], &pLocalParts, &iPrtAllocated, &iPrtSize, &ierr);

   CHECK_ERR_CRITICAL(ierr, "iMeshP_getLocalParts");
  
  /// For counting number of ghosting rules up-till now
  int iOwnRuleCount;

  // Check all possible combinations of boundary entities.
    for ( int eEType = iBase_REGION;
	eEType <= iBase_REGION; eEType++ ) {
       for(int iBridge = iBase_VERTEX; 
          iBridge < eEType; iBridge++ )
        {
        for(int iLayer = 1; iLayer <= iUpperNum; 
	    iLayer++)
         {
       bool bOK = true, bOK1 = true;
       /// Now delete all ghost entities in the partition
      iMeshP_deleteGhostEntsAll(instance,
            pPtnHandles[p],
            &ierr);

       CHECK_ERR(ierr, "deleteGhostEntsAll");

       if(ierr != iBase_SUCCESS)
	bOK1 = false; 
	fprintf(fp, "-----------------------------------------------");
       fprintf(fp,"\n Combination type %s Bridge %s ", astrTypes[eEType],astrTypes[iBridge]);
      for(int iInclCopies = 0; iInclCopies < 2; iInclCopies++ )
       {
      int iGhostRulesAllocated, iGhostSize;
      int* iGhostType = NULL;
      int* iBridgeType = NULL;
      int* iNumLayers = NULL;

      /// Step 1: Create ghosts of the specific entity type, bridge, number of layers and include copies(try with 0 & 1).
    iMeshP_createGhostEntsAll(instance,
            pPtnHandles[p], 
            eEType,
            iBridge,
            iLayer,
            iInclCopies,
            &ierr);
	
    CHECK_ERR(ierr, "iMeshP_createGhostEntsAll");       

      if(ierr != iBase_SUCCESS)
	{
       bOK1 = false;
	}
   /// Step 2: We need to count the ghost entity rules we are creating and check if it is getting updated correctly.
   ++iOwnRuleCount;

    iMeshP_ghostEntInfo(instance,
            pPtnHandles[p], 
            &iGhostRulesAllocated, 
            &iGhostSize, 
            &iGhostType,
            &iBridgeType,
            &iNumLayers,
            &ierr);

   CHECK_ERR(ierr, "iMeshP_ghostEntInfo");
   if(ierr != iBase_SUCCESS)
	{
	bOK = false;
	bOK1 = false;
	break; 
	}
   /// Step 2: Check the entity type of newly created ghost entity is present in the ghost information.
   bool bTypeCheck = false;
   for(int ii = 0; ii < iGhostRulesAllocated; ii++ )
        { 
        if(iGhostType[ii] ==  eEType)
	{
	   bTypeCheck = true;
	   break;
	}
        } // end ii

  if(!bTypeCheck)
    {
  // fprintf(fp, "\n The created ghosting type %s is not given back by the getGhostInfo \n", astrTypes[eEType]);
   CHECK_VALIDATION_ERR("iMeshP_ghostEntInfo");
   bOK1 = false;
   break;
    }

  /// Step 3: Check if the bridge type of newly created ghost entity is present in ghost information.
  bool bBridgeCheck = false;
   for(int ii = 0; ii < iGhostRulesAllocated; ii++ )
        { 
        if(iBridgeType[ii] ==  iBridge)
	{
	   bBridgeCheck = true;
	   break;
	}
        } // end ii

   if(!bBridgeCheck)
    {
   CHECK_VALIDATION_ERR("iMeshP_ghostEntInfo");
   bOK1 = false;
   break;
    }
  /// Step 4: Check if the number of layers of newly created ghost entity is present in the ghost information.
   bool bNumLayers = false;
  for(int ii = 0; ii < iGhostRulesAllocated; ii++ )
        { 
        if(iNumLayers[ii] ==  iLayer)
	{
	   bNumLayers = true;
	   break;
	}
        } // end ii

   if(!bNumLayers)
    {
   CHECK_VALIDATION_ERR("iMeshP_ghostEntInfo");
   bOK1 = false;
   break;
    }
   /// Step 5: Check if the number of ghost rules allocated is equal to our own count
    if(iOwnRuleCount != iGhostRulesAllocated)
     {
   // fprintf(fp, "\n getGhostInfo does not return correct number of ghosting rules \n");
    CHECK_VALIDATION_ERR("iMeshP_ghostEntInfo");
    bOK1 = false;
    break;
     }
   free(iNumLayers);
   free(iGhostType);
   free(iBridgeType);
   
     TEST(bOK, 0);
     TEST(bOK1, 1);
      } // end num layer loop
     } // end include copies loop
    } // end bridge loop
   } /// end types loop

/// Step 6: Now delete all ghost entities in the partition and check if everything is deleted using getGhostInfo.
      iMeshP_deleteGhostEntsAll(instance,
            pPtnHandles[p],
            &ierr);
       CHECK_ERR(ierr, "deleteGhostEntsAll");
    
     int iGhostRulesAllocated, iGhostSize;

     int* iGhostType = NULL; 
     int* iBridgeType = NULL;
     int* iNumLayers = NULL;

     iMeshP_ghostEntInfo(instance,
            pPtnHandles[p], 
            &iGhostRulesAllocated, 
            &iGhostSize, 
            &iGhostType,
            &iBridgeType,
            &iNumLayers,
            &ierr);
   if(iGhostRulesAllocated != 0 || iGhostSize != 0)
    {
    CHECK_VALIDATION_ERR("iMeshP_ghostEntInfo");
   // fprintf(fp, "\n Ghost deletion information not extracted! Fail. \n");
    }
  free(iGhostType);
  free(iBridgeType);
  free(iNumLayers);

  free(pLocalParts);
  } /// end partition loop
free(pPtnHandles);
} 
/////////////////////////////////////////Partition Functionality/////////////////////////////////////////////

/*void vCreatePartitionTest(iMesh_Instance instance)
{
/// Create the given number of partitions and keep track of the created partition count using iNumPrtn.
for(int ptn = 0; ptn < iPrtns; ptn++)
{
  MPI_Comm communicator;
  iMeshP_PartitionHandle pPartition;

  iMeshP_createPartitionAll(instance,
                          communicator,
                          &pPartition,
                          &ierr);

  if(ierr!=iBase_SUCCESS)
  {
   fprintf(fp, "\n Error creating partition. Cannot continue testing. Exiting program\n");
   ++iNFailed;
   exit(0);
  }
  iNumPrtn++;
} // end ptn loop

/// Desired number of partitions created. Now test them using getPartitions
iMeshP_PartitionHandle* pPartitions;
int iPrtnAllocated, iPrtnSize;

iMeshP_getPartitions(instance,
            &pPartitions,
            &iPrtnAllocated, 
            &iPrtnSize, 
            &ierr);

if(iPrtnSize != iNumPrtn) 
  {
  fprintf(fp, "\n Error getting size of partitions. Exiting program. \n");
  iNFailed++;
  exit(0);
  }

/// Now cross check the communicator of the returned partition.
for(int num = 0; num < iPrtnSize; num++ )
{
  MPI_Comm communicator;
  iMeshP_getPartitionComm(instance,
            pPartitions[num],
            &communicator,
            &ierr);
 if(ierr != iBase_SUCCESS)
  {
  fprintf(fp, "\n Unable to get communicator of the newly created partitions. Exiting. ");
  exit(0);
  }
}
} */
