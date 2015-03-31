// -*- C++ -*-

/*! 
  \file geom/kernel/distance.h
  \brief Define functions to compute distance.
*/
/*!
  \page geom_distance Distance

  There are functions for computing upper and lower bounds for the distance to 
  object(s) contained in a bounding box.

  signed_distance_upper_bound(const BBox<N,T>& box,const typename BBox<N,T>::point_type& x)

  signed_distance_lower_bound(const BBox<N,T>& box,const typename BBox<N,T>::point_type& x)

*/

#if !defined(__geom_distance_h__)
#define __geom_distance_h__

#include "BBox.h"
#include "Point.h"
#include "content.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_distance)
#define DEBUG_distance
#endif

BEGIN_NAMESPACE_GEOM

// CONTINUE: continue fixing.  I think I should remove the signed distance 
// functions.

//---------------------------------------------------------------------------
// Bounds on the distance to objects in a bounding box.
//---------------------------------------------------------------------------


//! Return an upper bound on the signed distance from the point to the objects in the box.
/*!
  Consider some objects contained in the bounding box: \c box.  (Specifically,
  the objects form an N-D manifold.  The distance from the point \c x to
  the manifold is signed; positive outside and negative inside.)  This
  function returns an upper bound on the distance to the objects.
*/
template<int N, typename T>
inline
T
computeUpperBoundOnSignedDistance(const BBox<N,T>& box, 
				  const typename BBox<N,T>::Point& x) {
  return computeUpperBoundOnSignedDistance(box, x, Loki::Int2Type<N!=1>());
}


//! Return an lower bound on the signed distance from the point to the objects in the box.
/*!
  Consider some objects contained in the bounding box: \c box.  (Specifically,
  the objects form an N-D manifold.  The distance from the point \c x to
  the manifold is signed; positive outside and negative inside.)  This
  function returns an lower bound on the distance to the objects.
*/
template<int N, typename T>
inline
T
computeLowerBoundOnSignedDistance(const BBox<N,T>& box, 
				  const typename BBox<N,T>::Point& x) {
  return computeLowerBoundOnSignedDistance(box, x, Loki::Int2Type<N!=1>());
}


//! Return an upper bound on the signed distance.
template<int N, typename T>
T
computeUpperBoundOnUnsignedDistance(const BBox<N,T>& box, 
				    const typename BBox<N,T>::Point& x);


//! Return a lower bound on the signed distance.
template<int N, typename T>
T
computeLowerBoundOnUnsignedDistance(const BBox<N,T>& box, 
				    const typename BBox<N,T>::Point& x);

END_NAMESPACE_GEOM

#define __geom_distance_ipp__
#include "distance.ipp"
#undef __geom_distance_ipp__

#endif
