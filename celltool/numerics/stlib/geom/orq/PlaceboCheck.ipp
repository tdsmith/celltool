// -*- C++ -*-

#if !defined(__geom_PlaceboCheck_ipp__)
#error This file is an implementation detail of the class PlaceboCheck.
#endif

BEGIN_NAMESPACE_GEOM


//
// Mathematical member functions
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<class OutputIterator>
inline
typename PlaceboCheck<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
PlaceboCheck<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator iter, const BBox& window) const {
  SizeType count = 0;
  typename std::vector<Record>::const_iterator recordIterator
    = _records.begin() + getStartingPoint();
  const typename std::vector<Record>::const_iterator recordEnd
    = recordIterator + getQuerySize();
  for (; recordIterator != recordEnd; ++recordIterator) {
    if (window.isIn(getMultiKey(*recordIterator))) {
      *(iter++) = *recordIterator;
      ++count;
    }
  }
  return count;
}


END_NAMESPACE_GEOM

// End of file.
