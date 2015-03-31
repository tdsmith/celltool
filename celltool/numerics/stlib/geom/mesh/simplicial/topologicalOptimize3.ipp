// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_topologicalOptimize3_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Return true if the edge is removable.
// The edge is not removable if it is on the boundary and is an edge feature.
template<typename SMR, int SD, typename T>
inline
bool
isRemovable(const typename SMR::Edge& edge,
	    const PointsOnManifold<3,2,SD,T>* manifold) {
  // If it is a boundary edge.
  if (isOnBoundary<SMR>(edge)) {
    const typename SMR::CellIterator cell = edge.first;
    const int i = edge.second;
    const int j = edge.third;
    // If the manifold is not specified, we don't remove boundary edges.
    // If the edge is a registered edge feature in the manifold, we don't 
    // remove it.
    // CONTINUE: REMOVE
#if 0
    std::cerr << "Before. " << i << " " << j << "\n"
	      << cell->getNode(i)->getIdentifier() << " "
	      << cell->getNode(j)->getIdentifier() << "\n"
	      << cell->getNode(i)->isOnBoundary() << " "
	      << cell->getNode(j)->isOnBoundary() << "\n";
#endif
    if (manifold == 0 || 
	manifold->hasEdge(cell->getNode(i)->getIdentifier(),
			  cell->getNode(j)->getIdentifier())) {
      // CONTINUE: REMOVE
      //std::cerr << "True.\n";
      return false;
    }
    else {
      // CONTINUE: REMOVE
      //std::cerr << "False.\n";
    }
  }
  return true;
}



// Get the next cell around the edge.
template<typename SMR>
inline
typename SMR::CellIterator
getNextCellAroundEdge(const typename SMR::CellIterator cell,
		      const typename SMR::NodeIterator a, 
		      const typename SMR::NodeIterator b) {
  return cell->getNeighbor(getNextNodeIndex<SMR>(cell, a, b));
}


// Get the previous cell around the edge.
template<typename SMR>
inline
typename SMR::CellIterator
getPreviousCellAroundEdge(const typename SMR::CellIterator cell,
			  const typename SMR::NodeIterator a, 
			  const typename SMR::NodeIterator b) {
  return cell->getNeighbor(getPreviousNodeIndex<SMR>(cell, a, b));
}


// Get the cells incident to an edge in positive order around it.
template<typename SMR, typename CellIteratorOutputIterator>
inline
void
getIncidentCellsInOrder(const typename SMR::Edge& edge, 
			CellIteratorOutputIterator output) {
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::NodeIterator NodeIterator;

  // Start at the known incident cell.
  CellIterator cell = edge.first;

  // The source and target nodes.
  const NodeIterator a = cell->getNode(edge.second);
  const NodeIterator b = cell->getNode(edge.third);

  if (isOnBoundary<SMR>(edge)) {
    // Back up until we reach the boundary.
    CellIterator prev;
    while ((prev = getPreviousCellAroundEdge<SMR>(cell, a, b)) != 
	   CellIterator(0)) {
      cell = prev;
    }
    // Go forward until we reach the boundary.
    while (cell != CellIterator(0)) {
      *output++ = cell;
      cell = getNextCellAroundEdge<SMR>(cell, a, b);
    }
  }
  else {
    // Record the known incident cell.
    *output++ = cell;
    // Go around the edge until we get back to the first cell.
    CellIterator next = cell;
    while ((next = getNextCellAroundEdge<SMR>(next, a, b)) != cell) {
      assert(next != CellIterator(0));
      *output++ = next;
    }
  }
}



//! Perform edge removal on the specified edge if it improves the quality of the mesh.
/*!
  \param mesh The simplicial mesh.
  \param edge The edge on which to try edge removal.
  \param nextValidCell The next valid cell after trying edge removal.  If the
  edge is not removed, this is simply the next cell.  If the edge removal
  operation is performed, then we may need to skip some of the deleted cells.
  \param edgeRemovalDataStructure This is passed as a parameter to avoid 
  the expense of constructing for each call of this function.
  \param activeCells The cells in the mesh that might be improved by 
  topological changes.  If this function deletes cells, they will be removed 
  from this set.  If this function inserts cells, they will be added to this
  set.
  \param edgeRemovalOperations This is used to keep track of which kind of 
  flips are performed.  You can pass 0 for this argument if you are not 
  using these statistics.
*/
template<typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class _Cont,
	 class _QualityMetric>
