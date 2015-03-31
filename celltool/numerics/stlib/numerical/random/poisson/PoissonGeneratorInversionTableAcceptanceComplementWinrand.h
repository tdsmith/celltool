// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionTableAcceptanceComplementWinrand.h
  \brief Poisson deviates using the WinRand implementation of table inversion/acceptance complement.
*/

#if !defined(__numerical_PoissonGeneratorInversionTableAcceptanceComplementWinrand_h__)
#define __numerical_PoissonGeneratorInversionTableAcceptanceComplementWinrand_h__

#include "../normal/Default.h"

#include <algorithm>

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorInversionTableAcceptanceComplementWinrand)
#define DEBUG_PoissonGeneratorInversionTableAcceptanceComplementWinrand
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Poisson deviates using the WinRand implementation of table inversion/acceptance complement.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().
  
  This functor computes Poisson deviates using the 
  <a href="http://www.stat.tugraz.at/stadl/random.html">WinRand</a>
  implementation of table inversion/acceptance complement.

  Modifications:
  - Changed <code>double</code> to <code>Number</code>
  - Use my own uniform random deviate generator and normal deviate generator.


  \image html random/poisson/same/sameInversionTableAcceptanceComplementWinrandSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionTableAcceptanceComplementWinrandSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameInversionTableAcceptanceComplementWinrandLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionTableAcceptanceComplementWinrandLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentInversionTableAcceptanceComplementWinrandSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionTableAcceptanceComplementWinrandSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionTableAcceptanceComplementWinrandLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionTableAcceptanceComplementWinrandLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionInversionTableAcceptanceComplementWinrandSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionTableAcceptanceComplementWinrandSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionTableAcceptanceComplementWinrandLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionTableAcceptanceComplementWinrandLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double, 
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorInversionTableAcceptanceComplementWinrand {
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
  PoissonGeneratorInversionTableAcceptanceComplementWinrand();
  
public:

  //! Construct using the normal generator.
  explicit
  PoissonGeneratorInversionTableAcceptanceComplementWinrand
  (NormalGenerator* normalGenerator) :
    _normalGenerator(normalGenerator)
  {}

  //! Copy constructor.
  PoissonGeneratorInversionTableAcceptanceComplementWinrand
  (const PoissonGeneratorInversionTableAcceptanceComplementWinrand& other) :
    _normalGenerator(other._normalGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorInversionTableAcceptanceComplementWinrand&
  operator=
  (const PoissonGeneratorInversionTableAcceptanceComplementWinrand& other) {
    if (this != &other) {
      _normalGenerator = other._normalGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInversionTableAcceptanceComplementWinrand()
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

#define __numerical_random_PoissonGeneratorInversionTableAcceptanceComplementWinrand_ipp__
#include "PoissonGeneratorInversionTableAcceptanceComplementWinrand.ipp"
#undef __numerical_random_PoissonGeneratorInversionTableAcceptanceComplementWinrand_ipp__

#endif
