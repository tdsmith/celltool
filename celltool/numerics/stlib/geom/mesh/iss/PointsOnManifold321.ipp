// -*- C++ -*-

#if !defined(__geom_PointsOnManifold321_ipp__)
#error This file is an implementation detail of the class PointsOnManifold.
#endif

BEGIN_NAMESPACE_GEOM

//--------------------------------------------------------------------------
// Constructors etc.
//--------------------------------------------------------------------------


// Construct from the mesh and the corners.
template<typename T>
template<bool A, typename V, typename IS, typename EdgeInIter,
	 typename IntInIter>
inline
PointsOnManifold<3,2,1,T>::
PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 EdgeInIter edgesBegin, EdgeInIter edgesEnd,
		 IntInIter cornersBegin, IntInIter cornersEnd) :
  // Build the surface manifold.
  _surfaceManifold(iss),
  // Initially, record each edge as a surface feature.
  _isAnEdge(ads::FixedArray<2,int>(_surfaceManifold.getSimplicesSize(), 
				   M + 1), false),
  // Make a null edge manifold.  We'll build it later.
  _edgeManifold(),
  // Empty array.  We'll build it later.
  _cornerIndices(),
  // Initially, record everything as being a surface feature.
  _vertexFeatures(_surfaceManifold.getVerticesSize(), SurfaceFeature),
  // Initially, there are no registered points on the manifold.
  _points(),
  // Initially, there are no registered edges on the manifold.
  _edges(),
  // Build the simplex query data structure.
  _surfaceQuery(_surfaceManifold),
  // Make a null edge query data structure.  We'll build it later.
  _edgeQuery(_edgeManifold),
  // Give the max corner distance a sensible initial value.
  _maxCornerDistance(0.1 * computeMinimumEdgeLength(_surfaceManifold)),
  // Give the max edge distance a sensible initial value.
  _maxEdgeDistance(_maxCornerDistance),
  // Do not bother to initialize.
  _cachedIdentifier(),
  // We initialize below.
  _cachedFace() {

  // Build the edge and corner features.  This builds the following member
  // variables: _isAnEdge, _edgeManifold, _cornerIndices, _vertexFeatures, 
  // _edges, and _edgeQuery.
  buildEdgesAndCorners(edgesBegin, edgesEnd, cornersBegin, cornersEnd);

  // Invalidate the cached face, just in case...
  _cachedFace.setToNull();
}


// Construct from the mesh and an angle to define corners.
template<typename T>
template<bool A, typename V, typename IS>
inline
PointsOnManifold<3,2,1,T>::
PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 Number maxDihedralAngleDeviation,
		 Number maxSolidAngleDeviation,
		 Number maxBoundaryAngleDeviation) :
  // Build the surface manifold.
  _surfaceManifold(iss),
  // Initially, record each edge as a surface feature.
  _isAnEdge(ads::FixedArray<2,int>(_surfaceManifold.getSimplicesSize(), 
				   M + 1), false),
  // Make a null edge manifold.  We'll build it later.
  _edgeManifold(),
  // Empty array.  We'll build it later.
  _cornerIndices(),
  // Initially, record everything as being a surface feature.
  _vertexFeatures(_surfaceManifold.getVerticesSize(), SurfaceFeature),
  // Initially, there are no registered points on the manifold.
  _points(),
  // Initially, there are no registered edges on the manifold.
  _edges(),
  // Build the simplex query data structure.
  _surfaceQuery(_surfaceManifold),
  // Make a null edge query data structure.  We'll build it later.
  _edgeQuery(_edgeManifold),
  // Give the max corner distance a sensible initial value.
  _maxCornerDistance(0.1 * computeMinimumEdgeLength(_surfaceManifold)),
  // Give the max edge distance a sensible initial value.
  _maxEdgeDistance(_maxCornerDistance),
  // Do not bother to initialize.
  _cachedIdentifier(),
  // We initialize below.
  _cachedFace() {

  typedef typename SurfaceManifold::FaceIterator EdgeIterator;

  // The edge features.
  std::vector<SurfaceEdge> edges;
  // If the maximum dihedral angle deviation has been specified.
  if (maxDihedralAngleDeviation >= 0) {
    // The maximum value of the cosine of the dihedral angle for a surface
    // feature.
    const Number maxCosine = std::cos(numerical::Constants<Number>::Pi() - 
				      maxDihedralAngleDeviation);
    // Determine the edge features.
    // For each edge in the surface manifold.
    for (EdgeIterator i = _surfaceManifold.getFacesBeginning(); 
	 i != _surfaceManifold.getFacesEnd(); ++i) {
      // If this is an internal edge.
      if (! _surfaceManifold.isOnBoundary(*i)) {
	// If the dihedral angle is too sharp.
	if (geom::computeCosineAngle(_surfaceManifold, *i) > maxCosine) {
	  edges.push_back(*i);
	}
      }
    }
  }
  
  //
  // Determine the vertices that are corner features.
  //

  // The maximum value of the cosine of the boundary angle for an edge feature.
  const Number maxCosineBoundaryAngle = 
    std::cos(numerical::Constants<Number>::Pi() - maxBoundaryAngleDeviation);

  std::vector<int> corners;
  // If the maximum solid angle has been specified.
  if (maxSolidAngleDeviation >= 0) {
    // If the maximum boundary angle has been specified.
    if (maxBoundaryAngleDeviation >= 0) {
      // For each vertex in the surface manifold.
      for (int i = 0; i != _surfaceManifold.getVerticesSize(); ++i) {
	// If this is a boundary vertex.
	if (_surfaceManifold.isVertexOnBoundary(i)) {
	  // If the boundary angle is too sharp.
	  if (geom::computeCosineBoundaryAngle(_surfaceManifold, i) > 
	      maxCosineBoundaryAngle) {
	    corners.push_back(i);
	  }
	}
	// Otherwise, this is an interior vertex.
	else {
	  // If the angle deviates too much from 2 pi.
	  if (std::abs(2 * numerical::Constants<Number>::Pi() - 
		       computeAngle(_surfaceManifold, i)) > 
	      maxSolidAngleDeviation) {
	    // It is a corner feature.
	    corners.push_back(i);
	  }
	}
      }
    }
    // Otherwise, the maximum boundary angle has not been specified.
    // Only check the interior vertices.
    else {
      // For each vertex in the surface manifold.
      for (int i = 0; i != _surfaceManifold.getVerticesSize(); ++i) {
	// If this is an interior vertex.
	if (! _surfaceManifold.isVertexOnBoundary(i)) {
	  // If the angle deviates too much from 2 pi.
	  if (std::abs(2 * numerical::Constants<Number>::Pi() - 
		       computeAngle(_surfaceManifold, i)) > 
	      maxSolidAngleDeviation) {
	    // It is a corner feature.
	    corners.push_back(i);
	  }
	}
      }
    }
  }
  // Otherwise, the maximum solid angle has not been specified.
  // Only check the boundary vertices.
  else {
    // If the maximum boundary angle has been specified.
    if (maxBoundaryAngleDeviation >= 0) {
      // For each vertex in the surface manifold.
      for (int i = 0; i != _surfaceManifold.getVerticesSize(); ++i) {
	// If this is a boundary vertex.
	if (_surfaceManifold.isVertexOnBoundary(i)) {
	  // If the boundary angle is too sharp.
	  if (geom::computeCosineBoundaryAngle(_surfaceManifold, i) > 
	      maxCosineBoundaryAngle) {
	    corners.push_back(i);
	  }
	}
      }
    }
    // If neither angle was specified, do nothing.
  }
  
  // Build the edge and corner features.  This builds the following member
  // variables: _isAnEdge, _edgeManifold, _vertexFeatures.
  buildEdgesAndCorners(edges.begin(), edges.end(), corners.begin(), 
		       corners.end());

  // Invalidate the cached face, just in case...
  _cachedFace.setToNull();
}


