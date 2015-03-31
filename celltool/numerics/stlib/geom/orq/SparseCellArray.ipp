// -*- C++ -*-

#if !defined(__geom_SparseCellArray_ipp__)
#error This file is an implementation detail of the class SparseCellArray.
#endif

BEGIN_NAMESPACE_GEOM


//
// Constructors
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
void
SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
build() {
  typename VectorArray::index_type arrayExtents;
  for (int n = 0; n != N-1; ++n) {
    arrayExtents[n] = getExtents()[n];
  }
  _vectorArray.resize(arrayExtents);
}


//
// Memory usage.
//

  
template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
typename SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
getMemoryUsage() const {
  SizeType usage = 0;
  // The (N-1)-D array of sparse vectors.
  usage += _vectorArray.size() 
    * sizeof(SparseCellVector<Cell>);
  // For each sparse vector.
  for (typename VectorArray::const_iterator i = _vectorArray.begin();
	i != _vectorArray.end();
	++i) {
    // The memory for the structure of the cell.
    usage += i->size() * 
      sizeof(IndexAndCell<Cell>);
    for (typename SparseCellVector<Cell>::
	    const_iterator j = i->begin();
	  j != i->end();
	  ++j) {
      // The memory for the contents of the cell.
      usage += j->cell.size() * sizeof(Record);
    }
  }
  return usage;
}


//
// Mathematical member functions
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template< class OutputIterator >
inline
typename SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator iter, const BBox& window) const {
  //
  // Convert the multi-key to array index coordinates.
  //

  // A multi-index and the lower and upper bounds of the index window.
  MultiIndex mi, lo, hi;
  convertMultiKeyToIndices(window.getLowerCorner(), &lo);
  convertMultiKeyToIndices(window.getUpperCorner(), &hi);
  
  //  
  // Truncate the index window to lie within the cell array.
  //

  for (int n = 0; n != N; ++n) {
    lo[n] = std::max(0, lo[n]);
    hi[n] = std::min(getExtents()[n] - 1, hi[n]);
  }

  // The interior portion of the index window.
  BBox interior(lo + 1, hi - 1);

  // The number of records in the window.
  SizeType count = 0;
  typename Cell::const_iterator recordIterator, recordIteratorEnd;
  typename SparseCellVector<Cell>::const_iterator cellIterator, 
    cellIteratorEnd;

  //
  // Iterate over the cells in the index window.
  //

  // Array Index.
  ads::FixedArray<N-1,int> ai;
  int n = N-2;
  mi = lo;
  for (int i = 0; i != N-1; ++i) {
    ai[i] = mi[i];
  }
  while (mi[N-2] <= hi[N-2]) {
    if (n == 0) {
      for (mi[0] = lo[0]; mi[0] <= hi[0]; ++mi[0]) {
	ai[0] = mi[0];
	const SparseCellVector<Cell>& cell_vector = _vectorArray(ai);
	cellIterator = cell_vector.lower_bound(lo[N-1]);
	cellIteratorEnd = cell_vector.end();
	for (; cellIterator != cellIteratorEnd && cellIterator->index <= hi[N-1]; 
	      ++cellIterator) {
	  const Cell& cell = cellIterator->cell;
	  recordIterator = cell.begin();
	  recordIteratorEnd = cell.end();

	  // If this is an interior cell.
	  mi[N-1] = cellIterator->index;
	  if (interior.isIn(mi)) {
	    for (; recordIterator != recordIteratorEnd; ++recordIterator) {
	      // There is no need to check if the record is in the window.
	      *iter = *recordIterator;
	      ++iter;
	    }
	    count += cell.size();
	  }
	  else { // This is a boundary cell.
	    for (; recordIterator != recordIteratorEnd; ++recordIterator) {
	      if (window.isIn(getMultiKey(*recordIterator))) {
		*iter = *recordIterator;
		++iter;
		++count;
	      }
	    }
	  }
	}
      }
      ++n;
    }
    else if (mi[n-1] > hi[n-1]) {
      ai[n-1] = mi[n-1] = lo[n-1];
      ++mi[n];
      ++ai[n];
      ++n;
    }
    else {
      --n;
    }
  }

  return count;
}


// Indexing by multi-key.  Return a reference to a cell.
template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
typename SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::Cell&
SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
operator()(const MultiKey& multiKey) {
  // CONTINUE: this is not efficient.
  MultiIndex mi;
  convertMultiKeyToIndices(multiKey, &mi);
  ads::FixedArray<N-1,int> ai;
  for (int n = 0; n != N-1; ++n) {
    ai[n] = mi[n];;
  }
  return _vectorArray(ai).find(mi[N-1]);
}


//
// File IO
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
void
SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const
{
  Base::put(out);

  typename Cell::const_iterator recordIterator;
  typename SparseCellVector<Cell>::const_iterator vectorIterator;

  typename VectorArray::const_iterator 
    i = _vectorArray.begin(),
    iEnd = _vectorArray.end();
  for (; i != iEnd; ++i) {
    const SparseCellVector<Cell>& cellVector = *i;
    vectorIterator = cellVector.begin();
    for (; vectorIterator != cellVector.end(); ++vectorIterator) {
      const Cell& b = vectorIterator->cell;
      recordIterator = b.begin();
      while (recordIterator != b.end()) {
	out << getMultiKey(*(recordIterator++)) << '\n';
      }
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
