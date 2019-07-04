#include <iFoo_mangle.h>

#ifdef __cplusplus
extern "C" {
#endif

enum iFoo_ErrorType {
    iFoo_ErrorType_MIN = 0, /**< facilitates iteration over all values */
    iFoo_SUCCESS = iFoo_ErrorType_MIN,
    iFoo_MESH_ALREADY_LOADED,
    iFoo_FILE_NOT_FOUND,
    iFoo_FILE_WRITE_ERROR,
    iFoo_NIL_ARRAY,
    iFoo_BAD_ARRAY_SIZE,
    iFoo_BAD_ARRAY_DIMENSION,
    iFoo_INVALID_ENTITY_HANDLE,
    iFoo_INVALID_ENTITY_COUNT,
    iFoo_INVALID_ENTITY_TYPE,
    iFoo_INVALID_ENTITY_TOPOLOGY,
    iFoo_BAD_TYPE_AND_TOPO,
    iFoo_ENTITY_CREATION_ERROR,
    iFoo_INVALID_TAG_HANDLE,
    iFoo_TAG_NOT_FOUND,
    iFoo_TAG_ALREADY_EXISTS,
    iFoo_TAG_IN_USE,
    iFoo_INVALID_ENTITYSET_HANDLE,
    iFoo_INVALID_ITERATOR_HANDLE,
    iFoo_INVALID_ARGUMENT,
    iFoo_MEMORY_ALLOCATION_FAILED,
    iFoo_NOT_SUPPORTED,
    iFoo_FAILURE,
    iFoo_ErrorType_MAX = iFoo_FAILURE /**< facilitates iteration over all values */
};

typedef struct iFoo_Instance_Private* iFoo_Instance;
typedef struct iFoo_EntityHandle_Private* iFoo_EntityHandle;
typedef struct iFoo_EntitySetHandle_Private* iFoo_EntitySetHandle;
typedef struct iFoo_TagHandle_Private* iFoo_TagHandle;
typedef struct iFoo_EntityIterator_Private* iFoo_EntityIterator;
typedef struct iFoo_EntityArrIterator_Private* iFoo_EntityArrIterator;

/***************************************************************************//**
 * \ingroup  Initialization
 * \brief  Construct a new iFoo instance
 *
 ******************************************************************************/

void iFoo_newMesh(
    const char* options,
        /**< [in] Pointer to implementation-specific options string
             (\ref options) */
    iFoo_Instance* instance,
        /**< [in] iFoo instance handle */
    int* err,
        /**< [out] Returned Error status (see iFoo_ErrorType) */
    int options_len
        /**< [in] Length of the character string pointed to by options
             (\ref strlen) */
);


/***************************************************************************//**
 * \ingroup  Initialization
 * \brief  Get handle of the root set for this instance
 *
 * Get handle of the root set for this instance.  All mesh entities in
 * this instance can be accessed from this set.
 ******************************************************************************/

void iFoo_getRootSet(
    iFoo_Instance instance,
        /**< [in] iFoo instance handle */
    iFoo_EntitySetHandle* root_set,
        /**< [out] Pointer to set handle returned from function */
    int* err
        /**< [out] Returned Error status (see iFoo_ErrorType) */
);

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
        /**< [in] iFoo instance handle */
    int geom_dim,
        /**< [in] Requested geometric dimension. */
    int* err
        /**< [out] Returned Error status (see iFoo_ErrorType) */
);


/***************************************************************************//**
 * \ingroup  VertexEntities
 * \brief  Create a new vertex at specified coordinates
 *
 * Create a new vertex at specified coordinates.
 ******************************************************************************/

void iFoo_createVtx(
    iFoo_Instance instance,
        /**< [in] iFoo instance handle */
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
);

#ifdef __cplusplus
}
#endif