//--------------------------------------------------------------------------
// Accessors.
//--------------------------------------------------------------------------



// Count the number of registered points on each feature.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
countNumberOfPointsOnFeatures(SizeType* surfaceCount, SizeType* edgeCount, 
			      SizeType* cornerCount) const {
  *surfaceCount = 0;
  *edgeCount = 0;
  *cornerCount = 0;
  for (typename PointMap::const_iterator i = _points.begin(); 
       i != _points.end(); ++i) {
    if (i->second.getFeature() == SurfaceFeature) {
      ++*surfaceCount;
    }
    else if (i->second.getFeature() == EdgeFeature) {
      ++*edgeCount;
    }
    else if (i->second.getFeature() == CornerFeature) {
      ++*cornerCount;
    }
    else {
      // This should not happen.
      assert(false);
    }
  }
}


// Count the number of points on surface features.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::SizeType
PointsOnManifold<3,2,1,T>::
countNumberOfSurfacePoints() const {
  SizeType surfaceCount, edgeCount, cornerCount;
  countNumberOfPointsOnFeatures(&surfaceCount, &edgeCount, &cornerCount);
  return surfaceCount;
}


// Count the number of points on edge features.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::SizeType
PointsOnManifold<3,2,1,T>::
countNumberOfEdgePoints() const {
  SizeType surfaceCount, edgeCount, cornerCount;
  countNumberOfPointsOnFeatures(&surfaceCount, &edgeCount, &cornerCount);
  return edgeCount;
}


// Count the number of points on corner features.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::SizeType
PointsOnManifold<3,2,1,T>::
countNumberOfCornerPoints() const {
  SizeType surfaceCount, edgeCount, cornerCount;
  countNumberOfPointsOnFeatures(&surfaceCount, &edgeCount, &cornerCount);
  return cornerCount;
}




// Return true if the edge between the two registered points has been 
// registered.
template<typename T>
inline
bool
PointsOnManifold<3,2,1,T>::
hasEdge(const int pointId1, const int pointId2) const {
#ifdef DEBUG_geom
  assert(hasPoint(pointId1) && hasPoint(pointId2));
#endif
  // Return true if the edge has been registered.
  return _edges.count(RegisteredEdge(pointId1, pointId2)) == 1;
}


// Get the simplex index associated with the point.
// The point must be on a surface feature.
template<typename T>
inline
int
PointsOnManifold<3,2,1,T>::
getSurfaceSimplexIndex(const int identifier) const {
  // Find the point.
  typename PointMap::const_iterator i = _points.find(identifier);
  // Make sure it has been registered.
  assert(i != _points.end());
  // It must be a surface feature.
  assert(i->second.getFeature() == SurfaceFeature);
  // Return the simplex index.
  return i->second.getIndex();
}



// Get the simplex index associated with the point.
// The point must be on an edge feature.
template<typename T>
inline
int
PointsOnManifold<3,2,1,T>::
getEdgeSimplexIndex(const int identifier) const {
  // Find the point.
  typename PointMap::const_iterator i = _points.find(identifier);
  // Make sure it has been registered.
  assert(i != _points.end());
  // It must be an edge feature.
  assert(i->second.getFeature() == EdgeFeature);
  // Return the simplex index.
  return i->second.getIndex();
}



// Get the vertex index associated with the point.
// The point must be on a corner feature.
template<typename T>
inline
int
PointsOnManifold<3,2,1,T>::
getVertexIndex(const int identifier) const {
  // Find the point.
  typename PointMap::const_iterator i = _points.find(identifier);
  // Make sure it has been registered.
  assert(i != _points.end());
  // It must be a corner feature.
  assert(i->second.getFeature() == CornerFeature);
  // Return the vertex index.
  return i->second.getIndex();
}


// Build a mesh of the registered edges.
template<typename T>
template<bool A33, typename V33, typename IS33, typename V31, typename IS31>
inline
void
PointsOnManifold<3,2,1,T>::
getEdgesOnEdgeFeatures(const IndSimpSet<3,3,A33,Number,V33,IS33>& solidMesh,
		       IndSimpSet<3,1,true,Number,V31,IS31>* edgeMesh) const {
  // Make an array of the indexed edges.
  ads::Array<1, IS31> indexedEdges(countNumberOfEdges());
  RegisteredEdge edge;
  int n = 0;
  for (typename EdgeSet::const_iterator i = _edges.begin(); i != _edges.end();
       ++i) {
    indexedEdges[n][0] = i->getFirst();
    indexedEdges[n][1] = i->getSecond();
    ++n;
  }
  assert(n == indexedEdges.size());

  // Build the edge mesh.
  edgeMesh->build(solidMesh.getVertices(), indexedEdges);
}

//--------------------------------------------------------------------------
// Manipulators.
//--------------------------------------------------------------------------



// Change the identifier of a registered point.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
changeIdentifier(const int existingIdentifier, const int newIdentifier) {
  // First check the trivial case.
  if (existingIdentifier == newIdentifier) {
    return;
  }

  //
  // Change the identifier for the registered point.
  //

  // Find the point.
  typename PointMap::iterator i = _points.find(existingIdentifier);
  // Make sure it has been registered.
  assert(i != _points.end());
  // Make a point with the new identifier.
  typename PointMap::value_type newPoint(newIdentifier, i->second);
  // Erase the old point.
  _points.erase(i);
  // Insert the new point.
  const bool result = _points.insert(newPoint).second;
  // Make sure the insertion was successful.
  assert(result);

  //
  // Change the identifier for registered edges.
  // CONTINUE:  This is slow.  I have to examine each edge.  I need a fancier
  // data edge data structure to do this efficiently.
  //
  
  // For each registered edge.
  for (EdgeSet::iterator edge = _edges.begin(); edge != _edges.end(); ) {
    // If the source needs to be changed
    if (edge->getFirst() == existingIdentifier) {
      // Insert the replacement.
      _edges.insert(RegisteredEdge(newIdentifier, edge->getSecond()));
      // Erase the old one and move to the next edge.
      _edges.erase(edge++);
    }
    // If the target needs to be changed
    else if (edge->getSecond() == existingIdentifier) {
      // Insert the replacement.
      _edges.insert(RegisteredEdge(edge->getFirst(), newIdentifier));
      // Erase the old one and move to the next edge.
      _edges.erase(edge++);
    }
    else {
      // Move to the next edge.
      ++edge;
    }
  }
}



