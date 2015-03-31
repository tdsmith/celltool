// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_geometry_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<class SMR>
inline
typename SMR::Number
computeIncidentCellsAngle(typename SMR::NodeConstIterator node, 
			  Loki::Int2Type<2> /*space dimension*/,
			  Loki::Int2Type<2> /*simplex dimension*/) {
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    CellIncidentToNodeConstIterator;
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::N == 2, SpaceDimensionMustBe2);
  LOKI_STATIC_CHECK(SMR::M == 2, SimplexDimensionMustBe2);
  const int M = 2;

#ifdef DEBUG_geom
  assert(node->isOnBoundary());
#endif

  //
  // Get the two two tangent vectors to the neighboring faces, a and b.
  //

#ifdef DEBUG_geom
  int aCount = 0, bCount = 0;
#endif
  int i;
  Vertex a(0.0, 0.0), b(0.0, 0.0);
  // For each incident cell.
  for (CellIncidentToNodeConstIterator c = node->getCellsBeginning();
       c != node->getCellsEnd(); ++c) {
    i = c->getIndex(node);

    if (c->isFaceOnBoundary((i+2)%(M+1))) {
      a = c->getNode((i+1)%(M+1))->getVertex();
      a -= node->getVertex();
      normalize(&a);
#ifdef DEBUG_geom
      ++aCount;
#endif
    }

    if (c->isFaceOnBoundary((i+1)%(M+1))) {
      b = c->getNode((i+2)%(M+1))->getVertex();
      b -= node->getVertex();
      normalize(&b);
#ifdef DEBUG_geom
      ++bCount;
#endif
    }
  }

#ifdef DEBUG_geom
  // Make sure we found the two boundary faces.
  assert(aCount == 1 && bCount == 1);
#endif

  //
  // Get the inward normal direction, n.
  //

  Vertex d, n;

  d = a;
  rotatePiOver2(&d);
  n = d;

  d = b;
  rotateMinusPiOver2(&d);
  n += d;

  // No need to normalize n.
  return computeAngle(a, n) + computeAngle(n, b);
}





template<class SMR>
inline
typename SMR::Number
computeIncidentCellsAngle(typename SMR::NodeConstIterator node, 
			  Loki::Int2Type<3> /*space dimension*/,
			  Loki::Int2Type<2> /*simplex dimension*/) {
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    CellIncidentToNodeConstIterator;
  typedef typename SMR::Number Number;
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::N == 3, SpaceDimensionMustBe3);
  LOKI_STATIC_CHECK(SMR::M == 2, SimplexDimensionMustBe2);
  const int M = 2;

  // For each each incident cell.
  int i;
  Vertex a, b;
  Number incidentAngle = 0;
  for (CellIncidentToNodeConstIterator cell = node->getCellsBeginning();
       cell != node->getCellsEnd(); ++cell) {
    i = cell->getIndex(node);
    a = cell->getNode((i+1)%(M+1))->getVertex();
    a -= node->getVertex();
    b = cell->getNode((i+2)%(M+1))->getVertex();
    b -= node->getVertex();
    incidentAngle += computeAngle(a, b);
  }

  return incidentAngle;
}



template<class SMR>
inline
typename SMR::Number
computeIncidentCellsAngle(typename SMR::NodeConstIterator node, 
			  Loki::Int2Type<3> /*space dimension*/,
			  Loki::Int2Type<3> /*simplex dimension*/) {
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    CellIncidentToNodeConstIterator;
  typedef typename SMR::Number Number;
  typedef typename SMR::Vertex Vertex;
  typedef Simplex<3,Vertex,Number> Tetrahedron;

  LOKI_STATIC_CHECK(SMR::N == 3, SpaceDimensionMustBe3);
  LOKI_STATIC_CHECK(SMR::M == 3, SimplexDimensionMustBe3);
  const int M = 3;

  // For each each incident cell.
  Tetrahedron t;
  int i, m;
  Vertex a, b;
  Number incidentAngle = 0;
  for (CellIncidentToNodeConstIterator cell = node->getCellsBeginning();
       cell != node->getCellsEnd(); ++cell) {
    // The local index of the node.
    i = cell->getIndex(node);
    // Copy the cell into a simplex.
    for (m = 0; m != (M+1); ++m) {
      t[m] = cell->getNode(m)->getVertex();
    }
    // Add the solid angle at the i_th vertex of the simplex.
    incidentAngle += computeAngle(t, i);
  }

  return incidentAngle;
}



