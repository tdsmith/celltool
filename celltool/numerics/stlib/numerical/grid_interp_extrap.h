// -*- C++ -*-

/*! 
  \file grid_interp_extrap.h
  \brief Includes the grid interpolation functions.
*/

/*!
  \page grid_interp_extrap Grid Interpolation/Extrapolation Package

  This package is used in level-set applications to
  interpolate/extrapolate fields which are known for some grid points
  and not known for others.  Specifically, the fields are only known
  on one side of an interface.  The interface is defined implicitly by
  a distance function (or any function that is non-negative for known
  points and negative for known points).

  The interpolation/extrapolation function is numerical::grid_interp_extrap(). 
  In N-D it uses as \f$ 4^N \f$ point stencil around
  the interpolation point.  The interpolation/extrapolation is done one 
  dimension at a time.

  Use the grid interpolation/extrapolation package by including the file 
  grid_interp_extrap.h.
*/

#if !defined(__numerical_grid_interp_extrap_h__)
#define __numerical_grid_interp_extrap_h__

#include "grid_interp_extrap/interp_extrap.h"

#endif
