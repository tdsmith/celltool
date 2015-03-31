// -*- C++ -*-

#if !defined(__GridBase_ipp__)
#error This file is an implementation detail of the class GridBase
#endif

BEGIN_NAMESPACE_CPT


//
// Constructors, Destructor
//


template<int N, typename T>
template<bool A1, bool A2, bool A3, bool A4>
inline
GridBase<N,T>::
GridBase(ads::Array<N,Number,A1>* distance,
	  ads::Array<N,Point,A2>* gradientOfDistance, 
	  ads::Array<N,Point,A3>* closestPoint,
	  ads::Array<N,int,A4>* closestFace) :
  _distance(*distance),
  _gradientOfDistance(*gradientOfDistance),
  _closestPoint(*closestPoint),
  _closestFace(*closestFace)
{}


template<int N, typename T>
inline
GridBase<N,T>&
GridBase<N,T>::
operator=(const GridBase& other) {
  if (this != &other) {
    _distance = other._distance;
    _gradientOfDistance = other._gradientOfDistance;
    _closestPoint = other._closestPoint;
    _closestFace = other._closestFace;
  }
  return *this;
}


//
// Mathematical operations
//


// Calculate the signed distance, closest point, etc. for the specified
// grid points.
template<int N, typename T>
template<class Component>
inline
std::pair<int,int>
GridBase<N,T>::
computeClosestPointTransform(const std::vector<Index>& indices,
			 const std::vector<Point>& positions,
			 const Component& component,
			 const Number maximumDistance) {
  // Sanity check.
  assert(indices.size() == positions.size());

  // Variables used in the following loops.
  int numberOfDistancesComputed = 0;
  int numberOfDistancesSet = 0;
  int index;
  Number dist;

  // Iterators for the loop over the grid points.
  typename std::vector<Index>::const_iterator indexIterator 
    = indices.begin();
  const typename std::vector<Index>::const_iterator indexEnd
    = indices.end();
  typename std::vector<Point>::const_iterator pointIterator 
    = positions.begin();

  if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed() && 
      isClosestFaceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance, gradient of distance and closest point.
	dist = component.computeClosestPointAndGradient
	  (*pointIterator, &cp, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	    std::abs(dist) < std::abs(getDistance()[index])) {
	  // If this is the first time the distance has been set.
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	  getGradientOfDistance()[index] = grad;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else if (isClosestPointBeingComputed() && 
	   isGradientOfDistanceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance, gradient of distance and closest point.
	dist = component.computeClosestPointAndGradient
	  (*pointIterator, &cp, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	  getGradientOfDistance()[index] = grad;
	}
      }
    }
  }
  else if (isClosestPointBeingComputed() && isClosestFaceBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and closest point.
	dist = component.computeClosestPoint(*pointIterator, &cp);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed() && isClosestFaceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and the gradient of the distance.
	dist = component.computeGradient(*pointIterator, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getGradientOfDistance()[index] = grad;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else if (isClosestPointBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and the closest point.
	dist = component.computeClosestPoint(*pointIterator, &cp);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	}
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and the gradient of the distance.
	dist = component.computeGradient(*pointIterator, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getGradientOfDistance()[index] = grad;
	}
      }
    }
  }
  else if (isClosestFaceBeingComputed()) {
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance.
	dist = component.computeDistance(*pointIterator);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else {
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      // CONTINUE: This could be the problem.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance.
	dist = component.computeDistance(*pointIterator);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (std::abs(dist) <= maximumDistance &&
	     std::abs(dist) < std::abs(getDistance()[index])) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	}
      }
    }
  }

  return std::pair<int,int>(numberOfDistancesComputed, numberOfDistancesSet);
}





