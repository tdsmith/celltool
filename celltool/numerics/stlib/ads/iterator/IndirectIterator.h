// -*- C++ -*-

/*! 
  \file IndirectIterator.h
  \brief An iterator that performs two dereferences in \c operator*().
*/

#if !defined(__ads_IndirectIterator_h__)
#define __ads_IndirectIterator_h__

#include "AdaptedIterator.h"

BEGIN_NAMESPACE_ADS

//! An iterator that performs two dereferences in \c operator*().
/*!
  This is a double-dereferencing iterator.  This is useful when you have
  an array of pointers to widgets that you would like to treat as an 
  array of widgets.
*/
template<typename _Iterator>
class IndirectIterator :
  public AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  typename std::iterator_traits< 
  typename std::iterator_traits<_Iterator>::value_type >::value_type,
  typename std::iterator_traits<_Iterator>::difference_type,
  /*
  typename std::iterator_traits< 
  typename std::iterator_traits<Iterator>::value_type >::pointer,
  */
  typename std::iterator_traits<_Iterator>::value_type,
  typename std::iterator_traits< 
  typename std::iterator_traits<_Iterator>::value_type >::reference> {
private:

  //
  // Private types.
  //

  typedef AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  typename std::iterator_traits< 
    typename std::iterator_traits<_Iterator>::value_type >::value_type,
  typename std::iterator_traits<_Iterator>::difference_type,
  typename std::iterator_traits<_Iterator>::value_type,
  typename std::iterator_traits< 
    typename std::iterator_traits<_Iterator>::value_type >::reference>
  Base;

public:

  //
  // Public types.
  //

  //! The base iterator type.
  typedef typename Base::Iterator Iterator;

  // The following five types are required to be defined for any iterator.

  //! The iterator category.
  typedef typename Base::iterator_category iterator_category;
  //! The type "pointed to" by the iterator.
  typedef typename Base::value_type value_type;
  //! Distance between iterators is represented as this type.
  typedef typename Base::difference_type difference_type;
  //! This type represents a pointer-to-value_type.
  typedef typename Base::pointer pointer;
  //! This type represents a reference-to-value_type.
  typedef typename Base::reference reference;

public:
  
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  IndirectIterator() :
    Base()
  {}

  //! Copy constructor.
  IndirectIterator(const IndirectIterator& other) :
    Base(other)
  {}

  //! Assignment operator.
  IndirectIterator& 
  operator=(const IndirectIterator& other) {
    Base::operator=(other);
    return *this;
  }

  //! Construct from an iterator.
  explicit
  IndirectIterator(const Iterator& i) :
    Base(i)
  {}

  //! Assignment from an iterator.
  IndirectIterator& 
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

  //@}
  //--------------------------------------------------------------------------
  //! \name Iterator Functionality.
  //@{

  //
  // Trivial iterator requirements.
  //

  //! Double dereference of the base iterator.
  reference
  operator*() const {
    return **Base::_iterator;
  }

  //! Single dereference of the base iterator.
  pointer
  operator->() const {
    return *Base::_iterator;
  }

  //
  // Forward iterator requirements.
  //

  //! Pre-increment.
  IndirectIterator&
  operator++() {
    ++Base::_iterator;
    return *this;
  }

  //! Post-increment.  Warning: This is not as efficient as the pre-increment.
  IndirectIterator
  operator++(int) {
    IndirectIterator tmp = *this;
    ++Base::_iterator;
    return tmp;
  }

  //
  // Bi-directional iterator requirements.
  //

  //! Pre-decrement.
  IndirectIterator&
  operator--() {
    --Base::_iterator;
    return *this;
  }

  //! Post-decrement.  Warning: This is not as efficient as the pre-decrement.
  IndirectIterator
  operator--(int) {
    IndirectIterator tmp = *this;
    --Base::_iterator;
    return tmp;
  }

  //
  // Random access iterator requirements.
  //

  //! Sub-scripting.
  value_type
  operator[](const difference_type& n) const {
    return *Base::_iterator[n];
  }

  //! Addition assignment.
  IndirectIterator&
  operator+=(const difference_type& n) {
    Base::_iterator += n;
    return *this;
  }

  //! Addition.
  IndirectIterator
  operator+(const difference_type& n) const {
    return IndirectIterator(Base::_iterator + n);
  }

  //! Subtraction assignment.
  IndirectIterator&
  operator-=(const difference_type& n) {
    Base::_iterator -= n;
    return *this;
  }

  //! Subtraction.
  IndirectIterator
  operator-(const difference_type& n) const {
    return IndirectIterator(Base::_iterator - n);
  }

  //@}
};

//
// Random access iterator requirements.
//

//! Offset from an iterator.
/*!
  \relates IndirectIterator
 */
template<typename _Iterator>
inline
IndirectIterator<_Iterator>
operator+(typename IndirectIterator<_Iterator>::difference_type n,
	  const IndirectIterator<_Iterator>& x) {
  return x + n;
}

//! Convenience function for instantiating a IndirectIterator.
/*!
  \relates IndirectIterator
 */
template<typename _Iterator>
inline
IndirectIterator<_Iterator>
constructIndirectIterator() {
  return IndirectIterator<_Iterator>();
}

//! Convenience function for instantiating a IndirectIterator.
/*!
  \relates IndirectIterator
 */
template<typename _Iterator>
inline
IndirectIterator<_Iterator>
constructIndirectIterator(const _Iterator& i) {
  return IndirectIterator<_Iterator>(i);
}











