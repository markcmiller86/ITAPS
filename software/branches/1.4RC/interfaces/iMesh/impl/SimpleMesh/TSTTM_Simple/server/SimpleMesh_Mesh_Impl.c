/*
* File:          SimpleMesh_Mesh_Impl.c
* Symbol:        SimpleMesh.Mesh-v0.7
* Symbol Type:   class
* Babel Version: 0.10.10
* Description:   Server-side implementation for SimpleMesh.Mesh
* 
* WARNING: Automatically generated; only changes within splicers preserved
* 
* babel-version = 0.10.10
*/

/*
* DEVELOPERS ARE EXPECTED TO PROVIDE IMPLEMENTATIONS
* FOR THE FOLLOWING METHODS BETWEEN SPLICER PAIRS.
*/

/*
* Symbol "SimpleMesh.Mesh" (version 0.7)
*/

#include "SimpleMesh_Mesh_Impl.h"

/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh._includes) */
#include "TSTTmesh.h"
#include "sidl_Exception.h"
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh._includes) */

/*
* Static class initializer called exactly once before any user-defined method is dispatched
*/

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh__load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh__load(
void)
{
/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh._load) */
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh._load) */
}
/*
* Class constructor called when the class is created.
*/

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh__ctor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh__ctor(
/* in */ SimpleMesh_Mesh self)
{
/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh._ctor) */
  struct SimpleMesh_Mesh__data *dataPtr;
  dataPtr = (struct SimpleMesh_Mesh__data *)malloc(sizeof(struct SimpleMesh_Mesh__data));

  SimpleMesh_Mesh__set_data(self,dataPtr);
  if (dataPtr) {
    Error err = {0, ""};
    TSTT_create((void **)&dataPtr->meshPtr,&err);
  }
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh._ctor) */
}

/*
* Class destructor called when the class is deleted.
*/

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh__dtor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh__dtor(
/* in */ SimpleMesh_Mesh self)
{
/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh._dtor) */
  struct SimpleMesh_Mesh__data *dataPtr = SimpleMesh_Mesh__get_data(self);
  free(dataPtr->meshPtr);
  free(dataPtr);
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh._dtor) */
}

/*
* Method:  load[]
*/

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_load(
/* in */ SimpleMesh_Mesh self,
/* in */ void* entity_set_handle,
/* in */ const char* name,
/* out */ sidl_BaseInterface *_ex)
{
/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.load) */

  /* the native implementation error */
  Error err = {0, ""};
  /* the native call */
  TSTT_load(entity_set_handle,name,&err);
  /* convert the native error to a tstt exception */
  NativeToTSTTException(&err, _ex);

/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.load) */
}

/*
* Method:  save[]
*/

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_save"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_save(
/* in */ SimpleMesh_Mesh self,
/* in */ void* entity_set_handle,
/* in */ const char* name,
/* out */ sidl_BaseInterface *_ex)
{
/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.save) */
  /* the native implementation error */
  Error err = {0, ""};
  /* the native call */
  TSTT_save(entity_set_handle,name,&err);
  /* convert the native error to a tstt exception */
  NativeToTSTTException(&err, _ex);
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.save) */
}

/*
* Method:  getRootSet[]
*/

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getRootSet"

#ifdef __cplusplus
extern "C"
#endif
void*
impl_SimpleMesh_Mesh_getRootSet(
/* in */ SimpleMesh_Mesh self,
/* out */ sidl_BaseInterface *_ex)
{
/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getRootSet) */
  struct SimpleMesh_Mesh__data *dataPtr = SimpleMesh_Mesh__get_data(self);
  Error err = {0, ""};
  void *root_set = TSTT_getRootSet((void *)dataPtr->meshPtr,&err);
  NativeToTSTTException(&err, _ex);
  return(root_set);
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getRootSet) */
}

