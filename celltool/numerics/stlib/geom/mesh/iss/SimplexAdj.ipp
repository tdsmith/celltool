// -*- C++ -*-

#if !defined(__geom_SimplexAdj_ipp__)
#error This file is an implementation detail of the class SimplexAdj.
#endif

BEGIN_NAMESPACE_GEOM

//
// Manipulators
//


template<int M>
template<typename IS, bool A>
inline
void
SimplexAdj<M>::
build(const ads::Array<1,IS,A>& simplices,
      const VertexSimplexInc<M>& vertexSimplexInc) {
  // Allocate memory for the adjacencies.
  _adj.resize(simplices.size());


  // Initialize the adjacent indices to a null value.
  _adj = IndexContainer(-1);

  int m, j, vertexIndex, simplexIndex, numIncident;
  const int sz = simplices.size();
  typename IS::Face face;
  // For each simplex.
  for (int i = 0; i != sz; ++i) {
    // For each vertex of the simplex
    for (m = 0; m != M+1; ++m) {
      // Get the face opposite the vertex.
      simplices[i].getFace(m, &face);
      // A vertex on the face.
      vertexIndex = face[0];
      // For each simplex that is incident to the vertex.
      numIncident = vertexSimplexInc.getSize(vertexIndex);
      for (j = 0; j != numIncident; ++j) {
	simplexIndex = vertexSimplexInc[vertexIndex][j];
	if (i != simplexIndex && simplices[simplexIndex].hasFace(face)) {
	  _adj[i][m] = simplexIndex;
	  break;
	}
      }
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
