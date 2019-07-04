/*****************************************************************************
 * Zoltan Library for Parallel Applications                                  *
 * Copyright (c) 2000,2001,2002, Sandia National Laboratories                *
 * For more info, see the README file in the top-level Zoltan directory      *
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile: ITAPSZoltan.cpp,v $
 *    $Author: vjleung $
 *    $Date: 2011/04/01 22:24:32 $
 *    $Revision: 1.55 $
 *****************************************************************************/

/*
 * Given a mesh via ITAPS, (re)write Zoltan
 * partition set(s) into that mesh via ITAPS
 */

#include "iMesh.h"
#include "ITAPSZoltan.hpp"

#include <iostream>
#include <vector>

#define FALSE 0

static iMesh_Instance *Mesh=NULL;
static iBase_EntitySetHandle *Root_Set=NULL;
static int Etype;

ITAPSZoltan::~ITAPSZoltan() 
{
  if (NULL == myZZ) delete myZZ;
}

int ITAPSZoltan::parallel_partition(const int nparts, const int etype,
				    const char *method, const char *approach,
				    const char *package) 
{
  Mesh = mesh;
  Root_Set = root_set;
  Etype = etype;

  // Initialize Zoltan.  This is a C call.  The simple C++ code 
  // that creates Zoltan objects does not keep track of whether 
  // Zoltan_Initialize has been called.

  float version;
  int rank;

  Zoltan_Initialize(argcArg, argvArg, &version);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (NULL != method &&
      (!strcmp(method, "Graph") || !strcmp(method, "Hypergraph")) &&
      NULL!=approach && strcmp(approach, "Partition") && 
      strcmp(approach, "Repartition") && strcmp(approach, "Refine")) {
    cout << "[" << rank << "] ITAPSZoltan WARNING: Invalid approach ("
	 << approach << "); being set to Repartition." << endl;
    approach = NULL;
  }

  if (NULL != method && !strcmp(method, "Graph") &&
      NULL!=package && strcmp(package,"PHG") && strcmp(package,"Parmetis")) {
    cout << "[" << rank << "] ITAPSZoltan WARNING: Invalid package ("
	 << package << "); being set to PHG." << endl;
    package = NULL;
  }

  iBase_EntityHandle *elems = NULL;

  // Create Zoltan object.  This calls Zoltan_Create.  
  if (NULL == myZZ) {
    myZZ = new Zoltan(MPI_COMM_WORLD);
    if (NULL == myZZ) {
      printf("[%d] ITAPSZoltan ERROR: Failed to create Zoltan object.\n",rank);
      return ZOLTAN_FATAL;
    }
  }

  if (NULL == method || !strcmp(method, "RCB"))
    SetRCB_Parameters();
  else if (!strcmp(method, "Block"))
    SetBlock_Parameters();
  else if (!strcmp(method, "Random"))
    SetRandom_Parameters();
  else if (!strcmp(method, "RIB"))
    SetRIB_Parameters();
  else if (!strcmp(method, "HSFC"))
    SetHSFC_Parameters();
  else if (!strcmp(method, "Hypergraph") ||
	   !strcmp(method,"Graph") && (NULL==package||!strcmp(package,"PHG")))
    if (NULL == approach)
      SetPHG_Parameters(method, "Repartition");
    else
      SetPHG_Parameters(method, approach);
  else if (!strcmp(method, "Graph") && !strcmp(package, "Parmetis"))
    if (NULL == approach)
      SetParmetis_Parameters("Repartition");
    else
      SetParmetis_Parameters(approach);
  else {
    cout << "[" << rank << "] ITAPSZoltan WARNING: Invalid method (" << method
	 << "); being set to RCB." << endl;
    SetRCB_Parameters();
  }

    // set # requested partitions
  char buff[10];
  sprintf(buff, "%d", nparts);
  myZZ->Set_Param("NUM_GLOBAL_PARTITIONS", buff);

  // set # gid entries
  sprintf(buff, "%d", 2);
  myZZ->Set_Param("NUM_GID_ENTRIES", buff);

  // Call backs:  Get a mesh from data base via ITAPS.

  myZZ->Set_Num_Obj_Fn(itapsGetNumberOfAssignedObjects, NULL);
  myZZ->Set_Obj_List_Fn(itapsGetObjectList, NULL);
  myZZ->Set_Num_Geom_Fn(itapsGetObjectSize, NULL);
  myZZ->Set_Geom_Multi_Fn(itapsGetObject, NULL);
  myZZ->Set_Num_Edges_Multi_Fn(itapsGetNumberOfEdges, NULL);
  myZZ->Set_Edge_List_Multi_Fn(itapsGetEdgeList, NULL);
  myZZ->Set_HG_Size_CS_Fn(itapsHgCsSize, NULL);
  myZZ->Set_HG_CS_Fn(itapsHgCs, NULL);

  // Perform the load balancing partitioning

  int changes;
  int numGidEntries;
  int numLidEntries;
  int numImport;
  ZOLTAN_ID_PTR importGlobalIds;
  ZOLTAN_ID_PTR importLocalIds;
  int *importProcs;
  int *importToPart;
  int numExport;
  ZOLTAN_ID_PTR exportGlobalIds;
  ZOLTAN_ID_PTR exportLocalIds;
  int *exportProcs;
  int *exportToPart;

  int rc = myZZ->LB_Partition(changes, numGidEntries, numLidEntries, 
                            numImport, importGlobalIds, importLocalIds, 
                            importProcs, importToPart,
                            numExport, exportGlobalIds, exportLocalIds, 
                            exportProcs, exportToPart);

  rc = GlobalSuccess(rc);
  
  int elems_alloc = 0, elems_size, result;

  if (!rc){
    //if (rank == 0) {
      // get the elements
      iMesh_getEntities(*mesh, *root_set, etype, iMesh_ALL_TOPOLOGIES,
			&elems, &elems_alloc, &elems_size, &result);
      if (iBase_SUCCESS != result) {
	cout << "[0] ITAPSZoltan ERROR: Failed to get the elements." << endl;
	return result;
      }
      //if (!elems_size) {
      //cout << "[0] ITAPSZoltan WARNING: No elements." << endl;
      //return 1;
      //}
      
      PrintGlobalResult("==============Result==============", 
                        elems_size, numImport, numExport, changes);
      //} else {
      //PrintGlobalResult("==============Result==============", 
      //0, numImport, numExport, changes);
      //}
  }
  else{
    printf("[%d] ITAPSZoltan ERROR: Partitioning failed on process\n",rank);
    return rc;
  }
  
  // take results & write onto mesh data base partition sets via ITAPS

  if (rank == 0) {
    int *assignment;

    FinalizePoints(elems_size, numExport, exportLocalIds,
		   exportToPart, &assignment);
  
    result = write_partition(nparts, elems, elems_size, assignment);

    if (iBase_SUCCESS != result) return result;

    free((int *) assignment);
  }

  // Free the memory allocated for lists returned by LB_Parition()

  myZZ->LB_Free_Part(&importGlobalIds, &importLocalIds, &importProcs, 
                     &importToPart);
  myZZ->LB_Free_Part(&exportGlobalIds, &exportLocalIds, &exportProcs, 
                     &exportToPart);

  // Implementation note:  A Zoltan object contains an MPI communicator.
  //   When the Zoltan object is destroyed, it uses it's MPI communicator.
  //   So it is important that the Zoltan object is destroyed before
  //   the MPI communicator is destroyed.  To ensure this, dynamically
  //   allocate the Zoltan object, so you can explicitly destroy it.
  //   If you create a Zoltan object on the stack, it's destructor will
  //   be invoked atexit, possibly after the communicator's
  //   destructor.

  free(elems);
  return 0;
}

