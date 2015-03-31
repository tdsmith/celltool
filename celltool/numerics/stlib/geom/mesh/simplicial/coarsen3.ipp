// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_coarsen3_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Return the minimum edge length.
/*!
  Set \c i and \c j to the minimum edge indices.
*/
template<class SMR>
inline
typename SMR::Number
computeMinimumEdgeLength(const typename SMR::CellConstIterator c, 
			 int* i, int* j) {
  return c->computeMinimumEdgeLength(i, j);
}



//! Return true if the edge has one or more interior nodes.
/*!
  \c i and \c j are the indices of the edge in the cell.
*/
template<class SMR>
inline
bool
edgeHasAnInteriorNode(const typename SMR::CellConstIterator c, 
		      const int i, const int j) {
  return ! c->getNode(i)->isOnBoundary() || 
    ! c->getNode(j)->isOnBoundary();
}



//! Get the incident boundary faces.
/*!
  \c i and \c j are the indices of the edge in the cell.
*/
template<class SMR>
inline
void
getBoundaryFaces(const typename SMR::CellConstIterator c, 
		 const int i, const int j,
		 typename SMR::CellConstIterator* a, int* p, 
		 typename SMR::CellConstIterator* b, int* q) {
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef typename SMR::NodeConstIterator NodeConstIterator;
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    IncidentCellIterator;

  // REMOVE
  assert(isOnBoundary<SMR>(c, i, j));

  CellConstIterator cell;
  NodeConstIterator source = c->getNode(i);
  NodeConstIterator target = c->getNode(j);

  // REMOVE
  /*
  std::cerr << "source:\n";
  source->put(std::cerr);
  std::cerr << "target:\n";
  target->put(std::cerr);
  */

  int sourceIndex, targetIndex;

  // This indicates we are still looking for the first face.
  *p = -1;
  // For each cell incident to a.
  for (IncidentCellIterator ci = source->getCellsBeginning(); 
       ci != source->getCellsEnd(); ++ci) {
    // REMOVE
    //std::cerr << "incident to a:\n";
    //ci->put(std::cerr);
    // If the cell is incident to target as well, then it is incident to 
    // the edge.
    if (ci->hasNode(target)) {
      // REMOVE
      //std::cerr << "incident to b:\n";

      cell = *ci.base();
      sourceIndex = cell->getIndex(source);
      targetIndex = cell->getIndex(target);
      // For each face.
      for (int m = 0; m != SMR::M + 1; ++m) {
	// The incident faces.
	if (m != sourceIndex && m != targetIndex) {
	  // If the incident face is on the boundary.
	  if (cell->getNeighbor(m) == 0) {
	    if (*p == -1) {
	      *a = cell;
	      *p = m;
	      // REMOVE
	      /*
	      std::cerr << "a = " << a->identifier() 
			<< "p = " << p << "\n";
	      */
	    }
	    else {
	      *b = cell;
	      *q = m;
	      // REMOVE
	      /*
	      std::cerr << "b = " << b->identifier() 
			<< "q = " << q << "\n";
	      */
	      return;
	    }
	  }
	}
      }
    }
  }

  // REMOVE
  //std::cerr << "getBoundaryFaces() assertion failure.\n";
  // If we make it here, we didn't find the two boundary faces.
  assert(false);
}



// CONTINUE: Remove.
#if 0
//! Return true if the edge is an edge feature.
template<class SMR>
inline
bool
isEdgeFeature(const typename SMR::CellConstIterator c, 
	      const int i, const int j,
	      const typename SMR::Number edgeDeviation) {
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef typename SMR::Vertex Vertex;

  // REMOVE
  //std::cerr << "isEdgeFeature() start.\n";

  // These define the boundary faces.
  CellConstIterator a, b;
  int p, q;
  // Get the boundary faces.
  getBoundaryFaces<SMR>(c, i, j, &a, &p, &b, &q);
  
  // The face normals.
  Vertex x, y;
  computeFaceNormal<SMR>(a, p, &x);
  computeFaceNormal<SMR>(b, q, &y);

  // REMOVE
  //std::cerr << "isEdgeFeature " << x << "    " << y << "   " 
  //<< edgeDeviation << "\n";
  //std::cerr << "isEdgeFeature() done.\n";

  // The angle between the face normals is the deviation from straight.
  if (computeAngle(x, y) > edgeDeviation) {
    return true;
  }
  return false;
}
#endif



// CONTINUE: Remove
#if 0
//! Return true if the boundary node is a corner feature.
template<class SMR>
inline
bool
isCornerFeature(const typename SMR::NodeConstIterator node, 
		const typename SMR::Number edgeDeviation,
		const typename SMR::Number cornerDeviation) {
  typedef typename SMR::Number Number;

  LOKI_STATIC_CHECK(SMR::N == 3 && SMR::M == 3, Bad_dimensions);

  if (std::abs(computeIncidentCellsAngle<SMR>(node) - 
	       2 * numerical::Constants<Number>::Pi()) > cornerDeviation) {
    return true;
  }
  
  // CONTINUE: Check for edge features.

  return false;
}
#endif



