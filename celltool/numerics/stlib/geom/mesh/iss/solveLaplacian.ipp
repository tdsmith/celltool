// -*- C++ -*-

#if !defined(__geom_mesh_iss_solveLaplacian_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
solveLaplacian(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh) {
  //
  // Determine which vertices are interior.
  //
  ads::Array<1,bool> interior(mesh->getVerticesSize());
  // Interior indices.
  ads::Array<1,int> ii(mesh->getVerticesSize(), -1);
  int numInterior = 0;
  for (int v = 0; v != mesh->getVerticesSize(); ++v) {
    if (mesh->isVertexOnBoundary(v)) {
      interior[v] = false;
    }
    else {
      interior[v] = true;
      ii[v] = numInterior++;
    }
  }

  TNT::Array2D<T> m(numInterior, numInterior, 0.0);
  TNT::Array1D<T> b(numInterior, 0.0);

  std::set<int> neighbors;

  int i, j;
  // For each space dimension.
  for (int n = 0; n != N; ++n) {

    //
    // Make the matrix, m, and the right hand side, b.
    //

    m = 0.0;
    b = 0.0;
    // For each interior vertex.
    for (int v = 0; v != mesh->getVerticesSize(); ++v) {
      if (interior[v]) {
	i = ii[v];
	// Get the neighboring vertex indices.
	getNeighbors(*mesh, v, neighbors);
	// The number of neighbors.
	m[i][i] = neighbors.size();
	// For each neighbor.
	for (std::set<int>::const_iterator iter = neighbors.begin();
	     iter != neighbors.end(); ++iter) {
	  j = ii[*iter];
	  // If the neighbor is an interior vertex.
	  if (interior[*iter]) {
	    m[i][j] = - 1.0;
	  }
	  else {
	    b[i] += mesh->getVertex(*iter)[n];
	  }
	}
      }
    }

    //
    // Solve the linear system.
    //

    JAMA::LU<T> lu(m);
    TNT::Array1D<T> x = lu.solve(b);
    // Set the vertex positions.
    for (int v = 0; v != mesh->getVerticesSize(); ++v) {
      if (interior[v]) {
	i = ii[v];
	mesh->getVertices()[v][n] = x[i];
      }
    }
  }  
}

END_NAMESPACE_GEOM

// End of file.
