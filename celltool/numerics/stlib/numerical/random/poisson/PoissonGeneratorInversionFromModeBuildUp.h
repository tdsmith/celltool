// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionFromModeBuildUp.h
  \brief Inversion from the mode method of generating Poisson deviates.
*/

#if !defined(__numerical_PoissonGeneratorInversionFromModeBuildUp_h__)
#define __numerical_PoissonGeneratorInversionFromModeBuildUp_h__

#include "../uniform/ContinuousUniformGenerator.h"

#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
#include "PoissonPdfCdfAtTheMode.h"
#else
#include "PoissonPdfCached.h"
#include "PoissonCdfAtTheMode.h"
#endif

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_PoissonGeneratorInversionFromModeBuildUp)
#define DEBUG_numerical_PoissonGeneratorInversionFromModeBuildUp
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Inversion from the mode method of generating Poisson deviates.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  CONTINUE: The mean absolute deviation is bounded above by the standard 
  deviation.

  \image html random/poisson/same/sameInversionFromModeBuildUp.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionFromModeBuildUp.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionFromModeBuildUp.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionFromModeBuildUp.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionFromModeBuildUp.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionFromModeBuildUp.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorInversionFromModeBuildUp {
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
  PoissonPdfCdfAtTheMode<Number> _pdfCdf;
#else
  PoissonPdfCached<Number> _pdf;
  PoissonCdfAtTheMode<Number> _cdfAtTheMode;
#endif

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  PoissonGeneratorInversionFromModeBuildUp();

  //! Copy constructor not implemented.
  PoissonGeneratorInversionFromModeBuildUp
  (const PoissonGeneratorInversionFromModeBuildUp&);

  //! Assignment operator not implemented.
  PoissonGeneratorInversionFromModeBuildUp&
  operator=(const PoissonGeneratorInversionFromModeBuildUp&);

public:

  //! Construct using the uniform generator and the maximum mean.
  explicit
  PoissonGeneratorInversionFromModeBuildUp(DiscreteUniformGenerator* generator,
					 const Number maximumMean) :
    _discreteUniformGenerator(generator),
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    _pdfCdf(0, maximumMean, 100)
#else
    _pdf(maximumMean + 1),
    _cdfAtTheMode(maximumMean + 1)
#endif
  {}

  //! Destructor.
  ~PoissonGeneratorInversionFromModeBuildUp()
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

#define __numerical_random_PoissonGeneratorInversionFromModeBuildUp_ipp__
#include "PoissonGeneratorInversionFromModeBuildUp.ipp"
#undef __numerical_random_PoissonGeneratorInversionFromModeBuildUp_ipp__

#endif
