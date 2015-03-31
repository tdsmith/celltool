// -*- C++ -*-

#if !defined(__geom_simplex_distance_ipp__)
#error This file is an implementation detail of the simplex_distance.
#endif

BEGIN_NAMESPACE_GEOM

// CONTINUE: Get rid of static variables. They are not thread safe. Use 
// functors instead.

//---------------------------------------------------------------------------
// Inside
//---------------------------------------------------------------------------

template<typename T>
inline
bool
isIn(const Simplex<1,ads::FixedArray<1,T>,T>& s, 
     const ads::FixedArray<1,T>& x) {
  return s[0][0] <= x[0] && x[0] <= s[1][0];
}


template<typename T>
inline
bool
isIn(const Simplex<2,ads::FixedArray<2,T>,T>& s, 
     const ads::FixedArray<2,T>& x) {
  // Make these static so the constructors are only called once.
  static typename Simplex<2,ads::FixedArray<2,T>,T>::Face face;
  static Line_2<T> line;
  
  // For each face.
  for (int n = 0; n != 3; ++n) {
    // Get the vertices of the n_th face.
    s.getFace(n, &face);
    // Make the supporting line of the face.
    line.make(face[0], face[1]);
    if (line.computeSignedDistance(x) > 0) {
      return false;
    }
  }
  
  // If all of the supporting line distances are non-positive, return true.
  return true;
}


template<typename T>
inline
bool
isIn(const Simplex<3,ads::FixedArray<3,T>,T>& s, 
     const ads::FixedArray<3,T>& x) {
  // Make these static so the constructors are only called once.
  static typename Simplex<3,ads::FixedArray<3,T>,T>::Face face;
  static Plane<T> plane;
  
  // For each face.
  for (int n = 0; n != 4; ++n) {
    // Get the vertices of the n_th face.
    s.getFace(n, &face);
    // Make the supporting plane of the face.
    plane.make(face[0], face[1], face[2]);
    if (plane.computeSignedDistance(x) > 0) {
      return false;
    }
  }
  
  // If all of the supporting plane distances are non-positive, return true.
  return true;
}


//---------------------------------------------------------------------------
// Interior distance.
//---------------------------------------------------------------------------

template<typename T>
inline
T
computeDistanceInterior(const Simplex<1,ads::FixedArray<1,T>,T>& s, 
			const ads::FixedArray<1,T>& x) {
#ifdef DEBUG_simplex_distance
  assert(isIn(s, x));
#endif
  return std::max(s[0][0] - x[0], x[0] - s[1][0]);
}


template<typename T>
inline
T
computeDistanceInterior(const Simplex<2,ads::FixedArray<2,T>,T>& s, 
			const ads::FixedArray<2,T>& x) {
  // Make these static so the constructors are only called once.
  static typename Simplex<2,ads::FixedArray<2,T>,T>::Face face;
  static Line_2<T> line;
  
  T d;
  T distance = - std::numeric_limits<T>::max();

  // For each face.
  for (int n = 0; n != 3; ++n) {
    // Get the vertices of the n_th face.
    s.getFace(n, &face);
    // Make the supporting line of the face.
    line.make(face[0], face[1]);
    // Compute the distance to the face.
    d = line.computeSignedDistance(x);
    // Update the distance to the simplex.
    if (d > distance) {
      distance = d;
    }
  }
  
#ifdef DEBUG_simplex_distance
  // The distance should be approximately non-positive.
  assert(distance < 10 * std::numeric_limits<T>::epsilon());
#endif

  // Return the distance to the simplex.
  return distance;
}


template<typename T>
inline
T
computeDistanceInterior(const Simplex<3,ads::FixedArray<3,T>,T>& s, 
			const ads::FixedArray<3,T>& x) {
  // Make these static so the constructors are only called once.
  static typename Simplex<3,ads::FixedArray<3,T>,T>::Face face;
  static Plane<T> plane;
  
  T d;
  T distance = - std::numeric_limits<T>::max();

  // For each face.
  for (int n = 0; n != 4; ++n) {
    // Get the vertices of the n_th face.
    s.getFace(n, &face);
    // Make the supporting plane of the face.
    plane.make(face[0], face[1], face[2]);
    // Compute the distance to the face.
    d = plane.computeSignedDistance(x);
    // Update the distance to the simplex.
    if (d > distance) {
      distance = d;
    }
  }
  
#ifdef DEBUG_simplex_distance
  // The distance should be approximately non-positive.
  assert(distance < 10 * std::numeric_limits<T>::epsilon());
#endif

  // Return the distance to the simplex.
  return distance;
}


