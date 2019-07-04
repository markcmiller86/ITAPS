#ifndef iField_H
#define iField_H

#ifdef __cplusplus
extern "C" {
#endif
  
/** \mainpage
iField.h -- ITAPS Field Interface

Version Date: 18 November, 2010

\section INTRO Overview

The iField interface is intended to provide support for tensor fields
and collections of tensor fields, with a strong bias towards supporting
first and best the capabilities required for scientific computing
applications.

Scientific computing applications deal in physical quantities expressed
as tensors: scalars such as temperature, vectors such as velocity, and
second-order tensors such as stress. In practice, these are formally
tensor fields: a tensor field assigns a tensor to each point in a
mathematical space (typically a Euclidean space or manifold).

Because tensors have a geometric interpretation, their underlying
physical meaning is independent of the coordinate system in which they
are defined; the numerical value of a tensor depends on the coordinate
system, and so any numerical instantiation of a tensor field is
incomplete without a specification of its coordinate system.

The ITAPS Field interface represents a field as a linear combination of
distribution functions and degrees of freedom (dof's): $\sum_{i} w_{i}
f_{i} \left(\vec{x}\right)$.  For typical discrete fields, the
distribution functions $f_{i}$ have compact support over a single
highest-dimensional mesh entity, and are each more or less associated
with a mesh entity, which may be of lower dimension; for instance, the
interpolation functions used in the finite element method may be
associated with a vertex, edge, face, or region. These distribution
functions are known in advance. The degrees of freedom $w_{i}$ are
associated with both an entity and a distribution function; calculation
of these degrees of freedom is the goal of a typical scientific computing
application.

An ITAPS Field is, for practical purposes, a function. The
representation of the function will, in most use cases, be piecewise and
will not necessarily by continuous (never mind smooth), but with
reasonable definitions in a few tight spots, a Field is in fact a
function.

In addition to supporting physical tensor fields, there's a reasonable
argument for allowing integer or boolean values for fields.  While these
don't represent physical tensors, and need to be restricted to scalars
(coordinate transforms for vectors of booleans, anyone?), applications
do and will continue to want to use such things.  This case implies
certain restrictions on distribution functions for integer and boolean
scalar "fields" and on the operations one can perform on them.

To support this representation of Fields and the sorts of operations on
Fields that applications require, we need precise definitions of the
domain of a function, of the data that a Field represents, and of how we
relate the domain and range for a Field.

\section ADM Abstract Data Model

\subsection DOM Domain

\subsubsection Preliminary Definitions

Primitive SET (pSET) An infinite point set corresponding, topologically,
  to an iMesh or iGeom Entity.

  Examples:

    iMesh_TRIANGLE
    iMesh_HEXAHEDRON

The key feature of a pSET is its topology and the adjacency information
implied by, for instance, knowing the identities of the faces, edges,
and vertices on the closure of a region.  Geometric information
(locations of vertices, shapes of higher-dimensional entities) is a
separate matter.

Pre-Domain: The union of a collection of primitive point sets; that is,
  an iMesh or iGeom EntitySet.

A pre-domain is a topological description of the domain; specifically,
it has no coordinates associated with it to define the spatial location
of iMesh and iGeom Entities. It represents an infinite point set on
which the field will be defined, decomposed into iMesh/iGeom primitives
(pSETs) with adjacency information to knit those pSETs together to cover
the infinite point set.



Domain (mesh based)

The domain of a field is expressed, at the lowest level, by a collection
of mesh entities over which the field is defined.  This collection can
potentially be expressed in multiple ways: as the entities in an iMesh
entity set (in many use cases, the root set) or as the mesh entities
related (by iRel) to one or more iGeom entities.

Loose coupling between mesh topology and geometry is a benefit in
several cases --- including mesh smoothing/relaxation, shape
optimization, and deforming domain problems.  This suggests that some
cooperation between iMesh and iField is ideal for handling
coordinates, with some cases handled strictly through iMesh, others
strictly through iField, and some jointly.

For practical reasons, the iMesh specification defines one very
important field; the coordinate field. In addition, the iMesh
specification defines only the most common coordinate field
representation; a piecewise linear, Lagrange field. Support for more
sophisticated coordinate fields will be possible but only through
iField, not through iMesh's 'native' coordinate field representation.

When a new mesh is instantiated, an iMesh client will indicate if the
mesh will employ iMesh's 'native' coordinate field representation or not
via a new boolean argument to iMesh_newMesh()

    **\brief  Construct a new iMesh instance
    *
    * Construct a new iMesh instance, using implementation-specific
    * options
    * \param options Pointer to implementation-specific options string
    * \param lagrange_coords flag indicating if iMesh' lagrange coordinates will be used
    * \param instance Pointer to iMesh instance handle returned from function
    * \param *err Pointer to error type returned from function
    * \param options_len Length of the character string pointed to by options
    *
  void iMesh_newMesh(const char *options,
                     int lagrange_coords,
                     iMesh_Instance *instance,
                     int *err,
                     int options_len);

If 'int lagrange_coords' is zero (false)...

        that means the coordinate field for the mesh will be handled
        through iField. In this case, the iMesh implementation is free
        to do whatever it wishes with coordinate values passed via
        createVtx calls including ignoring that data entirely and not
        even storing it. Calls to either iMesh_getVtxArrCoords or
        iMesh_getVtxCoord will return no coordinate data and set the err
        return to iBase_NO_LAGANGE_COORDS. Calls to
        iMesh_setVtxArrCoords and iMesh_setVtxCoord() will make no
        attempt to interpret coordinate data passed in and will likewise
        return err set to iBase_NO_LAGANGE_COORDS. Calls to
        iMesh_createVtxArr and iMesh_createVtx will make no attempt to
        interepret coordinate data passed in (e.g. will not attempt to
        de-reference pointers to coordinate values), but will return
        iBase_SUCCESS.
        
        Note, even if iField is later used to define a coordinate field
        that is in fact a piecewise-linear, lagrange coordinate field
        for the mesh, there will be no obligation that that fields dofs
        make its way into the iMesh mesh instance such that it be
        managed by iMesh as it otherwise ordinarily would have.

Otherwise, 'int lagrange_coords' is non-zero (true) and...

        that means the coordinate field for the mesh will be handled
        BOTH through iMesh AND through iField. By 'BOTH', here that does
        not mean that data is redundantly stored. It only means that it
        is redundantly accessible through set/getVtx methods in iMesh as
        well as set/getDof methods in iField.  For some implementations,
        this will require a different access/storage method for this
        particular type of coordinate field than for other fields.



\subsection TENSOR Representing Tensor Data

Tensor Templates

A tensor template is intended to define all of the information about a
physical tensor that is dependent only on the physical quantity being
represented.  For instance, a 3D velocity vector is always a 3-vector of
real numbers and represents a length/time.  A 2D stress tensor is always
a 2nd-order tensor (2x2) of real numbers and represents a force/area
(mass-length/(time^2 length^2)).  Electric field is a 3D vector of real
numbers and represents a force/unit charge.  These statements will be
true for any 3D velocity or 2D stress or 3D electric field, regardless
of system of units, coordinate system, precision of stored values, etc.

Important things to know about a tensor template

  Tensor order (integer) 0 for scalars, 1 for vectors, 2 for
  tensors mapping one vector to another, etc.

  Number of tensor components (integer) Is this a 2-vector, a
  3-vector, etc.

  Algebraic type (enumeration) This describes the rules for
  combining values and/or the range of legal values. For
  instance, real numbers, complex numbers, barycentric coordinate
  vectors, and quaternions all have different algebraic rules.

  Quantity (handle) See definition later in this document.

\subsection DFUNC Distribution Functions on Meshes

At this stage, the focus is on making the easy things easy. The easiest
case for distribution functions on meshes is the case where each entity
has the same set of distribution functions; that is, the case in which
the field representation is the same for every highest (topological)
dimensional entity.  Note that this does not preclude things like linear
elements for pressure and quadratic for velocity (these are, after all,
different fields and can have different dfuncs).  Nor does this preclude
things like edge-based elements: these split a typical shape function
over multiple regions (as do other continuous FE methods).  Note also
that the definition refers to the highest TOPOLOGICAL dimension entity:
this implies that a field can be defined over a collection of faces
(say, all or part of a boundary), with dfuncs defined over the faces.

Distribution functions and their derivatives will be hard-coded in a
distribution function kernel (DFK), which will contain all information
required to evaluate all distribution functions (and their gradients,
etc) for that highest-dimensional entity (HDE).  In practice, this takes
the form of a callback from the implementation to some external
distribution function kernel, which will evaluate a specified dfunc (one
of perhaps many for the HDE) as a specified location in a specified
entity.

In this scenario, each distribution function (dfunc) has support over
exactly one highest (topological) dimensional entity; one or more
distribution functions (dfuncs) can be have the same highest-dimensional
entity as their support. Each distribution function has exactly one
degree of freedom (dof) associated with it per tensor component. The
dfunc and dof may be associated directly with a vertex, edge, or face
instead of a region (think high-order FE). The dofs and dfuncs must have
a common ordering, dictated by the DFK and understood by the
application.  The implementation needs only to know how to call the DFK
callback and sum dof*dfunc value at a point.

As a practical matter, it is obviously very important to know how to
convert global coordinates into local coordinates and vice versa, as the
dfuncs will necessarily have to use local coordinates. Obvious choices
like barycentric and shifted cartesian coordinates could be specified in
an enum, but the current API anticipates that there will be a wide and
weird enough range of local coordinate systems that it makes sense for
the DFK to include functions for coordinate conversion in both
directions, as well as the Jacobian of the transformation.

The More General Case

More generally, it's often desirable to define distribution functions
that are not associated with highest-dimensional entities, or that are
have a support that extends beyond a single HDE.  Examples of this include
vertex-centered finite-volume schemes (dfunc associated with a vertex,
with support over parts of all regions incident on that vertex),
spectral-like methods, and filtering or convolution kernels.  Although
details have not yet been worked out, the interface for supporting these
types of DFK's is likely to bear some syntactic and semantic resemblence
to the iMeshP ghost descriptions.

Examples (all dof counts for tets at this point...)

  Galerkin FE (quadratic)
     One dof/vertex, one dof/edge, zero dof/face, zero dof/region.
     Local coordinates: barycentric.
     Code for 10 dfuncs (standard shape functions) must be provided.

  Discontinuous Galerkin (cubic)
     Zero dof/vertex, zero dof/edge, zero dof/face, twenty dof/region.
     Local coordinates: barycentric.
     Code for 20 dfuncs (standard shape functions) must be provided.

  Cell-centered FV (quadratic)
     Zero dof/vertex, zero dof/edge, zero dof/face, 10 dof/region.
     Local coordinates: cartesian w/ origin at centroid.
     Code for 10 dfuncs (monomials) must be provided.

  Vertex-centered FV (quadratic)
     10 dof/vertex, zero dof/edge, zero dof/face, zero dof/region.
     Local coordinates: cartesian w/ origin at vertex(?), plus a filter
       function that selects which vertex-centered control volume that
       covers part of a region the point actually falls into (filter
       function uses local barycentric coords).
     Code for 10 dfuncs (monomials) must be provided.
     Note: this is a bit of a hack; in practice most field evals that a
       solver does are already pre-identified with a CV.  This is a case
       that cries out for a more sophisticated approach to defining
       support of dfuncs.

     
\subsection Tensor Field

A tensor field is an intermediate step in instantiating a tensor
template: at this level, units, data precision, a type for physical
coordinates, a distribution function kernel and a domain are added.

  Units (handle) See definition later in this document.

  Coordinate type (enum): Possible values include cartesian,
  cylindrical, and spherical. This is critical for tensor
  transformations.

  DFK   See above.

  Data precision.  This identifies whether floating point data is stored
  in single, double, or quad precision.  This enumeration may need to be
  expanded.

  Domain


In addition to fields that describe single physical tensors, it is often
convenient in scientific computing to have multiple tensor fields stored
and accessed together.  For instance, in incompressible flow, a vector
velocity and scalar pressure are both required, and often stored
interleaved (uvwPuvwP...) to improve cache hit rates.  To support this
common usage --- including making it relatively easy for implementations
to store variables together --- the iField API includes a function to
create a collection of tensor fields together as well as an overall
field that acts as a container for these multiple tensors.  In a
possible abuse of the data model, these compound fields use the same
data type as regular tensor fields; implementations are required to be
able to distinguish between the two.  Some queries for a tensor field
can not be reasonably answered for a compound field; these cases are
noted in the API spec.

Glossary

Coordinate system How space is laid out and measured. A
  coordinate system may be local or global, and either may be
  physical or parametric. Common examples will surely include
  cartesian, polar, spherical, and barycentric coordinates.
  User-defined coordinate systems can be infinite in variety,
  obviously. Each coordinate field will be associated with a
  coordinate system; a coordinate system can apply to many
  coordinate fields. More on this when coordinate fields come
  into the interface in earnest.

  Key functionality: conversion to/from cartesian, plus producing
  the metric and Jacobian info required for differentiation and
  integration.

Quantity Measurable physical property of phenomena/bodies/substances.
	 
    Examples: mass, length, time, acceleration, volume, power,
      force, illuminance

  Any Quantity can be expressed in terms of the seven basic physical
  quantities, corresponding to the seven base units of the SI system:
  mass, length, time, temperature, electric charge, luminous intensity
  and amount of substance. A Quantity has the following data

  (required) int[7] num_powers; Powers of 7 basic quantities in
    numerator

  (required) int[7] den_powers; Powers of 7 basic quantities in
    denominator

  (required) int flags; bool bits to indicate e.g.
    intensive/extensive, conserved, etc.

  (optional) char *name; name of quantity

  (optional) char *abbr; abbreviation of quantity.

  Operations: 7 basic quantities are pre-defined and guaranteed
  to always exist. New, derived, quantities are defined by
  multiplying together old ones.

  Notes:

  It useful to distinguish a quantity representing a mass
    fraction (e.g. mass/mass) from a quantity representing a
    volume fraction (e.g. volume/volume) for example.

  Essentially there are two kinds of quantities; base quantities
  and derived quantities. Base quantities are, by necessity,
  defined by the data model specification itself and are
  guaranteed to exist. Derived quantities shall be definable by
  applications as necessary. See
  physics.nist.gov/cuu/Units/units.html for detailed discussion.

  Unit An adopted convention for assigning a numerical value to a
  Quantity. A Unit has

  (required) handle quant; the Quantity the unit is a measure for

  (required) double scale; scale of this unit relative to the
    default unit of measure for quant.

  (optional) double offset; likewise for offset units

  (optional) double logbase; likewise for log-related units

  (optional) double logcoef; likewise for log-related units

  (optional) char *name;

  (optional) char *abbr;

  Examples:

  slug is a unit of measure of the quantity mass

  pound is a unit of measure of the quantity force

  minute is a unit of measure of the quantity time

  meters/second is a unit of measure of the quantity velocity

  furlongs/fortnight is a unit of measure of the quantity
    velocity

  decibels is a (log scaled) unit of measure of the quantity
    power.

  degrees Fahrenheit is a (offset) unit of measure of the
    quantity thermodynamic temperature.

  Operations:

  Associate a Unit (of measure) with a Quantity

  Combine Unit A and Unit B producing new Unit C by scale factor
    and optional power.

  Set log and/or offset scaling of Unit A relative to Unit B

  Unit System A collection of quantities together with default
  units for them. A Unit System has

  (required) handle bunits[7]; default units for 7 base
    quantities.

  (optional) char *name; name of this unit system

  (optional) handle dunits*; default units for any derived
    quantities that are NOT deriveable from bunits.

  Examples:

  International System of Units (SI) is an example of a system of
    units where default units for base quantities are meters,
    kilograms, seconds, amperes, kelvin, mole, candela. From
    these, derived units for velocity, for example,
    meters/second, current density, amperes/meter^2, etc.

  A and B Divisions at LLNL use default units such as 10
    nanoseconds for time, centimeters for length, etc.

    Notes: If in a given iMesh/iField instance we ever wind up
    mixing field objects created by different applications, then
    it is very likely going to be necessary to be able to
    explicitly set units for any given field (e.g. use something
    other than the default determined by some Unit System).
    This will be especially true if in any given iMesh/iField
    instance we support only one 'active' Unit System.

  It might be nice to allow applications to refer to systems of
  units by name rather than having to create them from scratch as
  needed.

*/
#include "iBase.h"
#include "iMesh.h"
#include "iGeom.h"
#include "iRel.h"
    
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*             Definitions needed in iField                               */
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/** Precision of field data */
enum iField_Precision
/* MCM: 'Precision' is potentially confusing here, particularly since this
list is not either all floating point formats or all integer formats. Shouldn't
this list really come out of iBase anyways and/or shouldn't we extend iBase
to include types needed by iField? */
  {
    iField_BOOLEAN, /* MCM: Is 'boolean' same as 'bit' ? */
    iField_UCHAR,   /* MCM: do we need, maybe for opaque types
                            but what can iField do with that? */
    iField_INTEGER, /* MCM: Do we need C9x types, int16, int32, int64 */
    iField_FLOAT,
    iField_DOUBLE,
    iField_QUAD
  };

/** Algebraic type of field data */
/* MCM: The notion expressed here seems to be a kind of hybrid of the data type
   as well as operations it supports. None of these are really algebraic types
   in the sense that I introduced that term for this API. 

   Though I see the possible value in distinguishing between dofs obeying
   boolean algebra semantics and those obeying say complex number semantics,
   that isn't what I had originally introduced the notion of algebraic type
   to support.

   At the same time, we're missing entirely the notion of vector valued and
   tensor valued fields here as well as 'scalar' which all the examples below
   are really instances of certain classes of algebraic types of 'scalar'.

   If it was just a matter of distinguishing between scalar, vector and tensor
   type things, we'd simply declare all fields to be 'tensor valued' and just
   specify the tensor order (which we do anyways). But, that then misses the
   nuances of integral valued vs. real valued vs. complex valued (what about
   integral-complex? digital signal processing codes have to deal with this
   issue). 

   Also, for something like 'complex', that involves 2 things, not one. And,
   those things could be real-imaginary pairs (cartesian) or magnitude-phase
   pairs (polar). In other words, there is a tiny little 'coordinate system'
   that defines how values of the field (thats the range set of the domain-range
   pair of sets needed to define a function) are interpreted. For a quaternion,
   there would be 4 things. I mention quaternion only to help in understanding
   how to properly generalize and abstract this. Even for a single thing (number),
   there is a difference between just that number and that number multiplying
   a vector, say the unit X direction vector. One is a scalar, the other is a
   vector even though both involve only a single number, the magnitude in the X
   direction. In other words, 'a' is different from 'a multiplying x-hat'. The
   latter is what you'd call the 'component' of some vector field and that is
   qualitatively different from something that is truly a scalar value'd field.

   The issue with whether or not the values of a field take on only discrete
   (integral or boolean even) values is a statement about the range set
   (of the domain<->range pair that we're defining a function between. It says
   whether or not the set is 'continuous' or 'discrete' as in the difference
   between the real number line and the integers.

   I think this best way to handle these notions is a slightly more complex API
   for defining algebraic types similar to the DFK api already being developed.
*/

/* CFOG reply:
   Mark, I don't see (at least yet) why the combination of tensor order,
   precision, and algebraic type doesn't cover the cases you discuss.  I
   agree that there may be some overlap.  Would be be better off keeping
   precision (bool, int, float, double, quad, ???) and having this
   variable tell us only real/complex/??? ?  I think I'd prefer not to
   roll the two together (nearly doubling the number of precisions).  In
   any event, all three of the pieces of info in my first sentence are
   going to be needed to decide on storage. 

   Before we go making this a user-extensible kind of thing, I'd
   definitely want to see real, concrete use cases that aren't covered
   by something simple.
*/
enum iField_AlgType
  {
    iField_LOGICAL,
    iField_INTEGRAL,
    iField_REAL,
    iField_COMPLEX
  };

/** \brief Define possible coordinate systems for a Tensor
 *
 * These should perhaps be handled in the same way as dfuncs?
 * If so, candidate functions include two-way conversion and Jacobians.
 *
 * At the least, we probably need to come up with some way to support
 * global parametric coordinates.
 *
 */
enum iField_CoordType
  {
    iField_CARTESIAN,
    iField_CYLINDRICAL, /* Includes 2D polar */
    iField_SPHERICAL
    /* etc */
  };

/** \brief Define possible storage hints for a tensor
 *
 * Depending on application context, the optimal way to store field data
 * may differ.  For instance, a flow solver may prefer dofs for
 * each entity to be stored contiguously (in an array of all dofs) for
 * efficient access while an adaptation scheme may prefer dofs to be
 * associated directly with entities so that dofs can be created and
 * destroyed with entities.
 *
 * The application can provide suggestions to an implementation about
 * what dof storage pattern is likely to be advantageous using the
 * values enumerated here.  An implementation may or may not choose to
 * take advantage of the hint.  Either way, the implementation is still
 * responsible for correctly responding to all iField queries.
 *
 * Values of the enumeration are currently iField_BLOCKED,
 * iField_INTERLEAVED, iField_MIXED, and iField_PER_ENTITY. 
 *
 * Block implies that a scalar temperature (T) and velocity (u,v,w) in a
 * compound field are desired to be stored as: 
 *    (TTTTTT...uuuuuu...vvvvvv...wwwwww...).
 * For a simple velocity field, omit the T's in this example.
 *
 * Interleaved implies that a scalar temperature (T) and velocity
 * (u,v,w) in a compound field are desired to be stored as: 
 *    (TuvwTuvwTuvwTuvwTuvwTuvw...).
 * For a simple velocity field, omit the T's in this example.
 *
 * Mixed implies that a scalar temperature (T) and velocity (u,v,w) in a
 * compound field are desired to be stored as: 
 *     (TTTTTT...uvwuvwuvwuvwuvwuvw...).
 * For simple fields, this option is not allowed.
 *
 * Per entity implies that the app wants data associated with entities
 * to support mesh modification / adaptation.  (Yes, these can be
 * supported using array-based storage, but it's harder that way.)
 *
 * MCM or FD: We can't support the case where a caller has 3 separate
 * arrays one for X, one for Y and one for Z. That seems like way too
 * common a case to NOT handle. In fact, for many rank=1 or 2 tensor
 * fields, this is a real common storage paradigm. All we really need to
 * do to handle this case is add iField_SEPARATE.
 *
 * CFOG reply:  For data stored internally in the iField implementation,
 * how does iField_SEPARATE differ functionally from iField_BLOCKED?
 * Whether I use one array or separate arrays, I'm pretty sure that
 * minor pointer magic in the implementation will result in identical
 * machine code.
 *
 */
enum iField_StorageHint
  {
    iField_BLOCKED,
    iField_INTERLEAVED,
    iField_MIXED,
    iField_PER_ENTITY
    /* etc? */
  };
  
/** Definition of new types for iField. */
typedef void* iField_Instance;
typedef struct iField_DomainHandle_Private* iField_DomainHandle;
typedef struct iField_TensorHandle_Private* iField_TensorHandle;
typedef struct iField_TensorTemplateHandle_Private* iField_TensorTemplateHandle;
typedef struct iField_QuantHandle_Private* iField_QuantHandle;
typedef struct iField_UnitsHandle_Private* iField_UnitsHandle;
typedef struct iField_Handle_Private* iField_Handle;

/** \brief Create an iField instance.
 *
 * In typical usage, the fields associated with a single iMesh instance
 * (and its various subsets) will be part of the same Field instance.
 *
 * Question: do we want to enforce this typical usage by requiring an
 * iMesh instance as an argument in this constructor?  If so, do we also
 * want to pass an iRel instance so that that connection will be
 * mediated "properly" through iRel?  Or should we leave that up to the
 * application to handle properly?
 *
 * \param[in]  options     Implementation-specific options.
 * \param[out] instance    New interface instance
 * \param[out] err         Error code
 * \param[in]  options_len Length of the option string
 */
  void iField_create(const char *options,
		     iField_Instance *instance,
		     int *err,
		     const int options_len);

/** \brief Destroy a field interface object
 *
 * Naturally, this call also destroys all field data stored in the
 * interface object.
 *
 * \param[in]  instance     Field interface instance to destroy.
 * \param[out] err          Error code
 */
  void iField_destroy(iField_Instance instance,
		      int *err);

  /**\brief  Get a description of the error returned from the last iField function
   *
   * Get a description of the error returned from the last iField
   * function. Space must be allocated by the caller.
   *
   * \param[in]    instance    iField instance handle
   * \param[inout] descr       Pointer to a character string to be
   *                           filled with a description of the error
   *                           from the last iField function 
   * \param[out]   *err        Pointer to error type returned from this function
   * \param[in]    descr_len   Length of the character string pointed to
   *                           by descr
   */
  void iField_getDescription(iField_Instance instance,
			     char *descr, 
			     int *err, 
			     int descr_len);

/** \brief Create a tensor template; return its handle.
 *
 * Create a tensor template.  Once a tensor template is set up, its
 * basic data can be read, but not written.  So, for example, if you
 * want to change from a 2-vector to a 3-vector, you need to have
 * another tensor template for that tensor.
 *
 * \param[in]  instance          Field interface instance
 * \param[in]  order             Tensor order: 0 for scalars, 1 for
 *                                 vectors, etc.
 * \param[in]  num_comp          Number of components: a 2-vector or a
 *                                 3-vector, for instance.
 * \param[in]  alg_type          Is this data real, complex, etc?
 *                                 (enum)
 * \param[in]  quant_handle      Physical quantity this tensor represents. 
 * \param[out] handle            The newly created tensor template.
 * \param[out] err               Error code.
 */
void iField_createTensorTemplate(const iField_Instance instance,
				 int order, int num_comp,
				 int alg_type,
				 iField_QuantHandle quant_handle,
				 iField_TensorTemplateHandle* handle,
				 int *err);

/** \brief Retrieve the order of this tensor template.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  handle            The tensor template being queried.
 * \param[Out] order             The order of this tensor template.
 * \param[Out] err               Error code.
 */
void iField_getTensorTemplateOrder(const iField_Instance instance,
				   iField_TensorTemplateHandle handle,
				   int *order,
				   int *err);

/** \brief Retrieve the number of components for this tensor template.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  handle            The tensor template being queried.
 * \param[Out] num_comp          # of components of this tensor template.
 * \param[Out] err               Error code.
 */
void iField_getTensorTemplateNumComponents(const iField_Instance instance,
					   iField_TensorTemplateHandle handle,
					   int *num_comp,
					   int *err);
  
/** \brief Retrieve the algebraic type of this tensor template.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  handle            The tensor template being queried.
 * \param[Out] alg_type          The algebraic type of this tensor template.
 * \param[Out] err               Error code.
 */
void iField_getTensorTemplateAlgType(const iField_Instance instance,
				     iField_TensorTemplateHandle handle,
				     int *alg_type,
				     int *err);
  
/** \brief Retrieve the physical quantity this tensor template represents.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  handle            The tensor template being queried.
 * \param[Out] order             The order of this tensor template.
 * \param[Out] err               Error code.
 */
void iField_getTensorTemplateQuantity(const iField_Instance instance,
				      iField_TensorTemplateHandle handle,
				      iField_QuantHandle* quant_handle,
				      int *err);
  
/** \brief Destroy a tensor template.
 *
 * This should refuse to destroy if the template is in use.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  handle            Tensor template to destroy.
 * \param[Out] err               Error code.
 */
void iField_destroyTensorTemplate(const iField_Instance instance,
				  iField_TensorTemplateHandle handle,
				  int *err);
  
/** \brief Create a tensor field from a template, distribution
 *  functions, units, and domain.
 *
 * \param[in]  instance         Field interface instance
 * \param[In]  domain_handle    The domain this field is defined on.
 * \param[In]  precision        Is this data single precision, double
 *                                 precision, etc?  (enum)
 * \param[In]  dfunc_kernal     A function callback for converting
 *                              spatial coordinates into the value for a
 *                              distribution function.
 * \param[In]  tt_handle        The tensor template specifies whether
 *                              this is, for instance, a velocity, or a
 *                              temperature, or a stress.
 * \param[In]  units            SI, cgs, slugs-ft-sec, or user-defined.
 * \param[In]  name             A string giving a name for this tensor.
 * \param[Out] tensor_handle    The new tensor field.
 * \param[Out] err              Error code.
 * \param[In]  name_len         Length of name string.
 */
void iField_createTensorField(const iField_Instance instance,
			      iField_DomainHandle domain_handle,
			      int precision,
			      iField_DFuncKernel dfunc_kernel,
			      iField_TensorTemplateHandle tt_handle,
			      iField_UnitsHandle units_handle,
			      const char* name,
			      iField_TensorHandle *handle,
			      int *err,
			      const int name_len);

/** \brief Create multiple tensor fields from a template, distribution
 *  functions, units, and coordinate system.
 *
 *  The concept this multi-field create method provides are...
 *    1. hint to implementation of storing them together,
 *    2. possible convenience for caller to set all dofs together
 *    3. storage is predictable/computable because all fields obey
 *    properties that make this possible. This also has implications in
 *    array-based methods of getting/setting dofs as well.
 *
 *  Have to look into Fortran size requirements here!
 *
 * Restrictions:
 *  1. The resulting component fields can be evaluated and
 *  set/getDOF'ed individually, but can't be destroyed individually.
 *  2. All fields must use the same dfunc and data storage precision.
 *  3. Units need not be consistent between fields, as they're each
 *  defined separately.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  domain_handle     The domain this field is defined on.
 * \param[In]  precision         Is this data single precision, double
 *                                 precision, etc?  (enum)
 * \param[In]  dfunc_kernal      A function callback for converting
 *                               spatial coordinates into the value for a
 *                               distribution function.
 * \param[In]  tt_handles        One tensor template for each tensor to
 *                                 be created.
 * \param[In]  units_handles     Units for each tensor to be created.
 * \param[In]  names             An array of strings giving names of
 *                                 tensors (one more name than
 *                                 templates, so that the compound will
 *                                 also have a name).
 * \param[In]  num_tensors       How many new tensors?
 * \param[out] compound_handle   A separate handle for the collection of
 *                                 tensors. 
 * \param[InOut] tensor_handles  The new tensor fields.
 * \param[InOut] tensor_handles_alloc  Allocated size of tensor handle array.
 * \param[Out] tensor_handles_size  Number of tensors created and returned.
 * \param[Out] err               Error code.
 * \param[In]  name_len          Lengths of name strings. [CFOG: passing
 *                                 arrays of strings to/from Fortran is
 *                                 going to be different than single
 *                                 strings, according to my experiments
 *                                 so far.]
 */ 
void iField_createTensorFields(const iField_Instance instance,
			       iField_DomainHandle domain_handle,
			       int precision,
			       iField_DFuncKernel dfunc_kernel,
			       iField_TensorTemplateHandle *tt_handles,
			       iField_UnitsHandle *units_handles,
			       char** names,
			       const int num_tensors,
			       iField_TensorHandle *compound_handle,
			       iField_TensorHandle **tensor_handles,
			       int *tensor_handles_allocated,
			       int *tensor_handles_size,
			       int *err,
			       const int *name_len);

/** \brief Indicate that the given field should use tag data stored in
 *   the given taghandle.
 *
 * This function explicitly assumes that the tag data will be used as
 * the field DOFs; any existing DOFs will be destroyed by this call in
 * favor of retaining the values stored in the tag.
 *
 * iField set/get dofs will have the same effect as iMesh set/get tags.
 *
 * A default value is provided so that any entities that should have
 * DOFs but don't can be assigned values.  This value -must- be set to
 * tags (instead of held back as a default return value from getDOFs) so
 * that getTag will properly return the field dofs.
 *
 * A (simple or compound) field that is stored as a tag can not respond
 * to suggestions for storage.
 *
 * All floating point field types will be converted to double precision
 * by this call, including a change in the field precision.
 *
 * Floating point fields must attach to a tag of type double; integer
 * fields must attach to a tag of type int.
 *
 * Note that the use of tag data as field DOFs requires that all entities
 * with DOFs have the same number of DOFs.  That is, linear or quadratic
 * Lagrange interpolation (including p-refinement combining these two)
 * is fine.  Cubic Lagrange is not, because a vertex and an edge will
 * both have DOFs but different numbers of them, which the tag mechanism
 * can't accommodate in a single tag handle.  Usage contrary to this
 * will produce an error without affecting current field dofs or
 * attaching the field to tag data.
 *
 * Restrictions:
 *  This function can be used for simple tensor fields or compound
 *  fields, but not for individual tensor fields that are part of a
 *  compound field.
 *
 * \param[in]  instance         Field interface instance
 * \param[In]  field            Field to attach to tag data.
 * \param[In]  mesh_instance    iMesh instance owning the tag; must be
 *                                consistent with the field's domain.
 * \param[In]  tag_handle       iMesh tag handle containing data.
 * \param[In]  untagged_values  Value(s) to use for entities not yet tagged.
 *                                This is a pointer because not all data
 *                                is  scalar.
 * \param[Out] err              Error code.
 */
  void iField_attachToTagData(const iField_Instance instance,
			      iField_TensorHandle field,
			      iMesh_Instance mesh_instance,
			      iBase_TagHandle tag_handle,
			      double *untagged_values,
			      int *error);

/** \brief Indicate that the given field should use an external,
 *   application-created array to store DOF data.
 *
 * This function explicitly assumes that the external array data will be
 * used as the field DOFs; any existing DOFs will be destroyed by this
 * call in favor of retaining the values stored in the array. 
 *
 * A (simple or compound) field that is stored as an array can not respond
 * to suggestions for per-entity storage; changes between block,
 * interleaved, and mixed are permitted.
 *
 * Note that the use of array data as field DOFs requires that all
 * entities with DOFs have the same number of DOFs.  That is, linear or
 * quadratic Lagrange interpolation (including p-refinement combining
 * these two) is fine.  Cubic Lagrange is not, because a vertex and an
 * edge will both have DOFs but different numbers of them, which the tag
 * mechanism can't accommodate in a single tag handle.  Usage contrary
 * to this will produce an error without affecting current field dofs or
 * attaching the field to array data.
 *
 * Restrictions:
 *  This function can be used for simple tensor fields or compound
 *  fields, but not for individual tensor fields that are part of a
 *  compound field.
 *
 * \param[in]  instance         Field interface instance
 * \param[In]  field            Field to attach to array data.
 * \param[In]  dof_array        Pointer to data array
 * \param[In]  storage_order    Initial storage layout of the array
 *                                (blocked, interleaved or mixed).
 * \param[Out] err              Error code.
 */
  void iField_attachToArrayData(const iField_Instance instance,
				iField_TensorHandle field,
				double* dof_array,
				int storage_order,
				int *error);

/** \brief Give the implementation a hint about how to efficiently
 *   store data for this field.
 *
 * An implementation may or may not choose to take advantage of the hint
 * give here.  Either way, the implementation is still responsible for
 * correctly responding to all iField queries.
 *
 * See above for description of the possible values of hint.
 *
 * \param[in]  instance  Field interface instance
 * \param[In]  field     Field for which app is suggesting storage layout (ssl).
 * \param[In]  hint      An enumerated value for the ssl.
 * \param[out] err       Error code.
 */
  void iField_suggestStorage(const iField_Instance instance,
			     iField_TensorHandle field,
			     iField_StorageHint hint,
			     int *err);
  
/** \brief Return the values of all dfuncs at a given point.
 *
 * For a tensor_handle (and therefore distribution function kernel),
 * find all distribution function values at a particular 
 * location.  This function provides a way to get the results of
 * multiple calls to an iField_DFuncKernel in one API call.
 * dfunc_values* is the usual ITAPS array syntax.
 *
 * \param[in]    instance        Field interface instance
 * \param[In]    tensor_handle   Tensor handle to act on.
 * \param[In]    entity_handle   Entity in which dfuncs will be
 *                                 evaluated.
 * \param[In]    coords          Coordinates at which to evaluate
 *                                 dfunc.
 * \param[in]    is_param        True if coords are dfunc parametric
 *                                 coords; false for tensor physical
 *                                 coord system. 
 * \param[InOut] dfunc_values    Data is returned here.
 * \param[InOut] dfunc_values_allocated  Allocated size of data array.
 * \param[Out]   dfunc_values_size  Used size of data array.
 * \param[Out]   err             Error code.
 */ 
  void iField_evaluateAllDF(const iField_Instance instance,
			    const iField_TensorHandle tensor_handle,
			    const iBase_EntityHandle entity_handle,
			    const double coords[3], const int is_param,
			    double **dfunc_values, int *dfunc_values_allocated,
			    int *dfunc_values_size, int *err);
  
/** \brief Return the gradients of all dfuncs at a given point.
 *
 * For a tensor_handle (and therefore distribution function kernel),
 * find all distribution function gradients at a particular parametric
 * location.  This function provides a way to get the results of
 * multiple calls to an iField_DFuncKernel in one API call.
 * dfunc_values* is the usual ITAPS array syntax.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  tensor_handle      Tensor handle to act on.
 * \param[In]  entity_handle      Entity in which dfuncs will be
 *                                evaluated.
 * \param[In]  coords             Coordinates at which to evaluate
 *                                dfunc.
 * \param[In]  is_param           True if coords are parametric; false
 *                                for tensor physical coord system.
 * \param[In]  storage_order      Should data be blocked or interleaved?
 * \param[InOut]  dfunc_values      Data is returned here.
 * \param[InOut]  dfunc_values_allocated  Allocated size of data array.
 * \param[Out]  dfunc_values_size  Used size of data array.
 * \param[InOut]  dfunc_grad_values      Data is returned here.
 * \param[InOut]  dfunc_grad_values_allocated  Allocated size of data array.
 * \param[Out]  dfunc_grad_values_size  Used size of data array.
 * \param[Out]  err               Error code.
 */ 
  void iField_evaluateGradAllDF(const iField_Instance instance,
				const iField_TensorHandle tensor_handle,
				const iBase_EntityHandle entity_handle,
				const double coords[3], const int is_param,
				const iBase_StorageOrder storage_order,
				double **dfunc_values,
				int *dfunc_values_allocated,
				int *dfunc_values_size, 
				double **dfunc_grad_values,
				int *dfunc_grad_values_allocated,
				int *dfunc_grad_values_size, int *err);

/** \brief Return the hessians of all dfuncs at a given point.
 *
 * For a tensor_handle (and therefore distribution function kernel),
 * find all distribution function hessians at a particular parametric
 * location.  This function provides a way to get the results of
 * multiple calls to an iField_DFuncKernel in one API call.
 * dfunc_values* is the usual ITAPS array syntax.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  tensor_handle      Tensor handle to act on.
 * \param[In]  entity_handle      Entity in which dfuncs will be
 *                                evaluated.
 * \param[In]  coords             Coordinates at which to evaluate
 *                                dfunc.
 * \param[in]  is_param          True if coords are parametric; false
 *                                for tensor physical coord system.
 * \param[In]  storage_order      Should data be blocked or interleaved?
 * \param[InOut]  dfunc_values      Data is returned here.
 * \param[InOut]  dfunc_values_allocated  Allocated size of data array.
 * \param[Out]  dfunc_values_size  Used size of data array.
 * \param[InOut]  dfunc_grad_values      Data is returned here.
 * \param[InOut]  dfunc_grad_values_allocated  Allocated size of data array.
 * \param[Out]  dfunc_grad_values_size  Used size of data array.
 * \param[InOut]  dfunc_hess_values      Data is returned here.
 * \param[InOut]  dfunc_hess_values_allocated  Allocated size of data array.
 * \param[Out]  dfunc_hess_values_size  Used size of data array.
 * \param[Out]  err               Error code.
 */ 
  void iField_evaluateHessianAllDF(const iField_Instance instance,
				   const iField_TensorHandle tensor_handle,
				   const iBase_EntityHandle entity_handle,
				   const double coords[3], const int is_param,
				   const iBase_StorageOrder storage_order,
				   double **dfunc_values,
				   int *dfunc_values_allocated,
				   int *dfunc_values_size, 
				   double **dfunc_grad_values,
				   int *dfunc_grad_values_allocated,
				   int *dfunc_grad_values_size,
				   double **dfunc_hess_values,
				   int *dfunc_hess_values_allocated,
				   int *dfunc_hess_values_size, int *err);

/** \brief What type of coordinate system is used for physical coordinates?
 *
 * \param[in]   instance          Field interface instance
 * \param[In]   tensor_handle      Tensor handle to act on.
 * \param[Out]  coord_type        Type of coord system.
 * \param[Out]  err               Error code.
 */
  void iField_getCoordType(const iField_Instance instance,
			   const iField_TensorHandle tensor_handle,
			   int *coord_type,
			   int* err);

/** \brief Retrieve name for this tensor.
 *
 * \param[in]  instance        Field interface instance
 * \param[In]  tensor_handle   Tensor handle to act on.
 * \param[In]  name            Name of tensor; must be preallocated.
 * \param[Out] err             Error code.
 * \param[Out] name_len        Length of name string.
 */
void iField_getTensorName(const iField_Instance instance,
			  const iField_TensorHandle tensor_handle,
			  char *name, int *err, int *name_len);

/** \brief Retrieve distribution function kernel for this tensor.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  tensor_handle      Tensor handle to act on.
 * \param[In]  dfunc            Distribution function handle for this tensor.
 * \param[Out] err             Error code.
 */
void iField_getDistFunc(const iField_Instance instance,
			const iField_TensorHandle tensor_handle,
			iField_DFuncKernel *dfunc,
			int *err);

/** \brief Retrieve tensor template for this tensor.
 *
 * Restriction: tensor_handle must not be a compound field.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]   tensor_handle      Tensor handle to act on.
 * \param[In] template_handle  Tensor template handle for this tensor.
 * \param[Out] err             Error code.
 */
void iField_getTensorTemplate(const iField_Instance instance,
			      const iField_TensorHandle tensor_handle,
			      iField_TensorTemplateHandle *template_handle,
			      int *err);

/** \brief Retrieve units for this tensor.
 *
 * Restriction: tensor_handle must not be a compound field.
 *
 * \param[in]  instance        Field interface instance
 * \param[In]  tensor_handle   Tensor handle to act on.
 * \param[In]  units           Units handle for this tensor.
 * \param[Out] err             Error code.
 */
void iField_getUnits(const iField_Instance instance,
		     const iField_TensorHandle tensor_handle,
		     iField_UnitsHandle *units,
		     int *err);
  
/** \brief Retrieve precision for this tensor.
 *
 * \param[in]  instance        Field interface instance
 * \param[In]  tensor_handle   Tensor handle to act on.
 * \param[In]  precision       Precision for this tensor.
 * \param[Out] err             Error code.
 */
void iField_getPrecision(const iField_Instance instance,
			 const iField_TensorHandle tensor_handle,
			 int *precision,
			 int *err);

/** \brief Returns global indices for all DOFs that affect the TensorField
 *      on this entity.
 *
 * The entity given must be one over which the dfuncs are specified.
 * For TensorFields with the same DOF associations, the order of DOFs
 * for different TensorFields should be the same (principle of least
 * astonishment).
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  tensor_handle       Field to act on.
 * \param[In]  entity_handle      Entity for which DOF indices are desired.
 * \param[InOut]  indices           Data is returned here.
 * \param[InOut]  indices_allocated  Allocated size of data array.
 * \param[Out]  indices_size      Used size of data array.
 * \param[Out]  err               Error code.
 */
void iField_getDOFIndicesByEnt(const iField_Instance instance,
			       iField_TensorHandle tensor_handle,
			       iBase_EntityHandle entity_handle,
			       int **indices,
			       int *indices_allocated,
			       int *indices_size,
			       int *err);
  
/** \brief Returns global indices for all DOFs that affect the TensorField
 *      on these entities.
 *
 * The entities given must be ones over which the dfuncs are specified.
 * For TensorFields with the same DOF associations, the order of DOFs
 * for different TensorFields should be the same (principle of least
 * astonishment).
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  tensor_handle      Tensor to act on.
 * \param[In]  entity_handles     Entities for which DOF indices are desired.
 * \param[In]  num_entities       Number of entities passed in.
 * \param[InOut]  indices           Data is returned here.
 * \param[InOut]  indices_allocated Allocated size of data array.
 * \param[Out]  indices_size      Used size of data array.
 * \param[inout]  offsets         Offsets for data by entity are
 *                                  returned here
 * \param[InOut]  offsets_allocated Allocated size of offset array.
 * \param[Out]  offsets_size      Used size of offset array.
 * \param[Out]  err               Error code.
 */
void iField_getDOFIndicesByEntArr(const iField_Instance instance,
				  iField_TensorHandle tensor_handle,
				  iBase_EntityHandle *entity_handles,
				  int num_entities,
				  int **indices,
				  int *indices_allocated,
				  int *indices_size,
				  int **offsets,
				  int *offsets_allocated,
				  int *offsets_size,
				  int *err);

/** \brief Destroy a tensor field.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  handle            Tensor field to destroy.
 * \param[Out] err               Error code.
 */
  void iField_destroyTensorField(const iField_Instance instance,
				 iField_TensorHandle handle,
				 int *err);

/** \brief Create a domain from an entity set and return its handle.
 *
 * The entities used as support for distribution functions are mesh
 * entities in the given mesh instance, and the entity set is a set of
 * mesh entities from that instance. The set may be the root set.
 *
 * \param[in] field_instance     Field interface instance
 * \param[In] mesh_instance      iMesh instance to which the set belongs.
 * \param[In] set_handle         iMesh entity set containing the entities over
 *                               which the field is defined.
 * \param[Out] domain_handle     Handle for the new domain.
 * \param[Out] err               Error code.
 */
  void iField_createMeshDomainFromMeshSet
  (const iField_Instance field_instance,
   const iMesh_Instance mesh_instance,
   const iBase_EntitySetHandle set_handle,
   iField_DomainHandle* domain_handle,
   int *err);
			   
/** \brief Create a domain from a geometric entity and return its handle.
 *
 * The entities used as support for distribution functions are mesh
 * entities in the iMesh instance related to the iGeom instance by the
 * given iRel_PairHandle.  The mesh entities are related to the given
 * geometric entity, either directly or through a mesh set related to
 * the geometry entity.
 *
 * The call sequence here is cumbersome, because both the geometric entity
 * and the iRel relation must have instances to go with them.  As far as I
 * can tell, iRel can't provide this info currently.
 *
 * \param[in] field_instance     Field interface instance
 * \param[In] igeom_instance     iGeom instance to which the geometric entity
 *                               belongs.
 * \param[In] geom_ent_handle    iGeom entity to which mesh entities are
 *                               related.
 * \param[In] irel_instance      iRel instance to which the relation belongs.
 * \param[In] relation_handle    iRel relation to the mesh entities over
 *                               which the field is defined.
 * \param[Out] domain_handle     Handle for the new domain.
 * \param[Out] err               Error code.
 */
  void iField_createMeshDomainFromGeomEnt
  (const iField_Instance field_instance,
   const iGeom_Instance igeom_instance,
   const iBase_EntityHandle geom_ent_handle,
   const iRel_Instance irel_instance,
   const iRel_PairHandle relation_handle,
   iField_DomainHandle* domain_handle,
   int *err);
			   
/** \brief Create a domain from a set of geometric entities and return its handle.
 *
 * The entities used as support for distribution functions are mesh
 * entities in the iMesh instance related to the iGeom instance by the
 * given iRel_PairHandle.  The mesh entities are related to the geometry
 * entities in the given set of geometry entities, either directly or
 * through mesh sets related to the geometry entities.
 *
 * The call sequence here is cumbersome, because both the geometric
 * entity set and the iRel relation must have instances to go with them.
 * As far as I can tell, iRel can't provide this info currently.
 *
 * \param[in] field_instance     Field interface instance
 * \param[In] igeom_instance     iGeom instance to which the geometric entity
 *                               belongs.
 * \param[In] geom_set_handle    iGeom entity set to which mesh entities are
 *                               related.
 * \param[In] irel_instance      iRel instance to which the relation belongs.
 * \param[In] relation_handle    iRel relation to the mesh entities over
 *                               which the field is defined.
 * \param[Out] domain_handle     Handle for the new domain.
 * \param[Out] err               Error code.
 */
  void iField_createMeshDomainFromGeomSet
  (const iField_Instance field_instance,
   const iGeom_Instance igeom_instance,
   const iBase_EntitySetHandle geom_set_handle,
   const iRel_Instance irel_instance,
   const iRel_PairHandle relation_handle,
   iField_DomainHandle* domain_handle,
   int *err);

/** \brief Create a domain from a set of geometric entities and return its handle.
 *
 * The entities used as support for distribution functions are iGeom
 * entities in the given iGeom instance, and the entity set is a set of
 * iGeom entities from that instance. The set may be the root set.
 *
 * \param[in] field_instance     Field interface instance
 * \param[In] geom_instance      iGeom instance to which the set belongs.
 * \param[In] set_handle         iGeom entity set containing the entities over
 *                               which the field is defined.
 * \param[Out] domain_handle     Handle for the new domain.
 * \param[Out] err               Error code.
 */
  void iField_createGeomDomainFromGeomSet
  (const iField_Instance field_instance,
   const iGeom_Instance geom_instance,
   const iBase_EntitySetHandle set_handle,
   iField_DomainHandle* domain_handle,
   int *err);

/** \brief Which mesh instance contains entities supporting this field?
 *
 * This function will return an error if the domain was created using
 * createGeomDomainFromGeomSet.
 *
 * \param[In]  field_instance    Field interface instance.
 * \param[In]  domain_handle     Domain being queried.
 * \param[Out] mesh_instance     Mesh instance to which mesh entities
 *                                 belong.
 * \param[Out] err               Error code.
 */
  void iField_getDomainMesh(const iField_Instance field_instance,
			    const iField_DomainHandle domain_handle,
			    iMesh_Instance* mesh_instance,
			    int *err);

/** \brief Which mesh set contains entities supporting this field?
 *
 * This function will return an error if the domain was created using
 * createGeomDomainFromGeomSet or createMeshDomainFromGeomSet.  If
 * createMeshDomainFromGeomEnt was used, then an error is returned if
 * the iGeom entity was associated directly to multiple mesh entities
 * rather than to an iMesh set of entities.
 *
 * In any event, if the function returns iBase_SUCCESS, the set handle
 * must be the one which contains exactly the entities in the domain.
 *
 * \param[In]  field_instance    Field interface instance.
 * \param[In]  domain_handle     Domain being queried.
 * \param[Out] mesh_set_handle   Mesh set to which mesh entities belong.
 * \param[Out] err               Error code.
 */
  void iField_getDomainMeshSet(const iField_Instance field_instance,
			       const iField_DomainHandle domain_handle,
			       iBase_EntitySetHandle* mesh_set_handle,
			       int *err);

/** \brief Which geom ent is related to mesh entities supporting this field?
 *
 * This function will return an error if the domain was created using
 * createGeomDomainFromGeomSet, createMeshDomainFromMeshSet, or 
 * createMeshDomainFromGeomSet.
 *
 * \param[In]  field_instance    Field interface instance.
 * \param[In]  domain_handle     Domain being queried.
 * \param[Out] geom_instance     iGeom instance to which geometric
 *                                 entity belongs.
 * \param[Out] geom_entity       iGeom entity related to mesh entities
 * \param[Out] rel_instance      iRel instance to which rel_pair belongs.
 * \param[Out] rel_pair          iRel pair handle relating geom to mesh
 * \param[Out] err               Error code.
 */
  void iField_getDomainGeomEnt(const iField_Instance field_instance,
			       const iField_DomainHandle domain_handle,
			       iGeom_Instance* geom_instance,
			       iBase_EntityHandle* geom_entity,
			       iRel_Instance* rel_instance,
			       iRel_PairHandle* rel_pair,
			       int *err);

/** \brief Which geom set is related to mesh entities supporting this field?
 *
 * This function will return an error if the domain was created using
 * createGeomDomainFromGeomSet, createMeshDomainFromMeshSet, or 
 * createMeshDomainFromGeomEnt.
 *
 * \param[In]  field_instance    Field interface instance.
 * \param[In]  domain_handle     Domain being queried.
 * \param[Out] geom_instance     iGeom instance to which geometric
 *                                 entity belongs.
 * \param[Out] geom_entity_set   iGeom entity set related to mesh entities
 * \param[Out] rel_instance      iRel instance to which rel_pair belongs.
 * \param[Out] rel_pair          iRel pair handle relating geom to mesh
 * \param[Out] err               Error code.
 */
  void iField_getDomainGeomSet(const iField_Instance field_instance,
			       const iField_DomainHandle domain_handle,
			       iGeom_Instance* geom_instance,
			       iBase_EntitySetHandle* geom_entity_set,
			       iRel_Instance* rel_instance,
			       iRel_PairHandle* rel_pair,
			       int *err);

/** \brief Which geom set contains geom entities supporting this field?
 *
 * This function will return an error if the domain was created using
 * createMeshDomainFromGeomSet, createMeshDomainFromMeshSet, or 
 * createMeshDomainFromGeomEnt.
 *
 * \param[In]  field_instance    Field interface instance.
 * \param[In]  domain_handle     Domain being queried.
 * \param[Out] geom_instance     iGeom instance to which geometric
 *                                 entity belongs.
 * \param[Out] geom_entity_set   iGeom entity set related to mesh entities
 * \param[Out] err               Error code.
 */
  void iField_getGeomDomainInfo(const iField_Instance field_instance,
				const iField_DomainHandle domain_handle,
				iGeom_Instance* geom_instance,
				iBase_EntitySetHandle* geom_entity_set,
				int *err);


/**\brief Initialize an iterator over specified entity type and topology
 *
 * Initialize an array iterator over specified entity type and topology.
 * Iterator returned can be used as input to functions returning entities 
 * for the iterator.  If all entities of a specified type and/or topology
 * are to be iterated, specify iBase_ALL_TYPES or iMesh_ALL_TOPOLOGIES, 
 * respectively.  Specified type or topology must be a value in the
 * iBase_EntityType or iMesh_EntityTopology enumerations, respectively.
 *
 * Note that we will probably want (certainly need?) different syntax
 * for iterators over geometric domains.
 *
 * \param[in]  instance          Field interface instance
 * \param[In] domain_handle               Domain to iterate over
 * \param[In] requested_entity_type       Type of entity to iterate
 * \param[In] requested_entity_topology   Topology of entity to iterate
 * \param[Out] entity_iterator            New iterator returned by function
 * \param[Out] err                        Error code
 */
void iField_initEntIter(const iField_Instance instance,
			const iField_DomainHandle domain_handle,
			const int requested_entity_type,
			const int requested_entity_topology,
			iMesh_EntityIterator* entity_iterator,
			int *err);

/**\brief Initialize an array iterator over specified entity type, topology, and 
 *        size
 *
 * Initialize an array iterator over specified entity type, topology, and 
 * size, for a specified domain.  Iterator returned can be used 
 * as input to functions returning entities for the iterator.  If all 
 * entities of a specified type and/or topology are to be iterated, 
 * specify iBase_ALL_TYPES or iMesh_ALL_TOPOLOGIES, respectively.  
 * Specified type or topology must be a value in the iBase_EntityType or 
 * iMesh_EntityTopology enumerations, respectively.
 *
 * Note that we will probably want (certainly need?) different syntax
 * for iterators over geometric domains.
 *
 * \param[in]  instance          Field interface instance
 * \param[In] domain_handle               Domain to iterate over
 * \param[In] requested_entity_type       Type of entity to iterate
 * \param[In] requested_entity_topology   Topology of entity to iterate
 * \param[In] requested_array_size        Size of blocks for
 *                                        iterator to return.
 * \param[Out] entArr_iterator            New iterator returned by function
 * \param[Out] err                        Error code
 */
void iField_initEntArrIter(const iField_Instance instance,
			   const iField_DomainHandle domain_handle,
			   const int requested_entity_type,
			   const int requested_entity_topology,
			   const int requested_array_size,
			   iMesh_EntityArrIterator* entArr_iterator,
			   int *err);

  /** \brief Destroy a domain.
   *
   * This should refuse to destroy if the domain is in use.  This only
   * destroys the Domain, without affecting the underlying mesh
   * entities, or any possible mesh sets, geom ents, or geom sets.
   *
   * \param[in]  instance          Field interface instance
   * \param[In] handle               Domain to destroy.
   * \param[Out] err                 Error code.
   */
  void iField_destroyDomain(const iField_Instance instance,
			    iField_DomainHandle handle,
			    int *err);
  
/** \brief Create a coordinate field and return its handle.
 *
 * Coordinate fields are different from other fields:  we know in
 * advance that they are vector fields measuring length.  This
 * simplifies the calling sequence significantly
 *
 * Note that coordinate fields can -only- be evaluated in parametric
 * coordinates.  For one thing, otherwise you're asking "What are the
 * values of (x,y,z) for this value of (x,y,z)?"  Pretty sure I know the
 * answer to that question....
 *
 * MCM: We don't want to limit ourselves to length. We ought to be
 * able to define fields on any space including fields on pressure
 * temperature, 2D phase space for a material.
 *
 * CFOG reply:  I don't disagree, but let's make the simple things
 * simple and add that as a separate function later.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  mesh_instance     The mesh this coordinate field will provide
 *                                 coordinates for.
 * \param[In]  units             Units to measure length in.
 * \param[In]  coord_type        Coordinate system to use.
 * \param[In]  dfunc_handle      Distribution function kernel to use
 *                                 for coordinates.
 * \param[Out]  field_handle     Newly create coordinate field.
 * \param[Out]  err              Error code.
 */
void iField_createCoordinateField(const iField_Instance instance,
				  const iMesh_Instance mesh_instance,
				  const iField_UnitsHandle units,
				  int coord_type,
				  const iField_DFuncKernel dfunc_handle,
				  iField_Handle* field_handle,
				  int *err);

/** \brief Return the component tensor fields making up a compound field.
 *
 * \param[in]  instance          Field interface instance
 * \param[In] tensor_handle           Field to operate on.
 * \param[InOut] tensor_handles       Array of component tensor fields.
 * \param[InOut] tensor_handles_allocated  Allocated size of TF array.
 * \param[Out] tensor_handles_size    Used size of TF array.
 * \param[Out] err                    Error code.
 */
  void iField_getTensorFields(const iField_Instance instance,
			      const iField_TensorHandle handle,
			      iField_TensorHandle **tensor_handles,
			      int *tensor_handles_allocated,
			      int *tensor_handles_size,
			      int *err);
  
/*
 * NOTE: The syntax below is ridiculously awkward to work with in
 * practice.  In the near future, there will be precision-specific
 * versions of all get/setDOF and field evaluation functions, so that
 * conversion between data pointed to by a void* and double (or
 * whatever) actual data.  Implementation of this should be possible
 * relatively easy using pre-processor tricks to do the equivalent of
 * C++ templates.
 *
 * This approach will not only simplify app programming, but also allow
 * compile time type checking (no passing float data incorrectly labeled
 * as double....).
 *
 * We will also REQUIRE that if a type-specific evaluation method is
 * implemented then it is implemented in the precision the interface
 * implies.
 *
 * MCM: We could defer the issue of low-level datatype variability and
 * put in as a place-holder a simple macro DTYPE() which expands to
 * two things, an enum indicating type and a void* pointer but treat
 * that all as double for initial implementations.
 */

/** \brief Retrieve DOFs associated with this entity.
 *
 * This function returns dofs associated directly with this entity and,
 * optionally, entities on its closure.  This distinction is needed to
 * support mixed-accuracy discretizations, although full support for
 * this is not yet in the interface.
 *
 * For collective calls, DOFs are returned in either blocked,
 * interleaved, or mixed order, as requested.
 *
 * NOTE: With this function signature, the implementation is responsible
 * for returning a pointer to the location of that data; the application
 * is responsible for correctly interpreting the data at that address.
 * If (a pointer to) iField_DEFAULT_PRECISION is passed in, the
 * implementation will return data at the highest precision used by any
 * component field tensor and set *precision to this value.
 *
 * Array sizes are given in terms of number of values of the field's
 * data type, at *precision, NOT in terms of bytes.  This implies that
 * pointer conversion will make pointer arithmetic easier / more
 * readable. 
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  tensor_handle    The tensor being read.
 * \param[In]  entity_handle    Entity for which DOFs are requested.
 * \param[In]  include_closure  DOFs for closure, too?
 * \param[Out]  precision       Precision of returned data.
 * \param[Inout]  dofs            Storage for dof data.
 * \param[Inout]  dofs_allocated  Allocated size, of values of size precision.
 * \param[Out]  dofs_size       Used size, of values of size precision.
 * \param[Out]  err             Error code.
 */
  void iField_getDOFbyEnt(const iField_Instance instance,
			  iField_TensorHandle tensor_handle,
			  const iBase_EntityHandle entity_handle,
			  const int include_closure,
			  int *precision,
			  void **dofs, int *dofs_allocated, int *dofs_size,
			  int *err);
  void iField_getDOFbyEntArr(const iField_Instance instance,
			     iField_TensorHandle tensor_handle,
			     const iBase_EntityHandle *entity_handles,
			     const int num_handles,
			     const int include_closure,
			     const int storage_order,
			     int *precision,
			     void **dofs, int *dofs_allocated, int *dofs_size,
			     int **offsets, int *offsets_allocated,
			     int *offsets_size,
			     int *err);
  void iField_getAllDOF(const iField_Instance instance,
			iField_TensorHandle tensor_handle,
			int *precision,
			const int storage_order,
			void **dofs, int *dofs_allocated, int *dofs_size,
			int *err);

/** \brief Set DOFs associated with this entity.
 *
 * This function stores dofs associated directly with this entity and,
 * optionally, entities on its closure.  This distinction is needed to
 * support mixed-accuracy discretizations, although full support for
 * this is not yet in the interface.
 *
 * For collective calls, DOFs are given in either blocked, interleaved,
 * or mixed order, as requested.
 *
 * NOTE: The application is responsible for sending data at the
 * specified precision; the implementation is responsible for correctly
 * interpreting the data and storing it at the precision specified at
 * field creation.
 *
 * Array sizes are given in terms of number of values at *precision, NOT
 * in terms of bytes.  
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle     The Field being written.
 * \param[In]  entity_handle    Entity for which DOFs are requested.
 * \param[In]  include_closure  DOFs for closure, too?
 * \param[In]  precision        Precision of input data.
 * \param[In]  dofs             Storage for dof data.
 * \param[In]  dofs_size        Used size, in bytes.
 * \param[Out]  err             Error code.
 */
  void iField_setDOFbyEnt(const iField_Instance instance,
			  iField_TensorHandle tensor_handle,
			  const iBase_EntityHandle entity_handle,
			  const int include_closure,
			  const int storage_order,
			  int precision,
			  void *dofs, const int dofs_size,
			  int *err);
  void iField_setDOFbyEntArr(const iField_Instance instance,
			     iField_TensorHandle tensor_handle,
			     const iBase_EntityHandle *entity_handles,
			     const int num_entities,
			     const int include_closure,
			     const int storage_order,
			     int precision,
			     void *dofs, const int dofs_size,
			     int *offsets, const int offsets_size,
			     int *err);
  void iField_setAllDOF(const iField_Instance instance,
			iField_TensorHandle tensor_handle,
			int precision,
			const int storage_order,
			void *dofs, int *dofs_size,
			int *err);


/* CFOG Oct 4, 2010
 *   To handle evaluation of solution, gradient, Hessian, etc for a
 *   field, do we want to add functions, or add an arg to the current
 *   functions specifying what to evaluate?
 */
/** \brief Evaluate a compound field at a point in a known entity.
 *
 * Knowing which entity should allow significant optimization.
 *
 * In the collective call, each set of coordinates must have an entity
 * handle to go with it.
 *
 * NOTE: This function returns all data at the same precision.  The
 * implementation is responsible for returning a pointer to the location
 * of that data; the application is responsible for correctly
 * interpreting the data at that address.  If (a pointer to)
 * iField_DEFAULT_PRECISION is passed in, the implementation will return
 * data at the highest precision used by any component field tensor and
 * set *precision to this value.
 *
 * Array sizes are given in terms of number of floating point numbers at
 * *precision, NOT in terms of bytes.  This implies conversion to a
 * pointer to floating point number before doing pointer arithmetic.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle       Field instance to act on.
 * \param[In]  entity_handle      Entity in which dfuncs will be
 *                                evaluated.
 * \param[In]  coords             Coordinates at which to evaluate
 *                                dfunc.
 * \param[in]  is_param          True if coords are parametric; false
 *                                for tensor physical coord system.
 * \param[InOut]  precision         Precision of output data.
 * \param[InOut]  field_values      Data is returned here.
 * \param[InOut]  field_values_allocated  Allocated size of data array.
 * \param[Out]  field_values_size  Used size of data array.
 * \param[Out]  err               Error code.
 */ 
  void iField_evaluateInEnt(const iField_Instance instance,
			    iField_TensorHandle tensor_handle,
			    const iBase_EntityHandle entity_handle,
			    const double *coords,
			    const int is_param,
			    int *precision,
			    void **field_values, int *field_values_allocated,
			    int *field_values_size, int *err);
  void iField_evaluateInEntArr(const iField_Instance instance,
			       iField_TensorHandle tensor_handle,
			       const int num_points,
			       const iBase_EntityHandle *entity_handles,
			       const double *coords,
			       const int is_param,
			       int *precision,
			       void **field_values, int *field_values_allocated,
			       int *field_values_size, int *err);
			    
/** \brief Evaluate a compound field at a point near a known entity.
 *
 * This is the case that will be intermediate in efficiency.
 *
 * Note that, for this case, coordinates are in the physical coordinate
 * frame: if you're using parametric coordinates for a known entity, use
 * evaluateInEnt even if the point lies physically outside the entity.
 *
 * See notes for evaluateInEnt
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle       Field to act on.
 * \param[In]  entity_handle      Entity near which dfuncs will be
 *                                evaluated.
 * \param[In]  coords             Coordinates at which to evaluate
 *                                dfunc.
 * \param[InOut]  precision         Precision of output data.
 * \param[InOut]  field_values      Data is returned here.
 * \param[InOut]  field_values_allocated  Allocated size of data array.
 * \param[Out]  field_values_size  Used size of data array.
 * \param[Out]  err               Error code.
 */ 
  void iField_evaluateNearEnt(const iField_Instance instance,
			      iField_TensorHandle tensor_handle,
			      const iBase_EntityHandle entity_handle,
			      const double *coords,
			      int *precision,
			      void **field_values,
			      int *field_values_allocated,
			      int *field_values_size,
			      int *err);
  void iField_evaluateNearEntArr(const iField_Instance instance,
				 iField_TensorHandle tensor_handle,
				 const int num_points,
				 const iBase_EntityHandle *entity_handles,
				 const double *coords,
				 const int is_param,
				 int *precision,
				 void **field_values,
				 int *field_values_allocated,
				 int *field_values_size,
				 int *err);
  
/** \brief Evaluate a compound field with no location hint.
 *
 * This is the expensive case, because we have to find the entity first.
 * Might as well return it for future use....
 *
 * Note that, for this case, coordinates are in the physical coordinate
 * frame: if you're using parametric coordinates for a known entity, use
 * evaluateInEnt even if the point lies physically outside the entity.
 *
 * See notes for evaluateInEnt
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle       Field to act on.
 * \param[In]  coords             Coordinates at which to evaluate
 *                                dfunc.
 * \param[Out]  entity_handle      Entity in which dfuncs were
 *                                evaluated.
 * \param[InOut]  field_values      Data is returned here.
 * \param[InOut]  field_values_allocated  Allocated size of data array.
 * \param[Out]  field_values_size  Used size of data array.
 * \param[Out]  err               Error code.
 */ 
void iField_evaluate(const iField_Instance instance,
		     iField_TensorHandle tensor_handle,
		     const double *coords,
		     iBase_EntityHandle* entity_handle,
		     void **field_values,
		     int *field_values_allocated,
		     int *field_values_size, int *err);
void iField_evaluateArr(const iField_Instance instance,
			iField_TensorHandle tensor_handle,
			const int num_points,
			const double *coords,
			iBase_EntityHandle **entity_handles,
			int *entity_handles_allocated,
			int *entity_handles_size,
			void **field_values,
			int *field_values_allocated,
			int *field_values_size, int *err);
  
/** \brief Retrieve information about the domain for this compound field.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle       Field to act on.
 * \param[Out]  domain_handle     The domain for this Field.
 * \param[Out]  err               Error code.
 */ 
  void iField_getDomain(const iField_Instance instance,
			iField_TensorHandle tensor_handle,
			iField_DomainHandle* domain_handle,
			int* err);

  /* I/O stuff is still very raw.  From the comments, you can tell that
     Mark M, who was the last to touch this, is unhappy with what he's
     got.  Suggestions and/or comments welcome. */

/** \brief Save a Field to a file.
 *
 * There is a problem with maintaining the binding between iField and
 * iMesh (or iGeom) persistently. In a running executable, we've got
 * the iField and iMesh instance handles. But, when loading from
 * disk, all we've got is filenames.
 *
 * Our options for maintaining iMesh<->iField binding persisently are...
 *    a) Store iMesh file name in iField (perhaps reverse as well).
 *       This will probably require methods specifically for this task
 *       in both interfaces.
 *    b) Store iMesh and iField instances to the same file. This is
 *       the most natural thing to do anyways. But, I don't think we
 *       can assume all implementations are prepared to handle this
 *       so I am not sure we can make this a requirement. 
 *    c) Rely upon some naming convention such that given the name
 *       of one piece, you can construct the other and vice versa.
 *       This is the simplest strategy but feels kludgy. 
 *    d) Ignore the problem. Let the applications worry about it. I
 *       think this is just asking for trouble.
 *    e) A 'meta' file containing the iField/iMesh filenames.
 *       An xml-ish meta file might serve us well here.
 * Note: the above assumes all fields are in one file. But the save
 * call defined here doesn't necessarily REQUIRE that. Perhaps it
 * should. If we somehow loose track of the mesh file, then all of
 * our field data is pretty much totally useless.
 *
 * If this function is called multiple times for different fields in the
 * same iField_instance what happens? Can all the data go to the same
 * file? Must it go into different files? If think if the latter is true,
 * we're asking for serious I/O performance problems. On the other hand,
 * if the caller passes a different filename each time, then the impl.
 * has no choice, either.
 *
 * CFOG: Checksums on iMesh file would be useful to keep to know if
 * something goes wrong with the mesh file. Another issue is mixing,
 * for example, a GRUMMP iMesh instance with a MOAB iField instance.
 * There is no reason this should NOT be supported. Nonetheless, this
 * probably does raise some issues regarding save/load operations that
 * we've yet to consider.
 *
 * File format and options will inevitably be implementation dependent.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle       Field to act on.
 * \param[In]  filename           What file to write to.
 * \param[In]  options            Options, probably implementation dependent.
 * \param[Out]  err               Error code.
 * \param[In]  filename_len       Length of filename string.
 * \param[In]  options_len        Length of options string.
 */
  void iField_save(const iField_Instance instance,
		   iField_TensorHandle tensor_handle,
		   char *filename,
		   char *options,
		   int *err,
		   const int filename_len,
		   const int options_len);
  
/** \brief Read a Field definitions from a file 
 *
 * File format and options will inevitably be implementation dependent.
 * Note: No actual field dofs are read into memory in this call.
 *
 * \param[in]  instance          Field interface instance
 * \param[In]  field_handle       Field to act on.
 * \param[In]  filename           What file to read from.
 * \param[In]  options            Options, probably implementation dependent.
 * \param[Out]  err               Error code.
 * \param[In]  filename_len       Length of filename string.
 * \param[In]  options_len        Length of options string.
 *
 * MCM: maintaining the binding between dofs (in iField) and entities (in iMesh)
 * (e.g. the dof associations) involves assumptions regarding the ordering
 * of these things in the buckets in which they are stored. iField may need
 * to impose restrictions on what iMesh is allowed to do in the way of
 * re-ordering things on its _save _load operations. Or, iMesh needs to
 * deliver some permutation vector to iField so that iField can re-order its
 * dofs to match iMesh (for those dofs that are not handled as tags).
 *
 * MCM: The above sounds like it is similar to the problem of how do we deal
 * with addition/deletion of entities in iMesh? What happens to the dofs associated
 * with those entities? I don't think we can prevail upon iMesh to inform
 * iField that the associated dofs also need to be deleted. However, we
 * might be able to require iField to ask the iMesh instance if the entities
 * for which it is storing dofs have 'changed'. But even so, knowing the
 * ordering has changed is not sufficient. iField would need to query from
 * iMesh the new ordering and then adjust dof storage to match it. I think
 * having call in which all this magic was buried would be helpful to the
 * iField client. Something like...
 *
 * void iField_syncDofStorage(const iField_Instance instance, int *err);
 *
 * might be sufficient.
 *
 */
  void iField_load(const iField_Instance instance,
		   iField_TensorHandle tensor_handle,
/* MCM: Caller cannot have created a iField_TensorHandle yet. So,
   this argument should be moved to iField_load_dofs() call below. */
		   char *filename,
		   char *options,
		   int *err,
		   const int filename_len,
		   const int options_len);

/** \brief Read dofs for (some) fields in the instance
 *
 * MCM: This approach assumes an impl. can allocate memory for dofs
 * separately from the field 'header' information itself.
 * 
 * MCM: We discussed adjusting the iField_createTensorField call to
 * accept an option to indicate if the memory should be allocated
 * then or deferred to later. That is basically the same idea as
 * we are hoping to achieve here in the load call.
 * 
 * \param[in]   instance             Field interface instance
 * \param[in]   fields_whose_dofs_to_load The field handles for
 *                                        which dofs should be loaded
 * \param[in]   options
 * \param[Out]  err
 * \param[in]   options_len */
  void iField_loadDofs(const iField_Instance instance,
                       iField_TensorHandle fields_whose_dofs_to_load,
                       char *options,
                       int *err,
                       const int options_len);

  /**********************************************************************/
  /* Stuff below this in the file is still very much under development.
     Again, comments and/or suggestions are welcome. */
  /**********************************************************************/  



/* CFOG 9/26/10
                      PARALLEL

   Design principle: Keep iMesh/iMeshP iField-unaware, to the level of
   being able to build iMesh/iMeshP without ever using #include
   "iField.h".  This may or may not be completely possible, but if not,
   then the pieces where it's not (necessarily additions to the current
   iMesh/iMeshP should be pinched off into separate header files.

   For parallel fields, I propose that we construct a
   parallel field iff the domain is parallel.  I can see three ways of
   constructing a parallel domain offhand:
     1.  Add a partition arg to the create calls.
     2.  Add clones of the create calls that include a partition handle.
     3.  Add a function that identifies a newly created domain as
   parallel.

   Explicit assumptions:
     1.  A parallel tensor handle will contain data spanning an entire
     iMesh instance (ie, one parallel tensor handle for a given field
     per process, NOT per part).  This is consistent with iMesh(P).
     2.  A parallel tensor handle will not necessarily have data on all
     parts in a partition.

   Questions/Comments:
     1.  Having a createDomain...All call will make it so the domain is
     known on every part in a partition, even if not every part has
     entities in that domain.  Then...
     2.  A createTensorFieldAll call would seem to be the esthetically
     right way to set up a field on a parallel domain.  It also
     addresses Mark M's issue about all field templates being known on
     all parts (at least, all templates for parallel fields will be
     known).
*/

/******************** GHOSTING ********************/
/*
 * At our October 1 telecon, Fabien suggested introducing a
 * storage-level abstraction like ipcomman that does migration of
 * 'data.' Use an approach where migration expressed in terms of
 * 'entities' for iMesh/P or 'dofs' for iField is mapped into this
 * storage abstraction and the actual parallel data handling is then
 * done from this storage abstraction.
 */
/*
 * First an iMeshP proposal: the current iMeshP spec allows only one
 * ghosting pattern at a time.  This needs to be changed by:
 *   1. Adding an iMeshP_GhostRule opaque type.
 *   2. Adding create/destroyGhostRule functions.
 *   3. Tweaking the interface for the funcs that currently create and
 *        use ghost rules to take a specific rule as an arg.
 *
 * Second, I'm not seeing any obvious reasons that a part handle would
 * ever be needed as an arg in a iField call, since these are
 * fundamentally entity-based calls.
 *
 * Third, ghosting will require that we duplicate a bunch of iMeshP tag
 * ghosting calls as well as iMeshP wait for request, etc.  I can see
 * two ways around this:
 *   1. Declare that fields are tags.  This almost certainly fails on
 * philosophical grounds, and also almost certainly fails for certain
 * dfuncs whose DOFs can't be easily represented as tags.
 *   2. Overload tag handles with field handles somehow, just as part
 * handles can be used in place of set handles in some places.
 *
 * In the event that option 2 also fails, here are the functions that I
 * currently foresee needing.
 *
 * One functionality is still missing, maybe: migrateDOFs.  This would,
 * ideally, just happen for all fields when an entity migrates via
 * iMeshP.  I think that, especially if we have an iField instance that
 * contains all fields for a given iMesh instance (which we can easily
 * have by mandating a restriction on how iField instances are used),
 * then it could be possible for iMesh/iMeshP implementations to handle
 * this automatically, though an iField_migrateDOFs call might still be
 * required to go with this; not certain yet.
 */

  /* The ghosting function proposals are still way too far from
     primetime, so they're currently commented out.  No point worrying
     about syntax and defining types and so on until we're sure the
     semantics are converging. */

/* /\** \brief Asynchronously send field data for a given field. */
/*  * */
/*  * The field data is pushed from the owner entity to all copies.  This */
/*  * function assumes that fields given on calling parts are consistent; */
/*  * i.e., that they have the same name, data type, units, etc. */
/*  * */
/*  * This call does not block; applications should call */
/*  * iField_waitForRequest (or a similar wait function) to block until */
/*  * this push is completed. */
/*  * */
/*  *  COMMUNICATION:  point-to-point, non-blocking. */
/*  *  */
/*  * \param[in]  instance          Field interface instance */
/*  * \param[In] field            Field to transfer data for */
/*  * \param[In] ghost_rule       Ghost rule to use in determining data to send. */
/*  * \param[Out] request_handle  Transaction handle to use for a blocking wait. */
/*  * \param[Out] err             Error code. */
/*  *\/ */
/* void iField_iPushDOFs(const iField_Instance instance, */
/* 		      iField_TensorHandle field, */
/* 		      iMeshP_GhostRule ghost_rule, */
/* 		      iField_RequestHandle request_handle, */
/* 		      int *err); */

/* /\** \brief Asynchronously send field data for a given field and entities. */
/*  * */
/*  * The field data is pushed from the owner entity to all copies.  This */
/*  * function assumes that fields given on calling parts are consistent; */
/*  * i.e., that they have the same name, data type, units, etc. */
/*  * */
/*  * Only data for specified entities is sent. */
/*  * */
/*  * This call does not block; applications should call */
/*  * iField_waitForRequest (or a similar wait function) to block until */
/*  * this push is completed. */
/*  * */
/*  *  COMMUNICATION:  point-to-point, non-blocking. */
/*  *  */
/*  * \param[in]  instance          Field interface instance */
/*  * \param[In] field            Field to transfer data for */
/*  * \param[In] ghost_rule       Ghost rule to use in determining data to send. */
/*  * \param[In] entities         Entities for which to push data. */
/*  * \param[In] num_entities     Number of entities for which to push data. */
/*  * \param[Out] request_handle  Transaction handle to use for a blocking wait. */
/*  * \param[Out] err             Error code. */
/*  *\/ */
/* void iField_iPushEntDOFs(const iField_Instance instance, */
/* 			 iField_TensorHandle field, */
/* 			 iMeshP_GhostRule ghost_rule, */
/* 			 iBase_EntityHandle *entities, */
/* 			 const int num_entities, */
/* 			 iField_RequestHandle request_handle, */
/* 			 int *err); */

/* /\** \brief Copies send DOF data back to owner, which performs an */
/*  *   operation on the data it receives. */
/*  * */
/*  * Getting the result back to the copies requires a iPushDOFs call. */
/*  * */
/*  * The operation to perform can be any of the MPI standard ops. */
/*  * */
/*  *  COMMUNICATION:  point-to-point, non-blocking. */
/*  * */
/*  * \param[in]  instance          Field interface instance */
/*  * \param[In] field            Field to transfer data for */
/*  * \param[In] operation        Operation to perform on incoming data. */
/*  * \param[Out] request_handle  Transaction handle to use for a blocking wait. */
/*  * \param[Out] err             Error code. */
/*  *\/ */
/* void iField_accumulateDOFs(const iField_Instance instance, */
/* 			   iField_TensorHandle field, */
/* 			   MPI_Op operation, */
/* 			   iField_RequestHandle request_handle, */
/* 			   int *err); */

/* /\**\brief  Wait for a specific iField request to complete. */
/*  * */
/*  *  Given an iField_RequestHandle, wait for the request to complete. */
/*  * */
/*  *  COMMUNICATION:  Blocking point-to-point. */
/*  * */
/*  * \param[in]  instance          Field interface instance */
/*  *  \param[In]  field                     Field whose data is being transfered. */
/*  *  \param[In]  request                   iField request for whose completion */
/*  *                                        we should wait. */
/*  *  \param[Out]  err                      Error code. */
/*  *\/ */
/* void iField_waitForRequest(const iField_Instance instance, */
/* 			   iField_TensorHandle field */
/* 			   iField_RequestHandle request, */
/* 			   int *err); */


/* /\**\brief  Wait for any of the specified iField requests to complete. */
/*  * */
/*  *  Given an array of iField_RequestHandles, wait for any one of the requests  */
/*  *  to complete. */
/*  * */
/*  *  COMMUNICATION:  Blocking point-to-point. */
/*  * */
/*  * \param[in]  instance          Field interface instance */
/*  *  \param[In]  field                     Field whose data is being transfered. */
/*  *  \param[In]  requests                  iField requests for which we wait */
/*  *                                        until one request completes. */
/*  *  \param[In]  requests_size             Number of requests in requests. */
/*  *  \param[Out]  index                    Index of the request that completed. */
/*  *  \param[Out]  err                      Error code. */
/*  *\/ */
/* void iField_waitForAnyRequest(const iField_Instance instance, */
/* 			      iField_TensorHandle field */
/* 			      iField_RequestHandle *requests, */
/* 			      int requests_size, */
/* 			      int *index, */
/* 			      int *err); */


/* /\**\brief  Wait for all of the specified iField requests to complete. */
/*  * */
/*  *  Given an array of iField_RequestHandles, wait for all of the requests  */
/*  *  to complete. */
/*  * */
/*  *  COMMUNICATION:  Blocking point-to-point. */
/*  * */
/*  * \param[in]  instance          Field interface instance */
/*  *  \param[In]  field                     Field whose data is being transfered. */
/*  *  \param[In]  requests                  iField requests for which we wait */
/*  *                                        until one request completes. */
/*  *  \param[In]  requests_size             Number of requests in requests. */
/*  *  \param[Out]  err                      Error code. */
/*  *\/ */
/* void iField_waitForAllRequests(const iField_Instance instance, */
/* 			       iField_TensorHandle field */
/* 			       iField_RequestHandle *requests, */
/* 			       int requests_size, */
/* 			       int *err); */

/* CFOG 9/26/10 Another new section. */
/******************** p-Refinement ********************/
/* This, I think, is far easier from an interface point of view than for
 * implementations.   Here's what I'm -sure- we'll need; this may still
 * be incomplete.
 */
/** \brief Specify a particular dfunc for an entity.
 *
 * This function can be called on compound fields or simple fields that
 * are not part of compound fields.
 *
 * New DOFs aren't set by this call (what values should we guess?), so
 * the app is going to have to do that after.
 *
 * \param[in]  instance          Field interface instance
 * \param[In] field           Field to change dfunc for locally.
 * \param[In] entity          Entity to change dfunc for.
 * \param[In] new_dfunc       DFunc this entity will now use.
 * \param[Out] err            Error code.
 */
void iField_changeDFunc(const iField_Instance instance,
			iField_TensorHandle field,
			iBase_EntityHandle entity,
			iField_DFuncKernel new_dfunc,
			int *err);

/** \brief Specify a particular dfunc for an array of entities.
 *
 * This function can be called on compound fields or simple fields that
 * are not part of compound fields.
 *
 * New DOFs aren't set by this call (what values should we guess?), so
 * the app is going to have to do that after.
 *
 * \param[in]  instance          Field interface instance
 * \param[In] field           Field to change dfunc for locally.
 * \param[In] entity          Entity to change dfunc for.
 * \param[In] new_dfunc       DFunc this entity will now use.
 * \param[Out] err            Error code.
 */
void iField_changeDFuncArr(const iField_Instance instance,
			   iField_TensorHandle field,
			   iBase_EntityHandle entities,
			   const int num_entities,
			   iField_DFuncKernel new_dfunc,
			   int *err);

/** \brief Return all dfuncs in use for any entities for this field.
 *
 * \param[in]  instance          Field interface instance
 * \param[In] field                Field queried.
 * \param[InOut] dfunc             Array of dfuncs in use
 * \param[InOut] dfunc_allocated   Allocated size of dfunc.
 * \param[Out] dfunc_size          Number of dfuncs in use.
 * \param[Out] err                 Error code.
 */
void iField_getAllDFuncs(const iField_Instance instance,
			 iField_TensorHandle field,
			 iField_DFuncKernel **dfunc,
			 int *dfunc_allocated,
			 int *dfunc_size,
			 int *err);

/* CFOG 9/26/10  I can definitely see wanting to iterate separately over
   entities with different dfuncs.  Should these be separate calls to
   create iterators, or additional args to the existing iterator calls?

   Also, IMO, apps are responsible for ensuring sensible behavior in the
   transitional cases (entities with some faces p-refined but not all,
   say.  If nothing else, how this is dealt with depends on what dfuncs
   you combine, so the implementation needs guidance anyway.  Caveat
   user.
*/

/***********************************************************************
 * NOTES / QUESTIONS / TOPICS FOR DISCUSSION:
 *
 * Placeholder: At a minimum, we need to be able to represent tensor
 * product spaces $(\Re^{n}\times\Re)$ --- space-time is the most common
 * but by no means only example.  Regardless of details, this requires
 * more thought.
 *
 * Placeholder: Variable order (p-refinement)
 *
 * Placeholder: Thread safety: atomic += and lock/unlock?  Or strictly
 * up to implementation?
 *
 * Placeholder:  Returning mixed real / int data.  Up-convert to double?
 *
 * Placeholder:  dfunc kernel support:  iBase_Type, plus indication of
 * dual? 
 *
 * 1.  Being able to express relationships between fields is a useful
 *     capability, even if iField does nothing more than store the
 *     relationship and emit it on demand.  We discussed differential
 *     and algebraic operators; others are possible as well.  Evaluation
 *     of these operators is something that is probably best done as a
 *     service (implementations could of course choose to implement this
 *     service directly, making operations on their own fields more
 *     efficient...).  We tentatively concluded that this capability is
 *     unlikely to affect the core of the iField API and may even be
 *     orthogonal to most or all of the rest of the API.
 *
 * 4.  Coordinate consistency: I am thinking there is an argument for
 *     creating a (non-coordinate) field using a mesh topology and a
 *     coordinate field.  The latter would carry info about cartesian
 *     vs. polar, etc.  Physical coordinates passed to that field (and
 *     its component tensors and dfuncs, where appropriate) would be in
 *     that coordinate systems.  There are some obvious advantages to
 *     enforcing coordinate system consistency in this way, but also
 *     some possible disadvantages.  Any thoughts on this?
 *
 * 5.  We may want functions to be able to iterate over all dof clusters
 *     (dofs associated with the same entity) for things like changes of
 *     variables and/or coordinate system. Otherwise, you either have to
 *     retrieve all the dofs at once to do this, or duplicate work by
 *     doing the conversion (in a copy mode) multiple times for shared
 *     dofs, or keep track of which dofs you have done.
 */

#ifdef __cplusplus
} /*  extern "C"  */
#endif

#endif /* defined(iField_H) */
