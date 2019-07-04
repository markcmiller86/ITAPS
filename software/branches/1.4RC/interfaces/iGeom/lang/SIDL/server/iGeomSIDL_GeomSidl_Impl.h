/*
 * File:          iGeomSIDL_GeomSidl_Impl.h
 * Symbol:        iGeomSIDL.GeomSidl-v0.1
 * Symbol Type:   class
 * Babel Version: 1.4.0 (Revision: 6607 release-1-4-0-branch)
 * Description:   Server-side implementation for iGeomSIDL.GeomSidl
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

#ifndef included_iGeomSIDL_GeomSidl_Impl_h
#define included_iGeomSIDL_GeomSidl_Impl_h

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
#ifndef included_iGeom_Booleans_h
#include "iGeom_Booleans.h"
#endif
#ifndef included_iGeom_Construct_h
#include "iGeom_Construct.h"
#endif
#ifndef included_iGeom_CoreQuery_h
#include "iGeom_CoreQuery.h"
#endif
#ifndef included_iGeom_Geometry_h
#include "iGeom_Geometry.h"
#endif
#ifndef included_iGeom_Iterators_h
#include "iGeom_Iterators.h"
#endif
#ifndef included_iGeom_Modify_h
#include "iGeom_Modify.h"
#endif
#ifndef included_iGeom_Parametric_h
#include "iGeom_Parametric.h"
#endif
#ifndef included_iGeom_Primitives_h
#include "iGeom_Primitives.h"
#endif
#ifndef included_iGeom_Shape_h
#include "iGeom_Shape.h"
#endif
#ifndef included_iGeom_Tolerance_h
#include "iGeom_Tolerance.h"
#endif
#ifndef included_iGeom_Topology_h
#include "iGeom_Topology.h"
#endif
#ifndef included_iGeom_Transforms_h
#include "iGeom_Transforms.h"
#endif
#ifndef included_iGeomSIDL_GeomSidl_h
#include "iGeomSIDL_GeomSidl.h"
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
#line 94 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.h"
/* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._hincludes) */
/* insert code here (include files) */
#include "iGeom.h"
#include "iBase_ErrorActions.h"
/* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._hincludes) */
#line 99 "server/iGeomSIDL_GeomSidl_Impl.h"

/*
 * Private data for class iGeomSIDL.GeomSidl
 */

struct iGeomSIDL_GeomSidl__data {
#line 107 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.h"
  /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._data) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl._data} (private data members) */
    int mLastError;
    iGeom_Instance mInstance;
    enum iBase_ErrorActions__enum mErrorAction;
  /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._data) */
#line 113 "server/iGeomSIDL_GeomSidl_Impl.h"
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct iGeomSIDL_GeomSidl__data*
iGeomSIDL_GeomSidl__get_data(
  iGeomSIDL_GeomSidl);

extern void
iGeomSIDL_GeomSidl__set_data(
  iGeomSIDL_GeomSidl,
  struct iGeomSIDL_GeomSidl__data*);

