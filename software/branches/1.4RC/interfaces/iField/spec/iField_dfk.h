#ifndef iField_dfk_h
#define iField_dfk_h
#include "iBase.h"
#include "iMesh.h"

/* The interface symbols defined here are ultimately intended to be
   propagated back into iField.h. Creating a iField_dfk.h was just
   a convenient way to focus on just this part of the iField interface.

   Special Note: We intend for the DFK interface to support
   interoperability among multiple iField interface instances even
   if those instances come from different iField implementations.
   Why? That way, we can have a library of DFKs that can be shared
   among implementations. Developing a DFK is a non-trivial task.
   Once done, we'd like to be able to share the code as needed.

   But, in practical terms, having iField interface instances within
   the same executable from different implementations is going to be
   the same issue we've had with iMesh. We get symbol collision unless
   we have either a) a multiplexor, b) some means for name mangling
   each implementation's API or c) use shared libraries AND can 
   ensure that symbols from one dlopen are kept separated from
   another. We already do the 'c)' approach in VisIt but ultimately,
   the symbols from each implementation are confined to different
   instances of C++ database reader objects in VisIt. I am not sure
   same can be achieved with pure C/Fortran because we don't have
   object namespaces in which to bury an implementation's symbols.

   To the extent possible (and practical), we'd like the DFK interface
   to support additions, deletions, changes to the interface as it
   evolves with time without breaking backward compatibility. In
   specific practical terms, the use case(s) we may want to consider
   are the following...

       a) Compile w/older headers but linked w/newer implementation
       b) Compile w/newer headers but linked w/older implementation

   In this DFK interface design, an instance of a given DFK is, a
   iField_DFKHandle that is, is embodied as an array of void* pointers.
   There are a few ways we can define the iField_DFKHandle type...

       typedef void* iField_DFKHandle[IMPL_SPECIFIC_MAX];
       typedef void* iField_DFKHandle[];
       typedef void** iField_DFKHandle;
       typedef void* iField_DFKHandle[MAX_OVER_ALL_POSSIBLE_IMPLS];

   The primary difference between the first and the other two is that
   the array size is specific. We can catch problems compiling across
   implementations at compile time if the SIZE of the list of member
   functions is changed. However, it also means that we cannot maintain
   either a) or b) compatibility above when the SIZE (e.g. number of
   interface methods to a DFK) is changed. So, that seems problematic.
   
   The pointers in an iField_DFKHandle point to different interface
   methods of a DFK. Each 'slot' in this array has a 'name' as determined
   by the enum iField_DfuncMemberFuncId. As the interface needs to evolve
   with time, this list needs to change. If we follow some simple rules
   on when and how we change this list, we can a) compatibility easily.
   We may be able to handle b) compatibility as well.

   Example 1...

            OLD                                       NEW
    IFIELD_DFK_EVAL_DFUNC=0,               IFIELD_DFK_EVAL_DFUNC=0,
    IFIELD_DFK_EVAL_GRAD=1,                IFIELD_DFK_EVAL_GRAD=1,
    IFIELD_DFK_EVAL_HESSIAN=2,             IFIELD_DFK_EVAL_HESSIAN=2,
    IFIELD_DFK_UNDEFINED=3                 IFIELD_DFK_EVAL_JACOBIAN=3,
                                           IFIELD_DFK_UNDEFINED=4

    In this example, one function has been added, extending the list.
    A client compiled with the OLD header will not know about the
    IFIELD_DFK_EVAL_JACOBIAN symbol. Since ITAPS interfaces use 'int'
    in place of actual enums as the argument type in function calls,
    we need not worry about the possibility that the compiler may
    change the size of the datatype used to implement the enum (also
    more than likely, we won't have enough 'slots' in a iField_DFKHandle
    to ever approach going from 8-bit to 16 bit for example).
    When the OLD client is linked to a NEW implementation of the DFK,
    all the OLD 'slots' will continue to function as normal. Of course,
    the OLD client cannot expect to use the IFIELD_DFK_EVAL_JACOBIAN 
    part of the interface. But, it shouldn't expect to either since it
    is linking with the OLD header file, it probably does so because
    it doesn't need anything from the NEW interface. So, a)
    compatibility is easily achieved.
    
    Example 2...

    What about the reverse? What if instead we have...
    (OLD is now on the right)

            NEW                                       OLD 
    IFIELD_DFK_EVAL_DFUNC=0,               IFIELD_DFK_EVAL_DFUNC=0,
    IFIELD_DFK_EVAL_GRAD=1,                IFIELD_DFK_EVAL_GRAD=1,
    IFIELD_DFK_EVAL_HESSIAN=2,             IFIELD_DFK_EVAL_HESSIAN=2,
    IFIELD_DFK_UNDEFINED=3                 IFIELD_DFK_EVAL_JACOBIAN=3,
                                           IFIELD_DFK_UNDEFINED=4

    In this case, compiling the client with the OLD DFK header file,
    the client knows about the IFIELD_DFK_EVAL_JACOBIAN symbol but the
    implementation does NOT. However, again this enum'd value is passed
    to the implementation as an int. So, if the implementation simply
    made sure to check either that the value falls within the range of
    values it knows about (3 in this case) or that the corresponding
    slot in the iField_DFKHandle is non-NULL, then again, b) type
    compatibility would be achieved. Again, this assumes that the OLD
    client did NOT actually NEED parts of the interface that are NOT
    available in the NEW implementation (e.g. the
    IFIELD_DFK_EVAL_JACOBIAN slot). If it did, it would get a run-time
    error and a message indicating it should link against a newer
    version of the interface.

    Example 3..

            OLD                                       NEW
    IFIELD_DFK_EVAL_DFUNC=0,               IFIELD_DFK_EVAL_DFUNC=0,
    IFIELD_DFK_EVAL_GRAD=1,                IFIELD_DFK_EVAL_GRAD=1,
    IFIELD_DFK_EVAL_HESSIAN=2,             IFIELD_DFK_EVAL_MOMENT2=2
    IFIELD_DFK_EVAL_JACOBIAN=3,            IFIELD_DFK_EVAL_JACOBIAN=3,
    IFIELD_DFK_UNDEFINED=4                 IFIELD_DFK_UNDEFINED=4

    In this case, in the NEW interface, we've replaced a method in the
    middle of the list (IFIELD_DFK_EVAL_MOMENT2=2). We have two problems.
    First, the symbol IFIELD_DFK_EVAL_HESSIAN, defined in the OLD header
    file is not actually known to the NEW implementation.  Also, '2' in
    the OLD and NEW header files means something different. In the old
    header file it means, hessian and in the new implementation it means
    2nd moment. Worse, if the function signatures of both methods are
    the same, the even the registration methods would not be able to
    catch this problem. However, if the functions signatures of the
    methods are different, then we'd run into compile-time problems
    anyhow and that is something we'd like to avoid if we can.

    All this means is that we cannot re-use slots in the iField_DFKHandle,
    at least not across releases we intend to maintain compatibility
    between. So, if we did indeed want to get rid of the hessian in
    the OLD, we'd set that slot to some pre-defined value to indicate
    it is no longer valid...

            OLD                                       NEW
    IFIELD_DFK_EVAL_DFUNC=0,               IFIELD_DFK_EVAL_DFUNC=0,
    IFIELD_DFK_EVAL_GRAD=1,                IFIELD_DFK_EVAL_GRAD=1,
    IFIELD_DFK_EVAL_HESSIAN=2,             IFIELD_DFK_EVAL_HESSIAN=IFIELD_DFK_INVALID,
    IFIELD_DFK_EVAL_JACOBIAN=3,            IFIELD_DFK_EVAL_JACOBIAN=3,
    IFIELD_DFK_UNDEFINED=4                 IFIELD_DFK_EVAL_MOMENT2=4,
                                           IFIELD_DFK_UNDEFINED=5

    In this way, a client compiled with the OLD header file passing '2'
    for the associated method in the NEW implementation will get an error
    because slot '2' is among the slots that are set to INVALD. Likewise,
    a NEW client using IFIELD_DFK_EVAL_HESSIAN will wind up passing
    IFIELD_DFK_INVALID to the OLD implementation where it can detect
    and flag this as an error.

    Upon a major release, we could opt to collapse out any unused slots
    in the iField_DFKHandle and enforce INCOMPATIBILITY across major
    releases using other means (such as already adopted version checks).

    So, the rules for maintaining compatibility are simple...
        1) Only add NEW functions to the end.
	2) Replace existing functions by invalidating the old
	   and adding a new.
        3) Remove existing functions by invalidating the old.

    If we decided to adopt a reasonable upper bound on the total number
    of methods in a DFK, say 256, we could then decide that an
    iField_DFKHandle always has 256 slots, most of which are empty.
    We don't expect to have very many of these things running around
    so the wasted memory should be insignificant.

    REGISTRATION AND DISCOVERY OF AVAILABLE DFK's

    Ok, so suppose an iField client is linked with several different
    DFK implementations, each implementing different DFKs. How does
    the client go about, 'finding' them, figuring out their attributes
    and deciding from among options, which to use?

    The simple answer is that the application programmer knows the
    compile-time symbols (e.g. C/C++/Fortran function names) of the
    DFK(s) s/he is interested in and then
    codes those symbols into the iField client, controlled perhaps by
    conditional compilation directives indicating their existence,
    at compile time. The application programmer is then responsible
    to link the application to a DFK implementation that implements 
    the specific symbols used. Such an approach certainly suffices in
    the short term.

    Longer term, however, iField application would query a 'central'
    place (e.g. a registrar) to discover all DFK's available (e.g.
    among all the DFK implementations the application was linked with).

    The simplest query mechanism could be based upon some string 'name'
    for a given DFK. For example, the application would query for
    'linear lagrange' DFK and the registrar would respond with
    an iField_DFKHandle or NULL if none can be found. This simple
    naming mechanism may suffice for a while. But, as the library
    of available DFK's grows, the query mechanism may need to include
    various attributes about DFK implementations (e.g. precision in
    which they are implemented -- float, quad, double, maybe whether
    they are implemented on GPU or not, bounding behavior, etc.).
    So, more generally, the query would involve a set of attributes
    of DFK's an application needs and the query result would be 
    all DFK's (currently linked into the application) that satisfy
    the needs.

    In this longer term implementation, the 'central' registrar
    would need to be defined, along with how a) DFK implementors
    register their DFKs with the registrar and b) how applications
    query the registrar for available DFK's. The act of simply
    linking to a given DFK library would have the effect of 
    registering all the DFK's defined in it with the registrar.

    DFKs AND PERSISTENT STORAGE (e.g. iField_save/iField_load).

    iField stores DOFs. When these DOFs are written to a file
    for another application to later use, the particular DFK's
    necessary to properly combine DOFs and evaluate the fields need
    to go along with them. But, for all intents and purposes,
    DFKs are executable code. Certainly, we do not intend to
    store executable code of a DFK (or even source code) to the
    iField files. So, how does persistent knowledge of the DFKs
    work?

    In order for an iField client to use a particular DFK, that
    client would need to have been linked with the DFK implementation
    or the client would need to know or be told at run time, a shared
    library in which to find the DFK implementation. The latter
    approach, using a shared library, is definitely nicer in that
    it does not require fore-knowledge of the DFKs to be linked
    with.

    So, if we can adopt a shared library approach to handling
    DFKs, when iField opens a file, the file can inform the
    iField instance which DFKs it needs by some sort of 
    persistent identification mechanism. From there, the iField
    implementation can then locate and dlopen the necessary shared
    libraries or inform the client that a) none are available or
    b) some other substitutes are being used.

    DFK COMPARISON

    The notion of substituting one DFK (that is available) with
    another that is not requires the ability to compare DFKs at
    some level of detail to determine if, indeed, for the purposes
    of the client's needs some suitable substitutes can be
    identified.

*/

