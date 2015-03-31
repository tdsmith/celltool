// -*- C++ -*-

#if !defined(__geom_SortFirstDynamic_ipp__)
#error This file is an implementation detail of the class SortFirstDynamic.
#endif

BEGIN_NAMESPACE_GEOM


//
// Window queries.
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<typename OutputIterator>
inline
typename SortFirstDynamic<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SortFirstDynamic<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator output, const BBox& window) const {
  ConstIterator i = _records.lower_bound(window.getLowerCorner()[0]);
  const ConstIterator iEnd = _records.end();

  const Key upperBound = window.getUpperCorner()[0];
  SizeType count = 0;
  for (; i != iEnd && i->first <= upperBound; ++i) {
    if (window.isIn(getMultiKey(i->second))) {
      *output++ = i->second;
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
SortFirstDynamic<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const {
  for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
    out << getMultiKey(i->second) << '\n';
  }
}

END_NAMESPACE_GEOM

// End of file.
