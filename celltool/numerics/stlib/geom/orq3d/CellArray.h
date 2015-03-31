// -*- C++ -*-

/*! 
  \file CellArray.h
  \brief A class for a cell array in 3-D.
*/

#if !defined(__geom_CellArray_h__)
#define __geom_CellArray_h__

#include "CellArrayBase.h"

#include "../../ads/array/Array.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_CellArray)
#define DEBUG_CellArray
#endif

BEGIN_NAMESPACE_GEOM

//! A cell array in 3-D.
/*!
  A dense cell array in 3-D holding pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  the multi-key of the record.
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type>
class CellArray :
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

  //! The cell type.
  typedef typename base_type::cell_type cell_type;
  //! A reference to a cell.
  typedef typename base_type::cell_reference cell_reference;
  //! A const reference to a cell.
  typedef typename base_type::cell_const_reference cell_const_reference;

private:

  //
  // Private types.
  //

  typedef ads::Array<3,cell_type> array_type;

  //
  // Data
  //

  //! The array of cells.
  array_type _cell_array;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  CellArray();

  //! Copy constructor not implemented
  CellArray( const CellArray& );

  //! Assignment operator not implemented
  CellArray& 
  operator=( const CellArray& );

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct a cell array given the cell size and the Cartesian domain 
    that contains the records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
  */
  CellArray( const point_type& delta,
	     const semi_open_interval_type& domain ) :
    base_type( delta, domain ),
    _cell_array( extents() )
  {}

  //! Construct from a range of records.
  /*!
    Construct a cell grid given the cell size, the Cartesian domain
    and a range of records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template <class InputIterator>
  CellArray( const point_type& delta,
	     const semi_open_interval_type& domain, 
	     InputIterator first, InputIterator last ) :
    base_type( delta, domain ),
    _cell_array( extents() )
  {
    // Insert the grid elements in the range.
    insert( first, last );
  }

  //! Trivial Destructor.
  ~CellArray()
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
  template <typename InputIterator>
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

  //! Get the records in the window.  Return the # of records inside.
  template<typename OutputIterator>
  size_type
  window_query( OutputIterator iter, const bbox_type& window ) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the records.
  void 
  put( std::ostream& out ) const;

  // @}

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
  // Accesors: 3-D Cell Indexing
  //

  //! Return a reference to the cell that would hold the point.
  /*!
    Indexing by location.  Return a reference to a cell.
    The multi-key must be in the domain of the cell array.
  */
  template <typename AnyMultiKeyType>
  cell_reference 
  operator()( const AnyMultiKeyType& multi_key )
  {
    int i, j, k;
    multi_key_to_indices( multi_key, i, j, k );
    return _cell_array( i, j, k );
  }

};

//
// File I/O
//

//! Write to a file stream.
/*! \relates CellArray */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const CellArray<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_CellArray_ipp__
#include "CellArray.ipp"
#undef __geom_CellArray_ipp__

#endif
