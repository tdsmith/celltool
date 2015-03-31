// -*- C++ -*-

/*! 
  \file numerical/specialFunctions/ExponentialForSmallArgument.h
  \brief Exponential specialized for small argument.
*/

#if !defined(__numerical_ExponentialForSmallArgument_h__)
#define __numerical_ExponentialForSmallArgument_h__

#include "../defs.h"

#include <iostream>
#include <functional>
#include <limits>

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_ExponentialForSmallArgument)
#define DEBUG_numerical_ExponentialForSmallArgument
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Compute the exponential function.
/*!
  \param T The number type.  By default it is double.

  This function is optimized for small argument.  For small argument, it 
  uses a truncated Taylor series to compute the result.  Otherwise, it 
  uses the standard library function \c std::exp().

  The figure below shows the execution times for computing the exponential with
  this functor and with \c std::exp().  There are significant computation
  savings for very small arguments.  For large arguments, this functor is 
  about 0.5% more expensive than std::exp().  In this case, the functor adds
  only the cost of a function call and a single conditional.

  \image html ExponentialForSmallArgument.jpg "Execution times for computing the exponential."
  \image latex ExponentialForSmallArgument.pdf "Execution times for computing the exponential." width=0.5\textwidth

  Note that the constructor for this functor is expensive.  Do not call it 
  unecessarily.
*/
template<typename T = double>
class ExponentialForSmallArgument : 
  public std::unary_function<T,T> {
public:

  //! The number type.
  typedef T Number;

private:

  Number _threshhold1, _threshhold2, _threshhold3, _threshhold4, _threshhold5,
    _threshhold6, _threshhold7;

public:

  //! Default constructor.
  ExponentialForSmallArgument() :
    _threshhold1(std::numeric_limits<Number>::epsilon()),
    _threshhold2(std::sqrt(2 * std::numeric_limits<Number>::epsilon())),
    _threshhold3(std::pow(6 * std::numeric_limits<Number>::epsilon(), 
			  Number(1.0/3.0))),
    _threshhold4(std::pow(24 * std::numeric_limits<Number>::epsilon(), 
			  Number(1.0/4.0))),
    _threshhold5(std::pow(120 * std::numeric_limits<Number>::epsilon(),
			  Number(1.0/5.0))),
    _threshhold6(std::pow(720 * std::numeric_limits<Number>::epsilon(), 
			  Number(1.0/6.0))),
    _threshhold7(std::pow(5040 * std::numeric_limits<Number>::epsilon(), 
			  Number(1.0/7.0)))
  {}

  //! Copy constructor.
  ExponentialForSmallArgument(const ExponentialForSmallArgument& other) :
    _threshhold1(other._threshhold1),
    _threshhold2(other._threshhold2),
    _threshhold3(other._threshhold3),
    _threshhold4(other._threshhold4),
    _threshhold5(other._threshhold5),
    _threshhold6(other._threshhold6),
    _threshhold7(other._threshhold7)
  {}

  //! Assignment operator.
  ExponentialForSmallArgument&
  operator=(const ExponentialForSmallArgument& other) {
    if (this != &other) {
      _threshhold1 = other._threshhold1;
      _threshhold2 = other._threshhold2;
      _threshhold3 = other._threshhold3;
      _threshhold4 = other._threshhold4;
      _threshhold5 = other._threshhold5;
      _threshhold6 = other._threshhold6;
      _threshhold7 = other._threshhold7;
    }
    return *this;
  }

  //! Trivial destructor.
  ~ExponentialForSmallArgument()
  {}

  //! Return the exponential function.
  Number
  operator()(Number x) const;

  //! Print the threshholds.
  void
  printThreshholds(std::ostream& out) const;
};


//! Convenience function for constructing an ExponentialForSmallArgument.
template<typename T>
inline
ExponentialForSmallArgument<T>
constructExponentialForSmallArgument() {
  return ExponentialForSmallArgument<T>();
}


END_NAMESPACE_NUMERICAL

#define __numerical_specialFunctions_ExponentialForSmallArgument_ipp__
#include "ExponentialForSmallArgument.ipp"
#undef __numerical_specialFunctions_ExponentialForSmallArgument_ipp__

#endif
