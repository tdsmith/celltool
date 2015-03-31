// -*- C++ -*-

/*! 
  \file numerical/random/uniform/DiscreteUniformGeneratorNr1.h
  \brief Minimal generator.
*/

#if !defined(__numerical_DiscreteUniformGeneratorNr1_h__)
#define __numerical_DiscreteUniformGeneratorNr1_h__

#include "../../defs.h"

#include "../../../ads/array/FixedArray.h"

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DiscreteUniformGeneratorNr1)
#define DEBUG_numerical_DiscreteUniformGeneratorNr1
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Minimal generator.
/*!
  For documentation go to the 
  \ref numerical_random_uniform "uniform deviates page".

  This is adapted from the \c ran1() function in 
  \ref numerical_random_press2002 "Numerical Recipes".

  Minimal random number generator of Park and Miller with Bays-Durham
  shuffle and added safeguards.  
  Returns a discrete uniform random deviate between 0 and 2^31 - 1.  
  Initialize the sequence in the constructor or with the seed() member 
  function.

  This algorithm generates good random numbers when the number of calls is
  less than 10^8.
*/
class DiscreteUniformGeneratorNr1 {
private:

  int _idum;
  int _iy;
  ads::FixedArray<32,int> _iv;

public:

  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;


  //! Default constructor.  Seed the random number generator with 1.
  DiscreteUniformGeneratorNr1() :
    _idum(),
    _iy(),
    _iv() {
    seed(1);
  }

  //! Construct and seed.
  explicit
  DiscreteUniformGeneratorNr1(const int seedValue) :
    _idum(),
    _iy(),
    _iv() {
    seed(seedValue);
  }

  //! Copy constructor.
  DiscreteUniformGeneratorNr1(const DiscreteUniformGeneratorNr1& other) :
    _idum(other._idum),
    _iy(other._iy),
    _iv(other._iv)
  {}

  //! Assignment operator.
  DiscreteUniformGeneratorNr1&
  operator=(const DiscreteUniformGeneratorNr1& other) {
    if (this != &other) {
      _idum = other._idum;
      _iy = other._iy;
      _iv = other._iv;
    }
    return *this;
  }

  //! Trivial destructor.
  ~DiscreteUniformGeneratorNr1()
  {}

  //! Seed this random number generator.
  void
  seed(int value);

  //! Return a discrete uniform random deviate.
  result_type
  operator()();
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteUniformGeneratorNr1_ipp__
#include "DiscreteUniformGeneratorNr1.ipp"
#undef __numerical_random_DiscreteUniformGeneratorNr1_ipp__

#endif
