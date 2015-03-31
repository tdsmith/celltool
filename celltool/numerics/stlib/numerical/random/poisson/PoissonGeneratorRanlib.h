// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorRanlib.h
  \brief Uniform random deviates.
*/

#if !defined(__numerical_PoissonGeneratorRanlib_h__)
#define __numerical_PoissonGeneratorRanlib_h__

#include "../normal/Default.h"

#include <algorithm>

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorRanlib)
#define DEBUG_PoissonGeneratorRanlib
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor is adapted from the 
  <a href="http://www.netlib.org/random/">Ranlib</a> library. 
  Changes:
  - <code>float</code> -> <code>Number</code>
  - <code>long</code> -> <code>int</code>
  - Adde <code>std::</code> to the standard library functions.


  \image html random/poisson/same/sameRanlibSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameRanlibSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameRanlibLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameRanlibLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentRanlibSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentRanlibSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentRanlibLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentRanlibLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionRanlibSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionRanlibSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionRanlibLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionRanlibLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double, 
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class PoissonGeneratorRanlib {
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
  PoissonGeneratorRanlib();

public:

  //! Construct using the normal generator.
  explicit
  PoissonGeneratorRanlib(NormalGenerator* normalGenerator) :
    _normalGenerator(normalGenerator)
  {}

  //! Copy constructor.
  PoissonGeneratorRanlib(const PoissonGeneratorRanlib& other) :
    _normalGenerator(other._normalGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorRanlib&
  operator=(const PoissonGeneratorRanlib& other) {
    if (this != &other) {
      _normalGenerator = other._normalGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorRanlib()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _normalGenerator->seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);

private:

  Number
  sexpo();

  //! Transfers sign of argument sign to argument num.
  Number
  fsign(const Number num, const Number sign) const {
    if ((sign>0.0 && num<0.0) || (sign<0.0 && num>0.0)) {
      return -num;
    }
    else {
      return num;
    }
  }

};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorRanlib_ipp__
#include "PoissonGeneratorRanlib.ipp"
#undef __numerical_random_PoissonGeneratorRanlib_ipp__

#endif
