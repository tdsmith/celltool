// -*- C++ -*-

/*! 
  \file optimization.h
  \brief Includes the optimization classes.
*/

/*!
  \page optimization Optimization Package

  The numerical::QuasiNewton class implements the BFGS quasi-Newton method.

  The numerical::PenaltyQuasiNewton class implements the penalty method for 
  equality constrained optimization.  It uses the 
  numerical::FunctionWithQuadraticPenalty class with the quasi-Newton method.

  The numerical::Simplex class implements the downhill simplex method.

  The numerical::Penalty class implements the penalty method for equality 
  constrained optimization.  It uses the 
  numerical::FunctionWithQuadraticPenalty class with the downhill simplex 
  method.

  The numerical::CoordinateDescent class implements the coordinate descent 
  method of Hooke and Jeeves.

  Use the optimization package by including the file optimization.h.
*/

#if !defined(__numerical_optimization_h__)
#define __numerical_optimization_h__

#include "optimization/Simplex.h"
#include "optimization/CoordinateDescent.h"
#include "optimization/QuasiNewton.h"
#include "optimization/Penalty.h"
#include "optimization/PenaltyQuasiNewton.h"

#endif
