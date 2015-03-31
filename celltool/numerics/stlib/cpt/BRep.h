// -*- C++ -*-

#if !defined(__cpt_BRep_h__)
#define __cpt_BRep_h__

// Local
#include "Grid.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "performance.h"

// Algorithms and data structures.
#include "../ads/array/FixedArray.h"
#ifdef CPT_PERFORMANCE
#include "../ads/timer.h"
#endif

// Geometry
#include "../geom/mesh/iss/build.h"
#include "../geom/mesh/iss/file_io.h"
#include "../geom/mesh/iss/geometry.h"
#include "../geom/mesh/iss/quality.h"
#include "../geom/mesh/iss/set.h"
#include "../geom/polytope/IndexedEdgePolyhedron.h"

#include <fstream>
#include <vector>
#include <utility>
#include <set>

BEGIN_NAMESPACE_CPT

/*! 
  \file BRep.h
  \brief Implements a class for a b-rep.
*/

template <int N, typename T = double>
class BRep;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 1-D
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//! A class for a b-rep.
template <typename T>
class BRep<1,T> {
public:

  // 
  // Public types.
  //

  //! The number type.
  typedef T Number;
  //! A point in 1-D.
  typedef ads::FixedArray<1,Number> Point;
  //! A bounding box.
  typedef geom::BBox<1,Number> BBox;
  //! The grid type.
  typedef Grid<1,Number> Grid;

  // CONTINUE: With the Intel compiler, private members are not accessible
  // in nested classes.
#ifdef __INTEL_COMPILER
public:
#else
private:
#endif

  // The representation of a face.
  class FaceRep {
  private:

    //! The location of the face.
    Point _location;
    //! The orientation of the face.
    int _orientation;
    //! The identifier of the face.
    int _identifier;

  public:

    //------------------------------------------------------------------------
    //! \name Constructors etc.
    //@{

    //! Default constructor.  Uninitialized values.
    FaceRep()
    {}

    //! Make a 1-D face.
    FaceRep(const Point& location, const int orientation, 
	    const int identifier) :
      _location(location),
      _orientation(orientation),
      _identifier(identifier)
    {}

    //! Copy constructor.
    FaceRep(const FaceRep& other) :
      _location(other._location),
      _orientation(other._orientation),
      _identifier(other._identifier)
    {}

    //! Assignment operator.
    FaceRep&
    operator=(const FaceRep& other) {
      if (&other != this) {
	_location = other._location;
	_orientation = other._orientation;
	_identifier = other._identifier;
      }
      return *this;
    }

    //! Trivial destructor.
    ~FaceRep()
    {}

    //@}
    //------------------------------------------------------------------------
    //! \name Accessors.
    //@{

    //! Return the location of the face.
    const Point&
    getLocation() const {
      return _location;
    }

    //! Return the orientation of the face.
    int 
    getOrientation() const {
      return _orientation;
    }

    //! Return the identifier of the face.
    int 
    getIdentifier() const {
      return _identifier;
    }

    //@}
  };

  // Functor for comparing faces by their location.
  class LocationCompare :
    public std::binary_function<FaceRep,FaceRep,bool> {
  private:
    typedef std::binary_function<FaceRep,FaceRep,bool> Base;
  public:
    typedef typename Base::first_argument_type first_argument_type;
    typedef typename Base::second_argument_type second_argument_type;
    typedef typename Base::result_type result_type;

    result_type
    operator()(const first_argument_type& x, const second_argument_type& y) {
      return x.getLocation() < y.getLocation();
    }
  };

private:

  // CONTINUE REMOVE
  // This class needs access to private type inside BRep.
  //friend class LocationCompare;

  // Functor for comparing faces by their identifier.
  class IdentifierCompare :
    public std::binary_function<FaceRep,FaceRep,bool> {
  private:
    typedef std::binary_function<FaceRep,FaceRep,bool> Base;
  public:
    typedef typename Base::first_argument_type first_argument_type;
    typedef typename Base::second_argument_type second_argument_type;
    typedef typename Base::result_type result_type;

    result_type
    operator()(const first_argument_type& x, const second_argument_type& y) {
      return x.getIdentifier() < y.getIdentifier();
    }
  };

  // CONTINUE REMOVE
  // This class needs access to private type inside BRep.
  //friend class IdentifierCompare;

  //
  // Private types.
  //

  //! A face that is useful for computing distance and scan conversion.
  typedef Face<1,Number> Face;

  //
  // Member data.
  //

  // The faces.
  std::vector<FaceRep> _faces;
  // How far to compute the distance.
  mutable Number _maximumDistance;

public:

  //--------------------------------------------------------------------------
  // \name Constructors, etc.
  //@{
    
  //! Default constructor.  An empty b-rep.
  BRep() : 
    _faces(),
    _maximumDistance(0)
  {}

