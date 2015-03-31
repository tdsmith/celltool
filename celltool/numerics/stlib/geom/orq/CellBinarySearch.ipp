// -*- C++ -*-

#if !defined(__geom_CellBinarySearch_ipp__)
#error This file is an implementation detail of the class CellBinarySearch.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical member functions
//

template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<typename OutputIterator>
inline
typename CellBinarySearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
CellBinarySearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator iter, const BBox& window) const {
  //
  // Convert the multi-key to array indices.
  //
  Index mi, lo, hi;
  convertMultiKeyToIndices(window.getLowerCorner(), &lo);
  convertMultiKeyToIndices(window.getUpperCorner(), &hi);


  //  
  // Truncate the index window to lie within the cell array.
  //
  for (int n = 0; n != N-1; ++n) {
    lo[n] = std::max(0, lo[n]);
    hi[n] = std::min(getCellArrayExtents()[n] - 1, hi[n]);
  }

  // The interior portion of the index window.
  geom::BBox<N-1,int> interior(lo + 1, hi - 1);

  // The number of records in the window.
  SizeType count = 0;
  typename Cell::const_iterator recordIterator;
  typename Cell::const_iterator recordIteratorEnd;

  //
  // Iterate over the cells in the index window.
  //

  const Key coord_min = window.getLowerCorner()[N - 1];
  const Key coord_max = window.getUpperCorner()[N - 1];
  int n = N - 2;
  mi = lo;
  while (mi[N-2] <= hi[N-2]) {
    if (n == 0) {
      for (mi[0] = lo[0]; mi[0] <= hi[0]; ++mi[0]) {
	// Iterate over the records in the cell.
	const Cell& cell = getCell(mi);
	recordIteratorEnd = cell.end();

	// Binary search to find the beginning of the records in window.
	recordIterator = cell.search(coord_min);

	// If this is an interior cell.
	if (interior.isIn(mi)) {
	  for (; recordIterator != recordIteratorEnd && 
		 getMultiKey(*recordIterator)[N-1] <= coord_max;
	       ++recordIterator) {
	    // There is no need to check if the record is in the window.
	    *iter = *recordIterator;
	    ++iter;
	    ++count;
	  }
	}
	else { // This is a boundary cell.
	  for (; recordIterator != recordIteratorEnd && 
		 getMultiKey(*recordIterator)[N-1] <= coord_max;
		++recordIterator) {
	    if (window.isIn(getMultiKey(*recordIterator))) {
	      *iter = *recordIterator;
	      ++iter;
	      ++count;
	    }
	  }
	}
      }
      ++n;
    }
    else if (mi[n-1] > hi[n-1]) {
      mi[n-1] = lo[n-1];
      ++mi[n];
      ++n;
    }
    else {
      --n;
    }
  }

  return count;
}

END_NAMESPACE_GEOM

// End of file.
