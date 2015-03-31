// -*- C++ -*-

/*! 
  \file numerical/constants/Exponentiation.h
  \brief Statically exponentiate for non-negative, integer exponents.
*/

#if !defined(__numerical_constants_Exponentiation_h__)
#define __numerical_constants_Exponentiation_h__

#include "../defs.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_Exponentiation)
#define DEBUG_numerical_Exponentiation
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Statically compute Base^Exponent for integer arguments.
/*!
  The general case class uses the recursion \f$B^E = B * B^{E-1}\f$.
  The limiting and special cases are defined below.
*/
template<int Base, int Exponent>
class Exponentiation {
public:
  enum {Result = Base * Exponentiation<Base, Exponent - 1>::Result};
};

//! 0^0 is indeterminate, so we do not provide a result.
/*!
  Trying to compute 0^0 will cause a compilation error.
*/
template<>
class Exponentiation<0, 0> {
};

//! 0^E = 0 for nonzero E.
template<int Exponent>
class Exponentiation<0, Exponent> {
public:
  enum {Result = 0};
};

//! B^0 = 1 for nonzero B.
template<int Base>
class Exponentiation<Base, 0> {
public:
  enum {Result = 1};
};

END_NAMESPACE_NUMERICAL

//#define __numerical_constants_Exponentiation_ipp__
//#include "Exponentiation.ipp"
//#undef __numerical_constants_Exponentiation_ipp__

#endif
