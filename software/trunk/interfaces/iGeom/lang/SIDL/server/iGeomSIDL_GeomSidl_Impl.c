/*
 * File:          iGeomSIDL_GeomSidl_Impl.c
 * Symbol:        iGeomSIDL.GeomSidl-v0.1
 * Symbol Type:   class
 * Babel Version: 1.4.0 (Revision: 6607 release-1-4-0-branch)
 * Description:   Server-side implementation for iGeomSIDL.GeomSidl
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

/*
 * DEVELOPERS ARE EXPECTED TO PROVIDE IMPLEMENTATIONS
 * FOR THE FOLLOWING METHODS BETWEEN SPLICER PAIRS.
 */

/*
 * Symbol "iGeomSIDL.GeomSidl" (version 0.1)
 */

#include "iGeomSIDL_GeomSidl_Impl.h"
#include "sidl_NotImplementedException.h"
#include "sidl_Exception.h"
#ifndef included_sidl_MemAllocException_h
#include "sidl_MemAllocException.h"
#endif

#line 29 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
/* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._includes) */
/* Insert-Code-Here {iGeomSIDL.GeomSidl._includes} (additional includes or code) */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sidl_Exception.h>
#include "iBase_Error.h"
#include "iGeom.h"

#define INSTANCE (iGeomSIDL_GeomSidl__get_data(self)->mInstance)
#define ERROR    (iGeomSIDL_GeomSidl__get_data(self)->mLastError)

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
processError( iGeomSIDL_GeomSidl self,
              sidl_BaseInterface *_ex,
              const char* message )
{
  const enum iBase_ErrorActions action = 
    (enum iBase_ErrorActions)iGeomSIDL_GeomSidl__get_data( self )->mErrorAction;
  char* buffer = NULL;
  const int buflen = 1024;
  iBase_Error this_error = NULL;
  int local_error = iBase_SUCCESS;
  sidl_BaseInterface ignore = NULL;
  
  if (NULL == message && iBase_SILENT != action) {
    buffer = malloc( buflen );
    iGeom_getDescription( INSTANCE, buffer, &local_error, buflen );
    buffer[buflen-1] = '\0';
    if (local_error != iBase_SUCCESS)
      strcpy( buffer, "(error retreiving error description)" );
    else if (buffer[0] == '\0')
      strcpy( buffer, "(no error description)" );
    message = buffer;
  }

  switch (action) {
    case iBase_THROW_ERROR:
      this_error = iBase_Error__create(&ignore);
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

/* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._includes) */
#line 95 "server/iGeomSIDL_GeomSidl_Impl.c"

#define SIDL_IOR_MAJOR_VERSION 2
#define SIDL_IOR_MINOR_VERSION 0
/*
 * Static class initializer called exactly once before any user-defined method is dispatched
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl__load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl__load(
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 116 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._load) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl._load} (class initialization) */
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._load) */
#line 119 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}
/*
 * Class constructor called when the class is created.
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl__ctor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl__ctor(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 140 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._ctor) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl._ctor} (constructor) */
  iGeomSIDL_GeomSidl__set_data( self, malloc( sizeof(struct iGeomSIDL_GeomSidl__data) ) );
  iGeom_newGeom( 0, &INSTANCE, &ERROR, 0 );
  iGeomSIDL_GeomSidl__get_data( self )->mErrorAction = iBase_ErrorActions_THROW_ERROR;
  /*PROCESS_ERROR;*/
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._ctor) */
#line 147 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Special Class constructor called when the user wants to wrap his own private data.
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl__ctor2"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl__ctor2(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 170 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._ctor2) */
    /* insert code here (special constructor) */
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._ctor2) */
#line 173 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}
/*
 * Class destructor called when the class is deleted.
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl__dtor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl__dtor(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 194 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl._dtor) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl._dtor} (destructor) */
  iGeom_dtor(INSTANCE, &ERROR);
  free( iGeomSIDL_GeomSidl__get_data( self ) );
  /*PROCESS_ERROR;*/
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl._dtor) */
#line 200 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  createTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_createTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_createTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* tag_name,
  /* in */ int32_t number_of_values,
  /* in */ enum iBase_TagValueType__enum tag_type,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 226 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.createTag) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.createTag} (createTag method) */
  if (tag_type > iBase_TagValueType_BYTES || 
      tag_type < iBase_TagValueType_INTEGER) {
    PROCESS_ERROR_MSG(iBase_INVALID_ARGUMENT, "Wrong tag value type");
    return;
  }
  iBase_TagHandle handle;
  iGeom_createTag(INSTANCE, tag_name, number_of_values, 
                 (enum iBase_TagValueType)tag_type, 
                  &handle, &ERROR, strlen(tag_name));
  PROCESS_ERROR;
  *tag_handle = handle;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.createTag) */
#line 240 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  destroyTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_destroyTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_destroyTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* in */ sidl_bool forced,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 264 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.destroyTag) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.destroyTag} (destroyTag method) */
  iGeom_destroyTag(INSTANCE, (iBase_TagHandle)tag_handle, forced, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.destroyTag) */
#line 269 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getTagName[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTagName"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTagName(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ char** tag_name,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 293 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTagName) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTagName} (getTagName method) */
  char buffer[256];
  iGeom_getTagName(INSTANCE, (iBase_TagHandle)tag_handle, 
                   buffer, &ERROR, sizeof(buffer));
  PROCESS_ERROR;
  *tag_name = strdup(buffer);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTagName) */
#line 301 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getTagSizeValues[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTagSizeValues"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTagSizeValues(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_values,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 325 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTagSizeValues) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTagSizeValues} (getTagSizeValues method) */
  iGeom_getTagSizeValues(INSTANCE, (iBase_TagHandle)tag_handle, size_values, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTagSizeValues) */
#line 330 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getTagSizeBytes[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTagSizeBytes"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTagSizeBytes(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ int32_t* size_bytes,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 354 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTagSizeBytes) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTagSizeBytes} (getTagSizeBytes method) */
  iGeom_getTagSizeBytes(INSTANCE, (iBase_TagHandle)tag_handle, size_bytes, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTagSizeBytes) */
#line 359 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getTagHandle[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTagHandle"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTagHandle(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* tag_name,
  /* out */ void** tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 383 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTagHandle) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTagHandle} (getTagHandle method) */
  iBase_TagHandle handle;
  iGeom_getTagHandle(INSTANCE, tag_name, &handle, &ERROR, strlen(tag_name) );
  PROCESS_ERROR;
  *tag_handle = handle;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTagHandle) */
#line 390 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getTagType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTagType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTagType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* tag_handle,
  /* out */ enum iBase_TagValueType__enum* tag_data_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 414 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTagType) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTagType} (getTagType method) */
  int type;
  iGeom_getTagType( INSTANCE, (iBase_TagHandle)tag_handle, &type, &ERROR );
  PROCESS_ERROR;
  *tag_data_type = (enum iBase_TagValueType__enum)type;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTagType) */
#line 421 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 447 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getData} (getData method) */
  CREATE_TEMP_TAG_ARRAY(tag_value);
  
  iGeom_getData (INSTANCE,
                 (iBase_EntityHandle)entity_handle,
                 (iBase_TagHandle)tag_handle,
                 TEMP_ARRAY_INOUT(tag_value),
                 &ERROR);
  PROCESS_ERROR;

  ASSIGN_TAG_ARRAY(tag_value);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getData) */
#line 460 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 485 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getIntData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getIntData} (getIntData method) */
  iGeom_getIntData(INSTANCE,
                   (iBase_EntityHandle)entity_handle,
                   (iBase_TagHandle)tag_handle,
                   int_data, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getIntData) */
#line 493 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 518 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getDblData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getDblData} (getDblData method) */
  iGeom_getDblData(INSTANCE,
                   (iBase_EntityHandle)entity_handle,
                   (iBase_TagHandle)tag_handle,
                   dbl_data, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getDblData) */
#line 526 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 551 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEHData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEHData} (getEHData method) */
  iBase_EntityHandle data;
  iGeom_getEHData(INSTANCE,
                  (iBase_EntityHandle)entity_handle,
                  (iBase_TagHandle)tag_handle,
                  &data, &ERROR );
  PROCESS_ERROR;
  *eh_data = data;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEHData) */
#line 561 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 587 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setData} (setData method) */
  iGeom_setData(INSTANCE, 
                (iBase_EntityHandle)entity_handle,
                (iBase_TagHandle)tag_handle, 
                TEMP_TAG_ARRAY_IN(tag_value),
                &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setData) */
#line 596 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 621 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setIntData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setIntData} (setIntData method) */
  iGeom_setIntData (INSTANCE,
                    (iBase_EntityHandle)entity_handle,
                    (iBase_TagHandle)tag_handle,
                    tag_value, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setIntData) */
#line 629 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 654 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setDblData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setDblData} (setDblData method) */
  iGeom_setDblData (INSTANCE,
                    (iBase_EntityHandle)entity_handle,
                    (iBase_TagHandle)tag_handle,
                    tag_value, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setDblData) */
#line 662 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 687 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setEHData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setEHData} (setEHData method) */
  iBase_EntityHandle value;
  iGeom_setEHData (INSTANCE,
                    (iBase_EntityHandle)entity_handle,
                    (iBase_TagHandle)tag_handle,
                    value, &ERROR );
  PROCESS_ERROR;
  tag_value = value;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setEHData) */
#line 697 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getAllTags[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getAllTags"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getAllTags(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 722 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getAllTags) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getAllTags} (getAllTags method) */
  CREATE_TEMP_TH_ARRAY(tag_handles);
  
  iGeom_getAllTags(INSTANCE, (iBase_EntityHandle)entity_handle, 
                   TEMP_ARRAY_INOUT(tag_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(tag_handles);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getAllTags) */
#line 732 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 756 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvTag) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvTag} (rmvTag method) */
  iGeom_rmvTag (INSTANCE,
                (iBase_EntityHandle)entity_handle,
                (iBase_TagHandle)tag_handle, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvTag) */
#line 763 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 790 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrData} (getArrData method) */
  CREATE_TEMP_TAG_ARRAY(value_array);
  
  iGeom_getArrData(INSTANCE, 
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles), 
                   (iBase_TagHandle)tag_handle, 
                   TEMP_ARRAY_INOUT(value_array),
                   &ERROR);
  PROCESS_ERROR;

  ASSIGN_TAG_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrData) */
#line 803 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getIntArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getIntArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getIntArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<int> */ struct sidl_int__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 830 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getIntArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getIntArrData} (getIntArrData method) */
  CREATE_TEMP_INT_ARRAY(value_array);
  
  iGeom_getIntArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                       (iBase_TagHandle)tag_handle,
                       TEMP_ARRAY_INOUT(value_array), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_INT_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getIntArrData) */
#line 843 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getDblArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getDblArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getDblArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<double> */ struct sidl_double__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 870 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getDblArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getDblArrData} (getDblArrData method) */
  CREATE_TEMP_DBL_ARRAY(value_array);
  
  iGeom_getDblArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                       (iBase_TagHandle)tag_handle,
                       TEMP_ARRAY_INOUT(value_array), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_DOUBLE_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getDblArrData) */
