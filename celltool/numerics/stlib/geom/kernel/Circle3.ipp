// -*- C++ -*-

#if !defined(__geom_kernel_Circle3_ipp__)
#error This file is an implementation detail of the class Circle3.
#endif

BEGIN_NAMESPACE_GEOM


//
// Constructors
//

  
template<typename T>
inline
Circle3<T>& 
Circle3<T>::
operator=(const Circle3& other) {
  // Avoid assignment to self
  if (&other != this) {
    _center = other._center;
    _normal = other._normal;
    _radius = other._radius;
  }
  // Return *this so assignments can chain.
  return *this;
}


//
// Validity.
//


// Return true if the circle is valid.
template<typename T>
inline
bool
Circle3<T>::
isValid() const {
  // If the radius is negative.
  if (_radius < 0) {
    // The circle is not valid.
    return false;
  }
  // If the normal is not of unit length.
  if (std::abs(computeMagnitude(_normal) - 1.0) > 
      10.0 * std::numeric_limits<T>::epsilon()) {
    // The circle is not valid.
    return false;
  }
  // Otherwise, the circle is valid.
  return true;
}


//
// Mathematical functions.
//


// Compute the closest point on the circle.
template<typename T>
inline
void 
computeClosestPoint(const Circle3<T>& circle, 
		    typename Circle3<T>::Point x,
		    typename Circle3<T>::Point* closestPoint) {
  typedef typename Circle3<T>::Point Point;

  // The vector between the point and the circle center.
  Point vec = x;
  vec -= circle.getCenter();
  // Move the point into the plane of the circle.
  x -= geom::computeDotProduct(vec, circle.getNormal()) * circle.getNormal();
  // The vector between the point in the plane and the circle center.
  vec = x;
  vec -= circle.getCenter();

  // Deal with vec near zero length.
  if (computeMagnitude(vec) < 10.0 * std::numeric_limits<T>::epsilon()) {
    computeAnOrthogonalVector(circle.getNormal(), &vec);
  }

  // Change the vector length to the circle radius.
  normalize(&vec);
  vec *= circle.getRadius();
  // The closest point is center + vec.
  *closestPoint = circle.getCenter();
  *closestPoint += vec;
}


// Compute the closest point on the circle to the edge.
template<typename T>
inline
void 
computeClosestPoint(const Circle3<T>& circle, 
		    const typename Circle3<T>::Point& source,
		    const typename Circle3<T>::Point& target,
		    typename Circle3<T>::Point* closestPoint,
		    T tolerance, int maximumSteps) {
  typedef typename Circle3<T>::Point Point;

  // Make a line segment from the endpoints.
  SegmentMath<3,T> segment(source, target);

  // Start with the mid-point of the line segment.
  Point pointOnSegment = source;
  pointOnSegment = target;
  pointOnSegment *= 0.5;

  // Compute an initial closest point on the circle.
  computeClosestPoint(circle, pointOnSegment, closestPoint);

  // Iterate computing the closest point until we achieve convergence.
  Point pointOnCircle;
  // We have taken one step so far.
  int numberOfSteps = 1;
  do {
    // Increment the number of steps.
    ++numberOfSteps;
    // Record the old point on the circle.
    pointOnCircle = *closestPoint;
    // Compute the closest point on the line segment.
    computeClosestPoint(segment, pointOnCircle, &pointOnSegment);
    // Compute the closest point on the circle to the point on the segment.
    computeClosestPoint(circle, pointOnSegment, closestPoint);
  } while(numberOfSteps < maximumSteps &&
	  computeDistance(pointOnCircle, *closestPoint) * circle.getRadius()
	  > tolerance);
  // CONTINUE REMOVE
#if 0
  std::cout << "compute closest point: circle and line segment.\n"
	    << "circle = " << circle << "\n"
	    << "source = " << source << ", target = " << target << "\n"
	    << "numberOfSteps = " << numberOfSteps << "\n" 
	    << "distance = " << computeDistance(pointOnSegment, *closestPoint)
	    << "\n"
	    << "closest point = " << *closestPoint << "\n"
	    << "point on segment = " << pointOnSegment << "\n"
	    << "error = " << (computeDistance(pointOnCircle, *closestPoint) * 
			      circle.getRadius()) << "\n\n";
#endif
}


//
// File I/O Operators.
//


// Read a circle.
template<typename T>
inline
std::istream& 
operator>>(std::istream& in, Circle3<T>& x) {
  typename Circle3<T>::Point point;

  in >> point;
  x.setCenter(point);

  in >> point;
  x.setNormal(point);

  T radius;
  x.setRadius(radius);

  return in;
}

END_NAMESPACE_GEOM
