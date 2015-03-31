// -*- C++ -*-

/*! 
  \file Identity.h
  \brief The identity functor.
*/

#if !defined(__ads_Identity_h__)
#define __ads_Identity_h__

#include "../defs.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_identity Functor: Identity 
  The identity functor.
*/
// @{

//! The identity functor.
template<typename T>
struct Identity : 
  public std::unary_function<T,T> {
  //! Return a reference to the argument.
  T&
  operator()(T& x) const { 
    return x; 
  }

  //! Return a const reference to the argument.
  const T&
  operator()(const T& x) const { 
    return x; 
  }
};

//! Convenience function for constructing an \c Identity.
template<typename T>
inline
Identity<T>
identity() {
  return Identity<T>();
}

// @}

END_NAMESPACE_ADS

#endif
