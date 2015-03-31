// -*- C++ -*-

#if !defined(__geom_CellXYForwardSearchZ_ipp__)
#error This file is an implementation detail of the class CellXYForwardSearchZ.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType,
	  bool BoundarySpecialization>
template <typename OutputIterator>
inline
typename CellXYForwardSearchZ<RecordType, MultiKeyType, KeyType, 
			      BoundarySpecialization>::size_type
CellXYForwardSearchZ<RecordType,MultiKeyType,KeyType,BoundarySpecialization>::
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
  int i, j;
  typename cell_type::const_iterator record_iter;
  typename cell_type::const_iterator cell_end;

  //
  // Scan convert the window.
  //
  if (BoundarySpecialization) {
    for (i = istart; i <= istop; ++i) {
      for (j = jstart; j <= jstop; ++j) {
	const cell_type& cell = get_cell(i, j);
	cell_end = cell.end();
	record_iter = cell.search(window_zmin);

	if (i == istart) {
	  if (i == istop) {
	    if (j == jstart) {
	      if (j == jstop) {
		// i == istart && i == istop && j == jstart && j == jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[0] <= window_xmax &&
		       (*record_iter)->multi_key()[1] >= window_ymin &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i == istart && i == istop && j == jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[0] <= window_xmax &&
		       (*record_iter)->multi_key()[1] >= window_ymin) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	    }
	    else {
	      if (j == jstop) {
		// i == istart && i == istop && j != jstart && j == jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[0] <= window_xmax &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i == istart && i == istop && j != jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[0] <= window_xmax) {
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
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[1] >= window_ymin &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i == istart && i != istop && j == jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[1] >= window_ymin) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	    }
	    else {
	      if (j == jstop) {
		// i == istart && i != istop && j != jstart && j == jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i == istart && i != istop && j != jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] >= window_xmin) {
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
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] <= window_xmax &&
		       (*record_iter)->multi_key()[1] >= window_ymin &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i != istart && i == istop && j == jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] <= window_xmax &&
		       (*record_iter)->multi_key()[1] >= window_ymin) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	    }
	    else {
	      if (j == jstop) {
		// i != istart && i == istop && j != jstart && j == jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] <= window_xmax &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i != istart && i == istop && j != jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[0] <= window_xmax) {
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
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[1] >= window_ymin &&
		       (*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i != istart && i != istop && j == jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[1] >= window_ymin) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	    }
	    else {
	      if (j == jstop) {
		// i != istart && i != istop && j != jstart && j == jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  if ((*record_iter)->multi_key()[1] <= window_ymax) {
		    *(iter++) = *record_iter;
		    ++count;
		  }
		}
	      }
	      else {
		// i != istart && i != istop && j != jstart && j != jstop 
		for (; record_iter != cell_end && 
			(*record_iter)->multi_key()[2] <= window_zmax;
		      ++record_iter) {
		  *(iter++) = *record_iter;
		  ++count;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  else { // !BoundarySpecialization
    for (i = istart; i <= istop; ++i) {
      for (j = jstart; j <= jstop; ++j) {
	const cell_type& cell = get_cell(i, j);
	cell_end = cell.end();
	record_iter = cell.search(window_zmin);

	// If this is an interior cell.
	if (i > istart && i < istop && j > jstart && j < jstop) {
	  for (; record_iter != cell_end && 
		  (*record_iter)->multi_key()[2] <= window_zmax;
		++record_iter) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
	else { // This is a boundary cell.
	  for (; record_iter != cell_end && 
		  (*record_iter)->multi_key()[2] <= window_zmax;
		++record_iter) {
	    if ((*record_iter)->multi_key()[0] >= window_xmin &&
		 (*record_iter)->multi_key()[0] <= window_xmax &&
		 (*record_iter)->multi_key()[1] >= window_ymin &&
		 (*record_iter)->multi_key()[1] <= window_ymax) {
	      *(iter++) = *record_iter;
	      ++count;
	    }
	  }
	}
      }
    }
  }
  return count;
}

END_NAMESPACE_GEOM

// End of file.
