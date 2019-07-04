/*
 * File: TSTT_SIDL_Converters.c
 */

#include "TSTT_ArrayHelpers.h"
#include "NativeToTSTT.h"
#include "sidl_Exception.h"

/*
 * Create a SIDL array of opaques based on the local array of Tags.
 */
struct sidl_opaque__array*
NativeToTSTTTagArray(Tag **tags, int num) {
  struct sidl_opaque__array *res = NULL;
  if (num > 0) {
    int    i;
    res = sidl_opaque__array_create1d(num);
    memcpy(sidl_opaque__array_first(res), (void*)tags, 
           (size_t)(num*sizeof(Tag*)));
  }
  return res;
}

/*
 * Create a SIDL array of opaques based on the local Entity array.
 */
struct sidl_opaque__array*
NativeToTSTTEntityArray(Entity_list *entities, int num) {
  struct sidl_opaque__array *res   = sidl_opaque__array_create1d(num);
  Entity_list               *entry = entities;
  int i;
  for (i=0; (i<num) && (entry); i++) {
    sidl_opaque__array_set1(res, i, entry->entity);
    entry = entry->next;
  }
  return res;
}

/*
 * Create a local array of opaques based on the SIDL Entity array.
 */
Entity_list*
TSTTToNativeEntityArray(struct sidl_opaque__array* entities) {
  Entity_list *list, *entry;
  list = entry = NULL;
  int maxI = sidl_opaque__array_upper(entities,0);
  int i;
  for (i = sidl_opaque__array_lower(entities,0); i <= maxI; i++) {
    entry         = (Entity_list *)malloc(sizeof(Entity_list));
    entry->entity = sidl_opaque__array_get1(entities, i);
    entry->next   = list;
    list          = entry;
  }
  return list;
}

/*
 * Create a SIDL array of double based on the local double array.
 */
struct sidl_double__array*
NativeToTSTTDoubleArray(double *nbrs, int num) {
  struct sidl_double__array *res = NULL;
  if (num > 0) {
    int    i;
    res = sidl_double__array_create1d(num);
    memcpy(sidl_double__array_first(res), (void*)nbrs, 
           (size_t)(num*sizeof(double*)));
  }
  return res;
}

/*
 * Check the SIDL array size and return an error if too small
 */
void *
checkDoubleArraySize(struct sidl_double__array **sidl_array, int num, sidl_BaseInterface *_ex) 
{
  int sidl_array_size = sidl_double__array_upper(*sidl_array,0)-
                        sidl_double__array_lower(*sidl_array,0)+1; 

  if ((sidl_array_size > num ) || (sidl_array_size == num)) {
    /* array size is big enough; do nothing */

  } else if (sidl_array_size == 0) {
    /* array size is 0, allocate the array to the correct size */
    *sidl_array = sidl_double__array_create1d(num);

  } else {
    /* array size is too small - setting exception*/
    TSTTB_Error err = TSTTB_Error__create();
    if (err) {
      TSTTB_Error_set(err, TSTTB_ErrorType_BAD_ARRAY_SIZE, "Bad Array Size");
    }
    *_ex = sidl_BaseInterface__cast(err);
  }
}

/*
 * Check the SIDL array size and return an error if too small
 */
void *
checkAdjacencyInfoArraySize(struct TSTTM_AdjacencyInfo__array **sidl_array, int num, sidl_BaseInterface *_ex) 
{
  int sidl_array_size = TSTTM_AdjacencyInfo__array_upper(*sidl_array,0)-
                        TSTTM_AdjacencyInfo__array_lower(*sidl_array,0)+1; 

  if ((sidl_array_size > num ) || (sidl_array_size == num)) {
    /* array size is big enough; do nothing */

  } else if (sidl_array_size == 0) {
    /* array size is 0, allocate the array to the correct size */
    *sidl_array = TSTTM_AdjacencyInfo__array_create1d(num);

  } else {
    /* array size is too small - setting exception*/
    TSTTB_Error err = TSTTB_Error__create();
    if (err) {
      TSTTB_Error_set(err, TSTTB_ErrorType_BAD_ARRAY_SIZE, "Bad Array Size");
    }
    *_ex = sidl_BaseInterface__cast(err);
  }
}

/*
 * Create a SIDL array of double based on the local double array.
 */
void *
insertNativeDoubleArray(struct sidl_double__array **sidl_array, double *dbl_array, int num) {
  int i;

  memcpy(sidl_double__array_first(*sidl_array), (void*)dbl_array, 
    (size_t)(num*sizeof(double))); 
}


 /*
 * Check the SIDL array size and return an error if too small
 */
