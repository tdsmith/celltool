// -*- C++ -*-

#if !defined(__geom_mesh_iss_contact_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Move the vertices to remove contact.
template<int N, bool A, typename T, typename V, typename IS, 
	 typename VertexForwardIterator>
inline
int
removeContact(const IndSimpSet<N,N-1,A,T,V,IS>& surface, 
	      VertexForwardIterator verticesBeginning,
	      VertexForwardIterator verticesEnd) {
  typedef V Vertex;
  typedef IndSimpSet<N,N-1,A,T,V,IS> Mesh;
  typedef ISS_SignedDistance<Mesh> SignedDistance;

  // Make the signed distance data structure.
  SignedDistance signedDistance(surface);

  int count = 0;
  Vertex closestPoint;
  // For each vertex.
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    // If the vertex is inside the object.
    if (signedDistance(*verticesBeginning, &closestPoint) < 0) {
      ++count;
      // Move the vertex to the closest point.
      *verticesBeginning = closestPoint;
    }
  }
  return count;
}

END_NAMESPACE_GEOM
