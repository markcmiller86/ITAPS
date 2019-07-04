/*
 * File:          SimpleMesh_Mesh_Impl.h
 * Symbol:        SimpleMesh.Mesh-v0.7
 * Symbol Type:   class
 * Babel Version: 0.10.10
 * Description:   Server-side implementation for SimpleMesh.Mesh
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 * babel-version = 0.10.10
 */

#ifndef included_SimpleMesh_Mesh_Impl_h
#define included_SimpleMesh_Mesh_Impl_h

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_TSTTB_Error_h
#include "TSTTB_Error.h"
#endif
#ifndef included_SimpleMesh_Mesh_h
#include "SimpleMesh_Mesh.h"
#endif
#ifndef included_sidl_ClassInfo_h
#include "sidl_ClassInfo.h"
#endif
#ifndef included_TSTTM_Mesh_h
#include "TSTTM_Mesh.h"
#endif
#ifndef included_sidl_BaseInterface_h
#include "sidl_BaseInterface.h"
#endif
#ifndef included_sidl_BaseClass_h
#include "sidl_BaseClass.h"
#endif

/* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh._includes) */
#include "mesh.h"
/* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh._includes) */

/*
 * Private data for class SimpleMesh.Mesh
 */

struct SimpleMesh_Mesh__data {
  /* DO-NOT-DELETE splicer.begin(SimpleMesh.Mesh._data) */

/* wrap the mesh in the implementation using this pointer */
EntitySet   *meshPtr;

  /* DO-NOT-DELETE splicer.end(SimpleMesh.Mesh._data) */
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct SimpleMesh_Mesh__data*
SimpleMesh_Mesh__get_data(
  SimpleMesh_Mesh);

extern void
SimpleMesh_Mesh__set_data(
  SimpleMesh_Mesh,
  struct SimpleMesh_Mesh__data*);

extern
void
impl_SimpleMesh_Mesh__load(
  void);

extern
void
impl_SimpleMesh_Mesh__ctor(
  /* in */ SimpleMesh_Mesh self);

extern
void
impl_SimpleMesh_Mesh__dtor(
  /* in */ SimpleMesh_Mesh self);

/*
 * User-defined object methods
 */

extern struct TSTTB_Error__object* 
  impl_SimpleMesh_Mesh_fconnect_TSTTB_Error(char* url, sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_TSTTB_Error(struct 
  TSTTB_Error__object* obj);
extern struct SimpleMesh_Mesh__object* 
  impl_SimpleMesh_Mesh_fconnect_SimpleMesh_Mesh(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_SimpleMesh_Mesh(struct 
  SimpleMesh_Mesh__object* obj);
extern struct sidl_ClassInfo__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_ClassInfo(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj);
extern struct TSTTM_Mesh__object* 
  impl_SimpleMesh_Mesh_fconnect_TSTTM_Mesh(char* url, sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_TSTTM_Mesh(struct TSTTM_Mesh__object* 
  obj);
extern struct sidl_BaseInterface__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj);
extern struct sidl_BaseClass__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_BaseClass(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj);
extern
void
impl_SimpleMesh_Mesh_load(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set_handle,
  /* in */ const char* name,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_SimpleMesh_Mesh_save(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set_handle,
  /* in */ const char* name,
  /* out */ sidl_BaseInterface *_ex);

extern
void*
impl_SimpleMesh_Mesh_getRootSet(
  /* in */ SimpleMesh_Mesh self,
  /* out */ sidl_BaseInterface *_ex);

extern
int32_t
impl_SimpleMesh_Mesh_getGeometricDim(
  /* in */ SimpleMesh_Mesh self,
  /* out */ sidl_BaseInterface *_ex);

extern
enum TSTTM_StorageOrder__enum
impl_SimpleMesh_Mesh_getDfltStorage(
  /* in */ SimpleMesh_Mesh self,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_SimpleMesh_Mesh_getAdjTable(
  /* in */ SimpleMesh_Mesh self,
  /* inout array<TSTTM.AdjacencyInfo> */ struct TSTTM_AdjacencyInfo__array** 
    adjacency_table,
  /* out */ int32_t* adjacency_table_size,
  /* out */ sidl_BaseInterface *_ex);

extern
sidl_bool
impl_SimpleMesh_Mesh_areEHValid(
  /* in */ SimpleMesh_Mesh self,
  /* in */ sidl_bool reset,
  /* out */ sidl_BaseInterface *_ex);

extern
int32_t
impl_SimpleMesh_Mesh_getNumOfType(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set_handle,
  /* in */ enum TSTTM_EntityType__enum entity_type,
  /* out */ sidl_BaseInterface *_ex);

extern
int32_t
impl_SimpleMesh_Mesh_getNumOfTopo(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set_handle,
  /* in */ enum TSTTM_EntityTopology__enum entity_topology,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_SimpleMesh_Mesh_getAllVtxCoords(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set,
  /* inout array<double> */ struct sidl_double__array** coords,
  /* out */ int32_t* coords_size,
  /* inout array<int> */ struct sidl_int__array** in_entity_set,
  /* out */ int32_t* in_entity_set_size,
  /* inout */ enum TSTTM_StorageOrder__enum* storage_order,
  /* out */ sidl_BaseInterface *_ex);

extern
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
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_SimpleMesh_Mesh_getEntities(
  /* in */ SimpleMesh_Mesh self,
  /* in */ void* entity_set,
  /* in */ enum TSTTM_EntityType__enum entity_type,
  /* in */ enum TSTTM_EntityTopology__enum entity_topology,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_SimpleMesh_Mesh_getVtxArrCoords(
  /* in */ SimpleMesh_Mesh self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* inout */ enum TSTTM_StorageOrder__enum* storage_order,
  /* inout array<double> */ struct sidl_double__array** coords,
  /* out */ int32_t* coords_size,
  /* out */ sidl_BaseInterface *_ex);

extern
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
  /* out */ sidl_BaseInterface *_ex);

extern struct TSTTB_Error__object* 
  impl_SimpleMesh_Mesh_fconnect_TSTTB_Error(char* url, sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_TSTTB_Error(struct 
  TSTTB_Error__object* obj);
extern struct SimpleMesh_Mesh__object* 
  impl_SimpleMesh_Mesh_fconnect_SimpleMesh_Mesh(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_SimpleMesh_Mesh(struct 
  SimpleMesh_Mesh__object* obj);
extern struct sidl_ClassInfo__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_ClassInfo(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj);
extern struct TSTTM_Mesh__object* 
  impl_SimpleMesh_Mesh_fconnect_TSTTM_Mesh(char* url, sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_TSTTM_Mesh(struct TSTTM_Mesh__object* 
  obj);
extern struct sidl_BaseInterface__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj);
extern struct sidl_BaseClass__object* 
  impl_SimpleMesh_Mesh_fconnect_sidl_BaseClass(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_SimpleMesh_Mesh_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj);
#ifdef __cplusplus
}
#endif
#endif
