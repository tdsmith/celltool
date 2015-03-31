// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionChopDownSimple.h
  \brief Poisson deviates using inversion (chop-down).
*/

#if !defined(__numerical_PoissonGeneratorInversionChopDownSimple_h__)
#define __numerical_PoissonGeneratorInversionChopDownSimple_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <cmath>
#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_PoissonGeneratorInversionChopDownSimple)
#define DEBUG_numerical_PoissonGeneratorInversionChopDownSimple
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates using the inversion (chop-down) method.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  \image html random/poisson/same/sameInversionChopDownSimple.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionChopDownSimple.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionChopDownSimple.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionChopDownSimple.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionChopDownSimple.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionChopDownSimple.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \note This algorithm evaluates the probability density function.  For 
  mean \f$\mu\f$, this is
  \f[
  P(n) = \frac{e^{-\mu} \mu^n}{n!}.
  \f]
  If the mean is large enough, evaluating the exponential will cause underflow.
  Typically this means that what should be a small positive number is 
  truncated to zero.  In this case, the algorithm gives incorrect results.
  The maximum allowed mean is
  <pre>- std::log(std::numeric_limits<Number>::min())</pre>
  (I check this with an assertion when debugging is enabled.)
  When using double precision floating-point numbers, do not call 
  this function with arguments greater than 708.  For single 
  precision numbers, the threshhold is 87.
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorInversionChopDownSimple {
public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef Number argument_type;
  //! The result type.
  typedef int result_type;
  //! The discrete uniform generator.
  typedef Uniform DiscreteUniformGenerator;

  //
  // Member data.
  //

private:

  //! The discrete uniform generator.
  DiscreteUniformGenerator* _discreteUniformGenerator;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  PoissonGeneratorInversionChopDownSimple();


  //
  // Member functions.
  //
public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorInversionChopDownSimple(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
  {}

  //! Copy constructor.
  PoissonGeneratorInversionChopDownSimple
  (const PoissonGeneratorInversionChopDownSimple& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorInversionChopDownSimple&
  operator=(const PoissonGeneratorInversionChopDownSimple& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInversionChopDownSimple()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _discreteUniformGenerator->seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorInversionChopDownSimple_ipp__
#include "PoissonGeneratorInversionChopDownSimple.ipp"
#undef __numerical_random_PoissonGeneratorInversionChopDownSimple_ipp__

#endif
