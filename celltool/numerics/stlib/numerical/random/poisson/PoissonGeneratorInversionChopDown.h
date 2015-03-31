// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionChopDown.h
  \brief Poisson deviates using inversion (chop-down).
*/

#if !defined(__numerical_PoissonGeneratorInversionChopDown_h__)
#define __numerical_PoissonGeneratorInversionChopDown_h__

#include "../uniform/ContinuousUniformGenerator.h"

#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
#include "../../interpolation/hermite.h"
// CONTINUE: Use ads.
#include <ext/functional>
#endif

#ifdef NUMERICAL_POISSON_STORE_INVERSE
#include "../../../ads/array/Array.h"
#endif

#include <cassert>
#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_PoissonGeneratorInversionChopDown)
#define DEBUG_numerical_PoissonGeneratorInversionChopDown
#endif

#define __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__
#include "PoissonGeneratorInversionMaximumMean.ipp"
#undef __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates using the inversion (chop-down) method.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  \image html random/poisson/same/sameInversionChopDown.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionChopDown.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionChopDown.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionChopDown.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionChopDown.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionChopDown.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \note This algorithm evaluates the probability density function.  For 
  mean \f$\mu\f$, this is
  \f[
  P(n) = \frac{e^{-\mu} \mu^n}{n!}.
  \f]
  If the mean is large enough, evaluating the exponential will cause underflow.
  Typically this means that what should be a small positive number is 
  truncated to zero.  In this case, the algorithm gives incorrect results.
  The maximum allowed mean is
  <pre>- std::log(std::numeric_limits<Number>::min())</pre>
  (I check this with an assertion when debugging is enabled.)
  When using double precision floating-point numbers, do not call 
  this function with arguments greater than 708.  For single 
  precision numbers, the threshhold is 87.
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorInversionChopDown {
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
  Number _oldMean, _oldExponential;
#endif

#ifdef NUMERICAL_POISSON_STORE_INVERSE
  ads::Array<1, Number> _inverse;
#endif

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  PoissonGeneratorInversionChopDown();

  //
  // Member functions.
  //

public:

  // If we are using the approximate exponential, we need to specify the 
  // maximum mean.
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION

  //! Construct using the uniform generator and the maximum mean.
  explicit
  PoissonGeneratorInversionChopDown(DiscreteUniformGenerator* generator,
				  const Number maximumMean) :
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
    ,_oldMean(-1)
    ,_oldExponential(-1)
#endif
#ifdef NUMERICAL_POISSON_STORE_INVERSE
    // CONTINUE: choose an approprate size.
    ,_inverse(2 * PoissonGeneratorInversionMaximumMean<Number>::Value)
#endif
  {
#ifdef NUMERICAL_POISSON_STORE_INVERSE
    _inverse[0] = 0;
    for (int i = 1; i != _inverse.size(); ++i) {
      _inverse[i] = 1.0 / i;
    }
#endif
  }

#else // NUMERICAL_POISSON_HERMITE_APPROXIMATION

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorInversionChopDown(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    ,_oldMean(-1)
    ,_oldExponential(-1)
#endif
#ifdef NUMERICAL_POISSON_STORE_INVERSE
    // CONTINUE: choose an approprate size.
    ,_inverse(2 * PoissonGeneratorInversionMaximumMean<Number>::Value)
#endif
  {
#ifdef NUMERICAL_POISSON_STORE_INVERSE
    _inverse[0] = 0;
    for (int i = 1; i != _inverse.size(); ++i) {
      _inverse[i] = 1.0 / i;
    }
#endif
  }

#endif


  //! Copy constructor.
  PoissonGeneratorInversionChopDown(const PoissonGeneratorInversionChopDown& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    ,_expNeg(other._expNeg)
#endif
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    ,_oldMean(other._oldMean)
    ,_oldExponential(other._oldExponential)
#endif
#ifdef NUMERICAL_POISSON_STORE_INVERSE
    ,_inverse(other._inverse)
#endif
  {}

  //! Assignment operator.
  PoissonGeneratorInversionChopDown&
  operator=(const PoissonGeneratorInversionChopDown& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
      _expNeg = other._expNeg;
#endif
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
      _oldMean = other._oldMean;
      _oldExponential = other._oldExponential;
#endif
#ifdef NUMERICAL_POISSON_STORE_INVERSE
    _inverse = other._inverse;
#endif
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInversionChopDown()
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

#define __numerical_random_PoissonGeneratorInversionChopDown_ipp__
#include "PoissonGeneratorInversionChopDown.ipp"
#undef __numerical_random_PoissonGeneratorInversionChopDown_ipp__

#endif
