// -*- C++ -*-

/*! 
  \file numerical/random/uniform/DiscreteUniformGeneratorMt19937Gsl.h
  \brief Uniform random deviates using the Mersenne Twister algorithm from the GSL.
*/

#if !defined(__numerical_DiscreteUniformGeneratorMt19937Gsl_h__)
#define __numerical_DiscreteUniformGeneratorMt19937Gsl_h__

#include "../../defs.h"

#include <gsl/gsl_rng.h>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteUniformGeneratorMt19937Gsl)
#define DEBUG_DiscreteUniformGeneratorMt19937Gsl
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Uniform random deviates using the Mersenne Twister algorithm from the GSL.
/*!
  For documentation go to the 
  \ref numerical_random_uniform "uniform deviates page".
*/
class DiscreteUniformGeneratorMt19937Gsl {
private:

  gsl_rng* _generator;

public:

  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef unsigned result_type;


  //! Construct and seed.
  /*!
    If no seed is specified, it is initialized to 1.
  */
  explicit
  DiscreteUniformGeneratorMt19937Gsl(const unsigned seed = 0) :
    _generator(0) {
    _generator = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(_generator, seed);
  }

  //! Copy constructor.
  DiscreteUniformGeneratorMt19937Gsl(const DiscreteUniformGeneratorMt19937Gsl& other) :
    _generator(0) {
    _generator = gsl_rng_clone(other._generator);
  }

  //! Assignment operator.
  DiscreteUniformGeneratorMt19937Gsl&
  operator=(const DiscreteUniformGeneratorMt19937Gsl& other) {
    if (this != &other) {
      gsl_rng_memcpy(_generator, other._generator);
    }
    return *this;
  }

  //! Destructor.
  ~DiscreteUniformGeneratorMt19937Gsl() {
    gsl_rng_free(_generator);
  }

  //! Seed this random number generator.
  void
  seed(const unsigned value) {
    gsl_rng_set(_generator, value);
  }

  //! Return a discrete random deviate.
  result_type
  operator()() {
    return gsl_rng_get(_generator);
  }

  //! Get the random number generator data structure.
  /*!
    This is necessary for computing other distributions using this uniform 
    generator.
  */
  gsl_rng* 
  getGenerator() {
    return _generator;
  }
};


END_NAMESPACE_NUMERICAL

#endif
