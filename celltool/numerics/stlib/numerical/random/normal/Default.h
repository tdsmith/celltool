// -*- C++ -*-

/*! 
  \file numerical/random/normal/Default.h
  \brief Define the default generator for normal deviates.
*/

#if !defined(__numerical_random_normal_Default_h__)
//! Include guard.
#define __numerical_random_normal_Default_h__

#include "NormalGeneratorZigguratVoss.h"

#ifndef NORMAL_GENERATOR_DEFAULT
//! The default generator for normal deviates.
#define NORMAL_GENERATOR_DEFAULT NormalGeneratorZigguratVoss
#endif

#endif
