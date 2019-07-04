! Interfaces for wrapper routines (derived from the header file(s) iMesh.h)
!

!
! This file is included so that name mangling will be handled
! consistently between C and Fortran 2003, and will match the local F77
! compiler.  Note that the syntax here explicitly assumes the use of
! the C preprocessor on this file (or whatever includes it).
!
!#include "iMesh_protos.h"

module iBase
  use iso_c_binding

  enum, bind(C)
  enumerator :: iBase_VERTEX = 0, iBase_EDGE, iBase_FACE, iBase_REGION, &
       iBase_ALL_TYPES
  end enum

  enum, bind(C)
    enumerator :: iBase_UNAVAILABLE = 0  ! Adjacency information not supported 
    enumerator iBase_ALL_ORDER_1         ! No more than local mesh traversal
    enumerator iBase_ALL_ORDER_LOGN      ! Global tree search 
    enumerator iBase_ALL_ORDER_N         ! Global exhaustive search 
    enumerator iBase_SOME_ORDER_1        ! Only some adjacency info, local
    enumerator iBase_SOME_ORDER_LOGN     ! Only some adjacency info, tree 
    enumerator iBase_SOME_ORDER_N        ! Only some adjacency info, exhaustive 
  end enum

  enum, bind(C)
  enumerator :: iBase_NEW = 0, iBase_ALREADY_EXISTED, iBase_CREATED_DUPLICATE, &
       iBase_CREATION_FAILED
  end enum

  enum, bind(C)
  enumerator :: iBase_SILENT, iBase_WARN_ONLY, iBase_THROW_ERROR
  end enum

  enum, bind(C)
  enumerator iBase_SUCCESS, iBase_MESH_ALREADY_LOADED, iBase_NO_MESH_DATA, &
       iBase_FILE_NOT_FOUND, iBase_FILE_WRITE_ERROR, iBase_NIL_ARRAY, &
       iBase_BAD_ARRAY_SIZE, iBase_BAD_ARRAY_DIMENSION, &
       iBase_INVALID_ENTITY_HANDLE, iBase_INVALID_ENTITY_COUNT, &
       iBase_INVALID_ENTITY_TYPE, iBase_INVALID_ENTITY_TOPOLOGY, &
       iBase_BAD_TYPE_AND_TOPO, iBase_ENTITY_CREATION_ERROR, &
       iBase_INVALID_TAG_HANDLE, iBase_TAG_NOT_FOUND, iBase_TAG_ALREADY_EXISTS, &
       iBase_TAG_IN_USE, iBase_INVALID_ENTITYSET_HANDLE, &
       iBase_INVALID_ITERATOR_HANDLE, iBase_INVALID_ARGUMENT, &
       iBase_MEMORY_ALLOCATION_FAILED, iBase_NOT_SUPPORTED, iBase_FAILURE
  end enum

  enum, bind(C)
  enumerator iBase_BLOCKED, iBase_INTERLEAVED
  end enum

  enum, bind(C)
  enumerator iBase_INTEGER, iBase_DOUBLE, iBase_ENTITY_HANDLE, iBase_BYTES
  end enum

end module iBase