void *
checkIntArraySize(struct sidl_int__array **sidl_array, int num, sidl_BaseInterface *_ex) 
{
  int sidl_array_size = sidl_int__array_upper(*sidl_array,0)-
                        sidl_int__array_lower(*sidl_array,0)+1; 

  if ((sidl_array_size > num ) || (sidl_array_size == num)) {
    /* array size is big enough; do nothing */

  } else if (sidl_array_size == 0) {
    /* array size is 0, allocate the array to the correct size */
    *sidl_array = sidl_int__array_create1d(num);

  } else {
    /* array size is too small - setting exception*/
    TSTTB_Error err = TSTTB_Error__create();
    if (err) {
      TSTTB_Error_set(err, TSTTB_ErrorType_BAD_ARRAY_SIZE, "Bad Array Size");
    }
    *_ex = sidl_BaseInterface__cast(err);
  }
}

/*
 * Create a SIDL array of integers based on the native array.
 */
void *
insertNativeIntArray(struct sidl_int__array **sidl_array, int *int_array, int num) 
{
    memcpy(sidl_int__array_first(*sidl_array), (void*)int_array, 
           (size_t)(num*sizeof(int)));
}

/*
 * Create a SIDL array of entity topologies based on the native array.
 */
void *
insertNativeEntTopoArray(struct TSTTM_EntityTopology__array **sidl_array, EntityTopology *topo_array, int num) 
{

  int i;
  for (i=0;i<num;i++) {
    TSTTM_EntityTopology__array_set1(*sidl_array, i, 
                    NativeToTSTTEntityTopology(topo_array[i]));
  }
}

/*
 * Create a SIDL array of entity types based on the native array.
 */
void *
insertNativeEntTypeArray(struct TSTTM_EntityType__array **sidl_array, EntityType *type_array, int num) 
{

  int i;
  for (i=0;i<num;i++) {
    TSTTM_EntityType__array_set1(*sidl_array, i, 
                    NativeToTSTTEntityType(type_array[i]));
  }
}


/*
 * Check the SIDL array size and return an error if too small
 */
void *
checkEntityTypeArraySize(struct TSTTM_EntityType__array **sidl_array, int num, sidl_BaseInterface *_ex) 
{
  int sidl_array_size = TSTTM_EntityType__array_upper(*sidl_array,0)-
                        TSTTM_EntityType__array_lower(*sidl_array,0)+1; 

  if ((sidl_array_size > num ) || (sidl_array_size == num)) {
    /* array size is big enough; do nothing */

  } else if (sidl_array_size == 0) {
    /* array size is 0, allocate the array to the correct size */
    *sidl_array = TSTTM_EntityType__array_create1d(num);

  } else {
    /* array size is too small - setting exception*/
    TSTTB_Error err = TSTTB_Error__create();
    if (err) {
      TSTTB_Error_set(err, TSTTB_ErrorType_BAD_ARRAY_SIZE, "Bad Array Size");
    }
    *_ex = sidl_BaseInterface__cast(err);
  }
}


/*
 * Check the SIDL array size and return an error if too small
 */
void *
checkEntityTopoArraySize(struct TSTTM_EntityTopology__array **sidl_array, int num, sidl_BaseInterface *_ex) 
{
  int sidl_array_size = TSTTM_EntityTopology__array_upper(*sidl_array,0)-
                        TSTTM_EntityTopology__array_lower(*sidl_array,0)+1; 

  if ((sidl_array_size > num ) || (sidl_array_size == num)) {
    /* array size is big enough; do nothing */

  } else if (sidl_array_size == 0) {
    /* array size is 0, allocate the array to the correct size */
    *sidl_array = TSTTM_EntityTopology__array_create1d(num);

  } else {
    /* array size is too small - setting exception*/
    TSTTB_Error err = TSTTB_Error__create();
    if (err) {
      TSTTB_Error_set(err, TSTTB_ErrorType_BAD_ARRAY_SIZE, "Bad Array Size");
    }
    *_ex = sidl_BaseInterface__cast(err);
  }
}



/*
 * Create a local array of double based on the SIDL double array.
 */
double*
TSTTToNativeDoubleArray(struct sidl_double__array *nbrs) {
  double  *list = NULL;
  int32_t num   = M_ARRAY_SIZE(sidl_double, nbrs);
  if (num > 0) {
    int i;
    list = (double *)malloc(num * sizeof(double));
    memcpy(list, sidl_double__array_first(nbrs), (size_t)(num*sizeof(double)));
  }
  return list;
}

