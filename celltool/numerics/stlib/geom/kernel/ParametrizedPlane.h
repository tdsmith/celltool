// -*- C++ -*-

#if !defined(__geom_ParametrizedPlane_h__)
#define __geom_ParametrizedPlane_h__

#include <iostream>
#include <cassert>
#include <cmath>

#include "SegmentMath.h"

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_ParametrizedPlane)
#define DEBUG_ParametrizedPlane
#endif

BEGIN_NAMESPACE_GEOM

//! A parametrized plane in N-D.
/*!
  \param N is the space dimension.
  \param T is the number type.  By default it is double.
 */
template<int N, typename T = double>
class ParametrizedPlane {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;
  //! The Cartesian point type.
  typedef ads::FixedArray<N,Number> Point;
  //! The parameter point type.
  typedef ads::FixedArray<2,Number> ParameterPoint;

private:

  //
  // Member data
  //

  Point _origin, _axis0, _axis1;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.  Uninitialized memory.
  ParametrizedPlane()
  {}

  //! Construct from points.
  ParametrizedPlane(const Point& p0, const Point& p1, const Point& p2) : 
    _origin(p0),
    _axis0(p1 - p0),
    _axis1(p2 - p0) {
    assert(isValid());
  }

  //! Make from points.
  void
  build(const Point& p0, const Point& p1, const Point& p2) {
    _origin = p0;
    _axis0 = p1 - p0;
    _axis1 = p2 - p0;
    assert(isValid());
  }

  //! Copy constructor.
  ParametrizedPlane(const ParametrizedPlane& other) : 
    _origin(other._origin),
    _axis0(other._axis0),
    _axis1(other._axis1)
  {}

  //! Assignment operator.
  ParametrizedPlane& 
  operator=(const ParametrizedPlane& other) {
    if (this != &other) {
      _origin = other._origin;
      _axis0 = other._axis0;
      _axis1 = other._axis1;
    }
    return *this;
  }

  //! Trivial destructor.
  ~ParametrizedPlane()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  //@{

  //! Compute a point on the plane from parameter values.
  Point 
  computePosition(const ParameterPoint& parameterPoint) const {
    return _origin + parameterPoint[0] * _axis0 + parameterPoint[1] * _axis1;
  }

  //! Compute the derivative of position with respect to the parametrization.
  void
  computeDerivative(const ParameterPoint& parameterPoint,
		    Point* dxds, Point* dxdt) const {
    *dxds = _axis0;
    *dxdt = _axis1;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality operators.
  //@{

  //! Return true if the lines are equal.
  bool
  operator==(const ParametrizedPlane& other) {
    return _origin == other._origin && _axis0 == other._axis0 && 
      _axis1 == other._axis1;
  }


  //! Return true if the lines are not equal.
  bool
  operator!=(const ParametrizedPlane& other) {
    return ! operator==(other);
  }

  //@}
private:
  //--------------------------------------------------------------------------
  // Private member functions.
  
  // Check that the lengths are non-zero and that the axis are linearly 
  // independent.
  bool
  isValid() {
    const Number m0 = computeMagnitude(_axis0);
    const Number m1 = computeMagnitude(_axis1);
    // If either axis has zero length.
    if (m0 == 0 || m1 == 0) {
      return false;
    }
    // If the axes are linearly dependent.
    if (geom::computeMagnitude((_axis0 - (m0 / m1) * _axis1) / m0) < 
	std::sqrt(std::numeric_limits<Number>::epsilon())) {
      return false;
    }
    return true;
  }
};


END_NAMESPACE_GEOM

#endif
