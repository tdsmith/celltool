// -*- C++ -*-

#if !defined(__cpt_Vertex2_ipp__)
#error This file is an implementation detail of the class Vertex.
#endif

BEGIN_NAMESPACE_CPT

//! A vertex in 2-D.
template<typename T>
class Vertex<2,T> {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! A Cartesian point.
  typedef ads::FixedArray<2,T> Point;

  //! A Polygon.
  typedef geom::ScanConversionPolygon<Number> Polygon;

private:

  //
  // Member data.
  //

  // The location of the vertex.
  Point _location;

  // The normals of the right and left adjacent edges.
  Point _rightNormal, _leftNormal;

  // An outward normal to the surface.
  Point _normal;

  // The index of an adjacent edge.
  int _faceIndex;

  // The sign of the distance.
  int _signOfDistance;

public:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{
  
  //! Default constructor.  Uninitialized data.
  Vertex()
  {}

  //! Construct from a point and edge information.
  /*!
    Construct from a point, its neighboring face normals and
    the index of an adjacent face.  The neighbors are given in positive 
    order.
  */
  Vertex(const Point& location, 
	  const Point& rightNormal, 
	  const Point& leftNormal, 
	  const int faceIndex);

  //! Copy constructor.
  Vertex(const Vertex& other);

  //! Assignment operator.
  Vertex& 
  operator=(const Vertex& other);

  //! Make from a point and edge information.
  /*!
    If you already have a Vertex, this is more efficient than calling the 
    constructor.
  */
  void
  make(const Point& location,
	const Point& rightNormal, 
	const Point& leftNormal, 
	const int faceIndex);

  //! Trivial destructor.
  ~Vertex()
  {}
  
  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{
  
  //! The location of the vertex.
  const Point& 
  getLocation() const { 
    return _location; 
  }

  //! The normal of the right adjacent edge.
  const Point& 
  getRightNormal() const {
    return _rightNormal;
  }

  //! The normal of the left adjacent edge.
  const Point& 
  getLeftNormal() const {
    return _leftNormal;
  }

  //! The index of an adjacent face.
  int 
  getFaceIndex() const {
    return _faceIndex;
  }

  //! The sign of the distance to this vertex.
  int 
  getSignOfDistance() const {
    return _signOfDistance;
  }

  //! Return true if the curve is convex or concave.
  bool 
  isConvexOrConcave() const {
    return _signOfDistance != 0;
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Return the signed distance to the vertex.
  Number
  computeDistance(const Point& p) const {
#ifdef DEBUG_Vertex
    assert(_signOfDistance == 1 || _signOfDistance == -1);
#endif
    return _signOfDistance * geom::computeDistance(p, _location);
  }

  //! Return the signed distance to the vertex.
  /*!
    Use the vertex normal to determine the sign.
  */
  Number
  computeDistanceChecked(const Point& p) const {
    return computeSignOfDistance(p) * geom::computeDistance(p, _location);
  }

  //! Return the unsigned distance to the vertex.
  Number
  computeDistanceUnsigned(const Point& p) const {
    return geom::computeDistance(p, _location);
  }

  //! Return the unsigned distance to the vertex.
  Number
  computeDistanceUnsignedChecked(const Point& p) const {
    return computeDistanceUnsigned(p);
  }



  //! Return distance.  Compute closest point.
  Number 
  computeClosestPoint(const Point& p, Point* cp) const;

  //! Return distance.  Compute closest point.
  /*!
    Use the vertex normal to determine the sign.
  */
  Number 
  computeClosestPointChecked(const Point& p, Point* cp) const;

  //! Return unsigned distance.  Compute closest point.
  Number 
  computeClosestPointUnsigned(const Point& p, Point* cp) const;

  //! Return unsigned distance.  Compute closest point.
  Number 
  computeClosestPointUnsignedChecked(const Point& p, Point* cp) const {
    return computeClosestPointUnsigned(p, cp);
  }




  //! Return the distance.  Compute gradient of the distance.
  Number 
  computeGradient(const Point& p, Point* grad) const;

  //! Return the distance.  Compute gradient of the distance.
  /*!
    Use the vertex normal to determine the sign.
  */
  Number 
  computeGradientChecked(const Point& p, Point* grad) const;

  //! Return the unsigned distance.  Compute the gradient of this distance.
  Number 
  computeGradientUnsigned(const Point& p, Point* grad) const;

  //! Return the unsigned distance.  Compute the gradient of this distance.
  Number 
  computeGradientUnsignedChecked(const Point& p, Point* grad) const {
    return computeGradientUnsigned(p, grad);
  }



  //! Return the distance.  Compute the closest point and the gradient of the distance.
  Number 
  computeClosestPointAndGradient(const Point& p, Point* cp, 
			      Point* grad) const;

  //! Return the distance.  Compute the closest point and the gradient of the distance.
  /*!
    Use the vertex normal to determine the sign.
  */
  Number 
  computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
				     Point* grad) const;

  //! Return the unsigned distance.  Compute the closest point and the gradient of this distance.
  Number 
  computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
				   Point* grad) const;

  //! Return the unsigned distance.  Compute the closest point and the gradient of this distance.
  Number 
  computeClosestPointAndGradientUnsignedChecked(const Point& p, Point* cp,
					   Point* grad) const {
    return computeClosestPointAndGradientUnsigned(p, cp, grad);
  }



  //! Make a polygon that contains the closest points for signed distance.
  void
  buildCharacteristicPolygon(Polygon* polygon, Number height) const;

  //! Make a polygon that contains the closest points for unsigned distance.
  void
  buildCharacteristicPolygonUnsigned(Polygon* polygon, Number height) const;

  //@}

private:

  // Use the vertex normal to determine the sign of the distance.
  int
  computeSignOfDistance(const Point& pt) const {
    return geom::computeDotProduct(_normal, Point(pt - _location)) > 0 ? 
      1 : -1;
  }

  //! Given the sign of the distance: Return the distance.  Compute gradient of the distance.
  Number 
  computeGradient(const Point& p, Point* grad, int signOfDistance) const;
};





//
// Constructors, Destructor.
//
  
template<typename T>
inline
Vertex<2,T>::
Vertex(const Point& location, 
	const Point& rightNormal, 
	const Point& leftNormal, 
	const int faceIndex) :
  _location(location),
  _rightNormal(rightNormal),
  _leftNormal(leftNormal),
  _normal(),
  _faceIndex(faceIndex),
  _signOfDistance(ads::sign(geom::computeDiscriminant(rightNormal, 
						 leftNormal))) {
  _normal = _rightNormal;
  _normal += _leftNormal;
  geom::normalize(&_normal);
}


//
// Copy constructor.
//


template<typename T>
inline
Vertex<2,T>::
Vertex(const Vertex& other) :
  _location(other._location),
  _rightNormal(other._rightNormal),
  _leftNormal(other._leftNormal),
  _normal(other._normal),
  _faceIndex(other._faceIndex),
  _signOfDistance(other._signOfDistance)
{}


//
// Assignment operator.
//

template<typename T>
inline
Vertex<2,T>& 
Vertex<2,T>::
operator=(const Vertex& other) {
  // Avoid assignment to self
  if (&other != this){
    _location = other._location;
    _rightNormal = other._rightNormal;
    _leftNormal = other._leftNormal;
    _normal = other._normal;
    _faceIndex = other._faceIndex;
    _signOfDistance = other._signOfDistance;
  }
  
  // Return *this so assignments can chain
  return *this;
}


//
// Make
//


template<typename T>
inline
void
Vertex<2,T>::
make(const Point& location, 
      const Point& rightNormal, 
      const Point& leftNormal, 
      const int faceIndex) {
  _location = location;
  _rightNormal = rightNormal;
  _leftNormal = leftNormal;
  _normal = _rightNormal;
  _normal += _leftNormal;
  geom::normalize(&_normal);
  _faceIndex = faceIndex;
  _signOfDistance = ads::sign(geom::computeDiscriminant(rightNormal, 
						  leftNormal));
}

//
// Mathematical operations
//



template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeClosestPoint(const Point& p, Point* cp) const {
  *cp = _location;
  return computeDistance(p);
}


template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeClosestPointChecked(const Point& p, Point* cp) const {
  *cp = _location;
  return computeDistanceChecked(p);
}


template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeClosestPointUnsigned(const Point& p, Point* cp) const {
  *cp = _location;
  return computeDistanceUnsigned(p);
}




template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeGradient(const Point& p, Point* grad) const {
  // Call the private function with the sign of the distance.
  return computeGradient(p, grad, _signOfDistance);
}


template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeGradientChecked(const Point& p, Point* grad) const {
  // Call the private function with the sign of the distance.
  return computeGradient(p, grad, computeSignOfDistance(p));
}


template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeGradientUnsigned(const Point& p, Point* grad) const {
  //grad = pt - getLocation();
  *grad = p;
  *grad -= getLocation();
  Number mag = geom::computeMagnitude(*grad);
  if (mag > std::numeric_limits<Number>::epsilon()) {
    *grad /= mag;
  }
  else {
    // If the Cartesian point is very close to the vertex, choose the 
    // gradient of the distance to be the normal to the curve.
    *grad = _normal;
    *grad *= _signOfDistance;
  }
  return computeDistanceUnsigned(p);
}




template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeClosestPointAndGradient(const Point& p, Point* cp, 
			       Point* grad) const {
  *cp = _location;
  return computeGradient(p, grad);
}


template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
				Point* grad) const {
  *cp = _location;
  return computeGradientChecked(p, grad);
}


