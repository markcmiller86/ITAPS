#ifndef ITAPS_Swap_hh
#define ITAPS_Swap_hh

#include <set>
#include <assert.h>

/*! \mainpage ITAPS Swapping Service

Mesh swapping improves the quality of unstructured meshes by changing
the local topology of the mesh.  This technique is especially helpful in
three dimensions, where the combination of swapping and shape
improvement has been shown to dramatically improve mesh quality and
simulation efficiency.  As part of SciDAC-1, a state-of-the-art swapping
algorithm (part of the GRUMMP meshing suite) was re-implemented using
the mesh and geometry interfaces. This swapping software is already in
use in one widely-distributed SciDAC meshing code (GRUMMP) with over 500
downloads per year for applications in fluid and solid mechanics,
medicine, and astrophysics.

In two dimensions, face swapping chooses the best diagonal for the
quadrilateral formed by two neighboring triangles. Pre-defined quality
criteria include the Delaunay criterion and the maximum angle criterion.

In three dimensions, the swapping service performs both face swapping
and edge swapping.  The former reconfigures tetrahedra that share a
common face, while the latter reconfigures all tetrahedra incident on a
single edge.  The canonical face swapping case is exchanging two
tetrahedra that share a face with three tetrahedra, as shown in the top
left part of Figure 1. The inverse swap, from three to two tetrahedra,
is also possible. In addition, we allow reconfiguration of two tets to
two (T22 case in the figure); in this case, the two shaded faces must be
co-planar, and swapping decisions reduce to choosing the best diagonal
for the coplanar quadrilateral. If two pairs of tetrahedra in the
interior of the mesh share a pair of coplanar faces, this swap is also
permitted; in this case, two T22 configurations are back-to-back in the
mesh. In addition to these swappable configurations, there are a number
of unswappable cases, some of which are illustrated in the bottom of
Figure 1.

\image html http://itaps-scidac.org/graphics/swap-3Dface.jpg "Figure 1: 3D face swapping configurations; shaded faces are coplanar."

Edge swapping is a more complicated procedure, replacing N tetrahedra
incident a single edge by a new set of 2N-4 tetrahedra. In the example
of Figure 2, the edge TB is perpendicular to the screen. The five
tetrahedra originally incident on it (01TB, 12TB, 23TB, 34TB, 40TB) are
replaced by six new tetrahedra, two for each of the triangles of the
(non-planar) triangulation of polygon 01234: 012T, 024T, 234T, 021B,
042B, and 324B.

\image html http://itaps-scidac.org/graphics/swap-edge-schematic.jpg "Figure 2: Edge swapping schematic"

The challenge with edge swapping is that the number of possible
configurations grows rapidly with the number of tetrahedra incident on
the edge to be removed (for a 7-for-10 swap, there are 42 possible
configurations). In practice, the number of successful 7-for-10 swaps is
very small, so the swapping service does not explore possible swaps for
more complex initial configurations.

Checking the quality of each tetrahedron in each possible configuration
is a costly undertaking; instead, the quality for each unique
tetrahedron is computed only once, and the quality of a given
configuration is the minimum quality among its tetrahedra.  For a
7-for-10 swap, this reduces the number of quality checks from 420 for a
naive implementation of checking only the 70 unique tets that can appear
in the final configuration.  To simplify bookkeeping, we also take
advantage of the symmetries of the post-edge-swapping configurations and
store only a small set of canonical post-swap configurations, as shown
in Figure 3.

\image html http://itaps-scidac.org/graphics/swap-canonical.jpg "Figure 3: Canonical configurations for edge swapping, including repeat count."

Both face and edge swapping support pre-defined quality measures that
enforce the Delaunay criterion, maximize the minimum sine of dihedral
angles (recommended) or minimize the maximum dihedral angle.
User-defined quality measures are also supported. 
*/

#include "ITAPS_Swap_config.hh"
#include "iMesh.h"
#ifdef USE_RELATIONS
#include "iRel.h"
#endif

