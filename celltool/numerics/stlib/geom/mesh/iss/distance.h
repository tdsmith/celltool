// -*- C++ -*-

/*! 
  \file geom/mesh/iss/distance.h
  \brief Compute the distance to a simplicial mesh for a single point.
*/

#if !defined(__geom_mesh_iss_distance_h__)
#define __geom_mesh_iss_distance_h__

#include "geometry.h"

#include "../simplex/simplex_distance.h"

#include "../../../ads/algorithm/min_max.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_distance Compute the distance to a simplicial mesh for a single point. */
//@{

//---------------------------------------------------------------------------
// Signed distance, 2-D space, 1-manifold, single point.
//---------------------------------------------------------------------------

//! Compute the signed distance to the mesh and closest point on the mesh.
/*!
  Use this function to compute the distance to a mesh for a single point.
  Or call it a few times if you want to compute the signed distance for a 
  small number of points.  This function has linear computational complexity
  in the size of the mesh.  If you are computing the distance for many points,
  use \c ISS_SignedDistance instead.

  This function will return correct results only if the distance is 
  well-defined.  The mesh must be a 1-manifold.  If the mesh has a boundary,
  the closest point must lie on the interior.  If the closest point is 
  on the boundary, the signed distance is not defined.  (The unsigned
  distance, however, is defined.)
  
  The algorithm for computing the signed distance first uses the
  vertices of the mesh to obtain an upper bound on the squared
  distance to the mesh.  Then the signed distance is computed to those
  vertices and faces which could possibly contain the closest point.
*/
template<bool _A, typename _T, typename _V, typename _IS>
typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 const ads::Array<1, typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number>&
 squaredHalfLengths,
 const typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex* closestPoint);

//! Compute the signed distance to the mesh and closest point on the mesh.
/*!
  This function computes the squared half lengths of the faces and then
  calls the above function.
*/
template<bool _A, typename _T, typename _V, typename _IS>
typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 const typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex* closestPoint);

//! Compute the signed distance to the mesh.
/*!
  This function just calls the above function which computes the signed 
  distance and closest point.
*/
template<bool _A, typename _T, typename _V, typename _IS>
inline
typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh, 
 const typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex& point) {
  typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex closestPoint;
  return computeSignedDistance(mesh, point, &closestPoint);
}

//---------------------------------------------------------------------------
// Signed distance, 2-D space, 1-manifold, multiple points.
//---------------------------------------------------------------------------

//! Compute the signed distances to the mesh and closest points on the mesh.
template<bool _A, typename _T, typename _V, typename _IS, 
	 typename InputIterator, typename NumberOutputIterator,
	 typename PointOutputIterator>
void
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 InputIterator pointsBeginning, InputIterator pointsEnd,
 NumberOutputIterator distances, PointOutputIterator closestPoints);

//! Compute the signed distances to the mesh and closest points on the mesh.
template<bool _A, typename _T, typename _V, typename _IS, 
	 typename InputIterator, typename NumberOutputIterator>
inline
void
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 InputIterator pointsBeginning, InputIterator pointsEnd,
 NumberOutputIterator distances) {
  computeSignedDistance(mesh, pointsBeginning, pointsEnd, distances,
			ads::constructTrivialOutputIterator());
}

//---------------------------------------------------------------------------
// Signed distance, 3-D space, 2-manifold, single point.
//---------------------------------------------------------------------------

//! Compute the signed distance to the mesh and closest point on the mesh.
/*!
  Use this function to compute the distance to a mesh for a single point.
  Or call it a few times if you want to compute the signed distance for a 
  small number of points.  This function has linear computational complexity
  in the size of the mesh.  If you are computing the distance for many points,
  use \c ISS_SignedDistance instead.

  This function will return correct results only if the distance is 
  well-defined.  The mesh must be a 2-manifold.  If the mesh has a boundary,
  the closest point must lie on the interior.  If the closest point is 
  on the boundary, the signed distance is not defined.  (The unsigned
  distance, however, is defined.)
  
  The algorithm for computing the signed distance first uses the
  vertices of the mesh to obtain an upper bound on the squared
  distance to the mesh.  Then the signed distance is computed to those
  vertices, edges, and faces which could possibly contain the closest point.
*/
template<bool _A, typename _T, typename _V, typename _IS>
typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 const ads::Array<1, typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number>&
 squaredLongestEdgeLengths,
 const typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex* closestPoint);

//! Compute the signed distance to the mesh and closest point on the mesh.
/*!
  This function computes the squared edge lengths and then calls the
  above function.
*/
template<bool _A, typename _T, typename _V, typename _IS>
typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 const typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex* closestPoint);

//! Compute the signed distance to the mesh.
/*!
  This function just calls the above function which computes the signed 
  distance and closest point.
*/
template<bool _A, typename _T, typename _V, typename _IS>
inline
typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh, 
 const typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex& point) {
  typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex closestPoint;
  return computeSignedDistance(mesh, point, &closestPoint);
}

//---------------------------------------------------------------------------
// Signed distance, 3-D space, 2-manifold, multiple points.
//---------------------------------------------------------------------------

//! Compute the signed distances to the mesh and closest points on the mesh.
template<bool _A, typename _T, typename _V, typename _IS, 
	 typename InputIterator, typename NumberOutputIterator,
	 typename PointOutputIterator>
void
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 InputIterator pointsBeginning, InputIterator pointsEnd,
 NumberOutputIterator distances, PointOutputIterator closestPoints);

//! Compute the signed distances to the mesh and closest points on the mesh.
template<bool _A, typename _T, typename _V, typename _IS, 
	 typename InputIterator, typename NumberOutputIterator>
inline
void
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 InputIterator pointsBeginning, InputIterator pointsEnd,
 NumberOutputIterator distances) {
  computeSignedDistance(mesh, pointsBeginning, pointsEnd, distances,
			ads::constructTrivialOutputIterator());
}

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_distance_ipp__
#include "distance.ipp"
#undef __geom_mesh_iss_distance_ipp__

#endif
