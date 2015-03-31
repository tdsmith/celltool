// -*- C++ -*-

/*!
  \file numerical/interpolation/hermite.h
  \brief Hermite interpolation.
*/

#if !defined(__numerical_interpolation_hermite_h__)
#define __numerical_interpolation_hermite_h__

#include "../polynomial/evaluate.h"

#include "../../ads/array/Array.h"

#include <functional>

BEGIN_NAMESPACE_NUMERICAL

/*! 
  \defgroup interpolation_hermite Hermite Interpolation.
*/

//! Hermite interpolation.
/*!
  \param t The function argument.
  \param value0 f(0)
  \param value1 f(1)
  \param derivative0 f'(0)
  \param derivative1 f'(1)

  Interpolate a function on the range [0..1] from the values and first 
  derivates of the function at the endpoints.  That is, perform cubic 
  interpolation, using the values f(0), f(1), f'(0), and f'(1).

  This function implements the algorithm presented in
  "Geometric Tools for Computer Graphics" by Philip Schneider and David Eberly.
  The cubic Hermite polynomials are given below.
  \f[
  a_0(t) = 2 t^3 - 3 t^2 + 1
  \f]
  \f[
  a_1(t) = -2 t^3 + 3 t^2
  \f]
  \f[
  b_0(t) = t^3 - 2 t^2 + t
  \f]
  \f[
  b_1(t) = t^3 - t^2
  \f]
  The cubic interpolant is 
  \f[
  c(t) = f(0) a_0(t) + f(1) a_1(t) + f'(0) b_0(t) + f'(1) b_1(t).
  \f]
  This function simply evaluates the interpolant.

  \return The interpolated value of the function.

  \note Using this function is not efficient when one is evaluating a 
  fixed interpolant for multiple arguments.  For this usage scenario, use
  a divided differences algorithm instead.

  \ingroup interpolation_hermite
*/
template<typename T>
T
hermiteInterpolate(T t, T value0, T value1, T derivative0, T derivative1);


//! Compute the polynomial coefficients for Hermite interpolation.
/*!
  \param value0 f(0)
  \param value1 f(1)
  \param derivative0 f'(0)
  \param derivative1 f'(1)
  \param coefficients The coefficients of the cubic polynomial for performing
  Hermite interpolation of f(x).

  The coefficients <code>c</code> of the polynomial are
  \f[
  c[0] + c[1] x + c[2] x^2 + c[3] x^3.
  \f]
*/
template<typename T>
void
computeHermitePolynomialCoefficients(T value0, T value1, 
				     T derivative0, T derivative1,
				     T* coefficients);


//----------------------------------------------------------------------------


//! Class for Hermite interpolation with a number of patches.
/*!
  This class stores the cubic polynomial coefficients.  This 
  requires twice the storage of HermiteFunctionDerivative, but the 
  interpolation is faster.
*/
template<typename T = double>
class Hermite : 
  public std::unary_function<T,T> {
private:

  typedef std::unary_function<T,T> Base;

public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:
  
  // The lower bound of the range.
  Number _lowerBound;
  // Factor that will scale the argument to the index.
  Number _scaleToIndex;
  // Polynomial coefficients.
  ads::Array<1,Number> _coefficients;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  Hermite();

public:

  //! Construct from the functor, its derivative, the range, and the number of patches.
  template<typename _Function, typename Derivative>
  Hermite(const _Function& function, const Derivative& derivative,
	  Number closedLowerBound, Number openUpperBound, int numberOfPatches);

  //! Copy constructor.
  /*!
    \note This function is expensive.
  */
  Hermite(const Hermite& other);

  //! Assignment operator.
  /*!
    \note This function is expensive.
  */
  Hermite&
  operator=(const Hermite& other);

  //! Trivial destructor.
  ~Hermite()
  {}

  //! Return the interpolated function value.
  result_type
  operator()(argument_type x) const;
};


//----------------------------------------------------------------------------


//! Class for Hermite interpolation with a number of patches.
/*!
  This class stores the function and derivative values.  This  
  requires half the storage of Hermite, but the interpolation is not as fast.
*/
template<typename T = double>
class HermiteFunctionDerivative : 
  public std::unary_function<T,T> {
private:

  typedef std::unary_function<T,T> Base;

public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:
  
  // The lower bound of the range.
  Number _lowerBound;
  // Factor that will scale the argument to the index.
  Number _scaleToIndex;
  // The function and derivative values.
  ads::Array<1,Number> _functionAndDerivativeValues;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  HermiteFunctionDerivative();
  // Copy constructor not implemented.
  HermiteFunctionDerivative(const HermiteFunctionDerivative& other);
  // Assignment operator not implemented.
  HermiteFunctionDerivative&
  operator=(const HermiteFunctionDerivative& other);

public:

  //! Construct from the functor, its derivative, the range, and the number of patches.
  template<typename _Function, typename Derivative>
  HermiteFunctionDerivative(const _Function& function, const Derivative& derivative,
	  Number closedLowerBound, Number openUpperBound, int numberOfPatches);

  //! Trivial destructor.
  ~HermiteFunctionDerivative()
  {}

  //! Return the interpolated function value.
  result_type
  operator()(argument_type x) const;
};


END_NAMESPACE_NUMERICAL

#define __numerical_interpolation_hermite_ipp__
#include "hermite.ipp"
#undef __numerical_interpolation_hermite_ipp__

#endif
