// -*- C++ -*-

/*! 
  \file numerical/random/uniform/Default.h
  \brief Define the default generator for discrete uniform deviates.
*/

#if !defined(__numerical_random_uniform_Default_h__)
//! Include guard.
#define __numerical_random_uniform_Default_h__

#include "DiscreteUniformGeneratorMt19937.h"

#ifndef DISCRETE_UNIFORM_GENERATOR_DEFAULT
//! The default generator for discrete uniform deviates.
#define DISCRETE_UNIFORM_GENERATOR_DEFAULT DiscreteUniformGeneratorMt19937
#endif

#endif
