// -*- C++ -*-

/*! 
  \file numerical/random/uniform/DiscreteUniformGeneratorNr2.h
  \brief Long period (> 2 * 10^18) random number generator.
*/

#if !defined(__numerical_DiscreteUniformGeneratorNr2_h__)
#define __numerical_DiscreteUniformGeneratorNr2_h__

#include "../../defs.h"

#include "../../../ads/array/FixedArray.h"

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteUniformGeneratorNr2)
#define DEBUG_DiscreteUniformGeneratorNr2
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Long period (> 2 * 10^18) random number generator.
/*!
  For documentation go to the 
  \ref numerical_random_uniform "uniform deviates page".

  This is adapted from the \c ran2() function in 
  \ref numerical_random_press2002 "Numerical Recipes".

  Long period (> 2 * 10^18) random number generator of L'Ecuyer with
  Bays-Durham shuffle and added safeguards.  
  Returns a discrete uniform random deviate between 0 and 2^31 - 1.  
  Initialize the sequence in the constructor or with the seed() member 
  function.
*/
class DiscreteUniformGeneratorNr2 {
private:

  int _idum;
  int _idum2;
  int _iy;
  ads::FixedArray<32,int> _iv;

public:

  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;


  //! Default constructor.  Seed the random number generator with 1.
  DiscreteUniformGeneratorNr2() :
    _idum(),
    _idum2(),
    _iy(),
    _iv() {
    seed(1);
  }

  //! Construct and seed.
  explicit
  DiscreteUniformGeneratorNr2(const int seedValue) :
    _idum(),
    _idum2(),
    _iy(),
    _iv() {
    seed(seedValue);
  }

  //! Copy constructor.
  DiscreteUniformGeneratorNr2(const DiscreteUniformGeneratorNr2& other) :
    _idum(other._idum),
    _idum2(other._idum2),
    _iy(other._iy),
    _iv(other._iv)
  {}

  //! Assignment operator.
  DiscreteUniformGeneratorNr2&
  operator=(const DiscreteUniformGeneratorNr2& other) {
    if (this != &other) {
      _idum = other._idum;
      _idum2 = other._idum2;
      _iy = other._iy;
      _iv = other._iv;
    }
    return *this;
  }

  //! Trivial destructor.
  ~DiscreteUniformGeneratorNr2()
  {}

  //! Seed this random number generator.
  void
  seed(int value);

  //! Return a discrete uniform random deviate.
  result_type
  operator()();
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteUniformGeneratorNr2_ipp__
#include "DiscreteUniformGeneratorNr2.ipp"
#undef __numerical_random_DiscreteUniformGeneratorNr2_ipp__

#endif
