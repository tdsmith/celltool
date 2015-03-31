// -*- C++ -*-

#if !defined(__geom_SortProject_ipp__)
#error This file is an implementation detail of the class SortProject.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
template <typename OutputIterator>
inline
typename SortProject<RecordType,MultiKeyType,KeyType>::size_type
SortProject<RecordType,MultiKeyType,KeyType>::
window_query(OutputIterator iter, const bbox_type& window) const {
  //
  // Get the x slice.
  //

  const typename std::vector<value_type>::const_iterator xbegin
    = std::lower_bound(_xsorted.begin(), _xsorted.end(), window.getLowerCorner(), 
			xless_rh_m<value_type, point_type>());
  const typename std::vector<value_type>::const_iterator xend
    = std::upper_bound(xbegin, _xsorted.end(), window.getUpperCorner(), 
			xless_m_rh<point_type, value_type>());
  const int xsize = xend - xbegin;

  //
  // Get the y slice.
  //

  const typename std::vector<value_type>::const_iterator ybegin
    = std::lower_bound(_ysorted.begin(), _ysorted.end(), window.getLowerCorner(), 
			yless_rh_m<value_type, point_type>());
  const typename std::vector<value_type>::const_iterator yend
    = std::upper_bound(ybegin, _ysorted.end(), window.getUpperCorner(), 
			yless_m_rh<point_type, value_type>());
  const int ysize = yend - ybegin;

  //
  // Get the z slice.
  //

  const typename std::vector<value_type>::const_iterator zbegin
    = std::lower_bound(_zsorted.begin(), _zsorted.end(), window.getLowerCorner(),
			zless_rh_m<value_type, point_type>());
  const typename std::vector<value_type>::const_iterator zend
    = std::upper_bound(zbegin, _zsorted.end(), window.getUpperCorner(), 
			zless_m_rh<point_type, value_type>());
  const int zsize = zend - zbegin;

  //
  // Get the intersection of the three slices.
  //

  int count = 0;
  typename std::vector<value_type>::const_iterator i;

  // If the x slice is the smallest.
  if (xsize < ysize && xsize < zsize) {
    key_type window_ymin = window.getLowerCorner()[1];
    key_type window_ymax = window.getUpperCorner()[1];
    key_type window_zmin = window.getLowerCorner()[2];
    key_type window_zmax = window.getUpperCorner()[2];
    for (i = xbegin; i != xend; ++i) {
      if ((*i)->multi_key()[1] >= window_ymin && 
	   (*i)->multi_key()[1] <= window_ymax &&
	   (*i)->multi_key()[2] >= window_zmin && 
	   (*i)->multi_key()[2] <= window_zmax) {
	*(iter++) = *i;
	++count;
      }
    }
  }
  // If the y slice is the smallest.
  else if (ysize < zsize) {
    key_type window_xmin = window.getLowerCorner()[0];
    key_type window_xmax = window.getUpperCorner()[0];
    key_type window_zmin = window.getLowerCorner()[2];
    key_type window_zmax = window.getUpperCorner()[2];
    for (i = ybegin; i != yend; ++i) {
      if ((*i)->multi_key()[0] >= window_xmin && 
	   (*i)->multi_key()[0] <= window_xmax &&
	   (*i)->multi_key()[2] >= window_zmin && 
	   (*i)->multi_key()[2] <= window_zmax) {
	*(iter++) = *i;
	++count;
      }
    }
  }
  // If the z slice is the smallest.
  else {
    key_type window_xmin = window.getLowerCorner()[0];
    key_type window_xmax = window.getUpperCorner()[0];
    key_type window_ymin = window.getLowerCorner()[1];
    key_type window_ymax = window.getUpperCorner()[1];
    for (i = zbegin; i != zend; ++i) {
      if ((*i)->multi_key()[0] >= window_xmin && 
	   (*i)->multi_key()[0] <= window_xmax &&
	   (*i)->multi_key()[1] >= window_ymin && 
	   (*i)->multi_key()[1] <= window_ymax) {
	*(iter++) = *i;
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
SortProject<RecordType,MultiKeyType,KeyType>::
put(std::ostream& out) const {
  for (typename std::vector<value_type>::const_iterator i 
	  = _xsorted.begin();
	i != _xsorted.end();
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
SortProject<RecordType,MultiKeyType,KeyType>::
check() const {
  assert(ads::is_sorted(ads::constructIndirectIterator(_xsorted.begin()), 
			ads::constructIndirectIterator(_xsorted.end()),
			xless<record_type>()));
  assert(ads::is_sorted(ads::constructIndirectIterator(_ysorted.begin()),
			ads::constructIndirectIterator(_ysorted.end()),
			yless<record_type>()));
  assert(ads::is_sorted(ads::constructIndirectIterator(_zsorted.begin()),
			ads::constructIndirectIterator(_zsorted.end()),
			zless<record_type>()));
}

END_NAMESPACE_GEOM

// End of file.
