// -*- C++ -*-

#if !defined(__geom_content_ipp__)
#error This file is an implementation detail of content.
#endif

#include <cassert>
#include <cmath>

BEGIN_NAMESPACE_GEOM


//
// Distance.
//


// Return the distance between the points x and y.
// Specialization for 1-D: signed distance.
template<typename T>
inline
T
computeDistance(const ads::FixedArray<1,T>& x, const ads::FixedArray<1,T>& y) {
  return y[0] - x[0];
}


// Return the distance between the points x and y.
template<int N, typename T>
inline
T
computeDistance(const ads::FixedArray<N,T>& x, const ads::FixedArray<N,T>& y) {
  return std::sqrt(computeSquaredDistance(x, y));
}


// Return the distance between the points x[0] and x[1].
template<int N, typename T>
inline
T
computeDistance(const ads::FixedArray<2, ads::FixedArray<N,T> >& x) {
  return geom::computeDistance(x[0], x[1]);
}


// Return the distance between the points x and y.
template<int N, typename T>
inline
T
computeContent(const ads::FixedArray<N,T>& x, const ads::FixedArray<N,T>& y) {
  return geom::computeDistance(x, y);
}


// Return the distance between the points x[0] and x[1].
template<int N, typename T>
inline
T
computeContent(const ads::FixedArray<2, ads::FixedArray<N,T> >& x) {
  return geom::computeDistance(x[0], x[1]);
}


//
// Gradient of distance.
//


// Calculate the gradient with respect to x of the distance between 
// the points x and y.
// Specialization for 1-D.
template<typename T>
inline
void
computeGradientOfDistance(const ads::FixedArray<1,T>& x, 
			  const ads::FixedArray<1,T>& y,
			  ads::FixedArray<1,T>* gradient) {
  (*gradient)[0] = -1;
}


// Return gradient with respect to x of the distance between the 
// points x and y.
template<int N, typename T>
inline
void
computeGradientOfDistance(const ads::FixedArray<N,T>& x, 
			  const ads::FixedArray<N,T>& y, 
			  ads::FixedArray<N,T>* gradient) {
  const T d = computeDistance(x, y);
  for (int n = 0; n != N; ++n) {
    (*gradient)[n] = (x[n] - y[n]) / d;
  }
}


// Calculate the gradient (with respect to x[0]) of the distance between 
// the points \c x[0] and \c x[1].
template<int N, typename T>
inline
void
computeGradientOfDistance(const ads::FixedArray<2, ads::FixedArray<N,T> >& x,
			  ads::FixedArray<N,T>* gradient) {
  computeGradientOfDistance(x[0], x[1], gradient);
}


// Calculate the gradient (with respect to x) of the distance between 
// the points \c x and \c y.
template<int N, typename T>
inline
void
computeGradientOfContent(const ads::FixedArray<N,T>& x, 
			 const ads::FixedArray<N,T>& y,
			 ads::FixedArray<N,T>* gradient) {
  computeGradientOfDistance(x, y, gradient);
}


// Calculate the gradient (with respect to x[0]) of the distance between 
// the points \c x[0] and \c x[1].
template<int N, typename T>
inline
void
computeGradientOfContent(const ads::FixedArray<2, ads::FixedArray<N,T> >& x,
			 ads::FixedArray<N,T>* gradient) {
  computeGradientOfDistance(x, gradient);
}




//
// Area.
//

// Return the signed area of the triangle with 2-D points a, b and c.
template<typename T>
inline
T
computeArea(const ads::FixedArray<2,T>& a, const ads::FixedArray<2,T>& b,
	    const ads::FixedArray<2,T>& c) {
  return (a[0] * b[1] + b[0] * c[1] + c[0] * a[1] -
	  a[0] * c[1] - b[0] * a[1] - c[0] * b[1]) / 2;
}

// Return the signed area of the triangle with 2-D points p[0], p[1] and p[2].
template<typename T>
inline
T
computeArea(const ads::FixedArray<3, ads::FixedArray<2,T> >& p) {
  return computeArea(p[0], p[1], p[2]);
}


