#include <iFoo.h>

#include <stdlib.h>
#include <string.h>

class iFoo_EntityHandle_Private
{
friend class iFoo_Instance_Private;
    int index;
};

class iFoo_EntitySetHandle_Private
{
friend class iFoo_Instance_Private;
    int someInt;
};

class iFoo_Instance_Private
{
public:
    iFoo_Instance_Private(const char *, int) {
        name = strdup("iFooB");
        val = (int) 'B';
        dim = -1;
        nverts = 0;
        root.someInt = 0;
    };

    void getRootSet(iFoo_EntitySetHandle* root_set, int* err) {
        if (this->val != 'B') { *err = iFoo_FAILURE; return; }
        *root_set = &(this->root);
        *err = iFoo_SUCCESS;
    };
    void setGeometricDimension(int geom_dim, int* err) {
        if ((this->val != 'B') ||
            (geom_dim < 0 || geom_dim > 3))
        {
            *err = iFoo_FAILURE;
            return;
        }
        this->dim = geom_dim;
        *err = iFoo_SUCCESS;
    };
    void createVtx(double x, double y, double z,
        iFoo_EntityHandle *new_vertex_handle, int *err) {
        if (this->val != 'B') { *err = iFoo_FAILURE; return; }
        int n = this->nverts;

        this->coords[n*3+0] = x;
        this->coords[n*3+1] = y;
        this->coords[n*3+2] = z;
        this->nverts++;

        *new_vertex_handle = (iFoo_EntityHandle) malloc(sizeof(struct iFoo_EntityHandle_Private));
        (*new_vertex_handle)->index = n;

        *err = iFoo_SUCCESS;
    };

private:
    char *name;
    int val;
    int dim;
    struct iFoo_EntitySetHandle_Private root;
    int nverts;
    double coords[3*10];

    iFoo_Instance_Private() {;};
};

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
    iFoo_Instance_Private *retval;

    retval = new iFoo_Instance_Private(options, options_len);

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
    iFoo_Instance_Private inst = *instance;
    inst.getRootSet(root_set, err);
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
    iFoo_Instance_Private inst = *instance;
    inst.setGeometricDimension(geom_dim, err);
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
    iFoo_Instance_Private inst = *instance;
    inst.createVtx(x, y, z, new_vertex_handle, err);
}