// Calculate the signed distance, closest point, etc. for the specified
// grid points.
template<int N, typename T>
template<class Component>
inline
std::pair<int,int>
GridBase<N,T>::
computeClosestPointTransformUnsigned(const std::vector<Index>& indices,
				  const std::vector<Point>& positions,
				  const Component& component,
				  const Number maximumDistance) {
  // Sanity check.
  assert(indices.size() == positions.size());

  // Variables used in the following loops.
  int numberOfDistancesComputed = 0;
  int numberOfDistancesSet = 0;
  int index;
  Number dist;

  // Iterators for the loop over the grid points.
  typename std::vector<Index>::const_iterator indexIterator 
    = indices.begin();
  const typename std::vector<Index>::const_iterator indexEnd
    = indices.end();
  typename std::vector<Point>::const_iterator pointIterator 
    = positions.begin();

  if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed() && 
       isClosestFaceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance, gradient of distance and closest point.
	dist = component.computeClosestPointAndGradientUnsigned
	  (*pointIterator, &cp, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  // CONTINUE: Is this really what I want to count?
	  // Should I instead compute the total number of distance computed,
	  // (as opposed to distances set).
	  // If this is the first time the distance has been set.
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	  getGradientOfDistance()[index] = grad;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else if (isClosestPointBeingComputed() && 
	   isGradientOfDistanceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance, gradient of distance and closest point.
	dist = component.computeClosestPointAndGradientUnsigned
	  (*pointIterator, &cp, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	  getGradientOfDistance()[index] = grad;
	}
      }
    }
  }
  else if (isClosestPointBeingComputed() && isClosestFaceBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and closest point.
	dist = component.computeClosestPointUnsigned(*pointIterator, &cp);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed() && 
	   isClosestFaceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and the gradient of the distance.
	dist = component.computeGradientUnsigned(*pointIterator, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getGradientOfDistance()[index] = grad;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else if (isClosestPointBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and the closest point.
	dist = component.computeClosestPointUnsigned(*pointIterator, &cp);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestPoint()[index] = cp;
	}
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance and the gradient of the distance.
	dist = component.computeGradientUnsigned(*pointIterator, &grad);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getGradientOfDistance()[index] = grad;
	}
      }
    }
  }
  else if (isClosestFaceBeingComputed()) {
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance.
	dist = component.computeDistanceUnsigned(*pointIterator);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	  getClosestFace()[index] = component.getFaceIndex();
	}
      }
    }
  }
  else {
    // Loop over the grid points.
    for (; indexIterator != indexEnd; ++indexIterator, ++pointIterator) {
      // If the index is in the index range of this grid.
      if (getRanges().is_in(*indexIterator)) {
	// Convert the multi-index to a single container index.
	index = getDistance().index(*indexIterator);
	// Compute the distance.
	dist = component.computeDistanceUnsigned(*pointIterator);
	++numberOfDistancesComputed;
      
	// If the new distance is less than the old distance.
	if (dist <= maximumDistance && dist < getDistance()[index]) {
	  if (getDistance()[index] == 
	       std::numeric_limits<Number>::max()) {
	    ++numberOfDistancesSet;
	  }
	  getDistance()[index] = dist;
	}
      }
    }
  }

  return std::pair<int,int>(numberOfDistancesComputed, numberOfDistancesSet);
}





// Calculate the signed distance, closest point, etc. for the specified
// grid points.
template<int N, typename T>
template<class Component>
inline
std::pair<int,int>
GridBase<N,T>::
computeClosestPointTransform(const Lattice& lattice,
			 const Range& indexRangeInLattice,
			 const Component& component,
			 const Number maximumDistance) {
  // Compute the intersection of the index range in the lattice and the 
  // index range of this grid.
  Range indexRange;
  ads::compute_intersection(indexRangeInLattice, getRanges(), indexRange);

  // Variables used in the following loops.
  int numberOfDistancesComputed = 0;
  int numberOfDistancesSet = 0;
  int index;
  Number dist;
  Point x;

  // Iterators over the index range.
  ads::IndexIterator<N> i(indexRange);
  ads::IndexIterator<N> iEnd(indexRange);
  iEnd.set_end();

  if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed() && 
      isClosestFaceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance, gradient of distance and closest point.
      dist = component.computeClosestPointAndGradientChecked(x, &cp, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	// If this is the first time the distance has been set.
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
	getGradientOfDistance()[index] = grad;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed() && 
	   isGradientOfDistanceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance, gradient of distance and closest point.
      dist = component.computeClosestPointAndGradientChecked(x, &cp, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
	getGradientOfDistance()[index] = grad;
      }
    }
  }
  else if (isClosestPointBeingComputed() && isClosestFaceBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and closest point.
      dist = component.computeClosestPointChecked(x, &cp);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed() && isClosestFaceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and the gradient of the distance.
      dist = component.computeGradientChecked(x, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getGradientOfDistance()[index] = grad;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and the closest point.
      dist = component.computeClosestPointChecked(x, &cp);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and the gradient of the distance.
      dist = component.computeGradientChecked(x, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getGradientOfDistance()[index] = grad;
      }
    }
  }
  else if (isClosestFaceBeingComputed()) {
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance.
      dist = component.computeDistanceChecked(x);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else {
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance.
      dist = component.computeDistanceChecked(x);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
      }
    }
  }

  return std::pair<int,int>(numberOfDistancesComputed, numberOfDistancesSet);
}