//! An iterator that performs three dereferences in \c operator*().
/*!
  This is a triple-dereferencing iterator.  This is useful when you have
  an array of pointers to pointers to widgets that you would like to treat 
  as an array of widgets.
*/
template<typename _Iterator>
class IndirectIterator2 :
  public AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  typename std::iterator_traits<
  typename std::iterator_traits<
  typename std::iterator_traits<_Iterator>::value_type>::value_type >::value_type,
  typename std::iterator_traits<_Iterator>::difference_type,
  typename std::iterator_traits< 
  typename std::iterator_traits<
  typename std::iterator_traits<_Iterator>::value_type>::value_type >::pointer,
  typename std::iterator_traits< 
  typename std::iterator_traits<
  typename std::iterator_traits<_Iterator>::value_type>::value_type >::reference>
{
private:

  //
  // Private types.
  //

  typedef AdaptedIterator<
  _Iterator,
    typename std::iterator_traits<_Iterator>::iterator_category,
    typename std::iterator_traits<
    typename std::iterator_traits<
    typename std::iterator_traits<_Iterator>::value_type>::value_type >::value_type,
    typename std::iterator_traits<_Iterator>::difference_type,
    typename std::iterator_traits< 
    typename std::iterator_traits<
    typename std::iterator_traits<_Iterator>::value_type>::value_type >::pointer,
    typename std::iterator_traits< 
    typename std::iterator_traits<
    typename std::iterator_traits<_Iterator>::value_type>::value_type >::reference>
  Base;

public:

  //
  // Public types.
  //

  //! The base iterator type.
  typedef typename Base::Iterator Iterator;

  // The following five types are required to be defined for any iterator.

  //! The iterator category.
  typedef typename Base::iterator_category iterator_category;
  //! The type "pointed to" by the iterator.
  typedef typename Base::value_type value_type;
  //! Distance between iterators is represented as this type.
  typedef typename Base::difference_type difference_type;
  //! This type represents a pointer-to-value_type.
  typedef typename Base::pointer pointer;
  //! This type represents a reference-to-value_type.
  typedef typename Base::reference reference;

public:
  
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  IndirectIterator2() :
    Base()
  {}

  //! Copy constructor.
  IndirectIterator2(const IndirectIterator2& x) :
    Base(x)
  {}

  //! Assignment operator.
  IndirectIterator2& 
  operator=(const IndirectIterator2& other) {
    Base::operator=(other);
    return *this;
  }

  //! Construct from an iterator.
  explicit
  IndirectIterator2(const Iterator& i) :
    Base(i)
  {}

  //! Assignment from an iterator.
  IndirectIterator2& 
  operator=(const Iterator& other) {
    Base::operator=(other);
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

  //@}
  //--------------------------------------------------------------------------
  //! \name Iterator Functionality.
  //@{

  //
  // Trivial iterator requirements.
  //

  //! Triple dereference of the base iterator.
  reference
  operator*() const {
    return ***Base::_iterator;
  }

  //! Double dereference of the base iterator.
  pointer
  operator->() const {
    return **Base::_iterator;
  }

  //
  // Forward iterator requirements.
  //

  //! Pre-increment.
  IndirectIterator2&
  operator++() {
    ++Base::_iterator;
    return *this;
  }

  //! Post-increment.  Warning: This is not as efficient as the pre-increment.
  IndirectIterator2
  operator++(int) {
    IndirectIterator2 tmp = *this;
    ++Base::_iterator;
    return tmp;
  }

  //
  // Bi-directional iterator requirements.
  //

  //! Pre-decrement.
  IndirectIterator2&
  operator--() {
    --Base::_iterator;
    return *this;
  }

  //! Post-decrement.  Warning: This is not as efficient as the pre-decrement.
  IndirectIterator2
  operator--(int) {
    IndirectIterator2 tmp = *this;
    --Base::_iterator;
    return tmp;
  }

  //
  // Random access iterator requirements.
  //

  //! Sub-scripting.
  value_type
  operator[](const difference_type& n) const {
    return **Base::_iterator[n];
  }

  //! Addition assignment.
  IndirectIterator2&
  operator+=(const difference_type& n) {
    Base::_iterator += n;
    return *this;
  }

  //! Addition.
  IndirectIterator2
  operator+(const difference_type& n) const {
    return IndirectIterator2(Base::_iterator + n);
  }

  //! Subtraction assignment.
  IndirectIterator2&
  operator-=(const difference_type& n) {
    Base::_iterator -= n;
    return *this;
  }

  //! Subtraction.
  IndirectIterator2
  operator-(const difference_type& n) const {
    return IndirectIterator2(Base::_iterator - n);
  }

  //@}
};

//
// Random access iterator requirements.
//

//! Offset from an iterator.
/*!
  \relates IndirectIterator2
 */
template<typename _Iterator>
inline
IndirectIterator2<_Iterator>
operator+(typename IndirectIterator2<_Iterator>::difference_type n,
	  const IndirectIterator2<_Iterator>& x) {
  return x + n;
}

//! Convenience function for instantiating a IndirectIterator2.
/*!
  \relates IndirectIterator2
 */
template<typename _Iterator>
inline
IndirectIterator2<_Iterator>
constructIndirectIterator2() {
  return IndirectIterator2<_Iterator>();
}

//! Convenience function for instantiating a IndirectIterator2.
/*!
  \relates IndirectIterator2
 */
template<typename _Iterator>
inline
IndirectIterator2<_Iterator>
constructIndirectIterator2(const _Iterator& i) {
  return IndirectIterator2<_Iterator>(i);
}

END_NAMESPACE_ADS

#endif
