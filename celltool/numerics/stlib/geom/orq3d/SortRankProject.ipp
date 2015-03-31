// -*- C++ -*-

#if !defined(__geom_SortRankProject_ipp__)
#error This file is an implementation detail of the class SortRankProject.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
SortRankProject<RecordType,MultiKeyType,KeyType>::
SortRankProject() :
  base_type(),
  _record_pointers(),
  _xsorted(),
  _ysorted(),
  _zsorted(),
  _xrank(),
  _yrank(),
  _zrank()
{
}

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
SortRankProject<RecordType,MultiKeyType,KeyType>::
SortRankProject(size_type size) :
  base_type(),
  _record_pointers(),
  _xsorted(),
  _ysorted(),
  _zsorted(),
  _xrank(),
  _yrank(),
  _zrank()
{
  _record_pointers.reserve(size);
  _xsorted.reserve(size);
  _ysorted.reserve(size);
  _zsorted.reserve(size);
  _xrank.reserve(size);
  _yrank.reserve(size);
  _zrank.reserve(size);
}

template <typename RecordType, typename MultiKeyType, typename KeyType>
template <class InputIterator>
inline
SortRankProject<RecordType,MultiKeyType,KeyType>::
SortRankProject(InputIterator first, InputIterator last) :
  base_type(),
  _record_pointers(),
  _xsorted(),
  _ysorted(),
  _zsorted(),
  _xrank(),
  _yrank(),
  _zrank()
{
  insert(first, last);
  sort_rank();
}

//
// Memory usage.
//
  
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
typename SortRankProject<RecordType,MultiKeyType,KeyType>::size_type
SortRankProject<RecordType,MultiKeyType,KeyType>::
memory_usage() const
{
  return (sizeof(std::vector<value_type>) +
	   3 * sizeof(std::vector<pointer>) +
	   3 * sizeof(std::vector<int>) +
	   _record_pointers.size() * (sizeof(value_type)
				       + 3 * sizeof(pointer)
				       + 3 * sizeof(int)));
}
  
//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void
SortRankProject<RecordType,MultiKeyType,KeyType>::
sort_rank()
{
  //
  // Make the sorted pointer and rank vectors the right size.
  //
  _xsorted.resize(_record_pointers.size());
  _ysorted.resize(_record_pointers.size());
  _zsorted.resize(_record_pointers.size());
  _xrank.resize(_record_pointers.size());
  _yrank.resize(_record_pointers.size());
  _zrank.resize(_record_pointers.size());

  //
  // Initialize the vectors of sorted pointers.
  //
  pointer record_pointers_begin = &*_record_pointers.begin();
  for (size_type i = 0; i < num_records(); ++i) {
    _xsorted[i] = _ysorted[i] = _zsorted[i] 
      = record_pointers_begin + i;
  }

  //
  // Sort in each direction.
  //
  std::sort(_xsorted.begin(), _xsorted.end(),
	     xless_hh< typename std::vector<pointer>::value_type >());
  std::sort(_ysorted.begin(), _ysorted.end(),
	     yless_hh< typename std::vector<pointer>::value_type >());
  std::sort(_zsorted.begin(), _zsorted.end(),
	     zless_hh< typename std::vector<pointer>::value_type >());

  //
  // Make the rank vectors.
  //
  for (size_type i = 0; i < num_records(); ++i)
    _xrank[ _xsorted[i] - record_pointers_begin ] 
      = _yrank[ _ysorted[i] - record_pointers_begin ] 
      = _zrank[ _zsorted[i] - record_pointers_begin ] = i;
}

