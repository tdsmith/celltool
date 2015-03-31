// -*- C++ -*-

/*! 
  \file CellForwardSearchKey.h
  \brief A class for a sparse cell array in 3-D.
*/

#if !defined(__geom_CellForwardSearchKey_h__)
#define __geom_CellForwardSearchKey_h__

#include "CellSearch.h"

#include "../../ads/functor/Dereference.h"

BEGIN_NAMESPACE_GEOM


//! Data structure for performing forward searching on the keys of records.
template<int N,
	 typename _Record,
	 typename _Key,
	 typename _MultiKeyAccessor>
class ForwardSearchKey :
  public Search<N,_Record,_MultiKeyAccessor> {
private:
      
  typedef Search<N,_Record,_MultiKeyAccessor> Base;
  typedef _MultiKeyAccessor MultiKeyAccessor;

public:
      
  //
  // Public types.
  //

  //! The record type.
  typedef _Record Record;
  //! The key type.
  typedef _Key Key;

  //! The key container.
  typedef std::vector<_Key> KeyContainer;
  //! An iterator on const keys in the key container.
  typedef typename KeyContainer::const_iterator KeyConstIterator;

  //! The record type.
  typedef typename Base::value_type value_type;
  //! A pointer to the value type.
  typedef typename Base::pointer pointer;
  //! A const pointer to the value type.
  typedef typename Base::const_pointer const_pointer;
  //! An iterator to the value type.
  typedef typename Base::iterator iterator;
  //! A const iterator to the value type.
  typedef typename Base::const_iterator const_iterator;
  //! A reference to the value type.
  typedef typename Base::reference reference;
  //! A const reference to the value type.
  typedef typename Base::const_reference const_reference;
  //! The size type.
  typedef typename Base::size_type size_type;

private:

  //
  // Member data
  //

  //! The last coordinate of the record's multi-key
  KeyContainer _lastKeys;

  //! Index in the vector of record pointers.
  mutable int _current;

  //
  // Not implemented.
  //

  //! Assignment operator not implemented.
  ForwardSearchKey& 
  operator=(const ForwardSearchKey&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Default constructor.
  ForwardSearchKey() :
    Base(),
    _lastKeys(),
    _current(0)
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  ForwardSearchKey(const size_type size) :
    Base(size),
    _lastKeys(),
    _current(0)
  {}

  //! Copy constructor.
  ForwardSearchKey(const ForwardSearchKey& other) :
    Base(other),
    _lastKeys(other._lastKeys),
    _current(other._current)
  {}

  //! Construct from a range.
  template<typename InputIterator>
  ForwardSearchKey(InputIterator first, InputIterator last) :
    Base(first, last),
    _lastKeys(),
    _current(0)
  {}

  //! Destructor.
  ~ForwardSearchKey()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the number of records.
  using Base::size;

  //! Return the beginning of the range of records.
  using Base::begin;

  //! Return the end of the range of records.
  using Base::end;

  const KeyContainer& 
  getLastKeys() const { 
    return _lastKeys; 
  }

  //@}
  //--------------------------------------------------------------------------
  /*! \name Manipulators.
    Functionality inherited from std::vector.
  */
  // @{

  // Return the beginning of the range of records.
  //using Base::begin;

  // Return the end of the range of records.
  //using Base::end;

  // @}
  //--------------------------------------------------------------------------
  //! \name Sorting and searching.
  //@{

  //! Sort the record pointers in the last coordinate.
  void 
  sort() {
    // Sort the records.
    Base::sort();
    // Copy the last keys.
    _lastKeys.clear();
    _lastKeys.reserve(size());
    MultiKeyAccessor getMultiKey;
    for (iterator i = begin(); i != end(); ++i) {
      _lastKeys.push_back(getMultiKey(*i)[N-1]);
    }
  }

  //! Initialize for a set of queries.
  void 
  initialize() const {
    _current = 0;
  }

  //! Do a forward search to find the index of the record.
  int
  search(const Key x) const {
    KeyConstIterator i = _lastKeys.begin() + _current;
    KeyConstIterator iEnd = _lastKeys.end();
    while (i != iEnd && *i < x) {
      ++i;
    }
    _current = i - _lastKeys.begin();
    return _current;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{

  //! Return the memory usage.
  size_type 
  getMemoryUsage() const {
    return (sizeof(ForwardSearchKey)
	    + size() * sizeof(value_type)
	    + _lastKeys.size() * sizeof(Key));
  }

  //@}
};
    







//! A cell array in combined with a forward search on keys in the final coordinate.
/*!
  The cell array spans N-1 coordinates.  Record access 
  is accomplished with array indexing in these coordinates and a 
  forward search of a sorted vector in the final coordinate.
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class CellForwardSearchKey :
  public CellSearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		    ForwardSearchKey<N,_Record,_Key,_MultiKeyAccessor> > {
private:

  //
  // Private types.
  //

  typedef CellSearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		     ForwardSearchKey<N,_Record,_Key,_MultiKeyAccessor> > Base;

protected:

  //
  // Protected types.
  //

  //! The cell type.
  typedef typename Base::Cell Cell;
  //! A multi-index into the cell array.
  typedef typename Base::Index Index;

public:

  //
  // Public types.
  //

  //! The record type.
  typedef _Record Record;
  //! The multi-key type.
  typedef _MultiKey MultiKey;
  //! The key type.
  typedef _Key Key;

  //! The size type.
  typedef typename Base::SizeType SizeType;
  //! A Cartesian point.
  typedef typename Base::Point Point;
  //! Bounding box.
  typedef typename Base::BBox BBox;
  //! Semi-open interval.
  typedef typename Base::SemiOpenInterval SemiOpenInterval;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  CellForwardSearchKey();

  //! Copy constructor not implemented
  CellForwardSearchKey(const CellForwardSearchKey&);

  //! Assignment operator not implemented
  CellForwardSearchKey& 
  operator=(const CellForwardSearchKey&);

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
  CellForwardSearchKey(const Point& delta, const SemiOpenInterval& domain) :
    Base(delta, domain)
  {}

  //! Construct from a domain and a range of records.
  /*!
    Construct given the cell size, the Cartesian domain that
    contains the records and a range of records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template<typename InputIterator>
  CellForwardSearchKey(const Point& delta, const SemiOpenInterval& domain, 
		       InputIterator first, InputIterator last) :
    Base(delta, domain, first, last)
  {}

  //! Construct from a range of records.
  /*!
    Construct given the cell size a range of records. Compute an appropriate
    domain.

    \param delta the suggested size of a cell.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template<typename ForwardIterator>
  CellForwardSearchKey(const Point& delta,
		       ForwardIterator first, ForwardIterator last) :
    Base(delta, first, last)
  {}

  //! Destructor.
  ~CellForwardSearchKey()
  {}

  //@}
  //--------------------------------------------------------------------------
  /*! \name Accesors.
    Functionality inherited from ORQ.
   */
  // @{

  //! Return the number of records.
  using Base::getSize;

  //! Return true if the grid is empty.
  using Base::isEmpty;

  // @}
  //--------------------------------------------------------------------------
  /*! \name Insert records.
    Functionality inherited from CellSearch.
  */
  //@{

  //! Insert a single record or a range of records.
  using Base::insert;
  
  //@}
  //--------------------------------------------------------------------------
  /*! \name Memory usage.
    Functionality inherited from CellSearch.
  */
  //@{
  
  //! Return the memory usage.
  using Base::getMemoryUsage;

  //@}
  //--------------------------------------------------------------------------
  //! \name Window queries.
  //@{

  //! Get the records in the window.  Return the # of records inside.
  template<typename OutputIterator>
  SizeType
  computeWindowQuery(OutputIterator iter, const BBox& window) const;

  //@}
  //--------------------------------------------------------------------------
  /*! \name Sorting.
    Functionality inherited from CellSearch.
  */
  //@{

  //! Sort the records.
  using Base::sort;

  //! Initialize for a set of queries.
  using Base::initialize;

  //@}
  //--------------------------------------------------------------------------
  /*! \name File I/O
    Functionality inherited from CellSearch.
  */
  //@{

  //! Print the data structure.
  using Base::put;

  //@}

private:

  //! Get the multi-key of a record.
  using Base::getMultiKey;

  //! Convert the multikey to a cell array index.
  using Base::convertMultiKeyToIndices;

  //! Return the extents of the cell array.
  using Base::getCellArrayExtents;

  //! Return the specified cell.
  using Base::getCell;
};


//! Write to a file stream.
/*! \relates CellForwardSearchKey */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor>
inline
std::ostream& 
operator<<
  (std::ostream& out, 
   const CellForwardSearchKey<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_CellForwardSearchKey_ipp__
#include "CellForwardSearchKey.ipp"
#undef __geom_CellForwardSearchKey_ipp__

#endif
