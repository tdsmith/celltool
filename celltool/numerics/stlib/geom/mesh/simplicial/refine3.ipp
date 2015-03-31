// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_refine3_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Return the maximum edge length.
/*!
  Set \c i and \c j to the maximum edge indices.
*/
template<class SMR>
inline
typename SMR::Number
computeMaximumEdgeLength(const typename SMR::CellConstIterator c, 
			 int* i, int* j) {
  return c->computeMaximumEdgeLength(i, j);
}


//! Return true if this is a common maximum edge or a boundary edge.
template<class SMR>
inline
bool
isCommonMaximumEdge(const typename SMR::CellIterator c, 
		    const int i, const int j, 
		    typename SMR::CellIterator* cellWithLongerEdge) {
  typedef typename SMR::Number Number;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  NodeIterator a = c->getNode(i);
  NodeIterator b = c->getNode(j);

  // The maximum edge length of c.  We increase the length to avoid errors
  // in comparing floating point numbers.
  const Number maxLength = geom::computeDistance(a->getVertex(), 
						 b->getVertex()) *
    (1.0 + 10.0 * std::numeric_limits<Number>::epsilon());

  int p, q;
  // For each cell incident to a.
  for (CellIncidentToNodeIterator ci = a->getCellsBeginning(); 
       ci != a->getCellsEnd(); ++ci) {
    // If the cell is incident to b as well, then it is incident to the edge.
    // (The first condition prevents us from checking c.)
    if (*ci.base() != c && ci->hasNode(b)) {
      // If another incident cell has a longer edge than the maximum edge of c.
      if (computeMaximumEdgeLength<SMR>(*ci.base(), &p, &q) > maxLength) {
	*cellWithLongerEdge = *ci.base();
	return false;
      }
    }
  }
  return true;
}




template<typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont>
inline
void
splitCell(SimpMeshRed<3,3,T,_Node,_Cell,Cont>* mesh,
	  const typename SimpMeshRed<3,3,T,_Node,_Cell,Cont>::CellIterator c, 
	  const int i, const int j,
	  const typename SimpMeshRed<3,3,T,_Node,_Cell,Cont>::NodeIterator m) {
  typedef SimpMeshRed<3,3,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::Cell Cell;

  //
  // Make a new cell.
  //
  CellIterator cn;
  {
    Cell tmp(*c);
    tmp.setNode(i, m);
    // This inserts the cell and sets the node-cell incidencies.
    cn = mesh->insertCell(tmp);
    if (! cn->isFaceOnBoundary(i)) {
      cn->getNeighbor(i)->setNeighbor(c->getMirrorIndex(i), cn);
    }
  }

  //
  // Shrink the old cell by splitting the edge.
  //
  c->getNode(j)->removeCell(c);
  c->setNode(j, m);
  m->insertCell(c);

  //
  // Fix the cell adjacency information between the two cells.
  // (We fix the other adjacency information later.)
  //
  c->setNeighbor(i, cn);
  cn->setNeighbor(j, c);
}




