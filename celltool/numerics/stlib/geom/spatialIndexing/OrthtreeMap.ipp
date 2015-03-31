// -*- C++ -*-

#if !defined(__geom_spatialIndexing_OrthtreeMap_ipp__)
#error This file is an implementation detail of the class OrthtreeMap.
#endif

BEGIN_NAMESPACE_GEOM

//---------------------------------------------------------------------------
// Constructors etc.
//---------------------------------------------------------------------------

// Make an empty tree.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
OrthtreeMap(const Point& lowerCorner, const Point& extents,
	    Split split,
	    Merge merge,
	    Refine refine,
	    Coarsen coarsen,
	    Action action) :
  Base(),
  _lowerCorner(lowerCorner),
  _extents(),
  _split(split),
  _merge(merge),
  _refine(refine),
  _coarsen(coarsen),
  _action(action) {
  LOKI_STATIC_CHECK(int(Dimension) == int(Key::Dimension), DimensionMismatch);
  LOKI_STATIC_CHECK(int(MaximumLevel) == int(Key::MaximumLevel),
		    MaximumLevelMismatch);
  // Compute the leaf extents for each level.
  _extents[0] = extents;
  for (int i = 1; i < _extents.size(); ++i) {
    _extents[i] = _extents[i - 1];
    _extents[i] *= 0.5;
  }
}

// Copy constructor.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
OrthtreeMap(const OrthtreeMap& other) :
  Base(other),
  _lowerCorner(other._lowerCorner),
  _extents(other._extents),
  _split(other._split),
  _merge(other._merge),
  _refine(other._refine),
  _coarsen(other._coarsen),
  _action(other._action) {
}


// Assignment operator.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>&
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
operator=(const OrthtreeMap& other) {
  if (this != &other) {
    Base::operator=(other);
    _lowerCorner = other._lowerCorner;
    _extents = other._extents;
    _split = other._split;
    _merge = other._merge;
    _refine = other._refine;
    _coarsen = other._coarsen;
    _action = other._action;
  }
  return *this;
}


//---------------------------------------------------------------------------
// Operations on all nodes.
//---------------------------------------------------------------------------

// Apply the function to the element of each node.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _Function>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
apply(_Function f) {
#pragma omp parallel
  {
    // Partition the nodes.
    iterator start, finish;
    partition(&start, &finish);
    // Apply the function to our partition.
    for (iterator i = start; i != finish; ++i) {
      _apply(f, i);
    }
  }
}


// Performing refinement with the supplied criterion.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _Function>
inline
int
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
refine(_Function refinePredicate, bool areBalancing) {
#ifdef _OPENMP
  //
  // Threaded version.
  //

  // Partition the nodes.  We need to do this outside the concurrent block
  // because it may change the number of nodes.
  ads::Array<1, iterator> delimiters(omp_get_max_threads() + 1);
  partition(&delimiters);

  int count = 0;
#pragma omp parallel reduction (+:count)
  {
    // Get our partition
    const int thread = omp_get_thread_num();
    const iterator start = delimiters[thread];
    const iterator finish = delimiters[thread + 1];

    // The set of leaves that we may refine.
    std::vector<iterator> mayRefine;
    std::back_insert_iterator<std::vector<iterator> > 
      outputIterator(mayRefine);
    for (iterator i = start; i != finish; ++i) {
      mayRefine.push_back(i);
    }

    std::vector<iterator> doRefine;
    while (! mayRefine.empty()) {
      // Determine the nodes that should be refined.
      while (! mayRefine.empty()) {
	iterator i = mayRefine.back();
	mayRefine.pop_back();
	if (canBeRefined(i) && evaluate(refinePredicate, i)) {
	  doRefine.push_back(i);
	}
      }
      // Perform the splitting.  This needs to be performed in a critical
      // section as it modifies the container.
#pragma omp critical
      for (typename std::vector<iterator>::const_iterator i = doRefine.begin();
	   i != doRefine.end(); ++i) {
	split(*i, outputIterator);
      }
      count += doRefine.size();
      doRefine.clear();
    }
  }
#else
  //
  // Serial version.
  //
  int count = 0;
  // The set of leaves that we may refine.
  std::vector<iterator> mayRefine;
  std::back_insert_iterator<std::vector<iterator> > 
    outputIterator(mayRefine);
  for (iterator i = begin(); i != end(); ++i) {
    mayRefine.push_back(i);
  }

  while (! mayRefine.empty()) {
    iterator i = mayRefine.back();
    mayRefine.pop_back();
    if (canBeRefined(i) && evaluate(refinePredicate, i)) {
      split(i, outputIterator);
      ++count;
    }
  }
#endif

  if (areBalancing) {
    count += balance();
  }

  return count;
}


