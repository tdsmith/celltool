// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_coarsen2_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Return the minimum edge length.
/*!
  Set \c i to the minimum edge index.
*/
template<class SMR>
inline
typename SMR::Number
computeMinimumEdgeLength(const typename SMR::CellConstIterator c, int* i) {
  typedef typename SMR::Number Number;

  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  int a = 0, b = 0;
  Number d = c->computeMinimumEdgeLength(&a, &b);
  *i = 0;
  if (*i == a) {
    ++*i;
  }
  if (*i == b) {
    ++*i;
  }
  // CONTINUE
#if 0
  std::cerr << "computeMinimumEdgeLength, id = " 
	    << c->getNode(*i)->getIdentifier() 
	    << ",  d = " << d << "\n";
#endif
  return d;
}


//! Return the minimum edge length.
template<class SMR>
inline
typename SMR::Number
computeMinimumEdgeLength(const typename SMR::CellConstIterator c) {
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  int a, b;
  return c->computeMinimumEdgeLength(&a, &b);
}


// CONTINUE: Get rid of this function.
#if 0
//! Return true if the edge is on the boundary.
/*!
  \c i is the index of the edge.
*/
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::CellConstIterator c, const int i) {
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  return c->getNeighbor(i) == 0;
}
#endif


//! Return true if the edge has one or more interior nodes.
/*!
  \c i is the index of the edge.
*/
template<class SMR>
inline
bool
doesEdgeHaveAnInteriorNode(const typename SMR::CellConstIterator c, 
			   const int i) {
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  return ! c->getNode((i+1)%3)->isOnBoundary() || 
    ! c->getNode((i+2)%3)->isOnBoundary();
}



//! Return the minimum edge index.
template<class SMR>
inline
int
computeMinimumEdgeIndex(const typename SMR::CellConstIterator c) {
  typedef typename SMR::Number Number;

  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  int a = 0, b = 0, i = 0;
  c->computeMinimuumEdgeLength(&a, &b);
  if (i == a) {
    ++i;
  }
  if (i == b) {
    ++i;
  }
  return i;
}


//! Return true if this is a common minimum edge or a boundary edge.
template<class SMR>
inline
bool
isCommonMinimumEdge(const typename SMR::CellConstIterator c, const int i) {
  typedef typename SMR::Number Number;

  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  // CONTINUE
#if 0
  bool a = (c->getNeighbor(i) == 0);
  bool b = false;
  if (!a) {
    b = (computeMinimumEdgeLength<SMR>(c->getNeighbor(i)) >=
	 geom::computeDistance(c->getNode((i+1)%3)->getVertex(), 
			       c->getNode((i+2)%3)->getVertex()) *
	 (1.0 - 10.0 * std::numeric_limits<Number>::epsilon()));
  }
  std::cerr << "isCommonMinimumEdge, id = " 
	    << c->getNode(i)->getIdentifier() 
	    << "  " << computeMinimumEdgeLength<SMR>(c->getNeighbor(i))
	    << "  " << geom::computeDistance(c->getNode((i+1)%3)->getVertex(), 
					     c->getNode((i+2)%3)->getVertex()) *
    (1.0 - 10.0 * std::numeric_limits<Number>::epsilon())
	    << "  " << a
	    << "  " << b
	    << "\n";
#endif

  return c->isFaceOnBoundary(i) || 
    computeMinimumEdgeLength<SMR>(c->getNeighbor(i)) >=
    geom::computeDistance(c->getNode((i+1)%3)->getVertex(), 
			  c->getNode((i+2)%3)->getVertex()) *
    (1.0 - 10.0 * std::numeric_limits<Number>::epsilon());
}



