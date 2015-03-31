// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorInversionTable.h
  \brief Poisson deviates with the table inversion method.
*/

#if !defined(__numerical_PoissonGeneratorInversionTable_h__)
#define __numerical_PoissonGeneratorInversionTable_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <algorithm>

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_PoissonGeneratorInversionTable)
#define DEBUG_numerical_PoissonGeneratorInversionTable
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates using the table inversion method.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This algorithm is adapted from the WinRand library.  It is found in the
  file PPRSC.C.

  \image html random/poisson/same/sameInversionTable.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameInversionTable.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/different/differentInversionTable.jpg "Execution times for different means."
  \image latex random/poisson/different/differentInversionTable.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionInversionTable.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionInversionTable.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorInversionTable {
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
  PoissonGeneratorInversionTable();

public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorInversionTable(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
  {}

  //! Copy constructor.
  PoissonGeneratorInversionTable(const PoissonGeneratorInversionTable& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorInversionTable&
  operator=(const PoissonGeneratorInversionTable& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorInversionTable()
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

#define __numerical_random_PoissonGeneratorInversionTable_ipp__
#include "PoissonGeneratorInversionTable.ipp"
#undef __numerical_random_PoissonGeneratorInversionTable_ipp__

#endif
