// -*- C++ -*-

#if !defined(__geom_SequentialScan_ipp__)
#error This file is an implementation detail of the class SequentialScan.
#endif

BEGIN_NAMESPACE_GEOM


//
// Mathematical member functions
//


template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor>
template<class OutputIterator>
inline
typename SequentialScan<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::SizeType
SequentialScan<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
computeWindowQuery(OutputIterator output, const BBox& window) const {
  int count = 0;
  ConstIterator iter = _recordIterators.begin();
  const ConstIterator iterEnd = _recordIterators.end();
  for (; iter != iterEnd; ++iter) {
    if (window.isIn(getMultiKey(*iter))) {
      *(output++) = *iter;
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
SequentialScan<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>::
put(std::ostream& out) const {
  for (ConstIterator i = _recordIterators.begin();
       i != _recordIterators.end();++i) {
    out << **i << '\n';
  }
}

END_NAMESPACE_GEOM

// End of file.
