// -*- C++ -*-

/*! 
  \file geom/kernel/orientation.h
  \brief Functions to compute orientation.
*/
/*!
  \page geom_kernel_orientation Orientation

  CONTINUE.
*/

#if !defined(__geom_kernel_orientation_h__)
#define __geom_kernel_orientation_h__

#include "../defs.h"

#include "../../ads/tensor/SquareMatrix.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_kernel_orientation)
#define DEBUG_geom_kernel_orientation
#endif

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup geom_kernel_orientation Orientation Tests */
// @{


//! Compute the orientation determinant.
/*!
  Return the determinant of:
  \f[
  \Delta = \left[
  \begin{array}{ccc}
  1 & a_0 & a_1 \\
  1 & b_0 & b_1 \\
  1 & c_0 & c_1
  \end{array}
  \right]
  \f]
  It is positive for a left turn, negative for a right turn, and zero if
  the points are colinear.

  See \ref ads_edelsbrunner2001 "Geometry and Topology for Mesh Generation."
*/
template<typename T>
T
computeOrientationDeterminant(const ads::FixedArray<2,T>& a,
			      const ads::FixedArray<2,T>& b,
			      const ads::FixedArray<2,T>& c);


//! Compute the in-circle determinant.
/*!
  Return the determinant of:
  \f[
  \Gamma = \left[
  \begin{array}{cccc}
  1 & a_0 & a_1 & a_2\\
  1 & b_0 & b_1 & b_2\\
  1 & c_0 & c_1 & c_2\\
  1 & d_0 & d_1 & d_2
  \end{array}
  \right]
  \f]
  where \f$a_2 = a_0^2 + a_1^2\f$.
  Point \c d lies inside the circle passing through \c a, \c b, and \c c 
  if and only if \f$\mathrm{det}(\Delta) \cdot \mathrm{det}(\Gamma) < 0\f$.

  See \ref ads_edelsbrunner2001 "Geometry and Topology for Mesh Generation."
*/
template<typename T>
T
computeInCircleDeterminant(const ads::FixedArray<2,T>& a,
			   const ads::FixedArray<2,T>& b,
			   const ads::FixedArray<2,T>& c,
			   const ads::FixedArray<2,T>& d);

//! Return true if \c d is inside the circle with \c a, \c b, and \c c on its boundary.
/*!
  Point \c d lies inside the circle passing through \c a, \c b, and \c c 
  if and only if \f$\mathrm{det}(\Delta) \cdot \mathrm{det}(\Gamma) < 0\f$.

  See \ref ads_edelsbrunner2001 "Geometry and Topology for Mesh Generation."
*/
template<typename T>
inline
bool
isInCircle(const ads::FixedArray<2,T>& a,
	   const ads::FixedArray<2,T>& b,
	   const ads::FixedArray<2,T>& c,
	   const ads::FixedArray<2,T>& d) {
  return computeOrientationDeterminant(a, b, c) * 
    computeInCircleDeterminant(a, b, c, d) < 0;
}


// @}

END_NAMESPACE_GEOM

#define __geom_kernel_orientation_ipp__
#include "orientation.ipp"
#undef __geom_kernel_orientation_ipp__

#endif
