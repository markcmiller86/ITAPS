#ifndef SIDL_UTIL_HH
#define SIDL_UTIL_HH

#include "sidl.hh"

#include <vector>

template <typename T> static inline
sidl::array<T> to_sidl_array( T* c_array, size_t array_len )
{
  sidl::array<T> result;
  int32_t lower = 0, upper = array_len - 1, stride = 1;
  result.borrow( c_array, 1, &lower, &upper, &stride );
  return result;
}

template <typename T> static inline
sidl::array<T> to_sidl_array( std::vector<T>& vect )
  { to_sidl_array( &vect[0], vect.size() ); }

template <typename T> static inline
T* to_c_array( sidl::array<T>& array, size_t expected_len )
{
  if (array.dimen() != 1 || 
      array.stride() != 1 || 
      array.length(0) < expected_len)
    return 0;
  
  return array._get_ior()->d_firstElement;
}

#endif
