// -*- C++ -*-

#if !defined(__geom_IndSimpSetIncAdj_ipp__)
#error This file is an implementation detail of the class IndSimpSetIncAdj.
#endif

BEGIN_NAMESPACE_GEOM

template <int N, int M, bool A, typename T, typename V, typename IS>
inline
bool
IndSimpSetIncAdj<N,M,A,T,V,IS>::
isVertexOnBoundary(const int index) const {
  int si, m, n;
  // For each simplex incident to this vertex.
  for (IncidenceConstIterator sii = 
	 _vertexSimplexIncidence.getBeginning(index); 
       sii != _vertexSimplexIncidence.getEnd(index); ++sii) {
    // The simplex index.
    si = *sii;
    // The number of the vertex in the simplex.
    n = getIndexedSimplices()[si].getVertexIndex(index);
    // For each face incident to the vertex.
    for (m = 0; m != M+1; ++m) {
      if (m != n) {
	// If the face is on the boundary.
	if (_simplexAdjacencies(si, m) == -1) {
	  return true;
	}
      }
    }
  }
  return false;
}

END_NAMESPACE_GEOM

// End of file.
