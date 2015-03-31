// -*- C++ -*-

#if !defined(__cpt_Vertex_h__)
#define __cpt_Vertex_h__

// Local
#include "defs.h"

#include "../ads/array/FixedArray.h"
#include "../ads/algorithm/sign.h"

#include "../geom/grid/RegularGrid.h"
#include "../geom/kernel/Point.h"
#include "../geom/polytope/IndexedEdgePolyhedron.h"
#include "../geom/polytope/ScanConversionPolyhedron.h"

#include <vector>

#include <cmath>

BEGIN_NAMESPACE_CPT


template<int N, typename T = double>
class Vertex;


//! Equality operator
/*! \relates Vertex */
template<int N, typename T>
bool 
operator==(const Vertex<N,T>& a, const Vertex<N,T>& b);


//! Inequality operator
/*! \relates Vertex */
template<int N, typename T>
inline
bool 
operator!=(const Vertex<N,T>& a, const Vertex<N,T>& b) {
  return !(a == b);
}


END_NAMESPACE_CPT

#define __cpt_Vertex2_ipp__
#include "Vertex2.ipp"
#undef __cpt_Vertex2_ipp__

#define __cpt_Vertex3_ipp__
#include "Vertex3.ipp"
#undef __cpt_Vertex3_ipp__

#endif