#line 883 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEHArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEHArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEHArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* inout array<opaque> */ struct sidl_opaque__array** value_array,
  /* out */ int32_t* value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 910 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEHArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEHArrData} (getEHArrData method) */
  CREATE_TEMP_EH_ARRAY(value_array);
  
  iGeom_getEHArrData (INSTANCE,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                      (iBase_TagHandle)tag_handle,
                      TEMP_ARRAY_INOUT(value_array), 
                      &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(value_array);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEHArrData) */
#line 923 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<char> */ struct sidl_char__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 950 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setArrData} (setArrData method) */
  iGeom_setArrData(INSTANCE, 
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                   (iBase_TagHandle)tag_handle, 
                   TEMP_TAG_ARRAY_IN(value_array), 
                   &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setArrData) */
#line 959 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setIntArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setIntArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setIntArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<int> */ struct sidl_int__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 986 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setIntArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setIntArrData} (setIntArrData method) */
  iGeom_setIntArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                       (iBase_TagHandle)tag_handle,
                       TEMP_ARRAY_IN(value_array), 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setIntArrData) */
#line 995 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setDblArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setDblArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setDblArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<double> */ struct sidl_double__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1022 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setDblArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setDblArrData} (setDblArrData method) */
  iGeom_setDblArrData (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                       (iBase_TagHandle)tag_handle,
                       TEMP_ARRAY_IN(value_array), 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setDblArrData) */
#line 1031 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setEHArrData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setEHArrData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setEHArrData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* in array<opaque> */ struct sidl_opaque__array* value_array,
  /* in */ int32_t value_array_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1058 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setEHArrData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setEHArrData} (setEHArrData method) */
  iGeom_setEHArrData (INSTANCE,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                      (iBase_TagHandle)tag_handle,
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, value_array), 
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setEHArrData) */
#line 1067 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvArrTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvArrTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvArrTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1092 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvArrTag) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvArrTag} (rmvArrTag method) */
  iGeom_rmvArrTag (INSTANCE,
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                   (iBase_TagHandle)tag_handle, 
                   &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvArrTag) */
#line 1100 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setEntSetData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setEntSetData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* in */ int32_t tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1126 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setEntSetData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setEntSetData} (setEntSetData method) */
  iGeom_setEntSetData(INSTANCE, 
                     (iBase_EntitySetHandle)entity_set, 
                     (iBase_TagHandle)tag_handle,
                      TEMP_TAG_ARRAY_IN(*tag_value), 
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setEntSetData) */
#line 1135 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setEntSetIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setEntSetIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ int32_t tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1160 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setEntSetIntData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setEntSetIntData} (setEntSetIntData method) */
  iGeom_setEntSetIntData (INSTANCE,
                          (iBase_EntitySetHandle)entity_set,
                          (iBase_TagHandle)tag_handle,
                          tag_value, 
                          &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setEntSetIntData) */
#line 1169 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setEntSetDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setEntSetDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ double tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1194 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setEntSetDblData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setEntSetDblData} (setEntSetDblData method) */
  iGeom_setEntSetDblData (INSTANCE,
                          (iBase_EntitySetHandle)entity_set,
                          (iBase_TagHandle)tag_handle,
                          tag_value, 
                          &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setEntSetDblData) */
#line 1203 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  setEntSetEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_setEntSetEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_setEntSetEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* in */ void* tag_value,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1228 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.setEntSetEHData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.setEntSetEHData} (setEntSetEHData method) */
  iGeom_setEntSetEHData (INSTANCE,
                         (iBase_EntitySetHandle)entity_set,
                         (iBase_TagHandle)tag_handle,
                         (iBase_EntityHandle)tag_value, 
                         &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.setEntSetEHData) */
#line 1237 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntSetData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntSetData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* inout array<char> */ struct sidl_char__array** tag_value,
  /* out */ int32_t* tag_value_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1263 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntSetData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntSetData} (getEntSetData method) */
  CREATE_TEMP_TAG_ARRAY(tag_value);
  
  iGeom_getEntSetData (INSTANCE,
                       (iBase_EntitySetHandle)entity_set,
                       (iBase_TagHandle)tag_handle,
                       TEMP_ARRAY_INOUT(tag_value), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_TAG_ARRAY(tag_value);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntSetData) */
#line 1276 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetIntData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntSetIntData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntSetIntData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ int32_t* int_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1301 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntSetIntData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntSetIntData} (getEntSetIntData method) */
  iGeom_getEntSetIntData(INSTANCE,
                         (iBase_EntitySetHandle)entity_set,
                         (iBase_TagHandle)tag_handle, 
                         int_data,
                         &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntSetIntData) */
#line 1310 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetDblData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntSetDblData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntSetDblData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ double* dbl_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1335 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntSetDblData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntSetDblData} (getEntSetDblData method) */
  iGeom_getEntSetDblData(INSTANCE,
                         (iBase_EntitySetHandle)entity_set,
                         (iBase_TagHandle)tag_handle, 
                         dbl_data,
                         &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntSetDblData) */
#line 1344 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntSetEHData[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntSetEHData"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntSetEHData(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ void** eh_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1369 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntSetEHData) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntSetEHData} (getEntSetEHData method) */
  iBase_EntityHandle data;
  iGeom_getEntSetEHData(INSTANCE,
                        (iBase_EntitySetHandle)entity_set,
                        (iBase_TagHandle)tag_handle, 
                        &data,
                        &ERROR );
  PROCESS_ERROR;
  *eh_data = data;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntSetEHData) */
#line 1380 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getAllEntSetTags[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getAllEntSetTags"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getAllEntSetTags(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* inout array<opaque> */ struct sidl_opaque__array** tag_handles,
  /* out */ int32_t* tag_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1405 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getAllEntSetTags) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getAllEntSetTags} (getAllEntSetTags method) */
  CREATE_TEMP_TH_ARRAY(tag_handles);
  
  iGeom_getAllEntSetTags (INSTANCE,
                          (iBase_EntitySetHandle)entity_set,
                          TEMP_ARRAY_INOUT(tag_handles), 
                          &ERROR);
  PROCESS_ERROR;

  ASSIGN_ARRAY(opaque, tag_handles);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getAllEntSetTags) */
#line 1417 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntSetTag[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvEntSetTag"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvEntSetTag(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ void* tag_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1441 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvEntSetTag) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvEntSetTag} (rmvEntSetTag method) */
  iGeom_rmvEntSetTag (INSTANCE,
                      (iBase_EntitySetHandle)entity_set,
                      (iBase_TagHandle)tag_handle, 
                      &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvEntSetTag) */
#line 1449 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  createEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_createEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_createEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ sidl_bool isList,
  /* out */ void** entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1473 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.createEntSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.createEntSet} (createEntSet method) */
  iBase_EntitySetHandle handle;
  iGeom_createEntSet(INSTANCE, isList, &handle, &ERROR);
  PROCESS_ERROR;
  *entity_set = handle;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.createEntSet) */
#line 1480 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  destroyEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_destroyEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_destroyEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1503 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.destroyEntSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.destroyEntSet} (destroyEntSet method) */
  iGeom_destroyEntSet(INSTANCE, (iBase_EntitySetHandle)entity_set, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.destroyEntSet) */
#line 1508 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isList[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isList"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isList(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* out */ int32_t* is_list,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1532 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isList) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isList} (isList method) */
  iGeom_isList(INSTANCE, (iBase_EntitySetHandle)entity_set, is_list, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isList) */
#line 1537 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getNumEntSets[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getNumEntSets"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getNumEntSets(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_sets,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1562 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getNumEntSets) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getNumEntSets} (getNumEntSets method) */
  iGeom_getNumEntSets(INSTANCE, 
                      (iBase_EntitySetHandle)entity_set, 
                      num_hops, 
                      num_sets,
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getNumEntSets) */
#line 1571 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntSets[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntSets"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntSets(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** 
    contained_entset_handles,
  /* out */ int32_t* contained_entset_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1598 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntSets) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntSets} (getEntSets method) */
  CREATE_TEMP_ESH_ARRAY(contained_entset_handles);

  iGeom_getEntSets (INSTANCE,
                    (iBase_EntitySetHandle)entity_set, num_hops,
                    TEMP_ARRAY_INOUT(contained_entset_handles), &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(contained_entset_handles);

    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntSets) */
#line 1610 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  addEntToSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_addEntToSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_addEntToSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1634 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.addEntToSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.addEntToSet} (addEntToSet method) */
  iGeom_addEntToSet (INSTANCE,
                     (iBase_EntityHandle)entity_handle,
                     (iBase_EntitySetHandle)entity_set, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.addEntToSet) */
#line 1642 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntFromSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvEntFromSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvEntFromSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1666 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvEntFromSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvEntFromSet} (rmvEntFromSet method) */
  iGeom_rmvEntFromSet (INSTANCE,
                       (iBase_EntityHandle)entity_handle,
                       (iBase_EntitySetHandle)entity_set, 
                       &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvEntFromSet) */
#line 1674 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  addEntArrToSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_addEntArrToSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_addEntArrToSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1699 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.addEntArrToSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.addEntArrToSet} (addEntArrToSet method) */
  iGeom_addEntArrToSet(INSTANCE, 
                       TEMP_TYPED_ARRAY_IN( iBase_EntityHandle, entity_handles ),
                       (iBase_EntitySetHandle)entity_set, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.addEntArrToSet) */
#line 1707 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntArrFromSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvEntArrFromSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvEntArrFromSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* in */ void* entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1732 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvEntArrFromSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvEntArrFromSet} (rmvEntArrFromSet method) */
  iGeom_rmvEntArrFromSet(INSTANCE, 
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles), 
                         (iBase_EntitySetHandle)entity_set, 
                         &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvEntArrFromSet) */
#line 1740 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  addEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_addEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_addEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_to_add,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1764 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.addEntSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.addEntSet} (addEntSet method) */
  iGeom_addEntSet(INSTANCE, 
                  (iBase_EntitySetHandle)entity_set_to_add, 
                  (iBase_EntitySetHandle)entity_set_handle, 
                  &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.addEntSet) */
#line 1772 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvEntSet[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvEntSet"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvEntSet(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_to_remove,
  /* in */ void* entity_set_handle,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1796 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvEntSet) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvEntSet} (rmvEntSet method) */
  iGeom_rmvEntSet (INSTANCE,
                   (iBase_EntitySetHandle)entity_set_to_remove,
                   (iBase_EntitySetHandle)entity_set_handle, 
                   &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvEntSet) */
#line 1804 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isEntContained[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isEntContained"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isEntContained(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* entity_handle,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1829 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isEntContained) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isEntContained} (isEntContained method) */
 iGeom_isEntContained(INSTANCE,
                      (iBase_EntitySetHandle)containing_entity_set,
                      (iBase_EntityHandle)entity_handle, 
                      is_contained,
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isEntContained) */
#line 1838 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isEntArrContained[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isEntArrContained"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isEntArrContained(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* containing_set,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** is_contained,
  /* out */ int32_t* is_contained_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1865 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isEntArrContained) */

  CREATE_TEMP_INT_ARRAY(is_contained);

  iGeom_isEntArrContained( INSTANCE,
                           (iBase_EntitySetHandle)containing_set,
                           TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                           TEMP_ARRAY_INOUT(is_contained), &ERROR);
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isEntArrContained) */
#line 1876 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isEntSetContained[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isEntSetContained"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isEntSetContained(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* containing_entity_set,
  /* in */ void* contained_entity_set,
  /* out */ int32_t* is_contained,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1901 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isEntSetContained) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isEntSetContained} (isEntSetContained method) */
  iGeom_isEntSetContained(INSTANCE,
                          (iBase_EntitySetHandle)containing_entity_set,
                          (iBase_EntitySetHandle)contained_entity_set, 
                          is_contained,
                          &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isEntSetContained) */
