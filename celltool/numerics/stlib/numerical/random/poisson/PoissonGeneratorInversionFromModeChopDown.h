// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionFromModeChopDown.h
  \brief Inversion from the mode method of generating Poisson deviates.
*/

#if !defined(__numerical_PoissonGeneratorInversionFromModeChopDown_h__)
#define __numerical_PoissonGeneratorInversionFromModeChopDown_h__

#include "../uniform/ContinuousUniformGenerator.h"

#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
#include "PoissonPdfAtTheMode.h"
#else
#include "PoissonPdfCached.h"
#endif

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_PoissonGeneratorInversionFromModeChopDown)
#define DEBUG_numerical_PoissonGeneratorInversionFromModeChopDown
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Inversion from the mode method of generating Poisson deviates.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  CONTINUE: The mean absolute deviation is bounded above by the standard 
  deviation.

  \image html random/poisson/same/sameInversionFromModeChopDown.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionFromModeChopDown.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionFromModeChopDown.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionFromModeChopDown.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionFromModeChopDown.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionFromModeChopDown.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorInversionFromModeChopDown {
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

#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
  PoissonPdfAtTheMode<Number> _pdf;
#else
  PoissonPdfCached<Number> _pdf;
#endif
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  Number _oldMean, _oldPdf;
#endif

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  PoissonGeneratorInversionFromModeChopDown();

  //! Copy constructor not implemented.
  PoissonGeneratorInversionFromModeChopDown
  (const PoissonGeneratorInversionFromModeChopDown&);

  //! Assignment operator not implemented.
  PoissonGeneratorInversionFromModeChopDown&
  operator=(const PoissonGeneratorInversionFromModeChopDown&);

public:

  //! Construct using the uniform generator and the maximum mean.
  explicit
  PoissonGeneratorInversionFromModeChopDown(DiscreteUniformGenerator* generator,
					  const Number maximumMean) :
    _discreteUniformGenerator(generator),
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    _pdf(0, maximumMean, 100)
#else
    _pdf(maximumMean + 1)
#endif
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    ,_oldMean(-1)
    ,_oldPdf(-1)
#endif
  {}

  //! Destructor.
  ~PoissonGeneratorInversionFromModeChopDown()
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

#define __numerical_random_PoissonGeneratorInversionFromModeChopDown_ipp__
#include "PoissonGeneratorInversionFromModeChopDown.ipp"
#undef __numerical_random_PoissonGeneratorInversionFromModeChopDown_ipp__

#endif
