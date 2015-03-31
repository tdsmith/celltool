// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorDirectRejectionNr.h
  \brief Uniform random deviates.
*/

#if !defined(__numerical_PoissonGeneratorDirectRejectionNr_h__)
#define __numerical_PoissonGeneratorDirectRejectionNr_h__

#include "PoissonGeneratorDirectNr.h"
#include "PoissonGeneratorRejectionNr.h"

#include "../../specialFunctions/Gamma.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorDirectRejectionNr)
#define DEBUG_PoissonGeneratorDirectRejectionNr
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Generator for Poisson deviates using the direct and rejection methods.
/*!
  \param T The number type.  By default it is double.
  \param Generator The uniform random number generator.
  This generator can be initialized in the constructor or with seed().

  This functor is adapted from the poidev() function in "Numerical Recipes".
  It returns an integer value that is a random deviate drawn from a Poisson
  distribution with specified mean.  For small means, the algorithm uses
  the direct method; for large means, the rejection method.  

  The figures below shows the execution times in nanoseconds as a function
  of the mean for the Poisson generator based on each of UniformRandom{0,1,2}.
  They show execution times for computing Poisson deviates with
  the default implementation and with the implementation optimized
  for small means.  
  The test code is in stlib/performance/numerical/random.  It was compiled 
  with GNU g++ 4.0 using the flags: -O3 -funroll-loops -fstrict-aliasing.
  I ran the tests on a Mac Mini with a 1.66 GHz Intel Core Duo processor and
  512 MB DDR2 SDRAM.
  <!--CONTINUE Boost the above detail to the top level documentation.-->

  \image html random/poisson/same/sameDirectRejectionNrSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameDirectRejectionNrSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameDirectRejectionNrLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameDirectRejectionNrLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentDirectRejectionNrSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentDirectRejectionNrSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentDirectRejectionNrLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentDirectRejectionNrLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionDirectRejectionNrSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionDirectRejectionNrSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionDirectRejectionNrLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionDirectRejectionNrLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T = double,
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class PoissonGeneratorDirectRejectionNr {
public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef Number argument_type;
  //! The result type.
  typedef int result_type;
  //! The discrete uniform generator.
  typedef Uniform DiscreteUniformGenerator;

private:

  //
  // Member data.
  //
  
  PoissonGeneratorDirectNr<Number, DiscreteUniformGenerator> _directNr;
  PoissonGeneratorRejectionNr<Number, DiscreteUniformGenerator> _rejectionNr;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  PoissonGeneratorDirectRejectionNr();
  
public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorDirectRejectionNr(DiscreteUniformGenerator* generator) :
    _directNr(generator),
    _rejectionNr(generator)
  {}

  //! Copy constructor.
  PoissonGeneratorDirectRejectionNr
  (const PoissonGeneratorDirectRejectionNr& other) :
    _directNr(other._directNr),
    _rejectionNr(other._rejectionNr)
  {}

  //! Assignment operator.
  PoissonGeneratorDirectRejectionNr&
  operator=(const PoissonGeneratorDirectRejectionNr& other) {
    if (this != &other) {
      _directNr = other._directNr;
      _rejectionNr = other._rejectionNr;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorDirectRejectionNr()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _directNr.seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(argument_type mean);
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonGeneratorDirectRejectionNr_ipp__
#include "PoissonGeneratorDirectRejectionNr.ipp"
#undef __numerical_random_PoissonGeneratorDirectRejectionNr_ipp__

#endif
