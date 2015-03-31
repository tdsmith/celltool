// -*- C++ -*-

#if !defined(__geom_spatialIndexing_SpatialIndex_ipp__)
#error This file is an implementation detail of the class SpatialIndex.
#endif

BEGIN_NAMESPACE_GEOM


//--------------------------------------------------------------------------
// Manipulators.

// Transform to the specified neighbor.
template<int _Dimension, int _MaximumLevel>
inline
void
SpatialIndex<_Dimension, _MaximumLevel>::
transformToNeighbor(const int n) {
#ifdef DEBUG_geom_spatialIndexing_SpatialIndex
  assert(0 <= n && n < 2 * Dimension);
  assert(hasNeighbor(*this, n));
#endif
  // The coordinate is n / 2.
  // The direction in that coordinate is n % 2.
  // Change coordinate by +-1.
  _coordinates[n / 2] += 2 * (n % 2) - 1;
  updateCode();
}

  
END_NAMESPACE_GEOM

// End of file.
