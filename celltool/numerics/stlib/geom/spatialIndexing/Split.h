// -*- C++ -*-

/*! 
  \file geom/spatialIndexing/Split.h
  \brief Splitting functors.
*/

#if !defined(__geom_spatialIndexing_Split_h__)
#define __geom_spatialIndexing_Split_h__

#include "../defs.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_Split)
#define DEBUG_geom_spatialIndexing_Split
#endif

BEGIN_NAMESPACE_GEOM

//! Splitting functor that does nothing.
struct SplitNull {
  //! Do nothing.
  void
  operator()() const {
  }
};

//! Splitting functor that copies the value of the parent.
struct SplitCopy {
  //! Copy the value of the parent.
  template<typename _Element>
  const _Element&
  operator()(const _Element& parent) const {
    return parent;
  }
};

END_NAMESPACE_GEOM

#endif
