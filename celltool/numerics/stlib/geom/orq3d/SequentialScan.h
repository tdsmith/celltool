// -*- C++ -*-

/*! 
  \file geom/orq3d/SequentialScan.h
  \brief The sequential scan algorithm.
*/

#if !defined(__geom_orq3d_SequentialScan_h__)
#define __geom_orq3d_SequentialScan_h__

#include "ORQ.h"
#include "RecordCompare.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SequentialScan)
#define DEBUG_SequentialScan
#endif

BEGIN_NAMESPACE_GEOM

//! The sequential scan algorithm for ORQ's in 3-D.
/*!
  Stores a vector of pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  the multi-key of the record.
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type>
class SequentialScan :
  public ORQ<RecordType,MultiKeyType,KeyType>
{
private:

  //
  // Private types.
  //

  typedef ORQ<RecordType,MultiKeyType,KeyType> base_type;

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
  // Member data.
  //

  //! The vector of pointers to records.
  std::vector<value_type> _record_pointers;

private:

  //
  // Not implemented
  //

  //! Copy constructor not implemented
  SequentialScan( const SequentialScan& );

  //! Assignment operator not implemented
  SequentialScan& 
  operator=( const SequentialScan& );

public:

  //-------------------------------------------------------------------------
  //! \name Constructors.
  // @{

  //! Default constructor.
  SequentialScan() :
    base_type(),
    _record_pointers()
  {}

  //! Reserve storage for \c size records.
  explicit 
  SequentialScan( const size_type size ) :
    base_type(),
    _record_pointers()
  {
    _record_pointers.reserve( size );
  }

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template <typename InputIterator>
  SequentialScan( InputIterator first, InputIterator last ) :
    base_type(),
    _record_pointers()
  {
    insert( first, last );
  }

  // Trivial destructor.
  ~SequentialScan()
  {}

  // @}
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
  //-------------------------------------------------------------------------
  //! \name Insert records.
  // @{

  //! Add a single record.
  void 
  insert( value_type record_pointer )
  {
    _record_pointers.push_back( record_pointer );
    increment_num_records();
  }

  //! Add a range of records.
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
  //-------------------------------------------------------------------------
  //! \name Memory usage.
  // @{
  
  //! Return the total memory usage.
  size_type 
  memory_usage() const
  {
    return ( sizeof( std::vector<value_type> ) +
	     _record_pointers.size() * sizeof( value_type ) );
  }
  
  // @}
  //-------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Get the records in the window.  Return the # of records inside.
  template <typename OutputIterator>
  size_type 
  window_query( OutputIterator iter, const bbox_type& window ) const;

  // @}
  //-------------------------------------------------------------------------
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
};

//! Write to a file stream.
/*! \relates SequentialScan */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const SequentialScan<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_SequentialScan_ipp__
#include "SequentialScan.ipp"
#undef __geom_SequentialScan_ipp__

#endif
