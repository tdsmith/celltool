// -*- C++ -*-

#if !defined(__geom_SparseCellArray_ipp__)
#error This file is an implementation detail of the class SparseCellArray.
#endif

BEGIN_NAMESPACE_GEOM

//
// Memory usage.
//
  
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
typename SparseCellArray<RecordType,MultiKeyType,KeyType>::size_type
SparseCellArray<RecordType,MultiKeyType,KeyType>::
memory_usage() const
{
  size_type usage = 0;
  // The 2D array of sparse vectors.
  usage += _vector_array.size() 
    * sizeof(SparseCellVector<RecordType,MultiKeyType,KeyType>);
  // For each sparse vector.
  for (typename vector_array_type::const_iterator i = _vector_array.begin();
	i != _vector_array.end();
	++i) {
    // The memory for the structure of the cell.
    usage += i->size() * 
      sizeof(IndexAndCell<RecordType,MultiKeyType,KeyType>);
    for (typename SparseCellVector<RecordType,MultiKeyType,KeyType>::
	    const_iterator j = i->begin();
	  j != i->end();
	  ++j) {
      // The memory for the contents of the cell.
      usage += j->cell.size() * sizeof(value_type);
    }
  }
  return usage;
}

//
// Mathematical member functions
//
template <typename RecordType, typename MultiKeyType, typename KeyType>
template< class OutputIterator >
inline
typename SparseCellArray<RecordType,MultiKeyType,KeyType>::size_type
SparseCellArray<RecordType,MultiKeyType,KeyType>::
window_query(OutputIterator iter, const bbox_type& window) const
{
  //
  // Convert the multi-key to grid index coordinates.
  //

  int i0, j0, k0, i1, j1, k1;
  multi_key_to_indices(window.getLowerCorner(), i0, j0, k0);
  multi_key_to_indices(window.getUpperCorner(), i1, j1, k1);
  
  //
  // If the window does not intersect the domain do nothing.
  //

  if (i0 >= static_cast<int>(extents()[0]) || i1 < 0 || 
       j0 >= static_cast<int>(extents()[1]) || j1 < 0 ||
       k0 >= static_cast<int>(extents()[2]) || k1 < 0) {
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
  int kstart = std::max(0, k0);

  int istop = std::min(static_cast<int>(extents()[0]) - 1, i1);
  int jstop = std::min(static_cast<int>(extents()[1]) - 1, j1);
  int kstop = std::min(static_cast<int>(extents()[2]) - 1, k1);

  //
  // Do the boundary of the window.
  //

  int count = 0;
  int i, j;
  typename cell_type::const_iterator record_iter, record_iter_end;
  typename SparseCellVector<RecordType,MultiKeyType,KeyType>::const_iterator 
    cell_iter, 
    cell_iter_end;

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
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(istart, jstart);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[0] >= window_xmin &&
	       (*record_iter)->multi_key()[1] >= window_ymin &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // The lower right corner.
    // i != istart && i == istop && j == jstart && j != jstop 
    //
    {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(istop, jstart);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[0] <= window_xmax &&
	       (*record_iter)->multi_key()[1] >= window_ymin &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // The upper left corner.
    // i == istart && i != istop && j != jstart && j == jstop 
    //
    {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(istart, jstop);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[0] >= window_xmin &&
	       (*record_iter)->multi_key()[1] <= window_ymax &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // The upper right corner.
    // i != istart && i == istop && j != jstart && j == jstop 
    //
    {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(istop, jstop);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[0] <= window_xmax &&
	       (*record_iter)->multi_key()[1] <= window_ymax &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
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
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(i, jstart);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[1] >= window_ymin &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // The top side.
    // i != istart && i != istop && j != jstart && j == jstop 
    //
    for (i = istart + 1; i < istop; ++i) {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(i, jstop);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[1] <= window_ymax &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // The left side.
    // i == istart && i != istop && j != jstart && j != jstop 
    //
    for (j = jstart + 1; j < jstop; ++j) {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(istart, j);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[0] >= window_xmin &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // The right side.
    // i != istart && i == istop && j != jstart && j != jstop 
    //
    for (j = jstart + 1; j < jstop; ++j) {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	= _vector_array(istop, j);
      cell_iter = cell_vector.lower_bound(kstart);
      cell_iter_end = cell_vector.end();
      for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	    ++cell_iter) {
	cell_const_reference cell = cell_iter->cell;
	record_iter_end = cell.end();
	for (record_iter = cell.begin();
	      record_iter != record_iter_end;
	      ++record_iter) {
	  if ((*record_iter)->multi_key()[0] <= window_xmax &&
	       (*record_iter)->multi_key()[2] >= window_zmin &&
	       (*record_iter)->multi_key()[2] <= window_zmax) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // Finally do the interior
    // i != istart && i != istop && j != jstart && j != jstop 
    //
    for (i = istart + 1; i < istop; ++i) {
      for (j = jstart + 1; j < jstop; ++j) {
	const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	  = _vector_array(i, j);
	cell_iter = cell_vector.lower_bound(kstart);
	cell_iter_end = cell_vector.end();
	for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	      ++cell_iter) {
	  cell_const_reference cell = cell_iter->cell;
	  record_iter_end = cell.end();
	  for (record_iter = cell.begin();
		record_iter != record_iter_end;
		++record_iter) {
	    if ((*record_iter)->multi_key()[2] >= window_zmin &&
		 (*record_iter)->multi_key()[2] <= window_zmax) {
	      *(iter++) = *record_iter;
	      ++count;
	    }
	  }
	}
      }
    }

  }
  else { 
    // This version is used if there are a small number of cells.
    for (i = istart; i <= istop; ++i) {
      for (j = jstart; j <= jstop; ++j) {
	const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	  = _vector_array(i,j);
	cell_iter = cell_vector.lower_bound(kstart);
	cell_iter_end = cell_vector.end();
	for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
	      ++cell_iter) {
	  cell_const_reference cell = cell_iter->cell;
	  record_iter_end = cell.end();
	  for (record_iter = cell.begin();
		record_iter != record_iter_end;
		++record_iter) {
	    if ((*record_iter)->multi_key()[0] >= window_xmin &&
		 (*record_iter)->multi_key()[0] <= window_xmax &&
		 (*record_iter)->multi_key()[1] >= window_ymin &&
		 (*record_iter)->multi_key()[1] <= window_ymax &&
		 (*record_iter)->multi_key()[2] >= window_zmin &&
		 (*record_iter)->multi_key()[2] <= window_zmax) {
	      *(iter++) = *record_iter;
	      ++count;
	    }
	  }
	}
      }
    }
  }

  /***********************************
   //
   // Scan convert the window.
   //
      for (i = istart; i <= istop; ++i)
	for (j = jstart; j <= jstop; ++j) {
	  SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_vector
	    = _vector_array(i,j);
	  cell_iter = cell_vector.lower_bound(kstart);
	  cell_iter_end = cell_vector.end();
	  for (; cell_iter != cell_iter_end && cell_iter->index <= kstop; 
		++cell_iter) {
	    cell_reference cell = cell_iter->cell;
	    // If this is an interior cell.
	    if (i > istart && i < istop && 
		 j > jstart && j < jstop && 
		 cell_iter->index > kstart && cell_iter->index < kstop) {
	      record_iter_end = cell.end();
	      for (record_iter = cell.begin();
		    record_iter != record_iter_end;
		    ++record_iter) {
		    *(iter++) = *record_iter;
		++count;
	      }
	    }
	    else { // This is a boundary cell.
	      record_iter_end = cell.end();
	      for (record_iter = cell.begin();
		    record_iter != record_iter_end;
		    ++record_iter) {
		if ((*record_iter)->multi_key()[0] >= window_xmin &&
		     (*record_iter)->multi_key()[0] <= window_xmax &&
		     (*record_iter)->multi_key()[1] >= window_ymin &&
		     (*record_iter)->multi_key()[1] <= window_ymax &&
		     (*record_iter)->multi_key()[2] >= window_zmin &&
		     (*record_iter)->multi_key()[2] <= window_zmax) {
		     *(iter++) = *record_iter;
		  ++count;
		}
	      }
	    }
	  }
	}
  ***************************************/
  return count;
}

/*!
  Indexing function.  Return a const reference to a cell.
  \param i \f$ i < d\_xsize \f$.
  \param j \f$ j < d\_ysize \f$.
  \param k \f$ k < d\_zsize \f$.
*/
/*
  template <typename RecordType>
  inline
  typename SparseCellArray<RecordType>::const_cell_reference
  SparseCellArray<RecordType>::
  operator()(int i, int j, int k) 
  const 
  {
  return get_const_cell(_cell_array[k][j], i);
  }
*/

//
// Manipulators: 3D Indexing
//

/*!
  Indexing function.  Return a reference to a cell.
  \param i \f$ i < d\_xsize \f$.
  \param j \f$ j < d\_ysize \f$.
  \param k \f$ k < d\_zsize \f$.
*/
/*
  template <typename RecordType>
  inline
  typename SparseCellArray<RecordType>::cell_reference
  SparseCellArray<RecordType>::
  operator()(int i, int j, int k)
  {
  return _vector_array(i,j).find(k);
  }
*/

//
// File IO
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void
SparseCellArray<RecordType,MultiKeyType,KeyType>::
put(std::ostream& out) const
{
  base_type::put(out);

  typename cell_type::const_iterator record_iter;
  typename SparseCellVector<RecordType,MultiKeyType,KeyType>::const_iterator 
    ic_iter;

  for (size_type i = 0; i < _vector_array.extent(0); ++i) {
    for (size_type j = 0; j < _vector_array.extent(1); ++j) {
      const SparseCellVector<RecordType,MultiKeyType,KeyType>& cell_array 
	= _vector_array(i,j);
      ic_iter = cell_array.begin();
      for (; ic_iter != cell_array.end(); ++ic_iter) {
	cell_const_reference b = ic_iter->cell;
	record_iter = b.begin();
	while (record_iter != b.end())
	  out << **(record_iter++) << '\n';
      }
    }
  }
}

END_NAMESPACE_GEOM

// End of file.
