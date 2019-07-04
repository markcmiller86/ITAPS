/*
 * File:          iMeshSIDL_MeshSidl_Impl.c
 * Symbol:        iMeshSIDL.MeshSidl-v0.2
 * Symbol Type:   class
 * Babel Version: 1.4.0 (Revision: 6607 release-1-4-0-branch)
 * Description:   Server-side implementation for iMeshSIDL.MeshSidl
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

/*
 * DEVELOPERS ARE EXPECTED TO PROVIDE IMPLEMENTATIONS
 * FOR THE FOLLOWING METHODS BETWEEN SPLICER PAIRS.
 */

/*
 * Symbol "iMeshSIDL.MeshSidl" (version 0.2)
 */

#include "iMeshSIDL_MeshSidl_Impl.h"
#include "sidl_NotImplementedException.h"
#include "sidl_Exception.h"
#ifndef included_sidl_MemAllocException_h
#include "sidl_MemAllocException.h"
#endif

#line 29 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
/* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._includes) */
/* Insert-Code-Here {iMeshSIDL.MeshSidl._includes} (additional includes or code) */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sidl_Exception.h>
#include "iBase_Error.h"
#include "iMesh.h"

#define INSTANCE (iMeshSIDL_MeshSidl__get_data(self)->mInstance)
#define ERROR    (iMeshSIDL_MeshSidl__get_data(self)->mLastError)

#define PROCESS_ERROR do { \
  if (ERROR != iBase_SUCCESS) \
    processError(self, _ex, NULL); \
} while (0)

#define PROCESS_ERROR_MSG(a,b) do { \
   ERROR = a; \
   if (a != iBase_SUCCESS) \
     processError(self, _ex, b); \
} while (0)

#include "sidl_macros.h"

static void
processError( iMeshSIDL_MeshSidl self,
              sidl_BaseInterface *_ex,
              const char* message )
{
  const enum iBase_ErrorActions action = 
    (enum iBase_ErrorActions)iMeshSIDL_MeshSidl__get_data( self )->mErrorAction;
  char* buffer = NULL;
  const int buflen = 1024;
  iBase_Error this_error = NULL;
  int local_error = iBase_SUCCESS;
  sidl_BaseInterface ignore = NULL;
  
  if (NULL == message && iBase_SILENT != action) {
    buffer = malloc( buflen );
    iMesh_getDescription( INSTANCE, buffer, &local_error, buflen );
    buffer[buflen-1] = '\0';
    if (local_error != iBase_SUCCESS)
      strcpy( buffer, "(error retreiving error description)" );
    else if (buffer[0] == '\0')
      strcpy( buffer, "(no error description)" );
    message = buffer;
  }

  switch (action) {
    case iBase_THROW_ERROR:
      this_error = iBase_Error__create( &ignore );
      iBase_Error_set( this_error, ERROR, message, &ignore );
      if (_ex != NULL)
        *_ex = sidl_BaseInterface__cast( this_error, &ignore );
      break;
    case iBase_WARN_ONLY:
      fprintf(stderr,"Error %d: %s\n", (int)ERROR, message );
      break;
    case iBase_SILENT:
      break;
  }
  free( buffer );
}

/* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._includes) */
#line 95 "server/iMeshSIDL_MeshSidl_Impl.c"

#define SIDL_IOR_MAJOR_VERSION 2
#define SIDL_IOR_MINOR_VERSION 0
/*
 * Static class initializer called exactly once before any user-defined method is dispatched
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl__load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl__load(
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 116 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._load) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl._load} (class initialization) */
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._load) */
#line 119 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}
/*
 * Class constructor called when the class is created.
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl__ctor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl__ctor(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 140 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._ctor) */
  iMeshSIDL_MeshSidl__set_data( self, malloc( sizeof(struct iMeshSIDL_MeshSidl__data) ) );
  iMesh_newMesh( 0, &INSTANCE, &ERROR, 0 );
  iMeshSIDL_MeshSidl__get_data( self )->mErrorAction = iBase_ErrorActions_THROW_ERROR;
  /*PROCESS_ERROR;*/
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._ctor) */
#line 146 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Special Class constructor called when the user wants to wrap his own private data.
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl__ctor2"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl__ctor2(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 169 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._ctor2) */
    /* insert code here (special constructor) */
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._ctor2) */
#line 172 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}
/*
 * Class destructor called when the class is deleted.
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl__dtor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl__dtor(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 193 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl._dtor) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl._dtor} (destructor) */
  iMesh_dtor(INSTANCE, &ERROR);
  free( iMeshSIDL_MeshSidl__get_data( self ) );
  /*PROCESS_ERROR;*/
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl._dtor) */
#line 199 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  createTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_createTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_createTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ const char* tag_name,
  /* in */ int32_t number_of_values,
  /* in */ enum iBase_TagValueType__enum tag_type,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 225 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.createTag) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.createTag} (createTag method) */
  iBase_TagHandle handle;
  iMesh_createTag (INSTANCE,
                   tag_name, number_of_values, 
                   tag_type, &handle, &ERROR, 
                   strlen(tag_name));
  PROCESS_ERROR;
  *tag_handle = handle;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.createTag) */
#line 235 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  destroyTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_destroyTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_destroyTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* in */ sidl_bool forced,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 259 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.destroyTag) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.destroyTag} (destroyTag method) */
  int tmp_forced = forced;
  iMesh_destroyTag (INSTANCE, (iBase_TagHandle)tag_handle, tmp_forced, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.destroyTag) */