// CONTINUE: Remove
#if 0
//! Return true if the boundary node has incident edge features.
template<class SMR>
inline
bool
hasIncidentEdgeFeatures(const typename SMR::NodeConstIterator node, 
			const typename SMR::Number edgeDeviation) {
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    IncidentCellIterator;

  LOKI_STATIC_CHECK(SMR::N == 3 && SMR::M == 3, BadDimensions);

  // For each incident cell.
  CellConstIterator cell;
  int i, j;
  for (IncidentCellIterator ci = node->getCellsBeginning(); 
       ci != node->getCellsEnd(); ++ci) {
    cell = *ci.base();
    i = cell->getIndex(node);
    // For each incident edge.
    for (j = 0; j != SMR::M + 1; ++j) {
      if (j == i) {
	continue;
      }
      // If this is a boundary edge and an edge feature.
      if (isOnBoundary<SMR>(cell, i, j) &&
	  isEdgeFeature<SMR>(cell, i, j, edgeDeviation)) {
	return true;
      }
    }
  }
  return false;
}
#endif



//! Return true if this is a common minimum edge.
/*!
  \c i and \c j are the indices of the minimum edge of \c c.
*/
template<class SMR>
inline
bool
isCommonMinimumEdge(const typename SMR::CellConstIterator c, 
		    const int i, const int j) {
  typedef typename SMR::Number Number;
  typedef typename SMR::NodeConstIterator NodeConstIterator;
  typedef typename SMR::Node::CellIncidentToNodeConstIterator 
    IncidentCellIterator;

  NodeConstIterator a = c->getNode(i);
  NodeConstIterator b = c->getNode(j);

  // The minimum edge length of c.  We decrease the length to avoid errors
  // in comparing floating point numbers.
  const Number minLength = 
    geom::computeDistance(a->getVertex(), b->getVertex()) *
    (1.0 - 10.0 * std::numeric_limits<Number>::epsilon());

  int p, q;
  // For each cell incident to a.
  for (IncidentCellIterator ci = a->getCellsBeginning(); 
       ci != a->getCellsEnd(); ++ci) {
    // If the cell is incident to b as well, then it is incident to the edge.
    // (The first condition prevents us from checking c.)
    if (*ci.base() != c && ci->hasNode(b)) {
      // If another incident cell has a shorter edge than the min edge of c.
      if (computeMinimumEdgeLength<SMR>(*ci.base(), &p, &q) < minLength) {
	return false;
      }
    }
  }
  return true;
}



//! Calculate the merged node location for a collapsing edge.
/*!
  \param c The cell.
  \param i The source index of edge to be collapsed.
  \param i The target index of edge to be collapsed.
  \param x Will be set to the merged node location.
*/
template<class SMR>
inline
void
calculateMergedLocation(const typename SMR::CellConstIterator c, 
			const int i, const int j, 
			typename SMR::Vertex* x) {
  // Determine the position of the merged node.
  bool bdry1 = c->getNode(i)->isOnBoundary();
  bool bdry2 = c->getNode(j)->isOnBoundary();
  // If both are on the boundary or both are interior.
  if ((bdry1 && bdry2) || (!bdry1 && !bdry2)) {
    // Choose the midpoint.
    *x = c->getNode(i)->getVertex();
    *x += c->getNode(j)->getVertex();
    *x *= 0.5;
  }
  // If 1 is on the boundary and 2 is interior.
  else if (bdry1) {
    // Choose the boundary point.
    *x = c->getNode(i)->getVertex();
  }
  // If 2 is on the boundary and 1 is interior.
  else {
    // Choose the boundary point.
    *x = c->getNode(j)->getVertex();
  }
}



template<typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
collapseCell(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh,
	     const typename SimpMeshRed<3,3,T,Node,Cell,Cont>::CellIterator c, 
	     const int i, const int j) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;

  //std::cout << "collapseCell() " << c->identifier() << " " 
  //<< i << " " << j << "\n";

  // The two neigbors that aren't being collapsed.
  const CellIterator n1 = c->getNeighbor(i);
  const CellIterator n2 = c->getNeighbor(j);
  const int k1 = c->getMirrorIndex(i);
  const int k2 = c->getMirrorIndex(j);
  // Remove the cell.
  mesh->eraseCell(c);
  // Fix the edges.
  if (n1 != CellIterator(0)) {
    n1->setNeighbor(k1, n2);
  }
  if (n2 != CellIterator(0)) {
    n2->setNeighbor(k2, n1);
  }
}


