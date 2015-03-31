// -*- C++ -*-

#if !defined(__geom_SortProject_ipp__)
#error This file is an implementation detail of the class SortProject.
#endif

BEGIN_NAMESPACE_GEOM


//
// Window queries.
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<typename OutputIterator>
inline
typename SortProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SortProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator output, const BBox& window) const {
  //
  // Get each slice.
  //

  ads::FixedArray<N, ConstIterator> begin;
  ads::FixedArray<N, ConstIterator> end;
  ads::FixedArray<N, SizeType> size;
  for (int n = 0; n != N; ++n) {
    _lessThanCompareValueAndMultiKey.set(n);
    _lessThanCompareMultiKeyAndValue.set(n);
    begin[n] = std::lower_bound(_sorted[n].begin(), _sorted[n].end(), 
				window.getLowerCorner(), 
				_lessThanCompareValueAndMultiKey);
    end[n] = std::upper_bound(_sorted[n].begin(), _sorted[n].end(), 
			      window.getUpperCorner(), 
			      _lessThanCompareMultiKeyAndValue);
    size[n] = end[n] - begin[n];
  }

  // Choose the smallest slice.
  SizeType count = 0;
  const int n = size.min_index();

  ConstIterator i;
  const ConstIterator iEnd = end[n];
  for (i = begin[n]; i != iEnd; ++i) {
    if (window.isIn(getMultiKey(*i))) {
      *output++ = *i;
      ++count;
    }
  }

  return count;
}


//
// File I/O
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
void 
SortProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const {
  for (ConstIterator i = _sorted[0].begin(); i != _sorted[0].end(); ++i) {
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
SortProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
isValid() const {
  for (int n = 0; n != N; ++n) {
    _lessThanCompare.set(n);
    if (! ads::is_sorted(_sorted[n].begin(), _sorted[n].end(), 
			 _lessThanCompare)) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_GEOM

// End of file.
