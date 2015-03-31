// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_accessors_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Get the incident cells of the edge.
template<typename SMR, typename CellIteratorOutputIterator>
inline
void
getIncidentCells(const typename SMR::cell_iterator cell, 
		 const int i, const int j,
		 CellIteratorOutputIterator out) {
  typedef typename SMR::Node Node;
  typedef typename Node::CellIteratorIterator CellIteratorIterator;

  // The simplex dimension must be 3.
  LOKI_STATIC_CHECK(SMR::M == 3, SimplexDimensionMustBe3);

  const typename SMR::NodeIterator a = cell->getNode(i);
  const typename SMR::NodeIterator b = cell->getNode(j);

  for (CellIteratorIterator c = a->getCellIteratorsBeginning(); 
       c != a->getCellIteratorsEnd(); ++c) {
    // If the cell is incident to the other node as well, it is incident 
    // to the edge.  
    if ((*c)->hasNode(b)) {
      // Record the cell iterator.
      *out++ = *c;
    }
  }
}



// For a 2-simplex cell, a pair of nodes defines a 1-face.  
// Return the index of this 1-face.
template<class CellIterator, class NodeIterator>
inline
int
getFaceIndex(const CellIterator& cell, 
	     const NodeIterator& a, const NodeIterator& b) {
  // This function may only be used with 2-simplices.
  typedef typename std::iterator_traits<CellIterator>::value_type CellType;
  LOKI_STATIC_CHECK(CellType::M == 2, SimplexDimensionMustBeTwo);

  const int i = cell->getIndex(a);
  const int j = cell->getIndex(b);
  int k = 0;
  if (k == i || k == j) {
    ++k;
  }
  if (k == i || k == j) {
    ++k;
  }
  return k;
}



template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
bool
isOriented(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef Simplex<M,int> IndexedSimplex;
  typedef Simplex<M-1,int> IndexedFace;

  int i, m, mu;
  IndexedSimplex s, t;
  IndexedFace f, g;
  CellConstIterator d;

  // For each cell.
  for (CellConstIterator c = mesh.cells_begin(); c != mesh.cells_end(); ++c) {
    // Get the indexed simplex for c.
    for (i = 0; i != M + 1; ++i) {
      s[i] = c->getNode(i)->getIdentifier();
    }
    // For each adjacent cell.
    for (m = 0; m != M + 1; ++m) {
      // The m_th adjacent cell.
      d = c->getNeighbor(m);
      // If this is not a boundary face.
      if (d != 0) {
	// Get the indexed simplex for d.
	for (i = 0; i != M + 1; ++i) {
	  t[i] = d->getNode(i)->getIdentifier();
	}
	mu = c->getMirrorIndex(m);
	s.getFace(m, f);
	t.getFace(mu, g);
	g.negate();
	if (! haveSameOrientation(f, g)) {
	  /* CONTINUE REMOVE
	  std::cout << s << "    " << t << "\n"
		    << f << "    " << g << "\n";
	  */
	  return false;
	}
      }
    }
  }
  
  return true;
}

END_NAMESPACE_GEOM

// End of file.
