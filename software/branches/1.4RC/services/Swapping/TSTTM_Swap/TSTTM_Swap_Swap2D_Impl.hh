// 
// File:          TSTTM_Swap_Swap2D_Impl.hh
// Symbol:        TSTTM_Swap.Swap2D-v0.1
// Symbol Type:   class
// Babel Version: 0.9.2
// Description:   Server-side implementation for TSTTM_Swap.Swap2D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.9.2
// 

#ifndef included_TSTTM_Swap_Swap2D_Impl_hh
#define included_TSTTM_Swap_Swap2D_Impl_hh

#ifndef included_sidl_cxx_hh
#include "sidl_cxx.hh"
#endif
#ifndef included_TSTTM_Swap_Swap2D_IOR_h
#include "TSTTM_Swap_Swap2D_IOR.h"
#endif
// 
// Includes for all method dependencies.
// 
#ifndef included_TSTTB_Error_hh
#include "TSTTB_Error.hh"
#endif
#ifndef included_TSTTM_Modify_hh
#include "TSTTM_Modify.hh"
#endif
#ifndef included_TSTTM_Swap_QualMeasure_hh
#include "TSTTM_Swap_QualMeasure.hh"
#endif
#ifndef included_TSTTM_Swap_Swap2D_hh
#include "TSTTM_Swap_Swap2D.hh"
#endif
#ifndef included_TSTTM_Swap_SwapType_hh
#include "TSTTM_Swap_SwapType.hh"
#endif
#ifndef included_sidl_BaseInterface_hh
#include "sidl_BaseInterface.hh"
#endif
#ifndef included_sidl_ClassInfo_hh
#include "sidl_ClassInfo.hh"
#endif


// DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap2D._includes)
#ifndef included_TSTTM_Entity_hh
#include "TSTTM_Entity.hh"
#endif
#ifndef include_TSTTGM_Classification_hh
#include "TSTTGM_Classification.hh"
#endif

#include <set>

#undef NO_BABEL

#ifdef NO_BABEL
#include "/home/cfog/TSTT/lib/GRUMMP_TSTTM/GRUMMP_TSTTM_Plane_Impl.hh"
#define MESH_IMPLEMENTATION_CLASS GRUMMP_TSTTM::Plane_impl*
#else
#define MESH_IMPLEMENTATION_CLASS TSTTM::Modify
#endif
// DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap2D._includes)

namespace TSTTM_Swap { 

  /**
   * Symbol "TSTTM_Swap.Swap2D" (version 0.1)
   */
  class Swap2D_impl
  // DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap2D._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap2D._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Swap2D self;

    // DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap2D._implementation)
    // Private data
    SwapType ST_int;
    QualMeasure QM_int;
    bool qAllowRecursion;
    double dMaxAngle;

    std::set<void*> soRecursionQueue;

    // Need to access these various TSTT* interfaces internally.
#ifdef NO_BABEL    
    MESH_IMPLEMENTATION_CLASS pMesh;
  public:
    void
    setMeshData (
      /*in*/ MESH_IMPLEMENTATION_CLASS pModIn
    )
    throw ( 
      ::TSTT::Error
    );
  private:

#define ModMesh (*pMesh)
#define EntMesh (*pMesh)
#define ClassMesh (*pMesh)
#else
    TSTTGM::Classification ClassMesh;
    TSTTM::Entity EntMesh;
    TSTTM::Modify ModMesh;
#endif

    // Private functions
    void vNormal(const double adA[2], const double adB[2], double adRes[2]);
    int iOrient2D(const double adA[2], const double adB[2],
		  const double adC[2]);
    int iIncircle(const double adA[2], const double adB[2],
		  const double adC[2], const double adD[2]);
    bool qTieBreak(const double adA[2], const double adB[2],
		   const double adC[2], const double adD[2]);
    bool qDoSwapDelaunay(const double adA[2], const double adB[2],
			 const double adC[2], const double adD[2]);
    bool qDoSwapMaxDihed(const double adA[2], const double adB[2],
			 const double adC[2], const double adD[2]);
    bool qDoSwapUser(const double adA[2], const double adB[2],
		     const double adC[2], const double adD[2]);
    bool qDoSwap(const double adCoordsA[2], const double adCoordsB[2],
		 const double adCoordsL[2], const double adCoordsR[2]);
    // DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap2D._implementation)

  private:
    // private default constructor (required)
    Swap2D_impl() {} 

  public:
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Swap2D_impl( struct TSTTM_Swap_Swap2D__object * s ) : self(s,
      true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Swap2D_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

  public:

    /**
     * user defined non-static method.
     */
    void
    setSwapType (
      /*in*/ ::TSTTM_Swap::SwapType ST
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    void
    setUserQualMeasure (
      /*in*/ ::TSTTM_Swap::QualMeasure QM
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    ::TSTTM_Swap::SwapType
    getSwapType() throw ( 
      ::TSTTB::Error
    );
    /**
     * user defined non-static method.
     */
    void
    setSwapRecursion (
      /*in*/ bool allow_recursion
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    bool
    isRecursionAllowed() throw ( 
      ::TSTTB::Error
    );
    /**
     * user defined non-static method.
     */
    void
    setMeshData (
      /*in*/ ::TSTTM::Modify Mod
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    int32_t
    swap (
      /*inout*/ void*& entity_set,
      /*inout*/ void*& entity_handle
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    int32_t
    swapAll (
      /*inout*/ void*& entity_set
    )
    throw ( 
      ::TSTTB::Error
    );

  };  // end class Swap2D_impl

} // end namespace TSTTM_Swap

// DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap2D._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap2D._misc)

#endif