//! Change the location of a registered point.
/*!
  Return the new point on the manifold.
*/
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
changeLocation(int pointIdentifier, const Vertex& newLocation) {
  // Find the point and get its face.
  const typename PointMap::iterator pointIterator = 
    _points.find(pointIdentifier);
  // The point must be registered.
  assert(pointIterator != _points.end());
  const FaceHandle face = pointIterator->second;

  // If this is a corner feature, we cannot move it.
  if (face.getFeature() == CornerFeature) {
    // Return the location of the corner.
    return _surfaceManifold.getVertex(face.getIndex());
  }

  Vertex closestPoint;
  std::set<int> indices;

  // If this is an edge feature, we can move it along the edge.
  if (face.getFeature() == EdgeFeature) {
    // Get the neighborhood of edge simplices.
    getEdgeNeighborhood(face.getIndex(), 
			std::inserter(indices, indices.end()));

    // Compute the closest point on the edge manifold.
    closestPoint = 
      computeClosestPointInEdgeSimplices(indices.begin(), indices.end(), 
					 newLocation);
  
  }
  else {
    // Otherwise, it is a surface feature.
    assert(face.getFeature() == SurfaceFeature);

    // Get the neighborhood of surface simplices.
    getSurfaceNeighborhood(face.getIndex(), 
			   std::inserter(indices, indices.end()));

    // Compute the closest point on the surface manifold.
    closestPoint = 
      computeClosestPointInSurfaceSimplices(indices.begin(), indices.end(), 
					    newLocation);
  }
  
  // Update the face of the point.  Use the cached face determined in one of
  // the above closest point function calls.
  pointIterator->second = _cachedFace;

  // Return the closest point on the edge or surface manifold.
  return closestPoint;
}


// Change the surface simplex for a registered surface feature.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
changeSurfaceSimplex(int pointIdentifier, int simplexIndex) {
  // The simplex index must be valid.
  assert(0 <= simplexIndex && 
	 simplexIndex < _surfaceManifold.getSimplicesSize());

  // Find the point and get its face.
  const typename PointMap::iterator pointIterator = 
    _points.find(pointIdentifier);
  // The point must be registered.
  assert(pointIterator != _points.end());
  const FaceHandle face = pointIterator->second;

  // This must be a surface feature.
  assert(face.getFeature() == SurfaceFeature);

  // Set the new surface simplex.
  pointIterator->second.setIndex(simplexIndex);
}


// Change the edge simplex for a registered edge feature.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
changeEdgeSimplex(int pointIdentifier, int simplexIndex) {
  // The simplex index must be valid.
  assert(0 <= simplexIndex && 
	 simplexIndex < _edgeManifold.getSimplicesSize());

  // Find the point and get its face.
  const typename PointMap::iterator pointIterator = 
    _points.find(pointIdentifier);
  // The point must be registered.
  assert(pointIterator != _points.end());
  const FaceHandle face = pointIterator->second;

  // This must be an edge feature.
  assert(face.getFeature() == EdgeFeature);

  // Set the new edge simplex.
  pointIterator->second.setIndex(simplexIndex);
}


//--------------------------------------------------------------------------
// Insert/Erase Points.
//--------------------------------------------------------------------------


// Insert a point at the specified vertex.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
insertAtVertex(const int pointIdentifier, const int vertexIndex) {
  // Make sure the point has not already been registered.
  assert(_points.count(pointIdentifier) == 0);
  // The vertex index must be valid.
  assert(0 <= vertexIndex && vertexIndex < _surfaceManifold.getVerticesSize());

  // If the vertex is a corner feature.
  if (_vertexFeatures[vertexIndex] == CornerFeature) {
    // Make the point.
    const typename PointMap::value_type 
      x(pointIdentifier, FaceHandle(CornerFeature, vertexIndex));
    // Insert the point.
    _points.insert(x);
  }
  // If the vertex is an edge feature.
  else if (_vertexFeatures[vertexIndex] == EdgeFeature) {
    // An incident simplex.
    const int simplexIndex = _edgeManifold.getIncident(vertexIndex, 0);
    // Make the point.
    const typename PointMap::value_type 
      x(pointIdentifier, FaceHandle(EdgeFeature, simplexIndex));
    // Insert the point.
    _points.insert(x);
  }
  // If the vertex is a surface feature.
  else if (_vertexFeatures[vertexIndex] == SurfaceFeature) {
    // An incident simplex.
    const int simplexIndex = _surfaceManifold.getIncident(vertexIndex, 0);
    // Make the point.
    const typename PointMap::value_type 
      x(pointIdentifier, FaceHandle(SurfaceFeature, simplexIndex));
    // Insert the point.
    _points.insert(x);
  }
  // Otherwise the vertex is a null feature.
  else {
    assert(false);
  }
}



// Insert a point at each vertex.
// The point identifiers will be the vertex indices.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
insertAtVertices() {
  // For each vertex.
  for (int n = 0; n != _surfaceManifold.getVerticesSize(); ++n) {
    // Insert a point at the vertex.
    insertAtVertex(n, n);
  }
}


// Insert a point at each vertex and an edge at each edge feature.
// The point identifiers will be the vertex indices.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
insertAtVerticesAndEdges() {
  // Insert points at the vertices.
  insertAtVertices();
  // Insert edges along each ege feature.
  insertAtEdges();
}


// CONTINUE: Implement this or something like it.
#if 0
// Insert a point at the closest point to the specified position that is 
// near the existing point.  Return the point's position on the manifold.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insertNearPoint(const int newPointID, const Vertex& position, 
		const int existingPointID) {
  // Make sure the new point has not already been registered.
  assert(_points.count(newPointID) == 0);

  // Get the face for the existing point.
  FaceHandle existingFace;
  {
    typename PointMap::iterator i = _points.find(existingPointID);
    // The existing point must be registered.
    assert(i != _points.end());
    existingFace = i->second;
  }

  // Get the neighborhood of simplices.
  std::set<int> indices;
  getNeighborhood(existingFace, std::inserter(indices, indices.end()));

  // Compute the closest point on the manifold.
  const Vertex closestPoint = 
    computeClosestPointInSimplices(indices.begin(), indices.end(), position);
  
  // Insert the new point.
  _points.insert(Point(newPointID, _cachedFace));
  
  return closestPoint;
}
#endif