// Perform coarsening with the supplied criterion until no more nodes can 
// be coarsened.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _Function>
inline
int
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
coarsen(_Function coarsenPredicate, bool areBalancing) {
  int countCoarsened = 0;
  int c;
  // Perform sweeps until no more nodes are coarsened.
  do {
    c = coarsenSweep(coarsenPredicate, areBalancing);
    countCoarsened += c;
  } while (c != 0);
  return countCoarsened;
}


// CONTINUE: Consider a solution that uses vector.
// Performing refinement to balance the tree.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
int
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
balance() {
  // The set of leaves that we may refine.  We need a set instead of a vector
  // because when we refine a node we add the lower level neighbors as 
  // well as the new children.  Some of the neighbors may already be in the 
  // set.
  std::set<iterator, CompareIterator> mayRefine;
  std::insert_iterator<std::set<iterator, CompareIterator> >
    insertIterator(mayRefine, mayRefine.end());
  for (iterator i = begin(); i != end(); ++i) {
    *insertIterator++ = i;
  }

  int count = 0;
  while (! mayRefine.empty()) {
    // Get a node.
    iterator i = *--mayRefine.end();
    mayRefine.erase(--mayRefine.end());
    // If we need to refine the node.
    if (needsRefinementToBalance(i->first)) {
      // Add neighbors to the set of nodes which may need refinement.
      getLowerNeighbors(i, insertIterator);
      // Refine the node and add the children to the set of nodes that may
      // need to be refined.
      split(i, insertIterator);
      ++count;
    }
  }

  return count;
}


// Performing a single sweep of coarsening with the supplied criterion.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _Function>
inline
int
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
coarsenSweep(_Function coarsenPredicate, bool areBalancing) {
#ifdef _OPENMP
  //
  // Threaded version.
  //
  // Partition the nodes.  We need to do this outside the concurrent block
  // because it may change the number of nodes.
  const int threads = omp_get_max_threads();
  ads::Array<1, iterator> delimiters(threads + 1);
  partitionMergeable(&delimiters);
  ads::Array<1, iterator> upper(threads, delimiters.begin() + 1,
				delimiters.end());
  for (int i = 0; i != upper.size() - 1; ++i) {
    --upper[i];
  }

  int countCoarsened = 0;
#pragma omp parallel reduction (+:countCoarsened)
  {
    // Get our partition
    const int thread = omp_get_thread_num();
    const iterator start = delimiters[thread];
    const iterator finish = upper[thread];

    // The mergeable groups.
    std::vector<iterator> mergeable;
    std::back_insert_iterator<std::vector<iterator> > 
      outputIterator(mergeable);
    if (areBalancing) {
      getMergeableGroupsBalanced(outputIterator, start, finish);
    }
    else {
      getMergeableGroups(outputIterator, start, finish);
    }
    std::vector<iterator> doMerge;
    Key key;
    for (typename std::vector<iterator>::const_iterator i = mergeable.begin(); 
	 i != mergeable.end(); ++i) {
      // If the group should be coarsened.
      if (evaluate(coarsenPredicate, *i)) {
	doMerge.push_back(*i);
      }
    }
    // Perform the merging.  This needs to be performed in a critical
    // section as it modifies the container.
#pragma omp critical
    for (typename std::vector<iterator>::const_iterator i = doMerge.begin();
	 i != doMerge.end(); ++i) {
      merge(*i);
    }
    countCoarsened += doMerge.size();
  }
#else
  //
  // Serial version.
  //
  int countCoarsened = 0;
  // The mergeable groups.
  std::vector<iterator> mergeable;
  std::back_insert_iterator<std::vector<iterator> > 
    outputIterator(mergeable);
  if (areBalancing) {
    getMergeableGroupsBalanced(outputIterator);
  }
  else {
    getMergeableGroups(outputIterator);
  }
  Key key;
  for (typename std::vector<iterator>::const_iterator i = mergeable.begin(); 
       i != mergeable.end(); ++i) {
    // If the group should be coarsened.
    if (evaluate(coarsenPredicate, *i)) {
      merge(*i);
      ++countCoarsened;
    }
  }
#endif
  return countCoarsened;
}