extern
void
impl_iGeomSIDL_GeomSidl__load(
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl__ctor(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl__ctor2(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl__dtor(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ sidl_BaseInterface *_ex);

/*
 * User-defined object methods
 */

#ifdef WITH_RMI
extern struct sidl_BaseInterface__object* 
  impl_iGeomSIDL_GeomSidl_fconnect_sidl_BaseInterface(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
#endif /*WITH_RMI*/
extern
void
impl_iGeomSIDL_GeomSidl_createTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* tag_name,
  /* in */ int32_t number_of_values,
  /* in */ enum iBase_TagValueType__enum tag_type,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_destroyTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* in */ sidl_bool forced,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTagName(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ char** tag_name,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTagSizeValues(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_values,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTagSizeBytes(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_bytes,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTagHandle(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* tag_name,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTagType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ enum iBase_TagValueType__enum* tag_data_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getAllTags(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getIntArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<int> */ struct sidl_int__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getDblArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<double> */ struct sidl_double__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEHArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setIntArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<int> */ struct sidl_int__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setDblArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<double> */ struct sidl_double__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setEHArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<opaque> */ struct sidl_opaque__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvArrTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setEntSetData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setEntSetIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setEntSetDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_setEntSetEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntSetData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntSetIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntSetDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntSetEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getAllEntSetTags(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvEntSetTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_createEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ sidl_bool isList,
  /* out */ void** entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_destroyEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isList(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* out */ int32_t* is_list,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getNumEntSets(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_sets,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntSets(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** 
    contained_entset_handles,
  /* out */ int32_t* contained_entset_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_addEntToSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvEntFromSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_addEntArrToSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvEntArrFromSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_addEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_to_add,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_to_remove,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isEntContained(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* entity_handle,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isEntArrContained(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* containing_set,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** is_contained,
  /* out */ int32_t* is_contained_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isEntSetContained(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* contained_entity_set,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_addPrntChld(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rmvPrntChld(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isChildOf(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ int32_t* is_child,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getNumChld(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_child,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getNumPrnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_parent,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getChldn(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** child_handles,
  /* out */ int32_t* child_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getPrnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** parent_handles,
  /* out */ int32_t* parent_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_subtract(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_intersect(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_unite(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_load(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_save(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntities(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* set_handle,
  /* in */ enum iBase_EntityType__enum gentity_type,
  /* inout array<opaque> */ struct sidl_opaque__array** gentity_handles,
  /* out */ int32_t* gentity_handles_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getNumOfType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* set_handle,
  /* in */ enum iBase_EntityType__enum gentity_type,
  /* out */ int32_t* num_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* handle,
  /* out */ enum iBase_EntityType__enum* ent_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* inout array<iBase.EntityType> */ struct iBase_EntityType__array** gtype,
  /* out */ int32_t* gtype_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ enum iBase_EntityType__enum to_dimension,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_gentities,
  /* out */ int32_t* adj_gentities_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEnt2ndAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_handle,
  /* in */ enum iBase_EntityType__enum bridge_dimension,
  /* in */ enum iBase_EntityType__enum to_dimension,
  /* inout array<opaque> */ struct sidl_opaque__array** adjacent_gentities,
  /* out */ int32_t* adjacent_gentities_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArr2ndAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_EntityType__enum order_adjacent_key,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isEntAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_handle1,
  /* in */ void* gentity_handle2,
  /* out */ sidl_bool* are_adjacent,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isArrAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles_1,
  /* in */ int32_t entity_handles_1_size,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles_2,
  /* in */ int32_t entity_handles_2_size,
  /* inout array<int> */ struct sidl_int__array** is_adjacent_info,
  /* out */ int32_t* is_adjacent_info_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTopoLevel(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ int32_t* level,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntClosestPt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double near_x,
  /* in */ double near_y,
  /* in */ double near_z,
  /* out */ double* on_x,
  /* out */ double* on_y,
  /* out */ double* on_z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrClosestPt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* near_coordinates,
  /* in */ int32_t near_coordinates_size,
  /* inout array<double> */ struct sidl_double__array** on_coordinates,
  /* out */ int32_t* on_coordinates_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntNrmlXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* nrml_i,
  /* out */ double* nrml_j,
  /* out */ double* nrml_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrNrmlXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coordinates,
  /* in */ int32_t coordinates_size,
  /* inout array<double> */ struct sidl_double__array** normals,
  /* out */ int32_t* normals_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntNrmlPlXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* pt_x,
  /* out */ double* pt_y,
  /* out */ double* pt_z,
  /* out */ double* nrml_i,
  /* out */ double* nrml_j,
  /* out */ double* nrml_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrNrmlPlXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* near_coordinates,
  /* in */ int32_t near_coordinates_size,
  /* inout array<double> */ struct sidl_double__array** on_coordinates,
  /* out */ int32_t* on_coordinates_size,
  /* inout array<double> */ struct sidl_double__array** normals,
  /* out */ int32_t* normals_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntTgntXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* tgnt_i,
  /* out */ double* tgnt_j,
  /* out */ double* tgnt_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrTgntXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coordinates,
  /* in */ int32_t coordinates_size,
  /* inout array<double> */ struct sidl_double__array** tangents,
  /* out */ int32_t* tangents_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getFcCvtrXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* face_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* cvtr1_i,
  /* out */ double* cvtr1_j,
  /* out */ double* cvtr1_k,
  /* out */ double* cvtr2_i,
  /* out */ double* cvtr2_j,
  /* out */ double* cvtr2_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEgCvtrXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* edge_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* cvtr_i,
  /* out */ double* cvtr_j,
  /* out */ double* cvtr_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntArrCvtrXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coords,
  /* in */ int32_t coords_size,
  /* inout array<double> */ struct sidl_double__array** cvtr_1,
  /* out */ int32_t* cvtr_1_size,
  /* inout array<double> */ struct sidl_double__array** cvtr_2,
  /* out */ int32_t* cvtr_2_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEgEvalXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* edge_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* on_x,
  /* out */ double* on_y,
  /* out */ double* on_z,
  /* out */ double* tgnt_i,
  /* out */ double* tgnt_j,
  /* out */ double* tgnt_k,
  /* out */ double* cvtr_i,
  /* out */ double* cvtr_j,
  /* out */ double* cvtr_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getFcEvalXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* face_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* on_x,
  /* out */ double* on_y,
  /* out */ double* on_z,
  /* out */ double* nrml_i,
  /* out */ double* nrml_j,
  /* out */ double* nrml_k,
  /* out */ double* cvtr1_i,
  /* out */ double* cvtr1_j,
  /* out */ double* cvtr1_k,
  /* out */ double* cvtr2_i,
  /* out */ double* cvtr2_j,
  /* out */ double* cvtr2_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrEgEvalXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coords,
  /* in */ int32_t coords_size,
  /* inout array<double> */ struct sidl_double__array** on_coords,
  /* out */ int32_t* on_coords_size,
  /* inout array<double> */ struct sidl_double__array** tangent,
  /* out */ int32_t* tangent_size,
  /* inout array<double> */ struct sidl_double__array** cvtr,
  /* out */ int32_t* cvtr_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrFcEvalXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coords,
  /* in */ int32_t coords_size,
  /* inout array<double> */ struct sidl_double__array** on_coords,
  /* out */ int32_t* on_coords_size,
  /* inout array<double> */ struct sidl_double__array** normal,
  /* out */ int32_t* normal_size,
  /* inout array<double> */ struct sidl_double__array** cvtr_1,
  /* out */ int32_t* cvtr_1_size,
  /* inout array<double> */ struct sidl_double__array** cvtr_2,
  /* out */ int32_t* cvtr_2_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntBoundBox(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* min_x,
  /* out */ double* min_y,
  /* out */ double* min_z,
  /* out */ double* max_x,
  /* out */ double* max_y,
  /* out */ double* max_z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrBoundBox(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** min_corner,
  /* out */ int32_t* min_corner_size,
  /* inout array<double> */ struct sidl_double__array** max_corner,
  /* out */ int32_t* max_corner_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getVtxCoord(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* vertex_handle,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getVtxArrCoords(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** coordinates,
  /* out */ int32_t* coordinates_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getPntRayIntsct(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* in */ double dir_x,
  /* in */ double dir_y,
  /* in */ double dir_z,
  /* inout array<opaque> */ struct sidl_opaque__array** 
    intersect_entity_handles,
  /* out */ int32_t* intersect_entity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** intersect_coords,
  /* out */ int32_t* intersect_coords_size,
  /* inout array<double> */ struct sidl_double__array** param_coords,
  /* out */ int32_t* param_coords_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getPntArrRayIntsct(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coords,
  /* in */ int32_t coords_size,
  /* in array<double> */ struct sidl_double__array* directions,
  /* in */ int32_t directions_size,
  /* inout array<opaque> */ struct sidl_opaque__array** 
    intersect_entity_handles,
  /* out */ int32_t* intersect_entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** offset,
  /* out */ int32_t* offset_size,
  /* inout array<double> */ struct sidl_double__array** intersect_coords,
  /* out */ int32_t* intersect_coords_size,
  /* inout array<double> */ struct sidl_double__array** param_coords,
  /* out */ int32_t* param_coords_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntNrmlSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gface,
  /* in */ void* gregion,
  /* out */ int32_t* sense,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrNrmlSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* region_handles,
  /* in */ int32_t region_handles_size,
  /* inout array<int> */ struct sidl_int__array** sense,
  /* out */ int32_t* sense_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEgFcSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gedge,
  /* in */ void* gface,
  /* out */ int32_t* sense,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEgFcArrSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* inout array<int> */ struct sidl_int__array** sense,
  /* out */ int32_t* sense_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEgVtxSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gedge,
  /* in */ void* gvertex1,
  /* in */ void* gvertex2,
  /* out */ int32_t* sense,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEgVtxArrSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles_1,
  /* in */ int32_t vertex_handles_1_size,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles_2,
  /* in */ int32_t vertex_handles_2_size,
  /* inout array<int> */ struct sidl_int__array** sense,
  /* out */ int32_t* sense_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_measure(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* inout array<double> */ struct sidl_double__array** measures,
  /* out */ int32_t* measures_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getFaceType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gface_handle,
  /* inout */ char** face_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getParametric(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ int32_t* parametric,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isEntParametric(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_handle,
  /* out */ int32_t* is_parametric,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isArrParametric(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** is_parametric,
  /* out */ int32_t* is_parametric_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntUVtoXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrUVtoXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* uv,
  /* in */ int32_t uv_size,
  /* inout array<double> */ struct sidl_double__array** coordinates,
  /* out */ int32_t* coordinates_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntUtoXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrUtoXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in array<double> */ struct sidl_double__array* u,
  /* in */ int32_t u_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** on_coords,
  /* out */ int32_t* on_coords_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntXYZtoUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* u,
  /* out */ double* v,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntXYZtoU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* u,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrXYZtoUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coordinates,
  /* in */ int32_t coordinates_size,
  /* inout array<double> */ struct sidl_double__array** uv,
  /* out */ int32_t* uv_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrXYZtoU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coordinates,
  /* in */ int32_t coordinates_size,
  /* inout array<double> */ struct sidl_double__array** u,
  /* out */ int32_t* u_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntXYZtoUVHint(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* inout */ double* u,
  /* inout */ double* v,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrXYZtoUVHint(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* coords,
  /* in */ int32_t coords_size,
  /* inout array<double> */ struct sidl_double__array** uv,
  /* out */ int32_t* uv_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntUVRange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* u_min,
  /* out */ double* v_min,
  /* out */ double* u_max,
  /* out */ double* v_max,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntURange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* u_min,
  /* out */ double* u_max,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrUVRange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** uv_min,
  /* out */ int32_t* uv_min_size,
  /* inout array<double> */ struct sidl_double__array** uv_max,
  /* out */ int32_t* uv_max_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrURange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<double> */ struct sidl_double__array** u_min,
  /* out */ int32_t* u_min_size,
  /* inout array<double> */ struct sidl_double__array** u_max,
  /* out */ int32_t* u_max_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntUtoUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* edge_handle,
  /* in */ void* face_handle,
  /* in */ double in_u,
  /* out */ double* u,
  /* out */ double* v,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getVtxToUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* vertex_handle,
  /* in */ void* face_handle,
  /* out */ double* u,
  /* out */ double* v,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getVtxToU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* vertex_handle,
  /* in */ void* edge_handle,
  /* out */ double* u,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrUtoUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* in array<double> */ struct sidl_double__array* u_in,
  /* in */ int32_t u_in_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** uv,
  /* out */ int32_t* uv_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getVtxArrToUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** uv,
  /* out */ int32_t* uv_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getVtxArrToU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* inout array<double> */ struct sidl_double__array** u,
  /* out */ int32_t* u_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntNrmlUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* out */ double* nrml_i,
  /* out */ double* nrml_j,
  /* out */ double* nrml_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrNrmlUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gface_handles,
  /* in */ int32_t gface_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* parameters,
  /* in */ int32_t parameters_size,
  /* inout array<double> */ struct sidl_double__array** normals,
  /* out */ int32_t* normals_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntTgntU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double param_coord,
  /* out */ double* tngt_i,
  /* out */ double* tngt_j,
  /* out */ double* tngt_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrTgntU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gedge_handles,
  /* in */ int32_t gedge_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* parameters,
  /* in */ int32_t parameters_size,
  /* inout array<double> */ struct sidl_double__array** tangents,
  /* out */ int32_t* tangents_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEnt1stDrvt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* inout array<double> */ struct sidl_double__array** drvt_u,
  /* out */ int32_t* drvt_u_size,
  /* inout array<double> */ struct sidl_double__array** drvt_v,
  /* out */ int32_t* drvt_v_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArr1stDrvt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* uv,
  /* in */ int32_t uv_size,
  /* inout array<double> */ struct sidl_double__array** drvt_u,
  /* out */ int32_t* drvt_u_size,
  /* inout array<int> */ struct sidl_int__array** u_offset,
  /* out */ int32_t* u_offset_size,
  /* inout array<double> */ struct sidl_double__array** drvt_v,
  /* out */ int32_t* drvt_v_size,
  /* inout array<int> */ struct sidl_int__array** v_offset,
  /* out */ int32_t* v_offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEnt2ndDrvt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* inout array<double> */ struct sidl_double__array** drvt_uu,
  /* out */ int32_t* drvt_uu_size,
  /* inout array<double> */ struct sidl_double__array** drvt_vv,
  /* out */ int32_t* drvt_vv_size,
  /* inout array<double> */ struct sidl_double__array** drvt_uv,
  /* out */ int32_t* drvt_uv_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArr2ndDrvt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* uv,
  /* in */ int32_t uv_size,
  /* inout array<double> */ struct sidl_double__array** drvt_uu,
  /* out */ int32_t* drvt_uu_size,
  /* inout array<int> */ struct sidl_int__array** uu_offset,
  /* out */ int32_t* uu_offset_size,
  /* inout array<double> */ struct sidl_double__array** drvt_vv,
  /* out */ int32_t* drvt_vv_size,
  /* inout array<int> */ struct sidl_int__array** vv_offset,
  /* out */ int32_t* vv_offset_size,
  /* inout array<double> */ struct sidl_double__array** drvt_uv,
  /* out */ int32_t* drvt_uv_size,
  /* inout array<int> */ struct sidl_int__array** uv_offset,
  /* out */ int32_t* uv_offset_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getFcCvtrUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* out */ double* cvtr1_i,
  /* out */ double* cvtr1_j,
  /* out */ double* cvtr1_k,
  /* out */ double* cvtr2_i,
  /* out */ double* cvtr2_j,
  /* out */ double* cvtr2_k,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getFcArrCvtrUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* uv,
  /* in */ int32_t uv_size,
  /* inout array<double> */ struct sidl_double__array** cvtr_1,
  /* out */ int32_t* cvtr_1_size,
  /* inout array<double> */ struct sidl_double__array** cvtr_2,
  /* out */ int32_t* cvtr_2_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isEntPeriodic(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ int32_t* in_u,
  /* out */ int32_t* in_v,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isArrPeriodic(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** in_uv,
  /* out */ int32_t* in_uv_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isFcDegenerate(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* face_handle,
  /* out */ int32_t* is_degenerate,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_isFcArrDegenerate(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* inout array<int> */ struct sidl_int__array** degenerate,
  /* out */ int32_t* degenerate_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getTolerance(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ int32_t* type,
  /* out */ double* tolerance,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getEntTolerance(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* tolerance,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getArrTolerance(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<double> */ struct sidl_double__array** tolerances,
  /* out */ int32_t* tolerances_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_initEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_handle,
  /* in */ int32_t gentity_dimension,
  /* out */ void** gentity_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_initEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* in */ int32_t requested_array_size,
  /* out */ void** entArr_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getNextEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_iterator,
  /* out */ void** entity_handle,
  /* out */ int32_t* has_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_getNextEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entArr_iterator,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ int32_t* has_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_resetEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_resetEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_endEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* Gentity_dim_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_endEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_Copy(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* out */ void** geom_entity2,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_SweepAboutAxis(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double angle,
  /* in */ double axis_normal_x,
  /* in */ double axis_normal_y,
  /* in */ double axis_normal_z,
  /* out */ void** geom_entity2,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_Delete(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_createSphere(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double radius,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_createBrick(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_createCylinder(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double height,
  /* in */ double major_rad,
  /* in */ double minor_rad,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_createTorus(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double major_rad,
  /* in */ double minor_rad,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_moveEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_rotateEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double angle,
  /* in */ double axis_normal_x,
  /* in */ double axis_normal_y,
  /* in */ double axis_normal_z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_reflectEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double plane_normal_x,
  /* in */ double plane_normal_y,
  /* in */ double plane_normal_z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_scaleEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double scale_x,
  /* in */ double scale_y,
  /* in */ double scale_z,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_uniteEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* geom_entities,
  /* in */ int32_t geom_entities_size,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_subtractEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* blank,
  /* in */ void* tool,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_sectionEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double plane_normal_x,
  /* in */ double plane_normal_y,
  /* in */ double plane_normal_z,
  /* in */ double offset,
  /* in */ sidl_bool reverse,
  /* out */ void** geom_entity2,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_imprintEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* geom_entities,
  /* in */ int32_t geom_entities_size,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iGeomSIDL_GeomSidl_mergeEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* geom_entities,
  /* in */ int32_t geom_entities_size,
  /* in */ double tolerance,
  /* out */ sidl_BaseInterface *_ex);

#ifdef WITH_RMI
extern struct sidl_BaseInterface__object* 
  impl_iGeomSIDL_GeomSidl_fconnect_sidl_BaseInterface(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
#endif /*WITH_RMI*/

#line 1946 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.h"
/* DO-NOT-DELETE splicer.begin(_hmisc) */
/* insert code here (miscellaneous things) */
/* DO-NOT-DELETE splicer.end(_hmisc) */
#line 1949 "server/iGeomSIDL_GeomSidl_Impl.h"

#ifdef __cplusplus
}
#endif
#endif
