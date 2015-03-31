// -*- C++ -*-

#if !defined(__geom_SequentialScan_ipp__)
#error This file is an implementation detail of the class SequentialScan.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
template < class OutputIterator >
inline
typename SequentialScan<RecordType,MultiKeyType,KeyType>::size_type
SequentialScan<RecordType,MultiKeyType,KeyType>::
window_query(OutputIterator iter, const bbox_type& window) const
{
  // copy the window into keys for faster access.
  const key_type window_xmin = window.getLowerCorner()[0];
  const key_type window_ymin = window.getLowerCorner()[1];
  const key_type window_zmin = window.getLowerCorner()[2];
  const key_type window_xmax = window.getUpperCorner()[0];
  const key_type window_ymax = window.getUpperCorner()[1];
  const key_type window_zmax = window.getUpperCorner()[2];

  int count = 0;
  typename std::vector<value_type>::const_iterator record_ptr_iter
    = _record_pointers.begin();
  const typename std::vector<value_type>::const_iterator record_ptr_end
    = _record_pointers.end();
  for (; record_ptr_iter != record_ptr_end; ++record_ptr_iter) {
    const multi_key_type& p = (*record_ptr_iter)->multi_key();
    if (p[0] >= window_xmin && p[0] <= window_xmax &&
	 p[1] >= window_ymin && p[1] <= window_ymax &&
	 p[2] >= window_zmin && p[2] <= window_zmax) {
      *(iter++) = *record_ptr_iter;
      ++count;
    }
  }
  return count;
}
      
//
// File IO
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void 
SequentialScan<RecordType,MultiKeyType,KeyType>::
put(std::ostream& out) const
{
  for (typename std::vector<value_type>::const_iterator i 
	  = _record_pointers.begin();
	i != _record_pointers.end();
	++i) {
    out << **i << '\n';
  }
}

END_NAMESPACE_GEOM

// End of file.