inline
bool
tryEdgeRemoval
(SimpMeshRed<3,3,T,_Node,_Cell,_Cont>* mesh, 
 const typename SimpMeshRed<3,3,T,_Node,_Cell,_Cont>::Edge& edge,
 typename SimpMeshRed<3,3,T,_Node,_Cell,_Cont>::CellIterator* nextValidCell,
 EdgeRemoval<_QualityMetric>* edgeRemovalDataStructure,
 typename SimpMeshRed<3,3,T,_Node,_Cell,_Cont>::CellIteratorSet* activeCells,
 std::multiset<std::pair<int,int> >* edgeRemovalOperations) {

  typedef SimpMeshRed<3,3,T,_Node,_Cell,_Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Cell Cell;
  typedef typename SMR::Vertex Vertex;

  assert(mesh != 0);
  assert(edgeRemovalDataStructure != 0);
  assert(activeCells != 0);

  // Move to the next cell.
  *nextValidCell = edge.first;
  ++*nextValidCell;

  // Get the incident cells.
  std::vector<CellIterator> incidentCells;
  getIncidentCellsInOrder<SMR>(edge, std::back_inserter(incidentCells));
  const int incidentCellsSize = int(incidentCells.size());

  // There must be more than one adjacent cell for edge removal.
  if (incidentCells.size() == 1) {
    return false;
  }

  // The source and target nodes.
  const NodeIterator source = edge.first->getNode(edge.second);
  const NodeIterator target = edge.first->getNode(edge.third);

  const bool isEdgeOnBoundary = isOnBoundary<SMR>(edge);

  //
  // Build the edge removal data structure.
  //

  // Set the source and target.
  edgeRemovalDataStructure->setSource(source->getVertex());
  edgeRemovalDataStructure->setTarget(target->getVertex());

  int localIndex;
  std::vector<NodeIterator> ring;
  if (isEdgeOnBoundary) {
    ring.resize(incidentCellsSize + 1);
    localIndex = getNextNodeIndex<SMR>(incidentCells[0], source, target);
    ring[0] = incidentCells[0]->getNode(localIndex);
    for (int i = 0; i != incidentCellsSize; ++i) {
      localIndex = getPreviousNodeIndex<SMR>(incidentCells[i], source, target);
      ring[i+1] = incidentCells[i]->getNode(localIndex);
    }
  }
  else {
    ring.resize(incidentCellsSize);
    for (int i = 0; i != incidentCellsSize; ++i) {
      localIndex = getNextNodeIndex<SMR>(incidentCells[i], source, target);
      ring[i] = incidentCells[i]->getNode(localIndex);
    }
  }
  assert(ring.size() >= 3);
  {
    std::vector<Vertex> ringVertices(ring.size());
    const int Size = ring.size();
    for (int i = 0; i != Size; ++i) {
      ringVertices[i] = ring[i]->getVertex();
    }
    edgeRemovalDataStructure->setRing(ringVertices.begin(), 
				      ringVertices.end());
  }

  // If removing the edge will improve the mesh.
  if (edgeRemovalDataStructure->solve()) {

    // CONTINUE: REMOVE
#if 0
    std::cerr << "Incident group\n";
    for (int i = 0; i != incidentCellsSize; ++i) {
      std::cerr << i << " ";
      for (int n = 0; n != 4; ++n) {
	std::cerr << incidentCells[i]->isFaceOnBoundary(n) << " ";
      }
      std::cerr << "\n";
    }
#endif

    // Record the operation we are going to perform so statistics can be
    // reported later.  (If they pass a valid multiset pointer.)
    if (edgeRemovalOperations != 0) {
      if (isEdgeOnBoundary) {
	// A boundary edge removal replaces n tetrahedra with 2(n-1).
	const int n = ring.size() - 1;
	edgeRemovalOperations->insert(std::make_pair(n, 2 * (n - 1)));
      }
      else {
	// A interior edge removal replaces n tetrahedra with 2(n-2).
	const int n = ring.size();
	edgeRemovalOperations->insert(std::make_pair(n, 2 * (n - 2)));
      }
    }

    // Remove the cells from the active set.
    for (int i = 0; i != incidentCellsSize; ++i) {
      activeCells->erase(incidentCells[i]);
    }

    //
    // Make the new cells.
    //
    const int NumberOfTriangles = 
      edgeRemovalDataStructure->getNumberOfTriangles();
    // New cells.
    std::vector<CellIterator> sourceCells(NumberOfTriangles), 
      targetCells(NumberOfTriangles);

    // Ring indices.
    ads::FixedArray<3,int> ri;
    // For each triangle.
    for (int i = 0; i != NumberOfTriangles; ++i) {
      // Get the triangle in the ring.
      ri = edgeRemovalDataStructure->getTriangle(i);
      //
      // Insert cells with the incident nodes.
      // We will set the adjacent cells below.
      //
      // The tet made from the triangle and the edge target.
      targetCells[i] = mesh->insertCell(Cell(ring[ri[0]], ring[ri[1]], 
					     ring[ri[2]], target));
      // Add the new cell to the active set.
      activeCells->insert(targetCells[i]);
      // The tet made from the triangle and the edge source.
      sourceCells[i] = mesh->insertCell(Cell(source, ring[ri[0]], 
					     ring[ri[1]], ring[ri[2]]));
      // Add the new cell to the active set.
      activeCells->insert(sourceCells[i]);
    }

    // Set the adjacent cells.
    int faceIndex, mirrorIndex;
    for (int i = 0; i != NumberOfTriangles; ++i) {
      // Get the triangle in the ring.
      ri = edgeRemovalDataStructure->getTriangle(i);
      CellIterator& t = targetCells[i];
      CellIterator& s = sourceCells[i];
      // Set the adjacencies between the source-incident and target-incident
      // cells.
      t->setNeighbor(3, s);
      s->setNeighbor(0, t);
      // Combinations of two nodes of the triangle.
      for (int a = 0; a != 3; ++a) {
	const int b = (a + 1) % 3;
	const int c = (a + 2) % 3;

	// Set the adjacencies amongst the target cells.
	for (int j = 0; j != NumberOfTriangles; ++j) {
	  if (i == j) {
	    continue;
	  }
	  // A different target cell.
	  CellIterator& cell = targetCells[j];
	  // If the other cell has the same face.
	  if (hasFace<SMR>(cell, target, ring[ri[a]], ring[ri[b]])) {
	    // CONTINUE: REMOVE
	    //std::cerr << "amongst targets " << a << " " << b << " " << c 
	    //<< "\n";
	    // Set the neighbor of the target cell.
	    t->setNeighbor(c, cell);
	    // The corresponding adjacency will be set in another iteration.
	  }
	}

	// Set the adjacencies amongst the source cells.
	for (int j = 0; j != NumberOfTriangles; ++j) {
	  if (i == j) {
	    continue;
	  }
	  // A different source cell.
	  CellIterator& cell = sourceCells[j];
	  // If the other cell has the same face.
	  if (hasFace<SMR>(cell, source, ring[ri[a]], ring[ri[b]])) {
	    // CONTINUE: REMOVE
	    //std::cerr << "amongst sources " << a << " " << b << " " << c 
	    //<< "\n";
	    // Set the neighbor of the target cell.
	    s->setNeighbor(c + 1, cell);
	    // The corresponding adjacency will be set in another iteration.
	  }
	}

	// Use the cells incident to the removed edge to set the rest of the 
	// adjacencies.
	for (int j = 0; j != incidentCellsSize; ++j) {
	  CellIterator& cell = incidentCells[j];
	  // Target cell.
	  // If the incident cell has the same face.
	  if (hasFace<SMR>(cell, target, ring[ri[a]], ring[ri[b]], 
			   &faceIndex)) {
	    // CONTINUE: REMOVE
	    //std::cerr << "target " << a << " " << b << " " << c << "\n";
	    // Set the neighbor of the target cell.
	    t->setNeighbor(c, cell->getNeighbor(faceIndex));
	    // If it is not a boundary face.
	    if (! t->isFaceOnBoundary(c)) {
	      // Set the corresponding adjacency.
	      mirrorIndex = cell->getMirrorIndex(faceIndex);
	      t->getNeighbor(c)->setNeighbor(mirrorIndex, t);
	    }
	    // Unlink that face of the edge-incident cell.
	    cell->setNeighbor(faceIndex, CellIterator(0));
	  }
	  // Source cell.
	  // If the incident cell has the same face.
	  if (hasFace<SMR>(cell, source, ring[ri[a]], ring[ri[b]], 
			   &faceIndex)) {
	    // CONTINUE: REMOVE
	    //std::cerr << "source " << a << " " << b << " " << c << "\n";
	    // Set the neighbor of the target cell.
	    s->setNeighbor(c + 1, cell->getNeighbor(faceIndex));
	    // If it is not a boundary face.
	    if (! s->isFaceOnBoundary(c + 1)) {
	      // Set the corresponding adjacency.
	      mirrorIndex = cell->getMirrorIndex(faceIndex);
	      s->getNeighbor(c + 1)->setNeighbor(mirrorIndex, s);
	    }
	    // Unlink that face of the edge-incident cell.
	    cell->setNeighbor(faceIndex, CellIterator(0));
	  }
	} 
      }
    }

    // For the next valid cell, skip the cells that we are going to delete.
    *nextValidCell = ads::skipIteratorsUsingIteration(*nextValidCell,
						      incidentCells.begin(),
						      incidentCells.end());

    // Remove the cells around the edge.  We have already unlinked the 
    // edge-incident cells from the rest of the mesh.  The unlinking done in 
    // the following call is harmless.
    mesh->eraseCells(incidentCells.begin(), incidentCells.end());

    // CONTINUE: REMOVE
#if 0
    std::cerr << "Source group\n";
    for (int i = 0; i != NumberOfTriangles; ++i) {
      std::cerr << i << " ";
      for (int n = 0; n != 4; ++n) {
	std::cerr << sourceCells[i]->isFaceOnBoundary(n) << " ";
      }
      std::cerr << "\n";
    }
    std::cerr << "Target group\n";
    for (int i = 0; i != NumberOfTriangles; ++i) {
      std::cerr << i << " ";
      for (int n = 0; n != 4; ++n) {
	std::cerr << targetCells[i]->isFaceOnBoundary(n) << " ";
      }
      std::cerr << "\n";
    }
#endif

    // Indicate that we removed the edge.
    return true;
  }

  // We did not remove the edge.
  return false;
}







