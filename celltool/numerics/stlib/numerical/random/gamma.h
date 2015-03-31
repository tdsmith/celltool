// -*- C++ -*-

/*! 
  \file numerical/random/gamma.h
  \brief Includes the random number generator classes for the Gamma distribution.
*/

#if !defined(__numerical_random_gamma_h__)
#define __numerical_random_gamma_h__

#include "gamma/GammaGeneratorMarsagliaTsang.h"
#include "gamma/Default.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random_gamma Gamma Random Generator Package

  \section numerical_random_gamma_properties Properties

  Below is a table that summarizes some important properties of the 
  Gamma distribution.
  <table>
  <tr> <th> Probability density function 
  <td> \f$\mathrm{pdf}_{\mu}(n) = e^{-\mu} \mu^n / n!\f$
  <tr> <th> Cumulative distribution function 
  <td> \f$\mathrm{cdf}_{\mu}(n) = \sum_{k = 0}^n e^{-\mu} \mu^k / k!\f$
  <tr> <th> Mean <td> \f$\mu\f$
  <tr> <th> Mode <td> \f$\lfloor \mu \rfloor\f$
  <tr> <th> Variance <td> \f$\mu\f$
  <tr> <th> Skewness <td> \f$1 / \sqrt{\mu}\f$
  <tr> <th> Kurtosis <td> \f$1 / \mu\f$
  </table>

  \section numerical_random_gamma_classes Classes

  I have implemented the following classes:
  - GammaGeneratorMarsagliaTsang uses
  \ref numerical_random_gammaMarsaglia2000 "Marsaglia and Tsang's" method.

  I have implemented each of the Gamma deviate generators as 
  an <em>adaptable unary function</em>, a functor that takes one argument.
  (See \ref numerical_random_austern1999 "Generic Programming and the STL".)
  The classes are templated on the floating point number type
  (\c double by default) and the uniform deviate generator type
  (DiscreteUniformGeneratorMt19937 by default).
  In addition, some of the classes are templated on the exponential deviate 
  generator type (ExponentialGeneratorZiggurat by default)
  and/or the normal deviate generator type
  (NormalGeneratorZigguratVoss by default).
  Below are a few ways of constructing a Gamma generator.
  \code
  // Use the default number type (double) and the default generators.
  typedef numerical::GammaGeneratorMarsagliaTsang<> Gamma;
  Gamma::DiscreteUniformGenerator uniform;
  Gamma::NormalGenerator normal(&uniform);
  Gamma gamma(&normal); \endcode
  \code
  // Use single precision numbers and the default generators.
  typedef numerical::GammaGeneratorMarsagliaTsang<float> Gamma;
  ... \endcode
  \code
  // Use double precision numbers and the specify the generators.
  typedef numerical::GammaGeneratorMarsagliaTsang<double, numerical::UniformGeneratorMt19937Gsl, 
    numerical::NormalGeneratorZigguratGsl> Gamma;
  ... \endcode

  Each class defines the following types.
  - \c Number is the floating point number type.
  - \c DiscreteUniformGenerator is the discrete uniform generator type.
  - \c argument_type is \c Number.
  - \c result_type is \c Number.

  Each generator has the following member functions.
  - \c operator()(Number shape) Return a Gamma deviate with the specifed shape parameter and unit rate.
  - \c operator()(Number shape, Number rate) Return a Gamma deviate with the specifed shape and rate parameters.
  .
  \code
  double shape = 2.0;
  double deviate = gamma(shape);
  double rate = 3.0;
  deviate = gamma(shape, rate);
  \endcode

  The default gamma deviate generator is defined to be
  GammaGeneratorMarsagliaTsang with the macro 
  \c GAMMA_GENERATOR_DEFAULT in the file 
  GammaGeneratorDefault.h .  Some of the
  \ref numerical_random_poisson "Poisson"
  generators in this package are templated on 
  the gamma deviate generator type.  They use this macro to define
  the default generator.
  If you would like to change the default, just define the macro before 
  you include files from this package.
*/

END_NAMESPACE_NUMERICAL

#endif
