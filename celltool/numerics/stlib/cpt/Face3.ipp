// -*- C++ -*-

#if !defined(__cpt_Face3_ipp__)
#error This file is an implementation detail of the class Face.
#endif

BEGIN_NAMESPACE_CPT

//! A 3-D face on a b-rep.
template<typename T>
class Face<3,T> {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! A Cartesian point.
  typedef ads::FixedArray<3,Number> Point;

  //! An indexed edge polyhedron type.
  typedef geom::IndexedEdgePolyhedron<Number> Polyhedron;

private:

  //! The representation of a plane in 3 dimensions.
  typedef geom::Plane<Number> Plane;

private:

  // 
  // Member Data
  //

  //! The three vertices of the face.
  ads::FixedArray<3,Point> _vertices;

  //! The supporting plane of the face.
  Plane _supportingPlane;

  /*! The three planes which are incident on the three edges and orthogonal
    to the face.  These planes have inward pointing normals. */
  ads::FixedArray<3,Plane> _sides;

  //! The index of this face.
  int _index;

  //! An epsilon that is appropriate for the edge lengths.
  Number _epsilon;

public:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.  Unititialized memory.
  Face() :
    _vertices(),
    _supportingPlane(),
    _sides(),
    _index(),
    _epsilon()
  {}

  //! Construct a face from three vertices, a normal and the face index.
  Face(const Point& vertex1, const Point& vertex2, 
       const Point& vertex3, const Point& normal,
       const int index) { 
    make(vertex1, vertex2, vertex3, normal, index); 
  }

  //! Copy constructor.
  Face(const Face& other) :
    _vertices(other._vertices),
    _supportingPlane(other._supportingPlane),
    _sides(other._sides),
    _index(other._index),
    _epsilon(other._epsilon)
  {}

  //! Assignment operator.
  Face& 
  operator=(const Face& other);

  //! Make a face from three vertices, a normal and the face index.
  void 
  make(const Point& vertex1, const Point& vertex2, 
       const Point& vertex3, const Point& normal,
       const int index);

  //! Trivial destructor.
  ~Face()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{
    
  //! Return the vertices.
  const ads::FixedArray<3,Point>& 
  getVertices() const { 
    return _vertices; 
  }

  //! Return the face.
  const Plane& 
  getSupportingPlane() const { 
    return _supportingPlane; 
  }

  //! Return the sides.
  const ads::FixedArray<3,Plane>& 
  getSides() const { 
    return _sides; 
  }

  //! Return the normal to the face.
  const Point& 
  getNormal() const { 
    return _supportingPlane.getNormal(); 
  }

  //! Return the i_th side normal.
  const Point& 
  getSideNormal(const int i) const { 
    return _sides[i].getNormal(); 
  }

  //! Return the index of this face in the b-rep.
  int 
  getFaceIndex() const { 
    return _index; 
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Return true if the face is valid.
  bool 
  isValid() const;



  //! Return the signed distance to the supporting plane of the face.
  Number 
  computeDistance(const Point& p) const { 
    return _supportingPlane.computeSignedDistance(p); 
  }
    
  //! Compute distance with checking.
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number
  computeDistanceChecked(const Point& p) const;

  //! Return the unsigned distance to the supporting plane of the face.
  Number 
  computeDistanceUnsigned(const Point& p) const { 
    return std::abs(computeDistance(p)); 
  }

  //! Return the unsigned distance to the face.
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeDistanceUnsignedChecked(const Point& p) const { 
    return std::abs(computeDistanceChecked(p)); 
  }


    
  //! Return the distance and find the closest point.
  Number 
  computeClosestPoint(const Point& p, Point* cp) const { 
    return _supportingPlane.computeSignedDistanceAndClosestPoint(p, cp); 
  }

  //! Return the distance and find the closest point.
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeClosestPointChecked(const Point& p, Point* cp) const;

  //! Return the unsigned distance and find the closest point.
  Number 
  computeClosestPointUnsigned(const Point& p, Point* cp) const { 
    return std::abs(computeClosestPoint(p, cp));
  }

  //! Return the unsigned distance and find the closest point.
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeClosestPointUnsignedChecked(const Point& p, Point* cp) const { 
    return std::abs(computeClosestPointChecked(p, cp));
  }



  //! Return the distance and find the gradient of the distance.
  Number 
  computeGradient(const Point& p, Point* grad) const;

  //! Return the distance and find the gradient of the distance.
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeGradientChecked(const Point& p, Point* grad) const;

  //! Return the unsigned distance and find the gradient of the unsigned distance.
  Number 
  computeGradientUnsigned(const Point& p, Point* grad) const;

  //! Return the unsigned distance and find the gradient of the unsigned distance.
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeGradientUnsignedChecked(const Point& p, Point* grad) const;



  //! Return the distance and find the closest point and gradient of distance
  Number 
  computeClosestPointAndGradient(const Point& p, Point* cp, Point* grad) const;

  //! Return the distance and find the closest point and gradient of distance
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
				     Point* grad) const;

  //! Return the distance and find the closest point and gradient of distance
  Number 
  computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
				      Point* grad) const;

