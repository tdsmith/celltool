// -*- C++ -*-

/*! 
  \file geom/mesh/iss/onManifold.h
  \brief Implements on-manifold operations for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_onManifold_h__)
#define __geom_mesh_iss_onManifold_h__

#include "IndSimpSetIncAdj.h"
#include "ISS_SimplexQuery.h"

#include "../../../ads/iterator/IntIterator.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_onManifold Vertices on a manifold
  These functions determine the vertices on a manifold.
*/
//@{

//! Get the vertices on the manifold.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 int MM, bool MA, typename MT, typename MV, typename MIS,
	 typename IntOutIter>
void
determineVerticesOnManifold(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			    const IndSimpSet<N,MM,MA,MT,MV,MIS>& manifold, 
			    IntOutIter indexIterator,
			    T epsilon = 
			    std::sqrt(std::numeric_limits<T>::epsilon()));
			     

//! Get the vertices (from the set of vertices) on the manifold.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntInIter,
	 int MM, bool MA, typename MT, typename MV, typename MIS,
	 typename IntOutIter>
void
determineVerticesOnManifold(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			    IntInIter indicesBeginning, IntInIter indicesEnd,
			    const IndSimpSet<N,MM,MA,MT,MV,MIS>& manifold, 
			    IntOutIter indexIterator,
			    T epsilon = 
			    std::sqrt(std::numeric_limits<T>::epsilon()));

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_onManifold_ipp__
#include "onManifold.ipp"
#undef __geom_mesh_iss_onManifold_ipp__

#endif