//---------------------------------------------------------------------------
// Accessors.
//---------------------------------------------------------------------------

// Compute the lower corner of the leaf.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
computeLowerCorner(const Key& key, Point* lowerCorner) const {
  const Point& extents = getExtents(key);
  *lowerCorner = key.getCoordinates();
  *lowerCorner *= extents;
  *lowerCorner += _lowerCorner;
}


// Get the keys that are parents of 2^Dimension leaves.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getParentKeys(_OutputIterator parentKeys) const {
  // For each leaf.
  for (const_iterator i = begin(); i != end(); ++i) {
    // If this is a local lower corner.
    if (hasParent(i->first) && isLowerCorner(i->first)) {
      // Get the parent key.
      Key key = i->first;
      key.transformToParent();
      // See if the sibling leaves are in the tree.
      bool siblingsArePresent = true;
      for (int i = 1; i != NumberOfOrthants; ++i) {
	key.transformToChild(i);
	if (! count(key)) {
	  siblingsArePresent = false;
	  break;
	}
	key.transformToParent();
      }
      if (siblingsArePresent) {
	*parentKeys++ = key;
      }
    }
  }
}


// Get the keys that are parents of 2^Dimension leaves and would result in 
// a balanced tree under merging.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getParentKeysBalanced(_OutputIterator parentKeys) const {
  // Get the parent keys.
  std::vector<Key> keys;
  getParentKeys(std::back_inserter(keys));
  // Check which merging operations would result in a balanced tree.
  for (typename std::vector<Key>::const_iterator i = keys.begin(); 
       i != keys.end(); ++i) {
    // If merging would not imbalance the tree.
    if (! needsRefinementToBalance(*i)) {
      // Output that parent key.
      *parentKeys++ = *i;
    }
  }
}


// Return true if the tree is balanced.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
bool
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
isBalanced() const {
  // Check each node.
  for (const_iterator i = begin(); i != end(); ++i) {
    if (needsRefinementToBalance(i->first)) {
      return false;
    }
  }
  return true;
}


// Get the adjacent neighbors in the specified direction in a balanced tree.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getBalancedNeighbors(const const_iterator node, _OutputIterator output)
  const {
  // For each direction.
  for (int direction = 0; direction != 2 * Dimension; ++direction) {
    // Get the neighbors in that direction.
    getBalancedNeighbors(node, direction, output);
  }
}


// Get the adjacent neighbors in the specified direction in a balanced tree.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getBalancedNeighbors(const const_iterator node, const int neighborDirection,
		     _OutputIterator output) const {
  //
  // Get the key of the neighbor at the same level.
  //
  Key key(node->first);
  // If there is no adjacent neighbor in this direction.
  if (! hasNeighbor(key, neighborDirection)) {
    // Do nothing.
    return;
  }
  key.transformToNeighbor(neighborDirection);

  // Search for the neighbor.  Find the last element that is not
  // greater than the key.
  const_iterator neighbor = findAncestor(key);

  // If there is a neighbor at the same level or one level lower.
  if (neighbor->first.getLevel() <= key.getLevel()) {
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
    // Check that the level differs by at most 1.
    assert(key.getLevel() - neighbor->first.getLevel() <= 1);
#endif
    *output++ = neighbor;
  }
  // The adjacent neighbors are at a higher level.
  else {
    Key child;
    // Get the adjacent neighbors by refining the same-level neighbor key.
    // The refinement direction is the opposite of the adjacent direction.
    const int refinementDirection = neighborDirection -
      2 * (neighborDirection % 2) + 1;
    const int coordinate = refinementDirection / 2;
    const int direction = refinementDirection % 2;
    for (int i = 0; i != NumberOfOrthants; ++i) {
      // If this is refinement in the desired direction.
      if ((i >> coordinate) % 2 == direction) {
	child = key;
	child.transformToChild(i);
	const_iterator c = find(child);
	*output++ = c;
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
	assert(c != end() && c->first.getLevel() - 1 == key.getLevel());
#endif
      }
    }
  }
}


