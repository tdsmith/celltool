// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_valid_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

//! Return true if the node is valid.
/*!
  The node must have an incident cell.
*/
template<typename Mesh>
inline
bool
isNodeValid(const typename Mesh::NodeConstIterator node) {
  typedef typename Mesh::Node Node;
  typedef typename Node::CellIteratorConstIterator CellIteratorConstIterator;

  // The node must have at least one incident cell.
  if (node->getCell() == typename Mesh::CellConstIterator(0)) {
    return false;
  }
  // For each incident cell.
  for (CellIteratorConstIterator i = node->getCellIteratorsBeginning();
       i != node->getCellIteratorsEnd(); ++i) {
    // Check that the incident cell is incident to this node.
    if (! (*i)->hasNode(node)) {
      return false;
    }
  }

  typename Mesh::Cell::Face face;
  // For each pair of cells.
  for (CellIteratorConstIterator i = node->getCellIteratorsBeginning();
       i != node->getCellIteratorsEnd(); ++i) {
    for (CellIteratorConstIterator j = node->getCellIteratorsBeginning();
	 j != node->getCellIteratorsEnd(); ++j) {
      if (i == j) {
	continue;
      }
      // For each face.
      for (int m = 0; m != Mesh::M + 1; ++m) {
	(*i)->getFace(m, &face);
	// If j has the same face as i, but is not a neighbor.
	if (hasFace<Mesh>(*j, face) && ! (*i)->hasNeighbor(*j)) {
	  // The cells should be adjacent.
	  return false;
	}
      }
    }
  }

  return true;
}




//! Return true if the cell is valid.
template<typename Mesh>
inline
bool
isCellValid(const typename Mesh::CellConstIterator cell) {
  // Check the self iterator.
  if (cell->getSelf() != cell) {
    return false;
  }
  // For each node.
  for (int m = 0; m != Mesh::M + 1; ++m) {
    // The nodes must be distinct.
    for (int n = 0; n != Mesh::M + 1; ++n) {
      if (n == m) {
	continue;
      }
      if (cell->getNode(m) == cell->getNode(n)) {
	return false;
      }
    }
    // The iterator must be valid.
    if (cell->getNode(m) == typename Mesh::NodeConstIterator(0)) {
      return false;
    }
    // The cell must be valid.
    if (! isNodeValid<Mesh>(cell->getNode(m))) {
      return false;
    }
    // The cell must have this cell as an incident cell.
    if (! cell->getNode(m)->hasCell(cell)) {
      return false;
    }
    // Check the neighbor.
    if (! cell->isFaceOnBoundary(m)) {
      if (! cell->getNeighbor(m)->hasNeighbor(cell)) {
	return false;
      }
    }
  }
  return true;
}




//! Return true if the mesh is valid.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
bool
isValid(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef typename SMR::NodeConstIterator NodeConstIterator;

  // Check each cell.
  for (CellConstIterator i = mesh.getCellsBeginning(); 
       i != mesh.getCellsEnd(); ++i) {
    if (! isCellValid<SMR>(i)) {
      return false;
    }
  }

  // Check each node.
  for (NodeConstIterator i = mesh.getNodesBeginning(); 
       i != mesh.getNodesEnd(); ++i) {
    if (! isNodeValid<SMR>(i)) {
      return false;
    }
  }

  // The cell identifiers must be distinct.
  std::vector<int> identifiers;
  std::copy(mesh.getCellIdentifiersBeginning(), mesh.getCellIdentifiersEnd(),
	    std::back_inserter(identifiers));
  if (! ads::areElementsUnique(identifiers.begin(), identifiers.end())) {
    return false;
  }

  // The node identifiers must be distinct.
  identifiers.clear();
  std::copy(mesh.getNodeIdentifiersBeginning(), mesh.getNodeIdentifiersEnd(),
	    std::back_inserter(identifiers));
  if (! ads::areElementsUnique(identifiers.begin(), identifiers.end())) {
    return false;
  }

  return true;
}

END_NAMESPACE_GEOM

// End of file.
