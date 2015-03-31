// -*- C++ -*-

/*! 
  \file numerical/random/gamma/Default.h
  \brief Define the default generator for gamma deviates.
*/

#if !defined(__numerical_random_gamma_Default_h__)
//! Include guard.
#define __numerical_random_gamma_Default_h__

#include "GammaGeneratorMarsagliaTsang.h"

#ifndef GAMMA_GENERATOR_DEFAULT
//! The default generator for gamma deviates.
#define GAMMA_GENERATOR_DEFAULT GammaGeneratorMarsagliaTsang
#endif

#endif
