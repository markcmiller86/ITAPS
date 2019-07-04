/*****************************************************************************
 * Zoltan Library for Parallel Applications                                  *
 * Copyright (c) 2000,2001,2002, Sandia National Laboratories                *
 * For more info, see the README file in the top-level Zoltan directory      *
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile: ITAPSZoltan.hpp,v $
 *    $Author: vjleung $
 *    $Date: 2011/04/01 22:24:32 $
 *    $Revision: 1.19 $
 *****************************************************************************/

/*
 * ITAPSZoltan: class to get a mesh via ITAPS and (re)write
 * Zoltan partition set(s) into that mesh via ITAPS
 */

#ifndef ITAPS_ZOLTAN_HPP
#define ITAPS_ZOLTAN_HPP

#include "zoltan_cpp.h"

extern "C" 
{
  int itapsGetNumberOfAssignedObjects(void *userDefinedData, int *err);
  
  void itapsGetObjectList(void *userDefinedData, int numGlobalIds, int numLids,
			 ZOLTAN_ID_PTR gids, ZOLTAN_ID_PTR lids, int wgt_dim,
			 float *obj_wgts, int *err);
  
  int itapsGetObjectSize(void *userDefinedData, int *err);
  
  void itapsGetObject(void *userDefinedData, int numGlobalIds, int numLids,
		     int numObjs, ZOLTAN_ID_PTR gids, ZOLTAN_ID_PTR lids,
		     int numDim, double *pts, int *err);
  
  void itapsGetNumberOfEdges(void *userDefinedData, int numGlobalIds,
			     int numLids, int numObjs, ZOLTAN_ID_PTR gids,
			     ZOLTAN_ID_PTR lids, int *numEdges, int *err);
  
  void itapsGetEdgeList(void *userDefinedData, int numGlobalIds, int numLids,
		       int numObjs, ZOLTAN_ID_PTR gids, ZOLTAN_ID_PTR lids,
		       int *numEdges,ZOLTAN_ID_PTR nborGlobalIds,int*nborProcs,
		       int wgt_dim, float *edge_wgts, int *err);

  void itapsHgCsSize(void *userDefinedData, int *numLists, int *numPins,
		     int *format, int*ierr);

  void itapsHgCs(void *userDefinedData, int numGidEntries, int numLists,
		 int numPins, int format, ZOLTAN_ID_PTR regionGID,
		 int *regionPtr, ZOLTAN_ID_PTR meshvtxGID, int *ierr);
}

using namespace std;
//using namespace MPI;

  class ITAPSZoltan 
  {

  public:
    ITAPSZoltan(iMesh_Instance *m = NULL, iBase_EntitySetHandle *rs = NULL,
		int argc = 0, char **argv = NULL) 
      : mesh(m), root_set(rs), myZZ(NULL), argcArg(argc), argvArg(argv)
      {}

    ~ITAPSZoltan();

    int parallel_partition(const int nparts, const int etype,
			   const char *method, const char *approach,
			   const char *package);
    
    int serial_partition(const int nparts, const int etype, const char *method,
			 const char *approach, const char *package);
    
  private:

    iMesh_Instance *mesh;

    iBase_EntitySetHandle *root_set;

    Zoltan *myZZ;

    int argcArg;

    char **argvArg;
  
      // given a processor assignment returned from Zoltan, write that as a
      // processor assignment to mesh data base via ITAPS
    int write_partition(const int nparts, iBase_EntityHandle *elems,
			int elems_size,	const int *assignment);

    void SetParmetis_Parameters(const char *approach);
  
    void SetPHG_Parameters(const char *method, const char *approach);
  
    void SetHSFC_Parameters();
  
    void SetRIB_Parameters();

    void SetRCB_Parameters();

    void SetRandom_Parameters();
  
    void SetBlock_Parameters();
  
    int GlobalSuccess(int rc);
  
    void PrintGlobalResult(char*s, int begin, int import, int exp, int change);
  
    void ShowError(int val, char *s);
  
    void FinalizePoints(int npts, int numExport,
			ZOLTAN_ID_PTR exportLocalIDs, int *exportProcs,
			int **assignment);
  
  };

#endif
