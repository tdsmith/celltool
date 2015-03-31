// -*- C++ -*-

#if !defined(__geom_ScanConversionPolygon_h__)
#define __geom_ScanConversionPolygon_h__

#include "../kernel/Point.h"
#include "../kernel/Line_2.h"
#include "../grid/RegularGrid.h"
#include "CyclicIndex.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include <cassert>
#include <cmath>
#include <cfloat>

BEGIN_NAMESPACE_GEOM

//! Class for a polygon in 2-D.
/*! 
  \param T is the number type.  By default it is double.

  A ScanConversionPolygon is a list of vertices that are ordered in the positive, 
  (counter-clockwise), direction.  The edges have outward normals. 
*/
template<typename T = double>
class ScanConversionPolygon {
public:

  //
  // Public types.
  //

  //! The floating point number type.
  typedef T Number;

  //! The representation of a point in 2 dimensions.
  typedef ads::FixedArray<2,Number> Point;
      
  //! The size type.
  typedef int SizeType;

private:

  //! Container of points.
  typedef std::vector<Point> Container;

public:

  //
  // More public types.
  //

  //! An iterator over points.
  typedef typename Container::iterator Iterator;

  //! A const Iterator over points.
  typedef typename Container::const_iterator ConstIterator;

private:

  //
  // Data
  //

  Container _vertices;
  
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  ScanConversionPolygon() :
    _vertices()
  {}

  //! Constructor.  Reserve room for size vertices.
  ScanConversionPolygon(SizeType size);

  //! Copy constructor.
  ScanConversionPolygon(const ScanConversionPolygon& other);

  //! Assignment operator.
  ScanConversionPolygon& 
  operator=(const ScanConversionPolygon& other);

  //! Trivial destructor.
  ~ScanConversionPolygon()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the number of vertices.
  SizeType
  getVerticesSize() const {
    return SizeType(_vertices.size());
  }

  //! Return a const reference to the specified vertex.
  const Point& 
  getVertex(const int n) const { 
    return _vertices[n]; 
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  // Return the beginning of the vertices.
  Iterator
  getVerticesBeginning() {
    return _vertices.begin();
  }

  // Return the end of the vertices.
  Iterator
  getVerticesEnd() {
    return _vertices.end();
  }

  //! Clear the vertices.
  void
  clear() {
    _vertices.clear();
  }

  //! Add a vertex.
  void
  insert(const Point& x) {
    _vertices.push_back(x);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions.
  // @{

  //! Order the vertices in a positive orientation.
  void 
  orderVertices();

  //! Remove adjacent duplicate vertices of an ordered polygon.
  void 
  removeDuplicates();

  //! Find the top and bottom of the polygon. Return bottom vertex index.
  int 
  computeBottomAndTop(Number* bottom, Number* top) const;

  //! Scan convert the ScanConversionPolygon in a 2-D grid.
  /*!
    \param coords is an output Iterator for the set of grid indices.
    \param extents are the extents of the grid.
  */
  template<typename IndexOutputIterator>
  void 
  scanConvert(IndexOutputIterator coords,
	      const ads::FixedArray<2,int>& extents) const {
    ads::FixedArray<2,int> multiIndex(0);
    scanConvert(coords, extents, multiIndex);
  }

  //! Scan convert the ScanConversionPolygon in a 3-D grid.
  /*!
    \param coords is an output Iterator for the set of grid indices.
    \param extents are the extents of the grid.
    \param zCoordinate is the z-coordinate of the slice being scan-converted.
  */
  template<typename IndexOutputIterator>
  void 
  scanConvert(IndexOutputIterator coords,
	      const ads::FixedArray<3,int>& extents, 
	      const int zCoordinate) const {
    ads::FixedArray<3,int> multiIndex(0);
    multiIndex[2] = zCoordinate;
    scanConvert(coords, extents, multiIndex);
  }

  //! Clip the ScanConversionPolygon against the line.
  void 
  clip(const Line_2<Number>& line);

  //! Check if polygon is valid.
  /*! 
    Check that the ScanConversionPolygon has at least three vertices and that adjacent
    vertices are not equal.
  */
  bool 
  isValid() const;
  
  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Read the number of vertices and each vertex.
  void
  get(std::istream& in);

  //! Write each vertex.
  void
  put(std::ostream& out) const;

  //! Write a Line[] object that Mathematica can read.
  void 
  mathematicaPrint(std::ostream& out) const;

  // @}

private:

  //! Scan convert the ScanConversionPolygon.
  /*!
    This function can be used for scan conversion on 2-D or 3-D grids.
    For 3-D grids, the third coordinate of \c multiIndex should be 
    set to the z-coordinate of the slice being scan-converted.
  */
  template<typename IndexOutputIterator, int N>
  void 
  scanConvert(IndexOutputIterator coords,
	      const ads::FixedArray<N,int>& extents,
	      ads::FixedArray<N,int> multiIndex) const;

  //! Scan convert the triangle.
  /*! 
    Add the set of coordinates of the grid points in this triangle 
    to \c coords.
    Precondition:
    This polygon must have exactly three vertices. 
  */
  template<typename IndexOutputIterator, int N>
  void 
  scanConvertTriangle(IndexOutputIterator coords,
		      const ads::FixedArray<N,int>& extents,
		      ads::FixedArray<N,int> multiIndex) const;
};


//! Return true if the polygons have the same points in the same order.
/*! \relates ScanConversionPolygon */
template<typename T>
inline
bool 
operator==(const ScanConversionPolygon<T>& a, 
	   const ScanConversionPolygon<T>& b);


//! Return true if they don't have the same points in the same order.
/*! \relates ScanConversionPolygon */
template<typename T>
inline
bool 
operator!=(const ScanConversionPolygon<T>& a, 
	   const ScanConversionPolygon<T>& b) {
  return !(a == b);
}


//! Read the number of vertices and each vertex.
/*! \relates ScanConversionPolygon */
template<typename T>
inline
std::istream& 
operator>>(std::istream& in, ScanConversionPolygon<T>& x) {
  x.get(in);
  return in;
}


//! Write each vertex.
/*! \relates ScanConversionPolygon */
template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const ScanConversionPolygon<T>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_ScanConversionPolygon_ipp__
#include "ScanConversionPolygon.ipp"
#undef __geom_ScanConversionPolygon_ipp__

#endif
