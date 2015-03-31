// -*- C++ -*-

/*! 
  \file numerical/constants/Logarithm.h
  \brief Statically evaluate the (ceiling of) the logorithm.
*/

#if !defined(__numerical_constants_Logarithm_h__)
#define __numerical_constants_Logarithm_h__

#include "../defs.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_Logarithm)
#define DEBUG_numerical_Logarithm
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Statically compute the (ceiling of the) logarithm with the given base.
/*!
  The general case class uses the recursion 
  \f$\lceil log_B(A) \rceil = \lceil log_B((A+B-1)/B) \rceil + 1\f$.
  The limiting and special cases are defined below.
*/
template<int Base, int Argument>
class Logarithm {
public:
  enum {Result = Logarithm<Base, (Argument + Base - 1) / Base>::Result + 1};
};

//! The logarithm base 0 is indeterminate, so we do not provide a result.
/*!
  Trying to compute a logarithm base 0 will cause a compilation error.
*/
template<int Argument>
class Logarithm<0, Argument> {
};

//! The logarithm base 1 is indeterminate, so we do not provide a result.
/*!
  Trying to compute a logarithm base 1 will cause a compilation error.
*/
template<int Argument>
class Logarithm<1, Argument> {
};

//! The logarithm of 0 is indeterminate, so we do not provide a result.
/*!
  Trying to compute the logarithm of 0 will cause a compilation error.
*/
template<int Base>
class Logarithm<Base, 0> {
};

//! log_B(1) = 0 for nonzero B.
template<int Base>
class Logarithm<Base, 1> {
public:
  enum {Result = 0};
};

END_NAMESPACE_NUMERICAL

#endif
