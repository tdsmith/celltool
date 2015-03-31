// -*- C++ -*-

#if !defined(__geom_kernel_Ball_h__)
#define __geom_kernel_Ball_h__

#include "Point.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_Ball)
#define DEBUG_Ball
#endif

BEGIN_NAMESPACE_GEOM

//! A ball in N dimensional space.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  A ball is defined by a center and a radius.
*/
template<int N, typename T = double>
class Ball {
  //
  // Types
  //

public:
    
  //! The number type.
  typedef T Number;
  //! The representation of a point.
  typedef ads::FixedArray<N,Number> Point;

  //
  // Data
  //

private:
    
  Point _center;
  Number _radius;
    
public:
    
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  Ball() 
  {}

  //! Construct from a center and radius.
  Ball(const Point& center, const Number radius) : 
    _center(center),
    _radius(radius) 
  {}

  //! Copy constructor.
  Ball(const Ball& other) :
    _center(other._center),
    _radius(other._radius) 
  {}

  //! Assignment operator.
  Ball& 
  operator=(const Ball& other);

  //! Trivial destructor.
  ~Ball() 
  {}

  //! Make from a center and radius.
  void 
  make(const Point& center, const Number radius) { 
    _center = center;
    _radius = radius;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the center.
  const Point&
  getCenter() const { 
    return _center; 
  }

  //! Return the radius.
  Number
  getRadius() const { 
    return _radius; 
  }
    
  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Set the center.
  void
  setCenter(const Point& center) { 
    _center = center;
  }

  //! Set the radius.
  void
  setRadius(const Number radius) { 
    _radius = radius;
  }
    
  // @}
  //--------------------------------------------------------------------------
  //! \name Translations.
  // @{

  //! Translate by p.
  Ball& 
  operator+=(const Point& x) {
    _center += x;
    return (*this);
  }

  //! Translate by -p.
  Ball& 
  operator-=(const Point& x) {
    _center -= x;
    return (*this);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions.
  //@{
  
  //! Return true if the point is inside this ball.
  bool
  isInside(const Point& position) const {
    return computeSquaredDistance(_center, position) < _radius * _radius;
  }
  
  //@}
};


//
// Equality Operators.
//

//! Return true if the balls are equal.
/*! \relates Ball */
template<int N, typename T>
inline
bool 
operator==(const Ball<N,T>& x, const Ball<N,T>& y) {
  return (x.getCenter() == y.getCenter() && x.getRadius() == y.getRadius());
}


//! Return true if the balls are not equal.
/*! \relates Ball */
template<int N, typename T>
inline
bool 
operator!=(const Ball<N,T>& x, const Ball<N,T>& y) {
  return !(x == y);
}


//
// Binary Operators.
//

//! Return a ball translated by p.
/*! \relates Ball */
template<int N, typename T>
inline
Ball<N,T> 
operator+(const Ball<N,T>& b, const typename Ball<N,T>::Point& p) {
  return Ball<N,T>(b.getCenter() + p, b.getRadius());
}

//! Return a ball translated by -p.
/*! \relates Ball */
template<int N, typename T>
inline
Ball<N,T> 
operator-(const Ball<N,T>& b, const typename Ball<N,T>::Point& p) {
  return Ball<N,T>(b.getCenter() - p, b.getRadius());
}


//
// File I/O Operators.
//

//! Read a ball.
/*! \relates Ball */
template<int N, typename T>
std::istream& 
operator>>(std::istream& in, Ball<N,T>& x);

//! Write the ball.
/*! \relates Ball */
template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Ball<N,T>& x) {
  return out << x.getCenter() << " " << x.getRadius();
}

END_NAMESPACE_GEOM

#define __geom_kernel_Ball_ipp__
#include "Ball.ipp"
#undef __geom_kernel_Ball_ipp__
  
#endif