// Insert a point at the closest point on an edge feature.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insertOnAnEdge(const int pointIdentifier, const Vertex& position) {
  // There must be at least one edge feature.
  assert(_edgeManifold.getSimplicesSize() > 0);
  // Find the closest edge simplex.
  const int simplexIndex = _edgeQuery.computeMinimumDistanceIndex(position);
  // Check that the simplex index is in the right range.
  assert(0 <= simplexIndex && simplexIndex < _edgeManifold.getSimplicesSize());
  // Insert the point in the edge simplex and return the closest point 
  // in the simplex.
  return insertInEdgeSimplex(pointIdentifier, position, simplexIndex);
}



// Insert a point at the closest point on an edge or corner feature.
// First check if the point is close to a corner.  If not, insert it
// on an edge feature.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insertOnAnEdgeOrCorner(int pointIdentifier, const Vertex& position) {
  // Check if there is a close corner.
  const int vertexIndex = findCornerVertex(position);
  if (vertexIndex != -1) {
    // Insert the point at the corner vertex.
    insertAtVertex(pointIdentifier, vertexIndex);
    // Return the position of the corner vertex.
    return _surfaceManifold.getVertex(vertexIndex);
  }
  // Otherwise, insert on an edge.
  return insertOnAnEdge(pointIdentifier, position);
}



// Insert a point at the closest point on a surface feature.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insertOnASurface(const int pointIdentifier, const Vertex& position) {
  // There must be at least one surface simplex.
  assert(_surfaceManifold.getSimplicesSize() > 0);
  // Find the closest surface simplex.
  const int simplexIndex = _surfaceQuery.computeMinimumDistanceIndex(position);
  // Check that the simplex index is in the right range.
  assert(0 <= simplexIndex && 
	 simplexIndex < _surfaceManifold.getSimplicesSize());
  // Insert the point in the surface simplex and return the closest point 
  // in the simplex.
  return insertInSurfaceSimplex(pointIdentifier, position, simplexIndex);
}



// Insert a point at the closest point to the specified position.
// Return the point's position on the manifold.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insert(const int pointIdentifier, const Vertex& position) {
  //
  // First see if we should insert at a corner.
  //
  const int vertexIndex = findCornerVertex(position);
  // If there is a corner that is very close to the position.
  if (vertexIndex != -1) {
#ifdef DEBUG_PointsOnManifold
    // Make sure that there is not already a point at the corner vertex.
    // Note: This is an expensive test.
    // For each point.
    for (typename PointMap::const_iterator i = _points.begin();
	 i != _points.end(); ++i) {
      // If the point is at a corner feature.
      if (i->second.getFeature() == CornerFeature) {
	// It should not be at the corner vertex that we found above.
	if (i->second.getIndex() == vertexIndex) {
	  std::cerr << "Warning: Two points are close to corner vertex "
		    << vertexIndex << "\n"
		    << "Only one point will be inserted there.\n";
	}
      }
    }
#endif
    // Insert the point at the corner vertex.
    insertAtVertex(pointIdentifier, vertexIndex);
    // Return the position of the corner vertex.
    return _surfaceManifold.getVertex(vertexIndex);
  }

  //
  // Next see if we should insert it in an edge.
  //
  // CONTINUE REMOVE
  //std::cerr << "ID = " << pointIdentifier << "\n";
  int simplexIndex = findEdgeSimplex(position);
  // If there is an edge that is very close to the position.
  if (simplexIndex != -1) {
    // Insert the point in the edge simplex and return the closest point 
    // in the simplex.
    return insertInEdgeSimplex(pointIdentifier, position, simplexIndex);
  }

  // Otherwise, we will insert the point at a surface feature.
  // Find the closest simplex.
  simplexIndex = _surfaceQuery.computeMinimumDistanceIndex(position);
  // Insert the point in the simplex and return the closest point in the 
  // simplex.
  return insertInSurfaceSimplex(pointIdentifier, position, simplexIndex);
}



// Insert a range of points at their closest points.
// The point identifiers will be in the range [0...numPoints).
// Put the positions at the closest points on the manifold to the output 
// iterator.
template<typename T>
template<typename PointInputIterator, typename PointOutputIterator>
inline
void
PointsOnManifold<3,2,1,T>::
insert(PointInputIterator locationsBegin, PointInputIterator locationsEnd,
       PointOutputIterator closestPoints) {
  int pointIdentifier = 0;
  while (locationsBegin != locationsEnd) {
    *closestPoints++ = insert(pointIdentifier++, *locationsBegin++);
  }
}



// Insert the boundary vertices of the mesh.  Register the edge features.
template<typename T>
template<bool A, typename V, typename IS>
inline
void
PointsOnManifold<3,2,1,T>::
insertBoundaryVerticesAndEdges(IndSimpSetIncAdj<N,M+1,A,T,V,IS>* mesh,
			       const Number maxDihedralAngleDeviation) {
  // For each vertex on the boundary.
  for (int i = 0; i != mesh->getVerticesSize(); ++i) {
    // If the vertex is on the boundary.
    if (mesh->isVertexOnBoundary(i)) {
      // Move the vertex to lay on the boundary manifold.
      mesh->setVertex(i, insert(i, mesh->getVertex(i)));
    }
  }

  // If we are checking for edge features.
  if (maxDihedralAngleDeviation >= 0) {
    // The maximum value of the cosine of the dihedral angle for a surface
    // feature.
    const Number maxCosine = std::cos(numerical::Constants<Number>::Pi() - 
				      maxDihedralAngleDeviation);

    // Extract the boundary of the mesh.
    typedef typename IndSimpSetIncAdj<N,M,A,T,V>::FaceIterator FaceIterator;
    IndSimpSetIncAdj<N,M,A,T,V> boundary;
    std::vector<int> indices;
    buildBoundary(*mesh, &boundary, std::back_inserter(indices));

    int sourceId, targetId, simplexIndex, localIndex, localSource, localTarget;
    // For each edge on the boundary of the mesh.
    for (FaceIterator face = boundary.getFacesBeginning(); 
	 face != boundary.getFacesEnd(); ++face) {
      simplexIndex = face->first;
      localIndex = face->second;
      localSource = (localIndex + 1) % (M + 1);
      localTarget = (localIndex + 2) % (M + 1);
      // The point identifiers.
      sourceId = 
	indices[boundary.getIndexedSimplex(simplexIndex)[localSource]];
      targetId = 
	indices[boundary.getIndexedSimplex(simplexIndex)[localTarget]];
      // If the points have been registered as edge or corner features.
      if (! isOnSurface(sourceId) && ! isOnSurface(targetId)) {
	// If the angle is sharp enough to be an edge feature.
	if (geom::computeCosineAngle(boundary, *face) > maxCosine) {
	  // Insert the edge.
	  insert(sourceId, targetId);
	}
      }
    }
  }
}




