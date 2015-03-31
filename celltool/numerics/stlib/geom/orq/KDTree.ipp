// -*- C++ -*-

/*! 
  \file KDTree.ipp
  \brief A class for a kd-tree in N-D.
*/

#if !defined(__geom_KDTree_ipp__)
#error This file is an implementation detail of the class KDTree.
#endif

BEGIN_NAMESPACE_GEOM

//-----------------------------KDTreeBranch-------------------------------
  
//
// Constructors
//
  
// Construct from a set of grid elements.
template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
KDTreeBranch(const ads::FixedArray<N, std::vector<Record> >& sorted,
	     const SizeType leafSize) {
#ifdef DEBUG_KDTree
  // There must be more records than in a single leaf.
  assert(SizeType(sorted[0].size()) > leafSize);
  // Each of the sorted arrays should be of the same size.
  for (int n = 1; n != N; ++n) {
    assert(sorted[n].size() == sorted[0].size());
  }
#endif
  //
  // Determine the splitting direction.
  //

  ads::FixedArray<N, Key> spreads;
  for (int n = 0; n != N; ++n) {
    spreads[n] = getMultiKey(sorted[n].back())[n] 
      - getMultiKey(sorted[n].front())[n];
  }

  _splitDimension = spreads.max_index();
      
  //
  // Name the input vectors.
  //

  const std::vector<Record>& splitSorted = sorted[_splitDimension];
 
  //
  // Compute the median.
  //

  const int medianIndex = sorted[0].size() / 2;
  const SizeType leftSize = medianIndex;
  const SizeType rightSize = sorted[0].size() - medianIndex;
  const MultiKey& medianPoint = getMultiKey(splitSorted[medianIndex]);
  _splitValue = medianPoint[_splitDimension];

  //
  // Vectors for the subtrees.
  //
  ads::FixedArray< N, std::vector<Record> > sub;
  for (int n = 0; n != N; ++n) {
    sub[n].reserve(rightSize);
  }
  typename std::vector<Record>::const_iterator iter;

  //
  // Make the left subtree.
  //

  std::copy(splitSorted.begin(), splitSorted.begin() + medianIndex, 
	    std::back_inserter(sub[_splitDimension]));

  // For each dimension except the split dimension.
  for (int n = 0; n != N; ++n) {
    if (n != _splitDimension) {
      for (iter = sorted[n].begin(); iter != sorted[n].end(); ++iter) {
	if (ads::less_composite_fcn<N>(_splitDimension, getMultiKey(*iter), 
				       medianPoint)) {
	  sub[n].push_back(*iter);
	}
      }
    }
  }

  // If the left subtree is a leaf.
  if (leftSize <= leafSize) {
    std::vector<Record> leftLeaf(splitSorted.begin(), 
				 splitSorted.begin() + leftSize);
    _left = new Leaf(leftLeaf);
  }
  else {
    _left = new KDTreeBranch(sub, leafSize);
  }

  for (int n = 0; n != N; ++n) {
    sub[n].clear();
  }

  //
  // Make the right subtree.
  //

  std::copy(splitSorted.begin() + medianIndex, splitSorted.end(),
	    std::back_inserter(sub[_splitDimension]));

  // For each dimension except the split dimension.
  for (int n = 0; n != N; ++n) {
    if (n != _splitDimension) {
      for (iter = sorted[n].begin(); iter != sorted[n].end(); ++iter) {
	if (! ads::less_composite_fcn<N>(_splitDimension, getMultiKey(*iter), 
					 medianPoint)) {
	  sub[n].push_back(*iter);
	}
      }
    }
  }

  // If the right subtree is a leaf.
  if (rightSize <= leafSize) {
    std::vector<Record> 
      rightLeaf(splitSorted.begin() + medianIndex, splitSorted.end());
    _right = new Leaf(rightLeaf);
  }
  else {
    _right = new KDTreeBranch(sub, leafSize);
  }
}


