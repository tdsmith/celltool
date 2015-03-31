// -*- C++ -*-

#if !defined(__geom_mesh_simplex_geometry_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// The dihedral angle between two faces.
template<typename T>
inline
T
computeAngle(const Simplex<3,ads::FixedArray<3,T>,T>& s, 
	     const int a, const int b) {
  Simplex<2,ads::FixedArray<3,T>,T> face;
  ads::FixedArray<3,T> an, bn;

  s.getFace(a, &face);
  Plane<T> plane(face[0], face[1], face[2]);
  an = plane.getNormal();

  s.getFace(b, &face);
  plane.make(face[0], face[1], face[2]);
  bn = plane.getNormal();
  bn.negate();
  
  return computeAngle(an, bn);
}


// The solid angle at a vertex.
template<typename T>
inline
T
computeAngle(const Simplex<3,ads::FixedArray<3,T>,T>& s, const int n) {
  // The simplex dimension plus 1.
  const int D = 4;
  return computeAngle(s, (n+1)%D, (n+2)%D) +
    computeAngle(s, (n+2)%D, (n+3)%D) +
    computeAngle(s, (n+3)%D, (n+1)%D) - numerical::Constants<T>::Pi();
}


// Project the simplex to a lower dimension.
template<typename T>
inline
void
projectToLowerDimension(const Simplex<1,ads::FixedArray<2,T>,T>& s,
			Simplex<1,ads::FixedArray<1,T>,T>* t) {
  // Make the 1-D line segment.
  (*t)[0][0] = 0.0;
  (*t)[1][0] = geom::computeDistance(s[0], s[1]);
}


// Project the simplex to a lower dimension.
template<typename T>
inline
void
projectToLowerDimension(const Simplex<1,ads::FixedArray<3,T>,T>& s,
			Simplex<1,ads::FixedArray<1,T>,T>* t) {
  // Make the 1-D line segment.
  (*t)[0][0] = 0.0;
  (*t)[1][0] = geom::computeDistance(s[0], s[1]);
}


// Project the simplex to a lower dimension.
template<typename T>
inline
void
projectToLowerDimension(const Simplex<2,ads::FixedArray<3,T>,T>& s,
			Simplex<2,ads::FixedArray<2,T>,T>* t) {
  typedef ads::FixedArray<3,T> Pt3;

  //
  // Project the triangle in 3-D to a triangle in 2-D.
  //

  // Translate the first vertex to the origin.
  Pt3 v1 = s[1];
  v1 -= s[0];
  Pt3 v2 = s[2];
  v2 -= s[0];

  // Length of 0-1.
  const T a = computeMagnitude(v1);
  // Length of 0-2.
  const T b = computeMagnitude(v2);
  
  // Normalize the vectors that define the edges.
  if (a != 0) {
    v1 /= a;
  }
  if (b != 0) {
    v2 /= b;
  }

  // The cosine of the angle.
  const T cosTheta = computeDotProduct(v1, v2);
  // The sine of the angle.
  const T argument = 1.0 - cosTheta * cosTheta;
  const T sinTheta = (argument >= 0 ? std::sqrt(argument) : 0);

  // Make the 2-D triangle.
  (*t)[0] = 0.0;
  (*t)[1][0] = a;
  (*t)[1][1] = 0.0;
  (*t)[2][0] = b * cosTheta;
  (*t)[2][1] = b * sinTheta;
}

END_NAMESPACE_GEOM

// End of file.
