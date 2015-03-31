// -*- C++ -*-

#if !defined(__geom_Point_ipp__)
#error This file is an implementation detail of Point.
#endif

#include <cassert>
#include <cmath>

BEGIN_NAMESPACE_GEOM

//
// Math operators.
//


template<int N, typename T>
inline
T
computeDotProduct(const ads::FixedArray<N,T>& x, 
		  const ads::FixedArray<N,T>& y) {
  LOKI_STATIC_CHECK(N > 3, DimensionMustBeGreaterThan3BecauseOfSpecializations);
  T d = 0;
  for (int i = 0; i < N; ++i) {
    d += x[i] * y[i];
  }
  return d;
}


template<typename T>
inline
T
computeDotProduct(const ads::FixedArray<1,T>& x, 
		  const ads::FixedArray<1,T>& y) {
  return x[0] * y[0];
}


template<typename T>
inline
T
computeDotProduct(const ads::FixedArray<2,T>& x, 
		  const ads::FixedArray<2,T>& y) {
  return x[0] * y[0] + x[1] * y[1];
}


template<typename T>
inline
T
computeDotProduct(const ads::FixedArray<3,T>& x, 
		  const ads::FixedArray<3,T>& y) {
  return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}




template<typename T>
inline
ads::FixedArray<3,T>
computeCrossProduct(const ads::FixedArray<3,T>& x, 
		    const ads::FixedArray<3,T>& y) {
  return ads::FixedArray<3,T>(x[1]*y[2] - y[1]*x[2],
			      y[0]*x[2] - x[0]*y[2], 
			      x[0]*y[1] - y[0]*x[1]);
}


template<typename T>
inline
void
computeCrossProduct(const ads::FixedArray<3,T>& x, 
		    const ads::FixedArray<3,T>& y,
		    ads::FixedArray<3,T>* result) {
  (*result)[0] = x[1]*y[2] - y[1]*x[2];
  (*result)[1] = y[0]*x[2] - x[0]*y[2];
  (*result)[2] = x[0]*y[1] - y[0]*x[1];
}



template<typename T>
inline
T 
computeTripleProduct(const ads::FixedArray<3,T>& a, 
		     const ads::FixedArray<3,T>& b, 
		     const ads::FixedArray<3,T>& c) {
  ads::FixedArray<3,T> x;

  computeCrossProduct(b, c, &x);
  return computeDotProduct(a, x); 
}


template<typename T>
inline
T 
computeDiscriminant(const ads::FixedArray<2,T>& p, 
		    const ads::FixedArray<2,T>& q) { 
  return p[0] * q[1] - p[1] * q[0]; 
}


// Compute an orthogonal vector.
template<typename T>
inline
void
computeAnOrthogonalVector(const ads::FixedArray<3,T>& vector,
			  ads::FixedArray<3,T>* orthogonal) {
  const T mag = computeMagnitude(vector);
  assert(mag != 0);

  // One of (1,0,0) and (0,1,0) is independent to the vector.
  ads::FixedArray<3,T> x(1, 0, 0);
  const T d0 = computeDotProduct(vector, x);
  x[0] = 0;
  x[1] = 1;
  const T d1 = computeDotProduct(vector, x);
  // If (1,0,0) is a better independent direction than (0,1,0).
  if (d0 < d1) {
    (*orthogonal)[0] = 1;
    (*orthogonal)[1] = 0;
    (*orthogonal)[2] = 0;
    x[0] = 1;
    x[1] = 0;
    x *= d0 / mag;
  }
  // Otherwise (0,1,0) is a better independent direction than (1,0,0).
  else {
    (*orthogonal)[0] = 0;
    (*orthogonal)[1] = 1;
    (*orthogonal)[2] = 0;
    x *= d1 / mag;
  }
  *orthogonal -= x;
}


//
// Length operations.
//


template<int N, typename T>
inline
T 
computeSquaredMagnitude(const ads::FixedArray<N,T>& x) {
  return computeDotProduct(x, x); 
}


