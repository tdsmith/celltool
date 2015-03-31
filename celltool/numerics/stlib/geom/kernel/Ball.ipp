// -*- C++ -*-

#if !defined(__geom_kernel_Ball_ipp__)
#error This file is an implementation detail of the class Ball.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//
  
template<int N, typename T>
inline
Ball<N,T>& 
Ball<N,T>::
operator=(const Ball& other) {
  // Avoid assignment to self
  if (&other != this) {
    _center = other._center;
    _radius = other._radius;
  }
  // Return *this so assignments can chain.
  return *this;
}

//
// File I/O Operators.
//

//! Read a ball.
/*! \relates Ball */
template<int N, typename T>
inline
std::istream& 
operator>>(std::istream& in, Ball<N,T>& x) {
  typename Ball<N,T>::Point center;
  T radius;
  in >> center >> radius;
  x.setCenter(center);
  x.setRadius(radius);
  return in;
}

END_NAMESPACE_GEOM
