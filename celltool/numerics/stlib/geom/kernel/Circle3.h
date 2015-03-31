// -*- C++ -*-

#if !defined(__geom_kernel_Circle3_h__)
#define __geom_kernel_Circle3_h__

#include "Point.h"
#include "SegmentMath.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_Circle3)
#define DEBUG_Circle3
#endif

BEGIN_NAMESPACE_GEOM

//! A circle in 3-dimensional space.
/*!
  \param T is the number type.  By default it is double.

  A circle in 3-D is defined by a center, a normal, and a radius.
*/
template<typename T = double>
class Circle3 {
  //
  // Types
  //

public:
    
  //! The number type.
  typedef T Number;
  //! The representation of a point.
  typedef ads::FixedArray<3,Number> Point;

  //
  // Data
  //

private:
    
  Point _center;
  Point _normal;
  Number _radius;
    
public:
    
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  Circle3() 
  {}

  //! Construct from the center, normal, and radius.
  Circle3(const Point& center, const Point& normal, const Number radius) : 
    _center(center),
    _normal(normal),
    _radius(radius) 
  {}

  //! Copy constructor.
  Circle3(const Circle3& other) :
    _center(other._center),
    _normal(other._normal),
    _radius(other._radius) 
  {}

  //! Assignment operator.
  Circle3& 
  operator=(const Circle3& other);

  //! Trivial destructor.
  ~Circle3() 
  {}

  //! Make from the center, normal, and radius.
  void 
  make(const Point& center, const Point& normal, const Number radius) { 
    _center = center;
    _normal = normal;
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

  //! Return the normal.
  const Point&
  getNormal() const { 
    return _normal; 
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

  //! Set the normal.
  void
  setNormal(const Point& normal) { 
    _normal = normal;
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
  Circle3& 
  operator+=(const Point& x) {
    _center += x;
    return (*this);
  }

  //! Translate by -p.
  Circle3& 
  operator-=(const Point& x) {
    _center -= x;
    return (*this);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Validity.
  //@{
  
  //! Return true if the circle is valid.
  bool
  isValid() const;
  
  //@}
};

//
// Mathematical functions.
//

//! Compute the closest point on the circle.
template<typename T>
void 
computeClosestPoint(const Circle3<T>& circle, 
		    typename Circle3<T>::Point x,
		    typename Circle3<T>::Point* closestPoint);

//! Compute the closest point on the circle to the edge.
template<typename T>
void 
computeClosestPoint(const Circle3<T>& circle, 
		    const typename Circle3<T>::Point& source,
		    const typename Circle3<T>::Point& target,
		    typename Circle3<T>::Point* closestPoint,
		    T tolerance = std::sqrt(std::numeric_limits<T>::epsilon()),
		    int maximumSteps = 10);

//
// Equality Operators.
//

//! Return true if the balls are equal.
/*! \relates Circle3 */
template<typename T>
inline
bool 
operator==(const Circle3<T>& x, const Circle3<T>& y) {
  return (x.getCenter() == y.getCenter() && x.getNormal() == y.getNormal() && 
	  x.getRadius() == y.getRadius() );
}


//! Return true if the balls are not equal.
/*! \relates Circle3 */
template<typename T>
inline
bool 
operator!=(const Circle3<T>& x, const Circle3<T>& y) {
  return !(x == y);
}


//
// Binary Operators.
//

//! Return a circle translated by the vector.
/*! \relates Circle3 */
template<typename T>
inline
Circle3<T> 
operator+(const Circle3<T>& circle, const typename Circle3<T>::Point& vector) {
  return Circle3<T>(circle.getCenter() + vector, circle.getNormal(),
		    circle.getRadius());
}

//! Return a circle translated by the negative of the vector.
/*! \relates Circle3 */
template<typename T>
inline
Circle3<T> 
operator-(const Circle3<T>& circle, const typename Circle3<T>::Point& vector) {
  return Circle3<T>(circle.getCenter() - vector, circle.getNormal(),
		    circle.getRadius());
}


//
// File I/O Operators.
//

//! Read a circle.
/*! \relates Circle3 */
template<typename T>
std::istream& 
operator>>(std::istream& in, Circle3<T>& x);

//! Write the circle.
/*! \relates Circle3 */
template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Circle3<T>& x) {
  return out << x.getCenter() << " " << x.getNormal() << " " << x.getRadius();
}


END_NAMESPACE_GEOM

#define __geom_kernel_Circle3_ipp__
#include "Circle3.ipp"
#undef __geom_kernel_Circle3_ipp__
  
#endif
