// -*- C++ -*-

#if !defined(__cpt_State3_ipp__)
#error This file is an implementation detail of the class State.
#endif

BEGIN_NAMESPACE_CPT

//! Hold the state for a 3-D closest point transform.
/*!
  Most of the functionality is dimension independent and is thus implemented
  in the base class cpt::StateBase.

  To perform the closest point transform, you must call:
  -# cpt::StateBase<N,T>::setParameters() to specify the Cartesian domain 
  of interest and how far to compute the distance.  
  Note that the computational complexity of the alorithm is proportional to 
  this maximum distance.  (The number of grid points for which
  the distance is computed is proportional to the maximum distance.)  
  If performance is an issue, make sure that the \c max_distance
  parameter is no larger than needed.
  -# cpt::StateBase<N,T>::setLattice() to specify the lattice on which 
  the grids lie.
  -# cpt::StateBase<N,T>::insertGrid() for each grid on the lattice.
  Here you specify arrays that hold the
  distance, and optionally the gradient of distance, closest point 
  and closest face fields.  setLattice() must be called before any calls
  to insertGrid().
  -# cpt::StateBase<N,T>::setBRep() or 
  cpt::StateBase<N,T>::setBRepWithNoClipping() 
  to specify the surface.  The triangle surface is a boundary 
  representation, b-rep, of the volume that is inside the surface.  
  cpt::StateBase<N,T>::setParameters() must be called
  before calling cpt::StateBase<N,T>::setBRep().
  -# computeClosestPointTransform() to compute the the CPT on the specified 
  grids.

  To compute the CPT for a new b-rep, call:
  -# cpt::StateBase<N,T>::setBRep().
  -# computeClosestPointTransform().

  To compute the CPT for a different set of grids, call the following:
  -# cpt::StateBase<N,T>::clearGrids() to clear the old grids.
  -# cpt::StateBase<N,T>::setLattice() if the lattice has changed.
  -# cpt::StateBase<N,T>::insertGrid() for each new grid on the lattice.
  -# computeClosestPointTransform().
  .
  For AMR grids, one must follow these steps for each each level of the 
  AMR grid.  (Each level of the AMR grid is a separate lattice.)

  To determine which grid points are inside/outside the solid, call:
  -# cpt::StateBase<N,T>::setBRepWithNoClipping() to specify the boundary of 
  the solid.  Do not call cpt::StateBase<N,T>::setBRep().
  -# Either determinePointsInside() or determinePointsOutside().
*/
template<typename T>
class State<3,T> :
  public StateBase<3,T> {
  //
  // Private types.
  //

private:

  typedef StateBase<3,T> Base;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename Base::Number Number;
  //! A point in 3-D.
  typedef typename Base::Point Point;
  //! An indexed face in 3-D.
  typedef typename Base::IndexedFace IndexedFace;
  //! An index in 3-D.
  typedef typename Base::Index Index;
  //! An index range in 3-D.
  typedef typename Base::Range Range;
  //! A bounding box.
  typedef typename Base::BBox BBox;
  //! The grid geometry.
  typedef typename Base::Lattice Lattice;
  //! The grid.
  typedef typename Base::Grid Grid;
  //! The b-rep.
  typedef typename Base::BRep BRep;

  //
  // Not implemented.
  //

private:

  //! The copy constructor is not implemented.
  State(const State&);

  //! The assignment operator is not implemented.
  State&
  operator=(const State&);

  //
  // Private using.
  //

  // Member data.
  using Base::_brep;
  using Base::_hasCptBeenComputed;
  using Base::_lattice;
  using Base::_grids;
  using Base::_maximumDistance;

  // Accessors.
  using Base::hasBRepBeenSet;
  using Base::initializeGrids;

public:

  //
  // Public using.
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
  //! Compute the closest point transform for signed distance.
  using Base::computeClosestPointTransformUsingBBox;
  //! Compute the closest point transform for signed distance.
  using Base::computeClosestPointTransformUsingBruteForce;
  //! Compute the closest point transform for unsigned distance.
  using Base::computeClosestPointTransformUnsignedUsingBBox;
  //! Compute the closest point transform for unsigned distance.
  using Base::computeClosestPointTransformUnsignedUsingBruteForce;
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
  //! Set the b-rep.
  using Base::setBRepWithNoClipping;
  //! Set the b-rep.  Clip the mesh.
  using Base::setBRep;

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
  //! \name Closest point transform.
  //@{

  //! Compute the closest point transform for signed distance.
  /*!
    Compute the signed distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    The algorithm uses polyhedron scan conversion to determine which grid
    points are close to the shapes comprising the surface.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.
  */
  std::pair<int,int>
  computeClosestPointTransform();

  //! Compute the closest point transform for unsigned distance.
  /*!
    Compute the unsigned distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    The algorithm uses polyhedron scan conversion to determine which grid
    points are close to the shapes comprising the surface.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.
  */
  std::pair<int,int>
  computeClosestPointTransformUnsigned();

  //@}
  //-------------------------------------------------------------------------
  //! \name Inside/outside.
  //@{

  //! Determine which grid points are inside the solid (non-positive distance).
  /*!
    Use this interface if you repeatedly determine which points are inside.
    The distance array is needed for determining the sign of the distance.
    With this interface, the distance array is not allocated anew for each 
    function call.
  */
  template<bool A1, bool A2>
  std::pair<int,int>
  determinePointsInside(const BBox& domain,
			ads::Array<3,bool,A1>* areInside,
			ads::Array<3,Number,A2>* distance);

  //! Wrapper for the above function.
  /*!
    \param domain The array {xmin, ymin, zmin, xmax, ymax, zmax}.
    \param extents The array extents.
    \param areInside Boolean array is set to whether each grid point is 
    inside the solid.
    \param distance Number array that is used in the computation.
  */
  std::pair<int,int>
  determinePointsInside(const Number* domain,
			const int* extents,
			bool* areInside,
			Number* distance);

  //! Determine which grid points are inside the solid (non-positive distance).
  /*!
    Use this interface if you only determine which points are inside one time.
    This function allocates a distance array and calls the function above.
  */
  template<bool A1>
  std::pair<int,int>
  determinePointsInside(const BBox& domain,
			ads::Array<3,bool,A1>* areInside) {
    ads::Array<3,Number> distance(areInside->ranges());
    return determinePointsInside(domain, areInside, &distance);
  }

  //! Wrapper for the above function.
  /*!
    \param domain The array {xmin, ymin, zmin, xmax, ymax, zmax}.
    \param extents The array extents.
    \param areInside Boolean array is set to whether each grid point is 
    inside the solid.
  */
  std::pair<int,int>
  determinePointsInside(const Number* domain,
			const int* extents,
			bool* areInside);


  //! Determine which grid points are outside the solid (positive distance).
  /*!
    Use this interface if you repeatedly determine which points are outside.
    The distance array is needed for determining the sign of the distance.
    With this interface, the distance array is not allocated anew for each 
    function call.
  */
  template<bool A1, bool A2>
  std::pair<int,int>
  determinePointsOutside(const BBox& domain,
			 ads::Array<3,bool,A1>* areOutside,
			 ads::Array<3,Number,A2>* distance) {
    const std::pair<int,int> counts =
      determinePointsInside(domain, areOutside, distance);
    areOutside->negate();
    return counts;
  }

  //! Wrapper for the above function.
  /*!
    \param domain The array {xmin, ymin, zmin, xmax, ymax, zmax}.
    \param extents The array extents.
    \param areOutside Boolean array is set to whether each grid point is 
    outside the solid.
    \param distance Number array that is used in the computation.
  */
  std::pair<int,int>
  determinePointsOutside(const Number* domain,
			 const int* extents,
			 bool* areOutside,
			 Number* distance);

  //! Determine which grid points are outside the solid (positive distance).
  /*!
    Use this interface if you only determine which points are outside one time.
    This function allocates a distance array and calls the function above.
  */
  template<bool A1>
  std::pair<int,int>
  determinePointsOutside(const BBox& domain,
			 ads::Array<3,bool,A1>* areOutside) {
    const std::pair<int,int> counts =
      determinePointsInside(domain, areOutside);
    areOutside->negate();
    return counts;
  }

  //! Wrapper for the above function.
  /*!
    \param domain The array {xmin, ymin, zmin, xmax, ymax, zmax}.
    \param extents The array extents.
    \param areOutside Boolean array is set to whether each grid point is 
    outside the solid.
  */
  std::pair<int,int>
  determinePointsOutside(const Number* domain,
			 const int* extents,
			 bool* areOutside);

  // CONTINUE
#if 0
  //! Flood fill the distance.
  /*!
    The distance is flood filled.  If any of the distances are known, 
    set the unknown distances to +- farAway.  If no distances are 
    known, calculate the sign of the distance from the supplied mesh and
    set all distances to + farAway.
  */
  template<typename PointConstIterator, typename IndexConstIterator>
  void 
  floodFill(const Number farAway,
	    PointConstIterator verticesBeginning, 
	    PointConstIterator verticesEnd,
	    IndexConstIterator facesBeginning,
	    IndexConstIterator facesEnd);
#endif

  // CONTINUE
#if 0
  //! Flood fill the distance.
  /*!
    The distance is flood filled.  If any of the distances are known, 
    set the unknown distances to +- farAway.  If no distances are 
    known, calculate the sign of the distance from the supplied mesh and
    set all distances to + farAway.

    \param verticesSize is the number of vertices.
    \param vertices is a const pointer to the beginning of the vertices.
    \param facesSize is the number of faces.
    \param faces is a const pointer to the beginning of the faces.
  */
  void 
  floodFill(const Number farAway,
	    const int verticesSize, 
	    const void* vertices,
	    const int facesSize,
	    const void* faces);
#endif

  //@}

};






