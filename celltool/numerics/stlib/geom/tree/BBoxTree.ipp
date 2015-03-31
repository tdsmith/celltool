// -*- C++ -*-

/*! 
  \file BBoxTree.ipp
  \brief A class for a bounding box tree in N-D.
*/

#if !defined(__geom_BBoxTree_ipp__)
#error This file is an implementation detail of the class BBoxTree.
#endif

BEGIN_NAMESPACE_GEOM


//-----------------------------BBoxTreeLeaf-------------------------------


template<int N, typename T>
template<typename ObjectIter, typename ObjectIterIter>
inline
BBoxTreeLeaf<N,T>::
BBoxTreeLeaf(ObjectIter base, ObjectIterIter begin, ObjectIterIter end) :
  _domain(),
  _indices(std::distance(begin, end)) {
  for (IndexIterator i = _indices.begin(); i != _indices.end(); ++i) {
    *i = int(*begin - base);
    ++begin;
  }
}


template<int N, typename T>
inline
void
BBoxTreeLeaf<N,T>::
computeDomain(const std::vector<BBox>& boxes) {
  assert(_indices.size() != 0);
  _domain = boxes[_indices[0]];
  const int sz = int(_indices.size());
  for (int i = 1; i != sz; ++i) {
    _domain.add(boxes[_indices[i]]);
  }
}


template<int N, typename T>
inline
void
BBoxTreeLeaf<N,T>::
computeMinimumDistanceQuery(std::vector<const Leaf*>& leaves, 
			    const Point& x, Number* upperBound) const {
  // Update the upper bound on the distance.
  Number ub = computeUpperBoundOnUnsignedDistance(_domain, x);
  if (ub < *upperBound) {
    *upperBound = ub;
  }
  // Add this leaf.
  leaves.push_back(this);
}


template<int N, typename T>
inline
void
BBoxTreeLeaf<N,T>::
printAscii(std::ostream& out) const {
  out << "Leaf: " << _domain << "\n  ";
  for (IndexConstIterator i = _indices.begin(); i != _indices.end(); ++i) {
    out << *i << " ";
  }
  out << '\n';
}


template<int N, typename T>
inline
void
BBoxTreeLeaf<N,T>::
checkValidity(const std::vector<BBox>& boxes) const {
  const int sz = int(_indices.size());
  assert(sz != 0);
  for (int i = 0; i != sz; ++i) {
    assert(_domain.isIn(boxes[_indices[i]]));
  }
}


//-----------------------------BBoxTreeBranch-------------------------------
  
//
// Constructors
//
  
// Construct from a set of grid elements.
template<int N, typename T>
inline
BBoxTreeBranch<N,T>::
BBoxTreeBranch(const Point* base,
	       const ads::FixedArray<N, std::vector<const Point*> >& sorted,
	       const SizeType leafSize) {
#ifdef DEBUG_BBoxTree
  // There must be more records than in a single leaf.
  assert(SizeType(sorted[0].size()) > leafSize);
  // Each of the sorted arrays should be of the same size.
  for (int n = 1; n != N; ++n) {
    // CONTINUE: REMOVE
    if (sorted[n].size() != sorted[0].size()) {
      std::cerr << "n = " << n 
		<< " sorted[0].size() = " << sorted[0].size() 
		<< " sorted[n].size() = " << sorted[n].size() << "\n";
    }
    assert(sorted[n].size() == sorted[0].size());
  }
#endif
  //
  // Determine the splitting direction.
  //

  Point spreads;
  for (int n = 0; n != N; ++n) {
    spreads[n] = (*sorted[n].back())[n] - (*sorted[n].front())[n];
  }

  int splitDimension = spreads.max_index();
      
  //
  // Name the input vectors.
  //

  const std::vector<const Point*>& splitSorted = sorted[splitDimension];
 
  //
  // Compute the median.
  //

  const int medianIndex = int(sorted[0].size()) / 2;
  const SizeType leftSize = medianIndex;
  const SizeType rightSize = int(sorted[0].size()) - medianIndex;
  const Point& medianPoint = *splitSorted[medianIndex];

  //
  // Vectors for the subtrees.
  //
  ads::FixedArray<N, std::vector<const Point*> > sub;
  for (int n = 0; n != N; ++n) {
    sub[n].reserve(rightSize);
  }
  typename std::vector<const Point*>::const_iterator iter;

  //
  // Make the left subtree.
  //

  std::copy(splitSorted.begin(), splitSorted.begin() + medianIndex, 
	     back_inserter(sub[splitDimension]));

  // For each dimension except the split dimension.
  for (int n = 0; n != N; ++n) {
    if (n != splitDimension) {
      for (iter = sorted[n].begin(); iter != sorted[n].end(); ++iter) {
	if (ads::less_composite_fcn<N>(splitDimension, **iter, 
				       medianPoint)) {
	  sub[n].push_back(*iter);
	}
      }
    }
    assert(sub[n].size() == sub[splitDimension].size());
  }

  // If the left subtree is a leaf.
  if (leftSize <= leafSize) {
    _left = new Leaf(base, splitSorted.begin(), 
			   splitSorted.begin() + leftSize);
  }
  else {
    _left = new BBoxTreeBranch(base, sub, leafSize);
  }

  for (int n = 0; n != N; ++n) {
    sub[n].clear();
  }

  //
  // Make the right subtree.
  //

  std::copy(splitSorted.begin() + medianIndex, splitSorted.end(),
	     back_inserter(sub[splitDimension]));

  // For each dimension except the split dimension.
  for (int n = 0; n != N; ++n) {
    if (n != splitDimension) {
      for (iter = sorted[n].begin(); iter != sorted[n].end(); ++iter) {
	if (! ads::less_composite_fcn<N>(splitDimension, **iter, 
					 medianPoint)) {
	  sub[n].push_back(*iter);
	}
      }
    }
    assert(sub[n].size() == sub[splitDimension].size());
  }

  // If the right subtree is a leaf.
  if (rightSize <= leafSize) {
    _right = new Leaf(base, splitSorted.begin() + medianIndex, 
		      splitSorted.end());
  }
  else {
    _right = new BBoxTreeBranch(base, sub, leafSize);
  }
}