// Insert the boundary vertices of the mesh.  Register the edge features.
template<typename T>
template<template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Container>
inline
void
PointsOnManifold<3,2,1,T>::
insertBoundaryVerticesAndEdges(SimpMeshRed<N,M+1,T,Node,Cell,Container>* mesh,
			       const Number maxDihedralAngleDeviation) {
  typedef SimpMeshRed<N,M+1,T,Node,Cell> Mesh;
  typedef typename Mesh::NodeIterator NodeIterator;
  typedef typename Mesh::EdgeConstIterator EdgeIterator;

  // For each node in the mesh.
  for (NodeIterator i = mesh->getNodesBeginning(); i != mesh->getNodesEnd(); 
       ++i) {
    // If the node is on the boundary.
    if (i->isOnBoundary()) {
      // Move the node to lay on the boundary manifold.
      i->setVertex(insert(i->getIdentifier(), i->getVertex()));
    }
  }

  // If we are checking for edge features.
  if (maxDihedralAngleDeviation >= 0) {
    int sourceId, targetId;
    // For each edge in the mesh.
    for (EdgeIterator i = mesh->getEdgesBeginning(); i != mesh->getEdgesEnd();
	 ++i) {
      // If the edge is on the boundary.
      if (isOnBoundary<Mesh>(i->first, i->second, i->third)) {
	// The point identifiers.
	sourceId = i->first->getNode(i->second)->getIdentifier();
	targetId = i->first->getNode(i->third)->getIdentifier();
	// If the points have been registered as edge or corner features.
	if (! isOnSurface(sourceId) && ! isOnSurface(targetId)) {
	  // If the angle is sharp enough to be an edge feature.
	  if (std::abs(geom::computeDihedralAngle<Mesh>(*i) - 
		       numerical::Constants<Number>::Pi()) > 
	      maxDihedralAngleDeviation) {
	    // Insert the edge.
	    insert(sourceId, targetId);
	  }
	}
      }
    }
  }
}


// Insert the vertices of the mesh.  Register the edge features.
template<typename T>
template<template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Container>
inline
void
PointsOnManifold<3,2,1,T>::
insertVerticesAndEdges(SimpMeshRed<N,M,T,Node,Cell,Container>* mesh,
		       const Number maxDihedralAngleDeviation) {
  typedef SimpMeshRed<N,M,T,Node,Cell> Mesh;
  typedef typename Mesh::NodeIterator NodeIterator;
  typedef typename Mesh::FaceConstIterator FaceIterator;

  // For each node in the mesh.
  for (NodeIterator i = mesh->getNodesBeginning(); i != mesh->getNodesEnd();
       ++i) {
    // If the node is on the boundary.
    if (i->isOnBoundary()) {
      // Insert the node on an edge or corner feature.
      // Move the node to lay on the manifold.
      i->setVertex(insertOnAnEdgeOrCorner(i->getIdentifier(), i->getVertex()));
    }
    else {
      // Insert the node.  Move the node to lay on the manifold.
      i->setVertex(insert(i->getIdentifier(), i->getVertex()));
    }
  }

  // The maximum value of the cosine of the dihedral angle for a surface
  // feature.
  const Number maxCosine = std::cos(numerical::Constants<Number>::Pi() - 
				    maxDihedralAngleDeviation);
  int sourceId, targetId;
  // For each 1-face (edge) in the mesh.
  for (FaceIterator i = mesh->getFacesBeginning(); i != mesh->getFacesEnd();
       ++i) {
    // If the face is on the boundary.
    if (isOnBoundary<Mesh>(i)) {
      // Insert the edge.
      sourceId = i->first->getNode((i->second + 1) % (M + 1))->getIdentifier();
      targetId = i->first->getNode((i->second + 2) % (M + 1))->getIdentifier();
      insert(sourceId, targetId);
    }
    // If it is an interior edge and we are checking for interior 
    // edge features.
    else if (maxDihedralAngleDeviation >= 0) {
      sourceId = i->first->getNode((i->second + 1) % (M + 1))->getIdentifier();
      targetId = i->first->getNode((i->second + 2) % (M + 1))->getIdentifier();
      // If the points have been registered as edge or corner features.
      if (! isOnSurface(sourceId) && ! isOnSurface(targetId)) {
	// If the angle is sharp enough to be an edge feature.
	if (computeCosineAngle<Mesh>(i) > maxCosine) {
	  // Insert the edge.
	  insert(sourceId, targetId);
	}
      }
    }
    // Otherwise, do nothing.
  }
}


// Erase a point.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
erase(const int pointIdentifier) {
  // Find the point.
  typename PointMap::iterator i = _points.find(pointIdentifier);
  // The point must be registered.
  assert(i != _points.end());
  // Erase the point.
  _points.erase(i);
}


//--------------------------------------------------------------------------
// Insert/Erase Edges.
//--------------------------------------------------------------------------


// Insert an edge.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
insert(const int pointId1, const int pointId2) {
  // CONTINUE REMOVE
  //std::cerr << "insert " << pointId1 << " " << pointId2 << "\n";
#ifdef DEBUG_geom
  // The points must be registered.
  assert(hasPoint(pointId1) && hasPoint(pointId2));
#endif
  // Insert the edge.
  std::pair<typename EdgeSet::iterator,bool> result =
    _edges.insert(RegisteredEdge(pointId1, pointId2));
  // Make sure that the edge was inserted.  (It would not be inserted if it
  // were already there.)
  assert(result.second);
}


// Insert an edge at each edge feature.
// There must already be a point at each vertex of an edge feature.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
insertAtEdges() {
  typedef typename EdgeManifold::IndexedSimplexConstIterator 
    IndexedSimplexIterator;

  // For each simplex in the edge manifold.
  for (IndexedSimplexIterator i = _edgeManifold.getIndexedSimplicesBeginning();
       i != _edgeManifold.getIndexedSimplicesEnd(); ++i) {
    // Insert the edge.
    insert((*i)[0], (*i)[1]);
  }
}


// Erase an edge.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
erase(const int pointId1, const int pointId2) {
  // Try to erase the edge.
  typename EdgeSet::size_type num = 
    _edges.erase(RegisteredEdge(pointId1, pointId2));
  // Make sure that it was erased.
  assert(num == 1);
}


// Split an edge.
// Erase the edge between the source and the target.  Insert two edges 
// using the mid-point.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
splitEdge(const int source, const int target, const int middle) {
  erase(source, target);
  insert(source, middle);
  insert(middle, target);
}


//--------------------------------------------------------------------------
// Closest Point.
//--------------------------------------------------------------------------



