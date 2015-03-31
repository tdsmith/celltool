// -*- C++ -*-

/*! 
  \file RegularGrid.h
  \brief A class for a regular grid.
*/

#if !defined(__geom_RegularGrid_h__)
#define __geom_RegularGrid_h__

#include <iosfwd>

#include "../kernel/BBox.h"

#include <limits>

BEGIN_NAMESPACE_GEOM

//! A regular grid in N-D.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.
*/
template<int N, typename T = double>
class RegularGrid {
  //
  // Public types.
  //

public:
      
  //! The number type.
  typedef T Number;
  //! The size type.
  typedef int SizeType;
  //! The multi-index of a multi-array.
  typedef ads::FixedArray<N,SizeType> Index;
  //! The point type.
  typedef ads::FixedArray<N,Number> Point;
  //! A bounding box.
  typedef BBox<N,T> BBox;

  //
  // Data
  //

private:

  //! Extents of the grid.
  Index _extents;

  //! The domain spanned by the grid.
  BBox _domain;

  //! Lengths of the sides of the box.
  Point _length;

  //! Grid spacing.
  Point _delta;

  //! epsilon in index coordinates.
  Number _indexEpsilon;

  //! epsilon in cartesian coordinates.
  Number _cartesianEpsilon;


public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  RegularGrid() :
    _extents(),
    _domain(),
    _length(),
    _delta(),
    _indexEpsilon(),
    _cartesianEpsilon()
  {}

  //! Construct from grid dimensions and a Cartesian domain.
  /*!
    Construct a regular grid given the grid extents and the Cartesian domain 
    that the grid spans.
    
    \param extents the number of grid points in each direction.
    \param domain the Cartesian domain spanned by the grid.
  */
  RegularGrid(const Index& extents, const BBox& domain);

  //! Copy constructor.
  RegularGrid(const RegularGrid& other);

  //! Assignment operator.
  RegularGrid& 
  operator=(const RegularGrid& other);

  //! Trivial destructor.
  ~RegularGrid() 
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the grid dimensions.
  const Index&
  getExtents() const { 
    return _extents; 
  }

  //! Return the grid spacings.
  const Point&
  getDelta() const { 
    return _delta; 
  }

  //! Return the domain spanned by the grid.
  const BBox&
  getDomain() const { 
    return _domain; 
  }

  //! Return the index epsilon.
  Number 
  getIndexEpsilon() const { 
    return _indexEpsilon; 
  }

  //! Return the Cartesian epsilon.
  Number 
  getCartesianEpsilon() const { 
    return _cartesianEpsilon; 
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions.
  // @{

  //! Convert a Cartesian coordinate to a grid index coordinate.
  void 
  convertLocationToIndex(Point* p) const { 
    *p = (*p - _domain.getLowerCorner()) / _delta; 
  }

  //! Convert a grid index coordinate to a Cartesian coordinate.
  void 
  convertIndexToLocation(Point* p) const { 
    *p = _domain.getLowerCorner() + *p *  _delta; 
  }

  //! Convert a vector in Cartesian coordinates to index coordinates.
  void 
  convertVectorToIndex(Point* p) const { 
    *p /= _delta; 
  }

  //! Convert the Cartesian coordinates of the bounding box to a grid index coordinates.
  void 
  convertBBoxLocationsToIndices(BBox* box) const { 
    Point p = box->getLowerCorner();
    convertLocationToIndex(&p);
    box->setLowerCorner(p);
    p = box->getUpperCorner();
    convertLocationToIndex(&p);
    box->setUpperCorner(p);
  }

  //! Convert the grid index coordinates of a bounding box to Cartesian coordinates.
  void 
  convertBBoxIndicesToLocations(BBox* box) const { 
    Point p = box->getLowerCorner();
    convertIndexToLocation(&p);
    box->setLowerCorner(p);
    p = box->getUpperCorner();
    convertIndexToLocation(&p);
    box->setUpperCorner(p);
  }

  //! Convert the grid index coordinates of the first bounding box to Cartesian coordinates in the second.
  void 
  convertBBoxIndicesToLocations(const geom::BBox<N,int>& indexBox,
				BBox* cartesianBox) const { 
    Point p = indexBox.getLowerCorner();
    convertIndexToLocation(&p);
    cartesianBox->setLowerCorner(p);
    p = indexBox.getUpperCorner();
    convertIndexToLocation(&p);
    cartesianBox->setUpperCorner(p);
  }

  //! Convert a set of Cartesian coordinates to grid index coordinates.
  template<typename OutputIterator>
  void 
  convertLocationsToIndices(OutputIterator begin, OutputIterator end) const {
    for (; begin != end; ++begin) {
      convertLocationToIndex(&*begin);
    }
  }

  //! Convert a set of grid index coordinates to Cartesian coordinates.
  template<typename OutputIterator>
  void 
  convertIndicesToLocations(OutputIterator begin, OutputIterator end) const { 
    for (; begin != end; ++begin) {
      convertIndexToLocation(&*begin);
    }
  }

  //! Convert a set of vectors in Cartesian coordinates to index coordinates.
  template<typename OutputIterator>
  void 
  convertVectorsToIndices(OutputIterator begin, OutputIterator end) const { 
    for (; begin != end; ++begin) {
      convertVectorToIndex(&*begin);
    }
  }

  // @}
};


//
// Equality operators.
//


//! Return true if the true RegularGrid's are equal.
/*! \relates RegularGrid */
template<int N, typename T>
bool 
operator==(const RegularGrid<N,T>& a, const RegularGrid<N,T>& b);


//! Return true if the true RegularGrid's are not equal.
/*! \relates RegularGrid */
template<int N, typename T>
inline
bool 
operator!=(const RegularGrid<N,T>& a, const RegularGrid<N,T>& b) {
  return !(a == b);
}


//
// File I/O
//


//! Write to a file stream.
/*! \relates RegularGrid */
template<int N, typename T>
std::ostream& 
operator<<(std::ostream& out, const RegularGrid<N,T>& grid);


//! Read from a file stream.
/*! \relates RegularGrid */
template<int N, typename T>
std::istream& 
operator>>(std::istream& in, RegularGrid<N,T>& grid);


END_NAMESPACE_GEOM

#define __geom_RegularGrid_ipp__
#include "RegularGrid.ipp"
#undef __geom_RegularGrid_ipp__

#endif
