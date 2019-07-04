#ifndef SIDL_UTIL_H
#define SIDL_UTIL_H

#define DEFINE_TO_SIDL_ARRAY( TYPE ) \
  static void to_sidl_##TYPE##_array( sidl_##TYPE##__array* sidl_array,    \
                                      TYPE* c_array, size_t c_array_len )  \
  {                                                                        \
    uint32_t lower = 0, upper = c_array_len - 1, stride = 1;               \
    sidl_##TYPE##__array_borrow( sidl_array, c_array, 1, &lower,           \
                                 &upper, &stride );                        \
  }

#define to_sidl_array( TYPE, SIDL_ARRAY, C_ARRAY, C_ARRAY_LEN ) \
  to_sidl_##TYPE##_array( (SIDL_ARRAY), (C_ARRAY), (C_ARRAY_LEN) ) 

#define to_c_array( TYPE, SIDL_ARRAY, EXPECTED_LEN )               \
  ((sidl_##TYPE##__array_dimen((SIDL_ARRAY)) == 1 &&               \
    sidl_##TYPE##__array_stride((SIDL_ARRAY)) == 1 &&              \
    sidl_##TYPE##__array_length((SIDL_ARRAY),0) >= (EXPECTED_LEN)) \
    ? (SIDL_ARRAY)->d_firstElement : 0)

DEFINE_TO_SIDL_ARRAY( int )

DEFINE_TO_SIDL_ARRAY( void* )

DEFINE_TO_SIDL_ARRAY( char )

DEFINE_TO_SIDL_ARRAY( double )  

#endif
