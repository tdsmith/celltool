// -*- C++ -*-

/*! 
  \file StructuredGrid.h
  \brief Implements a class for a structured grid.
*/

#if !defined(__geom_StructuredGrid_h__)
#define __geom_StructuredGrid_h__

#include "../../defs.h"
#include "../../kernel/BBox.h"
#include "../../kernel/content.h"

#include "../../../ads/array/Array.h"

#include <iosfwd>

#include <cassert>

#if defined(DEBUG_geom) && !defined(DEBUG_StructuredGrid)
#define DEBUG_StructuredGrid
#endif

BEGIN_NAMESPACE_GEOM

//! Class for a structured grid.
/*!
  \param N is the space dimension.
  \param M is the grid dimension.
  \param T is the number type.  By default it is double.

  A structured grid is an N-dimensional array of M-dimensional points.
*/
template<int N, int M, typename T = double>
class StructuredGrid {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;
  //! The point type.
  typedef ads::FixedArray<M,Number> Point;
  //! The array of points.
  typedef ads::Array<N,Point> Array;
  //! A bounding box.
  typedef BBox<N,Number> BBox;

  //
  // Types that make this a container.
  //

  //! The element type of the grid is a point.
  typedef typename Array::value_type Value;

  //! A pointer to a grid element.
  typedef typename Array::pointer Pointer;
  //! A pointer to a constant grid element.
  typedef typename Array::const_pointer ConstPointer;

  //! An iterator on points.
  typedef typename Array::iterator Iterator;
  //! A const iterator on points.
  typedef typename Array::const_iterator ConstIterator;

  //! A reference to a point.
  typedef typename Array::reference Reference;
  //! A const reference to a point.
  typedef typename Array::const_reference ConstReference;

  //! The size type.
  typedef typename Array::size_type SizeType;
  //! Pointer difference type.
  typedef typename Array::difference_type DifferenceType;

  //! A multi-index.  Index in N dimensions.
  typedef typename Array::index_type Index;

private:

  //
  // Data
  //

  //! The array of points.
  Array _grid;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Empty grid.
  StructuredGrid() :
    _grid()
  {}

  //! Construct from an array of points.
  /*!
    \param grid is the array of points.
  */
  template<bool A>
  StructuredGrid(const ads::Array<N,Point,A>& grid) :
    _grid(grid)
  {}

  //! Construct from the grid extents.
  /*!
    \param extents are the grid extents.
   */
  StructuredGrid(const Index& extents) :
    _grid(extents)
  {}

  //! Copy constructor.
  StructuredGrid(const StructuredGrid& other) :
    _grid(other._grid)
  {}

  //! Assignment operator.
  StructuredGrid&
  operator=(const StructuredGrid& other) {
    if (this != &other) {
      _grid = other._grid;
    }
    return *this;
  }

  //! Destructor.  Free allocated memory.
  ~StructuredGrid()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical Functions
  //! @{

  //! Make a bounding box containing the points in the grid.
  BBox
  computeBBox() const {
    BBox bbox;
    bbox.bound(_grid.begin(), _grid.end()); 
    return bbox;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors
  //! @{

  //! Return the dimension of the space.
  static
  int
  getSpaceDimension() {
    return N;
  }

  //! Return the dimension of the grid.
  static
  int
  getGridDimension() {
    return M;
  }

  //! Return the grid extents.
  const Index&
  getExtents() const {
    return _grid.extents();
  }

  //! Return the number of grid points.
  SizeType
  getSize() const {
    return _grid.size();
  }

  //! Return a const iterator to the beginning of the points.
  ConstIterator
  getBeginning() const { 
    return _grid.begin();
  }

  //! Return a const iterator to the end of the points.
  ConstIterator
  getEnd() const { 
    return _grid.end();
  }

  //! Return a const reference to the grid array.
  const Array&
  getGrid() const {
    return _grid;
  }

  //! Return a const reference to the given point.
  ConstReference
  operator()(const Index& mi) const {
    return _grid(mi);
  }

  //! Return a const reference to the given point.
  ConstReference
  operator()(const int i, const int j) const {
    return _grid(i, j);
  }

  //! Return a const reference to the given point.
  ConstReference
  operator()(const int i, const int j, const int k) const {
    return _grid(i, j, k);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //! @{

  //! Return an iterator to the beginning of the points.
  Iterator
  getBeginning() { 
    return _grid.begin();
  }

  //! Return an iterator to the end of the points.
  Iterator
  getEnd() { 
    return _grid.end();
  }

  // CONTINUE
#if 0
  //! Return a reference to the grid array.
  Array&
  grid() {
    return _grid;
  }
#endif

  //! Return a reference to the given point.
  Reference
  operator()(const Index& mi) {
    return _grid(mi);
  }

  //! Return a reference to the given point.
  ConstReference
  operator()(const int i, const int j) {
    return _grid(i, j);
  }

  //! Return a reference to the given point.
  ConstReference
  operator()(const int i, const int j, const int k) {
    return _grid(i, j, k);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Equality
  //! @{

  //! Return true if the grid is equal to the argument.
  bool
  isEqualTo(const StructuredGrid& x) const {
    return _grid = x._grid;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name File I/O
  //! @{

  //! Write in ascii format.
  void
  put(std::ostream& out) const {
    out << _grid.extents() << '\n';
    _grid.write_elements_ascii(out);
  }

  //! Read in ascii format.
  void
  get(std::istream& in) {
    Index extents;
    in >> extents;
    _grid.resize(extents);
    _grid.read_elements_ascii(in);
  }

  //! Write as an indexed simplex set.
  void
  writeIndexedSimplexSet(std::ostream& out) const;

  //! @}
};


//
// File I/O
//


//! Write a grid in ascii format.
/*! \relates StructuredGrid */
template<int N, int M, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const StructuredGrid<N,M,T>& x) {
  x.put(out);
  return out;
}


//! Read a grid in ascii format.
/*! \relates StructuredGrid */
template<int N, int M, typename T>
inline
std::istream& 
operator>>(std::istream& in, StructuredGrid<N,M,T>& x) {
  x.get(in);
  return in;
}


//
// Equality tests
//


//! Return true if the grids are equal.
/*! \relates StructuredGrid */
template<int N, int M, typename T>
inline
bool 
operator==(const StructuredGrid<N,M,T>& a, const StructuredGrid<N,M,T>& b) {
  return a.isEqualTo(b);
}


//! Return true if the grids are not equal.
/*! \relates StructuredGrid */
template<int N, int M, typename T>
inline
bool 
operator!=(const StructuredGrid<N,M,T>& a, const StructuredGrid<N,M,T>& b) {
  return !(a == b);
}


END_NAMESPACE_GEOM

#define __geom_StructuredGrid_ipp__
#include "StructuredGrid.ipp"
#undef __geom_StructuredGrid_ipp__

#endif