#line 1910 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  addPrntChld[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_addPrntChld"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_addPrntChld(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1934 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.addPrntChld) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.addPrntChld} (addPrntChld method) */
  iGeom_addPrntChld(INSTANCE, 
                    (iBase_EntitySetHandle)parent_entity_set, 
                    (iBase_EntitySetHandle)child_entity_set, 
                    &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.addPrntChld) */
#line 1942 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rmvPrntChld[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rmvPrntChld"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rmvPrntChld(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1966 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rmvPrntChld) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rmvPrntChld} (rmvPrntChld method) */
  iGeom_rmvPrntChld(INSTANCE, 
                    (iBase_EntitySetHandle)parent_entity_set, 
                    (iBase_EntitySetHandle)child_entity_set, 
                    &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rmvPrntChld) */
#line 1974 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isChildOf[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isChildOf"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isChildOf(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* parent_entity_set,
  /* in */ void* child_entity_set,
  /* out */ int32_t* is_child,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 1999 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isChildOf) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isChildOf} (isChildOf method) */
  iGeom_isChildOf(INSTANCE, 
                  (iBase_EntitySetHandle)parent_entity_set, 
                  (iBase_EntitySetHandle)child_entity_set, 
                  is_child, 
                  &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isChildOf) */
#line 2008 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getNumChld[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getNumChld"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getNumChld(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_child,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2033 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getNumChld) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getNumChld} (getNumChld method) */
  iGeom_getNumChld(INSTANCE, 
                   (iBase_EntitySetHandle)entity_set, 
                   num_hops, 
                   num_child,
                   &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getNumChld) */
#line 2042 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getNumPrnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getNumPrnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getNumPrnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set,
  /* in */ int32_t num_hops,
  /* out */ int32_t* num_parent,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2067 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getNumPrnt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getNumPrnt} (getNumPrnt method) */
  iGeom_getNumPrnt(INSTANCE, 
                   (iBase_EntitySetHandle)entity_set, 
                   num_hops, 
                   num_parent,
                   &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getNumPrnt) */
#line 2076 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getChldn[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getChldn"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getChldn(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** child_handles,
  /* out */ int32_t* child_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2102 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getChldn) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getChldn} (getChldn method) */
  CREATE_TEMP_ESH_ARRAY(child_handles);
  
  iGeom_getChldn(INSTANCE, 
                 (iBase_EntitySetHandle)from_entity_set, 
                 num_hops, 
                 TEMP_ARRAY_INOUT(child_handles), 
                 &ERROR);
  PROCESS_ERROR;

  ASSIGN_ARRAY(opaque, child_handles);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getChldn) */
#line 2115 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getPrnts[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getPrnts"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getPrnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* from_entity_set,
  /* in */ int32_t num_hops,
  /* inout array<opaque> */ struct sidl_opaque__array** parent_handles,
  /* out */ int32_t* parent_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2141 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getPrnts) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getPrnts} (getPrnts method) */
  CREATE_TEMP_ESH_ARRAY(parent_handles);
  
  iGeom_getPrnts(INSTANCE, 
                 (iBase_EntitySetHandle)from_entity_set, 
                 num_hops, 
                 TEMP_ARRAY_INOUT(parent_handles), 
                 &ERROR);
  PROCESS_ERROR;
  
  ASSIGN_ARRAY(opaque, parent_handles);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getPrnts) */
#line 2154 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  subtract[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_subtract"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_subtract(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2179 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.subtract) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.subtract} (subtract method) */
  iGeom_subtract(INSTANCE, 
                 (iBase_EntitySetHandle)entity_set_1, 
                 (iBase_EntitySetHandle)entity_set_2, 
                 (iBase_EntitySetHandle*)result_entity_set, 
                 &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.subtract) */
#line 2188 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  intersect[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_intersect"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_intersect(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2213 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.intersect) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.intersect} (intersect method) */
  iGeom_intersect(INSTANCE, 
                  (iBase_EntitySetHandle)entity_set_1, 
                  (iBase_EntitySetHandle)entity_set_2, 
                  (iBase_EntitySetHandle*)result_entity_set, 
                  &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.intersect) */
#line 2222 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  unite[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_unite"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_unite(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_1,
  /* in */ void* entity_set_2,
  /* out */ void** result_entity_set,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2247 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.unite) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.unite} (unite method) */
  iGeom_unite(INSTANCE, 
              (iBase_EntitySetHandle)entity_set_1, 
              (iBase_EntitySetHandle)entity_set_2, 
              (iBase_EntitySetHandle*)result_entity_set, 
              &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.unite) */
#line 2256 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 *  
 * Load a model specified by name. Which formats are supported and the
 * specific meaning of this name string (e.g. file name, model name,
 * etc.) are implementation-dependent.  Options are also implementation-
 * dependent.
 * @param name Name of the model
 * @param options String options 
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_load(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2286 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.load) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.load} (load method) */
  iGeom_load(INSTANCE, name, options, &ERROR, 
             strlen(name), strlen(options));
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.load) */
#line 2292 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 *  
 * Save a model to a file specified by name. Which formats are supported and the
 * specific meaning of this name string (e.g. file name, model name,
 * etc.) are implementation-dependent.  Options are also implementation-
 * dependent.
 * @param name Name of the file to save to
 * @param options String options 
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_save"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_save(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ const char* name,
  /* in */ const char* options,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2322 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.save) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.save} (save method) */
  iGeom_save( INSTANCE, name, options, &ERROR, 
              strlen(name), strlen(options));
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.save) */
#line 2328 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return gentities of specified dimension in this set, or in whole model.
 * @param set_handle Entity set being queried (if 0, whole model)
 * @param gentity_dimension Dimension of entities being queried
 * @param gentity_handles Gentity handles
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntities"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntities(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* set_handle,
  /* in */ enum iBase_EntityType__enum gentity_type,
  /* inout array<opaque> */ struct sidl_opaque__array** gentity_handles,
  /* out */ int32_t* gentity_handles_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2357 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntities) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntities} (getEntities method) */
  if (gentity_type > iBase_EntityType_ALL_TYPES || 
      gentity_type < iBase_EntityType_VERTEX) {
    PROCESS_ERROR_MSG(iBase_INVALID_ARGUMENT, "Wrong entity type");
    return;
  }
    
  CREATE_TEMP_EH_ARRAY(gentity_handles);

  iGeom_getEntities (INSTANCE, 
                     (iBase_EntitySetHandle)set_handle,
                     gentity_type,
                     TEMP_ARRAY_INOUT(gentity_handles), 
                     &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(gentity_handles);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntities) */
#line 2376 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return number of gentities of specified dimension in this set, or in
 * whole model.
 * @param set_handle Entity set being queried (if 0, whole model)
 * @param gentity_dimension Dimension of entities being queried
 * @return Number of entities
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getNumOfType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getNumOfType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* set_handle,
  /* in */ enum iBase_EntityType__enum gentity_type,
  /* out */ int32_t* num_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2405 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getNumOfType) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getNumOfType} (getNumOfType method) */
  iGeom_getNumOfType(INSTANCE, 
                     (iBase_EntitySetHandle)set_handle,
                     gentity_type, 
                     num_type,
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getNumOfType) */
#line 2414 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* handle,
  /* out */ enum iBase_EntityType__enum* ent_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2438 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntType) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntType} (getEntType method) */
  int gtype;
  iGeom_getEntType ( INSTANCE, 
                     (iBase_EntityHandle)handle, 
                     &gtype, 
                     &ERROR );
  *ent_type = (enum iBase_EntityType__enum)gtype;
  PROCESS_ERROR;

    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntType) */
#line 2449 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Returns an integer array of topological dimensions for an input
 * array of entity handles.
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getArrType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* inout array<iBase.EntityType> */ struct iBase_EntityType__array** gtype,
  /* out */ int32_t* gtype_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2476 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrType) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrType} (getArrType method) */
  CREATE_TEMP_ENUM_ARRAY(gtype);
  
  iGeom_getArrType (INSTANCE, 
                    TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles), 
                    TEMP_ARRAY_INOUT(gtype), 
                    &ERROR);
  PROCESS_ERROR;

  ASSIGN_ENUM_ARRAY(gtype);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrType) */
#line 2488 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Get the adjacent entities of a given dimension.
 * @param gentity_handle Entity for which adjacencies are requested
 * @param to_dimension Target dimension of adjacent entities
 * @param adj_gentities List returned with adjacent entities
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntAdj"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ enum iBase_EntityType__enum to_dimension,
  /* inout array<opaque> */ struct sidl_opaque__array** adj_gentities,
  /* out */ int32_t* adj_gentities_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2517 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntAdj) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntAdj} (getEntAdj method) */
  CREATE_TEMP_EH_ARRAY(adj_gentities);
  
  iGeom_getEntAdj (INSTANCE, 
                   (iBase_EntityHandle)entity_handle,
                   to_dimension,
                   TEMP_ARRAY_INOUT(adj_gentities), 
                   &ERROR);
  PROCESS_ERROR;

  ASSIGN_ARRAY(opaque, adj_gentities);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntAdj) */
#line 2530 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Get the adjacent entities of a given dimension.
 * @param gentity_handle Entity for which adjacencies are requested
 * @param to_dimension Target dimension of adjacent entities
 * @param adj_gentities List returned with adjacent entities
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrAdj"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2562 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrAdj) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrAdj} (getArrAdj method) */
  CREATE_TEMP_EH_ARRAY(adj_entity_handles);
  CREATE_TEMP_INT_ARRAY(offset);
  
  iGeom_getArrAdj (INSTANCE, 
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, entity_handles),
                   requested_entity_type,
                   TEMP_ARRAY_INOUT(adj_entity_handles), 
                   TEMP_ARRAY_INOUT(offset),
                   &ERROR );
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(adj_entity_handles);
  ASSIGN_INT_ARRAY(offset);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrAdj) */