#line 265 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getTagName[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getTagName"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getTagName(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ char** tag_name,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 289 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getTagName) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getTagName} (getTagName method) */
  char tmp_name[120];
  iMesh_getTagName (INSTANCE, (iBase_TagHandle)tag_handle, tmp_name, &ERROR, 120);
  PROCESS_ERROR;
  *tag_name = strdup(tmp_name);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getTagName) */
#line 296 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getTagSizeValues[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getTagSizeValues"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getTagSizeValues(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_values,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 320 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getTagSizeValues) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getTagSizeValues} (getTagSizeValues method) */
  iMesh_getTagSizeValues (INSTANCE, (iBase_TagHandle)tag_handle, size_values, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getTagSizeValues) */
#line 325 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getTagSizeBytes[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getTagSizeBytes"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getTagSizeBytes(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_bytes,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 349 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getTagSizeBytes) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getTagSizeBytes} (getTagSizeBytes method) */
  iMesh_getTagSizeBytes (INSTANCE, (iBase_TagHandle)tag_handle, size_bytes, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getTagSizeBytes) */
#line 354 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getTagHandle[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getTagHandle"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getTagHandle(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ const char* tag_name,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 378 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getTagHandle) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getTagHandle} (getTagHandle method) */
  iBase_TagHandle handle = 0;
  iMesh_getTagHandle (INSTANCE, tag_name, &handle, &ERROR, strlen(tag_name));
  PROCESS_ERROR;
  *tag_handle = handle;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getTagHandle) */
#line 385 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getTagType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getTagType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getTagType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* tag_handle,
  /* out */ enum iBase_TagValueType__enum* tag_data_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 409 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getTagType) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getTagType} (getTagType method) */
  int result;
  iMesh_getTagType (INSTANCE, (iBase_TagHandle) tag_handle, 
                    &result, &ERROR);
  PROCESS_ERROR;
  *tag_data_type = (enum iBase_TagValueType__enum)result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getTagType) */
#line 417 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 443 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getData} (getData method) */
  CREATE_TEMP_TAG_ARRAY(tag_value);

  iMesh_getData (INSTANCE,
                 (iBase_EntityHandle)entity_handle, 
                 (iBase_TagHandle) tag_handle,
                 TEMP_ARRAY_INOUT(tag_value),
                 &ERROR);

  PROCESS_ERROR;

  ASSIGN_TAG_ARRAY(tag_value);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getData) */
#line 457 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 482 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getIntData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getIntData} (getIntData method) */
  iMesh_getIntData (INSTANCE, (iBase_EntityHandle) entity_handle, (iBase_TagHandle) tag_handle, int_data, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getIntData) */
#line 487 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 512 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getDblData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getDblData} (getDblData method) */
  iMesh_getDblData (INSTANCE, (iBase_EntityHandle) entity_handle, (iBase_TagHandle) tag_handle, dbl_data, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getDblData) */
#line 517 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 542 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEHData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEHData} (getEHData method) */
  iBase_EntityHandle result;
  iMesh_getEHData (INSTANCE, (iBase_EntityHandle) entity_handle, (iBase_TagHandle) tag_handle, 
                   &result, &ERROR);
  PROCESS_ERROR;
  *eh_data = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEHData) */
#line 550 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 576 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setData} (setData method) */
  iMesh_setData (INSTANCE, (iBase_EntityHandle) entity_handle,
                 (iBase_TagHandle) tag_handle, 
                 ARRAY_PTR(tag_value, const char),
                 tag_value_size, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setData) */
#line 584 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 609 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setIntData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setIntData} (setIntData method) */
  iMesh_setIntData (INSTANCE, (iBase_EntityHandle) entity_handle,
                    (iBase_TagHandle) tag_handle, tag_value, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setIntData) */
#line 615 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 640 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setDblData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setDblData} (setDblData method) */
  iMesh_setDblData (INSTANCE, (iBase_EntityHandle) entity_handle,
                    (iBase_TagHandle) tag_handle, tag_value, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setDblData) */
#line 646 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 671 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setEHData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setEHData} (setEHData method) */
  iMesh_setEHData (INSTANCE, (iBase_EntityHandle)entity_handle,
                   (iBase_TagHandle) tag_handle, 
                   (iBase_EntityHandle)tag_value, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setEHData) */
#line 678 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getAllTags[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getAllTags"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getAllTags(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 703 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getAllTags) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getAllTags} (getAllTags method) */
  CREATE_TEMP_TH_ARRAY(tag_handles);

  iMesh_getAllTags (INSTANCE, (iBase_EntityHandle) entity_handle,
                    TEMP_ARRAY_INOUT(tag_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(tag_handles);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getAllTags) */
#line 713 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 737 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvTag) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvTag} (rmvTag method) */
  iMesh_rmvTag (INSTANCE, (iBase_EntityHandle) entity_handle, (iBase_TagHandle) tag_handle, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvTag) */
#line 742 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 769 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getArrData} (getArrData method) */
  CREATE_TEMP_TAG_ARRAY(value_array);

  iMesh_getArrData (INSTANCE,
                    TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                    (iBase_TagHandle) tag_handle,
                    TEMP_ARRAY_INOUT(value_array), &ERROR);
  PROCESS_ERROR;

  ASSIGN_TAG_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getArrData) */
#line 781 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getIntArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getIntArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getIntArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<int> */ struct sidl_int__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 808 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getIntArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getIntArrData} (getIntArrData method) */
  CREATE_TEMP_INT_ARRAY(value_array);

  iMesh_getIntArrData (INSTANCE, TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                       (iBase_TagHandle) tag_handle, TEMP_ARRAY_INOUT(value_array), &ERROR);
  PROCESS_ERROR;

  ASSIGN_INT_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getIntArrData) */
