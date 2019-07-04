#include <iFoo.h>

#if HAVE_implc
#define IFOO_MANGLE(A) implc_ ## A
#include <iFoo.h>
#endif

#include <stdlib.h>
#include <string.h>

struct iFoo_mux_vtable
{

};

struct iFoo_EntityHandle_Private
{
    int index;
};

struct iFoo_EntitySetHandle_Private
{
    int someInt;
};

struct iFoo_Instance_Private
{
    char name[4]; /* always "MUX" */

    char implname[32];

    void *rinst; /* pointer to real instance handle */

    void (*newMesh)(const char *, iFoo_Instance*, int*, int);
    void (*getRootSet)(iFoo_Instance, iFoo_EntitySetHandle*, int*);
    void (*setGeometricDimension)(iFoo_Instance, int, int*);
    void (*createVtx)(iFoo_Instance, double, double, double, iFoo_EntityHandle*, int*);
};

#define IMPL_SETUP(N)								\	
    if (strstr(options, #N) != 0)						\
    {										\
        setup = 1;								\
        memset(retval->implname, '\0', sizeof(retval->implname));		\
        strncpy(retval->implname, #N, sizeof(retval->implname));		\
										\
        N##_iFoo_Instance *inst = (N##_iFoo_Instance*)				\
            malloc(sizeof(N##_iFoo_Instance));					\
        N##_iFoo_newMesh(options, inst, err, options_len);			\
        retval->rinst = inst;							\
 										\
        retval->newMesh = N##_iFoo_newMesh;					\
        retval->getRootSet = N##_iFoo_getRootSet;				\
        retval->setGeometricDimension = N##_iFoo_setGeometricDimension;		\
        retval->createVtx = N##_iFoo_createVtx;					\
    }


/***************************************************************************//**
 * \ingroup  Initialization
 * \brief  Construct a new iMesh instance
 *
 ******************************************************************************/

void iFoo_newMesh(
    const char* options,
        /**< [in] Pointer to implementation-specific options string (\ref options) */
    iFoo_Instance* instance,
        /**< [in] iMesh instance handle */
    int* err,
        /**< [out] Returned Error status (see iFoo_ErrorType) */
    int options_len
        /**< [in] Length of the character string pointed to by options
             (\ref strlen) */
)
{
    struct iFoo_Instance_Private *retval = (struct iFoo_Instance_Private*)
        malloc(sizeof(struct iFoo_Instance_Private));
    memset(retval->name, '\0', sizeof(retval->name));
    strncpy(retval->name, "MUX", sizeof(retval->name));
    int setup = 0;

#ifdef HAVE_implc
IMPL_SETUP(implc)
#endif
#ifdef HAVE_implC
IMPL_SETUP(implC)
#endif
#ifdef HAVE_implF77
IMPL_SETUP(implF77)
#endif

    if (setup != 1)
    {
        *err = iFoo_FAILURE;
        return;
    }

    *err = iFoo_SUCCESS;
}

/***************************************************************************//**
 * \ingroup  Initialization
 * \brief  Get handle of the root set for this instance
 *
 * Get handle of the root set for this instance.  All mesh entities in
 * this instance can be accessed from this set.
 ******************************************************************************/

void iFoo_getRootSet(
    iFoo_Instance instance,
        /**< [in] iMesh instance handle */
    iFoo_EntitySetHandle* root_set,
        /**< [out] Pointer to set handle returned from function */
    int* err
        /**< [out] Returned Error status (see iFoo_ErrorType) */
)
{
#ifdef HAVE_implc
    implc_getRootSet((implc_iFoo_Instance) instance,
                     (implc_iFoo_EntitySetHandle*) root_set,
                     err);
#endif
}

/***************************************************************************//**
 * \ingroup  Initialization
 * \brief  Set geometric dimension of vertex coordinates
 *
 * Set the geometric dimension of the mesh.  Note:  An application
 * should not expect this function to succeed unless the mesh instance
 * is empty. An empty mesh instance is any mesh instance in which there are 
 * no vertex entities.
 ******************************************************************************/

void iFoo_setGeometricDimension(
     iFoo_Instance instance,
        /**< [in] iMesh instance handle */
    int geom_dim,
        /**< [in] Requested geometric dimension. */
    int* err
        /**< [out] Returned Error status (see iFoo_ErrorType) */
)
{
    struct iFoo_mux_vtable *inst = instance->instab;
    inst->setGeometricDimension(inst->real_instance, geom_dim, err);
}


/***************************************************************************//**
 * \ingroup  VertexEntities
 * \brief  Create a new vertex at specified coordinates
 *
 * Create a new vertex at specified coordinates.
 ******************************************************************************/

void iFoo_createVtx(
    iFoo_Instance instance,
        /**< [in] iMesh instance handle */
    const double x,
        /**< [in] x coordinate of new vertex */
    const double y,
        /**< [in] y coordinate of new vertex */
    const double z,
        /**< [in] z coordinate of new vertex */
    iFoo_EntityHandle* new_vertex_handle,
        /**< [out] Pointer to new vertex handles returned from  */
    int* err
        /**< [out] Returned Error status (see iFoo_ErrorType) */
)
{
    struct iFoo_mux_vtable *inst = instance->instab;
    inst->createVtx(inst->real_instance, x, y, z, new_vertex_handle, err);
}
