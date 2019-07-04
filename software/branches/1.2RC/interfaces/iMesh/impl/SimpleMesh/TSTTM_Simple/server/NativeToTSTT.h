/*
 * File: TSTT_SIDL_Converters.h
 */

#ifndef TSTT_SIDL_CONVERTERS_H
#define TSTT_SIDL_CONVERTERS_H

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_TSTTM_EntityTopology_h
#include "TSTTM_EntityTopology.h"
#endif
#ifndef included_TSTTM_EntityType_h
#include "TSTTM_EntityType.h"
#endif
#ifndef included_TSTTM_AdjacencyInfo_h
#include "TSTTM_AdjacencyInfo.h"
#endif
#ifndef included_TSTTB_Error_h
#include "TSTTB_Error.h"
#endif
#ifndef included_TSTTM_StorageOrder_h
#include "TSTTM_StorageOrder.h"
#endif
#ifndef included_sidl_BaseException_h
#include "sidl_BaseException.h"
#endif
#ifndef included_sidl_BaseInterface_h
#include "sidl_BaseInterface.h"
#endif
#ifndef included_sidl_ClassInfo_h
#include "sidl_ClassInfo.h"
#endif
#ifndef included_sidl_SIDLException_h
#include "sidl_SIDLException.h"
#endif

#include "mesh.h"
#include "TSTTmesh.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

struct sidl_opaque__array*
NativeToTSTTTagArray(Tag **tags, int num);

struct sidl_opaque__array*
NativeToTSTTEntityArray(Entity_list *entities, int num);

Entity_list*
TSTTToNativeEntityArray(struct sidl_opaque__array* entities);

struct sidl_double__array*
NativeToTSTTDoubleArray(double *nbrs, int num);

double*
TSTTToNativeDoubleArray(struct sidl_double__array *nbrs);

struct sidl_int__array*
NativeToTSTTIntArray(int *nbrs, int num);

int*
TSTTToNativeIntArray(struct sidl_int__array *nbrs);

struct sidl_opaque__array*
NativeToTSTTOpaqueArray(void **ptrs, int num);

void**
TSTTToNativeOpaqueArray(struct sidl_opaque__array *ptrs, int ptrsize);

struct sidl_opaque__array*
NativeToTSTTEntityHandleArray(EntityHandle **handles,
                             int num);

EntityHandle**
TSTTToNativeEntityHandleArray(struct sidl_opaque__array *handles);

struct sidl_opaque__array*
NativeToTSTTEntitySetArray(EntitySet_list *sets, int num);

EntitySet_list*
TSTTToNativeEntitySetArray(struct sidl_opaque__array* sets);

void
newTSTTException(int32_t code, char *msg, TSTTB_Error *_ex);

void
NativeToTSTTException(Error *err, sidl_BaseInterface *_ex);

enum TSTTM_StorageOrder__enum
NativeToTSTTStorageOrder(StorageOrder order);

StorageOrder
TSTTToNativeStorageOrder(enum TSTTM_StorageOrder__enum order);

enum TSTTM_EntityType__enum
NativeToTSTTEntityType(EntityType type);

EntityType
TSTTToNativeEntityType(enum TSTTM_EntityType__enum type);

struct TSTTM_EntityType__array*
NativeToTSTTEntityTypeArray(EntityType *types, int num);

enum TSTTM_EntityTopology__enum
NativeToTSTTEntityTopology(EntityTopology topo);

EntityTopology
TSTTToNativeEntityTopology(enum TSTTM_EntityTopology__enum topo);

struct TSTTM_EntityTopology__array*
NativeToTSTTEntityTopologyArray(EntityTopology *topos, int num);

void *
checkDoubleArraySize(struct sidl_double__array **sidl_array, int num, sidl_BaseInterface *_ex) ;

void *
checkIntArraySize(struct sidl_int__array **sidl_array, int num, sidl_BaseInterface *_ex) ;
void *
checkEntityTypeArraySize(struct TSTTM_EntityType__array **sidl_array, int num, sidl_BaseInterface *_ex) ;

void *
checkEntityTopoArraySize(struct TSTTM_EntityTopology__array **sidl_array, int num, sidl_BaseInterface *_ex) ;

void *
checkAdjacencyInfoArraySize(struct TSTTM_AdjacencyInfo__array **sidl_array, 
                         int num, sidl_BaseInterface *_ex);

enum TSTTM_AdjacencyInfo__enum 
NativeToTSTTAdjacencyInfo(AdjacencyCost cost);


void *
insertNativeDoubleArray(struct sidl_double__array **sidl_array, double *dbl_array, int num);
void *
insertNativeIntArray(struct sidl_int__array **sidl_array, int *int_array, int num);

void *
insertNativeEntTopoArray(struct TSTTM_EntityTopology__array **sidl_array, 
                         EntityTopology *topo_array, int num);

void *
insertNativeEntTypeArray(struct TSTTM_EntityType__array **sidl_array, 
                         EntityType *type_array, int num);


#define CHECK_SIDL_ARRAY_SIZE(class99,array99,num99,ex99) { \
  int sidl_array_size99 = class99##__array_upper(*array99,0)- \
                          class99##__array_lower(*array99,0)+1;\
\
  if ((sidl_array_size99 > num99 ) || (sidl_array_size99 == num99)) {\
    /* array size is big enough; do nothing */\
\
  } else if (sidl_array_size99 == 0) {\
    /* array size is 0, allocate the array to the correct size */\
    *array = class99##__array_create1d(num);\
\
  } else {\
    /* array size is too small - setting exception*/\
    TSTTB_Error err99 = TSTTB_Error__create();\
    if (err99) {\
      TSTTB_Error_set(err99, TSTTB_ErrorType_BAD_ARRAY_SIZE, "Bad Array Size");\
    }\
    *ex99 = sidl_BaseInterface__cast(err99);\
  }\
}


#ifdef __cplusplus__
}
#endif /* __cplusplus__ */

#endif /* TSTT_SIDL_CONVERTERS_H */