/* Defined here temporarily and for convenience in specifying the API.
   Should be defined in iBase.h */
#define iBase_BAD_DFK 1001

/* Suggested interface for using array of void* function pointers as the
   'container' for all the functions associated with a Distribution
   Function Kernel */

/* Names of each 'slot' in the array of void* function pointers */
enum iField_DfuncMemberFuncId
{
    IFIELD_DFK_EVAL_DFUNC=0,
    IFIELD_DFK_EVAL_GRAD,
    IFIELD_DFK_EVAL_HESSIAN,
    IFIELD_DFK_GET_DOF_ASSOC,
    IFIELD_DFK_GET_PARAM_COORDS,
    IFIELD_DFK_GET_PHYS_COORDS,
    IFIELD_DFK_GET_JACOBIAN,
    IFIELD_DFK_GET_NAME,
    IFIELD_DFK_GET_CONTINUOUS_NESS,
    IFIELD_DFK_GET_DEGREE,
    IFIELD_DFK_UNDEFINED
};

/* Array of void* function pointers which acts as the DFK handle */ 
typedef void (*iField_DFuncKernel[IFIELD_DFK_UNDEFINED])();

/* convenience macro */
#define IFIELD_DFK_EVAL_SIGNATURE \
    /*in */ const int which_func,\
    /*in */ const double param_coords[3],\
    /*out*/ double *dfunc_value,\
    /*out*/ int *err\

