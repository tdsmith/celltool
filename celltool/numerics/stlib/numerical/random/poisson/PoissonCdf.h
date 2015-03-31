// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonCdf.h
  \brief Cumulative distribution function for the Poisson distribution.
*/

#if !defined(__numerical_PoissonCdf_h__)
#define __numerical_PoissonCdf_h__

#include "PoissonPdf.h"

#include <functional>

#include <cassert>
#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonCdf)
#define DEBUG_PoissonCdf
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Cumulative distribution function for the Poisson distribution.
/*!
  \param T The number type.  By default it is double.

  This functor computes 
  \f[
  \mathrm{cdf}(\mu, n) = \sum_{m=0}^n \mathrm{pdf}(\mu, n)
  = \sum_{m=0}^n \frac{e^{-\mu} \mu^m}{m!}.
  \f]

  \note This function generates correct results even evaluating the PDF causes
  underflow.

  CONTINUE: How accurate are the function values?
*/
template<typename T = double>
class PoissonCdf : 
  public std::binary_function<T,int,T> {
  //
  // Private types.
  //
private:

  typedef std::binary_function<T,int,T> Base;

  //
  // Public types.
  //
public:

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The number type.
  typedef T Number;

public:

  //! Default constructor.
  PoissonCdf()
  {}

  //! Copy constructor.
  PoissonCdf(const PoissonCdf&)
  {}

  //! Assignment operator.
  PoissonCdf&
  operator=(const PoissonCdf&) {
    return *this;
  }

  //! Trivial destructor.
  ~PoissonCdf()
  {}

  //! Return the Poisson cumulative distribution function.
  result_type
  operator()(first_argument_type mean, second_argument_type n);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonCdf_ipp__
#include "PoissonCdf.ipp"
#undef __numerical_random_PoissonCdf_ipp__

#endif
