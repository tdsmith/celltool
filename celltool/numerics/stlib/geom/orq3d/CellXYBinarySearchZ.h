// -*- C++ -*-

/*! 
  \file geom/orq3d/CellXYBinarySearchZ.h
  \brief A class for a cell array coupled with a binary search.
*/

#if !defined(__geom_orq3d_CellXYBinarySearchZ_h__)
#define __geom_orq3d_CellXYBinarySearchZ_h__

#include "CellXYSearchZ.h"

BEGIN_NAMESPACE_GEOM

//! Binary search of records in the z coordinate.
template <typename RecordType, typename MultiKeyType, typename KeyType>
class BinarySearchZ :
  public SearchZ<RecordType,MultiKeyType,KeyType>
{
private:
      
  typedef SearchZ<RecordType,MultiKeyType,KeyType> base_type;

public:
      
  //
  // Public types.
  //

  //! A pointer to the record type.
  typedef typename base_type::value_type value_type;
  //! A pointer to the value_type.
  typedef typename base_type::pointer pointer;
  //! A const pointer to the value_type.
  typedef typename base_type::const_pointer const_pointer;
  //! An iterator to the value_type.
  typedef typename base_type::iterator iterator;
  //! A const iterator to the value_type.
  typedef typename base_type::const_iterator const_iterator;
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

private:

  //
  // Not implemented.
  //

  //! Assignment operator not implemented.
  BinarySearchZ& 
  operator=( const BinarySearchZ& );

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Default constructor.
  BinarySearchZ() :
    base_type()
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  BinarySearchZ( const size_type n ) :
    base_type( n )
  {}

  //! Copy constructor.
  BinarySearchZ( const BinarySearchZ& x ) :
    base_type( x )
  {}

  //! Construct from a range.
  template <typename InputIterator>
  BinarySearchZ( InputIterator first, InputIterator last ) :
    base_type( first, last )
  {}

  //! Destructor.
  ~BinarySearchZ()
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
    return ( sizeof( BinarySearchZ ) + size() * sizeof( value_type ) );
  }
      
  // @}
  //--------------------------------------------------------------------------
  //! \name Searching.
  // @{
      
  //! Do a binary search to find the record.
  template <typename AnyMultiKeyType>
  iterator
  search( const AnyMultiKeyType& x )
  {
    return std::lower_bound( begin(), end(), x, 
			     zless_rh_m<value_type, AnyMultiKeyType>() );
  }
  
  //! Do a binary search to find the record.
  template <typename AnyMultiKeyType>
  const_iterator
  search( const AnyMultiKeyType& x ) const
  {
    return std::lower_bound( begin(), end(), x, 
			     zless_rh_m<value_type, AnyMultiKeyType>() );
  }
  
  // @}
};
    