#include "ITAPS_QualMeasure.hh"

#include <iostream>
#define CHECK_ERR(error_, pMesh_) do {					\
    if (error_ != iBase_SUCCESS) {					\
      char strDesc_[1024];						\
      iMesh_getDescription(pMesh_, strDesc_, 1024);			\
      std::cerr << "Error " << error_ << " at " << __FILE__ << ":" << __LINE__ \
		<< ":  " << strDesc_ << std::endl;			\
      throw(iBase_ErrorType(error_));					\
    }									\
  } while (0)

namespace ITAPS_Swap {
  /*! \brief Enumeration of quality measures for swapping decisions.
   *
   * The predefined choices include three standards: Delaunay,
   * maximizing the minimum sine of dihedral angle, and minimizing the
   * maximum dihedral angle.  In addition, user-defined measures are
   * supported; see the documentation for QM_Base for more information.
   */
  enum SwapType {
    SwapType_MAX_MIN_SINE,
    SwapType_MIN_MAX_ANGLE,
    SwapType_DELAUNAY,
    SwapType_MESQUITE,
    SwapType_USER_DEFINED,
    SwapType_UNDEFINED
  };

  /*! \class Swap
   * \brief Virtual base class for 2D and 3D swapping classes.
   *
   * The swapping classes hold minimal state data (an iMesh instance and
   * some control options); they're mostly a way to encapsulate
   * algorithms within a common interface.
   */
  class Swap
  {
  protected:
    /*! \brief The swapping measure to use. */
    SwapType ST_int;
    /*! \brief Quality measure class to use. */
    QM_Base *pQM_int;
    /*! \brief Swap faces near changes in the mesh if this is true.
     * \sa soRecursionQueue
     */
    bool qAllowRecursion;
    /*! \brief Change only entities in a given set if this is true.
     *
     * The set is stored in shSet, below.
     */
    bool qUsingSet;
    /*! \brief Create triangles and tets directly from verts if this is
     *      true.
     *
     * Some implementations support creation of triangles and tetrahedra
     * only from vertices, others only from edges (2D) or faces (3D),
     * still others from both.  This flag tells the swapping code which
     * to use.
     */
    bool qCreateFromVerts;
    /*! \brief A queue of faces to check because of recursion.
     * \sa qAllowRecursion
     */
    std::set<iBase_EntityHandle> soRecursionQueue;
    /*! \brief The mesh database containing the mesh to swap. */
    iMesh_Instance pMeshInst;
#ifdef USE_RELATIONS
    /*! \brief The relations database holding classification info.
     *
     * When specified, this improves the fidelity of swapping
     * by prohibiting swaps that involve triangles classified on
     * different geometric surfaces or tets classified on different
     * geometric volumes.
     *
     * This is optional at present.
     */
    iRel_Instance pRelInst;
#endif
    /*! \brief Only change highest-dimensional entities that are in this
     * set.
     *
     * This is optional; if no set is provided, the entire mesh is
     * swapped.
     * \sa qUsingSet
     */
    iBase_EntitySetHandle shSet;
    /*! \brief The root set of the mesh is needed for internal
     * purposes. */
    iBase_EntitySetHandle shRootSet;

  public:
    /*! \brief Default constructor.
     *
     * By default, Delaunay swapping is used, recursion is enabled, and
     * the entire mesh is swapped.  Note that the mesh database is not
     * yet specified.
     */
    Swap() : ST_int(SwapType_DELAUNAY), pQM_int(new Delaunay3D()),
	     qAllowRecursion(true), qUsingSet(false), soRecursionQueue(),
	     pMeshInst(NULL),
#ifdef USE_RELATIONS
	     pRelInst(NULL),
#endif
	     shSet(NULL), shRootSet(NULL)  
    {}

    /*! \brief Destructor */
    virtual ~Swap() {delete pQM_int;}

