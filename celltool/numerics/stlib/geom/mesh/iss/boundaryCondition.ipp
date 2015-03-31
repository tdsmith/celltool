// -*- C++ -*-

#if !defined(__geom_mesh_iss_boundaryCondition_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Apply the closest point boundary condition at a vertex.
template<bool A, typename T, typename V, typename IS,
	  class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPoint<ISS>& condition,
		       const int n) {
  mesh->getVertices()[n] = condition(mesh->getVertex(n));
}

// Apply the closest point in the normal direction boundary condition 
// at a vertex.
template<bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPointDirection<ISS>& condition,
		       const int n) {
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> Mesh;
  typedef typename Mesh::Vertex Vertex;

  // Get the normal direction.
  Vertex vertexNormal;
  computeVertexNormal(*mesh, n, &vertexNormal);

  // Apply the boundary condition.
  // Closest point in the normal direction.
  mesh->getVertices()[n] = condition(mesh->getVertex(n), vertexNormal);
}

// Apply the closest point boundary condition at a vertex.
template<bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<3,2,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPoint<ISS>& condition,
		       const int n) {
  mesh->getVertices()[n] = condition(mesh->getVertex(n));
}

// Apply the closest point in the normal direction boundary condition 
// at a vertex.
template<bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<3,2,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPointDirection<ISS>& condition,
		       const int n) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> Mesh;
  typedef typename Mesh::Vertex Vertex;

  // Get the normal direction.
  Vertex vertexNormal;
  computeVertexNormal(*mesh, n, &vertexNormal);

  // Apply the boundary condition.
  // Closest point in the normal direction.
  mesh->getVertices()[n] = condition(mesh->getVertex(n), vertexNormal);
}

// Apply the closest point boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS,
	 class UnaryFunction>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const UnaryFunction& condition,
		       const int n) {
  // The vertex may or may not be on the boundary.
  mesh->getVertices()[n] = condition(mesh->getVertex(n));
}

// Apply the closest point boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPoint<ISS>& condition,
		       const int n) {
#ifdef DEBUG_geom
  assert(mesh->isVertexOnBoundary(n));
#endif
  mesh->getVertices()[n] = condition(mesh->getVertex(n));
}

// Apply the closer point boundary condition at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_CloserPoint<ISS>& condition,
		       const int n) {
#ifdef DEBUG_geom
  assert(mesh->isVertexOnBoundary(n));
#endif
  mesh->getVertices()[n] = condition(mesh->getVertex(n));
}

// Apply the closest point in the normal direction boundary condition 
// at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_ClosestPointDirection<ISS>& condition,
		       const int n) {
  typedef IndSimpSetIncAdj<N,N,A,T,V,IS> Mesh;
  typedef typename Mesh::Vertex Vertex;

  // Get the normal direction.
  Vertex vertexNormal;
  computeVertexNormal(*mesh, n, &vertexNormal);

  // Apply the boundary condition.
  // Closest point in the normal direction.
  mesh->getVertices()[n] = condition(mesh->getVertex(n), vertexNormal);
}

// Apply the closer point in the normal direction boundary condition 
// at a vertex.
template<int N, bool A, typename T, typename V, typename IS, class ISS>
inline
void
applyBoundaryCondition(IndSimpSetIncAdj<N,N,A,T,V,IS>* mesh, 
		       const ISS_SD_CloserPointDirection<ISS>& condition,
		       const int n) {
  typedef IndSimpSetIncAdj<N,N,A,T,V,IS> Mesh;
  typedef typename Mesh::Vertex Vertex;

  // Get the normal direction.
  Vertex vertexNormal;
  computeVertexNormal(*mesh, n, &vertexNormal);

  // Apply the boundary condition.
  // Closest point in the normal direction.
  mesh->getVertices()[n] = condition(mesh->getVertex(n), vertexNormal);
}

END_NAMESPACE_GEOM

// End of file.
