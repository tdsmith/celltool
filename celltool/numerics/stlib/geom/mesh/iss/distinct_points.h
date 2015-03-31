// -*- C++ -*-

/*! 
  \file distinct_points.h
  \brief From a set of points, generate an indexed set of distinct points.
*/

#if !defined(__geom_mesh_iss_distinct_points_h__)
#define __geom_mesh_iss_distinct_points_h__

#include "IndSimpSet.h"

#include "../../kernel/BBox.h"
#include "../../orq/CellArray.h"

#include <iostream>
#include <vector>

#include <cassert>

BEGIN_NAMESPACE_GEOM


//-----------------------------------------------------------------------------
/*! \defgroup iss_distinct_points Identify distinct points and remove duplicate points. */
//@{

//! From a set of points, generate an indexed set of distinct points.
/*!
  \param pointsBeginning is the beginning of a range of points.
  \param pointsEnd is the end of a range of points.
  \param distinctPointsOutput  The distinct points will be written to this 
  iterator.
  \param indicesOutput  For each input point, there is an index into the 
  container of distinct points.
  \param minDistance is the minimum distance separating distinct points.

  Template parameters:
  - \c N is the space dimension.
  - \c PtForIter is a forward iterator for Cartesian points.
  - \c PtOutIter is an output iterator for Cartesian points.
  - \c IntOutIter in an output iterator for integers.
  - \c T is the number type.
*/
template<int N, typename PtForIter, typename PtOutIter, typename IntOutIter,
	 typename T>
void
buildDistinctPoints(PtForIter pointsBeginning, PtForIter pointsEnd,
		    PtOutIter distinctPointsOutput,
		    IntOutIter indicesOutput,
		    const T minDistance);


//! From a set of points, generate an indexed set of distinct points.
/*!
  \param pointsBeginning is the beginning of a range of points.
  \param pointsEnd is the end of a range of points.
  \param distinctPoints  The distinct points will be written to this iterator.
  \param indices  For each input point, there is an index into the 
  container of distinct points.

  Template parameters:
  - \c N is the space dimension.
  - \c PtForIter is a forward iterator for Cartesian points.
  - \c PtOutIter is an output iterator for Cartesian points.
  - \c IntOutIter in an output iterator for integers.
  
  This function chooses an appropriate minimum distance and then calls
  the above buildDistinctPoints() function.
*/
template<int N, typename PtForIter, typename PtOutIter, typename IntOutIter>
void
buildDistinctPoints(PtForIter pointsBeginning, PtForIter pointsEnd,
		    PtOutIter distinctPoints, IntOutIter indices);


//! Remove duplicate vertices.
template<int N, int M, bool A, typename T, typename V, typename IS>
void
removeDuplicateVertices(IndSimpSet<N,M,A,T,V,IS>* x, T minDistance);


//! Remove duplicate vertices.
/*!  
  This function chooses an appropriate minimum distance and then calls
  the above removeDuplicateVertices() function.
*/
template<int N, int M, bool A, typename T, typename V, typename IS>
void
removeDuplicateVertices(IndSimpSet<N,M,A,T,V,IS>* x);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_distinct_points_ipp__
#include "distinct_points.ipp"
#undef __geom_mesh_iss_distinct_points_ipp__

#endif