    /*! \brief Set a new swapping type.
     *
     * Only predefined swapping types are allowed for this function,
     * which also assumes that the mesh is 3D.
     */
    void setSwapType (SwapType ST)
    {
      ST_int = ST;
      if (pQM_int) delete pQM_int;
      switch (ST) {
      case SwapType_MAX_MIN_SINE:
	pQM_int = new MaxMinSine3D();
	break;
      case SwapType_MIN_MAX_ANGLE:
	pQM_int = new MinMaxAngle3D();
	break;
      case SwapType_DELAUNAY:
	pQM_int = new Delaunay3D();
	break;
      default:
	throw(iBase_INVALID_ARGUMENT);
	break;
      }
    }

#ifdef HAVE_MESQUITE_HPP
    /*! \brief Set a new swapping type.
     *
     * This version of the function defines a swap measure based on a
     * Mesquite quality metric.
     */
    void setSwapType (Mesquite::ElementQM *mesqQM)
    {
      ST_int = SwapType_MESQUITE;
      if (pQM_int) delete pQM_int;
      pQM_int = new Mesquite3D(mesqQM);
    }
#endif

    /*! \brief Return the swapping type in use. */
    SwapType getSwapType() const
      {return ST_int;}

    /*! \brief Set whether entities should be created directly from vertices */
    void createFromVertsOnly(bool create_from_verts) 
    { qCreateFromVerts = create_from_verts; }

    /*! \brief Set whether recursion should be used. */
    void setSwapRecursion (bool allow_recursion)
      {qAllowRecursion = allow_recursion;}

    /*! \brief Query whether recursion is in use. */
    bool isRecursionAllowed() const
      {return qAllowRecursion;}

    /*! \brief Set a user-defined quality measure. */
    void setUserQualMeasure (ITAPS_Swap::QM_Base* pQM)
    {
      ST_int = SwapType_USER_DEFINED;
      pQM_int = pQM;
    }

    /*! \brief Specify the mesh database to be swapped. */
    void setMeshData (iMesh_Instance instance)
    {
      pMeshInst = instance;
      int err = iBase_SUCCESS;
      iMesh_getRootSet(pMeshInst, &shRootSet, &err);
      CHECK_ERR(err, pMeshInst);
    }

#ifdef USE_RELATIONS
    /*! \brief Specify the relations database to be used for
     * classification data. */
    void setAssocData (iRel_Instance instance)
      {pRelInst = instance;}
#endif
    /*! \brief Swap a single d-1 dimensional entity.
     *
     * No recursion is ever done by this single-entity call.
     *
     * \param[in] entity_set If not the root set, all entities to be
     * changed must be in the set. 
     *
     * \param[in] entity_handle The handle of the d-1 dimensional entity
     * to check.
     *
     * \return Number of equivalent face swaps performed; can be more
     *         than one if an edge swap is performed in 3D.
     */
    virtual int swap (iBase_EntitySetHandle entity_set,
		      iBase_EntityHandle entity_handle) = 0;

    /*! \brief Swap the entire mesh or entity set.
     *
     * Checks all entities in the given set for swapping.  Multiple
     * passes are made through the mesh to ensure that all candidate
     * swaps are considered; a second call to this function with the
     * same swapping parameters should produce no additional change to
     * the mesh.
     *
     * An optional tag handle argument allows fine-grained control: a
     * d-1 dimensional entity is considered for swapping only if the
     * integer tag value is non-zero; if the tag is not set, a value of
     * zero (no swapping) is inferred.
     *
     * \param[in] entity_set The set whose entities will be considered
     * for swapping; passing the root set specifies swapping for the
     * entire mesh.
     *
     * \param[in] ptr_tag_handle An optional pointer to an
     * integer-valued tag.  If this tag handle is provided, only d-1
     * dimensional entities with a tag value set != 0 will be checked
     * for swapping.
     *
     * \return Total number of swaps performed.
     */
    virtual int swapAll (iBase_EntitySetHandle entity_set,
			 iBase_TagHandle *ptr_tag_handle = NULL) = 0;
  protected:
    /*! \brief Internal function for swapping a single entity. */
    virtual int swap(iBase_EntityHandle entity_handle) = 0;
    /*! \brief Internal convenience function to check whether an entity is in a
     * set. */
    bool qIsEntContained(iBase_EntityHandle entity_handle)
    {
      // This assertion is always true unless the swapping service is broken.
      assert(qUsingSet && shSet);
      int err, result;
      iMesh_isEntContained(pMeshInst, shSet, entity_handle, &result, &err);
      CHECK_ERR(err, pMeshInst);
      return result;
    } 
    /*! \brief Internal convenience function to add an entity to a set. */
    void vAddToSet(iBase_EntityHandle entity_handle) 
    {
      // This assertion is always true unless the swapping service is broken.
      assert(qUsingSet && shSet);
      int err;
      iMesh_addEntToSet(pMeshInst, entity_handle, shSet, &err);
      CHECK_ERR(err, pMeshInst);
    }
  };

