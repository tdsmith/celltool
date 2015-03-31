// -*- C++ -*-

/*! 
  \file CellSearch.h
  \brief A base class for a cell array with searching in N-D.
*/

#if !defined(__geom_CellSearch_h__)
#define __geom_CellSearch_h__

#include "ORQ.h"

#include "../../ads/array/Array.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_CellSearch)
#define DEBUG_CellSearch
#endif

BEGIN_NAMESPACE_GEOM

//! Base class for a search structure in the final coordinate.
template<int N,
	 typename _Record,
	 typename _MultiKeyAccessor>
class Search :
  public std::vector<_Record> {
private:

  typedef std::vector<_Record> Base;

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

  //! The record type.
  typedef typename Base::value_type value_type;
  //! A pointer to value_type.
  typedef typename Base::pointer pointer;
  //! A pointer to const value_type.
  typedef typename Base::const_pointer const_pointer;
  //! An iterator on value_type.
  typedef typename Base::iterator iterator;
  //! An iterator on const value_type.
  typedef typename Base::const_iterator const_iterator;
  //! A reference to value_type.
  typedef typename Base::reference reference;
  //! A reference to const value_type.
  typedef typename Base::const_reference const_reference;
  //! The size type.
  typedef typename Base::size_type size_type;

private:

  //
  // Functors.
  //

  //! Less than comparison in the final coordinate.
  class LessThanCompare :
    public std::binary_function<Record, Record, bool> {
  private:

    MultiKeyAccessor _f;

  public:
    
    //! Constructor.
    LessThanCompare() :
      _f()
    {}

    //! Destructor.
    ~LessThanCompare()
    {}

    //! Less than comparison in the final coordinate.
    bool 
    operator()(const Record x, const Record y) const {
      return _f(x)[N-1] < _f(y)[N-1];
    }
  };

  //
  // Member data.
  //


  //
  // Not implemented.
  //

  // Assignment operator not implemented.
  Search& 
  operator=(const Search&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Default constructor.
  Search() :
    Base()
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  Search(const size_type size) :
    Base() {
    reserve(size);
  }

  //! Copy constructor.
  Search(const Search& other) :
    Base(other)
  {}

  //! Construct from a range.
  template<typename InputIterator>
  Search(InputIterator first, InputIterator last) :
    Base(first, last)
  {}

  //! Destructor.
  ~Search()
  {}
  
  // @}
  //--------------------------------------------------------------------------
  /*! \name Accesors.
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
    return (sizeof(Search) + size() * sizeof(value_type));
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Sorting and searching.
  // @{

  //! Sort the records.
  void 
  sort() {
    // CONTINUE: This causes warnings with the PGI compiler.
    LessThanCompare compare;
    std::sort(begin(), end(), compare);
  }

  //! Initialize for a set of queries.
  void 
  initialize()
  {}

  //@}
};









//! Base class for a cell array combined with a search structure.
/*!
  A base class for a cell array in N-D.
  This class implements the common functionality of data structures
  which have cell arrays in the first N-1 coordinates and have a search
  data structure in the final coordinate.  It does not store pointers to 
  the records.  Instead it has info on the number and size of the cells.  
*/
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _Cell>
class CellSearch :
  public ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> {
private:

  //
  // Private types.
  //

  typedef ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> Base;

public:

  //
  // Public types.
  //

  //! A pointer to the record type.
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

protected:

  //
  // Protected types.
  //

  //! The cell type.
  typedef _Cell Cell;
  //! The cell array type.
  typedef ads::Array<N-1,Cell> DenseArray;
  //! A multi-index into the cell array.
  typedef typename DenseArray::index_type Index;
  //! The (N-1)-dimensional domain spanned by the cell array.
  typedef geom::SemiOpenInterval<N-1, Key> Domain;

private:

  //
  // Member data.
  //

  //! The semi-open domain spanned by the grid.
  Domain _domain;

  //! The (N-1)-D array of cells that span the first N-1 coordinates.
  DenseArray _cellArray;

  //! The inverse cell sizes.
  ads::FixedArray<N-1, Key> _inverseCellSizes;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  CellSearch();

  //! Copy constructor not implemented
  CellSearch(const CellSearch&);

  //! Assignment operator not implemented
  CellSearch& 
  operator=(const CellSearch&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct given the cell size and the Cartesian domain 
    that contains the records.

    \param delta is the suggested size of a cell.  The final coordinate 
    is ignored.
    \param domain is the Cartesian domain spanned by the records.
    The final coordinate is ignored.
  */
  CellSearch(const Point& delta, const SemiOpenInterval& domain);

  //! Construct from the cell size, the Cartision domain and a range of records.
  /*!
    Construct given the cell size, the Cartesian domain and a range of records.

    \param delta is the suggested size of a cell.
    \param domain is the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the range.
  */
  template<typename InputIterator>
  CellSearch(const Point& delta, const SemiOpenInterval& domain, 
	     InputIterator first, InputIterator last);

  //! Construct from the cell size and a range of records.
  /*!
    Construct given the cell size and a range of records. An appropriate
    domain will be computed.

    \param delta is the suggested size of a cell.
    \param first points to the begining of the range of records.
    \param last points to the end of the range.
  */
  template<typename ForwardIterator>
  CellSearch(const Point& delta, ForwardIterator first, ForwardIterator last);

  //! Trivial Destructor.
  ~CellSearch()
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
  //! \name Insert records.
  //@{

  //! Insert a single record.
  void 
  insert(const Record record) {
    getCell(getMultiKey(record)).push_back(record);
    incrementSize();
  }

  //! Insert a range of records.
  /*!
    The input iterators are to a container of records.
  */
  template<typename InputIterator>
  void 
  insert(InputIterator first, InputIterator last) {
    while (first != last) {
      insert(first);
      ++first;
    }
  }
  
  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{
  
  //! Return the memory usage.
  SizeType 
  getMemoryUsage() const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Sorting and searching.
  //@{

  //! Sort the records.
  void 
  sort();

  //! Initialize for a set of queries.
  void 
  initialize();

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O
  //@{

  //! Print the data structure.
  void 
  put(std::ostream& out) const;

  //@}

protected:

  //! Increment the number of records.
  using Base::incrementSize;

  //! Get the multi-key of a record.
  using Base::getMultiKey;

  //! Determine an appropriate domain to contain the records.
  template<class InputIterator>
  void
  computeDomain(InputIterator first, InputIterator last) {
    SemiOpenInterval cartesianDomain;
    Base::computeDomain(first, last, &cartesianDomain);
    setDomain(cartesianDomain);
  }

  //! Convert the multikey to a cell array index.
  template<typename AnyMultiKeyType>
  void 
  convertMultiKeyToIndices(const AnyMultiKeyType& multiKey, Index* mi) const {
    for (int n = 0; n != N - 1; ++n) {
      (*mi)[n] = int((multiKey[n] - _domain.getLowerCorner()[n]) *
		     _inverseCellSizes[n]);
    }
  }

  //! Return the extents of the cell array.
  const Index&
  getCellArrayExtents() const {
    return _cellArray.extents();
  }

  //! Return a reference to the specified cell.
  Cell& 
  getCell(const Index& index) {
    return _cellArray(index);
  }

  //! Return a const reference to the specified cell.
  const Cell& 
  getCell(const Index& index) const {
    return _cellArray(index);
  }

  //! Return a const reference to the search structure that would hold the point.
  template<typename AnyMultiKeyType>
  const Cell&
  getCell(const AnyMultiKeyType& multiKey) const {
    Index mi;
    convertMultiKeyToIndices(multiKey, &mi);
    return _cellArray(mi);
  }

  //! Return a reference to the search structure that would hold the point.
  template<typename AnyMultiKeyType>
  Cell& 
  getCell(const AnyMultiKeyType& multiKey) {
    Index mi;
    convertMultiKeyToIndices(multiKey, &mi);
    return _cellArray(mi);
  }

private:

  //! Compute the array extents and the sizes for the cells.
  void
  computeExtentsAndSizes(const Point& suggestedCellSize);

  //! Set the domain. Ignore the last coordinate.
  void
  setDomain(const SemiOpenInterval& domain);
};


//
// File I/O
//


//! Write to a file stream.
/*! \relates CellSearch */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _Cell>
inline
std::ostream& 
operator<<
  (std::ostream& out, 
   const CellSearch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,_Cell>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_CellSearch_ipp__
#include "CellSearch.ipp"
#undef __geom_CellSearch_ipp__

#endif
