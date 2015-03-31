// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_manipulators_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
orientPositive(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> Mesh;
  typedef typename Mesh::Simplex Simplex;
  typedef typename Mesh::CellIterator CellIterator;

  // This only makes sense if the simplex dimension is the same as 
  // the space dimension.
  LOKI_STATIC_CHECK(N == M, TheSpaceDimAndSimplexDimMustBeTheSame);

  Simplex s;
  SimplexJac<N,T> sj;
  // For each cell.
  for (CellIterator i = mesh->getCellsBeginning(); i != mesh->getCellsEnd(); 
       ++i) {
    mesh->getSimplex(i, &s);
    sj.setFunction(s);
    // If the content is negative.
    if (sj.getDeterminant() < 0) {
      // Reverse its orientation.
      i->negate();
    }
  }
}



//! Erase the nodes that do not have an incident cells.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
eraseUnusedNodes(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::NodeIterator NodeIterator;

  // For each node.
  for (NodeIterator i = mesh->getNodesBeginning(); i != mesh->getNodesEnd();) {
    if (i->getCellsSize() == 0) {
      mesh->eraseNode(i++);
    }
    else {
      ++i;
    }
  }
}



//! Remove cells until there are none with minimum adjacencies less than specified.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
eraseCellsWithLowAdjacencies(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh,
			     const int minAdjacencies) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;

  // Make sure that the min adjacency requirement is in the right range.
  assert(0 <= minAdjacencies && minAdjacencies <= M + 1);

  // Loop until the cells with low adjacencies are gone.
  int count;
  do {
    count = 0;
    // Loop over the cells.
    for (CellIterator i = mesh->getCellsBeginning(); 
	 i != mesh->getCellsEnd();) {
      // If this cell has a low number of adjacencies.
      if (i->getNumberOfNeighbors() < minAdjacencies) {
	mesh->eraseCell(i++);
	++count;
      }
      else {
	++i;
      }
    }
  } while (count != 0);
  
  eraseUnusedNodes(mesh);
}



// Re-number the node and cell identifiers so they start at 0 and are 
// contiguous.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
renumberIdentifiers(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> Mesh;
  typedef typename Mesh::NodeIterator NodeIterator;
  typedef typename Mesh::CellIterator CellIterator;

  // Set the node identifiers.
  int index = 0;
  for (NodeIterator i = mesh->getNodesBeginning(); i != mesh->getNodesEnd();
       ++i, ++index) {
    i->setIdentifier(index);
  }

  // Set the cell identifiers.
  index = 0;
  for (CellIterator i = mesh->getCellsBeginning(); i != mesh->getCellsEnd();
       ++i, ++index) {
    i->setIdentifier(index);
  }
}

END_NAMESPACE_GEOM

// End of file.