template<typename T>
inline
std::pair<int,int>
State<3,T>::
computeClosestPointTransform() {
  // CONTINUE: Remove the requirement that getNumberOfGrids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Compute the closest point transforms.
  return _brep.computeClosestPoint(_lattice, &_grids, _maximumDistance);
}


template<typename T>
inline
std::pair<int,int>
State<3,T>::
computeClosestPointTransformUnsigned() {
  // CONTINUE: Remove the requirement that getNumberOfGrids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Compute the closest point transforms.
  return _brep.computeClosestPointUnsigned(_lattice, &_grids, 
					   _maximumDistance);
}



// Determine which grid points are inside the solid (non-positive distance).
template<typename T>
template<bool A1, bool A2>
inline
std::pair<int,int>
State<3,T>::
determinePointsInside(const BBox& domain,
		      ads::Array<3,bool,A1>* areInside,
		      ads::Array<3,Number,A2>* distance) {
  // The two arrays must be the same size.
  assert(areInside->ranges() == distance->ranges());

  // Set up the lattice.
  setLattice(areInside->extents(), domain);
  // Set the maximum distance to slightly more than the maximum grid
  // spacing in any direction.
  setParameters(domain, 1.01 * ads::computeMaximum(_lattice.getDelta()));
  // Clear any old grids.
  clearGrids();
  // Add the distance grid.  Use empty grids for the rest.
  {
    ads::Array<3,Point> gradientOfDistance;
    ads::Array<3,Point> closestPoint;
    ads::Array<3,int> closestFace;
    insertGrid(distance, &gradientOfDistance, &closestPoint, &closestFace);
  }
  // Compute the distance.
  const std::pair<int,int> counts = computeClosestPointTransform();

  // If no distances were set, we need to determine the sign of the distance.
  if (counts.second == 0) {
    // Make a data structure that supports distance queries.
    geom::ISS_SignedDistance< geom::IndSimpSetIncAdj<3,2,true,T> > 
      signed_distance(_brep);
    // We can compute the signed distance to any point in the grid; we choose
    // the min corner.
    // If the distance is negative.
    if (signed_distance(domain.getLowerCorner()) <= 0) {
      // Set all of the distances to a negative value.
      _grids[0].getDistance() = -1.0;
    }
    // Else the distance is positive.
    else {
      // Set all of the distances to a positive value.
      _grids[0].getDistance() = 1.0;
    }
  }
  // Else, some of the distances were set.  We can flood fill the unknown
  // distances.
  else {
    // Flood fill the distance.  The far away value can be any positive number.
    floodFillAtBoundary(1.0);
  }

  // Clear the grids so they are not inadvertently used.
  clearGrids();
  // Use the sign of the distance to determine which points are inside.  
  typename ads::Array<3,bool,A1>::iterator i = areInside->begin();
  const typename ads::Array<3,bool,A1>::iterator iEnd = areInside->end();
  typename ads::Array<3,Number,A2>::const_iterator j = distance->begin();
  for (; i != iEnd; ++i, ++j) {
    // True iff the distance is non-positive.
    *i = *j <= 0.0;
  }

  return counts;
}