//! A cell array in X, Y and binary search in Z.
/*!
  This holds pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  a const reference to the multi-key of the record.
  The cell array spans the x and y directions.  Record access 
  is accomplished with array indexing in the x and y directions and a 
  binary search of a sorted vector in the z direction.
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type, 
	  bool BoundarySpecialization = true>
class CellXYBinarySearchZ :
  public CellXYSearchZ< RecordType, MultiKeyType, KeyType, 
			BinarySearchZ<RecordType,MultiKeyType,KeyType> >
{
private:

  //
  // Private types.
  //

  typedef CellXYSearchZ< RecordType, MultiKeyType, KeyType, 
			 BinarySearchZ<RecordType,MultiKeyType,KeyType> > 
  base_type;

protected:

  //
  // Protected types.
  //

  //! The cell type.
  typedef typename base_type::cell_type cell_type;
  // CONTINUE: Perhaps change to index_type.
  //! The cell array extent type.
  typedef typename base_type::cell_array_extent_type cell_array_extent_type;

public:

  //
  // Public types.
  //

  //! The record type.
  typedef typename base_type::record_type record_type;
  //! A pointer to RecordType.
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
  // Not implemented
  //

  //! Default constructor not implemented.
  CellXYBinarySearchZ();

  //! Copy constructor not implemented
  CellXYBinarySearchZ( const CellXYBinarySearchZ& );

  //! Assignment operator not implemented
  CellXYBinarySearchZ& 
  operator=( const CellXYBinarySearchZ& );

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct given the cell size and the Cartesian domain that
    contains the records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
  */
  CellXYBinarySearchZ( const point_type& delta,
		       const semi_open_interval_type& domain ) :
    base_type( delta, domain )
  {}

  //! Construct from a range of records.
  /*!
    Construct given the cell size, the Cartesian domain that
    contains the records and a range of records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template <typename InputIterator>
  CellXYBinarySearchZ( const point_type& delta,
		       const semi_open_interval_type& domain, 
		       InputIterator first, InputIterator last ) :
    base_type( delta, domain, first, last )
  {}

  //! Trivial Destructor.
  ~CellXYBinarySearchZ()
  {}

  //@}
  //--------------------------------------------------------------------------
  /*! \name Accesors.
    Functionality inherited from ORQ.
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

  // @}
  //--------------------------------------------------------------------------
  /*! \name Insert records.
    Functionality inherited from CellXYSearchZ.
  */
  //@{

  //! Insert a single record.
  void 
  insert( const value_type record_pointer )
  { 
    return base_type::insert( record_pointer );
  }

  //! Insert a range of records.
  /*!
    The input iterators are to a container of type RecordType.
  */
  template <typename InputIterator>
  void 
  insert( InputIterator first, InputIterator last )
  { 
    return base_type::insert( first, last );
  }
  
  //@}
  //--------------------------------------------------------------------------
  /*! \name Memory usage.
    Functionality inherited from CellXYSearchZ.
  */
  //@{
  
  //! Return the memory usage.
  size_type 
  memory_usage() const
  { 
    return base_type::memory_usage();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Window queries.
  //@{

  //! Get the records in the window.  Return the # of records inside.
  template <typename OutputIterator>
  size_type 
  window_query( OutputIterator iter, const bbox_type& window ) const;

  //@}
  //--------------------------------------------------------------------------
  /*! \name Sorting.
    Functionality inherited from CellXYSearchZ.
  */
  //@{

  //! Sort the records.
  void 
  sort()
  { 
    return base_type::sort();
  }

  //! Initialize for a set of queries.
  void 
  initialize()
  { 
    return base_type::initialize();
  }

  //@}
  //--------------------------------------------------------------------------
  /*! \name File I/O
    Functionality inherited from CellXYSearchZ.
  */
  //@{

  //! Print the data structure.
  void 
  put( std::ostream& out ) const
  { 
    return base_type::put( out );
  }

  //@}

protected:

  //
  // Inherited from CellXYSearchZ.
  //

  //! Convert the multi-key to a cell array index.
  template <typename AnyMultiKeyType>
  void 
  multi_key_to_indices( const AnyMultiKeyType& multi_key,
			int& i, int& j ) const
  {
    base_type::multi_key_to_indices( multi_key, i, j );
  }

  //! Return the extents of the cell array.
  const cell_array_extent_type&
  cell_array_extents() const
  {
    return base_type::cell_array_extents();
  }

  //! Return a reference to the specified cell.
  cell_type& 
  get_cell( const int i, const int j )
  {
    return base_type::get_cell( i, j );
  }

  //! Return a const reference to the specified cell.
  const cell_type& 
  get_cell( const int i, const int j ) const
  {
    return base_type::get_cell( i, j );
  }

};

//
// File I/O
//

//! Write to a file stream.
/*! \relates CellXYBinarySearchZ */
template <typename RecordType, typename MultiKeyType, typename KeyType, 
	  bool BoundarySpecialization>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const CellXYBinarySearchZ<RecordType,MultiKeyType,KeyType,
	    BoundarySpecialization>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_CellXYBinarySearchZ_ipp__
#include "CellXYBinarySearchZ.ipp"
#undef __geom_CellXYBinarySearchZ_ipp__

#endif
