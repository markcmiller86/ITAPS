/*
 * File:          TSTTM_Factory_Impl.c
 * Symbol:        TSTTM.Factory-v0.7
 * Symbol Type:   class
 * Babel Version: 0.10.10
 * Description:   Server-side implementation for TSTTM.Factory
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
 * Symbol "TSTTM.Factory" (version 0.7)
 */

#include "TSTTM_Factory_Impl.h"

/* DO-NOT-DELETE splicer.begin(TSTTM.Factory._includes) */
#include "SimpleMesh.h"
/* DO-NOT-DELETE splicer.end(TSTTM.Factory._includes) */

/*
 * Static class initializer called exactly once before any user-defined method is dispatched
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTM_Factory__load"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTM_Factory__load(
  void)
{
  /* DO-NOT-DELETE splicer.begin(TSTTM.Factory._load) */
  /* Insert-Code-Here {TSTTM.Factory._load} (static class initializer method) */
  /* DO-NOT-DELETE splicer.end(TSTTM.Factory._load) */
}
/*
 * Class constructor called when the class is created.
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTM_Factory__ctor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTM_Factory__ctor(
  /* in */ TSTTM_Factory self)
{
  /* DO-NOT-DELETE splicer.begin(TSTTM.Factory._ctor) */
  /* Insert-Code-Here {TSTTM.Factory._ctor} (constructor method) */
  /* DO-NOT-DELETE splicer.end(TSTTM.Factory._ctor) */
}

/*
 * Class destructor called when the class is deleted.
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTM_Factory__dtor"

#ifdef __cplusplus
extern "C"
#endif
void
impl_TSTTM_Factory__dtor(
  /* in */ TSTTM_Factory self)
{
  /* DO-NOT-DELETE splicer.begin(TSTTM.Factory._dtor) */
  /* Insert-Code-Here {TSTTM.Factory._dtor} (destructor method) */
  /* DO-NOT-DELETE splicer.end(TSTTM.Factory._dtor) */
}

/*
 * Method:  newMesh[]
 */

#undef __FUNC__
#define __FUNC__ "impl_TSTTM_Factory_newMesh"

#ifdef __cplusplus
extern "C"
#endif
TSTTM_Mesh
impl_TSTTM_Factory_newMesh(
  /* in */ const char* option,
  /* out */ sidl_BaseInterface *_ex)
{
  /* DO-NOT-DELETE splicer.begin(TSTTM.Factory.newMesh) */
  TSTTM_Mesh tsttm_mesh;
  SimpleMesh_Mesh simple_mesh;

  if (strcmp(option,"Simple")==0) {
    simple_mesh = SimpleMesh_Mesh__create();
    tsttm_mesh = TSTTM_Mesh__cast(simple_mesh);
    return(tsttm_mesh);
  } else {
    TSTTB_Error err = TSTTB_Error__create();
    TSTTB_Error_set(err,TSTTB_ErrorType_INVALID_ARGUMENT,"");
    *_ex = sidl_BaseInterface__cast(err); 
  }
  /* DO-NOT-DELETE splicer.end(TSTTM.Factory.newMesh) */
}
/* Babel internal methods, Users should not edit below this line. */
struct TSTTB_Error__object* impl_TSTTM_Factory_fconnect_TSTTB_Error(char* url,
  sidl_BaseInterface *_ex) {
  return TSTTB_Error__connect(url, _ex);
}
char * impl_TSTTM_Factory_fgetURL_TSTTB_Error(struct TSTTB_Error__object* obj) {
  return TSTTB_Error__getURL(obj);
}
struct sidl_ClassInfo__object* impl_TSTTM_Factory_fconnect_sidl_ClassInfo(char* 
  url, sidl_BaseInterface *_ex) {
  return sidl_ClassInfo__connect(url, _ex);
}
char * impl_TSTTM_Factory_fgetURL_sidl_ClassInfo(struct sidl_ClassInfo__object* 
  obj) {
  return sidl_ClassInfo__getURL(obj);
}
struct TSTTM_Mesh__object* impl_TSTTM_Factory_fconnect_TSTTM_Mesh(char* url,
  sidl_BaseInterface *_ex) {
  return TSTTM_Mesh__connect(url, _ex);
}
char * impl_TSTTM_Factory_fgetURL_TSTTM_Mesh(struct TSTTM_Mesh__object* obj) {
  return TSTTM_Mesh__getURL(obj);
}
struct sidl_BaseInterface__object* 
  impl_TSTTM_Factory_fconnect_sidl_BaseInterface(char* url,
  sidl_BaseInterface *_ex) {
  return sidl_BaseInterface__connect(url, _ex);
}
char * impl_TSTTM_Factory_fgetURL_sidl_BaseInterface(struct 
  sidl_BaseInterface__object* obj) {
  return sidl_BaseInterface__getURL(obj);
}
struct TSTTM_Factory__object* impl_TSTTM_Factory_fconnect_TSTTM_Factory(char* 
  url, sidl_BaseInterface *_ex) {
  return TSTTM_Factory__connect(url, _ex);
}
char * impl_TSTTM_Factory_fgetURL_TSTTM_Factory(struct TSTTM_Factory__object* 
  obj) {
  return TSTTM_Factory__getURL(obj);
}
struct sidl_BaseClass__object* impl_TSTTM_Factory_fconnect_sidl_BaseClass(char* 
  url, sidl_BaseInterface *_ex) {
  return sidl_BaseClass__connect(url, _ex);
}
char * impl_TSTTM_Factory_fgetURL_sidl_BaseClass(struct sidl_BaseClass__object* 
  obj) {
  return sidl_BaseClass__getURL(obj);
}
