#include "babel_config.h"

#include "sidl_char_IOR.h"
#include "sidl_double_IOR.h"
#include "sidl_float_IOR.h"
#include "sidl_int_IOR.h"
#include "sidl_long_IOR.h"
#include "sidl_opaque_IOR.h"

#ifndef BABEL_ENUM_ARR_TYPE
#  if defined(BABEL_VERSION_HEX) && BABEL_VERSION_HEX >= 0x01040000
#    define BABEL_ENUM_ARR_TYPE long
#  else
#    define BABEL_ENUM_ARR_TYPE int
#  endif
#endif

/* Get pointer to array data storage */
#define ARRAY_MEM( array ) ((array) == NULL ? NULL : (array)->d_firstElement)

/* Get pointer to array data storage and cast to specified type */
#define ARRAY_PTR(array, type) (type*) ARRAY_MEM(array)

/* Get size of 1D array with stride of 1 */
#define ARRAY_SIZE(array) ((array) == NULL ? 0 : sidlUpper(array,0) - sidlLower(array,0) + 1)

/* Test if array has been allocated */
#define ARRAY_ALLOCATED(array) ((array) != NULL)

/* If array has been allocated, check that allocates size is sufficient.
   If array has not been allocated and we have data to return, allocate 
     the array and insert the data.  We allocate the array by calling
     create1d with a size of zero and then modifying the array to contain
     the correct data.  Be careful to call free on the initial data in the
     array because malloc(0) is allowed to return either a valid pointer
     that must be free'd or NULL. */
#define ASSIGN_ARRAY_GENERAL( sidl_type, data_type, array, name ) \
        if (ARRAY_ALLOCATED(*array)) { \
          if (ARRAY_SIZE(*array) < name ## _allocated_size) {\
            PROCESS_ERROR_MSG(iBase_BAD_ARRAY_SIZE, "Inout array of insufficient size."); \
          }\
        }\
        else if (name ## _allocated_size > 0) {\
          *array = sidl_ ## sidl_type ## __array_create1dInit( \
                           name ## _allocated_size, \
                           (data_type*) name ## _temp ); \
        }
#define ASSIGN_TYPED_ARRAY( sidl_type, data_type, array ) \
        ASSIGN_ARRAY_GENERAL( sidl_type, data_type, array, array )

static void assign_int_enum_array( int* tmp_array_ptr,
                                   int allocated_size,
                                   int occupied_size,
                                   struct sidl_int__array** array )
{
  if (!(ARRAY_ALLOCATED(*array))) {
    *array = sidl_int__array_create1dInit( occupied_size, tmp_array_ptr );
  }
}

static void assign_long_enum_array( int* tmp_array_ptr,
                                    int allocated_size,
                                    int occupied_size,
                                    struct sidl_long__array** array )
{
  long* ptr;
  int i;
  
  if (sizeof(int) == sizeof(long)) {
    assign_int_enum_array( tmp_array_ptr, allocated_size, occupied_size,
                           (struct sidl_int__array**)array );
  }
  else if (!(ARRAY_ALLOCATED(*array))) {
    *array = sidl_long__array_create1d( occupied_size );
    ptr = ARRAY_MEM(*array);
    for (i = 0; i < occupied_size; ++i)
      ptr[i] = tmp_array_ptr[i];
    free(tmp_array_ptr);
  }
  else {
    ptr = ARRAY_MEM(*array);
    for (i = occupied_size - 1; i >= 0; --i)
      ptr[i] = tmp_array_ptr[i];
  }
}

