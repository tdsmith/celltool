// -*- C++ -*-

#if !defined(__Grid1_ipp__)
#error This file is an implementation detail of the class Grid
#endif

BEGIN_NAMESPACE_CPT

//CONTINUE: temporarily disabled.
#if 0
//! A class to hold the 1-D grid data.
template<typename T>
class Grid<1,T> : 
  public GridBase<1,T> {
private:

  //
  // Private types.
  //

  typedef GridBase<1,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef typename Base::Number Number;
  //! A point in N-D.
  typedef typename Base::Point Point;
  //! A bounding box in N-D.
  typedef typename Base::BBox BBox;
  //! A multi-index in N-D.
  typedef typename Base::Index Index;
  
public:

  //
  // Using.
  //

  // Accessors.

  //! Return the grid getExtents.
  using Base::getExtents;
  //! Return the grid index getRanges.
  using Base::getRanges;
  //! Return true if the grids are empty.
  using Base::isEmpty;
  //! Return a const reference to the distance grid.
  using Base::getDistance;
  //! Return a const reference to the gradient of the distance grid.
  using Base::getGradientOfDistance;
  //! Return a const reference to the closest point grid.
  using Base::getClosestPoint;
  //! Return a const reference to the closest face grid.
  using Base::getClosestFace;
  //! Is the gradient of the distance being computed?
  using Base::isGradientOfDistanceBeingComputed;
  //! Is the closest point being computed?
  using Base::isClosestPointBeingComputed;
  //! Is the closest face being computed?
  using Base::isClosestFaceBeingComputed;

  // Manipulators names are already brought in with the accessors.

  // Return a reference to the distance grid.
  //using Base::getDistance;
  // Return a reference to the gradient of the distance grid.
  //using Base::getGradientOfDistance;
  // Return a reference to the closest point grid.
  //using Base::getClosestPoint;
  // Return a reference to the closest face grid.
  //using Base::getClosestFace;

  // Mathematical operations.

  //! Initialize the grids.
  using Base::initialize;
  //! Flood fill the unsigned distance.
  using Base::floodFillUnsigned;

  // File I/O.

  using Base::put;
  using Base::displayInformation;
  using Base::countKnownDistances;
  using Base::computeMinimumAndMaximimDistances;

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.  Uninitialized memory.
  Grid() : 
    Base()
  {}

  //! Copy constructor.
  Grid(const Grid& other) : 
    Base(other)
  {}

  //! Construct from grid information.
  template<bool A1, bool A2, bool A3, bool A4>
  Grid(const BBox& domain,
       ads::Array<1,Number,A1>* distance,
       ads::Array<1,Point,A2>* gradientOfDistance, 
       ads::Array<1,Point,A3>* closestPoint,
       ads::Array<1,int,A4>* closestFace) :
    Base(domain, distance, gradientOfDistance, closestPoint, closestFace)
  {}

  //! Destructor.  Does not free grid memory.
  ~Grid()
  {}

  //! Assignment operator.
  Grid& 
  operator=(const Grid& other) {
    Base::operator=(other);
    return *this;
  }
  
  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{

  //! Calculate the distance and closest point for the grid points in \c cs.
  std::pair<int,int>
  computeClosestPointTransform(const Face<1,T>& face, 
			       Number maximumDistance);

  //! Calculate the unsigned distance and closest point for the grid points in \c cs.
  std::pair<int,int>
  computeClosestPointTransformUnsigned(const Face<1,T>& face, 
				       Number maximumDistance);

  //! Return true if the signed distance, closest point, etc. are valid.
  bool 
  isValid(Number maximumDistance, int maximumFaceIdentifier,
	  int maximumReportedErrors = 1000) const;

  //! Return true if the unsigned distance, closest point, etc. are valid.
  bool 
  isValidUnsigned(Number maximumDistance, 
		  int maximumFaceIdentifier,
		  int maximumReportedErrors = 1000) const;

  //! Flood fill the grids.
  /*!
    If there are any points with known distance then return true and set 
    the unknown distances to +- farAway.  Otherwise set all the distances 
    to + farAway and return false.
  */
  bool 
  floodFill(Number farAway);

  //@}
};
#endif






//
// Mathematical operations
//

// CONTINUE
#if 0
// Calculate the distance and closest Point for the Grid Points in cs.
template<typename T>
inline
std::pair<int,int>
Grid<1,T>::
computeClosestPointTransform(const Face<1,T>& face, 
			     const Number maximumDistance) {
  int numberOfDistancesSet = 0;

  //
  // Determine the index range.
  //
  // Convert the Cartesian location to an continuous index.
  Point leftPoint(face.domain().getLowerCorner());
  Point rightPoint(face.domain().getUpperCorner());
  convertLocationToIndex(&leftPoint);
  convertLocationToIndex(&rightPoint);
  // Enlarge the range so we don't miss any grid points.
  const Number left = leftPoint[0] - index_epsilon();
  const Number right = rightPoint[0] + index_epsilon();
  // Convert the continuous index to an integer index.
  const int lbound = std::max(0, int(left) + 1);
  const int ubound = std::min(getExtents()[0] - 1, int(right));

  Point cartesianPoint;
  Number dist;
  if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed() && 
      isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
	getGradientgetDistance()(i) = face.getNormal();
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed() && 
	   isGradientOfDistanceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
	getGradientgetDistance()(i) = face.getNormal();
      }
    }
  }
  else if (isClosestPointBeingComputed() && isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed() && isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getGradientgetDistance()(i) = face.getNormal();
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getGradientgetDistance()(i) = face.getNormal();
      }
    }
  }
  else if (isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistance(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (std::abs(dist) <= maximumDistance &&
	  std::abs(dist) < std::abs(getDistance()(i))) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
      }
    }
  }

  return std::pair<int,int>(ubound - lbound + 1, numberOfDistancesSet);
}
#endif


