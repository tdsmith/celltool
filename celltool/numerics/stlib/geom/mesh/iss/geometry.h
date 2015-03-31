// -*- C++ -*-

/*! 
  \file geom/mesh/iss/geometry.h
  \brief Geometric functions for indexed simplex sets.
*/

#if !defined(__geom_mesh_iss_geometry_h__)
#define __geom_mesh_iss_geometry_h__

#include "IndSimpSetIncAdj.h"

#include "../simplex/geometry.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_geometry Geometric functions for simplicial meshes. */
//@{

//
// Normal
//

//! Return the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
typename IndSimpSetIncAdj<2,1,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<2,1,A,T,V,IS>& mesh, int n);


//! Compute the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
void
computeVertexNormal(const IndSimpSetIncAdj<2,1,A,T,V,IS>& mesh, int n,
		    typename IndSimpSetIncAdj<2,1,A,T,V,IS>::Vertex* normal);


//! Return the outward normal at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
typename IndSimpSetIncAdj<2,2,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<2,2,A,T,V,IS>& mesh, int n);


//! Compute the outward normal at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
void
computeVertexNormal(const IndSimpSetIncAdj<2,2,A,T,V,IS>& mesh, int n,
		    typename IndSimpSetIncAdj<2,2,A,T,V,IS>::Vertex* normal);


//! Return the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
typename IndSimpSetIncAdj<3,2,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, int n);


//! Compute the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
void
computeVertexNormal(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, int n,
		    typename IndSimpSetIncAdj<3,2,A,T,V,IS>::Vertex* normal);


//! Return the outward normal at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
typename IndSimpSetIncAdj<3,3,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<3,3,A,T,V,IS>& mesh, int n);


//! Compute the outward normal at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
void
computeVertexNormal(const IndSimpSetIncAdj<3,3,A,T,V,IS>& mesh, int n,
		    typename IndSimpSetIncAdj<3,3,A,T,V,IS>::Vertex* normal);



//! Compute the outward normal for the specified simplex (triangle face).
template<bool A, typename T, typename V, typename IS>
void
computeSimplexNormal(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		     int simplexIndex, V* simplexNormal);


//! Compute the outward normals for the simplices (triangle faces).
template<bool A, typename T, typename V, typename IS, bool AA>
void
computeSimplexNormals(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		      ads::Array<1,V,AA>* simplexNormals);


//! Compute the outward normals for the simplices (line segments).
template<bool A, typename T, typename V, typename IS, bool AA>
void
computeSimplexNormals(const IndSimpSetIncAdj<2,1,A,T,V,IS>& mesh, 
		      ads::Array<1,V,AA>* simplexNormals);


//! Compute the outward normals for the vertices.
template<bool A, typename T, typename V, typename IS, bool AA>
void
computeVertexNormals(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		     const ads::Array<1,V,AA>& simplexNormals,
		     ads::Array<1,V,AA>* vertexNormals);


//! Compute the outward normals for the simplices and vertices.
template<bool A, typename T, typename V, typename IS, bool A1, bool A2>
void
computeSimplexAndVertexNormals(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh,
			       ads::Array<1,V,A1>* simplexNormals,
			       ads::Array<1,V,A2>* vertexNormals);



//! Return the cosine of the interior angle at the specified vertex.
/*!
  \pre The vertex must have two incident simplices.
 */
template<int N, bool A, typename T, typename V, typename IS>
T
computeCosineAngle(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, 
		   int vertexIndex);


//! Return the cosine of the interior angle at the specified 1-face.
/*!
  \pre The 1-face must have two incident simplices.
 */
template<bool A, typename T, typename V, typename IS>
T
computeCosineAngle(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		   const typename IndSimpSetIncAdj<3,2,A,T,V,IS>::Face& face);


//! Return the cosine of the interior angle at the specified boundary vertex.
/*!
  \pre It must be a boundary vertex.

  The angle is in the range [0..pi].
*/
template<bool A, typename T, typename V, typename IS>
T
computeCosineBoundaryAngle(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
			   int vertexIndex);


//! Return the solid interior angle at the specified vertex.
template<bool A, typename T, typename V, typename IS>
T
computeAngle(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, int n);


//! Project the line segments to 1-D and collect them.
template<bool A, typename T, typename V, typename IS, typename OutputIterator>
void
projectAndGetSimplices(const IndSimpSet<2,1,A,T,V,IS>& mesh, 
		       OutputIterator simplices);


//! Project the triangle simplices to 2-D and collect them.
template<bool A, typename T, typename V, typename IS, typename OutputIterator>
void
projectAndGetSimplices(const IndSimpSet<3,2,A,T,V,IS>& mesh, 
		       OutputIterator simplices);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_geometry_ipp__
#include "geometry.ipp"
#undef __geom_mesh_iss_geometry_ipp__

#endif
