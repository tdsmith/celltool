// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_set_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

//! Get the nodes that are outside the object.
template<typename NodeInIter, class LSF, typename OutIter>
inline
void
determineNodesOutside(NodeInIter begin, NodeInIter end,
		      const LSF& f, OutIter iter) {
  // Loop over the nodes.
  for (; begin != end; ++begin) {
    // If the vertex is outside the object.
    if (f(begin->getVertex()) > 0) {
      // Insert it into the container.
      *iter++ = begin;
    }
  }
}




//! Get the cells whose centroids are outside the object.
template<typename CellInIter, class LSF, typename OutIter>
inline
void
determineCellsOutside(CellInIter begin, CellInIter end,
		      const LSF& f, OutIter iter) {
  typedef typename std::iterator_traits<CellInIter>::value_type Cell;
  typedef typename Cell::Vertex Vertex;

  Vertex x;
  // Loop over the cells.
  for (; begin != end; ++begin) {
    begin->getCentroid(&x);
    // If the centroid is outside the object.
    if (f(x) > 0) {
      // Append the cell iterator into the container.
      *iter++ = begin;
    }
  }
}




//! Get the node iterators for the mesh.
template<typename NodeInIter, typename OutIter>
inline
void
getNodes(NodeInIter begin, NodeInIter end, OutIter iter) {
  // For each node.
  for (; begin != end; ++begin) {
    // Append the node.
    *iter++ = begin;
  }
}




//! Get the node iterators for the interior nodes.
template<typename NodeInIter, typename OutIter>
inline
void
determineInteriorNodes(NodeInIter begin, NodeInIter end, OutIter iter) {
  // For each node.
  for (; begin != end; ++begin) {
    // If this is an interior node.
    if (! begin->isOnBoundary()) {
      // Append the node.
      *iter++ = begin;
    }
  }
}




//! Get the node iterators for the boundary nodes.
template<typename NodeInIter, typename OutIter>
inline
void
determineBoundaryNodes(NodeInIter begin, NodeInIter end, OutIter iter) {
  // For each node.
  for (; begin != end; ++begin) {
    // If this is an boundary node.
    if (begin->isOnBoundary()) {
      // Append the node.
      *iter++ = begin;
    }
  }
}




//! Get the cell iterators with at least the specified number of adjacencies.
template<typename CellInIter, typename OutIter>
inline
void
determineCellsWithRequiredAdjacencies(CellInIter begin, CellInIter end, 
				      const int minimumRequiredAdjacencies, 
				      OutIter iter) {
  // For each cell.
  for (; begin != end; ++begin) {
    // If this cell has the minimum required number of adjacencies.
    if (begin->getNumberOfNeighbors() >= minimumRequiredAdjacencies) {
      // Append the cell.
      *iter++ = begin;
    }
  }
}




//! Get the cell iterators with adjacencies less than specified.
template<typename CellInIter, typename OutIter>
inline
void
determineCellsWithLowAdjacencies(CellInIter begin, CellInIter end, 
				 const int minimumRequiredAdjacencies, 
				 OutIter iter) {
  // For each cell.
  for (; begin != end; ++begin) {
    // If this cell has a low number of adjacencies.
    if (begin->getNumberOfNeighbors() < minimumRequiredAdjacencies) {
      // Append the cell.
      *iter++ = begin;
    }
  }
}



