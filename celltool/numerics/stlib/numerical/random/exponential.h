// -*- C++ -*-

/*! 
  \file numerical/random/exponential.h
  \brief Includes the exponential deviate generator classes.
*/

#if !defined(__numerical_random_exponential_h__)
//! Include guard.
#define __numerical_random_exponential_h__

#include "exponential/ExponentialGeneratorAcceptanceComplement.h"
#include "exponential/ExponentialGeneratorInversion.h"
#include "exponential/ExponentialGeneratorZiggurat.h"

#include "exponential/Default.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random_exponential Exponential Random Deviate Package

  Consider discrete events occurring in continuous time which occur with
  a known average rate.  The exponential distribution expresses the 
  waiting time between events.  It is a function of a non-negative real number.
  It depends on the average rate of events, \f$\lambda\f$.

  Below is a table that summarizes some important properties of the 
  exponential distribution.
  <table>
  <tr> <th> Probability density function 
  <td> \f$\mathrm{pdf}_{\lambda}(x) = \lambda e^{-\lambda x}\f$
  <tr> <th> Cumulative distribution function 
  <td> \f$\mathrm{cdf}_{\lambda}(x) = 1 - e^{-\lambda x}\f$
  <tr> <th> Mean <td> \f$1 / \lambda\f$
  <tr> <th> Median <td> \f$\ln(2) / \lambda\f$
  <tr> <th> Mode <td> 0
  <tr> <th> Variance <td> \f$\lambda^{-2}\f$
  <tr> <th> Skewness <td> 2
  <tr> <th> Kurtosis <td> 6
  </table>


  Below we plot the PDF and CDF of the exponential distribution for 
  average rates of 1, 2, and 3.

  \image html random/ExponentialPdf.jpg "Some probabilty density functions for the exponential distribution."
  \image latex random/ExponentialPdf.pdf "Some probabilty density functions for the exponential distribution." width=0.5\textwidth

  \image html random/ExponentialCdf.jpg "Some cumulative distribution functions for the exponential distribution."
  \image latex random/ExponentialCdf.pdf "Some cumulative distribution functions for the exponential distribution." width=0.5\textwidth

  This package provides the the following functors for computing 
  exponential random deviates.
  - ExponentialGeneratorZiggurat
  - ExponentialGeneratorAcceptanceComplement
  - ExponentialGeneratorInversion

  I have implemented each of the exponential deviate generators as 
  an <em>adaptable unary function</em>, a functor that takes one argument.
  (See \ref numerical_random_austern1999 "Generic Programming and the STL".)
  The classes are templated on the floating point number type
  (\c double by default) and the uniform deviate generator type
  (DiscreteUniformGeneratorMt19937 by default).
  Below are a few ways of constructing an exponential deviate generator.
  \code
  // Use the default number type (double) and the default uniform deviate generator.
  typedef numerical::ExponentialGeneratorZiggurat<> Generator;
  Generator::DiscreteUniformGenerator uniform;
  Generator generator(&uniform); \endcode
  \code
  // Use single precision numbers and the default uniform deviate generator.
  typedef numerical::ExponentialGeneratorZiggurat<float> Generator;
  Generator::DiscreteUniformGenerator uniform;
  Generator generator(&uniform); \endcode
  \code
  typedef numerical::ExponentialGeneratorZiggurat<double, numerical::DiscreteUniformGeneratorMt19937Gsl> Generator;
  Generator::DiscreteUniformGenerator uniform;
  Generator generator(&uniform); \endcode
  

  Each class defines the following types.
  - \c Number is the floating point number type.
  - \c DiscreteUniformGenerator is the discrete uniform generator type.
  - \c argument_type is \c Number.
  - \c result_type is \c Number.

  Each generator has the following member functions.
  - \c operator()(Number mean) Return an exponential deviate with 
  the specified mean.
  - \c operator()() Return an exponential deviate with unit mean.
  - seed(typename DiscreteUniformGenerator::result_type seedValue) 
  Seed the uniform generator.
  - getDiscreteUniformGenerator() Get the discrete, uniform generator.
  .
  \code
  double x = generator(2.0);
  double y = generator();
  generator.seed(123U);
  Generator::DiscreteUniformGenerator* uniform = generator.getDiscreteUniformGenerator(); \endcode

  The default exponential deviate generator is defined to be
  ExponentialGeneratorZiggurat with the macro 
  \c EXPONENTIAL_GENERATOR_DEFAULT in the file Default.h .  Some of the
  \ref numerical_random_poisson "Poisson"
  generators in this package are templated on 
  the exponential generator type.  They use this macro to define
  the default generator.
  If you would like to change the default, just define the macro before 
  you include files from this package.

  The table below gives the execution times for calling the exponential 
  generators.

  <table>
  <tr> 
  <th> Functor 
  <th> Execution Time (nanoseconds)
  <tr> 
  <td> ExponentialGeneratorZiggurat
  <td> 10.7
  <tr> 
  <td> ExponentialGeneratorAcceptanceComplement
  <td> 10.7
  <tr> 
  <td> ExponentialGeneratorInversion
  <td> 60.7
  </table>
*/

#endif

END_NAMESPACE_NUMERICAL