// Calculate the signed distance, closest point, etc. for the specified
// grid points.
template<int N, typename T>
template<class Component>
inline
std::pair<int,int>
GridBase<N,T>::
computeClosestPointTransformUnsigned(const Lattice& lattice,
				  const Range& indexRangeInLattice,
				  const Component& component,
				  const Number maximumDistance) {
  // Compute the intersection of the index range in the lattice and the 
  // index range of this grid.
  Range indexRange;
  ads::compute_intersection(indexRangeInLattice, getRanges(), indexRange);

  // Variables used in the following loops.
  int numberOfDistancesComputed = 0;
  int numberOfDistancesSet = 0;
  int index;
  Number dist;
  Point x;

  // Iterators over the index range.
  ads::IndexIterator<N> i(indexRange);
  ads::IndexIterator<N> iEnd(indexRange);
  iEnd.set_end();

  if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed() && 
       isClosestFaceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance, gradient of distance and closest point.
      dist = component.computeClosestPointAndGradientUnsignedChecked
	(x, &cp, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	// If this is the first time the distance has been set.
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
	getGradientOfDistance()[index] = grad;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed()) {
    Point cp, grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance, gradient of distance and closest point.
      dist = component.computeClosestPointAndGradientUnsignedChecked
	(x, &cp, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
	getGradientOfDistance()[index] = grad;
      }
    }
  }
  else if (isClosestPointBeingComputed() && isClosestFaceBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and closest point.
      dist = component.computeClosestPointUnsignedChecked(x, &cp);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed() && 
	   isClosestFaceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and the gradient of the distance.
      dist = component.computeGradientUnsignedChecked(x, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getGradientOfDistance()[index] = grad;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed()) {
    Point cp;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and the closest point.
      dist = component.computeClosestPointUnsignedChecked(x, &cp);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestPoint()[index] = cp;
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed()) {
    Point grad;
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance and the gradient of the distance.
      dist = component.computeGradientUnsignedChecked(x, &grad);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getGradientOfDistance()[index] = grad;
      }
    }
  }
  else if (isClosestFaceBeingComputed()) {
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance.
      dist = component.computeDistanceUnsignedChecked(x);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
	getClosestFace()[index] = component.getFaceIndex();
      }
    }
  }
  else {
    // Loop over the grid points.
    for (; i != iEnd; ++i) {
      // Compute the position of the point.
      x = *i;
      lattice.convertIndexToLocation(&x);
      // Convert the multi-index to a single container index.
      index = getDistance().index(*i);
      // Compute the distance.
      dist = component.computeDistanceUnsignedChecked(x);
      ++numberOfDistancesComputed;
      
      // If the new distance is less than the old distance.
      if (std::abs(dist) <= maximumDistance &&
	   std::abs(dist) < std::abs(getDistance()[index])) {
	if (getDistance()[index] == 
	     std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()[index] = dist;
      }
    }
  }

  return std::pair<int,int>(numberOfDistancesComputed, numberOfDistancesSet);
}







//
// Set all the distances to std::numeric_limits<Number>::max() in 
// preparation for the distance to be 
// computed.  Set the gradient of the distance and the closest points to
// std::numeric_limits<Number>::max().  Set the closest faces to -1.
//
template<int N, typename T>
inline
void 
GridBase<N,T>::
initialize() {
  _distance = std::numeric_limits<Number>::max();

  const Point infinity(std::numeric_limits<Number>::max());
  _gradientOfDistance = infinity;
 
  _closestPoint = infinity;
 
  _closestFace = -1;
}


