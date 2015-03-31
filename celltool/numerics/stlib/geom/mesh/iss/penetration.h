// -*- C++ -*-

/*! 
  \file geom/mesh/iss/penetration.h
  \brief Report penetrations.
*/

#if !defined(__geom_mesh_iss_penetration_h__)
#define __geom_mesh_iss_penetration_h__

#include "build.h"
#include "ISS_SignedDistance.h"

#include "../../orq/CellArray.h"

#include <map>
#include <tr1/tuple>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_penetration Report penetrations.
*/
//@{

//! Report the points that penetrate the solid mesh.
/*!
  \relates IndSimpSetIncAdj

  \param mesh The solid mesh.
  \param pointsBeginning The beginning of a list of points.
  \param pointsEnd The end of a list of points.
  \param penetrations Output iterator for the penetrations.
  \return The number of points that penetrate the solid.

  Penetrations are reported as a 3-tuple: point index, mesh simplex index, and
  closest point on the mesh boundary.  The structure for this is 
  std::tr1::tuple<int, int, ads::FixedArray<3, T> > .
*/
template<int N, bool A, typename T, typename V, typename IS, 
	 typename PointRandomAccessIterator,
	 typename TupleOutputIterator>
inline
int
reportPenetrations(const IndSimpSetIncAdj<N,N,A,T,V,IS>& mesh,
		   PointRandomAccessIterator pointsBeginning,
		   PointRandomAccessIterator pointsEnd,
		   TupleOutputIterator penetrations);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_penetration_ipp__
#include "penetration.ipp"
#undef __geom_mesh_iss_penetration_ipp__

#endif
