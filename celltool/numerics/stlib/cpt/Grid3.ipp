// -*- C++ -*-

#if !defined(__Grid3_ipp__)
#error This file is an implementation detail of the class Grid
#endif

BEGIN_NAMESPACE_CPT

//! A class to hold the 3-D grid data.
template<typename T>
class Grid<3,T> : 
  public GridBase<3,T> {
  //
  // Private types.
  //

private:

  typedef GridBase<3,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef typename Base::Number Number;
  //! A point in 3-D.
  typedef typename Base::Point Point;
  //! A multi-index in 3-D.
  typedef typename Base::Index Index;
  //! A multi-index range in 3-D.
  typedef typename Base::Range Range;
  //! A lattice.
  typedef typename Base::Lattice Lattice;
  
public:

  //
  // Using.
  //

  // Accessors.

  //! Return the grid extents.
  using Base::getExtents;
  //! Return the grid index ranges.
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
  using Base::computeMinimumAndMaximumDistances;

  // Mathematical operations.

  using Base::computeClosestPointTransform;
  using Base::computeClosestPointTransformUnsigned;

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.  Empty arrays.
  Grid() : 
    Base()
  {}

  //! Copy constructor.
  Grid(const Grid& other) :
    Base(other)
  {}

  //! Construct from the grids.
  template<bool A1, bool A2, bool A3, bool A4>
  Grid(ads::Array<3,Number,A1>* distance,
	ads::Array<3,Point,A2>* gradientOfDistance, 
	ads::Array<3,Point,A3>* closestPoint,
	ads::Array<3,int,A4>* closestFace) :
    Base(distance, gradientOfDistance, closestPoint, closestFace)
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

  //! Return true if the signed distance, closest point, etc. are valid.
  bool 
  isValid(const Lattice& lattice, Number maximumDistance, 
	    int maximumFaceIdentifier, 
	    int maximumReportedErrors = 1000) const;

  //! Return true if the unsigned distance, closest point, etc. are valid.
  bool 
  isValidUnsigned(const Lattice& lattice, 
		     Number maximumDistance, 
		     int maximumFaceIdentifier,
		     int maximumReportedErrors = 1000) const;

  //! Flood fill the signed distance.
  /*!
    If there are any points with known distance then return true and set 
    the unknown distances to +- farAway.  Otherwise set all the distances 
    to + farAway and return false.
  */
  bool 
  floodFill(Number farAway);

  //@}
};



//
// Mathematical operations
//





// CONTINUE: In all the is_valid_unsigned functions, check that known distances
// that are more than one grid point away from the max distance have
// known neighbors.

