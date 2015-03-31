// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorAcceptanceComplementWinrand.h
  \brief Poisson deviates using the WinRand implementation of acceptance complement.
*/

#if !defined(__numerical_PoissonGeneratorAcceptanceComplementWinrand_h__)
#define __numerical_PoissonGeneratorAcceptanceComplementWinrand_h__

#include "../normal/Default.h"

#include <algorithm>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorAcceptanceComplementWinrand)
#define DEBUG_PoissonGeneratorAcceptanceComplementWinrand
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using the WinRand implementation of acceptance complement.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.  
  This generator can be initialized in the constructor or with seed().
  
  This functor computes Poisson deviates using the 
  <a href="http://www.stat.tugraz.at/stadl/random.html">WinRand</a>
  implementation of acceptance complement.

  Modifications:
  - Changed <code>double</code> to <code>Number</code>
  - Use my own uniform random deviate generator and normal deviate generator.


  \image html random/poisson/same/sameAcceptanceComplementWinrand.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameAcceptanceComplementWinrand.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentAcceptanceComplementWinrand.jpg "Execution times for different means."
  \image latex random/poisson/different/differentAcceptanceComplementWinrand.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionAcceptanceComplementWinrand.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionAcceptanceComplementWinrand.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double, 
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = 
	 NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorAcceptanceComplementWinrand {
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
  PoissonGeneratorAcceptanceComplementWinrand();
  
public:

  //! Construct using the normal generator.
  explicit
  PoissonGeneratorAcceptanceComplementWinrand(NormalGenerator* normalGenerator) :
    _normalGenerator(normalGenerator)
  {}

  //! Copy constructor.
  PoissonGeneratorAcceptanceComplementWinrand
  (const PoissonGeneratorAcceptanceComplementWinrand& other) :
    _normalGenerator(other._normalGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorAcceptanceComplementWinrand&
  operator=(const PoissonGeneratorAcceptanceComplementWinrand& other) {
    if (this != &other) {
      _normalGenerator = other._normalGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorAcceptanceComplementWinrand()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _normalGenerator->seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorAcceptanceComplementWinrand_ipp__
#include "PoissonGeneratorAcceptanceComplementWinrand.ipp"
#undef __numerical_random_PoissonGeneratorAcceptanceComplementWinrand_ipp__

#endif