  /*! \class Swap2D
   * \brief A specialization of Swap for 2D meshes.
   *
   * All data members and public functions are inherited from Swap.
   */
  class Swap2D : public Swap
  {

  private:
    // Private data
    // Private functions

    /*! \brief Compute normal to the segment between two points.
     *
     * \param[in] adA First point
     * \param[in] adB Second point
     * \param[out] adRes Normal vector (non-normalized)
     */
    void vNormal(const double adA[2], const double adB[2], double adRes[2]);

    /*! \brief Compute orientation of triangle specified by three locations.
     *
     * \param[in] adA First point
     * \param[in] adB Second point
     * \param[in] adC Third point
     * \return Returns 1 if the three points are ordered CCW, -1 if they
     * are ordered CW, and 0 if they are colinear (within tolerance).
     */
    int iOrient2D(const double adA[2], const double adB[2],
		  const double adC[2]);

    /*! \brief Determines if a point lies inside the circumcircle of the
     * triangle specified by three other points.
     *
     * \param[in] adA First point of triangle
     * \param[in] adB Second point of triangle
     * \param[in] adC Third point of triangle
     * \param[in] adD Test point
     * \return Returns 1 if the test point is inside the circumcircle,
     * -1 if it is outside, and 0 if it's on the circle (within tolerance).
     */
    int iIncircle(const double adA[2], const double adB[2],
		  const double adC[2], const double adD[2]);

    /*! \brief Tiebreaker to decide whether to swap when quality is equal. 
     *
     * Sometimes a quality measure gives no preference for one
     * configuration over another.  This function breaks these ties, in
     * a way that's consistent regardless of cyclic permutations of the
     * input points.
     *
     * \param[in] adA First point 
     * \param[in] adB Second point
     * \param[in] adC Third point 
     * \param[in] adD Fourth point
     * \return True if the configuration should be swapped, false otherwise.
     */
    bool qTieBreak(const double adA[2], const double adB[2],
		   const double adC[2], const double adD[2]);

    /*! \brief Decide whether two triangles ABC and ACD should be
     * swapped according to the Delaunay criterion.
     *
     * Note that the maxmin criterion is identical to Delaunay in 2D.
     *
     * \param[in] adA First point 
     * \param[in] adB Second point
     * \param[in] adC Third point 
     * \param[in] adD Fourth point
     * \return True if the configuration should be swapped, false otherwise.
     */
    bool qDoSwapDelaunay(const double adA[2], const double adB[2],
			 const double adC[2], const double adD[2]);

    /*! \brief Decide whether two triangles ABC and ACD should be
     * swapped according to the minmax angle criterion.
     *
     * \param[in] adA First point 
     * \param[in] adB Second point
     * \param[in] adC Third point 
     * \param[in] adD Fourth point
     * \return True if the configuration should be swapped, false otherwise.
     */
    bool qDoSwapMaxDihed(const double adA[2], const double adB[2],
			 const double adC[2], const double adD[2]);