#line 2578 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Get the "2nd order" adjacent entities, through a specified "bridge"
 * dimension, of a target dimension.  For example, given a region, return
 * the regions (to_dimension=3) sharing an edge (bridge_dimension=1)
 * with that region.  bridge_dimension must be less than dimension of 
 * gentity_handle, and to_dimension must be greater than bridge dimension.
 * 
 * @param gentity_handle Entity for which 2nd order adjacencies are requested
 * @param to_dimension Target dimension of 2nd order adjacent entities
 * @param bridge_dimension Dimension of "bridge" entities
 * @param adj_gentities List returned with 2nd order adjacent entities
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEnt2ndAdj"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEnt2ndAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_handle,
  /* in */ enum iBase_EntityType__enum bridge_dimension,
  /* in */ enum iBase_EntityType__enum to_dimension,
  /* inout array<opaque> */ struct sidl_opaque__array** adjacent_gentities,
  /* out */ int32_t* adjacent_gentities_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2614 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEnt2ndAdj) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEnt2ndAdj} (getEnt2ndAdj method) */
  CREATE_TEMP_EH_ARRAY(adjacent_gentities);
  
  iGeom_getEnt2ndAdj (INSTANCE, 
                      (iBase_EntityHandle)gentity_handle,
                      bridge_dimension,
                      to_dimension,
                      TEMP_ARRAY_INOUT(adjacent_gentities), 
                      &ERROR);
  PROCESS_ERROR;
  
  ASSIGN_ARRAY(opaque, adjacent_gentities);
  
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEnt2ndAdj) */
#line 2629 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Get the "2nd order" adjacent entities, through a specified "bridge"
 * dimension, of a target dimension.  For example, given a region, return
 * the regions (to_dimension=3) sharing an edge (bridge_dimension=1)
 * with that region.  bridge_dimension must be less than dimension of 
 * gentity_handle, and to_dimension must be greater than bridge dimension.
 * 
 * @param gentity_handle Entity for which 2nd order adjacencies are requested
 * @param order_adjacent_key Dimension of "bridge" entities
 * @param requested_entity_type Target dimension of 2nd order adjacent entities
 * @param adj_entity_handles List returned with 2nd order adjacent entities
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArr2ndAdj"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2668 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArr2ndAdj) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArr2ndAdj} (getArr2ndAdj method) */
  CREATE_TEMP_EH_ARRAY(adj_entity_handles);
  CREATE_TEMP_INT_ARRAY(offset);
  
  iGeom_getArr2ndAdj (INSTANCE, 
                   TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                   order_adjacent_key,
                   requested_entity_type,
                   TEMP_ARRAY_INOUT(adj_entity_handles),
                   TEMP_ARRAY_INOUT(offset), 
                   &ERROR);
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(adj_entity_handles);
  ASSIGN_INT_ARRAY(offset);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArr2ndAdj) */
#line 2685 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return whether or not entities are adjacent.
 * @param gentity_handle1 1st entity
 * @param gentity_handle2 2nd entity
 * @param are_adjacent If true, entities are adjacent
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isEntAdj"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isEntAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_handle1,
  /* in */ void* gentity_handle2,
  /* out */ sidl_bool* are_adjacent,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2713 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isEntAdj) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isEntAdj} (isEntAdj method) */
  int adj;
  iGeom_isEntAdj (INSTANCE, 
                  (iBase_EntityHandle)gentity_handle1,
                  (iBase_EntityHandle)gentity_handle2,
                  &adj, 
                  &ERROR);
  PROCESS_ERROR;
  *are_adjacent = !!adj;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isEntAdj) */
#line 2724 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return whether or not arrays of entities are adjacent.
 * @param gentity_handle1 1st entity
 * @param gentity_handle2 2nd entity
 * @param are_adjacent If true, entities are adjacent
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isArrAdj"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isArrAdj(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles_1,
  /* in */ int32_t entity_handles_1_size,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles_2,
  /* in */ int32_t entity_handles_2_size,
  /* inout array<int> */ struct sidl_int__array** is_adjacent_info,
  /* out */ int32_t* is_adjacent_info_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2755 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isArrAdj) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isArrAdj} (isArrAdj method) */
  CREATE_TEMP_INT_ARRAY(is_adjacent_info);
  iGeom_isArrAdj (INSTANCE, 
                  TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles_1),
                  TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles_2),
                  TEMP_ARRAY_INOUT(is_adjacent_info), 
                  &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(is_adjacent_info);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isArrAdj) */
#line 2766 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getTopoLevel[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTopoLevel"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTopoLevel(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ int32_t* level,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2789 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTopoLevel) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTopoLevel} (getTopoLevel method) */
  iGeom_getTopoLevel (INSTANCE, level, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTopoLevel) */
#line 2794 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntClosestPt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntClosestPt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2823 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntClosestPt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntClosestPt} (getEntClosestPt method) */
  iGeom_getEntClosestPt (INSTANCE, 
                         (iBase_EntityHandle)entity_handle,
                         near_x, near_y, near_z, 
                         on_x, on_y, on_z, 
                         &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntClosestPt) */
#line 2832 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return a points on specified entities closest to specified points
 * in space.  Input coordinates and output points are interleaved in 
 * the arrays.
 * @param gentity_handles The gentities being queried
 * @param near_coordinates Input coordinates
 * @param on_coordinates Closest point on gentity
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrClosestPt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2866 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrClosestPt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrClosestPt} (getArrClosestPt method) */
  CREATE_TEMP_DBL_ARRAY(on_coordinates);
   iGeom_getArrClosestPt (INSTANCE, 
                             TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                             storage_order,
                             TEMP_ARRAY_IN(near_coordinates),
                             TEMP_ARRAY_INOUT(on_coordinates), 
                             &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(on_coordinates);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrClosestPt) */
#line 2879 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntNrmlXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntNrmlXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2908 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntNrmlXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntNrmlXYZ} (getEntNrmlXYZ method) */
  iGeom_getEntNrmlXYZ (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       x, y, z, 
                       nrml_i, nrml_j, nrml_k, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntNrmlXYZ) */
#line 2917 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the normals at point on specified entities.  Returns error
 * if any input entity is not a gface.  Input coordinates and normals
 * are interleaved in the arrays.
 * @param gentity_handles The gentities being queried
 * @param coordinates Input coordinates, interleaved
 * @param normals The normals at the specified points, interleaved
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrNrmlXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2951 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrNrmlXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrNrmlXYZ} (getArrNrmlXYZ method) */
  CREATE_TEMP_DBL_ARRAY(normals);
  iGeom_getArrNrmlXYZ (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                       storage_order,
                       TEMP_ARRAY_IN(coordinates),
                       TEMP_ARRAY_INOUT(normals), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(normals);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrNrmlXYZ) */
#line 2964 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntNrmlPlXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntNrmlPlXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 2996 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntNrmlPlXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntNrmlPlXYZ} (getEntNrmlPlXYZ method) */
  iGeom_getEntNrmlPlXYZ (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       x, y, z,
                       pt_x, pt_y, pt_z, 
                       nrml_i, nrml_j, nrml_k, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntNrmlPlXYZ) */
#line 3006 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return points and normals on specified entities closest to specified points
 * in space.  Input coordinates and output points are interleaved in 
 * the arrays.
 * @param gentity_handles The gentities being queried
 * @param near_coordinates Input coordinates
 * @param on_coordinates Closest point on gentity
 * @param normals Normals on gentity
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrNrmlPlXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3043 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrNrmlPlXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrNrmlPlXYZ} (getArrNrmlPlXYZ method) */
  CREATE_TEMP_DBL_ARRAY(on_coordinates);
  CREATE_TEMP_DBL_ARRAY(normals);
  iGeom_getArrNrmlPlXYZ (INSTANCE, 
                             TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                             storage_order,
                             TEMP_ARRAY_IN(near_coordinates),
                             TEMP_ARRAY_INOUT(on_coordinates),
                             TEMP_ARRAY_INOUT(normals), 
                             &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(on_coordinates);
  ASSIGN_DBL_ARRAY(normals);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrNrmlPlXYZ) */
#line 3059 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntTgntXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntTgntXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3088 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntTgntXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntTgntXYZ} (getEntTgntXYZ method) */
  iGeom_getEntTgntXYZ (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       x, y, z, 
                       tgnt_i, tgnt_j, tgnt_k, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntTgntXYZ) */
#line 3097 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the tangent at point on specified entities.  Returns error
 * if any input entity is not a gedge.  Input coordinates and tangents
 * are interleaved in the arrays.
 * @param gentity_handles The gentities being queried
 * @param coordinates Input coordinates, interleaved
 * @param tangents The tangents at the specified points, interleaved
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrTgntXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3131 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrTgntXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrTgntXYZ} (getArrTgntXYZ method) */
  CREATE_TEMP_DBL_ARRAY(tangents);
  iGeom_getArrTgntXYZ (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coordinates),
                        TEMP_ARRAY_INOUT(tangents), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(tangents);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrTgntXYZ) */
#line 3144 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getFcCvtrXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getFcCvtrXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3176 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getFcCvtrXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getFcCvtrXYZ} (getFcCvtrXYZ method) */
  iGeom_getFcCvtrXYZ (INSTANCE, 
                       (iBase_EntityHandle)face_handle,
                       x, y, z, 
                       cvtr1_i, cvtr1_j, cvtr1_k,
                       cvtr2_i, cvtr2_j, cvtr2_k, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getFcCvtrXYZ) */
#line 3186 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEgCvtrXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEgCvtrXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3215 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEgCvtrXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEgCvtrXYZ} (getEgCvtrXYZ method) */
  iGeom_getEgCvtrXYZ ( INSTANCE, 
                       (iBase_EntityHandle)edge_handle,
                       x, y, z, 
                       cvtr_i, cvtr_j, cvtr_k, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEgCvtrXYZ) */
#line 3224 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntArrCvtrXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntArrCvtrXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3255 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntArrCvtrXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntArrCvtrXYZ} (getEntArrCvtrXYZ method) */
  CREATE_TEMP_DBL_ARRAY(cvtr_1);
  CREATE_TEMP_DBL_ARRAY(cvtr_2);
  iGeom_getEntArrCvtrXYZ (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coords),
                        TEMP_ARRAY_INOUT(cvtr_1),
                        TEMP_ARRAY_INOUT(cvtr_2), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(cvtr_1);
  ASSIGN_DBL_ARRAY(cvtr_2);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntArrCvtrXYZ) */
#line 3271 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEgEvalXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEgEvalXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3306 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEgEvalXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEgEvalXYZ} (getEgEvalXYZ method) */
  iGeom_getEgEvalXYZ (INSTANCE, 
                       (iBase_EntityHandle)edge_handle,
                       x, y, z, 
                       on_x, on_y, on_z,
                       tgnt_i, tgnt_j, tgnt_k,
                       cvtr_i, cvtr_j, cvtr_k, 
                       &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEgEvalXYZ) */
#line 3317 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getFcEvalXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getFcEvalXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3355 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getFcEvalXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getFcEvalXYZ} (getFcEvalXYZ method) */
  iGeom_getFcEvalXYZ (INSTANCE, 
                       (iBase_EntityHandle)face_handle,
                       x, y, z, 
                       on_x, on_y, on_z,
                       nrml_i, nrml_j, nrml_k,
                       cvtr1_i, cvtr1_j, cvtr1_k,
                       cvtr2_i, cvtr2_j, cvtr2_k, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getFcEvalXYZ) */
#line 3367 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrEgEvalXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrEgEvalXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3400 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrEgEvalXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrEgEvalXYZ} (getArrEgEvalXYZ method) */
  CREATE_TEMP_DBL_ARRAY(on_coords);
  CREATE_TEMP_DBL_ARRAY(tangent);
  CREATE_TEMP_DBL_ARRAY(cvtr);

  iGeom_getArrEgEvalXYZ (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,edge_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coords),
                        TEMP_ARRAY_INOUT(on_coords),
                        TEMP_ARRAY_INOUT(tangent),
                        TEMP_ARRAY_INOUT(cvtr), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(on_coords);
  ASSIGN_DBL_ARRAY(tangent);
  ASSIGN_DBL_ARRAY(cvtr);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrEgEvalXYZ) */
