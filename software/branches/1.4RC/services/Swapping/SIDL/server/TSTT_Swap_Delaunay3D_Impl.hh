// 
// File:          TSTT_Swap_Delaunay3D_Impl.hh
// Symbol:        TSTT_Swap.Delaunay3D-v0.7
// Symbol Type:   class
// Babel Version: 0.10.10
// Description:   Server-side implementation for TSTT_Swap.Delaunay3D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.10
// 

#ifndef included_TSTT_Swap_Delaunay3D_Impl_hh
#define included_TSTT_Swap_Delaunay3D_Impl_hh

#ifndef included_sidl_cxx_hh
#include "sidl_cxx.hh"
#endif
#ifndef included_TSTT_Swap_Delaunay3D_IOR_h
#include "TSTT_Swap_Delaunay3D_IOR.h"
#endif
// 
// Includes for all method dependencies.
// 
#ifndef included_TSTTB_Error_hh
#include "TSTTB_Error.hh"
#endif
#ifndef included_TSTT_Swap_Delaunay3D_hh
#include "TSTT_Swap_Delaunay3D.hh"
#endif
#ifndef included_sidl_BaseInterface_hh
#include "sidl_BaseInterface.hh"
#endif
#ifndef included_sidl_ClassInfo_hh
#include "sidl_ClassInfo.hh"
#endif


// DO-NOT-DELETE splicer.begin(TSTT_Swap.Delaunay3D._includes)
#ifdef NO_BABEL_QUAL
#include "TSTT_Swap_QM_Base.hh"
#endif
// DO-NOT-DELETE splicer.end(TSTT_Swap.Delaunay3D._includes)

namespace TSTT_Swap { 

  /**
   * Symbol "TSTT_Swap.Delaunay3D" (version 0.7)
   */
  class Delaunay3D_impl
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Delaunay3D._inherits)
#ifdef NO_BABEL_QUAL
    : public QM_Base
#endif
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Delaunay3D._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Delaunay3D self;

    // DO-NOT-DELETE splicer.begin(TSTT_Swap.Delaunay3D._implementation)
    // Insert-Code-Here {TSTT_Swap.Delaunay3D._implementation} (additional details)
    // DO-NOT-DELETE splicer.end(TSTT_Swap.Delaunay3D._implementation)

  public:
    // private default constructor (required)
    Delaunay3D_impl() 
    {} 

  public:
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Delaunay3D_impl( struct TSTT_Swap_Delaunay3D__object * s ) : self(s,
      true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Delaunay3D_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

    // static class initializer
    static void _load();

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
      /* in */ ::sidl::array<double> coords,
      /* in */ int32_t coords_size
    )
    throw ( 
      ::TSTTB::Error
    );

  };  // end class Delaunay3D_impl

} // end namespace TSTT_Swap

// DO-NOT-DELETE splicer.begin(TSTT_Swap.Delaunay3D._misc)
// Insert-Code-Here {TSTT_Swap.Delaunay3D._misc} (miscellaneous things)
// DO-NOT-DELETE splicer.end(TSTT_Swap.Delaunay3D._misc)

#endif
