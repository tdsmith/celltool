// -*- C++ -*-

/*! 
  \file SparseCellArray.h
  \brief A class for a sparse cell array in N-D.
*/

#if !defined(__geom_SparseCellArray_h__)
#define __geom_SparseCellArray_h__

#include "CellArrayBase.h"

#include "../../ads/array/Array.h"

#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SparseCellArray)
#define DEBUG_SparseCellArray
#endif

BEGIN_NAMESPACE_GEOM

//! An index and cell for holding records.
template<typename Cell>
struct IndexAndCell {
  //! The index of the cell.
  int index;
  //! The cell containing pointers to records.
  Cell cell;
};


//! Less than comparison for indices.
template<typename Cell>
inline
bool
operator<(const IndexAndCell<Cell>& a, const IndexAndCell<Cell>& b) {
  return a.index < b.index;
}


//! A vector of sparse cells.
template<typename Cell>
class SparseCellVector :
  public std::vector< IndexAndCell<Cell> > {
private:
      
  //
  // Private types.
  //

  typedef std::vector<IndexAndCell<Cell> > Base;

  // Assignment operator not implemented.
  SparseCellVector& 
  operator=(const SparseCellVector&);

public:

  //
  // Public types.
  //

  //! A pointer to a record.
  typedef typename Base::value_type value_type;
  //! An iterator on the value type.
  typedef typename Base::iterator iterator;
  //! A const iterator on the value type.
  typedef typename Base::const_iterator const_iterator;
  //! The size type.
  typedef typename Base::size_type size_type;

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Default constructor.
  SparseCellVector() : 
    Base()
  {}

  //! Construct and reserve memory for n elements.
  explicit 
  SparseCellVector(const size_type n) : 
    Base(n)
  {}

  //! Copy constructor.
  SparseCellVector(const SparseCellVector& other) : 
    Base(other)
  {}

  //! Construct from a range.
  template<typename InputIterator>
  SparseCellVector(InputIterator first, InputIterator last) : 
    Base(first, last)
  {}

  //! Destructor.
  ~SparseCellVector()
  {}
  
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from std::vector.
  */
  // @{

  //! Return the beginning of the range.
  using Base::begin;

  //! Return the end of the range.
  using Base::end;

  // @}  
  //--------------------------------------------------------------------------
  /*! \name Manipulators.
    Functionality inherited from std::vector.
  */
  // @{

  //! Return the beginning of the range.
  //using Base::begin;

  //! Return the end of the range.
  //using Base::end;

  //! Insert the value before the iterator.
  /*!
    \return Return an iterator to the inserted element.
  */
  using Base::insert;

  // @}  
  //--------------------------------------------------------------------------
  //! \name Searching.
  //@{

  // Return a const iterator to the first index and cell with index >= i.
  const_iterator 
  lower_bound(const int i) const {
    IndexAndCell<Cell> val;
    val.index = i;
    return std::lower_bound(begin(), end(), val);
  }

  // Return an iterator to the first index and cell with index >= i.
  iterator 
  lower_bound(const int i) {
    IndexAndCell<Cell> val;
    val.index = i;
    return std::lower_bound(begin(), end(), val);
  }

  // Return an iterator to cell i.
  Cell&
  find(const int i) {
    IndexAndCell<Cell> val;
    val.index = i;
    iterator iter = std::lower_bound(begin(), end(), val);

    // If the cell does not yet exist, make it.
    if (iter == end() || iter->index != i) {
      iter = insert(iter, val);
    }

    return iter->cell;
  }

  //@}
};
    



