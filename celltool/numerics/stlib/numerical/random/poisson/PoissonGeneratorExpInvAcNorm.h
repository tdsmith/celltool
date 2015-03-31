// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorExpInvAcNorm.h
  \brief Poisson deviates using exponential inter-arrival, inversion, acceptance-complement, and normal approximation.
*/

#if !defined(__numerical_PoissonGeneratorExpInvAcNorm_h__)
#define __numerical_PoissonGeneratorExpInvAcNorm_h__

#include "PoissonGeneratorExponentialInterArrival.h"
#include "PoissonGeneratorInversionChopDown.h"
#include "PoissonGeneratorAcceptanceComplementWinrand.h"
#include "PoissonGeneratorNormal.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorExpInvAcNorm)
#define DEBUG_PoissonGeneratorExpInvAcNorm
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using exponential inter-arrival, inversion, acceptance-complement, and normal approximation.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor returns an integer value that is a random deviate drawn from a 
  Poisson distribution with specified mean.  For very small means the algorithm 
  uses the exponential inter-arrival method (see 
  PoissonGeneratorExponentialInterArrival); for small means it uses the
  chop-down version of inversion (see PoissonGeneratorInversionChopDown);
  for medium means it uses the acceptance-complement method 
  (see PoissonGeneratorAcceptanceComplementWinrand); for large means it uses
  normal approximation (see PoissonGeneratorNormal).

  \image html random/poisson/same/sameExpInvAcNormSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameExpInvAcNormSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameExpInvAcNormLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameExpInvAcNormLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentExpInvAcNormSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentExpInvAcNormSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentExpInvAcNormLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentExpInvAcNormLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionExpInvAcNormSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionExpInvAcNormSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionExpInvAcNormLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionExpInvAcNormLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Exponential = 
	 EXPONENTIAL_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorExpInvAcNorm {
public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef Number argument_type;
  //! The result type.
  typedef int result_type;
  //! The discrete uniform generator.
  typedef Uniform DiscreteUniformGenerator;
  //! The exponential generator.
  typedef Exponential<Number, DiscreteUniformGenerator> ExponentialGenerator;
  //! The normal generator.
  typedef Normal<Number, DiscreteUniformGenerator> NormalGenerator;

  //
  // Member data.
  //

private:

  //! The exponential inter-arrival method.
  PoissonGeneratorExponentialInterArrival
  <Number, DiscreteUniformGenerator, Exponential> _exponentialInterArrival;
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
  PoissonGeneratorExpInvAcNorm();

public:

  //! Construct using the exponential generator, the normal generator, and the threshhold.
  /*!
    Use the discrete from the exponential generator in the inversion method.
  */
  explicit
  PoissonGeneratorExpInvAcNorm(ExponentialGenerator* exponentialGenerator,
			       NormalGenerator* normalGenerator,
			       Number normalThreshhold = 
			       std::numeric_limits<Number>::max());

  //! Copy constructor.
  PoissonGeneratorExpInvAcNorm(const PoissonGeneratorExpInvAcNorm& other) :
    _exponentialInterArrival(other._exponentialInterArrival),
    _inversion(other._inversion),
    _acceptanceComplementWinrand(other._acceptanceComplementWinrand),
    _normal(other._normal),
    _normalThreshhold(other._normalThreshhold)
  {}

  //! Assignment operator.
  PoissonGeneratorExpInvAcNorm&
  operator=(const PoissonGeneratorExpInvAcNorm& other) {
    if (this != &other) {
      _exponentialInterArrival = other._exponentialInterArrival;
      _inversion = other._inversion;
      _acceptanceComplementWinrand = other._acceptanceComplementWinrand;
      _normal = other._normal;
      _normalThreshhold = other._normalThreshhold;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorExpInvAcNorm()
  {}

  //! Seed the uniform random number generators for each of the methods.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _exponentialInterArrival.seed(seedValue);
    // No need to seed the inversion method as it is borrowing the uniform
    // generator from the exponential method.
    _acceptanceComplementWinrand.seed(seedValue);
    _normal.seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorExpInvAcNorm_ipp__
#include "PoissonGeneratorExpInvAcNorm.ipp"
#undef __numerical_random_PoissonGeneratorExpInvAcNorm_ipp__

#endif
