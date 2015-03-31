// -*- C++ -*-

#if !defined(__cpt_StateBase_h__)
#define __cpt_StateBase_h__

// Local
#include "Grid.h"
#include "BRep.h"

#include "../geom/mesh/iss/distance.h"

#include <iostream>

#include <cmath>

BEGIN_NAMESPACE_CPT

//! Hold the state for a closest point transform.
/*!
  Implements the dimension-independent functionality.
*/
template<int N, typename T = double>
class StateBase {
  //
  // Protected types.
  //

protected:

  //! The number type.
  typedef T Number;
  //! A point in N-D.
  typedef ads::FixedArray<N,Number> Point;
  //! The indices of a face.
  typedef ads::FixedArray<N,int> IndexedFace;
  //! An index in N-D.
  typedef ads::FixedArray<N,int> Index;
  //! An index range in N-D.
  typedef ads::IndexRange<N,int> Range;
  //! A bounding box.
  typedef geom::BBox<N,Number> BBox;
  //! The lattice defines a domain and index extents.
  typedef geom::RegularGrid<N,T> Lattice;
  //! The grid.
  typedef Grid<N,T> Grid;
  //! The b-rep.
  typedef BRep<N,T> BRep;

  //
  // Member data.
  //

protected:

  //! Has the b-rep been set.
  bool _hasBRepBeenSet;
  //! Has the CPT been computed.
  bool _hasCptBeenComputed;
  //! The domain containing all grids for which the CPT will be computed.
  /*! This may be used in clipping the mesh. */
  BBox _domain;
  //! How far (in Cartesian space) to compute the distance.
  Number _maximumDistance;
  //! The lattice.
  Lattice _lattice;
  //! The grids.
  std::vector<Grid> _grids;
  //! The b-rep.
  BRep _brep;

  //
  // Not implemented.
  //

private:

  //! The copy constructor is not implemented.
  StateBase(const StateBase&);

  //! The assignment operator is not implemented.
  StateBase&
  operator=(const StateBase&);