template<int N, typename T>
inline
void
BBoxTreeBranch<N,T>::
computeDomain(const std::vector<BBox>& boxes) {
  // Compute the bounding box for the left and right branch.
  _left->computeDomain(boxes);
  _right->computeDomain(boxes);
  // Make a bounding box around those two boxes.
  _domain = _left->getDomain();
  _domain.add(_right->getDomain());
}


template<int N, typename T>
inline
void
BBoxTreeBranch<N,T>::
computePointQuery(std::vector<const Leaf*>& leaves, const Point& x) const {
  if (_domain.isIn(x)) {
    _left->computePointQuery(leaves, x);
    _right->computePointQuery(leaves, x);
  }
}


template<int N, typename T>
inline
void
BBoxTreeBranch<N,T>::
computeWindowQuery(std::vector<const Leaf*>& leaves, 
		   const BBox& window) const {
  if (doOverlap(window, _domain)) {
    _left->computeWindowQuery(leaves, window);
    _right->computeWindowQuery(leaves, window);
  }
}


template<int N, typename T>
inline
void
BBoxTreeBranch<N,T>::
computeMinimumDistanceQuery(std::vector<const Leaf*>& leaves, 
			    const Point& x, Number* upperBound) const {
  // Update the upper bound on the distance.
  Number ub = computeUpperBoundOnUnsignedDistance(_domain, x);
  if (ub < *upperBound) {
    *upperBound = ub;
  }

  const Number leftLowerBound = computeLowerBoundOnUnsignedDistance(_left->getDomain(), x);
  const Number rightLowerBound = computeLowerBoundOnUnsignedDistance(_right->getDomain(), 
						      x);

  // If the left is more promising than the right.
  if (leftLowerBound < rightLowerBound) {
    // First investigate the left branch.
    if (leftLowerBound <= *upperBound) {
      _left->computeMinimumDistanceQuery(leaves, x, upperBound);
    }
    // Then investigate the right branch.
    if (rightLowerBound <= *upperBound) {
      _right->computeMinimumDistanceQuery(leaves, x, upperBound);
    }
  }
  // Else the right branch is more promising.
  else {
    // First investigate the right branch.
    if (rightLowerBound <= *upperBound) {
      _right->computeMinimumDistanceQuery(leaves, x, upperBound);
    }
    // Then investigate the left branch.
    if (leftLowerBound <= *upperBound) {
      _left->computeMinimumDistanceQuery(leaves, x, upperBound);
    }
  }
}


template<int N, typename T>
inline
void
BBoxTreeBranch<N,T>::
checkValidity(const std::vector<BBox>& boxes) const {
  assert(_left && _right);
  _left->checkValidity(boxes);
  _right->checkValidity(boxes);
  assert(_domain.isIn(_left->getDomain()));
  assert(_domain.isIn(_right->getDomain()));
}


template<int N, typename T>
inline
void
BBoxTreeBranch<N,T>::
printAscii(std::ostream& out) const {
  out << "Branch: " << _domain << "\n";
  _left->printAscii(out);
  _right->printAscii(out);
}


//-----------------------------BBoxTree-----------------------------------

//
// Constructors
//

template<int N, typename T>
template<class BBoxInputIter>
inline
BBoxTree<N,T>::
BBoxTree(BBoxInputIter begin, BBoxInputIter end, const SizeType leafSize) :
  // Initially, the root is null.
  _root(0),
  // Copy the bounding boxes into our data structure.
  _boxes(begin, end),
  _leaves() {
  build(leafSize);
}