#line 818 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getDblArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getDblArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getDblArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<double> */ struct sidl_double__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 845 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getDblArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getDblArrData} (getDblArrData method) */
  CREATE_TEMP_DBL_ARRAY(value_array);

  iMesh_getDblArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                       (iBase_TagHandle) tag_handle,
                       TEMP_ARRAY_INOUT(value_array), &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getDblArrData) */
#line 857 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEHArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEHArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEHArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 884 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEHArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEHArrData} (getEHArrData method) */
  CREATE_TEMP_EH_ARRAY(value_array);

  iMesh_getEHArrData (INSTANCE,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                      (iBase_TagHandle) tag_handle,
                      TEMP_ARRAY_INOUT(value_array), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(value_array);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEHArrData) */
#line 897 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 924 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setArrData} (setArrData method) */
  iMesh_setArrData (INSTANCE,
                    TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                    (iBase_TagHandle) tag_handle,
                    TEMP_TAG_ARRAY_IN(value_array), &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setArrData) */
#line 932 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setIntArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setIntArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setIntArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<int> */ struct sidl_int__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 959 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setIntArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setIntArrData} (setIntArrData method) */
  iMesh_setIntArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                       (iBase_TagHandle) tag_handle,
                       TEMP_TYPED_ARRAY_IN(int32_t, value_array), &ERROR);
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setIntArrData) */
#line 968 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setDblArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setDblArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setDblArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<double> */ struct sidl_double__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 995 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setDblArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setDblArrData} (setDblArrData method) */
  iMesh_setDblArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                       (iBase_TagHandle) tag_handle,
                       TEMP_TYPED_ARRAY_IN(double, value_array), &ERROR);
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setDblArrData) */
#line 1004 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setEHArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setEHArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setEHArrData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<opaque> */ struct sidl_opaque__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1031 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setEHArrData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setEHArrData} (setEHArrData method) */
  iMesh_setEHArrData (INSTANCE,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                      (iBase_TagHandle) tag_handle,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, value_array), 
                      &ERROR);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setEHArrData) */
#line 1039 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvArrTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvArrTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvArrTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1064 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvArrTag) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvArrTag} (rmvArrTag method) */
  iMesh_rmvArrTag (INSTANCE,
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                   (iBase_TagHandle) tag_handle, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvArrTag) */
#line 1071 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setEntSetData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setEntSetData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1097 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setEntSetData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setEntSetData} (setEntSetData method) */
  iMesh_setEntSetData (INSTANCE, (iBase_EntitySetHandle)entity_set, (iBase_TagHandle) tag_handle,
                       ARRAY_PTR(*tag_value, const char), tag_value_size, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setEntSetData) */
#line 1103 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setEntSetIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setEntSetIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1128 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setEntSetIntData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setEntSetIntData} (setEntSetIntData method) */
  iMesh_setEntSetIntData (INSTANCE,
                          (iBase_EntitySetHandle)entity_set, (iBase_TagHandle) tag_handle, tag_value, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setEntSetIntData) */
#line 1134 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setEntSetDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setEntSetDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1159 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setEntSetDblData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setEntSetDblData} (setEntSetDblData method) */
  iMesh_setEntSetDblData (INSTANCE,
                          (iBase_EntitySetHandle)entity_set, (iBase_TagHandle) tag_handle, tag_value, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setEntSetDblData) */
#line 1165 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setEntSetEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setEntSetEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1190 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setEntSetEHData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setEntSetEHData} (setEntSetEHData method) */
  iMesh_setEntSetEHData (INSTANCE,
                         (iBase_EntitySetHandle)entity_set, 
                         (iBase_TagHandle) tag_handle, 
                         (iBase_EntityHandle)tag_value, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setEntSetEHData) */
#line 1198 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntSetData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntSetData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1224 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntSetData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntSetData} (getEntSetData method) */
  CREATE_TEMP_TAG_ARRAY(tag_value);
  
  iMesh_getEntSetData (INSTANCE,
                       (iBase_EntitySetHandle)entity_set, (iBase_TagHandle) tag_handle, 
                       TEMP_ARRAY_INOUT(tag_value), &ERROR);
  PROCESS_ERROR;

  ASSIGN_TAG_ARRAY(tag_value);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntSetData) */
#line 1235 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntSetIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntSetIntData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1260 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntSetIntData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntSetIntData} (getEntSetIntData method) */
  iMesh_getEntSetIntData (INSTANCE,
                          (iBase_EntitySetHandle)entity_set, (iBase_TagHandle) tag_handle, 
                          int_data, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntSetIntData) */
#line 1267 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntSetDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntSetDblData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1292 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntSetDblData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntSetDblData} (getEntSetDblData method) */
  iMesh_getEntSetDblData (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                          (iBase_TagHandle) tag_handle, dbl_data, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntSetDblData) */
#line 1298 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntSetEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntSetEHData(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1323 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntSetEHData) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntSetEHData} (getEntSetEHData method) */
  iBase_EntityHandle result;
  iMesh_getEntSetEHData (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                         (iBase_TagHandle) tag_handle, 
                         &result, &ERROR);
  PROCESS_ERROR;
  *eh_data = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntSetEHData) */
