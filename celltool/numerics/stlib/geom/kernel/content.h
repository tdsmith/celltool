// -*- C++ -*-

/*! 
  \file content.h
  \brief Define functions to compute content (length, area, volume, etc.).
*/
/*!
  \page content Content

  "Content" (also called hypervolume) is a dimension independent name for 
  length, area, volume, etc.  We supply functions to compute the content 
  (hypervolume) of simplices.  We group the functions according to 
  \ref content_distance "distance",
  \ref content_area "area" and
  \ref content_volume "volume".

  If the dimension
  of the simplex is equal to the dimension of its vertices, then the 
  content is signed.  If the dimension of the simplex is less than the 
  dimension of the vertices, then the content is unsigned.  (The content
  is not defined when the dimension of the simplex is greater than the 
  dimension of its vertices.)  Examples:
  - The area of a triangle in 2-D is signed.  
    (Simplex dimension = vertex dimension = 2.)
  - The area of a triangle in 3-D is unsigned.
    (Simplex dimension = 2.  Vertex dimension = 3.)
  - A triangle is 1-D does not have a well-defined area.
    (Simplex dimension = 2.  Vertex dimension = 1.)
*/

#if !defined(__geom_content_h__)
#define __geom_content_h__

#include "Point.h"

#include "../../ads/tensor/SquareMatrix.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_content)
#define DEBUG_content
#endif

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup content_distance Content: Distance */
// @{


//! Return the distance between the points \c x and \c y.
/*!
  In 1-D the distance is signed.  In N-D the distance is unsigned for
  N > 1.
 */
template<int N, typename T>
T
computeDistance(const ads::FixedArray<N,T>& x, const ads::FixedArray<N,T>& y);


//! Return the distance between the points \c x[0] and \c x[1].
template<int N, typename T>
T
computeDistance(const ads::FixedArray<2, ads::FixedArray<N,T> >& x);



//! Return the distance between the points \c x and \c y.
/*!
  In 1-D the distance is signed.  In N-D the distance is unsigned for
  N > 1.
 */
template<int N, typename T>
T
computeContent(const ads::FixedArray<N,T>& x, const ads::FixedArray<N,T>& y);


//! Return the distance between the points \c x[0] and \c x[1].
template<int N, typename T>
T
computeContent(const ads::FixedArray<2, ads::FixedArray<N,T> >& x);



//! Calculate the gradient (with respect to x) of the distance between the points \c x and \c y.
template<int N, typename T>
void
computeGradientOfDistance(const ads::FixedArray<N,T>& x, 
			  const ads::FixedArray<N,T>& y,
			  ads::FixedArray<N,T>* gradient);


//! Calculate the gradient (with respect to x[0]) of the distance between the points \c x[0] and \c x[1].
template<int N, typename T>
void
computeGradientOfDistance(const ads::FixedArray<2, ads::FixedArray<N,T> >& x,
			  ads::FixedArray<N,T>* gradient);



//! Calculate the gradient (with respect to x) of the distance between the points \c x and \c y.
template<int N, typename T>
void
computeGradientOfContent(const ads::FixedArray<N,T>& x, 
			 const ads::FixedArray<N,T>& y,
			 ads::FixedArray<N,T>* gradient);


//! Calculate the gradient (with respect to x[0]) of the distance between the points \c x[0] and \c x[1].
template<int N, typename T>
void
computeGradientOfContent(const ads::FixedArray<2, ads::FixedArray<N,T> >& x,
			 ads::FixedArray<N,T>* gradient);


// @}
//-----------------------------------------------------------------------------
/*! \defgroup content_area Content: Area */
// @{


//! Return the area of the triangle with points \c a, \c b and \c c.
/*!
  In 2-D the area is signed.
*/
template<typename T>
T
computeArea(const ads::FixedArray<2,T>& a, const ads::FixedArray<2,T>& b,
	    const ads::FixedArray<2,T>& c);


//! Return the area of the triangle with points \c p[0], \c p[1] and \c p[2].
/*!
  In 2-D the area is signed.
*/
template<typename T>
T
computeArea(const ads::FixedArray<3, ads::FixedArray<2,T> >& p);


//! Return the area of the triangle with points \c a, \c b and \c c.
/*!
  In 3-D the area is unsigned.
*/
template<typename T>
T
computeArea(const ads::FixedArray<3,T>& a, const ads::FixedArray<3,T>& b,
	    const ads::FixedArray<3,T>& c);


//! Return the area of the triangle with points \c p[0], \c p[1] and \c p[2].
/*!
  In 3-D the area is unsigned.
*/
template<typename T>
T
computeArea(const ads::FixedArray<3, ads::FixedArray<3,T> >& p);



//! Return the area of the triangle with points \c a, \c b and \c c.
/*!
  In 2-D the area is signed.  In N-D the area is unsigned for
  N > 2.
 */
template<int N, typename T>
T
computeContent(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b,
	       const ads::FixedArray<N,T>& c);