template <typename RecordType, typename MultiKeyType, typename KeyType>
template < class OutputIterator >
inline
typename SortRankProject<RecordType,MultiKeyType,KeyType>::size_type
SortRankProject<RecordType,MultiKeyType,KeyType>::
window_query(OutputIterator iter, const bbox_type& window) const
{
  //
  // Get the x slice.
  //

  const typename std::vector<pointer>::const_iterator xbegin
    = std::lower_bound(_xsorted.begin(), _xsorted.end(), window.getLowerCorner(), 
			xless_rhh_m<pointer, point_type>());
  const typename std::vector<pointer>::const_iterator xend
    = std::upper_bound(xbegin, _xsorted.end(), window.getUpperCorner(),
			xless_m_rhh<point_type, pointer>());
  const int xsize = xend - xbegin;

  //
  // Get the y slice.
  //

  const typename std::vector<pointer>::const_iterator ybegin
    = std::lower_bound(_ysorted.begin(), _ysorted.end(), window.getLowerCorner(), 
			yless_rhh_m<pointer, point_type>());
  const typename std::vector<pointer>::const_iterator yend
    = std::upper_bound(ybegin, _ysorted.end(), window.getUpperCorner(),
			yless_m_rhh<point_type, pointer>());
  const int ysize = yend - ybegin;

  //
  // Get the z slice.
  //

  const typename std::vector<pointer>::const_iterator zbegin
    = std::lower_bound(_zsorted.begin(), _zsorted.end(), window.getLowerCorner(),
			zless_rhh_m<pointer, point_type>());
  const typename std::vector<pointer>::const_iterator zend
    = std::upper_bound(zbegin, _zsorted.end(), window.getUpperCorner(),
			zless_m_rhh<point_type, pointer>());
  const int zsize = zend - zbegin;

  //
  // Get the intersection of the three slices.
  //

  int count = 0;
  typename std::vector<pointer>::const_iterator i;
  const typename std::vector<value_type>::const_pointer record_pointers_begin
    = &*_record_pointers.begin();

  // If the x slice is the smallest.
  if (xsize < ysize && xsize < zsize) {
    int yr, zr;
    const int yrbegin = ybegin - _ysorted.begin();
    const int yrend = yend - _ysorted.begin();
    const int zrbegin = zbegin - _zsorted.begin();
    const int zrend = zend - _zsorted.begin();
    for (i = xbegin; i < xend; ++i) {
      yr = _yrank[ *i - record_pointers_begin ];
      zr = _zrank[ *i - record_pointers_begin ];
      if (yrbegin <= yr && yr < yrend && zrbegin <= zr && zr < zrend) {
	*(iter++) = **i;
	++count;
      }
    }
  }
  // If the y slice is the smallest.
  else if (ysize < zsize) {
    int xr, zr;
    const int xrbegin = xbegin - _xsorted.begin();
    const int xrend = xend - _xsorted.begin();
    const int zrbegin = zbegin - _zsorted.begin();
    const int zrend = zend - _zsorted.begin();
    for (i = ybegin; i < yend; ++i) {
      xr = _xrank[ *i - record_pointers_begin ];
      zr = _zrank[ *i - record_pointers_begin ];
      if (xrbegin <= xr && xr < xrend && zrbegin <= zr && zr < zrend) {
	*(iter++) = **i;
	++count;
      }
    }
  }
  // If the z slice is the smallest.
  else {
    int xr, yr;
    const int xrbegin = xbegin - _xsorted.begin();
    const int xrend = xend - _xsorted.begin();
    const int yrbegin = ybegin - _ysorted.begin();
    const int yrend = yend - _ysorted.begin();
    for (i = zbegin; i < zend; ++i) {
      xr = _xrank[ *i - record_pointers_begin ];
      yr = _yrank[ *i - record_pointers_begin ];
      if (xrbegin <= xr && xr < xrend && yrbegin <= yr && yr < yrend) {
	*(iter++) = **i;
	++count;
      }
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
SortRankProject<RecordType,MultiKeyType,KeyType>::
put(std::ostream& out) const
{
  for (typename std::vector<value_type>::const_iterator i 
	  = _record_pointers.begin();
	i != _record_pointers.end();
	++i) {
    out << **i << '\n';
  }
}


//
// Validity check.
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void 
SortRankProject<RecordType,MultiKeyType,KeyType>::
check() const
{
  assert(ads::is_sorted(_xsorted.begin(), _xsorted.end(), 
			  xless_hh<pointer>()));
  assert(ads::is_sorted(_ysorted.begin(), _ysorted.end(), 
			  yless_hh<pointer>()));
  assert(ads::is_sorted(_zsorted.begin(), _zsorted.end(), 
			  zless_hh<pointer>()));
}

END_NAMESPACE_GEOM

// End of file.
