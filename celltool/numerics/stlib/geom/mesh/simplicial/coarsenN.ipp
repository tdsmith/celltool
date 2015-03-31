// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_coarsenN_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM



template<typename SMR, class QualityMetric, typename CellIteratorIterator>
inline
typename SMR::Number
computeMinimumQualityOfCells(CellIteratorIterator beginning,
			     CellIteratorIterator end) {
  const int M = SMR::M;
  
  typedef typename SMR::Number Number;
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::Vertex Vertex;
  typedef Simplex<M, Vertex, Number> Simplex;

  // CONTINUE
  //std::cout << "size = " << std::distance(beginning, end);

  // Construct a quality functor.
  QualityMetric quality;
  Simplex simplex;
  CellIterator cell;
  Number minimumQuality = std::numeric_limits<Number>::max();
  Number q;
  // For each cell.
  for ( ; beginning != end; ++beginning) {
    cell = *beginning;
    cell->getSimplex(&simplex);
    quality.setFunction(simplex);
    // CONTINUE: Now I take the inverse because the quality functions have
    // the range [1..infinity).
    q = 1.0 / quality();
    if (q < minimumQuality) {
      minimumQuality = q;
    }
  }
  // CONTINUE
  //std::cout << ", minimumQuality = " << minimumQuality << "\n";
  return minimumQuality;
}




template<typename SMR, class QualityMetric>
inline
typename SMR::Number
computeMinimumQualityOfCellsIncidentToNode
(const typename SMR::NodeIterator node) {
  return computeMinimumQualityOfCells<SMR, QualityMetric>
    (node->getCellIteratorsBeginning(), node->getCellIteratorsEnd());
}




template<typename SMR, class QualityMetric>
inline
typename SMR::Number
computeMinimumQualityOfCellsIncidentToNodesButNotToEdge
(const typename SMR::CellIterator c, const int i, const int j) {
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::CellIteratorSet CellIteratorSet;
  typedef typename SMR::CellIteratorCompare CellIteratorCompare;

  // The cells that are incident to the source node.
  CellIteratorSet source(c->getNode(i)->getCellIteratorsBeginning(),
			 c->getNode(i)->getCellIteratorsEnd());
  // The cells that are incident to the target node.
  CellIteratorSet target(c->getNode(j)->getCellIteratorsBeginning(),
			 c->getNode(j)->getCellIteratorsEnd());
  // The cells that are incident to the nodes but not to the edge.
  std::vector<CellIterator> incident;
  // Comparison functor for cell iterators.
  CellIteratorCompare compare;
  // Get the cells that will be incident to the node if the edge is collapsed.
  std::set_symmetric_difference(source.begin(), source.end(),
				target.begin(), target.end(),
				std::back_inserter(incident),
				compare);
  // Return the minimum quality of the cells.
  return computeMinimumQualityOfCells<SMR, QualityMetric>(incident.begin(),
							  incident.end());
}




template<typename SMR, class QualityMetric>
inline
typename SMR::Number
computeMinimumQualityOfCellsIncidentToNodesOfEdge
(const typename SMR::CellIterator c, const int i, const int j) {
  typedef typename SMR::CellIterator CellIterator;
  typedef typename SMR::CellIteratorSet CellIteratorSet;
  typedef typename SMR::CellIteratorCompare CellIteratorCompare;

  // The cells that are incident to the source node.
  CellIteratorSet source(c->getNode(i)->getCellIteratorsBeginning(),
			 c->getNode(i)->getCellIteratorsEnd());
  // The cells that are incident to the target node.
  CellIteratorSet target(c->getNode(j)->getCellIteratorsBeginning(),
			 c->getNode(j)->getCellIteratorsEnd());
  // The cells that are incident to either node.
  std::vector<CellIterator> incident;
  // Comparison functor for cell iterators.
  CellIteratorCompare compare;
  // Get the cells that will be incident to the node if the edge is collapsed.
  std::set_union(source.begin(), source.end(),
		 target.begin(), target.end(),
		 std::back_inserter(incident),
		 compare);
  // Return the minimum quality of the cells.
  return computeMinimumQualityOfCells<SMR, QualityMetric>(incident.begin(),
							  incident.end());
}

END_NAMESPACE_GEOM

// End of file.