//---------------------------------------------------------------------------
// Distance.
//---------------------------------------------------------------------------


template<typename T>
inline
T
computeDistance(const Simplex<1,ads::FixedArray<1,T>,T>& s, 
		const ads::FixedArray<1,T>& x) {
  if (x[0] < s[0][0]) {
    return s[0][0] - x[0];
  }
  if (s[1][0] < x[0]) {
    return x[0] - s[1][0];
  }
  return computeDistanceInterior(s, x);
}


template<typename T>
inline
T
computeDistance(const Simplex<1,ads::FixedArray<2,T>,T>& s, 
	  const ads::FixedArray<2,T>& x) {
  static Simplex<1,ads::FixedArray<1,T>,T> s1;
  static ads::FixedArray<1,T> x1, y1;
  
  project(s, x, &s1, &x1, &y1);
  T d = computeUnsignedDistance(s1, x1);
  return std::sqrt(d * d + y1[0] * y1[0]);
}


template<typename T>
inline
T
computeDistance(const Simplex<1,ads::FixedArray<3,T>,T>& s, 
		const ads::FixedArray<3,T>& x) {
  static ads::FixedArray<3,T> tangent, v0, v1;
  tangent = s[1];
  tangent -= s[0];
  normalize(&tangent);

  // See if the point is closest to the source end.
  v0 = x;
  v0 -= s[0];
  T ld0 = computeDotProduct(v0, tangent);
  //std::cerr << "ld0 = " << ld0;
  if (ld0 < 0) {
    return geom::computeDistance(s[0], x);
  }

  // Next see if the point is closest to the target end.
  v1 = x;
  v1 -= s[1];
  T ld1 = computeDotProduct(v1, tangent);
  //std::cerr << " ld1 = " << ld1;
  if (ld1 > 0) {
    return geom::computeDistance(s[1], x);
  }

  // Otherwise, compute the line distance.
  T d = geom::computeDistance(s[0], x);
  // With exact arithmetic, the argument of the square root is non-negative,
  // but it may be small and negative due to floating point arithmetic.
  const T arg = std::max(d * d - ld0 * ld0, 0.);
  //std::cerr << " d = " << d << "\n";
  return std::sqrt(arg);
}


template<typename T>
inline
T
computeDistance(const Simplex<2,ads::FixedArray<2,T>,T>& s, 
		const ads::FixedArray<2,T>& x) {
  typedef typename Simplex<2,ads::FixedArray<2,T>,T>::Face Face;

  //
  // Make these static so the constructors are only called once.
  //

  // The faces of the triangle.
  static ads::FixedArray<3, Face> faces;
  // The supporting line of a face.
  static Line_2<T> line;
  // Line distance.
  static ads::FixedArray<3,T> ld;
  // A simplex and point in 1-D.
  static Simplex<1,ads::FixedArray<1,T>,T> s1;
  static ads::FixedArray<1,T> x1;
  
  // For each vertex/face.
  for (int n = 0; n != 3; ++n) {
    // Get the vertices of the n_th face.
    s.getFace(n, &faces[n]);
    // Make the supporting line of the face.
    line.make(faces[n][0], faces[n][1]);
    // Compute the distance to the line.
    ld[n] = line.computeSignedDistance(x);
  }
  
  T d;
  // For each face.
  for (int n = 0; n != 3; ++n) {
    // If above the n_th face.
    if (ld[n] > 0) {
      // Return the distance to the line segment.
      project(faces[n], x, &s1, &x1);
      d = computeUnsignedDistance(s1, x1);
      return std::sqrt(d * d + ld[n] * ld[n]);
    }
  }
  // Else below all faces.
  return ads::computeMaximum(ld);
}