// CONTINUE: Should I return the mid-node?
//! Split the edge.
template<typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 int SD>
inline
void
splitCell(SimpMeshRed<3,3,T,_Node,_Cell,Cont>* mesh,
	  PointsOnManifold<3,2,SD,T>* boundaryManifold,
	  typename SimpMeshRed<3,3,T,_Node,_Cell,Cont>::CellIterator c, 
	  const int i, const int j) {
  typedef SimpMeshRed<3,3,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  const int M = 3;

  //std::cout << "splitCell() " << a->identifier() << " "
  //<< i << " " << j << "\n";

  //
  // Perform a couple calculations before we muck up the incidence/adjacency 
  // information.
  //

  // The source node.
  const NodeIterator a = c->getNode(i);
  // The target node.
  const NodeIterator b = c->getNode(j);

  // The cells incident to the edge.
  std::vector<CellIterator> inc;
  {
    // For each cell incident to a.
    for (CellIncidentToNodeIterator ci = a->getCellsBeginning(); 
	 ci != a->getCellsEnd(); ++ci) {
      // If the cell is incident to b as well, then it is incident to the edge.
      if (ci->hasNode(b)) {
	inc.push_back(*ci.base());
      }
    }
  }

  // The mid-node.
  NodeIterator midNode = mesh->insertNode();
  {
    // The mid-point of the edge.
    Vertex midPoint = a->getVertex();
    midPoint += b->getVertex();
    midPoint *= 0.5;
    midNode->setVertex(midPoint);
  }

  // If a boundary manifold was specified.
  if (boundaryManifold != 0) {
    // If the edge is on the boundary.
    if (isOnBoundary<SMR>(c, i, j)) {
      Vertex newLocation;
      // If the edge being split is an edge feature.
      if (boundaryManifold->hasEdge(a->getIdentifier(), b->getIdentifier())) {
	// Register the midpoint on an edge in the boundary manifold.
	newLocation = 
	  boundaryManifold->insertOnAnEdge(midNode->getIdentifier(), 
					   midNode->getVertex());
	// Split the edge feature in the boundary manifold.
	boundaryManifold->splitEdge(a->getIdentifier(), b->getIdentifier(), 
				    midNode->getIdentifier());
      }
      else {
	// Register the midpoint on a surface in the boundary manifold.
	newLocation = 
	  boundaryManifold->insertOnASurface(midNode->getIdentifier(), 
					     midNode->getVertex());
      }
      // Update the location.
      midNode->setVertex(newLocation);
    }
  }
  
  //
  // Let the mucking commence.
  //

  // Split each cell.
  for (typename std::vector<CellIterator>::iterator cii = inc.begin();
       cii != inc.end(); ++cii) {
    splitCell(mesh, *cii, (*cii)->getIndex(a), (*cii)->getIndex(b), midNode);
  }

  // Fix the cell adjacencies.
  CellIterator n0, n2;
  for (typename std::vector<CellIterator>::iterator cii = inc.begin();
       cii != inc.end(); ++cii) {
    int i0 = (*cii)->getIndex(a);
    int i1 = (*cii)->getIndex(midNode);
    for (int i2 = 0; i2 != M + 1; ++i2) {
      if (i2 != i0 && i2 != i1) {
	n0 = (*cii)->getNeighbor(i0);
	n2 = (*cii)->getNeighbor(i2);
	// CONTINUE REMOVE
	assert(n0->getIndex(midNode) == i0);
	assert(n0->getIndex(b) == i1);
	assert((*cii)->getNode(i2) == n0->getNode(i2));
	if (n2 != CellIterator(0)) {
	  // CONTINUE REMOVE
	  if (! n2->hasNode(a)) {
	    print(std::cerr, *mesh);
	    std::cerr << "cell id = " << (*cii)->getIdentifier() << "\n"
		      << i0 << " " << i1 << " " << i2 << "\n";
	    std::cerr << n2->getNode(0)->getIdentifier() << " "
		      << n2->getNode(1)->getIdentifier() << " "
		      << n2->getNode(2)->getIdentifier() << " "
		      << n2->getNode(3)->getIdentifier() << "\n"
		      << a->getIdentifier() << "\n";
	  }
	  assert(n2->hasNode(a));
	  n0->setNeighbor(i2, n2->getNeighbor(n2->getIndex(a)));
	}
	else {
	  n0->setNeighbor(i2, CellIterator(0));
	}
      }
    }
  }

#if 0
  // CONTINUE: REMOVE
  if (! isValid(*mesh)) {
    print(std::cerr, *mesh);
  }
  assert(isValid(*mesh));
#endif
}




//! Split the cell.
/*!
  Split other cells as necessary to ensure that only shared longest edges are 
  split.
  
  \return the number of edges split.
*/
template<typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 int SD>
inline
int
splitCell(SimpMeshRed<3,3,T,_Node,_Cell,Cont>* mesh,
	  PointsOnManifold<3,2,SD,T>* boundaryManifold,
	  typename SimpMeshRed<3,3,T,_Node,_Cell,Cont>::CellIterator c) {
  typedef SimpMeshRed<3,3,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;

  // Count that we are splitting the longest edge of this cell.
  int count = 1;

  //Initialize (with a bad value) to avoid compiler warning.
  int i = -1, j = -1;
  computeMaximumEdgeLength<SMR>(c, &i, &j);
  CellIterator other;
  // Recursively split the neighbor until the longest edge of c is a shared 
  // longest edge.
  while (! isCommonMaximumEdge<SMR>(c, i, j, &other)) {
    count += splitCell(mesh, boundaryManifold, other);
  }
  // Split this cell.
  splitCell(mesh, boundaryManifold, c, i, j);
  // Return the number of edges split.
  return count;
}




//! Split the cell.
/*!
  Split other cells as necessary to ensure that only shared longest edges are 
  split.  Record the cells that are split.
  
  \return the number of edges split.
*/
template<typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 int SD,
	 typename CellIterOutIter>
inline
int
splitCell(SimpMeshRed<3,3,T,_Node,_Cell,Cont>* mesh,
	  PointsOnManifold<3,2,SD,T>* boundaryManifold,
	  typename SimpMeshRed<3,3,T,_Node,_Cell,Cont>::CellIterator c,
	  CellIterOutIter splitCells) {
  typedef SimpMeshRed<3,3,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Node::CellIncidentToNodeIterator 
    CellIncidentToNodeIterator;

  // Count that we are splitting the longest edge of this cell.
  int count = 1;

  // Get the longest edge of this cell.
  int i = -1, j = -1;
  computeMaximumEdgeLength<SMR>(c, &i, &j);

  // Recursively split the neighbor until the longest edge of c is a shared 
  // longest edge.
  CellIterator other;
  while (! isCommonMaximumEdge<SMR>(c, i, j, &other)) {
    count += splitCell(mesh, boundaryManifold, other, splitCells);
  }

  //
  // Record the cells that will be split.
  //
  *splitCells++ = c;
  NodeIterator a = c->getNode(i);
  NodeIterator b = c->getNode(j);
  // For each cell incident to a.
  for (CellIncidentToNodeIterator ci = a->getCellsBeginning(); 
       ci != a->getCellsEnd(); ++ci) {
    // If the cell is incident to b as well, then it is incident to the edge.
    if (ci->hasNode(b)) {
      *splitCells++ = *ci.base();
    }
  }

  // Split this cell.
  splitCell(mesh, boundaryManifold, c, i, j);

  return count;
}


END_NAMESPACE_GEOM

// End of file.
