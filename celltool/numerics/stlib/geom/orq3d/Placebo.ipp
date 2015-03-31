// -*- C++ -*-

#if !defined(__geom_Placebo_ipp__)
#error This file is an implementation detail of the class Placebo.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
template < class OutputIterator >
inline
typename Placebo<RecordType,MultiKeyType,KeyType>::size_type
Placebo<RecordType,MultiKeyType,KeyType>::
window_query( OutputIterator iter, const bbox_type& window ) const
{
  typename std::vector<value_type>::const_iterator record_ptr_iter
    = _record_pointers.begin() + starting_point();
  const typename std::vector<value_type>::const_iterator record_ptr_end
    = record_ptr_iter + _query_size;
  while ( record_ptr_iter != record_ptr_end ) {
    *(iter++) = *(record_ptr_iter++);
  }
  return _query_size;
}
      
//
// File IO
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void 
Placebo<RecordType,MultiKeyType,KeyType>::
put( std::ostream& out ) const
{
  for ( typename std::vector<value_type>::const_iterator i 
	  = _record_pointers.begin();
	i != _record_pointers.end();
	++i ) {
    out << **i << '\n';
  }
}

END_NAMESPACE_GEOM

// End of file.