//! Return the area of the triangle with points \c p[0], \c p[1] and \c p[2].
template<int N, typename T>
T
computeContent(const ads::FixedArray<3, ads::FixedArray<N,T> >& p);



//! Calculate the gradient (with respect to a) of the area of the triangle with points \c a, \c b and \c c.
template<typename T>
void
computeGradientOfArea(const ads::FixedArray<2,T>& a, 
		      const ads::FixedArray<2,T>& b,
		      const ads::FixedArray<2,T>& c, 
		      ads::FixedArray<2,T>* gradient);


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c a, \c b and \c c.
template<typename T>
void
computeGradientOfArea(const ads::FixedArray<3,T>& a, 
		      const ads::FixedArray<3,T>& b,
		      const ads::FixedArray<3,T>& c, 
		      ads::FixedArray<3,T>* gradient);


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c p[0], \c p[1] and \c p[2].
template<int N, typename T>
void
computeGradientOfArea(const ads::FixedArray<3, ads::FixedArray<N,T> >& p, 
		      ads::FixedArray<N,T>* gradient);


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c a, \c b and \c c.
/*!
  This is simply a wrapper for computeGradientOfArea.
*/
template<int N, typename T>
inline
void
computeGradientOfContent(const ads::FixedArray<N,T>& a, 
			 const ads::FixedArray<N,T>& b,
			 const ads::FixedArray<N,T>& c, 
			 ads::FixedArray<N,T>* gradient) {
  computeGradientOfArea(a, b, c, gradient);
}


//! Calculate the gradient (with respect to a) of the area of the triangle with points \c p[0], \c p[1] and \c p[2].
/*!
  This is simply a wrapper for computeGradientOfArea.
*/
template<int N, typename T>
inline
void
computeGradientOfContent(const ads::FixedArray<3, ads::FixedArray<N,T> >& p, 
			 ads::FixedArray<N,T>* gradient) {
  computeGradientOfArea(p, gradient);
}


// @}
//-----------------------------------------------------------------------------
/*! \defgroup content_volume Content: Volume */
// @{


//! Return the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
/*!
  In 3-D the volume is signed.  In N-D the volume is unsigned for
  N > 3.
 */
template<int N, typename T>
T
computeVolume(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b,
	      const ads::FixedArray<N,T>& c, const ads::FixedArray<N,T>& d);


//! Return the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
template<int N, typename T>
T
computeVolume(const ads::FixedArray<4, ads::FixedArray<N,T> >& p);



//! Return the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
/*!
  In 3-D the volume is signed.  In N-D the volume is unsigned for
  N > 3.

  This is simply a wrapper for computeVolume().
*/
template<int N, typename T>
inline
T
computeContent(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b,
	       const ads::FixedArray<N,T>& c, const ads::FixedArray<N,T>& d) {
  return computeVolume(a, b, c, d);
}


//! Return the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
/*!
  In 3-D the volume is signed.  In N-D the volume is unsigned for
  N > 3.

  This is simply a wrapper for computeVolume().
*/
template<int N, typename T>
inline
T
computeContent(const ads::FixedArray<4, ads::FixedArray<N,T> >& p) {
  return computeVolume(p);
}



//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
template<typename T>
void
computeGradientOfVolume(const ads::FixedArray<3,T>& a, 
			const ads::FixedArray<3,T>& b,
			const ads::FixedArray<3,T>& c, 
			const ads::FixedArray<3,T>& d,
			ads::FixedArray<3,T>* gradient);


//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
template<int N, typename T>
inline
void
computeGradientOfVolume(const ads::FixedArray<4, ads::FixedArray<N,T> >& p,
			ads::FixedArray<N,T>* gradient) {
  computeGradientOfVolume(p[0], p[1], p[2], p[3], gradient);
}



//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c a, \c b, \c c and \c d.
/*!
  This is simply a wrapper for computeGradientOfVolume().
*/
template<int N, typename T>
inline
void
computeGradientOfContent(const ads::FixedArray<N,T>& a, 
			 const ads::FixedArray<N,T>& b,
			 const ads::FixedArray<N,T>& c, 
			 const ads::FixedArray<N,T>& d,
			 ads::FixedArray<N,T>* gradient) {
  computeGradientOfVolume(a, b, c, d, gradient);
}


//! Calculate the gradient (with respect to a) of the volume of the tetrahedron with points \c p[0], \c p[1], \c p[2] and \c p[3].
/*!
  This is simply a wrapper for computeGradientOfVolume().
*/
template<int N, typename T>
inline
void
computeGradientOfContent(const ads::FixedArray<4, ads::FixedArray<N,T> >& p,
			 ads::FixedArray<N,T>* gradient) {
  computeGradientOfVolume(p, gradient);
}


// @}

END_NAMESPACE_GEOM

#define __geom_content_ipp__
#include "content.ipp"
#undef __geom_content_ipp__

#endif
