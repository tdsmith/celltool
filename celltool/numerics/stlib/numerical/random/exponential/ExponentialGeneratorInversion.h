// -*- C++ -*-

/*! 
  \file numerical/random/exponential/ExponentialGeneratorInversion.h
  \brief Exponential random deviate with specified mean.
*/

#if !defined(__numerical_ExponentialGeneratorInversion_h__)
#define __numerical_ExponentialGeneratorInversion_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_ExponentialGeneratorInversion)
#define DEBUG_ExponentialGeneratorInversion
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Exponential random deviate with specified mean.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.

  See the \ref numerical_random_exponential "exponential deviate page"
  for general information.

  One can generate exponential deviates <em>X</em> from uniform deviates
  <em>U</em> with the usual method of inverting the CDF.  Let <em>U</em>
  be a uniform random deviate, \f$U \in (0..1)\f$.
  \f[
  X = \mathrm{cdf}_{\lamda}^{-1}(U)
  \f]
  \f[
  U = \mathrm{cdf}_{\lamda}(X) = 1 - e^{-\lambda X}
  \f]
  \f[
  X = - \frac{\ln(1 - U)}{\lambda}
  \f]
  We can simplify this formula by noting that if <em>U</em> is a uniform 
  random deviate, so is 1 - <em>U</em>.
  \f[
  X = - \frac{\ln U}{\lambda}
  \f]
  This class uses this formula to generate exponential deviates with the 
  inversion method.
*/
template<typename T = double, 
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class ExponentialGeneratorInversion {
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The continuous uniform generator.
  typedef ContinuousUniformGeneratorOpen<DiscreteUniformGenerator>
  ContinuousUniformGenerator;
  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef Number result_type;

private:

  //
  // Member data.
  //

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  ExponentialGeneratorInversion();

public:

  //! Construct using the uniform generator.
  explicit
  ExponentialGeneratorInversion(DiscreteUniformGenerator* generator) :
    _continuousUniformGenerator(generator) 
  {}

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  ExponentialGeneratorInversion(const ExponentialGeneratorInversion& other) :
    _continuousUniformGenerator(other._continuousUniformGenerator)
  {}

  //! Assignment operator.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  ExponentialGeneratorInversion&
  operator=(const ExponentialGeneratorInversion& other) {
    if (this != &other) {
      _continuousUniformGenerator = other._continuousUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~ExponentialGeneratorInversion()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _continuousUniformGenerator.seed(seedValue);
  }

  //! Return a standard exponential deviate.
  result_type
  operator()() {
    return - std::log(_continuousUniformGenerator());
  }

  //! Return an exponential deviate with specified mean.
  result_type
  operator()(const Number mean) {
    return mean * operator()();
  }

  //! Get the discrete uniform generator.
  DiscreteUniformGenerator*
  getDiscreteUniformGenerator() {
    return _continuousUniformGenerator.getDiscreteUniformGenerator();
  }
};


END_NAMESPACE_NUMERICAL

#endif