// CONTINUE: Check for collapses that will change the topology.
// If the edge may be collapsed: Set the node location for the source node.
// (The target node will be erased in merging the two.)  Update the boundary.
// Return true.
// If the edge may not be collapsed: return false.
template<typename SMR, class QualityMetric, int SD, typename T>
inline
bool
updateNodesAndManifoldForBoundaryEdgeCollapse
(const typename SMR::CellIterator c, 
 const int i, const int j,
 const T minimumAllowedQuality, const T qualityFactor,
 PointsOnManifold<3,2,SD,T>* manifold) {
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBe3);

  // CONTINUE
  //std::cout << "updateNodesAndManifoldForBoundaryEdgeCollapse\n";

  // If the cell has no neighbors, then we do not allow the edge to 
  // be collapsed.
  if (c->getNumberOfNeighbors() == 0) {
    return false;
  }

  // CONTINUE HERE.
  // Deal with the case that collapsing the edge collapses a cavity.
  // This is analogous to the 2-D case of collapsing a triangle cavity.

  // The old position for the source vertex.
  const Vertex oldPosition1(c->getNode(i)->getVertex());
  // The old position for the target vertex.
  const Vertex oldPosition2(c->getNode(j)->getVertex());
  // CONTINUE
  /*
  std::cout << "oldPosition1 = " << oldPosition1 
	    << ", oldPosition2 = " << oldPosition2 
	    << "\n";
  */
  // The midpoint.
  Vertex midPoint = oldPosition1;
  midPoint += oldPosition2;
  midPoint *= 0.5;

  T oldQuality = 0;
  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells before the proposed 
    // edge collapse.
    oldQuality = computeMinimumQualityOfCellsIncidentToNodesOfEdge
      <SMR, QualityMetric>(c, i, j);
  }

  // If a manifold is not specified, try moving the source node to the 
  // midpoint of the edge.
  if (manifold == 0) {
    //std::cout << "A manifold is not specified.\n";
    // Set the locations for the merged node.
    c->getNode(i)->setVertex(midPoint);
    c->getNode(j)->setVertex(midPoint);

    // If they specified a minimum quality or a quality factor.
    if (minimumAllowedQuality > 0 || qualityFactor > 0) {
      // Check if collapsing the edge will cause to much damage to the mesh.
      // Compute the minimum quality of the incident cells that will remain 
      // after the collapse.
      const T newQuality = 
	computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
	<SMR, QualityMetric>(c, i, j);
      // If the quality is too low.
      if (newQuality < minimumAllowedQuality ||
	  newQuality < qualityFactor * oldQuality) {
	// Move the nodes back to their old positions.
	c->getNode(i)->setVertex(oldPosition1);
	c->getNode(j)->setVertex(oldPosition2);
	// Do not collapse the edge.
	return false;
      }
    }
    // Otherwise, we may collapse the edge.
    return true;
  }

  const int identifier1 = c->getNode(i)->getIdentifier();
  const int identifier2 = c->getNode(j)->getIdentifier();

  // If the source is a corner feature.
  const bool isCorner1 = manifold->isOnCorner(identifier1);
  // If the target is a corner feature.
  const bool isCorner2 = manifold->isOnCorner(identifier2);

  /*
  std::cout << "isCorner1 = " << isCorner1
	    << ", isCorner2 = " << isCorner2 << "\n";
  */

  // If both are corner features, we cannot collapse the edge.
  if (isCorner1 && isCorner2) {
    //std::cout << "If both are corner features, we cannot collapse the edge.\n";
    return false;
  }

  const bool isEdgeFeature = manifold->hasEdge(identifier1, identifier2);
  const bool isSurface1 = manifold->isOnSurface(identifier1);
  const bool isSurface2 = manifold->isOnSurface(identifier2);

  /*
  std::cout << "isEdgeFeature = " << isEdgeFeature
	    << ", isSurface1 = " << isSurface1
	    << ", isSurface2 = " << isSurface2 << "\n";
  */

  // In this case, we cannot move either end point, so we cannot collapse the
  // edge.
  if (! isEdgeFeature && ! isSurface1 && ! isSurface2) {
    //std::cout << "We cannot move either end point.\n";
    return false;
  }

  const bool isMovable1 = ((! isEdgeFeature && isSurface1) || 
			   (isEdgeFeature && ! isCorner1));
  const bool isMovable2 = ((! isEdgeFeature && isSurface2) || 
			   (isEdgeFeature && ! isCorner2));

  //
  // Determine the new position for the merged vertex if we collapse the edge.
  //

  // Initialize with a bad value.
  Vertex newPosition(std::numeric_limits<T>::max());
  // If both are movable.
  if (isMovable1 && isMovable2) {
    // Temporarily insert a point into the manifold to determine the 
    // new location.
    if (isEdgeFeature) {
      newPosition = manifold->insertOnAnEdge(-1, midPoint);
    }
    else {
      newPosition = manifold->insertOnASurface(-1, midPoint);
    }
    // Remove the temporary point.
    manifold->erase(-1);
  }
  // If only the first is not movable.
  else if (! isMovable1 && isMovable2) {
    // The new position is the same as the old position.
    newPosition = oldPosition1;
  }
  // If only the second is not movable.
  else if (isMovable1 && ! isMovable2) {
    // The location of the second node.
    newPosition = oldPosition2;
  }
  // We already covered the case of two corner features.
  else {
    assert(false);
  }

  // CONTINUE
  //std::cout << "newPosition = " << newPosition << "\n";

  // Move the source and target nodes to their positions after the 
  // potential edge collapse.
  c->getNode(i)->setVertex(newPosition);
  c->getNode(j)->setVertex(newPosition);

  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Check if collapsing the edge will cause to much damage to the mesh.
    // Compute the minimum quality of the incident cells that will remain 
    // after the collapse.
    const T newQuality = 
      computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
      <SMR, QualityMetric>(c, i, j);

    // If the quality is too low.
    if (newQuality < minimumAllowedQuality ||
	newQuality < qualityFactor * oldQuality) {
      // Move the nodes back to their old positions.
      c->getNode(i)->setVertex(oldPosition1);
      c->getNode(j)->setVertex(oldPosition2);
      // Do not collapse the edge.
      return false;
    }
  }

  //
  // From here, we will return true, which means we will collapse the edge.
  //

  if (isEdgeFeature) {
    // Erase the edge from the manifold.
    manifold->erase(identifier1, identifier2);
  }
    
  // If both are movable.
  if (isMovable1 && isMovable2) {
    //std::cout << "If both are movable.\n";
    // CONTINUE: This is not quite right.  I compute the new position above,
    // but I recompute it in a different fashion here.
    // Update the location of the merged point in the manifold.
    midPoint = manifold->changeLocation(identifier1, midPoint);
    // Set the location for the merged node.
    c->getNode(i)->setVertex(midPoint);
    // Remove the other node from the manifold.
    manifold->erase(identifier2);
  }
  // If only the first is not movable.
  else if (! isMovable1 && isMovable2) {
    //std::cout << "If only the first is not movable.\n";
    // No need to move the first node.
    // Remove the other node from the manifold.
    manifold->erase(identifier2);
  }
  // If only the second is not movable.
  else if (isMovable1 && ! isMovable2) {
    //std::cout << "If only the second is not movable.\n";
    // The source node has already been moved.
    // Erase the first node from the manifold.
    manifold->erase(identifier1);
    // Update the identifier of the merged point in the manifold.
    manifold->changeIdentifier(identifier2, identifier1);
  }
  // We already covered the case of two corner features.
  else {
    assert(false);
  }

  // Since we can collapse the edge, return true.
  /*
  std::cout << "Since we can collapse the edge, return true.\n"
	    << "node position = " << c->getNode(i)->getVertex() << "\n";
  */
  return true;
}






