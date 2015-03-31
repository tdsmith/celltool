// -*- C++ -*-

#if !defined(__geom_Plane_ipp__)
#error This file is an implementation detail of the class Plane.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//


template<typename T>
inline
Plane<T>::
Plane() : 
  _point(), 
  _normal()
{}


template<typename T>
inline
Plane<T>::
Plane(const Point& point, const Point& normal) : 
  _point(point), 
  _normal(normal) {
#ifdef DEBUG_Plane
  assert(isValid());
#endif
}


template<typename T>
inline
Plane<T>::
Plane(const Point& a, const Point& b, const Point& c) :
  _point(), 
  _normal() {
  make(a, b, c);
}


template<typename T>
inline
void
Plane<T>::
make(const Point& a, const Point& b, const Point& c) {
  _point = b;
  computeCrossProduct(Point(c - b), Point(a - b), &_normal);
  normalize(&_normal);
#ifdef DEBUG_Plane
  assert(isValid());
#endif
}


template<typename T>
inline
Plane<T>::
Plane(const Plane& other) : 
  _point(other._point), 
  _normal(other._normal)
{}


template<typename T>
inline
Plane<T>&
Plane<T>::
operator=(const Plane& other) {
  // Avoid assignment to self
  if (&other != this) {
    _point = other._point;
    _normal = other._normal;
  }
  // Return *this so assignments can chain
  return *this;
}


//
// Validity checking
//

//! Return true if the plane is valid.
template<typename T>
inline
bool 
Plane<T>::
isValid() const { 
  return (std::abs(computeSquaredMagnitude(_normal) - 1) < 
	  10 * std::numeric_limits<Number>::epsilon() ?
	  true : false);
}

//
// Mathematical member functions
//

// distance to the plane.
template<typename T>
inline
T 
Plane<T>::
computeSignedDistance(const Point& p) const {
  return computeDotProduct(p - getPointOn(), getNormal());
}


// distance and closest Point to the plane
template<typename T>
inline
T 
Plane<T>::
computeSignedDistanceAndClosestPoint(const Point& x, 
				     Point* closestPoint) const {
  Number dist = computeSignedDistance(x);
  //*closestPoint = x - dist * getNormal();
  *closestPoint = getNormal();
  *closestPoint *= - dist;
  *closestPoint += x;
  return dist;
}


//
// File IO
//


template<typename T>
inline
std::istream& 
operator>>(std::istream& in, Plane<T>& x) {
  typename Plane<T>::Point point, normal;
  in >> point >> normal;
  x.make(point, normal);
  return in;
}


template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Plane<T>& x) {
  return out << x.getPointOn() << "\n" << x.getNormal();
}

END_NAMESPACE_GEOM
