// -*- C++ -*-

/*! 
  \file geom/spatialIndexing/OrthtreeElementNull.h
  \brief CONTINUE
*/

#if !defined(__geom_spatialIndexing_OrthtreeElementNull_h__)
#define __geom_spatialIndexing_OrthtreeElementNull_h__

#include "../defs.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_OrthtreeElementNull)
#define DEBUG_geom_spatialIndexing_OrthtreeElementNull
#endif

BEGIN_NAMESPACE_GEOM

//! A null element.  It holds no data.
template<int _Dimension>
class
OrthtreeElementNull {
};


//! Return true.
/*!
  \relates OrthtreeElementNull
*/
template<int _Dimension>
inline
bool
operator==(const OrthtreeElementNull<_Dimension>& x, 
	   const OrthtreeElementNull<_Dimension>& y) {
  return true;
}

//! Write nothing.
/*!
  \relates OrthtreeElementNull
*/
template<int _Dimension>
inline
std::ostream&
operator<<(std::ostream& out, const OrthtreeElementNull<_Dimension>& x) {
  return out;
}

//! Read nothing.
/*!
  \relates OrthtreeElementNull
*/
template<int _Dimension>
inline
std::istream&
operator>>(std::istream& in, OrthtreeElementNull<_Dimension>& x) {
  return in;
}

//! Refinement functor that does nothing.
struct RefineNull {
  template<class _Orthtree>
  //! Do nothing.
  void
  operator()(const _Orthtree& orthtree, 
	     const typename _Orthtree::Element& parent, const int n,
	     const typename _Orthtree::Key& key,
	     typename _Orthtree::Element* child) const {
  }
};

//! Coarsening functor that does nothing.
struct CoarsenNull {
  template<class _Orthtree>
  //! Do nothing.
  void
  operator()
  (const _Orthtree& orthtree, 
   const ads::FixedArray<_Orthtree::NumberOfOrthants, 
   const typename _Orthtree::Element*>& children,
   const typename _Orthtree::Key& key,
   typename _Orthtree::Element* parent) const {
  }
};

END_NAMESPACE_GEOM

#endif