template<typename T>
inline
T
computeDistance(const Simplex<2,ads::FixedArray<3,T>,T>& s, 
		const ads::FixedArray<3,T>& x) {
  static Simplex<2,ads::FixedArray<2,T>,T> s2;
  static ads::FixedArray<2,T> x2;
  static ads::FixedArray<1,T> z1;
  
  project(s, x, &s2, &x2, &z1);
  T d = computeUnsignedDistance(s2, x2);
  return std::sqrt(d * d + z1[0] * z1[0]);
}


template<typename T>
inline
T
computeDistance(const Simplex<3,ads::FixedArray<3,T>,T>& s, 
		const ads::FixedArray<3,T>& x) {
  typedef typename Simplex<3,ads::FixedArray<3,T>,T>::Face Face;

  //
  // Make these static so the constructors are only called once.
  //

  // The faces of the tetrahedron.
  static ads::FixedArray<4, Face> faces;
  // The supporting plane of a face.
  static Plane<T> plane;
  // Plane distance.
  static ads::FixedArray<4,T> pd;
  // A simplex and point in 2-D.
  static Simplex<2,ads::FixedArray<2,T>,T> s2;
  static ads::FixedArray<2,T> x2;

  // For each vertex/face.
  for (int n = 0; n != 4; ++n) {
    // Get the vertices of the n_th face.
    s.getFace(n, &faces[n]);
    // Make the supporting plane of the face.
    plane.make(faces[n][0], faces[n][1], faces[n][2]);
    // Compute the distance to the plane.
    pd[n] = plane.computeSignedDistance(x);
  }
  
  T d;
  // For each face.
  for (int n = 0; n != 4; ++n) {
    // If above the n_th face.
    if (pd[n] > 0) {
      // Return the distance to the triangle.
      project(faces[n], x, &s2, &x2);
      d = computeUnsignedDistance(s2, x2);
      return std::sqrt(d * d + pd[n] * pd[n]);
    }
  }
  // Else below all faces.
  return ads::computeMaximum(pd);
}


//---------------------------------------------------------------------------
// Signed distance.
//---------------------------------------------------------------------------


template <int N, typename T>
inline
T
computeSignedDistance(const ads::FixedArray<N,T>& p,
		 const ads::FixedArray<N,T>& n,
		 const ads::FixedArray<N,T>& x) {
  static ads::FixedArray<N,T> v;
  v = x;
  v -= p;

  T d = geom::computeDistance(p, x);

  if (computeDotProduct(v, n) > 0) {
    return d;
  }
  return -d;
}


template<typename T>
inline
T
computeSignedDistance(const Simplex<1,ads::FixedArray<2,T>,T>& s, 
		      const ads::FixedArray<2,T>& x) {
  static Simplex<1,ads::FixedArray<1,T>,T> s1;
  static ads::FixedArray<1,T> x1, y1;
  
  project(s, x, &s1, &x1, &y1);
  T d = computeUnsignedDistance(s1, x1);
  if (d <= 0) {
    // Return the signed distance.
    return -y1[0];
  }
  return std::numeric_limits<T>::max();
}


template<typename T>
inline
T
computeSignedDistance(const Simplex<1,ads::FixedArray<2,T>,T>& s, 
		      const ads::FixedArray<2,T>& x,
		      ads::FixedArray<2,T>* closestPoint) {
  static Simplex<1,ads::FixedArray<1,T>,T> s1;
  static ads::FixedArray<1,T> x1, y1;
  
  project(s, x, &s1, &x1, &y1);
  T d = computeUnsignedDistance(s1, x1);
  if (d <= 0) {
    // First compute the tangent.
    *closestPoint = s[1];
    *closestPoint -= s[0];
    normalize(closestPoint);
    // Then the closest point.
    *closestPoint *= x1[0];
    *closestPoint += s[0];
    // Return the signed distance.
    return -y1[0];
  }
  return std::numeric_limits<T>::max();
}



