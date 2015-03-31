// -*- C++ -*-

/*! 
  \file numerical/random/uniform/DiscreteUniformGeneratorNr0.h
  \brief Very simple generator.
*/

#if !defined(__numerical_DiscreteUniformGeneratorNr0_h__)
#define __numerical_DiscreteUniformGeneratorNr0_h__

#include "../../defs.h"

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DiscreteUniformGeneratorNr0)
#define DEBUG_numerical_DiscreteUniformGeneratorNr0
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Very simple generator.
/*!
  For documentation go to the 
  \ref numerical_random_uniform "uniform deviates page".

  This is adapted from the \c ran0() function in 
  \ref numerical_random_press2002 "Numerical Recipes".
  
  Minimal random number generator of Park and Miller.  
  Returns a discrete uniform random deviate between 0 and 2^31 - 1.  
  Initialize the sequence in the 
  constructor or with the seed() member function.
*/
class DiscreteUniformGeneratorNr0 {
private:

  int _idum;

public:

  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;


  //! Construct and seed.
  /*!
    If no seed is specified, it is initialized to 1.  If the seed is 0, it 
    will be changed to 1.
  */
  explicit
  DiscreteUniformGeneratorNr0(const int seed = 1) :
    _idum(seed) {
    if (_idum == 0) {
      _idum = 1;
    }
  }

  //! Copy constructor.
  DiscreteUniformGeneratorNr0(const DiscreteUniformGeneratorNr0& other) :
    _idum(other._idum)
  {}

  //! Assignment operator.
  DiscreteUniformGeneratorNr0&
  operator=(const DiscreteUniformGeneratorNr0& other) {
    if (this != &other) {
      _idum = other._idum;
    }
    return *this;
  }

  //! Trivial destructor.
  ~DiscreteUniformGeneratorNr0()
  {}

  //! Seed this random number generator.
  /*!
    If the seed is 0, it will be changed to 1.
  */
  void
  seed(const int value) {
    _idum = value;
    if (_idum == 0) {
      _idum = 1;
    }
  }

  //! Return a discrete uniform random deviate.
  result_type
  operator()();
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteUniformGeneratorNr0_ipp__
#include "DiscreteUniformGeneratorNr0.ipp"
#undef __numerical_random_DiscreteUniformGeneratorNr0_ipp__

#endif
