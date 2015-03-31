// -*- C++ -*-

/*! 
  \file geom/orq3d/SortProject.h
  \brief A class for an xyz sorted vector of records in 3-D.
*/

#if !defined(__geom_orq3d_SortProject_h__)
#define __geom_orq3d_SortProject_h__

#include "ORQ.h"
#include "RecordCompare.h"

#include "../../ads/algorithm/sort.h"
#include "../../ads/iterator/IndirectIterator.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SortProject)
#define DEBUG_SortProject
#endif

BEGIN_NAMESPACE_GEOM

//! A sorted vector of records in 3-D.
/*!
  x, y, and z sorted vectors of pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  the multi-key of the record.
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type>
class SortProject : 
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
  //! A Cartesian point.
  typedef typename base_type::point_type point_type;
  //! Bounding box.
  typedef typename base_type::bbox_type bbox_type;

protected:

  //
  // Member data.
  //

  //! Pointers to elements sorted by x, y and z coordinate.
  std::vector<value_type> _xsorted, _ysorted, _zsorted;

private:

  //
  // Not implemented
  //

  //! Copy constructor not implemented
  SortProject( const SortProject& );

  //! Assignment operator not implemented
  SortProject& 
  operator=( const SortProject& );

public:

  //-------------------------------------------------------------------------
  //! \name Constructors.
  // @{

  //! Default constructor.
  SortProject() :
    base_type(),
    _xsorted(),
    _ysorted(),
    _zsorted()
  {}

  //! Reserve storage for \c size records.
  explicit 
  SortProject( const size_type size ) :
    base_type(),
    _xsorted(),
    _ysorted(),
    _zsorted()
  {
    _xsorted.reserve( size );
    _ysorted.reserve( size );
    _zsorted.reserve( size );
  }

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template <class InputIterator>
  SortProject( InputIterator first, InputIterator last ) :
    base_type(),
    _xsorted(),
    _ysorted(),
    _zsorted()
  {
    insert( first, last );
    sort();
  }

  // Trivial destructor.
  ~SortProject()
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
  insert( const value_type record_pointer )
  {
    _xsorted.push_back( record_pointer );
    _ysorted.push_back( record_pointer );
    _zsorted.push_back( record_pointer );
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
    return ( 3 * sizeof( std::vector<value_type> ) + 
	     3 * _xsorted.size() * sizeof( value_type ) );
  }

  // @}
  //-------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Sort the records by x, y and z coordinate.
  void 
  sort()
  {
    //
    // Sort in each direction.
    //
    std::sort( _xsorted.begin(), _xsorted.end(), xless_h<value_type>() );
    std::sort( _ysorted.begin(), _ysorted.end(), yless_h<value_type>() );
    std::sort( _zsorted.begin(), _zsorted.end(), zless_h<value_type>() );
  }

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
  //-------------------------------------------------------------------------
  //! \name Validity check.
  // @{

  //! Check the validity of the data structure.
  void 
  check() const;

  //@}

protected:

  //! Increment the number of records.
  void
  increment_num_records()
  {
    base_type::increment_num_records();
  }
};

//! Write to a file stream.
/*! \relates SortProject */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const SortProject<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_SortProject_ipp__
#include "SortProject.ipp"
#undef __geom_SortProject_ipp__

#endif
