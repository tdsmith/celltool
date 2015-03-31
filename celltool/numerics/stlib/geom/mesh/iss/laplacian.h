// -*- C++ -*-

/*! 
  \file geom/mesh/iss/laplacian.h
  \brief Implements Laplacian smoothing.
*/

#if !defined(__geom_mesh_iss_laplacian_h__)
#define __geom_mesh_iss_laplacian_h__

#include "IndSimpSetIncAdj.h"
#include "PointsOnManifold.h"
#include "accessors.h"
#include "geometry.h"

#include "../../../numerical/constants.h"

#include <set>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_laplacian Laplacian Smoothing
*/
//@{


//! Perform sweeps of Laplacian smoothing on the interior vertices.
/*!
  \relates IndSimpSetIncAdj

  \param mesh Pointer to the simplicial mesh.
  \param numSweeps The number of smoothing sweeps.  By default it is one.
*/
template<int N, int M, bool A, typename T, typename V, typename IS>
void
applyLaplacian(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh, int numSweeps = 1);


//! Perform sweeps of Laplacian smoothing on the vertices.
/*!
  \relates IndSimpSetIncAdj

  \param mesh Pointer to the simplicial mesh.
  \param condition The functor that returns the closest point on the boundary.
  \param maxAngleDeviation Used to define corner features.  Nodes that are 
  corner features will not be moved.
  \param numSweeps The number of smoothing sweeps.  By default it is one.

  Perform Laplacian smoothing on a 2-1 mesh (a line segment mesh in 2-D).
*/
template<bool A, typename T, typename V, typename IS, class BoundaryCondition>
void
applyLaplacian(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
	       const BoundaryCondition& condition, 
	       T maxAngleDeviation, int numSweeps = 1);


//! Perform sweeps of Laplacian smoothing on the vertices.
/*!
  \relates IndSimpSetIncAdj

  \param mesh Pointer to the simplicial mesh.
  \param manifold The boundary manifold data structure.
  \param numSweeps The number of smoothing sweeps.  By default it is one.
*/
template<bool A, typename T, typename V, typename IS, int SD>
void
applyLaplacian(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh,
	       PointsOnManifold<2,1,SD,T>* manifold,
	       int numSweeps = 1);


//! Perform sweeps of Laplacian smoothing on the boundary vertices.
/*!
  \relates IndSimpSetIncAdj

  \param mesh Pointer to the simplicial mesh.
  \param condition The functor that returns the closest point on the boundary.
  \param maxAngleDeviation Used to define corner features.  Nodes that are 
  corner features will not be moved.
  \param numSweeps The number of smoothing sweeps.  By default it is one.
*/
template<bool A, typename T, typename V, typename IS, class BoundaryCondition>
void
applyLaplacian(IndSimpSetIncAdj<3,2,A,T,V,IS>* mesh, 
	       const BoundaryCondition& condition, 
	       T maxAngleDeviation, int numSweeps = 1);


//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_laplacian_ipp__
#include "laplacian.ipp"
#undef __geom_mesh_iss_laplacian_ipp__

#endif