  //! Return the distance and find the closest point and gradient of distance
  /*!
    The points that are closest to the face lie in a triangular prizm.
    If the point, p, is within a distance, delta, of being inside the prizm
    then return the distance.  Otherwise return infinity.
  */
  Number 
  computeClosestPointAndGradientUnsignedChecked(const Point& p, Point* cp,
					     Point* grad) const;



  //! Make the characteristic polyhedron containing the closest points.
  /*!
    The face is a triangle.  Consider the larger triangle made by
    moving the sides outward by delta.  Make a triangular prizm of
    height, 2 * height, with the given triangle at its center.
  */
  void 
  buildCharacteristicPolyhedron(Polyhedron* polyhedron, 
				const Number height) const;

  //@}

private:

  //! Return true if the point is within delta of being inside the prizm of closest points
  bool 
  isInside(const Point& p, const Number delta) const { 
    return (_sides[0].computeSignedDistance(p) >= - delta &&
	    _sides[1].computeSignedDistance(p) >= - delta &&
	    _sides[2].computeSignedDistance(p) >= - delta); 
  }

  //! Return true if the point is (close to being) inside the prizm of closest points
  bool 
  isInside(const Point& p) const { 
    return isInside(p, _epsilon);
  }

};




//
// Assignment operator.
//