// Get the neighboring nodes of a node.
/*
  The set of nodes (not including the specified node) that share a cell with
  the specified node.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
determineNeighbors(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		   NodeIterator node,
		   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		   NodeIteratorSet* neighbors) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  neighbors->clear();

  CellIncidentToNodeIterator c;
  NodeIterator n;
  // For each incident cell.
  for (c = node->getCellsBeginning(); c != node->getCellsEnd(); ++c) {
    // For each node of the cell.
    for (int m = 0; m != M + 1; ++m) {
      // The node.
      n = c->getNode(m);
      if (n != node) {
	// Add it to the set.
	neighbors->insert(n);
      }
    }
  }
}



// Get the neighboring boundary nodes of a node.
/*
  The set of boundary nodes (not including the specified node) that share 
  a boundary face with the specified node.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
determineBoundaryNeighbors(SimpMeshRed<N,2,T,Node,Cell,Cont>& mesh,
			   typename SimpMeshRed<N,2,T,Node,Cell,Cont>::
			   NodeIterator node,
			   typename SimpMeshRed<N,2,T,Node,Cell,Cont>::
			   NodeIteratorSet* neighbors) {
  typedef SimpMeshRed<N,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  const int M = 2;

  neighbors->clear();

  CellIncidentToNodeIterator c;
  NodeIterator n;
  // For each incident cell.
  for (c = node->getCellsBeginning(); c != node->getCellsEnd(); ++c) {
    // For each node of the cell.
    for (int m = 0; m != M + 1; ++m) {
      // The node.
      n = c->getNode(m);
      // The three tests are arranged from least to most expensive.
      if (n != node && neighbors->count(n) == 0 && 
	  c->isFaceOnBoundary(getFaceIndex(c, node, n))) {
	// Add it to the set.
	neighbors->insert(n);
      }
    }
  }
}



//! Get all the nodes within the specified radius of the specified node.
/*!
  The set includes the specified node.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
determineNeighbors(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		   NodeIterator node,
		   int radius,
		   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		   NodeIteratorSet* neighbors) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::NodeIteratorSet NodeIteratorSet;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  assert(radius >= 0);

  neighbors->clear();
  // The zero radius neighbors is the node itself.
  neighbors->insert(node);

  NodeIteratorSet front;
  CellIncidentToNodeIterator c;
  NodeIterator n;
  while (radius--) {
    // For each node in the set.
    for (typename NodeIteratorSet::const_iterator i = neighbors->begin(); 
	  i != neighbors->end(); ++i) {
      n = *i;
      // For each incident cell.
      for (c = n->getCellsBeginning(); c != n->getCellsEnd(); ++c) {
	// For each node of the cell.
	for (int m = 0; m != M + 1; ++m) {
	  // Add the node to the front.
	  front.insert(c->getNode(m));
	}
      }
    }
    neighbors->insert(front.begin(), front.end());
    front.clear();
  }
}




//! Get the faces of the incident cells.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
determineFacesOfIncidentCells(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			      typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			      NodeIterator node,
			      typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			      FaceSet* faces) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;
  typedef typename SMR::Face Face;

  Face face;
  int m;

  // Loop over the incident cells.
  for (CellIncidentToNodeIterator i = node->getCellsBeginning();
	i != node->getCellsEnd(); ++i) {
    face.first = *i.base();
    // Loop over faces of the cells.
    for (m = 0; m != M + 1; ++m) {
      face.second = m;
      faces->insert(face);
    }
  }
}



// Build a set of cell iterators from a range of cell identifiers.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename IntInIter>
inline
void
convertIdentifiersToIterators(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			      IntInIter begin, IntInIter end,
			      typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			      CellIteratorSet* cells) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::CellIteratorSet CellIteratorSet;

  // Make a vector of the cell identifiers.
  std::vector<int> identifiers;
  for (; begin != end; ++begin) {
    identifiers.push_back(*begin);
  }
  
  // The identifiers must be in sorted order.
  if (! ads::is_sorted(identifiers.begin(), identifiers.end())) {
    std::sort(identifiers.begin(), identifiers.end());
  }

#ifdef DEBUG_geom
  //
  // Check the identifier values.
  //

  // Determine the maximum identifier.
  int maximumIdentifier = -1;
  {
    CellIterator c = mesh.getCellsEnd();
    --c;
    if (c != mesh.getCellsEnd()) {
      maximumIdentifier = c->getIdentifier();
    }
  }
  if (identifiers.size() != 0) {
    assert(identifiers[0] >= 0 && 
	   identifiers[identifiers.size() - 1] <= maximumIdentifier);
  }
#endif

  // Make a set of the cell identifiers.
  cells->clear();
  CellIterator c = mesh.getCellsBeginning();
  std::vector<int>::const_iterator i = identifiers.begin();
  for (; c != mesh.getCellsEnd() && i != identifiers.end(); ++c) {
    if (c->getIdentifier() == *i) {
      // Since the cell identifiers are in sorted order, give it the hint
      // to insert it at the end.
      cells->insert(cells->end(), c);
      ++i;
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
