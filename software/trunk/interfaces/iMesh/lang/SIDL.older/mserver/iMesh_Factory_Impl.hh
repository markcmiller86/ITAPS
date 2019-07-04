// 
// File:          iMesh_Factory_Impl.hh
// Symbol:        iMesh.Factory-v0.7
// Symbol Type:   class
// Babel Version: 0.10.12
// sidl Created:  20070927 14:57:59 CDT
// Generated:     20070927 14:58:07 CDT
// Description:   Server-side implementation for iMesh.Factory
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.12
// source-line   = 296
// source-url    = file:/home/tautges/MOAB/tools/iMesh/SIDL/iMesh.sidl
// 

#ifndef included_iMesh_Factory_Impl_hh
#define included_iMesh_Factory_Impl_hh

#ifndef included_sidl_cxx_hh
#include "sidl_cxx.hh"
#endif
#ifndef included_iMesh_Factory_IOR_h
#include "iMesh_Factory_IOR.h"
#endif
// 
// Includes for all method dependencies.
// 
#ifndef included_iBase_Error_hh
#include "iBase_Error.hh"
#endif
#ifndef included_iMesh_Factory_hh
#include "iMesh_Factory.hh"
#endif
#ifndef included_iMesh_Mesh_hh
#include "iMesh_Mesh.hh"
#endif
#ifndef included_sidl_BaseInterface_hh
#include "sidl_BaseInterface.hh"
#endif
#ifndef included_sidl_ClassInfo_hh
#include "sidl_ClassInfo.hh"
#endif


// DO-NOT-DELETE splicer.begin(iMesh.Factory._includes)
// Insert-Code-Here {iMesh.Factory._includes} (includes or arbitrary code)
// DO-NOT-DELETE splicer.end(iMesh.Factory._includes)

namespace iMesh { 

  /**
   * Symbol "iMesh.Factory" (version 0.7)
   */
  class Factory_impl
  // DO-NOT-DELETE splicer.begin(iMesh.Factory._inherits)
  // Insert-Code-Here {iMesh.Factory._inherits} (optional inheritance here)
  // DO-NOT-DELETE splicer.end(iMesh.Factory._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Factory self;

    // DO-NOT-DELETE splicer.begin(iMesh.Factory._implementation)
    // Insert-Code-Here {iMesh.Factory._implementation} (additional details)
    // DO-NOT-DELETE splicer.end(iMesh.Factory._implementation)

  private:
    // private default constructor (required)
    Factory_impl() 
    {} 

  public:
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Factory_impl( struct iMesh_Factory__object * s ) : self(s,true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Factory_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

    // static class initializer
    static void _load();

  public:
    /**
     * user defined static method.
     */
    static void
    newMesh (
      /* in */ const ::std::string& option,
      /* out */ ::iMesh::Mesh& new_mesh
    )
    throw ( 
      ::iBase::Error
    );


  };  // end class Factory_impl

} // end namespace iMesh

// DO-NOT-DELETE splicer.begin(iMesh.Factory._misc)
// Insert-Code-Here {iMesh.Factory._misc} (miscellaneous things)
// DO-NOT-DELETE splicer.end(iMesh.Factory._misc)

#endif