template<class SMR>
inline
typename SMR::Number
computeIncidentCellsAngle(typename SMR::NodeConstIterator node) { 
  return computeIncidentCellsAngle<SMR>(node, Loki::Int2Type<SMR::N>(), 
					Loki::Int2Type<SMR::M>());
}




// Compute the dihedral angle at the specified edge.  
// The dihedral angle is accumulated from the incident cells.
template<class SMR>
inline
typename SMR::Number
computeDihedralAngle(typename SMR::ConstEdge edge) {
  typedef typename SMR::Number Number;
  typedef typename SMR::NodeConstIterator NodeIterator;
  typedef typename SMR::Node Node;
  typedef typename Node::CellIteratorConstIterator CellIteratorConstIterator;
  typedef typename SMR::Simplex Simplex;

  // The simplex dimension must be 3.
  LOKI_STATIC_CHECK(SMR::M == 3, SimplexDimensionMustBe3);

  // The source node of the edge.
  const NodeIterator a = edge.first->getNode(edge.second);
  // The target node of the edge.
  const NodeIterator b = edge.first->getNode(edge.third);
  
  Number dihedralAngle = 0;
  Simplex simplex;
  int faceIndex1, faceIndex2;
  // For each cell incident to the source node.
  for (CellIteratorConstIterator c = a->getCellIteratorsBeginning(); 
       c != a->getCellIteratorsEnd(); ++c) {
    // If the cell is incident to the target node as well, it is incident 
    // to the edge.  
    if ((*c)->hasNode(b)) {
      // Make a simplex from the cell.
      (*c)->getSimplex(&simplex);
      // Get the indices of the two faces that are incident to the edge.
      computeOtherIndices((*c)->getIndex(a), (*c)->getIndex(b),
			  &faceIndex1, &faceIndex2);
      // The the contribution from this cell.
      dihedralAngle += computeAngle(simplex, faceIndex1, faceIndex2);
    }
  }
  return dihedralAngle;
}



// Return the cosine of the interior angle at the specified 1-face.
// The 1-face must have two incident simplices.
template<class SMR>
inline
typename SMR::Number
computeCosineAngle(typename SMR::FaceConstIterator face) {
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::N == 3 && SMR::M == 2, MustBeA32Mesh);

  // Check that the face is valid.
  assert(face->first != typename SMR::CellConstIterator(0));
  assert(0 <= face->second && face->second < SMR::M + 1);
  // It must be an internal face.
  assert(! isOnBoundary<SMR>(face));

  // The cosine of the angle is the negative of the dot product of the 
  // incident simplex normals.
  // n0 . n1 == cos(pi - a) == - cos(a)
  const Vertex n0 = computeCellNormal<SMR>(face->first);
  const Vertex n1 = computeCellNormal<SMR>
    (face->first->getNeighbor(face->second));
  return - computeDotProduct(n0, n1);
}




// Compute the normal to the surface at the node.
template<class SMR>
inline
void
computeNodeNormal(typename SMR::NodeConstIterator node, 
		  typename SMR::Vertex* normal,
		  Loki::Int2Type<2> /*space_dimension*/,
		  Loki::Int2Type<2> /*simplex_dimension*/) {
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    CellIncidentToNodeConstIterator;
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::N == 2, TheSpaceDimensionMustBe2);
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  const int M = 2;

  int i;
  Vertex v;

  *normal = 0;
  // For each incident cell.
  for (CellIncidentToNodeConstIterator c = node->getCellsBeginning();
       c != node->getCellsEnd(); ++c) {
    i = c->getIndex(node);

    if (c->isFaceOnBoundary((i+2)%(M+1))) {
      v = c->getNode((i+1)%(M+1))->getVertex();
      v -= node->getVertex();
      rotateMinusPiOver2(&v);
      normalize(&v);
      *normal += v;
    }

    if (c->isFaceOnBoundary((i+1)%(M+1))) {
      v = c->getNode((i+2)%(M+1))->getVertex();
      v -= node->getVertex();
      rotatePiOver2(&v);
      normalize(&v);
      *normal += v;
    }
  }
  normalize(normal);
}



