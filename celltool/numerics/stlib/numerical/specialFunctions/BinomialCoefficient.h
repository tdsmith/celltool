// -*- C++ -*-

/*! 
  \file numerical/specialFunctions/BinomialCoefficient.h
  \brief The binomial coefficient function.
*/

#if !defined(__numerical_BinomialCoefficient_h__)
#define __numerical_BinomialCoefficient_h__

#include "HarmonicNumber.h"

#include <functional>

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_BinomialCoefficient)
#define DEBUG_numerical_BinomialCoefficient
#endif

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
//! \defgroup numerical_specialFunctions_BinomialCoefficient Binomial Coefficient
//@{


//! Return n! / (k! (n - k)!).
/*!
  \c T is the number type.  Note that you must specify the type explicitly
  as a template parameter.  It cannot be deduced.  If the result will 
  overflow the integer type, use a floating point type.
*/
template<typename T>
T
computeBinomialCoefficient(int n, int k);



//! The binomial coefficient functor returns n! / (k! (n - k)!).
/*!
  \c T is the number type.  By default it is int.  If you need larger values,
  use a floating point type.
 */
template<typename T = int>
class BinomialCoefficient : 
  public std::binary_function<int,int,T> {
private:

  //
  // Private types.
  //

  typedef std::binary_function<int,int,T> Base;

public:

  //
  //Public types.
  //

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  // The default constructor, copy, assignment, and destructor are fine.

  //
  // Functor.
  //

  //! Return n! / (k! (n - k)!).
  result_type
  operator()(const first_argument_type n, const second_argument_type k) const {
    return computeBinomialCoefficient<result_type>(n, k);
  }
};


//! Convenience function for constructing a \c BinomialCoefficient.
/*!
  \relates BinomialCoefficient
*/
template<typename T>
inline
BinomialCoefficient<T>
constructBinomialCoefficient() {
  return BinomialCoefficient<T>();
}



//! Return the derivative (with respect to the first argument of the binomial coefficient.
/*!
  \c T is the number type.  Note that you must specify the type explicitly
  as a template parameter.  It cannot be deduced.

  D[Binomial[n,k],n] == Binomial[n,k] (HarmonicNumber[n] - HarmonicNumber[n-k])
*/
template<typename T>
T
computeBinomialCoefficientDerivative(int n, int k);


//@}


END_NAMESPACE_NUMERICAL

#define __numerical_specialFunctions_BinomialCoefficient_ipp__
#include "BinomialCoefficient.ipp"
#undef __numerical_specialFunctions_BinomialCoefficient_ipp__

#endif
