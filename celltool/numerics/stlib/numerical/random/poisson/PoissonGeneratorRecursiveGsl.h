// -*- C++ -*-

/*! 
  \file numerical/random/poisson/PoissonGeneratorRecursiveGsl.h
  \brief Generator for Poisson deviates using the GNU Scientific Library.
*/

#if !defined(__numerical_PoissonGeneratorRecursiveGsl_h__)
#define __numerical_PoissonGeneratorRecursiveGsl_h__

#include "../uniform/DiscreteUniformGeneratorMt19937Gsl.h"

#include <gsl/gsl_randist.h>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonGeneratorRecursiveGsl)
#define DEBUG_PoissonGeneratorRecursiveGsl
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T = double, 
	 class Uniform = DiscreteUniformGeneratorMt19937Gsl>
class PoissonGeneratorRecursiveGsl;

//! Generator for Poisson deviates using the GNU Scientific Library.
/*!
  \param T The number type.  By default it is double.

  This is a template specialization for the case that the uniform deviate
  generator is UniformGeneratorMt19937Gsl .
  This generator can be initialized in the constructor or with seed().

  This functor is wraps functionality from the 
  <a href="http://www.gnu.org/software/gsl/">GNU Scientific Library</a>. 
  They use an algorithm described in 
  \ref numerical_random_ahrens1974 "Computer methods for sampling from gamma, beta, Poisson and binomial distributions."
  (Also see 
  \ref numerical_random_devroye1986 "Non-Uniform Random Variate Generation"
  and
  \ref numerical_random_knuth1998 "The Art of Computer Programming - Seminumerical Algorithms".)
  It uses recursive 
  properties of the Poisson distribution.  (Hence the name of this class.)
  The expected complexity of the algorithm is \f$\mathcal{O}(\log(\mu))\f$
  for mean \f$\mu\f$.  It has decent execution times for small means
  (less than 10), where is uses exponential inter-arrival times
  (see PoissonGeneratorExponentialInterArrival).  However, it does 
  not have competitive performance for larger means.
  

  \image html random/poisson/same/sameRecursiveGslSmallArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameRecursiveGslSmallArgument.pdf "Execution times for the same means." width=0.5\textwidth

  \image html random/poisson/same/sameRecursiveGslLargeArgument.jpg "Execution times for the same means."
  \image latex random/poisson/same/sameRecursiveGslLargeArgument.pdf "Execution times for the same means." width=0.5\textwidth


  \image html random/poisson/different/differentRecursiveGslSmallArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentRecursiveGslSmallArgument.pdf "Execution times for different means." width=0.5\textwidth

  \image html random/poisson/different/differentRecursiveGslLargeArgument.jpg "Execution times for different means."
  \image latex random/poisson/different/differentRecursiveGslLargeArgument.pdf "Execution times for different means." width=0.5\textwidth


  \image html random/poisson/distribution/distributionRecursiveGslSmallArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionRecursiveGslSmallArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionRecursiveGslLargeArgument.jpg "Execution times for a distribution of means."
  \image latex random/poisson/distribution/distributionRecursiveGslLargeArgument.pdf "Execution times for a distribution of means." width=0.5\textwidth
*/
template<typename T>
class PoissonGeneratorRecursiveGsl<T, DiscreteUniformGeneratorMt19937Gsl> {
public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef Number argument_type;
  //! The result type.
  typedef int result_type;
  //! The discrete uniform generator.
  typedef DiscreteUniformGeneratorMt19937Gsl DiscreteUniformGenerator;

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
  PoissonGeneratorRecursiveGsl();
 
public:

  //! Construct using the uniform generator.
  explicit
  PoissonGeneratorRecursiveGsl(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator)
  {}

  //! Copy constructor.
  PoissonGeneratorRecursiveGsl(const PoissonGeneratorRecursiveGsl& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  PoissonGeneratorRecursiveGsl&
  operator=(const PoissonGeneratorRecursiveGsl& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~PoissonGeneratorRecursiveGsl()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _discreteUniformGenerator->seed(seedValue);
  }

  //! Return a Poisson deviate with the specifed mean.
  result_type
  operator()(const argument_type mean) {
    return gsl_ran_poisson(_discreteUniformGenerator->getGenerator(), mean);
  }
};


END_NAMESPACE_NUMERICAL

#endif