/*
 * Method:  getGeometricDim[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getGeometricDim"

#ifdef __cplusplus
extern "C"
#endif
int32_t
impl_SimpleMesh_Mesh_getGeometricDim(
  /* in */ SimpleMesh_Mesh self,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getGeometricDim) */

  struct SimpleMesh_Mesh__data *dataPtr = SimpleMesh_Mesh__get_data(self);
  Error err = {0, ""};
  int dim = TSTT_getGeometricDim((void *)dataPtr->meshPtr,&err);
  NativeToTSTTException(&err, _ex);
  return(dim);

  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getGeometricDim) */
}

/*
 * Method:  getDfltStorage[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getDfltStorage"

#ifdef __cplusplus
extern "C"
#endif
enum TSTTM_StorageOrder__enum
impl_SimpleMesh_Mesh_getDfltStorage(
  /* in */ SimpleMesh_Mesh self,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getDfltStorage) */
  struct SimpleMesh_Mesh__data *dataPtr = SimpleMesh_Mesh__get_data(self);

  /* make the native call */
  Error err = {0, ""};
  StorageOrder native_order = TSTT_getDfltStorage((void *)dataPtr->meshPtr, &err);

  /* convert the native entities to tstt entities */
  enum TSTTM_StorageOrder__enum order = NativeToTSTTStorageOrder(native_order);
  NativeToTSTTException(&err, _ex);

  return(order);
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getDfltStorage) */
}

/*
 * Method:  getAdjTable[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getAdjTable"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_getAdjTable(
  /* in */ SimpleMesh_Mesh self,
  /* inout array<TSTTM.AdjacencyInfo> */ struct TSTTM_AdjacencyInfo__array** 
    adjacency_table,
  /* out */ int32_t* adjacency_table_size,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getAdjTable) */
  struct SimpleMesh_Mesh__data *dataPtr = SimpleMesh_Mesh__get_data(self);
  Error err = {0, ""};

  AdjacencyCost *native_adj_table;
  TSTT_getAdjTable((void *)dataPtr->meshPtr, &native_adj_table, 
                   adjacency_table_size, &err);

  checkAdjacencyInfoArraySize(adjacency_table,*adjacency_table_size,_ex);
  if ( SIDL_CATCH(*_ex,"TSTTB.Error") ) {
    printf("Error with adjacency info array size\n");
    return;
  }

  int i;
  for (i=0;i<*adjacency_table_size;i++) {
    TSTTM_AdjacencyInfo__array_set1(*adjacency_table,i,
                    NativeToTSTTAdjacencyInfo(native_adj_table[i]));
  }

  NativeToTSTTException(&err, _ex);

  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getAdjTable) */
}

/*
 * Method:  areEHValid[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_areEHValid"

#ifdef __cplusplus
extern "C"
#endif
sidl_bool
impl_SimpleMesh_Mesh_areEHValid(
  /* in */ SimpleMesh_Mesh self,
  /* in */ sidl_bool reset,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.areEHValid) */
  return(1);
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.areEHValid) */
}

/*
 * Method:  getNumOfType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getNumOfType"

#ifdef __cplusplus
extern "C"
#endif
int32_t
impl_SimpleMesh_Mesh_getNumOfType(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set_handle,
  /* in */ enum TSTTM_EntityType__enum entity_type,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getNumOfType) */
  /* convert to the native enumerator */
  EntityType type = TSTTToNativeEntityType(entity_type);

  /* the native call */
  Error err = {0, ""};
  int num_type = TSTT_getNumOfType(entity_set_handle, type, &err);

  /* convert the native error to a tstt exception */
  NativeToTSTTException(&err, _ex);

  return(num_type);
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getNumOfType) */
}

/*
 * Method:  getNumOfTopo[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getNumOfTopo"

#ifdef __cplusplusn
extern "C"
#endif
int32_t
impl_SimpleMesh_Mesh_getNumOfTopo(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set_handle,
  /* in */ enum TSTTM_EntityTopology__enum entity_topology,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getNumOfTopo) */
  EntityTopology topo = TSTTToNativeEntityTopology(entity_topology);

  /* the native call */
  Error err = {0, ""};
  int num_topo = TSTT_getNumOfTopo(entity_set_handle, topo, &err);

  /* convert the native error to a tstt exception */
  NativeToTSTTException(&err, _ex);

  return(num_topo);
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getNumOfTopo) */
}