template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
collapseCell(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh,
	     const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator c, 
	     const int i) {
  typedef SimpMeshRed<N,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;

  const int M = 2;

#if 0
  // CONTINUE
  std::cerr << "collapseCell() " << c->getIdentifier() << " " << i << "\n";
#endif

  // The other two node indices.
  const int i1 = (i + 1) % (M+1);
  const int i2 = (i + 2) % (M+1);
  // The two neigbors that aren't being collapsed.
  const CellIterator n1 = c->getNeighbor(i1);
  const CellIterator n2 = c->getNeighbor(i2);
  const int k1 = c->getMirrorIndex(i1);
  const int k2 = c->getMirrorIndex(i2);
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





// If the edge may be collapsed: Set the node location for the source node.
// (The target node will be erased in merging the two.)  Update the boundary.
// Return true.
// If the edge may not be collapsed: return false.
template<typename SMR, class QualityMetric, int N, int SD, typename T>
inline
bool
updateNodesAndManifoldForBoundaryEdgeCollapse
(const typename SMR::CellIterator c, 
 const int i,
 const T minimumAllowedQuality, const T qualityFactor,
 PointsOnManifold<N,N-1,SD,T>* manifold) {
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::N == 2 || SMR::N == 3, TheSpaceDimensionMustBe2Or3);
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);
  const int M = SMR::M;

  // If the cell has no neighbors, then we do not allow the edge to 
  // be collapsed.
  if (c->getNumberOfNeighbors() == 0) {
    return false;
  }

  // The other two node indices.
  const int i1 = (i + 1) % (M + 1);
  const int i2 = (i + 2) % (M + 1);

  // If there are two nodes in common between the links of the two endpoint 
  // nodes, then collapsing the edge, would collapse a triangular cavity.  This
  // would change the topology of the mesh and is not allowed.
  {
    typename SMR::NodeIteratorSet a, b;
    std::vector<typename SMR::NodeIterator> intersection;
    determineNodesInLink<SMR>(c->getNode(i1), std::inserter(a, a.begin()));
    determineNodesInLink<SMR>(c->getNode(i2), std::inserter(b, b.begin()));
    typename SMR::NodeIteratorSet::value_compare compare;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
			  std::back_inserter(intersection),
			  compare);
    if (intersection.size() == 2) {
      return false;
    }
    // The common node is from the cell incident to the edge being collapsed.
    assert(intersection.size() == 1);
  }

  // The old position for the source vertex.
  const Vertex oldPosition1(c->getNode(i1)->getVertex());
  // The old position for the target vertex.
  const Vertex oldPosition2(c->getNode(i2)->getVertex());

  Vertex midPoint = oldPosition1;
  midPoint += oldPosition2;
  midPoint *= 0.5;

  T oldQuality = 0;
  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells before the proposed 
    // edge collapse.
    oldQuality = computeMinimumQualityOfCellsIncidentToNodesOfEdge
      <SMR, QualityMetric>(c, i1, i2);
  }

  // If a manifold is not specified, move the source node to the midpoint 
  // of the edge.
  if (manifold == 0) {
    // Set the locations for the merged node.
    c->getNode(i1)->setVertex(midPoint);
    c->getNode(i2)->setVertex(midPoint);
    // If they specified a minimum quality or a quality factor.
    if (minimumAllowedQuality > 0 || qualityFactor > 0) {
      // Check if collapsing the edge will cause to much damage to the mesh.
      // Compute the minimum quality of the incident cells that will remain 
      // after the collapse.
      const T newQuality = 
	computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
	<SMR, QualityMetric>(c, i1, i2);
      // If the quality is too low.
      if (newQuality < minimumAllowedQuality ||
	  newQuality < qualityFactor * oldQuality) {
	// Move the nodes back to their old positions.
	c->getNode(i1)->setVertex(oldPosition1);
	c->getNode(i2)->setVertex(oldPosition2);
	// Do not collapse the edge.
	return false;
      }
    }
    // Otherwise, we may collapse the edge.
    return true;
  }

  const int identifier1 = c->getNode(i1)->getIdentifier();
  const int identifier2 = c->getNode(i2)->getIdentifier();

  // If the source is a corner feature.
  const bool isCorner1 = manifold->isOnCorner(identifier1);
  // If the target is a corner feature.
  const bool isCorner2 = manifold->isOnCorner(identifier2);

  // If both are corner features, we cannot collapse the edge.
  if (isCorner1 && isCorner2) {
    return false;
  }

  //
  // Determine the new position for the merged vertex if we collapse the edge.
  //

  // Initialize with a bad value.
  Vertex newPosition(std::numeric_limits<T>::max());
  // If neither is a corner feature.
  if (! isCorner1 && ! isCorner2) {
    // Temporarily insert a point into the manifold to determine the 
    // new location.
    newPosition = manifold->insertOnASurface(-1, midPoint);
    // Remove the temporary point.
    manifold->erase(-1);
  }
  // If only the first is a corner feature.
  else if (isCorner1 && ! isCorner2) {
    // The new position is the same as the old position.
    newPosition = oldPosition1;
  }
  // If only the second is a corner feature.
  else if (! isCorner1 && isCorner2) {
    // The location of the second node.
    newPosition = oldPosition2;
  }
  // We already covered the case of two corner features.
  else {
    assert(false);
  }

  //
  // Check if collapsing the edge will cause to much damage to the mesh.
  //

  // Move the source and target nodes to their positions after the 
  // potential edge collapse.
  c->getNode(i1)->setVertex(newPosition);
  c->getNode(i2)->setVertex(newPosition);

  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells that will remain 
    // after the collapse.
    const T newQuality = 
      computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
      <SMR, QualityMetric>(c, i1, i2);

    // If the quality is too low.
    if (newQuality < minimumAllowedQuality ||
	newQuality < qualityFactor * oldQuality) {
      // Move the nodes back to their old positions.
      c->getNode(i1)->setVertex(oldPosition1);
      c->getNode(i2)->setVertex(oldPosition2);
      // Do not collapse the edge.
      return false;
    }
  }

  //
  // From here, we will return true, which means we will collapse the edge.
  //

  // If neither is a corner feature.
  if (! isCorner1 && ! isCorner2) {
    // CONTINUE: This is not quite right.  I compute the new position above,
    // but I recompute it in a different fashion here.
    // Update the location of the merged point in the manifold.
    midPoint = manifold->changeLocation(identifier1, midPoint);
    // Set the location for the merged node.
    c->getNode(i1)->setVertex(midPoint);
    // Remove the other node from the manifold.
    manifold->erase(identifier2);
  }
  // If only the first is a corner feature.
  else if (isCorner1 && ! isCorner2) {
    // No need to move the first node.
    // Remove the other node from the manifold.
    manifold->erase(identifier2);
  }
  // If only the second is a corner feature.
  else if (! isCorner1 && isCorner2) {
    // The source node has already been moved.
    // Erase the first node from the manifold.
    manifold->erase(identifier1);
    // Update the location of the merged point in the manifold.
    manifold->changeIdentifier(identifier2, identifier1);
  }
  // We already covered the case of two corner features.
  else {
    assert(false);
  }

  // Since we can collapse the edge, return true.
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
 const int i,
 const T minimumAllowedQuality, const T qualityFactor,
 PointsOnManifold<2,1,SD,T>* manifold) {
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);
  const int M = SMR::M;