#line 1332 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getAllEntSetTags[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getAllEntSetTags"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getAllEntSetTags(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1357 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getAllEntSetTags) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getAllEntSetTags} (getAllEntSetTags method) */

  CREATE_TEMP_TH_ARRAY(tag_handles);

  iMesh_getAllEntSetTags (INSTANCE, (iBase_EntitySetHandle)entity_set,
                          TEMP_ARRAY_INOUT(tag_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(tag_handles);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getAllEntSetTags) */
#line 1369 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntSetTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvEntSetTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvEntSetTag(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1393 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvEntSetTag) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvEntSetTag} (rmvEntSetTag method) */
  iMesh_rmvEntSetTag (INSTANCE, (iBase_EntitySetHandle)entity_set, (iBase_TagHandle) tag_handle, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvEntSetTag) */
#line 1398 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  createEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_createEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_createEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ sidl_bool isList,
  /* out */ void** entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1422 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.createEntSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.createEntSet} (createEntSet method) */
  int tmp_islist = isList;
  iBase_EntitySetHandle handle;
  iMesh_createEntSet (INSTANCE, tmp_islist, &handle, &ERROR);
  PROCESS_ERROR;
  *entity_set = handle;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.createEntSet) */
#line 1430 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  destroyEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_destroyEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_destroyEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1453 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.destroyEntSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.destroyEntSet} (destroyEntSet method) */
  iMesh_destroyEntSet (INSTANCE, (iBase_EntitySetHandle)entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.destroyEntSet) */
#line 1458 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  isList[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_isList"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_isList(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* out */ int32_t* is_list,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1482 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.isList) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.isList} (isList method) */
  iMesh_isList (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                is_list, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.isList) */
#line 1488 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNumEntSets[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNumEntSets"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNumEntSets(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_sets,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1513 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNumEntSets) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNumEntSets} (getNumEntSets method) */
  iMesh_getNumEntSets (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                       num_hops, num_sets, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNumEntSets) */
#line 1519 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntSets[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntSets"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntSets(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** 
    contained_entset_handles,
  /* out */ int32_t* contained_entset_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1546 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntSets) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntSets} (getEntSets method) */
  CREATE_TEMP_ESH_ARRAY(contained_entset_handles);

  iMesh_getEntSets (INSTANCE,
                    (iBase_EntitySetHandle)entity_set, num_hops,
                    TEMP_ARRAY_INOUT(contained_entset_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(contained_entset_handles);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntSets) */
#line 1558 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  addEntToSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_addEntToSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_addEntToSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1582 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.addEntToSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.addEntToSet} (addEntToSet method) */
  iMesh_addEntToSet (INSTANCE, (iBase_EntityHandle) entity_handle, (iBase_EntitySetHandle)entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.addEntToSet) */
#line 1587 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntFromSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvEntFromSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvEntFromSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1611 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvEntFromSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvEntFromSet} (rmvEntFromSet method) */
  iMesh_rmvEntFromSet (INSTANCE, (iBase_EntityHandle) entity_handle, (iBase_EntitySetHandle)entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvEntFromSet) */
#line 1616 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  addEntArrToSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_addEntArrToSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_addEntArrToSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1641 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.addEntArrToSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.addEntArrToSet} (addEntArrToSet method) */
  iMesh_addEntArrToSet (INSTANCE,
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                        (iBase_EntitySetHandle)entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.addEntArrToSet) */
#line 1648 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntArrFromSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvEntArrFromSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvEntArrFromSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1673 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvEntArrFromSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvEntArrFromSet} (rmvEntArrFromSet method) */
  iMesh_rmvEntArrFromSet (INSTANCE,
                          TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                          (iBase_EntitySetHandle)entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvEntArrFromSet) */
#line 1680 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  addEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_addEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_addEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_to_add,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1704 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.addEntSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.addEntSet} (addEntSet method) */
  iMesh_addEntSet (INSTANCE, (iBase_EntitySetHandle)entity_set_to_add, 
                   (iBase_EntitySetHandle)entity_set_handle, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.addEntSet) */
#line 1710 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvEntSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_to_remove,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1734 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvEntSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvEntSet} (rmvEntSet method) */
  iMesh_rmvEntSet(INSTANCE, (iBase_EntitySetHandle)entity_set_to_remove,
                  (iBase_EntitySetHandle)entity_set_handle, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvEntSet) */
#line 1740 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  isEntContained[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_isEntContained"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_isEntContained(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* entity_handle,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1765 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.isEntContained) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.isEntContained} (isEntContained method) */
  iMesh_isEntContained(INSTANCE,
                       (iBase_EntitySetHandle)containing_entity_set, (iBase_EntityHandle) entity_handle, is_contained, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.isEntContained) */
#line 1771 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  isEntArrContained[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_isEntArrContained"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_isEntArrContained(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* containing_set,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** is_contained,
  /* out */ int32_t* is_contained_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1798 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.isEntArrContained) */

  CREATE_TEMP_INT_ARRAY(is_contained);

  iMesh_isEntArrContained( INSTANCE,
                           (iBase_EntitySetHandle)containing_set,
                           TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                           TEMP_ARRAY_INOUT(is_contained), &ERROR);
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.isEntArrContained) */
#line 1809 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  isEntSetContained[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_isEntSetContained"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_isEntSetContained(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* contained_entity_set,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1834 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.isEntSetContained) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.isEntSetContained} (isEntSetContained method) */
  iMesh_isEntSetContained(INSTANCE,
                          (iBase_EntitySetHandle)containing_entity_set, 
                          (iBase_EntitySetHandle)contained_entity_set,
                          is_contained, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.isEntSetContained) */
#line 1842 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  addPrntChld[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_addPrntChld"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_addPrntChld(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1866 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.addPrntChld) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.addPrntChld} (addPrntChld method) */
  iMesh_addPrntChld (INSTANCE,
                     (iBase_EntitySetHandle)parent_entity_set, 
                     (iBase_EntitySetHandle)child_entity_set, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.addPrntChld) */
