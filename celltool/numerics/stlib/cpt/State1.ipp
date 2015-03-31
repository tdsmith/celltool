// -*- C++ -*-

#if !defined(__cpt_State1_ipp__)
#error This file is an implementation detail of the class State.
#endif

BEGIN_NAMESPACE_CPT

// CONTINUE
#if 0
//! Hold the state for a 1-D closest point transform.
template<typename T>
class State<1,T> :
  public StateBase<1,T> {
private:

  //
  // Private types.
  //

  typedef StateBase<1,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef typename Base::Number Number;
  //! A point in 1-D.
  typedef typename Base::Point Point;
  //! A bounding box.
  typedef typename Base::BBox BBox;
  //! The grid.
  typedef typename Base::Grid Grid;
  //! The b-rep.
  typedef typename Base::Brep Brep;

public:

  //
  // Using.
  //

  // Accessors.

  //! Return the number of grids.
  using Base::getNumberOfGrids;
  //! Return the domain that contains all grids.
  using Base::getDomain;
  //! Return how far the distance is being computed.
  using Base::getMaximumDistance;

  // Manipulators.

  //! Set parameters for the closest point transform.
  using Base::setParameters;
  //! Set the lattice geometry.
  using Base::setLattice;
  //! Add a grid for the closest point transform.
  using Base::insertGrid;
  //! Clear the grids.
  using Base::clearGrids;
  //! Flood fill the signed distance.
  using Base::floodFillAtBoundary;
  //! Flood fill the signed distance.  Determine the sign of the distance on all grids.
  using Base::floodFillDetermineSign;
  //! Flood fill the unsigned distance.
  using Base::floodFillUnsigned;
  //! Check the grids.
  using Base::areGridsValid;
  //! Check the grids.
  using Base::areGridsValidUnsigned;

  // I/O.

  //! Display information about the state of the closest point transform.
  using Base::displayInformation;

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.
  State() :
    Base()
  {}

  //! Destructor.
  ~State()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Compute the Closest Point Transform with signed distance.
  /*!
    Compute the signed distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    add_grid() are nonzero.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.
  */
  std::pair<int,int>
  computeClosestPointTransform();

  //! Compute the Closest Point Transform with unsigned distance.
  /*!
    Compute the unsigned distance.  Compute the gradient of this distance, the
    closest face and closest point if their arrays specified in
    add_grid() are nonzero.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.
  */
  std::pair<int,int>
  computeClosestPointTransformUnsigned();

  //! Flood fill the distance.
  /*!
    The distance is flood filled.  If any of the distances are known, 
    set the unknown distances to +- far_away.  If no distances are 
    known, calculate the sign of the distance from the supplied mesh and
    set all distances to + far_away.
  */
  template<typename NumberInputIter, typename IntegerInputIter>
  void 
  floodFill(const Number farAway,
	    NumberInputIter locationsBeginning, 
	    NumberInputIter locationsEnd,
	    IntegerInputIter orientationsBeginning, 
	    IntegerInputIter orientationsEnd)
  {
    // Make sure the cpt has been computed first.
    assert(Base::_cpt_computed);

    // Flood fill the distance grid.
    Base::_grid.floodFill(farAway, locationsBeginning, locationsEnd, 
			  orientationsBeginning, orientationsEnd);
  }

  //! Flood fill the distance.
  /*!
    The distance is flood filled.  If any of the distances are known, 
    set the unknown distances to +- farAway.  If no distances are 
    known, calculate the sign of the distance from the supplied mesh and
    set all distances to + farAway.

    \param num_vertices is the number of vertices.
    \param vertices is a const pointer to the beginning of the vertices.
    \param facesSize is the number of faces.
    \param faces is a const pointer to the beginning of the faces.
  */
  void 
  floodFill(const Number farAway,
	    const int facesSize,
	    const void* locations,
	    const void* orientations)
  {
    const Point* loc = reinterpret_cast<const Point*>(locations);
    const int* ori = reinterpret_cast<const int*>(orientations);
    floodFill(farAway, loc, loc + facesSize, ori, ori + facesSize);
  }

  //! Set the b-rep.
  /*!
    Do not use the Cartesian domain to clip the mesh.
  
    Either this function or set_brep() must be called at least 
    once before calls to closest_point_transform().

    \param facesSize is the number of faces.
    \param locations is a const pointer to the beginning of the face locations.
    \param orientations is a const pointer to the beginning of the face 
    orientations.  +1 means that positive distances are to the right.  -1 
    means that positive distances are to the left.  
  */
  void 
  setBRepWithNoClipping(const int facesSize, const void* locations,
			const void* orientations)
  {
    const Number* loc = reinterpret_cast<const Number*>(locations);
    const int* ori = reinterpret_cast<const int*>(orientations);
    setBRepWithNoClipping(loc, loc + facesSize, ori, ori + facesSize);
  }

  //! Set the b-rep.
  /*!
    Clip the mesh to use only points that affect the cartesian domain.

    Either this function or set_brep_noclip() must be called at least 
    once before calls to closest_point_transform().  This version is more 
    efficient if the b-rep extends beyond the domain spanned by the grid.

    \param facesSize is the number of faces.
    \param locations is a const pointer to the beginning of the face locations.
    \param orientations is a const pointer to the beginning of the face 
    orientations.  +1 means that positive distances are to the right.  -1 
    means that positive distances are to the left.  
  */
  void 
  setBrep(const int facesSize, const void* locations,
	  const void* orientations)
  {
    const Point* loc = reinterpret_cast<const Point*>(locations);
    const int* ori = reinterpret_cast<const int*>(orientations);
    setBrep(loc, loc + facesSize, ori, ori + facesSize);
  }

  //@}
};
#endif













// CONTINUE
#if 0
template<typename T>
inline
std::pair<int,int>
State<1,T>::
computeClosestPointTransform() {
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && Base::hasBRepBeenSet());

  // Initialize the distance array.
  Base::initializeGrids();

  // Signify that the cpt has been computed.
  Base::_hasCptBeenComputed = true;

  // Compute the closest point transform.
  return Base::_brep.computeClosestPoint(Base::_grids,
					 Base::_maximumDistance);
}

template<typename T>
inline
std::pair<int,int>
State<1,T>::
computeClosestPointTransformUnsigned() {
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && Base::hasBRepBeenSet());

  // Initialize the distance array.
  Base::initializeGrids();

  // Signify that the cpt has been computed.
  Base::_hasCptBeenComputed = true;

  // Compute the closest point transform.
  return Base::_brep.computeClosestPointUnsigned(Base::_grids,
						 Base::_maximumDistance);
}
#endif

END_NAMESPACE_CPT
