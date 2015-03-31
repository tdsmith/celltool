// -*- C++ -*-

/*! 
  \file AdaptedIterator.h
  \brief A base class for all adapted iterators.
*/

#if !defined(__ads_iterator_AdaptedIterator_h__)
#define __ads_iterator_AdaptedIterator_h__

#include "../defs.h"

#include <iterator>

BEGIN_NAMESPACE_ADS

//! A base class for all adapted iterators.
template<typename _Iterator,
	 typename IteratorCategory,
	 typename ValueType,
	 typename DifferenceType,
	 typename Pointer,
	 typename Reference>
class AdaptedIterator :
  public std::iterator<IteratorCategory,
		       ValueType,
		       DifferenceType,
		       Pointer,
		       Reference> {
private:

  //
  // Private types.
  //

  typedef std::iterator<IteratorCategory,
			ValueType,
			DifferenceType,
			Pointer,
			Reference>
  Base;

public:

  //
  // Public types.
  //

  //! The base iterator type.
  typedef _Iterator Iterator;

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

protected:
  
  //
  // Member data.
  //

  //! The base iterator type.
  Iterator _iterator;

protected:
  
  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The constructors are proctected because this class is only meant to
    be used as a base class.
   */
  //@{

  //! Default constructor.
  AdaptedIterator() :
    _iterator()
  {}

  //! Copy constructor.
  AdaptedIterator(const AdaptedIterator& x) :
    _iterator(x._iterator)
  {}

  //! Assignment operator.
  AdaptedIterator& 
  operator=(const AdaptedIterator& other) {
    if (&other != this) {
      _iterator = other._iterator;
    }
    return *this;
  }

  //! Construct from an iterator.
  explicit
  AdaptedIterator(const Iterator& i) :
    _iterator(i)
  {}

  //! Assignment from an iterator.
  AdaptedIterator& 
  operator=(const Iterator& i) {
    _iterator = i;
    return *this;
  }

  //! Destructor.
  ~AdaptedIterator()
  {}

  //@}

public:

  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the base iterator.
  const Iterator&
  base() const {
    return _iterator;
  }

  //@}
};


//
// Trivial iterator requirements.
//


//! Return true if the base iterators are the same.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	  typename DifferenceType1, typename Pointer1, typename Reference1,
	  typename Iterator2, typename IteratorCategory2, typename ValueType2,
	  typename DifferenceType2, typename Pointer2, typename Reference2>
inline
bool
operator==(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	   DifferenceType1,Pointer1,Reference1>& x, 
	   const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	   DifferenceType2,Pointer2,Reference2>& y) {
  return x.base() == y.base();
}


//! Return true if the base iterators are not the same.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	 typename DifferenceType1, typename Pointer1, typename Reference1,
	 typename Iterator2, typename IteratorCategory2, typename ValueType2,
	 typename DifferenceType2, typename Pointer2, typename Reference2>
inline
bool
operator!=(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	   DifferenceType1,Pointer1,Reference1>& x, 
	   const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	   DifferenceType2,Pointer2,Reference2>& y) {
  return ! (x == y);
}


//
// Random access iterator requirements.
//


//! Compare the base iterators.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	 typename DifferenceType1, typename Pointer1, typename Reference1,
	 typename Iterator2, typename IteratorCategory2, typename ValueType2,
	 typename DifferenceType2, typename Pointer2, typename Reference2>
inline
bool
operator<(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	  DifferenceType1,Pointer1,Reference1>& x, 
	  const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	  DifferenceType2,Pointer2,Reference2>& y) {
  return x.base() < y.base();
}


//! Compare the base iterators.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	 typename DifferenceType1, typename Pointer1, typename Reference1,
	 typename Iterator2, typename IteratorCategory2, typename ValueType2,
	 typename DifferenceType2, typename Pointer2, typename Reference2>
inline
bool
operator>(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	  DifferenceType1,Pointer1,Reference1>& x, 
	  const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	  DifferenceType2,Pointer2,Reference2>& y) {
  return x.base() > y.base();
}


//! Compare the base iterators.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	 typename DifferenceType1, typename Pointer1, typename Reference1,
	 typename Iterator2, typename IteratorCategory2, typename ValueType2,
	 typename DifferenceType2, typename Pointer2, typename Reference2>
inline
bool
operator<=(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	   DifferenceType1,Pointer1,Reference1>& x, 
	   const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	   DifferenceType2,Pointer2,Reference2>& y) {
  return x.base() <= y.base();
}


//! Compare the base iterators.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	 typename DifferenceType1, typename Pointer1, typename Reference1,
	 typename Iterator2, typename IteratorCategory2, typename ValueType2,
	 typename DifferenceType2, typename Pointer2, typename Reference2>
inline
bool
operator>=(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	   DifferenceType1,Pointer1,Reference1>& x, 
	   const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	   DifferenceType2,Pointer2,Reference2>& y) {
  return x.base() >= y.base();
}


//! Difference of pointers.
/*!
  \relates AdaptedIterator
 */
template<typename Iterator1, typename IteratorCategory1, typename ValueType1,
	 typename DifferenceType1, typename Pointer1, typename Reference1,
	 typename Iterator2, typename IteratorCategory2, typename ValueType2,
	 typename DifferenceType2, typename Pointer2, typename Reference2>
inline
typename AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
			 DifferenceType1,Pointer1,Reference1>::difference_type
operator-(const AdaptedIterator<Iterator1,IteratorCategory1,ValueType1,
	  DifferenceType1,Pointer1,Reference1>& x, 
	  const AdaptedIterator<Iterator2,IteratorCategory2,ValueType2,
	  DifferenceType2,Pointer2,Reference2>& y) {
  return x.base() - y.base();
}


END_NAMESPACE_ADS

#endif
