// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorExponentialInterArrivalUsingUniform.h
  \brief Poisson deviate using exponential deviate inter-arrival times.
*/

#if !defined(__numerical_PoissonGeneratorExponentialInterArrivalUsingUniform_h__)
#define __numerical_PoissonGeneratorExponentialInterArrivalUsingUniform_h__

#include "../uniform/ContinuousUniformGenerator.h"

#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
#include "../../interpolation/hermite.h"
// CONTINUE: Use ads.
#include <ext/functional>
#endif

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorExponentialInterArrivalUsingUniform)
#define DEBUG_PoissonGeneratorExponentialInterArrivalUsingUniform
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor is adapted from the direct method of computing Poisson
  deviates presented in "Numerical Recipes".  The primary change I have 
  made is an optimization for very small means (\f$\mathrm{mean} \ll 1\f$).
  This functor returns an integer value that is a random deviate drawn from a 
  Poisson distribution with specified mean.  This is a practical method 
  for small means.

  \image html random/poisson/same/sameExponentialInterArrivalUsingUniform.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameExponentialInterArrivalUsingUniform.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentExponentialInterArrivalUsingUniform.jpg "Execution times for different means."
  \image latex random/poisson/different/differentExponentialInterArrivalUsingUniform.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionExponentialInterArrivalUsingUniform.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionExponentialInterArrivalUsingUniform.pdf "Execution times for a distribution of means." width=0.5\textwidth

  One might compute Poisson deviates for small means.
  If the the mean is usually less than one, the 
  defining NUMERICAL_POISSON_SMALL_MEAN may improve performance.
  In this case it can usually avoid computing an exponential
  for small means.

  If your application often calls this functor with exactly the same
  mean on successive calls, then defining the macro
  NUMERICAL_POISSON_CACHE_OLD_MEAN will improve the performance.  When it is
  defined, it caches values like the exponential of the mean and only
  recomputes them when the mean is different from the previous call.
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorExponentialInterArrivalUsingUniform {
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
  numerical::Hermite<Number> _expNeg;
#endif

#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  Number _g, _oldm;
#endif

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  PoissonGeneratorExponentialInterArrivalUsingUniform();

public:

  // If we are using the approximate exponential, we need to specify the 
  // maximum mean.
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION

  //! Construct using the uniform generator and the maximum mean.
  explicit
  PoissonGeneratorExponentialInterArrivalUsingUniform
  (DiscreteUniformGenerator* generator, const Number maximumMean) :
    _discreteUniformGenerator(generator)
    // Approximate exp(-x) for x in [0..maximumMean).  Choose a patch length of
    // about 1/100.  Add 0.01 to the maximumMean so there is not an error if 
    // this functor is called with maximumMean.
    ,_expNeg(__gnu_cxx::compose1(std::ptr_fun<double, double>(std::exp),
				 std::negate<double>()), 
	     __gnu_cxx::compose1(std::negate<double>(),
				 __gnu_cxx::compose1(std::ptr_fun<double, double>(std::exp),
						     std::negate<double>())),
	     0.0, maximumMean + 0.01, int(100 * maximumMean) + 1)
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    ,_g()
    ,_oldm(-1.0)
#endif
  {}

#else // NUMERICAL_POISSON_HERMITE_APPROXIMATION

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorExponentialInterArrivalUsingUniform
  (DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    ,_g()
    ,_oldm(-1.0)
#endif
  {}

#endif

  //! Copy constructor.
  PoissonGeneratorExponentialInterArrivalUsingUniform
  (const PoissonGeneratorExponentialInterArrivalUsingUniform& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    ,_expNeg(other._expNeg)
#endif
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    ,_g(other._g)
    ,_oldm(other._oldm)
#endif
  {}

  //! Assignment operator.
  PoissonGeneratorExponentialInterArrivalUsingUniform&
  operator=(const PoissonGeneratorExponentialInterArrivalUsingUniform& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
      _expNeg = other._expNeg;
#endif
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
      _g = other._g;
      _oldm = other._oldm;
#endif
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorExponentialInterArrivalUsingUniform()
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

#define __numerical_random_PoissonGeneratorExponentialInterArrivalUsingUniform_ipp__
#include "PoissonGeneratorExponentialInterArrivalUsingUniform.ipp"
#undef __numerical_random_PoissonGeneratorExponentialInterArrivalUsingUniform_ipp__

#endif