template<typename T>
inline
std::pair<int,int>
State<3,T>::
determinePointsInside(const Number* domain,
		      const int* extents,
		      bool* areInside,
		      Number* distance) {
  // Modified to compile with gcc 3.3.
  //ads::Array<3,bool,false> areInsideArray(Index(extents), areInside);
  Index arrayExtents(extents);
  ads::Array<3,bool,false> areInsideArray(arrayExtents, areInside);
  //ads::Array<3,Number,false> distanceArray(Index(extents), distance);
  ads::Array<3,Number,false> distanceArray(arrayExtents, distance);
  return determinePointsInside(BBox(domain), &areInsideArray, &distanceArray);
}




template<typename T>
inline
std::pair<int,int>
State<3,T>::
determinePointsInside(const Number* domain,
		      const int* extents,
		      bool* areInside) {
  // Modified to compile with gcc 3.3.
  //ads::Array<3,bool,false> areInsideArray(Index(extents), areInside);
  Index arrayExtents(extents);
  ads::Array<3,bool,false> areInsideArray(arrayExtents, areInside);
  return determinePointsInside(BBox(domain), &areInsideArray);
}




template<typename T>
inline
std::pair<int,int>
State<3,T>::
determinePointsOutside(const Number* domain,
		       const int* extents,
		       bool* areOutside,
		       Number* distance) {
  // Modified to compile with gcc 3.3.
  //ads::Array<3,bool,false> areOutsideArray(Index(extents), areOutside);
  Index arrayExtents(extents);
  ads::Array<3,bool,false> areOutsideArray(arrayExtents, areOutside);
  //ads::Array<3,Number,false> distanceArray(Index(extents), distance);
  ads::Array<3,Number,false> distanceArray(arrayExtents, distance);
  return determinePointsOutside(BBox(domain), &areOutsideArray, 
				&distanceArray);
}