template<typename T>
inline
Face<3,T>& 
Face<3,T>::
operator=(const Face& other) {
  // Avoid assignment to self
  if (&other != this) {
    _vertices = other._vertices;
    _supportingPlane = other._supportingPlane;
    _sides = other._sides;
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
Face<3,T>::
make(const Point& a, const Point& b, const Point& c, 
     const Point& nm, const int index) {
  // The three vertices comprising the face.
  _vertices[0] = a;
  _vertices[1] = b;
  _vertices[2] = c;
  
  // Make the face plane from a point and the normal.
  _supportingPlane = Plane(a, nm);
  
  // Make the sides of the prizm from three points each.
  _sides[0] = Plane(b, a, a + _supportingPlane.getNormal());
  _sides[1] = Plane(c, b, b + _supportingPlane.getNormal());
  _sides[2] = Plane(a, c, c + _supportingPlane.getNormal());

  // The index of this face.
  _index = index;

  // An appropriate epsilon for this face.  max_length * sqrt(eps)
  _epsilon = std::sqrt(ads::max(geom::computeSquaredDistance(a, b),
				geom::computeSquaredDistance(b, c),
				geom::computeSquaredDistance(c, a)) *
		       std::numeric_limits<Number>::epsilon());
}


//
// Mathematical Operations
//


template<typename T>
inline
bool
Face<3,T>::
isValid() const {
  // Check the plane of the face.
  if (! _supportingPlane.isValid()) {
    return false;
  }

  // Check the planes of the sides.
  for (int i = 0; i < 3; ++i) {
    if (! _sides[i].isValid()) {
      return false;
    }
  }

  // The index should be non-negative.
  if (_index < 0) {
    return false;
  }

  // Check that the normal points in the correct direction.
  const Number eps = 10 * std::numeric_limits<Number>::epsilon();
  if (std::abs(geom::computeDotProduct(_supportingPlane.getNormal(), 
				       _vertices[0] - _vertices[1])) > eps ||
      std::abs(geom::computeDotProduct(_supportingPlane.getNormal(), 
				       _vertices[1] - _vertices[2])) > eps ||
      std::abs(geom::computeDotProduct(_supportingPlane.getNormal(), 
				       _vertices[2] - _vertices[0])) > eps) {
    return false;
  }

  // Check that the side normals point in the correct direction.
  if (std::abs(geom::computeDotProduct(_supportingPlane.getNormal(), 
				       _sides[0].getNormal())) > eps ||
      std::abs(geom::computeDotProduct(_supportingPlane.getNormal(), 
				       _sides[1].getNormal())) > eps ||
      std::abs(geom::computeDotProduct(_supportingPlane.getNormal(), 
				       _sides[2].getNormal())) > eps) {
    return false;
  }
  if (std::abs(geom::computeDotProduct(_sides[0].getNormal(), 
				       _vertices[0] - _vertices[1])) > eps ||
      std::abs(geom::computeDotProduct(_sides[1].getNormal(), 
				       _vertices[1] - _vertices[2])) > eps ||
      std::abs(geom::computeDotProduct(_sides[2].getNormal(), 
				       _vertices[2] - _vertices[0])) > eps) {
    return false;
  }

  return true;
}


template<typename T>
inline
typename Face<3,T>::Number
Face<3,T>::
computeDistanceChecked(const Point& p) const {
  // If the point is inside the characteristic prizm.
  if (isInside(p)) {
    // Then compute the distance.
    return computeDistance(p);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<3,T>::Number
Face<3,T>::
computeClosestPointChecked(const Point& p, Point* cp) const {
  // If the point is inside the characteristic prizm.
  if (isInside(p)) {
    // Then compute the distance and closest point to the supporting plane 
    // of the face.
    return computeClosestPoint(p, cp);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeGradient(const Point& p, Point* grad) const {
  *grad = getNormal();
  return _supportingPlane.computeSignedDistance(p);
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeGradientChecked(const Point& p, Point* grad) const {
  // If the point is inside the characteristic prizm.
  if (isInside(p)) {
    // Then compute the distance and gradient.
    return computeGradient(p, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeGradientUnsigned(const Point& p, Point* grad) const {
  const Number signedDistance = _supportingPlane.computeSignedDistance(p);
  *grad = getNormal();
  if (signedDistance < 0) {
    grad->negate();
  }
  return std::abs(signedDistance);
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeGradientUnsignedChecked(const Point& p, Point* grad) const {
  // If the point is inside the characteristic prizm.
  if (isInside(p)) {
    // Then compute the distance and gradient.
    return computeGradientUnsigned(p, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeClosestPointAndGradient(const Point& p, Point* cp, Point* grad) const {
  *grad = getNormal();
  return _supportingPlane.computeSignedDistanceAndClosestPoint(p, cp);
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
				   Point* grad) const {
  // If the point is inside the characteristic prizm.
  if (isInside(p)) {
    // Then compute the distance, closest point, and gradient.
    return computeClosestPointAndGradient(p, cp, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
				    Point* grad) const {
  Number signedDistance = 
    _supportingPlane.computeSignedDistanceAndClosestPoint(p, cp);
  *grad = getNormal();
  if (signedDistance < 0) {
    grad->negate();
  }
  return std::abs(signedDistance);
}


template<typename T>
inline
typename Face<3,T>::Number 
Face<3,T>::
computeClosestPointAndGradientUnsignedChecked(const Point& p, Point* cp,
					   Point* grad) const {
  // If the point is inside the characteristic prizm.
  if (isInside(p)) {
    // Then compute the distance, closest point, and gradient.
    return computeClosestPointAndGradientUnsigned(p, cp, grad);
  }
  // Otherwise, return infinity.
  return std::numeric_limits<Number>::max();
}



// Utility Function.
// Offset of a point so that the sides with normals n1 and n2 are moved
// a distance of delta.
template<typename T>
ads::FixedArray<3,T>
offsetOutward(const ads::FixedArray<3,T>& n1, 
	      const ads::FixedArray<3,T>& n2, 
	      const T delta) {
  ads::FixedArray<3,T> outward(n1 + n2);
  geom::normalize(&outward);
  T den = geom::computeDotProduct(outward, n1);
  if (den != 0) {
    return (outward * (delta / den));
  }
  return ads::FixedArray<3,T>(0, 0, 0);
}



/*
  The face is a triangle.  Consider the larger triangle made by moving 
  the sides outward by delta.
  Make a triangular prizm of height, 2 * height, 
  with the given triangle at its center. 
*/
template<typename T>
inline
void
Face<3,T>::
buildCharacteristicPolyhedron(Polyhedron* polyhedron, 
			      const Number height) const {
  polyhedron->clear();

  // The initial triangular face.  Points on the bottom of the prizm.
  Point heightOffset = height * getNormal();
  Point bot0(getVertices()(0));
  bot0 -= heightOffset;
  Point bot1(getVertices()(1));
  bot1 -= heightOffset;
  Point bot2(getVertices()(2));
  bot2 -= heightOffset;

  // Compute the amount (delta) to enlarge the triangle.
  Number maximumCoordinate = 1.0;
  // Loop over the three vertices.
  for (int i = 0; i != 3; ++i) {
    // Loop over the space coordinates.
    for (int j = 0; j != 3; ++j) {
    maximumCoordinate = std::max(maximumCoordinate, 
				 std::abs(getVertices()[i][j]));
    }
  }
  // Below, 10 is the fudge factor.
  const Number delta = 
    maximumCoordinate * 10.0 * std::numeric_limits<Number>::epsilon();
  
  // Enlarge the triangle.
  Point out0(getSideNormal(0));
  out0.negate();
  Point out1(getSideNormal(1));
  out1.negate();
  Point out2(getSideNormal(2));
  out2.negate();
  bot0 += offsetOutward(out0, out2, delta);
  bot1 += offsetOutward(out1, out0, delta);
  bot2 += offsetOutward(out2, out1, delta);

  // Make the top of the prizm.
  heightOffset = 2 * height * getNormal();
  Point top0(bot0 + heightOffset);
  Point top1(bot1 + heightOffset);
  Point top2(bot2 + heightOffset);
  
  // Add the vertices of the triangular prizm.
  polyhedron->insertVertex(bot0); // 0
  polyhedron->insertVertex(bot1); // 1
  polyhedron->insertVertex(bot2); // 2
  polyhedron->insertVertex(top0); // 3
  polyhedron->insertVertex(top1); // 4
  polyhedron->insertVertex(top2); // 5

  // Add the edges of the triangular prizm.
  polyhedron->insertEdge(0, 1);
  polyhedron->insertEdge(1, 2);
  polyhedron->insertEdge(2, 0);

  polyhedron->insertEdge(3, 4);
  polyhedron->insertEdge(4, 5);
  polyhedron->insertEdge(5, 3);

  polyhedron->insertEdge(0, 3);
  polyhedron->insertEdge(1, 4);
  polyhedron->insertEdge(2, 5);
}


//
// Equality / Inequality
//


template<typename T>
inline
bool 
operator==(const Face<3,T>& f1, const Face<3,T>& f2) {
  if (f1.getVertices()[0] == f2.getVertices()[0] &&
      f1.getVertices()[1] == f2.getVertices()[1] &&
      f1.getVertices()[2] == f2.getVertices()[2] &&
      f1.getSupportingPlane() == f2.getSupportingPlane() &&
      f1.getSides()[0] == f2.getSides()[0] &&
      f1.getSides()[1] == f2.getSides()[1] &&
      f1.getSides()[2] == f2.getSides()[2] &&
      f1.getFaceIndex() == f2.getFaceIndex()) {
    return true;
  }
  return false;
}


//
// File I/O
//


template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Face<3,T>& face) {
  out << "Vertices:" << '\n'
      << face.getVertices() << '\n'
      << "Supporting Plane:" << '\n'
      << face.getSupportingPlane() << '\n'
      << "Sides:" << '\n'
      << face.getSides() << '\n'
      << "Face index:" << '\n'
      << face.getFaceIndex() << '\n';
  return out;
}

END_NAMESPACE_CPT