#line 3420 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrFcEvalXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrFcEvalXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3455 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrFcEvalXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrFcEvalXYZ} (getArrFcEvalXYZ method) */
  CREATE_TEMP_DBL_ARRAY(on_coords);
  CREATE_TEMP_DBL_ARRAY(normal);
  CREATE_TEMP_DBL_ARRAY(cvtr_1);
  CREATE_TEMP_DBL_ARRAY(cvtr_2);

  iGeom_getArrFcEvalXYZ (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coords),
                        TEMP_ARRAY_INOUT(on_coords),
                        TEMP_ARRAY_INOUT(normal),
                        TEMP_ARRAY_INOUT(cvtr_1),
                        TEMP_ARRAY_INOUT(cvtr_2), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(on_coords);
  ASSIGN_DBL_ARRAY(normal);
  ASSIGN_DBL_ARRAY(cvtr_1);
  ASSIGN_DBL_ARRAY(cvtr_2);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrFcEvalXYZ) */
#line 3478 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntBoundBox[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntBoundBox"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3507 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntBoundBox) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntBoundBox} (getEntBoundBox method) */
  iGeom_getEntBoundBox (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       min_x, min_y, min_z,
                       max_x, max_y, max_z, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntBoundBox) */
#line 3516 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the bounding boxex of given entities; coordinates returned
 * interleaved.
 * @param gentity_handles The gentities being queried
 * @param min_corners Minimum corner coordinates of the boxes, interleaved
 * @param max_corners Maximum corner coordinates of the boxes, interleaved
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrBoundBox"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3549 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrBoundBox) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrBoundBox} (getArrBoundBox method) */
  CREATE_TEMP_DBL_ARRAY(max_corner);
  CREATE_TEMP_DBL_ARRAY(min_corner);
  
  iGeom_getArrBoundBox (INSTANCE, 
                            TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                            storage_order,
                            TEMP_ARRAY_INOUT(min_corner),
                            TEMP_ARRAY_INOUT(max_corner), 
                            &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(min_corner);
  ASSIGN_DBL_ARRAY(max_corner);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrBoundBox) */
#line 3567 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getVtxCoord[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getVtxCoord"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getVtxCoord(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* vertex_handle,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3593 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getVtxCoord) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getVtxCoord} (getVtxCoord method) */
  
  iGeom_getVtxCoord (INSTANCE, (iBase_EntityHandle)vertex_handle, x, y, z, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getVtxCoord) */
#line 3599 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the coordinates of the specified vertices; returns error if any
 * of the entities are not gvertices.  Coordinates returned interleaved.
 * @param gentity_handles The gentities being queried
 * @param coordinates The coordinates of the gvertices, interleaved.
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getVtxArrCoords"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getVtxArrCoords(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* in */ enum iBase_StorageOrder__enum storage_order,
  /* inout array<double> */ struct sidl_double__array** coordinates,
  /* out */ int32_t* coordinates_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3629 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getVtxArrCoords) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getVtxArrCoords} (getVtxArrCoords method) */
  CREATE_TEMP_DBL_ARRAY(coordinates);
  
  iGeom_getVtxArrCoords (INSTANCE, 
                               TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                               storage_order,
                               TEMP_ARRAY_INOUT(coordinates), 
                               &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(coordinates);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getVtxArrCoords) */
#line 3644 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getPntRayIntsct[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getPntRayIntsct"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3680 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getPntRayIntsct) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getPntRayIntsct} (getPntRayIntsct method) */
  CREATE_TEMP_EH_ARRAY(intersect_entity_handles);
  CREATE_TEMP_DBL_ARRAY(intersect_coords);
  CREATE_TEMP_DBL_ARRAY(param_coords);
  
  iGeom_getPntRayIntsct (INSTANCE, x, y, z, dir_x, dir_y, dir_z,
                      TEMP_ARRAY_INOUT( intersect_entity_handles ),
                      storage_order,
                      TEMP_ARRAY_INOUT( intersect_coords ),
                      TEMP_ARRAY_INOUT( param_coords ), 
                      &ERROR );
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(intersect_entity_handles);
  ASSIGN_DBL_ARRAY(intersect_coords);
  ASSIGN_DBL_ARRAY(param_coords);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getPntRayIntsct) */
#line 3700 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getPntArrRayIntsct[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getPntArrRayIntsct"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3736 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getPntArrRayIntsct) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getPntArrRayIntsct} (getPntArrRayIntsct method) */
  CREATE_TEMP_EH_ARRAY(intersect_entity_handles);
  CREATE_TEMP_INT_ARRAY(offset);
  CREATE_TEMP_DBL_ARRAY(intersect_coords);
  CREATE_TEMP_DBL_ARRAY(param_coords);
  iGeom_getPntArrRayIntsct (INSTANCE, 
                      storage_order,
                      TEMP_ARRAY_IN( coords ),
                      TEMP_ARRAY_IN( directions ),
                      TEMP_ARRAY_INOUT( intersect_entity_handles ),
                      TEMP_ARRAY_INOUT( offset ),
                      TEMP_ARRAY_INOUT( intersect_coords ),
                      TEMP_ARRAY_INOUT( param_coords ), 
                      &ERROR );
  PROCESS_ERROR;

  ASSIGN_OPAQUE_ARRAY(intersect_entity_handles);
  ASSIGN_INT_ARRAY(offset);
  ASSIGN_DBL_ARRAY(intersect_coords);
  ASSIGN_DBL_ARRAY(param_coords);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getPntArrRayIntsct) */
#line 3758 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the sense of a gface with respect to a gregion.  Sense is either
 * forward (=1), reverse (=-1), both (=2), or unknown (=0).  Error is returned
 * if first entity is not a gface or second entity is not a gregion.
 * @param gface Gface whose sense is being queried.
 * @param gregion Gregion gface is being queried with respect to
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntNrmlSense"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntNrmlSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gface,
  /* in */ void* gregion,
  /* out */ int32_t* sense,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3787 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntNrmlSense) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntNrmlSense} (getEntNrmlSense method) */
  iGeom_getEntNrmlSense (INSTANCE, 
                         (iBase_EntityHandle)gface,
                         (iBase_EntityHandle)gregion, 
                         sense,
                         &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntNrmlSense) */
#line 3796 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrNrmlSense[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrNrmlSense"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getArrNrmlSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* region_handles,
  /* in */ int32_t region_handles_size,
  /* inout array<int> */ struct sidl_int__array** sense,
  /* out */ int32_t* sense_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3824 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrNrmlSense) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrNrmlSense} (getArrNrmlSense method) */
  CREATE_TEMP_INT_ARRAY(sense);
  iGeom_getArrNrmlSense (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles), 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,region_handles), 
                       TEMP_ARRAY_INOUT(sense), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(sense);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrNrmlSense) */
#line 3835 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the sense of a gedge with respect to a gface.  Sense is either
 * forward (=1), reverse (=-1), both (=2), or unknown (=0).  Error is returned
 * if first entity is not a gedge or second entity is not a gface.
 * @param gedge Gedge whose sense is being queried.
 * @param gface Gface gedge is being queried with respect to
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEgFcSense"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEgFcSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gedge,
  /* in */ void* gface,
  /* out */ int32_t* sense,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3864 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEgFcSense) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEgFcSense} (getEgFcSense method) */
  iGeom_getEgFcSense (INSTANCE, 
                      (iBase_EntityHandle)gedge,
                      (iBase_EntityHandle)gface, 
                      sense,
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEgFcSense) */
#line 3873 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEgFcArrSense[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEgFcArrSense"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEgFcArrSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* inout array<int> */ struct sidl_int__array** sense,
  /* out */ int32_t* sense_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3901 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEgFcArrSense) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEgFcArrSense} (getEgFcArrSense method) */
  CREATE_TEMP_INT_ARRAY(sense);
  iGeom_getEgFcArrSense (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,edge_handles), 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles), 
                       TEMP_ARRAY_INOUT(sense), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(sense);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEgFcArrSense) */
#line 3912 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the sense of a gedge with respect to a specified order of
 * vertices bounding the gedge.  Sense is either forward (=1), reverse (=-1), 
 * or unknown (=0).  Error is returned if any gentities are not the expected
 * type or if the gedge is bounded by only one gvertex (in this case, use
 * getGtangentSense).
 * @param gedge Gedge whose sense is being queried.
 * @param gvertex1 First gvertex
 * @param gvertex2 Second gvertex
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEgVtxSense"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEgVtxSense(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gedge,
  /* in */ void* gvertex1,
  /* in */ void* gvertex2,
  /* out */ int32_t* sense,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3945 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEgVtxSense) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEgVtxSense} (getEgVtxSense method) */
  iGeom_getEgVtxSense (INSTANCE, 
                       (iBase_EntityHandle)gedge,
                       (iBase_EntityHandle)gvertex1,
                       (iBase_EntityHandle)gvertex2, 
                       sense,
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEgVtxSense) */
#line 3955 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEgVtxArrSense[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEgVtxArrSense"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 3985 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEgVtxArrSense) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEgVtxArrSense} (getEgVtxArrSense method) */
  CREATE_TEMP_INT_ARRAY(sense);
  iGeom_getEgVtxArrSense (INSTANCE,
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,edge_handles), 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,vertex_handles_1), 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,vertex_handles_2), 
                       TEMP_ARRAY_INOUT(sense), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(sense);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEgVtxArrSense) */
#line 3997 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the arc length / area / volume of the entities
 * @param gentity_handles Entities for which measure is requested
 * @param gentity_handles_size Number of gentities
 * @param measures Arc length / area / volume of the entities
 * @param measures_length Number of entries in measures
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_measure"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_measure(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* gentity_handles,
  /* in */ int32_t gentity_handles_size,
  /* inout array<double> */ struct sidl_double__array** measures,
  /* out */ int32_t* measures_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4027 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.measure) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.measure} (measure method) */
  CREATE_TEMP_DBL_ARRAY(measures);
  iGeom_measure(INSTANCE, 
                TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                TEMP_ARRAY_INOUT(measures), 
                &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(measures);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.measure) */
#line 4038 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the type of surface as a string; if not a surface, an error is returned
 * @param face_handle Face for which the type is requested
 * @param face_type Type of face, returned as a string
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getFaceType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getFaceType(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gface_handle,
  /* inout */ char** face_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4064 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getFaceType) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getFaceType} (getFaceType method) */
  char buffer[256];
  int len = sizeof(buffer);
  iGeom_getFaceType( INSTANCE, 
                     (iBase_EntityHandle)gface_handle,
                     buffer,
                     &ERROR,
                     &len );
  buffer[len-1] = '\0';
  PROCESS_ERROR;
  *face_type = strdup(buffer);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getFaceType) */
#line 4077 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getParametric[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getParametric"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getParametric(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ int32_t* parametric,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4100 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getParametric) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getParametric} (getParametric method) */
  iGeom_getParametric( INSTANCE, parametric, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getParametric) */
#line 4105 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return whether a given gentity is parametric or not.  If a gentity
 * is not parametric, all of the following functions will return an error
 * when called on that entity.
 * @param gentity_handle Gentity being queried.
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isEntParametric"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isEntParametric(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_handle,
  /* out */ int32_t* is_parametric,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4132 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isEntParametric) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isEntParametric} (isEntParametric method) */
  iGeom_isEntParametric( INSTANCE, 
                         (iBase_EntityHandle)gentity_handle,
                         is_parametric,
                         &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isEntParametric) */
