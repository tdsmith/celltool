// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_SimpMeshRed_ipp__)
#error This file is an implementation detail of the class SimpMeshRed.
#endif

BEGIN_NAMESPACE_GEOM

template<int N, int M, typename T,
	 template<class> class V,
	 template<class> class C,
	 template<class,class> class CR>
template<typename VertInIter, typename SimpInIter>
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
build(VertInIter verticesBeginning, VertInIter verticesEnd, 
      SimpInIter simplicesBeginning, SimpInIter simplicesEnd) {
  // Clear the nodes and cells.
  _nodes.clear();
  _cells.clear();

  // Add the vertices.
  Node node;
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    node.setVertex(*verticesBeginning);
    insertNode(node);
  }

  // Make an array of the node iterators.
  ads::Array<1,NodeIterator> nodeHandles(computeNodesSize());
  {
    NodeIterator ni = getNodesBeginning();
    typename ads::Array<1,NodeIterator>::iterator i = nodeHandles.begin(); 
    for (; ni != getNodesEnd(); ++ni, ++i) {
      *i = ni;
    }
  }

  // Add the cells.
  Cell c;
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    for (int m = 0; m != M+1; ++m) {
      c.setNode(m, nodeHandles[(*simplicesBeginning)[m]]);
    }
    insertCell(c);
  }

  // Build the cell adjacencies.
  buildCellAdjacencies();
}


//----------------------------------------------------------------------------
// Manipulators.
//----------------------------------------------------------------------------


template<int N, int M, typename T,
	 template<class> class V,
	 template<class> class C,
	 template<class,class> class CR>
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
setNodeIdentifiers() const {
  int n = 0;
  for (NodeConstIterator i = getNodesBeginning(); i != getNodesEnd(); 
       ++i, ++n) {
    i->setIdentifier(n);
  }
}


template<int N, int M, typename T,
	 template<class> class V,
	 template<class> class C,
	 template<class,class> class CR>
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
setCellIdentifiers() const {
  int n = 0;
  for (CellConstIterator i = getCellsBeginning(); i != getCellsEnd(); 
       ++i, ++n) {
    i->setIdentifier(n);
  }
}


template<int N, int M, typename T,
	 template<class> class V,
	 template<class> class C,
	 template<class,class> class CR>
inline
typename SimpMeshRed<N,M,T,V,C,CR>::NodeIterator
SimpMeshRed<N,M,T,V,C,CR>::
insertNode(const Node& node) {
  // Check if this is the first node that we are inserting.
  const bool isFirstNode = _nodes.empty();

  // Insert a node at the end of the list.
  NodeIterator i = _nodes.insert(_nodes.end(), node);
  // Set the self iterator.
  i->setSelf(i);

  //
  // Set the identifier.
  //

  // If this is the first node inserted into the mesh.
  if (isFirstNode) {
    // The identifier is 0.
    i->setIdentifier(0);
  }
  else {
    NodeIterator prev = i;
    --prev;
    // If we have run out of positive integers.
    if (prev->getIdentifier() == std::numeric_limits<int>::max()) {
      // CONTINUE: This is problematic.  Something else may rely on the 
      // identifiers.
      // Recompute all of the identifiers.
      setNodeIdentifiers();
    }
    else {
      i->setIdentifier(prev->getIdentifier() + 1);
    }
  }

  return i;
}



template<int N, int M, typename T,
	 template<class> class V,
	 template<class> class C,
	 template<class,class> class CR>
inline
typename SimpMeshRed<N,M,T,V,C,CR>::CellIterator
SimpMeshRed<N,M,T,V,C,CR>::
insertCell(const Cell& c) {
  // Check if this is the first cell that we are inserting.
  const bool isFirstCell = areCellsEmpty();

  // Insert the cell at the end of the list.
  CellIterator ch = _cells.insert(_cells.end(), c);
  // Set the cell's self iterator.
  ch->setSelf(ch);
  NodeIterator null(0);
  // Set the cell iterators where necessary at the vertices.
  for (int m = 0; m != M + 1; ++m) {
    // CONTINUE: Should this be an assertion or an if?
#ifdef DEBUG_SimpMeshRed
    assert(ch->getNode(m) != NodeIterator(0));
#endif
    if (ch->getNode(m) != NodeIterator(0)) {
      ch->getNode(m)->insertCell(ch);
    }
  }

  //
  // Set the identifier.
  //
  // If this is the first cell inserted into the mesh.
  if (isFirstCell) {
    // The identifier is 0.
    ch->setIdentifier(0);
  }
  else {
    CellIterator prev = ch;
    --prev;
    // If we have run out of positive integers.
    if (prev->getIdentifier() == std::numeric_limits<int>::max()) {
      // Recompute all of the identifiers.
      setCellIdentifiers();
    }
    else {
      ch->setIdentifier(prev->getIdentifier() + 1);
    }
  }

  return ch;
}


