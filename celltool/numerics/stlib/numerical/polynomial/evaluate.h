// -*- C++ -*-

/*! 
  \file numerical/polynomial/evaluate.h
  \brief Evaluate polynomials.
*/

#if !defined(__numerical_polynomial_evaluate_h__)
#define __numerical_polynomial_evaluate_h__

#include "../defs.h"

#include "../../third-party/loki/static_check.h"
#include "../../third-party/loki/TypeManip.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_polynomial_evaluate)
#define DEBUG_polynomial_evaluate
#endif

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
//! \defgroup numerical_polynomial_evaluate Evaluating Polynomials
//@{

//! Evaluate the N_th order polynomial.
/*!
  \return \f$\sum_{n = 0}^{N} \mathrm{coefficients}[n] x^n\f$

  You must explicitly specify the polynomial order N.  It cannot be deduced
  from the template parameters.  Below, we evaluate the cubic polynomial
  \f$2 + 3 x + 5 x^2 + 7 x^3\f$.
  <pre>
  double x = 1.5;
  double c[4] = {2, 3, 5, 7};
  double result = evaluatePolynomial<3>(x, c);</pre>

  This function has specializations for constant, linear, quadratic, and 
  cubic equations. (Quadratic equations are so named because 
  <em>quadratus</em> is Latin for "square.")

  \warning Be careful of "off by one" errors.  N is not the size of the 
  coefficients array.  Its size is N + 1.
*/
template<int N, typename T>
T
evaluatePolynomial(T x, const T* coefficients);

//@}


END_NAMESPACE_NUMERICAL

#define __numerical_polynomial_evaluate_ipp__
#include "evaluate.ipp"
#undef __numerical_polynomial_evaluate_ipp__

#endif
