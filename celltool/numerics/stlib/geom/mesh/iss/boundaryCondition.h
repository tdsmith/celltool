// -*- C++ -*-

/*! 
  \file geom/mesh/iss/boundaryCondition.h
  \brief Boundary condition functions for indexed simplex sets.
*/

#if !defined(__geom_mesh_iss_boundaryCondition_h__)
#define __geom_mesh_iss_boundaryCondition_h__

#include "geometry.h"
#include "ISS_SignedDistance.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_boundaryCondition Boundary condition functions for simplicial meshes. */
//@{

//! Apply the closest point boundary condition at a vertex.
template<bool A, typename T, typename V, typename IS, class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPoint<ISS>& condition,
		       int n);

//! Apply the closest point in the normal direction boundary condition at a vertex.
template<bool A, typename T, typename V, typename IS,
	  class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPointDirection<ISS>& condition,
		       int n);

//! Apply the closest point boundary condition at a vertex.
template<bool A, typename T, typename V, typename IS, class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<3,2,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPoint<ISS>& condition,
		       int n);

//! Apply the closest point in the normal direction boundary condition at a vertex.
template<bool A, typename T, typename V, typename IS,
	  class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<3,2,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPointDirection<ISS>& condition,
		       int n);

//! Apply the condition at a vertex.
/*!
  \note The vertex may be in the interior or on the boundary.
*/
template<int N, bool A, typename T, typename V, typename IS,
	 class UnaryFunction>
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const UnaryFunction& condition,
		       int n);

//! Apply the closest point boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPoint<ISS>& condition,
		       int n);

//! Apply the closer point boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_CloserPoint<ISS>& condition,
		       int n);

//! Apply the closest point in the normal direction boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPointDirection<ISS>& condition,
		       int n);

//! Apply the closer point in the normal direction boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_CloserPointDirection<ISS>& condition,
		       int n);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_boundaryCondition_ipp__
#include "boundaryCondition.ipp"
#undef __geom_mesh_iss_boundaryCondition_ipp__

#endif