#line 4140 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isArrParametric[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isArrParametric"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isArrParametric(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** is_parametric,
  /* out */ int32_t* is_parametric_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4166 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isArrParametric) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isArrParametric} (isArrParametric method) */
  CREATE_TEMP_INT_ARRAY(is_parametric);
  
  iGeom_isArrParametric (INSTANCE, 
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                         TEMP_ARRAY_INOUT(is_parametric), 
                         &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(is_parametric);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isArrParametric) */
#line 4177 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntUVtoXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntUVtoXYZ"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntUVtoXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4205 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntUVtoXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntUVtoXYZ} (getEntUVtoXYZ method) */
  iGeom_getEntUVtoXYZ (INSTANCE, (iBase_EntityHandle)entity_handle, 
                       u, v, x, y, z, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntUVtoXYZ) */
#line 4211 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Given sets of parametric coordinates, return the corresponding real
 * space coordinates on the gentities.  Input and output coordinates are
 * interleaved.
 * @param gentity_handles Gentities being queried.
 * @param uv Input parametric coordinates
 * @param xyz Output real space coordinates
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrUVtoXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4245 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrUVtoXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrUVtoXYZ} (getArrUVtoXYZ method) */
  CREATE_TEMP_DBL_ARRAY(coordinates);
  iGeom_getArrUVtoXYZ (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                        storage_order,
                        TEMP_ARRAY_IN(uv),
                        TEMP_ARRAY_INOUT(coordinates), 
                        &ERROR);
  PROCESS_ERROR;
  ASSIGN_DBL_ARRAY(coordinates);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrUVtoXYZ) */
#line 4257 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntUtoXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntUtoXYZ"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntUtoXYZ(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* out */ double* x,
  /* out */ double* y,
  /* out */ double* z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4284 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntUtoXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntUtoXYZ} (getEntUtoXYZ method) */
  iGeom_getEntUtoXYZ (INSTANCE, (iBase_EntityHandle)entity_handle, 
                      u, x, y, z, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntUtoXYZ) */
#line 4290 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrUtoXYZ[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrUtoXYZ"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4319 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrUtoXYZ) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrUtoXYZ} (getArrUtoXYZ method) */
  CREATE_TEMP_DBL_ARRAY(on_coords);
  iGeom_getArrUtoXYZ (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                        TEMP_TYPED_ARRAY_IN(double,u),
                        storage_order,
                        TEMP_ARRAY_INOUT(on_coords), 
                        &ERROR);
  PROCESS_ERROR;
  ASSIGN_DBL_ARRAY(on_coords);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrUtoXYZ) */
#line 4333 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntXYZtoUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntXYZtoUV"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntXYZtoUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* u,
  /* out */ double* v,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4361 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntXYZtoUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntXYZtoUV} (getEntXYZtoUV method) */
  iGeom_getEntXYZtoUV (INSTANCE, (iBase_EntityHandle)entity_handle, 
                       x, y, z, u, v, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntXYZtoUV) */
#line 4367 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntXYZtoU[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntXYZtoU"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntXYZtoU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ double* u,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4394 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntXYZtoU) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntXYZtoU} (getEntXYZtoU method) */
  iGeom_getEntXYZtoU (INSTANCE, (iBase_EntityHandle)entity_handle, 
                      x, y, z, u, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntXYZtoU) */
#line 4400 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Given sets of real space coordinates, return the corresponding 
 * parametric coordinates on the gentities.  Input and output coordinates 
 * are interleaved.
 * @param gentity_handles Gentities being queried.
 * @param xyz Input real space coordinates
 * @param uv Output parametric coordinates
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrXYZtoUV"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4434 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrXYZtoUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrXYZtoUV} (getArrXYZtoUV method) */
  CREATE_TEMP_DBL_ARRAY(uv);
  iGeom_getArrXYZtoUV (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coordinates),
                        TEMP_ARRAY_INOUT(uv), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(uv);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrXYZtoUV) */
#line 4447 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrXYZtoU[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrXYZtoU"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4476 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrXYZtoU) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrXYZtoU} (getArrXYZtoU method) */
  CREATE_TEMP_DBL_ARRAY(u);
  iGeom_getArrXYZtoU (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coordinates),
                        TEMP_ARRAY_INOUT(u), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(u);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrXYZtoU) */
#line 4489 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntXYZtoUVHint[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntXYZtoUVHint"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntXYZtoUVHint(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* inout */ double* u,
  /* inout */ double* v,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4517 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntXYZtoUVHint) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntXYZtoUVHint} (getEntXYZtoUVHint method) */
  iGeom_getEntXYZtoUVHint (INSTANCE, (iBase_EntityHandle)entity_handle, 
                           x, y, z, u, v, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntXYZtoUVHint) */
#line 4523 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrXYZtoUVHint[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrXYZtoUVHint"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4552 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrXYZtoUVHint) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrXYZtoUVHint} (getArrXYZtoUVHint method) */
  CREATE_TEMP_DBL_ARRAY(uv);
  iGeom_getArrXYZtoUVHint (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                        storage_order,
                        TEMP_ARRAY_IN(coords),
                        TEMP_ARRAY_INOUT(uv), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(uv);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrXYZtoUVHint) */
#line 4565 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntUVRange[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntUVRange"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntUVRange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* u_min,
  /* out */ double* v_min,
  /* out */ double* u_max,
  /* out */ double* v_max,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4592 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntUVRange) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntUVRange} (getEntUVRange method) */
  iGeom_getEntUVRange (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       u_min, v_min, 
                       u_max, v_max, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntUVRange) */
#line 4601 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntURange[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntURange"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntURange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* u_min,
  /* out */ double* u_max,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4626 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntURange) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntURange} (getEntURange method) */
  iGeom_getEntURange (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       u_min, u_max, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntURange) */
#line 4634 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the uv range of the specified gentities.  Parameters are interleaved.
 * @param gentity_handles Gentities being queried.
 * @param uv_min Minimum parameters of gentities, interleaved
 * @param uv_max Maximum parameters of gentities, interleaved
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrUVRange"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4666 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrUVRange) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrUVRange} (getArrUVRange method) */
  CREATE_TEMP_DBL_ARRAY(uv_max);
  CREATE_TEMP_DBL_ARRAY(uv_min);
  iGeom_getArrUVRange (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gentity_handles),
                        storage_order,
                        TEMP_ARRAY_INOUT(uv_min),
                        TEMP_ARRAY_INOUT(uv_max), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(uv_max);
  ASSIGN_DBL_ARRAY(uv_min);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrUVRange) */
#line 4683 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrURange[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrURange"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getArrURange(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<double> */ struct sidl_double__array** u_min,
  /* out */ int32_t* u_min_size,
  /* inout array<double> */ struct sidl_double__array** u_max,
  /* out */ int32_t* u_max_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4711 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrURange) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrURange} (getArrURange method) */
  CREATE_TEMP_DBL_ARRAY(u_max);
  CREATE_TEMP_DBL_ARRAY(u_min);
  
  iGeom_getArrURange (INSTANCE, 
                        TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                         TEMP_ARRAY_INOUT(u_min),
                        TEMP_ARRAY_INOUT(u_max), 
                        &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(u_max);
  ASSIGN_DBL_ARRAY(u_min);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrURange) */
#line 4726 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntUtoUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntUtoUV"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntUtoUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* edge_handle,
  /* in */ void* face_handle,
  /* in */ double in_u,
  /* out */ double* u,
  /* out */ double* v,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4753 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntUtoUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntUtoUV} (getEntUtoUV method) */
  iGeom_getEntUtoUV (INSTANCE, 
                     (iBase_EntityHandle)edge_handle,
                     (iBase_EntityHandle)face_handle,
                     in_u, u, v, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntUtoUV) */
#line 4762 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getVtxToUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getVtxToUV"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getVtxToUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* vertex_handle,
  /* in */ void* face_handle,
  /* out */ double* u,
  /* out */ double* v,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4788 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getVtxToUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getVtxToUV} (getVtxToUV method) */
  iGeom_getVtxToUV (INSTANCE, 
                     (iBase_EntityHandle)vertex_handle,
                     (iBase_EntityHandle)face_handle,
                     u, v, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getVtxToUV) */
#line 4797 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getVtxToU[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getVtxToU"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getVtxToU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* vertex_handle,
  /* in */ void* edge_handle,
  /* out */ double* u,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4822 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getVtxToU) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getVtxToU} (getVtxToU method) */
  iGeom_getVtxToU (INSTANCE, 
                     (iBase_EntityHandle)vertex_handle,
                     (iBase_EntityHandle)edge_handle,
                     u, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getVtxToU) */
#line 4831 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArrUtoUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrUtoUV"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4862 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrUtoUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrUtoUV} (getArrUtoUV method) */
  CREATE_TEMP_DBL_ARRAY(uv);
  iGeom_getArrUtoUV (INSTANCE, 
                     TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,edge_handles),
                     TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles),
                     TEMP_ARRAY_IN(u_in),
                     storage_order,
                     TEMP_ARRAY_INOUT(uv), 
                     &ERROR);
  PROCESS_ERROR;
  ASSIGN_DBL_ARRAY(uv);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrUtoUV) */
#line 4877 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getVtxArrToUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getVtxArrToUV"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4906 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getVtxArrToUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getVtxArrToUV} (getVtxArrToUV method) */
  CREATE_TEMP_DBL_ARRAY(uv);
  iGeom_getVtxArrToUV (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,vertex_handles),
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles),
                       storage_order,
                       TEMP_ARRAY_INOUT(uv), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_DBL_ARRAY(uv);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getVtxArrToUV) */
#line 4920 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getVtxArrToU[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getVtxArrToU"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getVtxArrToU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* vertex_handles,
  /* in */ int32_t vertex_handles_size,
  /* in array<opaque> */ struct sidl_opaque__array* edge_handles,
  /* in */ int32_t edge_handles_size,
  /* inout array<double> */ struct sidl_double__array** u,
  /* out */ int32_t* u_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4948 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getVtxArrToU) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getVtxArrToU} (getVtxArrToU method) */
  CREATE_TEMP_DBL_ARRAY(u);
  iGeom_getVtxArrToU (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,vertex_handles),
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,edge_handles),
                       TEMP_ARRAY_INOUT(u), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_DBL_ARRAY(u);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getVtxArrToU) */
#line 4959 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntNrmlUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntNrmlUV"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntNrmlUV(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double u,
  /* in */ double v,
  /* out */ double* nrml_i,
  /* out */ double* nrml_j,
  /* out */ double* nrml_k,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 4987 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntNrmlUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntNrmlUV} (getEntNrmlUV method) */
  iGeom_getEntNrmlUV (INSTANCE, (iBase_EntityHandle)entity_handle, u, v, 
                      nrml_i, nrml_j, nrml_k, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntNrmlUV) */