int ITAPSZoltan::serial_partition(const int nparts, const int etype,
				  const char *method, const char *approach,
				  const char *package) 
{
  Mesh = mesh;
  Root_Set = root_set;
  Etype = etype;

  // Initialize Zoltan.  This is a C call.  The simple C++ code 
  // that creates Zoltan objects does not keep track of whether 
  // Zoltan_Initialize has been called.

  float version;

  Zoltan_Initialize(argcArg, argvArg, &version);

    // should only be called in serial
  int numProcs, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (numProcs != 1) {
    cout << "[" << rank << "] ITAPSZoltan Error: "
	 << "serial_partition should only be called in serial" << endl;
    return 1;
  }

  if (NULL != method &&
      (!strcmp(method,"Graph")||!strcmp(method,"Hypergraph")) &&
      NULL!=approach && strcmp(approach, "Partition") && 
      strcmp(approach, "Repartition") && strcmp(approach, "Refine")) {
    cout << "[0] ITAPSZoltan WARNING: Invalid approach (" << approach
	 << "); being set to Repartition." << endl;
    approach = NULL;
  }

  if (NULL != method && !strcmp(method, "Graph") &&
      NULL!=package && strcmp(package,"PHG") && strcmp(package,"Parmetis")) {
    cout << "[0] ITAPSZoltan WARNING: Invalid package (" << package
	 << "); being set to PHG." << endl;
    package = NULL;
  }

  iBase_EntityHandle *elems = NULL;

  // Create Zoltan object.  This calls Zoltan_Create.  
  if (NULL == myZZ) {
    myZZ = new Zoltan(MPI_COMM_WORLD);
    if (NULL == myZZ) {
      cout << "[0] ITAPSZoltan ERROR: Failed to create Zoltan object." << endl;
      return ZOLTAN_FATAL;
    }
  }

  if (NULL == method || !strcmp(method, "RCB"))
    SetRCB_Parameters();
  else if (!strcmp(method, "Block"))
    SetBlock_Parameters();
  else if (!strcmp(method, "Random"))
    SetRandom_Parameters();
  else if (!strcmp(method, "RIB"))
    SetRIB_Parameters();
  else if (!strcmp(method, "HSFC"))
    SetHSFC_Parameters();
  else if (!strcmp(method, "Hypergraph") ||
	   (!strcmp(method, "Graph") && !strcmp(package, "PHG")))
    if (NULL == approach)
      SetPHG_Parameters(method, "Repartition");
    else
      SetPHG_Parameters(method, approach);
  else if (!strcmp(method, "Graph") && !strcmp(package, "Parmetis"))
    if (NULL == approach)
      SetParmetis_Parameters("Repartition");
    else
      SetParmetis_Parameters(approach);
  else   {
    cout << "[0] ITAPSZoltan WARNING: Invalid method (" << method
	 << "); being set to RCB." << endl;
    SetRCB_Parameters();
  }


    // set # requested partitions
  char buff[10];
  sprintf(buff, "%d", nparts);
  myZZ->Set_Param("NUM_GLOBAL_PARTITIONS", buff);

  //  set # gid entries
  sprintf(buff, "%d", 2);
  myZZ->Set_Param("NUM_GID_ENTRIES", buff);

    // request only partition assignments
  myZZ->Set_Param("RETURN_LISTS", "PARTITION ASSIGNMENTS");
  
  // Call backs:  Get a mesh from data base via ITAPS.

  myZZ->Set_Num_Obj_Fn(itapsGetNumberOfAssignedObjects, NULL);
  myZZ->Set_Obj_List_Fn(itapsGetObjectList, NULL);
  myZZ->Set_Num_Geom_Fn(itapsGetObjectSize, NULL);
  myZZ->Set_Geom_Multi_Fn(itapsGetObject, NULL);
  myZZ->Set_Num_Edges_Multi_Fn(itapsGetNumberOfEdges, NULL);
  myZZ->Set_Edge_List_Multi_Fn(itapsGetEdgeList, NULL);
  myZZ->Set_HG_Size_CS_Fn(itapsHgCsSize, NULL);
  myZZ->Set_HG_CS_Fn(itapsHgCs, NULL);

  // Perform the load balancing partitioning

  int changes;
  int numGidEntries;
  int numLidEntries;
  int dumnum1;
  ZOLTAN_ID_PTR dum_local, dum_global;
  int *dum1, *dum2;
  int num_assign;
  ZOLTAN_ID_PTR assign_gid, assign_lid;
  int *assign_procs, *assign_parts;

  int result = myZZ->LB_Partition(changes, numGidEntries, numLidEntries, 
				  dumnum1, dum_global, dum_local, dum1, dum2,
				  num_assign, assign_gid, assign_lid,
				  assign_procs, assign_parts);
  if (ZOLTAN_OK != result) {
    cout << "[0] ITAPSZoltan ERROR: Partitioning failed " << endl;
    return result;
  }
  
  // take results & write onto mesh data base partition sets via ITAPS

  // get the elements
  int elems_alloc = 0, elems_size;
  iMesh_getEntities(*mesh, *root_set, etype, iMesh_ALL_TOPOLOGIES,
		    &elems, &elems_alloc, &elems_size, &result);
  if (iBase_SUCCESS != result) {
    cout << "[0] ITAPSZoltan ERROR: Failed to get the elements." << endl;
    return result;
  }
  if (!elems_size) {
    cout << "[0] ITAPSZoltan WARNING: No elements." << endl;
    return 1;
  }

  result = write_partition(nparts, elems, elems_size, assign_parts);

  if (iBase_SUCCESS != result) return result;


  // Free the memory allocated for lists returned by LB_Parition()
  myZZ->LB_Free_Part(&assign_gid, &assign_lid, &assign_procs, &assign_parts);

  free(elems);
  return 0;
}