// CONTINUE
#if 0
// Calculate the distance and closest Point for the Grid Points in cs.
template<typename T>
inline
std::pair<int,int>
Grid<1,T>::
computeClosestPointTransformUnsigned(const Face<1,T>& face, 
				     const Number maximumDistance) {
  int numberOfDistancesSet = 0;

  //
  // Determine the index range.
  //
  // Convert the Cartesian location to an continuous index.
  Point leftPoint(face.domain().getLowerCorner());
  Point rightPoint(face.domain().getUpperCorner());
  convertLocationToIndex(&leftPoint);
  convertLocationToIndex(&rightPoint);
  // Enlarge the range so we don't miss any grid points.
  const Number left = leftPoint[0] - index_epsilon();
  const Number right = rightPoint[0] + index_epsilon();
  // Convert the continuous index to an integer index.
  const int lbound = std::max(0, int(left) + 1);
  const int ubound = std::min(getExtents()[0] - 1, int(right));

  Point cartesianPoint;
  Number dist;
  if (isClosestPointBeingComputed() && isGradientOfDistanceBeingComputed() && 
      isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
	if (cartesianPoint[0] >= face.getLocation()) {
	  getGradientgetDistance()(i) = face.getNormal();
	}
	else {
	  getGradientgetDistance()(i) = - face.getNormal();
	}
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed() && 
	   isGradientOfDistanceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
	if (cartesianPoint[0] >= face.getLocation()) {
	  getGradientgetDistance()(i) = face.getNormal();
	}
	else {
	  getGradientgetDistance()(i) = - face.getNormal();
	}
      }
    }
  }
  else if (isClosestPointBeingComputed() && isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed() && isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	if (cartesianPoint[0] >= face.getLocation()) {
	  getGradientgetDistance()(i) = face.getNormal();
	}
	else {
	  getGradientgetDistance()(i) = - face.getNormal();
	}
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else if (isClosestPointBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestPoint()(i) = face.getLocation();
      }
    }
  }
  else if (isGradientOfDistanceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	if (cartesianPoint[0] >= face.getLocation()) {
	  getGradientgetDistance()(i) = face.getNormal();
	}
	else {
	  getGradientgetDistance()(i) = - face.getNormal();
	}
      }
    }
  }
  else if (isClosestFaceBeingComputed()) {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
	getClosestFace()(i) = face.getFaceIndex();
      }
    }
  }
  else {
    // For each grid point in the index range.
    for (int i = lbound; i <= ubound; ++i) {
      // Compute the distance.
      cartesianPoint[0] = i;
      convertIndexToLocation(&cartesianPoint);
      dist = face.computeDistanceUnsigned(cartesianPoint[0]);
      // If the new distance is less than the old distance.
      // I have to check that is no greater than maximumDistance because I 
      // enlarged the domain.
      if (dist <= maximumDistance && dist < getDistance()(i)) {
	if (getDistance()(i) == std::numeric_limits<Number>::max()) {
	  ++numberOfDistancesSet;
	}
	getDistance()(i) = dist;
      }
    }
  }

  return std::pair<int,int>(ubound - lbound + 1, numberOfDistancesSet);
}
#endif









