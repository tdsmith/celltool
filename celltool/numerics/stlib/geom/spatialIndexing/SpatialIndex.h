// -*- C++ -*-

/*!
  \file geom/spatialIndexing/SpatialIndex.h
  \brief N-D spatial index that uses both interlaced and non-interlaced representations.
*/

#if !defined(__geom_spatialIndexing_SpatialIndex_h__)
#define __geom_spatialIndexing_SpatialIndex_h__

#include "../defs.h"

#include "../../ads/Array/FixedArray.h"

#include "../../numerical/integer/bits.h"
#include "../../numerical/integer/print.h"
#include "../../numerical/constants/Exponentiation.h"
#include "../../numerical/constants/Logarithm.h"

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_SpatialIndex)
#define DEBUG_geom_spatialIndexing_SpatialIndex
#endif

BEGIN_NAMESPACE_GEOM

//! N-D spatial index that uses both interlaced and non-interlaced representations.
/*!
  \param _Dimension The Dimension of the space.
  \param _MaximumLevel The levels are in the range [0 .. _MaximumLevel].

  At the finest level, there are \f$2^{\mathrm{MaximumLevel}}\f$
  elements along each dimension.
*/
template<int _Dimension, int _MaximumLevel>
class
SpatialIndex {
  //
  // Enumerations.
  //
public:

  enum {Dimension = _Dimension, MaximumLevel = _MaximumLevel,
	NumberOfOrthants = numerical::Exponentiation<2, _Dimension>::Result};

  //
  // Public types.
  //
public:

  //! An integer type that can hold the level.
  typedef typename numerical::UnsignedInteger
  <numerical::Logarithm<2, MaximumLevel + 1>::Result>::Type Level;
  //! An integer type that can hold a binary coordinate.
  typedef typename numerical::UnsignedInteger<MaximumLevel>::Type 
  Coordinate;
  //! An integer type that can hold the interleaved coordinate code.
  typedef typename numerical::UnsignedInteger<Dimension * MaximumLevel>::Type
  Code;

  //
  // Member data.
  //
private:

  //! The interleaved (left-shifted) coordinates.
  Code _code;
  //! Discrete Cartesian coordinates.
  /*!
    Note that these are right-shifted.  Left shift by 
    (MaximumLevel - _level) to get the actual coordinate.
  */
  ads::FixedArray<Dimension, Coordinate> _coordinates;
  //! The level is in the range [0..MaximumLevel].
  Level _level;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Default constructor.  Zero level and coordinates.
  SpatialIndex() :
    _code(0),
    _coordinates(Coordinate(0)),
    _level(0) {
    LOKI_STATIC_CHECK(Dimension > 0, BadDimension);
  }

  //! Construct from the level and the coordinates.
  SpatialIndex(const Level level, 
	       const ads::FixedArray<Dimension, Coordinate>& coordinates) :
    _code(),
    _coordinates(coordinates),
    _level(level) {
    LOKI_STATIC_CHECK(Dimension > 0, BadDimension);
    updateCode();
  }

  //! Copy constructor.
  SpatialIndex(const SpatialIndex& other) :
    _code(other._code),
    _coordinates(other._coordinates),
    _level(other._level) {
  }

  //! Assignment operator.
  SpatialIndex&
  operator=(const SpatialIndex& other) {
    if (this != &other) {
      _code = other._code;
      _coordinates = other._coordinates;
      _level = other._level;
    }
    return *this;
  }

  //! Destructor.
  ~SpatialIndex() {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the interleaved code.
  Code
  getCode() const {
    return _code;
  }

  //! Get the level.
  Level
  getLevel() const {
    return _level;
  }

  //! Get the coordinates.
  const ads::FixedArray<Dimension, Coordinate>&
  getCoordinates() const {
    return _coordinates;
  }

  //! Return true if the level can be increased.
  bool
  canBeRefined() const {
    return _level < MaximumLevel;
  }

  //! Return true if the level can be decreased.
  bool
  canBeCoarsened() const {
    return _level > 0;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the level and coordinates.
  void
  set(const Level level, 
      const ads::FixedArray<Dimension, Coordinate> coordinates) {
    _level = level;
    _coordinates = coordinates;
    updateCode();
  }

  //! Transform to the parent node index.
  /*!
    \pre The level is positive.  (This node has a parent.)
  */
  void
  transformToParent() {
#ifdef DEBUG_geom_spatialIndexing_SpatialIndex
    assert(_level != 0);
#endif
    --_level;
    _coordinates >>= 1;
    updateCode();
  }

  //! Transform to the specified child node index.
  /*!
    \pre This node must not already be at the deepest level.
  */
  void
  transformToChild(unsigned int n) {
#ifdef DEBUG_geom_spatialIndexing_SpatialIndex
    assert(n < NumberOfOrthants);
#endif
    ++_level;
    _coordinates <<= 1;
    for (int i = 0; i != Dimension; ++i) {
      _coordinates[i] += n % 2;
      n >>= 1;
    }
    updateCode();
  }

  //! Transform to the specified child node index the specified number of times.
  /*!
    \pre The child node must not exceed the deepest level.
  */
  void
  transformToChild(const unsigned int n, const int steps) {
#ifdef DEBUG_geom_spatialIndexing_SpatialIndex
    assert(n < NumberOfOrthants);
    assert(steps >= 0);
#endif
    _level += steps;
    for (int s = 0; s != steps; ++s) {
      _coordinates <<= 1;
      unsigned int m = n;
      for (int i = 0; i != Dimension; ++i) {
	_coordinates[i] += m % 2;
	m >>= 1;
      }
    }
    updateCode();
  }

  //! Transform to the specified neighbor.
  /*!
    \pre This node must have an adjacent neighbor in the specified direction.
  */
  void
  transformToNeighbor(int n);

private:

  void
  updateCode() {
    // Make a copy of the coordinates.  We will right-shift them to strip
    // off the binary digits.
    ads::FixedArray<Dimension, Coordinate> c(_coordinates);
    // First left-shift to get the real coordinates.
    c <<= (MaximumLevel - _level);
    // Interlace the coordinates.
    _code = numerical::interlaceBits<Code>(c, MaximumLevel);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print the level and the coordinates.
  void
  print(std::ostream& out) const {
    out << double(_level) << " ";
    for (int i = 0; i != Dimension; ++i) {
      numerical::printBits(out, _coordinates[i]);
      out << " ";
    }
    numerical::printBits(out, _code);
  }

  //@}
};
  
//---------------------------------------------------------------------------
// Equality.

//! Return true if they are equal.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
operator==(const SpatialIndex<_Dimension, _MaximumLevel>& a,
	   const SpatialIndex<_Dimension, _MaximumLevel>& b) {
  // We don't need to check the coordinates.  The code is the interleaved 
  // coordinates.
  const bool result = a.getLevel() == b.getLevel() && 
    a.getCode() == b.getCode();
#ifdef DEBUG_geom_spatialIndexing_SpatialIndex
  if (result) {
    assert(a.getCoordinates() == b.getCoordinates());
  }
#endif
  return result;
}

//! Return true if they are not equal.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
operator!=(const SpatialIndex<_Dimension, _MaximumLevel>& a,
	   const SpatialIndex<_Dimension, _MaximumLevel>& b) {
  return !(a == b);
}


//! Less than comparison on the code.
/*!
  \relates SpatialIndex
  \note The code is not sufficient to describe a node.  For example,
  the code 0 can represent the node in the lower corner at any level.  
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
operator<(const SpatialIndex<_Dimension, _MaximumLevel>& a,
	  const SpatialIndex<_Dimension, _MaximumLevel>& b) {
  return a.getCode() < b.getCode();
}

//---------------------------------------------------------------------------
// File I/O.

//! Print the spatial index.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
std::ostream&
operator<<(std::ostream& out, 
	   const SpatialIndex<_Dimension, _MaximumLevel>& x) {
  x.print(out);
  return out;
}

//---------------------------------------------------------------------------
// Topology and geometry.

//! Return true if the key is a local lower corner.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
isLowerCorner(const SpatialIndex<_Dimension, _MaximumLevel>& x) {
  int sum = 0;
  for (int d = 0; d != _Dimension; ++d) {
    sum += x.getCoordinates()[d] % 2;
  }
  return sum == 0;
}


//! Return true if the key has a parent.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
hasParent(const SpatialIndex<_Dimension, _MaximumLevel>& x) {
  return x.getLevel() != 0;
}

//! Return the position of the lower corner of the node.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
void
computeLocation(const SpatialIndex<_Dimension, _MaximumLevel>& spatialIndex,
		ads::FixedArray<_Dimension, 
		typename SpatialIndex<_Dimension, _MaximumLevel>::Coordinate>*
		location) {
  *location = spatialIndex.getCoordinates();
  *location <<= _MaximumLevel - spatialIndex.getLevel();
}

//! Return the position of the lower side of the node.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
typename SpatialIndex<_Dimension, _MaximumLevel>::Coordinate
computeLocation(const SpatialIndex<_Dimension, _MaximumLevel>& spatialIndex,
		const int n) {
  return spatialIndex.getCoordinates()[n] << 
    (_MaximumLevel - spatialIndex.getLevel());
}

//! Compute the length of a side.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
int
computeLength(const SpatialIndex<_Dimension, _MaximumLevel>& spatialIndex) {
  return 1 << (_MaximumLevel - spatialIndex.getLevel());
}

//! Compute the distance between the two nodes.
/*!
  \relates SpatialIndex
*/
template<int _Dimension, int _MaximumLevel>
inline
void
computeSeparations(const SpatialIndex<_Dimension, _MaximumLevel>& index1,
		   const SpatialIndex<_Dimension, _MaximumLevel>& index2,
		   ads::FixedArray<_Dimension, int>* separations) {
  typedef SpatialIndex<_Dimension, _MaximumLevel> SpatialIndex;
  typedef typename SpatialIndex::Coordinate Coordinate;

  ads::FixedArray<_Dimension, Coordinate> location1, location2;
  computeLocation(index1, &location1);
  computeLocation(index2, &location2);
  // CONTINUE: Does int have enough digits.
  const int length1 = computeLength(index1);
  const int length2 = computeLength(index2);
  for (int d = 0; d != _Dimension; ++d) {
    (*separations)[d] = (location1[d] < location2[d] ? 
			 (location2[d] - location1[d]) - length1 : 
			 (location1[d] - location2[d]) - length2);
  }
}

//! Return true if the nodes are adjacent.
/*!
  \relates SpatialIndex
  
  Two nodes in N-D are adjacent if they share a (N-1)-D boundary.
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
areAdjacent(const SpatialIndex<_Dimension, _MaximumLevel>& a,
	    const SpatialIndex<_Dimension, _MaximumLevel>& b) {
  ads::FixedArray<_Dimension, int> separations;
  computeSeparations(a, b, &separations);
  int countZero = 0, countNegative = 0;
  for (int i = 0; i != _Dimension; ++i) {
    countZero += separations[i] == 0;
    countNegative += separations[i] < 0;
  }
  return countZero == 1 && countNegative == _Dimension - 1;
}

//! Return true if the node has a neighbor in the specified direction.
/*!
  \relates SpatialIndex
  direction / 2 gives the coordinate.  direction % 2 gives the direction in
  that coordinate (negative or positive).
*/
template<int _Dimension, int _MaximumLevel>
inline
bool
hasNeighbor(const SpatialIndex<_Dimension, _MaximumLevel>& node,
	    const int direction) {
  typedef typename SpatialIndex<_Dimension, _MaximumLevel>::Coordinate 
    Coordinate;

  // Negative direction.
  if (direction % 2 == 0) {
    // The lower side of the node.
    return node.getCoordinates()[direction / 2] != 0;
  }
  // Positive direction.
  else {
    // The upper side of the node.
    Coordinate upper = computeLocation(node, direction / 2)
      + computeLength(node);
    // The coordinate may have more than _MaximumLevel digits.  Keep only 
    // _MaximumLevel digits.
    upper <<= std::numeric_limits<Coordinate>::digits - _MaximumLevel;
    return upper != 0;
  }
}

END_NAMESPACE_GEOM

#define __geom_spatialIndexing_SpatialIndex_ipp__
#include "SpatialIndex.ipp"
#undef __geom_spatialIndexing_SpatialIndex_ipp__

#endif
