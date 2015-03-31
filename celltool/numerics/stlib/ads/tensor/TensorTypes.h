// -*- C++ -*-

/*! 
  \file TensorTypes.h
  \brief A base class that defines types for all tensors.
*/

#if !defined(__ads_TensorTypes_h__)
#define __ads_TensorTypes_h__

#include "../defs.h"
#include <cstddef>

BEGIN_NAMESPACE_ADS

//! A base class that defines types for all tensors.
template<typename T>
class TensorTypes {
public:

  //
  // public types
  //

  //! The element type of the tensor.
  typedef T value_type;
  //! A pointer to a tensor element.
  typedef value_type* pointer;
  //! A const pointer to a tensor element.
  typedef const value_type* const_pointer;
  //! An iterator in the tensor.
  typedef value_type* iterator;
  //! A const iterator in the tensor.
  typedef const value_type* const_iterator;
  //! A reference to a tensor element.
  typedef value_type& reference;
  //! A const reference to a tensor element.
  typedef const value_type& const_reference;
  //! The size type.
  typedef int size_type;
  //! Pointer difference type.
  typedef std::ptrdiff_t difference_type;
  //! An index into the tensor.
  typedef int index_type;
};

END_NAMESPACE_ADS

#endif
