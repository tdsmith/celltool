// -*- C++ -*-

#if !defined(__geom_Triangle_ipp__)
#error This file is an implementation detail of the class Triangle.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//

template<int N, typename T>
inline
const Triangle<N,T>& 
Triangle<N,T>::
operator=(const Triangle& other) {
  // Avoid assignment to self
  if (&other != this){
    _vertices = other._vertices;
  }
  // Return *this so assignments can chain
  return *this;
}


//
// Arithmetic member operators
//


template<int N, typename T>
inline
Triangle<N,T>& 
Triangle<N,T>::
operator+=(const Point& p) {
  _vertices += p;
  return *this;
}


template<int N, typename T>
inline
Triangle<N,T>& 
Triangle<N,T>::
operator-=(const Point& p) {
  _vertices -= p;
  return *this;
}


//
// Mathematical free functions
//


template<typename T>
inline
Plane<T> 
buildSupportingPlane(const Triangle<3,T>& x) {
  return Plane<T>(x.getVertex(0), x.getVertex(1), x.getVertex(2));
}


//
// File IO
//


template<int N, typename T>
inline
std::istream& 
operator>>(std::istream& in, Triangle<N,T>& x) {
  typename Triangle<N,T>::Point point;
  in >> point;
  x.setVertex(0, point);
  in >> point;
  x.setVertex(1, point);
  in >> point;
  x.setVertex(2, point);
  return in;
}


template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Triangle<N,T>& x) {
  return out << x.getVertex(0) << "\n"
	     << x.getVertex(1) << "\n"
	     << x.getVertex(2) << "\n";
}

END_NAMESPACE_GEOM