// Return true if the element needs refinement in order to balance the tree.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
bool
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
needsRefinementToBalance(const Key& key) const {
  // If there are no levels more than one higher than this node's level.
  if (key.getLevel() > MaximumLevel - 2) {
    return false;
  }
  const Level maximumAllowedLevel = key.getLevel() + 1;
  Key neighbor, child;
  // Check for neighbors in each direction.
  for (int adjacentDirection = 0; adjacentDirection != 2 * Dimension;
       ++adjacentDirection) {
    // The neighbor at the same level.
    neighbor = key;
    if (! hasNeighbor(neighbor, adjacentDirection)) {
      continue;
    }
    neighbor.transformToNeighbor(adjacentDirection);
    // The refinement direction is the opposite of the adjacent direction.
    const int refinementDirection = adjacentDirection -
      2 * (adjacentDirection % 2) + 1;
    const int coordinate = refinementDirection / 2;
    const int direction = refinementDirection % 2;
    for (int i = 0; i != NumberOfOrthants; ++i) {
      if ((i >> coordinate) % 2 == direction) {
	child = neighbor;
	child.transformToChild(i, 2);
	const_iterator c = find(child);
	if (c != end() && c->first.getLevel() > maximumAllowedLevel) {
	  return true;
	}
      }
    }
  }

  // We did not find any small adjacent neighbors.
  return false;
}


// Find the node that matches the code.  If the node is not in the tree, 
// find its ancestor.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
typename OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
		     _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::const_iterator
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
findAncestor(const Key& key) const {
  // Search for the neighbor.  Find the last element that is not
  // greater than the key.
  const_iterator neighbor = Base::lower_bound(key);
  if (neighbor->first.getCode() != key.getCode()) {
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
    assert(neighbor != begin());
#endif
    --neighbor;
  }
  return neighbor;
}


// Return true if the node has a higher level neighbor in the specified 
// direction.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
bool
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
hasHigherNeighbor(const const_iterator node, const int direction) const {
  // If there is no neighbor in that direction.
  if (! hasNeighbor(node, direction)) {
    // Then there are no higher level neighbors in that direction.
    return false;
  }
  // Find the node that intersects that intersects the neighbor at the 
  // same level.
  Key neighborKey(node->first);
  neighborKey.transformToNeighbor(direction);
  const_iterator neighbor = findAncestor(neighborKey);
  // Check the level of that node.
  return neighbor->first.getLevel() > node->first.getLevel();
}


// Return true if the node has a higher level neighbor in the specified 
// direction.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
bool
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
hasHigherNeighbor(const const_iterator node) const {
  // Check each direction.
  for (int direction = 0; direction != 2 * Dimension; ++direction) {
    if (hasHigherNeighbor(node, direction)) {
      return true;
    }
  }
  return false;
}


//---------------------------------------------------------------------------
// Manipulators.
//---------------------------------------------------------------------------


// Insert a value type.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
typename OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
		     _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::iterator
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
insert(const value_type& x) {
  // Try to insert the value.
  std::pair<iterator, bool> result = Base::insert(x);
  // Assert that we were able to do so.
  assert(result.second);
  return result.first;
}


// Refine an element.  Get the children.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _Function, typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
split(_Function splitFunctor, const iterator parent,
      _OutputIterator children) {
  // Erase the parent.  We have to do this first because one of the children
  // will have the same code.
  Key parentKey = parent->first;
  Element parentElement = parent->second;
  erase(parent);

  Element element = Element();
  for (int i = 0; i != NumberOfOrthants; ++i) {
    // Make the key.
    Key key = parentKey;
    key.transformToChild(i);
    // Make the element.
    evaluateSplit(splitFunctor, parentElement, i, key, &element);
    // Insert the node.
    *children++ = insert(key, element);
  }
}

// Coarsen leaves given the parent key.  Return the coarsened leaf.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _Function>
inline
typename OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
		     _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::iterator
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
merge(_Function mergeFunctor, iterator firstChild) {
  // The parent key.
  Key parentKey = firstChild->first;
  parentKey.transformToParent();

  // Coarsen the element.
  Element parentElement = Element();
  evaluateMerge(mergeFunctor, firstChild, parentKey, &parentElement);

  // Erase the children.
  for (int i = 0; i != NumberOfOrthants; ++i) {
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
    assert(firstChild != end());
    assert(firstChild->first.getLevel() == parentKey.getLevel() + 1);
#endif
    // Increment the iterator before we erase the node. (Erasing invalidates
    // the iterator.)
    iterator node = firstChild;
    ++firstChild;
    erase(node);
  }
  // Insert the parent.
  return insert(value_type(parentKey, parentElement));
}


