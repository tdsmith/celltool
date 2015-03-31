// -*- C++ -*-

/*! 
  \file CellXYSearchZ.h
  \brief A base class for a cell array with searching in 3-D.
*/

#if !defined(__geom_CellXYSearchZ_h__)
#define __geom_CellXYSearchZ_h__

#include "ORQ.h"
#include "RecordCompare.h"

#include "../../ads/array/Array.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_CellSearch)
#define DEBUG_CellSearch
#endif

BEGIN_NAMESPACE_GEOM

//! Base class for a search structure in the Z direction.
template <typename RecordType, typename MultiKeyType, typename KeyType>
class SearchZ :
  public std::vector<RecordType*>
{
private:

  typedef std::vector<RecordType*> base_type;
      
public:
      
  //
  // Public types.
  //

  //! The record type.
  typedef RecordType record_type;
  //! A pointer to the record type.
  typedef typename base_type::value_type value_type;
  //! A pointer to value_type.
  typedef typename base_type::pointer pointer;
  //! A pointer to const value_type.
  typedef typename base_type::const_pointer const_pointer;
  //! An iterator on value_type.
  typedef typename base_type::iterator iterator;
  //! An iterator on const value_type.
  typedef typename base_type::const_iterator const_iterator;
  //! A reference to value_type.
  typedef typename base_type::reference reference;
  //! A reference to const value_type.
  typedef typename base_type::const_reference const_reference;
  //! The size type.
  typedef typename base_type::size_type size_type;

  //! The multi-key type.
  typedef MultiKeyType multi_key_type;
  //! The key type.
  typedef KeyType key_type;

private:

  // Assignment operator not implemented.
  SearchZ& 
  operator=( const SearchZ& );

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Default constructor.
  SearchZ() :
    base_type()
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  SearchZ( const size_type n ) :
    base_type( n )
  {
    base_type::clear();
  }

  //! Copy constructor.
  SearchZ( const SearchZ& x ) :
    base_type( x )
  {}

  //! Construct from a range.
  template <typename InputIterator>
  SearchZ( InputIterator first, InputIterator last ) :
    base_type( first, last )
  {}

  //! Destructor.
  ~SearchZ()
  {}
  
  // @}
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from std::vector.
  */
  // @{

  //! Return the number of records.
  size_type
  size() const 
  {
    return base_type::size();
  }

  //! Return the beginning of the range of record pointers.
  const_iterator
  begin() const
  {
    return base_type::begin();
  }

  //! Return the end of the range of record pointers.
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

  //! Return the beginning of the range of record pointers.
  iterator
  begin()
  {
    return base_type::begin();
  }

  //! Return the end of the range of record pointers.
  iterator
  end()
  {
    return base_type::end();
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  // @{
  
  //! Return the memory usage.
  size_type 
  memory_usage() const
  { 
    return ( sizeof( SearchZ ) + size() * sizeof( value_type ) );
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Mathematical member functions.
  // @{
  
  //! Sort the records.
  void sort()
  {
    std::sort( base_type::begin(), base_type::end(), zless_h<value_type>() );
  }

  //! Initialize for a set of queries.
  void initialize()
  {}

  // @}
};


//! Base class for a cell array in X,Y and a search structure in Z.
/*!
  A base class for a cell array in 3D holding pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  a const reference to the multi-key of the record.

  This class implements the common functionality of data structures
  which have cell arrays in the X and Y directions and have a search
  data structure in the Z direction.  It does not store pointers to 
  the records.  Instead it has info on the number and size of the cells.  
*/
template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
class CellXYSearchZ :
  public ORQ<RecordType,MultiKeyType,KeyType>
{
private:

  //
  // Private types.
  //

  typedef ORQ<RecordType,MultiKeyType,KeyType> base_type;

  //! The cell array type.
  typedef ads::Array<2,SearchStructureType> cell_array_type;

protected:

  //
  // Protected types.
  //

  //! The cell type.
  typedef SearchStructureType cell_type;
  //! The cell array extent type.
  typedef typename cell_array_type::index_type cell_array_extent_type;

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

private:

  //
  // Member data.
  //

  //! The semi-open domain spanned by the grid.
  key_type _xmin, _xmax, _ymin, _ymax;

  //! The 2D array of cells that span the x and y dimensions.
  cell_array_type _cell_array;

  //! Cell size.
  key_type _xdelta, _ydelta;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  CellXYSearchZ();

  //! Copy constructor not implemented
  CellXYSearchZ( const CellXYSearchZ& );

  //! Assignment operator not implemented
  CellXYSearchZ& 
  operator=( const CellXYSearchZ& );

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct given the cell size and the Cartesian domain 
    that contains the records.

    \param delta the suggested size of a cell.  The z coordinate 
    is ignored.
    \param domain the Cartesian domain spanned by the records.
    The z coordinate is ignored.
  */
  CellXYSearchZ( const point_type& delta,
		 const semi_open_interval_type& domain );

  //! Construct from a range of records.
  /*!
    Construct given the cell size, the Cartesian domain,
    and a range of records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template <typename InputIterator>
  CellXYSearchZ( const point_type& delta,
		 const semi_open_interval_type& domain, 
		 InputIterator first, InputIterator last );

  //! Trivial Destructor.
  ~CellXYSearchZ()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //
  // Inherited.
  //

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

  // @}
  //--------------------------------------------------------------------------
  //! \name Insert records.
  // @{

  // CONTINUE: Add const to all files.
  //! Add a single record.
  void 
  insert( const value_type record_pointer )
  {
    get_cell( record_pointer->multi_key() ).push_back( record_pointer );
    increment_num_records();
  }

  //! Add a range of records.
  /*!
    The input iterators are to a container of type RecordType.
  */
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
  //! \name Sorting and searching.
  // @{
  
  //! Sort the records.
  void 
  sort();

  //! Initialize for a set of queries.
  void 
  initialize()
  {
    // Loop over the search structures.
    for ( typename cell_array_type::iterator i = _cell_array.begin();
	  i != _cell_array.end();	++i ) {
      i->initialize();
    }
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the data structure.
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
			int& i, int& j ) const
  {
    i = int(  ( multi_key[0] - _xmin ) / _xdelta );
    j = int(  ( multi_key[1] - _ymin ) / _ydelta );
  }

  //! Return the extents of the cell array.
  const cell_array_extent_type&
  cell_array_extents() const
  {
    return _cell_array.extents();
  }

  //! Return a reference to the specified cell.
  cell_type& 
  get_cell( const int i, const int j )
  {
    return _cell_array( i, j );
  }

  //! Return a const reference to the specified cell.
  const cell_type& 
  get_cell( const int i, const int j ) const
  {
    return _cell_array( i, j );
  }

  //! Return a reference to the search structure that would hold the point.
  template <typename AnyMultiKeyType>
  cell_type& 
  get_cell( const AnyMultiKeyType& multi_key )
  {
    int i, j;
    multi_key_to_indices(  multi_key, i, j );
    return _cell_array( i, j );
  }

};

//
// File I/O
//

//! Write to a file stream.
/*! \relates CellXYSearchZ */
template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  typename SearchStructureType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const CellXYSearchZ<RecordType,MultiKeyType,KeyType,
	    SearchStructureType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_CellXYSearchZ_ipp__
#include "CellXYSearchZ.ipp"
#undef __geom_CellXYSearchZ_ipp__

#endif
