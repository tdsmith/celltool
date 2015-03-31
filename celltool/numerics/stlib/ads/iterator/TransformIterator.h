// -*- C++ -*-

/*! 
  \file TransformIterator.h
  \brief A transform iterator.
*/

#if !defined(__ads_TransformIterator_h__)
#define __ads_TransformIterator_h__

#include "AdaptedIterator.h"

#include "../functor/index.h"

BEGIN_NAMESPACE_ADS

//! A transform iterator.
/*!
  The dereferencing member function, operator*(), dereferences and then 
  applies the transformation.
*/
template<typename _Iterator, class _Transform>
class TransformIterator :
  public AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  typename _Transform::result_type,
  typename std::iterator_traits<_Iterator>::difference_type,
  void,
  void> {
private:

  //
  // Private types.
  //

  typedef AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  typename _Transform::result_type,
  typename std::iterator_traits<_Iterator>::difference_type,
  void,
  void> Base;

public:

  //
  // Public types.
  //

  //! The un-transformed iterator type.
  typedef typename Base::Iterator Iterator;
  //! The unary function that transforms the iterator.
  typedef _Transform Transform;

  // The following five types are required to be defined for any iterator.

  //! The iterator category.
  typedef typename Base::iterator_category iterator_category;
  //! The type "pointed to" by the iterator.
  typedef typename Base::value_type value_type;
  //! Distance between iterators is represented as this type.
  typedef typename Base::difference_type difference_type;
  //! This type represents a pointer-to-value_type.  It is not defined for this iterator.
  typedef typename Base::pointer pointer;
  //! This type represents a reference-to-value_type.  It is not defined for this iterator.
  typedef typename Base::reference reference;

private:
  
  //
  // Member data.
  //

  Transform _transform;

public:
  
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  TransformIterator() :
    Base(),
    _transform()
  {}

  //! Copy constructor.
  TransformIterator(const TransformIterator& x) :
    Base(x),
    _transform(x._transform)
  {}

  //! Assignment operator.
  TransformIterator& 
  operator=(const TransformIterator& x) {
    // For the sake of efficiency, don't assign the transform.
    Base::operator=(x);
    return *this;
  }

  //! Construct from an iterator.
  explicit
  TransformIterator(const Iterator& i) :
    Base(i),
    _transform()
  {}

  //! Construct from a transform.
  TransformIterator(const Transform& t) :
    Base(),
    _transform(t)
  {}

  //! Construct from an iterator and a transform
  TransformIterator(const Iterator& i, const Transform& t) :
    Base(i),
    _transform(t)
  {}

  //! Assignment from an iterator.
  TransformIterator& 
  operator=(const Iterator& i) {
    Base::operator=(i);
    return *this;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the base iterator.
  const Iterator&
  base() const {
    return Base::base();
  }

  //! Return the transform functor.
  const Transform&
  transform() const {
    return _transform;
  }

  //@}
  //--------------------------------------------------------------------------
  /*! \name Functionality
    \note There is no reasonable way to define operator->() because we don't 
    have a pointer type.
   */
  //@{

  //
  // Trivial iterator requirements.
  //

  //! Dereference.
  value_type
  operator*() const {
    return _transform(*Base::_iterator);
  }

  //
  // Forward iterator requirements.
  //

  //! Pre-increment.
  TransformIterator&
  operator++() {
    ++Base::_iterator;
    return *this;
  }

  //! Post-increment.  Warning: This is not as efficient as the pre-increment.
  TransformIterator
  operator++(int) {
    TransformIterator tmp = *this;
    ++Base::_iterator;
    return tmp;
  }

  //
  // Bi-directional iterator requirements.
  //

  //! Pre-decrement.
  TransformIterator&
  operator--() {
    --Base::_iterator;
    return *this;
  }

  //! Post-decrement.  Warning: This is not as efficient as the pre-decrement.
  TransformIterator
  operator--(int) {
    TransformIterator tmp = *this;
    --Base::_iterator;
    return tmp;
  }

  //
  // Random access iterator requirements.
  //

  //! Sub-scripting.
  value_type
  operator[](const difference_type& n) const {
    return _transform(Base::_iterator[n]);
  }

  //! Addition assignment.
  TransformIterator&
  operator+=(const difference_type& n) {
    Base::_iterator += n;
    return *this;
  }

  //! Addition.
  TransformIterator
  operator+(const difference_type& n) const {
    return TransformIterator(Base::_iterator + n);
  }

  //! Subtraction assignment.
  TransformIterator&
  operator-=(const difference_type& n) {
    Base::_iterator -= n;
    return *this;
  }

  //! Subtraction.
  TransformIterator
  operator-(const difference_type& n) const {
    return TransformIterator(Base::_iterator - n);
  }

  //@}
};

//
// Random access iterator requirements.
//

//! Offset from an iterator.
/*!
  \relates TransformIterator
*/
template<typename _Iterator, class _Transform>
inline
TransformIterator<_Iterator,_Transform>
operator+(typename TransformIterator<_Iterator,_Transform>::difference_type n,
	  const TransformIterator<_Iterator,_Transform>& x) {
  return x + n;
}

//! Convenience function for instantiating a TransformIterator.
/*!
  \relates TransformIterator
 */
template<typename _Iterator, class _Transform>
inline
TransformIterator<_Iterator,_Transform>
constructTransformIterator() {
  return TransformIterator<_Iterator,_Transform>();
}

//! Convenience function for instantiating a TransformIterator.
/*!
  \relates TransformIterator
 */
template<typename _Iterator, class _Transform>
inline
TransformIterator<_Iterator,_Transform>
constructTransformIterator
(const typename TransformIterator<_Iterator,_Transform>::Iterator& i) {
  return TransformIterator<_Iterator,_Transform>(i);
}

//! Convenience function for instantiating a TransformIterator.
/*!
  \relates TransformIterator
 */
template<typename _Iterator, class _Transform>
inline
TransformIterator<_Iterator,_Transform>
constructTransformIterator
(const typename TransformIterator<_Iterator,_Transform>::Transform& t) {
  return TransformIterator<_Iterator,_Transform>(t);
}

//! Convenience function for instantiating a TransformIterator.
/*!
  \relates TransformIterator
 */
template<typename _Iterator, class _Transform>
inline
TransformIterator<_Iterator,_Transform>
constructTransformIterator
(const typename TransformIterator<_Iterator,_Transform>::Iterator& i,
 const typename TransformIterator<_Iterator,_Transform>::Transform& t) {
  return TransformIterator<_Iterator,_Transform>(i, t);
}

//! Convenience function for instantiating a TransformIterator that indexes an array.
/*!
  \relates TransformIterator

  This changes an iterator on integers to an iterator over the values in 
  an array.
*/
template<typename IntIter, typename DataIter>
inline
TransformIterator<IntIter, IndexIterUnary<DataIter> >
constructArrayIndexingIterator(IntIter i, DataIter d) {
  IndexIterUnary<DataIter> f(d);
  return TransformIterator<IntIter, IndexIterUnary<DataIter> >(i, f);
}

END_NAMESPACE_ADS

#endif