// Get the adjacent neighbors which have lower levels.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getLowerNeighbors(const iterator node, _OutputIterator i) {
  Key key;
  // For each neighbor direction.
  for (int n = 0; n != 2 * Dimension; ++n) {
    key = node->first;
    // If there is an adjacent neighbor in this direction.
    if (hasNeighbor(key, n)) {
      // Get the key of the neighbor at the same level.
      key.transformToNeighbor(n);
      // Search for the neighbor.  Find the last element that is not
      // greater than the key.
      iterator neighbor = findAncestor(key);
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
      assert(neighbor->first.getLevel() > node->first.getLevel() ||
	     areAdjacent(node->first, neighbor->first));
#endif
      // If the neighbor has a lower level.
      if (neighbor->first.getLevel() < node->first.getLevel()) {
	// Record the neighbor.
	*i++ = neighbor;
      }
    }
  }
}


// Find the node that matches the code.  If the node is not in the tree, find 
// its ancestor.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
typename OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
		     _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::iterator
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
findAncestor(const Key& key) {
  // Search for the neighbor.  Find the last element that is not
  // greater than the key.
  iterator neighbor = Base::lower_bound(key);
  if (neighbor->first.getCode() != key.getCode()) {
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
    assert(neighbor != begin());
#endif
    --neighbor;
  }
  return neighbor;
}


// Get the mergeable groups of 2^Dimension nodes.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getMergeableGroups(_OutputIterator lowerCornerNodes, iterator start,
		   iterator finish) {
  // For each leaf.
  for (iterator i = start; i != finish; ++i) {
    // If this is a local lower corner.
    if (hasParent(i->first) && isLowerCorner(i->first)) {
      iterator j = i;
      ++j;
      bool isMergeable = true;
      for (int n = 1; n != NumberOfOrthants && isMergeable; ++n, ++j) {
	if (i->first.getLevel() != j->first.getLevel()) {
	  isMergeable = false;
	}
      }
      if (isMergeable) {
	*lowerCornerNodes++ = i;
      }
    }
  }
}


// Get the mergeable groups of 2^Dimension nodes whose merging would result 
// in a balanced tree.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
template<typename _OutputIterator>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
getMergeableGroupsBalanced(_OutputIterator lowerCornerNodes, iterator start,
			   iterator finish) {
  // Get the mergeable groups.
  std::vector<iterator> mergeable;
  getMergeableGroups(std::back_inserter(mergeable), start, finish);
  // Check which merging operations would result in a balanced tree.
  Key key;
  for (typename std::vector<iterator>::const_iterator i = mergeable.begin(); 
       i != mergeable.end(); ++i) {
    key = (*i)->first;
    key.transformToParent();
    // If merging would not imbalance the tree.
    if (! needsRefinementToBalance(key)) {
      // Output that group.
      *lowerCornerNodes++ = *i;
    }
  }
}



//---------------------------------------------------------------------------
// File I/O.
//---------------------------------------------------------------------------

// Print the keys and the elements.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
	    _Split, _Merge, _Refine, _Coarsen, _Action, _Key>::
print(std::ostream& out) const {
  for (const_iterator i = begin(); i != end(); ++i) {
    out << i->first << "\n" << i->second << "\n";
  }
}



//---------------------------------------------------------------------------
// Free functions.
//---------------------------------------------------------------------------

//! Define the VTK output type.
/*!
  This class is a level of indirection that allows one to define supported
  output types.  By default, element data is not printed when writing output
  in VTK format.  Below we define output for \c double, \c float, and \c int.
  If a user-defined element can be printed in one of these formats, 
  specialize this class with that element type.
*/
template<typename _T>
struct ElementVtkOutput {
  //! For un-supported types, the output type is void*.
  typedef void* Type;
};

//! Define the VTK output type.
template<>
struct ElementVtkOutput<double> {
  //! The output type is double.
  typedef double Type;
};

//! Define the VTK output type.
template<>
struct ElementVtkOutput<float> {
  //! The output type is float.
  typedef float Type;
};

