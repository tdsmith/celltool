// -*- C++ -*-

#if !defined(__cpt_Face2_ipp__)
#error This file is an implementation detail of the class Face.
#endif

BEGIN_NAMESPACE_CPT

//! A 2-D face on a b-rep.
template<typename T>
class Face<2,T> {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! A Cartesian point.
  typedef ads::FixedArray<2,T> Point;

  //! A polygon.
  typedef geom::ScanConversionPolygon<Number> Polygon;

private:

  // 
  // Member Data
  //

  // CONTINUE: Should I use a SegmentMath?

  // The source point.
  Point _source;
  // The target point.
  Point _target;
  // The length of the edge.
  Number _length;
  // The tangent to the edge.
  Point _tangent;
  // The normal to the edge.
  Point _normal;
  // The index of this edge.
  int _index;
  //! An epsilon that is appropriate for the edge lengths.
  Number _epsilon;

public:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.  Uninitialized data.
  Face()
  {}

  //! Construct from points, normal, and indices.
  Face(const Point& source, 
       const Point& target,
       const Point& normal, 
       int index);

  //! Copy constructor.
  Face(const Face& other);

  //! Assignment operator.
  Face& 
  operator=(const Face& other);

  //! Make from points, the tangent and the normal.
  /*!
    If you alread have a Face, this is more efficient than calling the 
    constructor.
  */
  void 
  make(const Point& source, 
       const Point& target,
       const Point& normal, 
       int index);

  //! Trivial destructor.
  ~Face()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  const Point& 
  getSource() const {
    return _source;
  }

  const Point& 
  getTarget() const {
    return _target;
  }

  Number 
  getLength() const {
    return _length;
  }

  const Point& 
  getTangent() const {
    return _tangent;
  }

  const Point& 
  getNormal() const {
    return _normal;
  }