template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
computeClosestPoint(const int pointIdentifier, const Vertex& position) const {
  // Update the point identifier in case we decide to update the point.
  _cachedIdentifier = pointIdentifier;

  // Get the face for the point.
  FaceHandle face;
  {
    typename PointMap::const_iterator i = _points.find(pointIdentifier);
    // Make sure the point was registered.
    assert(i != _points.end());
    face = i->second;
  }

  // If the face is a corner.
  if (face.getFeature() == CornerFeature) {
    // The new face is the same as the old face.
    _cachedFace = face;
    // Return the location of the corner.
    return _surfaceManifold.getVertex(face.getIndex());
  }

  // If the face is an edge.
  if (face.getFeature() == EdgeFeature) {
    // Get the neighborhood of edge simplices.
    std::set<int> indices;
    getEdgeNeighborhood(face.getIndex(), 
			std::inserter(indices, indices.end()));
    // Return the closest point to the neighborhood of edge simplices.
    return computeClosestPointInEdgeSimplices(indices.begin(), indices.end(), 
					      position);
  }

  // Otherwise, the face is a surface feature.
  assert(face.getFeature() == SurfaceFeature);
  // Get the neighborhood of surface simplices.
  std::set<int> indices;
  getSurfaceNeighborhood(face.getIndex(), 
			 std::inserter(indices, indices.end()));
  // Return the closest point to the neighborhood of surface simplices.
  return computeClosestPointInSurfaceSimplices(indices.begin(), indices.end(), 
					       position);
}



// Update the face of a registered point.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
updatePoint() {
  // Check that the cached face is valid.
  assert(isValid(_cachedFace));

  // Find the point.
  typename PointMap::iterator i = _points.find(_cachedIdentifier);
  // Make sure we found it.
  assert(i != _points.end());

  // Update the face.
  i->second = _cachedFace;

  // Invalidate the cached face.
  _cachedFace.setToNull();
}



//--------------------------------------------------------------------------
// File I/O.
//--------------------------------------------------------------------------


// Print information about the data structure.
template<typename T>
inline
void
PointsOnManifold<3,2,1,T>::
printInformation(std::ostream& out) const {
  out 
    << "The surface manifold has " << _surfaceManifold.getSimplicesSize()
    << " simplices and " << _surfaceManifold.getVerticesSize()
    << " vertices.\n"
    << "The edge manifold has " << _edgeManifold.getSimplicesSize()
    << " simplices and " << _edgeManifold.getVerticesSize()
    << " vertices.\n"
    << int(std::count(_vertexFeatures.begin(), _vertexFeatures.end(),
		      SurfaceFeature))
    << " vertices are surface features.\n"
    << int(std::count(_vertexFeatures.begin(), _vertexFeatures.end(),
		      EdgeFeature))
    << " vertices are edge features.\n"
    << int(std::count(_vertexFeatures.begin(), _vertexFeatures.end(),
		      CornerFeature))
    << " vertices are corner features.\n"
    << "There are " << countNumberOfPoints() << " registered points.\n"
    << countNumberOfSurfacePoints() << " are on surface features.\n"
    << countNumberOfEdgePoints() << " are on edge features.\n"
    << countNumberOfCornerPoints() << " are on corner features.\n"
    << "There are " << countNumberOfEdges() << " registered edges.\n"
    << "The maximum corner distance is " << getMaxCornerDistance() << "\n"
    << "The maximum edge distance is " << getMaxEdgeDistance() << "\n";
}


//--------------------------------------------------------------------------
// Private member functions.
//--------------------------------------------------------------------------



// Build the edge and corner data structures.
template<typename T>
template<typename EdgeInIter, typename IntInIter>
inline
void
PointsOnManifold<3,2,1,T>::
buildEdgesAndCorners(EdgeInIter edgesBegin, EdgeInIter edgesEnd,
		     IntInIter cornersBegin, IntInIter cornersEnd) {
  typedef typename SurfaceManifold::Face Edge;
  typedef typename SurfaceManifold::FaceIterator EdgeIterator;
  typedef typename EdgeManifold::IndexedSimplex IndexedEdge;

  // The set of edges, represented as simplex faces.
  std::set<Edge> edges;

  //
  // Add the input edges.
  //
  Edge edge;
  // Simplex index.
  int n;
  // Simplex vertex index.
  int m;
  // For each input edge.
  for (; edgesBegin != edgesEnd; ++edgesBegin) {
    edge = *edgesBegin;
    n = edge.first;
    m = edge.second;
    // If it is not a boundary edge.
    if (! _surfaceManifold.isOnBoundary(edge)) {
      // If the adjacent simplex index is less than this simplex index.
      if (_surfaceManifold.getAdjacent(n, m) < n) {
	// Switch to the canonical representation of the edge.
	edge.first = _surfaceManifold.getAdjacent(n, m);
	edge.second = _surfaceManifold.getMirrorIndex(n, m);
      }
    }
    // Add the edge to the set.
    edges.insert(edge);
  }  
 
  //
  // Add the boundary edges.
  //
  // For each edge in the surface manifold.
  for (EdgeIterator i = _surfaceManifold.getFacesBeginning(); 
       i != _surfaceManifold.getFacesEnd(); ++i) {
    // If this is a boundary edge.
    if (_surfaceManifold.isOnBoundary(*i)) {
      // Add it to the set.
      edges.insert(*i);
    }
  }

  //
  // Now we have all of the edge features.  Build the edge data structures.
  //
  {
    IndexedEdge indexedEdge;
    // The indexed simplices in the edge manifold.
    ads::Array<1,IndexedEdge> indexedEdges(SizeType(edges.size()));
    // For each edge, represented as a simplex face.
    typename std::set<Edge>::const_iterator i = edges.begin();
    typename ads::Array<1,IndexedEdge>::iterator j = indexedEdges.begin();
    for (; i != edges.end(); ++i, ++j) {
      edge = *i;
      n = edge.first;
      m = edge.second;
      // Record the edge of the simplex as being an edge feature.
      _isAnEdge(n, m) = true;
      // If it is not a boundary edge.
      if (! _surfaceManifold.isOnBoundary(edge)) {
	// Do the same for the adjacent simplex that shares the edge.
	_isAnEdge(_surfaceManifold.getAdjacent(n, m),
		  _surfaceManifold.getMirrorIndex(n, m)) = true;
      }
      // Record the indexed edge.
      indexedEdge[0] = 
	_surfaceManifold.getIndexedSimplex(n)[(m + 1) % (M + 1)];
      indexedEdge[1] = 
	_surfaceManifold.getIndexedSimplex(n)[(m + 2) % (M + 1)];
      *j = indexedEdge;
    }
    // Build the edge manifold.
    _edgeManifold.build(_surfaceManifold.getVertices(), indexedEdges);
    // Build the edge query data structure.
    _edgeQuery.build();
  }

  //
  // Initially, all of the vertex features are set as surface features.
  // Now set the edge features.  To do this, set each vertex with an 
  // incident edge feature as an edge feature.  Some of these will later 
  // be set to corner features.
  //

  // For each edge feature.
  for (int i = 0; i != _edgeManifold.getSimplicesSize(); ++i) {
    _vertexFeatures[_edgeManifold.getIndexedSimplex(i)[0]] = EdgeFeature;
    _vertexFeatures[_edgeManifold.getIndexedSimplex(i)[1]] = EdgeFeature;
  }

  //
  // Set the corner features that were specified as input.
  //
  for (; cornersBegin != cornersEnd; ++cornersBegin) {
    _vertexFeatures[*cornersBegin] = CornerFeature;
  }

  //
  // Set the corner features that have other than 0 or 2 incident edge 
  // features.
  //
  int numIncidentEdges;
  // For each vertex.
  for (int i = 0; i != _edgeManifold.getVerticesSize(); ++i) {
    numIncidentEdges = _edgeManifold.getIncidentSize(i);
    // If the vertex has other than 0 or 2 incident edge features.
    if (! (numIncidentEdges == 0 || numIncidentEdges == 2)) {
      _vertexFeatures[i] = CornerFeature;
    }
  }

  //
  // Record the corner indices.
  //
  std::vector<int> indices;
  for (int i = 0; i != _vertexFeatures.size(); ++i) {
    if (_vertexFeatures[i] == CornerFeature) {
      indices.push_back(i);
    }
  }
  _cornerIndices = ads::Array<1,int>(indices.begin(), indices.end());
}



