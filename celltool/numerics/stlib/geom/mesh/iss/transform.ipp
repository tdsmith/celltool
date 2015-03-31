// -*- C++ -*-

#if !defined(__geom_mesh_iss_transform_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

template<int N, int M, typename T, typename V, typename IS, 
	 typename IntOutputIterator>
inline
void
pack(IndSimpSet<N,M,true,T,V,IS>* mesh, IntOutputIterator usedVertexIndices) {
  typedef IndSimpSet<N,M,true,T,V,IS> ISS;
  typedef typename ISS::IndexedSimplexConstIterator 
    IndexedSimplexConstIterator;
  typedef typename ISS::IndexedSimplexIterator IndexedSimplexIterator;
  typedef typename ISS::VertexContainer VertexContainer;

  // The vertices that are used.
  ads::Array<1,bool> used(mesh->getVerticesSize(), false);

  // Loop over the simplices.
  int m;
  for (IndexedSimplexConstIterator s = mesh->getIndexedSimplicesBeginning(); 
       s != mesh->getIndexedSimplicesEnd(); ++s) {
    // Note which vertices are used.
    for (m = 0; m != M + 1; ++m) {
      used[(*s)[m]] = true;
    }
  }

  // The packed vertices.
  VertexContainer 
    packedVertices(static_cast<int>(std::count(used.begin(), used.end(), 
					       true)));

  //
  // Determine the vertex positions and indices for the new mesh.
  //

  // vertex index.
  int vi = 0; 
  // This array maps the old vertex indices to the packed vertex indices.
  ads::Array<1,int> indices(mesh->getVerticesSize(), -1);
  // Loop over the vertices.
  for (int n = 0; n != mesh->getVerticesSize(); ++n) {
    // If the vertex is used in the packed mesh.
    if (used[n]) {
      // Record in the container of the used vertex indices.
      *usedVertexIndices++ = n;
      // Calculate its index in the packed mesh.
      indices[n] = vi;
      // Add the vertex to the packed mesh.
      packedVertices[vi] = mesh->getVertex(n);
      ++vi;
    }
  }
  
  // Swap the old vertices with the packed vertices.
  mesh->getVertices().swap(packedVertices);

  // Map the vertex indices from the old mesh to the packed mesh.
  for (IndexedSimplexIterator s = mesh->getIndexedSimplicesBeginning(); 
       s != mesh->getIndexedSimplicesEnd(); ++s) {
    for (m = 0; m != M + 1; ++m) {
      (*s)[m] = indices[(*s)[m]];
    }
  }

  // Update any auxilliary topological information.
  mesh->updateTopology();
}



template<int N, bool A, typename T, typename V, typename IS>
inline
void
orientPositive(IndSimpSet<N,N,A,T,V,IS>* mesh) {
  typedef IndSimpSet<N,N,A,T,V,IS> ISS;
  typedef typename ISS::Simplex Simplex;
  typedef typename ISS::IndexedSimplexIterator IndexedSimplexIterator;

  Simplex s;
  SimplexJac<N,T> sj;
  // For each simplex.
  for (IndexedSimplexIterator i = mesh->getIndexedSimplicesBeginning(); 
       i != mesh->getIndexedSimplicesEnd(); ++i) {
    mesh->getSimplex(i, &s);
    sj.setFunction(s);
    // If the content is negative.
    if (sj.getDeterminant() < 0) {
      // Reverse its orientation.
      i->negate();
    }
  }

  // Update any auxilliary topological information.
  mesh->updateTopology();
}



template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
reverseOrientation(IndSimpSet<N,M,A,T,V,IS>* mesh) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;
  typedef typename ISS::IndexedSimplexIterator IndexedSimplexIterator;

  // For each simplex.
  for (IndexedSimplexIterator i = mesh->getIndexedSimplicesBeginning(); 
       i != mesh->getIndexedSimplicesEnd(); ++i) {
    // Reverse the orientation of the indexed simplemesh->
    i->negate();
  }

  // Update any auxilliary topological information.
  mesh->updateTopology();
}



template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
orient(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh) {
  typedef IndSimpSetIncAdj<N,M,A,T,V,IS> ISS;
  typedef typename ISS::IndexedSimplexFace IndexedSimplexFace;

  const int simplicesSize = mesh->getSimplicesSize();

  // Initially flag every simplex as un-oriented.
  std::vector<bool> isSimplexOriented(simplicesSize, false);

  // The number of oriented faces.
  int numOriented = 0;
  
  int n, m, nu, mu;
  IndexedSimplexFace f, g;
  std::stack<int> boundary;
  // While not all simplices have been oriented.
  while (numOriented < simplicesSize) {
      
    // Pick a simplex which currently is un-oriented to have a known 
    // orientation.
    for (n = 0; n != simplicesSize && isSimplexOriented[n]; ++n)
      ;
    assert(n != simplicesSize);
    // Set the orientation of this simplemesh->
    isSimplexOriented[n] = true;
    ++numOriented;

    // Add the simplex to the boundary.
    boundary.push(n);
    
    // Loop until the boundary is empty.
    while (! boundary.empty()) {

      // Get a simplex from the boundary.
      n = boundary.top();
      boundary.pop();

      // For each adjacent simplemesh->
      for (m = 0; m != M + 1; ++m) {
	// The m_th adjacent simplemesh->
	nu = mesh->getAdjacent(n, m);
	// If this is a boundary face or the adjacent simplex already has
	// known orientation, do nothing.
	if (nu == -1 || isSimplexOriented[nu]) {
	  continue;
	}
	
	mu = mesh->getMirrorIndex(n, m);
	mesh->getIndexedSimplex(n).getFace(m, &f);
	mesh->getIndexedSimplex(nu).getFace(mu, &g);
	g.negate();
	if (! haveSameOrientation(f, g)) {
	  // Reverse the orientation of the neighbor.
	  mesh->reverseOrientation(nu);
	  // Add it to the boundary.
	  boundary.push(nu);
	  // The orientation is now known.
	  isSimplexOriented[nu] = true;
	  ++numOriented;
	}
      }
    }
  }
}