//
// Window queries.
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
typename KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQuery(RecordOutputIterator iter, const BBox& window) const {
  if (_splitValue < window.getLowerCorner()[_splitDimension]) {
    return _right->computeWindowQuery(iter, window);
  }
  else if (_splitValue > window.getUpperCorner()[_splitDimension]) {
    return _left->computeWindowQuery(iter, window);
  }
  return (_left->computeWindowQuery(iter, window) +
	   _right->computeWindowQuery(iter, window));
}


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
typename KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQuery(RecordOutputIterator iter, BBox* domain, const BBox& window)
  const {
  SizeType count = 0;

  // If the domain of the left sub-tree intersects the window.
  if (_splitValue >= window.getLowerCorner()[_splitDimension]) {
    // Make the domain of the left sub-tree.
    Key max = domain->getUpperCorner()[_splitDimension];
    domain->setUpperCoordinate(_splitDimension, _splitValue);

    // If the domain lies inside the window.
    if (window.isIn(*domain)) {
      // Report the records in the left sub-tree.
      count += _left->report(iter);
    }
    else {
      // Do a window query of the left sub-tree.
      count += _left->computeWindowQuery(iter, domain, window);
    }

    // Reset the domain.
    domain->setUpperCoordinate(_splitDimension, max);
  }

  // If the domain of the right sub-tree intersects the window.
  if (_splitValue <= window.getUpperCorner()[_splitDimension]) {
    // Make the domain of the right sub-tree.
    Key min = domain->getLowerCorner()[_splitDimension];
    domain->setLowerCoordinate(_splitDimension, _splitValue);

    // If the domain lies inside the window.
    if (window.isIn(*domain)) {
      // Report the records in the right sub-tree.
      count += _right->report(iter);
    }
    // If the domain intersects the window.
    else {
      // Do a window query of the right sub-tree.
      count += _right->computeWindowQuery(iter, domain, window);
    }

    // Reset the domain.
    domain->setLowerCoordinate(_splitDimension, min);
  }

  return count;
}


//
// Validity check.
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
bool
KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
isValid(const BBox& window) const {
  if (window.getLowerCorner()[_splitDimension] > _splitValue ||
      _splitValue > window.getUpperCorner()[_splitDimension]) {
    return false;
  }

  BBox win(window);
  win.setUpperCoordinate(_splitDimension, _splitValue);
  if (! _left->isValid(win)) {
    return false;
  }

  win = window;
  win.setLowerCoordinate(_splitDimension, _splitValue);
  if (! _right->isValid(win)) {
    return false;
  }
  
  return true;
}


//-----------------------------KDTreeLeaf-------------------------------


//
// Mathematical member functions
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
typename KDTreeLeaf<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
KDTreeLeaf<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQuery(RecordOutputIterator iter, const BBox& window) const {
  SizeType count = 0;
  ConstIterator recordsEnd = _records.end(); 
  for (ConstIterator i = _records.begin(); i != recordsEnd; ++i) {
    if (window.isIn(getMultiKey(*i))) {
      *(iter++) = (*i);
      ++count;
    }
  }
  return count;
}

  
template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
typename KDTreeLeaf<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::SizeType
KDTreeLeaf<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
computeWindowQuery(RecordOutputIterator iter, BBox* domain,
		   const BBox& window) const {
  SizeType count = 0;
  if (window.isIn(*domain)) {
    for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
      *(iter++) = (*i);
    }
    count += _records.size();
  }
  else {
    ConstIterator recordsEnd = _records.end(); 
    for (ConstIterator i = _records.begin(); i != recordsEnd; ++i) {
      if (window.isIn(getMultiKey(*i))) {
	*(iter++) = (*i);
	++count;
      }
    }
  }
  return count;
}







  

//-----------------------------KDTree-----------------------------------

//
// Constructors
//

template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
template<class InputIterator>
inline
KDTree<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
KDTree(InputIterator first, InputIterator last, const SizeType leafSize) :
  Base(),
  _root(0),
  _domain() {
  if (first == last) {
    std::vector<Record> empty;
    _root = new Leaf(empty);
    _domain.setLowerCorner(Point(Key(0)));
    _domain.setUpperCorner(Point(Key(-1)));
    return;
  }

  // Make N vectors of pointers to the records.
  ads::FixedArray<N, std::vector<Record> > sorted;
  while (first != last) {
    sorted[0].push_back(first);
    ++first;
  }
  for (int n = 1; n != N; ++n) {
    sorted[n] = sorted[0];
  }

  // Sort these vectors in each coordinate.
  LessThanComposite comparison;
  for (int n = 0; n != N; ++n) {
    comparison.set(n);
    std::sort(sorted[n].begin(), sorted[n].end(), comparison);
  }

  // Determine the domain.
  for (int n = 0; n != N; ++n) {
    _domain.setLowerCoordinate(n, getMultiKey(sorted[n].front())[n]);
    _domain.setUpperCoordinate(n, getMultiKey(sorted[n].back())[n]);
  }

  // The number of records.
  setSize(sorted[0].size());

  // Make the tree.
  if (getSize() > leafSize) {
    _root = new Branch(sorted, leafSize);
  }
  else {
    _root = new Leaf(sorted[0]);
  }
}
  

//
// File I/O
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
void
KDTree<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_RecordOutputIterator>::
put(std::ostream& out) const {
  out << getSize() << " records" 
      << '\n'
      << "domain = " << getDomain()
      << '\n';
  _root->put(out);
}

END_NAMESPACE_GEOM

// End of file.