int ITAPSZoltan::write_partition(const int nparts,
				 iBase_EntityHandle *elems, 
				 int elems_size,
				 const int *assignment) 
{

    // get the partition set tag
  iBase_TagHandle part_set_tag = NULL;
  
    // get any sets already with this tag, and clear this tag
  iBase_EntitySetHandle *sets = NULL;
  int set_alloc = 0, sets_size;
  int result;
  iMesh_getEntSets(*mesh,*root_set,1,&sets,&set_alloc,&sets_size,&result);
  if (iBase_SUCCESS != result) {
    cout << "[0] ITAPSZoltan ERROR: "
	 << "entitysetGetEntitites failed in write_partition." << endl;
    return result;
  }

  // iterate through them, checking whether they have tags
  int i;
  for (i = 0; i < sets_size; i++) {
    iBase_TagHandle *tags = NULL;
    int tags_alloc = 0, tags_size;

    iMesh_getAllEntSetTags(*mesh,sets[i],&tags,&tags_alloc,&tags_size,&result);
    if (iBase_SUCCESS != result) {
      cout << "[0] ITAPSZoltan ERROR: "
	   << "Couldn't get all tag handles from entityset." << endl;
      return result;
    }

    for (int j = 0; j < tags_size; j++) {
      char dum_name[120];
      int dum_name_size = 120;
      iMesh_getTagName(*mesh, tags[j], dum_name, &result, dum_name_size);
      if (iBase_SUCCESS != result) {
	cout << "[0] ITAPSZoltan ERROR: Couldn't get name of tag." << endl;
	return result;
      }
      if (!strcmp(dum_name, "PARALLEL_PARTITION")) {
	part_set_tag = tags[j];
	if (tags_size == 1) {
	  iMesh_destroyEntSet(*mesh, sets[i], &result);
	  if (iBase_SUCCESS != result) {
	    printf("[0] ITAPSZoltan ERROR: Failed to destroy sets[%d].\n", i);
	    return result;
	  }
	}
	break;
      }
    }
  }
  
  if (part_set_tag) {
    iMesh_destroyTag(*mesh, part_set_tag, 1, &result);
    if (iBase_SUCCESS != result) {
      cout << "[0] ITAPSZoltan ERROR: "
	   << "Couldn't force-delete an in-use tag from mesh." << endl;
      return result;
    }
  }

  iMesh_createTag(*mesh, "PARALLEL_PARTITION", 1, iBase_INTEGER,
		  &part_set_tag, &result, 19);
  /*if (iBase_SUCCESS != result) {
    cout << "[0] ITAPSZoltan ERROR: "
	 << "Failed to create tag part_set_tag in write_partition." << endl;
    return result;
    }*/
  
    // first, create partition sets and store in vector
  if (nparts > 0) {
    iBase_EntitySetHandle *part_sets =
      (iBase_EntitySetHandle *)malloc(nparts * sizeof(iBase_EntitySetHandle));
  
    // partition sets - create
    for (i = 0; i < nparts; i++) {
      iMesh_createEntSet(*mesh, FALSE, &part_sets[i], &result);
      if (iBase_SUCCESS != result) {
	cout << "[0] ITAPSZoltan ERROR: "
	     << "Problem creating entityset in write_partition." << endl;
	return result;
      }
    }
    
    // write a tag to those sets denoting they're partition sets, with a value
    // of the proc number
    for (i = 0; i < nparts; i++) {
      int dum_ids = i;
      
      iMesh_setEntSetData(*mesh, part_sets[i], part_set_tag,
			  (char*)(&dum_ids), sizeof(int), &result);
      if (iBase_SUCCESS != result) {
	cout << "[0] ITAPSZoltan ERROR: Failed to set int tag (val="
	    << i << ") in write_partition." << endl;
	return result;
      }
    }
    
    // assign entities to the relevant sets
    for (i = 0; i < elems_size; i++) {
      iMesh_addEntToSet(*mesh, elems[i], part_sets[assignment[i]], &result);
      if (iBase_SUCCESS != result) {
	cout << "[0] ITAPSZoltan ERROR: "
	     << "Failed to add entity in write_partition." << endl;
	return result;
      }
    }
    
    // allocate integer-size partitions
    int assignment_size = elems_size * sizeof(int);
    iMesh_setArrData(*mesh, elems, elems_size, part_set_tag,
			 (const char*)assignment, assignment_size, &result);
    if (iBase_SUCCESS != result) {
      cout << "[0] ITAPSZoltan ERROR: "
	   << "Failed to set int tags in write_partition." << endl;
      return result;
    }
  }

  return iBase_SUCCESS;
}