// Insert the point in the specified surface simplex.
// Return the closest point in the simplex.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insertInSurfaceSimplex(const int pointIdentifier, const Vertex& position, 
		       const int simplexIndex) {
  // Make sure the point has not already been registered.
  assert(_points.count(pointIdentifier) == 0);
  // The simplex index must be valid.
  assert(0 <= simplexIndex && 
	 simplexIndex < _surfaceManifold.getSimplicesSize());

  // Make the point.
  const typename PointMap::value_type 
    x(pointIdentifier, FaceHandle(SurfaceFeature, simplexIndex));
  // Insert the point.
  _points.insert(x);

  // Return the closest point.
  return computeClosestPointInSurfaceSimplex(simplexIndex, position);
}



// Insert the point in the specified edge simplex.
// Return the closest point in the simplex.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
insertInEdgeSimplex(const int pointIdentifier, const Vertex& position, 
		    const int simplexIndex) {
  // Make sure the point has not already been registered.
  assert(_points.count(pointIdentifier) == 0);
  // The simplex index must be valid.
  assert(0 <= simplexIndex && 
	 simplexIndex < _edgeManifold.getSimplicesSize());

  // Make the point.
  const typename PointMap::value_type 
    x(pointIdentifier, FaceHandle(EdgeFeature, simplexIndex));
  // Insert the point.
  _points.insert(x);

  // Return the closest point.
  return computeClosestPointInEdgeSimplex(simplexIndex, position);
}



// Try to find a corner vertex that is very close to the position.
// If there is a close corner, return the index of the vertex.  
// Otherwise, return -1.
template<typename T>
inline
int
PointsOnManifold<3,2,1,T>::
findCornerVertex(const Vertex& position) const {
  // CONTINUE: This is a slow algorithm if there are a lot of corner features.
  // Perhaps use a KDTree instead.
  
  // Find the closest corner.
  Number minDistance = std::numeric_limits<Number>::max();
  Number d;
  int vertexIndex;
  int minIndex = -1;
  for (int n = 0; n != _cornerIndices.size(); ++n) {
    vertexIndex = _cornerIndices[n];
    d = geom::computeDistance(position, 
			      _surfaceManifold.getVertex(vertexIndex));
    if (d < minDistance) {
      minDistance = d;
      minIndex = vertexIndex;
    }
  }
  
  // If we found a corner vertex close enough to the position.
  if (minDistance <= _maxCornerDistance) {
    // Return the index of that vertex.
    return minIndex;
  }
  // Otherwise, there is no close corner vertex.
  return -1;
}



// Try to find an edge that is very close to the position.
// If there is a close edge, return the index of the edge simplex.  
// Otherwise, return -1.
template<typename T>
inline
int
PointsOnManifold<3,2,1,T>::
findEdgeSimplex(const Vertex& position) const {
  Number minDistance;
  // Find the closest simplex and compute the distance.
  const int simplexIndex = 
    _edgeQuery.computeMinimumDistanceAndIndex(position, &minDistance);
  // CONTINUE REMOVE
  //std::cerr << " position = " << position << " d = " << minDistance << "\n";
  // if the edge is close enough.
  if (minDistance <= _maxEdgeDistance) {
    // Return the edge simplex index.
    return simplexIndex;
  }
  // Otherwise, there is no close edge.
  return -1;
}





template<typename T>
template<typename IntInputIterator>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
computeClosestPointInSurfaceSimplices(IntInputIterator indicesBegin, 
				      IntInputIterator indicesEnd, 
				      const Vertex& position) const {
  // There must be at least one simplex index.
  assert(indicesBegin != indicesEnd);

  // The feature is a surface.
  _cachedFace.setFeature(SurfaceFeature);

  Vertex closestPoint;
  Number distance;
  // Compute distance and closest point for the first simplex.
  int i;
  {
    i = *indicesBegin++;
    distance = computeClosestPointInSurfaceSimplex(i, position, &closestPoint);
    _cachedFace.setIndex(i);
  }

  // Compute distance and closest point for the rest of the simplices.
  Vertex cp;
  Number d;
  for (; indicesBegin != indicesEnd; ++indicesBegin) {
    i = *indicesBegin;
    d = computeClosestPointInSurfaceSimplex(i, position, &cp);
    if (d < distance) {
      // Update the closest point.
      closestPoint = cp;
      // Update the new face handle.
      _cachedFace.setIndex(i);
    }
  }

  return closestPoint;
}




template<typename T>
template<typename IntInputIterator>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
computeClosestPointInEdgeSimplices(IntInputIterator indicesBegin, 
				      IntInputIterator indicesEnd, 
				      const Vertex& position) const {
  // There must be at least one simplex index.
  assert(indicesBegin != indicesEnd);

  // The feature is a surface.
  _cachedFace.setFeature(EdgeFeature);

  Vertex closestPoint;
  Number distance;
  // Compute distance and closest point for the first simplex.
  int i;
  {
    i = *indicesBegin++;
    distance = computeClosestPointInEdgeSimplex(i, position, &closestPoint);
    _cachedFace.setIndex(i);
  }

  // Compute distance and closest point for the rest of the simplices.
  Vertex cp;
  Number d;
  for (; indicesBegin != indicesEnd; ++indicesBegin) {
    i = *indicesBegin;
    d = computeClosestPointInEdgeSimplex(i, position, &cp);
    if (d < distance) {
      // Update the closest point.
      closestPoint = cp;
      // Update the new face handle.
      _cachedFace.setIndex(i);
    }
  }

  return closestPoint;
}




