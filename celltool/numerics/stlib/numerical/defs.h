// -*- C++ -*-

/*! 
  \file numerical/defs.h
  \brief Definitions for the numerical algorithms package.
*/

#if !defined(__numerical_defs_h__)
//! Include guard.
#define __numerical_defs_h__

// If we are debugging everything in STLib.
#if defined(DEBUG_stlib) && !defined(DEBUG_numerical)
#define DEBUG_numerical
#endif

//! Start the numerical namespace.
#define BEGIN_NAMESPACE_NUMERICAL namespace numerical {
//! End the numerical namespace.
#define END_NAMESPACE_NUMERICAL }

//! All classes and functions in the Numerical Algorithms package are defined in the numerical namespace.
BEGIN_NAMESPACE_NUMERICAL
END_NAMESPACE_NUMERICAL

#endif
