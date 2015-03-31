// -*- C++ -*-

/*! 
  \file geom/mesh/iss/transform.h
  \brief Implements operations that transform a IndSimpSet.
*/

#if !defined(__geom_mesh_iss_transform_h__)
#define __geom_mesh_iss_transform_h__

#include "boundaryCondition.h"
#include "build.h"
#include "quality.h"
#include "set.h"

#include "../simplex/SimplexJac.h"

#include "../../../ads/iterator/IntIterator.h"
#include "../../../ads/iterator/TrivialOutputIterator.h"
#include "../../../ads/iterator/TransformIterator.h"

#include <stack>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_transform Transform Vertices or Simplices
  These are function that transform indexed simplex sets.
*/
//@{

//! Pack the ISS to get rid of unused vertices.
/*!
  \relates IndSimpSet

  Adjust the vertex indices accordingly.
*/
template<int N, int M, typename T, typename V, typename IS, 
	 typename IntOutputIterator>
void
pack(IndSimpSet<N,M,true,T,V,IS>* mesh, IntOutputIterator usedVertexIndices);



//! Pack the ISS to get rid of unused vertices.
/*!
  \relates IndSimpSet

  Adjust the vertex indices accordingly.
*/
template<int N, int M, typename T, typename V, typename IS>
inline
void
pack(IndSimpSet<N,M,true,T,V,IS>* mesh) { 
  pack(mesh, ads::TrivialOutputIterator());
}


//! Orient each simplex so it has non-negative volume.
/*! \relates IndSimpSet */
template<int N, bool A, typename T, typename V, typename IS>
void
orientPositive(IndSimpSet<N,N,A,T,V,IS>* mesh);


//! Reverse the orientation of each simplex.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
reverseOrientation(IndSimpSet<N,M,A,T,V,IS>* mesh);


//! Transform each vertex in the range with the specified function.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter, class UnaryFunction>
void
transform(IndSimpSet<N,M,A,T,V,IS>* mesh,
	  IntForIter beginning, IntForIter end, const UnaryFunction& f);


//! Transform each vertex in the mesh with the specified function.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS, 
	  class UnaryFunction>
void
transform(IndSimpSet<N,M,A,T,V,IS> mesh, const UnaryFunction& f);


//! Transform each vertex in the range with the closest point in the normal direction.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter, class ISS>
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  IntForIter beginning, IntForIter end,
	  const ISS_SD_ClosestPointDirection<ISS>& f);


//! Transform each vertex in the mesh with the closest point in the normal direction.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS, class ISS>
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  const ISS_SD_ClosestPointDirection<ISS>& f);


//! Transform each vertex in the range with the closer point in the normal direction.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter, class ISS>
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  IntForIter beginning, IntForIter end,
	  const ISS_SD_CloserPointDirection<ISS>& f);


//! Transform each vertex in the mesh with the closer point in the normal direction.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS, 
	  class ISS>
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  const ISS_SD_CloserPointDirection<ISS>& f);


//! Remove simplices until there are none with minimum adjacencies less than specified.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, typename T, typename V, typename IS>
void
removeLowAdjacencies(IndSimpSetIncAdj<N,M,true,T,V,IS>* mesh, 
		     int minRequiredAdjencies);


//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_transform_ipp__
#include "transform.ipp"
#undef __geom_mesh_iss_transform_ipp__

#endif
