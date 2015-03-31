// -*- C++ -*-

/*! 
  \file Point.h
  \brief Define functions to treat a FixedArray as a point or vector.
*/

#if !defined(__geom_Point_h__)
#define __geom_Point_h__

#include "../defs.h"

#include "../../ads/array/FixedArray.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_Point)
#define DEBUG_Point
#endif

BEGIN_NAMESPACE_GEOM

/*!
  \page point Point Functions

  We define functions for treating an ads::FixedArray as a point.
  We group the functions according to:
  \ref point_math "mathematical functions",
  \ref point_length "length functions",
  \ref point_distance "distance functions",
  \ref point_angle "angle functions" and
  \ref point_rotation "rotation functions".

  \todo Check the efficiency of these functions.
*/

//-----------------------------------------------------------------------------
/*! \defgroup point_math Point: Mathematical Functions */
// @{


//! Return the dot product of x and y.
/*! 
  This has specializations for 1, 2 and 3-D.
*/
template<int N, typename T>
T
computeDotProduct(const ads::FixedArray<N,T>& x, 
		  const ads::FixedArray<N,T>& y);


//! Return the cross product of x and y.
template<typename T>
ads::FixedArray<3,T>
computeCrossProduct(const ads::FixedArray<3,T>& x, 
		    const ads::FixedArray<3,T>& y);

    
//! Compute the cross product of x and y.
/*! 
  This exists as an optimization of geom::cross().  With this function
  there is no need to construct an ads::FixedArray.
*/
template<typename T>
void
computeCrossProduct(const ads::FixedArray<3,T>& x, 
		    const ads::FixedArray<3,T>& y,
		    ads::FixedArray<3,T>* result);

    
//! The scalar triple product of three vectors: \f$a \cdot b \times c\f$.
template<typename T>
T 
computeTripleProduct(const ads::FixedArray<3,T>& a, 
		     const ads::FixedArray<3,T>& b, 
		     const ads::FixedArray<3,T>& c);


//! Return the discriminant of the vectors.
template<typename T>
T 
computeDiscriminant(const ads::FixedArray<2,T>& p, 
		    const ads::FixedArray<2,T>& q);


//! Compute an orthogonal vector.
template<typename T>
void
computeAnOrthogonalVector(const ads::FixedArray<3,T>& vector,
			  ads::FixedArray<3,T>* orthogonal);


// @}
//-----------------------------------------------------------------------------
/*! \defgroup point_length Point: Length Functions */
// @{


//! Return the squared magnitude of this vector.
template<int N, typename T>
T 
computeSquaredMagnitude(const ads::FixedArray<N,T>& x);


//! Return the magnitude of this vector.
template<int N, typename T>
T 
computeMagnitude(const ads::FixedArray<N,T>& x);


//! Normalize this vector so it has unit length.
template<int N, typename T>
void 
normalize(ads::FixedArray<N,T>* x);


// @}
//-----------------------------------------------------------------------------
/*! \defgroup point_distance Point: Distance Functions */
// @{

  
//! Return the squared distance between two points.
/*! 
  This function has specializations for 1, 2 and 3-D.
*/
template<int N, typename T>
T 
computeSquaredDistance(const ads::FixedArray<N,T>& p, 
		       const ads::FixedArray<N,T>& q);


// @}
//-----------------------------------------------------------------------------
/*! \defgroup point_angle Point: Angles */
// @{


//! Positive turn: return 1.  No turn: return 0.  Negative turn: return -1.
template<typename T>
int 
computeSignOfTurn(const ads::FixedArray<2,T>& p, 
		  const ads::FixedArray<2,T>& q, 
		  const ads::FixedArray<2,T>& r);


// CONTINUE: perhaps replace this with something better.
//! Positive turn: return 1.  No turn: return 0.  Negative turn: return -1.
template<typename T>
int 
computeApproximateSignOfTurn(const ads::FixedArray<2,T>& p, 
			     const ads::FixedArray<2,T>& q, 
			     const ads::FixedArray<2,T>& r);


//! Return the pseudo-angle between vec and the x axis.
template<typename T>
T 
computePseudoAngle(const ads::FixedArray<2,T>& vec);


// CONTINUE: Check the implementation.
//! Return the angle between the two vectors.
/*!
  The angle is in the range [0..pi].
*/
template<int N, typename T>
T
computeAngle(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b);


// @}
//-----------------------------------------------------------------------------
/*! \defgroup point_rotation Point: Rotation */
// @{


//! Rotate the vector + pi / 2.
template<typename T>
void 
rotatePiOver2(ads::FixedArray<2,T>* p);

  
//! Rotate the vector - pi / 2.
template<typename T>
void 
rotateMinusPiOver2(ads::FixedArray<2,T>& p);


// @}
  
END_NAMESPACE_GEOM

#define __geom_Point_ipp__
#include "Point.ipp"
#undef __geom_Point_ipp__

#endif
