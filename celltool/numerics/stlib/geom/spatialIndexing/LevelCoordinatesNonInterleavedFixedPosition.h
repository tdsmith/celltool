// -*- C++ -*-

/*! 
  \file geom/spatialIndexing/LevelCoordinatesNonInterleavedFixedPosition.h
  \brief CONTINUE
*/

#if !defined(__geom_spatialIndexing_LevelCoordinatesNonInterleavedFixedPosition_h__)
#define __geom_spatialIndexing_LevelCoordinatesNonInterleavedFixedPosition_h__

#include "../defs.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_LevelCoordinatesNonInterleavedFixedPosition)
#define DEBUG_geom_spatialIndexing_LevelCoordinatesNonInterleavedFixedPosition
#endif

BEGIN_NAMESPACE_GEOM

template<int Dimensions, int NumberOfLevels>
class
LevelCoordinatesNonInterleavedFixedPosition;

END_NAMESPACE_GEOM

#define __geom_spatialIndexing_LevelCoordinatesNonInterleavedFixedPosition_ipp__
#include "LevelCoordinatesNonInterleavedFixedPosition.ipp"
#undef __geom_spatialIndexing_LevelCoordinatesNonInterleavedFixedPosition_ipp__

#endif
