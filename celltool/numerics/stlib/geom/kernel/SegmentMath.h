// -*- C++ -*-

#if !defined(__geom_SegmentMath_h__)
#define __geom_SegmentMath_h__

#include "Segment.h"
#include "content.h"

#include <iosfwd>
#include <cmath>

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_SegmentMath)
#define DEBUG_SegmentMath
#endif

BEGIN_NAMESPACE_GEOM

//! A segment in N dimensional space designed for doing math operations.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  A segment is an ordered doublet of points.  This class stores the length
  of the segment and its tangent.
*/
template<int N, typename T = double>
class SegmentMath : 
  public Segment<N,T> {
  //
  // Private types.
  //

private:

  typedef Segment<N,T> Base;

public:

  //! The number type.
  typedef typename Base::Number Number;
  //! The point type.
  typedef typename Base::Point Point;
      
private:
    
  Point _tangent;
  T _length;
    
public:
    
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  SegmentMath() :
    Base(),
    _tangent(),
    _length() 
  {}

  //! Construct from two points.
  SegmentMath(const Point& source, const Point& target);

  //! Construct from a Segment
  SegmentMath(const Segment<N,T>& s)
    : SegmentMath(s.source(), s.target()) 
  {}

  //! Copy constructor.
  SegmentMath(const SegmentMath& other) : 
    Segment<N,T>(other.getSource(), other.getTarget()), 
    _tangent(other._tangent),
    _length(other._length) 
  {}

  //! Assignment operator.
  SegmentMath& 
  operator=(const SegmentMath& other);

  //! Trivial destructor.
  ~SegmentMath() 
  {}

  //! Make from two points.
  void 
  make(const Point& source, const Point& target);

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //
  // Inherited from Segment.
  //

  //! Return the first point of the line segment.
  using Base::getSource;

  //! Return the second point of the line segment.
  using Base::getTarget;
    
  //
  // New.
  //

  //! Return the unit tangent to the line segment.
  const Point& 
  getTangent() const { 
    return _tangent; 
  }

  //! Return the length of the line segment.
  T 
  getLength() const { 
    return _length; 
  }
    
  // @}
  //--------------------------------------------------------------------------
  //! \name Translations.
  // @{

  //! Translate by p.
  SegmentMath& 
  operator+=(const Point& p);

  //! Translate by -p.
  SegmentMath& 
  operator-=(const Point& p);

  // @}
  //--------------------------------------------------------------------------
  //! \name Validity.
  // @{

  //! Return true if the segment is valid.
  bool 
  isValid() const;

  // @}
};
  
  
//
// Distance.
//

//
// Unary Operators.
//


//! Return the segment.
/*! \relates SegmentMath */
template<int N, typename T>
inline
const SegmentMath<N,T>&
operator+(const SegmentMath<N,T>& x) {
  return x;
}


//! Return a reversed segment.
/*! \relates SegmentMath */
template<int N, typename T>
inline
SegmentMath<N,T> 
operator-(const SegmentMath<N,T>& x) {
  return SegmentMath<N,T>(x.getTarget(), x.getSource());
}


//
// Binary Operators.
//


//! Translate by p.
/*! \relates SegmentMath */
template<int N, typename T>
SegmentMath<N,T> 
operator+(const SegmentMath<N,T>& s, 
	  const typename SegmentMath<N,T>::Point& p);


//! Translate by -p.
/*! \relates SegmentMath */
template<int N, typename T>
SegmentMath<N,T> 
operator-(const SegmentMath<N,T>& s, 
	  const typename SegmentMath<N,T>::Point& p);


//
// Equality Operators.
//


//! Return true if the segments are equal.
/*! \relates SegmentMath */
template<int N, typename T>
inline 
bool 
operator==(const SegmentMath<N,T>& x, const SegmentMath<N,T>& y) {
  return (Segment<N,T>(x) == Segment<N,T>(y) &&
	   x.getTangent() == y.getTangent() && x.getLength() == y.getLength());
}


//! Return true if the segments are not equal.
/*! \relates SegmentMath */
template<int N, typename T>
inline 
bool 
operator!=(const SegmentMath<N,T>& x, const SegmentMath<N,T>& y) {
  return !(x == y);
}


//
// Mathematical Functions.
//


//! Compute the unsigned distance to the line segment.
/*! \relates SegmentMath */
template<int N, typename T>
T 
computeDistance(const SegmentMath<N,T>& segment, 
		const typename SegmentMath<N,T>::Point& x);


//! Compute closest point on the line segment.
/*! \relates SegmentMath */
template<int N, typename T>
void
computeClosestPoint(const SegmentMath<N,T>& segment, 
		    const typename SegmentMath<N,T>::Point& x, 
		    typename SegmentMath<N,T>::Point* closestPoint);


//! Compute the unsigned distance to the line segment and the closest point on it.
/*! \relates SegmentMath */
template<int N, typename T>
T 
computeDistanceAndClosestPoint(const SegmentMath<N,T>& segment, 
			       const typename SegmentMath<N,T>::Point& x, 
			       typename SegmentMath<N,T>::Point* closestPoint);


//! Compute the unsigned distance to the supporting line of the segment.
/*! \relates SegmentMath */
template<int N, typename T>
T 
computeUnsignedDistanceToSupportingLine
(const SegmentMath<N,T>& segment, 
 const typename SegmentMath<N,T>::Point& x);


//! Compute the unsigned distance to the supporting line of the segment and the closest point on the line.
/*! \relates SegmentMath */
template<int N, typename T>
T 
computeUnsignedDistanceAndClosestPointToSupportingLine
(const SegmentMath<N,T>& segment, const typename SegmentMath<N,T>::Point& x, 
 typename SegmentMath<N,T>::Point* closestPoint);


//! Return true if the segment intersects the plane of constant z.
/*!
  \relates SegmentMath
  Set x and y to the point of intersection.
*/
template<typename T>
bool 
computeZIntersection(const SegmentMath<3,T>& segment, T* x, T* y, T z);

//! If the two Segments intersect, return true and the point of intersection. Otherwise return false.
/*! \relates SegmentMath */
template<typename T>
bool
computeIntersection(const SegmentMath<2,T>& s1, const SegmentMath<2,T>& s2,
		    typename SegmentMath<2,T>::Point* intersectionPoint);


//
// File I/O Operators.
//


//! File input.
/*! \relates SegmentMath */
template<int N, typename T>
std::istream& 
operator>>(std::istream& in, SegmentMath<N,T>& s);


//! File output.
/*! \relates SegmentMath */
template<int N, typename T>
std::ostream& 
operator<<(std::ostream& out, const SegmentMath<N,T>& s);


END_NAMESPACE_GEOM

#define __geom_SegmentMath_ipp__
#include "SegmentMath.ipp"
#undef __geom_SegmentMath_ipp__
  
#endif
