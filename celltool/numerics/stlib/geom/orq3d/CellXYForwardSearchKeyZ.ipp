// -*- C++ -*-

#if !defined(__geom_CellXYForwardSearchKeyZ_ipp__)
#error This file is an implementation detail of the class CellXYForwardSearchKeyZ.
#endif

BEGIN_NAMESPACE_GEOM

//
// Sort
//
    
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void
ForwardSearchKeyZ<RecordType,MultiKeyType,KeyType>::
sort()
{
  // Sort the records.
  std::sort(begin(), end(), zless_h<value_type>());
  // Copy the keys.
  const int sz = size();
  _x_key.reserve(sz);
  _y_key.reserve(sz);
  _z_key.reserve(sz);
  _x_key.clear();
  _y_key.clear();
  _z_key.clear();
  for (iterator i = begin(); i != end(); ++i) {
    _x_key.push_back((*i)->multi_key()[0]);
    _y_key.push_back((*i)->multi_key()[1]);
    _z_key.push_back((*i)->multi_key()[2]);
  }
}
    
//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
int
ForwardSearchKeyZ<RecordType,MultiKeyType,KeyType>::
search(key_type z) const
{
  key_const_iterator i = _z_key.begin() + _current;
  key_const_iterator i_end = _z_key.end();
  while (i != i_end && *i < z) {
    ++i;
  }
  _current = i - _z_key.begin();
  return _current;
}

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
template <typename OutputIterator>
inline
typename CellXYForwardSearchKeyZ<RecordType,MultiKeyType,KeyType>::size_type
CellXYForwardSearchKeyZ<RecordType,MultiKeyType,KeyType>::
window_query(OutputIterator iter, const bbox_type& window) const
{
  //
  // Convert the multi-key to array indices.
  //
  int i0, j0, i1, j1;
  multi_key_to_indices(window.getLowerCorner(), i0, j0);
  multi_key_to_indices(window.getUpperCorner(), i1, j1);

  //
  // If the window does not intersect the domain do nothing.
  //
  if (i0 >= static_cast<int>(cell_array_extents()(0)) || i1 < 0 || 
       j0 >= static_cast<int>(cell_array_extents()(1)) || j1 < 0) {
    return 0;
  }

  //
  // copy the window into keys for faster access.
  //
  const key_type window_xmin = window.getLowerCorner()[0];
  const key_type window_ymin = window.getLowerCorner()[1];
  const key_type window_zmin = window.getLowerCorner()[2];
  const key_type window_xmax = window.getUpperCorner()[0];
  const key_type window_ymax = window.getUpperCorner()[1];
  const key_type window_zmax = window.getUpperCorner()[2];

  //  
  // Determine the index window.
  //

  int istart = std::max(0, i0);
  int jstart = std::max(0, j0);
  int istop = std::min(static_cast<int>(cell_array_extents()(0)) - 1, i1);
  int jstop = std::min(static_cast<int>(cell_array_extents()(1)) - 1, j1);



  int count = 0;
  int i, j, n;
  typename cell_type::const_iterator record_iter;
  typename cell_type::key_const_iterator x, y, z, zstart, zend;

  //
  // Scan convert the window.
  //
  if (istart != istop && jstart != jstop) {

    //
    // First do the four corners.
    //

    //
    // The lower left corner.
    // i == istart && i != istop && j == jstart && j != jstop 
    //
    {
      const cell_type& record_container = get_cell(istart, jstart);
      n = record_container.search(window_zmin);
      x = record_container.x_key().begin() + n;
      y = record_container.y_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++x, ++y, ++z) {
	if (*x >= window_xmin && 
	     *y >= window_ymin) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // The lower right corner.
    // i != istart && i == istop && j == jstart && j != jstop 
    //
    {
      const cell_type& record_container = get_cell(istop, jstart);
      n = record_container.search(window_zmin);
      x = record_container.x_key().begin() + n;
      y = record_container.y_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++x, ++y, ++z) {
	if (*x <= window_xmax &&
	     *y >= window_ymin) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // The upper left corner.
    // i == istart && i != istop && j != jstart && j == jstop 
    //
    {
      const cell_type& record_container = get_cell(istart, jstop);
      n = record_container.search(window_zmin);
      x = record_container.x_key().begin() + n;
      y = record_container.y_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++x, ++y, ++z) {
	if (*x >= window_xmin && 
	     *y <= window_ymax) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // The upper right corner.
    // i != istart && i == istop && j != jstart && j == jstop 
    //
    {
      const cell_type& record_container = get_cell(istop, jstop);
      n = record_container.search(window_zmin);
      x = record_container.x_key().begin() + n;
      y = record_container.y_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++x, ++y, ++z) {
	if (*x <= window_xmax &&
	     *y <= window_ymax) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // Next do the four sides.
    //

    //
    // The bottom side.
    // i != istart && i != istop && j == jstart && j != jstop 
    //
    for (i = istart + 1; i < istop; ++i) {
      const cell_type& record_container = get_cell(i, jstart);
      n = record_container.search(window_zmin);
      y = record_container.y_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++y, ++z) {
	if (*y >= window_ymin) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // The top side.
    // i != istart && i != istop && j != jstart && j == jstop 
    //
    for (i = istart + 1; i < istop; ++i) {
      const cell_type& record_container = get_cell(i, jstop);
      n = record_container.search(window_zmin);
      y = record_container.y_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++y, ++z) {
	if (*y <= window_ymax) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // The left side.
    // i == istart && i != istop && j != jstart && j != jstop 
    //
    for (j = jstart + 1; j < jstop; ++j) {
      const cell_type& record_container = get_cell(istart, j);
      n = record_container.search(window_zmin);
      x = record_container.x_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++x, ++z) {
	if (*x >= window_xmin) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // The right side.
    // i != istart && i == istop && j != jstart && j != jstop 
    //
    for (j = jstart + 1; j < jstop; ++j) {
      const cell_type& record_container = get_cell(istop, j);
      n = record_container.search(window_zmin);
      x = record_container.x_key().begin() + n;
      z = record_container.z_key().begin() + n;
      zend = record_container.z_key().end();
      record_iter = record_container.begin() + n;
      for (; z != zend && *z <= window_zmax; 
	    ++record_iter, ++x, ++z) {
	if (*x <= window_xmax) {
	  *(iter++) = *record_iter;
	  ++count;
	}
      }
    }

    //
    // Finally do the interior
    // i != istart && i != istop && j != jstart && j != jstop 
    //
    for (i = istart + 1; i < istop; ++i) {
      for (j = jstart + 1; j < jstop; ++j) {
	const cell_type& record_container = get_cell(i, j);
	n = record_container.search(window_zmin);
	z = record_container.z_key().begin() + n;
	zstart = z;
	zend = record_container.z_key().end();
	record_iter = record_container.begin() + n;
	for (; z != zend && *z <= window_zmax; 
	      ++record_iter, ++z) {
	  *(iter++) = *record_iter;
	}
	count += z - zstart;
      }
    }


    /***********************************************

	for (i = istart; i <= istop; ++i) {
	  for (j = jstart; j <= jstop; ++j) {
	    ForwardSearchKeyZ<RecordType>& record_container 
	      = _cell_array(i,j);

	    z = zstart = record_container.z_key().begin() 
	      + record_container.current();
	    zend = record_container.z_key().end();
	    // Move the current pointer to the beginning of the window.
	    while (z != zend && *z < window_zmin) {
	      ++z;
	    }
	    n = record_container.current() += z - zstart;
	    record_iter = record_container.record_pointers().begin() + n;

	    // Here is the optimized implementation.
	    if (i == istart) {
	      if (i == istop) {
		if (j == jstart) {
		  if (j == jstop) {
		  // i == istart && i == istop && j == jstart && j == jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x >= window_xmin && *x <= window_xmax &&
		      *y >= window_ymin && *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i == istart && i == istop && j == jstart && j != jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x >= window_xmin && *x <= window_xmax &&
		      *y >= window_ymin) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
		else {
		  if (j == jstop) {
		  // i == istart && i == istop && j != jstart && j == jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x >= window_xmin && *x <= window_xmax &&
		      *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i == istart && i == istop && j != jstart && j != jstop 
		    x = record_container.x_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++z) {
		      if (*x >= window_xmin && *x <= window_xmax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
	      }
	      else {
		if (j == jstart) {
		  if (j == jstop) {
		  // i == istart && i != istop && j == jstart && j == jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x >= window_xmin &&
		      *y >= window_ymin && *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i == istart && i != istop && j == jstart && j != jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x >= window_xmin && 
		      *y >= window_ymin) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
		else {
		  if (j == jstop) {
		  // i == istart && i != istop && j != jstart && j == jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x >= window_xmin && 
		      *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i == istart && i != istop && j != jstart && j != jstop 
		    x = record_container.x_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++z) {
		      if (*x >= window_xmin) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
	      }
	    }
	    else {
	      if (i == istop) {
		if (j == jstart) {
		  if (j == jstop) {
		  // i != istart && i == istop && j == jstart && j == jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x <= window_xmax &&
		      *y >= window_ymin && *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i != istart && i == istop && j == jstart && j != jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x <= window_xmax &&
		      *y >= window_ymin) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
		else {
		  if (j == jstop) {
		  // i != istart && i == istop && j != jstart && j == jstop 
		    x = record_container.x_key().begin() + n;
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++y, ++z) {
		      if (*x <= window_xmax &&
		      *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i != istart && i == istop && j != jstart && j != jstop 
		    x = record_container.x_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++x, ++z) {
		      if (*x <= window_xmax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
	      }
	      else {
		if (j == jstart) {
		  if (j == jstop) {
		  // i != istart && i != istop && j == jstart && j == jstop 
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++y, ++z) {
		      if (*y >= window_ymin && *y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i != istart && i != istop && j == jstart && j != jstop 
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++y, ++z) {
		      if (*y >= window_ymin) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		}
		else {
		  if (j == jstop) {
		  // i != istart && i != istop && j != jstart && j == jstop 
		    y = record_container.y_key().begin() + n;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++y, ++z) {
		      if (*y <= window_ymax) {
		      *(iter++) = *record_iter;
			++count;
		      }
		    }
		  }
		  else {
		  // i != istart && i != istop && j != jstart && j != jstop 
		    zstart = z;
		    for (; z != zend && *z <= window_zmax; 
			  ++record_iter, ++z) {
			  *(iter++) = *record_iter;
		    }
		    count += z - zstart;
		  }
		}
	      }
	    }
	  }
	}
    *************************************************/
  }
  else { 
    // This version is used if there are a small number of cells.

    for (i = istart; i <= istop; ++i) {
      for (j = jstart; j <= jstop; ++j) {
	const cell_type& record_container = get_cell(i, j);
	n = record_container.search(window_zmin);
	x = record_container.x_key().begin() + n;
	y = record_container.y_key().begin() + n;
	z = record_container.z_key().begin() + n;
	zend = record_container.z_key().end();
	record_iter = record_container.begin() + n;
	for (; z != zend && *z <= window_zmax; 
	      ++record_iter, ++x, ++y, ++z) {
	  if (*x >= window_xmin && *x <= window_xmax &&
	       *y >= window_ymin && *y <= window_ymax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }
  }

  return count;
}

END_NAMESPACE_GEOM

// End of file.