// If the edge may be collapsed: Set the node location for the source node.
// (The target node will be erased in merging the two.)  Update the boundary.
// Return true.
// If the edge may not be collapsed: return false.
template<typename SMR, class QualityMetric, int SD, typename T>
inline
bool
updateNodesAndManifoldForInteriorEdgeCollapse
(const typename SMR::CellIterator c, 
 const int i, const int j,
 const T minimumAllowedQuality, const T qualityFactor,
 PointsOnManifold<3,2,SD,T>* manifold) {
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBe3);

  // If the cell has no neighbors along the remaining faces, then we do 
  // not allow the edge to be collapsed.
  if (c->isFaceOnBoundary(i) && c->isFaceOnBoundary(j)) {
    return false;
  }
  // CONTINUE: I have to check this for each cell incident to the edge.
  // CONTINUE: I'm not even sure that the above test is correct.

  // If both nodes are on the boundary, then we do not allow the edge 
  // to be collapsed.
  const bool isOnBoundary1 = c->getNode(i)->isOnBoundary();
  const bool isOnBoundary2 = c->getNode(j)->isOnBoundary();
  if (isOnBoundary1 && isOnBoundary2) {
    return false;
  }

  //
  // Determine the new position for the merged vertex if we collapse the edge.
  //

  // The old positions for the source and target nodes.
  const Vertex oldPosition1(c->getNode(i)->getVertex());
  const Vertex oldPosition2(c->getNode(j)->getVertex());
  // Initialize with a bad value.
  Vertex newPosition(std::numeric_limits<T>::max());
  // If neither is on the boundary.
  if (! isOnBoundary1 && ! isOnBoundary2) {
    // The midpoint.
    newPosition = c->getNode(i)->getVertex();
    newPosition += c->getNode(j)->getVertex();
    newPosition *= 0.5;
  }
  // If only the first is on the boundary.
  else if (isOnBoundary1 && ! isOnBoundary2) {
    // The new position is the same as the old position.
    newPosition = oldPosition1;
  }
  // If only the second is on the boundary.
  else if (! isOnBoundary1 && isOnBoundary2) {
    // The location of the second node.
    newPosition = oldPosition2;
  }
  // We already covered the case of both nodes on the boundary.
  else {
    assert(false);
  }

  //
  // Check if collapsing the edge will cause to much damage to the mesh.
  //

  T oldQuality = 0;
  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells before the proposed 
    // edge collapse.
    oldQuality = computeMinimumQualityOfCellsIncidentToNodesOfEdge
      <SMR, QualityMetric>(c, i, j);
  }

  // Move the nodes to their positions after the potential edge collapse.
  c->getNode(i)->setVertex(newPosition);
  c->getNode(j)->setVertex(newPosition);

  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells that will remain 
    // after the collapse.
    const T newQuality = 
      computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
      <SMR, QualityMetric>(c, i, j);

    // If the quality is too low.
    if (newQuality < minimumAllowedQuality ||
	newQuality < qualityFactor * oldQuality) {
      // Move the nodes back to their old positions.
      c->getNode(i)->setVertex(oldPosition1);
      c->getNode(j)->setVertex(oldPosition2);
      // Do not collapse the edge.
      return false;
    }
  }

  // If only the second is on the boundary.
  if (! isOnBoundary1 && isOnBoundary2) {
    if (manifold != 0) {
      // Update the identifier of the merged point in the manifold.
      manifold->changeIdentifier(c->getNode(j)->getIdentifier(),
				 c->getNode(i)->getIdentifier());
    }
  }

  // Since we can collapse the edge, return true.
  return true;
}