// Transform each vertex in the range with the specified function.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter, class UnaryFunction>
inline
void
transform(IndSimpSet<N,M,A,T,V,IS>* mesh,
	  IntForIter begin, IntForIter end, const UnaryFunction& f) {
  std::transform
    (ads::constructArrayIndexingIterator(begin, mesh->getVerticesBeginning()),
     ads::constructArrayIndexingIterator(end, mesh->getVerticesBeginning()),
     ads::constructArrayIndexingIterator(begin, mesh->getVerticesBeginning()),
     f);
}



// Transform each vertex in the mesh with the specified function.
template<int N, int M, bool A, typename T, typename V, typename IS, 
	 class UnaryFunction>
inline
void
transform(IndSimpSet<N,M,A,T,V,IS>* mesh, const UnaryFunction& f) {
  std::transform
    (ads::constructArrayIndexingIterator(ads::constructIntIterator<int>(0), 
					 mesh->getVerticesBeginning()),
     ads::constructArrayIndexingIterator(ads::constructIntIterator<int>
				  (mesh->getVerticesSize()),
					 mesh->getVerticesBeginning()),
     ads::constructArrayIndexingIterator(ads::constructIntIterator<int>(0), 
					 mesh->getVerticesBeginning()),
     f);
}



// Transform each vertex in the range with the closest point in the normal direction.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter, class ISS>
inline
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  IntForIter beginning, IntForIter end,
	  const ISS_SD_ClosestPointDirection<ISS>& f) {
  for (; beginning != end; ++beginning) {
    applyBoundaryCondition(mesh, f, *beginning);
  }
}



// Transform each vertex in the mesh with the closest point in the normal direction.
template<int N, int M, bool A, typename T, typename V, typename IS, class ISS>
inline
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  const ISS_SD_ClosestPointDirection<ISS>& f) {
  const int size = mesh->getVerticesSize();
  for (int n = 0; n != size; ++n) {
    boundary_condition(mesh, f, n);
  }
}



// Transform each vertex in the range with the closer point in the normal direction.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter, class ISS>
inline
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  IntForIter beginning, IntForIter end,
	  const ISS_SD_CloserPointDirection<ISS>& f) {
  for (; beginning != end; ++beginning) {
    applyBoundaryCondition(mesh, f, *beginning);
  }
}



// Transform each vertex in the mesh with the closer point in the normal direction.
template<int N, int M, bool A, typename T, typename V, typename IS, class ISS>
inline
void
transform(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
	  const ISS_SD_CloserPointDirection<ISS>& f) {
  const int size = mesh->getVerticesSize();
  for (int n = 0; n != size; ++n) {
    applyBoundaryCondition(mesh, f, n);
  }
}



template<int N, int M, typename T, typename V, typename IS>
inline
void
removeLowAdjacencies(IndSimpSetIncAdj<N,M,true,T,V,IS>* mesh, 
		     const int minRequiredAdjacencies) {
  // Make sure that the min adjacency requirement is in the right range.
  assert(0 <= minRequiredAdjacencies && minRequiredAdjacencies <= M + 1);

  ads::FixedArray<M+2,int> adjacencyCounts;
  int hi, lo;
  std::vector<int> ss;
  // Loop until the simplices with low adjacencies are gone.
  do {
    //
    // Get rid of the simplices with low adjacency counts.
    //

    // Get the set of simplices with sufficient adjacencies.
    ss.clear();
    determineSimplicesWithRequiredAdjacencies(*mesh, minRequiredAdjacencies, 
					      std::back_inserter(ss));
    // If not all simplices have sufficient adjacencies.
    if (int(ss.size()) != mesh->getSimplicesSize()) {
      // Remove those with low adjacencies.
      IndSimpSetIncAdj<N,M,true,T,V,IS> y;
      buildFromSubsetSimplices(*mesh, ss.begin(), ss.end(), &y);
      y.updateTopology();
      mesh->swap(y);
    }

    //
    // Check the adjacency counts.
    //
    countAdjacencies(*mesh, &adjacencyCounts);
    lo = std::accumulate(adjacencyCounts.begin(), 
			 adjacencyCounts.begin() + minRequiredAdjacencies, 
			 0);
    hi = std::accumulate(adjacencyCounts.begin() + minRequiredAdjacencies, 
			 adjacencyCounts.end(), 
			 0);
  } while (lo != 0 && hi != 0);
}

END_NAMESPACE_GEOM

// End of file.
