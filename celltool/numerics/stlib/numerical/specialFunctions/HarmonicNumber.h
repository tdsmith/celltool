// -*- C++ -*-

/*! 
  \file numerical/specialFunctions/HarmonicNumber.h
  \brief The factorial function.
*/

// CONTINUE: Add HarmonicNumberCached.

#if !defined(__numerical_HarmonicNumber_h__)
#define __numerical_HarmonicNumber_h__

#include "../defs.h"

#include <functional>

#include <cassert>
#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_HarmonicNumber)
#define DEBUG_numerical_HarmonicNumber
#endif

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
//! \defgroup numerical_specialFunctions_HarmonicNumber The Harmonic Number Function.
//@{


//! Return the n_th harmonic number.
/*!
  \c T is the number type.  Note that you must specify the type explicitly
  as a template parameter.  It cannot be deduced.

  The n_th harmonic number is
  \f[
  H_n = \sum_{k = n}^n \frac{1}{k}.
  \f]
  It is a partial sum of the harmonic series.
*/
template<typename T>
T
computeHarmonicNumber(int n);



//! Compute the difference of two harmonic numbers.
/*!
  \c T is the number type.  Note that you must specify the type explicitly
  as a template parameter.  It cannot be deduced.
*/
template<typename T>
T
computeDifferenceOfHarmonicNumbers(int m, int n);



//! The factorial functor.
/*!
  \c T is the number type.  By default it is double.
*/
template<typename T = double>
class HarmonicNumber : 
  public std::unary_function<int,T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<int,T> Base;

public:

  //
  //Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  // The default constructor, copy, assignment, and destructor are fine.

  //
  // Functor.
  //

  //! Return the n_th Harmonic number.
  result_type
  operator()(const argument_type n) const {
    return computeHarmonicNumber<result_type>(n);
  }
};


//! Convenience function for constructing a \c HarmonicNumber.
/*!
  \relates HarmonicNumber
*/
template<typename T>
inline
HarmonicNumber<T>
constructHarmonicNumber() {
  return HarmonicNumber<T>();
}


//@}


END_NAMESPACE_NUMERICAL

#define __numerical_specialFunctions_HarmonicNumber_ipp__
#include "HarmonicNumber.ipp"
#undef __numerical_specialFunctions_HarmonicNumber_ipp__

#endif