template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
				 Point* grad) const {
  *cp = _location;
  return computeGradientUnsigned(p, grad);
}





template<typename T>
inline
typename Vertex<2,T>::Number
Vertex<2,T>::
computeGradient(const Point& p, Point* grad, const int signOfDistance) const {
  //grad = signOfDistance * (pt - getLocation());
  *grad = p;
  *grad -= getLocation();
  *grad *= signOfDistance;
  Number mag = geom::computeMagnitude(*grad);
  if (mag > std::numeric_limits<Number>::epsilon()) {
    *grad /= mag;
  }
  else {
    // If the Cartesian point is very close to the vertex, choose the 
    // gradient of the distance to be the normal to the curve.
    *grad = _normal;
  }
  return signOfDistance * geom::computeDistance(p, _location);
}



template<typename T>
inline
void
Vertex<2,T>::
buildCharacteristicPolygon(Polygon* polygon, Number height) const {
#ifdef DEBUG_Vertex
  assert(height > 0);
#endif

  polygon->clear();

  if (getSignOfDistance() == 1) {  // If positive distance.
#ifdef DEBUG_Vertex
    // Make sure that the curve is convex.
    assert(geom::computeDiscriminant(_rightNormal, _leftNormal) >= 0);
#endif
    // If the angle is less than pi/2.
    if (geom::computeDotProduct(_rightNormal, _leftNormal) > 0) {  
      // Bisect the angle to calculate the length factor.
      Point v = Number(0.5) * (_rightNormal + _leftNormal);
      height /= geom::computeMagnitude(v);
      // Add the vertices of the triangle.
      polygon->insert(_location);
      polygon->insert(_location + height * _rightNormal);
      polygon->insert(_location + height * _leftNormal);
    }
    // Else the angle is greater than pi/2.
    else { 
      // Bisect the angle.
      Point rightTangent(_rightNormal);
      geom::rotatePiOver2(&rightTangent);
      Point leftTangent(_leftNormal);
      geom::rotatePiOver2(&leftTangent);
      Point v = rightTangent - leftTangent;
      geom::normalize(&v);
      // Bisect again to calculate the length factor.
      Point w = Number(0.5) * (_rightNormal + v);
      height /= geom::computeMagnitude(w);
      // Add the vertices of the quadrangle.
      polygon->insert(_location);
      polygon->insert(_location + height * _rightNormal);
      polygon->insert(_location + height * v);
      polygon->insert(_location + height * _leftNormal);
    }
  }
  else if (getSignOfDistance() == -1) { // Else negative distance.
#ifdef DEBUG_Vertex
    // Make sure that the curve is concave.
    assert(geom::computeDiscriminant(_rightNormal, _leftNormal) <= 0);
#endif
    // If the angle is less than pi/2.
    if (geom::computeDotProduct(_rightNormal, _leftNormal) > 0) {  
      // Bisect the angle to calculate the length factor.
      Point v = Number(0.5) * (_rightNormal + _leftNormal);
      height /= geom::computeMagnitude(v);
      // Add the vertices of the triangle.
      polygon->insert(_location);
      polygon->insert(_location - height * _leftNormal);
      polygon->insert(_location - height * _rightNormal);
    }
    // Else the angle is greater than pi/2.
    else { 
      // Bisect the angle.
      Point rightTangent(_rightNormal);
      geom::rotatePiOver2(&rightTangent);
      Point leftTangent(_leftNormal);
      geom::rotatePiOver2(&leftTangent);
      Point v = rightTangent - leftTangent;
      geom::normalize(&v);
      // Bisect again to calculate the length factor.
      Point w = Number(0.5) * (v - _rightNormal);
      height /= geom::computeMagnitude(w);
      // Add the vertices of the quadrangle.
      polygon->insert(_location);
      polygon->insert(_location - height * _leftNormal);
      polygon->insert(_location + height * v);
      polygon->insert(_location - height * _rightNormal);
    }
  }
}