//! Define the VTK output type.
template<>
struct ElementVtkOutput<int> {
  //! The output type is int.
  typedef int Type;
};


// Print the bounding boxes for the leaves in VTK format.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printVtkUnstructuredGrid
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  printVtkUnstructuredGrid(Loki::Int2Type<_Dimension <= 3>(), out, x);
}

// Print a message that VTK output in this dimension is not supported.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printVtkUnstructuredGrid
(Loki::Int2Type<false> /*supported dimension*/,
 std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  out << _Dimension << "-D VTK output is not supported.\n";
}



template<typename _Orthtree>
class PrintElementsVtkDataArray {
public:

  // Print the element data array.
  void
  operator()(std::ostream& out, const _Orthtree& orthtree) {
    typedef typename ElementVtkOutput<typename _Orthtree::Element>::Type Type;
    Type x = Type();
    print(out, orthtree, x);
  }

private:

  // Print nothing for unsupported element types.
  template<typename _T>
  void
  print(std::ostream& out, const _Orthtree& orthtree, _T /*dummy*/) {
  }

  // Print the element data array.
  //template<>
  void
  print(std::ostream& out, const _Orthtree& orthtree, double /*dummy*/) {
    out << "<DataArray type=\"Float64\" Name=\"element\">\n";
    printTheRest(out, orthtree);
  }

  // Print the element data array.
  //template<>
  void
  print(std::ostream& out, const _Orthtree& orthtree, float /*dummy*/) {
    out << "<DataArray type=\"Float32\" Name=\"element\">\n";
    printTheRest(out, orthtree);
  }

  // Print the element data array.
  //template<>
  void
  print(std::ostream& out, const _Orthtree& orthtree, int /*dummy*/) {
    out << "<DataArray type=\"Int32\" Name=\"element\">\n";
    printTheRest(out, orthtree);
  }

  void
  printTheRest(std::ostream& out, const _Orthtree& x) {
    typedef typename _Orthtree::const_iterator Iterator;

    for (Iterator i = x.begin(); i != x.end(); ++i) {
      out << i->second << "\n";
    }
    out << "</DataArray>\n";
  }
};


// Print the element.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printElementsVtkDataArray
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> Orthtree;
  PrintElementsVtkDataArray<Orthtree> printer;
  printer(out, x);
}

// CONTINUE
#if 0
// Print the element.
template<int _Dimension, int _MaximumLevel,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printElementsVtkDataArray
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, double, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, double, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> Orthtree;
  typedef typename Orthtree::const_iterator Iterator;

  out << "<DataArray type=\"Float64\" Name=\"element\">\n";
  for (Iterator i = x.begin(); i != x.end(); ++i) {
    out << i->second << "\n";
  }
  out << "</DataArray>\n";
}


// Print the element.
template<int _Dimension, int _MaximumLevel,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printElementsVtkDataArray
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, float, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, float, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> Orthtree;
  typedef typename Orthtree::const_iterator Iterator;

  out << "<DataArray type=\"Float32\" Name=\"element\">\n";
  for (Iterator i = x.begin(); i != x.end(); ++i) {
    out << i->second << "\n";
  }
  out << "</DataArray>\n";
}


// Print the element.
template<int _Dimension, int _MaximumLevel,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printElementsVtkDataArray
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, int, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, int, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> Orthtree;
  typedef typename Orthtree::const_iterator Iterator;

  out << "<DataArray type=\"Int32\" Name=\"element\">\n";
  for (Iterator i = x.begin(); i != x.end(); ++i) {
    out << i->second << "\n";
  }
  out << "</DataArray>\n";
}
#endif

