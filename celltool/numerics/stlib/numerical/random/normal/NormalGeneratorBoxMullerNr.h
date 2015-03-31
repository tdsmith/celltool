// -*- C++ -*-

/*! 
  \file numerical/random/normal/NormalGeneratorBoxMullerNr.h
  \brief Normal random deviate with zero mean and unit variance.
*/

#if !defined(__numerical_NormalGeneratorBoxMullerNr_h__)
#define __numerical_NormalGeneratorBoxMullerNr_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_NormalGeneratorBoxMullerNr)
#define DEBUG_NormalGeneratorBoxMullerNr
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Normal random deviate with zero mean and unit variance.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.  By default it is
  UniformRandom1.  (You can also use UniformRandom0, UniformRandom2, or 
  something else.)  This generator can be initialized in the constructor
  or with seed().

  This functor is adapted from the Box-Muller algorithm 
  presented in "Numerical Recipes".
  It returns a floating point value that is a random deviate drawn from a 
  normal (Gaussian) distribution with zero mean and unit variance.
*/
template<typename T = double, 
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class NormalGeneratorBoxMullerNr {
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The continuous uniform generator.
  typedef ContinuousUniformGeneratorClosed<DiscreteUniformGenerator>
  ContinuousUniformGenerator;
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

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;
  bool _haveCachedGenerator;
  Number _cachedGenerator;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  NormalGeneratorBoxMullerNr();
  
public:

  //! Construct using the uniform generator.
  explicit
  NormalGeneratorBoxMullerNr(DiscreteUniformGenerator* generator) :
    _continuousUniformGenerator(generator),
    _haveCachedGenerator(false),
    _cachedGenerator(0)
  {}

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  NormalGeneratorBoxMullerNr(const NormalGeneratorBoxMullerNr& other) :
    _continuousUniformGenerator(other._continuousUniformGenerator),
    _haveCachedGenerator(other._haveCachedGenerator),
    _cachedGenerator(other._cachedGenerator)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  NormalGeneratorBoxMullerNr&
  operator=(const NormalGeneratorBoxMullerNr& other) {
    if (this != &other) {
      _continuousUniformGenerator = other._continuousUniformGenerator;
      _haveCachedGenerator = other._haveCachedGenerator;
      _cachedGenerator = other._cachedGenerator;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~NormalGeneratorBoxMullerNr()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _continuousUniformGenerator.seed(seedValue);
  }

  //! Return a standard normal deviate.
  result_type
  operator()();

  //! Return a normal deviate with specified mean and variance.
  result_type
  operator()(const Number mean, const Number variance) {
    return std::sqrt(variance) * operator()() + mean;
  }

  //! Get the discrete uniform generator.
  DiscreteUniformGenerator*
  getDiscreteUniformGenerator() {
    return _continuousUniformGenerator.getDiscreteUniformGenerator();
  }
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_NormalGeneratorBoxMullerNr_ipp__
#include "NormalGeneratorBoxMullerNr.ipp"
#undef __numerical_random_NormalGeneratorBoxMullerNr_ipp__

#endif