#line 1874 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  rmvPrntChld[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_rmvPrntChld"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_rmvPrntChld(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1898 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.rmvPrntChld) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.rmvPrntChld} (rmvPrntChld method) */
  iMesh_rmvPrntChld (INSTANCE,
                    (iBase_EntitySetHandle)parent_entity_set,
                    (iBase_EntitySetHandle)child_entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.rmvPrntChld) */
#line 1905 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  isChildOf[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_isChildOf"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_isChildOf(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ int32_t* is_child,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1930 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.isChildOf) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.isChildOf} (isChildOf method) */
  iMesh_isChildOf (INSTANCE,
                   (iBase_EntitySetHandle)parent_entity_set, 
                   (iBase_EntitySetHandle)child_entity_set,
                   is_child, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.isChildOf) */
#line 1938 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNumChld[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNumChld"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNumChld(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_child,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1963 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNumChld) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNumChld} (getNumChld method) */
  iMesh_getNumChld (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                    num_hops, num_child, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNumChld) */
#line 1969 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNumPrnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNumPrnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNumPrnt(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_parent,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1994 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNumPrnt) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNumPrnt} (getNumPrnt method) */
  iMesh_getNumPrnt (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                    num_hops, num_parent, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNumPrnt) */
#line 2000 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getChldn[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getChldn"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getChldn(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** child_handles,
  /* out */ int32_t* child_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2026 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getChldn) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getChldn} (getChldn method) */

  CREATE_TEMP_ESH_ARRAY(child_handles);

  iMesh_getChldn (INSTANCE, 
                  (iBase_EntitySetHandle)from_entity_set, 
                  num_hops,
                  TEMP_ARRAY_INOUT(child_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(child_handles);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getChldn) */
#line 2040 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getPrnts[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getPrnts"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getPrnts(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** parent_handles,
  /* out */ int32_t* parent_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2066 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getPrnts) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getPrnts} (getPrnts method) */

  CREATE_TEMP_ESH_ARRAY(parent_handles);

  iMesh_getPrnts (INSTANCE, 
                  (iBase_EntitySetHandle)from_entity_set, 
                  num_hops,
                  TEMP_ARRAY_INOUT(parent_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(parent_handles);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getPrnts) */
#line 2079 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  subtract[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_subtract"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_subtract(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2104 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.subtract) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.subtract} (subtract method) */
  iBase_EntitySetHandle result;
  iMesh_subtract (INSTANCE, 
                  (iBase_EntitySetHandle)entity_set_1,
                  (iBase_EntitySetHandle)entity_set_2, 
                  &result, 
                  &ERROR);
  PROCESS_ERROR;
  *result_entity_set = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.subtract) */
#line 2115 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  intersect[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_intersect"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_intersect(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2140 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.intersect) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.intersect} (intersect method) */
  iBase_EntitySetHandle result;
  iMesh_intersect (INSTANCE, 
                   (iBase_EntitySetHandle)entity_set_1,
                   (iBase_EntitySetHandle)entity_set_2, 
                   &result, 
                   &ERROR);
  PROCESS_ERROR;
  *result_entity_set = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.intersect) */
#line 2151 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  unite[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_unite"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_unite(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2176 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.unite) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.unite} (unite method) */
  iBase_EntitySetHandle result;
  iMesh_unite (INSTANCE, 
               (iBase_EntitySetHandle)entity_set_1,
               (iBase_EntitySetHandle)entity_set_2, 
               &result, 
               &ERROR);
  *result_entity_set = result;
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.unite) */
#line 2187 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  initEntIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_initEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_initEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* in */ enum iMesh_EntityTopology__enum requested_entity_topology,
  /* out */ void** entity_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2213 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.initEntIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.initEntIter} (initEntIter method) */
  iMesh_EntityIterator result;
  iMesh_initEntIter (INSTANCE, (iBase_EntitySetHandle)entity_set_handle,
                     requested_entity_type, 
                     requested_entity_topology,
                     &result, 
                     &ERROR);
  PROCESS_ERROR;
  *entity_iterator = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.initEntIter) */
#line 2224 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNextEntIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNextEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNextEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_iterator,
  /* out */ void** entity_handle,
  /* out */ int32_t* at_end,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2249 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNextEntIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNextEntIter} (getNextEntIter method) */
  iBase_EntityHandle result;
  iMesh_getNextEntIter (INSTANCE, (iMesh_EntityIterator)entity_iterator, 
                        &result, at_end, &ERROR);
  PROCESS_ERROR;
  *entity_handle = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNextEntIter) */
#line 2257 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  resetEntIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_resetEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_resetEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2280 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.resetEntIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.resetEntIter} (resetEntIter method) */
  iMesh_resetEntIter (INSTANCE, 
                      (iMesh_EntityIterator)entity_iterator, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.resetEntIter) */
#line 2286 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  endEntIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_endEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_endEntIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2309 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.endEntIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.endEntIter} (endEntIter method) */
  iMesh_endEntIter (INSTANCE, 
                    (iMesh_EntityIterator)entity_iterator, 
                    &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.endEntIter) */
#line 2316 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntTopo[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntTopo"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntTopo(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* out */ enum iMesh_EntityTopology__enum* ent_topo,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2340 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntTopo) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntTopo} (getEntTopo method) */
  int result;
  iMesh_getEntTopo (INSTANCE, (iBase_EntityHandle)entity_handle, &result, &ERROR);
  PROCESS_ERROR;
  *ent_topo = (enum iMesh_EntityTopology__enum)result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntTopo) */
