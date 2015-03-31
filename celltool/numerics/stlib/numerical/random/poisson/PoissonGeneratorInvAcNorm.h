// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInvAcNorm.h
  \brief Poisson deviates using inversion, acceptance-complement, and normal approximation.
*/

#if !defined(__numerical_PoissonGeneratorInvAcNorm_h__)
#define __numerical_PoissonGeneratorInvAcNorm_h__

#include "PoissonGeneratorInversionChopDown.h"
#include "PoissonGeneratorAcceptanceComplementWinrand.h"
#include "PoissonGeneratorNormal.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorInvAcNorm)
#define DEBUG_PoissonGeneratorInvAcNorm
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using inversion, acceptance-complement, and normal approximation.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor returns an integer value that is a random deviate drawn from a 
  Poisson distribution with specified mean.  For small means the algorithm 
  uses the inversion (chop-down) method (see PoissonGeneratorInversionChopDown);
  for medium means it uses the acceptance-complement method 
  (see PoissonGeneratorAcceptanceComplementWinrand); for large means it uses
  normal approximation (see PoissonGeneratorNormal).

  \image html random/poisson/same/sameInvAcNormSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInvAcNormSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameInvAcNormLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInvAcNormLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentInvAcNormSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInvAcNormSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentInvAcNormLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInvAcNormLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionInvAcNormSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInvAcNormSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInvAcNormLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInvAcNormLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorInvAcNorm {
public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef Number argument_type;
  //! The result type.
  typedef int result_type;
  //! The discrete uniform generator.
  typedef Uniform DiscreteUniformGenerator;
  //! The normal generator.
  typedef Normal<Number, DiscreteUniformGenerator> NormalGenerator;

  //
  // Member data.
  //

private:

  //! The inversion method.
  PoissonGeneratorInversionChopDown<Number, DiscreteUniformGenerator> _inversion;
  //! The acceptance-complement method.
  PoissonGeneratorAcceptanceComplementWinrand
  <Number, DiscreteUniformGenerator, Normal> _acceptanceComplementWinrand;
  //! The normal approximation method.
  PoissonGeneratorNormal<Number, DiscreteUniformGenerator, Normal> _normal;
  //! The normal deviates for means greater than this.
  Number _normalThreshhold;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  PoissonGeneratorInvAcNorm();

public:

  //! Construct using the normal generator and the threshhold.
  explicit
  PoissonGeneratorInvAcNorm(NormalGenerator* normalGenerator,
			  Number normalThreshhold = 
			  std::numeric_limits<Number>::max());

  //! Copy constructor.
  PoissonGeneratorInvAcNorm(const PoissonGeneratorInvAcNorm& other) :
    _inversion(other._inversion),
    _acceptanceComplementWinrand(other._acceptanceComplementWinrand),
    _normal(other._normal),
    _normalThreshhold(other._normalThreshhold)
  {}

  //! Assignment operator.
  PoissonGeneratorInvAcNorm&
  operator=(const PoissonGeneratorInvAcNorm& other) {
    if (this != &other) {
      _inversion = other._inversion;
      _acceptanceComplementWinrand = other._acceptanceComplementWinrand;
      _normal = other._normal;
      _normalThreshhold = other._normalThreshhold;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInvAcNorm()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _acceptanceComplementWinrand.seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorInvAcNorm_ipp__
#include "PoissonGeneratorInvAcNorm.ipp"
#undef __numerical_random_PoissonGeneratorInvAcNorm_ipp__

#endif
