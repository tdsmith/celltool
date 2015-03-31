// -*- C++ -*-

#if !defined(__geom_PointsOnManifoldN11_ipp__)
#error This file is an implementation detail of the class PointsOnManifold.
#endif

BEGIN_NAMESPACE_GEOM


//--------------------------------------------------------------------------
// Constructors etc.
//--------------------------------------------------------------------------


// Construct from the mesh and the corners.
template<int _N, typename T>
template<bool A, typename V, typename IS, typename IntInIter>
inline
PointsOnManifold<_N,1,1,T>::
PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 IntInIter cornersBegin, IntInIter cornersEnd) :
  // Build the surface manifold.
  _surfaceManifold(iss),
  // Empty array.  We'll build it later.
  _cornerIndices(),
  // Initially, record everything as being a surface feature.
  _vertexFeatures(_surfaceManifold.getVerticesSize(), SurfaceFeature),
  // Initially, there are no registered points on the manifold.
  _points(),
  // Build the simplex query data structure.
  _simplexQuery(_surfaceManifold),
  // Give the max corner distance a sensible initial value.
  _maxCornerDistance(0.1 * computeMinimumEdgeLength(_surfaceManifold)),
  // Do not bother to initialize.
  _cachedIdentifier(),
  // We initialize below.
  _cachedFace() {
  // Set the corner features.
  for (; cornersBegin != cornersEnd; ++cornersBegin) {
    _vertexFeatures[*cornersBegin] = CornerFeature;
  }

  // Boundary vertices are corners as well.
  determineBoundaryCorners();

  // Record the corners.  This builds the _cornerIndices array.
  recordCorners();

  // Invalidate the cached face, just in case...
  _cachedFace.setToNull();
}


// Construct from the mesh and an angle to define corners.
template<int _N, typename T>
template<bool A, typename V, typename IS>
inline
PointsOnManifold<_N,1,1,T>::
PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 const Number maxAngleDeviation) :
  // Build the surface manifold.
  _surfaceManifold(iss),
  // Empty array.  We'll build it later.
  _cornerIndices(),
  // Initially, record everything as being a surface feature.
  _vertexFeatures(_surfaceManifold.getVerticesSize(), SurfaceFeature),
  // Initially, there are no registered points on the manifold.
  _points(),
  // Build the simplex query data structure.
  _simplexQuery(_surfaceManifold),
  // Give the max corner distance a sensible initial value.
  _maxCornerDistance(0.1 * computeMinimumEdgeLength(_surfaceManifold)),
  // Do not bother to initialize.
  _cachedIdentifier(),
  // We initialize below.
  _cachedFace() {
  // If the max angle deviation has been specified.
  if (maxAngleDeviation >= 0) {
    // Determine the corner features.
    determineCorners(maxAngleDeviation);
  }
  else {
    determineBoundaryCorners();
  }

  // Record the corners.  This builds the _cornerIndices array.
  recordCorners();

  // Invalidate the cached face, just in case...
  _cachedFace.setToNull();
}


//--------------------------------------------------------------------------
// Accessors.
//--------------------------------------------------------------------------


