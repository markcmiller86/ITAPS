/*
 * File:          iMeshSIDL_MeshSidl_Impl.h
 * Symbol:        iMeshSIDL.MeshSidl-v0.2
 * Symbol Type:   class
 * Babel Version: 1.4.0 (Revision: 6607 release-1-4-0-branch)
 * Description:   Server-side implementation for iMeshSIDL.MeshSidl
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

#ifndef included_iMeshSIDL_MeshSidl_Impl_h
#define included_iMeshSIDL_MeshSidl_Impl_h

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_iBase_ArrTag_h
#include "iBase_ArrTag.h"
#endif
#ifndef included_iBase_EntSet_h
#include "iBase_EntSet.h"
#endif
#ifndef included_iBase_EntTag_h
#include "iBase_EntTag.h"
#endif
#ifndef included_iBase_Error_h
#include "iBase_Error.h"
#endif
#ifndef included_iBase_SetBoolOps_h
#include "iBase_SetBoolOps.h"
#endif
#ifndef included_iBase_SetRelation_h
#include "iBase_SetRelation.h"
#endif
#ifndef included_iBase_SetTag_h
#include "iBase_SetTag.h"
#endif
#ifndef included_iBase_Tag_h
#include "iBase_Tag.h"
#endif
#ifndef included_iMesh_Arr_h
#include "iMesh_Arr.h"
#endif
#ifndef included_iMesh_ArrMod_h
#include "iMesh_ArrMod.h"
#endif
#ifndef included_iMesh_Entity_h
#include "iMesh_Entity.h"
#endif
#ifndef included_iMesh_Mesh_h
#include "iMesh_Mesh.h"
#endif
#ifndef included_iMesh_Modify_h
#include "iMesh_Modify.h"
#endif
#ifndef included_iMeshSIDL_MeshSidl_h
#include "iMeshSIDL_MeshSidl.h"
#endif
#ifndef included_sidl_BaseClass_h
#include "sidl_BaseClass.h"
#endif
#ifndef included_sidl_BaseInterface_h
#include "sidl_BaseInterface.h"
#endif
#ifndef included_sidl_ClassInfo_h
#include "sidl_ClassInfo.h"
#endif
#ifndef included_sidl_RuntimeException_h
#include "sidl_RuntimeException.h"
#endif
#line 73 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.h"
/* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._hincludes) */
/* insert code here (include files) */
#include "iMesh.h"
#include "iBase_ErrorActions.h"
/* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._hincludes) */
#line 78 "server/iMeshSIDL_MeshSidl_Impl.h"

/*
 * Private data for class iMeshSIDL.MeshSidl
 */

struct iMeshSIDL_MeshSidl__data {
#line 86 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.h"
  /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._data) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl._data} (private data members) */
    int mLastError;
    iMesh_Instance mInstance;
    enum iBase_ErrorActions__enum mErrorAction;
  /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._data) */
#line 92 "server/iMeshSIDL_MeshSidl_Impl.h"
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct iMeshSIDL_MeshSidl__data*
iMeshSIDL_MeshSidl__get_data(
  iMeshSIDL_MeshSidl);

extern void
iMeshSIDL_MeshSidl__set_data(
  iMeshSIDL_MeshSidl,
  struct iMeshSIDL_MeshSidl__data*);

