// -*- C++ -*-

#if !defined(__cpt_Edge_ipp__)
#error This file is an implementation detail of the class Edge.
#endif

BEGIN_NAMESPACE_CPT

//
// Constructors
//


template<typename T>
inline
Edge<3,T>::
Edge(const Point& source, const Point& target, 
     const Point& leftNormal, const Point& rightNormal,
     const int adjacentFaceIndex) {
  make(source, target, leftNormal, rightNormal, adjacentFaceIndex);
}


template<typename T>
inline
void
Edge<3,T>::
make(const Point& source, const Point& target, 
     const Point& leftNormal, const Point& rightNormal,
     const int adjacentFaceIndex) {
  _segment.make(source, target);
  _leftFaceNormal = leftNormal;
  _rightFaceNormal = rightNormal;
  _adjacentFaceIndex = adjacentFaceIndex;

  _leftSideNormal = geom::computeCrossProduct(_segment.getTangent(), leftNormal);
  _rightSideNormal = geom::computeCrossProduct(rightNormal,_segment.getTangent());
    
  const Number vol = geom::computeDotProduct
    (geom::computeCrossProduct(leftNormal, rightNormal), 
     _segment.getTangent());
  if (vol > 0) {
    _signOfDistance = 1;
  }
  else if (vol < 0) {
    _signOfDistance = -1;
  }
  else {
    _signOfDistance = 0;
  }

  _epsilon = _segment.getLength() * 
    std::sqrt(std::numeric_limits<Number>::epsilon());
}


template<typename T>
inline
Edge<3,T>::
Edge(const Edge& other) : 
  _segment(other._segment),
  _leftFaceNormal(other._leftFaceNormal),
  _rightFaceNormal(other._rightFaceNormal),
  _leftSideNormal(other._leftSideNormal),
  _rightSideNormal(other._rightSideNormal),
  _adjacentFaceIndex(other._adjacentFaceIndex),
  _signOfDistance(other._signOfDistance),
  _epsilon(other._epsilon)
{}


template<typename T>
inline
Edge<3,T>& 
Edge<3,T>::
operator=(const Edge& other) {
  // Avoid assignment to self
  if (&other != this) {
    // Copy the member data.
    _segment = other._segment;
    _leftFaceNormal = other._leftFaceNormal;
    _rightFaceNormal = other._rightFaceNormal;
    _leftSideNormal = other._leftSideNormal;
    _rightSideNormal = other._rightSideNormal;
    _adjacentFaceIndex = other._adjacentFaceIndex;
    _signOfDistance = other._signOfDistance;
    _epsilon = other._epsilon;
  }
  // Return *this so assignments can chain
  return *this;
}


//
// Mathematical operations
//


