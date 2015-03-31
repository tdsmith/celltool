// -*- C++ -*-

#if !defined(__Grid2_ipp__)
#error This file is an implementation detail of the class Grid
#endif

BEGIN_NAMESPACE_CPT

//! A class to hold the 2-D grid data.
template<typename T>
class Grid<2,T> : 
  public GridBase<2,T> {
  //
  // Private types.
  //

private:

  typedef GridBase<2,T> Base;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename Base::Number Number;
  //! A point in 2-D.
  typedef typename Base::Point Point;
  //! A multi-index in 2-D.
  typedef typename Base::Index Index;
  //! A multi-index range in 2-D.
  typedef typename Base::Range Range;
  //! A lattice.
  typedef typename Base::Lattice Lattice;

  // CONTINUE: remove these when the closest point functions are templated.
  //! A vertex in the b-rep.
  typedef Vertex<2,Number> Vertex;
  //! A face in the b-rep.
  typedef Face<2,Number> Face;
  
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
  Grid(ads::Array<2,Number,A1>* distance,
       ads::Array<2,Point,A2>* gradientOfDistance, 
       ads::Array<2,Point,A3>* closestPoint,
       ads::Array<2,int,A4>* closestFace) :
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
  isValid(const Lattice& lattice, 
	  Number maximumDistance, int maximumFaceIdentifier,
	  int maximumReportedErrors = 100) const;

  //! Return true if the unsigned distance, closest point, etc. are valid.
  bool 
  isValidUnsigned(const Lattice& lattice, 
		  Number maximumDistance, 
		  int maximumFaceIdentifier,
		  int maximumReportedErrors = 100) const;

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



template<typename T>
inline
bool 
Grid<2,T>::
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
  const int ubi = getRanges().ubound(0);
  const int ubj = getRanges().ubound(1);

  Number d;
  int i, j;
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj; ++j) {
      d =  getDistance()(i, j);
      if (! (d == std::numeric_limits<Number>::max() || 
	     std::abs(d) <= maximumDistance)) {
	std::cerr << "In Grid<2,T>::isValid():" << '\n'
		  << "    Bad distance value." << '\n'
		  << "    d = " << d << '\n'
		  << "    (i,j) = " << i << " " << j << '\n';
	result = false;
	if (++numberOfErrors >= maximumReportedErrors) {
	  std::cerr << "Maximum number of errors exceeded." << '\n';
	  return false;
	}
      }
    }
  }

  // Check the numerical derivative of distance in the x direction.
  Number d1, d2;
  const Number deltaX = lattice.getDelta()[0] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (j = lbj; j != ubj; ++j) {
    for (i = lbi; i != ubi - 1; ++i) {
      d1 =  getDistance()(i, j);
      d2 =  getDistance()(i+1, j);
      if (d1 != std::numeric_limits<Number>::max() && 
	  d2 != std::numeric_limits<Number>::max()) {
	if (std::abs(d1 - d2) > deltaX) {
	  std::cerr << "In Grid<2,T>::isValid():" 
		    << '\n'
		    << "    Bad distance difference in x direction." 
		    << '\n'
		    << "    d1 = " << d1 << "  d2 = " << d2 
		    << '\n'
		    << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		    << '\n'
		    << "    (i,j) = " << i << " " << j 
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
  
  // Check the numerical derivative of distance in the y direction.
  const Number deltaY = lattice.getDelta()[1] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj - 1; ++j) {
      d1 =  getDistance()(i, j);
      d2 =  getDistance()(i, j+1);
      if (d1 != std::numeric_limits<Number>::max() && 
	  d2 != std::numeric_limits<Number>::max()) {
	if (std::abs(d1 - d2) > deltaY) {
	  std::cerr << "In Grid::isValid():" 
		    << '\n'
		    << "    Bad distance difference in y direction." 
		    << '\n'
		    << "    d1 = " << d1 << "  d2 = " << d2 
		    << '\n'
		    << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		    << '\n'
		    << "    (i,j) = " << i << " " << j 
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

  //
  // Check the closest face grid.
  //
  
  // If the closest face is being computed.
  if (isClosestFaceBeingComputed()) {
    Point cp;
    
    int face;
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {

	face = getClosestFace()(i, j);
	if (face < -1 || face > maximumFaceIdentifier) {
	  std::cerr << "In Grid<2,T>::isValid():" 
		    << '\n'
		    << "    Bad closest face value." 
		    << '\n'
		    << "i = " << i 
		    << '\n'
		    << "j = " << j 
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

	d =  getDistance()(i, j);
	if (face == -1) {
	  if (d != std::numeric_limits<Number>::max()) {
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Face is -1 but distance is not huge." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
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
	    cp = getClosestPoint()(i, j);
	    if (cp != HugePoint) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Face is -1 but closest point is not huge." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Face is known, distance is too big." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestPointBeingComputed()) {
	    cp = getClosestPoint()(i, j);
	    if (cp == HugePoint) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Face is known but closest point is huge." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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

	cp = getClosestPoint()(i, j);
	d =  getDistance()(i, j);
	if (cp == HugePoint) {
	  if (d != std::numeric_limits<Number>::max()) {
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Closest pt is huge, distance is not huge." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face != -1) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Closest pt is huge but face != -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Closest pt is known, distance is too big." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face == -1) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Closest pt is known but face == -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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

	gd = getGradientOfDistance()(i, j);
	d =  getDistance()(i, j);
	if (gd == HugePoint) {
	  if (d != std::numeric_limits<Number>::max()) {
	    std::cerr << "In Grid<2,T>::isValid():" 
		      << '\n'
		      << "    Grad dist is huge but distance is not huge." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face != -1) {
	      std::cerr << "In Grid<2,T>::isValid():" 
			<< '\n'
			<< "    Grad dist is huge but face != -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j << '\n';
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
	    std::cerr << "In Grid<2,T>::isValid():" 
		      << '\n'
		      << "    Grad dist is known, distance is too big." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face == -1) {
	      std::cerr << "In Grid<2,T>::isValid():" 
			<< '\n'
			<< "    Grad dist is known but face == -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j << '\n';
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
	    std::cerr << "In Grid<2,T>::isValid():" 
		      << '\n'
		      << "    Magnitude of gradient, " 
		      << geom::computeMagnitude(gd)
		      << ", is not unity."
		      << '\n'
		      << "    (i,j) = " << i << " " << j << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }
	  // Check the direction of the gradient of the distance.
	  if (isClosestPointBeingComputed()) {
	    position = Point(i, j);
	    lattice.convertIndexToLocation(&position);
	    cp = getClosestPoint()(i, j);
	    if (std::abs(geom::computeDiscriminant(Point(position - cp), gd)) >
		std::sqrt(std::numeric_limits<Number>::epsilon())) {
	      std::cerr << "In Grid<2,T>::isValid():" 
			<< '\n'
			<< "    Direction of gradient is wrong." 
			<< '\n'
			<< "    gradient = " << gd 
			<< '\n'
			<< "    position - closest point = " 
			<< position - cp
			<< '\n'
			<< "    (i,j) = " << i << " " << j << '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }
	  }
	} // else
      } // for j
    } // for i
  } // if (isGradientOfDistanceBeingComputed())
  
  return result;
}


template<typename T>
inline
bool 
Grid<2,T>::
isValidUnsigned(const Lattice& lattice, 
		const Number maximumDistance, 
		const int maximumFaceIdentifier,
		const int maximumReportedErrors) const
{
  bool result = true;
  int numberOfErrors = 0;

  const Point HugePoint(std::numeric_limits<Number>::max());
  const Number Eps10 = 10 * std::numeric_limits<Number>::epsilon();

  // 
  // Check the distance grid.
  //
  const int lbi = getRanges().lbound(0);
  const int lbj = getRanges().lbound(1);
  const int ubi = getRanges().ubound(0);
  const int ubj = getRanges().ubound(1);
  
  Number d;
  int i, j;
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj; ++j) {
      d =  getDistance()(i, j);
      if (! (d == std::numeric_limits<Number>::max() || 
	     (0 <= d && d <= maximumDistance))) {
	std::cerr << "In Grid::isValid():" << '\n'
		  << "    Bad distance value." << '\n'
		  << "    d = " << d << '\n'
		  << "    (i,j) = " << i << " " << j << '\n';
	result = false;
	if (++numberOfErrors >= maximumReportedErrors) {
	  std::cerr << "Maximum number of errors exceeded." << '\n';
	  return false;
	}
      }
    }
  }

  // Check the numerical derivative of distance in the x direction.
  Number d1, d2;
  const Number deltaX = lattice.getDelta()[0] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (j = lbj; j != ubj; ++j) {
    for (i = lbi; i != ubi - 1; ++i) {
      d1 =  getDistance()(i, j);
      d2 =  getDistance()(i+1, j);
      if (d1 != std::numeric_limits<Number>::max() && 
	  d2 != std::numeric_limits<Number>::max()) {
	if (std::abs(d1 - d2) > deltaX) {
	  std::cerr << "In Grid::isValid():" 
		    << '\n'
		    << "    Bad distance difference in x direction." 
		    << '\n'
		    << "    d1 = " << d1 << "  d2 = " << d2 
		    << '\n'
		    << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		    << '\n'
		    << "    (i,j) = " << i << " " << j 
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
  
  // Check the numerical derivative of distance in the y direction.
  const Number deltaY = lattice.getDelta()[1] * 
    (1 + std::sqrt(std::numeric_limits<Number>::epsilon()));
  for (i = lbi; i != ubi; ++i) {
    for (j = lbj; j != ubj - 1; ++j) {
      d1 =  getDistance()(i, j);
      d2 =  getDistance()(i, j+1);
      if (d1 != std::numeric_limits<Number>::max() && 
	  d2 != std::numeric_limits<Number>::max()) {
	if (std::abs(d1 - d2) > deltaY) {
	  std::cerr << "In Grid::isValid():" 
		    << '\n'
		    << "    Bad distance difference in y direction." 
		    << '\n'
		    << "    d1 = " << d1 << "  d2 = " << d2 
		    << '\n'
		    << "    std::abs(d1 - d2) = " << std::abs(d1 - d2) 
		    << '\n'
		    << "    (i,j) = " << i << " " << j 
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

  //
  // Check the closest face grid.
  //
  
  // If the closest face is being computed.
  if (isClosestFaceBeingComputed()) {
    Point cp;
    
    int face;
    for (i = lbi; i < ubi; ++i) {
      for (j = lbj; j < ubj; ++j) {

	face = getClosestFace()(i, j);
	if (face < -1 || face > maximumFaceIdentifier) {
	  std::cerr << "In Grid::isValid():" 
		    << '\n'
		    << "    Bad closest face value." 
		    << '\n'
		    << "i = " << i 
		    << '\n'
		    << "j = " << j 
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

	d =  getDistance()(i, j);
	if (face == -1) {
	  if (d != std::numeric_limits<Number>::max()) {
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Face is -1 but distance is not huge." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
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
	    cp = getClosestPoint()(i, j);
	    if (cp != HugePoint) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Face is -1 but closest point is not huge." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Face is known, distance is out of range." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestPointBeingComputed()) {
	    cp = getClosestPoint()(i, j);
	    if (cp == HugePoint) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Face is known but closest point is huge." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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

	cp = getClosestPoint()(i, j);
	d =  getDistance()(i, j);
	if (cp == HugePoint) {
	  if (d != std::numeric_limits<Number>::max()) {
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Closest pt is huge, distance is not huge." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face != -1) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Closest pt is huge but face != -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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
	    std::cerr << "In Grid::isValid():" 
		      << '\n'
		      << "    Closest pt is known, distance is out of range." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j 
		      << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face == -1) {
	      std::cerr << "In Grid::isValid():" 
			<< '\n'
			<< "    Closest pt is known but face == -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j 
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

	gd = getGradientOfDistance()(i, j);
	d =  getDistance()(i, j);
	if (gd == HugePoint) {
	  if (d != std::numeric_limits<Number>::max()) {
	    std::cerr << "In Grid<2,T>::isValid():" 
		      << '\n'
		      << "    Grad dist is huge but distance is not huge." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face != -1) {
	      std::cerr << "In Grid<2,T>::isValid():" 
			<< '\n'
			<< "    Grad dist is huge but face != -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j << '\n';
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
	    std::cerr << "In Grid<2,T>::isValid():" 
		      << '\n'
		      << "    Grad dist is known, distance is out of range." 
		      << '\n'
		      << "    (i,j) = " << i << " " << j << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }

	  if (isClosestFaceBeingComputed()) {
	    face = getClosestFace()(i, j);
	    if (face == -1) {
	      std::cerr << "In Grid<2,T>::isValid():" 
			<< '\n'
			<< "    Grad dist is known but face == -1." 
			<< '\n'
			<< "    (i,j) = " << i << " " << j << '\n';
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
	    std::cerr << "In Grid<2,T>::isValid():" 
		      << '\n'
		      << "    Magnitude of gradient, " 
		      << geom::computeMagnitude(gd)
		      << ", is not unity."
		      << '\n'
		      << "    (i,j) = " << i << " " << j << '\n';
	    result = false;
	    if (++numberOfErrors >= maximumReportedErrors) {
	      std::cerr << "Maximum number of errors exceeded." 
			<< '\n';
	      return false;
	    }
	  }
	  // Check the direction of the gradient of the distance.
	  if (isClosestPointBeingComputed()) {
	    position = Point(i, j);
	    lattice.convertIndexToLocation(&position);
	    cp = getClosestPoint()(i, j);
	    if (std::abs(geom::computeDiscriminant(Point(position - cp), gd)) >
		std::sqrt(std::numeric_limits<Number>::epsilon())) {
	      std::cerr << "In Grid<2,T>::isValid():" 
			<< '\n'
			<< "    Direction of gradient is wrong." 
			<< '\n'
			<< "    gradient = " << gd 
			<< '\n'
			<< "    position - closest point = " 
			<< position - cp
			<< '\n'
			<< "    (i,j) = " << i << " " << j << '\n';
	      result = false;
	      if (++numberOfErrors >= maximumReportedErrors) {
		std::cerr << "Maximum number of errors exceeded." 
			  << '\n';
		return false;
	      }
	    }
	  }
	} // else
      } // for j
    } // for i
  } // if (isGradientOfDistanceBeingComputed())
  
  return result;
}



template<typename T>
inline
bool 
Grid<2,T>::
floodFill(const Number farAway) {
  //
  // See if the distance is known for any grid points
  //
  bool result = false;
  int sign = 0;
  typename ads::Array<2,Number,false>::const_iterator iter = 
    getDistance().begin();
  const typename ads::Array<2,Number,false>::const_iterator iter_end = 
    getDistance().end();
  for (; !result && iter != iter_end; ++iter) {
    if (*iter != std::numeric_limits<Number>::max()) {
      result = true;
      sign = (*iter > 0) ? 1 : -1;
    }
  }

  //
  // If there are any points in a known distance.
  //
  if (result) {
    int ySign = sign;

    int i, j;
    const int lbi = getRanges().lbound(0);
    const int lbj = getRanges().lbound(1);
    const int ubi = getRanges().ubound(0);
    const int ubj = getRanges().ubound(1);

    //
    // Flood fill the distance with +- farAway.
    //
    for (j = lbj; j != ubj; ++j) {
      if (getDistance()(lbi, j) != std::numeric_limits<Number>::max()) {
	ySign = (getDistance()(lbi, j) > 0) ? 1 : -1;
      } 
      sign = ySign;
      for (i = lbi; i != ubi; ++i) {
	if (getDistance()(i, j) != std::numeric_limits<Number>::max()) {
	  sign = (getDistance()(i, j) > 0) ? 1 : -1;
	}
	else {
	  // Set the distance to +- farAway.
	  getDistance()(i, j) = sign * farAway;
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
