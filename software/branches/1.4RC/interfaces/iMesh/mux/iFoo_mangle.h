#ifndef IFOO_MANGLE_H
#define IFOO_MANGLE_H

#ifdef IFOO_MANGLE

/* types */
#define iFoo_Instance IFOO_MANGLE(iFoo_Instance)
#define iFoo_EntityHandle IFOO_MANGLE(iFoo_EntityHandle)
#define iFoo_EntitySetHandle IFOO_MANGLE(iFoo_EntitySetHandle)
#define iFoo_TagHandle IFOO_MANGLE(iFoo_TagHandle)
#define iFoo_EntityIterator IFOO_MANGLE(iFoo_EntityIterator)
#define iFoo_EntityArrIterator IFOO_MANGLE(iFoo_EntityArrIterator)

/* enums */
#define iFoo_ErrorType IFOO_MANGLE(iFoo_ErrorType)
#define iFoo_ErrorType_MIN IFOO_MANGLE(iFoo_ErrorType_MIN)
#define iFoo_SUCCESS IFOO_MANGLE(iFoo_SUCCESS)
#define iFoo_MESH_ALREADY_LOADED IFOO_MANGLE(iFoo_MESH_ALREADY_LOADED)
#define iFoo_FILE_NOT_FOUND IFOO_MANGLE(iFoo_FILE_NOT_FOUND)
#define iFoo_FILE_WRITE_ERROR IFOO_MANGLE(iFoo_FILE_WRITE_ERROR)
#define iFoo_NIL_ARRAY IFOO_MANGLE(iFoo_NIL_ARRAY)
#define iFoo_BAD_ARRAY_SIZE IFOO_MANGLE(iFoo_BAD_ARRAY_SIZE)
#define iFoo_BAD_ARRAY_DIMENSION IFOO_MANGLE(iFoo_BAD_ARRAY_DIMENSION)
#define iFoo_INVALID_ENTITY_HANDLE IFOO_MANGLE(iFoo_INVALID_ENTITY_HANDLE)
#define iFoo_INVALID_ENTITY_COUNT IFOO_MANGLE(iFoo_INVALID_ENTITY_COUNT)
#define iFoo_INVALID_ENTITY_TYPE IFOO_MANGLE(iFoo_INVALID_ENTITY_TYPE)
#define iFoo_INVALID_ENTITY_TOPOLOGY IFOO_MANGLE(iFoo_INVALID_ENTITY_TOPOLOGY)
#define iFoo_BAD_TYPE_AND_TOPO IFOO_MANGLE(iFoo_BAD_TYPE_AND_TOPO)
#define iFoo_ENTITY_CREATION_ERROR IFOO_MANGLE(iFoo_ENTITY_CREATION_ERROR)
#define iFoo_INVALID_TAG_HANDLE IFOO_MANGLE(iFoo_INVALID_TAG_HANDLE)
#define iFoo_TAG_NOT_FOUND IFOO_MANGLE(iFoo_TAG_NOT_FOUND)
#define iFoo_TAG_ALREADY_EXISTS IFOO_MANGLE(iFoo_TAG_ALREADY_EXISTS)
#define iFoo_TAG_IN_USE IFOO_MANGLE(iFoo_TAG_IN_USE)
#define iFoo_INVALID_ENTITYSET_HANDLE IFOO_MANGLE(iFoo_INVALID_ENTITYSET_HANDLE)
#define iFoo_INVALID_ITERATOR_HANDLE IFOO_MANGLE(iFoo_INVALID_ITERATOR_HANDLE)
#define iFoo_INVALID_ARGUMENT IFOO_MANGLE(iFoo_INVALID_ARGUMENT)
#define iFoo_MEMORY_ALLOCATION_FAILED IFOO_MANGLE(iFoo_MEMORY_ALLOCATION_FAILED)
#define iFoo_NOT_SUPPORTED IFOO_MANGLE(iFoo_NOT_SUPPORTED)
#define iFoo_FAILURE IFOO_MANGLE(iFoo_FAILURE)
#define iFoo_ErrorType_MAX IFOO_MANGLE(iFoo_ErrorType_MAX)

/* functions */
#define iFoo_newMesh IFOO_MANGLE(iFoo_newMesh)
#define iFoo_getRootSet IFOO_MANGLE(iFoo_getRootSet)
#define iFoo_setGeometricDimension IFOO_MANGLE(iFoo_setGeometricDimension)
#define iFoo_createVtx IFOO_MANGLE(iFoo_createVtx)

#elif defined(IFOO_MANGLE_F_LIB)

#define iFoo_newMesh IFOO_MANGLE_F_LIB(iFoo_newMesh)
#define iFoo_getRootSet IFOO_MANGLE_F_LIB(iFoo_getRootSet)
#define iFoo_setGeometricDimension IFOO_MANGLE_F_LIB(iFoo_setGeometricDimension)
#define iFoo_createVtx IFOO_MANGLE_F_LIB(iFoo_createVtx)

#elif defined(IFOO_MANGLE_F_APP)

#define iFoo_newMesh IFOO_MANGLE_F_APP( ifoo_newmesh, IFOO_NEWMESH )
#define iFoo_getRootSet IFOO_MANGLE_F_APP( ifoo_getrootset, IFOO_GETROOTSET )
#define iFoo_setGeometricDimension IFOO_MANGLE_F_APP( ifoo_setgeometricdimension, IFOO_SETGEOMETRICDIMENSION )
#define iFoo_createVtx IFOO_MANGLE_F_APP( ifoo_createvtx, IFOO_CREATEVTX )

#endif /* #ifdef IFOO_MANGLE */

#endif /* IFOO_MANGLE_H */