template<typename T>
inline
T
computeSignedDistance(const Simplex<2,ads::FixedArray<3,T>,T>& s, 
		      const ads::FixedArray<3,T>& x) {
  static Simplex<2,ads::FixedArray<2,T>,T> s2;
  static ads::FixedArray<2,T> x2;
  static ads::FixedArray<1,T> z1;
  
  project(s, x, &s2, &x2, &z1);
  T d = computeUnsignedDistance(s2, x2);
  if (d <= 0) {
    return z1[0];
  }
  return std::numeric_limits<T>::max();
}


template<typename T>
inline
T
computeSignedDistance(const Simplex<2,ads::FixedArray<3,T>,T>& s, 
		      const ads::FixedArray<3,T>& n,
		      const ads::FixedArray<3,T>& x,
		      ads::FixedArray<3,T>* closestPoint) {
  static Simplex<2,ads::FixedArray<2,T>,T> s2;
  static ads::FixedArray<3,T> offset;
  static ads::FixedArray<2,T> x2;
  static ads::FixedArray<1,T> z1;

  project(s, x, &s2, &x2, &z1);
  const T d = computeUnsignedDistance(s2, x2);
  // If the point is directly above of below the triangle face.
  if (d <= 0) {
    // For the closest point on the triangle face, start at the point x.
    *closestPoint = x;
    // Then subtract the normal to the face times the distance from the face.
    offset = n;
    offset *= z1[0];
    *closestPoint -= offset;
    // Return the signed distance.
    return z1[0];
  }
  return std::numeric_limits<T>::max();
}


template<typename T>
inline
T
computeSignedDistance(const Simplex<1,ads::FixedArray<3,T>,T>& s, 
		      const ads::FixedArray<3,T>& n,
		      const ads::FixedArray<3,T>& x) {
  static ads::FixedArray<3,T> tangent, v0, v1;
  tangent = s[1];
  tangent -= s[0];
  normalize(&tangent);

  // See if the point is closest to the source end.
  v0 = x;
  v0 -= s[0];
  T ld0 = computeDotProduct(v0, tangent);
  if (ld0 < 0) {
    return std::numeric_limits<T>::max();
  }

  // Next see if the point is closest to the target end.
  v1 = x;
  v1 -= s[1];
  T ld1 = computeDotProduct(v1, tangent);
  if (ld1 > 0) {
    return std::numeric_limits<T>::max();
  }

  // Otherwise, compute the distance from the supporting line.
  T d = geom::computeDistance(s[0], x);
  // With exact arithmetic, the argument of the square root is non-negative,
  // but it may be small and negative due to floating point arithmetic.
  const T arg = std::max(d * d - ld0 * ld0, 0.);
  d = std::sqrt(arg);

  // Return the signed distance.
  if (computeDotProduct(v0, n) > 0) {
    return d;
  }
  return -d;
}


template<typename T>
inline
T
computeSignedDistance(const Simplex<1,ads::FixedArray<3,T>,T>& s, 
		      const ads::FixedArray<3,T>& n,
		      const ads::FixedArray<3,T>& x,
		      ads::FixedArray<3,T>* closestPoint) {
  static ads::FixedArray<3,T> tangent, v0, v1;
  tangent = s[1];
  tangent -= s[0];
  normalize(&tangent);

  // See if the point is closest to the source end.
  v0 = x;
  v0 -= s[0];
  T ld0 = computeDotProduct(v0, tangent);
  if (ld0 < 0) {
    return std::numeric_limits<T>::max();
  }

  // Next see if the point is closest to the target end.
  v1 = x;
  v1 -= s[1];
  T ld1 = computeDotProduct(v1, tangent);
  if (ld1 > 0) {
    return std::numeric_limits<T>::max();
  }

  // Otherwise, compute the distance from the supporting line.
  T d = geom::computeDistance(s[0], x);
  // With exact arithmetic, the argument of the square root is non-negative,
  // but it may be small and negative due to floating point arithmetic.
  const T arg = std::max(d * d - ld0 * ld0, 0.);
  d = std::sqrt(arg);

  // To compute the closest point, we start at the source vertex and add 
  // the line distance times the tangent.
  *closestPoint = s[0];
  tangent *= ld0;
  *closestPoint += tangent;

  // Return the signed distance.
  if (computeDotProduct(v0, n) > 0) {
    return d;
  }
  return -d;
}


