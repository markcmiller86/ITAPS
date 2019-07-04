// 
// File:          TSTTM_Swap_Swap3D_Impl.hh
// Symbol:        TSTTM_Swap.Swap3D-v0.1
// Symbol Type:   class
// Babel Version: 0.9.2
// Description:   Server-side implementation for TSTTM_Swap.Swap3D
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.9.2
// 

#ifndef included_TSTTM_Swap_Swap3D_Impl_hh
#define included_TSTTM_Swap_Swap3D_Impl_hh

#ifndef included_sidl_cxx_hh
#include "sidl_cxx.hh"
#endif
#ifndef included_TSTTM_Swap_Swap3D_IOR_h
#include "TSTTM_Swap_Swap3D_IOR.h"
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
#ifndef included_TSTTM_Swap_Swap3D_hh
#include "TSTTM_Swap_Swap3D.hh"
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


// DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap3D._includes)
#ifndef included_TSTTM_Entity_hh
#include "TSTTM_Entity.hh"
#endif
#ifndef include_TSTTGM_Classification_hh
#include "TSTTGM_Classification.hh"
#endif

#include <set>

#undef NO_BABEL

#ifdef NO_BABEL
#include "TSTTM_Swap_QM_Base.hh"
#include "/home/cfog/TSTT/lib/GRUMMP_TSTTM/GRUMMP_TSTTM_Volume_Impl.hh"
#define MESH_IMPLEMENTATION_CLASS GRUMMP_TSTTM::Volume_impl*
#define QUALITY_MEASURE QM_Base*
#else
#define MESH_IMPLEMENTATION_CLASS TSTTM::Modify
#define QUALITY_MEASURE TSTTM_Swap::QualMeasure
#endif
// DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap3D._includes)

namespace TSTTM_Swap { 

  /**
   * Symbol "TSTTM_Swap.Swap3D" (version 0.1)
   */
  class Swap3D_impl
  // DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap3D._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap3D._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Swap3D self;

    // DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap3D._implementation)
    enum eFaceCat {eT23 = 0, eT32 = 1, eT22 = 2, eT44 = 3, eN32 = 4,
		   eN44 = 5, eN40 = 6, eN30 = 7, eN20 = 8, eBdry = 9,
		   eBdryReflex = 10, eOther = 11};
    // Private data

    SwapType ST_int;
    bool qAllowRecursion, qAllowEdgeSwapping, qAllowBdryChanges;
    double dMaxAngle;

    std::set<void*> soRecursionQueue;
    // Need functions from all of these interfaces
#ifdef NO_BABEL    
    QUALITY_MEASURE pQM;
    MESH_IMPLEMENTATION_CLASS pMesh;
  public:
    void
    setMeshData (
      /*in*/ MESH_IMPLEMENTATION_CLASS pModIn
    )
    throw ( 
      ::TSTT::Error
    );
    void
    setUserQualMeasure (
      /*in*/ QUALITY_MEASURE QM
    )
    throw ( 
      ::TSTT::Error
    );

  private:
#define QM_int (*pQM)
#define ModMesh (*pMesh)
#define EntMesh (*pMesh)
#define ClassMesh (*pMesh)
#else
    QualMeasure QM_int;

    TSTTM::Entity EntMesh;
    TSTTM::Modify ModMesh;
    TSTTGM::Classification ClassMesh;
#endif
    
    eFaceCat eFC;

