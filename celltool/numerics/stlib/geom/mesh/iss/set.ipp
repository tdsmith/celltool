// -*- C++ -*-

#if !defined(__geom_mesh_iss_set_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<int N, int M, bool A, typename T, typename V, typename IS,
	 class LSF, typename IntOutIter>
inline
void
determineVerticesInside(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			const LSF& f, IntOutIter indexIterator) {
  // Loop over the vertices.
  for (int n = 0; n != mesh.getVerticesSize(); ++n) {
    // If the vertex is inside the object.
    if (f(mesh.getVertex(n)) <= 0) {
      // Insert it into the container.
      *indexIterator++ = n;
    }
  }
}



template<int N, int M, bool A, typename T, typename V, typename IS,
	 class LSF, typename IntOutIter>
inline
void
determineSimplicesInside(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			 const LSF& f, IntOutIter indexIterator) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::SimplexConstIterator SimplexConstIterator;

  Vertex x;
  SimplexConstIterator s = mesh.getSimplicesBeginning();
  // Loop over the simplices.
  for (int n = 0; n != mesh.getSimplicesSize(); ++n, ++s) {
    s->computeCentroid(&x);
    // If the centroid is inside the object.
    if (f(x) <= 0) {
      // Insert the simplex index into the container.
      *indexIterator++ = n;
    }
  }
}



// Determine the simplices which satisfy the specified condition.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 class UnaryFunction, typename IntOutIter>
inline
void
determineSimplicesThatSatisfyCondition(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
				       const UnaryFunction& f, 
				       IntOutIter indexIterator) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;
  typedef typename ISS::SimplexConstIterator SimplexConstIterator;

  SimplexConstIterator s = mesh.getSimplicesBeginning();
  // Loop over the simplices.
  const int size = mesh.getSimplicesSize();
  for (int n = 0; n != size; ++n, ++s) {
    // If the condition is satisfied.
    if (f(*s)) {
      // Insert the simplex index into the container.
      *indexIterator++ = n;
    }
  }
}



// Determine the simplices whose bounding boxes overlap the domain.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
void
determineOverlappingSimplices(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			      const BBox<N,T>& domain, 
			      IntOutIter indexIterator) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;
  typedef typename ISS::Simplex Simplex;

  Simplex s;
  BBox<N,T> box;
  // Loop over the simplices.
  for (int n = 0; n != mesh.getSimplicesSize(); ++n) {
    mesh.getSimplex(n, &s);
    // Make a bounding box around the simplex.
    box.bound(s.getBeginning(), s.getEnd());
    if (doOverlap(domain, box)) {
      // Insert the simplex index into the container.
      *indexIterator++ = n;
    }
  }
}



template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
inline
void
determineSimplicesWithRequiredAdjacencies
(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
 const int minRequiredAdjacencies, IntOutIter indexIterator) {
  // For each simplex.
  for (int n = 0; n != mesh.getSimplicesSize(); ++n) {
    // If this simplex has the minimum required number of adjacencies.
    if (mesh.getAdjacentSize(n) >= minRequiredAdjacencies) {
      // Add the simplex to the set.
      *indexIterator++ = n;
    }
  }
}



// Add the boundary vertex indices to the set.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
inline
void
determineInteriorVertices(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
			  IntOutIter indexIterator) {
  // For each vertex.
  for (int n = 0; n != mesh.getVerticesSize(); ++n) {
    // If this is a boundary vertex.
    if (! mesh.isVertexOnBoundary(n)) {
      // Add the vertex to the set.
      *indexIterator++ = n;
    }
  }
}



// Add the boundary vertex indices to the set.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
inline
void
determineBoundaryVertices(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
			  IntOutIter indexIterator) {
  // For each vertex.
  for (int n = 0; n != mesh.getVerticesSize(); ++n) {
    // If this is a boundary vertex.
    if (mesh.isVertexOnBoundary(n)) {
      // Add the vertex to the set.
      *indexIterator++ = n;
    }
  }
}



// Add the vertices which are incident to the simplices.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntInIter, typename IntOutIter>
inline
void
determineIncidentVertices(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			  IntInIter simplexIndicesBeginning, 
			  IntInIter simplexIndicesEnd,
			  IntOutIter vertexIndicesIterator) {
  std::set<int> vertexIndices;

  int m, s;
  // For each indexed simplex in the range.
  for (; simplexIndicesBeginning != simplexIndicesEnd; 
       ++simplexIndicesBeginning) {
    // The index of a simplex.
    s = *simplexIndicesBeginning;
    // For each vertex index of the indexed simplex.
    for (m = 0; m != M + 1; ++m) {
      // Add the vertex index to the set.
      vertexIndices.insert(mesh.getIndexedSimplex(s)[m]);
    }
  }

  // Add the vertex indices to the output iterator.
  for (std::set<int>::const_iterator i = vertexIndices.begin();
	i != vertexIndices.end(); ++i) {
    *vertexIndicesIterator++ = *i;
  }
}