    /*! \brief Decide whether two triangles ABC and ACD should be
     * swapped according to a user-defined criterion.
     *
     * \param[in] adA First point 
     * \param[in] adB Second point
     * \param[in] adC Third point 
     * \param[in] adD Fourth point
     * \return True if the configuration should be swapped, false otherwise.
     */
    bool qDoSwapExternal(const double adA[2], const double adB[2],
			 const double adC[2], const double adD[2]);

    /*! \brief Decide whether two triangles ABC and ACD should be
     * swapped according to the current criterion.
     *
     * \param[in] adA First point 
     * \param[in] adB Second point
     * \param[in] adC Third point 
     * \param[in] adD Fourth point
     * \return True if the configuration should be swapped, false otherwise.
     */
    bool qDoSwap(const double adCoordsA[2], const double adCoordsB[2],
		 const double adCoordsL[2], const double adCoordsR[2]);

  public:
    Swap2D()  : Swap() {}
    virtual ~Swap2D() {}
    virtual int swap (iBase_EntitySetHandle entity_set,
		      iBase_EntityHandle entity_handle);
    virtual int swapAll (iBase_EntitySetHandle entity_set,
			 iBase_TagHandle* ptr_tag_handle = NULL);
  private:
    virtual int swap (iBase_EntityHandle entity_handle);
  };


  /*! \class Swap3D
   * \brief A specialization of Swap for 3D meshes.
   *
   * Because of the possibility of changes to the boundary during 3D
   * swapping, some additional publicly available functionality is
   * defined to control changes to the boundary.
   */
  class Swap3D : public Swap
  {

  private:
    double dMaxAngle;
    enum eFaceCat {eT23 = 0, eT32 = 1, eT22 = 2, eT44 = 3, eN32 = 4,
		   eN44 = 5, eN40 = 6, eN30 = 7, eN20 = 8, eBdry = 9,
		   eBdryReflex = 10, eOther = 11};
    // Private data

    bool qAllowEdgeSwapping, qAllowBdryChanges;

    eFaceCat eFC;

