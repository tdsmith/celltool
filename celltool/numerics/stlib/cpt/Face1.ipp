// -*- C++ -*-

#if !defined(__cpt_Face1_ipp__)
#error This file is an implementation detail of the class Face.
#endif

BEGIN_NAMESPACE_CPT

//! A 1-D face on a b-rep.
template<typename T>
class Face<1,T> {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! A Cartesian point.
  typedef Number Point;

  //! A bounding box.
  typedef geom::BBox<1,Number> BBox;

private:

  // 
  // Member Data
  //

  //! The location of the face.
  Point _location;

  //! The orientation of the face.
  int _orientation;

  // The index of this face.
  int _index;

  //! The domain containing possible closest points.
  BBox _domain;

public:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.  Unititialized memory.
  Face()
  {}

  //! Construct a face.
  /*!
    \param location is the Cartesian location of the face.
    \param orientation is +-1.
    \param index is the index of the face.
    \param left is the location of the left neighbor.
    \param right is the location of the right neighbor.
    \param max_distance is how far the distance is being computed.
   */
  Face(const Point location, 
	const int orientation,
	const int index,
	const Point left,
	const Point right,
	const Number max_distance) { 
    make(location, orientation, index, left, right, max_distance);
  }

  //! Copy constructor.
  Face(const Face& other) :
    _location(other._location),
    _orientation(other._orientation),
    _index(other._index),
    _domain(other._domain)
  {}

  //! Assignment operator.
  Face& 
  operator=(const Face& other) {
    if (&other != this) {
      _location = other._location;
      _orientation = other._orientation;
      _index = other._index;
      _domain = other._domain;
    }
    return *this;
  }

  //! Construct a face.
  /*!
    \param location is the Cartesian location of the face.
    \param orientation is +-1.
    \param index is the index of the face.
    \param left is the location of the left neighbor.
    \param right is the location of the right neighbor.
    \param max_distance is how far the distance is being computed.
   */
  void 
  make(const Point location, 
       const int orientation,
       const int index,
       const Point left,
       const Point right,
       const Number max_distance) {
    _location = location;
    _orientation = orientation;
    _index = index;
    _domain.setLowerCoordinate(0, std::max((left + location) / 2.0, 
					   location - max_distance));
    _domain.setUpperCoordinate(0, std::min((location + right) / 2.0, 
					   location + max_distance));
  }

  //! Trivial destructor.
  ~Face()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{
    
  //! Return the location of the face.
  Point
  getLocation() const { 
    return _location; 
  }

  //! Return the orientation of the face.
  int
  getOrientation() const { 
    return _orientation; 
  }

  //! Return the normal to the face.
  Point
  getNormal() const { 
    return _orientation;
  }

  //! Return the index of this face in the b-rep.
  int 
  getFaceIndex() const { 
    return _index; 
  }

  //! Return the domain containing possible closest points.
  const BBox&
  getDomain() const {
    return _domain;
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Return true if the face is valid.
  bool 
  isValid() const;

  //! Return the signed distance to the face.
  Number 
  computeDistance(const Point p) const {
    return _orientation * (p - _location);
  }

  //! Return the unsigned distance to the face.
  Number 
  computeDistanceUnsigned(const Point p) const {
    return std::abs(p - _location);
  }

  //@}
};





//
// Mathematical Operations
//

template<typename T>
inline
bool
Face<1,T>::
isValid() const {
  if (!(_orientation == -1 || _orientation == 1)) {
    return false;
  }
  if (_index < 0) {
    return false;
  }
  if (_domain.getLowerCorner()[0] >= _location) {
    return false;
  }
  if (_location >= _domain.getUpperCorner()[0]) {
    return false;
  }
  return true;
}



//
// Equality / Inequality
//

template<typename T>
inline
bool 
operator==(const Face<1,T>& x, const Face<1,T>& y) {
  if (x.getLocation() == y.getLocation() &&
      x.getOrientation() == y.getOrientation() &&
      x.getFaceIndex() == y.getFaceIndex() ) {
    return true;
  }
  return false;
}


//
// File I/O
//

template<typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Face<1,T>& x) {
  return out << "location = " << x.getLocation() << '\n'
	     << "orientation = " << x.getOrientation() << '\n'
	     << "face index = " << x.getFaceIndex() << '\n'
	     << "domain = " << x.getDomain() << '\n';
}

END_NAMESPACE_CPT
