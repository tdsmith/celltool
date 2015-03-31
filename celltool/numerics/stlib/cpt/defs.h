// -*- C++ -*-

/*! 
  \file cpt/defs.h
  \brief Definitions for the closest point transform.
*/

#if !defined(__cpt_defs_h__)
//! Include guard.
#define __cpt_defs_h__

// If we are debugging everything in STLib.
#if defined(DEBUG_stlib) && !defined(DEBUG_cpt)
#define DEBUG_cpt
#endif

//! Begin the cpt namespace.
#define BEGIN_NAMESPACE_CPT namespace cpt {
//! End the cpt namespace.
#define END_NAMESPACE_CPT }

//! All the standard interface functions are in the cpt namespace.
BEGIN_NAMESPACE_CPT
END_NAMESPACE_CPT

#endif
