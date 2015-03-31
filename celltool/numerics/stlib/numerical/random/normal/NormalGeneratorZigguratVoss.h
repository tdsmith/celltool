// -*- C++ -*-

/*! 
  \file numerical/random/normal/NormalGeneratorZigguratVoss.h
  \brief Normal random deviate with zero mean and unit variance.
*/

// Copyright from Jochen Voss.
/* gauss.c - gaussian random numbers, using the Ziggurat method
 *
 * Copyright (C) 2005  Jochen Voss.
 *
 * For details see the following article.
 *
 *     George Marsaglia, Wai Wan Tsang
 *     The Ziggurat Method for Generating Random Variables
 *     Journal of Statistical Software, vol. 5 (2000), no. 8
 *     http://www.jstatsoft.org/v05/i08/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#if !defined(__numerical_NormalGeneratorZigguratVoss_h__)
#define __numerical_NormalGeneratorZigguratVoss_h__

#include "../uniform/ContinuousUniformGenerator.h"

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_NormalGeneratorZigguratVoss)
#define DEBUG_NormalGeneratorZigguratVoss
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Normal random deviate with zero mean and unit variance.
/*!
  \param T The number type.  By default it is double.

  This is a template specialization for the case that the uniform deviate
  generator is UniformGeneratorMt19937Gsl .
  This generator can be initialized in the constructor or with seed().

  This functor wraps the function call to the Marsaglia-Tsang ziggurat 
  algorithm for computing Gaussian deviates in the
  <a href="http://www.gnu.org/software/gsl/">GNU Scientific Library</a>.
  It returns a floating point value that is a random deviate drawn from a 
  normal (Gaussian) distribution with zero mean and unit variance.
*/
template<typename T = double,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class NormalGeneratorZigguratVoss {
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The number type.
  typedef T Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef Number result_type;

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
  NormalGeneratorZigguratVoss();
  
public:

  //! Construct using the uniform generator.
  explicit
  NormalGeneratorZigguratVoss(DiscreteUniformGenerator* generator) :
    _discreteUniformGenerator(generator) 
  {}

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  NormalGeneratorZigguratVoss(const NormalGeneratorZigguratVoss& other) :
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  NormalGeneratorZigguratVoss&
  operator=(const NormalGeneratorZigguratVoss& other) {
    if (this != &other) {
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~NormalGeneratorZigguratVoss()
  {}

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _discreteUniformGenerator->seed(seedValue);
  }

  //! Return a standard normal deviate.
  result_type
  operator()();

  //! Return a normal deviate with specified mean and variance.
  result_type
  operator()(const Number mean, const Number variance) {
    return std::sqrt(variance) * operator()() + mean;
  }

  //! Get the discrete uniform generator.
  DiscreteUniformGenerator*
  getDiscreteUniformGenerator() {
    return _discreteUniformGenerator;
  }
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_NormalGeneratorZigguratVoss_ipp__
#include "NormalGeneratorZigguratVoss.ipp"
#undef __numerical_random_NormalGeneratorZigguratVoss_ipp__

#endif