void ITAPSZoltan::SetBlock_Parameters()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\n Block Partitioning" << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");     // no debug messages
  myZZ->Set_Param("LB_METHOD", "Block");   // Block Partitioning
}

void ITAPSZoltan::SetRandom_Parameters()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\nRandom Partitioning" << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");     // no debug messages
  myZZ->Set_Param("LB_METHOD", "Random");  // Random Partitioning
}

void ITAPSZoltan::SetRCB_Parameters()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\nRecursive Coordinate Bisection" << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");     // no debug messages

  // RCB parameters:

  myZZ->Set_Param("RCB_OUTPUT_LEVEL", "2");
  myZZ->Set_Param("KEEP_CUTS", "1");            //save decomposition
}

void ITAPSZoltan::SetRIB_Parameters()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\nRecursive Inertial Bisection" << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");     // no debug messages
  myZZ->Set_Param("LB_METHOD", "RIB");     // Recursive Inertial Bisection

  // RIB parameters:

  myZZ->Set_Param("RIB_OUTPUT_LEVEL", "2");
  myZZ->Set_Param("KEEP_CUTS", "1");              // save decomposition
}

void ITAPSZoltan::SetHSFC_Parameters()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\nHilbert Space Filling Curve" << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");   // no debug messages
  myZZ->Set_Param("LB_METHOD", "HSFC");  // perform Hilbert space filling curve

  // HSFC parameters:

  myZZ->Set_Param("KEEP_CUTS", "1");              // save decomposition
}

void ITAPSZoltan::SetPHG_Parameters(const char *method, const char *approach)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\n " << method << ": PHG: " << approach << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");     // no debug messages
  myZZ->Set_Param("LB_METHOD", method);

  // PHG parameters:
  myZZ->Set_Param("LB_APPROACH", approach); // graph approach
  myZZ->Set_Param("PHG_OUTPUT_LEVEL", "1");
}

void ITAPSZoltan::SetParmetis_Parameters(const char *approach)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    cout << "\nGraph: Parmetis: " << approach << endl;
  // General parameters:

  myZZ->Set_Param("DEBUG_LEVEL", "0");          // no debug messages
  myZZ->Set_Param("LB_METHOD", "Graph");     
  myZZ->Set_Param("GRAPH_PACKAGE", "Parmetis"); // the ParMETIS library

  // Parmetis parameters:

  myZZ->Set_Param("LB_APPROACH", approach); // graph approach
}

void ITAPSZoltan::FinalizePoints(int npts, int numExport,
				ZOLTAN_ID_PTR exportLocalIDs,
				int *exportToPart, int **assignment)
{
  int *MyAssignment;
  int i;

  /* assign pts to start */

  MyAssignment = (int *)malloc(sizeof(int) * npts);

  for (i=0; i<npts; i++)
    MyAssignment[i] = 0;

  for (i=0; i<numExport; i++)
    MyAssignment[exportLocalIDs[i]] = exportToPart[i];

  *assignment = MyAssignment;
}

int ITAPSZoltan::GlobalSuccess(int rc)
{
  int fail = 0;
  int i;
  int numProcs;
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  int *vals = (int *)malloc(numProcs * sizeof(int));

  MPI_Allgather(&rc, 1, MPI_INT, vals, 1, MPI_INT, MPI_COMM_WORLD);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  for (i=0; i<numProcs; i++){
    if (vals[i] != ZOLTAN_OK){
      if (0 == rank){
        ShowError(vals[i], "Result on process ");
      }
      fail = vals[i];
    }
  }

  free(vals);
  return fail;
}

