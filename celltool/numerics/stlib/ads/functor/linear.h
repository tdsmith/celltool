// -*- C++ -*-

/*! 
  \file linear.h
  \brief Linear functors.
*/

#if !defined(__ads_functor_Linear_h__)
#define __ads_functor_Linear_h__

#include "../defs.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_linear Functor: Linear 
  Linear functors.
*/
// @{

//! Linear functor.
template<typename T>
class UnaryLinear : 
  public std::unary_function<T,T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<T,T> Base;

public:

  //
  //Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //
  // Member data.
  //

  //! The multiplicative coefficient.
  T a;
  //! The additive coefficient.
  T b;

  //
  // Constructors.
  //

  //! Construct from the coefficients.
  UnaryLinear(const T a_ = 0, const T b_ = 0) :
    a(a_),
    b(b_)
  {}

  // The default copy and assignment and destructor are fine.

  //
  // Functor.
  //

  //! Return the value of the linear function.
  result_type
  operator()(const argument_type& x) const {
    return a * x + b;
  }

};

//! Convenience function for constructing a \c UnaryLinear.
/*!
  The coefficients are specified.
*/
template<typename T>
inline
UnaryLinear<T>
unary_linear(const T a, const T b) {
  return UnaryLinear<T>(a, b);
}

//! Convenience function for constructing a \c UnaryLinear.
/*!
  The coefficients are uninitialized.
*/
template<typename T>
inline
UnaryLinear<T>
unary_linear() {
  return UnaryLinear<T>();
}

// @}

END_NAMESPACE_ADS

#endif
