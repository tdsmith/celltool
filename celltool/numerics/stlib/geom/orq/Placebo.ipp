// -*- C++ -*-

#if !defined(__geom_Placebo_ipp__)
#error This file is an implementation detail of the class Placebo.
#endif

BEGIN_NAMESPACE_GEOM


//
// Mathematical member functions
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<class OutputIterator>
inline
typename Placebo<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
Placebo<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator iter, const BBox& window) const {
  typename std::vector<Record>::const_iterator recordIterator
    = _records.begin() + getStartingPoint();
  const typename std::vector<Record>::const_iterator recordEnd
    = recordIterator + _querySize;
  while (recordIterator != recordEnd) {
    *(iter++) = *(recordIterator++);
  }
  return _querySize;
}


//
// File I/O
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
inline
void 
Placebo<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const {
  for (typename std::vector<Record>::const_iterator i = _records.begin();
	i != _records.end(); ++i) {
    out << getMultiKey(*i) << '\n';
  }
}

END_NAMESPACE_GEOM

// End of file.