// Count the number of registered points on each feature.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
countNumberOfPointsOnFeatures(SizeType* surfaceCount, 
			      SizeType* cornerCount) const {
  *surfaceCount = 0;
  *cornerCount = 0;
  for (typename PointMap::const_iterator i = _points.begin(); 
       i != _points.end(); ++i) {
    if (i->second.getFeature() == SurfaceFeature) {
      ++*surfaceCount;
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
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::SizeType
PointsOnManifold<_N,1,1,T>::
countNumberOfSurfacePoints() const {
  SizeType surfaceCount, cornerCount;
  countNumberOfPointsOnFeatures(&surfaceCount, &cornerCount);
  return surfaceCount;
}


// Count the number of points on corner features.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::SizeType
PointsOnManifold<_N,1,1,T>::
countNumberOfCornerPoints() const {
  SizeType surfaceCount, cornerCount;
  countNumberOfPointsOnFeatures(&surfaceCount, &cornerCount);
  return cornerCount;
}


// Get the simplex index associated with the point.
// The point must be on a surface feature.
template<int _N, typename T>
inline
int
PointsOnManifold<_N,1,1,T>::
getSimplexIndex(const int identifier) const {
  // Find the point.
  typename PointMap::const_iterator i = _points.find(identifier);
  // Make sure it has been registered.
  assert(i != _points.end());
  // It must be a surface feature.
  assert(i->second.getFeature() == SurfaceFeature);
  // Return the simplex index.
  return i->second.getIndex();
}



// Get the vertex index associated with the point.
// The point must be on a corner feature.
template<int _N, typename T>
inline
int
PointsOnManifold<_N,1,1,T>::
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



//--------------------------------------------------------------------------
// Manipulators.
//--------------------------------------------------------------------------



// Change the identifier of a registered point.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
changeIdentifier(const int existingIdentifier, const int newIdentifier) {
  // First check the trivial case.
  if (existingIdentifier == newIdentifier) {
    return;
  }
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
}



//! Change the location of a registered point.
/*!
  Return the new point on the manifold.
*/
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
changeLocation(int pointIdentifier, const Vertex& newLocation) {
  // Find the point and get its face.
  const typename PointMap::iterator pointIterator = 
    _points.find(pointIdentifier);
  // The point must be registered.
  assert(pointIterator != _points.end());
  const FaceHandle face = pointIterator->second;

  // If this is a corner feature, we cannot move it.
  if (face.getFeature() == CornerFeature) {
    return _surfaceManifold.getVertex(face.getIndex());
  }

  // Otherwise, it is a surface feature.
  assert(face.getFeature() == SurfaceFeature);

  // Get the neighborhood of simplices.
  std::set<int> indices;
  getSurfaceNeighborhood(face.getIndex(), 
			 std::inserter(indices, indices.end()));

  // Compute the closest point on the manifold.
  const Vertex closestPoint = 
    computeClosestPointInSimplices(indices.begin(), indices.end(), 
				   newLocation);
  
  // Update the face of the point.  Use the cached face determined in the
  // above closest point function call.
  pointIterator->second = _cachedFace;

  // Return the closest point on the manifold.
  return closestPoint;
}


// Change the surface simplex for a registered surface feature.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
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


//--------------------------------------------------------------------------
// Insert/Erase Points.
//--------------------------------------------------------------------------


// Insert a point at the specified vertex.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
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



// Insert a point at the closest point to the specified position that is 
// near the existing point.  Return the point's position on the manifold.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
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



// Insert a point at the closest point to the specified position that is 
// near one of the existing points.  Return the point's position on the 
// manifold.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
insertNearPoints(const int newPointID, const Vertex& position, 
		 const int existingPointID1, const int existingPointID2) {
  // Make sure the new point has not already been registered.
  assert(_points.count(newPointID) == 0);

  // Get the face for the first existing point.
  FaceHandle existingFace;
  {
    typename PointMap::iterator i = _points.find(existingPointID1);
    // The existing point must be registered.
    assert(i != _points.end());
    existingFace = i->second;
  }
  // Get the neighborhood of simplices of the first point.
  std::set<int> indices;
  getNeighborhood(existingFace, std::inserter(indices, indices.end()));

  // Get the face for the second existing point.
  {
    typename PointMap::iterator i = _points.find(existingPointID2);
    // The existing point must be registered.
    assert(i != _points.end());
    existingFace = i->second;
  }
  // Get the neighborhood of simplices of the second point.
  getNeighborhood(existingFace, std::inserter(indices, indices.end()));

  // Compute the closest point on the manifold.
  const Vertex closestPoint = 
    computeClosestPointInSimplices(indices.begin(), indices.end(), position);
  
  // Insert the new point.
  _points.insert(Point(newPointID, _cachedFace));
  
  return closestPoint;
}



// Insert a point at each vertex.
// The point identifiers will be the vertex indices.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
insertAtVertices() {
  // For each vertex.
  for (int n = 0; n != _surfaceManifold.getVerticesSize(); ++n) {
    // Insert a point at the vertex.
    insertAtVertex(n, n);
  }
}



// Insert a point at the closest point on a surface feature.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
insertOnASurface(const int pointIdentifier, const Vertex& position) {
  // There must be at least one surface simplex.
  assert(_surfaceManifold.getSimplicesSize() > 0);
  // Find the closest surface simplex.
  const int simplexIndex = _simplexQuery.computeMinimumDistanceIndex(position);
  // Check that the simplex index is in the right range.
  assert(0 <= simplexIndex && 
	 simplexIndex < _surfaceManifold.getSimplicesSize());
  // Insert the point in the surface simplex and return the closest point 
  // in the simplex.
  return insertInSimplex(pointIdentifier, position, simplexIndex);
}




// Insert a point at the closest point to the specified position.
// Return the point's position on the manifold.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
insert(int pointIdentifier, const Vertex& position) {
  // First see if we should insert at a corner.
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

  // Otherwise, we will insert the point at a surface feature.
  // Find the closest simplex.
  const int simplexIndex = _simplexQuery.computeMinimumDistanceIndex(position);
  // Insert the point in the simplex and return the closest point in the 
  // simplex.
  return insertInSimplex(pointIdentifier, position, simplexIndex);
}



// Insert a range of points at their closest points.
// The point identifiers will be in the range [0...numPoints).
// Put the positions at the closest points on the manifold to the output 
// iterator.
template<int _N, typename T>
template<typename PointInputIterator, typename PointOutputIterator>
inline
void
PointsOnManifold<_N,1,1,T>::
insert(PointInputIterator locationsBegin, PointInputIterator locationsEnd,
       PointOutputIterator closestPoints) {
  int pointIdentifier = 0;
  while (locationsBegin != locationsEnd) {
    *closestPoints++ = insert(pointIdentifier++, *locationsBegin++);
  }
}

// Insert the boundary vertices of the mesh.
// The point identifiers will be the vertex indices.
template<int _N, typename T>
template<bool A, typename V, typename IS>
inline
void
PointsOnManifold<_N,1,1,T>::
insertBoundaryVertices(IndSimpSetIncAdj<N,M+1,A,T,V,IS>* mesh) {
  // For each vertex in the mesh.
  for (int i = 0; i != mesh->getVerticesSize(); ++i) {
    // If the vertex is on the boundary.
    if (mesh->isVertexOnBoundary(i)) {
      mesh->setVertex(i, insert(i, mesh->getVertex(i)));
    }
  }
}


// Insert the boundary vertices of the mesh.
// The point identifiers will be the vertex indices.
template<int _N, typename T>
template<template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Container>
inline
void
PointsOnManifold<_N,1,1,T>::
insertBoundaryVertices(SimpMeshRed<N,M+1,T,Node,Cell,Container>* mesh) {
  typedef SimpMeshRed<N,M+1,T,Node,Cell> Mesh;
  typedef typename Mesh::NodeIterator NodeIterator;

  // For each node in the mesh.
  for (NodeIterator i = mesh->getNodesBeginning(); i != mesh->getNodesEnd(); 
       ++i) {
    // If the node is on the boundary.
    if (i->isOnBoundary()) {
      // Move the node to lay on the boundary manifold.
      i->setVertex(insert(i->getIdentifier(), i->getVertex()));
    }
  }
}



// Erase a point.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
erase(const int pointIdentifier) {
  // Find the point.
  typename PointMap::iterator i = _points.find(pointIdentifier);
  // The point must be registered.
  assert(i != _points.end());
  // Erase the point.
  _points.erase(i);
}



//--------------------------------------------------------------------------
// Closest Point.
//--------------------------------------------------------------------------



template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
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

  // Otherwise, the face is a simplex.

  // Get the neighborhood of simplices.
  std::set<int> indices;
  getSurfaceNeighborhood(face.getIndex(), 
			 std::inserter(indices, indices.end()));
  
  return computeClosestPointInSimplices(indices.begin(), indices.end(), 
					position);
}



//! Update the face of a registered point.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
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
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
printInformation(std::ostream& out) const {
  out 
    << "The surface manifold has " << _surfaceManifold.getSimplicesSize()
    << " simplices and " << _surfaceManifold.getVerticesSize()
    << " vertices.\n"
    << int(std::count(_vertexFeatures.begin(), _vertexFeatures.end(),
		      CornerFeature))
    << " vertices are corner features.\n"
    << int(std::count(_vertexFeatures.begin(), _vertexFeatures.end(),
		      SurfaceFeature))
    << " vertices are surface features.\n"
    << "There are " << countNumberOfPoints() << " registered points.\n"
    << countNumberOfSurfacePoints() << " are on surface features.\n"
    << countNumberOfCornerPoints() << " are on corner features.\n"
    << "The maximum corner distance is " << getMaxCornerDistance() << "\n";
}


//--------------------------------------------------------------------------
// Private member functions.
//--------------------------------------------------------------------------


// Insert the point in the specified simplex.
// Return the closest point in the simplex.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
insertInSimplex(const int pointIdentifier, const Vertex& position, 
		const int simplexIndex) {
  // Make sure the point has not already been registered.
  assert(_points.count(pointIdentifier) == 0);
  // The simplex index must be valid.
  assert(0 <= simplexIndex && simplexIndex < 
	 _surfaceManifold.getSimplicesSize());

  // Make the point.
  const typename PointMap::value_type 
    x(pointIdentifier, FaceHandle(SurfaceFeature, simplexIndex));
  // Insert the point.
  _points.insert(x);

  // Return the closest point.
  return computeClosestPointInSimplex(simplexIndex, position);
}



// Try to find a corner vertex that is very close to the position.
// If there is a close corner, return the index of the vertex.  
// Otherwise, return -1.
template<int _N, typename T>
inline
int
PointsOnManifold<_N,1,1,T>::
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



template<int _N, typename T>
template<typename IntInputIterator>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
computeClosestPointInSimplices(IntInputIterator indicesBegin, 
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
    distance = computeClosestPointInSimplex(i, position, &closestPoint);
    _cachedFace.setIndex(i);
  }

  // Compute distance and closest point for the rest of the simplices.
  Vertex cp;
  Number d;
  for (; indicesBegin != indicesEnd; ++indicesBegin) {
    i = *indicesBegin;
    d = computeClosestPointInSimplex(i, position, &cp);
    if (d < distance) {
      // Update the closest point.
      closestPoint = cp;
      // Update the new face handle.
      _cachedFace.setIndex(i);
    }
  }

  return closestPoint;
}




template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Number
PointsOnManifold<_N,1,1,T>::
computeClosestPointInSimplex(const int simplexIndex, const Vertex& x, 
			     Vertex* closestPoint) const {
  // Make a segment from the two vertices.
  geom::SegmentMath<N,T> 
    segment(_surfaceManifold.getSimplexVertex(simplexIndex,0), 
	    _surfaceManifold.getSimplexVertex(simplexIndex,1));
  // Compute the closest point.  Return the unsigned distance.
  return computeDistanceAndClosestPoint(segment, x, closestPoint);
}



// Return the closest point to the n_th simplex.
template<int _N, typename T>
inline
typename PointsOnManifold<_N,1,1,T>::Vertex
PointsOnManifold<_N,1,1,T>::
computeClosestPointInSimplex(const int simplexIndex, const Vertex& x) const {
  Vertex closestPoint;
  computeClosestPointInSimplex(simplexIndex, x, &closestPoint);
  return closestPoint;
}



// Return true if the point is on the specified feature.
template<int _N, typename T>
inline
bool
PointsOnManifold<_N,1,1,T>::
isOnFeature(const int pointIdentifier, const Feature feature) const {
  // Find the point by its identifier.
  typename PointMap::const_iterator i = _points.find(pointIdentifier);
  // Assert that we found it.
  assert(i != _points.end());
  return i->second.getFeature() == feature;
}


// Return true if the face is valid.
template<int _N, typename T>
inline
bool
PointsOnManifold<_N,1,1,T>::
isValid(const FaceHandle& face) const {
  // If it is the null feature.
  if (face.getFeature() == NullFeature) {
    // The null feature is not valid.
    return false;
  }
  // If it is a corner feature.
  if (face.getFeature() == CornerFeature) {
    return 0 <= face.getIndex() && face.getIndex() < 
      _surfaceManifold.getVerticesSize();
  }
  // Else, it is a surface feature
  assert(face.getFeature() == SurfaceFeature);
  return 0 <= face.getIndex() && face.getIndex() < 
    _surfaceManifold.getSimplicesSize();
}



// Get the simplices in the neighborhood of the face. (0-face or 1-face)
template<int _N, typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<_N,1,1,T>::
getNeighborhood(const FaceHandle& face, IntInsertIterator iter) const {
  if (face.getFeature() == CornerFeature) {
    getCornerNeighborhood(face.getIndex(), iter);
  }
  else if (face.getFeature() == SurfaceFeature) {
    getSurfaceNeighborhood(face.getIndex(), iter);
  }
  else {
    assert(false);
  }
}



// Get the simplex index and the adjacent simplex indices.
// Do not step across corners.
template<int _N, typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<_N,1,1,T>::
getSurfaceNeighborhood(const int simplexIndex, IntInsertIterator iter) const {
  // No need to validate the simplex index.  That will be done by the ISS.

  // Add the simplex.
  *iter = simplexIndex;

  // Add the adjacent simplices which are not separated by a corner.
  for (int i = 0; i != M + 1; ++i) {
    // The index of the adjacent simplex.
    const int adjacentIndex = _surfaceManifold.getAdjacent(simplexIndex, i);
    // If there is an adjacent simplex in this direction.
    if (adjacentIndex != -1) {
      // The vertex that is incident to the adjacent face.
      const int vertexIndex = 
	_surfaceManifold.getIndexedSimplex(simplexIndex)[(i+1)%(M+1)];
      // If the vertex is not a corner.
      if (_vertexFeatures[vertexIndex] == SurfaceFeature) {
	// Add the adjacent simplex.
	*iter = adjacentIndex;
      }
    }
  }
}



// Get the incident simplex indices.
template<int _N, typename T>
template<typename IntInsertIterator>
inline
void
PointsOnManifold<_N,1,1,T>::
getCornerNeighborhood(const int vertexIndex, IntInsertIterator iter) const {
  // The vertex must be a corner.
  assert(_vertexFeatures[vertexIndex] == CornerFeature);
  // There must be at least one incident simplex.
  assert(_surfaceManifold.getIncidentSize(vertexIndex) != 0);

  // For each incident simplex
  for (int i = 0; i != _surfaceManifold.getIncidentSize(vertexIndex); ++i) {
    // Add the incident simplex.
    *iter = _surfaceManifold.getIncident(vertexIndex, i);
  }
}


// Determine the corners from the maximum allowed angle deviation.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
determineCorners(const Number maxAngleDeviation) {
  // The maximum cosine for a surface vertex.
  const Number maxCosine = std::cos(numerical::Constants<Number>::Pi() - 
				    maxAngleDeviation);
  
  // For each vertex.
  for (int n = 0; n != _surfaceManifold.getVerticesSize(); ++n) {
    // Interior vertex.
    if (_surfaceManifold.getIncidentSize(n) == 2) {
      // If the angle is not too sharp.
      if (computeCosineAngle(_surfaceManifold, n) <= maxCosine) {
	_vertexFeatures[n] = SurfaceFeature;
      }
      else {
	_vertexFeatures[n] = CornerFeature;
      }
    }
    // Boundary vertex.
    else if (_surfaceManifold.getIncidentSize(n) == 1) {
      _vertexFeatures[n] = CornerFeature;
    }
    // Vertices without incident simplices are not allowed.
    else {
      assert(false);
    }
  }
}


// Determine the boundary corners.
template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
determineBoundaryCorners() {
  int numIncident;
  // For each vertex.
  for (int n = 0; n != _surfaceManifold.getVerticesSize(); ++n) {
    // The number of incident simplices.
    numIncident = _surfaceManifold.getIncidentSize(n);
    assert(numIncident == 1 || numIncident == 2);
    // If this is a boundary vertex.
    if (numIncident == 1) {
      // It is a corner feature.
      _vertexFeatures[n] = CornerFeature;
    }
  }
}


template<int _N, typename T>
inline
void
PointsOnManifold<_N,1,1,T>::
recordCorners() {
  std::vector<int> indices;
  // For each vertex.
  for (int i = 0; i != _vertexFeatures.size(); ++i) {
    // If the vertex is a corner feature.
    if (_vertexFeatures[i] == CornerFeature) {
      indices.push_back(i);
    }
  }
  // Copy the indices into the corner indices array.
  _cornerIndices = ads::Array<1,int>(indices.begin(), indices.end());
}

END_NAMESPACE_GEOM

// End of file.
