// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorNormal.h
  \brief Generator for approximate Poisson deviates.
*/

#if !defined(__numerical_PoissonGeneratorNormal_h__)
#define __numerical_PoissonGeneratorNormal_h__

#include "../normal/Default.h"

#include <algorithm>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorNormal)
#define DEBUG_PoissonGeneratorNormal
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for approximate Poisson deviates.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().
  
  CONTINUE.

  \image html random/poisson/same/sameNormal.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameNormal.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentNormal.jpg "Execution times for different means."
  \image latex random/poisson/different/differentNormal.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionNormal.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionNormal.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double, 
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorNormal {
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

private:

  //
  // Member data.
  //

  //! The normal generator.
  NormalGenerator* _normalGenerator;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  PoissonGeneratorNormal();
  
public:

  //! Construct using the normal generator.
  explicit
  PoissonGeneratorNormal(NormalGenerator* normalGenerator) :
    _normalGenerator(normalGenerator)
  {}

  //! Copy constructor.
  PoissonGeneratorNormal(const PoissonGeneratorNormal& other) :
    _normalGenerator(other._normalGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorNormal&
  operator=(const PoissonGeneratorNormal& other) {
    if (this != &other) {
      _normalGenerator = other._normalGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorNormal()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _normalGenerator->seed(seedValue);
  }

  //! Return a normal approximation of a Poisson deviate with the specifed mean.
  /*!
    The normal deviate is rounded to the nearest integer.  We assume that
    the mean is large enough that the result is never negative.  (This is 
    assumed and not checked.)
  */
  result_type
  operator()(const argument_type mean) {
    return int((*_normalGenerator)(mean, mean) + 0.5);
  }
};


END_NAMESPACE_NUMERICAL

#endif
