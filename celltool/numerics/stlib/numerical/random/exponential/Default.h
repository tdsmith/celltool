// -*- C++ -*-

/*! 
  \file numerical/random/exponential/Default.h
  \brief Define the default generator for exponential deviates.
*/

#if !defined(__numerical_random_exponential_Default_h__)
//! Include guard.
#define __numerical_random_exponential_Default_h__

#include "ExponentialGeneratorZiggurat.h"

#ifndef EXPONENTIAL_GENERATOR_DEFAULT
//! The default generator for exponential deviates.
#define EXPONENTIAL_GENERATOR_DEFAULT ExponentialGeneratorZiggurat
#endif

#endif