// Compute the closest point to the surface simplex.  
// Return the unsigned distance.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Number
PointsOnManifold<3,2,1,T>::
computeClosestPointInSurfaceSimplex(const int simplexIndex, const Vertex& x, 
				    Vertex* closestPoint) const {
  typename SurfaceManifold::Simplex simplex;
  _surfaceManifold.getSimplex(simplexIndex, &simplex);
  return geom::computeClosestPoint(simplex, x, closestPoint);
}



// Compute the closest point to the edge simplex.  
// Return the unsigned distance.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Number
PointsOnManifold<3,2,1,T>::
computeClosestPointInEdgeSimplex(const int simplexIndex, const Vertex& x, 
				 Vertex* closestPoint) const {
  typename EdgeManifold::Simplex simplex;
  _edgeManifold.getSimplex(simplexIndex, &simplex);
  return geom::computeClosestPoint(simplex, x, closestPoint);
}



// Return the closest point to the specified surface simplex.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
computeClosestPointInSurfaceSimplex(const int simplexIndex, 
				    const Vertex& x) const {
  Vertex closestPoint;
  computeClosestPointInSurfaceSimplex(simplexIndex, x, &closestPoint);
  return closestPoint;
}



// Return the closest point to the specified edge simplex.
template<typename T>
inline
typename PointsOnManifold<3,2,1,T>::Vertex
PointsOnManifold<3,2,1,T>::
computeClosestPointInEdgeSimplex(const int simplexIndex, 
				 const Vertex& x) const {
  Vertex closestPoint;
  computeClosestPointInEdgeSimplex(simplexIndex, x, &closestPoint);
  return closestPoint;
}



// Return true if the point is on the specified feature.
template<typename T>
inline
bool
PointsOnManifold<3,2,1,T>::
isOnFeature(const int pointIdentifier, const Feature feature) const {
  // Find the point by its identifier.
  typename PointMap::const_iterator i = _points.find(pointIdentifier);
  // Assert that we found it.
  assert(i != _points.end());
  return i->second.getFeature() == feature;
}


// Return true if the face is valid.
template<typename T>
inline
bool
PointsOnManifold<3,2,1,T>::
isValid(const FaceHandle& face) const {
  // If it is the null feature.
  if (face.getFeature() == NullFeature) {
    // The null feature is not valid.
    return false;
  }
  // If it is a corner feature.
  if (face.getFeature() == CornerFeature) {
    return (0 <= face.getIndex() && 
	    face.getIndex() < _surfaceManifold.getVerticesSize());
  }
  // If it is an edge feature.
  if (face.getFeature() == EdgeFeature) {
    return (0 <= face.getIndex() && 
	    face.getIndex() < _edgeManifold.getVerticesSize());
  }
  // Else, it is a surface feature
  assert(face.getFeature() == SurfaceFeature);
  return (0 <= face.getIndex() && 
	  face.getIndex() < _surfaceManifold.getSimplicesSize());
}



// Get the simplices in the neighborhood of the face. (0, 1 or 2-face)
template<typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<3,2,1,T>::
getNeighborhood(const FaceHandle& face, IntInsertIterator iter) const {
  if (face.getFeature() == CornerFeature) {
    getCornerNeighborhood(face.getIndex(), iter);
  }
  else if (face.getFeature() == EdgeFeature) {
    getEdgeNeighborhood(face.getIndex(), iter);
  }
  else if (face.getFeature() == SurfaceFeature) {
    getSurfaceNeighborhood(face.getIndex(), iter);
  }
  else {
    assert(false);
  }
}



// Get the indices of the surface simplices in the neighborhood.
// Do not step across edges.  Only take one step in each direction around 
// corners.
template<typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<3,2,1,T>::
getSurfaceNeighborhood(const int simplexIndex, IntInsertIterator iter) const {
  typedef typename SurfaceManifold::IncidenceConstIterator 
    IncidenceConstIterator;

  // No need to validate the simplex index.  That will be done by the ISS.

  // Add the simplex.
  *iter = simplexIndex;

  // Add the adjacent simplices which are not separated by an edge.
  for (int m = 0; m != M + 1; ++m) {
    // If the adjacent simplex is not separated by an edge.  (This takes care
    // of the boundary edge case as well.)
    if (! _isAnEdge(simplexIndex, m)) {
      // Add the adjacent simplex.
      *iter = _surfaceManifold.getAdjacent(simplexIndex, m);
    }
  }

  int vertexIndex;
  IncidenceConstIterator i, iEnd;
  // Add the incident simplices for the vertices which are surface features.
  for (int m = 0; m != M + 1; ++m) {
    // The vertex index.
    vertexIndex = _surfaceManifold.getIndexedSimplex(simplexIndex)[m];
    // If the vertex is a surface feature.
    if (isVertexASurfaceFeature(vertexIndex)) {
      // Add the incident simplices.
      iEnd = _surfaceManifold.getIncidentEnd(vertexIndex);
      for (i = _surfaceManifold.getIncidentBeginning(vertexIndex); i != iEnd; ++i) {
	*iter = *i;
      }
    }
  }
}



template<typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<3,2,1,T>::
getEdgeNeighborhood(const int simplexIndex, IntInsertIterator iter) const {
  typedef typename EdgeManifold::IncidenceConstIterator IncidenceConstIterator;

  // No need to validate the simplex index.  That will be done by the ISS.

  // Add the simplex.
  *iter = simplexIndex;

  int vertexIndex;
  IncidenceConstIterator i, iEnd;
  // For each vertex that is not a corner, add the incident simplices.
  for (int m = 0; m != EdgeManifold::M + 1; ++m) {
    // The vertex that is incident to the adjacent face.
    vertexIndex = _edgeManifold.getIndexedSimplex(simplexIndex)[m];
    // If the vertex is not a corner.
    if (! isVertexACornerFeature(vertexIndex)) {
      // Add the incident simplices.
      iEnd = _edgeManifold.getIncidentEnd(vertexIndex);
      for (i = _edgeManifold.getIncidentBeginning(vertexIndex); i != iEnd; ++i) {
	*iter = *i;
      }
    }
  }
}




// Get the incident surface simplex indices.
template<typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<3,2,1,T>::
getCornerNeighborhood(const int vertexIndex, IntInsertIterator iter) const {
  typedef typename SurfaceManifold::IncidenceConstIterator 
    IncidenceConstIterator;

  // The vertex must be a corner.
  assert(isVertexACornerFeature(vertexIndex));
  // There must be at least one incident simplex.
  assert(! _surfaceManifold.isIncidentEmpty(vertexIndex));

  // Add the incident simplices.
  IncidenceConstIterator i, iEnd;
  iEnd = _surfaceManifold.getIncidentEnd(vertexIndex);
  for (i = _surfaceManifold.getIncidentBeginning(vertexIndex); i != iEnd; 
       ++i) {
    *iter = *i;
  }
}

END_NAMESPACE_GEOM

// End of file.