// Update the nodes and manifold for the edge collapse.
// Return true if the edge can be collapsed.
template<typename SMR, class QualityMetric, int SD, typename T>
inline
bool
updateNodesAndManifoldForEdgeCollapse(const typename SMR::CellIterator c, 
				      const int i, const int j,
				      const T minimumAllowedQuality, 
				      const T qualityFactor,
				      PointsOnManifold<3,2,SD,T>* manifold) {
  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBe3);

  // CONTINUE
  //std::cout << "updateNodesAndManifoldForEdgeCollapse\n";

  // We cannot collapse the edge if the end points are mirror nodes.
  if (areMirrorNodes<SMR>(c->getNode(i), c->getNode(j))) {
    //std::cout << "  Mirror nodes.\n";
    return false;
  }  
  if (isOnBoundary<SMR>(c, i, j)) {
    //std::cout << "  Boundary edge.\n";
    return updateNodesAndManifoldForBoundaryEdgeCollapse<SMR, QualityMetric>
      (c, i, j, minimumAllowedQuality, qualityFactor, manifold);
  }
  //std::cout << "  Interior edge.\n";
  return updateNodesAndManifoldForInteriorEdgeCollapse<SMR, QualityMetric>
    (c, i, j, minimumAllowedQuality, qualityFactor, manifold);
}




// Get the next valid cell after collapsing the specified edge.
// The last two arguments give the range of cells incident to the edge.
template<typename SMR>
inline
typename SMR::CellIterator
getNextValidCellAfterCollapse
(const typename SMR::CellIterator c, 
 const int i, const int j) {
  LOKI_STATIC_CHECK(SMR::M == 3, TheSimplexDimensionMustBe3);
  
  typedef typename SMR::CellIterator CellIterator;

  // The cells incident to the edge.
  std::vector<CellIterator> incidentCells;
  determineCellsIncidentToEdge<SMR>(c, i, j, 
				    std::back_inserter(incidentCells));

  // The next valid cell.  (After collapsing the edge.)
  CellIterator nextValidCell = c;
  // Move forward from this cell.
  ++nextValidCell;
  // Move forward until the result is not incident to the edge being collapsed.
  while(std::count(incidentCells.begin(), incidentCells.end(), 
		   nextValidCell)) {
    ++nextValidCell;
  }

  return nextValidCell;
}



//! Collapse the edge.
/*!
  This removes the incident cells.  Return a cell iterator to the next 
  valid cell.

  \image html SimpMeshRed_2_collapse_interior.jpg "Collapse an interior edge."
  \image latex SimpMeshRed_2_collapse_interior.pdf "Collapse an interior edge."

  \return a cell iterator to the next valid cell.
*/
template<typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
collapse(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh,
	 typename SimpMeshRed<3,3,T,Node,Cell,Cont>::CellIterator c, 
	 const int i, const int j) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::CellIterator CellIterator;
  // CONTINUE: REMOVE once I have verified that I don't need this below.
  typedef typename SMR::NodeIteratorSet NodeIteratorSet;

  const int M = 3;

  //
  // Perform a couple calculations before we muck up the incidence/adjacency 
  // information.
  //

  // The source node.
  const NodeIterator a = c->getNode(i);
  // The target node.
  const NodeIterator b = c->getNode(j);

  // REMOVE
  /*
  std::cout << "\nSource:\n";
  std::cout << "-----------------------------------------------------------\n";
  a->put(std::cout);
  for (IncidentCellIterator ci = a->getCellsBeginning(); ci != a->getCellsEnd(); 
	++ci) {
    ci->put(std::cout);
  }
  std::cout << "Target:\n";
  std::cout << "-----------------------------------------------------------\n";
  b->put(std::cout);
  for (IncidentCellIterator ci = b->getCellsBeginning(); ci != b->getCellsEnd(); 
	++ci) {
    ci->put(std::cout);
  }
  */

  // The cells incident to the edge.
  std::vector<CellIterator> incidentCells;
  determineCellsIncidentToEdge<SMR>(c, i, j, 
				    std::back_inserter(incidentCells));

  // The nodes (other than the target node) that are incident to the cells.
  // (The source node and the target node will be merged, so we don't need
  // to include the latter.)
  // CONTINUE: REMOVE once I have verified that I don't need this below.
  NodeIteratorSet affectedNodes;
  affectedNodes.insert(a);
  {
    NodeIterator ni;
    for (typename std::vector<CellIterator>::const_iterator ci 
	   = incidentCells.begin(); ci != incidentCells.end(); ++ci) {
      for (int m = 0; m != M + 1; ++m) {
	ni = (*ci)->getNode(m);
	if (ni != a && ni != b) {
	  affectedNodes.insert(ni);
	}
      }
    }
  }

  //
  // Let the mucking commence.
  //

  // Collapse each cell.
  for (typename std::vector<CellIterator>::iterator cell = 
	 incidentCells.begin(); cell != incidentCells.end(); ++cell) {
    collapseCell(mesh, *cell, (*cell)->getIndex(a), (*cell)->getIndex(b));
  }

  // Merge the two nodes.
  mesh->merge(a, b);
  
  // REMOVE
  /*
  std::cout << "Merged:\n";
  std::cout << "-----------------------------------------------------------\n";
  a->put(std::cout);
  for (IncidentCellIterator ci = a->getCellsBeginning(); ci != a->getCellsEnd(); 
	++ci) {
    ci->put(std::cout);
  }
  if (is_oriented(*mesh)) {
    std::cout << "The mesh is oriented.\n";
  }
  else {
    std::cout << "The mesh is not oriented.\n";
  }
  */

  //
  // See if we have created any nodes with no incident simplices.
  // If so, remove those nodes.
  //
  // CONTINUE: REMOVE once I have verified that this cannot happen. 
  for (typename NodeIteratorSet::iterator nii = affectedNodes.begin();
	nii != affectedNodes.end(); ++nii) {
    if ((*nii)->getCellsSize() == 0) {
      assert(false);
    }
  }
}