#if 0
  // CONTINUE
  std::cerr << "updateNodesAndManifoldForInteriorEdgeCollapse()\n";
#endif

  // The other two node indices.
  const int i1 = (i + 1) % (M + 1);
  const int i2 = (i + 2) % (M + 1);

#if 0
  // CONTINUE
  std::cerr << i << " " << i1 << " " << i2 << "\n";
#endif

  // CONTINUE
#if 0
  std::cerr << "id1 = " << c->getNode(i1)->getIdentifier()
	    << ",  id2 = " << c->getNode(i2)->getIdentifier() << "\n";
#endif

  // If this cell has no neighbors along the remaining faces, then we do 
  // not allow the edge to be collapsed.
  if (c->isFaceOnBoundary(i1) && c->isFaceOnBoundary(i2)) {
    return false;
  }
  // Check the same thing for the adjacent cell.
  {
    typename SMR::CellIterator d = c->getNeighbor(i);
    assert(d != typename SMR::CellIterator(0));
    const int j = c->getMirrorIndex(i);
    const int j1 = (j + 1) % (M + 1);
    const int j2 = (j + 2) % (M + 1);
    if (d->isFaceOnBoundary(j1) && d->isFaceOnBoundary(j2)) {
      return false;
    }
  }

  // If both nodes are on the boundary, then we do not allow the edge 
  // to be collapsed.
  const bool isOnBoundary1 = c->getNode(i1)->isOnBoundary();
  const bool isOnBoundary2 = c->getNode(i2)->isOnBoundary();
  if (isOnBoundary1 && isOnBoundary2) {
    return false;
  }

  //
  // Determine the new position for the merged vertex if we collapse the edge.
  //

  // The old positions for the source and target nodes.
  const Vertex oldPosition1(c->getNode(i1)->getVertex());
  const Vertex oldPosition2(c->getNode(i2)->getVertex());
  // CONTINUE
#if 0
  std::cerr << "oldPosition1 = " << oldPosition1
	    << ",  oldPosition2 = " << oldPosition2 << "\n";
