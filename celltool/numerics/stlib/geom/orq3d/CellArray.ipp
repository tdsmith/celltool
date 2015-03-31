// -*- C++ -*-

/*
  \file CellArray.ipp
  \brief A class for a cell array in 3-D.
*/

#if !defined(__geom_CellArray_ipp__)
#error This file is an implementation detail of the class CellArray.
#endif

BEGIN_NAMESPACE_GEOM

//
// Memory usage.
//
  
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
typename CellArray<RecordType,MultiKeyType,KeyType>::size_type
CellArray<RecordType,MultiKeyType,KeyType>::
memory_usage() const
{
  int usage = 0;
  for ( typename array_type::const_iterator i = _cell_array.begin();
	i != _cell_array.end();
	++i ) {
    usage += i->size() * sizeof( value_type );
  }
  usage += _cell_array.size() * sizeof( cell_type );
  return usage;
}

//
// Mathematical member functions
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
template< class OutputIterator >
inline
typename CellArray<RecordType,MultiKeyType,KeyType>::size_type
CellArray<RecordType,MultiKeyType,KeyType>::
window_query( OutputIterator iter, const bbox_type& window ) const
{
  //
  // Convert the multi-key to grid index coordinates.
  //

  int i0, j0, k0, i1, j1, k1;
  multi_key_to_indices( window.getLowerCorner(), i0, j0, k0 );
  multi_key_to_indices( window.getUpperCorner(), i1, j1, k1 );
  
  //
  // If the window does not intersect the domain do nothing.
  //

  if ( i0 >= static_cast<int>( extents()[0] ) || i1 < 0 || 
       j0 >= static_cast<int>( extents()[1] ) || j1 < 0 ||
       k0 >= static_cast<int>( extents()[2] ) || k1 < 0 ) {
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

  int istart = std::max( 0, i0 );
  int jstart = std::max( 0, j0 );
  int kstart = std::max( 0, k0 );

  int istop = std::min( static_cast<int>( extents()[0] ) - 1, i1 );
  int jstop = std::min( static_cast<int>( extents()[1] ) - 1, j1 );
  int kstop = std::min( static_cast<int>( extents()[2] ) - 1, k1 );

  //
  // Do the boundary of the window.
  //

  int count = 0;
  int i, j, k;
  typename cell_type::const_iterator record_iter;
  typename cell_type::const_iterator record_iter_end;

  // If the query window is larger than a cell in each dimension.
  if ( istart != istop && jstart != jstop && kstart != kstop ) {

    // The constant x boundary.
    for ( j = jstart; j <= jstop; ++j ) {
      for ( k = kstart; k <= kstop; ++k ) {
	// The bottom side.
	cell_const_reference bot = _cell_array( istart, j, k );
	record_iter_end = bot.end();
	for ( record_iter = bot.begin(); 
	      record_iter != record_iter_end;
	      ++record_iter ) {
	  const multi_key_type& p = (*record_iter)->multi_key();
	  if ( p[0] >= window_xmin &&
	       p[1] >= window_ymin && p[1] <= window_ymax &&
	       p[2] >= window_zmin && p[2] <= window_zmax ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
	// The top side.
	cell_const_reference top = _cell_array( istop, j, k );
	record_iter_end = top.end();
	for ( record_iter = top.begin();
	      record_iter != record_iter_end;
	      ++record_iter ) {
	  const multi_key_type& p = (*record_iter)->multi_key();
	  if ( p[0] <= window_xmax &&
	       p[1] >= window_ymin && p[1] <= window_ymax &&
	       p[2] >= window_zmin && p[2] <= window_zmax ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }
  
    // The constant y boundary.
    for ( i = istart + 1; i < istop; ++i ) {
      for ( k = kstart; k <= kstop; ++k ) {
	// The bottom side.
	cell_const_reference bot = _cell_array( i, jstart, k );
	record_iter_end = bot.end();
	for ( record_iter = bot.begin();
	      record_iter != record_iter_end;
	      ++record_iter ) {
	  const multi_key_type& p = (*record_iter)->multi_key();
	  if ( p[1] >= window_ymin && 
	       p[2] >= window_zmin && p[2] <= window_zmax ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
	// The top side.
	cell_const_reference top = _cell_array( i, jstop, k );
	record_iter_end = top.end();
	for ( record_iter = top.begin();
	      record_iter != record_iter_end;
	      ++record_iter ) {
	  const multi_key_type& p = (*record_iter)->multi_key();
	  if ( p[1] <= window_ymax && 
	       p[2] >= window_zmin && p[2] <= window_zmax ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    // The constant z boundary.
    for ( i = istart + 1; i < istop; ++i ) {
      for ( j = jstart + 1; j < jstop; ++j ) {
	// The bottom side.
	cell_const_reference bot = _cell_array( i, j, kstart );
	record_iter_end = bot.end();
	for ( record_iter = bot.begin();
	      record_iter != record_iter_end;
	      ++record_iter ) {
	  if ( (*record_iter)->multi_key()[2] >= window_zmin ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
	// The top side.
	cell_const_reference top = _cell_array( i, j, kstop );
	record_iter_end = top.end();
	for ( record_iter = top.begin();
	      record_iter != record_iter_end;
	      ++record_iter ) {
	  if ( (*record_iter)->multi_key()[2] <= window_zmax ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }

    //
    // Do the interior of the window.
    //
    for ( i = istart + 1; i < istop; ++i ) {
      for ( j = jstart + 1; j < jstop; ++j ) {
	for ( k = kstart + 1; k < kstop; ++k ) {
	  cell_const_reference cell = _cell_array( i, j, k );
	  record_iter_end = cell.end();
	  for ( record_iter = cell.begin();
		record_iter != record_iter_end;
		++record_iter ) {
	    *(iter++) = *record_iter;
	    ++count;
	  }
	}
      }
    }
  }
  else {
    // This version is used if the number of cells is small.
    for ( i = istart; i <= istop; ++i ) {
      for ( j = jstart; j <= jstop; ++j ) {
	for ( k = kstart; k <= kstop; ++k ) {
	  cell_const_reference cell = _cell_array( i, j, k );
	  record_iter_end = cell.end();
	  for ( record_iter = cell.begin();
		record_iter != record_iter_end;
		++record_iter ) {
	    const multi_key_type& p = (*record_iter)->multi_key();
	    if ( p[0] >= window_xmin && p[0] <= window_xmax &&
		 p[1] >= window_ymin && p[1] <= window_ymax &&
		 p[2] >= window_zmin && p[2] <= window_zmax ) {
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


//
// File I/O
//

template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
void
CellArray<RecordType,MultiKeyType,KeyType>::
put( std::ostream& out ) const
{
  base_type::put( out );

  for ( typename array_type::const_iterator i = _cell_array.begin();
	i != _cell_array.end(); ++i ) {
    cell_const_reference b = *i;
    typename cell_type::const_iterator iter( b.begin() );
    while ( iter != b.end() ) {
      out << **(iter++) << '\n';
    }
  }
}
    
END_NAMESPACE_GEOM

// End of file.
