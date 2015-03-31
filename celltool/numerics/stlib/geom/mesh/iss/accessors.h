// -*- C++ -*-

/*! 
  \file geom/mesh/iss/accessors.h
  \brief Implements accessors for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_accessors_h__)
#define __geom_mesh_iss_accessors_h__

#include "IndSimpSetIncAdj.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_accessors Accessors */
//@{


//! Get the previous vertex index.
/*! \relates IndSimpSetIncAdj */
template<int N, bool A, typename T, typename V, typename IS>
int
getPreviousVertexIndex(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, int n);


//! Get the next vertex index.
/*! \relates IndSimpSetIncAdj */
template<int N, bool A, typename T, typename V, typename IS>
int
getNextVertexIndex(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, int n);


//! Get the previous vertex.
/*! \relates IndSimpSetIncAdj */
template<int N, bool A, typename T, typename V, typename IS>
inline
typename IndSimpSetIncAdj<N,1,A,T,V,IS>::Vertex
getPreviousVertex(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, const int n) {
  return mesh.getVertex(getPreviousVertexIndex(mesh, n));
}


//! Get the next vertex.
/*! \relates IndSimpSetIncAdj */
template<int N, bool A, typename T, typename V, typename IS>
inline
typename IndSimpSetIncAdj<N,1,A,T,V,IS>::Vertex
getNextVertex(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, const int n) {
  return mesh.getVertex(getNextVertexIndex(mesh, n));
}



//! Get the face in the simplex that is opposite to the given vertex.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
getFace(const IndSimpSet<N,M,A,T,V,IS>& mesh,
	int simplexIndex, int vertexIndex, 
	typename IndSimpSet<N,M,A,T,V,IS>::SimplexFace* face);


//! Get the indexed face in the simplex that is opposite to the given vertex.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
getIndexedFace(const IndSimpSet<N,M,A,T,V,IS>& mesh,
	       int simplexIndex, int vertexIndex, 
	       typename IndSimpSet<N,M,A,T,V,IS>::IndexedSimplexFace* face);


//! Get the centroid of the specified simplex.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
getCentroid(const IndSimpSet<N,M,A,T,V,IS>& mesh, int n, 
	    typename IndSimpSet<N,M,A,T,V,IS>::Vertex* x);


//! Return true if the simplices of the mesh have consistent orientations.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS>
bool
isOriented(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh);


//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_accessors_ipp__
#include "accessors.ipp"
#undef __geom_mesh_iss_accessors_ipp__

#endif
