// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorDirectNr.h
  \brief Uniform random deviates.
*/

#if !defined(__numerical_PoissonGeneratorDirectNr_h__)
#define __numerical_PoissonGeneratorDirectNr_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorDirectNr)
#define DEBUG_PoissonGeneratorDirectNr
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor is adapted from the direct method of computing Poisson
  deviates presented in "Numerical Recipes".
  It returns an integer value that is a random deviate drawn from a 
  Poisson distribution with specified mean.  This is a practical method 
  for small means.

  \image html random/poisson/same/sameDirectNr.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameDirectNr.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentDirectNr.jpg "Execution times for different means."
  \image latex random/poisson/different/differentDirectNr.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionDirectNr.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionDirectNr.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorDirectNr {
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
  Number _g, _oldm;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  PoissonGeneratorDirectNr();
  
public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorDirectNr(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator),
    _g(),
    _oldm(-1.0)
  {}

  //! Copy constructor.
  PoissonGeneratorDirectNr(const PoissonGeneratorDirectNr& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator),
    _g(other._g),
    _oldm(other._oldm)
  {}

  //! Assignment operator.
  PoissonGeneratorDirectNr&
  operator=(const PoissonGeneratorDirectNr& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
      _g = other._g;
      _oldm = other._oldm;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorDirectNr()
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

#define __numerical_random_PoissonGeneratorDirectNr_ipp__
#include "PoissonGeneratorDirectNr.ipp"
#undef __numerical_random_PoissonGeneratorDirectNr_ipp__

#endif