//! Perform face removal on the specified face if it improves the quality of the mesh.
/*!
  \param mesh The simplicial mesh.
  \param face The face on which to try face removal.
  \param nextValidCell The next valid cell after trying face removal.  If the
  face is not removed, this is simply the next cell.  If the face removal
  operation is performed, then we may need to skip some of the deleted cells.
  \param faceRemovalDataStructure This is passed as a parameter to avoid 
  the expense of constructing for each call of this function.
  \param activeCells The cells in the mesh that might be improved by 
  topological changes.  If this function deletes cells, they will be removed 
  from this set.  If this function inserts cells, they will be added to this
  set.
  \param faceRemovalOperations This is used to keep track of which kind of 
  flips are performed.  You can pass 0 for this argument if you are not 
  using these statistics.
*/
template<typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class _Cont,
	 class _QualityMetric>
inline
bool
tryFaceRemoval
(SimpMeshRed<3,3,T,_Node,_Cell,_Cont>* mesh, 
 const typename SimpMeshRed<3,3,T,_Node,_Cell,_Cont>::Face& face,
 typename SimpMeshRed<3,3,T,_Node,_Cell,_Cont>::CellIterator* nextValidCell,
 FaceRemoval<_QualityMetric>* faceRemovalDataStructure,
 typename SimpMeshRed<3,3,T,_Node,_Cell,_Cont>::CellIteratorSet* activeCells,
 std::multiset<std::pair<int,int> >* faceRemovalOperations) {

  typedef SimpMeshRed<3,3,T,_Node,_Cell,_Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::Cell Cell;
  typedef typename SMR::Vertex Vertex;

  assert(mesh != 0);
  assert(faceRemovalDataStructure != 0);
  assert(activeCells != 0);

#ifdef DEBUG_TetMesh
  // Make sure the face is not in the boundary.
  assert(! isOnBoundary<SMR>(face));
#endif

  // Move to the next cell.
  *nextValidCell = face.first;
  ++*nextValidCell;

  // Get the two adjacent cells.  
  CellIterator cell0 = face.first;
  const int sourceIndex = face.second;
  CellIterator cell1 = cell0->getNeighbor(sourceIndex);
  const int targetIndex = cell0->getMirrorIndex(sourceIndex);
  // Get the two nodes opposite the face.
  const NodeIterator source = cell0->getNode(sourceIndex);
  const NodeIterator target = cell1->getNode(targetIndex);
  
  // Return false if there is already an edge between the source
  // and the target.
  if (doNodesShareACell<SMR>(source, target)) {
    return false;
  }

  // Set the vertices on the shared face.
  typename Cell::Face faceNodes;
  cell0->getFace(sourceIndex, &faceNodes);
  faceRemovalDataStructure->setFace(faceNodes[0]->getVertex(),
				    faceNodes[1]->getVertex(),
				    faceNodes[2]->getVertex());

  // Set the source and target vertices.
  faceRemovalDataStructure->setSource(source->getVertex());
  faceRemovalDataStructure->setTarget(target->getVertex());

  // If removing the face will improve the mesh.
  if (faceRemovalDataStructure->flip23()) {
    // A face removal replaces n tetrahedra with n/2 + 2 tetrahedra.
    if (faceRemovalOperations != 0) {
      faceRemovalOperations->insert(std::make_pair(2, 3));
    }

    //
    // Make the three new cells.
    //
    // New cells.
    ads::FixedArray<3,CellIterator> newCells;

    // For each cell.
    for (int i = 0; i != 3; ++i) {
      // Insert cells with the incident nodes.
      // We will set the adjacent cells below.
      // The tet made from the triangle and the edge target.
      newCells[i] = mesh->insertCell(Cell(source, faceNodes[i], 
					  faceNodes[(i + 1) % 3], target));
      // Add the new cell to the active set.
      activeCells->insert(newCells[i]);
    }

    // For each cell.
    int n, m;
    CellIterator neighbor;
    for (int i = 0; i != 3; ++i) {
      // Set the adjacencies among the three new cells.
      newCells[i]->setNeighbor(1, newCells[(i + 1) % 3]);
      newCells[i]->setNeighbor(2, newCells[(i + 2) % 3]);

      //
      // Set the adjacencies with the surrounding region.
      //

      // Source adjacency.
      n = cell1->getIndex(faceNodes[(i + 2) % 3]);
      neighbor = cell1->getNeighbor(n);
      newCells[i]->setNeighbor(0, neighbor);
      // If this is not a boundary face.
      if (neighbor != 0) {
	// Set the corresponding adjacency to the new cell.
	m = cell1->getMirrorIndex(n);
	neighbor->setNeighbor(m, newCells[i]);
      }
      // Unlink the old cell.
      cell1->setNeighbor(n, 0);

      // Target adjacency.
      n = cell0->getIndex(faceNodes[(i + 2) % 3]);
      neighbor = cell0->getNeighbor(n);
      newCells[i]->setNeighbor(3, neighbor);
      // If this is not a boundary face.
      if (neighbor != 0) {
	// Set the corresponding adjacency to the new cell.
	m = cell0->getMirrorIndex(n);
	neighbor->setNeighbor(m, newCells[i]);
      }
      // Unlink the old cell.
      cell0->setNeighbor(n, 0);
    }

    // For the next valid cell, skip the cells that we are going to delete.
    ads::FixedArray<2,CellIterator> cellsToDelete(cell0, cell1);
    *nextValidCell = ads::skipIteratorsUsingIteration(*nextValidCell,
						      cellsToDelete.begin(),
						      cellsToDelete.end());

    // Remove the two old cells.  We have already unlinked these cells 
    // from the rest of the mesh.  The unlinking done in 
    // the following calls is harmless.
    mesh->eraseCell(cell0);
    mesh->eraseCell(cell1);

    // Indicate that we removed the face.
    return true;
  }
  // We did not remove the face.
  return false;
}