extern
void
impl_iMeshSIDL_MeshSidl__load(
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl__ctor(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl__ctor2(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl__dtor(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ sidl_BaseInterface *_ex);

/*
 * User-defined object methods
 */

#ifdef WITH_RMI
extern struct sidl_BaseInterface__object* 
  impl_iMeshSIDL_MeshSidl_fconnect_sidl_BaseInterface(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
#endif /*WITH_RMI*/
extern
void
impl_iMeshSIDL_MeshSidl_createTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ const char* tag_name,
  /* in */ int32_t number_of_values,
  /* in */ enum iBase_TagValueType__enum tag_type,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_destroyTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* in */ sidl_bool forced,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getTagName(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ char** tag_name,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getTagSizeValues(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_values,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getTagSizeBytes(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_bytes,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getTagHandle(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ const char* tag_name,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getTagType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ enum iBase_TagValueType__enum* tag_data_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getAllTags(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getIntArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<int> */ struct sidl_int__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getDblArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<double> */ struct sidl_double__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEHArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setIntArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<int> */ struct sidl_int__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setDblArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<double> */ struct sidl_double__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setEHArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<opaque> */ struct sidl_opaque__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvArrTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setEntSetData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setEntSetIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setEntSetDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setEntSetEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntSetData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntSetIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntSetDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntSetEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getAllEntSetTags(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvEntSetTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_createEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ sidl_bool isList,
  /* out */ void** entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_destroyEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_isList(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* out */ int32_t* is_list,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNumEntSets(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_sets,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntSets(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** 
    contained_entset_handles,
  /* out */ int32_t* contained_entset_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_addEntToSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvEntFromSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_addEntArrToSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvEntArrFromSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_addEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_to_add,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_to_remove,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_isEntContained(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* entity_handle,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_isEntArrContained(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* containing_set,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** is_contained,
  /* out */ int32_t* is_contained_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_isEntSetContained(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* contained_entity_set,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_addPrntChld(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_rmvPrntChld(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_isChildOf(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ int32_t* is_child,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNumChld(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_child,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNumPrnt(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_parent,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getChldn(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** child_handles,
  /* out */ int32_t* child_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getPrnts(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** parent_handles,
  /* out */ int32_t* parent_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_subtract(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_intersect(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_unite(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_initEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* in */ enum iMesh_EntityTopology__enum requested_entity_topology,
  /* out */ void** entity_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNextEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_iterator,
  /* out */ void** entity_handle,
  /* out */ int32_t* at_end,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_resetEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_endEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntTopo(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* out */ enum iMesh_EntityTopology__enum* ent_topo,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* out */ enum iBase_EntityType__enum* ent_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getVtxCoord(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* vertex_handle,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntAdj(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEnt2ndAdj(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ enum iBase_EntityType__enum order_adjacent_key,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_load(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_save(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getRootSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ void** root_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getGeometricDim(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ int32_t* dim,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setGeometricDim(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ int32_t dim,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getDfltStorage(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ enum iBase_StorageOrder__enum* dflt_storage,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getAdjTable(
  /* in */ iMeshSIDL_MeshSidl self,
  /* inout array<iMesh.AdjacencyInfo> */ struct iMesh_AdjacencyInfo__array** 
    adjacency_table,
  /* out */ int32_t* adjacency_table_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_areEHValid(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ int32_t reset,
  /* out */ int32_t* are_valid,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNumOfType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum entity_type,
  /* out */ int32_t* num_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNumOfTopo(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iMesh_EntityTopology__enum entity_topology,
  /* out */ int32_t* num_topo,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntities(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ enum iBase_EntityType__enum entity_type,
  /* in */ enum iMesh_EntityTopology__enum entity_topology,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getVtxArrCoords(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** coords,
  /* out */ int32_t* coords_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getAdjEntIndices(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum entity_type_requestor,
  /* in */ enum iMesh_EntityTopology__enum entity_topology_requestor,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** adj_entity_indices,
  /* out */ int32_t* adj_entity_indices_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_initEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* in */ enum iMesh_EntityTopology__enum requested_entity_topology,
  /* in */ int32_t requested_array_size,
  /* out */ void** entArr_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getNextEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entArr_iterator,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ int32_t* at_end,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_resetEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_endEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntArrTopo(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<iMesh.EntityTopology> */ struct iMesh_EntityTopology__array** 
    topology,
  /* out */ int32_t* topology_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntArrType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<iBase.EntityType> */ struct iBase_EntityType__array** type,
  /* out */ int32_t* type_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntArrAdj(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_getEntArr2ndAdj(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_EntityType__enum order_adjacent_key,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setVtxCoord(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* vertex_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_createVtx(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ void** new_vertex_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_createEnt(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ enum iMesh_EntityTopology__enum new_entity_topology,
  /* in array<opaque> */ struct sidl_opaque__array* lower_order_entity_handles,
  /* in */ int32_t lower_order_entity_handles_size,
  /* out */ void** new_entity_handle,
  /* out */ enum iBase_CreationStatus__enum* status,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_deleteEnt(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_setVtxArrCoords(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* new_coords,
  /* in */ int32_t new_coords_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_createVtxArr(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ int32_t num_verts,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* new_coords,
  /* in */ int32_t new_coords_size,
  /* inout array<opaque> */ struct sidl_opaque__array** new_vertex_handles,
  /* out */ int32_t* new_vertex_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_createEntArr(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ enum iMesh_EntityTopology__enum new_entity_topology,
  /* in array<opaque> */ struct sidl_opaque__array* lower_order_entity_handles,
  /* in */ int32_t lower_order_entity_handles_size,
  /* inout array<opaque> */ struct sidl_opaque__array** new_entity_handles,
  /* out */ int32_t* new_entity_handles_size,
  /* inout array<iBase.CreationStatus> */ struct iBase_CreationStatus__array** 
    status,
  /* out */ int32_t* status_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iMeshSIDL_MeshSidl_deleteEntArr(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* out */ sidl_BaseInterface *_ex);

#ifdef WITH_RMI
extern struct sidl_BaseInterface__object* 
  impl_iMeshSIDL_MeshSidl_fconnect_sidl_BaseInterface(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
#endif /*WITH_RMI*/

#line 1082 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.h"
/* DO-NOT-DELETE splicer.begin(_hmisc) */
/* insert code here (miscellaneous things) */
/* DO-NOT-DELETE splicer.end(_hmisc) */
#line 1071 "server/iMeshSIDL_MeshSidl_Impl.h"

#ifdef __cplusplus
}
#endif
#endif
