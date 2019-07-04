/*
 * File:          TSTTM_Factory_Impl.h
 * Symbol:        TSTTM.Factory-v0.7
 * Symbol Type:   class
 * Babel Version: 0.10.10
 * Description:   Server-side implementation for TSTTM.Factory
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 * babel-version = 0.10.10
 */

#ifndef included_TSTTM_Factory_Impl_h
#define included_TSTTM_Factory_Impl_h

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_TSTTB_Error_h
#include "TSTTB_Error.h"
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
#ifndef included_TSTTM_Factory_h
#include "TSTTM_Factory.h"
#endif
#ifndef included_sidl_BaseClass_h
#include "sidl_BaseClass.h"
#endif

/* DO-NOT-DELETE splicer.begin(TSTTM.Factory._includes) */
#include "SimpleMesh.h"
/* DO-NOT-DELETE splicer.end(TSTTM.Factory._includes) */

/*
 * Private data for class TSTTM.Factory
 */

struct TSTTM_Factory__data {
  /* DO-NOT-DELETE splicer.begin(TSTTM.Factory._data) */
  /* Insert-Code-Here {TSTTM.Factory._data} (private data members) */
  int ignore; /* dummy to force non-empty struct; remove if you add data */
  /* DO-NOT-DELETE splicer.end(TSTTM.Factory._data) */
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct TSTTM_Factory__data*
TSTTM_Factory__get_data(
  TSTTM_Factory);

extern void
TSTTM_Factory__set_data(
  TSTTM_Factory,
  struct TSTTM_Factory__data*);

extern
void

impl_TSTTM_Factory__load(
  void);

extern
void
impl_TSTTM_Factory__ctor(
  /* in */ TSTTM_Factory self);

extern
void
impl_TSTTM_Factory__dtor(
  /* in */ TSTTM_Factory self);

/*
 * User-defined object methods
 */

extern
TSTTM_Mesh
impl_TSTTM_Factory_newMesh(
  /* in */ const char* option,
  /* out */ sidl_BaseInterface *_ex);

extern struct TSTTB_Error__object* 
  impl_TSTTM_Factory_fconnect_TSTTB_Error(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_TSTTB_Error(struct TSTTB_Error__object* 
  obj);
extern struct sidl_ClassInfo__object* 
  impl_TSTTM_Factory_fconnect_sidl_ClassInfo(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj);
extern struct TSTTM_Mesh__object* impl_TSTTM_Factory_fconnect_TSTTM_Mesh(char* 
  url, sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_TSTTM_Mesh(struct TSTTM_Mesh__object* 
  obj);
extern struct sidl_BaseInterface__object* 
  impl_TSTTM_Factory_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj);
extern struct TSTTM_Factory__object* 
  impl_TSTTM_Factory_fconnect_TSTTM_Factory(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_TSTTM_Factory(struct 
  TSTTM_Factory__object* obj);
extern struct sidl_BaseClass__object* 
  impl_TSTTM_Factory_fconnect_sidl_BaseClass(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj);
extern struct TSTTB_Error__object* 
  impl_TSTTM_Factory_fconnect_TSTTB_Error(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_TSTTB_Error(struct TSTTB_Error__object* 
  obj);
extern struct sidl_ClassInfo__object* 
  impl_TSTTM_Factory_fconnect_sidl_ClassInfo(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_sidl_ClassInfo(struct 
  sidl_ClassInfo__object* obj);
extern struct TSTTM_Mesh__object* impl_TSTTM_Factory_fconnect_TSTTM_Mesh(char* 
  url, sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_TSTTM_Mesh(struct TSTTM_Mesh__object* 
  obj);
extern struct sidl_BaseInterface__object* 
  impl_TSTTM_Factory_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj);
extern struct TSTTM_Factory__object* 
  impl_TSTTM_Factory_fconnect_TSTTM_Factory(char* url, sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_TSTTM_Factory(struct 
  TSTTM_Factory__object* obj);
extern struct sidl_BaseClass__object* 
  impl_TSTTM_Factory_fconnect_sidl_BaseClass(char* url,
  sidl_BaseInterface *_ex);
extern char* impl_TSTTM_Factory_fgetURL_sidl_BaseClass(struct 
  sidl_BaseClass__object* obj);
#ifdef __cplusplus
}
#endif
#endif