  // CONTINUE: Modified to compile with gcc 3.3.  It cannot use insertGrid
  // in the derived class when it is protected here.
  //protected:
public:

  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{

  //! Default constructor.
  StateBase() :
    _hasBRepBeenSet(false),
    _hasCptBeenComputed(false),
    _domain(),
    _maximumDistance(-1),
    _lattice(),
    _grids(),
    _brep()
  {}

  //! Destructor.
  ~StateBase()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the number of grids.
  int
  getNumberOfGrids() const {
    return int(_grids.size());
  }

  //! Return true if the b-rep has been set.
  bool
  hasBRepBeenSet() const {
    return _hasBRepBeenSet;
  }

  //! Return the domain that contains all grids.
  const BBox&
  getDomain() const {
    return _domain;
  }

  //! Return how far the distance is being computed.
  Number
  getMaximumDistance() const {
    return _maximumDistance;
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  // CONTINUE: The max distance should be specified on a per level basis.
  //! Set the parameters for the Closest Point Transform.
  /*! 
    This function must be called at least once before calls to 
    computeClosestPointTransform().
  
    \param domain is the Cartesian domain that contains all grids.
    \param maximumDistance
    The distance will be computed up to maximumDistance away from the surface.

    The distance for grid points whose distance is larger than
    maximumDistance will be set to \c std::numeric_limits<Number>::max().  
    Each component of the closest point of these far away
    points will be set to \c std::numeric_limits<Number>::max().  
    The closest face of far away points will be set to -1.
  */
  void 
  setParameters(const BBox& domain, Number maximumDistance);

  //! Set parameters for the closest point transform.
  /*! 
    This is a wrapper for the above setParameters function.
  */
  void 
  setParameters(const Number* domain, const Number maximumDistance) {
    setParameters(BBox(domain), maximumDistance);
  }

  //! Set the parameters for the Closest Point Transform.
  /*! 
    This calls the first setParameters() function.
    The domain containing all grids is set to all space.  This means that
    clipping the mesh will have no effect.
  */
  void 
  setParameters(const Number maximumDistance) {
    // The point at infinity.
    const Point Infinity(std::numeric_limits<Number>::max());
    // Call setParameters() with all of space as the domain.
    setParameters(BBox(-Infinity, Infinity), maximumDistance);
  }

  // CONTINUE Make sure setLattice is called before adding grids.
  // CONTINUE enable multiple levels.  Add documentation.
  //! Set the grid geometry.
  /*!
    \param extents are the grid geometry extents.
    \param domain is the grid geometry domain.
  */
  void 
  setLattice(const Index& extents, const BBox& domain);

  //! Set the lattice geometry.
  /*!
    This is a wrapper for the above setLattice function().
  */
  void 
  setLattice(const int* extents, const Number* domain) {
    setLattice(Index(extents), BBox(domain));
  }

  //! Add a grid for the Closest Point Transform.
  /*!
    This function must be called at least once before calls to 
    computeClosestPointTransform().

    \param distance 
    is the Array that will be assigned the distance from the grid 
    points to the surface by the computeClosestPointTransform() function.

    \param gradientOfDistance
    is a Array that holds the gradient of the distance It is
    computed from the geometric primitives and not by differencing the
    distance array.  Thus it is accurate to machine precision.  If this
    array has zero size, the gradient of the distance will not be
    computed.

    \param closestPoint 
    is a Array that holds the closest point on the triangle surface.
    If this array has zero size, the closest point will not be computed.

    \param closestFace 
    is a Array that holds the index of the closest face on the 
    triangle surface.  If this array has zero size, the closest face 
    will not be computed.
  */
  template<bool A1, bool A2, bool A3, bool A4>
  void 
  insertGrid(ads::Array<N,Number,A1>* distance,
	     ads::Array<N,Point,A2>* gradientOfDistance,
	     ads::Array<N,Point,A3>* closestPoint,
	     ads::Array<N,int,A4>* closestFace);

  //! Add a grid for the Closest Point Transform.
  /*!
    This is a wrapper for the above insertGrid() function.

    The first two parameters describe the index range of the grid.
    The lower bounds are closed; the upper bounds are open.
    For each of the \c gradientOfDistance, \c closestPoint and \c closestFace
    arrays: if the pointer is non-zero, that quantity will be computed.
  */
  void 
  insertGrid(const int* indexLowerBounds,
	     const int* indexUpperBounds,
	     Number* distance,
	     Number* gradientOfDistance,
	     Number* closestPoint,
	     int* closestFace);

  //! Clear the grids.
  /*!
    If the grids change, call this function and then add all the new grids with
    insertGrid().
  */
  void
  clearGrids() {
    _grids.clear();
  }

  //! Compute the closest point transform for signed distance.
  /*!
    Compute the signed distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    This algorithm does not use polyhedron scan conversion.  Instead, it builds
    bounding boxes around the characteristic polyhedra.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.

    \return
    Return the number of points for which the distance was computed and the 
    number of points for which the distance was set.
  */
  std::pair<int,int>
  computeClosestPointTransformUsingBBox();

  //! Compute the closest point transform for signed distance.
  /*!
    Compute the signed distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    This algorithm does not use polyhedron scan conversion or the 
    characteristic polyhedra.  Instead, it builds
    bounding boxes around the faces, edges and vertices.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.

    \return
    Return the number of points for which the distance was computed and the 
    number of points for which the distance was set.
  */
  std::pair<int,int>
  computeClosestPointTransformUsingBruteForce();

  //! Compute the closest point transform for signed distance.
  /*!
    Compute the signed distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    This algorithm uses a bounding box tree to store the mesh and 
    a lower-upper-bound queries to determine the distance.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.

    \return
    Return the number of points for which the distance was computed and the 
    number of points for which the distance was set.
  */
  std::pair<int,int>
  computeClosestPointTransformUsingTree();

  //! Compute the closest point transform for unsigned distance.
  /*!
    Compute the unsigned distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    This algorithm does not use polyhedron scan conversion.  Instead, it builds
    bounding boxes around the characteristic polyhedra.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.

    \return
    Return the number of points for which the distance was computed and the 
    number of points for which the distance was set.
  */
  std::pair<int,int>
  computeClosestPointTransformUnsignedUsingBBox();

  //! Compute the closest point transform for unsigned distance.
  /*!
    Compute the unsigned distance.  Compute the gradient of the distance, the
    closest face and closest point if their arrays specified in
    insertGrid() are nonzero.

    This algorithm does not use polyhedron scan conversion or the 
    characteristic polyhedra.  Instead, it builds
    bounding boxes around the faces, edges and vertices.

    The unknown distances, gradients, and closest points are set to 
    \c std::numeric_limits<Number>::max().
    The unknown closest faces are set to -1.

    \return
    Return the number of points for which the distance was computed and the 
    number of points for which the distance was set.
  */
  std::pair<int,int>
  computeClosestPointTransformUnsignedUsingBruteForce();

  //! Flood fill the distance.
  /*!
    This function is used to prepare the distance for visualization.
    The signed distance is flood filled.  
    If any of the distances are known in a particular grid, 
    set the unknown distances to +-farAway.  If no 
    distances are known, set all distances to +farAway.
    Thus note that if no points in a particular grid have known distance,
    then the sign of the distance is not determined.
  */
  void
  floodFillAtBoundary(Number farAway);

  //! Flood fill the distance.
  /*!
    This function is used to prepare the distance for visualization.
    The signed distance is flood filled.  
    If any of the distances are known in a particular grid, 
    set the unknown distances to +-farAway.  If no 
    distances are known, determine the correct sign by computing the signed
    distance to the boundary for a single point in the grid.

    \note In order to determine the sign of the distance for far away grids,
    this algorithm needs that portion of the boundary that is closest to 
    those grids.  Using setBRep() may clip away the needed portion (or for 
    that matter all) of the boundary.  You should use 
    setBRepWithNoClipping() before calling this function.  To be on the
    safe side, you should give the entire boundary to 
    setBRepWithNoClipping() .
  */
  void
  floodFillDetermineSign(Number farAway);

  //! Flood fill the unsigned distance.
  /*!
    The unsigned distance is flood filled.  Unknown distances are set to 
    \c farAway.
  */
  void
  floodFillUnsigned(Number farAway);

  //! Check the grids.
  /*!
    Verify that the distance grids are valid.  The known distances should
    be between +-maximumDistance.  The difference between adjacent grid
    points should not be more than the grid spacing.  Verify that the
    closest point and closest face grids are valid.  Return true if the grids
    are valid.  Return false and print a message to stderr otherwise.
  */
  bool 
  areGridsValid();

  //! Check the grids.
  /*!
    Verify that the distance grids are valid.  The known distances should
    be between 0 and maximumDistance.  The difference between adjacent grid
    points should not be more than the grid spacing.  Verify that the
    closest point and closest face grids are valid.  Return true if the grids
    are valid.  Return false and print a message to stderr otherwise.
  */
  bool 
  areGridsValidUnsigned();

  //! Set the b-rep.
  /*!
    Do not use the Cartesian domain to clip the mesh.
  
    Either this function or setBRep() must be called at least 
    once before calls to computeClosestPointTransform().

    \param verticesSize is the number of vertices.
    \param vertices is a const pointer to the beginning of the vertices.
    \param facesSize is the number of faces.
    \param faces is a const pointer to the beginning of the faces.
  */
  void 
  setBRepWithNoClipping(int verticesSize,
			const void* vertices,
			int facesSize,
			const void* faces);

  //! Set the b-rep.
  /*!
    Clip the mesh to use only points that affect the cartesian domain.

    Either this function or setBRepWithNoClipping() must be called at least 
    once before calls to computeClosestPointTransform().  This version is more 
    efficient if the b-rep extends beyond the domain spanned by the grid.

    \param verticesSize is the number of vertices.
    \param vertices is a const pointer to the beginning of the vertices.
    \param facesSize is the number of faces.
    \param faces is a const pointer to the beginning of the faces.
  */
  void 
  setBRep(int verticesSize,
	  const void* vertices,
	  int facesSize,
	  const void* faces);

  //@}
  //-------------------------------------------------------------------------
  //! \name I/O.
  //@{

  //! Display information about the state of the closest point transform.
  void 
  displayInformation(std::ostream& out) const;

  //@}
  //-------------------------------------------------------------------------
  //! \name Grid operations.
  //@{

protected:

  //! Initialize the grids.
  void
  initializeGrids() {
    for (int n = 0; n != getNumberOfGrids(); ++n) {
      _grids[n].initialize();
    }
  }

  //@}

};

END_NAMESPACE_CPT

#define __cpt_StateBase_ipp__
#include "StateBase.ipp"
#undef __cpt_StateBase_ipp__

#endif
