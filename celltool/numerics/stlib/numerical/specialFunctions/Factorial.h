// -*- C++ -*-

/*! 
  \file numerical/specialFunctions/Factorial.h
  \brief The factorial function.
*/

// CONTINUE: Add FactorialCached.

#if !defined(__numerical_Factorial_h__)
#define __numerical_Factorial_h__

#include "../defs.h"

#include <functional>

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_Factorial)
#define DEBUG_Factorial
#endif

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
//! \defgroup numerical_specialFunctions_Factorial The factorial function.
//@{

//! Return n!.
/*!
  \c T is the number type.  Note that you must specify the type explicitly
  as a template parameter.  It cannot be deduced.  
  If \c n is large, n! will overflow as an integer.
  In this case you can use a floating point type for \c T.  However, if 
  \c n is large enough the floating point type will overflow as well.  
*/
template<typename T>
T
computeFactorial(int n);



//! The factorial functor.
/*!
  \c T is the number type.  By default it is int.  If you need larger values,
  use a floating point type.
 */
template<typename T = int>
class Factorial : 
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

  //! Return n!.
  result_type
  operator()(const argument_type n) const {
    return computeFactorial<result_type>(n);
  }
};


//! Convenience function for constructing a \c Factorial.
/*!
  \relates Factorial
*/
template<typename T>
inline
Factorial<T>
constructFactorial() {
  return Factorial<T>();
}


//@}


END_NAMESPACE_NUMERICAL

#define __numerical_specialFunctions_Factorial_ipp__
#include "Factorial.ipp"
#undef __numerical_specialFunctions_Factorial_ipp__

#endif
