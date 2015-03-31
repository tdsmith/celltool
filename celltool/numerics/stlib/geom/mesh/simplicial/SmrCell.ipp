// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_SmrCell_ipp__)
#error This file is an implementation detail of the class SmrCell.
#endif

BEGIN_NAMESPACE_GEOM


// Return true if the cell has a boundary face incident to the edge.
template<class SMR>
inline
bool
doesCellHaveIncidentFaceOnBoundary(const typename SMR::CellConstIterator& c, 
				   const int i, const int j) {
  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBeThree);

  // For each face.
  for (int m = 0; m != SMR::M + 1; ++m) {
    // The i_th and j_th faces are not incident to the edge.
    // Select the incident faces.
    if (m != i && m != j) {
      // If the incident face is on the boundary.
      //if (c->getNeighbor(m) == 0) {
      if (c->isFaceOnBoundary(m)) {
	return true;
      }
    }
  }
  // No incident boundary faces.
  return false;
}


// Return true if the edge is on the boundary.
// \c i and \c j are the indices of the edge in the cell.
// An edge is on the boundary iff an incident face is on the boundary.
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::CellConstIterator& c, 
	     const int i, const int j) {
  typedef typename SMR::Node Node;
  typedef typename SMR::NodeConstIterator NodeIterator;
  typedef typename Node::CellIteratorConstIterator CellIteratorIterator;

  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBeThree);

  const NodeIterator a = c->getNode(i);
  const NodeIterator b = c->getNode(j);

  // For each cell incident to the source node.
  for (CellIteratorIterator c = a->getCellIteratorsBeginning(); 
       c != a->getCellIteratorsEnd(); ++c) {
    // If the cell is incident to the target node as well, it is incident 
    // to the edge.  
    if ((*c)->hasNode(b)) {
      // If there is an incident face on the boundary.
      if (doesCellHaveIncidentFaceOnBoundary<SMR>(*c, (*c)->getIndex(a), 
						  (*c)->getIndex(b))) {
	// The edge is on the boundary as well.
	return true;
      }
    }
  }

  // If there are no incident faces on the boundary, the edge is not on
  // the boundary.
  return false;
}


END_NAMESPACE_GEOM

// End of file.
