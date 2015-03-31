// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorIfmAcNorm.h
  \brief Poisson deviates using inversion, acceptance-complement, and normal approximation.
*/

#if !defined(__numerical_PoissonGeneratorIfmAcNorm_h__)
#define __numerical_PoissonGeneratorIfmAcNorm_h__

#include "PoissonGeneratorInversionFromModeBuildUp.h"
#include "PoissonGeneratorAcceptanceComplementWinrand.h"
#include "PoissonGeneratorNormal.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorIfmAcNorm)
#define DEBUG_PoissonGeneratorIfmAcNorm
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using inversion from the mode, acceptance-complement, and normal approximation.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor returns an integer value that is a random deviate drawn from a 
  Poisson distribution with specified mean.  For small means the algorithm 
  uses the inversion from the mode (build-up) method 
  (see PoissonGeneratorInversionFromModeBuildUp);
  for medium means it uses the acceptance-complement method 
  (see PoissonGeneratorAcceptanceComplementWinrand); for large means it uses
  normal approximation (see PoissonGeneratorNormal).

  \image html random/poisson/same/sameIfmAcNormSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameIfmAcNormSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameIfmAcNormLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameIfmAcNormLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentIfmAcNormSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentIfmAcNormSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentIfmAcNormLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentIfmAcNormLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionIfmAcNormSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionIfmAcNormSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionIfmAcNormLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionIfmAcNormLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorIfmAcNorm {
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

  //! The inversion from the mode method.
  PoissonGeneratorInversionFromModeBuildUp<Number, DiscreteUniformGenerator> 
  _inversionFromTheMode;
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
  PoissonGeneratorIfmAcNorm();

public:

  //! Construct using the normal generator and the threshhold.
  explicit
  PoissonGeneratorIfmAcNorm(NormalGenerator* normalGenerator,
			  Number normalThreshhold = 
			  std::numeric_limits<Number>::max());

  //! Copy constructor.
  PoissonGeneratorIfmAcNorm(const PoissonGeneratorIfmAcNorm& other) :
    _inversionFromTheMode(other._inversionFromTheMode),
    _acceptanceComplementWinrand(other._acceptanceComplementWinrand),
    _normal(other._normal),
    _normalThreshhold(other._normalThreshhold)
  {}

  //! Assignment operator.
  PoissonGeneratorIfmAcNorm&
  operator=(const PoissonGeneratorIfmAcNorm& other) {
    if (this != &other) {
      _inversionFromTheMode = other._inversionFromTheMode;
      _acceptanceComplementWinrand = other._acceptanceComplementWinrand;
      _normal = other._normal;
      _normalThreshhold = other._normalThreshhold;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorIfmAcNorm()
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

#define __numerical_random_PoissonGeneratorIfmAcNorm_ipp__
#include "PoissonGeneratorIfmAcNorm.ipp"
#undef __numerical_random_PoissonGeneratorIfmAcNorm_ipp__

#endif