#line 4993 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the normals at specified uv positions on gfaces.  If any
 * gentity input is not a face, returns error.  Input parameters and 
 * output normals are interleaved.
 * @param gface_handles The entities being queried
 * @param parameters The uv parameters of points being queried, interleaved
 * @param normals Normals at specified points, interleaved
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrNrmlUV"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5027 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrNrmlUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrNrmlUV} (getArrNrmlUV method) */
  CREATE_TEMP_DBL_ARRAY(normals);
  iGeom_getArrNrmlUV (INSTANCE, 
                      TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,gface_handles),
                      storage_order,
                      TEMP_TYPED_ARRAY_IN(double, parameters),
                      TEMP_ARRAY_INOUT(normals), 
                      &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(normals);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrNrmlUV) */
#line 5040 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntTgntU[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntTgntU"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntTgntU(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* in */ double param_coord,
  /* out */ double* tngt_i,
  /* out */ double* tngt_j,
  /* out */ double* tngt_k,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5067 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntTgntU) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntTgntU} (getEntTgntU method) */
  iGeom_getEntTgntU (INSTANCE, (iBase_EntityHandle)entity_handle, param_coord, 
                     tngt_i, tngt_j, tngt_k, &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntTgntU) */
#line 5073 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the tangents at specified u positions on gedges.  If any
 * gentity input is not a face, returns error.  Output normals are 
 * interleaved.
 * @param gentity_handles The gedges being queried
 * @param parameters The u parameters of points being queried
 * @param tangents Tangents at specified points, interleaved
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrTgntU"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5107 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrTgntU) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrTgntU} (getArrTgntU method) */
  CREATE_TEMP_DBL_ARRAY(tangents);
  iGeom_getArrTgntU (INSTANCE, 
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, gedge_handles),
                         storage_order,
                         TEMP_ARRAY_IN(parameters),
                         TEMP_ARRAY_INOUT(tangents), 
                         &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(tangents);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrTgntU) */
#line 5120 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEnt1stDrvt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEnt1stDrvt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5149 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEnt1stDrvt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEnt1stDrvt} (getEnt1stDrvt method) */
  CREATE_TEMP_DBL_ARRAY(drvt_u);
  CREATE_TEMP_DBL_ARRAY(drvt_v);
  
  iGeom_getEnt1stDrvt (INSTANCE, 
                       (iBase_EntityHandle)entity_handle, u, v,
                       TEMP_ARRAY_INOUT(drvt_u),
                       TEMP_ARRAY_INOUT(drvt_v), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(drvt_u);
  ASSIGN_DBL_ARRAY(drvt_v);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEnt1stDrvt) */
#line 5164 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArr1stDrvt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArr1stDrvt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5199 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArr1stDrvt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArr1stDrvt} (getArr1stDrvt method) */
  CREATE_TEMP_DBL_ARRAY(drvt_u);
  CREATE_TEMP_INT_ARRAY(u_offset);
  CREATE_TEMP_DBL_ARRAY(drvt_v);
  CREATE_TEMP_INT_ARRAY(v_offset);
  iGeom_getArr1stDrvt (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                       storage_order,
                       TEMP_ARRAY_IN(uv),
                       TEMP_ARRAY_INOUT(drvt_u),
                       TEMP_ARRAY_INOUT(u_offset),
                       TEMP_ARRAY_INOUT(drvt_v),
                       TEMP_ARRAY_INOUT(v_offset), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(drvt_u);
  ASSIGN_INT_ARRAY(u_offset);
  ASSIGN_DBL_ARRAY(drvt_v);
  ASSIGN_INT_ARRAY(v_offset);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArr1stDrvt) */
#line 5221 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEnt2ndDrvt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEnt2ndDrvt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5252 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEnt2ndDrvt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEnt2ndDrvt} (getEnt2ndDrvt method) */
  CREATE_TEMP_DBL_ARRAY(drvt_uu);
  CREATE_TEMP_DBL_ARRAY(drvt_uv);
  CREATE_TEMP_DBL_ARRAY(drvt_vv);
 
  iGeom_getEnt2ndDrvt (INSTANCE, 
                       (iBase_EntityHandle)entity_handle, u, v,
                       TEMP_ARRAY_INOUT(drvt_uu),
                       TEMP_ARRAY_INOUT(drvt_uv),
                       TEMP_ARRAY_INOUT(drvt_vv), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(drvt_uu);
  ASSIGN_DBL_ARRAY(drvt_uv);
  ASSIGN_DBL_ARRAY(drvt_vv);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEnt2ndDrvt) */
#line 5270 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getArr2ndDrvt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArr2ndDrvt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5309 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArr2ndDrvt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArr2ndDrvt} (getArr2ndDrvt method) */
  CREATE_TEMP_DBL_ARRAY(drvt_uu);
  CREATE_TEMP_INT_ARRAY(uu_offset);
  CREATE_TEMP_DBL_ARRAY(drvt_uv);
  CREATE_TEMP_INT_ARRAY(uv_offset);
  CREATE_TEMP_DBL_ARRAY(drvt_vv);
  CREATE_TEMP_INT_ARRAY(vv_offset);
  iGeom_getArr2ndDrvt (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                       storage_order,
                       TEMP_ARRAY_IN(uv),
                       TEMP_ARRAY_INOUT(drvt_uu),
                       TEMP_ARRAY_INOUT(uu_offset),
                       TEMP_ARRAY_INOUT(drvt_uv),
                       TEMP_ARRAY_INOUT(uv_offset),
                       TEMP_ARRAY_INOUT(drvt_vv),
                       TEMP_ARRAY_INOUT(vv_offset), 
                       &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(drvt_uu);
  ASSIGN_INT_ARRAY(uu_offset);
  ASSIGN_DBL_ARRAY(drvt_uv);
  ASSIGN_INT_ARRAY(uv_offset);
  ASSIGN_DBL_ARRAY(drvt_vv);
  ASSIGN_INT_ARRAY(vv_offset);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArr2ndDrvt) */
#line 5337 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getFcCvtrUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getFcCvtrUV"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5368 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getFcCvtrUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getFcCvtrUV} (getFcCvtrUV method) */
  iGeom_getFcCvtrUV (INSTANCE, (iBase_EntityHandle)entity_handle, 
                     u, v, 
                     cvtr1_i, cvtr1_j, cvtr1_k,
                     cvtr2_i, cvtr2_j, cvtr2_k, 
                     &ERROR );
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getFcCvtrUV) */
#line 5377 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getFcArrCvtrUV[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getFcArrCvtrUV"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5408 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getFcArrCvtrUV) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getFcArrCvtrUV} (getFcArrCvtrUV method) */
  CREATE_TEMP_DBL_ARRAY(cvtr_1);
  CREATE_TEMP_DBL_ARRAY(cvtr_2);
  iGeom_getFcArrCvtrUV (INSTANCE, 
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles),
                         storage_order,
                         TEMP_ARRAY_IN(uv),
                         TEMP_ARRAY_INOUT(cvtr_1),
                         TEMP_ARRAY_INOUT(cvtr_2), 
                         &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(cvtr_1);
  ASSIGN_DBL_ARRAY(cvtr_2);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getFcArrCvtrUV) */
#line 5424 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isEntPeriodic[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isEntPeriodic"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isEntPeriodic(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ int32_t* in_u,
  /* out */ int32_t* in_v,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5449 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isEntPeriodic) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isEntPeriodic} (isEntPeriodic method) */
  iGeom_isEntPeriodic (INSTANCE, 
                       (iBase_EntityHandle)entity_handle,
                       in_u, in_v, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isEntPeriodic) */
#line 5457 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isArrPeriodic[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isArrPeriodic"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isArrPeriodic(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<int> */ struct sidl_int__array** in_uv,
  /* out */ int32_t* in_uv_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5483 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isArrPeriodic) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isArrPeriodic} (isArrPeriodic method) */
  CREATE_TEMP_INT_ARRAY(in_uv);
  iGeom_isArrPeriodic (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                       TEMP_ARRAY_INOUT(in_uv), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(in_uv);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isArrPeriodic) */
#line 5493 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isFcDegenerate[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isFcDegenerate"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isFcDegenerate(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* face_handle,
  /* out */ int32_t* is_degenerate,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5517 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isFcDegenerate) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isFcDegenerate} (isFcDegenerate method) */
  iGeom_isFcDegenerate (INSTANCE, (iBase_EntityHandle)face_handle, is_degenerate, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isFcDegenerate) */
#line 5522 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  isFcArrDegenerate[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_isFcArrDegenerate"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_isFcArrDegenerate(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* face_handles,
  /* in */ int32_t face_handles_size,
  /* inout array<int> */ struct sidl_int__array** degenerate,
  /* out */ int32_t* degenerate_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5548 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.isFcArrDegenerate) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.isFcArrDegenerate} (isFcArrDegenerate method) */
  CREATE_TEMP_INT_ARRAY(degenerate);
  iGeom_isFcArrDegenerate (INSTANCE, 
                       TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,face_handles),
                       TEMP_ARRAY_INOUT(degenerate), 
                       &ERROR);
  PROCESS_ERROR;
  ASSIGN_INT_ARRAY(degenerate);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.isFcArrDegenerate) */
#line 5558 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the relative and absolute tolerances at the modeler level.  If
 * model does not have a modeler-wide tolerance, zero is returned for both
 * values.
 * @param relative_tolerance Relative tolerance for model as a whole
 * @param absolute_tolerance Absolute tolerance for model as a whole
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getTolerance"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getTolerance(
  /* in */ iGeomSIDL_GeomSidl self,
  /* out */ int32_t* type,
  /* out */ double* tolerance,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5586 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getTolerance) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getTolerance} (getTolerance method) */
  iGeom_getTolerance (INSTANCE, 
                       type,
                       tolerance, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getTolerance) */
#line 5594 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getEntTolerance[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getEntTolerance"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getEntTolerance(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_handle,
  /* out */ double* tolerance,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5618 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getEntTolerance) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getEntTolerance} (getEntTolerance method) */
  iGeom_getEntTolerance (INSTANCE, 
                        (iBase_EntityHandle)entity_handle, 
                        tolerance,
                        &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getEntTolerance) */
#line 5626 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Return the relative and absolute tolerances for specified gentities.  If
 * a gentity does not have a specific tolerance, zero is returned for both
 * values.
 * @param gentity_handles Gentities being queried
 * @param relative_tolerances Relative tolerances
 * @param absolute_tolerances Absolute tolerances
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getArrTolerance"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getArrTolerance(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* entity_handles,
  /* in */ int32_t entity_handles_size,
  /* inout array<double> */ struct sidl_double__array** tolerances,
  /* out */ int32_t* tolerances_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5657 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getArrTolerance) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getArrTolerance} (getArrTolerance method) */
  CREATE_TEMP_DBL_ARRAY(tolerances);
  
  iGeom_getArrTolerance (INSTANCE, 
                         TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,entity_handles),
                         TEMP_ARRAY_INOUT(tolerances), 
                         &ERROR);
  PROCESS_ERROR;

  ASSIGN_DBL_ARRAY(tolerances);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getArrTolerance) */