//---------------------------------------------------------------------------
// Project to a lower dimension.
//---------------------------------------------------------------------------


// Project the simplex and the point in 2-D to 1-D.
template<typename T>
inline
void
project(const Simplex<1,ads::FixedArray<2,T>,T>& s2, 
	const ads::FixedArray<2,T>& x2,
	Simplex<1,ads::FixedArray<1,T>,T>* s1, 
	ads::FixedArray<1,T>* x1) {
  // We don't use the y offset.
  static ads::FixedArray<1,T> y1;
  project(s2, x2, s1, x1, &y1);
}



// Project the simplex and the point in 2-D to 1-D.
template<typename T>
inline
void
project(const Simplex<1,ads::FixedArray<2,T>,T>& s2, 
	const ads::FixedArray<2,T>& x2,
	Simplex<1,ads::FixedArray<1,T>,T>* s1, 
	ads::FixedArray<1,T>* x1,
	ads::FixedArray<1,T>* y1) {
  // A 2-D Cartesian point.
  typedef ads::FixedArray<2,T> P2;

  static P2 tangent, x;

  //
  // We use the line segment tangent to determine the mapping.
  //

  // Convert the line segment to a vector.
  tangent = s2[1];
  tangent -= s2[0];
  // The length of the vector.
  const T mag = computeMagnitude(tangent);
  // The unit tangent to the line segment.
  if (mag != 0) {
    tangent /= mag;
  }
  else {
    // Degenerate case: zero length simplex.
    tangent[0] = 1;
    tangent[1] = 0;
  }
  
  //
  // Map the simplex in 2-D to a simplex in 1-D
  //

  // The first vertex of the mapped simplex is the origin.
  (*s1)[0][0] = 0;
  // The second vertex of the mapped simplex lies on the positive x axis.
  (*s1)[1][0] = mag;

  // Copy the 2-D point.
  x = x2;
  // Translate the point by the same amount the simplex was translated.
  x -= s2[0];
  // Rotate the point.
  (*x1)[0] = x[0] * tangent[0] + x[1] * tangent[1];
  (*y1)[0] = - x[0] * tangent[1] + x[1] * tangent[0];
  // (c  s) (x[0])
  // (-s c) (x[1])
  // c = tangent[0]
  // s = tangent[1]
}



// Project the simplex and the point in 3-D to 2-D.
template<typename T>
inline
void
project(const Simplex<2,ads::FixedArray<3,T>,T>& s3, 
	const ads::FixedArray<3,T>& x3,
	Simplex<2,ads::FixedArray<2,T>,T>* s2, 
	ads::FixedArray<2,T>* x2) {
  // We don't use the z offset.
  static ads::FixedArray<1,T> z1;
  project(s3, x3, s2, x2, &z1);
}



// Project the simplex and the point in 3-D to 2-D.
template<typename T>
inline
void
project(const Simplex<2,ads::FixedArray<3,T>,T>& s3, 
	const ads::FixedArray<3,T>& x3,
	Simplex<2,ads::FixedArray<2,T>,T>* s2, 
	ads::FixedArray<2,T>* x2,
	ads::FixedArray<1,T>* z1) {
  typedef ads::FixedArray<3,T> P3;
  typedef ads::SquareMatrix<3,T> Matrix;

  static Matrix inverseMapping, mapping;
  static P3 xi, psi, zeta, a, b;

  //
  // First determine the inverse mapping, (x,y,z)->(xi,psi,zeta).
  //

  // xi is the tangent to the first edge of the triangle.
  xi = s3[1];
  xi -= s3[0];
  normalize(&xi);

  // zeta is normal to the triangle.
  a = s3[2];
  a -= s3[0];
  computeCrossProduct(xi, a, &zeta);
  normalize(&zeta);

  computeCrossProduct(zeta, xi, &psi);

  inverseMapping.set(xi[0], psi[0], zeta[0],  
		     xi[1], psi[1], zeta[1],
		     xi[2], psi[2], zeta[2]);

  // Take the inverse to get the mapping.  Since this is a rotation, 
  // the determinant of the matrix is 1.
  ads::computeInverse(inverseMapping, 1., &mapping);

  // The first point is mapped to the 2-D origin.
  (*s2)[0] = 0;
  // The second point is mapped to the x axis.
  a = s3[1];
  a -= s3[0];
  ads::computeProduct(mapping, a, &b);
  (*s2)[1][0] = b[0];
  (*s2)[1][1] = b[1];
  // The third point is mapped to the xy plane.
  a = s3[2];
  a -= s3[0];
  ads::computeProduct(mapping, a, &b);
  (*s2)[2][0] = b[0];
  (*s2)[2][1] = b[1];
  // Finally, map the free point.
  a = x3;
  a -= s3[0];
  ads::computeProduct(mapping, a, &b);
  (*x2)[0] = b[0];
  (*x2)[1] = b[1];
  (*z1)[0] = b[2];
}





