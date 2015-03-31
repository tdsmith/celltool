// -*- C++ -*-

#if !defined(__cpt_Vertex3_ipp__)
#error This file is an implementation detail of the class Vertex.
#endif

BEGIN_NAMESPACE_CPT

//! A vertex in 3-D.
template<typename T>
class Vertex<3,T> {
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

  //
  // Data
  //

  // The location of the vertex.
  Point _location;

  // The points that are closest to the vertex lie in a pyramid.
  // The edge directions of the pyramid.
  std::vector<Point> _edgeDirections;

  // An outward normal to the surface.
  Point _normal;

  // The sign of the distance.
  int _signOfDistance;

  // True if the surface is convex.
  bool _isConvex;
  
  // True if the surface is concave.
  bool _isConcave;
  
  // The index of an adjacent face.
  int _faceIndex;

public:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{
  
  //! Default constructor.
  Vertex() :
    _location(),
    _edgeDirections(),
    _normal(),
    _signOfDistance(),
    _isConvex(),
    _isConcave(),
    _faceIndex()
  {}

  //! Copy constructor.
  Vertex(const Vertex& other);

  //! Assignment operator.
  Vertex& 
  operator=(const Vertex& other);
  
  //! Construct from b-rep information.
  /*!
    Construct from point, vertex normal, neighboring points, 
    neighboring face normals and the index of an adjacent face.
    \c face_normals are given in positive order.
    For signed distance, the closest points to the vertex lie in a pyramid.
  */
  Vertex(const Point& pt, const Point& normal, 
	 const std::vector<Point>& neighbors, 
	 const std::vector<Point>& faceNormals, 
	 int faceIndex);

  //! Make from b-rep information.
  /*!
    Make from point, vertex normal, neighboring points, 
    neighboring face normals and the index of an adjacent face.
    \c faceNormals are given in positive order.
    For signed distance, the closest points to the vertex lie in a pyramid.
  */
  void
  make(const Point& pt, const Point& normal, 
       const std::vector<Point>& neighbors, 
       const std::vector<Point>& faceNormals, 
       int faceIndex);

  //! Make without b-rep information.
  /*!
    Make from a point and the index of an adjacent face.
    This is used for computing unsigned distance to a vertex on the boundary
    of the surface.
  */
  void
  make(const Point& pt, int faceIndex);

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

  //! Return true if the surface is convex at the vertex.
  bool 
  isConvex() const { 
    return _isConvex; 
  }

  //! Return true if the surface is concave at the vertex.
  bool 
  isConcave() const { 
    return _isConcave; 
  }

  //! Return the vector of edge directions in the pyramid of closest points.
  const std::vector<Point>& 
  getEdgeDirections() const { 
    return _edgeDirections; 
  }

  //! Return the index of an adjacent face.
  int 
  getFaceIndex() const { 
    return _faceIndex; 
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Return the distance to the supporting point of the vertex.
  Number 
  computeDistance(const Point& p) const;

  //! Return the distance to the supporting point of the vertex.
  /*!
    Use the vertex normal to determine the sign.
  */
  Number 
  computeDistanceChecked(const Point& p) const;

  //! Return the unsigned distance to the supporting point of the vertex.
  Number 
  computeDistanceUnsigned(const Point& p) const;

  //! Return the unsigned distance to the supporting point of the vertex.
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



  //! Make the polyhedron that contains the points of positive signed distance.
  void 
  buildCharacteristicPolyhedronPositive(Polyhedron* polyhedron, 
					Number height);
  
  //! Make the polyhedron that contains the points of negative signed distance.
  void 
  buildCharacteristicPolyhedronNegative(Polyhedron* polyhedron, 
					Number height);
  
  //@}

private:

  // Use the vertex normal to determine the sign of the distance.
  int
  computeSignOfDistance(const Point& p) const {
    return geom::computeDotProduct(_normal, Point(p - _location)) > 0 ? 1 : -1;
  }

  //! Given the sign of the distance: Return the distance.  Compute gradient of the distance.
  Number 
  computeGradient(const Point& p, Point* grad, int signOfDistance) const;

  // Make the characteristic polyhedron.
  void 
  buildCharacteristicPolyhedron(Polyhedron* polyhedron, 
				Number height);
};





//
// Constructors, Destructor.
//

  
template<typename T>
inline
Vertex<3,T>::
Vertex(const Point& pt, 
       const Point& normal, 
       const std::vector<Point>& neighbors, 
       const std::vector<Point>& faceNormals, 
       const int faceIndex) {
  make(pt, normal, neighbors, faceNormals, faceIndex) ;
}


template<typename T>
inline
void
Vertex<3,T>::
make(const Point& pt, 
     const Point& normal, 
     const std::vector<Point>& neighbors, 
     const std::vector<Point>& faceNormals, 
     const int faceIndex) {
  _location = pt;
  // The edge directions of the pyramid are the neighboring face normals.
  _edgeDirections = faceNormals;
  _normal = normal;
  _signOfDistance = 0;
  _isConvex = false;
  _isConcave = false;
  _faceIndex = faceIndex;

  assert(neighbors.size() >= 3 && neighbors.size() == faceNormals.size());
  assert(faceIndex >= 0);

  //  
  // Determine if it's convex, concave, or neither.
  // The surface is convex/concave at a vertex if each of the neighboring
  // vertices is on or below/above the plane defined by the point and the
  // normal.
  //
  Number dp;
  int below = 0, above = 0;
  Point neighborDirection;
  const int edgeDirectionsSize = int(_edgeDirections.size());
  // Loop through the neighboring vertex directions.
  for (int i = 0; i < edgeDirectionsSize; ++i) {
    // The direction of the i_th neighboring vertex.
    neighborDirection = neighbors[i];
    neighborDirection -= _location;
    dp = geom::computeDotProduct(_normal, neighborDirection);
    if (dp < 0) {
      ++below;
    }
    else if (dp > 0) {
      ++above;
    }
  }
  if (above == 0) {
    _isConvex = true;
    _signOfDistance = 1;
  }
  if (below == 0) {
    _isConcave = true;
    _signOfDistance = -1;
  }
}


template<typename T>
inline
void
Vertex<3,T>::
make(const Point& pt, const int faceIndex) {
  _location = pt;
  _edgeDirections.clear();
  _normal = 0.0;
  _signOfDistance = 0;
  _isConvex = false;
  _isConcave = false;
  _faceIndex = faceIndex;
}


//
// Copy constructor.
//


template<typename T>
inline
Vertex<3,T>::
Vertex(const Vertex& other) :
  _location(other._location),
  _edgeDirections(other._edgeDirections),
  _normal(other._normal),
  _signOfDistance(other._signOfDistance),
  _isConvex(other._isConvex),
  _isConcave(other._isConcave),
  _faceIndex(other._faceIndex)
{}


//
// Assignment operator.
//


template<typename T>
inline
Vertex<3,T>& 
Vertex<3,T>::
operator=(const Vertex& other) {
  // Avoid assignment to self
  if (&other != this) {
    _location = other._location;
    _edgeDirections= other._edgeDirections;
    _normal = other._normal;
    _signOfDistance = other._signOfDistance;
    _isConvex = other._isConvex;
    _isConcave = other._isConcave;
    _faceIndex = other._faceIndex;
  }

  // Return *this so assignments can chain
  return *this;
}


//
// Mathematical operations
//


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeDistance(const Point& p) const {
  // If the sign of the distance is not known, find it.
  if (_signOfDistance == 0) {
    return computeDistanceChecked(p);
  }
  // Return the sign of the distance times the unsigned distance.
  return _signOfDistance * geom::computeDistance(p, _location);
}
  

template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeDistanceChecked(const Point& p) const {
  // Return the sign of the distance times the unsigned distance.
  return computeSignOfDistance(p) * geom::computeDistance(p, _location);
}
  

template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeDistanceUnsigned(const Point& p) const {
  return geom::computeDistance(p, _location);
}



  
template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeClosestPoint(const Point& p, Point* cp) const {
  *cp = _location;
  return computeDistance(p);
}


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeClosestPointChecked(const Point& p, Point* cp) const {
  *cp = _location;
  return computeDistanceChecked(p);
}


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeClosestPointUnsigned(const Point& p, Point* cp) const {
  *cp = _location;
  return computeDistanceUnsigned(p);
}




template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeGradient(const Point& p, Point* grad) const
{
  // If the sign of the distance is not known, find it.
  if (_signOfDistance == 0) {
    computeGradientChecked(p, grad);
  }
  // Call the private function with the sign of the distance.
  return computeGradient(p, grad, _signOfDistance);
}


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeGradientChecked(const Point& p, Point* grad) const {
  // Call the private function with the sign of the distance.
  return computeGradient(p, grad, computeSignOfDistance(p));
}


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
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
    // gradient of the distance to be the normal to an adjacent face.
    *grad = getEdgeDirections()[0];
    int signOfDistance = _signOfDistance;
    // if the sign of the distance is not known, find it.
    if (signOfDistance == 0) {
      signOfDistance = computeSignOfDistance(p);
    }
    *grad *= signOfDistance;
  }
  return computeDistanceUnsigned(p);
}