  int 
  getFaceIndex() const {
    return _index;
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Find the signed distance.
  Number 
  computeDistance(const Point& p) const;

  //! Compute distance with checking.
  /*!
    The points that are closest to the face lie in a strip.
    If the point, p, is within a distance, delta, of being inside the strip
    then return the distance.  Otherwise return infinity.
  */
  Number
  computeDistanceChecked(const Point& p) const;

  //! Return the unsigned distance to the face.
  Number 
  computeDistanceUnsigned(const Point& p) const;

  //! Return the unsigned distance to the face.
  /*!
    The points that are closest to the face lie in a strip.
    If the point, p, is within a distance, delta, of being inside the strip
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeDistanceUnsignedChecked(const Point& p) const { 
    return std::abs(computeDistanceChecked(p)); 
  }



  //! Find the signed distance and closest point.
  Number 
  computeClosestPoint(const Point& p, Point* cp) const;
  
  //! Find the signed distance and closest point.
  Number 
  computeClosestPointChecked(const Point& p, Point* cp) const;
  
  //! Find the unsigned distance and closest point.
  Number 
  computeClosestPointUnsigned(const Point& p, Point* cp) const;

  //! Find the unsigned distance and closest point.
  Number 
  computeClosestPointUnsignedChecked(const Point& p, Point* cp) const {
    return std::abs(computeClosestPointChecked(p, cp)); 
  }


  
  //! Find the signed distance and the gradient of the distance.
  Number 
  computeGradient(const Point& p, Point* grad) const;
  
  //! Find the signed distance and the gradient of the distance.
  Number 
  computeGradientChecked(const Point& p, Point* grad) const;
  
  //! Find the unsigned distance and the gradient of this distance.
  Number 
  computeGradientUnsigned(const Point& p, Point* grad) const;
  
  //! Find the unsigned distance and the gradient of this distance.
  Number 
  computeGradientUnsignedChecked(const Point& p, Point* grad) const;

  

  //! Find the signed distance, closest point and gradient of the distance.
  Number 
  computeClosestPointAndGradient(const Point& p, Point* cp,
			      Point* grad) const;
  
  //! Find the signed distance, closest point and gradient of the distance.
  Number 
  computeClosestPointAndGradientChecked(const Point& p, Point* cp,
				     Point* grad) const;
  
  //! Find the unsigned distance, closest point and gradient of this distance.
  Number 
  computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
				      Point* grad) 
    const;
  
  //! Find the unsigned distance, closest point and gradient of this distance.
  Number 
  computeClosestPointAndGradientUnsignedChecked(const Point& p, 
					     Point* cp, 
					     Point* grad) const;
  


  //! Make a polygon that contains the closest points with positive and negative distance.
  /*!
    This routine enlarges the polygon in the tangential direction but 
    not in the normal direction.
  */
  void
  buildCharacteristicPolygon(Polygon* polygon, 
			     const Number height) const;

  //! Make a clipped polygon that contains the closest points with either positive or negative distance.
  /*!
    Use the neighboring faces to clip the polygon.
    This routine enlarges the polygon in the tangential direction but 
    not in the normal direction.
  */
  void
  buildCharacteristicPolygon(Polygon* polygon, const Face& prev,
			     const Face& next, const Number height) const;

  //@}

private:

  //! Return true if the point is within delta of being inside the strip of closest points
  bool 
  isInside(const Point& p, const Number delta) const {
    Point x = p;
    x -= _source;
    const Number d = geom::computeDotProduct(x, _tangent);
    return (d >= - delta && _length + delta >= d);
  }

  //! Return true if the point is (close to being) inside the strip of closest points
  bool 
  isInside(const Point& p) const { 
    return isInside(p, _epsilon);
  }

};





template<typename T>
inline
Face<2,T>::
Face(const Point& source, 
     const Point& target,
     const Point& normal, 
     const int index) :
  _source(source),
  _target(target),
  _length(geom::computeDistance(_source, _target)),
  _tangent(normal),
  _normal(normal),
  _index(index),
  // An appropriate epsilon for this face.  length * sqrt(eps)
  _epsilon(_length * 
	   std::sqrt(std::numeric_limits<Number>::epsilon())) {
  geom::rotatePiOver2(&_tangent);
}


//
// Copy constructor.
//

template<typename T>
inline
Face<2,T>::
Face(const Face& other) :
  _source(other._source),
  _target(other._target),
  _length(other._length),
  _tangent(other._tangent),
  _normal(other._normal),
  _index(other._index),
  _epsilon(other._epsilon)
{}


//
// Assignment operator.
//


template<typename T>
inline
Face<2,T>& 
Face<2,T>::
operator=(const Face& other) {
  // Avoid assignment to self.
  if (&other != this) {
    _source = other._source;
    _target = other._target;
    _length = other._length;
    _tangent = other._tangent;
    _normal = other._normal;
    _index = other._index;
    _epsilon = other._epsilon;
  }
  
  // Return *this so assignments can chain
  return *this;
}


//
// Make.
//


template<typename T>
inline
void 
Face<2,T>::
make(const Point& source, 
     const Point& target,
     const Point& normal, 
     const int index) {
  _source = source;
  _target = target;
  _length = geom::computeDistance(_source, _target);
  _tangent = normal;
  geom::rotatePiOver2(&_tangent);
  _normal = normal;
  _index = index;
  // An appropriate epsilon for this face.  length * sqrt(eps)
  _epsilon = _length * 
    std::sqrt(std::numeric_limits<Number>::epsilon());
}


//
// Mathematical Operations
//


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeDistance(const Point& p) const {
  Number dist;
  // Projection of vector on segment.
  Number proj = geom::computeDotProduct(p - getSource(), getTangent());  
  // Distance to the line defined by this edge.
  Number lineDistance = geom::computeDotProduct(p - getSource(), getNormal());
  
  if (proj < 0.0) {  // If the point is closest to the source end.
    Number d = geom::computeDistance(p, getSource());
    dist =  lineDistance > 0 ? d : -d;
  }
  else if (proj > getLength()) { // If the point is closest to the target end.
    Number d = geom::computeDistance(p, getTarget());
    dist =  lineDistance > 0 ? d : -d;
  }
  else { // If the point is closest to the interior of the edge.
    dist = lineDistance;
  }

  return dist;
}


template<typename T>
inline
typename Face<2,T>::Number
Face<2,T>::
computeDistanceChecked(const Point& p) const {
  // If the point is inside the characteristic strip.
  if (isInside(p)) {
    // Then compute the distance.
    return computeDistance(p);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeDistanceUnsigned(const Point& p) const {
  Number dist;
  // Projection of vector on segment.
  Number proj = geom::computeDotProduct(p - getSource(), getTangent());  
  
  if (proj < 0.0) {  // If the point is closest to the source end.
    dist = geom::computeDistance(p, getSource());
  }
  else if (proj > getLength()) { // If the point is closest to the target end.
    dist = geom::computeDistance(p, getTarget());
  }
  else { // If the point is closest to the interior of the edge.
    // Unsigned distance to the line defined by this edge.
    dist = std::abs(geom::computeDotProduct(p - getSource(), getNormal()));
  }

  return dist;
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeClosestPoint(const Point& p, Point* cp) const {
  Number dist;
  // Projection of vector on segment.
  Number proj = geom::computeDotProduct(p - getSource(), getTangent());  
  // Distance to the line defined by this edge.
  Number lineDistance = geom::computeDotProduct(p - getSource(), getNormal());
  
  if (proj < 0.0) {  // If the point is closest to the source end.
    *cp = getSource();
    Number d = geom::computeDistance(p, getSource());
    dist =  lineDistance > 0 ? d : -d;
  }
  else if (proj > getLength()) { // If the point is closest to the target end.
    *cp = getTarget();
    Number d = geom::computeDistance(p, getTarget());
    dist =  lineDistance > 0 ? d : -d;
  }
  else { // If the point is closest to the interior of the edge.
    *cp = getSource() + proj * getTangent();
    dist = lineDistance;
  }
  
  return dist;
}


template<typename T>
inline
typename Face<2,T>::Number
Face<2,T>::
computeClosestPointChecked(const Point& p, Point* cp) const {
  // If the point is inside the characteristic strip.
  if (isInside(p)) {
    // Then compute the distance and closest point.
    return computeClosestPoint(p, cp);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeClosestPointUnsigned(const Point& p, Point* cp) const {
  Number dist;
  // Projection of vector on segment.
  Number proj = geom::computeDotProduct(p - getSource(), getTangent());  
  
  if (proj < 0.0) {  // If the point is closest to the source end.
    *cp = getSource();
    dist = geom::computeDistance(p, getSource());
  }
  else if (proj > getLength()) { // If the point is closest to the target end.
    *cp = getTarget();
    dist = geom::computeDistance(p, getTarget());
  }
  else { // If the point is closest to the interior of the edge.
    *cp = getSource() + proj * getTangent();
    // Unsigned distance to the line defined by this edge.
    dist = std::abs(geom::computeDotProduct(p - getSource(), getNormal()));
  }
  
  return dist;
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeGradient(const Point& p, Point* grad) const {
  *grad = getNormal();
  return computeDistance(p);
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeGradientChecked(const Point& p, Point* grad) const {
  // If the point is inside the characteristic strip.
  if (isInside(p)) {
    // Then compute the distance and gradient.
    return computeGradient(p, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeGradientUnsigned(const Point& p, Point* grad) const {
  *grad = getNormal();
  Number dist = computeDistance(p);
  if (dist < 0) {
    grad->negate();
  }
  return std::abs(dist);
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeGradientUnsignedChecked(const Point& p, Point* grad) const {
  // If the point is inside the characteristic strip.
  if (isInside(p)) {
    // Then compute the distance and gradient.
    return computeGradientUnsigned(p, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeClosestPointAndGradient(const Point& p, Point* cp, 
			    Point* grad) const {
  Number dist;
  // Projection of vector on segment.
  Number proj = geom::computeDotProduct(p - getSource(), getTangent());  
  // Distance to the line defined by this edge.
  Number lineDistance = geom::computeDotProduct(p - getSource(), getNormal());
  
  if (proj < 0.0) {  // If the point is closest to the source end.
    *cp = getSource();
    Number d = geom::computeDistance(p, getSource());
    dist =  lineDistance > 0 ? d : -d;
  }
  else if (proj > getLength()) { // If the point is closest to the target end.
    *cp = getTarget();
    Number d = geom::computeDistance(p, getTarget());
    dist =  lineDistance > 0 ? d : -d;
  }
  else { // If the point is closest to the interior of the edge.
    *cp = getSource() + proj * getTangent();
    dist = lineDistance;
  }
  
  *grad = getNormal();

  return dist;
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
				   Point* grad) const {
  // If the point is inside the characteristic strip.
  if (isInside(p)) {
    // Then compute the distance, closest point, and gradient.
    return computeClosestPointAndGradient(p, cp, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeClosestPointAndGradientUnsigned(const Point& p, Point* cp,
				    Point* grad) const {
  Number dist;
  // Projection of vector on segment.
  Number proj = geom::computeDotProduct(p - getSource(), getTangent());  
  // Distance to the line defined by this edge.
  Number lineDistance = geom::computeDotProduct(p - getSource(), getNormal());
  
  if (proj < 0.0) {  // If the point is closest to the source end.
    *cp = getSource();
    dist = geom::computeDistance(p, getSource());
  }
  else if (proj > getLength()) { // If the point is closest to the target end.
    *cp = getTarget();
    dist = geom::computeDistance(p, getTarget());
  }
  else { // If the point is closest to the interior of the edge.
    *cp = getSource() + proj * getTangent();
    // Unsigned distance to the line defined by this edge.
    dist = std::abs(lineDistance);
  }

  *grad = getNormal();
  if (lineDistance < 0) {
    grad->negate();
  }

  return dist;
}


template<typename T>
inline
typename Face<2,T>::Number 
Face<2,T>::
computeClosestPointAndGradientUnsignedChecked(const Point& p, 
					   Point* cp,
					   Point* grad) const {
  // If the point is inside the characteristic strip.
  if (isInside(p)) {
    // Then compute the distance, closest point, and gradient.
    return computeClosestPointAndGradientUnsigned(p, cp, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
void
Face<2,T>::
buildCharacteristicPolygon(Polygon* polygon, const Number height) const {
#ifdef DEBUG_Face
  assert(height > 0);
#endif

  polygon->clear();

  const Number maximumCoordinate = 
    ads::max(std::abs(getSource()[0]), std::abs(getSource()[1]),
	     std::abs(getTarget()[0]), std::abs(getTarget()[1]));
  // Below, 10 is the fudge factor.
  const Number epsilon = 
    std::max(Number(1.0), maximumCoordinate) * 
    10.0 * std::numeric_limits<Number>::epsilon();

  Point s = _source;
  s -= epsilon * _tangent;
  s -= height * _normal;

  Point t = _target;
  t += epsilon * _tangent;
  t -= height * _normal;

  Point side = _normal;
  side *= 2 * height;

  polygon->insert(s);
  polygon->insert(s + side);
  polygon->insert(t + side);
  polygon->insert(t);
}


template<typename T>
inline
void
Face<2,T>::
buildCharacteristicPolygon(Polygon* polygon, const Face& prev,
			   const Face& next, Number height) const {
#ifdef DEBUG_Face
  assert(height > 0);
#endif
  const Number discriminantThreshhold = 0.1 * _length / height;

  // See whether the b-rep is convex or concave at the end points of 
  // the edge.
  const Number rightDiscriminant = 
    geom::computeDiscriminant(prev.getTangent(), getTangent());
  const Number leftDiscriminant = 
    geom::computeDiscriminant(getTangent(), next.getTangent());

  // If no clipping will be done.
  if (std::abs(rightDiscriminant) <= discriminantThreshhold && 
      std::abs(leftDiscriminant) <= discriminantThreshhold) {
    // Call the version that does not perform clipping.
    buildCharacteristicPolygon(polygon, height);
    return;
  }
    
  polygon->clear();
  
  // How local clipping is done on the left and right sides.
  const int NotClipped = 0, SideClipped = 1, TopClipped = 2;

  // Enlarge the edge in the tangential direction.
  const Number maximumCoordinate = 
    ads::max(std::abs(getSource()[0]), std::abs(getSource()[1]),
	     std::abs(getTarget()[0]), std::abs(getTarget()[1]));
  // Below, 10 is the fudge factor.
  const Number epsilon = 
    std::max(Number(1.0), maximumCoordinate) * 
    10.0 * std::numeric_limits<Number>::epsilon();
  const Point src = getSource() - epsilon * getTangent();
  const Point tgt = getTarget() + epsilon * getTangent();

  //
  // First determine the points of the polygon that contains grid 
  // points with positive distance from the edge.
  //

  //
  // Find the right edge.
  //
  Point rightEdgePoint;
  int clipRight = NotClipped;

  // If the previous edge should be used to clip the polygon->
  if (rightDiscriminant < -discriminantThreshhold) {
    Number x = geom::computeDotProduct(prev.getSource() - tgt, getTangent());
    Number d = geom::computeDotProduct(prev.getSource() - tgt, getNormal());
#ifdef DEBUG_Face
    assert(d > 0);
#endif
    Number y = (x * x + d * d) / (2 * d);
    rightEdgePoint = tgt + y * getNormal();
    if (std::abs(y) > height) {
      clipRight = TopClipped;
      rightEdgePoint = src + (height / std::abs(y))
	*(rightEdgePoint - src);
    }
    else {
      clipRight = SideClipped;
    }
  }
  else {
    // The right edge is not clipped
    rightEdgePoint = src + height * getNormal();
  }

  //
  // Find the left edge.
  //
  Point leftEdgePoint;
  int clipLeft = NotClipped;

  // If the next edge should be used to clip the polygon->
  if (leftDiscriminant < -discriminantThreshhold) {
    Number x = geom::computeDotProduct(next.getTarget() - src, getTangent());
    Number d = geom::computeDotProduct(next.getTarget() - src, getNormal());
#ifdef DEBUG_Face
    assert(d > 0);
#endif
    Number y = (x * x + d * d) / (2 * d);
    leftEdgePoint = src + y * getNormal();
    if (std::abs(y) > height) {
      clipLeft = TopClipped;
      leftEdgePoint = tgt + (height / std::abs(y)) 
	* (leftEdgePoint - tgt);
    }
    else {
      clipLeft = SideClipped;
    }
  }
  else {
    // The left edge is not clipped
    leftEdgePoint = tgt + height * getNormal();
  }
  
  //
  // Make the portion of the polygon containing grid points with 
  // positive distance.
  //

  Point intersectionPoint;
  polygon->insert(src);

  // CONTINUE: this could be more efficient.
  if (clipLeft == SideClipped && clipRight == NotClipped) {
    polygon->insert(leftEdgePoint);
  }
  else if (clipRight == SideClipped && clipLeft == NotClipped) {
    polygon->insert(rightEdgePoint);
  }
  else if (geom::computeIntersection(geom::SegmentMath<2,Number>
				     (src, rightEdgePoint), 
				     geom::SegmentMath<2,Number>
				     (tgt, leftEdgePoint),
				     &intersectionPoint)) {
    polygon->insert(intersectionPoint);
  }
  else {
    polygon->insert(rightEdgePoint);
    polygon->insert(leftEdgePoint);
  }


  //
  // Next determine the points of the polygon that contains grid 
  // points with negative distance from the edge.
  //

  //
  // Find the right edge.
  //
  clipRight = NotClipped;

  // If the previous edge should be used to clip the polygon->
  if (rightDiscriminant > discriminantThreshhold) {
    Number x = geom::computeDotProduct(prev.getSource() - tgt, getTangent());
    Number d = geom::computeDotProduct(prev.getSource() - tgt, getNormal());
#ifdef DEBUG_Face
    assert(d < 0);
#endif
    Number y = (x * x + d * d) / (2 * d);
    rightEdgePoint = tgt + y * getNormal();
    if (std::abs(y) > height) {
      clipRight = TopClipped;
      rightEdgePoint = src + (height / std::abs(y))
	*(rightEdgePoint - src);
    }
    else {
      clipRight = SideClipped;
    }
  }
  else {
    // The right edge is not clipped
    rightEdgePoint = src - height * getNormal();
  }

  //
  // Find the left edge.
  //
  clipLeft = NotClipped;

  // If the next edge should be used to clip the polygon->
  if (leftDiscriminant > discriminantThreshhold) {
    Number x = geom::computeDotProduct(next.getTarget() - src, getTangent());
    Number d = geom::computeDotProduct(next.getTarget() - src, getNormal());
#ifdef DEBUG_Face
    assert(d < 0);
#endif
    Number y = (x * x + d * d) / (2 * d);
    leftEdgePoint = src + y * getNormal();
    if (std::abs(y) > height) {
      clipLeft = TopClipped;
      leftEdgePoint = tgt + (height / std::abs(y)) 
	* (leftEdgePoint - tgt);
    }
    else {
      clipLeft = SideClipped;
    }
  }
  else {
    // The left edge is not clipped
    leftEdgePoint = tgt - height * getNormal();
  }
  
  //
  // Make the portion of the polygon containing grid points with 
  // negative distance.
  //

  polygon->insert(tgt);

  if (clipLeft == SideClipped && clipRight == NotClipped) {
    polygon->insert(leftEdgePoint);
  }
  else if (clipRight == SideClipped && clipLeft == NotClipped) {
    polygon->insert(rightEdgePoint);
  }
  else if (geom::computeIntersection(geom::SegmentMath<2,Number>
				     (src, rightEdgePoint), 
				     geom::SegmentMath<2,Number>
				     (tgt, leftEdgePoint),
				     &intersectionPoint)) {
    polygon->insert(intersectionPoint);
  }
  else {
    polygon->insert(leftEdgePoint);
    polygon->insert(rightEdgePoint);
  }
}


//
// Equality / Inequality
//


template<typename T>
inline
bool 
operator==(const Face<2,T>& x, const Face<2,T>& y) {
  return (x.getSource() == y.getSource() &&
	  x.getTarget() == y.getTarget() &&
	  x.getLength() == y.getLength() &&
	  x.getTangent() == y.getTangent() &&
	  x.getNormal() == y.getNormal() &&
	  x.getFaceIndex() == y.getFaceIndex());
}


//
// File I/O
//


template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Face<2,T>& x) {
  return out << "getSource() = " << x.getSource() << '\n'
	     << "getTarget() = " << x.getTarget() << '\n'
	     << "getLength() = " << x.getLength() << '\n'
	     << "getTangent() = " << x.getTangent() << '\n'
	     << "getNormal() = " << x.getNormal() << '\n'
	     << "index() = " << x.index() << '\n';
}


END_NAMESPACE_CPT