// CONTINUE
#if 0
template<typename T>
inline
bool 
Grid<1,T>::
isValid(const Number maximumDistance, const int maximumFaceIdentifier,
	const int maximumReportedErrors) const {
  bool result = true;
  int numberOfErrors = 0;

  const Point HugePoint(std::numeric_limits<Number>::max());

  // 
  // Check the distance grid.
  //
  const int gridSizeX = getExtents()[0];
  
  Number d;
  int i;
  for (i = 0; i != gridSizeX; ++i) {
    d =  getDistance()(i);
    if (! (d == std::numeric_limits<Number>::max() || 
	   std::abs(d) <= maximumDistance)) {
      std::cerr << "In Grid::isValid():\n"
		<< "    Bad distance value.\n"
		<< "    d = " << d << '\n'
		<< "    (i) = " << i << '\n';
      result = false;
      if (++numberOfErrors >= maximumReportedErrors) {
	std::cerr << "Maximum number of errors exceeded.\n";
	return false;
      }
    }
  }

  // Check the numerical derivative of distance.
  Number d1, d2;
  const Number deltaX = lattice.delta()[0] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = 0; i != gridSizeX - 1; ++i) {
    d1 =  getDistance()(i);
    d2 =  getDistance()(i+1);
    if (d1 != std::numeric_limits<Number>::max() && 
	d2 != std::numeric_limits<Number>::max()) {
      if (std::abs(d1 - d2) > deltaX) {
	std::cerr << "In Grid::isValid():\n" 
		  << "    Bad distance difference in x direction.\n" 
		  << "    d1 = " << d1 << "  d2 = " << d2 << '\n'
		  << "    std::abs(d1-d2) = " << std::abs(d1-d2) << '\n'
		  << "    (i) = " << i << '\n'
		  << "    deltaX = " << deltaX << '\n';
	result = false;
	if (++numberOfErrors >= maximumReportedErrors) {
	  std::cerr << "Maximum number of errors exceeded.\n";
	  return false;
	}
      }
    }
  }
  
  //
  // Check the closest face grid.
  //
  
  // If the closest face is being computed.
  if (isClosestFaceBeingComputed()) {
    Point cp;
    
    int face;
    for (i = 0; i != gridSizeX; ++i) {

      face = getClosestFace()(i);
      if (face < -1 || face > maximumFaceIdentifier) {
	std::cerr << "In Grid::isValid():\n"
		  << "    Bad closest face value.\n"
		  << "i = " << i  << '\n'
		  << "face = " << face << '\n'
		  << "maximumFaceIdentifier = " << maximumFaceIdentifier 
		  << '\n';
	result = false;
	if (++numberOfErrors >= maximumReportedErrors) {
	  std::cerr << "Maximum number of errors exceeded.\n";
	  return false;
	}
      }

      d =  getDistance()(i);
      if (face == -1) {
	if (d != std::numeric_limits<Number>::max()) {
	  std::cerr << "In Grid::isValid():\n"
		    << "    Face is -1 but distance is not huge.\n" 
		    << "    (i) = " << i << '\n'
		    << "distance = " << d << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestPointBeingComputed()) {
	  cp = getClosestPoint()(i);
	  if (cp != HugePoint) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Face is -1 but closest point is not huge.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // if (face == -1)
      else { // 0 <= face <= maximumFaceIdentifier
	if (std::abs(d) > maximumDistance) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Face is known, distance is too big.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestPointBeingComputed()) {
	  cp = getClosestPoint()(i);
	  if (cp == HugePoint) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Face is known but closest point is huge.\n"
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // else
    } // for i
  } // if (isClosestFaceBeingComputed())

  //
  // Check the closest point grid.
  //

  // If the closest point is being computed.
  if (isClosestPointBeingComputed()) {
    Point cp;
    int face;
    
    for (i = 0; i != gridSizeX; ++i) {

      cp = getClosestPoint()(i);
      d =  getDistance()(i);
      if (cp == HugePoint) {
	if (d != std::numeric_limits<Number>::max()) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Closest pt is huge, distance is not huge.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face != -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Closest pt is huge but face != -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // if (cp == HugePoint)
      else {
	if (std::abs(d) > maximumDistance) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Closest pt is known, distance is too big.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face == -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Closest pt is known but face == -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // else
    } // for i
  } // if (isClosestPointBeingComputed())

  //
  // Check the gradient of the distance grid.
  //
  
  // If the gradient of the distance is being computed.
  if (isGradientOfDistanceBeingComputed()) {
    Point gd;
    int face;
    
    for (i = 0; i < gridSizeX; ++i) {

      gd = getGradientgetDistance()(i);
      d =  getDistance()(i);
      if (gd == HugePoint) {
	if (d != std::numeric_limits<Number>::max()) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Grad dist is huge but distance is not huge.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face != -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Grad dist is huge but face != -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // if (gd == HugePoint)
      else {
	if (std::abs(d) > maximumDistance) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Grad dist is known, distance is too big.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face == -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Grad dist is known but face == -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
	// Check the magnitude of the gradient of the distance.
	if (std::abs(gd[0]) - 1.0 > 
	    10 * std::numeric_limits<Number>::epsilon()) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Magnitude of gradient, " << std::abs(gd[0])
		    << ", is not unity.\n"
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}
      } // else
    } // for i
  } // if (isGradientOfDistanceBeingComputed())
  
  return result;
}
#endif




// CONTINUE
#if 0
template<typename T>
inline
bool 
Grid<1,T>::
isValidUnsigned(const Number maximumDistance, 
		const int maximumFaceIdentifier,
		const int maximumReportedErrors) const {
  bool result = true;
  int numberOfErrors = 0;

  const Point HugePoint(std::numeric_limits<Number>::max());

  // 
  // Check the distance grid.
  //
  const int gridSizeX = getExtents()[0];
  
  Number d;
  int i;
  for (i = 0; i != gridSizeX; ++i) {
    d =  getDistance()(i);
    if (! (d == std::numeric_limits<Number>::max() || 
	   (0 <= d && d <= maximumDistance))) {
      std::cerr << "In Grid::isValid():\n"
		<< "    Bad distance value.\n"
		<< "    d = " << d << '\n'
		<< "    (i) = " << i << '\n';
      result = false;
      if (++numberOfErrors >= maximumReportedErrors) {
	std::cerr << "Maximum number of errors exceeded.\n";
	return false;
      }
    }
  }

  // Check the numerical derivative of distance.
  Number d1, d2;
  const Number deltaX = lattice.delta()[0] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = 0; i != gridSizeX - 1; ++i) {
    d1 =  getDistance()(i);
    d2 =  getDistance()(i+1);
    if (d1 != std::numeric_limits<Number>::max() && 
	d2 != std::numeric_limits<Number>::max()) {
      if (std::abs(d1 - d2) > deltaX) {
	std::cerr << "In Grid::isValid():\n" 
		  << "    Bad distance difference in x direction.\n" 
		  << "    d1 = " << d1 << "  d2 = " << d2 << '\n'
		  << "    std::abs(d1-d2) = " << std::abs(d1-d2) << '\n'
		  << "    (i) = " << i << '\n'
		  << "    deltaX = " << deltaX << '\n';
	result = false;
	if (++numberOfErrors >= maximumReportedErrors) {
	  std::cerr << "Maximum number of errors exceeded.\n";
	  return false;
	}
      }
    }
  }
  
  //
  // Check the closest face grid.
  //
  
  // If the closest face is being computed.
  if (isClosestFaceBeingComputed()) {
    Point cp;
    
    int face;
    for (i = 0; i != gridSizeX; ++i) {

      face = getClosestFace()(i);
      if (face < -1 || face > maximumFaceIdentifier) {
	std::cerr << "In Grid::isValid():\n"
		  << "    Bad closest face value.\n"
		  << "i = " << i  << '\n'
		  << "face = " << face << '\n'
		  << "maximumFaceIdentifier = " << maximumFaceIdentifier 
		  << '\n';
	result = false;
	if (++numberOfErrors >= maximumReportedErrors) {
	  std::cerr << "Maximum number of errors exceeded.\n";
	  return false;
	}
      }

      d =  getDistance()(i);
      if (face == -1) {
	if (d != std::numeric_limits<Number>::max()) {
	  std::cerr << "In Grid::isValid():\n"
		    << "    Face is -1 but distance is not huge.\n" 
		    << "    (i) = " << i << '\n'
		    << "distance = " << d << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestPointBeingComputed()) {
	  cp = getClosestPoint()(i);
	  if (cp != HugePoint) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Face is -1 but closest point is not huge.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // if (face == -1)
      else { // 0 <= face <= maximumFaceIdentifier
	if (d < 0 || d > maximumDistance) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Face is known, distance is out of range.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestPointBeingComputed()) {
	  cp = getClosestPoint()(i);
	  if (cp == HugePoint) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Face is known but closest point is huge.\n"
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // else
    } // for i
  } // if (isClosestFaceBeingComputed())

  //
  // Check the closest point grid.
  //

  // If the closest point is being computed.
  if (isClosestPointBeingComputed()) {
    Point cp;
    int face;
    
    for (i = 0; i != gridSizeX; ++i) {

      cp = getClosestPoint()(i);
      d =  getDistance()(i);
      if (cp == HugePoint) {
	if (d != std::numeric_limits<Number>::max()) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Closest pt is huge, distance is not huge.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face != -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Closest pt is huge but face != -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // if (cp == HugePoint)
      else {
	if (d < 0 || d > maximumDistance) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Closest pt is known, distance is out of range.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face == -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Closest pt is known but face == -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // else
    } // for i
  } // if (isClosestPointBeingComputed())

  //
  // Check the gradient of the distance grid.
  //
  
  // If the gradient of the distance is being computed.
  if (isGradientOfDistanceBeingComputed()) {
    Point gd;
    int face;
    
    for (i = 0; i < gridSizeX; ++i) {

      gd = getGradientgetDistance()(i);
      d =  getDistance()(i);
      if (gd == HugePoint) {
	if (d != std::numeric_limits<Number>::max()) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Grad dist is huge but distance is not huge.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face != -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Grad dist is huge but face != -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
      } // if (gd == HugePoint)
      else {
	if (d < 0 || d > maximumDistance) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Grad dist is known, distance is out of range.\n" 
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}

	if (isClosestFaceBeingComputed()) {
	  face = getClosestFace()(i);
	  if (face == -1) {
	    std::cerr << "In Grid::isValid():\n" 
		      << "    Grad dist is known but face == -1.\n" 
		      << "    (i) = " << i << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded.\n";
	      return false;
	    }
	  }
	}
	// Check the magnitude of the gradient of the distance.
	if (std::abs(gd[0]) - 1.0 > 
	    10 * std::numeric_limits<Number>::epsilon()) {
	  std::cerr << "In Grid::isValid():\n" 
		    << "    Magnitude of gradient, " << std::abs(gd[0])
		    << ", is not unity.\n"
		    << "    (i) = " << i << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded.\n";
	    return false;
	  }
	}
      } // else
    } // for i
  } // if (isGradientOfDistanceBeingComputed())
  
  return result;
}
#endif




// CONTINUE
#if 0
template<typename T>
inline
bool 
Grid<1,T>::
floodFill(const Number farAway) {
  //
  // See if the distance is known for any grid points
  //
  bool result = false;
  int sign = 0;
  int i;
  const int extent = getDistance().extent(0);
  for (i = 0; !result && i != extent; ++i) {
    if (getDistance()(i) != std::numeric_limits<Number>::max()) {
      result = true;
      sign = (getDistance()(i) > 0) ? 1 : -1;
    }
  }

  //
  // If there are any points in a known distance.
  //
  if (result) {
    for (i = 0; i != extent; ++i) {
      if (getDistance()(i) != std::numeric_limits<Number>::max()) {
	sign = (getDistance()(i) > 0) ? 1 : -1;
      }
      else {
	// Set the distance to +- farAway.
	getDistance()(i) = sign * farAway;
      }
    }
  } // end if (result)
  else {
    // Set the distance to +farAway.
    getDistance() = farAway;
  }
  
  return result;
}
#endif

END_NAMESPACE_CPT
