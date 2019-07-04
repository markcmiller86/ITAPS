/*
 * File:          TSTTB_Error_Impl.h
 * Symbol:        TSTTB.Error-v0.7
 * Symbol Type:   class
 * Babel Version: 0.10.10
 * Description:   Server-side implementation for TSTTB.Error
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 * babel-version = 0.10.10
 */

#ifndef included_TSTTB_Error_Impl_h
#define included_TSTTB_Error_Impl_h

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_sidl_SIDLException_h
#include "sidl_SIDLException.h"
#endif
#ifndef included_TSTTB_Error_h
#include "TSTTB_Error.h"
#endif
#ifndef included_sidl_ClassInfo_h
#include "sidl_ClassInfo.h"
#endif
#ifndef included_sidl_BaseInterface_h
#include "sidl_BaseInterface.h"
#endif
#ifndef included_sidl_BaseException_h
#include "sidl_BaseException.h"
#endif
#ifndef included_sidl_BaseClass_h
#include "sidl_BaseClass.h"
#endif

/* DO-NOT-DELETE splicer.begin(TSTTB.Error._includes) */
/* Insert-Code-Here {TSTTB.Error._includes} (include files) */
/* DO-NOT-DELETE splicer.end(TSTTB.Error._includes) */

/*
 * Private data for class TSTTB.Error
 */

struct TSTTB_Error__data {
  /* DO-NOT-DELETE splicer.begin(TSTTB.Error._data) */
  enum TSTTB_ErrorType__enum errorType;
  char *errorDescription;
  /* DO-NOT-DELETE splicer.end(TSTTB.Error._data) */
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct TSTTB_Error__data*
TSTTB_Error__get_data(
  TSTTB_Error);

extern void
TSTTB_Error__set_data(
  TSTTB_Error,
  struct TSTTB_Error__data*);

extern
void
impl_TSTTB_Error__load(
  void);

extern
void
impl_TSTTB_Error__ctor(
  /* in */ TSTTB_Error self);

extern
void
impl_TSTTB_Error__dtor(
  /* in */ TSTTB_Error self);

/*
 * User-defined object methods
 */

extern struct sidl_SIDLException__object* 
  impl_TSTTB_Error_fconnect_sidl_SIDLException(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_SIDLException(struct 
  sidl_SIDLException__object* obj);
extern struct TSTTB_Error__object* impl_TSTTB_Error_fconnect_TSTTB_Error(char* 
  url, sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_TSTTB_Error(struct TSTTB_Error__object* 
  obj);
extern struct sidl_ClassInfo__object* 
  impl_TSTTB_Error_fconnect_sidl_ClassInfo(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj);
extern struct sidl_BaseInterface__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj);
extern struct sidl_BaseException__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseException(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_BaseException(struct 
  sidl_BaseException__object* obj);
extern struct sidl_BaseClass__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseClass(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj);
extern
void
impl_TSTTB_Error_set(
  /* in */ TSTTB_Error self,
  /* in */ enum TSTTB_ErrorType__enum error,
  /* in */ const char* description);

extern
enum TSTTB_ErrorType__enum
impl_TSTTB_Error_getErrorType(
  /* in */ TSTTB_Error self);

extern
void
impl_TSTTB_Error_get(
  /* in */ TSTTB_Error self,
  /* out */ enum TSTTB_ErrorType__enum* err,
  /* out */ char** description);

extern
char*
impl_TSTTB_Error_getDescription(
  /* in */ TSTTB_Error self);

extern
void
impl_TSTTB_Error_echo(
  /* in */ TSTTB_Error self,
  /* in */ const char* label);

extern struct sidl_SIDLException__object* 
  impl_TSTTB_Error_fconnect_sidl_SIDLException(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_SIDLException(struct 
  sidl_SIDLException__object* obj);
extern struct TSTTB_Error__object* impl_TSTTB_Error_fconnect_TSTTB_Error(char* 
  url, sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_TSTTB_Error(struct TSTTB_Error__object* 
  obj);
extern struct sidl_ClassInfo__object* 
  impl_TSTTB_Error_fconnect_sidl_ClassInfo(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj);
extern struct sidl_BaseInterface__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj);
extern struct sidl_BaseException__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseException(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_BaseException(struct 
  sidl_BaseException__object* obj);
extern struct sidl_BaseClass__object* 
  impl_TSTTB_Error_fconnect_sidl_BaseClass(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTB_Error_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj);
#ifdef __cplusplus
}
#endif
#endif
