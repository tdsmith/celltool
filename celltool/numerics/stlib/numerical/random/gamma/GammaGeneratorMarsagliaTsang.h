// -*- C++ -*-

/*! 
  \file numerical/random/gamma/GammaGeneratorMarsagliaTsang.h
  \brief Gamma deviates using the method of Marsaglia and Tsang.
*/

#if !defined(__numerical_GammaGeneratorMarsagliaTsang_h__)
#define __numerical_GammaGeneratorMarsagliaTsang_h__

#include "../normal/Default.h"

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_GammaGeneratorMarsagliaTsang)
#define DEBUG_GammaGeneratorMarsagliaTsang
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Gamma deviates using the method of Marsaglia and Tsang.
/*!
  \param T The number type.  By default it is double.
  \param Uniform The uniform random number generator.
  This generator can be initialized with seed().
  \param Normal The generator for normal deviates.
  
  This functor computes Gamma deviates using 
  \ref numerical_random_gammaMarsaglia2000 "Marsaglia and Tsang's" method.
*/
template<typename T = double, 
	 class Uniform = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 template<typename, class> class Normal = NORMAL_GENERATOR_DEFAULT>
class GammaGeneratorMarsagliaTsang {
public:

  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef Number argument_type;
  //! The result type.
  typedef Number result_type;
  //! The discrete uniform generator.
  typedef Uniform DiscreteUniformGenerator;
  //! The normal generator.
  typedef Normal<Number, DiscreteUniformGenerator> NormalGenerator;

  //
  // Member data.
  //

private:

  //! The normal generator.
  NormalGenerator* _normalGenerator;

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  GammaGeneratorMarsagliaTsang();
  
public:

  //! Construct using the normal generator.
  explicit
  GammaGeneratorMarsagliaTsang(NormalGenerator* normalGenerator) :
    _normalGenerator(normalGenerator)
  {}

  //! Copy constructor.
  GammaGeneratorMarsagliaTsang(const GammaGeneratorMarsagliaTsang& other) :
    _normalGenerator(other._normalGenerator)
  {}

  //! Assignment operator.
  GammaGeneratorMarsagliaTsang&
  operator=(const GammaGeneratorMarsagliaTsang& other) {
    if (this != &other) {
      _normalGenerator = other._normalGenerator;
    }
    return *this;
  }

  //! Destructor.
  ~GammaGeneratorMarsagliaTsang()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _normalGenerator->seed(seedValue);
  }

  //! Return a Gamma deviate with the specifed shape and unit rate.
  result_type
  operator()(argument_type shape);

  //! Return a Gamma deviate with the specifed shape and rate.
  result_type
  operator()(const Number shape, const Number rate) {
    return rate * operator()(shape);
  }
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_GammaGeneratorMarsagliaTsang_ipp__
#include "GammaGeneratorMarsagliaTsang.ipp"
#undef __numerical_random_GammaGeneratorMarsagliaTsang_ipp__

#endif
