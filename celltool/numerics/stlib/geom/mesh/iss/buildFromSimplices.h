// -*- C++ -*-

/*! 
  \file geom/mesh/iss/buildFromSimplices.h
  \brief Implements a builder for IndSimpSet.

  I separated this from build.h because it uses the orthogonal range query 
  package.
*/

#if !defined(__geom_mesh_iss_buildFromSimplices_h__)
#define __geom_mesh_iss_buildFromSimplices_h__

#include "IndSimpSetIncAdj.h"
#include "distinct_points.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_buildFromSimplices Build from simplices.
  These functions build the mesh from simplices.
*/
//@{

//! Build from the simplices.
/*!
  \relates IndSimpSet

  The range of vertices determines the simplices.
  Each M+1 vertices is a simplex.
  \c VertexForwardIter is a vertex forward iterator.

  \param verticesBeginning is the beginning of the range of vertices.
  \param verticesEnd is the end of the range of vertices.
  \param mesh is the output mesh.
*/
template<int N, int M, typename T, typename V, typename IS,
	 typename VertexForIter>
void
buildFromSimplices(VertexForIter verticesBeginning, 
		   VertexForIter verticesEnd,
		   IndSimpSet<N,M,true,T,V,IS>* mesh);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_buildFromSimplices_ipp__
#include "buildFromSimplices.ipp"
#undef __geom_mesh_iss_buildFromSimplices_ipp__

#endif
