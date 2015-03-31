// -*- C++ -*-

/*! 
  \file geom/orq3d/Placebo.h
  \brief A placebo class.
*/

#if !defined(__geom_orq3d_Placebo_h__)
#define __geom_orq3d_Placebo_h__

#include "ORQ.h"
#include "RecordCompare.h"

#include "../../ads/functor/UniformRandom.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_Placebo)
#define DEBUG_Placebo
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
class Placebo : 
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

  //! The number of records to return with each query.
  size_type _query_size;

  //! Random number generator.
  mutable ads::SubtractiveRNG _random;

private:

  //
  // Not implemented
  //

  //! Copy constructor not implemented
  Placebo( const Placebo& );

  //! Assignment operator not implemented
  Placebo& 
  operator=( const Placebo& );

public:

  //-------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Default constructor.
  Placebo() :
    base_type(),
    _record_pointers(),
    _query_size( 0 )
  {}

  //! Reserve storage for \c size records.
  explicit 
  Placebo( const size_type size ) :
    base_type(),
    _record_pointers(),
    _query_size( 0 )
  {
    _record_pointers.reserve( size );
  }

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template <typename InputIterator>
  Placebo( InputIterator first, InputIterator last ) :
    base_type(),
    _record_pointers(),
    _query_size( 0 )
  {
    insert( first, last );
    shuffle();
  }

  // Trivial destructor.
  ~Placebo()
  {}

  //@}
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

  //
  // New.
  //

  //! Return the number of records to return with each query.
  size_type
  query_size() const
  { 
    return _query_size;
  }

  //! Return the vector of pointers to records.
  const std::vector<value_type>&
  record_pointers() const
  {
    return _record_pointers;
  }

  // @}
  //-------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Set the number of records to return with each query.
  void
  set_query_size( const size_type num )
  { 
    _query_size = num;
  }

  //! Return the vector of pointers to records.
  std::vector<value_type>&
  record_pointers()
  {
    return _record_pointers;
  }

  //! Shuffle the record pointers.
  void
  shuffle()
  {
    std::random_shuffle( _record_pointers.begin(), _record_pointers.end() );
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Insert records.
  //@{

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
    shuffle();
  }

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
  //! \name Memory usage.
  //@{
  
  //! Return the memory requirement for storing the record pointers.
  size_type 
  memory_usage() const
  {
    return ( sizeof( std::vector<value_type> ) +
	     _record_pointers.size() * sizeof( value_type ) );
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name File I/O.
  //@{

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

  //! Return a starting point for the window query.
  int 
  starting_point() const
  { 
    return _random( _record_pointers.size() - _query_size ); 
  }
};

//! Write to a file stream.
/*! \relates Placebo */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const Placebo<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM  

#define __geom_Placebo_ipp__
#include "Placebo.ipp"
#undef __geom_Placebo_ipp__

#endif