template<int N, typename T>
template<class BBoxInputIter>
inline
void
BBoxTree<N,T>::
build(BBoxInputIter begin, BBoxInputIter end, const SizeType leafSize) {
  // Destroy the old data structure.
  destroy();
  // Copy the bounding boxes into our data structure.
  {
    std::vector<BBox> tmp(begin, end);
    _boxes.swap(tmp);
  }
  // Build the tree.
  build(leafSize);
}

template<int N, typename T>
inline
void
BBoxTree<N,T>::
destroy() {
  // Delete the tree.
  if (_root) {
    delete _root;
  }
  _root = 0;
  // Delete the bounding boxes.
  {
    std::vector<BBox> tmp;
    _boxes.swap(tmp);
  }
  // Free the memory for the leaf container.
  {
    std::vector<const Leaf*> tmp;
    _leaves.swap(tmp);
  }
}


//
// File I/O
//

template<int N, typename T>
inline
void
BBoxTree<N,T>::
printAscii(std::ostream& out) const {
  out << "BBoxTree size = " << getSize() << '\n';
  if (_root) {
    _root->printAscii(out);
  }
}

//
// Private member functions.
//

template<int N, typename T>
inline
void
BBoxTree<N,T>::
build(const SizeType leafSize) {
  // A discrete uniform generator that uses the multiplicative congruential
  // method.
  typedef numerical::DiscreteUniformGeneratorMc32<> DiscreteUniformGenerator;
  // A continuous uniform generator.
  typedef numerical::ContinuousUniformGeneratorClosed<DiscreteUniformGenerator, 
    Number> ContinuousUniformGenerator;

  if (isEmpty()) {
    return;
  }

  // Make a vector of bounding box centers.
  std::vector<Point> centers(getSize());
  for (int i = 0; i != getSize(); ++i) {
    centers[i] = _boxes[i].getLowerCorner();
    centers[i] += _boxes[i].getUpperCorner();
    centers[i] /= 2;
  }

  //
  // CONTINUE.
  // Make sure that the centers are all distinct.  This is only necessary 
  // because of the way I build the data structure.  I need to come back and
  // fix this.  This is a dirty hack.
  //
  // Check for the degenerate case.
  if (getSize() != 0) {
    std::vector<Point*> sorted(getSize());
    for (int i = 0; i != getSize(); ++i) {
      sorted[i] = &centers[i];
    }
    // Composite compare in the first coordinate.
    ads::binary_compose_binary_unary<ads::less_composite<N,Point>,
      ads::Dereference<Point*, Point>, ads::Dereference<Point*, Point> > comp;
    comp.outer().set(0);
    // Uniform generator.
    DiscreteUniformGenerator discreteGenerator;
    ContinuousUniformGenerator continuousGenerator(&discreteGenerator);
    int count;
    do {
      count = 0;
      std::sort(sorted.begin(), sorted.end(), comp);
      typename std::vector<Point*>::const_iterator next = sorted.begin();
      ++next;  // We already checked for zero size.
      for (typename std::vector<Point*>::const_iterator i = sorted.begin();
	   next != sorted.end(); ++i, ++next) {
	// If the center points are not distinct.
	if (! comp(*i, *next)) {
	  ++count;
	  // Move the latter point to the right by an "infinitessimal" 
	  // amount.
	  Number old = (**next)[0];
	  if ((**next)[0] != 0) {
	    (**next)[0] += (**next)[0] * (1 + 9 * discreteGenerator()) *
	      std::numeric_limits<Number>::epsilon();
	  }
	  else {
	    (**next)[0] += (1 + 9 * discreteGenerator()) * 
	      std::numeric_limits<Number>::epsilon();
	  }
	  assert((**next)[0] != old);
	}
      }
    } while (count != 0);
  }

  // Make N vectors of pointers to the centers.
  ads::FixedArray< N, std::vector<const Point*> > sorted;
  for (int n = 0; n != N; ++n) {
    sorted[n].resize(getSize());
  }
  for (int i = 0; i != getSize(); ++i) {
    sorted[0][i] = &centers[i];
  }
  for (int n = 1; n != N; ++n) {
    sorted[n] = sorted[0];
  }

  // Sort these vectors in each coordinate.
  ads::binary_compose_binary_unary<ads::less_composite<N,Point>,
    ads::Dereference<const Point*, Point>, 
    ads::Dereference<const Point*, Point> > comp;
  for (int n = 0; n != N; ++n) {
    comp.outer().set(n);
    std::sort(sorted[n].begin(), sorted[n].end(), comp);
    typename std::vector<const Point*>::const_iterator 
      next = sorted[n].begin();
    if (next != sorted[n].end()) {
      ++next;
      for (typename std::vector<const Point*>::const_iterator 
	     i = sorted[n].begin(); next != sorted[n].end(); ++i, ++next) {
	// CONTINUE
	if (! comp(*i, *next)) {
	  std::cerr << **i << "\n" << **next << "\n";
	}
	assert(comp(*i, *next));
      }
    }
  }

  // Make the tree.
  if (getSize() > leafSize) {
    _root = new Branch(&centers[0], sorted, leafSize);
  }
  else {
    _root = new Leaf(&centers[0], sorted[0].begin(), sorted[0].end());
  }

  // Compute the domains at each branch and leaf.
  _root->computeDomain(_boxes);
}




