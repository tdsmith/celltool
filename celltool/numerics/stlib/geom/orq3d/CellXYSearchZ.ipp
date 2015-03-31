// -*- C++ -*-

#if !defined(__geom_CellXYSearchZ_ipp__)
#error This file is an implementation detail of the class CellXYSearchZ.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//

template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
inline
CellXYSearchZ<RecordType, MultiKeyType, KeyType, SearchStructureType>::
CellXYSearchZ(const point_type& delta,
	       const semi_open_interval_type& domain) :
  base_type(),
  _xmin(domain.getLowerCorner()[0]),
  _xmax(domain.getUpperCorner()[0]),
  _ymin(domain.getLowerCorner()[1]),
  _ymax(domain.getUpperCorner()[1]),
  _cell_array(typename cell_array_type::
	       index_type(static_cast<int>(ceil((_xmax - _xmin)/delta[0])),
			   static_cast<int>(ceil((_ymax - _ymin)/delta[1])))),
  _xdelta((_xmax - _xmin) / _cell_array.extent(0)),
  _ydelta((_ymax - _ymin) / _cell_array.extent(1))
{
}

template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
template <typename InputIterator>
inline
CellXYSearchZ<RecordType, MultiKeyType, KeyType, SearchStructureType>::
CellXYSearchZ(const point_type& delta,
	       const semi_open_interval_type& domain,
	       InputIterator first, InputIterator last) :
  base_type(),
  _xmin(domain.getLowerCorner()[0]),
  _xmax(domain.getUpperCorner()[0]),
  _ymin(domain.getLowerCorner()[1]),
  _ymax(domain.getUpperCorner()[1]),
  _cell_array(typename cell_array_type::
	       index_type(static_cast<int>(ceil((_xmax-_xmin)/delta[0])),
			   static_cast<int>(ceil((_ymax-_ymin)/delta[1])))),
  _xdelta((_xmax - _xmin) / _cell_array.extent(0)),
  _ydelta((_ymax - _ymin) / _cell_array.extent(1))
{
  insert(first, last);
  sort();
  initialize();
}

//
// Memory usage.
//
  
template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
inline
typename CellXYSearchZ<RecordType,MultiKeyType,KeyType,SearchStructureType>::
size_type
CellXYSearchZ<RecordType, MultiKeyType, KeyType, SearchStructureType>::
memory_usage() const
{
  int usage = 0;
  // The 2-D array of search structures.
  usage += sizeof(CellXYSearchZ);
  // For each search structure.
  for (typename cell_array_type::const_iterator cell_iter 
	  = _cell_array.begin();
	cell_iter != _cell_array.end();
	++cell_iter) {
    usage += cell_iter->memory_usage();
  }
  return usage;
}

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
inline
void
CellXYSearchZ<RecordType, MultiKeyType, KeyType, SearchStructureType>::
sort()
{
  // Loop over the search structures.
  for (typename cell_array_type::iterator i = _cell_array.begin();
	i != _cell_array.end();
	++i) {
    // Sort the records in the z direction.
    i->sort();
  }
}

//
// File IO
//

template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
inline
void
CellXYSearchZ<RecordType, MultiKeyType, KeyType, SearchStructureType>::
put(std::ostream& out) const
{
  out.precision(16);
  out << "x,y domain = [" << _xmin << ", " << _xmax << ") x ["
      << _ymin << ", " << _ymax << ")"
      << '\n'
      << "cell array size = " << _cell_array.extent(0)
      << " x " << _cell_array.extent(1)
      << '\n'
      << "cell size = " << _xdelta << " x " << _ydelta
      << '\n';

  typename SearchStructureType::const_iterator record_ptr_iter;

  // Loop over the search structures.
  for (typename cell_array_type::const_iterator cell_iter 
	  = _cell_array.begin();
	cell_iter != _cell_array.end();
	++cell_iter) {
    for (record_ptr_iter = cell_iter->begin();
	  record_ptr_iter != cell_iter->end();
	  ++record_ptr_iter) {
      out << **record_ptr_iter << '\n';
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