// One can map a 2-simplex in 3-D to a 2-simplex in 2-D with a translation 
// and a rotation.
// Compute the rotation (and its inverse) for this mapping from the 
// 2-simplex in 3-D to a 2-simplex in 2-D.
template<typename T>
inline
void
computeRotation(const Simplex<2,ads::FixedArray<3,T>,T>& simplex, 
		ads::SquareMatrix<3,T>* rotation,
		ads::SquareMatrix<3,T>* inverseRotation) {
  typedef ads::FixedArray<3,T> Point;
  
  Point xi, psi, zeta;

  //
  // First determine the inverse mapping, (x,y,z)->(xi,psi,zeta).
  //

  // xi is the tangent to the first edge of the triangle.
  xi = simplex[1];
  xi -= simplex[0];
  normalize(&xi);

  // zeta is normal to the triangle.
  psi = simplex[2]; // Use psi as a temporary variable.
  psi -= simplex[0];
  computeCrossProduct(xi, psi, &zeta);
  normalize(&zeta);

  // psi is orthonormal to zeta and xi.
  computeCrossProduct(zeta, xi, &psi);

  inverseRotation->set(xi[0], psi[0], zeta[0],  
		       xi[1], psi[1], zeta[1],
		       xi[2], psi[2], zeta[2]);

  // Take the inverse to get the mapping.  Since this is a rotation, 
  // the determinant of the matrix is 1.
  ads::computeInverse(*inverseRotation, 1., rotation);
}



template<typename T>
inline
void
mapSimplex(const Simplex<2,ads::FixedArray<3,T>,T>& s3, 
	   const ads::SquareMatrix<3,T>& rotation,
	   Simplex<2,ads::FixedArray<2,T>,T>* s2) {
  typedef ads::FixedArray<3,T> P3;

  P3 a, b;

  // The first point is mapped to the 2-D origin.
  (*s2)[0] = 0;
  // The second point is mapped to the x axis.
  a = s3[1];
  a -= s3[0];
  ads::computeProduct(rotation, a, &b);
  (*s2)[1][0] = b[0];
  (*s2)[1][1] = b[1];
  // The third point is mapped to the xy plane.
  a = s3[2];
  a -= s3[0];
  ads::computeProduct(rotation, a, &b);
  (*s2)[2][0] = b[0];
  (*s2)[2][1] = b[1];
}



template<typename T>
inline
void
mapPointDown(const ads::FixedArray<3,T>& x3,
	     const Simplex<2,ads::FixedArray<3,T>,T>& s3, 
	     const ads::SquareMatrix<3,T>& rotation,
	     ads::FixedArray<2,T>* x2) {
  typedef ads::FixedArray<3,T> P3;

  P3 a, b;

  // Translate.
  a = x3;
  a -= s3[0];
  // Rotate.
  ads::computeProduct(rotation, a, &b);
  (*x2)[0] = b[0];
  (*x2)[1] = b[1];
  //(*z1)[0] = b[2];
}



