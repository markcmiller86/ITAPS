// 
// File:          TSTTM_Swap_Delaunay3D_Impl.hh
// Symbol:        TSTTM_Swap.Delaunay3D-v0.1
// Symbol Type:   class
// Babel Version: 0.9.2
// Description:   Server-side implementation for TSTTM_Swap.Delaunay3D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.9.2
// 

#ifndef included_TSTTM_Swap_Delaunay3D_Impl_hh
#define included_TSTTM_Swap_Delaunay3D_Impl_hh

#ifndef included_sidl_cxx_hh
#include "sidl_cxx.hh"
#endif
#ifndef included_TSTTM_Swap_Delaunay3D_IOR_h
#include "TSTTM_Swap_Delaunay3D_IOR.h"
#endif
// 
// Includes for all method dependencies.
// 
#ifndef included_TSTTB_Error_hh
#include "TSTTB_Error.hh"
#endif
#ifndef included_TSTTM_Swap_Delaunay3D_hh
#include "TSTTM_Swap_Delaunay3D.hh"
#endif
#ifndef included_sidl_BaseInterface_hh
#include "sidl_BaseInterface.hh"
#endif
#ifndef included_sidl_ClassInfo_hh
#include "sidl_ClassInfo.hh"
#endif


// DO-NOT-DELETE splicer.begin(TSTTM_Swap.Delaunay3D._includes)
#undef NO_BABEL
#ifdef NO_BABEL
#include "TSTTM_Swap_QM_Base.hh"
#endif
// DO-NOT-DELETE splicer.end(TSTTM_Swap.Delaunay3D._includes)

namespace TSTTM_Swap { 

  /**
   * Symbol "TSTTM_Swap.Delaunay3D" (version 0.1)
   */
  class Delaunay3D_impl
  // DO-NOT-DELETE splicer.begin(TSTTM_Swap.Delaunay3D._inherits)
#ifdef NO_BABEL
    : public QM_Base
#endif
  // DO-NOT-DELETE splicer.end(TSTTM_Swap.Delaunay3D._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Delaunay3D self;

    // DO-NOT-DELETE splicer.begin(TSTTM_Swap.Delaunay3D._implementation)
    // Put additional implementation details here...
    // DO-NOT-DELETE splicer.end(TSTTM_Swap.Delaunay3D._implementation)

  private:
    // private default constructor (required)
    Delaunay3D_impl() {} 

  public:
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Delaunay3D_impl( struct TSTTM_Swap_Delaunay3D__object * s ) : self(s,
      true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Delaunay3D_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

  public:

    /**
     * user defined non-static method.
     */
    bool
    shouldMinimize() throw ( 
      ::TSTTB::Error
    );
    /**
     * user defined non-static method.
     */
    double
    calcQuality (
      /*in*/ ::sidl::array<double> coords,
      /*in*/ int32_t coords_size
    )
    throw ( 
      ::TSTTB::Error
    );

  };  // end class Delaunay3D_impl

} // end namespace TSTTM_Swap

// DO-NOT-DELETE splicer.begin(TSTTM_Swap.Delaunay3D._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(TSTTM_Swap.Delaunay3D._misc)

#endif
