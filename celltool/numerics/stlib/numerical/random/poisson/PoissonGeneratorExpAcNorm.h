// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorExpAcNorm.h
  \brief Poisson deviates using exponential inter-arrival, acceptance-complement, and normal approximation.
*/

#if !defined(__numerical_PoissonGeneratorExpAcNorm_h__)
#define __numerical_PoissonGeneratorExpAcNorm_h__

#include "PoissonGeneratorExponentialInterArrival.h"
#include "PoissonGeneratorAcceptanceComplementWinrand.h"
#include "PoissonGeneratorNormal.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorExpAcNorm)
#define DEBUG_PoissonGeneratorExpAcNorm
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using exponential inter-arrival, acceptance-complement, and normal approximation.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor returns an integer value that is a random deviate drawn from a 
  Poisson distribution with specified mean.  For small means the algorithm 
  uses the exponential inter-arrival method (see 
  PoissonGeneratorExponentialInterArrival);
  for medium means it uses the acceptance-complement method 
  (see PoissonGeneratorAcceptanceComplementWinrand); for large means it uses
  normal approximation (see PoissonGeneratorNormal).

  \image html random/poisson/same/sameExpAcNormSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameExpAcNormSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameExpAcNormLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameExpAcNormLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentExpAcNormSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentExpAcNormSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentExpAcNormLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentExpAcNormLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionExpAcNormSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionExpAcNormSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionExpAcNormLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionExpAcNormLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double, 
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Exponential = 
	 EXPONENTIAL_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorExpAcNorm {
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
  PoissonGeneratorExpAcNorm();

public:

  //! Construct using the exponential generator, the normal generator, and the threshhold.
  explicit
  PoissonGeneratorExpAcNorm(ExponentialGenerator* exponentialGenerator,
			  NormalGenerator* normalGenerator,
			  Number normalThreshhold = 
			  std::numeric_limits<Number>::max()) :
    _exponentialInterArrival(exponentialGenerator),
    _acceptanceComplementWinrand(normalGenerator),
    _normal(normalGenerator),
    _normalThreshhold(normalThreshhold)
  {}

  //! Copy constructor.
  PoissonGeneratorExpAcNorm(const PoissonGeneratorExpAcNorm& other) :
    _exponentialInterArrival(other._exponentialInterArrival),
    _acceptanceComplementWinrand(other._acceptanceComplementWinrand),
    _normal(other._normal),
    _normalThreshhold(other._normalThreshhold)
  {}

  //! Assignment operator.
  PoissonGeneratorExpAcNorm&
  operator=(const PoissonGeneratorExpAcNorm& other) {
    if (this != &other) {
      _exponentialInterArrival = other._exponentialInterArrival;
      _acceptanceComplementWinrand = other._acceptanceComplementWinrand;
      _normal = other._normal;
      _normalThreshhold = other._normalThreshhold;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorExpAcNorm()
  {}

  //! Seed the uniform random number generators for each of the methods.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _exponentialInterArrival.seed(seedValue);
    _acceptanceComplementWinrand.seed(seedValue);
    _normal.seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorExpAcNorm_ipp__
#include "PoissonGeneratorExpAcNorm.ipp"
#undef __numerical_random_PoissonGeneratorExpAcNorm_ipp__

#endif