template<typename T>
inline
std::pair<int,int>
State<3,T>::
determinePointsOutside(const Number* domain,
		       const int* extents,
		       bool* areOutside) {
  // Modified to compile with gcc 3.3.
  //ads::Array<3,bool,false> areOutsideArray(Index(extents), areOutside);
  Index arrayExtents(extents);
  ads::Array<3,bool,false> areOutsideArray(arrayExtents, areOutside);
  return determinePointsOutside(BBox(domain), &areOutsideArray);
}




// CONTINUE
#if 0
template<typename T>
template<typename PointConstIterator, typename IndexConstIterator>
inline
void 
State<3,T>::
floodFill(const Number farAway,
	  PointConstIterator verticesBeginning, 
	  PointConstIterator verticesEnd,
	  IndexConstIterator facesBeginning,
	  IndexConstIterator facesEnd) {
  // Make sure the cpt has been computed first.
  assert(_hasCptBeenComputed);
  // For each grid.
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    // Flood fill the distance grid.
    _grids[n].floodFill(farAway, verticesBeginning, verticesEnd, 
			facesBeginning, facesEnd);
  }
}
#endif


// CONTINUE
#if 0
template<typename T>
inline
void 
State<3,T>::
floodFill(const Number farAway,
	  const int verticesSize, 
	  const void* vertices,
	  const int facesSize,
	  const void* faces) {
  const Point* v = reinterpret_cast<const Point*>(vertices);
  const IndexedFace* f = 
    reinterpret_cast<const IndexedFace*>(faces);
  floodFill(farAway, v, v + 3 * verticesSize, f, f + 3 * facesSize);
}
#endif

END_NAMESPACE_CPT