#line 5669 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Initialize an iterator over gentities of a specified dimension.
 * @param gentity_dimension Dimension of gentities to be iterated over
 * @param gentity_iterator Iterator initialized by this function
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_initEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_initEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_handle,
  /* in */ int32_t gentity_dimension,
  /* out */ void** gentity_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5696 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.initEntIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.initEntIter} (initEntIter method) */
  iGeom_EntityIterator iter;
  iGeom_initEntIter (INSTANCE, 
                     (iBase_EntitySetHandle)entity_set_handle,
                     gentity_dimension, 
                     &iter, 
                     &ERROR);
  PROCESS_ERROR;
  *gentity_iterator = iter;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.initEntIter) */
#line 5707 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  initEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_initEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_initEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_set_handle,
  /* in */ enum iBase_EntityType__enum requested_entity_type,
  /* in */ int32_t requested_array_size,
  /* out */ void** entArr_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5733 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.initEntArrIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.initEntArrIter} (initEntArrIter method) */
 iGeom_EntityArrIterator iter;
 iGeom_initEntArrIter (INSTANCE, 
         (iBase_EntitySetHandle)entity_set_handle,
         requested_entity_type,
         requested_array_size,
         &iter, 
         &ERROR);
  PROCESS_ERROR;
  *entArr_iterator = iter;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.initEntArrIter) */
#line 5745 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Get the next entity for this iterator.
 * @param gentity_iterator Iterator being iterated over
 * @param gentity_handle Next gentity
 * @return If true, there are more gentities, if false, this is the last one
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getNextEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getNextEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entity_iterator,
  /* out */ void** entity_handle,
  /* out */ int32_t* has_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5773 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getNextEntIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getNextEntIter} (getNextEntIter method) */
  iBase_EntityHandle result;
  iGeom_getNextEntIter (INSTANCE, 
                        (iGeom_EntityIterator)entity_iterator,
                        &result, 
                        has_data,
                        &ERROR);
  PROCESS_ERROR;
  *entity_handle = result;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getNextEntIter) */
#line 5784 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  getNextEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_getNextEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_getNextEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entArr_iterator,
  /* inout array<opaque> */ struct sidl_opaque__array** entity_handles,
  /* out */ int32_t* entity_handles_size,
  /* out */ int32_t* has_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5810 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.getNextEntArrIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.getNextEntArrIter} (getNextEntArrIter method) */
  CREATE_TEMP_EH_ARRAY(entity_handles);
  iGeom_getNextEntArrIter (INSTANCE, 
                           (iGeom_EntityArrIterator)entArr_iterator,
                           TEMP_ARRAY_INOUT(entity_handles), 
                           has_data,
                           &ERROR);
  PROCESS_ERROR;
  ASSIGN_OPAQUE_ARRAY(entity_handles);
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.getNextEntArrIter) */
#line 5821 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Reset an iterator back to the first gentity
 * @param gentity_iterator Iterator reset by this function
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_resetEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_resetEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* gentity_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5845 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.resetEntIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.resetEntIter} (resetEntIter method) */
  iGeom_resetEntIter (INSTANCE, 
                     (iGeom_EntityIterator)gentity_iterator, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.resetEntIter) */
#line 5852 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  resetEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_resetEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_resetEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5875 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.resetEntArrIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.resetEntArrIter} (resetEntArrIter method) */
  iGeom_resetEntArrIter (INSTANCE, 
                         (iGeom_EntityArrIterator)entArr_iterator, 
                         &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.resetEntArrIter) */
#line 5882 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Delete an iterator
 * @param gentity_iterator Iterator deleted by this function
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_endEntIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_endEntIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* Gentity_dim_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5906 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.endEntIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.endEntIter} (endEntIter method) */
  iGeom_endEntIter (INSTANCE, 
                    (iGeom_EntityIterator)Gentity_dim_iterator, 
                    &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.endEntIter) */
#line 5913 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  endEntArrIter[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_endEntArrIter"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_endEntArrIter(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* entArr_iterator,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5936 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.endEntArrIter) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.endEntArrIter} (endEntArrIter method) */
  iGeom_endEntArrIter (INSTANCE, 
                       (iGeom_EntityArrIterator)entArr_iterator, 
                       &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.endEntArrIter) */
#line 5943 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  Copy[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_Copy"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_Copy(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* out */ void** geom_entity2,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 5967 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.Copy) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.Copy} (Copy method) */
  iGeom_copyEnt(INSTANCE, 
                (iBase_EntityHandle)geom_entity, 
                (iBase_EntityHandle*)geom_entity2, 
                &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.Copy) */
#line 5975 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  SweepAboutAxis[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_SweepAboutAxis"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_SweepAboutAxis(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double angle,
  /* in */ double axis_normal_x,
  /* in */ double axis_normal_y,
  /* in */ double axis_normal_z,
  /* out */ void** geom_entity2,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6003 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.SweepAboutAxis) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.SweepAboutAxis} (SweepAboutAxis method) */
  iGeom_sweepEntAboutAxis( INSTANCE, 
                        (iBase_EntityHandle)geom_entity, 
                        angle,
                        axis_normal_x, axis_normal_y, axis_normal_z, 
                        (iBase_EntityHandle*)geom_entity2, 
                        &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.SweepAboutAxis) */
#line 6013 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  Delete[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_Delete"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_Delete(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6036 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.Delete) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.Delete} (Delete method) */
  iGeom_deleteEnt(INSTANCE, (iBase_EntityHandle)geom_entity, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.Delete) */
#line 6041 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  createSphere[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_createSphere"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_createSphere(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double radius,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6065 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.createSphere) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.createSphere} (createSphere method) */
  iGeom_createSphere( INSTANCE, 
                      radius, 
                      (iBase_EntityHandle*)geom_entity, 
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.createSphere) */
#line 6073 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  createBrick[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_createBrick"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_createBrick(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6099 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.createBrick) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.createBrick} (createBrick method) */
  iGeom_createBrick( INSTANCE, 
                     x, y, z, 
                     (iBase_EntityHandle*)geom_entity, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.createBrick) */
#line 6107 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  createCylinder[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_createCylinder"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_createCylinder(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double height,
  /* in */ double major_rad,
  /* in */ double minor_rad,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6133 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.createCylinder) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.createCylinder} (createCylinder method) */
  iGeom_createCylinder( INSTANCE, 
                        height, major_rad, minor_rad, 
                        (iBase_EntityHandle*)geom_entity, 
                        &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.createCylinder) */
#line 6141 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  createTorus[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_createTorus"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_createTorus(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ double major_rad,
  /* in */ double minor_rad,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6166 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.createTorus) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.createTorus} (createTorus method) */
  iGeom_createTorus( INSTANCE, 
                     major_rad, minor_rad, 
                     (iBase_EntityHandle*)geom_entity, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.createTorus) */
#line 6174 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  moveEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_moveEnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_moveEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double x,
  /* in */ double y,
  /* in */ double z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6200 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.moveEnt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.moveEnt} (moveEnt method) */
  iGeom_moveEnt(INSTANCE, (iBase_EntityHandle)geom_entity, x, y, z, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.moveEnt) */
#line 6205 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  rotateEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_rotateEnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_rotateEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double angle,
  /* in */ double axis_normal_x,
  /* in */ double axis_normal_y,
  /* in */ double axis_normal_z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6232 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.rotateEnt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.rotateEnt} (rotateEnt method) */
  iGeom_rotateEnt(INSTANCE, (iBase_EntityHandle)geom_entity, 
                  angle, axis_normal_x, axis_normal_y, axis_normal_z, 
                  &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.rotateEnt) */
#line 6239 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  reflectEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_reflectEnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_reflectEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double plane_normal_x,
  /* in */ double plane_normal_y,
  /* in */ double plane_normal_z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6265 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.reflectEnt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.reflectEnt} (reflectEnt method) */
  iGeom_reflectEnt(INSTANCE, (iBase_EntityHandle)geom_entity, 
                   plane_normal_x, plane_normal_y, plane_normal_z, 
                   &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.reflectEnt) */
#line 6272 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  scaleEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_scaleEnt"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_scaleEnt(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* geom_entity,
  /* in */ double scale_x,
  /* in */ double scale_y,
  /* in */ double scale_z,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6298 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.scaleEnt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.scaleEnt} (scaleEnt method) */
  iGeom_scaleEnt(INSTANCE, (iBase_EntityHandle)geom_entity, 
                 scale_x, scale_y, scale_z, 
                 &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.scaleEnt) */
#line 6305 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  uniteEnts[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_uniteEnts"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_uniteEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* geom_entities,
  /* in */ int32_t geom_entities_size,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6330 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.uniteEnts) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.uniteEnts} (uniteEnts method) */
  iGeom_uniteEnts(INSTANCE, 
                  TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, geom_entities), 
                  (iBase_EntityHandle*)geom_entity, 
                  &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.uniteEnts) */
#line 6338 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  subtractEnts[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_subtractEnts"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_subtractEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in */ void* blank,
  /* in */ void* tool,
  /* out */ void** geom_entity,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6363 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.subtractEnts) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.subtractEnts} (subtractEnts method) */
  iGeom_subtractEnts( INSTANCE, 
                      (iBase_EntityHandle)blank, 
                      (iBase_EntityHandle)tool, 
                      (iBase_EntityHandle*)geom_entity, 
                      &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.subtractEnts) */
#line 6372 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  sectionEnt[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_sectionEnt"

#ifdef __cplusplus
extern "C"
#endif
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
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6401 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.sectionEnt) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.sectionEnt} (sectionEnt method) */
  iGeom_sectionEnt( INSTANCE, 
                     (iBase_EntityHandle)geom_entity, 
                     plane_normal_x, plane_normal_y, plane_normal_z, 
                     offset, reverse, 
                     (iBase_EntityHandle*)geom_entity2, 
                     &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.sectionEnt) */
#line 6411 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  imprintEnts[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_imprintEnts"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_imprintEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* geom_entities,
  /* in */ int32_t geom_entities_size,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6435 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.imprintEnts) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.imprintEnts} (imprintEnts method) */
  iGeom_imprintEnts(INSTANCE,
                    TEMP_TYPED_ARRAY_IN(iBase_EntityHandle, geom_entities), 
                    &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.imprintEnts) */
#line 6442 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}

/*
 * Method:  mergeEnts[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iGeomSIDL_GeomSidl_mergeEnts"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iGeomSIDL_GeomSidl_mergeEnts(
  /* in */ iGeomSIDL_GeomSidl self,
  /* in array<opaque> */ struct sidl_opaque__array* geom_entities,
  /* in */ int32_t geom_entities_size,
  /* in */ double tolerance,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 6467 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iGeomSIDL.GeomSidl.mergeEnts) */
  /* Insert-Code-Here {iGeomSIDL.GeomSidl.mergeEnts} (mergeEnts method) */
  iGeom_mergeEnts(INSTANCE,
               TEMP_TYPED_ARRAY_IN(iBase_EntityHandle,geom_entities),
               tolerance, &ERROR);
  PROCESS_ERROR;
    /* DO-NOT-DELETE splicer.end(iGeomSIDL.GeomSidl.mergeEnts) */
#line 6474 "server/iGeomSIDL_GeomSidl_Impl.c"
  }
}
/* Babel internal methods, Users should not edit below this line. */

#line 6480 ".././iGeom/server/iGeomSIDL_GeomSidl_Impl.c"
/* DO-NOT-DELETE splicer.begin(_misc) */
/* insert code here (miscellaneous code) */
/* DO-NOT-DELETE splicer.end(_misc) */
#line 6483 "server/iGeomSIDL_GeomSidl_Impl.c"

