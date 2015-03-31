// -*- C++ -*-

#if !defined(__geom_ISS_SignedDistance_2_ipp__)
#error This file is an implementation detail of the class ISS_SignedDistance.
#endif

BEGIN_NAMESPACE_GEOM

//! Signed distance to a polygon in 2-D.
/*!
  \param ISS is the indexed simplex set.

  This class stores a constant reference to an indexed simplex set.
*/
template<class ISS>
class ISS_SignedDistance<ISS,2> {
  //
  // Private types.
  //

private:

  //! The indexed simplex set.
  typedef ISS IssType;

  //! The (un-indexed) simplex type.
  typedef typename IssType::Simplex Simplex;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename IssType::Number Number;
  //! A vertex.
  typedef typename IssType::Vertex Vertex;
  //! A bounding box.
  typedef geom::BBox<ISS::N,Number> BBox;

  //
  // Member data.
  //

private:

  //! The indexed simplex set.
  const IssType& _iss;
  //! The vertex normals.
  ads::Array<1,Vertex> _vertexNormals;
  //! The face normals.
  ads::Array<1,Vertex> _faceNormals;
  //! Bounding box tree.
  BBoxTree<ISS::N,Number> _bboxTree;
  

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  ISS_SignedDistance();

  //! Copy constructor not implemented
  ISS_SignedDistance(const ISS_SignedDistance&);