#endif
  // Initialize with a bad value.
  Vertex newPosition(std::numeric_limits<T>::max());
  // If neither is on the boundary.
  if (! isOnBoundary1 && ! isOnBoundary2) {
    // The midpoint.
    newPosition = c->getNode(i1)->getVertex();
    newPosition += c->getNode(i2)->getVertex();
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

  // Compute the minimum quality of the incident cells before the proposed edge
  // collapse.
  T oldQuality = 0;
  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    oldQuality = computeMinimumQualityOfCellsIncidentToNodesOfEdge
      <SMR, QualityMetric>(c, i1, i2);
  }

  // Move the nodes to their positions after the potential edge collapse.
  c->getNode(i1)->setVertex(newPosition);
  c->getNode(i2)->setVertex(newPosition);

  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells that will remain 
    // after the collapse.
    const T newQuality = 
      computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
      <SMR, QualityMetric>(c, i1, i2);

    // If the quality is too low.
    if (newQuality < minimumAllowedQuality ||
	newQuality < qualityFactor * oldQuality) {
      // Move the nodes back to their old positions.
      c->getNode(i1)->setVertex(oldPosition1);
      c->getNode(i2)->setVertex(oldPosition2);
      // Do not collapse the edge.
      return false;
    }
  }

  // If only the second is on the boundary.
  if (! isOnBoundary1 && isOnBoundary2) {
    if (manifold != 0) {
      // Update the identifier of the merged point in the manifold.
      manifold->changeIdentifier(c->getNode(i2)->getIdentifier(),
				 c->getNode(i1)->getIdentifier());
    }
  }

  // Since we can collapse the edge, return true.
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
(const typename SMR::CellIterator c, const int i,
 const T minimumAllowedQuality, const T qualityFactor,
 PointsOnManifold<3,2,SD,T>* manifold) {
  typedef typename SMR::Vertex Vertex;

  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);

  const int M = SMR::M;

  // The other two node indices.
  const int i1 = (i + 1) % (M + 1);
  const int i2 = (i + 2) % (M + 1);

  // If the cell has no neighbors along the remaining faces, then we do 
  // not allow the edge to be collapsed.
  if (c->isFaceOnBoundary(i1) && c->isFaceOnBoundary(i2)) {
    return false;
  }
  // Check the same thing for the adjacent cell.
  {
    typename SMR::CellIterator d = c->getNeighbor(i);
    assert(d != typename SMR::CellIterator(0));
    const int j = c->getMirrorIndex(i);
    const int j1 = (j + 1) % (M + 1);
    const int j2 = (j + 2) % (M + 1);
    if (d->isFaceOnBoundary(j1) && d->isFaceOnBoundary(j2)) {
      return false;
    }
  }

  // Because this in an interior node in a 3-2 mesh, there are at least 
  // two nodes in common between the links of the two endpoint 
  // nodes.  Ordinarily there are exactly two nodes in common.  If there are
  // more, then collapsing the edge would pinch off the volume inside the
  // mesh and change the topology of the surface mesh.  Therefore this is not 
  // allowed.
  {
    typename SMR::NodeIteratorSet a, b;
    std::vector<typename SMR::NodeIterator> intersection;
    determineNodesInLink<SMR>(c->getNode(i1), std::inserter(a, a.begin()));
    determineNodesInLink<SMR>(c->getNode(i2), std::inserter(b, b.begin()));
    typename SMR::NodeIteratorSet::value_compare compare;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
			  std::back_inserter(intersection),
			  compare);
    assert(intersection.size() >= 2);
    if (intersection.size() != 2) {
      return false;
    }
  }

  // The old position for the source vertex.
  const Vertex oldPosition1(c->getNode(i1)->getVertex());
  // The old position for the target vertex.
  const Vertex oldPosition2(c->getNode(i2)->getVertex());

  // The midpoint.
  Vertex midPoint = c->getNode(i1)->getVertex();
  midPoint += c->getNode(i2)->getVertex();
  midPoint *= 0.5;

  T oldQuality = 0;
  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Compute the minimum quality of the incident cells before the proposed 
    // edge collapse.
    oldQuality = computeMinimumQualityOfCellsIncidentToNodesOfEdge
      <SMR, QualityMetric>(c, i1, i2);
  }

  // If a manifold is not specified, try moving the source node to the 
  // midpoint of the edge.
  if (manifold == 0) {
    // Set the locations for the merged node.
    c->getNode(i1)->setVertex(midPoint);
    c->getNode(i2)->setVertex(midPoint);

    // If they specified a minimum quality or a quality factor.
    if (minimumAllowedQuality > 0 || qualityFactor > 0) {
      // Check if collapsing the edge will cause to much damage to the mesh.
      // Compute the minimum quality of the incident cells that will remain 
      // after the collapse.
      const T newQuality = 
	computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
	<SMR, QualityMetric>(c, i1, i2);
      // If the quality is too low.
      if (newQuality < minimumAllowedQuality ||
	  newQuality < qualityFactor * oldQuality) {
	// Move the nodes back to their old positions.
	c->getNode(i1)->setVertex(oldPosition1);
	c->getNode(i2)->setVertex(oldPosition2);
	// Do not collapse the edge.
	return false;
      }
    }
    // Otherwise, we may collapse the edge.
    return true;
  }

  const int identifier1 = c->getNode(i1)->getIdentifier();
  const int identifier2 = c->getNode(i2)->getIdentifier();

  // If the source is a corner feature.
  const bool isCorner1 = manifold->isOnCorner(identifier1);
  // If the target is a corner feature.
  const bool isCorner2 = manifold->isOnCorner(identifier2);

  // If both are corner features, we cannot collapse the edge.
  if (isCorner1 && isCorner2) {
    return false;
  }

  const bool isEdgeFeature = manifold->hasEdge(identifier1, identifier2);
  const bool isSurface1 = manifold->isOnSurface(identifier1);
  const bool isSurface2 = manifold->isOnSurface(identifier2);

  // In this case, we cannot move either end point, so we cannot collapse the
  // edge.
  if (! isEdgeFeature && ! isSurface1 && ! isSurface2) {
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


  // Move the source and target nodes to their positions after the 
  // potential edge collapse.
  c->getNode(i1)->setVertex(newPosition);
  c->getNode(i2)->setVertex(newPosition);

  // If they specified a minimum quality or a quality factor.
  if (minimumAllowedQuality > 0 || qualityFactor > 0) {
    // Check if collapsing the edge will cause to much damage to the mesh.
    // Compute the minimum quality of the incident cells that will remain 
    // after the collapse.
    const T newQuality = 
      computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
      <SMR, QualityMetric>(c, i1, i2);

    // If the quality is too low.
    if (newQuality < minimumAllowedQuality ||
	newQuality < qualityFactor * oldQuality) {
      // Move the nodes back to their old positions.
      c->getNode(i1)->setVertex(oldPosition1);
      c->getNode(i2)->setVertex(oldPosition2);
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
    // CONTINUE: This is not quite right.  I compute the new position above,
    // but I recompute it in a different fashion here.
    // Update the location of the merged point in the manifold.
    midPoint = manifold->changeLocation(identifier1, midPoint);
    // Set the location for the merged node.
    c->getNode(i1)->setVertex(midPoint);
    // Remove the other node from the manifold.
    manifold->erase(identifier2);
  }
  // If only the first is not movable.
  else if (! isMovable1 && isMovable2) {
    // No need to move the first node.
    // Remove the other node from the manifold.
    manifold->erase(identifier2);
  }
  // If only the second is not movable.
  else if (isMovable1 && ! isMovable2) {
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
  return true;
}





// Update the nodes and manifold for the edge collapse.
// Return true if the edge can be collapsed.
template<typename SMR, class QualityMetric, int SD, int N, typename T>
inline
bool
updateNodesAndManifoldForEdgeCollapse(const typename SMR::CellIterator c, 
				      const int i,
				      const T minimumAllowedQuality, 
				      const T qualityFactor,
				      PointsOnManifold<N,N-1,SD,T>* manifold) {
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);
  LOKI_STATIC_CHECK(N == 2 || N == 3, BadSpaceDimension);

  const int M = SMR::M;

  // We cannot collapse the edge if the end points are mirror nodes.
  if (areMirrorNodes<SMR>(c->getNode((i + 1) % (M + 1)), 
			  c->getNode((i + 2) % (M + 1)))) {
    // CONTINUE
    //std::cerr << "Mirror.\n";
    return false;
  }
  if (c->isFaceOnBoundary(i)) {
    // CONTINUE
    //std::cerr << "Boundary.\n";
    return updateNodesAndManifoldForBoundaryEdgeCollapse<SMR, QualityMetric>
      (c, i, minimumAllowedQuality, qualityFactor, manifold);
  }
  // CONTINUE
  //std::cerr << "Interior.\n";
  return updateNodesAndManifoldForInteriorEdgeCollapse<SMR, QualityMetric>
    (c, i, minimumAllowedQuality, qualityFactor, manifold);
}



// Get the next valid cell after collapsing the specified edge.
template<class SMR>
inline
typename SMR::CellIterator
getNextValidCellAfterCollapse(const typename SMR::CellIterator c, 
			      const int i) {
  LOKI_STATIC_CHECK(SMR::M == 2, TheSimplexDimensionMustBe2);
  
  typedef typename SMR::CellIterator CellIterator;

  // The neighboring cell.
  const CellIterator b = c->getNeighbor(i);

  // The next valid cell.  (After collapsing the edge.)
  CellIterator nextValidCell = c;
  ++nextValidCell;
  if (b != CellIterator(0) && nextValidCell == b) {
    ++nextValidCell;
  }
  return nextValidCell;
}



//! Collapse the edge.
/*!
  This removes the incident cells.  The edge must be collapsable.

  \image html SimpMeshRed_2_collapse_interior.jpg "Collapse an interior edge."
  \image latex SimpMeshRed_2_collapse_interior.pdf "Collapse an interior edge."
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
collapse(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh,
	 typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator a, 
	 const int i) {
  typedef SimpMeshRed<N,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::NodeIterator NodeIterator;
  typedef typename SMR::CellIterator CellIterator;

  const int M = 2;

#if 0
  // CONTINUE
  std::cerr << "begin collapse.\n";
  std::cerr << "identifier = " << a->getIdentifier() << ", i = " << i << "\n";
#endif

  //
  // Perform a couple calculations before we muck up the incidence/adjacency 
  // information.
  //

  // The neighboring cell.
  const CellIterator b = a->getNeighbor(i);
  const int j = (b != CellIterator(0)) ? a->getMirrorIndex(i) : 0;

#if 0
  // CONTINUE
  if (b != 0) {
    std::cerr << "b = " << b->getIdentifier() << ", j = " << j << "\n";
  }
  else {
    std::cerr << "b == 0\n";
  }
#endif

  const NodeIterator topNode = a->getNode(i);
  const NodeIterator bottomNode = (b != CellIterator(0)) ? b->getNode(j) : 
    NodeIterator(0);

  // The other two nodes.
  const NodeIterator x = a->getNode((i + 1) % (M+1));
  const NodeIterator y = a->getNode((i + 2) % (M+1));

#if 0
  // CONTINUE
  std::cerr << "x->id = " << x->getIdentifier() 
	    << ", y->id = " << y->getIdentifier() << "\n"
	    << "x->v = " << x->getVertex() 
	    << ", y->v = " << y->getVertex() << "\n";
#endif

  //
  // Let the mucking commence.
  //

  // Collapse the first cell.
  collapseCell(mesh, a, i);

  // Collapse the second cell.
  if (b != CellIterator(0)) {
    collapseCell(mesh, b, j);
  }

  // Merge the two nodes.
  mesh->merge(x, y);
  
#if 0
  // CONTINUE
  std::cerr << "x->id = " << x->getIdentifier() << "\n"
	    << "x->v = " << x->getVertex() << "\n";
  renumberIdentifiers(mesh);
  writeAscii(std::cerr, *mesh);
#endif

  //
  // See if we have created any nodes with no incident simplices.
  // If so, remove those nodes.
  //

  // CONTINUE: Verify and clean up.
#if 0
  if (topNode->getCellsSize() == 0) {
    mesh->eraseNode(topNode);
  }
  if (bottomNode != 0 && bottomNode->getCellsSize() == 0) {
    mesh->eraseNode(bottomNode);
  }
#endif
  // CONTINUE
  //std::cerr << "topNode->getCellsSize() != 0.\n";
  assert(topNode->getCellsSize() != 0);
  // CONTINUE
  //std::cerr << "! (bottomNode != 0 && bottomNode->getCellsSize() == 0).\n";
  assert(! (bottomNode != NodeIterator(0) && bottomNode->getCellsSize() == 0));
  // CONTINUE
  //std::cerr << "x->getCellsSize() != 0.\n";
  //std::cerr << "size = " << x->getCellsSize() << "\n";
  assert(x->getCellsSize() != 0);
  // CONTINUE
  //std::cerr << "end collapse.\n";
}





//! Perform a coarsening sweep using the min edge length function.
/*!
  Return the number of edges collapsed.

  This function can be used for 2-2 or 3-2 meshes.
*/
template<class QualityMetric,
	 int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
inline
int
coarsenSweep(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, 
	     const MinEdgeLength& f,
	     const T minimumAllowedQuality, const T qualityFactor,
	     PointsOnManifold<N,N-1,SD,T>* manifold) {
  LOKI_STATIC_CHECK(N == 2 || N == 3, BadSpaceDimension);

  typedef SimpMeshRed<N,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  int i;
  int count = 0;
  Vertex x;
  CellIterator nextValidCell;

#if 0
  // CONTINUE
  static int fileNumber = 0;
  // CONTINUE
  std::cerr << "coarsenSweep\n";
#endif

  // Loop over the cells.
  CellIterator c = mesh->getCellsBeginning();
  while (c != mesh->getCellsEnd()) {
    c->getCentroid(&x);

#if 0
    // CONTINUE
    std::cerr << "x = " << x << "\n";
#endif

    // The last condition prevents collapsing an interior edge with two
    // boundary nodes.  Doing this would change the topology of the mesh.
    if (computeMinimumEdgeLength<SMR>(c, &i) < f(x) &&
	isCommonMinimumEdge<SMR>(c, i)) {

#if 0
      // CONTINUE
      std::cerr << "Passed first test.\n";
      // CONTINUE
      {
	std::ostringstream fileName;
	fileName << "mesh." << fileNumber << ".txt";
	std::ofstream out(fileName.str().c_str());
	writeAscii(out, *mesh);
      }
#endif

#if 0
      // CONTINUE
      writeAscii(std::cerr, *mesh);
      print(std::cerr, *mesh);
#endif

      // If the edge can be collapsed: Update one of the node locations in
      // preparation for the collapse and update the manifold.
      if (updateNodesAndManifoldForEdgeCollapse<SMR, QualityMetric>
	  (c, i, minimumAllowedQuality, qualityFactor, manifold)) {

#if 0
	// CONTINUE
	std::cerr << "Passed second test.\n";
#endif

	nextValidCell = getNextValidCellAfterCollapse<SMR>(c, i);

#if 0
	// CONTINUE
	{
	  std::ostringstream fileName;
	  fileName << "before." << fileNumber << ".txt";
	  std::ofstream out(fileName.str().c_str());
	  print(out, *mesh);
	}
	assert(isValid(*mesh));
#endif

	// Collapse the ege.
	collapse(mesh, c, i);

#if 0
	// CONTINUE
	renumberIdentifiers(mesh);
	{
	  std::ostringstream fileName;
	  fileName << "after." << fileNumber << ".txt";
	  std::ofstream out(fileName.str().c_str());
	  print(out, *mesh);
	}
	assert(isValid(*mesh));
	++fileNumber;
#endif

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





//! Perform a coarsening sweep using the min edge length function.
/*!
  Return the number of edges collapsed.
*/
template<class QualityMetric,
	 int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
inline
int
coarsenInteriorSweep(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, 
		     const MinEdgeLength& f,
		     const T minimumAllowedQuality, const T qualityFactor,
		     PointsOnManifold<2,1,SD,T>* manifold) {
  typedef SimpMeshRed<2,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  int i;
  int count = 0;
  Vertex x;
  CellIterator nextValidCell;

  // Loop over the cells.
  CellIterator c = mesh->getCellsBeginning();
  while (c != mesh->getCellsEnd()) {
    c->getCentroid(&x);
    // The last condition prevents collapsing an interior edge with two
    // boundary nodes.  Doing this would change the topology of the mesh.
    if (computeMinimumEdgeLength<SMR>(c, &i) < f(x) &&
	isCommonMinimumEdge<SMR>(c, i) &&
	! c->isFaceOnBoundary(i)) {
      // If the edge can be collapsed: Update one of the node locations in
      // preparation for the collapse and update the manifold.
      if (updateNodesAndManifoldForInteriorEdgeCollapse<SMR, QualityMetric>
	  (c, i, minimumAllowedQuality, qualityFactor, manifold)) {
	nextValidCell = getNextValidCellAfterCollapse<SMR>(c, i);
	// Collapse the ege.
	collapse(mesh, c, i);
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



//! Perform a coarsening sweep using the min edge length function.
/*!
  Return the number of edges collapsed.
*/
template<class QualityMetric,
	 int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD,
	 class MinEdgeLength>
inline
int
coarsenBoundarySweep(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, 
		     const MinEdgeLength& f,
		     const T minimumAllowedQuality, const T qualityFactor,
		     PointsOnManifold<2,1,SD,T>* manifold) {
  typedef SimpMeshRed<2,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::Vertex Vertex;
  typedef typename SMR::CellIterator CellIterator;

  int i;
  int count = 0;
  Vertex x;
  CellIterator nextValidCell;

  // Loop over the cells.
  CellIterator c = mesh->getCellsBeginning();
  while (c != mesh->getCellsEnd()) {
    c->getCentroid(&x);
    // The last condition prevents collapsing an interior edge with two
    // boundary nodes.  Doing this would change the topology of the mesh.
    if (computeMinimumEdgeLength<SMR>(c, &i) < f(x) &&
	c->isFaceOnBoundary(i)) {
      // If the edge can be collapsed: Update one of the node locations in
      // preparation for the collapse and update the manifold.
      if (updateNodesAndManifoldForBoundaryEdgeCollapse<SMR, QualityMetric>
	  (c, i, minimumAllowedQuality, qualityFactor, manifold)) {
	nextValidCell = getNextValidCellAfterCollapse<SMR>(c, i);
	// Collapse the ege.
	collapse(mesh, c, i);
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
coarsen(SimpMeshRed<2,2,T,Node,Cell,Cont>* mesh, 
	const MinEdgeLength& f,
	const T minimumAllowedQuality, const T qualityFactor,
	PointsOnManifold<2,1,SD,T>* manifold,
	int maxSweeps) {
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
coarsen(SimpMeshRed<2,2,T,Node,Cell,Cont>* mesh, const MinEdgeLength& f,
	const T minimumAllowedQuality, const T qualityFactor,
	const T cornerDeviation, const int maxSweeps) {
  // If they specified an angle for defining corners.
  if (cornerDeviation >= 0) {
    // CONTINUE: Fix this when I write a better manifold constructor.
    // Build a boundary manifold data structure.
    IndSimpSetIncAdj<2,2,true,T> iss;
    buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
    IndSimpSetIncAdj<2,1,true,T> boundary;
    buildBoundary(iss, &boundary);
    PointsOnManifold<2,1,1,T> manifold(boundary, cornerDeviation);
    manifold.insertBoundaryVertices(mesh);
  
    // Call the above coarsen function.
    return coarsen<QualityMetric>(mesh, f, minimumAllowedQuality, 
				  qualityFactor, &manifold, maxSweeps);
  }
  // Otherwise, don't use a manifold.
  // Call the above coarsen function.
  PointsOnManifold<2,1,1,T>* nullManifoldPointer = 0;
  return coarsen<QualityMetric>(mesh, f, minimumAllowedQuality, 
				qualityFactor, nullManifoldPointer,
				maxSweeps);
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
coarsenInterior(SimpMeshRed<2,2,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		PointsOnManifold<2,1,SD,T>* manifold,
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
coarsenInterior(SimpMeshRed<2,2,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		const T cornerDeviation, const int maxSweeps) {
  // If they specified an angle for defining corners.
  if (cornerDeviation >= 0) {
    // CONTINUE: Fix this when I write a better manifold constructor.
    // Build a boundary manifold data structure.
    IndSimpSetIncAdj<2,2,true,T> iss;
    buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
    IndSimpSetIncAdj<2,1,true,T> boundary;
    buildBoundary(iss, &boundary);
    PointsOnManifold<2,1,1,T> manifold(boundary, cornerDeviation);
    manifold.insertBoundaryVertices(mesh);
  
    // Call the above coarsen function.
    return coarsenInterior<QualityMetric>(mesh, f, minimumAllowedQuality, 
					  qualityFactor, &manifold, maxSweeps);
  }
  // Otherwise, don't use a manifold.
  // Call the above function.
  PointsOnManifold<2,1,1,T>* nullManifoldPointer = 0;
  return coarsenInterior<QualityMetric>(mesh, f, minimumAllowedQuality, 
					qualityFactor, nullManifoldPointer,
					maxSweeps);
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
coarsenBoundary(SimpMeshRed<2,2,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		PointsOnManifold<2,1,SD,T>* manifold,
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
coarsenBoundary(SimpMeshRed<2,2,T,Node,Cell,Cont>* mesh, 
		const MinEdgeLength& f,
		const T minimumAllowedQuality, const T qualityFactor,
		const T cornerDeviation, const int maxSweeps) {
  // If they specified an angle for defining corners.
  if (cornerDeviation >= 0) {
    // CONTINUE: Fix this when I write a better manifold constructor.
    // Build a boundary manifold data structure.
    IndSimpSetIncAdj<2,2,true,T> iss;
    buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
    IndSimpSetIncAdj<2,1,true,T> boundary;
    buildBoundary(iss, &boundary);
    PointsOnManifold<2,1,1,T> manifold(boundary, cornerDeviation);
    manifold.insertBoundaryVertices(mesh);
  
    // Call the above coarsen function.
    return coarsenBoundary<QualityMetric>(mesh, f, minimumAllowedQuality,
					  qualityFactor, &manifold, maxSweeps);
  }
  // Otherwise, don't use a manifold.
  // Call the above function.
  PointsOnManifold<2,1,1,T>* nullManifoldPointer = 0;
  return coarsenBoundary<QualityMetric>(mesh, f, minimumAllowedQuality, 
					qualityFactor, nullManifoldPointer,
					maxSweeps);
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
coarsen(SimpMeshRed<3,2,T,Node,Cell,Cont>* mesh, 
	const MinEdgeLength& f,
	const T minimumAllowedQuality, const T qualityFactor,
	PointsOnManifold<3,2,SD,T>* manifold,
	int maxSweeps) {
  int c; 
  int count = 0;
  int sweep = 0;
  // CONTINUE
  //std::cerr << "manifold = " << int(manifold) << "\n";
  do {
    // CONTINUE
    //std::cerr << "Before the sweep: " << mesh->computeCellsSize() << "\n";
    c = coarsenSweep<QualityMetric>(mesh, f, minimumAllowedQuality, 
				    qualityFactor, manifold);
    // CONTINUE
    //std::cerr << "After the sweep: " << mesh->computeCellsSize() << "\n";
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
coarsen(SimpMeshRed<3,2,T,Node,Cell,Cont>* mesh, const MinEdgeLength& f,
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
    IndSimpSetIncAdj<3,2,true,T> iss;
    buildIndSimpSetFromSimpMeshRed(*mesh, &iss);
    PointsOnManifold<3,2,1,T> manifold(iss, 
				       maxDihedralAngleDeviation,
				       maxSolidAngleDeviation,
				       maxBoundaryAngleDeviation);
    manifold.insertVerticesAndEdges(mesh);
  
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





//! Return the minimum collapseable edge.
/*!
  If there is no collapseable edge, return M + 1.

  An edge is not collapseable if collapsing it would change the topology 
  of the mesh.
*/
template<class SMR>
inline
int
computeMinimumCollapseableEdge(const typename SMR::CellConstIterator c) {
  LOKI_STATIC_CHECK(SMR::M == 2, TheDimensionMustBe2);
  typedef typename SMR::Number Number;

  const int M = SMR::M;

  ads::FixedArray<M + 1,Number> lengths;

  ads::FixedArray<M + 1,const Number*> ptrs;
  for (int m = 0; m != ptrs.size(); ++m) {
    ptrs[m] = &lengths[m];
  }
  
  for (int m = 0; m != lengths.size(); ++m) {
    lengths[m] = geom::computeDistance(c->getNode((m+1)%(M+1))->getVertex(),
				       c->getNode((m+2)%(M+1))->getVertex());
  }
  // Sort the length pointers.
  {
    ads::binary_compose_binary_unary< std::less<Number>,
      ads::Dereference<const Number*>,
      ads::Dereference<const Number*> > comp;
    std::sort(ptrs.begin(), ptrs.end(), comp);
  }

  ads::FixedArray<M + 1,int> sorted;
  for (int m = 0; m != sorted.size(); ++m) {
    sorted[m] = int(ptrs[m] - &lengths[0]);
  }

  // For each edge in sorted order.
  int i;
  for (int m = 0; m != sorted.size(); ++m) {
    i = sorted[m];
    // If the edge is collapseable.
    if (! areMirrorNodes<SMR>(c->getNode((i+1)%(M+1)), 
			      c->getNode((i+2)%(M+1))) &&
	(c->isFaceOnBoundary(i) || 
	 doesEdgeHaveAnInteriorNode<SMR>(c, i))) {
      return i;
    }
  }
  return M + 1;
}



//! Collapse edges to remove the specified cells.
/*!
  \param mesh The simplicial mesh.

  \return the number of edges collapsed.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename IntInIter>
inline
int
coarsen(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, 
	IntInIter begin, IntInIter end) {
  typedef SimpMeshRed<N,2,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::CellIteratorSet CellIteratorSet;

  const int M = 2;

  // Make a set of the cell iterators.
  CellIteratorSet cells;
  convertIdentifiersToIterators(*mesh, begin, end, &cells);
  
  // Loop until all the specified cell have been collapsed.
  int count = 0;
  int i;
  CellIterator c;
  while (! cells.empty()) {
    c = *cells.begin();
    // Erase this cell through its set iterator.
    cells.erase(cells.begin());
    // Find the minimum length collapseable edge.
    i = computeMinimumCollapseableEdge<SMR>(c);
    // If there is a collapseable edge.
    if (i != M + 1) {
      // If it has a neighbor across the collapsing edge.
      if (! c->isFaceOnBoundary(i)) {
	// Erase the neighbor from the set.
	cells.erase(c->getNeighbor(i));
      }
      // Collapse the edge.
      collapse(mesh, c, i);
      ++count;
    }
  }
  // Return the number of edges collapsed.
  return count;
}

END_NAMESPACE_GEOM

// End of file.
