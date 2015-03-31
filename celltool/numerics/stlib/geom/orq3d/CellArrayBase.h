// -*- C++ -*-

/*! 
  \file CellArrayBase.h
  \brief A base class for a cell array in 3-D.
*/

#if !defined(__geom_CellArrayBase_h__)
#define __geom_CellArrayBase_h__

#include "ORQ.h"

#include <vector>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_CellArrayBase)
#define DEBUG_CellArrayBase
#endif

BEGIN_NAMESPACE_GEOM

//! Base class for a cell array in 3-D.
/*!
  A base class for a cell array in 3-D holding pointers to RecordType.
  RecordType must have the const member function multi_key() that returns 
  a const reference to the multi-key of the record.

  This class implements the common functionality of dense and sparse
  cell arrays.  It does not store pointers to the records.  Instead
  it has info on the number and size of the cells.  
*/
template <typename RecordType, typename MultiKeyType, typename KeyType>
class CellArrayBase :
  public ORQ<RecordType, MultiKeyType, KeyType>
{
private:

  //
  // Private types.
  //

  typedef ORQ<RecordType, MultiKeyType, KeyType> base_type;

public:

  //
  // Public types.
  //

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
  typedef typename base_type::semi_open_interval_type 
  semi_open_interval_type;

  //! The cell type.
  typedef std::vector<value_type> cell_type;
  //! A reference to a cell.
  typedef cell_type& cell_reference;
  //! A const reference to a cell.
  typedef const cell_type& cell_const_reference;

private:

  //
  // Member data.
  //

  //! The domain spanned by the grid.
  semi_open_interval_type _domain;

  //! The number of cells in each dimension.
  ads::FixedArray<3,size_type> _extents;

  //! Cell size.
  point_type _delta;

private:

  //
  // Not implemented.
  //

  //! Default constructor not implemented.
  CellArrayBase();

  //! Copy constructor not implemented
  CellArrayBase(const CellArrayBase&);

  //! Assignment operator not implemented
  CellArrayBase& 
  operator=(const CellArrayBase&);

public:

  //---------------------------------------------------------------------------
  //! \name Constructors.
  // @{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct a cell array given the cell size and the Cartesian domain 
    that contains the records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain spanned by the records.
  */
  CellArrayBase(const point_type& delta,
		 const semi_open_interval_type& domain) :
    base_type(),
    _domain(domain),
    _extents(ceil((domain.getUpperCorner() - domain.getLowerCorner()) / 
		    delta)),
    _delta((domain.getUpperCorner() - domain.getLowerCorner()) / 
	    point_type(_extents))
  {}

  //! Trivial Destructor.
  ~CellArrayBase()
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

  //
  // New.
  //

  //! Return the domain spanned by the grid.
  const semi_open_interval_type& 
  domain() const 
  { 
    return _domain; 
  }

  //! Return the number of cells in each dimension.
  const ads::FixedArray<3,size_type>& 
  extents() const
  {
    return _extents;
  }

  //! Return the cell size.
  const point_type& 
  delta() const
  {
    return _delta;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the data structure.
  void 
  put(std::ostream& out) const
  {
    out << _domain << '\n'
	<< _extents << '\n'
	<< _delta << '\n';
  }

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
  multi_key_to_indices(const AnyMultiKeyType& multi_key,
			int& i, int& j, int& k) const
  {
    i = int((multi_key[0] - _domain.getLowerCorner()[0]) / _delta[0]);
    j = int((multi_key[1] - _domain.getLowerCorner()[1]) / _delta[1]);
    k = int((multi_key[2] - _domain.getLowerCorner()[2]) / _delta[2]);
  }

};

//
// File I/O
//

//! Write to a file stream.
/*! \relates CellArrayBase */
template <typename RecordType, typename MultiKeyType, typename KeyType>
inline
std::ostream& 
operator<<(std::ostream& out, 
	    const CellArrayBase<RecordType,MultiKeyType,KeyType>& x)
{
  x.put(out);
  return out;
}

END_NAMESPACE_GEOM

#endif
