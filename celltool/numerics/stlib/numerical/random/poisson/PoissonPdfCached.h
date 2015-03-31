// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonPdfCached.h
  \brief Probability density function for the Poisson distribution.
*/

#if !defined(__numerical_PoissonPdfCached_h__)
#define __numerical_PoissonPdfCached_h__

#include "../../specialFunctions/LogarithmOfFactorialCached.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonPdfCached)
#define DEBUG_PoissonPdfCached
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Probability density function for the Poisson distribution.
/*!
  \param T The number type.  By default it is double.

  This functor computes 
  \f[
  \mathrm{pdf}(\mu, n) = \frac{e^{-\mu} \mu^n}{n!}.
  \f]
  It uses LogarithmOfFactorialCached to get values for the factorial.
  Since this class stores values for the logarithm of the factorial function
  in a table, you must specify the size of the table in the constructor.
  This defines the values of n for which you can evaluate the PDF.
*/
template<typename T = double>
class PoissonPdfCached : 
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

  //
  // Member data.
  //
private:

  LogarithmOfFactorialCached<Number> _logarithmOfFactorial;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  PoissonPdfCached();
  //! Copy constructor not implemented.
  PoissonPdfCached(const PoissonPdfCached&);
  //! Assignment operator not implemented.
  PoissonPdfCached&
  operator=(const PoissonPdfCached&);

public:

  //! Size constructor.
  /*!
    The PDF may be evaluated for n in the range [0..size).
  */
  PoissonPdfCached(const int size) :
    _logarithmOfFactorial(size)
  {}

  //! Trivial destructor.
  ~PoissonPdfCached()
  {}

  //! Return the Poisson probability density function.
  result_type
  operator()(first_argument_type mean, second_argument_type n);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonPdfCached_ipp__
#include "PoissonPdfCached.ipp"
#undef __numerical_random_PoissonPdfCached_ipp__

#endif
