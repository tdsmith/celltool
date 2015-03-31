// -*- C++ -*-

/*! 
  \file IntIterator.h
  \brief A random access iterator over an integer type.
*/

#if !defined(__IntIterator_h__)
#define __IntIterator_h__

#include "../defs.h"

#include <iterator>

// If we are debugging the whole ads package.
#if defined(DEBUG_ads) && !defined(DEBUG_IntIterator)
#define DEBUG_IntIterator
#endif

BEGIN_NAMESPACE_ADS

//! A random access iterator over an integer type.
/*!
  Suppose that you need to input a range of consecutive integers to a 
  algorithm.  Instead of having to build a container and fill it with these
  values, you can use an IntIterator.  This iterator just stores an integer
  value.  Dereferencing returns this value.  Incrementing the iterator 
  increments the value.  Likewise, decrementing and offseting just operator
  an the stored integer value.

  As an example, intsead of using a container:

  \verbatim
  std::vector<int> indices;
  for (int n = 0; n != 100; ++n) {
    indices.push_back(n);
  }
  foo(indices.begin(), indices.end()); \endverbatim

  one can use IntIterator:

  \verbatim
  IntIterator begin(0), end(100);
  foo(begin, end); \endverbatim

  One can make the code more compact by using constructIntIterator() to 
  construct the IntIterator's.

  \verbatim
  foo(constructIntIterator(0), constructIntIterator(100)); \endverbatim
*/
template<typename T = int>
class IntIterator :
  public std::iterator<std::random_access_iterator_tag,
		       T,
		       std::ptrdiff_t,
		       const T*,
		       const T&> {
  //
  // Private types.
  //

private:
  
  typedef std::iterator<std::random_access_iterator_tag,
			T,
			std::ptrdiff_t,
			const T*,
			const T&> Base;

  //
  // Public types.
  //

public:

  //! The iterator category.
  typedef typename Base::iterator_category iterator_category;
  //! The value type.
  typedef typename Base::value_type value_type;
  //! Pointer difference type.
  typedef typename Base::difference_type difference_type;
  //! Pointer to the value type.
  typedef typename Base::pointer pointer;
  //! Reference to the value type.
  typedef typename Base::reference reference;

  //
  // Member data.
  //

private:

  value_type _value;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  // Default constructor.  The value is initialized to zero.
  IntIterator() :
    _value(0)
  {}

  //! Construct and initialize the value.
  IntIterator(const value_type x) :
    _value(x)
  {}

  //! Copy constructor.
  IntIterator(const IntIterator& x) :
    _value(x._value)
  {}

  //! Assignment operator.
  IntIterator&
  operator=(const IntIterator& other) {
    if (&other != this) {
      _value = other._value;
    }
    return *this;
  }

  //! Destructor.
  ~IntIterator()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Forward iterator requirements
  //@{

  //! Dereference.
  reference
  operator*() const {
    // Return a reference to the value.
    return _value;
  }

  //! Pointer dereference.
  pointer
  operator->() const {
    // Return a pointer to the value.
    return &_value;
  }
  
  //! Pre-increment.
  IntIterator&
  operator++() { 
    ++_value; 
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  IntIterator
  operator++(int) {
    IntIterator x(*this); 
    ++*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  IntIterator&
  operator--() { 
    --_value; 
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  IntIterator
  operator--(int) {
    IntIterator x(*this); 
    --*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Random access iterator requirements
  //@{

  //! Iterator indexing.
  value_type
  operator[](const difference_type n) { 
    // Return the value offset by n.
    return value_type(_value + n);
  }
  
  //! Positive offseting.
  IntIterator&
  operator+=(const difference_type n) { 
    _value += value_type(n);
    return *this; 
  }

  //! Positive offseting.
  /*!
    \note This is not efficient.  If possible, use \c += instead.
  */
  IntIterator
  operator+(const difference_type n) const { 
    IntIterator x(*this); 
    x += n;
    return x;
  }
      
  //! Negative offseting.
  IntIterator&
  operator-=(const difference_type n) { 
    _value -= value_type(n);
    return *this; 
  }

  //! Negative offseting.
  /*!
    \note This is not efficient.  If possible, use \c -= instead.
  */
  IntIterator
  operator-(const difference_type n) const { 
    IntIterator x(*this); 
    x -= n;
    return x;
  }

  //! Return the value.
  value_type
  base() const {
    return _value;
  }

  //@}
};

//
// Convenience functions.
//

//! Convenience function for instantiating an IntIterator.
/*! \relates IntIterator */
template<typename T>
IntIterator<T>
constructIntIterator(const T x) {
  IntIterator<T> i(x);
  return i;
}

//
// Forward iterator requirements
//

//! Return true if the iterators have a handle to the same index.
/*! \relates IntIterator */
template<typename T>
inline 
bool
operator==(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return x.base() == y.base(); 
}

//! Return true if the iterators do not have a handle to the same index.
/*! \relates IntIterator */
template<typename T>
inline 
bool
operator!=(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return !(x == y);
}

//
// Random access iterator requirements
//

//! Return true if the index of \c x precedes that of \c y.
/*! \relates IntIterator */
template<typename T>
inline 
bool 
operator<(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return x.base() < y.base(); 
}

//! Return true if the index of \c x follows that of \c y.
/*! \relates IntIterator */
template<typename T>
inline 
bool
operator>(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return x.base() > y.base(); 
}

//! Return true if the index of \c x precedes or is equal to that of \c y.
/*! \relates IntIterator */
template<typename T>
inline 
bool
operator<=(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return x.base() <= y.base(); 
}

//! Return true if the index of \c x follows or is equal to that of \c y.
template<typename T>
inline bool
operator>=(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return x.base() >= y.base(); 
}

//! The difference of two iterators.
/*! \relates IntIterator */
template<typename T>
inline 
typename IntIterator<T>::difference_type
operator-(const IntIterator<T>& x, const IntIterator<T>& y) { 
  return x.base() - y.base(); 
}

//! Iterator advance.
/*! \relates IntIterator */
template<typename T>
inline 
IntIterator<T>
operator+(typename IntIterator<T>::difference_type n, 
	  const IntIterator<T>& i) { 
  IntIterator<T> x(i); 
  x += n;
  return x;
}

END_NAMESPACE_ADS

#endif
