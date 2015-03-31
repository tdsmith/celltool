// -*- C++ -*-

#if !defined(__geom_mesh_iss_penetration_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

template<int N, bool A, typename T, typename V, typename IS, 
	 typename PointRandomAccessIterator,
	 typename TupleOutputIterator>
inline
int
reportPenetrations(const IndSimpSetIncAdj<N,N,A,T,V,IS>& mesh,
		   PointRandomAccessIterator pointsBeginning,
		   PointRandomAccessIterator pointsEnd,
		   TupleOutputIterator penetrations) {
  typedef IndSimpSetIncAdj<N,N,A,T,V,IS> Mesh;
  typedef IndSimpSet<N,N - 1,true,T,V> Boundary;
  typedef ISS_SignedDistance<Boundary, N> SignedDistance;
  typedef typename Mesh::Number Number;
  typedef typename Mesh::Vertex Point;
  typedef typename Mesh::Simplex Simplex;

  typedef CellArray<N, PointRandomAccessIterator> Orq;

  typedef std::map<int, std::pair<int, Number> > Map;
  typedef typename Map::value_type MapValue;

  Orq orq(1000, pointsBeginning, pointsEnd);

  //
  // Find the points that are inside simplices.
  //
  std::vector<PointRandomAccessIterator> pointsInside;
  Map indexAndDistance;
  Simplex simplex;
  BBox<N, Number> bbox;
  Number distance;
  // For each simplex in the mesh.
  for (int simplexIndex = 0; simplexIndex != mesh.getSimplicesSize(); 
       ++simplexIndex) {
    mesh.getSimplex(simplexIndex, &simplex);
    // Make a bounding box around the simplex.
    simplex.computeBBox(&bbox);
    // Get the points in the bounding box.
    pointsInside.clear();
    orq.computeWindowQuery(std::back_inserter(pointsInside), bbox);
    // For each point inside the bounding box.
    for (typename std::vector<PointRandomAccessIterator>::const_iterator 
	   i = pointsInside.begin(); i != pointsInside.end(); ++i) {
      const Point& point = **i;
      // If the point is inside the simplex.
      if (isIn(simplex, point)) {
	const int pointIndex = *i - pointsBeginning;
	// Compute the distance to the simplex.
	distance = computeDistanceInterior(simplex, point);
	// See if this point is inside another simplex.
	typename Map::iterator i = indexAndDistance.find(pointIndex);
	// If this point has not yet been found in another simplex.
	if (i == indexAndDistance.end()) {
	  indexAndDistance.insert(MapValue(pointIndex,
					   std::make_pair(simplexIndex,
							  distance)));
	}
	// If this point is also in another simplex, but is further inside 
	// this one.
	else if (distance < i->second.second) {
	  i->second.first = simplexIndex;
	  i->second.second = distance;
	}
      }
    }
  }
  
  // Early exit if there are no penetrating points.
  if (indexAndDistance.empty()) {
    return 0;
  }

  //
  // Determine the closest points for the penetrating points
  //
  // Make the boundary mesh.
  Boundary boundary;
  buildBoundary(mesh, &boundary);
  // Make the data structure for computing the signed distance and closest 
  // point on the boundary.
  SignedDistance signedDistance(boundary);
  Point closestPoint;
  // For each point inside a simplex.
  for (typename Map::const_iterator i = indexAndDistance.begin();
       i != indexAndDistance.end(); ++i) {
    // Compute the closest point on the boundary. Ignore the signed distance.
    signedDistance(pointsBeginning[i->first], &closestPoint);
    // Record the penetration: point index, simplex index, and closest point.
    *penetrations++ = std::tr1::make_tuple(i->first, i->second.first, 
					   closestPoint);
  }

  // Return the number of penetrations.
  return indexAndDistance.size();
}

END_NAMESPACE_GEOM