template<int N, typename T>
inline
T 
computeMagnitude(const ads::FixedArray<N,T>& x) { 
  return std::sqrt(computeDotProduct(x, x)); 
}


template<int N, typename T>
inline
void 
normalize(ads::FixedArray<N,T>* x) 
{
#ifdef DEBUG_Point
  const T mag = computeMagnitude(*x);
  assert(mag != 0);
  *x /= mag;
#else
  *x /= computeMagnitude(*x);
#endif
}


//
// Distance
//

  
template<int N, typename T>
inline
T
computeSquaredDistance(const ads::FixedArray<N,T>& p, 
		       const ads::FixedArray<N,T>& q) {
  LOKI_STATIC_CHECK(N > 3, DimensionMustBeGreaterThan3BecauseOfSpecializations);
  ads::FixedArray<N,T> a(p - q);
  return computeDotProduct(a, a);
}


// Specialization for 1-D
template<typename T>
inline
T
computeSquaredDistance(const ads::FixedArray<1,T>& x, 
		       const ads::FixedArray<1,T>& y) {
  const T a = x[0] - y[0];
  return a * a;
}
  

// Specialization for 2-D
template<typename T>
inline
T
computeSquaredDistance(const ads::FixedArray<2,T>& x, 
		       const ads::FixedArray<2,T>& y) {
  const T a = x[0] - y[0];
  const T b = x[1] - y[1];
  return a * a + b * b;
}

  
// Specialization for 3-D
template<typename T>
inline
T
computeSquaredDistance(const ads::FixedArray<3,T>& x, 
		       const ads::FixedArray<3,T>& y) {
  const T a = x[0] - y[0];
  const T b = x[1] - y[1];
  const T c = x[2] - y[2];
  return a * a + b * b + c * c;
}
  

//
// Angles between points
//


template<typename T>
inline
int 
computeSignOfTurn(const ads::FixedArray<2,T>& p, 
		  const ads::FixedArray<2,T>& q, 
		  const ads::FixedArray<2,T>& r) {
  const T disc = computeDiscriminant(q - p, r - q);
  if (disc > 0) {
    return -1;
  }
  else if (disc < 0) {
    return 1;
  }
  return 0;
}


template<typename T>
inline
int 
computeApproximateSignOfTurn(const ads::FixedArray<2,T>& p, 
			     const ads::FixedArray<2,T>& q, 
			     const ads::FixedArray<2,T>& r) {
  ads::FixedArray<2,T> u = q - p;
  ads::FixedArray<2,T> v = r - q;
  normalize(&u);
  normalize(&v);
  const T disc = computeDiscriminant(u, v);
  if (disc > 1e-5) {
    return -1;
  }
  else if (disc < -1e-5) {
    return 1;
  }
  return 0;
}


template<typename T>
inline
T 
computePseudoAngle(const ads::FixedArray<2,T>& vec) {
  const T dxpdy = std::abs(vec[0]) + std::abs(vec[1]);
  T theta = (dxpdy == 0) ? 0 : vec[1] / dxpdy;
  if (vec[0] < 0) {
    theta = 2 - theta;
  }
  else if (vec[1] < 0) {
    theta = 4 + theta;
  }
  return theta;
}


template<int N, typename T>
inline
T
computeAngle(const ads::FixedArray<N,T>& a, const ads::FixedArray<N,T>& b) {
  return std::acos(computeDotProduct(a, b) / (computeMagnitude(a) * 
					      computeMagnitude(b)));
}


//
// Rotations
//

template<typename T>
inline
void 
rotatePiOver2(ads::FixedArray<2,T>* p) {
  const T temp = (*p)[0];
  (*p)[0] = -(*p)[1];
  (*p)[1] = temp;
}
  

template<typename T>
inline
void 
rotateMinusPiOver2(ads::FixedArray<2,T>* p) {
  const T temp = (*p)[0];
  (*p)[0] = (*p)[1];
  (*p)[1] = -temp;
}
  
END_NAMESPACE_GEOM