/* Typedefs for member functions of a DFK */
typedef void (*evalDFunc_t)(IFIELD_DFK_EVAL_SIGNATURE);
typedef void (*evalGrad_t)(IFIELD_DFK_EVAL_SIGNATURE);
typedef void (*evalHessian_t)(IFIELD_DFK_EVAL_SIGNATURE);
typedef void (*getDOFAssoc_t)(
    /*in */ const int topo,
    /*out*/ int* dofs_per_vert,
    /*out*/ int* dofs_per_edge,
    /*out*/ int* dofs_per_face,
    /*out*/ int* dofs_per_region,
    /*out*/ int *err);
typedef void (*getParamCoords_t)(
    /*in */ const iMesh_Instance instance,
    /*in */ const iBase_EntityHandle entity,
    /*in */ const double phys_coords[3],
    /*in */ const int coord_type,
    /*out*/ double local_param_coords[3],
    /*out*/ int *err);
typedef void (*getPhysCoords_t)(
    /*in */ const iMesh_Instance instance,
    /*in */ const iBase_EntityHandle entity,
    /*in */ const double local_param_coords[3],
    /*in */ const int coord_type,
    /*out*/ double phys_coords[3],
    /*out*/ int *err);
typedef void (*getJacobian_t)(
    /*in */ const iMesh_Instance instance,
    /*in */ const iBase_EntityHandle entity,
    /*in */ const double local_param_coords[3],
    /*in */ const int coord_type,
    /*out*/ double* jacobian,
    /*out*/ int *err);