// Compute the normal to the surface at the node.
template<class SMR>
inline
void
computeNodeNormal(typename SMR::NodeConstIterator node,	
		  typename SMR::Vertex* vertexNormal, 
		  Loki::Int2Type<3> /*space_dimension*/,
		  Loki::Int2Type<2> /*simplex_dimension*/) {
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    CellIncidentToNodeConstIterator;
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::N == 3, TheSpaceDimensionMustBe3);
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  const int M = 2;

  // The vertex should have at least 3 incident faces.
  assert(node->getCellsSize() >= 3);

  *vertexNormal = 0.0;
  int i;
  Vertex x, y, faceNormal;

  // For each incident face.
  for (CellIncidentToNodeConstIterator c = node->getCellsBeginning();
       c != node->getCellsEnd(); ++c) {
    // The local index of the node in the face.
    i = c->getIndex(node);

    // Compute the face normal.
    x = c->getNode((i+1)%(M+1))->getVertex();
    x -= node->getVertex();
    normalize(&x);
    y = c->getNode((i+2)%(M+1))->getVertex();
    y -= node->getVertex();
    normalize(&y);
    computeCrossProduct(x, y, &faceNormal);
    normalize(&faceNormal);

    // Contribute to the vertex normal.
    // Multiply by the angle between the edges.
    faceNormal *= std::acos(computeDotProduct(x, y));
    *vertexNormal += faceNormal;
  }
  normalize(vertexNormal);
}



template<class SMR>
inline
void
computeNodeNormal(typename SMR::NodeConstIterator node,
		  typename SMR::Vertex* normal) {
  computeNodeNormal<SMR>(node, normal, Loki::Int2Type<SMR::N>(), 
			 Loki::Int2Type<SMR::M>());
}


// Implementation for 2-1 meshes.
template<class SMR>
inline
void
computeCellNormal(typename SMR::CellConstIterator cell, 
		  typename SMR::Vertex* normal,
		  Loki::Int2Type<1> /*dummy*/) {
  LOKI_STATIC_CHECK(SMR::M == 1, Incorrectly_called);
  LOKI_STATIC_CHECK(SMR::M + 1 == SMR::N, Bad_dimensions_for_a_normal);

  *normal = cell->getNode(1)->getVertex();
  *normal -= cell->getNode(0)->getVertex();
  rotateMinusPiOver2(normal);
  normalize(normal);
}



// Implementation for 3-2 meshes.
template<class SMR>
inline
void
computeCellNormal(typename SMR::CellConstIterator cell, 
		  typename SMR::Vertex* normal,
		  Loki::Int2Type<2> /*dummy*/) {
  LOKI_STATIC_CHECK(SMR::M == 2, Incorrectly_called);
  LOKI_STATIC_CHECK(SMR::M + 1 == SMR::N, Bad_dimensions_for_a_normal);

  typedef typename SMR::Vertex Vertex;

  Vertex a = cell->getNode(1)->getVertex();
  a -= cell->getNode(0)->getVertex();
  Vertex b = cell->getNode(2)->getVertex();
  b -= cell->getNode(0)->getVertex();
  computeCrossProduct(a, b, normal);
  normalize(normal);
}


// Compute the cell normal.
template<class SMR>
void
computeCellNormal(typename SMR::CellConstIterator cell, 
		  typename SMR::Vertex* normal) {
  computeCellNormal<SMR>(cell, normal, Loki::Int2Type<SMR::M>());
}