template<typename T>
inline
bool
Edge<3,T>::
isValid() const {
  if (! _segment.isValid()) {
    return false;
  }

  const Number eps = 10 * std::numeric_limits<Number>::epsilon();

  if (std::abs(geom::computeMagnitude(_leftFaceNormal) - 1) > eps) {
    return false;
  }
  if (std::abs(geom::computeMagnitude(_rightFaceNormal) - 1) > eps) {
    return false;
  }

  if (std::abs(geom::computeMagnitude(_leftSideNormal) - 1) > eps) {
    return false;
  }
  if (std::abs(geom::computeMagnitude(_rightSideNormal) - 1) > eps) {
    return false;
  }

  if (_adjacentFaceIndex < 0) {
    return false;
  }

  if (! (_signOfDistance == 1 || _signOfDistance == -1)) {
    return false;
  }
  
  return true;
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeDistance(const Point& p) const {
  return _signOfDistance * computeDistanceUnsigned(p);
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeDistanceChecked(const Point& p) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance.
    return computeDistance(p);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeDistanceUnsigned(const Point& p) const {
  return computeUnsignedDistanceToSupportingLine(_segment, p);
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeDistanceUnsignedChecked(const Point& p) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance.
    return computeDistanceUnsigned(p);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPoint(const Point& p, Point* cp) const {
  return _signOfDistance * computeClosestPointUnsigned(p, cp);
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointChecked(const Point& p, Point* cp) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance and closest point.
    return computeClosestPoint(p, cp);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointUnsigned(const Point& p, Point* cp) const {
  return computeUnsignedDistanceAndClosestPointToSupportingLine
    (_segment, p, cp);
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointUnsignedChecked(const Point& p, Point* cp) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance and closest point.
    return computeClosestPointUnsigned(p, cp);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeGradient(const Point& p, Point* grad) const {
  Point cp;
  Number dist = computeClosestPoint(p, &cp);

  //grad = Number(getSignOfDistance()) * (pt - closest_pt);
  *grad = p;
  *grad -= cp;
  *grad *= getSignOfDistance();
  Number mag = geom::computeMagnitude(*grad);
  if (mag > getLength() * 10.0 * std::numeric_limits<Number>::epsilon()) {
    *grad /= mag;
  }
  else {
    *grad = getLeftFaceNormal();
  }

  return dist;
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeGradientChecked(const Point& p, Point* grad) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance and gradient.
    return computeGradient(p, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeGradientUnsigned(const Point& p, Point* grad) const {
  Point cp;
  Number dist = computeClosestPointUnsigned(p, &cp);

  // grad = pt - closest_pt;
  *grad = p;
  *grad -= cp;
  Number mag = geom::computeMagnitude(*grad);
  if (mag > getLength() * 10.0 * std::numeric_limits<Number>::epsilon()) {
    *grad /= mag;
  }
  else {
    *grad = getLeftFaceNormal();
    *grad *= getSignOfDistance();
  }

  return dist;
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeGradientUnsignedChecked(const Point& p, Point* grad) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance and gradient.
    return computeGradientUnsigned(p, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}



template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointAndGradient(const Point& p, Point* cp, Point* grad) const {
  Number dist = computeClosestPoint(p, cp);

  //grad = Number(getSignOfDistance()) * (pt - closest_pt);
  *grad = p;
  *grad -= *cp;
  *grad *= getSignOfDistance();
  Number mag = geom::computeMagnitude(*grad);
  if (mag > getLength() * 10.0 * std::numeric_limits<Number>::epsilon()) {
    *grad /= mag;
  }
  else {
    *grad = getLeftFaceNormal();
  }

  return dist;
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointAndGradientChecked(const Point& p, Point* cp,
				   Point* grad) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance, closest point, and gradient.
    return computeClosestPointAndGradient(p, cp, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointAndGradientUnsigned(const Point& p, Point* cp,
				    Point* grad) const {
  Number dist = computeClosestPointUnsigned(p, cp);

  // grad = pt - closest_pt;
  *grad = p;
  *grad -= *cp;
  Number mag = geom::computeMagnitude(*grad);
  if (mag > getLength() * 10.0 * std::numeric_limits<Number>::epsilon()) {
    *grad /= mag;
  }
  else {
    *grad = getLeftFaceNormal();
    *grad *= getSignOfDistance();
  }

  return dist;
}



template<typename T>
inline
typename Edge<3,T>::Number 
Edge<3,T>::
computeClosestPointAndGradientUnsignedChecked(const Point& p, Point* cp,
					   Point* grad) const {
  // If the point is inside the characteristic wedge.
  if (isInside(p)) {
    // Then compute the distance, closest point, and gradient.
    return computeClosestPointAndGradientUnsigned(p, cp, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


// Make the wedge that contains the closest points to the edge.
template<typename T>
inline
void
Edge<3,T>::
buildCharacteristicPolyhedron(Polyhedron* polyhedron, 
			      Number height) const {
  polyhedron->clear();
  
  // If the surface is flat at the edge, do nothing.
  if (getSignOfDistance() == 0) {
    return;
  }

  height *= getSignOfDistance();

  Number cosTheta = geom::computeDotProduct(getLeftFaceNormal(), 
					    getRightFaceNormal());

  // if the angle between the normals to the adjacent faces is less 
  // than or equal to pi/2.
  if (cosTheta >= 0) {
    // p = 0.5 * (getLeftFaceNormal() + getRightFaceNormal());
    Point p = getLeftFaceNormal();
    p += getRightFaceNormal();
    p *= 0.5;
    height /= geom::computeMagnitude(p);

    //
    // Add the vertices of the wedge.
    //

    polyhedron->insertVertex(getSource()); // Vertex 0
    polyhedron->insertVertex(getTarget()); // Vertex 1

    Point leftOffset = getLeftFaceNormal();
    leftOffset *= height;
    Point rightOffset = getRightFaceNormal();
    rightOffset *= height;

    p = getSource();
    p += leftOffset;
    polyhedron->insertVertex(p); // Vertex 2
    p = getSource();
    p += rightOffset;
    polyhedron->insertVertex(p); // Vertex 3
    p = getTarget();
    p += leftOffset;
    polyhedron->insertVertex(p); // Vertex 4
    p = getTarget();
    p += rightOffset;
    polyhedron->insertVertex(p); // Vertex 5

    //
    // Add the edges of the wedge.
    //

    polyhedron->insertEdge(0, 1);
    polyhedron->insertEdge(2, 4);
    polyhedron->insertEdge(3, 5);
    
    polyhedron->insertEdge(0, 2);
    polyhedron->insertEdge(0, 3);
    polyhedron->insertEdge(2, 3);
    
    polyhedron->insertEdge(1, 4);
    polyhedron->insertEdge(1, 5);
    polyhedron->insertEdge(4, 5);
  }
  else {  // The angle is greater than pi/2.
    // The polyhedra will be two wedges joined together.

    // Bisect the angle.
    Point bisect;
    if (cosTheta > -0.9) {
      bisect = getLeftFaceNormal() + getRightFaceNormal();
      //      bisect *= getSignOfDistance();
    }
    else {
      bisect = getLeftSideNormal() + getRightSideNormal();
    }
    geom::normalize(&bisect);
    
    Point p = getLeftFaceNormal();
    p += bisect;
    p *= 0.5;
    height /= geom::computeMagnitude(p);

    //
    // Add the vertices of the wedge.
    //

    polyhedron->insertVertex(getSource()); // Vertex 0
    polyhedron->insertVertex(getTarget()); // Vertex 1
    
    Point leftOffset = getLeftFaceNormal();
    leftOffset *= height;
    Point rightOffset = getRightFaceNormal();
    rightOffset *= height;
    Point middle_offset = bisect;
    middle_offset *= height;
    
    p = getSource();
    p += leftOffset;
    polyhedron->insertVertex(p); // Vertex 2
    p = getSource();
    p += rightOffset;
    polyhedron->insertVertex(p); // Vertex 3
    p = getSource();
    p += middle_offset;
    polyhedron->insertVertex(p); // Vertex 4

    p = getTarget();
    p += leftOffset;
    polyhedron->insertVertex(p); // Vertex 5
    p = getTarget();
    p += rightOffset;
    polyhedron->insertVertex(p); // Vertex 6
    p = getTarget();
    p += middle_offset;
    polyhedron->insertVertex(p); // Vertex 7

    //
    // Add the edges of the wedge.
    //

    polyhedron->insertEdge(0, 1);
    polyhedron->insertEdge(2, 5);
    polyhedron->insertEdge(3, 6);
    polyhedron->insertEdge(4, 7);
    
    polyhedron->insertEdge(0, 2);
    polyhedron->insertEdge(2, 4);
    polyhedron->insertEdge(4, 3);
    polyhedron->insertEdge(3, 0);
    
    polyhedron->insertEdge(1, 5);
    polyhedron->insertEdge(5, 7);
    polyhedron->insertEdge(7, 6);
    polyhedron->insertEdge(6, 1);
  }
}



// Make the wedge or box that contains the closest points to the edge.
template<typename T>
inline
void
Edge<3,T>::
buildCharacteristicPolyhedronUnsigned(Polyhedron* polyhedron, 
				      Number height) const {
  // CONTINUE: is static a good idea?
  static Point zero(0.0);

  // If both of the adjacent faces exist.
  if (getLeftFaceNormal() != zero && getRightFaceNormal() != zero) {
    // The characteristic polyhedron for unsigned distance is the same as that
    // for signed distance.
    buildCharacteristicPolyhedron(polyhedron, height);
  }
  // There is exactly one adjacent face.
  else {
    polyhedron->clear();

    /*
      Point src(getSource());
      grid.location_to_index(src);
      Point tgt(getTarget());
      grid.location_to_index(tgt);
    */
    Point p, y, z;

    // If the there is a right adjacent face.
    if (getRightFaceNormal() != zero) {
      y = getRightFaceNormal();
      geom::computeCrossProduct(y, getTangent(), &z);
    }
    // If the there is a left adjacent face.
    else if (getLeftFaceNormal() != zero) {
      y = getLeftFaceNormal();
      geom::computeCrossProduct(getTangent(), y, &z);
    }
    else {
      // There must be at least one adjacent face.
      assert(false);
    }

    y *= height;
    z *= height;

    //
    // Add the vertices of the box.
    //

    p = getSource(); p -= y;
    polyhedron->insertVertex(p); // Vertex 0
    p = getTarget(); p -= y;
    polyhedron->insertVertex(p); // Vertex 1
    p = getSource(); p += y;
    polyhedron->insertVertex(p); // Vertex 2
    p = getTarget(); p += y;
    polyhedron->insertVertex(p); // Vertex 3

    p = polyhedron->getVertex(0); p += z;
    polyhedron->insertVertex(p); // Vertex 4
    p = polyhedron->getVertex(1); p += z;
    polyhedron->insertVertex(p); // Vertex 5
    p = polyhedron->getVertex(2); p += z;
    polyhedron->insertVertex(p); // Vertex 6
    p = polyhedron->getVertex(3); p += z;
    polyhedron->insertVertex(p); // Vertex 7

    //
    // Add the edges of the box.
    //

    // Bottom.
    polyhedron->insertEdge(0, 1);
    polyhedron->insertEdge(1, 3);
    polyhedron->insertEdge(3, 2);
    polyhedron->insertEdge(2, 0);

    // Top.
    polyhedron->insertEdge(4, 5);
    polyhedron->insertEdge(5, 7);
    polyhedron->insertEdge(7, 6);
    polyhedron->insertEdge(6, 4);

    // Sides.
    polyhedron->insertEdge(0, 4);
    polyhedron->insertEdge(1, 5);
    polyhedron->insertEdge(3, 7);
    polyhedron->insertEdge(2, 6);
  }
}




template<typename T>
inline
bool 
Edge<3,T>::
isInside(const Point& pt, const Number delta) const {
  // If the point is between two sides of the wedge.
  if (geom::computeDotProduct(pt - getSource(), _leftSideNormal) >= - delta &&
      geom::computeDotProduct(pt - getSource(), _rightSideNormal) >= - delta) {
    // If the point is between the top and bottom of the wedge.
    Number proj = geom::computeDotProduct(pt - getSource(), getTangent());
    if (proj >= - delta && proj <= getLength() + delta) {
      // The point is inside.
      return true;
    }
  }
  // If the point is not in the wedge.
  return false;
}



//
// Equality / Inequality
//

template<typename T>
inline
bool 
operator==(const Edge<3,T>& e1, const Edge<3,T>& e2) {
  if (e1.getSource() == e2.getSource() &&
      e1.getTarget() == e2.getTarget() &&
      e1.getTangent() == e2.getTangent() &&
      e1.getLength() == e2.getLength() &&
      e1.getLeftFaceNormal() == e2.getLeftFaceNormal() &&
      e1.getRightFaceNormal() == e2.getRightFaceNormal() &&
      e1.getLeftSideNormal() == e2.getLeftSideNormal() &&
      e1.getRightSideNormal() == e2.getRightSideNormal() &&
      e1.getFaceIndex() == e2.getFaceIndex() &&
      e1.getSignOfDistance() == e2.getSignOfDistance()) {
    return true;
  }
  return false;
}


template<typename T>
inline
bool 
operator!=(const Edge<3,T>& e1, const Edge<3,T>& e2) {
  return !(e1 == e2);
}

//
// File I/O
//

template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Edge<3,T>& edge) {
  return out << "Source:" << '\n'
	     << edge.getSource() << '\n'
	     << "Target:" << '\n'
	     << edge.getTarget() << '\n'
	     << "Tangent:" << '\n'
	     << edge.getTangent() << '\n'
	     << "Length:" << '\n'
	     << edge.getLength() << '\n'
	     << "Left Face Normal:" << '\n'
	     << edge.getLeftFaceNormal() << '\n'
	     << "Right Face Normal:" << '\n'
	     << edge.getRightFaceNormal() << '\n'
	     << "Left Side Normal:" << '\n'
	     << edge.getLeftSideNormal() << '\n'
	     << "Right Side Normal:" << '\n'
	     << edge.getRightSideNormal() << '\n'
	     << "Adjacent Face Index:" << '\n'
	     << edge.getFaceIndex() << '\n'
	     << "Sign Distance:" << '\n'
	     << edge.getSignOfDistance() << '\n';
}


END_NAMESPACE_CPT
