// -*- C++ -*-

#if !defined(__geom_Segment_h__)
#define __geom_Segment_h__

#include "Point.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_Segment)
#define DEBUG_Segment
#endif

BEGIN_NAMESPACE_GEOM

//! A segment in N dimensional space.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  A segment is an ordered double of points.
*/
template<int N, typename T = double>
class Segment {
  //
  // Public types.
  //

public:
    
  //! The number type.
  typedef T Number;
  //! The representation of a point.
  typedef ads::FixedArray<N,T> Point;

  //
  // Data
  //

private:
    
  Point _source, _target;
    
public:
    
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  Segment() :
    _source(),
    _target() 
  {}

  //! Construct from two points.
  Segment(const Point& source, const Point& target) : 
    _source(source), 
    _target(target) 
  {}

  //! Copy constructor.
  Segment(const Segment& other) : 
    _source(other._source), 
    _target(other._target) 
  {}

  //! Assignment operator.
  Segment& 
  operator=(const Segment& other);

  //! Trivial destructor.
  ~Segment() 
  {}

  //! Make from two points.
  void 
  make(const Point& source, const Point& target) { 
    _source = source; 
    _target = target; 
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the first point of the line segment.
  const Point& 
  getSource() const { 
    return _source; 
  }

  //! Return the second point of the line segment.
  const Point& 
  getTarget() const { 
    return _target; 
  }
    
  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  // CONTINUE
#if 0
  //! Set the first point of the line segment.
  void
  setSource(const Point& source) { 
    _source = source; 
  }

  //! Set the second point of the line segment.
  void
  setTarget(const Point& target) { 
    _target = target; 
  }
#endif

  // @}
  //--------------------------------------------------------------------------
  //! \name Translations.
  // @{

  //! Translate by p.
  Segment& 
  operator+=(const Point& p);

  //! Translate by -p.
  Segment& 
  operator-=(const Point& p);

  // @}
};

  
//
// Equality Operators.
//


//! Return true if the segments are equal.
/*! \relates Segment */
template<int N, typename T>
inline
bool 
operator==(const Segment<N,T>& x, const Segment<N,T>& y) {
  return (x.getSource() == y.getSource() && x.getTarget() == y.getTarget());
}


//! Return true if the segments are not equal.
/*! \relates Segment */
template<int N, typename T>
inline
bool 
operator!=(const Segment<N,T>& x, const Segment<N,T>& y) {
  return !(x == y);
}


//
// Unary Operators.
//


//! Return the segment.
/*! \relates Segment */
template<int N, typename T>
inline
const Segment<N,T>&
operator+(const Segment<N,T>& x) {
  return x;
}


//! Return a segment with the opposite orientation.
/*! \relates Segment */
template<int N, typename T>
inline
Segment<N,T> 
operator-(const Segment<N,T>& x) {
  return Segment<N,T>(x.getTarget(), x.getSource());
}


//
// Binary Operators.
//


//! Return a segment translated by p.
/*! \relates Segment */
template<int N, typename T>
inline
Segment<N,T> 
operator+(const Segment<N,T>& s, const typename Segment<N,T>::Point& p) {
  return Segment<N,T>(s.getSource() + p, s.getTarget() + p);
}


//! Return a segment translated by -p.
/*! \relates Segment */
template<int N, typename T>
inline
Segment<N,T> 
operator-(const Segment<N,T>& s, const typename Segment<N,T>::Point& p) {
  return Segment<N,T>(s.getSource() - p, s.getTarget() - p);
}


//
// File I/O Operators.
//


//! Read a segment.
/*! \relates Segment */
template<int N, typename T>
inline
std::istream& 
operator>>(std::istream& in, Segment<N,T>& x) {
  typename Segment<N,T>::Point source, target;
  in >> source >> target;
  x.make(source, target);
  return in;
}


//! Write the segment.
/*! \relates Segment */
template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Segment<N,T>& x)
{
  return out << x.getSource() << " " << x.getTarget();
}


END_NAMESPACE_GEOM

#define __geom_Segment_ipp__
#include "Segment.ipp"
#undef __geom_Segment_ipp__
  
#endif
