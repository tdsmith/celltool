// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_refine_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Perform a refining sweep over the mesh using the maximum edge length 
// function.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD,
	 class MaxEdgeLength>
inline
int
refineSweep(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
	    PointsOnManifold<N,MM,SD,T>* manifold,
	    const MaxEdgeLength& f) {
  // REMOVE
  //std::cerr << "start refineSweep, num cells = " << mesh->computeCellsSize() 
  //<< "\n";

  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  int count = 0;
  Vertex x;
  
  // Loop over the cells.
  for (CellIterator c = mesh->getCellsBeginning(); c != mesh->getCellsEnd();
       ++c) {
    c->getCentroid(&x);
    if (c->computeMaximumEdgeLength() > f(x)) {
      count += splitCell(mesh, manifold, c);
#if 0
      {
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << count << ".vtu";
	std::ofstream file(oss.str().c_str());
	writeVtkXml(file, *mesh);
      }
#endif
    }
  }
  // REMOVE
  //std::cerr << "end refineSweep, count = " << count << "\n";
  return count;
}





// Refine the mesh using the max edge length function.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD,
	 class MaxEdgeLength>
inline
int
refine(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
       PointsOnManifold<N,MM,SD,T>* manifold,
       const MaxEdgeLength& f) {
  int c; 
  int count = 0;
  do {
    c = geom::refineSweep(mesh, manifold, f);
    count += c;
  } while (c != 0);
  return count;
}




//! Refine the mesh by splitting the specified cells.
/*!
  \return the number of edges split.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD>
inline
int
refineCells(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
	    PointsOnManifold<N,MM,SD,T>* manifold,
	    typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
	    CellIteratorSet* cells) {
  typedef typename SimpMeshRed<N,M,T,Node,Cell,Cont>::CellIterator 
    CellIterator;

  int count = 0;

  // This stores the cells that are split during a single recursive cell 
  // splitting operation.
  std::vector<CellIterator> splitCells;
  
  // Iterate until all the specified cells have been split.
  while (! cells->empty()) {
    // Recursively split a cell.
    count += splitCell(mesh, manifold, *cells->begin(), 
		       std::back_inserter(splitCells));
    // Remove the cells that were split from the set.
    for (typename std::vector<CellIterator>::const_iterator i = 
	    splitCells.begin(); i != splitCells.end(); ++i) {
      cells->erase(*i);
    }
    splitCells.clear();
  }

  return count;
}




//! Refine the mesh by splitting the specified cells.
/*!
  \return the number of edges split.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD,
	 typename IntInputIterator>
inline
int
refine(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
       PointsOnManifold<N,MM,SD,T>* manifold,
       IntInputIterator begin, IntInputIterator end) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIteratorSet CellIteratorSet;

  // Make a set of the cell iterators.
  CellIteratorSet cells;
  convertIdentifiersToIterators(*mesh, begin, end, &cells);

  // Refine the specified cells.
  return refineCells(mesh, manifold, &cells);
}



// Refine the specified cells using the maximum edge length function.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD,
	 typename IntInputIterator,
	 class MaxEdgeLength>
inline
int
refine(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
       PointsOnManifold<N,MM,SD,T>* manifold,
       IntInputIterator begin, IntInputIterator end,
       const MaxEdgeLength& f) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::CellIteratorSet CellIteratorSet;

  // Make a set of the cell iterators.
  CellIteratorSet cells;
  convertIdentifiersToIterators(*mesh, begin, end, &cells);

  // Make a set of the cells to refine.
  CellIteratorSet cellsToRefine;
  Vertex centroid;
  CellIterator c;
  // Loop over all of the specified cells.
  for (typename CellIteratorSet::const_iterator i = cells.begin(); 
       i != cells.end(); ++i) {
    c = *i;
    c->getCentroid(&centroid);
    if (c->computeMaximumEdgeLength() > f(centroid)) {
      cellsToRefine.insert(c);
    }
  }

  // Refine the cells whose edges are too long.
  return refineCells(mesh, manifold, &cellsToRefine);
}



END_NAMESPACE_GEOM

// End of file.