/*
 * Method:  getAllVtxCoords[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getAllVtxCoords"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_getAllVtxCoords(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set,
  /* inout array<double> */ struct sidl_double__array** coords,
  /* out */ int32_t* coords_size,
  /* inout array<int> */ struct sidl_int__array** in_entity_set,
  /* out */ int32_t* in_entity_set_size,
  /* inout */ enum TSTTM_StorageOrder__enum* storage_order,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getAllVtxCoords) */
  Error err = {0, ""};
  int *native_in_entityset = 0;
  double *native_coords = 0;

  /* convert the storage order */
  StorageOrder native_order = TSTTToNativeStorageOrder((*storage_order));

  /* check that the sidl arrays are the right size */
  int num_vtx = TSTT_getNumOfType(entity_set,VERTEX,&err);
  checkDoubleArraySize(coords,num_vtx*2,_ex);
  if ( SIDL_CATCH(*_ex,"TSTTB.Error") ) return;

  checkIntArraySize(in_entity_set,num_vtx,_ex);
  if ( SIDL_CATCH(*_ex,"TSTTB.Error") ) return;

  /* get the coordinates */
  TSTT_getAllVtxCoords(entity_set,&native_coords,coords_size,&native_in_entityset,
                       in_entity_set_size, &native_order, &err);

  /* convert the arrays to SIDL arrays*/
  insertNativeDoubleArray(coords,native_coords,*coords_size);
  insertNativeIntArray(in_entity_set,native_in_entityset,*in_entity_set_size);
  (*storage_order) = NativeToTSTTStorageOrder(native_order);

  /* convert the error */
  NativeToTSTTException(&err, _ex);

  /* free the local memory */
  /*  if (native_coords) free((void*)native_coords);
      if (native_in_entityset) free((void*)native_in_entityset); */

  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getAllVtxCoords) */
}

/*
 * Method:  getVtxCoordIndex[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getVtxCoordIndex"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_getVtxCoordIndex(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set,
  /* in */ enum TSTTM_EntityType__enum requested_entity_type,
  /* in */ enum TSTTM_EntityTopology__enum requested_entity_topology,
  /* in */ enum TSTTM_EntityType__enum entity_adjacency_type,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* inout array<int> */ struct sidl_int__array** index,
  /* out */ int32_t* index_size,
  /* inout array<TSTTM.EntityTopology> */ struct TSTTM_EntityTopology__array** 
    entity_topologies,
  /* out */ int32_t* entity_topologies_size,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getVtxCoordIndex) */
  Error err = {0, ""};
  int *native_offset = 0;
  int *native_index = 0;
  EntityTopology *native_topologies=0;

  /* convert the storage order */
  EntityTopology requested_topo = TSTTToNativeEntityTopology(requested_entity_topology);
  EntityType requested_type = TSTTToNativeEntityType(requested_entity_type);
  EntityType adj_type = TSTTToNativeEntityType(entity_adjacency_type);

  /* check that the sidl arrays are the right size */
  int num_vtx = TSTT_getNumOfType(entity_set,VERTEX,&err);
  int num_face = TSTT_getNumOfType(entity_set,FACE,&err);

  checkIntArraySize(index,num_face*3,_ex);
  if ( SIDL_CATCH(*_ex,"TSTTB.Error") ) {
    printf("bad array size for index\n");
    return;
  }

  checkIntArraySize(offset,num_face+1,_ex);
  if ( SIDL_CATCH(*_ex,"TSTTB.Error") ) {
    printf("bad array size for offset\n");
    return;
  }

  checkEntityTopoArraySize(entity_topologies,num_face,_ex);
  if ( SIDL_CATCH(*_ex,"TSTTB.Error") ) {
    printf("bad array size for entity_topo\n");
    return;
  }

  /* get the coordinate indices */
  TSTT_getVtxCoordIndex(entity_set,requested_type,requested_topo,adj_type,
                        &native_offset,offset_size,
                        &native_index,index_size,
                        &native_topologies,entity_topologies_size,&err);

  /* convert the arrays to SIDL arrays */
  insertNativeIntArray(offset,native_offset,*offset_size);
  insertNativeIntArray(index,native_index,*index_size);
  insertNativeEntTopoArray(entity_topologies,native_topologies,*entity_topologies_size);

  /* convert the error */
  NativeToTSTTException(&err, _ex);

  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getVtxCoordIndex) */
}

