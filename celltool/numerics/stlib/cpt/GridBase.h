// -*- C++ -*-

/*! 
  \file GridBase.h
  \brief Implements a class for the grid data.
*/

#if !defined(__cpt_GridBase_h__)
#define __cpt_GridBase_h__

// Local
#include "defs.h"

// Algorithms and data structures package.
#include "../ads/array/Array.h"
#include "../ads/array/IndexIterator.h"

// Computational geometry package.
#include "../geom/grid/RegularGrid.h"

#include <iostream>
#include <vector>

BEGIN_NAMESPACE_CPT

//! Base class for Grid.
/*! 
  Implements common functionality for grids of different dimensions.
  Stores the distance, gradient of distance, closest point, and closest
  face arrays.
*/
template<int N, typename T>
class GridBase {
  //
  // Private types.
  //

private:

  //! An array type used to define the index type and range type.
  typedef ads::Array<N,int> ArrayInt;

  //
  // Protected types.
  //

protected:

  //! The number type.
  typedef T Number;
  //! A point in N-D.
  typedef ads::FixedArray<N,Number> Point;
  //! A multi-index in N-D.
  typedef typename ArrayInt::index_type Index;
  //! A multi-index range in N-D.
  typedef typename ArrayInt::range_type Range;
  //! A lattice.
  typedef geom::RegularGrid<N,Number> Lattice;
  
  //
  // Data.
  //

private:

  //! Array of distances.
  ads::Array<N,Number,false> _distance;

  //! Array of gradient of distance.
  ads::Array<N,Point,false> _gradientOfDistance;

  //! Array of closest Points.
  ads::Array<N,Point,false> _closestPoint;

  //! Array of closest faces.
  ads::Array<N,int,false> _closestFace;

protected:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.  Empty arrays.
  GridBase() : 
    _distance(),
    _gradientOfDistance(),
    _closestPoint(),
    _closestFace()
  {}

  //! Copy constructor.  Reference the arrays.
  GridBase(const GridBase& other) :
    _distance(other._distance),
    _gradientOfDistance(other._gradientOfDistance),
    _closestPoint(other._closestPoint),
    _closestFace(other._closestFace)
  {}

  //! Construct from the grids.
  template<bool A1, bool A2, bool A3, bool A4>
  GridBase(ads::Array<N,Number,A1>* distance,
	   ads::Array<N,Point,A2>* gradientOfDistance, 
	   ads::Array<N,Point,A3>* closestPoint,
	   ads::Array<N,int,A4>* closestFace);

  //! Destructor.  Does not free grid memory.
  ~GridBase()
  {}

  //! Assignment operator.
  GridBase& 
  operator=(const GridBase& other);
  
  //@}

public:

  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the grid extents.
  const Index&
  getExtents() const { 
    return _distance.extents();
  }

  //! Return the grid ranges.
  const Range&
  getRanges() const { 
    return _distance.ranges();
  }

  //! Return true if the grids are empty.
  bool
  isEmpty() const {
    return _distance.empty();
  }

  //! Return a const reference to the distance grid.
  const ads::Array<N,Number,false>& 
  getDistance() const { 
    return _distance; 
  }

  //! Return a const reference to the gradient of the distance grid.
  const ads::Array<N,Point,false>& 
  getGradientOfDistance() const { 
    return _gradientOfDistance; 
  }

  //! Return a const reference to the closest point grid.
  const ads::Array<N,Point,false>& 
  getClosestPoint() const { 
    return _closestPoint; 
  }

  //! Return a const reference to the closest face grid.
  const ads::Array<N,int,false>& 
  getClosestFace() const { 
    return _closestFace; 
  }

  //! Is the gradient of the distance being computed?
  bool 
  isGradientOfDistanceBeingComputed() const { 
    return ! _gradientOfDistance.empty(); 
  }

  //! Is the closest point being computed?
  bool 
  isClosestPointBeingComputed() const { 
    return ! _closestPoint.empty(); 
  }

  //! Is the closest face being computed?
  bool 
  isClosestFaceBeingComputed() const { 
    return ! _closestFace.empty(); 
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Return a reference to the distance grid.
  ads::Array<N,Number,false>& 
  getDistance() { 
    return _distance; 
  }

  //! Return a reference to the gradient of the distance grid.
  ads::Array<N,Point,false>& 
  getGradientOfDistance() { 
    return _gradientOfDistance; 
  }

  //! Return a reference to the closest point grid.
  ads::Array<N,Point,false>& 
  getClosestPoint() { 
    return _closestPoint; 
  }

  //! Return a reference to the closest face grid.
  ads::Array<N,int,false>& 
  getClosestFace() { 
    return _closestFace; 
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Calculate the signed distance, closest point, etc. for the specified grid points.
  /*!
    \return the number of distances computed and the number of distances set.
  */
  template<class Component>
  std::pair<int,int>
  computeClosestPointTransform(const std::vector<Index>& indices,
			       const std::vector<Point>& positions,
			       const Component& component,
			       Number maximumDistance);

  //! Calculate the unsigned distance, closest point, etc. for the specified grid points.
  template<class Component>
  std::pair<int,int>
  computeClosestPointTransformUnsigned(const std::vector<Index>& indices,
				       const std::vector<Point>& positions,
				       const Component& component,
				       Number maximumDistance);

  //! Calculate the signed distance, closest point, etc. for the specified grid points.
  /*!
    In computing distances, use the versions that check with the 
    characteristics of the component.

    \return the number of distances computed and the number of distances set.
  */
  template<class Component>
  std::pair<int,int>
  computeClosestPointTransform(const Lattice& lattice,
			       const Range& indexRangeInLattice,
			       const Component& component,
			       Number maximumDistance);

  //! Calculate the unsigned distance, closest point, etc. for the specified grid points.
  /*!
    In computing distances, use the versions that check with the 
    characteristics of the component.

    \return the number of distances computed and the number of distances set.
  */
  template<class Component>
  std::pair<int,int>
  computeClosestPointTransformUnsigned(const Lattice& lattice,
				       const Range& indexRangeInLattice,
				       const Component& component,
				       Number maximumDistance);

  //! Initialize the grids.
  /*!
    Set all the distances to \c std::numeric_limits<Number>::max() in 
    preparation for the distance to be 
    computed.  Set the gradient of the distance and the closest points to
    std::numeric_limits<Number>::max().  Set the closest faces to -1.
  */
  void 
  initialize();

  //! Flood fill the unsigned distance.
  /*!
    If there are any points with known distance then return true and set 
    the unknown distances to farAway.  Otherwise set all the distances 
    to farAway and return false.
  */
  bool 
  floodFillUnsigned(const Number farAway);

  //@}
  //-------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  void 
  put(std::ostream& out) const;

  void 
  displayInformation(std::ostream& out) const;

  int
  countKnownDistances(const Number maximumDistance) const;

  void 
  computeMinimumAndMaximumDistances(const Number maximumDistance, 
				    Number* minimum, Number* maximum) const;

  //@}
};

END_NAMESPACE_CPT

//
// Equality operators
//

//! Return true if the grids are equal.
/*! \relates cpt::GridBase */
template<int N, typename T>
bool 
operator==(const cpt::GridBase<N,T>& a, const cpt::GridBase<N,T>& b);

//! Return true if the grids are not equal.
/*! \relates cpt::GridBase */
template<int N, typename T>
inline
bool 
operator!=(const cpt::GridBase<N,T>& a, const cpt::GridBase<N,T>& b) {
  return !(a == b);
}

#define __GridBase_ipp__
#include "GridBase.ipp"
#undef __GridBase_ipp__

#endif
