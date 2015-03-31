// -*- C++ -*-

#if !defined(__geom_SortRankProject_ipp__)
#error This file is an implementation detail of the class SortRankProject.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//

template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
SortRankProject() :
  Base(),
  _records(),
  _sorted(),
  _rank()
{}


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
SortRankProject(const SizeType size) :
  Base(),
  _records(),
  _sorted(),
  _rank() {
  _records.reserve(size);
  for (int n = 0; n != N; ++n) {
    _sorted[n].reserve(size);
    _rank[n].reserve(size);
  }
}


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<typename InputIterator>
inline
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
SortRankProject(InputIterator first, InputIterator last) :
  Base(),
  _records(),
  _sorted(),
  _rank(){
  insert(first, last);
  sortAndRank();
}


//
// Memory usage.
//

  
template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
typename SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
getMemoryUsage() const {
  return (sizeof(SortRankProject) +
	  _records.size() * (sizeof(Record) + N * sizeof(const_pointer) +
			     N * sizeof(int)));
}

  
//
// Mathematical member functions
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
void
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
sortAndRank() {
  //
  // Make the sorted pointer and rank vectors the right size.
  //
  for (int n = 0; n != N; ++n) {
    _sorted[n].resize(_records.size());
    _rank[n].resize(_records.size());
  }

  //
  // Initialize the vectors of sorted pointers.
  //
  const_pointer recordsBeginning = &*(_records.begin());
  for (SizeType i = 0; i != getSize(); ++i) {
    for (int n = 0; n != N; ++n) {
      _sorted[n][i] = recordsBeginning + i;
    }
  }

  //
  // Sort in each direction.
  //
  LessThanCompare compare;
  for (int n = 0; n != N; ++n) {
    compare.set(n);
    std::sort(_sorted[n].begin(), _sorted[n].end(), compare);
  }
  
  //
  // Make the rank vectors.
  //
  for (int n = 0; n != N; ++n) {
    for (SizeType i = 0; i != getSize(); ++i) {
      _rank[n][_sorted[n][i] - recordsBeginning] = i;
    }
  }
}


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<typename OutputIterator>
inline
typename SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator iter, const BBox& window) const {
  //
  // Get each slice.
  //

  ads::FixedArray<N, typename std::vector<const_pointer>::const_iterator> 
    begin, end;
  ads::FixedArray<N, SizeType> size;
  LessThanComparePointerAndMultiKey comparePointerAndMultiKey;
  LessThanCompareMultiKeyAndPointer compareMultiKeyAndPointer;
  for (int n = 0; n != N; ++n) {
    comparePointerAndMultiKey.set(n);
    compareMultiKeyAndPointer.set(n);
    begin[n] = std::lower_bound(_sorted[n].begin(), _sorted[n].end(), 
				window.getLowerCorner(), 
				comparePointerAndMultiKey);
    end[n] = std::upper_bound(_sorted[n].begin(), _sorted[n].end(), 
			      window.getUpperCorner(), 
			      compareMultiKeyAndPointer);
    size[n] = end[n] - begin[n];
  }

  //
  // Get the intersection of the N slices.
  //

  // The ranks corresponding to the query window.
  SemiOpenInterval<N,int> rankWindow;
  for (int n = 0; n != N; ++n) {
    rankWindow.setLowerCoordinate(n, begin[n] - _sorted[n].begin());
    rankWindow.setUpperCoordinate(n, end[n] - _sorted[n].begin());
  }

  // The smallest slice.
  const int smallestSliceIndex = size.min_index();

  SizeType count = 0;
  ads::FixedArray<N,int> r;
  typename std::vector<const_pointer>::const_iterator i;
  const const_pointer recordsBeginning
    = &*(_records.begin());
  int index;
  // Iterate over the smallest slice.
  for (i = begin[smallestSliceIndex]; i != end[smallestSliceIndex]; ++i) {
    // The index of the record.
    index = *i - recordsBeginning;
    // Get the ranks of the record.
    for (int n = 0; n != N; ++n) {
      r[n] = _rank[n][index];
    }
    // If the ranks are in the query window.
    if (rankWindow.isIn(r)) {
      // Add the record.
      *(iter++) = **i;
      ++count;
    }
  }

  return count;
}


//
// File IO
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
void 
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const {
  for (typename std::vector<Record>::const_iterator i = _records.begin();
	i != _records.end(); ++i) {
    out << getMultiKey(*i) << '\n';
  }
}


//
// Validity check.
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
bool
SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
isValid() const {
  LessThanCompare compare;
  for (int n = 0; n != N; ++n) {
    compare.set(n);
    if (! ads::is_sorted(_sorted[n].begin(), _sorted[n].end(), compare)) {
      return false;
    }
  }
  return true;
}


END_NAMESPACE_GEOM

// End of file.
