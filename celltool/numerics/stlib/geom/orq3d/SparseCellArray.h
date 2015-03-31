// -*- C++ -*-

/*! 
  \file SparseCellArray.h
  \brief A class for a sparse cell array in 3-D.
*/

#if !defined(__geom_SparseCellArray_h__)
#define __geom_SparseCellArray_h__

#include "CellArrayBase.h"

#include "../../ads/array/Array.h"

#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SparseCellArray)
#define DEBUG_SparseCellArray
#endif

BEGIN_NAMESPACE_GEOM

//! An index and cell for holding records.
template <typename RecordType, typename MultiKeyType, typename KeyType>
struct IndexAndCell
{
  //! The index of the cell.
  int index;
  //! The cell containing pointers to records.
  typename CellArrayBase<RecordType,MultiKeyType,KeyType>::cell_type cell;
};

//! Less than comparison for indices.
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
bool
operator<( const IndexAndCell<RecordType,MultiKeyType,KeyType>& a, 
	   const IndexAndCell<RecordType,MultiKeyType,KeyType>& b )
{
  return a.index < b.index;
}

//! A vector of sparse cells.
template <typename RecordType, typename MultiKeyType, typename KeyType>
class SparseCellVector :
  public std::vector< IndexAndCell<RecordType,MultiKeyType,KeyType> >
{
private:
      
  //
  // Private types.
  //

  typedef std::vector< IndexAndCell<RecordType,MultiKeyType,KeyType> > 
  base_type;

  // Assignment operator not implemented.
  SparseCellVector& 
  operator=( const SparseCellVector& );

public:

  //
  // Public types.
  //

  //! A pointer to a record.
  typedef typename base_type::value_type value_type;
  //! An iterator on the value type.
  typedef typename base_type::iterator iterator;
  //! A const iterator on the value type.
  typedef typename base_type::const_iterator const_iterator;
  //! The size type.
  typedef typename base_type::size_type size_type;

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Default constructor.
  SparseCellVector() : 
    base_type()
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  SparseCellVector( const size_type n ) : 
    base_type( n )
  {}

  //! Copy constructor.
  SparseCellVector( const SparseCellVector& x ) : 
    base_type( x )
  {}

  //! Construct from a range.
  template <typename InputIterator>
  SparseCellVector( InputIterator first, InputIterator last ) : 
    base_type( first, last )
  {}

  //! Destructor.
  ~SparseCellVector()
  {}

  // @}  
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from std::vector.
  */
  // @{

  //! Return the beginning of the range.
  const_iterator
  begin() const 
  {
    return base_type::begin();
  }

  //! Return the end of the range.
  const_iterator
  end() const 
  {
    return base_type::end();
  }

  // @}  
  //--------------------------------------------------------------------------
  /*! \name Manipulators.
    Functionality inherited from std::vector.
  */
  // @{

  //! Return the beginning of the range.
  iterator
  begin()
  {
    return base_type::begin();
  }

  //! Return the end of the range.
  iterator
  end()
  {
    return base_type::end();
  }

  //! Insert the value before the iterator.
  /*!
    \return Return an iterator to the inserted element.
  */
  iterator
  insert( iterator position, const value_type& x )
  {
    return base_type::insert( position, x );
  }

  // @}  
  //--------------------------------------------------------------------------
  //! \name Searching.
  // @{

  // Return a const iterator to the first index and cell with index >= i.
  const_iterator 
  lower_bound( const int i ) const
  {
    IndexAndCell<RecordType,MultiKeyType,KeyType> val;
    val.index = i;
    return std::lower_bound( begin(), end(), val );
  }

  // Return an iterator to the first index and cell with index >= i.
  iterator 
  lower_bound( const int i )
  {
    IndexAndCell<RecordType,MultiKeyType,KeyType> val;
    val.index = i;
    return std::lower_bound( begin(), end(), val );
  }

  // Return an iterator to cell i.
  typename CellArrayBase<RecordType,MultiKeyType,KeyType>::cell_reference 
  find( const int i )
  {
    IndexAndCell<RecordType,MultiKeyType,KeyType> val;
    val.index = i;
    iterator iter = std::lower_bound( begin(), end(), val );

    // If the cell does not yet exist, make it.
    if ( iter == end() || iter->index != i ) {
      iter = insert( iter, val );
    }

    return iter->cell;
  }

  // @}
};
    