#line 2347 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* out */ enum iBase_EntityType__enum* ent_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2371 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntType) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntType} (getEntType method) */
  int type;
  iMesh_getEntType (INSTANCE, (iBase_EntityHandle)entity_handle, &type, &ERROR);
  PROCESS_ERROR;
  *ent_type = (enum iBase_EntityType__enum)type;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntType) */
#line 2378 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getVtxCoord[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getVtxCoord"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getVtxCoord(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* vertex_handle,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2404 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getVtxCoord) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getVtxCoord} (getVtxCoord method) */

  iMesh_getVtxCoord (INSTANCE, 
                     (iBase_EntityHandle)vertex_handle,
                     x, y, z, &ERROR);
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getVtxCoord) */
#line 2413 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntAdj[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntAdj"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntAdj(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2439 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntAdj) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntAdj} (getEntAdj method) */

  CREATE_TEMP_EH_ARRAY(adj_entity_handles);

  iMesh_getEntAdj (INSTANCE, (iBase_EntityHandle) entity_handle,  
                   entity_type_requested,
                   TEMP_ARRAY_INOUT(adj_entity_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(adj_entity_handles);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntAdj) */
#line 2452 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEnt2ndAdj[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEnt2ndAdj"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEnt2ndAdj(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* in */ enum iBase_EntityType__enum order_adjacent_key,
  /* in */ enum iBase_EntityType__enum entity_type_requested,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_entity_handles,
  /* out */ int32_t* adj_entity_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2479 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEnt2ndAdj) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEnt2ndAdj} (getEnt2ndAdj method) */
  CREATE_TEMP_EH_ARRAY(adj_entity_handles);

  iMesh_getEnt2ndAdj (INSTANCE, (iBase_EntityHandle) entity_handle,  
                      order_adjacent_key,
                      entity_type_requested,
                      TEMP_ARRAY_INOUT(adj_entity_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(adj_entity_handles);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEnt2ndAdj) */
#line 2492 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  load[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_load(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2517 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.load) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.load} (load method) */

  iMesh_load (INSTANCE, (iBase_EntitySetHandle)entity_set_handle, 
              name, options,
              &ERROR, strlen(name), strlen(options));
  PROCESS_ERROR;
 
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.load) */
#line 2526 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  save[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_save"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_save(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2551 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.save) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.save} (save method) */

  iMesh_save (INSTANCE, (iBase_EntitySetHandle)entity_set_handle, 
              name, options,
              &ERROR, strlen(name), strlen(options));
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.save) */
#line 2559 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getRootSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getRootSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getRootSet(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ void** root_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2582 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getRootSet) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getRootSet} (getRootSet method) */
  iBase_EntitySetHandle result;
  iMesh_getRootSet (INSTANCE, 
                    &result, 
                    &ERROR);
  PROCESS_ERROR;
  *root_set = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getRootSet) */
#line 2591 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getGeometricDim[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getGeometricDim"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getGeometricDim(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ int32_t* dim,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2614 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getGeometricDim) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getGeometricDim} (getGeometricDim method) */

  iMesh_getGeometricDimension(INSTANCE, dim, &ERROR);
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getGeometricDim) */
#line 2621 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setGeometricDim[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setGeometricDim"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setGeometricDim(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ int32_t dim,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2644 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setGeometricDim) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setGeometricDim} (setGeometricDim method) */
  iMesh_setGeometricDimension(INSTANCE, dim, &ERROR);
  PROCESS_ERROR;
  
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setGeometricDim) */
#line 2650 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getDfltStorage[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getDfltStorage"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getDfltStorage(
  /* in */ iMeshSIDL_MeshSidl self,
  /* out */ enum iBase_StorageOrder__enum* dflt_storage,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2673 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getDfltStorage) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getDfltStorage} (getDfltStorage method) */
  int result;
  iMesh_getDfltStorage (INSTANCE, &result, &ERROR);
  *dflt_storage = (enum iBase_StorageOrder__enum)result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getDfltStorage) */
#line 2679 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getAdjTable[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getAdjTable"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getAdjTable(
  /* in */ iMeshSIDL_MeshSidl self,
  /* inout array<iMesh.AdjacencyInfo> */ struct iMesh_AdjacencyInfo__array** 
    adjacency_table,
  /* out */ int32_t* adjacency_table_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2704 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getAdjTable) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getAdjTable} (getAdjTable method) */
  CREATE_TEMP_ENUM_ARRAY(adjacency_table);

  iMesh_getAdjTable (INSTANCE,
                     TEMP_ARRAY_INOUT(adjacency_table), &ERROR);
  PROCESS_ERROR;
  ASSIGN_ENUM_ARRAY(adjacency_table);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getAdjTable) */
#line 2713 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  areEHValid[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_areEHValid"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_areEHValid(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ int32_t reset,
  /* out */ int32_t* are_valid,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2737 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.areEHValid) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.areEHValid} (areEHValid method) */
  iMesh_areEHValid(INSTANCE, reset, are_valid, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.areEHValid) */
#line 2742 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNumOfType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNumOfType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNumOfType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum entity_type,
  /* out */ int32_t* num_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2767 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNumOfType) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNumOfType} (getNumOfType method) */
  iMesh_getNumOfType (INSTANCE,
                      (iBase_EntitySetHandle)entity_set_handle, entity_type, 
                      num_type, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNumOfType) */
#line 2774 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNumOfTopo[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNumOfTopo"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNumOfTopo(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iMesh_EntityTopology__enum entity_topology,
  /* out */ int32_t* num_topo,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2799 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNumOfTopo) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNumOfTopo} (getNumOfTopo method) */
  iMesh_getNumOfTopo (INSTANCE, (iBase_EntitySetHandle)entity_set_handle,
                      entity_topology, 
                      num_topo, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNumOfTopo) */
