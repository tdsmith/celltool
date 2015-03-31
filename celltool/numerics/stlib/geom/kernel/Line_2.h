// -*- C++ -*-

#if !defined(__geom_Line_2_h__)
#define __geom_Line_2_h__

#include <iostream>
#include <cassert>
#include <cmath>

#include "SegmentMath.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_Line_2)
#define DEBUG_Line_2
#endif

BEGIN_NAMESPACE_GEOM

//! A line in 2-D.
/*!
  \param T is the number type.  By default it is double.
 */
template<typename T = double>
class Line_2 {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;
  //! The segment upon which this line is built.
  typedef SegmentMath<2,T> Segment;
  //! The point type.
  typedef typename Segment::Point Point;

private:

  //
  // Member data
  //

  Segment _segment;
  Point _normal;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  Line_2()
  {}

  //! Construct from points.
  Line_2(const Point& source, const Point& target) : 
    _segment(source, target),
    _normal(_segment.getTangent()[1], - _segment.getTangent()[0])
  {}

  //! Make from points.
  void
  make(const Point& source, const Point& target) {
    _segment.make(source, target);
    _normal[0] = _segment.getTangent()[1];
    _normal[1] = - _segment.getTangent()[0];
  }

  //! Construct from a segment.
  Line_2(const Segment& segment) : 
    _segment(segment),
    _normal(_segment.getTangent()[1], - _segment.getTangent()[0])
  {}

  //! Construct from a \c Segment<2,Number>.
  Line_2(const geom::Segment<2,Number>& segment) : 
    _segment(segment),
    _normal(_segment.getTangent()[1], - _segment.getTangent()[0])
  {}

  //! Copy constructor.
  Line_2(const Line_2& other) : 
    _segment(other._segment),
    _normal(other._normal)
  {}

  //! Assignment operator.
  Line_2& 
  operator=(const Line_2& other);

  //! Trivial destructor.
  ~Line_2()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return a point on the line.
  const Point& 
  getPointOn() const {
    return _segment.getSource();
  }

  //! Return the tangent to the line.
  const Point& 
  getTangent() const {
    return _segment.getTangent();
  }

  //! Return the normal to the line.
  const Point& 
  getNormal() const {
    return _normal;
  }
	
  //! Return the segment on which the line is built.
  const Segment& 
  getSegment() const {
    return _segment;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  // CONTINUE
#if 0
  //! Return the segment on which the line is built.
  Segment& 
  segment() {
    return _segment;
  }
#endif

  // @}
  //--------------------------------------------------------------------------
  //! \name Mathematical operations.
  // @{

  //! Translate the line by \c p.
  Line_2& 
  operator+=(const Point& p) {
    _segment += p;
    return *this;
  }

  //! Translate the line by \c -p.
  Line_2& 
  operator-=(const Point& p) {
    _segment -= p;
    return *this;
  }

  //! Distance to the line.
  Number 
  computeSignedDistance(const Point& p) const {
    return computeDotProduct(p - getPointOn(), getNormal());
  }

  //! Distance and closest point to the line.
  Number 
  computeSignedDistanceAndClosestPoint(const Point& p, Point* cp) const
  {
    *cp = getPointOn() + computeDotProduct(p - getPointOn(), 
					   getTangent()) * getTangent();
    return computeDotProduct(p - getPointOn(), getNormal());
  }

  //! Compute the point where the line through \c p1 and \c p2 intersects this line.
  void 
  computeIntersection(Point p1, Point p2, Point* intersectionPoint) const;

  // @}    
};


//
// Mathematical Functions.
//


//! Unary positive operator.
/*! \relates Line_2 */
template<typename T>
inline
const Line_2<T>& 
operator+(const Line_2<T>& x) {
  return x;
}


//! Unary negative operator.
/*! \relates Line_2 */
template<typename T>
inline
Line_2<T>
operator-(const Line_2<T>& x) {
  return Line_2<T>(- x.getSegment());
}


//
// File I/O.
//


//! Read a line.
/*! \relates Line_2 */
template<typename T>
inline
std::istream& 
operator>>(std::istream& in, Line_2<T>& x) {
  typename Line_2<T>::Point p, q;
  in >> p >> q;
  x = Line_2<T>(p, q);
  return in;
}


//! Write a line.
/*! \relates Line_2 */
template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Line_2<T>& x) {
  out << x.getSegment() << x.getNormal() << '\n';
  return out;
}


//
// Equality Operators.
//


//! Return true if the lines are equal.
/*! \relates Line_2 */
template<typename T>
inline
bool
operator==(const Line_2<T>& a, const Line_2<T>& b) {
  return a.getSegment() == b.getSegment();
}


//! Return true if the lines are not equal.
/*! \relates Line_2 */
template<typename T>
inline
bool
operator!=(const Line_2<T>& a, const Line_2<T>& b) {
  return !(a == b);
}


END_NAMESPACE_GEOM

#define __geom_Line_2_ipp__
#include "Line_2.ipp"
#undef __geom_Line_2_ipp__

#endif