//! A sparse cell array in N-D.
/*!
  A sparse cell array in N-D.
  The array is sparse in the last dimension.  Cell access is accomplished
  with array indexing in the N-1 directions and a binary search
  of a sorted vector in the final direction.
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class SparseCellArray :
  public CellArrayBase<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> {
private:

  //
  // Private types.
  //

  typedef CellArrayBase<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> Base;

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

protected:

  //
  // Protected types.
  //

  //! A multi-index.
  typedef typename Base::MultiIndex MultiIndex;
  //! The cell type.
  typedef typename Base::Cell Cell;

  //! (N-1)-D array of 1-D sparce cell vectors.
  typedef ads::Array<N-1, SparseCellVector<Cell> > VectorArray;

private:

  // 
  // Member data
  //

  //! The array of vectors.
  VectorArray _vectorArray;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  SparseCellArray();

  //! Copy constructor not implemented
  SparseCellArray(const SparseCellArray&);

  //! Assignment operator not implemented
  SparseCellArray& 
  operator=(const SparseCellArray&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  // @{

  //! Construct from the size of a cell and a Cartesian domain.
  /*!
    Construct a cell grid given the grid size and the Cartesian domain that
    the grid spans.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain spanned by the records.
  */
  SparseCellArray(const Point& delta,
		  const SemiOpenInterval& domain) :
    Base(delta, domain),
    _vectorArray() {
    build();
  }

  //! Construct from the domain and a range of records.
  /*!
    Construct a cell grid given the array size, the Cartesian domain
    and a range of records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain that contains the records.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template<class InputIterator>
  SparseCellArray(const Point& delta, const SemiOpenInterval& domain, 
		  InputIterator first, InputIterator last) :
    Base(delta, domain),
    _vectorArray() {
    build();
    // Insert the grid elements in the range.
    insert(first, last);
  }

  //! Construct from a range of records.
  /*!
    Construct a cell grid given the array size and a range of records.
    Compute an appropriate domain.

    \param delta the suggested size of a cell.
    \param first points to the begining of the range of records.
    \param last points to the end of the semi-open range.
  */
  template<class ForwardIterator>
  SparseCellArray(const Point& delta,
		  ForwardIterator first, ForwardIterator last) :
    Base(delta, first, last),
    _vectorArray() {
    build();
    // Insert the grid elements in the range.
    insert(first, last);
  }

  //! Destructor.
  ~SparseCellArray()
  {}

  // @}
  //--------------------------------------------------------------------------
  /*! \name Accesors.
    Functionality inherited from CellArrayBase.
  */
  // @{

  //! Return the number of records.
  using Base::getSize;

  //! Return true if the grid is empty.
  using Base::isEmpty;

  //! Return the domain spanned by the grid.
  using Base::getDomain;

  //! Return the number of cells in each dimension.
  using Base::getExtents;

  // @}
  //--------------------------------------------------------------------------
  //! \name Insert records.
  // @{

  //! Insert a single record.
  void 
  insert(const Record record)
  {
    Cell& b = (*this)(getMultiKey(record));
    b.push_back(record);
    incrementSize();
  }

  //! Insert a range of records.
  /*!
    The input iterators are to a container of records.
  */
  template<class InputIterator>
  void 
  insert(InputIterator first, InputIterator last) {
    while (first != last) {
      insert(first);
      ++first;
    }
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  // @{
  
  //! Return the memory usage.
  SizeType 
  getMemoryUsage() const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Get the grid points in the window.  Return the # of grid pts inside.
  template<class OutputIterator>
  SizeType 
  computeWindowQuery(OutputIterator iter, const BBox& window) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the records.
  void 
  put(std::ostream& out) const;

  //@}

protected:

  //! Increment the number of records.
  using Base::incrementSize;

  //! Get the multi-key of a record.
  using Base::getMultiKey;

  //! Convert the multikey to a cell array index.
  using Base::convertMultiKeyToIndices;

private:

  //  
  // Accesors: Cell Indexing
  //

  //! Return a reference to the cell that would hold the point.
  /*!
    Indexing by location.  Return a reference to a cell.
    The multi-key must be in the domain of the cell array.
  */
  Cell&
  operator()(const MultiKey& multiKey);

  void
  build();
};

//
// File I/O
//

//! Write to a file stream.
/*! \relates SparseCellArray */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor>
inline
std::ostream& 
operator<<
  (std::ostream& out, 
   const SparseCellArray<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_SparseCellArray_ipp__
#include "SparseCellArray.ipp"
#undef __geom_SparseCellArray_ipp__

#endif