#define ASSIGN_ENUM_ARRAY__( A, name ) \
  if (ARRAY_ALLOCATED(*name) && ARRAY_SIZE(*name) < name ## _allocated_size) { \
    PROCESS_ERROR_MSG(iBase_BAD_ARRAY_SIZE, "Inout array of insufficient size."); \
  }\
  else { \
    assign_ ## A ## _enum_array( name ## _temp, \
                                 name ## _allocated_size, \
                                 *name ## _size, \
                                 (struct sidl_ ## A ## __array**)(name) ); \
  }

#define ASSIGN_ENUM_ARRAY_( A, array ) ASSIGN_ENUM_ARRAY__( A, array )
#define ASSIGN_ENUM_ARRAY( array ) ASSIGN_ENUM_ARRAY_( BABEL_ENUM_ARR_TYPE, array )

#define ASSIGN_OPAQUE_ARRAY( array ) ASSIGN_TYPED_ARRAY( opaque, void*, array )
#define ASSIGN_INT_ARRAY( array ) ASSIGN_TYPED_ARRAY( int, int32_t, array )
#define ASSIGN_DBL_ARRAY( array ) ASSIGN_TYPED_ARRAY( double, double, array )
#define ASSIGN_CHAR_ARRAY( array ) ASSIGN_TYPED_ARRAY( char, char, array )
#define ASSIGN_ARRAY( type, array ) ASSIGN_TYPED_ARRAY( type, void*, array )

#define ASSIGN_TAG_ARRAY( array ) ASSIGN_CHAR_ARRAY( array)

/* Delcare temporary variables for use with ASSIGN_ARRAY and TEMP_ARRAY_INOUT */
#define CREATE_TEMP_ARRAY_GENERAL( type, array, name ) \
        type *name ## _temp = ARRAY_PTR( *array, type ); \
        int name ## _allocated_size = ARRAY_SIZE(*array); 

#define CREATE_TEMP_ARRAY( type, array ) CREATE_TEMP_ARRAY_GENERAL( type, array, array )

#define CREATE_TEMP_ENUM_ARRAY__( A, name ) \
   int *name ## _temp = (sizeof(A) >= sizeof(int)) ? (ARRAY_PTR( *(struct sidl_ ## A ## __array**)name, int )) : 0; \
   int name ## _allocated_size = (sizeof(A) >= sizeof(int)) ? ARRAY_SIZE(*(struct sidl_ ## A ## __array**)name) : 0;
  
#define CREATE_TEMP_ENUM_ARRAY_( A, array ) CREATE_TEMP_ENUM_ARRAY__( A, array )
#define CREATE_TEMP_ENUM_ARRAY( array ) CREATE_TEMP_ENUM_ARRAY_( BABEL_ENUM_ARR_TYPE, array )

#define CREATE_TEMP_EH_ARRAY( array ) CREATE_TEMP_ARRAY( iBase_EntityHandle, array )
#define CREATE_TEMP_ESH_ARRAY( array ) CREATE_TEMP_ARRAY( iBase_EntitySetHandle, array )
#define CREATE_TEMP_TH_ARRAY( array ) CREATE_TEMP_ARRAY( iBase_TagHandle, array )
#define CREATE_TEMP_INT_ARRAY( array ) CREATE_TEMP_ARRAY( int32_t, array )
#define CREATE_TEMP_DBL_ARRAY( array ) CREATE_TEMP_ARRAY( double, array )
#define CREATE_TEMP_CHAR_ARRAY( array ) CREATE_TEMP_ARRAY( char, array )

#define CREATE_TEMP_TAG_ARRAY( array ) CREATE_TEMP_CHAR_ARRAY( array )

#define TEMP_ARRAY_INOUT( array ) &array ## _temp, &array ## _allocated_size, array ## _size

#define TEMP_EH_ARRAY_INOUT( array ) TEMP_ARRAY_INOUT( array )

#define TEMP_ARRAY_IN( array ) ARRAY_MEM( array ), array ## _size

#define TEMP_TYPED_ARRAY_IN( type, array ) (type*) TEMP_ARRAY_IN( array )

#define TEMP_TAG_ARRAY_IN( array ) ARRAY_MEM( array ), ARRAY_SIZE( array )
