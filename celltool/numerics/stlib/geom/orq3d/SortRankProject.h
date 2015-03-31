// -*- C++ -*-

/*! 
  \file geom/orq3d/SortRankProject.h
  \brief A class for an xyz sorted and ranked vector of grid elements in 3D.
  The class implements the point in box method.
*/

#if !defined(__geom_orq3d_SortRankProject_h__)
#define __geom_orq3d_SortRankProject_h__

#include "ORQ.h"
#include "RecordCompare.h"

#include "../../ads/algorithm/sort.h"
#include "../../ads/iterator/IndirectIterator.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SortRankProject)
#define DEBUG_SortRankProject
#endif

BEGIN_NAMESPACE_GEOM

//! A sorted and ranked vector of grid elements in 3D.
/*!
  An xyz sorted and ranked vector of pointers to RecordType.
  RecordType must have the member function multi_key() that returns 
  the multi-key of the record.

  This class implements the point-in-box method developed by Swegle, 
  et. al.  
*/
template <typename RecordType,
	  typename MultiKeyType = typename RecordType::multi_key_type,
	  typename KeyType = typename RecordType::key_type>
class SortRankProject : 
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

private:

  //
  // Member data.
  //

  //! The vector of pointers to records.
  std::vector<value_type> _record_pointers;

  //! Pointers to elements sorted by x, y and z coordinate.
  std::vector<pointer> _xsorted, _ysorted, _zsorted;

  //! The rank of the records in the x, y and z coordinate.
  std::vector<int> _xrank, _yrank, _zrank;

private:

  //
  // Not implemented
  //

  //! Copy constructor not implemented
  SortRankProject( const SortRankProject& );

  //! Assignment operator not implemented
  SortRankProject& 
  operator=( const SortRankProject& );

public:

  //-------------------------------------------------------------------------
  //! \name Constructors.
  // @{

  //! Default constructor.
  SortRankProject();

  //! Reserve storage for \c size records.
  explicit 
  SortRankProject( const size_type size );

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template <typename InputIterator>
  SortRankProject( InputIterator first, InputIterator last );

  // Trivial destructor.
  ~SortRankProject()
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
  memory_usage() const;
  
  // @}
  //-------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Sort and rank the records by x, y and z coordinates.
  void 
  sort_rank();

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
/*! \relates SortRankProject */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<( std::ostream& out, 
	    const SortRankProject<RecordType,MultiKeyType,KeyType>& x )
{
  x.put( out );
  return out;
}

END_NAMESPACE_GEOM

#define __geom_SortRankProject_ipp__
#include "SortRankProject.ipp"
#undef __geom_SortRankProject_ipp__

#endif