  //! Construct from the faces of the b-rep.  Throw away irrelevant ones.
  /*!
    \param locationsBeginning is the beginning of the face locations.
    \param locations_end is the end of the face locations.
    \param orientations_begin is the beginning of the face orientations.
    +1 means that positive distances are to the right.  -1 means that 
    positive distances are to the left.  
    \param orientationsEnd is the end of the face orientations.
    \param cartesianDomain is the domain of the grid.
    \param maximumDistance is how far the distance is being computed.

    Clip the b-rep so that faces outside the relevant Cartesian domain 
    are thrown away.

    This constructor calls make() with the same arguments.
  */
  template <typename NumberInputIter, typename IntegerInputIter>
  BRep(NumberInputIter locationsBeginning, NumberInputIter locationsEnd,
       IntegerInputIter orientationsBeginning, 
       IntegerInputIter orientationsEnd,
       const BBox& cartesianDomain,
       const Number maximumDistance) {
    make(locationsBeginning, locationsEnd, 
	 orientationsBeginning, orientationsEnd,
	 cartesianDomain, maximumDistance);
  }

  //! Copy constructor.
  BRep(const BRep& other) :
    _faces(other._faces),
    _maximumDistance(other._maximumDistance)
  {}

  //! Assignment operator.
  BRep& 
  operator=(const BRep& other) {
    if (&other != this) {
      _faces = other._faces;
      _maximumDistance = other._maximumDistance;
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~BRep()
  {}
    
  //! Make from the faces of the b-rep.
  /*!
    \param locationsBeginning is the beginning of the face locations.
    \param locationsEnd is the end of the face locations.
    \param orientationsBeginning is the beginning of the face orientations.
    +1 means that positive distance are to the right.  -1 means that 
    positive distance are to the left.  
    \param orientationsEnd is the end of the face orientations.
  */
  template <typename NumberInputIter, typename IntegerInputIter>
  void 
  make(NumberInputIter locationsBeginning, NumberInputIter locationsEnd,
       IntegerInputIter orientationsBeginning, 
       IntegerInputIter orientationsEnd) {
    // Add the faces.
    NumberInputIter location = locationsBeginning;
    IntegerInputIter orientation = orientationsBeginning;
    int identifier = 0;
    while (location != locationsEnd) {
      insertFace(*location, *orientation, identifier);
      ++location;
      ++orientation;
      ++identifier;
    }
#ifdef DEBUG_cpt
    assert(orientation == orientationsEnd);
#endif

    // Sort the faces.
    LocationCompare comp;
    std::sort(_faces.begin(), _faces.end(), comp);
  }

  //! Make from the faces of the b-rep.  Throw away irrelevant ones.
  /*!
    \param locationsBeginning is the beginning of the face locations.
    \param locationsEnd is the end of the face locations.
    \param orientationsBeginning is the beginning of the face orientations.
    +1 means that positive distance are to the right.  -1 means that 
    positive distance are to the left.  
    \param orientationsEnd is the end of the face orientations.
    \param cartesianDomain is the domain of the grid.
    \param maximumDistance is how far the distance is being computed.

    Clip the b-rep so that faces outside the relevant Cartesian domain 
    are thrown away.
  */  
  template <typename NumberInputIter, typename IntegerInputIter>
  void 
  make(NumberInputIter locationsBeginning, NumberInputIter locationsEnd,
       IntegerInputIter orientationsBeginning, 
       IntegerInputIter orientationsEnd,
       const BBox& cartesianDomain,
       const Number maximumDistance) {
    _maximumDistance = maximumDistance;
    const BBox 
      interestDomain(cartesianDomain.getLowerCorner()[0] - maximumDistance,
		     cartesianDomain.getUpperCorner()[0] + maximumDistance);

    // Add the faces.
    NumberInputIter location = locationsBeginning;
    IntegerInputIter orientation = orientationsBeginning;
    int identifier = 0;
    ads::FixedArray<1,Number> loc;
    while (location != locationsEnd) {
      loc[0] = (*location)[0];
      if (interestDomain.isIn(loc)) {
	insertFace(loc, *orientation, identifier);
      }
      ++location;
      ++orientation;
      ++identifier;
    }
#ifdef DEBUG_cpt
    assert(orientation == orientationsEnd);
#endif

    // Sort the faces.
    LocationCompare comp;
    std::sort(_faces.begin(), _faces.end(), comp);
  }

  //@}
  //--------------------------------------------------------------------------
  // \name Size accessors.
  //@{

  //! Return true if there are no faces.
  bool
  isEmpty() const {
    return _faces.empty();
  }

  //! Return the number of faces.
  int
  getSimplicesSize() const {
    return _faces.size();
  }

  //! Return the maximum face identifier.
  int 
  computeMaximumFaceIdentifier() const {
    if (isEmpty()) {
      return -1;
    }
    IdentifierCompare comp;
    return std::max_element(_faces.begin(), _faces.end(), 
			    comp)->getIdentifier();
  }

  //@}
  //--------------------------------------------------------------------------
  // \name Mathematical Operations.
  //@{

  //! Calculate the closest point transform to this BRep.
  std::pair<int,int>
  computeClosestPoint(std::vector<Grid>& grids, 
		      const Number maximumDistance) const {
    // CONTINUE: Make efficient.
    const int gridsSize = grids.size();
    assert(gridsSize > 0);

    _maximumDistance = maximumDistance;
    int i;
    int scanConversionCount = 0;
    int distanceCount = 0;
    std::pair<int,int> faceCount;

    // Find the closest points and distance for the faces.
    Face face;
    for (i = 0; i != getSimplicesSize(); ++i) {
      // Get the i_th face.
      getFace(i, &face);
      // For each grid.
      for (int n = 0; n != gridsSize; ++n) {
	// Scan convert the grid points and compute the distance etc.
	faceCount = grids[n].computeClosestPointTransform(face, 
							  maximumDistance);
	scanConversionCount += faceCount.first;
	distanceCount += faceCount.second;
      }
    }

    return std::pair<int,int>(scanConversionCount, distanceCount);
  }

  //! Calculate the closest point transform with unsigned distance to this BRep.
  std::pair<int,int>
  computeClosestPointUnsigned(std::vector<Grid>& grids, 
			      const Number maximumDistance) const {
    // CONTINUE: Make efficient.
    const int gridsSize = grids.size();
    assert(gridsSize > 0);

    _maximumDistance = maximumDistance;
    int i;
    int scanConversionCount = 0;
    int distanceCount = 0;
    std::pair<int,int> faceCount;

    // Find the closest points and distance for the faces.
    Face face;
    for (i = 0; i != getSimplicesSize(); ++i) {
      // Get the i_th face.
      getFace(i, &face);
      // For each grid.
      for (int n = 0; n != gridsSize; ++n) {
	// Scan convert the grid points and compute the distance etc.
	faceCount = grids[n].computeClosestPointTransformUnsigned
	  (face, maximumDistance);
	scanConversionCount += faceCount.first;
	distanceCount += faceCount.second;
      }
    }

    return std::pair<int,int>(scanConversionCount, distanceCount);
  }

  //! Return the bounding box that contains the mesh.
  BBox
  computeBBox() const {
    if (getSimplicesSize() == 0) {
      return BBox(0, -1); 
    }
    BBox box(_faces[0].getLocation()[0], _faces[0].getLocation()[0]);
    for (int n = 1; n != getSimplicesSize(); ++n) {
      box.add(_faces[n].getLocation());
    }
    return box;
  }

  //@}
  //--------------------------------------------------------------------------
  // \name File I/O.
  //@{

  //! Display information about the b-rep.
  /*!
    Report if the manifold is closed.
  */
  void 
  displayInformation(std::ostream& out) const {
    out << "Number of faces: " << getSimplicesSize() << '\n'
	<< "Bounding box: " << computeBBox() << '\n';
  }

  //! Display the b-rep.
  void 
  display(std::ostream& out) const {
    out << "Number of faces: " << getSimplicesSize() << '\n';
    const int iEnd = _faces.size();
    for (int i = 0; i != iEnd; ++i) {
      out << _faces[i].getLocation() << " "
	  << _faces[i].getOrientation() << " "
	  << _faces[i].getIdentifier() << '\n';
    }
  }

  //@}

private:

  //
  // Accessors.
  //

  // Make the n_th face
  void 
  getFace(const int n, Face* face) const {
#ifdef DEBUG_cpt
    assert(0 <= n && n < getSimplicesSize());
#endif
    // Get locations for the left and right neighbors.
    // If there is no neighbor, give a far-away point.
    // I divide by 2 to avoid overflow problems in Face.
    Point left = - std::numeric_limits<Number>::max() / 2.0;
    if (n != 0) {
      left = _faces[n-1].getLocation();
    }
    Point right = std::numeric_limits<Number>::max() / 2.0;
    if (n != getSimplicesSize() - 1) {
      right = _faces[n+1].getLocation();
    }
    // Make the face.
    face->make(_faces[n].getLocation(), _faces[n].getOrientation(), 
	       _faces[n].getIdentifier(), left, right, _maximumDistance);
  }

  int 
  getFaceIdentifier(const int n) const {
    return _faces[n].getIdentifier();
  }

  //
  // Manipulators.
  //

  // Add a face. 
  // a, b and c are indices of vertices in the positive orientation.
  void 
  insertFace(const Point& location, const int orientation, 
	     const int faceIdentifier) {
    _faces.push_back(FaceRep(location, orientation, faceIdentifier));
  }

  // Clear all the member data.
  void 
  clear() {
    _faces.clear();
  }

};


//
// File IO
//

//! Write the b-rep.
/*! 
  \relates BRep<N,T> 
  CONTINUE
*/
template <int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const BRep<N,T>& br) {
  br.display(out);
  return out;
}

END_NAMESPACE_CPT

#define __cpt_BRep3_ipp__
#include "BRep3.ipp"
#undef __cpt_BRep3_ipp__

#define __cpt_BRep2_ipp__
#include "BRep2.ipp"
#undef __cpt_BRep2_ipp__

#endif
