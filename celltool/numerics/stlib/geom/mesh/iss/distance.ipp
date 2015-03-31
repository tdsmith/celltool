// -*- C++ -*-

#if !defined(__geom_mesh_iss_distance_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

//---------------------------------------------------------------------------
// Signed distance, 2-D space, 1-manifold, single point.
//---------------------------------------------------------------------------

// Compute the signed distance to the mesh and closest point on the mesh.
template<bool _A, typename _T, typename _V, typename _IS>
typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 const ads::Array<1, typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number>&
 squaredHalfLengths,
 const typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex* closestPoint) {
  // Types.
  typedef typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number Number;
  typedef typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex Vertex;
  typedef typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Simplex Simplex;

  // Check for an empty mesh.
  if (mesh.areSimplicesEmpty()) {
    return std::numeric_limits<Number>::max();
  }
  assert(! mesh.areVerticesEmpty());

  //
  // Find the closest vertex.
  //
  int closestVertexIndex = -1;
  ads::Array<1, Number> vertexSquaredDistances(mesh.getVerticesSize());
  Number minimumSquaredDistance = std::numeric_limits<Number>::max();
  for (int i = 0; i != mesh.getVerticesSize(); ++i) {
    vertexSquaredDistances[i] = 
      computeSquaredDistance(point, mesh.getVertex(i));
    if (vertexSquaredDistances[i] < minimumSquaredDistance) {
      closestVertexIndex = i;
      minimumSquaredDistance = vertexSquaredDistances[i];
    }
  }
  assert(closestVertexIndex != -1);
  
  // Compute the signed distance to the closest vertex.
  *closestPoint = mesh.getVertex(closestVertexIndex);
  Number signedDistance =
    computeSignedDistance(*closestPoint,
			  computeVertexNormal(mesh, closestVertexIndex), point);
  
  //
  // Check the faces.
  //
  Simplex face;
  Number sd;
  Vertex cp;
  for (int i = 0; i != mesh.getSimplicesSize(); ++i) {
    const int sourceIndex = mesh.getIndexedSimplex(i)[0];
    const int targetIndex = mesh.getIndexedSimplex(i)[1];
    // A lower bound on the squared distance to the face.  Subtract the
    // square of half of the edge length from the squared distance to the
    // closer end point.
    Number lowerBound = std::min(vertexSquaredDistances[sourceIndex],
				 vertexSquaredDistances[targetIndex]) -
      squaredHalfLengths[i];
    if (lowerBound < minimumSquaredDistance) {
      // Compute the signed distance and closest point on the face.
      mesh.getSimplex(i, &face);
      sd = computeSignedDistance(face, point, &cp);
      // If the distance is smaller than previous ones.
      if (std::abs(sd) < std::abs(signedDistance)) {
	signedDistance = sd;
	*closestPoint = cp;
	minimumSquaredDistance = sd * sd;
      }
    }
  }

  return signedDistance;
}

// Compute the signed distance to the mesh and closest point on the mesh.
template<bool _A, typename _T, typename _V, typename _IS>
inline
typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 const typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex* closestPoint) {
  typedef typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number Number;
  ads::Array<1, Number> squaredHalfLengths(mesh.getSimplicesSize());
  for (int i = 0; i != mesh.getSimplicesSize(); ++i) {
    squaredHalfLengths[i] = 0.25 * 
      computeSquaredDistance(mesh.getSimplexVertex(i, 0),
			     mesh.getSimplexVertex(i, 1));
  }
  return computeSignedDistance(mesh, squaredHalfLengths, point, closestPoint);
}

//---------------------------------------------------------------------------
// Signed distance, 2-D space, 1-manifold, multiple points.
//---------------------------------------------------------------------------

//! Compute the signed distances to the mesh and closest points on the mesh.
template<bool _A, typename _T, typename _V, typename _IS, 
	 typename InputIterator, typename NumberOutputIterator,
	 typename PointOutputIterator>
