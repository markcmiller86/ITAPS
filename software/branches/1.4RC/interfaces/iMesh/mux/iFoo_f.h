#ifndef IFOO_F_H
#define IFOO_F_H

#ifdef POINTER_SIZE
!  user-defined override
#  define IFOO_HANDLE_T integer*POINTER_SIZE
#elif defined(__SIZEOF_SIZE_T__)
!  gfortran
#  define IFOO_HANDLE_T integer*__SIZEOF_SIZE_T__
!  gfortran 4.2 and earlier
#elif defined(__GNUC__) && defined(_LANGUAGE_FORTRAN)
#  ifdef _LP64
#    define IFOO_HANDLE_T integer*8
#  else
!    NOTE: not integer*4 (see meaning of _LP64 in GNU docs)
#    define IFOO_HANDLE_T integer
#  endif
#elif defined(__INTEL_COMPILER)
#  ifdef __X86_64
#    define IFOO_HANDLE_T integer*8
#  else
#    define IFOO_HANDLE_T integer*4
#  endif
#elif defined(__XLCPP__) || defined(__bgp__) || defined(__bgq__)
! __XLCPP doesn't work for IBM. At least make things work for BGP/Q
#  define IFOO_HANDLE_T C_PTR
#else
#  error Unknown compiler. Please specify '-DPOINTER_SIZE=<4|8>' in FCFLAGS or F77FLAGS.
#endif

#define iFoo_Instance IFOO_HANDLE_T
#define iFoo_EntityHandle IFOO_HANDLE_T
#define iFoo_EntitySetHandle IFOO_HANDLE_T
#define iFoo_TagHandle IFOO_HANDLE_T
#define iFoo_EntityIterator IFOO_HANDLE_T
#define iFoo_EntityArrIterator IFOO_HANDLE_T

#endif

      integer  iFoo_ErrorType_MIN
      integer  iFoo_SUCCESS
      integer  iFoo_MESH_ALREADY_LOADED
      integer  iFoo_FILE_NOT_FOUND
      integer  iFoo_FILE_WRITE_ERROR
      integer  iFoo_NIL_ARRAY
      integer  iFoo_BAD_ARRAY_SIZE
      integer  iFoo_BAD_ARRAY_DIMENSION
      integer  iFoo_INVALID_ENTITY_HANDLE
      integer  iFoo_INVALID_ENTITY_COUNT
      integer  iFoo_INVALID_ENTITY_TYPE
      integer  iFoo_INVALID_ENTITY_TOPOLOGY
      integer  iFoo_BAD_TYPE_AND_TOPO
      integer  iFoo_ENTITY_CREATION_ERROR
      integer  iFoo_INVALID_TAG_HANDLE
      integer  iFoo_TAG_NOT_FOUND
      integer  iFoo_TAG_ALREADY_EXISTS
      integer  iFoo_TAG_IN_USE
      integer  iFoo_INVALID_ENTITYSET_HANDLE
      integer  iFoo_INVALID_ITERATOR_HANDLE
      integer  iFoo_INVALID_ARGUMENT
      integer  iFoo_MEMORY_ALLOCATION_FAILED
      integer  iFoo_NOT_SUPPORTED
      integer  iFoo_FAILURE
      integer  iFoo_ErrorType_MAX

      parameter(iFoo_ErrorType_MIN=0)
      parameter(iFoo_SUCCESS=0)
      parameter(iFoo_MESH_ALREADY_LOADED=1)
      parameter(iFoo_FILE_NOT_FOUND=2)
      parameter(iFoo_FILE_WRITE_ERROR=3)
      parameter(iFoo_NIL_ARRAY=4)
      parameter(iFoo_BAD_ARRAY_SIZE=5)
      parameter(iFoo_BAD_ARRAY_DIMENSION=6)
      parameter(iFoo_INVALID_ENTITY_HANDLE=7)
      parameter(iFoo_INVALID_ENTITY_COUNT=8)
      parameter(iFoo_INVALID_ENTITY_TYPE=9)
      parameter(iFoo_INVALID_ENTITY_TOPOLOGY=10)
      parameter(iFoo_BAD_TYPE_AND_TOPO=11)
      parameter(iFoo_ENTITY_CREATION_ERROR=12)
      parameter(iFoo_INVALID_TAG_HANDLE=13)
      parameter(iFoo_TAG_NOT_FOUND=14)
      parameter(iFoo_TAG_ALREADY_EXISTS=15)
      parameter(iFoo_TAG_IN_USE=16)
      parameter(iFoo_INVALID_ENTITYSET_HANDLE=17)
      parameter(iFoo_INVALID_ITERATOR_HANDLE=18)
      parameter(iFoo_INVALID_ARGUMENT=19)
      parameter(iFoo_MEMORY_ALLOCATION_FAILED=20)
      parameter(iFoo_NOT_SUPPORTED=21)
      parameter(iFoo_FAILURE=22)
      parameter(iFoo_ErrorType_MAX=22)