//! Get the indices of the bounding boxes that contain the point.
template<int N, typename T>
template<typename IntegerOutputIter>
inline
void
BBoxTree<N,T>::
computePointQuery(IntegerOutputIter iter, const Point& x) const {
  // Get the leaves containing bounding boxes that might contain the point.
  _leaves.clear();
  if (_root) {
    _root->computePointQuery(_leaves, x);
  }

  // From the relevant leaves, find the bounding boxes that actually overlap.
  IndexConstIterator i, iEnd;
  for (typename std::vector<const Leaf*>::const_iterator leafIter = 
	  _leaves.begin(); leafIter != _leaves.end(); ++leafIter) {
    iEnd = (*leafIter)->getIndicesEnd();
    for (i = (*leafIter)->getIndicesBeginning(); i != iEnd; ++i) {
      if (_boxes[*i].isIn(x)) {
	*iter++ = *i;
      }
    }
  }
}




//! Get the indices of the bounding boxes that overlap the window.
template<int N, typename T>
template<typename IntegerOutputIter>
inline
void
BBoxTree<N,T>::
computeWindowQuery(IntegerOutputIter iter, const BBox& window) const {
  // Get the leaves containing bounding boxes that might overlap the window.
  _leaves.clear();
  if (_root) {
    _root->computeWindowQuery(_leaves, window);
  }

  // From the relevant leaves, find the bounding boxes that actually overlap.
  IndexConstIterator i, iEnd;
  for (typename std::vector<const Leaf*>::const_iterator leafIter = 
	  _leaves.begin(); leafIter != _leaves.end(); ++leafIter) {
    iEnd = (*leafIter)->getIndicesEnd();
    for (i = (*leafIter)->getIndicesBeginning(); i != iEnd; ++i) {
      if (doOverlap(window, _boxes[*i])) {
	*iter++ = *i;
      }
    }
  }
}




//! Get the indices of the bounding boxes that might contain objects of minimum distance.
template<int N, typename T>
template<typename IntegerOutputIter>
inline
void
BBoxTree<N,T>::
computeMinimumDistanceQuery(IntegerOutputIter iter, const Point& x) const {
  // If the tree is empty, do nothing.
  if (! _root) {
    return;
  }

  // Get the leaves containing bounding boxes that might contain objects of 
  // minimum distance.
  _leaves.clear();
  Number upperBound = std::numeric_limits<Number>::max();
  _root->computeMinimumDistanceQuery(_leaves, x, &upperBound);
  // There must be some entity of minimum distance.
  assert(_leaves.size() != 0);

  //
  // From the relevant leaves, find the bounding boxes that might contain
  // objects of minimum distance.
  //
  IndexConstIterator i, iEnd;
  Number d;
  // First determine the final upper bound on the distance.
  for (typename std::vector<const Leaf*>::const_iterator leafIter = 
	 _leaves.begin(); leafIter != _leaves.end(); ++leafIter) {
    iEnd = (*leafIter)->getIndicesEnd();
    for (i = (*leafIter)->getIndicesBeginning(); i != iEnd; ++i) {
      d = computeUpperBoundOnUnsignedDistance(_boxes[*i], x);
      if (d < upperBound) {
	upperBound = d;
      }
    }
  }
  assert(upperBound != std::numeric_limits<Number>::max());
  // Increase the upper bound to avoid missing boxes due to round-off error.
  upperBound *= (1.0 + 10.0 * std::numeric_limits<Number>::epsilon());
  upperBound += 10.0 * std::numeric_limits<Number>::epsilon();
  //upperBound *= 1.1;

  // Then add all the boxes that have a distance lower bound that is no 
  // more than the final upper bound.
  for (typename std::vector<const Leaf*>::const_iterator leafIter = 
	  _leaves.begin(); leafIter != _leaves.end(); ++leafIter) {
    iEnd = (*leafIter)->getIndicesEnd();
    for (i = (*leafIter)->getIndicesBeginning(); i != iEnd; ++i) {
      d = computeLowerBoundOnUnsignedDistance(_boxes[*i], x);
      if (d <= upperBound) {
	*iter++ = *i;
      }
    }
  }
}
  
END_NAMESPACE_GEOM

// End of file.
