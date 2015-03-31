// -*- C++ -*-

/*! 
  \file numerical/random/uniform/DiscreteUniformGeneratorTt800.h
  \brief Uniform random deviates using Matsumoto's TT800 algorithm.
*/

// Adapted from:
/* A C-program for TT800 : July 8th 1996 Version */
/* by M. Matsumoto, email: matumoto@math.keio.ac.jp */
/* genrand() generate one pseudorandom number with double precision */
/* which is uniformly distributed on [0,1]-interval */
/* for each call.  One may choose any initial 25 seeds */
/* except all zeros. */

/* See: ACM Transactions on Modelling and Computer Simulation, */
/* Vol. 4, No. 3, 1994, pages 254-266. */

#if !defined(__numerical_DiscreteUniformGeneratorTt800_h__)
#define __numerical_DiscreteUniformGeneratorTt800_h__

#include "DiscreteUniformGeneratorMc32.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteUniformGeneratorTt800)
#define DEBUG_DiscreteUniformGeneratorTt800
#endif

BEGIN_NAMESPACE_NUMERICAL

namespace tt800 {
  //! Generate 25 words at one time.
  const int N = 25;
  //! Period parameter.
  const int M = 7;
}

//! Uniform random deviates using Matsumoto's TT800 algorithm.
/*!
  \param T The number type.  By default it is double.

  For documentation go to the 
  \ref numerical_random_uniform "uniform deviates page".

  See http://random.mat.sbg.ac.at/ftp/pub/data/tt800.c
  for a C implementation of TT800.
*/
class DiscreteUniformGeneratorTt800 {
private:

  int k;
  unsigned x[tt800::N];
  unsigned mag01[2];

public:

  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef unsigned result_type;


  //! Construct using the default seeds.
  explicit
  DiscreteUniformGeneratorTt800() {
    // Initial 25 seeds as provided in Matsumoto's algorithm.
    const unsigned defaultSeeds[tt800::N]={ 
	0x95f24dab, 0x0b685215, 0xe76ccae7, 0xaf3ec239, 0x715fad23,
	0x24a590ad, 0x69e4b5ef, 0xbf456141, 0x96bc1b7b, 0xa7bdf825,
	0xc1de75b7, 0x8858a9c9, 0x2da87693, 0xb657f9dd, 0xffdc8a9f,
	0x8121da71, 0x8b823ecb, 0x885d05f5, 0x4e20cd47, 0x5a9ad5d9,
	0x512c0c03, 0xea857ccd, 0x4cc1d30f, 0x8891a8a1, 0xa6b7aadb
    };
    seed(defaultSeeds);
  }

  //! Construct and seed.
  explicit
  DiscreteUniformGeneratorTt800(const unsigned seedValue) {
    seed(seedValue);
  }

  //! Copy constructor.
  DiscreteUniformGeneratorTt800(const DiscreteUniformGeneratorTt800& other) :
    k(other.k) {
    for (int i = 0; i != tt800::N; ++i) {
      x[i] = other.x[i];
    }
    mag01[0] = other.mag01[0];
    mag01[1] = other.mag01[1];
  }

  //! Assignment operator.
  DiscreteUniformGeneratorTt800&
  operator=(const DiscreteUniformGeneratorTt800& other) {
    if (this != &other) {
      k = other.k;
      for (int i = 0; i != tt800::N; ++i) {
	x[i] = other.x[i];
      }
      mag01[0] = other.mag01[0];
      mag01[1] = other.mag01[1];
    }
    return *this;
  }

  //! Destructor.
  ~DiscreteUniformGeneratorTt800() 
  {}

  //! Seed this random number generator.
  /*!
    Generate 25 random seeds with DiscreteUniformGeneratorMc32 .
  */
  void
  seed(const unsigned s);

  //! Set the 25 seeds.
  void
  seed(const unsigned* seeds);

  //! Return a discrete uniform random deviate.
  result_type
  operator()();
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteUniformGeneratorTt800_ipp__
#include "DiscreteUniformGeneratorTt800.ipp"
#undef __numerical_random_DiscreteUniformGeneratorTt800_ipp__

#endif
