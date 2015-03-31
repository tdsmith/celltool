// -*- C++ -*-

/*! 
  \file geom/orq3d/PlaceboCheck.h
  \brief A placebo class.
*/

#if !defined(__geom_orq3d_PlaceboCheck_h__)
#define __geom_orq3d_PlaceboCheck_h__

#include "Placebo.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_PlaceboCheck)
#define DEBUG_PlaceboCheck
#endif

BEGIN_NAMESPACE_GEOM

//! A placebo for ORQ's in 3-D
/*!
  Stores a vector of pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  the multi-key of the record.
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type>
class PlaceboCheck : 
  public Placebo<RecordType,MultiKeyType,KeyType>
{
private:

  //
  // Private types.
  //

  typedef Placebo<RecordType,MultiKeyType,KeyType> base_type;

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
  //! Bounding box.
  typedef typename base_type::bbox_type bbox_type;

private:

  //
  // Not implemented.
  //

  //! Copy constructor not implemented
  PlaceboCheck( const PlaceboCheck& );

  //! Assignment operator not implemented
  PlaceboCheck& 
  operator=( const PlaceboCheck& );

public:

  //-------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Default constructor.
  PlaceboCheck() :
    base_type()
  {}

  //! Reserve storage for \c size records.
  explicit 
  PlaceboCheck( const size_type size ) :
    base_type( size )
  {}

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template <typename InputIterator>
  PlaceboCheck( InputIterator first, InputIterator last ) :
    base_type( first, last )
  {}

  // Trivial destructor.
  ~PlaceboCheck()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Window queries.
  //@{

  //! Get the records in the window.  Return the # of records inside.
  template <typename OutputIterator>
  size_type 
  window_query( OutputIterator iter, const bbox_type& window ) const;

  //@}
  //-------------------------------------------------------------------------
  //! \name Insert records.
  //@{

  //! Add a single record.
  void 
  insert( value_type record_pointer )
  {
    base_type::insert( record_pointer );
  }

  //! Add a range of records.
  template <class InputIterator>
  void 
  insert( InputIterator first, InputIterator last )
  {
    base_type::insert( first, last );
  }
  
  //@}
  //-------------------------------------------------------------------------
  //! \name Memory usage.
  //@{
  
  //! Return the total memory usage.
  size_type 
  memory_usage() const
  {
    return base_type::memory_usage();
  }
  
  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //
  // Inherited from ORQ.
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

  //
  // Inherited from Placebo.
  //

  //! Return the number of records to return with each query.
  size_type
  query_size() const
  { 
    return base_type::query_size();
  }

  //! Return the vector of pointers to records.
  const std::vector<value_type>&
  record_pointers() const
  {
    return base_type::record_pointers();
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Set the number of records to return with each query.
  void
  set_query_size( const size_type num )
  { 
    base_type::set_query_size( num );
  }

  //! Return the vector of pointers to records.
  std::vector<value_type>&
  record_pointers()
  {
    return base_type::record_pointers();
  }

  //! Shuffle the record pointers.
  void
  shuffle()
  {
    base_type::shuffle();
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Print the records.
  void 
  put( std::ostream& out ) const
  {
    base_type::put( out );
  }

  //@}

protected:

  //! Increment the number of records.
  void
  increment_num_records()
  {
    base_type::increment_num_records();
  }

  //! Return a starting point for the window query.
  int 
  starting_point() const
  { 
    return base_type::starting_point();
  }
};

//! Write to a file stream.
/*! \relates PlaceboCheck */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const PlaceboCheck<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_PlaceboCheck_ipp__
#include "PlaceboCheck.ipp"
#undef __geom_PlaceboCheck_ipp__

#endif
