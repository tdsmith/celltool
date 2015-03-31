// -*- C++ -*-

#if !defined(__geom_mesh_quadrilateral_QuadMesh_ipp__)
#error This file is an implementation detail of the class QuadMesh.
#endif

BEGIN_NAMESPACE_GEOM


// Build from pointers to the vertices and indexed faces.
template<int N, bool A, typename T, typename V, typename IF>
inline
void
QuadMesh<N,A,T,V,IF>::
build(const SizeType numVertices, void* vertices,
      const SizeType numFaces, void* indexedFaces) {
  _vertices.resize(numVertices);
  const Vertex* v = reinterpret_cast<const Vertex*>(vertices);
  std::copy(v, v + numVertices, _vertices.begin());

  _indexedFaces.resize(numFaces);
  const IndexedFace* s = 
    reinterpret_cast<const IndexedFace*>(indexedFaces);
  std::copy(s, s + numFaces, _indexedFaces.begin());
  updateTopology();
}


// Build from pointers to the vertices and indexed faces.
template<int N, bool A, typename T, typename V, typename IF>
inline
void
QuadMesh<N,A,T,V,IF>::
build(const SizeType numVertices, const void* vertices,
      const SizeType numFaces, const void* indexedFaces) {
  LOKI_STATIC_CHECK(A == true, MustAllocateOwnMemory);

  const Vertex* v = reinterpret_cast<const Vertex*>(vertices);
  _vertices.rebuild(v, v + numVertices);

  const IndexedFace* s = 
    reinterpret_cast<const IndexedFace*>(indexedFaces);
  _indexedFaces.rebuild(s, s + numFaces);
  updateTopology();
}


// Assignment operator.
template<int N, bool A, typename T, typename V, typename IF>
inline
QuadMesh<N,A,T,V,IF>&
QuadMesh<N,A,T,V,IF>::
operator=(const QuadMesh& other) {
  if (this != &other) {
    _vertices = other._vertices;
    _indexedFaces = other._indexedFaces;
  }
  return *this;
}

END_NAMESPACE_GEOM

// End of file.