template<int N, typename T>
inline
bool 
GridBase<N,T>::
floodFillUnsigned(const Number farAway) {
  bool result = false;
  //
  // Flood fill the unknown distances with farAway.
  //
  typename ads::Array<N,Number,false>::iterator i = getDistance().begin(),
    iEnd = getDistance().end();
  for (; i != iEnd; ++i) {
    if (*i == std::numeric_limits<Number>::max()) {
      *i = farAway;
    }
    else {
      result = true;
    }
  }
  
  return result;
}


//
// File I/O
//


template<int N, typename T>
inline
void 
GridBase<N,T>::
put(std::ostream& out) const {
  out << "getRanges() = " << getRanges() << '\n';
  _distance.write_elements_ascii(out);
  _gradientOfDistance.write_elements_ascii(out);
  _closestPoint.write_elements_ascii(out);
  _closestFace.write_elements_ascii(out);
}


template<int N, typename T>
inline
void 
GridBase<N,T>::
displayInformation(std::ostream& out) const {
  out << "The grid index ranges are " << getRanges() << '\n';

  if (isGradientOfDistanceBeingComputed()) {
    out << "The gradient of the distance is being computed.\n";
  }
  else {
    out << "The gradient of the distance is not being computed.\n";
  }

  if (isClosestPointBeingComputed()) {
    out << "The closest point is being computed.\n";
  }
  else {
    out << "The closest point is not being computed.\n";
  }

  if (isClosestFaceBeingComputed()) {
    out << "The closest face is being computed.\n";
  }
  else {
    out << "The closest face is not being computed.\n";
  }
}


template<int N, typename T>
inline
int
GridBase<N,T>::
countKnownDistances(const Number maximumDistance) const {
  //
  // Find the number of known distances.
  //
  int numberOfKnownDistances = 0;
  typename ads::Array<N,Number,false>::const_iterator 
    ptr = _distance.begin();
  const typename ads::Array<N,Number,false>::const_iterator 
    end = _distance.end();
  for (; ptr != end; ++ptr) {
    if (std::abs(*ptr) < maximumDistance) {
      ++numberOfKnownDistances;
    }
  }
    
  return numberOfKnownDistances;
}


template<int N, typename T>
inline
void 
GridBase<N,T>::
computeMinimumAndMaximumDistances(const Number maximumDistance, 
				  Number* minimum,
				  Number* maximum) const {
  //
  // Find the max and min known distance.
  //
  typename ads::Array<N,Number,false>::const_iterator 
    ptr = _distance.begin();
  const typename ads::Array<N,Number,false>::const_iterator 
    end = _distance.end();
  *minimum = std::numeric_limits<Number>::max();
  *maximum = -std::numeric_limits<Number>::max();
  for (; ptr != end; ++ptr) {
    if (std::abs(*ptr) < maximumDistance) {
      if (*ptr < *minimum) {
	*minimum = *ptr;
      }
      else if (*ptr > *maximum) {
	*maximum = *ptr;
      }
    }
  }
}


END_NAMESPACE_CPT


//
// Equality operators
//


template<int N, typename T>
inline
bool 
operator==(const cpt::GridBase<N,T>& a, const cpt::GridBase<N,T>& b) {
  if (a.getDistance() != b.getDistance()) {
    return false;
  }

  // Check that the gradient of the distance is or is not being computed.
  if (a.isGradientOfDistanceBeingComputed() != 
      b.isGradientOfDistanceBeingComputed()) {
    return false;
  }

  // Check equality for each of the gradients of the distance.
  if (a.isGradientOfDistanceBeingComputed()) {
    if (a.getGradientOfDistance() != b.getGradientOfDistance()) {
      return false;
    }
  }

  // Check that the closest point is or is not being computed.
  if (a.isClosestPointBeingComputed() != b.isClosestPointBeingComputed()) {
    return false;
  }

  // Check equality for each of the closest points.
  if (a.isClosestPointBeingComputed()) {
    if (a.getClosestPoint() != b.getClosestPoint()) {
      return false;
    }
  }

  // Check that the closest face is or is not being computed.
  if (a.isClosestFaceBeingComputed() != b.isClosestFaceBeingComputed()) {
    return false;
  }
  // Check equality for each of the closest faces.
  if (a.isClosestFaceBeingComputed()) {
    if (a.getClosestFace() != b.getClosestFace()) {
      return false;
    }
  }
  
  return true;
}
