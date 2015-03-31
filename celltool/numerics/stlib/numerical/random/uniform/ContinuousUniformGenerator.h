// -*- C++ -*-

/*! 
  \file numerical/random/uniform/ContinuousUniformGenerator.h
  \brief Continuous uniform deviate generators.
*/

#if !defined(__numerical_random_uniform_ContinuousUniformGenerator_h__)
#define __numerical_random_uniform_ContinuousUniformGenerator_h__

#include "Default.h"

#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_random_uniform_ContinuousUniformGenerator)
#define DEBUG_numerical_random_uniform_ContinuousUniformGenerator
#endif

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
//! \defgroup numerical_random_uniform_continuous Transform discrete, uniform deviates to continuous, uniform deviates.
//@{

//! Transform an integer deviate to a continuous deviate in the range (0..1).
/*!
  The range is open, it does not include the end points.  
  Use this function if you will take the logarithm of the deviate.
*/
template<typename Number, typename Integer>
inline
Number
transformDiscreteDeviateToContinuousDeviateOpen(const Integer deviate) {
  // This should be especially efficient for architectures with a 
  // fused multiply-add.
  return Number(deviate) * 
    (1.0 / (Number(std::numeric_limits<Integer>::max()) + 1.0)) + 
    (0.5 / (Number(std::numeric_limits<Integer>::max()) + 1.0));
  // The method below is less efficient.
  //return Number(devate | 1U) * 
  // (1.0 / (Number(std::numeric_limits<Integer>::max()) + 1.0));
}


//! Transform an integer deviate to a continuous deviate in the range [0..1].
/*!
  This function is more efficient than 
  transformDiscreteDeviateToContinuousDeviateOpen .
*/
template<typename Number, typename Integer>
inline
Number
transformDiscreteDeviateToContinuousDeviateClosed(const Integer deviate) {
  return Number(deviate) * (1.0 / Number(std::numeric_limits<Integer>::max()));
}

//@}


//! Return a continuous uniform deviate in the open range (0..1).
template<class _DiscreteUniformGenerator = DISCRETE_UNIFORM_GENERATOR_DEFAULT, 
	 typename Number = double>
class ContinuousUniformGeneratorOpen {
  //
  // Public types.
  //

public:

  //! The discrete uniform deviate generator.
  typedef _DiscreteUniformGenerator DiscreteUniformGenerator;
  //! The integer type for discrete deviates.
  typedef typename DiscreteUniformGenerator::result_type Integer;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef Number result_type;

  //
  // Member data.
  //

private:

  //! The discrete uniform deviate generator.
  DiscreteUniformGenerator* _discreteUniformGenerator;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  /*!
    We can't make a continuous generator without a discrete generator.
  */
  ContinuousUniformGeneratorOpen();

  //
  // Member functions.
  //

public:

  //! Construct from the discrete uniform deviate generator.
  ContinuousUniformGeneratorOpen
  (DiscreteUniformGenerator* discreteUniformGenerator) :
    _discreteUniformGenerator(discreteUniformGenerator)
  {}

  //! Copy constructor.
  ContinuousUniformGeneratorOpen(const ContinuousUniformGeneratorOpen& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  const ContinuousUniformGeneratorOpen&
  operator=(const ContinuousUniformGeneratorOpen& other) {
    if (&other != this) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Return a continuous uniform deviate in the open range (0..1).
  result_type
  operator()() {
    return transformDiscreteDeviateToContinuousDeviateOpen<result_type>
      ((*_discreteUniformGenerator)());
  }

  //! Seed the discrete uniform deviate generator.
  void
  seed(const Integer& s) {
    _discreteUniformGenerator->seed(s);
  }

  //! Get the discrete uniform generator.
  DiscreteUniformGenerator*
  getDiscreteUniformGenerator() {
    return _discreteUniformGenerator;
  }
};





//! Return a continuous uniform deviate in the closed range [0..1].
template<class _DiscreteUniformGenerator = DISCRETE_UNIFORM_GENERATOR_DEFAULT, 
	 typename Number = double>
class ContinuousUniformGeneratorClosed {
  //
  // Public types.
  //

public:

  //! The discrete uniform deviate generator.
  typedef _DiscreteUniformGenerator DiscreteUniformGenerator;
  //! The integer type for discrete deviates.
  typedef typename DiscreteUniformGenerator::result_type Integer;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef Number result_type;

  //
  // Member data.
  //

private:

  //! The discrete uniform deviate generator.
  DiscreteUniformGenerator* _discreteUniformGenerator;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  /*!
    We can't make a continuous generator without a discrete generator.
  */
  ContinuousUniformGeneratorClosed();

  //
  // Member functions.
  //

public:

  //! Construct from the discrete uniform deviate generator.
  ContinuousUniformGeneratorClosed
  (DiscreteUniformGenerator* discreteUniformGenerator) :
    _discreteUniformGenerator(discreteUniformGenerator)
  {}

  //! Copy constructor.
  ContinuousUniformGeneratorClosed(const ContinuousUniformGeneratorClosed& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  const ContinuousUniformGeneratorClosed&
  operator=(const ContinuousUniformGeneratorClosed& other) {
    if (&other != this) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Return a continuous uniform deviate in the closed range [0..1].
  result_type
  operator()() {
    return transformDiscreteDeviateToContinuousDeviateClosed<result_type>
      ((*_discreteUniformGenerator)());
  }

  //! Seed the discrete uniform deviate generator.
  void
  seed(const Integer& s) {
    _discreteUniformGenerator->seed(s);
  }

  //! Get the discrete uniform generator.
  DiscreteUniformGenerator*
  getDiscreteUniformGenerator() {
    return _discreteUniformGenerator;
  }
};


END_NAMESPACE_NUMERICAL

#endif
