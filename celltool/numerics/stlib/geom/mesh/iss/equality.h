// -*- C++ -*-

/*! 
  \file geom/mesh/iss/equality.h
  \brief Equality operators for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_equality_h__)
#define __geom_mesh_iss_equality_h__

#include "IndSimpSetIncAdj.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_equality Equality 
  Test the equality of two indexed simplex sets.  These functions are for 
  debugging purposes only.  They don't do anything fancy like check if the 
  vertices or simplices are given in different order.
*/
// @{

//! Return true if the vertices and indexed simplices are equal.
/*! \relates IndSimpSet */
template<int N, int M, 
	 bool A1, bool A2, 
	 typename T, 
	 typename V1, typename V2, 
	 typename IS1, typename IS2>
inline
bool
operator==(const IndSimpSet<N,M,A1,T,V1,IS1>& x,
	   const IndSimpSet<N,M,A2,T,V2,IS2>& y) {
  return (x.getVertices() == y.getVertices() && 
	   x.getIndexedSimplices() == y.getIndexedSimplices());
}

//! Return true if the vertices and indexed simplices are not equal.
/*! \relates IndSimpSet */
template<int N, int M, 
	 bool A1, bool A2, 
	 typename T, 
	 typename V1, typename V2, 
	 typename IS1, typename IS2>
inline
bool
operator!=(const IndSimpSet<N,M,A1,T,V1,IS1>& x,
	   const IndSimpSet<N,M,A2,T,V2,IS2>& y) {
  return !(x == y);
}

//
// Equality.
//

//! Return true if the meshes are equal.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, 
	 bool A1, bool A2, 
	 typename T, 
	 typename V1, typename V2, 
	 typename IS1, typename IS2>
inline
bool
operator==(const IndSimpSetIncAdj<N,M,A1,T,V1,IS1>& x,
	   const IndSimpSetIncAdj<N,M,A2,T,V2,IS2>& y) {
  return (x.getVertices() == y.getVertices() && 
	  x.getIndexedSimplices() == y.getIndexedSimplices() &&
	  x.getVertexSimplexIncidence() == y.getVertexSimplexIncidence() &&
	  x.getSimplexAdjacencies() == y.getSimplexAdjacencies());
}

//! Return true if the meshes are not equal.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, 
	 bool A1, bool A2, 
	 typename T, 
	 typename V1, typename V2, 
	 typename IS1, typename IS2>
inline
bool
operator!=(const IndSimpSetIncAdj<N,M,A1,T,V1,IS1>& x,
	   const IndSimpSetIncAdj<N,M,A2,T,V2,IS2>& y) {
  return !(x == y);
}


// @}

END_NAMESPACE_GEOM

#endif
