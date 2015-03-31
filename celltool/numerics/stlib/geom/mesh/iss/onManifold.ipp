// -*- C++ -*-

#if !defined(__geom_mesh_iss_onManifold_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Get the vertices on the manifold.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 int MM, bool MA, typename MT, typename MV, typename MIS,
	 typename IntOutIter>
inline
void
determineVerticesOnManifold(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			    const IndSimpSet<N,MM,MA,MT,MV,MIS>& manifold, 
			    IntOutIter indexIterator,
			    const T epsilon) {
  // Call the function below with the full range of vertex indices.
  determineVerticesOnManifold
    (mesh, ads::constructIntIterator(0),
     ads::constructIntIterator(mesh.getVerticesSize()),
     manifold, indexIterator, epsilon);
}



// Get the vertices (from the set of vertices) on the manifold.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntInIter,
	 int MM, bool MA, typename MT, typename MV, typename MIS,
	 typename IntOutIter>
inline
void
determineVerticesOnManifold(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			    IntInIter indicesBeginning, IntInIter indicesEnd,
			    const IndSimpSet<N,MM,MA,MT,MV,MIS>& manifold, 
			    IntOutIter indexIterator,
			    const T epsilon) {
  typedef IndSimpSet<N,MM,MA,MT,MV,MIS> Manifold;

  LOKI_STATIC_CHECK(MM < M, TheManifoldDimensionMustBeLessThanM);

  // The data structure for computing unsigned distance.
  ISS_SimplexQuery<Manifold> distance(manifold);

  int i;
  for (; indicesBeginning != indicesEnd; ++indicesBeginning) {
    i = *indicesBeginning;
    if (distance.computeMinimumDistance(mesh.getVertex(i)) <= epsilon) {
      *indexIterator++ = i;
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