// Implementation for 2-2 meshes.
template<class SMR>
inline
void
computeFaceNormal(typename SMR::CellConstIterator cell, const int i,
		  typename SMR::Vertex* normal,
		  Loki::Int2Type<2> /*The space and simplex dimension*/) {
  LOKI_STATIC_CHECK(SMR::N == 2 && SMR::M == 2, Incorrectly_called);

  const int M = 2;

  *normal = cell->getNode((i+2)%(M+1))->getVertex();
  *normal -= cell->getNode((i+1)%(M+1))->getVertex();
  rotateMinusPiOver2(normal);
  normalize(normal);

  // For the simplex (v[0], ... v[N]) the face is 
  // (-1)^n (v[0], ..., v[n-1], v[n+1], ..., v[N]).
  if (i % 2 == 1) {
    normal->negate();
  }
}


// Implementation for 3-3 meshes.
template<class SMR>
inline
void
computeFaceNormal(typename SMR::CellConstIterator cell, const int i,
		  typename SMR::Vertex* normal,
		  Loki::Int2Type<3> /*The space and simplex dimension*/) {
  LOKI_STATIC_CHECK(SMR::N == 3 && SMR::M == 3, Incorrectly_called);

  typedef typename SMR::Vertex Vertex;

  const int M = 3;

  Vertex a = cell->getNode((i+2)%(M+1))->getVertex();
  a -= cell->getNode((i+1)%(M+1))->getVertex();
  Vertex b = cell->getNode((i+3)%(M+1))->getVertex();
  b -= cell->getNode((i+1)%(M+1))->getVertex();
  computeCrossProduct(a, b, normal);
  normalize(normal);

  // For the simplex (v[0], ... v[N]) the face is 
  // (-1)^n (v[0], ..., v[n-1], v[n+1], ..., v[N]).
  if (i % 2 == 1) {
    normal->negate();
  }
}


// Compute the face normal.
template<class SMR>
void
computeFaceNormal(const typename SMR::CellConstIterator cell, const int i, 
		  typename SMR::Vertex* normal) {
  LOKI_STATIC_CHECK(SMR::N == SMR::M, 
		TheSpaceAndSimplexDimensionMustBeTheSame);
  computeFaceNormal<SMR>(cell, i, normal, Loki::Int2Type<SMR::N>());
}





// Project the line segments to 1-D and collect them.
template<typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutputIterator>
inline
void
projectAndGetSimplices(const SimpMeshRed<2,1,T,Node,Cell,Cont>& mesh,
			  OutputIterator simplices) {
  typedef SimpMeshRed<2,1,T,Node,Cell,Cont> SMR;
  typedef typename SMR::SimplexIterator SimplexIterator;
  typedef Simplex<1,ads::FixedArray<1,T>,T> Segment;

  Segment t;

  // For each simplex.
  for (SimplexIterator s = mesh.getSimplicesBeginning(); 
       s != mesh.getSimplicesEnd(); ++s) {
    // Project the line segment in 2-D to a line segment in 1-D.
    projectToLowerDimension(*s, &t);
    // Add the line segment to the sequence of simplices.
    *simplices++ = t;
  }
}


// Project the triangle simplices to 2-D and collect them.
template<typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutputIterator>
inline
void
projectAndGetSimplices(const SimpMeshRed<3,2,T,Node,Cell,Cont>& mesh,
		       OutputIterator simplices) {
  typedef SimpMeshRed<3,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::SimplexIterator SimplexIterator;
  typedef Simplex<2,ads::FixedArray<2,T>,T> Triangle;

  Triangle t;

  // For each simplex.
  for (SimplexIterator s = mesh.getSimplicesBeginning(); 
       s != mesh.getSimplicesEnd(); ++s) {
    // Project the triangle in 3-D to a triangle in 2-D.
    projectToLowerDimension(*s, &t);
    // Add the triangle to the sequence of simplices.
    *simplices++ = t;
  }
}

END_NAMESPACE_GEOM

// End of file.