//! Perform a coarsening sweep using the min edge length function.
/*!
  Return the number of edges collapsed.

  For the boundary edges:
  - If it is an edge feature:
    * If neither node is a corner feature, it may be collapsed to the midpoint.
    * If one node is a corner feature, it may be collapsed to the corner.
    * If both nodes are corner features, it may not be collapsed.
  - If it is not an edge feature.
    * If neither node is a corner feature nor has incident edge features, 
      it may be collapsed to the midpoint.
    * If one node is a corner feature or has incident edge features, 
      it may be collapsed to that node.
    * If both nodes are corner features or have incident edge features, 
      it may not be collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
inline
int
coarsenSweep(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
	     const MinEdgeLength& f,
	     const T minimumAllowedQuality, const T qualityFactor,
	     PointsOnManifold<3,2,SD,T>* manifold) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  // Initialize i and j with junk.
  int i = -1, j = -1;
  int count = 0;
  Vertex x;
  CellIterator nextValidCell;

  // Loop over the cells.
  CellIterator c = mesh->getCellsBeginning();
  while (c != mesh->getCellsEnd()) {
    c->getCentroid(&x);
    // The last condition prevents collapsing an interior edge with two
    // boundary nodes.  Doing this would change the topology of the mesh.
    if (computeMinimumEdgeLength<SMR>(c, &i, &j) < f(x) &&
	isCommonMinimumEdge<SMR>(c, i, j)) {
      // If the edge can be collapsed: Update one of the node locations in
      // preparation for the collapse and update the manifold.
      if (updateNodesAndManifoldForEdgeCollapse<SMR, QualityMetric>
	  (c, i, j, minimumAllowedQuality, qualityFactor, manifold)) {
	nextValidCell = getNextValidCellAfterCollapse<SMR>(c, i, j);
	// CONTINUE
	//std::cout << "Collapse the edge.\n";
	typename SMR::NodeIterator node = c->getNode(i);
	// Collapse the edge.
	collapse(mesh, c, i, j);
	/* CONTINUE
	T q = computeMinimumQualityOfCellsIncidentToNode<SMR, QualityMetric>
	  (node);
	if (q < minimumAllowedQuality) {
	  std::cout << "Warning: q = " << q << "\n";
	}
	*/
	// Move to the next valid cell.
	c = nextValidCell;
	// Increment the collapsed edge count.
	++count;
      }
      else {
	// Move to the next cell.
	++c;
      }
    }
    else {
      ++c;
    }
  }
  return count;
}