//---------------------------------------------------------------------------
// Private member functions.
//---------------------------------------------------------------------------

/* CONTINUE REMOVE
   template<int N, int M, typename T,
   template<class> class V,
   template<class> class C,
   template<class,class> class CR>
   template<class Map>
   inline
   void
   SimpMeshRed<N,M,T,V,C,CR>::
   map_vertex_iterators_to_indices(Map& x) const
   {
   typedef typename Map::value_type value_type;

   x.clear();
   int i = 0;
   for (vertex_const_iterator vh = vertices_begin(); vh != vertices_end(); 
   ++vh, ++i) {
   x.insert(value_type(vh, i));
   }
   }
*/

template<int N, int M, typename T,
	 template<class> class V,
	 template<class> class C,
	 template<class,class> class CR>
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
buildCellAdjacencies() {
  //
  // First set all the neighbors to a null value.
  //
  
  const CellIterator nullCell(0);
  // Loop over the cells of the mesh.
  CellIterator ch = getCellsBeginning();
  const CellIterator chEnd = getCellsEnd();
  for (; ch != chEnd; ++ch) {
    // Loop over the nodes.
    for (int m = 0; m != M+1; ++m) {
      ch->setNeighbor(m, nullCell);
    }
  }

  // We can convert a node iterator to an integer with its identifier.
  // Thus we can represent a face with M integers.  We store these
  // integers in sorted order.  Then we can compare faces with the
  // opposite orientation.
  typedef ads::FixedArray<M,int> SortedFace;
  // To store the faces in a map data structure, we need less than
  // comparison.  We use comparison of the M-D composite number.
  typedef ads::less_composite<M,SortedFace> FaceComp;
  FaceComp comp;
  comp.set(0);
  // The face map stores cell iterators with the sorted faces as keys.
  typedef std::map<SortedFace, CellIterator, FaceComp> FaceMap;
  typedef typename FaceMap::value_type FaceMapValue;
  typedef typename FaceMap::iterator FaceMapIterator;
  FaceMap faceMap(comp);

  //
  // In the following loop we build one-sided adjacency information.
  //
    
  typename Cell::Face face;
  SortedFace sortedFace;
  FaceMapIterator fmi;
  // Loop over the cells of the mesh.
  for (ch = getCellsBeginning(); ch != chEnd; ++ch) {
    // Loop over the faces of this cell.
    for (int fi = 0; fi != M + 1; ++fi) {

      // Get the face composed of node iterators.
      ch->getFace(fi, &face);
      // Convert the node iterators to integers.
      for (int i = 0; i != M; ++i) {
	sortedFace[i] = face[i]->getIdentifier();
      }
      // Sort the vertex identifiers of the face.
      sortedFace.sort();

      // Check if this sorted face has already been encountered.
      fmi = faceMap.find(sortedFace);
      // If it has not been encountered before
      if (fmi == faceMap.end()) {
	// Add it to the map.
	faceMap.insert(FaceMapValue(sortedFace, ch));
      }
      // If it has been encountered before.
      else {
	// Set the adjacency information.
	ch->setNeighbor(fi, fmi->second);
	// Remove the face from the map.
	faceMap.erase(fmi);
      }
    }
  }

  //
  // Finally convert the one-sided adjacency links to two-sided links.
  //

  // Neighbor iterators.
  CellIterator nh;
  // Loop over the cells of the mesh.
  for (ch = getCellsBeginning(); ch != chEnd; ++ch) {
    // Loop over the faces of this cell.
    for (int m = 0; m != M + 1; ++m) {
      nh = ch->getNeighbor(m);
      if (nh != nullCell) {
	ch->getFace(m, &face);
	nh->setNeighbor(nh->getIndex(face), ch);
      }
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