  //! Assignment operator not implemented
  ISS_SignedDistance& 
  operator=(const ISS_SignedDistance&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Construct from the indexed simplex set.
  /*!
    \param iss is the indexed simplex set.
  */
  ISS_SignedDistance(const IssType& iss) :
    _iss(iss),
    _vertexNormals(_iss.getVerticesSize()),
    _bboxTree() {
    build();
  }

  //! Destructor has no effect on the indexed simplex set.
  ~ISS_SignedDistance()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Signed distance.
  //! @{

  //! Return the signed distance to the manifold.
  Number
  operator()(const Vertex& x) const {
    //CONTINUE: Should I use static?
    static Vertex closestPoint;
    return operator()(x, &closestPoint);
  }

  //! Return the signed distance to the manifold.  Compute the closest point.
  Number
  operator()(const Vertex& x, Vertex* closestPoint) const {
    static Vertex normal;
    return computeClosestPointNormal(x, closestPoint, &normal);
  }

  //! Return the signed distance to the manifold.  Compute the closest point, gradient of the distance and the index of the closest simplex.
  Number
  operator()(const Vertex& x, Vertex* closestPoint, Vertex* gradient, 
	     int* index) const;

  //! Return the closest point on the manifold.
  const Vertex&
  computeClosestPoint(const Vertex& x) const {
    static Vertex closestPoint;
    operator()(x, &closestPoint);
    return closestPoint;
  }

  //! Return the normal at the closest point on the manifold.
  const Vertex&
  computeNormal(const Vertex& x) const {
    static Vertex closestPoint, normal;
    computeClosestPointNormal(x, &closestPoint, &normal);
    return normal;
  }

  //! Return the signed distance to the manifold.  Compute the closest point and the normal.
  Number
  computeClosestPointNormal(const Vertex& x, Vertex* closestPoint, 
			    Vertex* normal) const;

  //! @}

  //
  // Private member functions.
  //

private:

  //! Build the bounding box tree.
  void
  build();
};




//
// Signed distance.
//


// Return the signed distance to the manifold.  Compute the closest point, gradient of the distance and the index of the closest simplex.
template<class ISS>
inline
typename ISS_SignedDistance<ISS,2>::Number
ISS_SignedDistance<ISS,2>::
operator()(const Vertex& x, Vertex* closestPoint, Vertex* gradient, 
	   int* index) const {
  // CONTINUE: Should I make the static variables into class member data?
  static Simplex s;
  static Vertex v;
  static std::vector<int> candidateEdges;
  static std::map<int,int> candidateVertices;
  int n;
  Number d, mag;

  assert(_iss.getSimplicesSize() != 0);

  //
  // Get the candidate simplices.
  //
  candidateEdges.clear();
  _bboxTree.computeMinimumDistanceQuery(std::back_inserter(candidateEdges), x);

  //
  // Calculate the distance to the candidate simplices.
  //
  Number minDistance = std::numeric_limits<Number>::max();
  const int iEnd = int(candidateEdges.size());
  for (int i = 0; i != iEnd; ++i) {
    n = candidateEdges[i];
    // Get the simplex.
    _iss.getSimplex(n, &s);
    // Calculate the signed distance to the simplex.
    d = computeSignedDistance(s, x, &v);
    // If this is the current minimum distance.
    if (std::abs(d) < std::abs(minDistance)) {
      // Record the distance and closest point.
      minDistance = d;
      *closestPoint = v;
      *gradient = _faceNormals[n];
      // Closest simplex index.
      *index = n;
    }
  }

  //
  // Build the candidate vertices.
  //
  candidateVertices.clear();
  for (int i = 0; i != iEnd; ++i) {
    n = candidateEdges[i];
    candidateVertices.insert(std::pair<const int, int>
			     (_iss.getIndexedSimplices()[n][0], n));
    candidateVertices.insert(std::pair<const int, int>
			     (_iss.getIndexedSimplices()[n][1], n));
  }

  //
  // Calculate the distance to the candidate vertices.
  //
  std::map<int,int>::iterator vEnd = candidateVertices.end();
  std::map<int,int>::iterator v_iter = candidateVertices.begin();
  for (; v_iter != vEnd; ++v_iter) {
    n = v_iter->first;
    // Calculate the signed distance to the vertex.
    d = computeSignedDistance(_iss.getVertices()[n], _vertexNormals[n], x);
    // If this is the current minimum distance.
    if (std::abs(d) < std::abs(minDistance)) {
      // Record the distance, closest point, and normal.
      minDistance = d;
      *closestPoint = _iss.getVertices()[n];
      // Gradient of the distance.
      *gradient = x;
      *gradient -= *closestPoint;
      mag = geom::computeMagnitude(*gradient);
      if (mag != 0) {
	*gradient /= mag;
      }
      else {
	*gradient = _vertexNormals[n];
      }
      if (d < 0) {
	(*gradient).negate();
      }
      // Closest simplex index.
      *index = v_iter->second;
    }
  }
    
  assert(minDistance != std::numeric_limits<Number>::max());
  return minDistance;
}







// Return the signed distance to the manifold.  Compute the closest point
// and the normal.
template<class ISS>
inline
typename ISS_SignedDistance<ISS,2>::Number
ISS_SignedDistance<ISS,2>::
computeClosestPointNormal(const Vertex& x, Vertex* closestPoint, 
			  Vertex* normal) const {
  // CONTINUE: Should I make the static variables into class member data?
  static Simplex s;
  static std::vector<int> candidateEdges;
  static std::set<int> candidateVertices;
  int n;
  Number d;

  assert(_iss.getSimplicesSize() != 0);

  //
  // Get the candidate simplices.
  //
  candidateEdges.clear();
  _bboxTree.computeMinimumDistanceQuery(std::back_inserter(candidateEdges), x);

  //
  // Calculate the distance to the candidate simplices.
  //
  // Initialize the normal to avoid possible division by zero later.
  *normal = 1.0;
  Number minDistance = std::numeric_limits<Number>::max();
  Vertex cp;
  const int iEnd = int(candidateEdges.size());
  for (int i = 0; i != iEnd; ++i) {
    n = candidateEdges[i];
    // Get the simplex.
    _iss.getSimplex(n, &s);
    // Calculate the signed distance to the simplex.
    d = computeSignedDistance(s, x, &cp);
    // If this is the current minimum distance.
    if (std::abs(d) < std::abs(minDistance)) {
      // Record the distance and closest point.
      minDistance = d;
      *closestPoint = cp;
      *normal = _faceNormals[n];
    }
  }

  //
  // Build the candidate vertices.
  //
  candidateVertices.clear();
  for (int i = 0; i != iEnd; ++i) {
    n = candidateEdges[i];
    candidateVertices.insert(_iss.getIndexedSimplices()[n][0]);
    candidateVertices.insert(_iss.getIndexedSimplices()[n][1]);
  }  

  //
  // Calculate the distance to the candidate vertices.
  //
  std::set<int>::iterator vEnd = candidateVertices.end();
  std::set<int>::iterator vIter = candidateVertices.begin();
  for (; vIter != vEnd; ++vIter) {
    n = *vIter;
    // Calculate the signed distance to the vertex.
    d = computeSignedDistance(_iss.getVertices()[n], _vertexNormals[n], x);
    // If this is the current minimum distance.
    if (std::abs(d) < std::abs(minDistance)) {
      // Record the distance, closest point, and normal.
      minDistance = d;
      *closestPoint = _iss.getVertices()[n];
      *normal = _vertexNormals[n];
    }
  }
    
  assert(minDistance != std::numeric_limits<Number>::max());
  return minDistance;
}



//
// Private member functions.
//



template<class ISS>
inline
void
ISS_SignedDistance<ISS,2>::
build() {
  //
  // Build the bounding box tree.
  //

  ads::Array<1, BBox> boxes(_iss.getSimplicesSize());
  Simplex simplex;
  // For each simplex.
  for (int n = 0; n != _iss.getSimplicesSize(); ++n) {
    _iss.getSimplex(n, &simplex);
    // Make a bounding box around the simplex.
    boxes[n].bound(simplex.begin(), simplex.end());
  }
  // Build the tree from the bounding boxes.
  _bboxTree.build(boxes.begin(), boxes.end());

  //
  // Build the vertex normals.
  //

  // Compute the face normals.
  _faceNormals.resize(_iss.getSimplicesSize());
  Vertex normal;
  // For each simplex.
  for (int n = 0; n != _iss.getSimplicesSize(); ++n) {
    _iss.getSimplex(n, &simplex);
    normal = simplex[1];
    normal -= simplex[0];
    rotateMinusPiOver2(&normal);
    normalize(&normal);
    _faceNormals[n] = normal;
  }

  // Calculate the vertex normal directions.
  _vertexNormals = Vertex(0, 0);
  // For each face.
  int m;
  for (int n = 0; n != _iss.getSimplicesSize(); ++n) {
    m = _iss.getIndexedSimplices()[n][0];
    _vertexNormals[m] += _faceNormals[n];
    m = _iss.getIndexedSimplices()[n][1];
    _vertexNormals[m] += _faceNormals[n];
  }

  // Normalize the directions.
  for (int n = 0; n != _vertexNormals.size(); ++n) {
    normalize(&_vertexNormals[n]);
  }
}

END_NAMESPACE_GEOM

// End of file.