template<typename T>
inline
void
mapPointUp(const ads::FixedArray<2,T>& x2,
	   const Simplex<2,ads::FixedArray<3,T>,T>& s3, 
	   const ads::SquareMatrix<3,T>& inverseRotation,
	   ads::FixedArray<3,T>* x3) {
  typedef ads::FixedArray<3,T> P3;

  // Rotate.
  P3 a;
  a[0] = x2[0];
  a[1] = x2[1];
  a[2] = 0;
  ads::computeProduct(inverseRotation, a, x3);
  // Translate.
  *x3 += s3[0];
}



//---------------------------------------------------------------------------
// Closest Point.
//---------------------------------------------------------------------------

// Return the unsigned distance from the 2-D point to the 1-simplex 
// and compute the closest point.
template<typename T>
inline
T
computeClosestPoint(const Simplex<1,ads::FixedArray<2,T>,T>& simplex, 
		    const ads::FixedArray<2,T>& point,
		    ads::FixedArray<2,T>* closestPoint) {
  geom::SegmentMath<2,T> segment(simplex[0], simplex[1]);
  return computeDistanceAndClosestPoint(segment, point, closestPoint);
}



// Return the unsigned distance from the 3-D point to the 1-simplex 
// and compute the closest point.
template<typename T>
inline
T
computeClosestPoint(const Simplex<1,ads::FixedArray<3,T>,T>& simplex, 
		    const ads::FixedArray<3,T>& point,
		    ads::FixedArray<3,T>* closestPoint) {
  geom::SegmentMath<3,T> segment(simplex[0], simplex[1]);
  return computeDistanceAndClosestPoint(segment, point, closestPoint);
}



// Return the unsigned distance from the 2-D point to the 2-simplex 
// and compute the closest point.
template<typename T>
inline
T
computeClosestPoint(const Simplex<2,ads::FixedArray<2,T>,T>& simplex, 
		    const ads::FixedArray<2,T>& point,
		    ads::FixedArray<2,T>* closestPoint) {
  typedef ads::FixedArray<2,T> Point2;
  typedef Simplex<2,Point2,T> Simplex2;
  typedef typename Simplex2::Face Face;

  // The simplex dimension.
  const int M = 2;

  // A face of the triangle.
  Face face;
  // The supporting line of a face.
  Line_2<T> line;
  // Line distance.
  ads::FixedArray<M + 1,T> lineDistance;

  // For each vertex/face.
  for (int m = 0; m != M + 1; ++m) {
    // Get the vertices of the m_th face.
    simplex.getFace(m, &face);
    // Make the supporting line of the face.
    line.make(face[0], face[1]);
    // Compute the signed distance to the line.
    lineDistance[m] = line.computeSignedDistance(point);
    // If above the m_th face.  (Outside the triangle.)
    if (lineDistance[m] > 0) {
      // Compute the closest point on the line segment.
      // Return the positive distance to the line segment.
      return computeClosestPoint(face, point, closestPoint);
    }
  }
  
  // If the point is below all faces, the distance is non-positive.
  *closestPoint = point;
  return ads::computeMaximum(lineDistance);
}



// Return the unsigned distance from the 3-D point to the 2-simplex 
// and compute the closest point.
template<typename T>
inline
T
computeClosestPoint(const Simplex<2,ads::FixedArray<3,T>,T>& simplex, 
		    const ads::FixedArray<3,T>& point,
		    ads::FixedArray<3,T>* closestPoint) {
  typedef ads::FixedArray<2,T> Point2;
  typedef Simplex<2,Point2,T> Simplex2;

  // Determine the rotations in the mapping between 3-D and 2-D.
  ads::SquareMatrix<3,T> rotation, inverseRotation;
  computeRotation(simplex, &rotation, &inverseRotation);

  // Map the simplex.
  Simplex2 simplex2;
  mapSimplex(simplex, rotation, &simplex2);

  // Map the point down to 2-D.
  Point2 point2;
  mapPointDown(point, simplex, rotation, &point2);

  // Compute the closest point in 2-D.
  Point2 closestPoint2;
  computeClosestPoint(simplex2, point2, &closestPoint2);

  // Map the point up to 3-D.
  mapPointUp(closestPoint2, simplex, inverseRotation, closestPoint);

  // Return the unsigned distance.
  return geom::computeDistance(point, *closestPoint);
}


END_NAMESPACE_GEOM

// End of file.
