// -*- C++ -*-

/*! 
  \file interpolation.h
  \brief Includes the interpolation functions.
*/

/*!
  \page interpolation Interpolation Package

  - \ref interpolation_simplex
  - \ref interpolation_hermite
  - The numerical::LinInterpGrid class is a functor for performing
    linear interpolation on a regular grid.

  Use the interpolation package by including the file interpolation.h.
*/

#if !defined(__numerical_interpolation_h__)
#define __numerical_interpolation_h__

#include "interpolation/hermite.h"
#include "interpolation/LinInterpGrid.h"
#include "interpolation/simplex.h"

#endif
