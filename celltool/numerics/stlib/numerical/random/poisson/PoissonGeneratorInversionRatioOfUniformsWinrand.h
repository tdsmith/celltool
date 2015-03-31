// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionRatioOfUniformsWinrand.h
  \brief Poisson deviates using WinRand implementation of inversion/ratio of uniforms.
*/

#if !defined(__numerical_PoissonGeneratorInversionRatioOfUniformsWinrand_h__)
#define __numerical_PoissonGeneratorInversionRatioOfUniformsWinrand_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorInversionRatioOfUniformsWinrand)
#define DEBUG_PoissonGeneratorInversionRatioOfUniformsWinrand
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using WinRand implementation of inversion/ratio of uniforms.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor computes Poisson deviates using the 
  <a href="http://www.stat.tugraz.at/stadl/random.html">WinRand</a>
  implementation of inversion/ratio of uniforms.

  Modifications:
  - Changed <code>double</code> to <code>Number</code>
  - Use my own uniform random deviate generator.


  \image html random/poisson/same/sameInversionRatioOfUniformsWinrandSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionRatioOfUniformsWinrandSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameInversionRatioOfUniformsWinrandLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionRatioOfUniformsWinrandLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentInversionRatioOfUniformsWinrandSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionRatioOfUniformsWinrandSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionRatioOfUniformsWinrandLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionRatioOfUniformsWinrandLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionInversionRatioOfUniformsWinrandSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionRatioOfUniformsWinrandSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionRatioOfUniformsWinrandLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionRatioOfUniformsWinrandLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorInversionRatioOfUniformsWinrand {
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
  PoissonGeneratorInversionRatioOfUniformsWinrand();

public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorInversionRatioOfUniformsWinrand
  (DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
  {}

  //! Copy constructor.
  PoissonGeneratorInversionRatioOfUniformsWinrand
  (const PoissonGeneratorInversionRatioOfUniformsWinrand& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorInversionRatioOfUniformsWinrand&
  operator=(const PoissonGeneratorInversionRatioOfUniformsWinrand& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInversionRatioOfUniformsWinrand()
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

#define __numerical_random_PoissonGeneratorInversionRatioOfUniformsWinrand_ipp__
#include "PoissonGeneratorInversionRatioOfUniformsWinrand.ipp"
#undef __numerical_random_PoissonGeneratorInversionRatioOfUniformsWinrand_ipp__

#endif