// Return the unsigned area of the triangle with 3-D points a, b and c.
template<typename T>
inline
T
computeArea(const ads::FixedArray<3,T>& a, const ads::FixedArray<3,T>& b,
	    const ads::FixedArray<3,T>& c) {
  return std::sqrt(std::pow(a[0] * b[1] + b[0] * c[1] + c[0] * a[1] -
			    a[0] * c[1] - b[0] * a[1] - c[0] * b[1], 2) +
		   std::pow(a[1] * b[2] + b[1] * c[2] + c[1] * a[2] -
			    a[1] * c[2] - b[1] * a[2] - c[1] * b[2], 2) +
		   std::pow(a[2] * b[0] + b[2] * c[0] + c[2] * a[0] -
			    a[2] * c[0] - b[2] * a[0] - c[2] * b[0], 2)) / 2;
}


// Return the unsigned area of the triangle with 3-D points p[0], p[1] and p[2].
template<typename T>
inline
T
computeArea(const ads::FixedArray<3, ads::FixedArray<3,T> >& p) {
  return computeArea(p[0], p[1], p[2]);
}


// Return the area of the triangle with points a, b and c.
template<int N, typename T>
inline
T
computeContent(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b,
	       const ads::FixedArray<N,T>& c) {
  return computeArea(a, b, c);
}


// Return the area of the triangle with points p[0], p[1] and p[2].
template<int N, typename T>
inline
T
computeContent(const ads::FixedArray<3, ads::FixedArray<N,T> >& p) {
  return computeArea(p);
}



//
// Gradient of area.
//


// Calculate the gradient (with respect to a) of the signed area 
// of the triangle with 2-D points a, b and c.
template<typename T>
inline
void
computeGradientOfArea(const ads::FixedArray<2,T>& a, 
		      const ads::FixedArray<2,T>& b,
		      const ads::FixedArray<2,T>& c, 
		      ads::FixedArray<2,T>* gradient) {
  (*gradient)[0] = (b[1] - c[1]) / 2;
  (*gradient)[1] = (c[0] - b[0]) / 2;
}


// Calculate the gradient (with respect to a) of the unsigned area 
// of the triangle with 3-D points a, b and c.
template<typename T>
inline
void
computeGradientOfArea(const ads::FixedArray<3,T>& a, 
		      const ads::FixedArray<3,T>& b,
		      const ads::FixedArray<3,T>& c, 
		      ads::FixedArray<3,T>* gradient) {
  const T ar = computeArea(a, b, c);
  (*gradient)[0] = ((b[1] - c[1]) * (a[0] * b[1] + b[0] * c[1] + c[0] * a[1] -
				     a[0] * c[1] - b[0] * a[1] - c[0] * b[1])+
		    (c[2] - b[2]) * (a[2] * b[0] + b[2] * c[0] + c[2] * a[0] -
				     a[2] * c[0] - b[2] * a[0] - c[2] * b[0]))/
    (4 * ar);
  (*gradient)[1] = ((c[0] - b[0]) * (a[0] * b[1] + b[0] * c[1] + c[0] * a[1] -
				     a[0] * c[1] - b[0] * a[1] - c[0] * b[1])+
		    (b[2] - c[2]) * (a[1] * b[2] + b[1] * c[2] + c[1] * a[2] -
				     a[1] * c[2] - b[1] * a[2] - c[1] * b[2]))/
    (4 * ar);
  (*gradient)[2] = ((b[0] - c[0]) * (a[2] * b[0] + b[2] * c[0] + c[2] * a[0] -
				     a[2] * c[0] - b[2] * a[0] - c[2] * b[0])+
		    (c[1] - b[1]) * (a[1] * b[2] + b[1] * c[2] + c[1] * a[2] -
				     a[1] * c[2] - b[1] * a[2] - c[1] * b[2]))/
    (4 * ar);
}


// Calculate the gradient (with respect to a) of the area of the triangle 
// with points \c p[0], \c p[1] and \c p[2].
template<int N, typename T>
inline
void
computeGradientOfArea(const ads::FixedArray<3, ads::FixedArray<N,T> >& p, 
		      ads::FixedArray<N,T>* gradient) {
  return computeGradientOfArea(p[0], p[1], p[2], gradient);
}





//
// Volume.
//