// Use edge and face removal to optimize the mesh.
template<class _QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD>
inline
int
topologicalOptimize(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		    const PointsOnManifold<3,2,SD,T>* manifold,
		    std::multiset<std::pair<int,int> >* edgeRemovalOperations,
		    std::multiset<std::pair<int,int> >* faceRemovalOperations,
		    const int maxSteps) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::EdgeIterator EdgeIterator;
  typedef typename SMR::Edge Edge;
  typedef typename SMR::Face Face;
  typedef typename SMR::CellIteratorSet CellIteratorSet;

  // CONTINUE: REMOVE
#if 0
  std::cerr << "In topologicalOptimize, Boundary nodes.\n";
  // For each node in the mesh.
  for (typename SMR::NodeIterator i = mesh->getNodesBeginning(); 
       i != mesh->getNodesEnd(); ++i) {
    // If the node is on the boundary.
    if (i->isOnBoundary()) {
      std::cerr << i->getIdentifier() << " ";
    }
  }
  std::cerr << "\n";
#endif

  // CONTINUE: REMOVE
#if 0
  std::cerr << "Has points:\n";
  const int size = mesh->computeNodesSize();
  for (int i = 0; i != size; ++i) {
    if (manifold->hasPoint(i)) {
      std::cerr << i << " ";
    }
  }
  std::cerr << "\n";