void ITAPSZoltan::PrintGlobalResult(char *s, 
                                   int begin, int import, int exp, int change)
{
  int i;
  int *v1 = (int *)malloc(4 * sizeof(int));
  int *v2 = NULL;
  int *v;

  v1[0] = begin;
  v1[1] = import;
  v1[2] = exp;
  v1[3] = change;

  int rank, numProcs;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  if (rank == 0){
    v2 = (int *)malloc(4 * numProcs * sizeof(int));
  }

  MPI_Gather(v1, 4, MPI_INT, v2, 4, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0){
    cout << "======" << s << "======" << endl;
    for (i=0, v=v2; i<numProcs; i++, v+=4){
      cout << i << ": originally had " << v[0] << ", import " << v[1]
	   << ", exp " << v[2] << ", "
	   << (v[3] ? "a change of partitioning" : "no change") << endl;
    }
    cout << "==========================================" << endl;
    fflush(stdout); 

    free(v2);
  }

  free(v1);
}

void ITAPSZoltan::ShowError(int val, char *s)
{
  if (s)
    {
      cout << s << " ";
    }
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  switch (val)
    {
    case ZOLTAN_OK:
      cout << rank << ": SUCCESSFUL" << endl;
      break;
    case ZOLTAN_WARN:
      cout << rank << ": WARNING" << endl;
      break;
    case ZOLTAN_FATAL:
      cout << rank << ": FATAL ERROR" << endl;
      break;
    case ZOLTAN_MEMERR:
      cout << rank << ": MEMORY ALLOCATION ERROR" << endl;
      break;
    default:
      cout << rank << ": INVALID RETURN CODE" << endl;
      break;
    }
}

/**********************
** call backs
**********************/

int itapsGetNumberOfAssignedObjects(void *userDefinedData, int *err)
{
  int num_elems = 0;

  //if (COMM_WORLD.Get_rank() == 0) {

    // get the number of elements
    int result;
    iMesh_getNumOfType(*Mesh, *Root_Set, Etype, &num_elems, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return FALSE;
    }
    //if (!num_elems) {
      //*err = ZOLTAN_WARN;
      //return FALSE;
    //}

    //}
    
    *err = ZOLTAN_OK;

  return num_elems;
}

void itapsGetObjectList(void *userDefinedData, int numGlobalIds, int numLids,
  ZOLTAN_ID_PTR gids, ZOLTAN_ID_PTR lids, int wgt_dim, float *obj_wgts,
  int *err)
{
  //if (COMM_WORLD.Get_rank() == 0) {
    iBase_EntityHandle *elems = NULL;
    int elems_alloc = 0, elems_size;
    int result;

    // get the elements
    iMesh_getEntities(*Mesh, *Root_Set, Etype, iMesh_ALL_TOPOLOGIES,
		     &elems, &elems_alloc, &elems_size, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }

    for (int i = 0; i < elems_size; i++) {

      gids[2 * i] = (unsigned long)elems[i] >> 16;
      gids[2 * i + 1] = (unsigned long)elems[i] & 0xffff;
      lids[i] = i;
    }

    free(elems);
    //}
    
  *err = ZOLTAN_OK;
}

int itapsGetObjectSize(void *userDefinedData, int *err)
{
  *err = ZOLTAN_OK;
  return 3;
} 

