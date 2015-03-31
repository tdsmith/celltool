// -*- C++ -*-

/*! 
  \file cpt_c.h
  \brief The C interface to the cpt library.
*/

#if defined(__cplusplus)
extern "C" {
#endif

  //-------------------------------------------------------------------------
  //                        Set the parameters.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::setParameters().
  void 
  cptSetParameters3(const double* domain,
		    const double maximumDistance);

  //! Wrapper for State<2,double>::setParameters().
  void 
  cptSetParameters2(const double* domain,
		    const double maximumDistance,
		    const bool localClipping,
		    const int globalClipping,
		    const int decimationFactor);

  //-------------------------------------------------------------------------
  //                       Set the lattice.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::setLattice().
  void 
  cptSetLattice3(const int* extents, const double* domain);

  //! Wrapper for State<2,double>::setLattice().
  void 
  cptSetLattice2(const int* extents, const double* domain);

  //-------------------------------------------------------------------------
  //                            Add grids.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::insertGrid().
  void 
  cptInsertGrid3(const int* indexLowerBounds, 
		 const int* indexUpperBounds,
		 double* distance,
		 double* gradientOfDistance,
		 double* closestPoint,
		 int* closestFace);

  //! Wrapper for State<2,double>::insertGrid().
  void 
  cptInsertGrid2(const int* indexLowerBounds, 
		 const int* indexUpperBounds,
		 double* distance,
		 double* gradientOfDistance,
		 double* closestPoint,
		 int* closestFace);


  //-------------------------------------------------------------------------
  //                        Clear the grids.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::clearGrids().
  void 
  cptClearGrids3();

  //! Wrapper for State<2,double>::clearGrids().
  void 
  cptClearGrids2();


  //-------------------------------------------------------------------------
  //                       Set the b-rep.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::setBRepWithNoClipping().
  /*!
    \param verticesSize is the number of vertices.
    \param vertices
    is the beginning of the cartesian coordinates of the vertices in the 
    surface.  The coordinates are in the order: 
    \f$ \{ x_0, y_0, z_0, x_1, y_1, z_1, \ldots \} \f$
    \param facesSize is the number of faces.
    \param faces
    is the beginning of the vertex indices.  Three indices describe a face.
    The indices have positive orientation.  The indices are in the order:
    \f$ \{ face_0index_0, face_0index_1, face_0index2,
    face_1index_0, face_1index_1, face_1index2, \ldots \} \f$
  */
  void 
  cptSetBRepWithNoClipping3(const int verticesSize,
			    const double* vertices,
			    const int facesSize,
			    const int* faces);

  //! Wrapper for State<2,double>::setBRepWithNoClipping().
  /*!
    \param verticesSize is the number of vertices.
    \param vertices
    is the beginning of the cartesian coordinates of the vertices in the 
    surface.  The coordinates are in the order: 
    \f$ \{ x_0, y_0, x_1, y_1, \ldots \} \f$
    \param facesSize is the number of faces.
    \param faces
    is the beginning of the vertex indices.  Two indices describe a face.
    The indices have positive orientation.  The indices are in the order:
    \f$ \{ face_0index_0, face_0index_1,
    face_1index_0, face_1index_1, \ldots \} \f$
  */
  void 
  cptSetBRepWithNoClipping2(const int verticesSize,
			    const double* vertices,
			    const int facesSize,
			    const int* faces);



  //-------------------------------------------------------------------------
  //                       Initialize the b-rep.  
  //    Clip the mesh to use only points that affect the cartesian domain.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::setBRep().
  /*!
    \param verticesSize is the number of vertices.
    \param vertices
    is the beginning of the cartesian coordinates of the vertices in the 
    surface.  The coordinates are in the order: 
    \f$ \{ x_0, y_0, z_0, x_1, y_1, z_1, \ldots \} \f$
    \param facesSize is the number of faces.
    \param faces
    is the beginning of the vertex indices.  Three indices describe a face.
    The indices have positive orientation.  The indices are in the order:
    \f$ \{ face_0index_0, face_0index_1, face_0index2,
    face_1index_0, face_1index_1, face_1index2, \ldots \} \f$
  */
  void 
  cptSetBRep3(const int verticesSize,
	      const double* vertices,
	      const int facesSize,
	      const int* faces);

  //! Wrapper for State<2,double>::setBRep().
  /*!
    \param verticesSize is the number of vertices.
    \param vertices
    is the beginning of the cartesian coordinates of the vertices in the 
    surface.  The coordinates are in the order: 
    \f$ \{ x_0, y_0, x_1, y_1, \ldots \} \f$
    \param facesSize is the number of faces.
    \param faces
    is the beginning of the vertex indices.  Two indices describe a face.
    The indices have positive orientation.  The indices are in the order:
    \f$ \{ face_0index_0, face_0index_1,
    face_1index_0, face_1index_1, \ldots \} \f$
  */
  void 
  cptSetBRep2(const int verticesSize,
	      const double* vertices,
	      const int facesSize,
	      const int* faces);


  //-------------------------------------------------------------------------
  //                      Closest Point Transform.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::computeClosestPointTransform().
  void 
  cptComputeClosestPointTransform3();

  //! Wrapper for State<3,double>::computeClosestPointTransformUnsigned().
  void 
  cptComputeClosestPointTransformUnsigned3();


  //! Wrapper for State<3,double>::computeClosestPointTransformUsingBBox().
  void 
  cptComputeClosestPointTransformUsingBBox3();

  //! Wrapper for State<3,double>::computeClosestPointTransformUnsignedUsingBBox().
  void 
  cptComputeClosestPointTransformUnsignedUsingBBox3();


  //! Wrapper for State<3,double>::computeClosestPointTransformUsingBruteForce().
  void 
  cptComputeClosestPointTransformUsingBruteForce3();

  //! Wrapper for State<3,double>::computeClosestPointTransformUnsignedUsingBruteForce().
  void 
  cptComputeClosestPointTransformUnsignedUsingBruteForce3();



  //! Wrapper for State<2,double>::computeClosestPointTransform().
  void 
  cptComputeClosestPointTransform2();

  //! Wrapper for State<2,double>::computeClosestPointTransformUnsigned().
  void 
  cptComputeClosestPointTransformUnsigned2();


  //! Wrapper for State<2,double>::computeClosestPointTransformUsingBBox().
  void 
  cptComputeClosestPointTransformUsingBBox2();

  //! Wrapper for State<2,double>::computeClosestPointTransformUnsignedUsingBBox().
  void 
  cptComputeClosestPointTransformUnsignedUsingBBox2();


  //! Wrapper for State<2,double>::computeClosestPointTransformUsingBruteForce().
  void 
  cptComputeClosestPointTransformUsingBruteForce2();

  //! Wrapper for State<2,double>::computeClosestPointTransformUnsignedUsingBruteForce().
  void 
  cptComputeClosestPointTransformUnsignedUsingBruteForce2();


  //-------------------------------------------------------------------------
  //    Flood fill the distance.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::floodFillAtBoundary().
  void 
  cptFloodFillAtBoundary3(double farAway);

  //! Wrapper for State<3,double>::floodFillDetermineSign().
  void 
  cptFloodFillDetermineSign3(double farAway);

  //! Wrapper for State<3,double>::floodFillUnsigned().
  void 
  cptFloodFillUnsigned3(double farAway);

  //! Wrapper for State<3,double>::floodFillAtBoundary().
  void 
  cptFloodFillAtBoundary2(double farAway);

  //! Wrapper for State<3,double>::floodFillDetermineSign().
  void 
  cptFloodFillDetermineSign2(double farAway);

  //! Wrapper for State<3,double>::floodFillUnsigned().
  void 
  cptFloodFillUnsigned2(double farAway);


  //-------------------------------------------------------------------------
  //    Check if the grids are valid.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::areGridsValid().
  int
  cptAreGridsValid3();

  //! Wrapper for State<3,double>::areGridsValidUnsigned().
  int
  cptAreGridsValidUnsigned3();

  //! Wrapper for State<2,double>::areGridsValid().
  int
  cptAreGridsValid2();

  //! Wrapper for State<2,double>::areGridsValidUnsigned().
  int
  cptAreGridsValidUnsigned2();


  //-------------------------------------------------------------------------
  //        Write information about the state of the grid variables.
  //-------------------------------------------------------------------------

  //! Wrapper for State<3,double>::displayInformation().
  void 
  cptDisplayInformation3();

  //! Wrapper for State<2,double>::displayInformation().
  void 
  cptDisplayInformation2();

#if defined(__cplusplus)
}
#endif
