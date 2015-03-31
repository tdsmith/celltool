// -*- C++ -*-

/*! 
  \file numerical/specialFunctions/LogarithmOfFactorialCachedDynamic.h
  \brief The logarithm of the factorial function.  Uses a table of values.
*/

#if !defined(__numerical_LogarithmOfFactorialCachedDynamic_h__)
#define __numerical_LogarithmOfFactorialCachedDynamic_h__

#include "../defs.h"

#include <vector>

#include <functional>

#include <cassert>
#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_LogarithmOfFactorialCachedDynamic)
#define DEBUG_numerical_LogarithmOfFactorialCachedDynamic
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The logarithm of the factorial function.  Uses a table of values.
/*!
  \c T is the number type.  By default it is double.

  Since copying the table of values is expensive, I do not provide
  a copy constructor or an assignment operator.  They should not be
  necessary; if used as an argument, this class
  should be passed by reference or const reference.

  CONTINUE: How accurate are the function values?
*/
template<typename T = double>
class LogarithmOfFactorialCachedDynamic : 
  public std::unary_function<int,T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<int,T> Base;

public:

  //
  // Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The number type.
  typedef T Number;

  //
  // Member data.
  //

private:

  mutable std::vector<Number> _values;
  
  //
  // Not implemented.
  //

private:

  //! Copy constructor not implemented.
  LogarithmOfFactorialCachedDynamic(const LogarithmOfFactorialCachedDynamic&);

  //! Assignment operator not implemented.
  LogarithmOfFactorialCachedDynamic&
  operator=(const LogarithmOfFactorialCachedDynamic&);
  
public:

  //! Default constructor.
  LogarithmOfFactorialCachedDynamic() :
    _values()
  {}

  //! Maximum argument constructor.
  LogarithmOfFactorialCachedDynamic(const int maximumArgument) :
    _values() {
    // This is not necessary, but is done for the sake of efficiency.
    _values.reserve(maximumArgument + 1);
    fillTable(maximumArgument);
  }

  //! Trivial destructor.
  ~LogarithmOfFactorialCachedDynamic()
  {}

  //! Return log(n!).
  result_type
  operator()(const argument_type n) const {
#ifdef DEBUG_numerical_LogarithmOfFactorialCachedDynamic
    assert(0 <= n);
#endif
    if (n >= int(_values.size())) {
      fillTable(n);
    }
    return _values[n];
  }

  //
  // Private member functions.
  //
private:
  void
  fillTable(int maximumArgument) const;
};


//@}


END_NAMESPACE_NUMERICAL

#define __numerical_specialFunctions_LogarithmOfFactorialCachedDynamic_ipp__
#include "LogarithmOfFactorialCachedDynamic.ipp"
#undef __numerical_specialFunctions_LogarithmOfFactorialCachedDynamic_ipp__

#endif