//! Perform a coarsening sweep on interior edges using the min edge length function.
/*!
  Return the number of edges collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
inline
int
coarsenInteriorSweep(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		     const MinEdgeLength& f,
		     const T minimumAllowedQuality, const T qualityFactor,
		     PointsOnManifold<3,2,SD,T>* manifold) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  // Initialize i and j with junk.
  int i = -1, j = -1;
  int count = 0;
  Vertex x;
  CellIterator nextValidCell;

  // Loop over the cells.
  CellIterator c = mesh->getCellsBeginning();
  while (c != mesh->getCellsEnd()) {
    c->getCentroid(&x);
    // The last condition prevents collapsing an interior edge with two
    // boundary nodes.  Doing this would change the topology of the mesh.
    if (computeMinimumEdgeLength<SMR>(c, &i, &j) < f(x) &&
	isCommonMinimumEdge<SMR>(c, i, j) &&
	! isOnBoundary<SMR>(c, i, j)) {
      // If the edge can be collapsed: Update one of the node locations in
      // preparation for the collapse and update the manifold.
      if (updateNodesAndManifoldForInteriorEdgeCollapse<SMR, QualityMetric>
	  (c, i, j, minimumAllowedQuality, qualityFactor, manifold)) {
	nextValidCell = getNextValidCellAfterCollapse<SMR>(c, i, j);
	// Collapse the ege.
	collapse(mesh, c, i, j);
	// Move to the next valid cell.
	c = nextValidCell;
	// Increment the collapsed edge count.
	++count;
      }
      else {
	// Move to the next cell.
	++c;
      }
    }
    else {
      ++c;
    }
  }
  return count;
}



//! Perform a coarsening sweep on boundary edges using the min edge length function.
/*!
  Return the number of edges collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
inline
int
coarsenBoundarySweep(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		     const MinEdgeLength& f,
		     const T minimumAllowedQuality, const T qualityFactor,
		     PointsOnManifold<3,2,SD,T>* manifold) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  int i, j;
  int count = 0;
  Vertex x;
  CellIterator nextValidCell;

  // Loop over the cells.
  CellIterator c = mesh->getCellsBeginning();
  while (c != mesh->getCellsEnd()) {
    c->getCentroid(&x);
    // The last condition prevents collapsing an interior edge with two
    // boundary nodes.  Doing this would change the topology of the mesh.
    if (computeMinimumEdgeLength<SMR>(c, &i, &j) < f(x) &&
	isCommonMinimumEdge<SMR>(c, i, j) &&
	isOnBoundary<SMR>(c, i, j)) {
      // If the edge can be collapsed: Update one of the node locations in
      // preparation for the collapse and update the manifold.
      if (updateNodesAndManifoldForBoundaryEdgeCollapse<SMR, QualityMetric>
	  (c, i, j, minimumAllowedQuality, qualityFactor, manifold)) {
	nextValidCell = getNextValidCellAfterCollapse<SMR>(c, i, j);
	// Collapse the ege.
	collapse(mesh, c, i, j);
	// Move to the next valid cell.
	c = nextValidCell;
	// Increment the collapsed edge count.
	++count;
      }
      else {
	// Move to the next cell.
	++c;
      }
    }
    else {
      ++c;
    }
  }
  return count;
}










// Coarsen the mesh using the min edge length function.
// Return the number of edges collapsed.
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
int
coarsen(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
	const MinEdgeLength& f,
	const T minimumAllowedQuality, const T qualityFactor,
	PointsOnManifold<3,2,SD,T>* manifold,
	const int maxSweeps) {
  int c; 
  int count = 0;
  int sweep = 0;
  do {
    c = coarsenSweep<QualityMetric>(mesh, f, minimumAllowedQuality, 
				    qualityFactor, manifold);
    count += c;
    ++sweep;
  } while (c != 0 && sweep != maxSweeps);
  return count;
}



// Coarsen the mesh using the min edge length function.
// Return the number of edges collapsed.
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class MinEdgeLength>
inline
int
coarsen(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, const MinEdgeLength& f,
	const T minimumAllowedQuality, const T qualityFactor,
	const T maxDihedralAngleDeviation,
	const T maxSolidAngleDeviation,
	const T maxBoundaryAngleDeviation,
	const int maxSweeps) {
  // If they specified angles for defining features.
  if (maxDihedralAngleDeviation >= 0 || maxSolidAngleDeviation >= 0 || 
      maxBoundaryAngleDeviation >= 0) {
    // CONTINUE: Fix this when I write a better manifold constructor.
    // Build a boundary manifold data structure.
    IndSimpSetIncAdj<3,3,true,T> iss;
    buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
    IndSimpSetIncAdj<3,2,true,T> boundary;
    buildBoundary(iss, &boundary);
    PointsOnManifold<3,2,1,T> manifold(boundary, 
				       maxDihedralAngleDeviation,
				       maxSolidAngleDeviation,
				       maxBoundaryAngleDeviation);
    manifold.insertBoundaryVerticesAndEdges(mesh);
  
    // Call the above coarsen function.
    return coarsen<QualityMetric>(mesh, f, minimumAllowedQuality, 
				  qualityFactor, &manifold, maxSweeps);
  }
  // Otherwise, don't use a manifold.
  // Call the above function.
  PointsOnManifold<3,2,1,T>* nullManifoldPointer = 0;
  return coarsen<QualityMetric>(mesh, f, minimumAllowedQuality, 
				qualityFactor, nullManifoldPointer, maxSweeps);
}






//! Coarsen the mesh using the min edge length function by collapsing interior edges.
/*!
  \return the number of edges collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
int
coarsenInterior(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		PointsOnManifold<3,2,SD,T>* manifold,
		int maxSweeps) {
  int c; 
  int count = 0;
  int sweep = 0;
  do {
    c = coarsenInteriorSweep<QualityMetric>(mesh, f, minimumAllowedQuality, 
					    qualityFactor, manifold);
    count += c;
    ++sweep;
  } while (c != 0 && sweep != maxSweeps);
  return count;
}



//! Coarsen the mesh using the min edge length function by collapsing interior edges.
/*!
  \return the number of edges collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class MinEdgeLength>
inline
int
coarsenInterior(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		const T edgeDeviation, const T cornerDeviation, 
		const int maxSweeps) {
  // CONTINUE: Fix this when I write a better manifold constructor.
  // Build a boundary manifold data structure.
  IndSimpSetIncAdj<3,3,true,T> iss;
  buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
  IndSimpSetIncAdj<3,2,true,T> boundary;
  buildBoundary(iss, &boundary);
  PointsOnManifold<3,2,1,T> manifold(boundary, edgeDeviation, cornerDeviation);
  manifold.insertBoundaryVerticesAndEdges(mesh);
  
  // Call the above coarsen function.
  return coarsenInterior<QualityMetric>(mesh, f, minimumAllowedQuality, 
					qualityFactor, &manifold, maxSweeps);
}






//! Coarsen the mesh using the min edge length function by collapsing boundary edges.
/*!
  \return the number of edges collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
int
coarsenBoundary(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		PointsOnManifold<3,2,SD,T>* manifold,
		int maxSweeps) {
  int c; 
  int count = 0;
  int sweep = 0;
  do {
    c = coarsenBoundarySweep<QualityMetric>(mesh, f, minimumAllowedQuality,
					    qualityFactor, manifold);
    count += c;
    ++sweep;
  } while (c != 0 && sweep != maxSweeps);
  return count;
}



//! Coarsen the mesh using the min edge length function by collapsing boundary edges.
/*!
  \return the number of edges collapsed.
*/
template<class QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class MinEdgeLength>
inline
int
coarsenBoundary(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		const T edgeDeviation, const T cornerDeviation, 
		const int maxSweeps) {
  // CONTINUE: Fix this when I write a better manifold constructor.
  // Build a boundary manifold data structure.
  IndSimpSetIncAdj<3,3,true,T> iss;
  buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
  IndSimpSetIncAdj<3,2,true,T> boundary;
  buildBoundary(iss, &boundary);
  PointsOnManifold<3,2,1,T> manifold(boundary, edgeDeviation, cornerDeviation);
  manifold.insertBoundaryVerticesAndEdges(mesh);
  
  // Call the above coarsen function.
  return coarsenBoundary<QualityMetric>(mesh, f, minimumAllowedQuality,
					qualityFactor, &manifold, maxSweeps);
}










