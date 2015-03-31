// -*- C++ -*-

/*! 
  \file CellBinarySearch.h
  \brief A class for a cell array coupled with a binary search.
*/

#if !defined(__geom_CellBinarySearch_h__)
#define __geom_CellBinarySearch_h__

#include "CellSearch.h"

#include "../../ads/functor/Dereference.h"

BEGIN_NAMESPACE_GEOM

//! Binary search of records in the final coordinate.
template<int N,
	 typename _Record,
	 typename _Key,
	 typename _MultiKeyAccessor>
class BinarySearch :
  public Search<N,_Record,_MultiKeyAccessor> {
private:
      
  typedef Search<N,_Record,_MultiKeyAccessor> Base;

  // CONTINUE: With the Intel compiler, private members are not accessible
  // in nested classes.
#ifdef __INTEL_COMPILER
public:
#else
private:
#endif

  typedef _MultiKeyAccessor MultiKeyAccessor;

public:
      
  //
  // Public types.
  //

  //! The record type.
  typedef _Record Record;
  //! The key type.
  typedef _Key Key;

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
  // Functors.
  //

  //! Less than comparison in the final coordinate for a record and a key.
  class LessThanCompareRecordAndKey :
    public std::binary_function<Record, Key, bool> {
  private:

    MultiKeyAccessor _f;

  public:

    //! Less than comparison in the final coordinate.
    bool 
    operator()(const Record record, const Key key) const {
      return _f(record)[N-1] < key;
    }
  };

  // Compare a record and a key.
  LessThanCompareRecordAndKey _compare;

  //
  // Not implemented.
  //

  // Assignment operator not implemented.
  BinarySearch& 
  operator=(const BinarySearch&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Default constructor.
  BinarySearch() :
    Base(),
    _compare()
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  BinarySearch(const size_type size) :
    Base(size),
    _compare()
  {}

  //! Copy constructor.
  BinarySearch(const BinarySearch& other) :
    Base(other),
    _compare()
  {}

  //! Construct from a range.
  template <typename InputIterator>
  BinarySearch(InputIterator first, InputIterator last) :
    Base(first, last),
    _compare()
  {}

  //! Destructor.
  ~BinarySearch()
  {}

  // @}  
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from std::vector.
  */
  // @{

  //! Return the number of records.
  using Base::size;

  //! Return the beginning of the range of record pointers.
  using Base::begin;

  //! Return the end of the range of record pointers.
  using Base::end;

  // @}
  //--------------------------------------------------------------------------
  /*! \name Manipulators.
    Functionality inherited from std::vector.
  */
  // @{

  // Return the beginning of the range of record pointers.
  //using Base::begin;

  // Return the end of the range of record pointers.
  //using Base::end;

  // @}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  // @{
  
  //! Return the memory usage.
  size_type 
  getMemoryUsage() const { 
    return (sizeof(BinarySearch) + size() * sizeof(value_type));
  }
      
  // @}
  //--------------------------------------------------------------------------
  //! \name Searching.
  // @{
   
  //! Do a binary search to find the record.
  iterator
  search(const Key x) {
    return std::lower_bound(begin(), end(), x, _compare);
  }
      
  //! Do a binary search to find the record.
  const_iterator
  search(const Key x) const {
    return std::lower_bound(begin(), end(), x, _compare);
  }

  // @}      
};
    









//! A cell array in combined with a binary search in the final coordinate.
/*!
  The cell array spans N-1 coordinates.  Record access 
  is accomplished with array indexing in these coordinates and a 
  binary search of a sorted vector in the final coordinate.
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class CellBinarySearch :
  public CellSearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		    BinarySearch<N,_Record,_Key,_MultiKeyAccessor> > {
private:

  //
  // Private types.
  //

  typedef CellSearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		    BinarySearch<N,_Record,_Key,_MultiKeyAccessor> > Base;

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
  CellBinarySearch();

  //! Copy constructor not implemented
  CellBinarySearch(const CellBinarySearch&);

  //! Assignment operator not implemented
  CellBinarySearch& 
  operator=(const CellBinarySearch&);

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
  CellBinarySearch(const Point& delta, const SemiOpenInterval& domain) :
    Base(delta, domain)
  {}

  //! Construct from the Cartesian domain and a range of records.
  /*!
    Construct given the cell size, the Cartesian domain that
    contains the records and a range of records.

    \param delta is the suggested size of a cell.
    \param domain is the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template<typename InputIterator>
  CellBinarySearch(const Point& delta, const SemiOpenInterval& domain, 
		   InputIterator first, InputIterator last) :
    Base(delta, domain, first, last)
  {}

  //! Construct from a range of records.
  /*!
    Construct given the cell size a range of records. Compute an appropriate
    domain.

    \param delta is the suggested size of a cell.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template<typename ForwardIterator>
  CellBinarySearch(const Point& delta,
		   ForwardIterator first, ForwardIterator last) :
    Base(delta, first, last)
  {}

  //! Trivial Destructor.
  ~CellBinarySearch()
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


//
// File I/O
//


//! Write to a file stream.
/*! \relates CellBinarySearch */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor>
inline
std::ostream& 
operator<<
  (std::ostream& out, 
   const CellBinarySearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_CellBinarySearch_ipp__
#include "CellBinarySearch.ipp"
#undef __geom_CellBinarySearch_ipp__

#endif
