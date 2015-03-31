// -*- C++ -*-

/*! 
  \file Plane.h
  \brief Implements a class for a plane in 3 dimensions.
*/

#if !defined(__geom_Plane_h__)
#define __geom_Plane_h__

#include "Point.h"

#include <cmath>

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_Plane)
#define DEBUG_Plane
#endif

BEGIN_NAMESPACE_GEOM

//! A plane in 3 dimensions.
/*!
  \param T is the number type.  By default it is double.
*/
template<typename T = double>
class Plane {
  //
  // Public types.
  //

public:

  //! The number type.
  typedef T Number;
  //! The representation for a point.
  typedef ads::FixedArray<3,T> Point;
    
private:

  Point _point, _normal;
  
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  Plane();

  //! Construct from a point on the plane and a normal to the plane.
  Plane(const Point& point, const Point& normal);

  //! Construct from three points on the plane.
  Plane(const Point& a, const Point& b, const Point& c);

  //! Make from three points on the plane.
  void
  make(const Point& a, const Point& b, const Point& c);
  
  //! Copy constructor.
  Plane(const Plane<T>& other);

  //! Assignment operator.
  Plane& 
  operator=(const Plane& other);

  //! Trivial destructor.
  ~Plane() 
  {}

  //  int make (const Point& a, const Point& b, const Point& c);

  // @}  
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return a point on the plane.
  const Point& 
  getPointOn() const { 
    return _point; 
  }

  //! Return the normal to the plane.
  const Point& 
  getNormal() const { 
    return _normal; 
  }

  // @}  
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  // CONTINUE
#if 0
  //! Return a point on the plane.
  Point& 
  point() 
  { 
    return _point; 
  }

  //! Return the normal to the plane.
  Point& 
  normal() 
  { 
    return _normal; 
  }
#endif

  // @}  
  //--------------------------------------------------------------------------
  //! \name Validity checking.
  // @{

  //! Return true if the plane is valid.
  bool 
  isValid() const;

  // @}  
  //--------------------------------------------------------------------------
  //! \name Arithmetic operators.
  // @{

  //! Translate the plane by +p.
  Plane& 
  operator+=(const Point& p) {
    _point += p;
    return *this;
  }

  //! Translate the plane by -p.
  Plane& 
  operator-=(const Point& p) {
    _point -= p;
    return *this;
  }

  // @}  
  //--------------------------------------------------------------------------
  //! \name Mathematical functions.
  // @{

  //! Return the distance from p to the plane.
  T 
  computeSignedDistance(const Point& p) const;

  //! Return distance from p to the plane.  Set cp to be the closest point.
  T 
  computeSignedDistanceAndClosestPoint(const Point& x, 
				       Point* closestPoint) const;

  // @}
};


//
// Unary Operators
//


//! The positive operator.  Return the same plane.
/*! \relates Plane */
template<typename T>
inline
const Plane<T>&
operator+(const Plane<T>& x) {
  return x;
}


//! The negative operator.  Return the plane with opposite orientation.
/*! \relates Plane */
template<typename T>
inline
Plane<T> 
operator-(const Plane<T>& x) {
  return Plane<T>(x.getPointOn(), - x.getNormal());
}


//
// Equality Operators
//


//! Return true if the planes are equal.
/*! \relates Plane */
template<typename T>
inline
bool 
operator==(const Plane<T>& x, const Plane<T>& y) { 
  return (x.getPointOn() == y.getPointOn() && 
	  x.getNormal() == y.getNormal()); 
}


//! Return true if the planes are not equal.
/*! \relates Plane */
template<typename T>
inline
bool 
operator!=(const Plane<T>& x, const Plane<T>& y) { 
  return !(x == y); 
}


//
// File I/O
//


//! Read the point and normal.
/*! \relates Plane */
template<typename T>
std::istream& 
operator>>(std::istream& in, Plane<T>& x);


//! Write the point and normal.
/*! \relates Plane */
template<typename T>
std::ostream& 
operator<<(std::ostream& out, const Plane<T>& x);


END_NAMESPACE_GEOM

#define __geom_Plane_ipp__
#include "Plane.ipp"
#undef __geom_Plane_ipp__

#endif
