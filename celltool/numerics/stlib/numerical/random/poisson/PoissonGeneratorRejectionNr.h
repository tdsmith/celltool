// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorRejectionNr.h
  \brief Uniform random deviates.
*/

#if !defined(__numerical_PoissonGeneratorRejectionNr_h__)
#define __numerical_PoissonGeneratorRejectionNr_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include "../../specialFunctions/Gamma.h"
#include "../../constants.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorRejectionNr)
#define DEBUG_PoissonGeneratorRejectionNr
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates using the rejection method.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  \note: This algorithm does not give correct results for means smaller
  than 1.

  This functor is adapted from the rejection method in "Numerical Recipes".
  It returns an integer value that is a random deviate drawn from a Poisson
  distribution with specified mean.

  \image html random/poisson/same/sameRejectionNr.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameRejectionNr.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentRejectionNr.jpg "Execution times for different means."
  \image latex random/poisson/different/differentRejectionNr.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionRejectionNr.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionRejectionNr.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorRejectionNr {
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
  Number _sq, _alxm, _g, _oldm;
  LogarithmOfGamma<Number> _logarithmOfGamma;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  PoissonGeneratorRejectionNr();

public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorRejectionNr(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator),
    _sq(),
    _alxm(),
    _g(),
    _oldm(-1.0),
    _logarithmOfGamma()
  {}

  //! Copy constructor.
  PoissonGeneratorRejectionNr(const PoissonGeneratorRejectionNr& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator),
    _sq(other._sq),
    _alxm(other._alxm),
    _g(other._g),
    _oldm(other._oldm),
    _logarithmOfGamma(other._logarithmOfGamma)
  {}

  //! Assignment operator.
  PoissonGeneratorRejectionNr&
  operator=(const PoissonGeneratorRejectionNr& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
      _sq = other._sq;
      _alxm = other._alxm;
      _g = other._g;
      _oldm = other._oldm;
      _logarithmOfGamma = other._logarithmOfGamma;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorRejectionNr()
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

#define __numerical_random_PoissonGeneratorRejectionNr_ipp__
#include "PoissonGeneratorRejectionNr.ipp"
#undef __numerical_random_PoissonGeneratorRejectionNr_ipp__

#endif