#line 2806 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntities[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntities"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntities(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set,
  /* in */ enum iBase_EntityType__enum entity_type,
  /* in */ enum iMesh_EntityTopology__enum entity_topology,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2833 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntities) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntities} (getEntities method) */
  
  CREATE_TEMP_EH_ARRAY(entity_handles);
  
  iMesh_getEntities (INSTANCE, (iBase_EntitySetHandle)entity_set, 
                     entity_type, entity_topology,
                     TEMP_ARRAY_INOUT(entity_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(entity_handles);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntities) */
#line 2845 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getVtxArrCoords[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getVtxArrCoords"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getVtxArrCoords(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** coords,
  /* out */ int32_t* coords_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2872 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getVtxArrCoords) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getVtxArrCoords} (getVtxArrCoords method) */
  CREATE_TEMP_DBL_ARRAY(coords);
  iMesh_getVtxArrCoords (INSTANCE,
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, vertex_handles),
                         storage_order,
                         TEMP_ARRAY_INOUT(coords), &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(coords);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getVtxArrCoords) */
#line 2885 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getAdjEntIndices[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getAdjEntIndices"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2968 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getAdjEntIndices) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getAdjEntIndices} (getAdjEntIndices method) */
  CREATE_TEMP_EH_ARRAY( entity_handles );
  CREATE_TEMP_EH_ARRAY( adj_entity_handles );
  CREATE_TEMP_INT_ARRAY( adj_entity_indices );
  CREATE_TEMP_INT_ARRAY( offset );
  iMesh_getAdjEntIndices( INSTANCE,
                          entity_set_handle,
                          entity_type_requestor,
                          entity_topology_requestor,
                          entity_type_requested,
                          TEMP_ARRAY_INOUT( entity_handles ),
                          TEMP_ARRAY_INOUT( adj_entity_handles ),
                          TEMP_ARRAY_INOUT( adj_entity_indices ),
                          TEMP_ARRAY_INOUT( offset ),
                          &ERROR );
  ASSIGN_OPAQUE_ARRAY( entity_handles );
  ASSIGN_OPAQUE_ARRAY( adj_entity_handles );
  ASSIGN_INT_ARRAY( adj_entity_indices );
  ASSIGN_INT_ARRAY( offset );
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getAdjEntIndices) */
#line 2940 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  initEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_initEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_initEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* in */ enum iMesh_EntityTopology__enum requested_entity_topology,
  /* in */ int32_t requested_array_size,
  /* out */ void** entArr_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3016 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.initEntArrIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.initEntArrIter} (initEntArrIter method) */
  iMesh_EntityArrIterator result;
  iMesh_initEntArrIter (INSTANCE, (iBase_EntitySetHandle)entity_set_handle,
                        requested_entity_type, 
                        requested_entity_topology,
                        requested_array_size, 
                        &result, 
                        &ERROR);
  PROCESS_ERROR;
  *entArr_iterator = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.initEntArrIter) */
#line 2979 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getNextEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getNextEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getNextEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entArr_iterator,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ int32_t* at_end,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3054 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getNextEntArrIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getNextEntArrIter} (getNextEntArrIter method) */
  CREATE_TEMP_EH_ARRAY(entity_handles);
  
  iMesh_getNextEntArrIter (INSTANCE, 
                           (iMesh_EntityArrIterator)entArr_iterator,
                           TEMP_ARRAY_INOUT(entity_handles), 
                           at_end, &ERROR);


  ASSIGN_OPAQUE_ARRAY(entity_handles);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getNextEntArrIter) */
#line 3017 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  resetEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_resetEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_resetEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3089 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.resetEntArrIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.resetEntArrIter} (resetEntArrIter method) */
  iMesh_resetEntArrIter (INSTANCE, 
                         (iMesh_EntityArrIterator)entArr_iterator, 
                         &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.resetEntArrIter) */
#line 3047 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  endEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_endEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_endEntArrIter(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3119 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.endEntArrIter) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.endEntArrIter} (endEntArrIter method) */
  iMesh_endEntArrIter (INSTANCE, 
                       (iMesh_EntityArrIterator)entArr_iterator, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.endEntArrIter) */
#line 3077 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntArrTopo[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntArrTopo"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntArrTopo(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<iMesh.EntityTopology> */ struct iMesh_EntityTopology__array** 
    topology,
  /* out */ int32_t* topology_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3153 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntArrTopo) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntArrTopo} (getEntArrTopo method) */

  CREATE_TEMP_ENUM_ARRAY(topology);

  iMesh_getEntArrTopo (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                       TEMP_ARRAY_INOUT(topology), &ERROR);
  PROCESS_ERROR;


  ASSIGN_ENUM_ARRAY(topology);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntArrTopo) */
#line 3117 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntArrType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntArrType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_getEntArrType(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<iBase.EntityType> */ struct iBase_EntityType__array** type,
  /* out */ int32_t* type_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3192 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntArrType) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntArrType} (getEntArrType method) */


  CREATE_TEMP_ENUM_ARRAY(type);

  iMesh_getEntArrType (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                       TEMP_ARRAY_INOUT(type), &ERROR);
  PROCESS_ERROR;

  ASSIGN_ENUM_ARRAY(type);
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntArrType) */
#line 3156 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntArrAdj[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntArrAdj"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3234 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntArrAdj) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntArrAdj} (getEntArrAdj method) */

  CREATE_TEMP_EH_ARRAY(adj_entity_handles);
  CREATE_TEMP_INT_ARRAY(offset);

  iMesh_getEntArrAdj (INSTANCE,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                      entity_type_requested,
                      TEMP_ARRAY_INOUT(adj_entity_handles),
                      TEMP_ARRAY_INOUT(offset), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(adj_entity_handles);
  ASSIGN_INT_ARRAY(offset);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntArrAdj) */