    // Private functions
    void vUnitNormal(const double adA[3], const double adB[3],
		     const double adC[3], double adRes[3]);
    int iOrient3D(const double adA[3], const double adB[3],
		  const double adC[3], const double adD[3]);
    int iOrient3D(iBase_EntityHandle oVertA, iBase_EntityHandle oVertB,
		  iBase_EntityHandle oVertC, iBase_EntityHandle oVertD);
    int iOrient3D(iBase_EntityHandle oTet);
    int iGenOrient3D(iBase_EntityHandle oVertA, iBase_EntityHandle oVertB,
		     iBase_EntityHandle oVertC, iBase_EntityHandle oVertD,
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
    bool qDoSwapExternal(const double adA[3], const double adB[3],
			 const double adC[3], const double adD[3],
			 const double adE[3]);
    double dEvaluateQual(const double adLocA[3], const double adLocB[3],
			 const double adLocC[3], const double adLocD[3]);
    double dEvaluateQual(iBase_EntityHandle oTet);
    double dEvaluateQual(iBase_EntityHandle oVertA, iBase_EntityHandle oVertB,
			 iBase_EntityHandle oVertC, iBase_EntityHandle oVertD);
    bool qDoSwap(const double adCoordsA[3], const double adCoordsB[3],
		 const double adCoordsC[3], const double adCoordsD[3],
		 const double adCoordsE[3]);
    bool qDoSwap(iBase_EntityHandle oVertA, iBase_EntityHandle oVertB,
		 iBase_EntityHandle oVertC, iBase_EntityHandle oVertD,
		 iBase_EntityHandle oVertE);

    // The following gets the faces opposite three verts at once, since
    // this comes up often.
    void getFacesOpposite(iBase_EntityHandle const oCell,
			  iBase_EntityHandle const aoVerts[],
			  iBase_EntityHandle aoFaces[]);
    iBase_EntityHandle getFaceOpposite(iBase_EntityHandle const oCell,
				       iBase_EntityHandle const oVert);
    iBase_EntityHandle getVertOpposite(iBase_EntityHandle const oCell,
				       iBase_EntityHandle const oFace);
    iBase_EntityHandle getCellOpposite(iBase_EntityHandle const oFace,
				       iBase_EntityHandle const oCell);

    eFaceCat eCategorizeFace(iBase_EntityHandle oFace,
			     iBase_EntityHandle aoVerts[6],
			     iBase_EntityHandle aoTets[4],
			     iBase_EntityHandle& oPivot0,
			     iBase_EntityHandle& oPivot1,
			     iBase_EntityHandle& oOther);

    int iReconfTet23(iBase_EntityHandle aoTets[],
		     iBase_EntityHandle oFace, iBase_EntityHandle aoVerts[]);
    int iReconfTet32(iBase_EntityHandle aoTets[],
		     iBase_EntityHandle oFace, iBase_EntityHandle aoVerts[]);
    int iReconfTet22(iBase_EntityHandle aoTets[],
		     iBase_EntityHandle oFace, iBase_EntityHandle aoVerts[]);
    int iReconfTet44(iBase_EntityHandle aoTets[],
		     iBase_EntityHandle oFace, iBase_EntityHandle aoVerts[]);

    int iEdgeSwap3D(iBase_EntityHandle oFace,
		    iBase_EntityHandle oVNorth,
		    iBase_EntityHandle oVSouth,
		    iBase_EntityHandle oVOther);
    int iBdryEdgeSwap3D(iBase_EntityHandle oFace,
			iBase_EntityHandle oVNorth,
			iBase_EntityHandle oVSouth,
			iBase_EntityHandle oVOther);

    iBase_EntityHandle createFace(iBase_EntityHandle oVert0,
				  iBase_EntityHandle oVert1,
				  iBase_EntityHandle oVert2);
    iBase_EntityHandle createTet (iBase_EntityHandle oFace0,
				  iBase_EntityHandle oFace1,
				  iBase_EntityHandle oFace2,
				  iBase_EntityHandle oFace3);
    iBase_EntityHandle createTetFromVerts (iBase_EntityHandle oVert0,
				  iBase_EntityHandle oVert1,
				  iBase_EntityHandle oVert2,
				  iBase_EntityHandle oVert3);

    bool qTetCheck(iBase_EntityHandle oTet);
    bool qFaceCheck(iBase_EntityHandle oFace);
    bool qHasVert(iBase_EntityHandle oEntity, iBase_EntityHandle oVert);


  public:
    /*! \brief Default constructor.
     *
     * By default, edge swapping is enabled (in addition to face
     * swapping).  Also by default, pairs of boundary faces that are
     * nearly coplanar and classified on the same geometric face can be
     * reconfigured.
     */
    Swap3D() : Swap(), qAllowEdgeSwapping(true), qAllowBdryChanges(true) {}

    virtual ~Swap3D() {}

    /*! \brief Set whether edge swapping is allowed. */
    void setEdgeSwapping (bool allow_edge_swapping)
      {qAllowEdgeSwapping = allow_edge_swapping;}

    /*! \brief Query whether edge swapping is allowed. */
    bool isEdgeSwappingAllowed()
      {return qAllowEdgeSwapping;}

    /*! \brief Set whether boundary changes are allowed. */
    void setBdryMutable (bool allow_bdry_changes)
      {qAllowBdryChanges = allow_bdry_changes;}

    /*! \brief Query whether boundary changes are allowed. */
    bool isBdryMutable()
      {return qAllowBdryChanges;}

    virtual int swap (iBase_EntitySetHandle entity_set,
		      iBase_EntityHandle entity_handle);

    virtual int swapAll (iBase_EntitySetHandle entity_set,
			 iBase_TagHandle* ptr_tag_handle = NULL);

  private:
    virtual int swap (iBase_EntityHandle entity_handle);

  };  // end class Swap3D
} // end namespace ITAPS_Swap

#endif