//! Find the minimum collapseable edge.
/*!
  If there is no collapseable edge, return false.

  An edge is not collapseable if collapsing it would change the topology 
  of the mesh.
*/
template<class SMR>
inline
bool
computeMinimumCollapseableEdge(const typename SMR::CellConstIterator c,
		    int* i, int* j) {
  LOKI_STATIC_CHECK(SMR::M == 3, TheDimensionMustBe3);
  typedef typename SMR::Number Number;

  // The Number of Edges.
  const int NE = 6;

  // The source and target node indices.
  const int source[NE] = {0, 0, 0, 1, 1, 2};
  const int target[NE] = {1, 2, 3, 2, 3, 3};

  // The edge lengths.
  ads::FixedArray<NE,Number> lengths;

  // Pointers to the edge lengths.
  ads::FixedArray<NE,const Number*> ptrs;
  for (int n = 0; n != ptrs.size(); ++n) {
    ptrs[n] = &lengths[n];
  }
  
  for (int n = 0; n != lengths.size(); ++n) {
    lengths[n] = geom::computeDistance(c->getNode(source[n])->getVertex(),
				       c->getNode(target[n])->getVertex());
  }
  // Sort the length pointers.
  {
    ads::binary_compose_binary_unary<std::less<Number>,
      ads::Dereference<const Number*>,
      ads::Dereference<const Number*> > comp;
    std::sort(ptrs.begin(), ptrs.end(), comp);
  }

  ads::FixedArray<NE,int> sorted;
  for (int n = 0; n != sorted.size(); ++n) {
    sorted[n] = int(ptrs[n] - &lengths[0]);
  }

  // For each edge in sorted order.
  for (int n = 0; n != sorted.size(); ++n) {
    *i = source[sorted[n]];
    *j = target[sorted[n]];
    // If the edge is collapseable.
    if (! areMirrorNodes<SMR>(c->getNode(*i), c->getNode(*j)) &&
	isOnBoundary<SMR>(c, *i, *j) || 
	edgeHasAnInteriorNode<SMR>(c, *i, *j)) {
      return true;
    }
  }
  return false;
}



// CONTINUE
//! Collapse edges to remove the specified cells.
/*!
  \param mesh The simplicial mesh.

  \return the number of edges collapsed.
*/
template<typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename IntInIter>
inline
int
coarsen(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
	IntInIter begin, IntInIter end) {
  typedef SimpMeshRed<3,3,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::CellIteratorSet CellIteratorSet;
  typedef typename SMR::Node::CellIncidentToNodeIterator IncidentCellIterator;

  // Make a set of the cell iterators.
  CellIteratorSet cells;
  convertIdentifiersToIterators(*mesh, begin, end, &cells);
  
  int count = 0;
  int i, j;
  CellIterator c;
  NodeIterator a, b;
  IncidentCellIterator ci;
  // Loop until all the specified cell have been collapsed.
  while (! cells.empty()) {
    c = *cells.begin();
    // If there is a collapseable edge.
    // Find the minimum length collapseable edge.
    if (computeMinimumCollapseableEdge<SMR>(c, &i, &j)) {

      //
      // Erase the cells incident to the edge from the set.
      //
      // The source node.
      a = c->getNode(i);
      // The target node.
      b = c->getNode(j);
      // For each cell incident to a.
      for (ci = a->getCellsBeginning(); ci != a->getCellsEnd(); ++ci) {
	// If the cell is incident to b as well,
	// then it is incident to the edge.
	if (ci->hasNode(b)) {
	  cells.erase(*ci.base());
	}
      }

      // Collapse the edge.  (Ignore the return value.)
      collapse(mesh, c, i, j);
      ++count;
    }
    // If we cannot remove the cell by collapsing an edge.
    else {
      cells.erase(cells.begin());
    }
  }
  // Return the number of edges collapsed.
  return count;
}


END_NAMESPACE_GEOM

// End of file.
