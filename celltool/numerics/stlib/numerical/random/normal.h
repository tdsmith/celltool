// -*- C++ -*-

/*! 
  \file numerical/random/normal.h
  \brief Includes the normal (Gaussian) random number generator classes.
*/

#if !defined(__numerical_random_normal_h__)
#define __numerical_random_normal_h__

#include "normal/NormalGeneratorZigguratVoss.h"

#include "normal/NormalGeneratorZigguratGsl.h"
#include "normal/NormalGeneratorRatioGsl.h"
#include "normal/NormalGeneratorBoxMullerGsl.h"

#include "normal/NormalGeneratorBoxMullerNr.h"

#include "normal/Default.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random_normal Normal Random Number Package

  This package implements a number of generators for the 
  normal (Gaussian) distribution.  
  Below is a table that summarizes some important properties of the 
  normal distribution with mean \f$\mu\f$ and variance \f$\sigma^2\f$
  <table>
  <tr> <th> Probability density function 
  <td> \f$\mathrm{pdf}(x) = \frac{1}{\sigma \sqrt{2 \pi}} 
  \exp\left( - \frac{(x - \mu)^2}{2 \sigma^2} \right)\f$
  <tr> <th> Cumulative distribution function 
  <td> \f$\mathrm{cdf}(x) = \frac{1}{2} \left( 1 + 
  \mathrm{erf}\left( \frac{x - \mu}{\sigma \sqrt{2}} \right) \right)\f$
  <tr> <th> Mean <td> \f$\mu\f$
  <tr> <th> Median <td> \f$\mu\f$
  <tr> <th> Mode <td> \f$\mu\f$
  <tr> <th> Variance <td> \f$\sigma^2\f$
  <tr> <th> Skewness <td> 0
  <tr> <th> Kurtosis <td> 0
  </table>

  This package provides the the following functors for computing 
  normal random deviates.
  - NormalGeneratorZigguratVoss
  - NormalGeneratorBoxMullerNr
  - NormalGeneratorZigguratGsl<T,DiscreteUniformGeneratorMt19937Gsl>
  - NormalGeneratorRatioGsl<T,DiscreteUniformGeneratorMt19937Gsl>
  - NormalGeneratorBoxMullerGsl<T,DiscreteUniformGeneratorMt19937Gsl>

  I have implemented each of the normal deviate generators as 
  an <em>adaptable generator</em>, a functor that takes no arguments.
  (See \ref numerical_random_austern1999 "Generic Programming and the STL".)
  The classes are templated on the floating point number type
  (\c double by default) and the discrete, uniform deviate generator type
  (DiscreteUniformGeneratorMt19937 by default).
  Below are a few ways of constructing a normal deviate generator.
  \code
  // Use the default number type (double) and the default uniform deviate generator.
  typedef numerical::NormalGeneratorZigguratVoss<> Normal;
  Normal::DiscreteUniformGenerator uniform;
  Normal normal(&unform); \endcode
  \code
  // Use single precision numbers and the default uniform deviate generator.
  typedef numerical::NormalGeneratorZigguratVoss<float> Normal;
  ... \endcode
  \code
  // Use double precision numbers and the Mersenne twister generator from the GSL.
  typedef numerical::NormalGeneratorZigguratVoss<double, numerical::DiscreteUniformGeneratorMt19937Gsl> Normal;
  ... \endcode

  Each class defines the following types.
  - \c Number is the floating point number type.
  - \c DiscreteUniformGenerator is the discrete uniform generator type.
  - \c argument_type is \c void.
  - \c result_type is \c Number.

  Each generator has the following member functions.
  - \c operator()() Return a normal deviate with zero mean and unit variance.
  - \c operator()(Number mean, Number variance) Return a normal deviate with specified mean and variance.
  - seed(typename DiscreteUniformGenerator::result_type seedValue) 
  Seed the uniform generator.
  \code
  typedef numerical::NormalGeneratorZigguratVoss<> Generator;
  Generator a;
  double x = a();
  double y = a(1.0, 2.0);
  a.seed(123U);
  \endcode

  The default normal deviate generator is defined to be
  NormalGeneratorZigguratVoss with the macro 
  \c NORMAL_GENERATOR_DEFAULT in the file Default.h .  Some of the
  \ref numerical_random_poisson "Poisson"
  generators in this package are templated on 
  the normal deviate generator type.  They use this macro to define
  the default generator.
  If you would like to change the default, just define the macro before 
  you include files from this package.

  Below is a table of the execution times for the standard normal generators.

  <table>
  <tr> 
  <th> Functor
  <th> Execution Time (nanoseconds)
  <tr> 
  <td> NormalGeneratorZigguratVoss
  <td> 19
  <tr> 
  <td> NormalGeneratorBoxMullerNr
  <td> 58
  <tr> 
  <td> NormalGeneratorZigguratGsl<T,DiscreteUniformGeneratorMt19937Gsl>
  <td> 188
  <tr> 
  <td> NormalGeneratorRatioGsl<T,DiscreteUniformGeneratorMt19937Gsl>
  <td> 194
  <tr> 
  <td> NormalGeneratorBoxMullerGsl<T,DiscreteUniformGeneratorMt19937Gsl>
  <td> 255
  </table>
*/

END_NAMESPACE_NUMERICAL

#endif
