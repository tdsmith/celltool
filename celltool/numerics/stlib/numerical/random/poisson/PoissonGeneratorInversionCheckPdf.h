// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionCheckPdf.h
  \brief Uniform random deviates.
*/

#if !defined(__numerical_PoissonGeneratorInversionCheckPdf_h__)
#define __numerical_PoissonGeneratorInversionCheckPdf_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <limits>

#include <cmath>
#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_PoissonGeneratorInversionCheckPdf)
#define DEBUG_numerical_PoissonGeneratorInversionCheckPdf
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates using the inversion method.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This algorithm was communicated to me by Professor Dan Gillespie.

  \image html random/poisson/same/sameInversionCheckPdf.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionCheckPdf.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionCheckPdf.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionCheckPdf.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionCheckPdf.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionCheckPdf.pdf "Execution times for a distribution of means." width=0.5\textwidth

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
class PoissonGeneratorInversionCheckPdf {
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

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  PoissonGeneratorInversionCheckPdf();
  
public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorInversionCheckPdf(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
  {}

  //! Copy constructor.
  PoissonGeneratorInversionCheckPdf(const PoissonGeneratorInversionCheckPdf& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorInversionCheckPdf&
  operator=(const PoissonGeneratorInversionCheckPdf& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInversionCheckPdf()
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

#define __numerical_random_PoissonGeneratorInversionCheckPdf_ipp__
#include "PoissonGeneratorInversionCheckPdf.ipp"
#undef __numerical_random_PoissonGeneratorInversionCheckPdf_ipp__

#endif
