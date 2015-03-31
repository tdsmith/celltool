// -*- C++ -*-

#if !defined(__geom_SortFirst_ipp__)
#error This file is an implementation detail of the class SortFirst.
#endif

BEGIN_NAMESPACE_GEOM


//
// Window queries.
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<typename OutputIterator>
inline
typename SortFirst<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SortFirst<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator output, const BBox& window) const {
  ConstIterator i = std::lower_bound(_sorted.begin(), _sorted.end(), 
				     window.getLowerCorner(), 
				     _lessThanCompareValueAndMultiKey);
  const ConstIterator iEnd = 
    std::upper_bound(_sorted.begin(), _sorted.end(), window.getUpperCorner(), 
		     _lessThanCompareMultiKeyAndValue);

  SizeType count = 0;
  for (; i != iEnd; ++i) {
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
SortFirst<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const {
  for (ConstIterator i = _sorted.begin(); i != _sorted.end(); ++i) {
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
SortFirst<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
isValid() const {
  if (! ads::is_sorted(_sorted.begin(), _sorted.end(), _lessThanCompare)) {
    return false;
  }
  return true;
}

END_NAMESPACE_GEOM

// End of file.