// Return the signed volume of the 3-D tetrahedron with points a, b, c and d.
/*
  Specialization for 3-D.
  The volume is the determinant of
  | 1 a[0] a[1] a[2] |
  | 1 b[0] b[1] b[2] |
  | 1 c[0] c[1] c[2] |
  | 1 d[0] d[1] d[2] |
  (The formula from MathWorld has the wrong sign.)
 */
template<typename T>
inline
T
computeVolume(const ads::FixedArray<3,T>& a, const ads::FixedArray<3,T>& b,
	      const ads::FixedArray<3,T>& c, const ads::FixedArray<3,T>& d) {
  return 
    (- a[0] * b[1] * c[2] - a[1] * b[2] * c[0] - a[2] * b[0] * c[1] 
     + a[0] * b[2] * c[1] + a[1] * b[0] * c[2] + a[2] * b[1] * c[0] 
     + a[0] * b[1] * d[2] + a[1] * b[2] * d[0] + a[2] * b[0] * d[1] 
     - a[0] * b[2] * d[1] - a[1] * b[0] * d[2] - a[2] * b[1] * d[0] 
     - a[0] * c[1] * d[2] - a[1] * c[2] * d[0] - a[2] * c[0] * d[1] 
     + a[0] * c[2] * d[1] + a[1] * c[0] * d[2] + a[2] * c[1] * d[0] 
     + b[0] * c[1] * d[2] + b[1] * c[2] * d[0] + b[2] * c[0] * d[1] 
     - b[0] * c[2] * d[1] - b[1] * c[0] * d[2] - b[2] * c[1] * d[0]) / 6;
}


// Return the signed volume of the tetrahedron with points p[0], p[1], p[2] and p[3].
template<typename T>
inline
T
computeVolume(const ads::FixedArray<4, ads::FixedArray<3,T> >& p) {
  return computeVolume(p[0], p[1], p[2], p[3]);
}


// Return the volume of the tetrahedron with points a, b, c and d.
template<int N, typename T>
inline
T
computeVolume(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b,
	      const ads::FixedArray<N,T>& c, const ads::FixedArray<N,T>& d) {
  ads::FixedArray<4, ads::FixedArray<N,T> > p(a, b, c, d);
  return computeVolume(p);
}


// Return the volume of the tetrahedron with points p[0], p[1], p[2] and p[3].
template<int N, typename T>
inline
T
computeVolume(const ads::FixedArray<4, ads::FixedArray<N,T> >& p) {
  ads::SquareMatrix<5,T> m;
  m(0,0) = m(1,1) = m(2,2) = m(3,3) = m(4,4) = 0;
  m(0,1) = m(0,2) = m(0,3) = m(0,4) = m(1,0) = m(2,0) = m(3,0) = m(4,0) = 1;
  for (int i = 0; i != 3; ++i) {
    for (int j = i+1; j != 4; ++j) {
      m(i+1,j+1) = m(j+1,i+1) = computeSquaredDistance(p[i], p[j]);
    }
  }
  return std::sqrt(ads::computeDeterminant(m) / 288);
}




//
// Gradient of volume.
//


// Calculate the gradient (with respect to a) of the signed volume of the 
// 3-D tetrahedron with points a, b, c and d.
template<typename T>
inline
void
computeGradientOfVolume(const ads::FixedArray<3,T>& a, 
			const ads::FixedArray<3,T>& b,
			const ads::FixedArray<3,T>& c, 
			const ads::FixedArray<3,T>& d,
			ads::FixedArray<3,T>* gradient) {
  (*gradient)[0] = (b[2] * c[1] - b[1] * c[2] - b[2] * d[1] + 
		    c[2] * d[1] + b[1] * d[2] - c[1] * d[2]) / 6;
  (*gradient)[1] = (-b[2] * c[0] + b[0] * c[2] + b[2] * d[0] - 
		    c[2] * d[0] - b[0] * d[2] + c[0] * d[2]) / 6;
  (*gradient)[2] = (b[1] * c[0] - b[0] * c[1] - b[1] * d[0] + 
		    c[1] * d[0] + b[0] * d[1] - c[0] * d[1]) / 6;
}


// CONTINUE: Implement the gradient of unsigned volume.


END_NAMESPACE_GEOM
