// -*- C++ -*-

/*! 
  \file centered_difference.h
  \brief Numerical differentiation with centered difference schemes.
*/

#if !defined(__numerical_centered_difference_h__)
#define __numerical_centered_difference_h__

#include "../defs.h"

#include "../../ads/array/FixedArray.h"

#include <functional>
#include <limits>

#include <cmath>

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
/*! \defgroup derivative_centered_difference Derivative with the Centered Difference Scheme

These functions and functor calculate the derivative of a functor using a 
centered difference scheme:
\f[ f'(x) = \frac{ f(x + \Delta x) - f(x - \Delta x) }{ 2 \Delta x }
+ \mathcal{O}( \Delta x^2 ). \f]
*/
// @{

//! Calculate f'(x).
template <class Functor>
inline
void
derivative_centered_difference
( const Functor& f, 
  const typename Functor::argument_type x, 
  typename Functor::result_type& deriv, 
  const typename Functor::argument_type 
  delta = std::pow( std::numeric_limits<typename Functor::argument_type>::
		    epsilon(), 1.0 / 3.0 ) ) 
{
  const typename Functor::argument_type p = x + delta;
  const typename Functor::argument_type n = x - delta;
  deriv = ( f( p ) - f( n ) ) / ( p - n );
}

//! Return f'(x).
template <class Functor>
inline
typename Functor::result_type
derivative_centered_difference
( const Functor& f, 
  const typename Functor::argument_type x, 
  const typename Functor::argument_type 
  delta = std::pow( std::numeric_limits<typename Functor::argument_type>::
		    epsilon(), 1.0 / 3.0 ) ) 
{
  typename Functor::result_type deriv;
  derivative_centered_difference( f, x, deriv, delta );
  return deriv;
}

//! The numerical derivative of a functor.
template <class Functor>
class DerivativeCenteredDifference
  : public std::unary_function<typename Functor::argument_type,
			       typename Functor::result_type>
{
  //
  // Types
  //

private:

  typedef std::unary_function<typename Functor::argument_type,
			      typename Functor::result_type> base_type;

public:

  //! The function to differentiate.
  typedef Functor function_type;
  //! The argument type.
  typedef typename base_type::argument_type argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

private:
  
  //
  // Member data.
  //

  const Functor& _f;
  
  argument_type _delta;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  DerivativeCenteredDifference();

  // Assignment operator not implemented.
  DerivativeCenteredDifference&
  operator=( const DerivativeCenteredDifference& x );

public:

  //! Construct from the functor.
  DerivativeCenteredDifference
  ( const function_type& f, 
    const argument_type delta = 
    std::pow( std::numeric_limits<argument_type>::epsilon(), 1.0 / 3.0 ) ) : 
    _f( f ),
    _delta( delta )
  {}

  //! Copy constructor.
  DerivativeCenteredDifference( const DerivativeCenteredDifference& x ) : 
    _f( x._f ),
    _delta( x._delta )
  {}
  
  //! Calculate f'(x).
  void
  operator()( const argument_type x, result_type& deriv ) const 
  {
    derivative_centered_difference( _f, x, deriv, _delta );
  }

  //! Return f'(x).
  result_type
  operator()( const argument_type x ) const 
  {
    return derivative_centered_difference( _f, x, _delta );
  }

  //! Return the differencing offset.
  argument_type
  delta() const
  {
    return _delta;
  }

  //! Set the differencing offset.
  void
  set_delta( const argument_type delta )
  {
    _delta = delta;
  }
};

// @}


//-----------------------------------------------------------------------------
/*! \defgroup gradient_centered_difference Gradient with the Centered Difference Scheme

These functions and functor calculate the gradient of a functor using a 
centered difference scheme.
*/
// @{

//! Calculate the gradient of f at x.
template <int N, class Functor, typename T>
inline
void
gradient_centered_difference
( const Functor& f, 
  typename Functor::argument_type x,
  ads::FixedArray<N,typename Functor::result_type>& gradient, 
  const T delta = std::pow( std::numeric_limits<T>::epsilon(), 1.0 / 3.0 ) ) 
{
  T temp, xp, xn;
  typename Functor::result_type fp, fn;
  for ( int n = 0; n != N; ++n ) {
    temp = x[n];
    // Positive offset.
    xp = x[n] += delta;
    fp = f( x );
    // Negative offset.
    x[n] = temp;
    xn = x[n] -= delta;
    fn = f( x );
    // Reset
    x[n] = temp;
    // Centered difference.
    gradient[n] = ( fp - fn ) / ( xp - xn );
  }
}

//! Return the gradient of f at x.
/*!
  You must specify \c N explicitly as it cannot be inferred from the  
  arguments.
*/
template <int N, class Functor, typename T>
inline
ads::FixedArray<N,typename Functor::result_type>
gradient_centered_difference
( const Functor& f, 
  typename Functor::argument_type x,
  const T delta = std::pow( std::numeric_limits<T>::epsilon(), 1.0 / 3.0 ) ) 
{
  ads::FixedArray<N,typename Functor::result_type> gradient;
  gradient_centered_difference( f, x, gradient, delta );
  return gradient;
}

//! The numerical gradient of a functor.
template <int N, 
	  class Functor, 
	  typename T = typename Functor::argument_type::value_type>
class GradientCenteredDifference : 
  public std::unary_function
< typename Functor::argument_type,
  ads::FixedArray<N,typename Functor::result_type> >
{
private:
  typedef std::unary_function
  < typename Functor::argument_type,
    ads::FixedArray<N,typename Functor::result_type> >
  base_type;
  typedef T number_type;

private:
  
  //
  // Member data.
  //

  const Functor& _f;
  
  number_type _delta;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  GradientCenteredDifference();

  // Assignment operator not implemented.
  GradientCenteredDifference&
  operator=( const GradientCenteredDifference& x );

public:

  //! The function to differentiate.
  typedef Functor function_type;
  //! The argument type.
  typedef typename base_type::argument_type argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

  //! Construct from the functor.
  GradientCenteredDifference
  ( const function_type& f, 
    const number_type delta = 
    std::pow( std::numeric_limits<T>::epsilon(), 1.0 / 3.0 ) ) : 
    _f( f ),
    _delta( delta )
  {}

  //! Copy constructor.
  GradientCenteredDifference( const GradientCenteredDifference& x ) : 
    _f( x._f ),
    _delta( x._delta )
  {}
  
  //! Calculate the gradient of f at x.
  void
  operator()( const argument_type x, result_type& deriv ) const 
  {
    gradient_centered_difference( _f, x, deriv, _delta );
  }

  //! Return the gradient of f at x.
  result_type
  operator()( const argument_type x ) const 
  {
    return gradient_centered_difference<N>( _f, x, _delta );
  }

  //! Return the differencing offset.
  number_type
  delta() const
  {
    return _delta;
  }

  //! Set the differencing offset.
  void
  set_delta( const number_type delta )
  {
    _delta = delta;
  }
};

// @}

END_NAMESPACE_NUMERICAL

#endif
