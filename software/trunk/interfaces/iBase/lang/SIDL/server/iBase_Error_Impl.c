/*
 * File:          iBase_Error_Impl.c
 * Symbol:        iBase.Error-v0.8
 * Symbol Type:   class
 * Babel Version: 1.4.0 (Revision: 6607 release-1-4-0-branch)
 * Description:   Server-side implementation for iBase.Error
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

/*
 * DEVELOPERS ARE EXPECTED TO PROVIDE IMPLEMENTATIONS
 * FOR THE FOLLOWING METHODS BETWEEN SPLICER PAIRS.
 */

/*
 * Symbol "iBase.Error" (version 0.8)
 */

#include "iBase_Error_Impl.h"
#include "sidl_NotImplementedException.h"
#include "sidl_Exception.h"
#ifndef included_sidl_MemAllocException_h
#include "sidl_MemAllocException.h"
#endif

#line 29 ".././iBase/server/iBase_Error_Impl.c"
/* DO-NOT-DELETE splicer.begin(iBase.Error._includes) */
/* Insert-Code-Here {iBase.Error._includes} (includes and arbitrary code) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* DO-NOT-DELETE splicer.end(iBase.Error._includes) */
#line 35 "server/iBase_Error_Impl.c"

#define SIDL_IOR_MAJOR_VERSION 2
#define SIDL_IOR_MINOR_VERSION 0
/*
 * Static class initializer called exactly once before any user-defined method is dispatched
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error__load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error__load(
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 56 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error._load) */
  /* Insert-Code-Here {iBase.Error._load} (static class initializer method) */
    /* DO-NOT-DELETE splicer.end(iBase.Error._load) */
#line 59 "server/iBase_Error_Impl.c"
  }
}
/*
 * Class constructor called when the class is created.
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error__ctor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error__ctor(
  /* in */ iBase_Error self,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 80 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error._ctor) */
  /* Insert-Code-Here {iBase.Error._ctor} (constructor method) */
  struct iBase_Error__data* data = malloc( sizeof(struct iBase_Error__data) );
  data->mCode = iBase_ErrorType_SUCCESS;
  data->mDesc = NULL;
  iBase_Error__set_data( self, data );
    /* DO-NOT-DELETE splicer.end(iBase.Error._ctor) */
#line 87 "server/iBase_Error_Impl.c"
  }
}

/*
 * Special Class constructor called when the user wants to wrap his own private data.
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error__ctor2"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error__ctor2(
  /* in */ iBase_Error self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 110 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error._ctor2) */
    /* insert code here (special constructor) */
    /* DO-NOT-DELETE splicer.end(iBase.Error._ctor2) */
#line 113 "server/iBase_Error_Impl.c"
  }
}
/*
 * Class destructor called when the class is deleted.
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error__dtor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error__dtor(
  /* in */ iBase_Error self,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 134 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error._dtor) */
  /* Insert-Code-Here {iBase.Error._dtor} (destructor method) */
  struct iBase_Error__data* data = iBase_Error__get_data( self );
  if (data) {
    free(data->mDesc);
    free(data);
  }
    /* DO-NOT-DELETE splicer.end(iBase.Error._dtor) */
#line 142 "server/iBase_Error_Impl.c"
  }
}

/*
 * Method:  set[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error_set"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error_set(
  /* in */ iBase_Error self,
  /* in */ enum iBase_ErrorType__enum error,
  /* in */ const char* description,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 166 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error.set) */
  /* Insert-Code-Here {iBase.Error.set} (set method) */
  struct iBase_Error__data* data = iBase_Error__get_data( self );
  data->mCode = error;
  free(data->mDesc);
  data->mDesc = strdup( description );
    /* DO-NOT-DELETE splicer.end(iBase.Error.set) */
#line 173 "server/iBase_Error_Impl.c"
  }
}

/*
 * Method:  getErrorType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error_getErrorType"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error_getErrorType(
  /* in */ iBase_Error self,
  /* out */ enum iBase_ErrorType__enum* err_type,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 196 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error.getErrorType) */
  /* Insert-Code-Here {iBase.Error.getErrorType} (getErrorType method) */
  struct iBase_Error__data* data = iBase_Error__get_data( self );
  *err_type = data->mCode;
    /* DO-NOT-DELETE splicer.end(iBase.Error.getErrorType) */
#line 201 "server/iBase_Error_Impl.c"
  }
}

/*
 * Method:  get[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error_get"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error_get(
  /* in */ iBase_Error self,
  /* out */ enum iBase_ErrorType__enum* err,
  /* out */ char** description,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 225 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error.get) */
  /* Insert-Code-Here {iBase.Error.get} (get method) */
  struct iBase_Error__data* data = iBase_Error__get_data( self );
  *err = data->mCode;
  *description = strdup( data->mDesc );
    /* DO-NOT-DELETE splicer.end(iBase.Error.get) */
#line 231 "server/iBase_Error_Impl.c"
  }
}

/*
 * Method:  getDescription[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error_getDescription"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error_getDescription(
  /* in */ iBase_Error self,
  /* out */ char** description,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 254 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error.getDescription) */
  /* Insert-Code-Here {iBase.Error.getDescription} (getDescription method) */
  struct iBase_Error__data* data = iBase_Error__get_data( self );
  *description = strdup( data->mDesc );
    /* DO-NOT-DELETE splicer.end(iBase.Error.getDescription) */
#line 259 "server/iBase_Error_Impl.c"
  }
}

/*
 * Method:  echo[]
 */

#undef __FUNC__
#define __FUNC__ "impl_iBase_Error_echo"

#ifdef __cplusplus
extern "C"
#endif
void
impl_iBase_Error_echo(
  /* in */ iBase_Error self,
  /* in */ const char* label,
  /* out */ sidl_BaseInterface *_ex)
{
  *_ex = 0;
  {
#line 282 ".././iBase/server/iBase_Error_Impl.c"
    /* DO-NOT-DELETE splicer.begin(iBase.Error.echo) */
  /* Insert-Code-Here {iBase.Error.echo} (echo method) */
  struct iBase_Error__data* data = iBase_Error__get_data( self );
  if (data->mCode != iBase_ErrorType_SUCCESS) 
    fprintf(stderr,"Error %d: %s\n", (int)data->mCode, data->mDesc ? data->mDesc : "");
    /* DO-NOT-DELETE splicer.end(iBase.Error.echo) */
#line 288 "server/iBase_Error_Impl.c"
  }
}
/* Babel internal methods, Users should not edit below this line. */

#line 294 ".././iBase/server/iBase_Error_Impl.c"
/* DO-NOT-DELETE splicer.begin(_misc) */
/* insert code here (miscellaneous code) */
/* DO-NOT-DELETE splicer.end(_misc) */
#line 297 "server/iBase_Error_Impl.c"