template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeClosestPointAndGradient(const Point& p, Point* cp, 
			       Point* grad) const {
  *cp = _location;
  return computeGradient(p, grad);
}


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
				      Point* grad) const {
  *cp = _location;
  return computeGradientChecked(p, grad);
}


template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
				       Point* grad) const {
  *cp = _location;
  return computeGradientUnsigned(p, grad);
}




// Make the pyramid that contains the closest points of positive distance 
// to the vertex.
template<typename T>
inline
void
Vertex<3,T>::
buildCharacteristicPolyhedronPositive(Polyhedron* polyhedron, 
				      const Number height) {
  // Set the sign of the distance.
  _signOfDistance = 1;

  // Make the polyhedron.
  buildCharacteristicPolyhedron(polyhedron, height);
}


// Make the pyramid that contains the closest points of positive distance 
// to the vertex.
template<typename T>
inline
void
Vertex<3,T>::
buildCharacteristicPolyhedronNegative(Polyhedron* polyhedron, 
				      const Number height) {
  // Set the sign of the distance.
  _signOfDistance = -1;

  // Reverse the edge directions and surface normal.
  for (typename std::vector<Point>::iterator i 
	 = _edgeDirections.begin();
       i != _edgeDirections.end(); ++i) {
    (*i).negate();
  }
  _normal.negate();

  // Make the polyhedron.
  buildCharacteristicPolyhedron(polyhedron, height);

  // Return the edge directions and surface normal to original directions.
  for (typename std::vector<Point>::iterator i 
	 = _edgeDirections.begin();
       i != _edgeDirections.end(); ++i) {
    (*i).negate();
  }
  _normal.negate();
}



