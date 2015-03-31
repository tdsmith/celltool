// -*- C++ -*-

#if !defined(__geom_kernel_CircularArc3_ipp__)
#error This file is an implementation detail of the class CircularArc3.
#endif

BEGIN_NAMESPACE_GEOM


//
// Constructors
//

  
template<typename T>
inline
CircularArc3<T>& 
CircularArc3<T>::
operator=(const CircularArc3& other) {
  // Avoid assignment to self
  if (&other != this) {
    _circle = other._circle;
    _axis0 = other._axis0;
    _axis1 = other._axis1;
    _angle = other._angle;
  }
  // Return *this so assignments can chain.
  return *this;
}


// Make from the center, the source, and the target.
template<typename T>
inline
void 
CircularArc3<T>::
make(const Point& center, const Point& source, const Point& target) {
  // Use the source and the center to get the first axis.
  _axis0 = source;
  _axis0 -= center;
  normalize(&_axis0);

  // Use the target and the center to get an independent direction in the 
  // plane.
  Point direction = target;
  direction -= center;
  normalize(&direction);

  // Use the source direction and the indepentent direction to compute 
  // the angle.
  // _axis0 . direction = cos(_angle)
  _angle = std::acos(computeDotProduct(_axis0, direction));

  // Use the first axis and the direction to compute the normal to the 
  // plane of the circular arc.
  Point normal;
  computeCrossProduct(_axis0, direction, &normal);
  normalize(&normal);

  // Use the first axis and the normal to compute the second axis.
  computeCrossProduct(normal, _axis0, &_axis1);
  normalize(&_axis1);

  // The circle center.
  _circle.setCenter(center);
  // The circle normal.
  _circle.setNormal(normal);
  // The circle radius.
  _circle.setRadius(computeDistance(center, source));
  // CONTINUE: This check is ad-hoc.
  const Number r = computeDistance(center, target);
  const Number Epsilon = std::sqrt(std::numeric_limits<Number>::epsilon());
  if (r < Epsilon) {
    // Use the difference.
    assert(std::abs(_circle.getRadius() - r) < Epsilon);
  }
  else {
    // Use the relative difference.
    assert(std::abs(_circle.getRadius() - r) / r < Epsilon);
  }
}


//
// Validity.
//


// Return true if the circle is valid.
template<typename T>
inline
bool
CircularArc3<T>::
isValid() const {
  if (! _circle.isValid()) {
    return false;
  }

  // The arc must be between a point and a circle.
  if (_angle <= 0 || _angle > 2.0 * numerical::Constants<Number>::Pi() * 
      (1.0 + 10.0 * std::numeric_limits<T>::epsilon())) {
    return false;
  }
  
  // If the axes are not of unit length.
  if (std::abs(computeMagnitude(_axis0) - 1.0) > 
      10.0 * std::numeric_limits<T>::epsilon()) {
    return false;
  }
  if (std::abs(computeMagnitude(_axis1) - 1.0) > 
      10.0 * std::numeric_limits<T>::epsilon()) {
    return false;
  }

  // Otherwise, the circle is valid.
  return true;
}


//
// Mathematical functions.
//


// Compute the closest point on the circular arc.
template<typename T>
inline
void 
computeClosestPoint(const CircularArc3<T>& circularArc, 
		    typename CircularArc3<T>::Point x,
		    typename CircularArc3<T>::Point* closestPoint) {
  typedef typename CircularArc3<T>::Point Point;

  // First compute the closest point on the circle.
  computeClosestPoint(circularArc.getCircle(), x, closestPoint);

  // Convert to the coordinates in the axes of the plane.
  Point p(*closestPoint);
  p -= circularArc.getCenter();
  p /= circularArc.getRadius();
  const T x0 = computeDotProduct(circularArc.getFirstAxis(), p);
  const T x1 = computeDotProduct(circularArc.getSecondAxis(), p);
  // Compute the angle.
  const T theta = std::atan2(x1, x0);

  // If the closest point is on the circular arc.
  if (0 <= theta && theta <= circularArc.getAngle()) {
    // We are done.  Return with the current closest point.
    return;
  }

  // Check out the endpoints.
  const Point source = circularArc(0.0);
  const Point target = circularArc(1.0);
  if (geom::computeDistance(x, source) < geom::computeDistance(x, target)) {
    *closestPoint = source;
  }
  else {
    *closestPoint = target;
  }
}


// Compute the closest point on the circle to the edge.
template<typename T>
inline
void 
computeClosestPoint(const Circle3<T>& circle, 
		    const CircularArc3<T>& circularArc,
		    typename CircularArc3<T>::Point* closestPoint,
		    const T tolerance, const int maximumSteps) {
  typedef typename Circle3<T>::Point Point;

  // Start with the mid-point of the circular arc..
  Point pointOnArc = circularArc(0.5);

  // Compute an initial closest point on the circle.
  computeClosestPoint(circle, pointOnArc, closestPoint);

  // Iterate computing the closest point until we achieve convergence.
  Point pointOnCircle;
  // We have taken one step so far.
  int numberOfSteps = 1;
  do {
    // Increment the number of steps.
    ++numberOfSteps;
    // Record the old point on the circle.
    pointOnCircle = *closestPoint;
    // Compute the closest point on the circular arc.
    computeClosestPoint(circularArc, pointOnCircle, &pointOnArc);
    // Compute the closest point on the circle to the point on the arc.
    computeClosestPoint(circle, pointOnArc, closestPoint);
  } while(numberOfSteps < maximumSteps &&
	  computeDistance(pointOnCircle, *closestPoint) * circle.getRadius()
	  > tolerance);
  // CONTINUE REMOVE
#if 0
  std::cout << "compute closest point: circle and circular arc.\n"
	    << "circle = " << circle << "\n"
	    << "circular arc = " << circularArc << "\n"
	    << "numberOfSteps = " << numberOfSteps << "\n" 
	    << "distance = " << computeDistance(pointOnArc, *closestPoint)
	    << "\n"
	    << "closest point = " << *closestPoint << "\n"
	    << "point on arc = " << pointOnArc << "\n"
	    << "error = " << (computeDistance(pointOnCircle, *closestPoint) * 
			      circle.getRadius()) << "\n\n";
#endif
}


//
// File I/O Operators.
//


// Read a circular arc.
template<typename T>
inline
std::istream& 
operator>>(std::istream& in, CircularArc3<T>& x) {
  typedef typename CircularArc3<T>::Point Point;

  Point center, source, target;
  in >> center >> source >> target;
  x.make(center, source, target);
  return in;
}

END_NAMESPACE_GEOM
