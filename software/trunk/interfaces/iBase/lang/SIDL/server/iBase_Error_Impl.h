/*
 * File:          iBase_Error_Impl.h
 * Symbol:        iBase.Error-v0.8
 * Symbol Type:   class
 * Babel Version: 1.4.0 (Revision: 6607 release-1-4-0-branch)
 * Description:   Server-side implementation for iBase.Error
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

#ifndef included_iBase_Error_Impl_h
#define included_iBase_Error_Impl_h

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_iBase_Error_h
#include "iBase_Error.h"
#endif
#ifndef included_sidl_BaseClass_h
#include "sidl_BaseClass.h"
#endif
#ifndef included_sidl_BaseException_h
#include "sidl_BaseException.h"
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
#ifndef included_sidl_SIDLException_h
#include "sidl_SIDLException.h"
#endif
#ifndef included_sidl_io_Deserializer_h
#include "sidl_io_Deserializer.h"
#endif
#ifndef included_sidl_io_Serializable_h
#include "sidl_io_Serializable.h"
#endif
#ifndef included_sidl_io_Serializer_h
#include "sidl_io_Serializer.h"
#endif
#line 49 ".././iBase/server/iBase_Error_Impl.h"
/* DO-NOT-DELETE splicer.begin(iBase.Error._hincludes) */
/* insert code here (include files) */
/* DO-NOT-DELETE splicer.end(iBase.Error._hincludes) */
#line 52 "server/iBase_Error_Impl.h"

/*
 * Private data for class iBase.Error
 */

struct iBase_Error__data {
#line 60 ".././iBase/server/iBase_Error_Impl.h"
  /* DO-NOT-DELETE splicer.begin(iBase.Error._data) */
  /* Insert-Code-Here {iBase.Error._data} (private data members) */
  enum iBase_ErrorType__enum mCode;
  char* mDesc;
  /* DO-NOT-DELETE splicer.end(iBase.Error._data) */
#line 65 "server/iBase_Error_Impl.h"
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct iBase_Error__data*
iBase_Error__get_data(
  iBase_Error);

extern void
iBase_Error__set_data(
  iBase_Error,
  struct iBase_Error__data*);

extern
void
impl_iBase_Error__load(
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error__ctor(
  /* in */ iBase_Error self,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error__ctor2(
  /* in */ iBase_Error self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error__dtor(
  /* in */ iBase_Error self,
  /* out */ sidl_BaseInterface *_ex);

/*
 * User-defined object methods
 */

#ifdef WITH_RMI
extern struct sidl_io_Deserializer__object* 
  impl_iBase_Error_fconnect_sidl_io_Deserializer(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_BaseInterface__object* 
  impl_iBase_Error_fconnect_sidl_BaseInterface(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_io_Serializer__object* 
  impl_iBase_Error_fconnect_sidl_io_Serializer(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
#endif /*WITH_RMI*/
extern
void
impl_iBase_Error_set(
  /* in */ iBase_Error self,
  /* in */ enum iBase_ErrorType__enum error,
  /* in */ const char* description,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error_getErrorType(
  /* in */ iBase_Error self,
  /* out */ enum iBase_ErrorType__enum* err_type,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error_get(
  /* in */ iBase_Error self,
  /* out */ enum iBase_ErrorType__enum* err,
  /* out */ char** description,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error_getDescription(
  /* in */ iBase_Error self,
  /* out */ char** description,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_iBase_Error_echo(
  /* in */ iBase_Error self,
  /* in */ const char* label,
  /* out */ sidl_BaseInterface *_ex);

#ifdef WITH_RMI
extern struct sidl_io_Deserializer__object* 
  impl_iBase_Error_fconnect_sidl_io_Deserializer(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_BaseInterface__object* 
  impl_iBase_Error_fconnect_sidl_BaseInterface(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_io_Serializer__object* 
  impl_iBase_Error_fconnect_sidl_io_Serializer(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
#endif /*WITH_RMI*/

#line 174 ".././iBase/server/iBase_Error_Impl.h"
/* DO-NOT-DELETE splicer.begin(_hmisc) */
/* insert code here (miscellaneous things) */
/* DO-NOT-DELETE splicer.end(_hmisc) */
#line 177 "server/iBase_Error_Impl.h"

#ifdef __cplusplus
}
#endif
#endif