// Print the bounding boxes for the leaves in VTK format.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
void
printVtkUnstructuredGrid
(Loki::Int2Type<true> /*supported dimension*/,
 std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> Orthtree;
  typedef typename Orthtree::const_iterator Iterator;
  typedef typename Orthtree::Point Point;

  LOKI_STATIC_CHECK(_Dimension >= 1 && _Dimension <= 3, BadDimension);

  Point lowerCorner, p;
  const int size = x.size();

  // Header.
  out << "<?xml version=\"1.0\"?>\n";
  // Begin VTKFile.
  out << "<VTKFile type=\"UnstructuredGrid\">\n";
  // Begin UnstructuredGrid.
  out << "<UnstructuredGrid>\n";
  // Begin Piece.
  out << "<Piece NumberOfPoints=\"" << Orthtree::NumberOfOrthants * size
      << "\" NumberOfCells=\"" << size << "\">\n";

  // Begin PointData.
  out << "<PointData>\n";
  // End PointData.
  out << "</PointData>\n";

  // Begin CellData.
  out << "<CellData>\n";
  // The elements.
  printElementsVtkDataArray(out, x);
  // The level.
  out << "<DataArray type=\"Int32\" Name=\"level\">\n";
  for (Iterator i = x.begin(); i != x.end(); ++i) {
    out << int(i->first.getLevel()) << "\n";
  }
  out << "</DataArray>\n";
  // The coordinates.
  for (int d = 0; d != _Dimension; ++d) {
    out << "<DataArray type=\"Int32\" Name=\"coordinate" << d << "\">\n";
    for (Iterator i = x.begin(); i != x.end(); ++i) {
      out << int(i->first.getCoordinates()[d]) << "\n";
    }
    out << "</DataArray>\n";
  }
  // The rank.
  out << "<DataArray type=\"Int32\" Name=\"rank\">\n";
  for (int rank = 0; rank != size; ++rank) {
    out << rank << "\n";
  }
  out << "</DataArray>\n";
  // End CellData.
  out << "</CellData>\n";

  // Begin Points.
  out << "<Points>\n";
  out << "<DataArray type=\"Float32\" NumberOfComponents=\"3\">\n";
  for (Iterator i = x.begin(); i != x.end(); ++i) {
    const Point& extents = x.getExtents(i->first);
    x.computeLowerCorner(i->first, &lowerCorner);
    if (_Dimension == 1) {
      out << lowerCorner << " 0 0\n";
      p = lowerCorner;
      p[0] += extents[0];
      out << p << " 0 0\n";
    }
    else if (_Dimension == 2) {
      out << lowerCorner << " 0\n";
      p = lowerCorner;
      p[0] += extents[0];
      out << p << " 0\n";
      out << lowerCorner + extents << " 0\n";
      p = lowerCorner;
      p[1] += extents[1];
      out << p << " 0\n";
    }
    else if (_Dimension == 3) {
      // 0
      out << lowerCorner << "\n";
      // 1
      p = lowerCorner;
      p[0] += extents[0];
      out << p << "\n";
      // 2
      p[1] += extents[1];
      out << p << "\n";
      // 3
      p[0] -= extents[0];
      out << p << "\n";
      // 4
      p = lowerCorner;
      p[2] += extents[2];
      out << p << "\n";
      // 5
      p[0] += extents[0];
      out << p << "\n";
      // 6
      p[1] += extents[1];
      out << p << "\n";
      // 7
      p[0] -= extents[0];
      out << p << "\n";
    }
    else {
      assert(false);
    }
  }
  out << "</DataArray>\n";
  // End Points.
  out << "</Points>\n";

  // Begin Cells.
  out << "<Cells>\n";
  out << "<DataArray type=\"Int32\" Name=\"connectivity\">\n";
  for (int n = 0; n != size; ++n) {
    for (int i = 0; i != Orthtree::NumberOfOrthants; ++i) {
      out << Orthtree::NumberOfOrthants * n + i << " ";
    }
    out << "\n";
  }
  out << "</DataArray>\n";
  out << "<DataArray type=\"Int32\" Name=\"offsets\">\n";
  for (int n = 1; n <= size; ++n) {
    out << Orthtree::NumberOfOrthants * n << "\n";
  }
  out << "</DataArray>\n";
  out << "<DataArray type=\"UInt8\" Name=\"types\">\n";
  for (int n = 0; n != size; ++n) {
    if (_Dimension == 1) {
      // Each cell is a line.
      out << "3\n";
    }
    else if (_Dimension == 2) {
      // Each cell is a quad.
      out << "9\n";
    }
    else if (_Dimension == 3) {
      // Each cell is a hexahedron.
      out << "12\n";
    }
    else {
      assert(false);
    }
  }
  out << "</DataArray>\n";
  // End Cells.
  out << "</Cells>\n";

  // End Piece.
  out << "</Piece>\n";

  // End UnstructuredGrid.
  out << "</UnstructuredGrid>\n";

  // End VTKFile.
  out << "</VTKFile>\n";
}

  
END_NAMESPACE_GEOM

// End of file.