/*
 * Create a SIDL array of int based on the local int array.
 */
struct sidl_int__array*
NativeToTSTTIntArray(int *nbrs, int num) {
  struct sidl_int__array *res = NULL;
  if (num > 0) {
    int    i;
    res = sidl_int__array_create1d(num);
    memcpy(sidl_int__array_first(res), (void*)nbrs, (size_t)(num*sizeof(int*)));
  }
  return res;
}

/*
 * Create a local array of int based on the SIDL int array.
 */
int*
TSTTToNativeIntArray(struct sidl_int__array *nbrs) {
  int32_t num   = M_ARRAY_SIZE(sidl_int, nbrs);
  int     *list = NULL;
  if (num > 0) {
    int i;
    list = (int *)malloc(num * sizeof(int));
    memcpy(list, sidl_int__array_first(nbrs), (size_t)(num*sizeof(int)));
  }
  return list;
}

/*
 * Create a SIDL array of opaques based on the local opaque array.
 */
struct sidl_opaque__array*
NativeToTSTTOpaqueArray(void **ptrs, int num) {
  struct sidl_opaque__array *res = NULL;
  if (num > 0) {
    int    i;
    res = sidl_opaque__array_create1d(num);
    memcpy(sidl_opaque__array_first(res), (void*)ptrs, 
           (size_t)(num*sizeof(void*)));
  }
  return res;
}

/*
 * Create a local array of opaques based on the SIDL opaque array.
 */
void**
TSTTToNativeOpaqueArray(struct sidl_opaque__array *ptrs, int ptrsize) {
  int32_t num    = M_ARRAY_SIZE(sidl_opaque, ptrs);
  void    **list = NULL;
  if (num > 0) {
    if (ptrsize == sizeof(int)) {
      list = (void **)malloc(num * sizeof(int));
    } else {
      list = (void **)malloc(num * sizeof(double));
    }
    int i;
    for (i=0; i<=num; i++) {
      list[i] = sidl_opaque__array_get1(ptrs, i);
    }
  }
  return list;
}

/*
 * Create a SIDL array of opaques based on the local Entity Handle array.
 */
struct sidl_opaque__array*
NativeToTSTTEntityHandleArray(EntityHandle **handles,
                             int num)
{
  struct sidl_opaque__array *res = NULL;
  if (num > 0) {
    int    i;
    res = sidl_opaque__array_create1d(num);
    memcpy(sidl_opaque__array_first(res), (void*)handles, 
           (size_t)(num*sizeof(EntityHandle*)));
  }
  return res;
}

/*
 * Create a local array of opaques based on the SIDL Entity Handle array.
 */
EntityHandle**
TSTTToNativeEntityHandleArray(struct sidl_opaque__array *handles)
{
  EntityHandle **list = NULL;
  int32_t      num    = M_ARRAY_SIZE(sidl_opaque, handles);
  if (num > 0) {
    int     i;
    list = (EntityHandle **)malloc(num * sizeof(EntityHandle));
    memcpy(list, sidl_opaque__array_first(handles), 
           (size_t)(num*sizeof(EntityHandle*)));
  }
  return list;
}

/*
 * Create a SIDL array of opaques based on the local EntitySet array.
 */
struct sidl_opaque__array*
NativeToTSTTEntitySetArray(EntitySet_list *sets, int num) {
  struct sidl_opaque__array *res   = sidl_opaque__array_create1d(num);
  EntitySet_list            *entry = sets;
  int i;
  for (i=0; i<num; i++) {
    sidl_opaque__array_set1(res, i, entry->entity_set);
    entry = entry->next;
  }
  return res;
}

/*
 * Create a local array of opaques based on the SIDL EntitySet array.
 */
EntitySet_list*
TSTTToNativeEntitySetArray(struct sidl_opaque__array* sets) {
  EntitySet_list *list, *entry;
  list = entry = NULL;
  int i;
  int maxI = sidl_opaque__array_lower(sets,0);
  for (i =  sidl_opaque__array_upper(sets,0); i <= maxI; i++) {
    entry             = (EntitySet_list *)malloc(sizeof(EntitySet_list));
    entry->entity_set = sidl_opaque__array_get1(sets, i);
    entry->next       = list;
    list              = entry;
  }
  return list;
}

/*
 * Create a new TSTTB_Error exception using the specified error code
 * and message.
 */
