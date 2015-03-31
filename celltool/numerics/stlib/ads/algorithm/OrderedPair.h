// -*- C++ -*-

/*! 
  \file OrderedPair.h
  \brief The ordered pair data structure.
*/

#if !defined(__ads_algorithm_OrderedPair_h__)
#define __ads_algorithm_OrderedPair_h__

#include "../defs.h"

#include "../../third-party/loki/TypeTraits.h"

#include <algorithm>
#include <utility>

BEGIN_NAMESPACE_ADS


//! OrderedPair holds two objects of the same arbitrary type.
/*!
  The objects are ordered so that the first precedes the second.
*/
template<typename T>
class OrderedPair :
  public std::pair<T,T> {

  //
  // Public types.
  //

public:

  //! The value type
  typedef T Value;
  //! The const parameter type.
  typedef typename Loki::TypeTraits<Value>::ParameterType ParameterType;
  //! The base type is a pair.
  typedef std::pair<T,T> Base;

  //
  // Using
  //

private:

  using Base::first;
  using Base::second;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  /*! 
    Create the elements using their default constructors.
  */
  OrderedPair() :
    Base()
  {}
  
  //! Copy constructor.
  OrderedPair(const OrderedPair& other) :
    Base(other)
  {}
  
  //! Assignment operator.
  OrderedPair&
  operator=(const OrderedPair& other)
  {
    // Avoid assignment to self.
    if (this != &other) {
      Base::operator=(other);
    }
    // Return *this so assignments can chain.
    return *this;
  }
  
  //! Trivial destructor.
  ~OrderedPair()
  {}
  
  //! Construct by copying the elements.
  OrderedPair(const Value& a, const Value& b) : 
    Base(a, b) {
    order();
  }

  //! Construct from a pair of different types.
  template<typename T1, typename T2>
  OrderedPair(const std::pair<T1,T2>& x) : 
    Base(x) {
    order();
  }
  
  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the first element.
  ParameterType
  getFirst() const {
    return first;
  }

  //! Return the second element.
  ParameterType
  getSecond() const {
    return second;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Set the elements.
  /*!
    \note The elements may be swapped.
  */
  void
  set(ParameterType x, ParameterType y) {
    first = x;
    second = y;
    order();
  }

  //! Set the first element.
  /*!
    \note This this may cause the first and second element to be swapped.
  */
  void
  setFirst(ParameterType x) {
    first = x;
    order();
  }

  //! Set the second element.
  /*!
    \note This this may cause the first and second element to be swapped.
  */
  void
  setSecond(ParameterType x) {
    second = x;
    order();
  }

  //@}
  //--------------------------------------------------------------------------
  // Private member functions.

  //! Order the two elements.
  void
  order() {
    if (second < first) {
      std::swap(first, second);
    }
  }

};

// Inherit equality and inequality from std::pair.

//! A convenience wrapper for creating a OrderedPair.
/*!
  \param x The first object.
  \param y The second object.
  \return A newly-constructed OrderedPair<> object of the appropriate type.
*/
template<typename T>
inline 
OrderedPair<T>
makeOrderedPair(const T& x, const T& y) { 
  return OrderedPair<T>(x, y); 
}

END_NAMESPACE_ADS

#endif
