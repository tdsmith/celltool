// -*- C++ -*-

/*! 
  \file numerical/random/normal/NormalGeneratorBoxMullerGsl.h
  \brief Normal random deviate with zero mean and unit variance.
*/

#if !defined(__numerical_NormalGeneratorBoxMullerGsl_h__)
#define __numerical_NormalGeneratorBoxMullerGsl_h__

#include "../uniform/DiscreteUniformGeneratorMt19937Gsl.h"

#include <gsl/gsl_randist.h>

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_NormalGeneratorBoxMullerGsl)
#define DEBUG_NormalGeneratorBoxMullerGsl
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T = double, 
	 class Generator = DiscreteUniformGeneratorMt19937Gsl>
class NormalGeneratorBoxMullerGsl;

//! Normal random deviate with zero mean and unit variance.
/*!
  \param T The number type.  By default it is double.

  This is a template specialization for the case that the uniform deviate
  generator is UniformGeneratorMt19937Gsl .
  This generator can be initialized in the constructor or with seed().

  This functor wraps the function call to the Box-Muller
  algorithm for computing Gaussian deviates in the
  <a href="http://www.gnu.org/software/gsl/">GNU Scientific Library</a>.
  It returns a floating point value that is a random deviate drawn from a 
  normal (Gaussian) distribution with zero mean and unit variance.
*/
template<typename T>
class NormalGeneratorBoxMullerGsl<T, DiscreteUniformGeneratorMt19937Gsl> {
public:

  //! The discrete uniform generator.
  typedef DiscreteUniformGeneratorMt19937Gsl DiscreteUniformGenerator;
  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef Number result_type;

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
  NormalGeneratorBoxMullerGsl();

public:

  //! Construct using the uniform generator.
  explicit
  NormalGeneratorBoxMullerGsl(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator) 
  {}

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  NormalGeneratorBoxMullerGsl(const NormalGeneratorBoxMullerGsl& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  NormalGeneratorBoxMullerGsl&
  operator=(const NormalGeneratorBoxMullerGsl& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~NormalGeneratorBoxMullerGsl()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _discreteUniformGenerator->seed(seedValue);
  }

  //! Return a standard normal deviate.
  result_type
  operator()() {
    return gsl_ran_ugaussian(_discreteUniformGenerator->getGenerator());
  }

  //! Return a normal deviate with specified mean and variance.
  result_type
  operator()(const Number mean, const Number variance) {
    return std::sqrt(variance) * operator()() + mean;
  }

  //! Get the discrete uniform generator.
  DiscreteUniformGenerator*
  getDiscreteUniformGenerator() {
    return _discreteUniformGenerator;
  }
};


END_NAMESPACE_NUMERICAL

#endif