template<typename T>
inline
void
Vertex<2,T>::
buildCharacteristicPolygonUnsigned(Polygon* polygon, Number height) const {
  static Point zero(0.0);

#ifdef DEBUG_Vertex
  assert(height > 0);
#endif
  
  // If the sign of the distance is non-zero, then there are two known edges.
  if (getSignOfDistance() != 0) {
#ifdef DEBUG_Vertex
    assert(getLeftNormal() != zero && getRightNormal() != zero);
#endif
    // The characteristic polygon for unsigned distance is the same as that
    // for signed distance.
    buildCharacteristicPolygon(polygon, height);
  }
  else {
    // The vertex has exactly one adjacent face.
    polygon->clear();

    // If there is a right edge.
    if (getLeftNormal() == zero) {
#ifdef DEBUG_Vertex
      assert(getRightNormal() != zero);
#endif
      // The normal and tangential offsets from the vertex.
      Point normalOffset(getRightNormal());
      normalOffset *= height;
      Point tangentOffset(normalOffset);
      geom::rotatePiOver2(&tangentOffset);
      // Add the vertices of the rectangle.
      Point p = getLocation();
      p += normalOffset;
      polygon->insert(p);
      p += tangentOffset;
      polygon->insert(p);
      p -= normalOffset;
      p -= normalOffset;
      polygon->insert(p);
      p -= tangentOffset;
      polygon->insert(p);
    }
    // Otherwise there is a left edge.
    else {
#ifdef DEBUG_Vertex
      assert(getLeftNormal() != zero && getRightNormal() == zero);
#endif
      // The normal and tangential offsets from the vertex.
      Point normalOffset(getLeftNormal());
      normalOffset *= height;
      Point tangentOffset(normalOffset);
      geom::rotatePiOver2(&tangentOffset);
      // Add the vertices of the rectangle.
      Point p = getLocation();
      p -= normalOffset;
      polygon->insert(p);
      p -= tangentOffset;
      polygon->insert(p);
      p += normalOffset;
      p += normalOffset;
      polygon->insert(p);
      p += tangentOffset;
      polygon->insert(p);
    }
  }
}


//
// Equality / Inequality
//


template<typename T>
inline
bool 
operator==(const Vertex<2,T>& x, const Vertex<2,T>& y) {
  return (x.getLocation() == y.getLocation() &&
	   x.getRightNormal() == y.getRightNormal() &&
	   x.getLeftNormal() == y.getLeftNormal() &&
	   x.getFaceIndex() == y.getFaceIndex() &&
	   x.getSignOfDistance() == y.getSignOfDistance());
}

END_NAMESPACE_CPT
