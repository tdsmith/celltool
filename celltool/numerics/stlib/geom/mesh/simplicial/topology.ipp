// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_topology_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Return true if they are mirror nodes.
template<class SMR>
inline
bool
areMirrorNodes(typename SMR::NodeConstIterator x, 
	       typename SMR::NodeConstIterator y) {
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    CellIncidentToNodeConstIterator;
  
  CellConstIterator a, b;
  // For each cell incident to x.
  for (CellIncidentToNodeConstIterator ci = x->getCellsBeginning(); 
       ci != x->getCellsEnd(); ++ci) {
    // The incident cell.
    a = *ci.base();
    // The neighbor opposite x.
    b = a->getNeighbor(a->getIndex(x));
    // If the face opposite x is not a boundary face and the neighbor 
    // has the node y.
    if (b != CellConstIterator(0) && b->hasNode(y)) {
      // If they are mirror nodes.
      if (b->getNeighbor(b->getIndex(y)) == a) {
	return true;
      }
    }
  }

  // CONTINUE: Now that I check the quality, I think I want to allow collapsing
  // for the following cases.
#if 0
  const bool xIsOnBoundary = x->isOnBoundary();
  const bool yIsOnBoundary = y->isOnBoundary();
  
  if (! xIsOnBoundary && yIsOnBoundary) {
    // For each cell incident to x.
    for (CellIncidentToNodeConstIterator ci = x->getCellsBeginning(); 
	 ci != x->getCellsEnd(); ++ci) {
      // The incident cell.
      a = *ci.base();
      // If the face opposite x is a boundary face and the cell is not 
      // incident to y.
      if (a->getNeighbor(a->getIndex(x)) == 0 && ! a->hasNode(y)) {
	return true;
      }
    }
  }

  if (! yIsOnBoundary && xIsOnBoundary) {
    // For each cell incident to y.
    for (CellIncidentToNodeConstIterator ci = y->getCellsBeginning(); 
	 ci != y->getCellsEnd(); ++ci) {
      // The incident cell.
      a = *ci.base();
      // If the face opposite y is a boundary face and the cell is not 
      // incident to x.
      if (a->getNeighbor(a->getIndex(y)) == 0 && ! a->hasNode(x)) {
	return true;
      }
    }
  }
#endif

  return false;
}



// Return true if the nodes are incident to a common cell.
template<class SMR>
inline
bool
doNodesShareACell(typename SMR::NodeConstIterator x, 
		  typename SMR::NodeConstIterator y) {
  typedef typename SMR::Node::CellIncidentToNodeConstIterator CellIterator;

  // For each cell incident to the first node.
  for (CellIterator i = x->getCellsBeginning(); i != x->getCellsEnd(); ++i) {
    // If the cell is incident to the second node.
    if (i->hasNode(y)) {
      // The nodes share a cell.
      return true;
    }
  }
  // If we did not find a common incident cell, return false.
  return false;
}



// Determine the cells incident to the edge.
template<typename SMR, typename CellIteratorOutputIterator>
void
determineCellsIncidentToEdge(typename SMR::CellIterator c, 
			     const int i, const int j,
			     CellIteratorOutputIterator output) {
  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBe3);

  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  // The source node.
  const NodeIterator a = c->getNode(i);
  // The target node.
  const NodeIterator b = c->getNode(j);

  // For each cell incident to a.
  for (CellIncidentToNodeIterator cell = a->getCellsBeginning(); 
       cell != a->getCellsEnd(); ++cell) {
    // If the cell is incident to b as well, then it is incident to the edge.
    if (cell->hasNode(b)) {
      *output++ = *cell.base();
    }
  }
}


// Determine the nodes in the link of the specified node.
template<typename SMR, typename NodeIteratorInsertIterator>
inline
void
determineNodesInLink(typename SMR::NodeIterator node, 
		     NodeIteratorInsertIterator nodesInLink) {
  typedef typename SMR::Node::CellIteratorIterator CellIteratorIterator;
  typedef typename SMR::NodeIterator NodeIterator;

  NodeIterator n;
  // For each incident cell.
  for (CellIteratorIterator cell = node->getCellIteratorsBeginning();
       cell != node->getCellIteratorsEnd(); ++cell) {
    // For each node in the incident cell.
    for (int m = 0; m != SMR::M + 1; ++m) {
      n = (*cell)->getNode(m);
      // The center node is not part of the link.
      if (n == node) {
	continue;
      }
      // Insert the cell's node into the link.
      *nodesInLink = n;
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