template<typename T>
inline
typename Vertex<3,T>::Number
Vertex<3,T>::
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
    // gradient of the distance to be the normal to an adjacent face.
    *grad = getEdgeDirections()[0];
  }
  return signOfDistance * geom::computeDistance(p, _location);
}



// Make the pyramid that contains the closest points to the vertex.
template<typename T>
inline
void
Vertex<3,T>::
buildCharacteristicPolyhedron(Polyhedron* polyhedron, 
			      const Number height) {
  // Clear the polyhedron->
  polyhedron->clear();
  
  // If the surface is flat or if degenerate, do nothing.
  if ((_isConvex && _isConcave) || getEdgeDirections().size() < 3) {
    return;
  }

  // _normal is the average of the edge directions.
  // CONTINUE:  Get a better formula for the average which will minimize
  // the maximum angle.
  Point average = _normal;

  //
  // Find the maximum angle between the average direction and the 
  // edge directions.
  //
  Number minimumCosineAngle = 1;
    
  Number cosineAngle;
  for (typename std::vector<Point>::const_iterator edgeIterator 
	 = _edgeDirections.begin();
       edgeIterator != _edgeDirections.end(); 
       ++edgeIterator) {
    cosineAngle = geom::computeDotProduct(*edgeIterator, average);
    if (cosineAngle < minimumCosineAngle) {
      minimumCosineAngle = cosineAngle;
    }
  }

  // If the vertex is not pointy.
  if (minimumCosineAngle > 0.2) {

    //
    // Make a pyramid containing the closest grid points.
    //

    // CONTINUE: Make a polyhedron that is smaller.  Consider a diamond 
    // shape with 8 edges.

    //      
    // Make a unit vector that is orthogonal to average.
    //
    Point orthogonalToAverage;
    // If the z coordinate is biggest.
    if (std::abs(average[2]) > std::abs(average[0]) &&
	std::abs(average[2]) > std::abs(average[1])) {
      orthogonalToAverage = Point(- average[2], 0, average[0]);
    }
    // The x or y coordinate is biggest.
    else {
      orthogonalToAverage = Point(- average[1], average[0], 0);
    }
    // Make it a unit vector.
    geom::normalize(&orthogonalToAverage);
    // Make a vector orthogonal to average and orthogonalToAverage.
    Point orthogonalToBoth = 
      geom::computeCrossProduct(average, orthogonalToAverage);

    // Make average the correct length.
    average *= height;
    // Make the orthogonals the correct length.
    Number orthogonalLength 
      = std::sqrt(2.0) * height * 
      std::sqrt(1 - minimumCosineAngle * minimumCosineAngle) / 
      minimumCosineAngle;
    orthogonalToAverage *= orthogonalLength;
    orthogonalToBoth *= orthogonalLength;
    
    //
    // Add the vertices of the pyramid.
    //
    Point baseCenter = getLocation(); baseCenter += average;
    Point p;
    p = baseCenter; p += orthogonalToAverage; p += orthogonalToBoth;
    polyhedron->insertVertex(p); // Vertex 0
    p = baseCenter; p += orthogonalToAverage; p -= orthogonalToBoth;
    polyhedron->insertVertex(p); // Vertex 1
    p = baseCenter; p -= orthogonalToAverage; p -= orthogonalToBoth;
    polyhedron->insertVertex(p); // Vertex 2
    p = baseCenter; p -= orthogonalToAverage; p += orthogonalToBoth;
    polyhedron->insertVertex(p); // Vertex 3

    polyhedron->insertVertex(getLocation()); // Vertex 4

    // Make the polyhedron->
    polyhedron->insertEdge(4, 0);
    polyhedron->insertEdge(4, 1);
    polyhedron->insertEdge(4, 2);
    polyhedron->insertEdge(4, 3);

    polyhedron->insertEdge(0, 1);
    polyhedron->insertEdge(1, 2);
    polyhedron->insertEdge(2, 3);
    polyhedron->insertEdge(3, 0);
  }
  // If the vertex is too pointy.
  else {
    // Indicate that the sign of the distance of scan converted is not known.
    _signOfDistance = 0;

    //
    // Add the vertices of the cube containing the closest grid points.
    //
    
    Point p;
    p = getLocation(); p[0] -= height; p[1] -= height; p[2] -= height;
    polyhedron->insertVertex(p); // Vertex 0
    p = getLocation(); p[0] += height; p[1] -= height; p[2] -= height;
    polyhedron->insertVertex(p); // Vertex 1
    p = getLocation(); p[0] += height; p[1] += height; p[2] -= height;
    polyhedron->insertVertex(p); // Vertex 2
    p = getLocation(); p[0] -= height; p[1] += height; p[2] -= height;
    polyhedron->insertVertex(p); // Vertex 3
    p = getLocation(); p[0] -= height; p[1] -= height; p[2] += height;
    polyhedron->insertVertex(p); // Vertex 4
    p = getLocation(); p[0] += height; p[1] -= height; p[2] += height;
    polyhedron->insertVertex(p); // Vertex 5
    p = getLocation(); p[0] += height; p[1] += height; p[2] += height;
    polyhedron->insertVertex(p); // Vertex 6
    p = getLocation(); p[0] -= height; p[1] += height; p[2] += height;
    polyhedron->insertVertex(p); // Vertex 7

    //
    // Add the edges.
    //

    // Bottom
    polyhedron->insertEdge(0, 1);
    polyhedron->insertEdge(1, 2);
    polyhedron->insertEdge(2, 3);
    polyhedron->insertEdge(3, 0);

    // Sides
    polyhedron->insertEdge(0, 4);
    polyhedron->insertEdge(1, 5);
    polyhedron->insertEdge(2, 6);
    polyhedron->insertEdge(3, 7);

    // Top
    polyhedron->insertEdge(4, 5);
    polyhedron->insertEdge(5, 6);
    polyhedron->insertEdge(6, 7);
    polyhedron->insertEdge(7, 0);
  }
}


//
// Equality / Inequality
//


template<typename T>
inline
bool 
operator==(const Vertex<3,T>& a, const Vertex<3,T>& b) {
  if (!(a.getLocation() == b.getLocation() &&
	a.isConvex() == b.isConvex() &&
	a.isConcave() == b.isConcave() &&
	a.getFaceIndex() == b.getFaceIndex())) {
    return false;
  }
  
  if (a.getEdgeDirections().size() != b.getEdgeDirections().size()) {
    return false;
  }
  typename std::vector<typename Vertex<3,T>::Point>::const_iterator i, j;
  for (i = a.getEdgeDirections().begin(), j = b.getEdgeDirections().begin();
       i != a.getEdgeDirections().end();
       ++i, ++j) {
    if (*i != *j) {
      return false;
    }
  }

  return true;
}


//
// File I/O
//


template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Vertex<3,T>& vertex) {
  out << "Location:" << '\n'
      << vertex.getLocation() << '\n'
      << "Adjacent Face Index:" << '\n'
      << vertex.getFaceIndex() << '\n'
      << "Is Convex:" << '\n'
      << vertex.isConvex() << '\n'
      << "Is Concave:" << '\n'
      << vertex.isConcave() << '\n';
  return out;
}

END_NAMESPACE_CPT
