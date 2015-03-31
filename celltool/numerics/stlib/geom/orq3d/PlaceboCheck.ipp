// -*- C++ -*-

#if !defined(__geom_PlaceboCheck_ipp__)
#error This file is an implementation detail of the class PlaceboCheck.
#endif

BEGIN_NAMESPACE_GEOM

//
// Window queries.
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
template < class OutputIterator >
inline
typename PlaceboCheck<RecordType,MultiKeyType,KeyType>::size_type
PlaceboCheck<RecordType,MultiKeyType,KeyType>::
window_query(OutputIterator iter, const bbox_type& window) const
{
  // copy the window into keys for faster access.
  const key_type window_xmin = window.getLowerCorner()[0];
  const key_type window_ymin = window.getLowerCorner()[1];
  const key_type window_zmin = window.getLowerCorner()[2];
  const key_type window_xmax = window.getUpperCorner()[0];
  const key_type window_ymax = window.getUpperCorner()[1];
  const key_type window_zmax = window.getUpperCorner()[2];

  size_type count = 0;
  typename std::vector<value_type>::const_iterator record_ptr_iter
    = record_pointers().begin() + starting_point();
  const typename std::vector<value_type>::const_iterator record_ptr_end
    = record_ptr_iter + query_size();
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
      
END_NAMESPACE_GEOM

// End of file.