typedef void (*getName_t)(
    /*out*/ char *name,
    /*out*/ int *err,
    /*i/o*/ int *name_len);
typedef void (*getContinuous_ness_t)(
    /*out*/ int *continuous_ness,
    /*out*/ int *err);
typedef void (*getDegree_t)(
    /*out*/ int *degree,
    /*out*/ int *err);

/* Note the use of static function declarations AND DEFINITIONS here
   in the header file. I don't really like doing it this way but 
   the implementations are tiny and trivial and putting them here
   saves a client from having to compile and link to yet another library
   I guess we could make it such that the implementation (.c code)
   is provided and just gets compiled into any implementation too. */

/* Convenience functions for implementations to build DFKs.
   This provides type checked access to the void*-based dfk_handle.
   These methods are part of the iField public interface because they
   allow any iField client to build up its dfks.
   Note, the implementations are trivial. */
static void iField_setDfuncEvalFunc(iField_DFuncKernel dfk_handle,
				    evalDFunc_t func, int *err)
{
  dfk_handle[IFIELD_DFK_EVAL_DFUNC] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGradEvalFunc(iField_DFuncKernel dfk_handle,
				   evalGrad_t func, int *err)
{
  dfk_handle[IFIELD_DFK_EVAL_GRAD] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setHessianEvalFunc(iField_DFuncKernel dfk_handle,
				      evalHessian_t func, int *err)
{
  dfk_handle[IFIELD_DFK_EVAL_HESSIAN] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetDOFAssocFunc(iField_DFuncKernel dfk_handle,
				      getDOFAssoc_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_DOF_ASSOC] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetParamCoordsFunc(iField_DFuncKernel dfk_handle,
					 getParamCoords_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_PARAM_COORDS] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetPhysCoordsFunc(iField_DFuncKernel dfk_handle,
					getPhysCoords_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_PHYS_COORDS] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetJacobianFunc(iField_DFuncKernel dfk_handle,
				      getJacobian_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_JACOBIAN] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetNameFunc(iField_DFuncKernel dfk_handle,
				  getName_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_NAME] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetContinuousNessFunc(iField_DFuncKernel dfk_handle,
					    getContinuous_ness_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_CONTINUOUS_NESS] = (void(*)())(func); *err = iBase_SUCCESS;
}

static void iField_setGetDegreeFunc(iField_DFuncKernel dfk_handle,
				    getDegree_t func, int *err)
{
  dfk_handle[IFIELD_DFK_GET_DEGREE] = (void(*)())(func); *err = iBase_SUCCESS;
}

/* This one register function can serve to replace all those above. The
   only reason I don't really like this interface is that as we add
   more features (callbacks) to a DFK, the argument list gets longer
   and longer and eventually, totally unwieldly. And for functions that
   have the same type-signature, we won't be able detect if the caller
   somehow confused their order in the argument list. */
static void iField_registerDFK
(iField_DFuncKernel dfk_handle,
 evalDFunc_t func0, evalGrad_t func1, evalHessian_t func2,
 getDOFAssoc_t func3, getParamCoords_t func4, getParamCoords_t func5,
 getParamCoords_t func6, getName_t func7, getContinuous_ness_t func8,
 getDegree_t func9, int *err)
{
    dfk_handle[IFIELD_DFK_EVAL_DFUNC] = (void(*)())(func0);
    dfk_handle[IFIELD_DFK_EVAL_GRAD] = (void(*)())(func1);
    dfk_handle[IFIELD_DFK_EVAL_HESSIAN] = (void(*)())(func2);
    dfk_handle[IFIELD_DFK_GET_DOF_ASSOC] = (void(*)())(func3);
    dfk_handle[IFIELD_DFK_GET_PARAM_COORDS] = (void(*)())(func4);
    dfk_handle[IFIELD_DFK_GET_PHYS_COORDS] = (void(*)())(func5);
    dfk_handle[IFIELD_DFK_GET_JACOBIAN] = (void(*)())(func6);
    dfk_handle[IFIELD_DFK_GET_NAME] = (void(*)())(func7);
    dfk_handle[IFIELD_DFK_GET_CONTINUOUS_NESS] = (void(*)())(func8);
    dfk_handle[IFIELD_DFK_GET_DEGREE] = (void(*)())(func9);
    *err = iBase_SUCCESS;
}

/* These are the real functions any iField client will call to actually 
   perform any of the work the individual functions of a DFK do. In other
   words, an iField client won't ever attempt to dereference the void*
   pointers of a dfk_handle directly. It will do so, indirectly, through
   these functions. This is the interface to a DFK. Note, the implementation
   is trivial. */
static void iField_DFKEvalDfunc(const iField_DFuncKernel dfk_handle,
				IFIELD_DFK_EVAL_SIGNATURE)
{
  if (!dfk_handle[IFIELD_DFK_EVAL_DFUNC]) {*err = iBase_BAD_DFK; return;}
  (*((evalDFunc_t)(dfk_handle[IFIELD_DFK_EVAL_DFUNC])))
    (which_func, param_coords, dfunc_value, err);
}

static void iField_DFKEvalGrad(const iField_DFuncKernel dfk_handle,
			       IFIELD_DFK_EVAL_SIGNATURE)
{
  if (!dfk_handle[IFIELD_DFK_EVAL_GRAD]) {*err = iBase_BAD_DFK; return;}
  (*((evalGrad_t)(dfk_handle[IFIELD_DFK_EVAL_GRAD])))
        (which_func, param_coords, dfunc_value, err);
}

static void iField_DFKEvalHessian(const iField_DFuncKernel dfk_handle,
				  IFIELD_DFK_EVAL_SIGNATURE)
{
  if (!dfk_handle[IFIELD_DFK_EVAL_HESSIAN]) {*err = iBase_BAD_DFK; return;}
  (*((evalHessian_t)(dfk_handle[IFIELD_DFK_EVAL_HESSIAN])))
    (which_func, param_coords, dfunc_value, err);
}

static void iField_DFKGetDOFAssoc(const iField_DFuncKernel dfk_handle,
    /*in */ const int topo,
    /*out*/ int* dofs_per_vert,
    /*out*/ int* dofs_per_edge,
    /*out*/ int* dofs_per_face,
    /*out*/ int* dofs_per_region,
    /*out*/ int *err)
{
  if (!dfk_handle[IFIELD_DFK_GET_DOF_ASSOC]) {*err = iBase_BAD_DFK; return;}
  (*((getDOFAssoc_t)(dfk_handle[IFIELD_DFK_GET_DOF_ASSOC])))
    (topo, dofs_per_vert, dofs_per_edge, dofs_per_face, dofs_per_region, err);
}

static void iField_DFKGetParamCoords(const iField_DFuncKernel dfk_handle,
    /*in */ const iMesh_Instance instance,
    /*in */ const iBase_EntityHandle entity,
    /*in */ const double phys_coords[3],
    /*in */ const int coord_type,
    /*out*/ double local_param_coords[3],
    /*out*/ int *err)
{
  if (!dfk_handle[IFIELD_DFK_GET_PARAM_COORDS])
    {*err = iBase_BAD_DFK; return;}
  (*((getParamCoords_t)(dfk_handle[IFIELD_DFK_GET_PARAM_COORDS])))
    (instance, entity, phys_coords, coord_type, local_param_coords, err);
}

static void iField_DFKGetPhysCoords(const iField_DFuncKernel dfk_handle,
    /*in */ const iMesh_Instance instance,
    /*in */ const iBase_EntityHandle entity,
    /*in */ const double local_param_coords[3],
    /*in */ const int coord_type,
    /*out*/ double phys_coords[3],
    /*out*/ int *err)
{
  if (!dfk_handle[IFIELD_DFK_GET_PHYS_COORDS])
    {*err = iBase_BAD_DFK; return;}
  (*((getPhysCoords_t)(dfk_handle[IFIELD_DFK_GET_PHYS_COORDS])))
    (instance, entity, local_param_coords, coord_type, phys_coords, err);
}

static void iField_DFKGetJacobian(const iField_DFuncKernel dfk_handle,
    /*in */ const iMesh_Instance instance,
    /*in */ const iBase_EntityHandle entity,
    /*in */ const double local_param_coords[3],
    /*in */ const int coord_type,
    /*out*/ double *jacobian,
    /*out*/ int *err)
{
  if (!dfk_handle[IFIELD_DFK_GET_PARAM_COORDS])
    {*err = iBase_BAD_DFK; return;}
  (*((getJacobian_t)(dfk_handle[IFIELD_DFK_GET_PARAM_COORDS])))
    (instance, entity, local_param_coords, coord_type, jacobian, err);
}

static void iField_DFKGetName(const iField_DFuncKernel dfk_handle,
    /*out*/ char *name,
    /*out*/ int *err,
    /*i/o*/ int *name_len)
{ 
  if (!dfk_handle[IFIELD_DFK_GET_NAME]) {*err = iBase_BAD_DFK; return;}
  (*((getName_t)(dfk_handle[IFIELD_DFK_GET_NAME])))
    (name, err, name_len);
}

static void iField_DFKGetContinuousNess(iField_DFuncKernel dfk_handle,
    /*out*/ int *continuous_ness,
    /*out*/ int *err)
{
  if (!dfk_handle[IFIELD_DFK_GET_CONTINUOUS_NESS])
    {*err = iBase_BAD_DFK; return;}
  (*((getContinuous_ness_t)(dfk_handle[IFIELD_DFK_GET_CONTINUOUS_NESS])))
    (continuous_ness, err);
}

static void iField_DFKGetDegree(iField_DFuncKernel dfk_handle,
				/*out*/ int *degree,
				/*out*/ int *err)
{
  if (!dfk_handle[IFIELD_DFK_GET_DEGREE]) {*err = iBase_BAD_DFK; return;}
  (*((getDegree_t)(dfk_handle[IFIELD_DFK_GET_DEGREE])))
    (degree, err);
}

#endif