    // Private functions
    void vUnitNormal(const double adA[3], const double adB[3],
		     const double adC[3], double adRes[3]);
    int iOrient3D(const double adA[3], const double adB[3],
		  const double adC[3], const double adD[3]);
    int iOrient3D(void* oVertA, void* oVertB,
		  void* oVertC, void* oVertD);
    int iOrient3D(void* oTet);
    int iGenOrient3D(void* oVertA, void* oVertB,
		     void* oVertC, void* oVertD,
		     bool& qApproxFlat);
    int iInsphere(const double adA[3], const double adB[3],
		  const double adC[3], const double adD[3],
		  const double adE[3]);
    double dDet4By4(const double a2dMat4[4][4]) const;
    bool qTieBreak(const double adA[3], const double adB[3],
		   const double adC[3], const double adD[3],
		   const double adE[3]);
    bool qDoSwapDelaunay(const double adA[3], const double adB[3],
			 const double adC[3], const double adD[3],
			 const double adE[3]);
    bool qDoSwapMaxDihed(const double adA[3], const double adB[3],
			 const double adC[3], const double adD[3],
			 const double adE[3]);
    bool qDoSwapMinSine(const double adA[3], const double adB[3],
			const double adC[3], const double adD[3],
			const double adE[3]);
    bool qDoSwapUser(const double adA[3], const double adB[3],
		     const double adC[3], const double adD[3],
		     const double adE[3]);
    double dEvaluateQual(const double adLocA[3], const double adLocB[3],
			 const double adLocC[3], const double adLocD[3]);
    double dEvaluateQual(void* oTet);
    double dEvaluateQual(void* oVertA, void* oVertB,
			 void* oVertC, void* oVertD);
    bool qDoSwap(const double adCoordsA[3], const double adCoordsB[3],
		 const double adCoordsC[3], const double adCoordsD[3],
		 const double adCoordsE[3]);
    bool qDoSwap(void* oVertA, void* oVertB,
		 void* oVertC, void* oVertD,
		 void* oVertE);

    // The following gets the faces opposite three verts at once, since
    // this comes up often.
    void getFacesOpposite(void* const oCell, void* const aoVerts[],
			  void* aoFaces[]);
    void* getFaceOpposite(void* const oCell, void* const oVert);
    void* getVertOpposite(void* const oCell, void* const oFace);
    void* getCellOpposite(void* const oFace, void* const oCell);

    eFaceCat eCategorizeFace(void* oFace, void* aoVerts[6], void* aoTets[4],
			     void*& oPivot0, void*& oPivot1, void*& oOther);
    
    int iReconfTet23(void* entity_set, void* aoTets[],
		     void* oFace, void* aoVerts[]) throw(TSTT::Error);
    int iReconfTet32(void* entity_set, void* aoTets[],
		     void* oFace, void* aoVerts[]) throw(TSTT::Error);
    int iReconfTet22(void* entity_set, void* aoTets[],
		     void* oFace, void* aoVerts[]) throw(TSTT::Error);
    int iReconfTet44(void* entity_set, void* aoTets[],
		     void* oFace, void* aoVerts[]) throw(TSTT::Error);

    int iEdgeSwap3D(void* entity_set, void* oFace, void* oVNorth,
		    void* oVSouth, void* oVOther) throw(TSTT::Error);
    int iBdryEdgeSwap3D(void* entity_set, void* oFace, void* oVNorth,
			void* oVSouth, void* oVOther) throw(TSTT::Error);

    void* createFace(void * oVert0, void * oVert1, void * oVert2)
      throw(TSTT::Error);
    void* createTet (void * oFace0, void * oFace1, void * oFace2,
		     void * oFace3) throw(TSTT::Error);

    bool qTetCheck(void * oTet);
    bool qFaceCheck(void * oFace);
    bool qHasVert(void * oEntity, void * oVert);
    // DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap3D._implementation)

  private:
    // private default constructor (required)
    Swap3D_impl() {} 

  public:
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Swap3D_impl( struct TSTTM_Swap_Swap3D__object * s ) : self(s,
      true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Swap3D_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

  public:

    /**
     * user defined non-static method.
     */
    void
    setEdgeSwapping (
      /*in*/ bool allow_edge_swapping
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    bool
    isEdgeSwappingAllowed() throw ( 
      ::TSTTB::Error
    );
    /**
     * user defined non-static method.
     */
    void
    setBdryMutable (
      /*in*/ bool allow_bdry_changes
    )
    throw ( 
      ::TSTTB::Error
    );

    /**
     * user defined non-static method.
     */
    bool
    isBdryMutable() throw ( 
      ::TSTTB::Error
    );
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

  };  // end class Swap3D_impl

} // end namespace TSTTM_Swap

// DO-NOT-DELETE splicer.begin(TSTTM_Swap.Swap3D._misc)
// DO-NOT-DELETE splicer.end(TSTTM_Swap.Swap3D._misc)

#endif
