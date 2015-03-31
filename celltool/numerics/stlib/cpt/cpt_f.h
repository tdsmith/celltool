// -*- C++ -*-

/*! 
  \file cpt_f.h
  \brief The fortran interface to the cpt library.
*/


#include "f77_bindings.h"

#if defined(__cplusplus)
extern "C" {
#endif


  //-------------------------------------------------------------------------
  //                         Set the parameters.
  //-------------------------------------------------------------------------

  //! Wrapper for cptSetParameters3().
  void 
  cptSetParameters3F(double* domain,
		     double *maximumDistance);

  //! Wrapper for cptSetParameters2().
  void 
  cptSetParameters2F(double* domain,
		     double* maximumDistance,
		     int* localClipping,
		     int* globalClipping,
		     int* decimationFactor);

  //-------------------------------------------------------------------------
  //                       Set the lattice.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::setLattice().
  void 
  cptSetLattice3F(int* extents, double* domain);

  //! Wrapper for State<2,double>::setLattice().
  void 
  cptSetLattice2F(int* extents, double* domain);

  //-------------------------------------------------------------------------
  //                            Add grids.
  //-------------------------------------------------------------------------

  //! Wrapper for cptInsertGrid3().
  /*!
    \param indexLowerBounds are the closed lower bounds on the grid indices.
    \param indexUpperBounds are the open lower bounds on the grid indices.
    \param distance is the distance array.
    \param computeGradientDistance
    If \c computeGradientDistance is non-zero, the gradient of the
    distance will be computed.  If \c computeGradientDistance is zero,
    it will not be computed.  Then just pass a \c double for \c
    gradientDistance instead of an array.
    \param gradientDistance is the gradient of the distance array.
    \param computeClosestPoint
    If \c computeClosestPoint is non-zero, the closest point will be
    computed.  If \c computeClosestPoint is zero, it will not be
    computed.  Then just pass a \c double for closestPoint
    instead of an array.
    \param closestPoint is the closest point array.
    \param computeClosestFace:
    If \c computeClosestFace is non-zero, the closest face will be
    computed.  If \c computeClosestFace is zero, it will not be
    computed.  Then just pass an integer for \c closestFace instead
    of an array.
    \param closestFace is the closest face array.
  */
  void 
  cptInsertGrid3F(int* indexLowerBounds, 
		  int* indexUpperBounds,
		  double* distance,
		  int* computeGradientDistance,
		  double* gradientDistance,
		  int* computeClosestPoint,
		  double* closestPoint,
		  int* computeClosestFace,
		  int* closestFace);
  
  //! Wrapper for cptInsertGrid2().
  /*!
    \param indexLowerBounds are the closed lower bounds on the grid indices.
    \param indexUpperBounds are the open lower bounds on the grid indices.
    \param distance is the distance array.
    \param computeGradientDistance
    If \c computeGradientDistance is non-zero, the gradient of the
    distance will be computed.  If \c computeGradientDistance is zero,
    it will not be computed.  Then just pass a \c double for \c
    gradientDistance instead of an array.
    \param gradientDistance is the gradient of the distance array.
    \param computeClosestPoint
    If \c computeClosestPoint is non-zero, the closest point will be
    computed.  If \c computeClosestPoint is zero, it will not be
    computed.  Then just pass a \c double for closestPoint
    instead of an array.
    \param closestPoint is the closest point array.
    \param computeClosestFace:
    If \c computeClosestFace is non-zero, the closest face will be
    computed.  If \c computeClosestFace is zero, it will not be
    computed.  Then just pass an integer for \c closestFace instead
    of an array.
    \param closestFace is the closest face array.
  */
  void 
  cptInsertGrid2F(int* indexLowerBounds, 
		  int* indexUpperBounds,
		  double* distance,
		  int* computeGradientDistance,
		  double* gradientDistance,
		  int* computeClosestPoint,
		  double* closestPoint,
		  int* computeClosestFace,
		  int* closestFace);
  
  //-------------------------------------------------------------------------
  //                         Clear the grids.
  //-------------------------------------------------------------------------

  //! Wrapper for cptClearGrids3().
  void 
  cptClearGrids3F();
  
  //! Wrapper for cptClearGrids2().
  void 
  cptClearGrids2F();
  
  //-------------------------------------------------------------------------
  //                          Set the b-rep.
  //-------------------------------------------------------------------------

  //! Wrapper for cptSetBRepWithNoClipping3().
  void 
  cptSetBRepWithNoClipping3F(int *verticesSize,
			     const double* vertices,
			     int *facesSize,
			     const int* faces);

  //! Wrapper for cptSetBRepWithNoClipping2().
  void 
  cptSetBRepWithNoClipping2F(int *verticesSize,
			     const double* vertices,
			     int *facesSize,
			     const int* faces);

  //-------------------------------------------------------------------------
  //                       Initialize the b-rep.  
  //    Clip the mesh to use only points that affect the cartesian domain.
  //-------------------------------------------------------------------------

  //! Wrapper for cptSetBRep3().
  void 
  cptSetBRep3F(int *verticesSize,
	       const double* vertices,
	       int *facesSize,
	       const int* faces);

  //! Wrapper for cptSetBRep2().
  void 
  cptSetBRep2F(int *verticesSize,
	       const double* vertices,
	       int *facesSize,
	       const int* faces);

  //-------------------------------------------------------------------------
  //                      Closest Point Transform.
  //-------------------------------------------------------------------------

  //! Wrapper for cptComputeClosestPointTransform3().
  void 
  cptComputeClosestPointTransform3F();

  //! Wrapper for cptComputeClosestPointTransformUnsigned3().
  void 
  cptComputeClosestPointTransformUnsigned3F();


  //! Wrapper for cptComputeClosestPointTransformUsingBBox3().
  void 
  cptComputeClosestPointTransformUsingBBox3F();

  //! Wrapper for cptComputeClosestPointTransformUnsignedUsingBBox3().
  void 
  cptComputeClosestPointTransformUnsignedUsingBBox3F();


  //! Wrapper for cptComputeClosestPointTransformUsingBruteForce3().
  void 
  cptComputeClosestPointTransformUsingBruteForce3F();

  //! Wrapper for cptComputeClosestPointTransformUnsignedUsingBruteForce3().
  void 
  cptComputeClosestPointTransformUnsignedUsingBruteForce3F();


  //! Wrapper for cptComputeClosestPointTransform2().
  void 
  cptComputeClosestPointTransform2F();

  //! Wrapper for cptComputeClosestPointTransformUnsigned2().
  void 
  cptComputeClosestPointTransformUnsigned2F();


  //! Wrapper for cptComputeClosestPointTransformUsingBBox2().
  void 
  cptComputeClosestPointTransformUsingBBox2F();

  //! Wrapper for cptComputeClosestPointTransformUnsignedUsingBBox2().
  void 
  cptComputeClosestPointTransformUnsignedUsingBBox2F();


  //! Wrapper for cptComputeClosestPointTransformUsingBruteForce2().
  void 
  cptComputeClosestPointTransformUsingBruteForce2F();

  //! Wrapper for cptComputeClosestPointTransformUnsignedUsingBruteForce2().
  void 
  cptComputeClosestPointTransformUnsignedUsingBruteForce2F();


  //-------------------------------------------------------------------------
  //    Flood fill the distance.
  //-------------------------------------------------------------------------

  //! Wrapper for cptFloodFillAtBoundary3().
  void 
  cptFloodFillAtBoundary3F(double* farAway);

  //! Wrapper for cptFloodFillDetermineSign3().
  void 
  cptFloodFillDetermineSign3F(double* farAway);

  //! Wrapper for cptFloodFillUnsigned3().
  void 
  cptFloodFillUnsigned3F(double* farAway);

  //! Wrapper for cptFloodFillAtBoundary2().
  void 
  cptFloodFillAtBoundary2F(double* farAway);

  //! Wrapper for cptFloodFillDetermineSign2().
  void 
  cptFloodFillDetermineSign2F(double* farAway);

  //! Wrapper for cptFloodFillUnsigned2().
  void 
  cptFloodFillUnsigned2F(double* farAway);

  //-------------------------------------------------------------------------
  //    Check if the grids are valid.
  //-------------------------------------------------------------------------

  //! Wrapper for cptAreGridsValid3().
  int 
  cptAreGridsValid3F();

  //! Wrapper for cptAreGridsValidUnsigned3().
  int 
  cptAreGridsValidUnsigned3F();

  //! Wrapper for cptAreGridsValid2().
  int 
  cptAreGridsValid2F();

  //! Wrapper for cptAreGridsValidUnsigned2().
  int 
  cptAreGridsValidUnsigned2F();

  //-------------------------------------------------------------------------
  //        Write information about the state of the variables.
  //-------------------------------------------------------------------------

  //! Wrapper for cptDisplayInformation3().
  void 
  cptDisplayInformation3F();

  //! Wrapper for cptDisplayInformation2().
  void 
  cptDisplayInformation2F();

#if defined(__cplusplus)
}
#endif