/*
 * Method:  getEntities[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getEntities"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_getEntities(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set,
  /* in */ enum TSTTM_EntityType__enum entity_type,
  /* in */ enum TSTTM_EntityTopology__enum entity_topology,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getEntities) */
  /* Insert-Code-Here {SimpleMesh.Mesh.getEntities} (getEntities method) */
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getEntities) */
}

/*
 * Method:  getVtxArrCoords[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getVtxArrCoords"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_getVtxArrCoords(
  /* in */ SimpleMesh_Mesh self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* inout */ enum TSTTM_StorageOrder__enum* storage_order,
  /* inout array<double> */ struct sidl_double__array** coords,
  /* out */ int32_t* coords_size,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getVtxArrCoords) */
  /* Insert-Code-Here {SimpleMesh.Mesh.getVtxArrCoords} (getVtxArrCoords method) */
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getVtxArrCoords) */
}

/*
 * Method:  getAdjEntities[]
 */

#undef __FUNC__
#define __FUNC__ "impl_SimpleMesh_Mesh_getAdjEntities"

#ifdef __cplusplus
extern "C"
#endif
void
impl_SimpleMesh_Mesh_getAdjEntities(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set,
  /* in */ enum TSTTM_EntityType__enum entity_type_requestor,
  /* in */ enum TSTTM_EntityTopology__enum entity_topology_requestor,
  /* in */ enum TSTTM_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* inout array<int> */ struct sidl_int__array** in_entity_set,
  /* out */ int32_t* in_entity_set_size,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh.getAdjEntities) */
  /* Insert-Code-Here {SimpleMesh.Mesh.getAdjEntities} (getAdjEntities method) */
  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh.getAdjEntities) */
}
/* Babel internal methods, Users should not edit below this line. */
struct TSTTB_Error__object* impl_SimpleMesh_Mesh_fconnect_TSTTB_Error(char* url,
  sidl_BaseInterface *_ex) {
  return TSTTB_Error__connect(url, _ex);
}
char * impl_SimpleMesh_Mesh_fgetURL_TSTTB_Error(struct TSTTB_Error__object* 
  obj) {
  return TSTTB_Error__getURL(obj);
}
struct SimpleMesh_Mesh__object* 
  impl_SimpleMesh_Mesh_fconnect_SimpleMesh_Mesh(char* url,
  sidl_BaseInterface *_ex) {
  return SimpleMesh_Mesh__connect(url, _ex);
}
char * impl_SimpleMesh_Mesh_fgetURL_SimpleMesh_Mesh(struct 
  SimpleMesh_Mesh__object* obj) {
  return SimpleMesh_Mesh__getURL(obj);
}
struct sidl_ClassInfo__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_ClassInfo(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_ClassInfo__connect(url, _ex);
}
char * impl_SimpleMesh_Mesh_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj) {
  return sidl_ClassInfo__getURL(obj);
}
struct TSTTM_Mesh__object* impl_SimpleMesh_Mesh_fconnect_TSTTM_Mesh(char* url,
  sidl_BaseInterface *_ex) {
  return TSTTM_Mesh__connect(url, _ex);
}
char * impl_SimpleMesh_Mesh_fgetURL_TSTTM_Mesh(struct TSTTM_Mesh__object* obj) {
  return TSTTM_Mesh__getURL(obj);
}
struct sidl_BaseInterface__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_BaseInterface__connect(url, _ex);
}
char * impl_SimpleMesh_Mesh_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj) {
  return sidl_BaseInterface__getURL(obj);
}
struct sidl_BaseClass__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_BaseClass(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_BaseClass__connect(url, _ex);
}
char * impl_SimpleMesh_Mesh_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj) {
  return sidl_BaseClass__getURL(obj);
}