inline
void
computeSignedDistance
(const IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>& mesh,
 InputIterator pointsBeginning, InputIterator pointsEnd,
 NumberOutputIterator distances, PointOutputIterator closestPoints) {
  typedef typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Number Number;
  typedef typename IndSimpSetIncAdj<2,1,_A,_T,_V,_IS>::Vertex Vertex;

  // Compute the square of the half edge lengths.
  ads::Array<1, Number> squaredHalfLengths(mesh.getSimplicesSize());
  for (int i = 0; i != mesh.getSimplicesSize(); ++i) {
    squaredHalfLengths[i] = 0.25 * 
      computeSquaredDistance(mesh.getSimplexVertex(i, 0),
			     mesh.getSimplexVertex(i, 1));
  }

  Vertex cp;
  for ( ; pointsBeginning != pointsEnd; ++pointsBeginning) {
    *distances++ = computeSignedDistance(mesh, squaredHalfLengths, 
					 *pointsBeginning, &cp);
    *closestPoints++ = cp;
  }
}

//---------------------------------------------------------------------------
// Signed distance, 3-D space, 2-manifold, single point.
//---------------------------------------------------------------------------

// Compute the signed distance to the mesh and closest point on the mesh.
template<bool _A, typename _T, typename _V, typename _IS>
typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 const ads::Array<1, typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number>&
 squaredLongestEdgeLengths,
 const typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex* closestPoint) {
  // Types.
  typedef IndSimpSetIncAdj<3,2,_A,_T,_V,_IS> Mesh;
  typedef typename Mesh::Number Number;
  typedef typename Mesh::Vertex Vertex;
  typedef typename Mesh::Simplex Simplex;

  const int M = Mesh::M;

  // Check for an empty mesh.
  if (mesh.areSimplicesEmpty()) {
    return std::numeric_limits<Number>::max();
  }
  assert(! mesh.areVerticesEmpty());

  //
  // Find the closest vertex.
  //
  int closestVertexIndex = -1;
  ads::Array<1, Number> vertexSquaredDistances(mesh.getVerticesSize());
  Number minimumSquaredDistance = std::numeric_limits<Number>::max();
  for (int i = 0; i != mesh.getVerticesSize(); ++i) {
    vertexSquaredDistances[i] = 
      computeSquaredDistance(point, mesh.getVertex(i));
    if (vertexSquaredDistances[i] < minimumSquaredDistance) {
      closestVertexIndex = i;
      minimumSquaredDistance = vertexSquaredDistances[i];
    }
  }
  assert(closestVertexIndex != -1);
  
  // Compute the signed distance to the closest vertex.
  *closestPoint = mesh.getVertex(closestVertexIndex);
  Number signedDistance =
    computeSignedDistance(*closestPoint,
			  computeVertexNormal(mesh, closestVertexIndex), point);
  
  //
  // Check the faces and edges.
  //
  Simplex face;
  geom::Simplex<1, Vertex, Number> edge;
  Number sd;
  Vertex cp, faceNormal, edgeNormal;
  for (int i = 0; i != mesh.getSimplicesSize(); ++i) {
    // A lower bound on the squared distance to the face.  Subtract the
    // square of the longest edge length (of the simplex) from the squared
    // distance to the closest vertex (of the simplex).
    Number lowerBound = 
      ads::min(vertexSquaredDistances[mesh.getIndexedSimplex(i)[0]],
	       vertexSquaredDistances[mesh.getIndexedSimplex(i)[1]],
	       vertexSquaredDistances[mesh.getIndexedSimplex(i)[2]]) -
      squaredLongestEdgeLengths[i];
    if (lowerBound < minimumSquaredDistance) {
      // Compute the signed distance and closest point on the face.
      mesh.getSimplex(i, &face);
      computeSimplexNormal(mesh, i, &faceNormal);
      sd = computeSignedDistance(face, faceNormal, point, &cp);
      // If the distance is smaller than previous ones.
      if (std::abs(sd) < std::abs(signedDistance)) {
	signedDistance = sd;
	*closestPoint = cp;
	minimumSquaredDistance = sd * sd;
      }
      // Compute the signed distance and closest point on the on the 
      // appropriate edges.  (We don't need to check edges that will be
      // processed by other faces.)
      for (int n = 0; n != M + 1; ++n) {
	int adjacentIndex = mesh.getAdjacent(i, n);
	// If this face is responsible for this edge.
	if (i < adjacentIndex) {
	  // Make the edge.
	  edge[0] = face[(n + 1) % (M + 1)];
	  edge[1] = face[(n + 2) % (M + 1)];
	  // Calculate the edge normal.
	  computeSimplexNormal(mesh, i, &edgeNormal);
	  computeSimplexNormal(mesh, adjacentIndex, &faceNormal);
	  edgeNormal += faceNormal;
	  normalize(&edgeNormal);
	  // Compute the signed distance and closest point on the edge.
	  sd = computeSignedDistance(edge, edgeNormal, point, &cp);
	  // If the distance is smaller than previous ones.
	  if (std::abs(sd) < std::abs(signedDistance)) {
	    signedDistance = sd;
	    *closestPoint = cp;
	    minimumSquaredDistance = sd * sd;
	  }
	}
      }
    }
  }

  return signedDistance;
}