#line 3202 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  getEntArr2ndAdj[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_getEntArr2ndAdj"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3281 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.getEntArr2ndAdj) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.getEntArr2ndAdj} (getEntArr2ndAdj method) */
  CREATE_TEMP_EH_ARRAY(adj_entity_handles);
  CREATE_TEMP_INT_ARRAY(offset);

  iMesh_getEntArr2ndAdj (INSTANCE,
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                         order_adjacent_key,
                         entity_type_requested,
                         TEMP_ARRAY_INOUT(adj_entity_handles),
                         TEMP_ARRAY_INOUT(offset), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(adj_entity_handles);
  ASSIGN_INT_ARRAY(offset);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.getEntArr2ndAdj) */
#line 3249 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setVtxCoord[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setVtxCoord"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setVtxCoord(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* vertex_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setVtxCoord) */
  iMesh_setVtxCoord (INSTANCE, 
                     (iBase_EntityHandle)vertex_handle,
                     x, y, z, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setVtxCoord) */
  }
}

/*
 * Method:  createVtx[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_createVtx"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_createVtx(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ void** new_vertex_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3357 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.createVtx) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.createVtx} (createVtx method) */
  iBase_EntityHandle result;
  iMesh_createVtx (INSTANCE, x, y, z, &result, &ERROR);
  PROCESS_ERROR;
  *new_vertex_handle = result;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.createVtx) */
#line 3309 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  createEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_createEnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_createEnt(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ enum iMesh_EntityTopology__enum new_entity_topology,
  /* in array<opaque> */ struct sidl_opaque__array* lower_order_entity_handles,
  /* in */ int32_t lower_order_entity_handles_size,
  /* out */ void** new_entity_handle,
  /* out */ enum iBase_CreationStatus__enum* status,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3391 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.createEnt) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.createEnt} (createEnt method) */
  iBase_EntityHandle result;
  int stat;
  iMesh_createEnt (INSTANCE, 
                   new_entity_topology,
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, lower_order_entity_handles),
                   &result, &stat, &ERROR);
  PROCESS_ERROR;
  *new_entity_handle = result;
  *status = (enum iBase_CreationStatus__enum)stat;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.createEnt) */
#line 3348 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  deleteEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_deleteEnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_deleteEnt(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ void* entity_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3426 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.deleteEnt) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.deleteEnt} (deleteEnt method) */
  iMesh_deleteEnt (INSTANCE, (iBase_EntityHandle) entity_handle, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.deleteEnt) */
#line 3376 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  setVtxArrCoords[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_setVtxArrCoords"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_setVtxArrCoords(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* new_coords,
  /* in */ int32_t new_coords_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3458 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.setVtxArrCoords) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.setVtxArrCoords} (setVtxArrCoords method) */
  iMesh_setVtxArrCoords (INSTANCE,
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, vertex_handles), 
                         storage_order,
                         TEMP_TYPED_ARRAY_IN(double, new_coords), &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.setVtxArrCoords) */
#line 3411 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  createVtxArr[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_createVtxArr"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_createVtxArr(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in */ int32_t num_verts,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* in array<double> */ struct sidl_double__array* new_coords,
  /* in */ int32_t new_coords_size,
  /* inout array<opaque> */ struct sidl_opaque__array** new_vertex_handles,
  /* out */ int32_t* new_vertex_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3494 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.createVtxArr) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.createVtxArr} (createVtxArr method) */
  CREATE_TEMP_EH_ARRAY(new_vertex_handles);

  iMesh_createVtxArr (INSTANCE,
                      num_verts, 
                      storage_order,
                      TEMP_TYPED_ARRAY_IN(double, new_coords),
                      TEMP_ARRAY_INOUT(new_vertex_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(new_vertex_handles);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.createVtxArr) */
#line 3453 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  createEntArr[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_createEntArr"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3538 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.createEntArr) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.createEntArr} (createEntArr method) */
  CREATE_TEMP_EH_ARRAY(new_entity_handles);
  CREATE_TEMP_ENUM_ARRAY(status);

  iMesh_createEntArr (INSTANCE,
                      new_entity_topology,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, lower_order_entity_handles),
                      TEMP_ARRAY_INOUT(new_entity_handles),
                      TEMP_ARRAY_INOUT(status), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(new_entity_handles);
  ASSIGN_ENUM_ARRAY(status);

    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.createEntArr) */
#line 3499 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}

/*
 * Method:  deleteEntArr[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iMeshSIDL_MeshSidl_deleteEntArr"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iMeshSIDL_MeshSidl_deleteEntArr(
  /* in */ iMeshSIDL_MeshSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3578 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iMeshSIDL.MeshSidl.deleteEntArr) */
  /* Insert-Code-Here {iMeshSIDL.MeshSidl.deleteEntArr} (deleteEntArr method) */
  iMesh_deleteEntArr (INSTANCE,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iMeshSIDL.MeshSidl.deleteEntArr) */
#line 3529 "server/iMeshSIDL_MeshSidl_Impl.c"
  }
}
/* Babel internal methods, Users should not edit below this line. */

#line 3590 ".././iMesh/server/iMeshSIDL_MeshSidl_Impl.c"
/* DO-NOT-DELETE splicer.begin(_misc) */
/* insert code here (miscellaneous code) */
/* DO-NOT-DELETE splicer.end(_misc) */
#line 3538 "server/iMeshSIDL_MeshSidl_Impl.c"