#endif

  // CONTINUE: REMOVE
#if 0
  std::cerr << "Has points:\n";
  for (CellIterator i = mesh->getCellsBeginning(); i != mesh->getCellsEnd(); 
       ++i) {
    for (int n = 0; n != 4; ++n) {
      int identifier = i->getNode(n)->getIdentifier();
      if (i->getNode(n)->isOnBoundary()) {
	std::cerr << identifier << " ";
      }
    }
  }
  std::cerr << "\n";
#endif

  int count = 0;

  // The edge removal data structure.
  EdgeRemoval<_QualityMetric> edgeRemovalDataStructure;
  // The face removal data structure.
  FaceRemoval<_QualityMetric> faceRemovalDataStructure;

  // Initially, all of the cells are active.
  CellIteratorSet activeCells;
  for (CellIterator i = mesh->getCellsBeginning(); i != mesh->getCellsEnd();
       ++i) {
    activeCells.insert(i);
  }

  // We do not need to use the next valid cell capability of tryEdgeRemoval and
  // tryFaceRemoval.
  CellIterator dummy;
  typename CellIteratorSet::iterator cii;
  bool result;
  // Iterate until there are no more active cells.
  while (! activeCells.empty()) {
    // Get an active cell.
    cii = activeCells.begin();
    CellIterator ci = *cii;
    activeCells.erase(cii);

    // Try edge removal on each of the six edges.
    Edge edge(ci, 0, 0);
    result = false;
    for (edge.second = 0; edge.second != 3 && ! result; ++edge.second) {
      for (edge.third = edge.second + 1; edge.third != 4 && ! result; 
	   ++edge.third) {
	// CONTINUE: REMOVE
#if 0
	int i = edge.second;
	int j = edge.third;
	std::cerr << "In topologicalOptimize:\n"
		  << ci->getNode(i)->getIdentifier() << " "
		  << ci->getNode(j)->getIdentifier() << "\n"
		  << ci->getNode(i)->isOnBoundary() << " "
		  << ci->getNode(j)->isOnBoundary() << "\n";
#endif
	// Check that the edge is not a boundary edge feature.
	if (isRemovable<SMR>(edge, manifold)) {
	  // Try edge removal.
	  result = tryEdgeRemoval(mesh, edge, &dummy, 
				  &edgeRemovalDataStructure, 
				  &activeCells, edgeRemovalOperations);
	  // CONTINUE: REMOVE
	  //std::cerr << "Try edge removal: " << result << "\n";
	}
      }
    }

    // CONTINUE: add back in
#if 0
    // If there was no edge removal operation.
    if (! result) {
      // Try face removal on each of the four faces.
      Face face(ci, 0);
      for (; face.second != 4 && ! result; ++face.second) {
	if (! isOnBoundary<SMR>(face)) {
	  result = tryFaceRemoval(mesh, face, &dummy,
				  &faceRemovalDataStructure, 
				  &activeCells, faceRemovalOperations);
	}
      }
    }
#endif

    if (result) {
      ++count;
    }
    
    if (count >= maxSteps) {
      break;
    }
  }
  return count;
}

END_NAMESPACE_GEOM

// End of file.