// Compute the signed distance to the mesh and closest point on the mesh.
template<bool _A, typename _T, typename _V, typename _IS>
inline
typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 const typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex& point,
 typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex* closestPoint) {
  typedef typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number Number;

  // Compute the square of the longest edge lengths.
  ads::Array<1, Number> squaredLongestEdgeLengths(mesh.getSimplicesSize());
  for (int i = 0; i != mesh.getSimplicesSize(); ++i) {
    squaredLongestEdgeLengths[i] = 
      ads::max(computeSquaredDistance(mesh.getSimplexVertex(i, 0),
				      mesh.getSimplexVertex(i, 1)),
	       computeSquaredDistance(mesh.getSimplexVertex(i, 1),
				      mesh.getSimplexVertex(i, 2)),
	       computeSquaredDistance(mesh.getSimplexVertex(i, 2),
				      mesh.getSimplexVertex(i, 0)));
  }

  return computeSignedDistance(mesh, squaredLongestEdgeLengths, point, 
			       closestPoint);
}

//---------------------------------------------------------------------------
// Signed distance, 3-D space, 2-manifold, multiple points.
//---------------------------------------------------------------------------

//! Compute the signed distances to the mesh and closest points on the mesh.
template<bool _A, typename _T, typename _V, typename _IS, 
	 typename InputIterator, typename NumberOutputIterator,
	 typename PointOutputIterator>
inline
void
computeSignedDistance
(const IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>& mesh,
 InputIterator pointsBeginning, InputIterator pointsEnd,
 NumberOutputIterator distances, PointOutputIterator closestPoints) {
  typedef typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Number Number;
  typedef typename IndSimpSetIncAdj<3,2,_A,_T,_V,_IS>::Vertex Vertex;

  // Compute the square of the longest edge lengths.
  ads::Array<1, Number> squaredLongestEdgeLengths(mesh.getSimplicesSize());
  for (int i = 0; i != mesh.getSimplicesSize(); ++i) {
    squaredLongestEdgeLengths[i] = 
      ads::max(computeSquaredDistance(mesh.getSimplexVertex(i, 0),
				      mesh.getSimplexVertex(i, 1)),
	       computeSquaredDistance(mesh.getSimplexVertex(i, 1),
				      mesh.getSimplexVertex(i, 2)),
	       computeSquaredDistance(mesh.getSimplexVertex(i, 2),
				      mesh.getSimplexVertex(i, 0)));
  }

  Vertex cp;
  for ( ; pointsBeginning != pointsEnd; ++pointsBeginning) {
    *distances++ = computeSignedDistance(mesh, squaredLongestEdgeLengths, 
					 *pointsBeginning, &cp);
    *closestPoints++ = cp;
  }
}

END_NAMESPACE_GEOM

// End of file.
