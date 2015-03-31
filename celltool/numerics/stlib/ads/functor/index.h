// -*- C++ -*-

// CONTINUE: Consider giving all the classes a Functor suffix.

/*! 
  \file index.h
  \brief Contains functors for indexing iterators and objects.

  - ads::IndexIteratorFunctor is a binary functor for indexing an iterator.
  - ads::IndexIterUnary is a unary functor for indexing an iterator.
  - ads::IndexObject is a binary functor for indexing an object.
  - ads::index_iterator is a convenience function for constructing an
  ads::IndexIterator.
  - ads::index_iter_unary is a convenience function for constructing an
  ads::IndexIterUnary.
  - ads::index_object is a convenience function for constructing an
  ads::IndexObject.
*/

#if !defined(__ads_index_h__)
#define __ads_index_h__

#include "../defs.h"

#include <iterator>
#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_index Functor: Index */
// @{

//! Index the iterator.
template <typename RAIter>
class IndexIteratorFunctor : 
  public std::binary_function<RAIter, 
			      int,
			      typename std::iterator_traits
			      <RAIter>::reference> {
private:
  typedef std::binary_function<RAIter, 
			       int,
			       typename std::iterator_traits
			       <RAIter>::reference> Base;

public:

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //! Return \c x[i].
  result_type
  operator()(first_argument_type x, second_argument_type i) const {
    return x[i];
  }
};

//! Convenience function for constructing an \c IndexIterator.
template <typename RAIter> 
inline
IndexIteratorFunctor<RAIter>
index_iterator_functor() {
  return IndexIteratorFunctor<RAIter>();
}




//! Unary function for indexing an iterator.
template <typename RAIter>
class IndexIterUnary : 
  public std::unary_function<int,
			     typename std::iterator_traits
			     <RAIter>::reference> {
  //
  // Private types.
  //

private:

  typedef std::unary_function<int,
			      typename std::iterator_traits
			      <RAIter>::reference> Base;

  //
  // Public types.
  //

public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The iterator type.
  typedef RAIter iterator;

  //
  // Data.
  //

private:

  iterator _i;

  //
  // Not implemented.
  //

private:

  IndexIterUnary();  


  //
  // Constructors.
  //

public:

  //! Construct from an iterator.
  IndexIterUnary(iterator i) :
    _i(i)
  {}
 
  //
  // Functor call.
  //

  //! Return the n_th element of the array.
  result_type
  operator()(argument_type n) const {
    return _i[n];
  }
};

//! Convenience function for constructing an \c IndexIterator.
template <typename RAIter> 
inline
IndexIterUnary<RAIter>
index_iter_unary(RAIter i) {
  return IndexIterUnary<RAIter>(i);
}




//! Index the object.
template <typename Object>
class IndexObject : 
  public std::binary_function<Object, int, typename Object::reference> {
private:
  typedef std::binary_function<Object, int, typename Object::reference> 
  Base;

public:

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //! Return \c x[i].
  result_type
  operator()(first_argument_type& x, second_argument_type i) const {
    return x[i];
  }
};

//! Index a const object.
template <typename Object>
class IndexConstObject : 
  public std::binary_function<Object, int, typename Object::parameter_type> {
private:
  typedef std::binary_function<Object, int, typename Object::parameter_type>
  Base;

public:

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //! Return \c x[i].
  result_type
  operator()(const first_argument_type& x, second_argument_type i) const {
    return x[i];
  }
};

//! Convenience function for constructing an \c IndexObject.
template <typename Object>
inline
IndexObject<Object>
index_object() {
  return IndexObject<Object>();
}

//! Convenience function for constructing an \c IndexConstObject.
template <typename Object>
inline
IndexConstObject<Object>
index_const_object() {
  return IndexConstObject<Object>();
}

// @}

END_NAMESPACE_ADS

#endif
