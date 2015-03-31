// -*- C++ -*-

/*! 
  \file CellArrayBase.h
  \brief A base class for a cell array in N-D.
*/

// CONTINUE: Make sure I do not instantiate any multi-keys or multi-indices
// in any files.

#if !defined(__geom_CellArrayBase_h__)
#define __geom_CellArrayBase_h__

#include "ORQ.h"

#include "../../ads/functor/Dereference.h"
#include "../../ads/algorithm/sort.h"

#include <vector>
#include <cmath>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_CellArrayBase)
#define DEBUG_CellArrayBase
#endif

BEGIN_NAMESPACE_GEOM

//! Base class for a cell array in N-D.
/*!
  A base class for a cell arrays in N-D.

  This class implements the common functionality of dense and sparse
  cell arrays.  It does not store the records.  Instead
  it has info on the number and size of the cells.  
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class CellArrayBase :
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

  //! A multi-index.
  typedef ads::FixedArray<N,int> MultiIndex;
  //! The cell type.
  typedef std::vector<Record> Cell;

private:

  //
  // Member data.
  //

  //! The domain spanned by the grid.
  SemiOpenInterval _domain;

  //! The number of cells in each dimension.
  ads::FixedArray<N,SizeType> _extents;

  //! The inverse cell sizes.
  Point _inverseCellSizes;

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
    Construct a cell array given the grid size and the Cartesian domain 
    that contains the records.

    \param delta the suggested size of a cell.
    \param domain the Cartesian domain spanned by the records.
  */
  CellArrayBase(const Point& delta, const SemiOpenInterval& domain) :
    Base(),
    _domain(domain),
    _extents(),
    _inverseCellSizes() {
    computeExtentsAndSizes(delta);
  }

  //! Construct from the cell size and a range of records.
  /*!
    \param delta the suggested size of a cell.
    \param first The first record.
    \param last The last record.

    \pre There must be a non-zero number of records.
    \note This function assumes that the records are iterators.
  */
  template<class InputIterator>
  CellArrayBase(const Point& delta, InputIterator first, InputIterator last) :
    Base(),
    _domain(),
    _extents(),
    _inverseCellSizes() {
    // Determine a domain to contain the records.
    computeDomain(first, last, &_domain);
    computeExtentsAndSizes(delta);
  }

  //! Construct from the number of cells and a range of records.
  /*!
    \param numberOfCells the suggested number of cells.
    \param first The first record.
    \param last The last record.

    \pre There must be a non-zero number of records.
    \note This function assumes that the records are iterators.
  */
  template<class InputIterator>
  CellArrayBase(const std::size_t numberOfCells, InputIterator first,
		InputIterator last) :
    Base(),
    _domain(),
    _extents(),
    _inverseCellSizes() {
    // Determine a domain to contain the records.
    computeDomain(first, last, &_domain);
    computeExtentsAndSizes(numberOfCells);
  }

  //! Destructor.
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
  using Base::getSize;

  //! Return true if the grid is empty.
  using Base::isEmpty;

  //
  // New.
  //

  //! Return the domain spanned by the grid.
  const SemiOpenInterval& 
  getDomain() const { 
    return _domain; 
  }

  //! Return the number of cells in each dimension.
  const ads::FixedArray<N,SizeType>&
  getExtents() const {
    return _extents;
  }

  //! Get the inverse cell sizes.
  const Point&
  getInverseCellSizes() const {
    return _inverseCellSizes;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the data structure.
  void 
  put(std::ostream& out) const {
    out << _domain << '\n'
	<< _extents << '\n'
	<< _inverseCellSizes << '\n';
  }

  // @}

protected:

  //! Increment the number of records.
  using Base::incrementSize;

  //! Set the number of records.
  using Base::setSize;

  //! Get the multi-key of a record.
  using Base::getMultiKey;

  //! Determine an appropriate domain to contain the records.
  using Base::computeDomain;

  //! Convert the multikey to a cell array index.
  template<typename AnyMultiKeyType>
  void 
  convertMultiKeyToIndices(const AnyMultiKeyType& multiKey,
			   MultiIndex* multiIndex) const {
    for (int n = 0; n != N; ++n) {
      (*multiIndex)[n] = int((multiKey[n] - _domain.getLowerCorner()[n]) * 
			     _inverseCellSizes[n]);
    }
  }

private:

  //! Compute the array extents and the sizes for the cells.
  void
  computeExtentsAndSizes(const Point& suggestedCellSize) {
    typedef typename Point::value_type Number;

    for (int n = 0; n != N; ++n) {
      assert(suggestedCellSize[n] > 0);
      Number cartesianExtent = _domain.getUpperCorner()[n] - 
	_domain.getLowerCorner()[n];
      // Compute the cell array extents. Add 1 to round up.
      _extents[n] = int(cartesianExtent / suggestedCellSize[n]) + 1;
      // From the domain and the cell array extents, compute the cell size.
      if (cartesianExtent == 0 || _extents[n] == 1) {
	// The cell covers the entire domain.
	_inverseCellSizes[n] = 0;
      }
      else {
	_inverseCellSizes[n] = _extents[n] / cartesianExtent;
      }
    }
  }
  
  //! Compute the array extents and the sizes for the cells.
  void
  computeExtentsAndSizes(const std::size_t numberOfCells) {
    typedef typename Point::value_type Number;

    assert(numberOfCells > 0);

    // Work from the the least to greatest Cartesian extent to compute the 
    // grid extents.
    ads::FixedArray<N, int> order;
    Point ext = _domain.getUpperCorner() - _domain.getLowerCorner();
    ads::computeOrder(ext.begin(), ext.end(), order.begin());
    for (int i = 0; i != N; ++i) {
      // Normalize the domain to numberOfCells content.
      Number content = ads::computeProduct(ext);
      assert(content != 0);
      const Number factor = std::pow(numberOfCells / content, 1.0 / (N - i));
      for (int j = i; j != N; ++j) {
	ext[order[j]] *= factor;
      }
      // The current index;
      const int n = order[i];
      // Add 0.5 and truncate to round to the nearest integer.
      ext[n] = _extents[n] = std::max(1, int(ext[n] + 0.5));
    }

    // From the domain and the cell array extents, compute the cell size.
    for (int n = 0; n != N; ++n) {
      const Number d = _domain.getUpperCorner()[n] - 
	_domain.getLowerCorner()[n];
      if (d == 0 || _extents[n] == 1) {
	// The cell covers the entire domain.
	_inverseCellSizes[n] = 0;
      }
      else {
	_inverseCellSizes[n] = _extents[n] / d;
      }
    }
  }
  
};


//
// File I/O
//


//! Write to a file stream.
/*! \relates CellArrayBase */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor>
inline
std::ostream& 
operator<<(std::ostream& out, 
	   const CellArrayBase<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>& x){
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#endif
