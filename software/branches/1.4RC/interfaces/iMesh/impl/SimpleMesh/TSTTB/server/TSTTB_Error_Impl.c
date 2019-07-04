/*
 * File:          TSTTB_Error_Impl.c
 * Symbol:        TSTTB.Error-v0.7
 * Symbol Type:   class
 * Babel Version: 0.10.10
 * Description:   Server-side implementation for TSTTB.Error
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
 * Symbol "TSTTB.Error" (version 0.7)
 */

#include "TSTTB_Error_Impl.h"

/* DO-NOT-DELETE splicer.begin(TSTTB.Error._includes) */
#include "sidl_String.h"
/* DO-NOT-DELETE splicer.end(TSTTB.Error._includes) */

/*
 * Static class initializer called exactly once before any user-defined method is dispatched
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error__load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTB_Error__load(
  void)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error._load) */
  /* Insert-Code-Here {TSTTB.Error._load} (static class initializer method) */
  /* DO-NOT-DELETE splicer.end(TSTTB.Error._load) */
}
/*
 * Class constructor called when the class is created.
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error__ctor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTB_Error__ctor(
  /* in */ TSTTB_Error self)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error._ctor) */
  struct TSTTB_Error__data *dataPtr = malloc(sizeof(struct TSTTB_Error__data));
  if (dataPtr) {
    dataPtr->errorType = TSTTB_ErrorType_SUCCESS;
    dataPtr->errorDescription = NULL;
  }
  TSTTB_Error__set_data(self, dataPtr);
  /* DO-NOT-DELETE splicer.end(TSTTB.Error._ctor) */
}

/*
 * Class destructor called when the class is deleted.
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error__dtor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTB_Error__dtor(
  /* in */ TSTTB_Error self)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error._dtor) */
  struct TSTTB_Error__data *dataPtr = TSTTB_Error__get_data(self);
  if (dataPtr) {
    free((void*)dataPtr->errorDescription);
    free((void*)dataPtr);
  }
  TSTTB_Error__set_data(self,NULL);
  /* DO-NOT-DELETE splicer.end(TSTTB.Error._dtor) */
}

/*
 * Method:  set[]
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error_set"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTB_Error_set(
  /* in */ TSTTB_Error self,
  /* in */ enum TSTTB_ErrorType__enum error,
  /* in */ const char* description)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error.set) */
  struct TSTTB_Error__data *dataPtr = TSTTB_Error__get_data(self);
  if (dataPtr) {
    dataPtr->errorType = error;
    if (description != NULL) {
      dataPtr->errorDescription = sidl_String_strdup(description);
    } else {
      dataPtr->errorDescription = NULL;
    }
  }
  TSTTB_Error__set_data(self, dataPtr);
  /* DO-NOT-DELETE splicer.end(TSTTB.Error.set) */
}

/*
 * Method:  getErrorType[]
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error_getErrorType"

#ifdef __cplusplus
extern "C"
#endif
enum TSTTB_ErrorType__enum
impl_TSTTB_Error_getErrorType(
  /* in */ TSTTB_Error self)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error.getErrorType) */
  struct TSTTB_Error__data *dataPtr = TSTTB_Error__get_data(self);
  return(dataPtr->errorType);
  /* DO-NOT-DELETE splicer.end(TSTTB.Error.getErrorType) */
}

/*
 * Method:  get[]
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error_get"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTB_Error_get(
  /* in */ TSTTB_Error self,
  /* out */ enum TSTTB_ErrorType__enum* err,
  /* out */ char** description)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error.get) */
  struct TSTTB_Error__data *dataPtr = TSTTB_Error__get_data(self);
  *err = dataPtr->errorType;
  (*description) = sidl_String_strdup(dataPtr->errorDescription);
  /* DO-NOT-DELETE splicer.end(TSTTB.Error.get) */
}

/*
 * Method:  getDescription[]
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error_getDescription"

#ifdef __cplusplus
extern "C"
#endif
char*
impl_TSTTB_Error_getDescription(
  /* in */ TSTTB_Error self)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error.getDescription) */
  struct TSTTB_Error__data *dataPtr = TSTTB_Error__get_data(self);
  return(sidl_String_strdup(dataPtr->errorDescription));
  /* DO-NOT-DELETE splicer.end(TSTTB.Error.getDescription) */
}

/*
 * Method:  echo[]
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTB_Error_echo"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTB_Error_echo(
  /* in */ TSTTB_Error self,
  /* in */ const char* label)
{
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error.echo) */
  /* Insert-Code-Here {TSTTB.Error.echo} (echo method) */
  /* DO-NOT-DELETE splicer.end(TSTTB.Error.echo) */
}
/* Babel internal methods, Users should not edit below this line. */
struct sidl_SIDLException__object* 
  impl_TSTTB_Error_fconnect_sidl_SIDLException(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_SIDLException__connect(url, _ex);
}
char * impl_TSTTB_Error_fgetURL_sidl_SIDLException(struct 
  sidl_SIDLException__object* obj) {
  return sidl_SIDLException__getURL(obj);
}
struct TSTTB_Error__object* impl_TSTTB_Error_fconnect_TSTTB_Error(char* url,
  sidl_BaseInterface *_ex) {
  return TSTTB_Error__connect(url, _ex);
}
char * impl_TSTTB_Error_fgetURL_TSTTB_Error(struct TSTTB_Error__object* obj) {
  return TSTTB_Error__getURL(obj);
}
struct sidl_ClassInfo__object* impl_TSTTB_Error_fconnect_sidl_ClassInfo(char* 
  url, sidl_BaseInterface *_ex) {
  return sidl_ClassInfo__connect(url, _ex);
}
char * impl_TSTTB_Error_fgetURL_sidl_ClassInfo(struct sidl_ClassInfo__object* 
  obj) {
  return sidl_ClassInfo__getURL(obj);
}
struct sidl_BaseInterface__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_BaseInterface__connect(url, _ex);
}
char * impl_TSTTB_Error_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj) {
  return sidl_BaseInterface__getURL(obj);
}
struct sidl_BaseException__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseException(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_BaseException__connect(url, _ex);
}
char * impl_TSTTB_Error_fgetURL_sidl_BaseException(struct 
  sidl_BaseException__object* obj) {
  return sidl_BaseException__getURL(obj);
}
struct sidl_BaseClass__object* impl_TSTTB_Error_fconnect_sidl_BaseClass(char* 
  url, sidl_BaseInterface *_ex) {
  return sidl_BaseClass__connect(url, _ex);
}
char * impl_TSTTB_Error_fgetURL_sidl_BaseClass(struct sidl_BaseClass__object* 
  obj) {
  return sidl_BaseClass__getURL(obj);
}