//! A sparse cell array in 3-D.
/*!
  A sparse cell array in 3-D holding pointers to class RecordType.
  RecordType must have the member function multi_key() that returns 
  a const reference to the multi-key of the record.
  The array is sparse in the z dimension.  Cell access is accomplished
  with array indexing in the x and y directions and a binary search
  of a sorted vector in the z direction.
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type>
class SparseCellArray :
  public CellArrayBase<RecordType, MultiKeyType, KeyType>
{
private:

  //
  // Private types.
  //

  typedef CellArrayBase<RecordType, MultiKeyType, KeyType> base_type;

public:

  //
  // Public types.
  //

  //! The record type.
  typedef typename base_type::record_type record_type;
  //! A pointer to the record type.
  typedef typename base_type::value_type value_type;
  //! A pointer to the value_type.
  typedef typename base_type::pointer pointer;
  //! A const pointer to the value_type.
  typedef typename base_type::const_pointer const_pointer;
  //! A reference to the value_type.
  typedef typename base_type::reference reference;
  //! A const reference to the value_type.
  typedef typename base_type::const_reference const_reference;
  //! The size type.
  typedef typename base_type::size_type size_type;
  //! The multi-key type.
  typedef typename base_type::multi_key_type multi_key_type;
  //! The key type.
  typedef typename base_type::key_type key_type;
  //! A Cartesian point.
  typedef typename base_type::point_type point_type;
  //! Bounding box.
  typedef typename base_type::bbox_type bbox_type;
  //! Semi-open interval.
  typedef typename base_type::semi_open_interval_type semi_open_interval_type;

protected:

  //
  // Protected types.
  //

  //! 2-D array of 1-D sparce cell vectors.
  typedef ads::Array< 2, SparseCellVector<record_type, multi_key_type, 
					  key_type> > vector_array_type;

  //! The cell type.
  typedef typename base_type::cell_type cell_type;
  //! A reference to a cell.
  typedef typename base_type::cell_reference cell_reference;
  //! A const reference to a cell.
  typedef typename base_type::cell_const_reference cell_const_reference;

private:

  // 
  // Member data
  //

  //! The array of vectors.
  vector_array_type _vector_array;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  SparseCellArray();

  //! Copy constructor not implemented
  SparseCellArray( const SparseCellArray& );

  //! Assignment operator not implemented
  SparseCellArray& 
  operator=( const SparseCellArray& );

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct a sparse cell array given the cell size and the Cartesian 
    domain that the cell array spans.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain spanned by the records.
  */
  SparseCellArray( const point_type& delta,
		   const semi_open_interval_type& domain ) :
    base_type( delta, domain ),
    _vector_array( typename vector_array_type::index_type( extents()[0], 
							   extents()[1] ) )
  {}

  //! Construct from a range of records.
  /*!
    Construct a sparse cell array given the size of a cell, the Cartesian 
    domain, and a range of records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template <class InputIterator>
  SparseCellArray( const point_type& delta,
		   const semi_open_interval_type& domain, 
		   InputIterator first, InputIterator last ) :
    base_type( delta, domain ),
    _vector_array( typename vector_array_type::index_type( extents()[0], 
							   extents()[1] ) )
  {
    // Insert the grid elements in the range.
    insert( first, last );
  }

  //! Trivial Destructor.
  ~SparseCellArray()
  {}

  // @}
  //--------------------------------------------------------------------------
  /*! \name Accesors.
    Functionality inherited from CellArrayBase.
  */
  // @{

  //! Return the number of records.
  size_type 
  num_records() const 
  { 
    return base_type::num_records();
  }

  //! Return true if the grid is empty.
  bool 
  is_empty() const 
  { 
    return base_type::is_empty();
  }

  //! Return the domain spanned by the grid.
  const semi_open_interval_type& 
  domain() const 
  { 
    return base_type::domain();
  }

  //! Return the number of cells in each dimension.
  const ads::FixedArray<3,size_type>& 
  extents() const
  {
    return base_type::extents();
  }

  //! Return the cell size.
  const point_type& 
  delta() const
  {
    return base_type::delta();
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Insert records.
  // @{

  //! Add a single record.
  void 
  insert( const value_type record_pointer )
  {
    cell_reference b = (*this)( record_pointer->multi_key() );
    b.push_back( record_pointer );
    increment_num_records();
  }

  //! Add a range of records.
  /*!
    The input iterators are to a container of type RecordType.
  */
  template <class InputIterator>
  void 
  insert( InputIterator first, InputIterator last )
  {
    while ( first != last ) {
      insert( &*first );
      ++first;
    }
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  // @{
  
  //! Return the memory usage.
  size_type 
  memory_usage() const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Get the grid points in the window.  Return the # of grid pts inside.
  template< class OutputIterator >
  size_type 
  window_query( OutputIterator iter, const bbox_type& window ) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the records.
  void 
  put( std::ostream& out ) const;

  //@}

protected:

  //! Increment the number of records.
  void
  increment_num_records()
  {
    base_type::increment_num_records();
  }


  //! Convert the multi-key to a cell array index.
  template <typename AnyMultiKeyType>
  void 
  multi_key_to_indices( const AnyMultiKeyType& multi_key,
			int& i, int& j, int& k ) const
  {
    base_type::multi_key_to_indices( multi_key, i, j, k );
  }

private:

  //  
  // Accesors: 3D Cell Indexing
  //

  // CONTINUE: Is this used?
  //! Return a reference to the cell that would hold the point.
  /*!
    Indexing by location.  Return a reference to a cell.
    \param pt \f$ xmin() \leq pt.x() < xmax() \f$,
    \f$ ymin() \leq pt.y() < ymax() \f$,
    \f$ zmin() \leq pt.z() < zmax() \f$.
  */
  cell_reference 
  operator()( const multi_key_type& multi_key )
  {
    int i, j, k;
    multi_key_to_indices( multi_key, i, j, k );
    return _vector_array(i,j).find( k );
  }

};

//
// File I/O
//

//! Write to a file stream.
/*! \relates SparseCellArray */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const SparseCellArray<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_SparseCellArray_ipp__
#include "SparseCellArray.ipp"
#undef __geom_SparseCellArray_ipp__

#endif
