// -*- C++ -*-

/*! 
  \file geom/mesh/simplex/topology.h
  \brief Geometric functions for simplices.
*/

#if !defined(__geom_mesh_simplex_topology_h__)
#define __geom_mesh_simplex_topology_h__

#include "Simplex.h"

BEGIN_NAMESPACE_GEOM

//---------------------------------------------------------------------------
// Simplex indices.
//---------------------------------------------------------------------------


//! Compute the other indices of the simplex.
/*!
  \relates Simplex
*/
void
computeOtherIndices(int i, int j, int* a, int* b);


//! Compute the other index of the simplex.
/*!
  \relates Simplex
*/
int
computeOtherIndex(int i, int j, int k);


//! Return true if the N-simplex has the specified (N-1)-face.
/*!
  \relates Simplex

  If true, set the face index.
*/
template<int N, typename V, typename T>
inline
bool
hasFace(const Simplex<N,V,T>& simplex, 
	const typename Simplex<N,V,T>::Face& face,
	int* faceIndex) {
  for (int n = 0; n != face.size(); ++n) {
    if (! simplex.hasVertex(face[n])) {
      *faceIndex = n;
      return false;
    }
  }
  return true;
}


//! Return true if the N-simplex has the specified (N-1)-face.
/*!
  \relates Simplex
*/
template<int N, typename V, typename T>
inline
bool
hasFace(const Simplex<N,V,T>& simplex, 
	const typename Simplex<N,V,T>::Face& face) {
  int faceIndex;
  return hasFace(simplex, face, &faceIndex);
}


//! Return true if the 3-simplex has the face specified by the three vertices.
/*!
  \relates Simplex
*/
template<typename V, typename T>
inline
bool
hasFace(const Simplex<3,V,T>& simplex, const V& x, const V& y, const V& z) {
  return simplex.hasVertex(x) && simplex.hasVertex(y) && simplex.hasVertex(z);
}


//! Return true if the 3-simplex has the face specified by the three vertices.
/*!
  \relates Simplex

  Set the face index.
*/
template<typename V, typename T>
inline
bool
hasFace(const Simplex<3,V,T>& simplex, const V& x, const V& y, const V& z,
	int* faceIndex) {
  if (hasFace(simplex, x, y, z)) {
    *faceIndex = computeOtherIndex(simplex.getVertexIndex(x),
				   simplex.getVertexIndex(y),
				   simplex.getVertexIndex(z));
    return true;
  }
  return false;
}

END_NAMESPACE_GEOM

#define __geom_mesh_simplex_topology_ipp__
#include "topology.ipp"
#undef __geom_mesh_simplex_topology_ipp__

#endif