module iMesh
  use iso_c_binding
  implicit none

  enum, bind(C)
    enumerator :: iMesh_POINT = 0   ! a general zero-dimensional entity  
    enumerator iMesh_LINE_SEGMENT   ! a general one-dimensional entity  
    enumerator iMesh_POLYGON        ! a general two-dimensional element  
    enumerator iMesh_TRIANGLE       ! a three-sided, two-dimensional element  
    enumerator iMesh_QUADRILATERAL  ! a four-sided, two-dimensional element  
    enumerator iMesh_POLYHEDRON     ! a general three-dimensional element 
    enumerator iMesh_TETRAHEDRON    ! a four-sided, three-dimensional
    !  element whose faces are triangles 
    enumerator iMesh_HEXAHEDRON     ! a six-sided, three-dimensional
    !  element whose faces are quadrilaterals 
    enumerator iMesh_PRISM          ! a five-sided, three-dimensional
    !  element which has three quadrilateral faces and two triangular faces  
    enumerator iMesh_PYRAMID        ! a five-sided, three-dimensional
    !  element which has one quadrilateral face and four triangular faces 
    enumerator iMesh_SEPTAHEDRON    ! a hexahedral entity with one
    !  collapsed edge 
    enumerator iMesh_ALL_TOPOLOGIES ! allows the user to request
    !  information about all the topology types  
  end enum
  
  interface

     subroutine iMesh_getErrorType (instance, error_type, err) 
       !,bind(C,name="iMesh_getGetErrorType") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(out) :: error_type
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getErrorType
     
     subroutine iMesh_getDescription ( instance, descr, err, descr_len ) 
       !,bind(C,name="iMesh_getDescription") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       character(len=descr_len), intent(inout) :: descr
       integer(c_int), intent(inout) :: err
       integer(c_int), intent(in), value :: descr_len
     end subroutine iMesh_getDescription
     
     subroutine iMesh_newMesh ( options, instance, err, options_len ) 
       !,bind(C,name="iMesh_newMesh") 
       use iso_c_binding
       character(len=options_len), intent(in) :: options
       type(c_ptr), intent(out) :: instance
       integer(c_int), intent(out) :: err
       integer(c_int), intent(in), value :: options_len
     end subroutine iMesh_newMesh
     
     subroutine iMesh_dtor ( instance, err ) 
       ! ,bind(C,name="iMesh_dtor") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(out) :: err
     end subroutine iMesh_dtor

     subroutine iMesh_load ( instance, entity_set_handle, name, options, err, name_len, options_len ) 
       ! ,bind(C,name="iMesh_load") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       character(len=name_len), intent(in) :: name
       character(len=options_len), intent(in) :: options
       integer(c_int), intent(out) :: err
       integer(c_int), intent(in), value :: name_len
       integer(c_int), intent(in), value :: options_len
     end subroutine iMesh_load

     subroutine iMesh_save ( instance, entity_set_handle, name, options, err, name_len, options_len ) 
       ! ,bind(C,name="iMesh_save") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       character(len=name_len), intent(in) :: name
       character(len=options_len), intent(in) :: options
       integer(c_int), intent(out) :: err
       integer(c_int), intent(in), value :: name_len
       integer(c_int), intent(in), value :: options_len
     end subroutine iMesh_save

     subroutine iMesh_getRootSet ( instance, root_set, err ) 
       ! ,bind(C,name="iMesh_getRootSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(out) :: root_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getRootSet

     subroutine iMesh_getGeometricDimension ( instance, geom_dim, err ) 
       ! ,bind(C,name="iMesh_getGeometricDimension") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(out) :: geom_dim
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getGeometricDimension

     subroutine iMesh_setGeometricDimension ( instance, geom_dim, err ) 
       ! ,bind(C,name="iMesh_setGeometricDimension") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(in), value :: geom_dim
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setGeometricDimension

     subroutine iMesh_getDfltStorage ( instance, order, err ) 
       ! ,bind(C,name="iMesh_getDfltStorage") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(out) :: order
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getDfltStorage

     subroutine iMesh_getAdjTable ( instance, adjacency_table, adjacency_table_allocated, adjacency_table_size, &
          err ) 
       ! ,bind(C,name="iMesh_getAdjTable") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(inout) :: adjacency_table
       integer(c_int), intent(inout) :: adjacency_table_allocated
       integer(c_int), intent(out) :: adjacency_table_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getAdjTable

     subroutine iMesh_getNumOfType ( instance, entity_set_handle, entity_type, num_type, err ) 
       ! ,bind(C,name="iMesh_getNumOfType") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: entity_type
       integer(c_int), intent(out) :: num_type
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNumOfType

     subroutine iMesh_getNumOfTopo ( instance, entity_set_handle, entity_topology, num_topo, &
          err ) 
       ! ,bind(C,name="iMesh_getNumOfTopo") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: entity_topology
       integer(c_int), intent(out) :: num_topo
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNumOfTopo

     subroutine iMesh_areEHValid ( instance, doReset, areHandlesInvariant, err ) 
       ! ,bind(C,name="iMesh_areEHValid") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(in), value :: doReset
       integer(c_int), intent(out) :: areHandlesInvariant
       integer(c_int), intent(out) :: err
     end subroutine iMesh_areEHValid

     subroutine iMesh_getEntities ( instance, entity_set_handle, entity_type, entity_topology, &
          entity_handles, entity_handles_allocated, entity_handles_size, err ) 
       ! ,bind(C,name="iMesh_getEntities") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: entity_type
       integer(c_int), intent(in), value :: entity_topology
       type(c_ptr), intent(inout) :: entity_handles
       integer(c_int), intent(inout) :: entity_handles_allocated
       integer(c_int), intent(out) :: entity_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntities

     subroutine iMesh_getVtxArrCoords ( instance, vertex_handles, vertex_handles_size, storage_order, &
          coords, coords_allocated, coords_size, err ) 
       ! ,bind(C,name="iMesh_getVtxArrCoords") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: vertex_handles
       integer(c_int), intent(in), value :: vertex_handles_size
       integer(c_int), intent(in), value :: storage_order
       type(c_ptr), intent(inout) :: coords
       integer(c_int), intent(inout) :: coords_allocated
       integer(c_int), intent(out) :: coords_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getVtxArrCoords

     subroutine iMesh_initEntArrIter ( instance, entity_set_handle, requested_entity_type, requested_entity_topology, &
          requested_array_size, entArr_iterator, err ) 
       ! ,bind(C,name="iMesh_initEntArrIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: requested_entity_type
       integer(c_int), intent(in), value :: requested_entity_topology
       integer(c_int), intent(in), value :: requested_array_size
       type(c_ptr), intent(out) :: entArr_iterator
       integer(c_int), intent(out) :: err
     end subroutine iMesh_initEntArrIter

     subroutine iMesh_getNextEntArrIter ( instance, entArr_iterator, entity_handles, entity_handles_allocated, &
          entity_handles_size, has_data, err ) 
       ! ,bind(C,name="iMesh_getNextEntArrIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entArr_iterator
       type(c_ptr), intent(inout) :: entity_handles
       integer(c_int), intent(inout) :: entity_handles_allocated
       integer(c_int), intent(out) :: entity_handles_size
       integer(c_int), intent(out) :: has_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNextEntArrIter

     subroutine iMesh_resetEntArrIter ( instance, entArr_iterator, err ) 
       ! ,bind(C,name="iMesh_resetEntArrIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entArr_iterator
       integer(c_int), intent(out) :: err
     end subroutine iMesh_resetEntArrIter

     subroutine iMesh_endEntArrIter ( instance, entArr_iterator, err ) 
       ! ,bind(C,name="iMesh_endEntArrIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entArr_iterator
       integer(c_int), intent(out) :: err
     end subroutine iMesh_endEntArrIter

     subroutine iMesh_getEntArrTopo ( instance, entity_handles, entity_handles_size, topology, topology_allocated, topology_size, &
          err ) 
       ! ,bind(C,name="iMesh_getEntArrTopo") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(inout) :: topology
       integer(c_int), intent(inout) :: topology_allocated
       integer(c_int), intent(out) :: topology_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntArrTopo

     subroutine iMesh_getEntArrType ( instance, entity_handles, entity_handles_size, type, type_allocated, type_size, &
          err ) 
       ! ,bind(C,name="iMesh_getEntArrType") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(inout) :: type
       integer(c_int), intent(inout) :: type_allocated
       integer(c_int), intent(out) :: type_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntArrType

     subroutine iMesh_getEntArrAdj ( instance, entity_handles, entity_handles_size, entity_type_requested, &
          adjacentEntityHandles, adjacentEntityHandles_allocated, adj_entity_handles_size, &
          offset, offset_allocated, offset_size, err ) 
       ! ,bind(C,name="iMesh_getEntArrAdj") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       integer(c_int), intent(in), value :: entity_type_requested
       type(c_ptr), intent(inout) :: adjacentEntityHandles
       integer(c_int), intent(inout) :: adjacentEntityHandles_allocated
       integer(c_int), intent(out) :: adj_entity_handles_size
       type(c_ptr), intent(inout) :: offset
       integer(c_int), intent(inout) :: offset_allocated
       integer(c_int), intent(out) :: offset_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntArrAdj

     subroutine iMesh_getEntArr2ndAdj ( instance, entity_handles, entity_handles_size, bridge_entity_type, requested_entity_type, &
          adj_entity_handles, adj_entity_handles_allocated, adj_entity_handles_size, &
          offset, offset_allocated, offset_size, err ) 
       ! ,bind(C,name="iMesh_getEntArr2ndAdj") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       integer(c_int), intent(in), value :: bridge_entity_type
       integer(c_int), intent(in), value :: requested_entity_type
       type(c_ptr), intent(inout) :: adj_entity_handles
       integer(c_int), intent(inout) :: adj_entity_handles_allocated
       integer(c_int), intent(out) :: adj_entity_handles_size
       type(c_ptr), intent(inout) :: offset
       integer(c_int), intent(inout) :: offset_allocated
       integer(c_int), intent(out) :: offset_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntArr2ndAdj

     subroutine iMesh_getAdjEntIndices ( instance, entity_set_handle, entity_type_requestor, entity_topology_requestor, &
          entity_type_requested, entity_handles, entity_handles_allocated, entity_handles_size, &
          adj_entity_handles, adj_entity_handles_allocated, adj_entity_handles_size, &
          adj_entity_indices, adj_entity_indices_allocated, adj_entity_indices_size, &
          offset, offset_allocated, offset_size, err ) 
       ! ,bind(C,name="iMesh_getAdjEntIndices") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: entity_type_requestor
       integer(c_int), intent(in), value :: entity_topology_requestor
       integer(c_int), intent(in), value :: entity_type_requested
       type(c_ptr), intent(inout) :: entity_handles
       integer(c_int), intent(inout) :: entity_handles_allocated
       integer(c_int), intent(out) :: entity_handles_size
       type(c_ptr), intent(inout) :: adj_entity_handles
       integer(c_int), intent(inout) :: adj_entity_handles_allocated
       integer(c_int), intent(out) :: adj_entity_handles_size
       type(c_ptr), intent(inout) :: adj_entity_indices
       integer(c_int), intent(inout) :: adj_entity_indices_allocated
       integer(c_int), intent(out) :: adj_entity_indices_size
       type(c_ptr), intent(inout) :: offset
       integer(c_int), intent(inout) :: offset_allocated
       integer(c_int), intent(out) :: offset_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getAdjEntIndices

     subroutine iMesh_createEntSet ( instance, isList, entity_set_created, err ) 
       ! ,bind(C,name="iMesh_createEntSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(in), value :: isList
       type(c_ptr), intent(out) :: entity_set_created
       integer(c_int), intent(out) :: err
     end subroutine iMesh_createEntSet

     subroutine iMesh_destroyEntSet ( instance, entity_set, err ) 
       ! ,bind(C,name="iMesh_destroyEntSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_destroyEntSet

     subroutine iMesh_isList ( instance, entity_set, is_list, err ) 
       ! ,bind(C,name="iMesh_isList") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(out) :: is_list
       integer(c_int), intent(out) :: err
     end subroutine iMesh_isList

     subroutine iMesh_getNumEntSets ( instance, entity_set_handle, num_hops, num_sets, err ) 
       ! ,bind(C,name="iMesh_getNumEntSets") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: num_hops
       integer(c_int), intent(out) :: num_sets
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNumEntSets

     subroutine iMesh_getEntSets ( instance, entity_set_handle, num_hops, &
          contained_set_handles, contained_set_handles_allocated, contained_set_handles_size, &
          err ) 
       ! ,bind(C,name="iMesh_getEntSets") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: num_hops
       type(c_ptr), intent(inout) :: contained_set_handles
       integer(c_int), intent(inout) :: contained_set_handles_allocated
       integer(c_int), intent(out) :: contained_set_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntSets

     subroutine iMesh_addEntToSet ( instance, entity_handle, entity_set, err ) 
       ! ,bind(C,name="iMesh_addEntToSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_addEntToSet

     subroutine iMesh_rmvEntFromSet ( instance, entity_handle, entity_set, err ) 
       ! ,bind(C,name="iMesh_rmvEntFromSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvEntFromSet

     subroutine iMesh_addEntArrToSet ( instance, entity_handles, entity_handles_size, entity_set, &
          err ) 
       ! ,bind(C,name="iMesh_addEntArrToSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_addEntArrToSet

     subroutine iMesh_rmvEntArrFromSet ( instance, entity_handles, entity_handles_size, entity_set, &
          err ) 
       ! ,bind(C,name="iMesh_rmvEntArrFromSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvEntArrFromSet

     subroutine iMesh_addEntSet ( instance, entity_set_to_add, entity_set_handle, err ) 
       ! ,bind(C,name="iMesh_addEntSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_to_add
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_addEntSet

     subroutine iMesh_rmvEntSet ( instance, entity_set_to_remove, entity_set_handle, err ) 
       ! ,bind(C,name="iMesh_rmvEntSet") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_to_remove
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvEntSet

     subroutine iMesh_isEntContained ( instance, containing_entity_set, contained_entity, is_contained, &
          err ) 
       ! ,bind(C,name="iMesh_isEntContained") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: containing_entity_set
       type(c_ptr), intent(in), value :: contained_entity
       integer(c_int), intent(out) :: is_contained
       integer(c_int), intent(out) :: err
     end subroutine iMesh_isEntContained

     subroutine iMesh_isEntArrContained ( instance, containing_set, entity_handles, num_entity_handles, &
          is_contained, is_contained_allocated, is_contained_size, err ) 
       ! ,bind(C,name="iMesh_isEntArrContained") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: containing_set
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: num_entity_handles
       type(c_ptr), intent(inout) :: is_contained
       integer(c_int), intent(inout) :: is_contained_allocated
       integer(c_int), intent(out) :: is_contained_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_isEntArrContained

     subroutine iMesh_isEntSetContained ( instance, containing_entity_set, contained_entity_set, is_contained, &
          err ) 
       ! ,bind(C,name="iMesh_isEntSetContained") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: containing_entity_set
       type(c_ptr), intent(in), value :: contained_entity_set
       integer(c_int), intent(out) :: is_contained
       integer(c_int), intent(out) :: err
     end subroutine iMesh_isEntSetContained

     subroutine iMesh_addPrntChld ( instance, parent_entity_set, child_entity_set, err ) 
       ! ,bind(C,name="iMesh_addPrntChld") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: parent_entity_set
       type(c_ptr), intent(in), value :: child_entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_addPrntChld

     subroutine iMesh_rmvPrntChld ( instance, parent_entity_set, child_entity_set, err ) 
       ! ,bind(C,name="iMesh_rmvPrntChld") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: parent_entity_set
       type(c_ptr), intent(in), value :: child_entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvPrntChld

     subroutine iMesh_isChildOf ( instance, parent_entity_set, child_entity_set, is_child, err ) 
       ! ,bind(C,name="iMesh_isChildOf") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: parent_entity_set
       type(c_ptr), intent(in), value :: child_entity_set
       integer(c_int), intent(out) :: is_child
       integer(c_int), intent(out) :: err
     end subroutine iMesh_isChildOf

     subroutine iMesh_getNumChld ( instance, entity_set, num_hops, num_child, err ) 
       ! ,bind(C,name="iMesh_getNumChld") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(in), value :: num_hops
       integer(c_int), intent(out) :: num_child
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNumChld

     subroutine iMesh_getNumPrnt ( instance, entity_set, num_hops, num_parent, err ) 
       ! ,bind(C,name="iMesh_getNumPrnt") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       integer(c_int), intent(in), value :: num_hops
       integer(c_int), intent(out) :: num_parent
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNumPrnt

     subroutine iMesh_getChldn ( instance, from_entity_set, num_hops, &
          entity_set_handles, entity_set_handles_allocated, entity_set_handles_size, err ) 
       ! ,bind(C,name="iMesh_getChldn") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: from_entity_set
       integer(c_int), intent(in), value :: num_hops
       type(c_ptr), intent(inout) :: entity_set_handles
       integer(c_int), intent(inout) :: entity_set_handles_allocated
       integer(c_int), intent(out) :: entity_set_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getChldn

     subroutine iMesh_getPrnts ( instance, from_entity_set, num_hops, &
          entity_set_handles, entity_set_handles_allocated, entity_set_handles_size, err ) 
       ! ,bind(C,name="iMesh_getPrnts") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: from_entity_set
       integer(c_int), intent(in), value :: num_hops
       type(c_ptr), intent(inout) :: entity_set_handles
       integer(c_int), intent(inout) :: entity_set_handles_allocated
       integer(c_int), intent(out) :: entity_set_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getPrnts

     subroutine iMesh_setVtxArrCoords ( instance, vertex_handles, vertex_handles_size, storage_order, &
          new_coords, new_coords_size, err ) 
       ! ,bind(C,name="iMesh_setVtxArrCoords") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: vertex_handles
       integer(c_int), intent(in), value :: vertex_handles_size
       integer(c_int), intent(in), value :: storage_order
       real(c_double), intent(in) :: new_coords
       integer(c_int), intent(in), value :: new_coords_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setVtxArrCoords

     subroutine iMesh_createVtxArr ( instance, num_verts, storage_order, new_coords, new_coords_size, &
          new_vertex_handles, new_vertex_handles_allocated, new_vertex_handles_size, err ) 
       ! ,bind(C,name="iMesh_createVtxArr") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(in), value :: num_verts
       integer(c_int), intent(in), value :: storage_order
       real(c_double), intent(in) :: new_coords
       integer(c_int), intent(in), value :: new_coords_size
       type(c_ptr), intent(inout) :: new_vertex_handles
       integer(c_int), intent(inout) :: new_vertex_handles_allocated
       integer(c_int), intent(out) :: new_vertex_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_createVtxArr

     subroutine iMesh_createEntArr ( instance, new_entity_topology, lower_order_entity_handles, lower_order_entity_handles_size, &
          new_entity_handles, new_entity_handles_allocated, new_entity_handles_size, status, status_allocated, status_size, &
          err ) 
       ! ,bind(C,name="iMesh_createEntArr") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(in), value :: new_entity_topology
       type(c_ptr), intent(in), value :: lower_order_entity_handles
       integer(c_int), intent(in), value :: lower_order_entity_handles_size
       type(c_ptr), intent(inout) :: new_entity_handles
       integer(c_int), intent(inout) :: new_entity_handles_allocated
       integer(c_int), intent(out) :: new_entity_handles_size
       type(c_ptr), intent(inout) :: status
       integer(c_int), intent(inout) :: status_allocated
       integer(c_int), intent(out) :: status_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_createEntArr

     subroutine iMesh_deleteEntArr ( instance, entity_handles, entity_handles_size, err ) 
       ! ,bind(C,name="iMesh_deleteEntArr") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_deleteEntArr

     subroutine iMesh_createTag ( instance, tag_name, tag_size, tag_type, tag_handle, &
          err, tag_name_len ) 
       ! ,bind(C,name="iMesh_createTag") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       character(len=tag_name_len), intent(in) :: tag_name
       integer(c_int), intent(in), value :: tag_size
       integer(c_int), intent(in), value :: tag_type
       type(c_ptr), intent(out) :: tag_handle
       integer(c_int), intent(out) :: err
       integer(c_int), intent(in), value :: tag_name_len
     end subroutine iMesh_createTag

     subroutine iMesh_destroyTag ( instance, tag_handle, forced, err ) 
       ! ,bind(C,name="iMesh_destroyTag") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(in), value :: forced
       integer(c_int), intent(out) :: err
     end subroutine iMesh_destroyTag

     subroutine iMesh_getTagName ( instance, tag_handle, name, err, name_len ) 
       ! ,bind(C,name="iMesh_getTagName") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: tag_handle
       character(len=name_len), intent(inout) :: name
       integer(c_int), intent(out) :: err
       integer(c_int), intent(in), value :: name_len
     end subroutine iMesh_getTagName

     subroutine iMesh_getTagSizeValues ( instance, tag_handle, tag_size, err ) 
       ! ,bind(C,name="iMesh_getTagSizeValues") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: tag_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getTagSizeValues

     subroutine iMesh_getTagSizeBytes ( instance, tag_handle, tag_size, err ) 
       ! ,bind(C,name="iMesh_getTagSizeBytes") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: tag_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getTagSizeBytes

     subroutine iMesh_getTagHandle ( instance, tag_name, tag_handle, err, tag_name_len ) 
       ! ,bind(C,name="iMesh_getTagHandle") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       character(len=tag_name_len), intent(in) :: tag_name
       type(c_ptr), intent(inout) :: tag_handle
       integer(c_int), intent(out) :: err
       integer(c_int), intent(in), value :: tag_name_len
     end subroutine iMesh_getTagHandle

     subroutine iMesh_getTagType ( instance, tag_handle, tag_type, err ) 
       ! ,bind(C,name="iMesh_getTagType") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: tag_type
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getTagType

     subroutine iMesh_setEntSetData ( instance, entity_set_handle, tag_handle, tag_value, tag_value_size, &
          err ) 
       ! ,bind(C,name="iMesh_setEntSetData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       type(c_ptr), intent(in), value :: tag_handle
       character(len=tag_value_size), intent(in) :: tag_value
       integer(c_int), intent(in), value :: tag_value_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setEntSetData

     subroutine iMesh_setEntSetIntData ( instance, entity_set, tag_handle, tag_value, &
          err ) 
       ! ,bind(C,name="iMesh_setEntSetIntData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(in), value :: tag_value
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setEntSetIntData

     subroutine iMesh_setEntSetDblData ( instance, entity_set, tag_handle, tag_value, &
          err ) 
       ! ,bind(C,name="iMesh_setEntSetDblData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       type(c_ptr), intent(in), value :: tag_handle
       real(c_double), intent(in), value :: tag_value
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setEntSetDblData

     subroutine iMesh_setEntSetEHData ( instance, entity_set, tag_handle, tag_value, &
          err ) 
       ! ,bind(C,name="iMesh_setEntSetEHData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(in), value :: tag_value
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setEntSetEHData

     subroutine iMesh_getEntSetData ( instance, entity_set_handle, tag_handle, tag_value, tag_value_allocated, tag_value_size, &
          err ) 
       ! ,bind(C,name="iMesh_getEntSetData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(inout) :: tag_value
       integer(c_int), intent(inout) :: tag_value_allocated
       integer(c_int), intent(out) :: tag_value_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntSetData

     subroutine iMesh_getEntSetIntData ( instance, entity_set, tag_handle, out_data, err ) 
       ! ,bind(C,name="iMesh_getEntSetIntData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: out_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntSetIntData

     subroutine iMesh_getEntSetDblData ( instance, entity_set, tag_handle, out_data, err ) 
       ! ,bind(C,name="iMesh_getEntSetDblData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       type(c_ptr), intent(in), value :: tag_handle
       real(c_double), intent(out) :: out_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntSetDblData

     subroutine iMesh_getEntSetEHData ( instance, entity_set, tag_handle, out_data, err ) 
       ! ,bind(C,name="iMesh_getEntSetEHData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(out) :: out_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntSetEHData

     subroutine iMesh_getAllEntSetTags ( instance, entity_set_handle, tag_handles, tag_handles_allocated, tag_handles_size, &
          err ) 
       ! ,bind(C,name="iMesh_getAllEntSetTags") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       type(c_ptr), intent(inout) :: tag_handles
       integer(c_int), intent(inout) :: tag_handles_allocated
       integer(c_int), intent(out) :: tag_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getAllEntSetTags

     subroutine iMesh_rmvEntSetTag ( instance, entity_set_handle, tag_handle, err ) 
       ! ,bind(C,name="iMesh_rmvEntSetTag") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvEntSetTag

     subroutine iMesh_setVtxCoord ( instance, vertex_handle, x, y, z, err ) 
       ! ,bind(C,name="iMesh_setVtxCoord") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: vertex_handle
       real(c_double), intent(in), value :: x
       real(c_double), intent(in), value :: y
       real(c_double), intent(in), value :: z
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setVtxCoord

     subroutine iMesh_createVtx ( instance, x, y, z, new_vertex_handle, err ) 
       ! ,bind(C,name="iMesh_createVtx") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       real(c_double), intent(in), value :: x
       real(c_double), intent(in), value :: y
       real(c_double), intent(in), value :: z
       type(c_ptr), intent(out) :: new_vertex_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_createVtx

     subroutine iMesh_createEnt ( instance, new_entity_topology, lower_order_entity_handles, lower_order_entity_handles_size, &
          new_entity_handle, status, err ) 
       ! ,bind(C,name="iMesh_createEnt") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       integer(c_int), intent(in), value :: new_entity_topology
       type(c_ptr), intent(in), value :: lower_order_entity_handles
       integer(c_int), intent(in), value :: lower_order_entity_handles_size
       type(c_ptr), intent(out) :: new_entity_handle
       integer(c_int), intent(out) :: status
       integer(c_int), intent(out) :: err
     end subroutine iMesh_createEnt

     subroutine iMesh_deleteEnt ( instance, entity_handle, err ) 
       ! ,bind(C,name="iMesh_deleteEnt") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_deleteEnt

     subroutine iMesh_getArrData ( instance, entity_handles, entity_handles_size, tag_handle, &
          tag_values, tag_values_allocated, tag_values_size, err ) 
       ! ,bind(C,name="iMesh_getArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(inout) :: tag_values
       integer(c_int), intent(inout) :: tag_values_allocated
       integer(c_int), intent(out) :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getArrData

     subroutine iMesh_getIntArrData ( instance, entity_handles, entity_handles_size, tag_handle, &
          tag_values, tag_values_allocated, tag_values_size, err ) 
       ! ,bind(C,name="iMesh_getIntArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(inout) :: tag_values
       integer(c_int), intent(inout) :: tag_values_allocated
       integer(c_int), intent(out) :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getIntArrData

     subroutine iMesh_getDblArrData ( instance, entity_handles, entity_handles_size, tag_handle, &
          tag_values, tag_values_allocated, tag_values_size, err ) 
       ! ,bind(C,name="iMesh_getDblArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(inout) :: tag_values
       integer(c_int), intent(inout) :: tag_values_allocated
       integer(c_int), intent(out) :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getDblArrData

     subroutine iMesh_getEHArrData ( instance, entity_handles, entity_handles_size, tag_handle, &
          tag_values, tag_value_allocated, tag_value_size, err ) 
       ! ,bind(C,name="iMesh_getEHArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(inout) :: tag_values
       integer(c_int), intent(inout) :: tag_value_allocated
       integer(c_int), intent(out) :: tag_value_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEHArrData

     subroutine iMesh_setArrData ( instance, entity_handles, entity_handles_size, tag_handle, tag_values, tag_values_size, &
          err ) 
       ! ,bind(C,name="iMesh_setArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       character(len=tag_values_size), intent(in) :: tag_values
       integer(c_int), intent(in), value :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setArrData

     subroutine iMesh_setIntArrData ( instance, entity_handles, entity_handles_size, tag_handle, tag_values, tag_values_size, &
          err ) 
       ! ,bind(C,name="iMesh_setIntArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(in) :: tag_values
       integer(c_int), intent(in), value :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setIntArrData

     subroutine iMesh_setDblArrData ( instance, entity_handles, entity_handles_size, tag_handle, tag_values, tag_values_size, &
          err ) 
       ! ,bind(C,name="iMesh_setDblArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       real(c_double), intent(in) :: tag_values
       integer(c_int), intent(in), value :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setDblArrData

     subroutine iMesh_setEHArrData ( instance, entity_handles, entity_handles_size, tag_handle, tag_values, tag_values_size, &
          err ) 
       ! ,bind(C,name="iMesh_setEHArrData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(in), value :: tag_values
       integer(c_int), intent(in), value :: tag_values_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setEHArrData

     subroutine iMesh_rmvArrTag ( instance, entity_handles, entity_handles_size, tag_handle, &
          err ) 
       ! ,bind(C,name="iMesh_rmvArrTag") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handles
       integer(c_int), intent(in), value :: entity_handles_size
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvArrTag

     subroutine iMesh_getData ( instance, entity_handle, tag_handle, tag_value, tag_value_allocated, tag_value_size, &
          err ) 
       ! ,bind(C,name="iMesh_getData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(inout) :: tag_value
       integer(c_int), intent(inout) :: tag_value_allocated
       integer(c_int), intent(out) :: tag_value_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getData

     subroutine iMesh_getIntData ( instance, entity_handle, tag_handle, out_data, err ) 
       ! ,bind(C,name="iMesh_getIntData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: out_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getIntData

     subroutine iMesh_getDblData ( instance, entity_handle, tag_handle, out_data, err ) 
       ! ,bind(C,name="iMesh_getDblData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       real(c_double), intent(out) :: out_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getDblData

     subroutine iMesh_getEHData ( instance, entity_handle, tag_handle, out_data, err ) 
       ! ,bind(C,name="iMesh_getEHData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(out) :: out_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEHData

     subroutine iMesh_setData ( instance, entity_handle, tag_handle, tag_value, tag_value_size, &
          err ) 
       ! ,bind(C,name="iMesh_setData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       character(len=tag_value_size), intent(in) :: tag_value
       integer(c_int), intent(in), value :: tag_value_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setData

     subroutine iMesh_setIntData ( instance, entity_handle, tag_handle, tag_value, err ) 
       ! ,bind(C,name="iMesh_setIntData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(in), value :: tag_value
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setIntData

     subroutine iMesh_setDblData ( instance, entity_handle, tag_handle, tag_value, err ) 
       ! ,bind(C,name="iMesh_setDblData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       real(c_double), intent(in), value :: tag_value
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setDblData

     subroutine iMesh_setEHData ( instance, entity_handle, tag_handle, tag_value, err ) 
       ! ,bind(C,name="iMesh_setEHData") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       type(c_ptr), intent(in), value :: tag_value
       integer(c_int), intent(out) :: err
     end subroutine iMesh_setEHData

     subroutine iMesh_getAllTags ( instance, entity_handle, tag_handles, tag_handles_allocated, tag_handles_size, &
          err ) 
       ! ,bind(C,name="iMesh_getAllTags") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(inout) :: tag_handles
       integer(c_int), intent(inout) :: tag_handles_allocated
       integer(c_int), intent(out) :: tag_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getAllTags

     subroutine iMesh_rmvTag ( instance, entity_handle, tag_handle, err ) 
       ! ,bind(C,name="iMesh_rmvTag") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       type(c_ptr), intent(in), value :: tag_handle
       integer(c_int), intent(out) :: err
     end subroutine iMesh_rmvTag

     subroutine iMesh_initEntIter ( instance, entity_set_handle, requested_entity_type, requested_entity_topology, &
          entity_iterator, err ) 
       ! ,bind(C,name="iMesh_initEntIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_handle
       integer(c_int), intent(in), value :: requested_entity_type
       integer(c_int), intent(in), value :: requested_entity_topology
       type(c_ptr), intent(out) :: entity_iterator
       integer(c_int), intent(out) :: err
     end subroutine iMesh_initEntIter

     subroutine iMesh_getNextEntIter ( instance, entity_iterator, entity_handle, has_data, &
          err ) 
       ! ,bind(C,name="iMesh_getNextEntIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_iterator
       type(c_ptr), intent(out) :: entity_handle
       integer(c_int), intent(out) :: has_data
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getNextEntIter

     subroutine iMesh_resetEntIter ( instance, entity_iterator, err ) 
       ! ,bind(C,name="iMesh_resetEntIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_iterator
       integer(c_int), intent(out) :: err
     end subroutine iMesh_resetEntIter

     subroutine iMesh_endEntIter ( instance, entity_iterator, err ) 
       ! ,bind(C,name="iMesh_endEntIter") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_iterator
       integer(c_int), intent(out) :: err
     end subroutine iMesh_endEntIter

     subroutine iMesh_getEntTopo ( instance, entity_handle, out_topo, err ) 
       ! ,bind(C,name="iMesh_getEntTopo") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       integer(c_int), intent(out) :: out_topo
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntTopo

     subroutine iMesh_getEntType ( instance, entity_handle, out_type, err ) 
       ! ,bind(C,name="iMesh_getEntType") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       integer(c_int), intent(out) :: out_type
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntType

     subroutine iMesh_getVtxCoord ( instance, vertex_handle, x, y, z, err ) 
       ! ,bind(C,name="iMesh_getVtxCoord") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: vertex_handle
       real(c_double), intent(out) :: x
       real(c_double), intent(out) :: y
       real(c_double), intent(out) :: z
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getVtxCoord

     subroutine iMesh_getEntAdj ( instance, entity_handle, entity_type_requested, adj_entity_handles, adj_entity_handles_allocated,&
          adj_entity_handles_size, err ) 
       ! ,bind(C,name="iMesh_getEntAdj")  
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       integer(c_int), intent(in), value :: entity_type_requested
       type(c_ptr), intent(inout) :: adj_entity_handles
       integer(c_int), intent(inout) :: adj_entity_handles_allocated
       integer(c_int), intent(out) :: adj_entity_handles_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEntAdj

     subroutine iMesh_getEnt2ndAdj ( instance, entity_handle, bridge_entity_type, requested_entity_type, adjacent_entities,&
          & adjacent_entities_allocated, adjacent_entities_size, err ) 
       ! ,bind(C,name="iMesh_getEnt2ndAdj")  
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_handle
       integer(c_int), intent(in), value :: bridge_entity_type
       integer(c_int), intent(in), value :: requested_entity_type
       type(c_ptr), intent(inout) :: adjacent_entities
       integer(c_int), intent(inout) :: adjacent_entities_allocated
       integer(c_int), intent(out) :: adjacent_entities_size
       integer(c_int), intent(out) :: err
     end subroutine iMesh_getEnt2ndAdj

     subroutine iMesh_subtract ( instance, entity_set_1, entity_set_2, result_entity_set, err ) 
       ! ,bind(C,name="iMesh_subtract")  
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_1
       type(c_ptr), intent(in), value :: entity_set_2
       type(c_ptr), intent(out) :: result_entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_subtract

     subroutine iMesh_intersect ( instance, entity_set_1, entity_set_2, result_entity_set, err ) 
       ! ,bind(C,name="iMesh_intersect") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_1
       type(c_ptr), intent(in), value :: entity_set_2
       type(c_ptr), intent(out) :: result_entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_intersect

     subroutine iMesh_unite ( instance, entity_set_1, entity_set_2, result_entity_set, err ) 
       ! ,bind(C,name="iMesh_unite") 
       use iso_c_binding
       type(c_ptr), intent(in), value :: instance
       type(c_ptr), intent(in), value :: entity_set_1
       type(c_ptr), intent(in), value :: entity_set_2
       type(c_ptr), intent(out) :: result_entity_set
       integer(c_int), intent(out) :: err
     end subroutine iMesh_unite
  end interface


  
  !
  ! Parameters (macros) from the C header file
  !
  integer(c_int), parameter :: IMESH_MAJOR_VERSION = 1 
  integer(c_int), parameter :: IMESH_MINOR_VERSION = 0 
  integer(c_int), parameter :: IMESH_PATCH_VERSION = 0 

end module iMesh
