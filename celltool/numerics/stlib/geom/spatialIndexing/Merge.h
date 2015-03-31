// -*- C++ -*-

/*! 
  \file geom/spatialIndexing/Merge.h
  \brief Merging functors.
*/

#if !defined(__geom_spatialIndexing_Merge_h__)
#define __geom_spatialIndexing_Merge_h__

#include "../defs.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_Merge)
#define DEBUG_geom_spatialIndexing_Merge
#endif

BEGIN_NAMESPACE_GEOM

//! Merging functor that does nothing.
struct MergeNull {
  //! Do nothing.
  void
  operator()() const {
  }
};

//! Merging functor that copies the value of the first child.
struct MergeCopyFirst {
  //! Copy the value of the first child.
  template<int _NumberOfOrthants, typename _Element>
  void
  operator()
  (const ads::FixedArray<_NumberOfOrthants, const _Element*>& children,
   _Element* parent) const {
    *parent = *children[0];
  }
};

//! Merging functor that averages the values of the children.
struct MergeAverage {
  //! Average the children.
  template<int _NumberOfOrthants, typename _Element>
  void
  operator()
  (const ads::FixedArray<_NumberOfOrthants, const _Element*>& children,
   _Element* parent) const {
    *parent = *children[0];
    for (int i = 0; i != _NumberOfOrthants; ++i) {
      *parent == *children[i];
    }
    *parent /= _NumberOfOrthants;
  }
};

END_NAMESPACE_GEOM

#endif
