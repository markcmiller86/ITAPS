#ifndef IBASE_F_H
#define IBASE_F_H

#ifdef POINTER_SIZE
#if POINTER_SIZE == 8
#define iBase_Instance integer*8
#define iBase_EntityHandle integer*8
#define iBase_EntitySetHandle integer*8
#define iBase_TagHandle integer*8
#define iBase_EntityIterator integer*8
#define iBase_EntityArrIterator integer*8
#elif POINTER_SIZE == 4
#define iBase_Instance integer*4
#define iBase_EntityHandle integer*4
#define iBase_EntitySetHandle integer*4
#define iBase_TagHandle integer*4
#define iBase_EntityIterator integer*4
#define iBase_EntityArrIterator integer*4
#else
#define iBase_Instance integer
#define iBase_EntityHandle integer
#define iBase_EntitySetHandle integer
#define iBase_TagHandle integer
#define iBase_EntityIterator integer
#define iBase_EntityArrIterator integer
#endif
#else
#define iBase_Instance integer
#define iBase_EntityHandle integer
#define iBase_EntitySetHandle integer
#define iBase_TagHandle integer
#define iBase_EntityIterator integer
#define iBase_EntityArrIterator integer
#endif
#endif

      integer  iBase_EntityType_MIN
      integer  iBase_VERTEX
      integer  iBase_EDGE
      integer  iBase_FACE
      integer  iBase_REGION
      integer  iBase_ALL_TYPES
      integer  iBase_EntityType_MAX

      integer  iBase_AdjacencyCost_MIN
      integer  iBase_UNAVAILABLE
      integer  iBase_ALL_ORDER_1
      integer  iBase_ALL_ORDER_LOGN
      integer  iBase_ALL_ORDER_N
      integer  iBase_SOME_ORDER_1
      integer  iBase_SOME_ORDER_LOGN
      integer  iBase_SOME_ORDER_N
      integer  iBase_AVAILABLE
      integer  iBase_AdjacencyCost_MAX

      integer  iBase_CreationStatus_MIN
      integer  iBase_NEW
      integer  iBase_ALREADY_EXISTED
      integer  iBase_CREATED_DUPLICATE
      integer  iBase_CREATION_FAILED
      integer  iBase_CreationStatus_MAX

      integer  iBase_ErrorType_MIN
      integer  iBase_SUCCESS
      integer  iBase_MESH_ALREADY_LOADED
      integer  iBase_FILE_NOT_FOUND
      integer  iBase_FILE_WRITE_ERROR
      integer  iBase_NIL_ARRAY
      integer  iBase_BAD_ARRAY_SIZE
      integer  iBase_BAD_ARRAY_DIMENSION
      integer  iBase_INVALID_ENTITY_HANDLE
      integer  iBase_INVALID_ENTITY_COUNT
      integer  iBase_INVALID_ENTITY_TYPE
      integer  iBase_INVALID_ENTITY_TOPOLOGY
      integer  iBase_BAD_TYPE_AND_TOPO
      integer  iBase_ENTITY_CREATION_ERROR
      integer  iBase_INVALID_TAG_HANDLE
      integer  iBase_TAG_NOT_FOUND
      integer  iBase_TAG_ALREADY_EXISTS
      integer  iBase_TAG_IN_USE
      integer  iBase_INVALID_ENTITYSET_HANDLE
      integer  iBase_INVALID_ITERATOR_HANDLE
      integer  iBase_INVALID_ARGUMENT
      integer  iBase_MEMORY_ALLOCATION_FAILED
      integer  iBase_NOT_SUPPORTED
      integer  iBase_FAILURE
      integer  iBase_ErrorType_MAX

      integer  iBase_StorageOrder_MIN
      integer  iBase_BLOCKED
      integer  iBase_INTERLEAVED
      integer  iBase_StorageOrder_MAX

      integer  iBase_TagValueType_MIN
      integer  iBase_BYTES
      integer  iBase_INTEGER
      integer  iBase_DOUBLE
      integer  iBase_ENTITY_HANDLE
      integer  iBase_ENTITY_SET_HANDLE
      integer  iBase_TagValueType_MAX

      parameter(iBase_EntityType_MIN=0)
      parameter(iBase_VERTEX=0)
      parameter(iBase_EDGE=1)
      parameter(iBase_FACE=2)
      parameter(iBase_REGION=3)
      parameter(iBase_ALL_TYPES=4)
      parameter(iBase_EntityType_MAX=4)

      parameter(iBase_AdjacencyCost_MIN=0)
      parameter(iBase_UNAVAILABLE=0)
      parameter(iBase_ALL_ORDER_1=1)
      parameter(iBase_ALL_ORDER_LOGN=2)
      parameter(iBase_ALL_ORDER_N=3)
      parameter(iBase_SOME_ORDER_1=4)
      parameter(iBase_SOME_ORDER_LOGN=5)
      parameter(iBase_SOME_ORDER_N=6)
      parameter(iBase_AVAILABLE=7)
      parameter(iBase_AdjacencyCost_MAX=7)

      parameter(iBase_CreationStatus_MIN=0)
      parameter(iBase_NEW=0)
      parameter(iBase_ALREADY_EXISTED=1)
      parameter(iBase_CREATED_DUPLICATE=2)
      parameter(iBase_CREATION_FAILED=3)
      parameter(iBase_CreationStatus_MAX=3)

      parameter(iBase_ErrorType_MIN=0)
      parameter(iBase_SUCCESS=0)
      parameter(iBase_MESH_ALREADY_LOADED=1)
      parameter(iBase_FILE_NOT_FOUND=2)
      parameter(iBase_FILE_WRITE_ERROR=3)
      parameter(iBase_NIL_ARRAY=4)
      parameter(iBase_BAD_ARRAY_SIZE=5)
      parameter(iBase_BAD_ARRAY_DIMENSION=6)
      parameter(iBase_INVALID_ENTITY_HANDLE=7)
      parameter(iBase_INVALID_ENTITY_COUNT=8)
      parameter(iBase_INVALID_ENTITY_TYPE=9)
      parameter(iBase_INVALID_ENTITY_TOPOLOGY=10)
      parameter(iBase_BAD_TYPE_AND_TOPO=11)
      parameter(iBase_ENTITY_CREATION_ERROR=12)
      parameter(iBase_INVALID_TAG_HANDLE=13)
      parameter(iBase_TAG_NOT_FOUND=14)
      parameter(iBase_TAG_ALREADY_EXISTS=15)
      parameter(iBase_TAG_IN_USE=16)
      parameter(iBase_INVALID_ENTITYSET_HANDLE=17)
      parameter(iBase_INVALID_ITERATOR_HANDLE=18)
      parameter(iBase_INVALID_ARGUMENT=19)
      parameter(iBase_MEMORY_ALLOCATION_FAILED=20)
      parameter(iBase_NOT_SUPPORTED=21)
      parameter(iBase_FAILURE=22)
      parameter(iBase_ErrorType_MAX=22)

      parameter(iBase_StorageOrder_MIN=0)
      parameter(iBase_BLOCKED=0)
      parameter(iBase_INTERLEAVED=1)
      parameter(iBase_StorageOrder_MAX=1)

      parameter(iBase_TagValueType_MIN=0)
      parameter(iBase_BYTES=0)
      parameter(iBase_INTEGER=1)
      parameter(iBase_DOUBLE=2)
      parameter(iBase_ENTITY_HANDLE=3)
      parameter(iBase_ENTITY_SET_HANDLE=4)
      parameter(iBase_TagValueType_MAX=4)