void
newTSTTException(int32_t code, char *msg, TSTTB_Error *_ex) {
  TSTTB_Error err = TSTTB_Error__create();
  if (err) {
    TSTTB_Error_set(err, code, msg);
  }
  _ex = &err;
}

/*
 * Convert the specified local Error into a SIDL exception, deleting/
 * freeing the local error.
 */
void
NativeToTSTTException(Error *err, sidl_BaseInterface *_ex) {
  if (err) {
    if (err->code) {
      newTSTTException(err->code, err->msg, (TSTTB_Error*)_ex);
    } else {
      (*_ex) = NULL;
    }
    //if (err->msg) free(err->msg);
    //free(err);
  } else {
    (*_ex) = NULL;
  }
}

/*
 * Convert from the local StorageOrder enumeration to the TSTT one.
 */
enum TSTTM_StorageOrder__enum
NativeToTSTTStorageOrder(StorageOrder order) {
  enum TSTTM_StorageOrder__enum tOrder;
  switch (order) {
    case BLOCKED:
      tOrder = TSTTM_StorageOrder_BLOCKED;
      break;
    case INTERLEAVED:
      tOrder = TSTTM_StorageOrder_INTERLEAVED;
      break;
    case UNDETERMINED:
      tOrder = TSTTM_StorageOrder_UNDETERMINED;
      break;
    default:
      tOrder = TSTTM_StorageOrder_UNDETERMINED;
      break;
  }
  return tOrder;
}

/*
 * Convert from the local StorageOrder enumeration to the TSTT one.
 */
StorageOrder
TSTTToNativeStorageOrder(enum TSTTM_StorageOrder__enum order) {
  StorageOrder lOrder;
  switch (order) {
    case TSTTM_StorageOrder_BLOCKED:
      lOrder = BLOCKED;
      break;
    case TSTTM_StorageOrder_INTERLEAVED:
      lOrder = INTERLEAVED;
      break;
    case TSTTM_StorageOrder_UNDETERMINED:
      lOrder = UNDETERMINED;
      break;
    default:
      lOrder = UNDETERMINED;
      break;
  }
  return lOrder;
}

/*
 * Convert from the local EntityType enumeration to the TSTT one.
 */
enum TSTTM_EntityType__enum
NativeToTSTTEntityType(EntityType type) {
  enum TSTTM_EntityType__enum tType;
  switch (type) {
    case VERTEX:
      tType = TSTTM_EntityType_VERTEX;
      break;
    case EDGE:
      tType = TSTTM_EntityType_EDGE;
      break;
    case FACE:
      tType = TSTTM_EntityType_FACE;
      break;
    case REGION:
      tType = TSTTM_EntityType_REGION;
      break;
    case ALL_TYPES:
      tType = TSTTM_EntityType_ALL_TYPES;
      break;
    default:
      tType = TSTTM_EntityType_ALL_TYPES;
      break;
  }
  return tType;
}


/*
 * Convert from the native AdjacencyCost enumeration to the TSTT one.
 */
enum TSTTM_AdjacencyInfo__enum
NativeToTSTTAdjacencyInfo(AdjacencyCost cost) {
  enum TSTTM_AdjacencyInfo__enum tstt_info;
  switch (cost) {
    case UNAVAILABLE:
      tstt_info = TSTTM_AdjacencyInfo_UNAVAILABLE;
      break;
    case IMMEDIATE:
      tstt_info = TSTTM_AdjacencyInfo_ALL_ORDER_1;
      break;
    case LOCAL_TRAVERSAL:
      tstt_info = TSTTM_AdjacencyInfo_ALL_ORDER_1;
      break;
    case GLOBAL_TRAVERSAL:
      tstt_info = TSTTM_AdjacencyInfo_ALL_ORDER_N;
      break;
    default:
      tstt_info = TSTTM_AdjacencyInfo_UNAVAILABLE;
      break;
  }
  return tstt_info;
}


/*
 * Convert from the TSTT EntityType enumeration to the local one.
 */
EntityType
TSTTToNativeEntityType(enum TSTTM_EntityType__enum type) {
  EntityType lType;
  switch (type) {
    case TSTTM_EntityType_VERTEX:
      lType = VERTEX;
      break;
    case TSTTM_EntityType_EDGE:
      lType = EDGE;
      break;
    case TSTTM_EntityType_FACE:
      lType = FACE;
      break;
    case TSTTM_EntityType_REGION:
      lType = REGION;
      break;
    case TSTTM_EntityType_ALL_TYPES:
      lType = ALL_TYPES;
      break;
    default:
      lType = ALL_TYPES;
      break;
  }
  return lType;
}

