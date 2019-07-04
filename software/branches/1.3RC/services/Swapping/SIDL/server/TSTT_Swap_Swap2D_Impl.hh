// 
// File:          TSTT_Swap_Swap2D_Impl.hh
// Symbol:        TSTT_Swap.Swap2D-v0.7
// Symbol Type:   class
// Babel Version: 0.10.10
// Description:   Server-side implementation for TSTT_Swap.Swap2D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.10.10
// 

#ifndef included_TSTT_Swap_Swap2D_Impl_hh
#define included_TSTT_Swap_Swap2D_Impl_hh

#ifndef included_sidl_cxx_hh
#include "sidl_cxx.hh"
#endif
#ifndef included_TSTT_Swap_Swap2D_IOR_h
#include "TSTT_Swap_Swap2D_IOR.h"
#endif
// 
// Includes for all method dependencies.
// 
#ifndef included_TSTTB_Error_hh
#include "TSTTB_Error.hh"
#endif
#ifndef included_TSTTG_Model_hh
#include "TSTTG_Model.hh"
#endif
#ifndef included_TSTTM_Modify_hh
#include "TSTTM_Modify.hh"
#endif
#ifndef included_TSTTR_Associate_hh
#include "TSTTR_Associate.hh"
#endif
#ifndef included_TSTT_Swap_QualMeasure_hh
#include "TSTT_Swap_QualMeasure.hh"
#endif
#ifndef included_TSTT_Swap_Swap2D_hh
#include "TSTT_Swap_Swap2D.hh"
#endif
#ifndef included_TSTT_Swap_SwapType_hh
#include "TSTT_Swap_SwapType.hh"
#endif
#ifndef included_sidl_BaseInterface_hh
#include "sidl_BaseInterface.hh"
#endif
#ifndef included_sidl_ClassInfo_hh
#include "sidl_ClassInfo.hh"
#endif


// DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._includes)
#ifndef included_TSTTM_Entity_hh
#include "TSTTM_Entity.hh"
#endif

#include <set>

#ifdef NO_BABEL
#include "TSTTM_GRUMMP_Plane_Impl.hh"
#define MESH_IMPLEMENTATION_CLASS TSTTM_GRUMMP::Plane_impl*
#else
#define MESH_IMPLEMENTATION_CLASS TSTTM::Modify
#endif
// DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._includes)

namespace TSTT_Swap { 

  /**
   * Symbol "TSTT_Swap.Swap2D" (version 0.7)
   */
  class Swap2D_impl
  // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._inherits)
  // Insert-Code-Here {TSTT_Swap.Swap2D._inherits} (optional inheritance here)
  // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Swap2D self;

    // DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._implementation)
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
	     ) throw ( ::TSTTB::Error )
    { pMesh = pModIn; }
  private:

#define ModMesh (*pMesh)
#define EntMesh (*pMesh)
#else
    TSTTM::Entity EntMesh;
    TSTTM::Modify ModMesh;
#endif
    TSTTR::Associate Assoc;
    TSTTG::Model GModel;

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
    // DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._implementation)

  public:
    // private default constructor (required)
    Swap2D_impl() 
    {} 

  public:
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Swap2D_impl( struct TSTT_Swap_Swap2D__object * s ) : self(s,
      true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Swap2D_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

    // static class initializer
    static void _load();

  public:

    /**
     * user defined non-static method.
     */
    void
    setSwapType (
      /* in */ ::TSTT_Swap::SwapType ST
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    void
    setUserQualMeasure (
      /* in */ ::TSTT_Swap::QualMeasure QM
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    ::TSTT_Swap::SwapType
    getSwapType() throw ( 
      ::TSTTB::Error
    );
    /**
     * user defined non-static method.
     */
    void
    setSwapRecursion (
      /* in */ bool allow_recursion
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
      /* in */ ::TSTTM::Modify Mod_Mesh
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    void
    setModelData (
      /* in */ ::TSTTG::Model Geom_Model
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    void
    setAssocData (
      /* in */ ::TSTTR::Associate Assoc_Data
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    int32_t
    swap (
      /* inout */ void*& entity_set,
      /* inout */ void*& entity_handle
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    int32_t
    swapAll (
      /* inout */ void*& entity_set
    )
    throw ( 
      ::TSTTB::Error
    );

  };  // end class Swap2D_impl

} // end namespace TSTT_Swap

// DO-NOT-DELETE splicer.begin(TSTT_Swap.Swap2D._misc)
// Insert-Code-Here {TSTT_Swap.Swap2D._misc} (miscellaneous things)
// DO-NOT-DELETE splicer.end(TSTT_Swap.Swap2D._misc)

#endif