// Add the simplices (simplex indices) which are incident to the vertices.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntInIter, typename IntOutIter>
inline
void
determineIncidentSimplices(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
			   IntInIter vertexIndicesBeginning, 
			   IntInIter vertexIndicesEnd,
			   IntOutIter simplexIndicesIterator) {
  std::set<int> simplexIndices;

  int m, size, v;
  // For each vertex in the range.
  for (; vertexIndicesBeginning != vertexIndicesEnd; 
       ++vertexIndicesBeginning) {
    // The index of a vertex.
    v = *vertexIndicesBeginning;
    // For each incidend simplex index to the vertex.
    size = mesh.getIncidentSize(v);
    for (m = 0; m != size; ++m) {
      // Add the simplex index to the set.
      simplexIndices.insert(mesh.getIncident(v, m));
    }
  }

  // Add the simplex indices to the output iterator.
  for (std::set<int>::const_iterator i = simplexIndices.begin();
	i != simplexIndices.end(); ++i) {
    *simplexIndicesIterator++ = *i;
  }
}



// Make the complement set of indices.
template<typename IntForIter, typename IntOutIter>
inline
void
determineComplementSetOfIndices(const int upperBound,
				IntForIter beginning, IntForIter end,
				IntOutIter indexIterator) {
  // Loop over all integers in the range.
  for (int n = 0; n != upperBound; ++n) {
    // If the element is in the set.
    if (beginning != end && *beginning == n) {
      // Skip the element.
      ++beginning;
    }
    // If the element is not in the set.
    else {
      // Add it to b.
      *indexIterator++ = n;
    }
  }
  assert(beginning == end);
}



// Add the simplices in the component with the n_th simpex to the set.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
inline
void
determineSimplicesInComponent(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh,
			      const int index, IntOutIter indexIterator) {
  assert(0 <= index && index < mesh.getSimplicesSize());

  // The simplices on the boundary of those that have been identified as 
  // being in the component.
  std::stack<int> boundary;
  // The simplex indices in the component.
  std::set<int> component;
  int i, m, n;

  boundary.push(index);
  while(! boundary.empty()) {
    // Get a simplex from the boundary.
    n = boundary.top();
    // REMOVE
    //std::cerr << "top = " << n << "\n";
    boundary.pop();
    // Add it to the component set.
    component.insert(n);
    // Add the neighbors that aren't already in the component set to 
    // the boundary.
    for (m = 0; m != M + 1; ++m) {
      i = mesh.getAdjacent(n, m);
      // If there is an adjacent simplex and it's not already in 
      // the component set.
      if (i != -1 && component.count(i) == 0) {
	// REMOVE
	//std::cerr << "new adjacent = " << i << "\n";
	boundary.push(i);
      }
    }
  }
  
  // Copy the simplex indices in the component to the output iterator.
  for (std::set<int>::const_iterator iter = component.begin();
	iter != component.end(); ++iter) {
    *indexIterator++ = *iter;
  }
}


// Separate the connected components of the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutputIterator>
inline
void
separateComponents(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
		   IntOutputIterator delimiterIterator) {
  separateComponents(mesh, delimiterIterator, 
		     ads::constructTrivialOutputIterator());
}


// Separate the connected components of the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutputIterator1, typename IntOutputIterator2>
inline
void
separateComponents(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
		   IntOutputIterator1 delimiterIterator,
		   IntOutputIterator2 permutationIterator) {
  typedef IndSimpSetIncAdj<N,M,A,T,V,IS> ISS;

  // Check for the trivial case of an empty mesh.
  if (mesh->getSimplicesSize() == 0) {
    return;
  }

  // Simplex indices, grouped by component.
  std::vector<int> indices;
  indices.reserve(mesh->getSimplicesSize());
  // Which simplices have been identified as belonging to a particular 
  // component.
  std::vector<bool> used(mesh->getSimplicesSize(), false);

  // The beginning of the first component.
  *delimiterIterator++ = 0;

  int i;
  int n = 0;
  int oldSize, newSize;
  do {
    // Get the first unused simplex.
    while (used[n]) {
      ++n;
    }
    oldSize = indices.size();
    // Get the component.
    determineSimplicesInComponent(*mesh, n, std::back_inserter(indices));
    // Record the end delimiter for this component.
    *delimiterIterator++ = indices.size();

    // Record the simplices that are used in this component.
    newSize = indices.size();
    for (i = oldSize; i != newSize; ++i) {
      used[indices[i]] = true;
    }
  } while (int(indices.size()) != mesh->getSimplicesSize());

  // Separate the components.
  typename ISS::IndexedSimplexContainer 
    indexedSimplices(mesh->getIndexedSimplices());
  for (i = 0; i != mesh->getSimplicesSize(); ++i) {
    mesh->getIndexedSimplices()[i] = indexedSimplices[indices[i]];
  }

  // Record the permutation.
  std::copy(indices.begin(), indices.end(), permutationIterator);

  // Rebuild the incidences and adjacencies.
  mesh->updateTopology();
}

END_NAMESPACE_GEOM

// End of file.
