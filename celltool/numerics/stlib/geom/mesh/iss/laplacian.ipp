// -*- C++ -*-

#if !defined(__geom_mesh_iss_laplacian_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// CONTINUE: Move
// Get the neighboring vertices of a vertex.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
getNeighbors(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, const int index,
	     std::set<int>& indexSet) {
  indexSet.clear();

  int simp, vert;
  // For each incident simplex.
  for (int n = 0; n != mesh.getIncidentSize(index); ++n) {
    // The index of the simplex.
    simp = mesh.getIncident(index, n);
    // For each vertex of the simplex.
    for (int m = 0; m != M + 1; ++m) {
      // The vertex index.
      vert = mesh.getIndexedSimplex(simp)[m];
      // Don't include the index_th vertex.
      if (vert != index) {
	// Add it to the set (if it is not already in the set).
	indexSet.insert(vert);
      }
    }
  }
}


// CONTINUE This will give me boundary vertices that do not share a boundary
// face with the specified vertex.  Is this what I want?

// CONTINUE: Move
// Get the neighboring boundary vertices of a vertex.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
getBoundaryNeighbors(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
		     const int index, std::set<int>& indexSet) {
  indexSet.clear();

  int simp, vert;
  // For each incident simplex.
  for (int n = 0; n != mesh.getIncidentSize(index); ++n) {
    // The index of the simplex.
    simp = mesh.getIncident(index, n);
    // For each vertex of the simplex.
    for (int m = 0; m != M + 1; ++m) {
      // The vertex index.
      vert = mesh.getIndexedSimplex(simp)[m];
      // Don't include the index_th vertex.
      if (vert != index && mesh.isVertexOnBoundary(vert)) {
	// Add it to the set (if it is not already in the set).
	indexSet.insert(vert);
      }
    }
  }
}






template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
applyLaplacian(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh, const int numSweeps) {
  typedef IndSimpSetIncAdj<N,M,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  
  assert(numSweeps >= 0);

  //
  // Determine which vertices are interior.
  //
  ads::Array<1,bool> interior(mesh->getVerticesSize());
  for (int v = 0; v != mesh->getVerticesSize(); ++v) {
    interior[v] = ! mesh->isVertexOnBoundary(v);
  }

  std::set<int> neighbors;
  Vertex pt;

  // Loop for the number of sweeps.
  for (int sweep = 0; sweep != numSweeps; ++sweep) {
    // For each interior vertex.
    for (int v = 0; v != mesh->getVerticesSize(); ++v) {
      if (interior[v]) {
	// Get the neighboring vertex indices.
	getNeighbors(*mesh, v, neighbors);
	assert(neighbors.size() != 0);
	pt = 0.0;
	// For each neighbor.
	for (std::set<int>::const_iterator iter = neighbors.begin();
	     iter != neighbors.end(); ++iter) {
	  pt += mesh->getVertex(*iter);
	}
	pt /= neighbors.size();
	mesh->getVertices()[v] = pt;
      }
    }
  }
}



// Perform sweeps of Laplacian smoothing on the boundary vertices.
template<bool A, typename T, typename V, typename IS, class BoundaryCondition>
inline
void
applyLaplacian(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
	       const BoundaryCondition& condition, 
	       const T maxAngleDeviation, int numSweeps) {
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  
  assert(numSweeps >= 0);

  // The maximum cosine for moving a vertex.
  const T maxCosine = std::cos(numerical::Constants<T>::Pi() - 
			       maxAngleDeviation);

  Vertex a;

  const int size = mesh->getVerticesSize();

  // Loop for the number of sweeps.
  while (numSweeps-- != 0) {
    // Loop over the vertices.
    for (int n = 0; n != size; ++n) {
      // Don't alter boundary vertices, they have only one incident edge.
      if (mesh->isVertexOnBoundary(n)) {
	continue;
      }

      // Don't alter the vertex if the angle is too sharp.
      if (computeCosineAngle(*mesh, n) > maxCosine) {
	continue;
      }

      //
      // Laplacian smoothing on the vertex.
      //
      
      // Averaging.
      a = getPreviousVertex(*mesh, n);
      a += getNextVertex(*mesh, n);
      a *= 0.5;
      // Set the position.
      mesh->getVertices()[n] = a;
      // Apply the boundary condition.
      applyBoundaryCondition(mesh, condition, n);
    }
  }
}




// Perform sweeps of Laplacian smoothing on the vertices.
template<bool A, typename T, typename V, typename IS, int SD>
inline
void
applyLaplacian(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh,
	       PointsOnManifold<2,1,SD,T>* manifold,
	       int numSweeps) {
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  
  assert(numSweeps >= 0);

  Vertex x;

  const int size = mesh->getVerticesSize();

  // Loop for the number of sweeps.
  while (numSweeps-- != 0) {
    // Loop over the vertices.
    for (int n = 0; n != size; ++n) {
      // Don't alter corner vertices.
      if (manifold->isOnCorner(n)) {
	continue;
      }

      //
      // Laplacian smoothing on the vertex.
      //
      
      // Averaging.
      x = getPreviousVertex(*mesh, n);
      x += getNextVertex(*mesh, n);
      x *= 0.5;
      // Compute the closest point on the manifold.
      x = manifold->computeClosestPoint(n, x);
      // Update the point information on the manifold.
      manifold->updatePoint();
      // Set the position.
      mesh->getVertices()[n] = x;
    }
  }
}



// Perform sweeps of Laplacian smoothing on the boundary vertices.
template<bool A, typename T, typename V, typename IS, class BoundaryCondition>
inline
void
applyLaplacian(IndSimpSetIncAdj<3,2,A,T,V,IS>* mesh, 
	       const BoundaryCondition& condition, 
	       const T maxAngleDeviation, int numSweeps) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  assert(numSweeps >= 0);

  const int size = mesh->getVerticesSize();

  const bool areCheckingAngle = 
    (maxAngleDeviation < 2 * numerical::Constants<T>::Pi() ? true : false);
  
  std::set<int> neighbors;
  Vertex pt;

  // Loop for the number of sweeps.
  while (numSweeps-- != 0) {
    // Loop over the vertices.
    for (int n = 0; n != size; ++n) {
      // Don't alter boundary vertices.
      if (mesh->isVertexOnBoundary(n)) {
	continue;
      }

      // The vertex should have at least three incident faces.
      assert(mesh->getIncidentSize(n) >= 3);


      if (areCheckingAngle) {
	// Don't alter the vertex if the angle is too sharp.
	if (std::abs(computeAngle(*mesh, n) - 
		     2 * numerical::Constants<T>::Pi()) > maxAngleDeviation) {
	  continue;
	}
      }

      //
      // Laplacian smoothing on the vertex.
      //
      
      // Get the neighboring boundary vertex indices.
      getNeighbors(*mesh, n, neighbors);
      assert(neighbors.size() >= 3);
      pt = 0.0;
      // For each neighbor.
      for (std::set<int>::const_iterator iter = neighbors.begin();
	   iter != neighbors.end(); ++iter) {
        pt += mesh->getVertex(*iter);
      }
      pt /= neighbors.size();
      mesh->getVertices()[n] = pt;
      // Apply the boundary condition.
      applyBoundaryCondition(mesh, condition, n);
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