template<typename T>
inline
bool 
Grid<3,T>::
isValid(const Lattice& lattice, const Number maximumDistance, 
	const int maximumFaceIdentifier, 
	const int maximumReportedErrors) const {
  bool result = true;
  int numberOfErrors = 0;

  const Point HugePoint(std::numeric_limits<Number>::max());
  const Number Eps10 = 10 * std::numeric_limits<Number>::epsilon();

  // 
  // Check the distance grid.
  //
  const int lbi = getRanges().lbound(0);
  const int lbj = getRanges().lbound(1);
  const int lbk = getRanges().lbound(2);
  const int ubi = getRanges().ubound(0);
  const int ubj = getRanges().ubound(1);
  const int ubk = getRanges().ubound(2);

  Number d;
  int i, j, k;
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj; ++j) {
      for (k = lbk; k != ubk; ++k) {
	d =  getDistance()(i, j, k);
	if (! (d == std::numeric_limits<Number>::max() || 
		 std::abs(d) <= maximumDistance)) {
	  std::cerr << "In Grid<3,T>::isValid():" << '\n'
		    << "    Bad distance value." << '\n'
		    << "    d = " << d << '\n'
		    << "    (i,j,k) = " << i << " " << j << " " << k 
		    << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded." << '\n';
	    return false;
	  }
	}
      }
    }
  }

  // Check the numerical derivative of distance in the x direction.
  Number d1, d2;
  const Number deltaX = lattice.getDelta()[0] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (j = lbj; j != ubj; ++j) {
    for (k = lbk; k != ubk; ++k) {
      for (i = lbi; i != ubi - 1; ++i) {
	d1 =  getDistance()(i, j, k);
	d2 =  getDistance()(i+1, j, k);
	if (d1 != std::numeric_limits<Number>::max() && 
	     d2 != std::numeric_limits<Number>::max()) {
	  if (std::abs(d1 - d2) > deltaX) {
	    std::cerr << "In Grid<3,T>::isValid():" 
		      << '\n'
		      << "    Bad distance difference in x direction." 
		      << '\n'
		      << "    d1 = " << d1 << "  d2 = " << d2 
		      << '\n'
		      << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		      << '\n'
		      << "    (i,j,k) = " << i << " " << j << " " << k 
		      << '\n'
		      << "    deltaX = " << deltaX 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }
	}
      }
    }
  }
  
  // Check the numerical derivative of distance in the y direction.
  const Number deltaY = lattice.getDelta()[1] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = lbi; i != ubi; ++i) {
    for (k = lbk; k != ubk; ++k) {
      for (j = lbj; j != ubj - 1; ++j) {
	d1 =  getDistance()(i, j, k);
	d2 =  getDistance()(i, j+1, k);
	if (d1 != std::numeric_limits<Number>::max() && 
	     d2 != std::numeric_limits<Number>::max()) {
	  if (std::abs(d1 - d2) > deltaY) {
	    std::cerr << "In Grid<3,T>::isValid():" 
		      << '\n'
		      << "    Bad distance difference in y direction." 
		      << '\n'
		      << "    d1 = " << d1 << "  d2 = " << d2 
		      << '\n'
		      << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		      << '\n'
		      << "    (i,j,k) = " << i << " " << j << " " << k 
		      << '\n'
		      << "    deltaY = " << deltaY 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }
	}
      }
    }
  }
  
  // Check the numerical derivative of distance in the z direction.
  const Number deltaZ = lattice.getDelta()[2] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj; ++j) {
      for (k = lbk; k != ubk - 1; ++k) {
	d1 =  getDistance()(i, j, k);
	d2 =  getDistance()(i, j, k+1);
	if (d1 != std::numeric_limits<Number>::max() && 
	     d2 != std::numeric_limits<Number>::max()) {
	  if (std::abs(d1 - d2) > deltaZ) {
	    std::cerr << "In Grid<3,T>::isValid():" 
		      << '\n'
		      << "    Bad distance difference in z direction." 
		      << '\n'
		      << "    d1 = " << d1 << "  d2 = " << d2 
		      << '\n'
		      << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		      << '\n'
		      << "    (i,j,k) = " << i << " " << j << " " << k 
		      << '\n'
		      << "    deltaZ = " << deltaZ << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }
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
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {
	for (k = lbk; k < ubk; ++k) {

	  face = getClosestFace()(i, j, k);
	  if (face < -1 || face > maximumFaceIdentifier) {
	    std::cerr << "In Grid<3,T>::isValid():" 
		      << '\n'
		      << "    Bad closest face value." 
		      << '\n'
		      << "i = " << i 
		      << '\n'
		      << "j = " << j 
		      << '\n'
		      << "k = " << k 
		      << '\n'
		      << "face = " << face 
		      << '\n'
		      << "maximumFaceIdentifier = " << maximumFaceIdentifier 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }

	  d =  getDistance()(i, j, k);
	  if (face == -1) {
	    if (d != std::numeric_limits<Number>::max()) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Face is -1 but distance is not huge." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n'
			<< "distance = " << d 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestPointBeingComputed()) {
	      cp = getClosestPoint()(i, j, k);
	      if (cp != HugePoint) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Face is -1 but closest point is not huge." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // if (face == -1)
	  else { // 0 <= face <= maximumFaceIdentifier
	    if (std::abs(d) > maximumDistance) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Face is known, distance is too big." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestPointBeingComputed()) {
	      cp = getClosestPoint()(i, j, k);
	      if (cp == HugePoint) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Face is known but closest point is huge." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // else
	} // for (k = 0; k < grid_size_z; ++k)
      } // for j
    } // for i
  } // if (isClosestFaceBeingComputed())
  
    //
    // Check the closest point grid.
    //

    // If the closest point is being computed.
  if (isClosestPointBeingComputed()) {
    Point cp;
    int face;
    
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {
	for (k = lbk; k < ubk; ++k) {

	  cp = getClosestPoint()(i, j, k);
	  d =  getDistance()(i, j, k);
	  if (cp == HugePoint) {
	    if (d != std::numeric_limits<Number>::max()) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Closest pt is huge, distance is not huge." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face != -1) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Closest pt is huge but face != -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // if (cp == HugePoint)
	  else {
	    if (std::abs(d) > maximumDistance) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Closest pt is known, distance is too big." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face == -1) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Closest pt is known but face == -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // else
	} // for (k = 0; k < grid_size_z; ++k)
      } // for j
    } // for i
  } // if (isClosestPointBeingComputed())

  //
  // Check the gradient of the distance grid.
  //
  
  // If the gradient of the distance is being computed.
  if (isGradientOfDistanceBeingComputed()) {
    Point gd, cp, position;
    int face;
    
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {
	for (k = lbk; k < ubk; ++k) {

	  gd = getGradientOfDistance()(i, j, k);
	  d =  getDistance()(i, j, k);
	  if (gd == HugePoint) {
	    if (d != std::numeric_limits<Number>::max()) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Grad dist is huge but distance is not huge." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face != -1) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Grad dist is huge but face != -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // if (gd == HugePoint)
	  else {
	    if (std::abs(d) > maximumDistance) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Grad dist is known, distance is too big." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face == -1) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Grad dist is known but face == -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	    // Check the magnitude of the gradient of the distance.
	    if (std::abs(geom::computeMagnitude(gd) - 1) > Eps10) {
	      std::cerr << "In Grid<3,T>::isValid():" 
			<< '\n'
			<< "    Magnitude of gradient, " 
			<< geom::computeMagnitude(gd)
			<< ", is not unity."
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }
	    // Check the direction of the gradient of the distance.
	    if (isClosestPointBeingComputed()) {
	      position = Point(i, j, k);
	      lattice.convertIndexToLocation(&position);
	      cp = getClosestPoint()(i, j, k);
	      if (geom::computeMagnitude
		  (geom::computeCrossProduct(Point(position - cp), gd)) > 
		  std::sqrt(std::numeric_limits<Number>::epsilon())) {
		std::cerr << "In Grid<3,T>::isValid():" 
			  << '\n'
			  << "    Direction of gradient is wrong." 
			  << '\n'
			  << "    gradient = " << gd 
			  << '\n'
			  << "    position - closest point = " 
			  << position - cp
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // else
	} // for (k = 0; k < grid_size_z; ++k)
      } // for j
    } // for i
  } // if (isGradientOfDistanceBeingComputed())
  
  return result;
}





template<typename T>
inline
bool 
Grid<3,T>::
isValidUnsigned(const Lattice& lattice, 
		   const Number maximumDistance, 
		   const int maximumFaceIdentifier, 
		   const int maximumReportedErrors) const {
  bool result = true;
  int numberOfErrors = 0;

  const Point HugePoint(std::numeric_limits<Number>::max());
  const Number Eps10 = 10 * std::numeric_limits<Number>::epsilon();

  // 
  // Check the distance grid.
  //
  const int lbi = getRanges().lbound(0);
  const int lbj = getRanges().lbound(1);
  const int lbk = getRanges().lbound(2);
  const int ubi = getRanges().ubound(0);
  const int ubj = getRanges().ubound(1);
  const int ubk = getRanges().ubound(2);
  
  Number d;
  int i, j, k;
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj; ++j) {
      for (k = lbk; k != ubk; ++k) {
	d =  getDistance()(i, j, k);
	if (! (d == std::numeric_limits<Number>::max() || 
		 (0 <= d && d <= maximumDistance))) {
	  std::cerr << "In Grid<3,T>::isValidUnsigned():" << '\n'
		    << "    Bad distance value." << '\n'
		    << "    d = " << d << '\n'
		    << "    (i,j,k) = " << i << " " << j << " " << k 
		    << '\n';
	  result = false;
	  if (++numberOfErrors >= maximumReportedErrors) {
	    std::cerr << "Maximum number of errors exceeded." << '\n';
	    return false;
	  }
	}
      }
    }
  }

  // Check the numerical derivative of distance in the x direction.
  Number d1, d2;
  const Number deltaX = lattice.getDelta()[0] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (j = lbj; j != ubj; ++j) {
    for (k = lbk; k != ubk; ++k) {
      for (i = lbi; i != ubi - 1; ++i) {
	d1 =  getDistance()(i, j, k);
	d2 =  getDistance()(i+1, j, k);
	if (d1 != std::numeric_limits<Number>::max() && 
	     d2 != std::numeric_limits<Number>::max()) {
	  if (std::abs(d1 - d2) > deltaX) {
	    std::cerr << "In Grid<3,T>::isValidUnsigned():" 
		      << '\n'
		      << "    Bad distance difference in x direction." 
		      << '\n'
		      << "    d1 = " << d1 << "  d2 = " << d2 
		      << '\n'
		      << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		      << '\n'
		      << "    (i,j,k) = " << i << " " << j << " " << k 
		      << '\n'
		      << "    deltaX = " << deltaX 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }
	}
      }
    }
  }
  
  // Check the numerical derivative of distance in the y direction.
  const Number deltaY = lattice.getDelta()[1] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = lbi; i != ubi; ++i) {
    for (k = lbk; k != ubk; ++k) {
      for (j = lbj; j != ubj - 1; ++j) {
	d1 =  getDistance()(i, j, k);
	d2 =  getDistance()(i, j+1, k);
	if (d1 != std::numeric_limits<Number>::max() && 
	     d2 != std::numeric_limits<Number>::max()) {
	  if (std::abs(d1 - d2) > deltaY) {
	    std::cerr << "In Grid<3,T>::isValidUnsigned():" 
		      << '\n'
		      << "    Bad distance difference in y direction." 
		      << '\n'
		      << "    d1 = " << d1 << "  d2 = " << d2 
		      << '\n'
		      << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		      << '\n'
		      << "    (i,j,k) = " << i << " " << j << " " << k 
		      << '\n'
		      << "    deltaY = " << deltaY 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }
	}
      }
    }
  }
  
  // Check the numerical derivative of distance in the z direction.
  const Number deltaZ = lattice.getDelta()[2] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj; ++j) {
      for (k = lbk; k != ubk - 1; ++k) {
	d1 =  getDistance()(i, j, k);
	d2 =  getDistance()(i, j, k+1);
	if (d1 != std::numeric_limits<Number>::max() && 
	     d2 != std::numeric_limits<Number>::max()) {
	  if (std::abs(d1 - d2) > deltaZ) {
	    std::cerr << "In Grid<3,T>::isValidUnsigned():" 
		      << '\n'
		      << "    Bad distance difference in z direction." 
		      << '\n'
		      << "    d1 = " << d1 << "  d2 = " << d2 
		      << '\n'
		      << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		      << '\n'
		      << "    (i,j,k) = " << i << " " << j << " " << k 
		      << '\n'
		      << "    deltaZ = " << deltaZ << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }
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
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {
	for (k = lbk; k < ubk; ++k) {

	  face = getClosestFace()(i, j, k);
	  if (face < -1 || face > maximumFaceIdentifier) {
	    std::cerr << "In Grid<3,T>::isValidUnsigned():" 
		      << '\n'
		      << "    Bad closest face value." 
		      << '\n'
		      << "i = " << i 
		      << '\n'
		      << "j = " << j 
		      << '\n'
		      << "k = " << k 
		      << '\n'
		      << "face = " << face 
		      << '\n'
		      << "maximumFaceIdentifier = " << maximumFaceIdentifier 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." << '\n';
	      return false;
	    }
	  }

	  d =  getDistance()(i, j, k);
	  if (face == -1) {
	    if (d != std::numeric_limits<Number>::max()) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Face is -1 but distance is not huge." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n'
			<< "distance = " << d 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestPointBeingComputed()) {
	      cp = getClosestPoint()(i, j, k);
	      if (cp != HugePoint) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Face is -1 but closest point is not huge." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // if (face == -1)
	  else { // 0 <= face <= maximumFaceIdentifier
	    if (d < 0 || d > maximumDistance) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Face is known, distance is out of range." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestPointBeingComputed()) {
	      cp = getClosestPoint()(i, j, k);
	      if (cp == HugePoint) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Face is known but closest point is huge." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // else
	} // for (k = 0; k < grid_size_z; ++k)
      } // for j
    } // for i
  } // if (isClosestFaceBeingComputed())
  
    //
    // Check the closest point grid.
    //

    // If the closest point is being computed.
  if (isClosestPointBeingComputed()) {
    Point cp;
    int face;
    
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {
	for (k = lbk; k < ubk; ++k) {

	  cp = getClosestPoint()(i, j, k);
	  d =  getDistance()(i, j, k);
	  if (cp == HugePoint) {
	    if (d != std::numeric_limits<Number>::max()) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Closest pt is huge, distance is not huge." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face != -1) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Closest pt is huge but face != -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // if (cp == HugePoint)
	  else {
	    if (d < 0 || d > maximumDistance) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Closest pt is known, distance is out of range."
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face == -1) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Closest pt is known but face == -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // else
	} // for (k = 0; k < grid_size_z; ++k)
      } // for j
    } // for i
  } // if (isClosestPointBeingComputed())

  //
  // Check the gradient of the distance grid.
  //
  
  // If the gradient of the distance is being computed.
  if (isGradientOfDistanceBeingComputed()) {
    Point gd, cp, position;
    int face;
    
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {
	for (k = lbk; k < ubk; ++k) {

	  gd = getGradientOfDistance()(i, j, k);
	  d =  getDistance()(i, j, k);
	  if (gd == HugePoint) {
	    if (d != std::numeric_limits<Number>::max()) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Grad dist is huge but distance is not huge." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face != -1) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Grad dist is huge but face != -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // if (gd == HugePoint)
	  else {
	    if (d < 0 || d > maximumDistance) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Grad dist is known, distance is out of range." 
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }

	    if (isClosestFaceBeingComputed()) {
	      face = getClosestFace()(i, j, k);
	      if (face == -1) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Grad dist is known but face == -1." 
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	    // Check the magnitude of the gradient of the distance.
	    if (std::abs(geom::computeMagnitude(gd) - 1) > Eps10) {
	      std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			<< '\n'
			<< "    Magnitude of gradient, " 
			<< geom::computeMagnitude(gd)
			<< ", is not unity."
			<< '\n'
			<< "    (i,j,k) = " << i << " " << j << " " << k 
			<< '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }
	    // Check the direction of the gradient of the distance.
	    if (isClosestPointBeingComputed()) {
	      position = Point(i, j, k);
	      lattice.convertIndexToLocation(&position);
	      cp = getClosestPoint()(i, j, k);
	      if (geom::computeMagnitude
		  (geom::computeCrossProduct(Point(position - cp), gd)) > 
		  std::sqrt(std::numeric_limits<Number>::epsilon())) {
		std::cerr << "In Grid<3,T>::isValidUnsigned():" 
			  << '\n'
			  << "    Direction of gradient is wrong." 
			  << '\n'
			  << "    gradient = " << gd 
			  << '\n'
			  << "    position - closest point = " 
			  << position - cp
			  << '\n'
			  << "    (i,j,k) = " << i << " " << j << " " << k 
			  << '\n';
		result = false;
		if (++numberOfErrors >= maximumReportedErrors) {
		  std::cerr << "Maximum number of errors exceeded." 
			    << '\n';
		  return false;
		}
	      }
	    }
	  } // else
	} // for (k = 0; k < grid_size_z; ++k)
      } // for j
    } // for i
  } // if (isGradientOfDistanceBeingComputed())
  
  return result;
}




//
// If there are any points with known distance then return true and set 
// the unknown distances to +- farAway.  Otherwise set all the distances 
// to + farAway and return false.
//
template<typename T>
inline
bool 
Grid<3,T>::
floodFill(const Number farAway) {
  //
  // See if the distance is known for any grid points
  //
  bool result = false;
  int sign = 0;
  typename ads::Array<3,Number,false>::const_iterator iter = 
    getDistance().begin();
  const typename ads::Array<3,Number,false>::const_iterator iterEnd = 
    getDistance().end();
  for (; !result && iter != iterEnd; ++iter) {
    if (*iter != std::numeric_limits<Number>::max()) {
      result = true;
      sign = (*iter > 0) ? 1 : -1;
    }
  }

  //
  // If there are any points in a known distance.
  //
  if (result) {
    int ySign = sign, zSign = sign;

    int i, j, k;
    const int lbi = getRanges().lbound(0);
    const int lbj = getRanges().lbound(1);
    const int lbk = getRanges().lbound(2);
    const int ubi = getRanges().ubound(0);
    const int ubj = getRanges().ubound(1);
    const int ubk = getRanges().ubound(2);

    //
    // Flood fill the distance with +- farAway.
    //
    for (k = lbk; k != ubk; ++k) {
      if (getDistance()(lbi,lbj,k) != std::numeric_limits<Number>::max()) {
	zSign = (getDistance()(lbi,lbj,k) > 0) ? 1 : -1;
      }
      ySign = zSign;
      for (j = lbj; j != ubj; ++j) {
	if (getDistance()(lbi,j,k) != std::numeric_limits<Number>::max()) {
	  ySign = (getDistance()(lbi,j,k) > 0) ? 1 : -1;
	}
	sign = ySign;
	for (i = lbi; i != ubi; ++i) {
	  if (getDistance()(i,j,k) != std::numeric_limits<Number>::max()) {
	    sign = (getDistance()(i,j,k) > 0) ? 1 : -1;
	  }
	  else {
	    // Set the distance to +- farAway.
	    getDistance()(i,j,k) = sign * farAway;
	  }
	}
      }
    }
  } // end if (result)
  else {
    // Set the distance to +farAway.
    getDistance() = farAway;
  }
  
  return result;
}

END_NAMESPACE_CPT