/*
 * Create a SIDL array of entity types based on the local array.
 */
struct TSTTM_EntityType__array*
NativeToTSTTEntityTypeArray(EntityType *types, int num)
{
  struct TSTTM_EntityType__array   *res;
  if (num > 0) {
    int    i;
    res = TSTTM_EntityType__array_create1d(num);
    for (i=0; i<num; i++) {
      TSTTM_EntityType__array_set1(res, (int32_t)i,
                                  NativeToTSTTEntityType(types[i]));
    }
  }
  return res;
}

/*
 * Convert from the local EntityTopology enumeration to the TSTT one.
 */
enum TSTTM_EntityTopology__enum
NativeToTSTTEntityTopology(EntityTopology topo) {
  enum TSTTM_EntityTopology__enum tTopo;
  switch (topo) {
    case POINT:
      tTopo = TSTTM_EntityTopology_POINT;
      break;
    case LINE_SEGMENT:
      tTopo = TSTTM_EntityTopology_LINE_SEGMENT;
      break;
    case POLYGON:
      tTopo = TSTTM_EntityTopology_POLYGON;
      break;
    case TRIANGLE:
      tTopo = TSTTM_EntityTopology_TRIANGLE;
      break;
    case QUADRILATERAL:
      tTopo = TSTTM_EntityTopology_QUADRILATERAL;
      break;
    case POLYHEDRON:
      tTopo = TSTTM_EntityTopology_POLYHEDRON;
      break;
    case TETRAHEDRON:
      tTopo = TSTTM_EntityTopology_TETRAHEDRON;
      break;
    case HEXAHEDRON:
      tTopo = TSTTM_EntityTopology_HEXAHEDRON;
      break;
    case PRISM:
      tTopo = TSTTM_EntityTopology_PRISM;
      break;
    case PYRAMID:
      tTopo = TSTTM_EntityTopology_PYRAMID;
      break;
    case SEPTAHEDRON:
      tTopo = TSTTM_EntityTopology_SEPTAHEDRON;
      break;
    case ALL_TOPOLOGIES:
      tTopo = TSTTM_EntityTopology_ALL_TOPOLOGIES;
      break;
    default:
      tTopo = TSTTM_EntityTopology_ALL_TOPOLOGIES;
      break;
  }
  return tTopo;
}

/*
 * Convert from the TSTT EntityTopology enumeration to the local one.
 */
EntityTopology
TSTTToNativeEntityTopology(enum TSTTM_EntityTopology__enum topo) {
  EntityTopology lTopo;
  switch (topo) {
    case TSTTM_EntityTopology_POINT:
      lTopo = POINT;
      break;
    case TSTTM_EntityTopology_LINE_SEGMENT:
      lTopo = LINE_SEGMENT;
      break;
    case TSTTM_EntityTopology_POLYGON:
      lTopo = POLYGON;
      break;
    case TSTTM_EntityTopology_TRIANGLE:
      lTopo = TRIANGLE;
      break;
    case TSTTM_EntityTopology_QUADRILATERAL:
      lTopo = QUADRILATERAL;
      break;
    case TSTTM_EntityTopology_POLYHEDRON:
      lTopo = POLYHEDRON;
      break;
    case TSTTM_EntityTopology_TETRAHEDRON:
      lTopo = TETRAHEDRON;
      break;
    case TSTTM_EntityTopology_HEXAHEDRON:
      lTopo = HEXAHEDRON;
      break;
    case TSTTM_EntityTopology_PRISM:
      lTopo = PRISM;
      break;
    case TSTTM_EntityTopology_PYRAMID:
      lTopo = PYRAMID;
      break;
    case TSTTM_EntityTopology_SEPTAHEDRON:
      lTopo = SEPTAHEDRON;
      break;
    case TSTTM_EntityTopology_ALL_TOPOLOGIES:
      lTopo = ALL_TOPOLOGIES;
      break;
    default:
      lTopo = ALL_TOPOLOGIES;
      break;
  }
  return lTopo;
}

/*
 * Create a SIDL array of entity topologies based on the local array.
 */
struct TSTTM_EntityTopology__array*
NativeToTSTTEntityTopologyArray(EntityTopology *topos, int num) {
  struct TSTTM_EntityTopology__array *res =
                                       TSTTM_EntityTopology__array_create1d(num);  int i;
  for (i=0; i<num; i++) {
    TSTTM_EntityTopology__array_set1(res, i,
                                    NativeToTSTTEntityTopology(topos[i]));
  }
  return res;
}
