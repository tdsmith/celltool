// -*- C++ -*-

#if !defined(__geom_IndSimpSet_ipp__)
#error This file is an implementation detail of the class IndSimpSet.
#endif

BEGIN_NAMESPACE_GEOM


// Build from pointers to the vertices and indexed simplices.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
IndSimpSet<N,M,A,T,V,IS>::
build(const SizeType numVertices, void* vertices,
      const SizeType numSimplices, void* indexedSimplices) {
  _vertices.resize(numVertices);
  const Vertex* v = reinterpret_cast<const Vertex*>(vertices);
  std::copy(v, v + numVertices, _vertices.begin());

  _indexedSimplices.resize(numSimplices);
  const IndexedSimplex* s = 
    reinterpret_cast<const IndexedSimplex*>(indexedSimplices);
  std::copy(s, s + numSimplices, _indexedSimplices.begin());
  updateTopology();
}


// Build from pointers to the vertices and indexed simplices.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
IndSimpSet<N,M,A,T,V,IS>::
build(const SizeType numVertices, const void* vertices,
      const SizeType numSimplices, const void* indexedSimplices) {
  LOKI_STATIC_CHECK(A == true, MustAllocateOwnMemory);

  const Vertex* v = reinterpret_cast<const Vertex*>(vertices);
  _vertices.rebuild(v, v + numVertices);

  const IndexedSimplex* s = 
    reinterpret_cast<const IndexedSimplex*>(indexedSimplices);
  _indexedSimplices.rebuild(s, s + numSimplices);
  updateTopology();
}


// Assignment operator.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
IndSimpSet<N,M,A,T,V,IS>&
IndSimpSet<N,M,A,T,V,IS>::
operator=(const IndSimpSet& other) {
  if (this != &other) {
    _vertices = other._vertices;
    _indexedSimplices = other._indexedSimplices;
  }
  return *this;
}


END_NAMESPACE_GEOM

// End of file.
