#include <iFoo.h>

#include <stdlib.h>
#include <string.h>

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
    char *name;
    int val;
    int dim;
    struct iFoo_EntitySetHandle_Private root;
    int nverts;
    double coords[3*10];
};

/***************************************************************************//**
 * \ingroup  Initialization
 * \brief  Construct a new iMesh instance
 *
 ******************************************************************************/

void iFoo_newMesh(
    const char* options,
        /**< [in] Pointer to implementation-specific options string
             (\ref options) */
    iFoo_Instance* instance,
        /**< [in] iMesh instance handle */
    int* err,
        /**< [out] Returned Error status (see iFoo_ErrorType) */
    int options_len
        /**< [in] Length of the character string pointed to by options
             (\ref strlen) */
)
{
    struct iFoo_Instance_Private *retval;

    retval = (struct iFoo_Instance_Private *) malloc(sizeof(struct iFoo_Instance_Private));
    retval->name = strdup("iFooA");
    retval->val = (int) 'A';
    retval->dim = -1;
    retval->nverts = 0;
    retval->root.someInt = 0;

    *instance = retval;
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
    if (instance->val != 'A')
    {
        *err = iFoo_FAILURE;
        return;
    }

    /**root_set = (iFoo_EntitySetHandle) malloc(sizeof(struct iFoo_EntitySetHandle_Private));*/
    *root_set = &(instance->root);

    *err = iFoo_SUCCESS;
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
    if ((instance->val != 'A') ||
        (geom_dim < 0 || geom_dim > 3))
    {
        *err = iFoo_FAILURE;
        return;
    }

    instance->dim = geom_dim;
    *err = iFoo_SUCCESS;
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
    if (instance->val != 'A')
    {
        *err = iFoo_FAILURE;
        return;
    }

    int n = instance->nverts;

    instance->coords[n*3+0] = x;
    instance->coords[n*3+1] = y;
    instance->coords[n*3+2] = z;
    instance->nverts++;

    *new_vertex_handle = (iFoo_EntityHandle) malloc(sizeof(struct iFoo_EntityHandle_Private));
    (*new_vertex_handle)->index = n;

    *err = iFoo_SUCCESS;
}