void itapsGetObject(void *userDefinedData, int numGlobalIds, int numLids,
		 int numObjs, ZOLTAN_ID_PTR gids, ZOLTAN_ID_PTR lids,
		 int numDim, double *pts, int *err)
{
  //if (COMM_WORLD.Get_rank() == 0) {
    vector<double> coords;
    iBase_EntityHandle *elems = NULL;
    int elems_alloc = 0, elems_size;
    int result;
    int i, id, id3;
    int next = 0;
    
    // check storage order
    int this_order;// = iBase_UNDETERMINED;
    iMesh_getDfltStorage(*Mesh, &this_order, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    if (numDim != 3)    
      {
	*err = ZOLTAN_FATAL;         
	return;
      }
    
    // get the elements
    iMesh_getEntities(*Mesh, *Root_Set, Etype, iMesh_ALL_TOPOLOGIES, &elems,
		      &elems_alloc, &elems_size, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    //if (!elems_size) {
    //*err = ZOLTAN_WARN;
    //return;
    //}

    // iterate through them;
    // first have to get an Entity interface instance
    double avg_position[3];
    
    for (i = 0; i < elems_size; i++) {
      // now get the vertex coordinates from a vertex array
      // need to get the vertices in the element
      iBase_EntityHandle *verts = NULL;
      int verts_alloc = 0, verts_size;
      
      if (Etype != iBase_VERTEX) {
	iMesh_getEntAdj(*Mesh, elems[i], iBase_VERTEX, &verts, &verts_alloc,
			&verts_size, &result);
	if (iBase_SUCCESS != result) {
	  *err = ZOLTAN_FATAL;
	  return;
	}
      }
      
      // get the coordinates in one array
      double *vert_coords = NULL;
      int vert_coords_alloc = 0, vert_coords_size;
      
      if (Etype != iBase_VERTEX)
	iMesh_getVtxArrCoords(*Mesh, verts, verts_size, this_order,
			      &vert_coords, &vert_coords_alloc,
			      &vert_coords_size, &result);
      else {
	verts_size = 1;
	iMesh_getVtxArrCoords(*Mesh, &elems[i], verts_size, this_order,
			      &vert_coords, &vert_coords_alloc,
			      &vert_coords_size, &result);
      }
      if (iBase_SUCCESS != result) {
	*err = ZOLTAN_FATAL;
	return;
      }
      
      // get average position of vertices
      int j, k;
      for (j = 0; j < 3; j++)
	avg_position[j] = 0;
      
      if (this_order == iBase_BLOCKED)
	for (j = 0; j < 3; j++) {
	  for (k = 0; k < verts_size; k++)
	    avg_position[j] += vert_coords[j * verts_size + k];
	  
	  avg_position[j] /= verts_size;
	}
      else
	for (j = 0; j < 3; j++) {
	  for (k = 0; k < verts_size; k++)
	    avg_position[j] += vert_coords[3 * k + j];
	  
	  avg_position[j] /= verts_size;
	}
      
      // copy those into coords vector
      copy(avg_position, avg_position+3, back_inserter(coords));
      free(verts);
      free(vert_coords);
    }
    
    for (i=0; i<numObjs; i++)
      {
	id = lids[i];
	
	if ((id < 0) || (id >= elems_size))
	  {
	    *err = ZOLTAN_FATAL;
	    return;
	  }
	
	id3 = lids[i] * 3;
	
	pts[next++] = coords[id3];
	pts[next++] = coords[id3 + 1];
	pts[next++] = coords[id3 + 2];
      }

    free(elems);
    //}

  *err = ZOLTAN_OK;
}

void itapsGetNumberOfEdges(void *userDefinedData, int numGlobalIds,
			   int numLids, int numObjs, ZOLTAN_ID_PTR gids,
			   ZOLTAN_ID_PTR lids, int *numEdges, int *err)
{
  //if (COMM_WORLD.Get_rank() == 0) {
    vector<int> length;
    iBase_EntityHandle *elems = NULL;
    int elems_alloc = 0, elems_size;
    int result;
    
    // get the elements
    iMesh_getEntities(*Mesh, *Root_Set, Etype, iMesh_ALL_TOPOLOGIES,
		      &elems, &elems_alloc, &elems_size, &result);
    if (iBase_SUCCESS != result){
      *err = ZOLTAN_FATAL;
      return;
    }
    //if (!elems_size) {
    //*err = ZOLTAN_WARN;
    //return;
    //}
    
    // now assemble the graph, calling getEntArrAdj to get bridge adjacencies
    // through 2 dimensional neighbors
    /*int adjs;*/
    
    // get all adjacent subelements of element
    /*iBase_EntityHandle *adj_selems = NULL;
    int *selem_offsets = NULL;
    int adj_selems_alloc = 0, selem_offsets_alloc = 0;
    int adj_selems_size = 0, selem_offsets_size = 0;
    iMesh_getEntArrAdj(*Mesh,elems,elems_size,Etype?Etype-1:Etype+1,
		       &adj_selems,&adj_selems_alloc,&adj_selems_size,
		       &selem_offsets,&selem_offsets_alloc,&selem_offsets_size,
		       &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    if (elems_size+1 != selem_offsets_size) {
      *err = ZOLTAN_FATAL;
      return;
      }*/
    
    // get all adjacent elements of adjacent subelements
    iBase_EntityHandle *adj_elems = NULL;
    int adj_elems_alloc = 0, adj_elems_size = 0;
    int *elem_offsets = NULL;
    int elem_offsets_alloc = 0, elem_offsets_size = 0;
    /*iMesh_getEntArrAdj(*Mesh, adj_selems, adj_selems_size, Etype,
		       &adj_elems, &adj_elems_alloc, &adj_elems_size,
		       &elem_offsets, &elem_offsets_alloc,
		       &elem_offsets_size, &result);*/
    iMesh_getEntArr2ndAdj(*Mesh,elems,elems_size,Etype?Etype-1:Etype+1, Etype,
			  &adj_elems, &adj_elems_alloc, &adj_elems_size,
			  &elem_offsets, &elem_offsets_alloc,
			  &elem_offsets_size, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    /*if (adj_selems_size+1 != elem_offsets_size) {
      *err = ZOLTAN_FATAL;
      return;
      }*/
    if (elems_size+1 != elem_offsets_size) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    for (int i = 0; i < elems_size; i++) {
      
      // get bridge adjacencies
      /*adjs = selem_offsets[i + 1] - selem_offsets[i];
      for (int j = selem_offsets[i]; j < selem_offsets[i + 1]; j++)
	adjs += elem_offsets[j + 1] - elem_offsets[j] - 2;

	length.push_back(adjs);*/
      length.push_back(elem_offsets[i + 1] - elem_offsets[i]);
    }
    
    int i, id;
    int next = 0;
    
    for (i=0; i<numObjs; i++)
      {
	id = lids[i];
	
	if ((id < 0) || (id >= elems_size))
	  {
	    *err = ZOLTAN_FATAL;
	    return;
	  }
	
	numEdges[next++] = length[id];
    }

    free(elems);
    /*free(adj_selems);
      free(selem_offsets);*/
    free(adj_elems);
    free(elem_offsets);
    //}

  *err = ZOLTAN_OK;
}

void itapsGetEdgeList(void *userDefinedData, int numGlobalIds, int numLids,
		   int numObjs,
		   ZOLTAN_ID_PTR gids, ZOLTAN_ID_PTR lids, int *numEdges,
		   ZOLTAN_ID_PTR nborGlobalIds, int *nborProcs, int wgt_dim,
		   float *edge_wgts, int *err)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vector<int> adjacencies0;
    vector<int> adjacencies1;
    vector<int> length;
    iBase_EntityHandle *elems = NULL;
    int elems_alloc = 0, elems_size;
    int result;
    // get the elements
    iMesh_getEntities(*Mesh, *Root_Set, Etype, iMesh_ALL_TOPOLOGIES,
		      &elems, &elems_alloc, &elems_size, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    //if (!elems_size) {
    //*err = ZOLTAN_WARN;
    //return;
    //}
    
    // now assemble the graph, calling getEntArrAdj to get bridge adjacencies
    // through 2 dimensional neighbors
    int adjs;
    
    // get all adjacent subelements of elements
    /*iBase_EntityHandle *adj_selems = NULL;
    int *selem_offsets = NULL;
    int adj_selems_alloc = 0, selem_offsets_alloc = 0;
    int adj_selems_size = 0, selem_offsets_size = 0;
    iMesh_getEntArrAdj(*Mesh,elems,elems_size,Etype?Etype-1:Etype+1,
		       &adj_selems,&adj_selems_alloc,&adj_selems_size,
		       &selem_offsets,&selem_offsets_alloc,&selem_offsets_size,
		       &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    if (elems_size+1 != selem_offsets_size) {
      *err = ZOLTAN_FATAL;
      return;
      }*/
    
    // get all adjacent elements of adjacent subelements
    iBase_EntityHandle *adj_elems = NULL;
    int adj_elems_alloc = 0, adj_elems_size = 0;
    int *elem_offsets = NULL;
    int elem_offsets_alloc = 0, elem_offsets_size = 0;
    /*iMesh_getEntArrAdj(*Mesh, adj_selems, adj_selems_size, Etype,
		       &adj_elems, &adj_elems_alloc, &adj_elems_size,
		       &elem_offsets, &elem_offsets_alloc,
		       &elem_offsets_size, &result);*/
    iMesh_getEntArr2ndAdj(*Mesh,elems,elems_size,Etype?Etype-1:Etype+1, Etype,
			  &adj_elems, &adj_elems_alloc, &adj_elems_size,
			  &elem_offsets, &elem_offsets_alloc,
			  &elem_offsets_size, &result);
    if (iBase_SUCCESS != result) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    /*if (adj_selems_size+1 != elem_offsets_size) {
      *err = ZOLTAN_FATAL;
      return;
      }*/
    if (elems_size+1 != elem_offsets_size) {
      *err = ZOLTAN_FATAL;
      return;
    }
    
    vector<int> neighbors0;
    vector<int> neighbors1;
    
    for (int i = 0; i < elems_size; i++) {
      
      // get bridge adjacencies
      /*adjs = selem_offsets[i + 1] - selem_offsets[i];*/
      neighbors0.clear();
      neighbors1.clear();
      int j;
      /*for (j = selem_offsets[i]; j < selem_offsets[i + 1]; j++) {
	adjs += elem_offsets[j + 1] - elem_offsets[j] - 2;
	// get the graph vertex ids of those
	for (int k = elem_offsets[j]; k < elem_offsets[j + 1]; k++)
	  if (adj_elems[k] != elems[i]) {
	    neighbors0.push_back((unsigned long)adj_elems[k] >> 16);
	    neighbors1.push_back((unsigned long)adj_elems[k] & 0xffff);
	  }
	  }*/
      adjs = elem_offsets[i + 1] - elem_offsets[i];
      for (j = elem_offsets[i]; j < elem_offsets[i + 1]; j++) {
	neighbors0.push_back((unsigned long)adj_elems[j] >> 16);
	neighbors1.push_back((unsigned long)adj_elems[j] & 0xffff);
      }
      
      // copy those into adjacencies vector
      length.push_back(adjs);
      for (j = 0; j < adjs; j++) {
	adjacencies0.push_back(neighbors0[j]);
	adjacencies1.push_back(neighbors1[j]);
      }
    }
    
    int i, id, idSum, j;
    int next = 0;
    
    for (i=0; i<numObjs; i++)
      {
	id = lids[i];
	
	if ((id < 0) || (id >= elems_size))
	  {
	    *err = ZOLTAN_FATAL;
	    return;
	  }
	
	idSum = 0;
	
	for (j=0; j<id; j++)
	  idSum += length[j];

	for (j=0; j<length[id]; j++)
	  {
	    nborGlobalIds[2 * next] = adjacencies0[idSum];
	    nborGlobalIds[2 * next + 1] = adjacencies1[idSum++];
	    nborProcs[next++] = 0;
	  }
      }

    free(elems);
    /*free(adj_selems);
      free(selem_offsets);*/
    free(adj_elems);
    free(elem_offsets);
  }

  *err = ZOLTAN_OK;
}

//#ifdef KDDKDD_READY
//////////////////////////////////////////////////////////////////////////////
// Erik & Cedric's hypergraph model for edges.
// Objects to partition = HG vertices = Mesh regions.
// Hyperedges = list of all regions connected by a mesh vertex = Mesh vertices.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ZOLTAN_HG_SIZE_CS_FN
void itapsHgCsSize(
  void *userDefinedData,
  int *numLists,
  int *numPins, 
  int *format,
  int *ierr
)
{
  int result;  // Result of ITAPS calls.

  // For now, assume partitioning regions (3D) or faces (2D).
  // Later can extend to partitioning mesh vertices using mesh regions as
  // hyperedges.
  if (Etype != iBase_REGION && Etype != iBase_FACE) {
    *ierr = ZOLTAN_FATAL;
    return;
  }

  // We'll list the hyperedges (mesh vertices) to which a hypergraph vertex
  // (mesh region) is connected.
  *format = ZOLTAN_COMPRESSED_VERTEX;

  // numLists = num mesh regions on this proc
  // Later, we'll specify one list per region; each list will contain the adj
  // vertex IDs for that region.
  int num_elems;
  iMesh_getNumOfType(*Mesh, *Root_Set, Etype, &num_elems, &result);
  if (iBase_SUCCESS != result) {
    *ierr = ZOLTAN_FATAL;
    return;
  }
  *numLists = num_elems;

  // We need to know the total number of vertex adjacencies are for all
  // regions on this processor.  Getting the entities and their adjacencies
  // is overkill for this function; we might consider pre-fetching this
  // data before calling the partitioner so we don't have to repeatedly
  // call these functions.  Or if we knew what each region had a fixed
  // number N of mesh vertices, we could return 
  // *numPins = N * num_elems;
  // Does iMesh have a shortcut returning the number of mesh vertices per
  // region?
  // VJL - If the following iMesh call has to be made, I believe the previous
  // iMesh call does not have to be made.
  iBase_EntityHandle *elems = NULL;
  int elems_alloc = 0;
  int elems_size = 0;
  iMesh_getEntities(*Mesh, *Root_Set, Etype, iMesh_ALL_TOPOLOGIES,
                    &elems, &elems_alloc, &elems_size, &result);
  if (iBase_SUCCESS != result) {
    *ierr = ZOLTAN_FATAL;
    return;
  }
  
  iBase_EntityHandle *adj_selems = NULL;
  int adj_selems_alloc = 0;
  int adj_selems_size = 0;
  int *selem_offsets = NULL;
  int selem_offsets_alloc = 0;
  int selem_offsets_size = 0;
  iMesh_getEntArrAdj(*Mesh,elems,elems_size,iBase_VERTEX,
                     &adj_selems,&adj_selems_alloc,&adj_selems_size,
                     &selem_offsets,&selem_offsets_alloc,&selem_offsets_size,
                     &result);
  if (iBase_SUCCESS != result) {
    *ierr = ZOLTAN_FATAL;
    return;
  }

  *numPins = adj_selems_size;

  free(elems);
  free(adj_selems);
  free(selem_offsets);
  *ierr = ZOLTAN_OK;
}

/////////////////////////////////////////////////////////////////////////////
// ZOLTAN_HG_CS_FN
void itapsHgCs(
  void *userDefinedData,
  int numGidEntries,
  int numLists,
  int numPins,
  int format,
  ZOLTAN_ID_PTR regionGID,
  int *regionPtr,
  ZOLTAN_ID_PTR meshvtxGID,
  int *ierr
)
{
  // For now, assume partitioning regions (3D) or faces (2D).
  // Later can extend to partitioning mesh vertices using mesh regions as
  // hyperedges.
  if (Etype != iBase_REGION && Etype != iBase_FACE) {
    *ierr = ZOLTAN_FATAL;
    return;
  }

  // iZoltan converts EntityHandles to two ZOLTAN_ID_PTR entries.
  if (numGidEntries != 2) {
    *ierr = ZOLTAN_FATAL;
    return;
  }

  // First, get the regions (faces) to partition.
  iBase_EntityHandle *elems = NULL;
  int elems_alloc = 0;
  int elems_size = 0;
  int result;
  iMesh_getEntities(*Mesh, *Root_Set, Etype, iMesh_ALL_TOPOLOGIES,
                    &elems, &elems_alloc, &elems_size, &result);
  if (iBase_SUCCESS != result) {
    *ierr = ZOLTAN_FATAL;
    return;
  }

  if (elems_size != numLists) {
    // Sanity check.
    *ierr = ZOLTAN_FATAL;
    return;
  }

  for (int i = 0; i < elems_size; i++) {
    regionGID[2 * i] = (unsigned long)elems[i] >> 16;
    regionGID[2 * i + 1] = (unsigned long)elems[i] & 0xffff;
  }

  // Next, get the adjacent mesh vertices.
  iBase_EntityHandle *adj_selems = NULL;
  int adj_selems_alloc = 0;
  int adj_selems_size = 0;
  int *selem_offsets = NULL;//regionPtr;
  int selem_offsets_alloc = 0;//numLists;
  int selem_offsets_size = 0;
  iMesh_getEntArrAdj(*Mesh,elems,elems_size,iBase_VERTEX,
                     &adj_selems,&adj_selems_alloc,&adj_selems_size,
                     &selem_offsets,&selem_offsets_alloc,&selem_offsets_size,
                     &result);
  if (iBase_SUCCESS != result) {
    *ierr = ZOLTAN_FATAL;
    return;
  }

  if ((selem_offsets_size != numLists + 1) || (adj_selems_size != numPins)) {
    // Sanity check.
    *ierr = ZOLTAN_FATAL;
    return;
  }

  for (int i = 0; i < numLists; i++) {
    regionPtr[i] = selem_offsets[i];
  }

  for (int i = 0; i < adj_selems_size; i++) {
    meshvtxGID[2 * i] = (unsigned long)adj_selems[i] >> 16;
    meshvtxGID[2 * i + 1] = (unsigned long)adj_selems[i] & 0xffff;
  }

  free(elems);
  free(adj_selems);
  free(selem_offsets);
  *ierr = ZOLTAN_OK;
}

//#endif // KDDKDD_READY

