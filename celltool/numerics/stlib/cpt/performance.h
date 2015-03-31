// -*- C++ -*-

#if !defined(__cpt_performance_h__)
#define __cpt_performance_h__

#include "defs.h"

#include <iostream>

BEGIN_NAMESPACE_CPT

#ifdef CPT_PERFORMANCE
namespace performance {

  extern int countFaceScanConverted;
  extern int countEdgeScanConverted;
  extern int countVertexScanConverted;

  extern int countFaceDistancesComputed;
  extern int countEdgeDistancesComputed;
  extern int countVertexDistancesComputed;

  extern int countFaceDistancesSet;
  extern int countEdgeDistancesSet;
  extern int countVertexDistancesSet;

  extern double timeMakeFacePolyhedra;
  extern double timeMakeEdgePolyhedra;
  extern double timeMakeVertexPolyhedra;

  extern double timeScanConvertFacePolyhedra;
  extern double timeScanConvertEdgePolyhedra;
  extern double timeScanConvertVertexPolyhedra;

  extern double timeFaceCpt;
  extern double timeEdgeCpt;
  extern double timeVertexCpt;

  void
  print(std::ostream& out);

}
#endif

END_NAMESPACE_CPT

#endif
