// -*- C++ -*-

#if !defined(__cpt_BRep3_ipp__)
#error This file is an implementation detail of the class BRep.
#endif

BEGIN_NAMESPACE_CPT

//! A class for a b-rep in 3-D.
template<typename T>
class BRep<3,T> :
  public geom::IndSimpSetIncAdj<3,2,true,T> {
  // 
  // Private types.
  //

private:

  //! The base is an indexed simplex set with incidence and adjacency information.
  typedef geom::IndSimpSetIncAdj<3,2,true,T> Base;

  // 
  // Public types.
  //

public:

  //! The size type.
  typedef typename Base::SizeType SizeType;
  //! The number type.
  typedef T Number;
  //! A point in 3-D.
  typedef ads::FixedArray<3,Number> Point;
  //! A multi-index in 3-D.
  typedef ads::FixedArray<3,int> Index;
  //! A bounding box.
  typedef geom::BBox<3,Number> BBox;
  //! The lattice.
  typedef geom::RegularGrid<3,T> Lattice;
  //! The grid type stores the arrays for distance, closest point, etc.
  typedef Grid<3,Number> Grid;

  // 
  // Private types.
  //

private:

  typedef typename Base::FaceIterator EdgeIterator;
  typedef typename Base::IndexedSimplex IndexedSimplex;
  typedef cpt::Vertex<3,T> VertexDistance;
  typedef cpt::Edge<3,T> EdgeDistance;
  typedef cpt::Face<3,T> FaceDistance;
  typedef geom::ScanConversionPolyhedron<T> Polyhedron;
  //! An index range.
  typedef typename Grid::Range Range;

  //
  // Member data.
  //

private:

  //! The unit outward normals of the faces.
  ads::Array<1,Point> _faceNormals;
  //! The unit outward normals of the vertices.
  ads::Array<1,Point> _vertexNormals;
  //! The face identifiers.
  ads::Array<1,int> _faceIdentifiers;

  //
  // Using the base member functions.
  //

  // Vertex Accessors
  using Base::getSpaceDimension;
  // Declared as public below.
  //using Base::getVerticesSize;
  using Base::getVerticesBeginning;
  using Base::getVerticesEnd;
  // Declared as public below.
  //using Base::getVertex;
  using Base::getVertices;

  // Simplex Accessors
  using Base::getSimplexDimension;
  // Declared as public below.
  //using Base::getSimplicesSize;
  using Base::getIndexedSimplicesBeginning;
  using Base::getIndexedSimplicesEnd;
  // Declared as public below.
  //using Base::getIndexedSimplex;
  using Base::getIndexedSimplices;
  using Base::getSimplicesBeginning;
  using Base::getSimplicesEnd;
  using Base::getSimplexVertex;
  using Base::getSimplex;

  // Vertex-Simplex Incidence Accessors
  using Base::getVertexSimplexIncidence;
  using Base::getIncidentSize;
  using Base::isIncidentEmpty;
  using Base::getIncidentBeginning;
  using Base::getIncidentEnd;
  using Base::getIncident;

  // Simplex Adjacency Accessors
  using Base::getSimplexAdjacencies;
  using Base::getAdjacentSize;
  using Base::getAdjacent;
  using Base::getMirrorIndex;

  // Face accessors.
  using Base::computeFacesSize;
  using Base::getFacesBeginning;
  using Base::getFacesEnd;
  using Base::isOnBoundary;

  // Other Accessors
  using Base::isVertexOnBoundary;

public:

  //
  // Public using the base member functions.
  //

  using Base::getVerticesSize;
  // CONTINUE
#ifdef __INTEL_COMPILER
  //! Return a const reference to the n_th vertex.
  const Point&
  getVertex(const int n) const {
    return Base::getVertex(n);
  }
#else
  using Base::getVertex;
#endif
  using Base::getSimplicesSize;
  using Base::getIndexedSimplex;

  //--------------------------------------------------------------------------
  // \name Constructors, etc.
  //@{
    
  //! Default constructor.  An empty b-rep.
  BRep() : 
    Base(),
    _faceNormals(),
    _vertexNormals(),
    _faceIdentifiers()
  {}

  //! Copy constructor.
  BRep(const BRep& other);

  //! Assignment operator.
  BRep& 
  operator=(const BRep& other);
  
  //! Destructor.
  ~BRep()
  {}
    
  //! Construct from vertices and faces.  Throw away irrelevant ones.
  /*!
    \param verticesSize The number of vertices.
    \param vertices The vertices.
    \param simplicesSize The number of triangle faces.
    \param indexedSimplices The indexed triangle faces.
    \param cartesianDomain is the domain of interest.
    \param maximumDistance is how far the distance will be computed.

    Make the b-rep from vertex coordinates and face indices.
    Clip the b-rep so that faces outside the relevant Cartesian domain 
    are thrown away.  (Any face within \c maximumDistance of 
    \c cartesianDomain is regarded as relevant.)

    This constructor calls make() with the same arguments.
  */
  BRep(const SizeType verticesSize, 
       const void* vertices,
       const SizeType simplicesSize, 
       const void* indexedSimplices,
       const BBox& cartesianDomain,
       const Number maximumDistance) : 
    Base(),
    _faceNormals(),
    _vertexNormals(),
    _faceIdentifiers() {
    make(verticesSize, vertices, simplicesSize, indexedSimplices,
	 cartesianDomain, maximumDistance);
  }

  //! Make from vertices and faces.
  /*!
    \param verticesSize The number of vertices.
    \param vertices The vertices.
    \param simplicesSize The number of triangle faces.
    \param indexedSimplices The indexed triangle faces.
  */
  void 
  make(const SizeType verticesSize, 
       const void* vertices,
       const SizeType simplicesSize, 
       const void* indexedSimplices);

  //! Make from vertices and faces.
  /*!
    \param verticesSize The number of vertices.
    \param vertices The vertices.
    \param simplicesSize The number of triangle faces.
    \param indexedSimplices The indexed triangle faces.
    \param cartesianDomain is the domain of interest.
    \param maximumDistance is how far the distance will be computed.

    Make the b-rep from vertex coordinates and face indices.
    Clip the b-rep so that faces outside the relevant Cartesian domain 
    are thrown away.  (Any face within \c maximumDistance of 
    \c cartesianDomain is regarded as relevant.)
  */
  void 
  make(const SizeType verticesSize, 
       const void* vertices,
       const SizeType simplicesSize, 
       const void* indexedSimplices,
       const BBox& cartesianDomain,
       const Number maximumDistance);


  //@}
  //--------------------------------------------------------------------------
  // \name Size accessors.
  //@{

  //! Return the maximum face identifier.
  int 
  getMaximumFaceIdentifier() const {
    if (_faceIdentifiers.empty()) {
      return -1;
    }
    return *(_faceIdentifiers.end() - 1);
  }

  //@}
  //--------------------------------------------------------------------------
  // \name Mathematical Operations.
  //@{

  //! Calculate the closest point transform with signed distance to this b-rep.
  /*!
    Calculate the signed distance, closest point, etc. to this b-rep for all 
    the points in the grid.

    \param lattice is the lattice on which the grids lie.
    \param grids is the container of grids.  Each one holds the distance, 
    closest point, etc. arrays.
    \param maximumDistance is the distance to calculate distance away from the 
    surface.

    \return
    Return the number of points scan converted and the number of points
    for which the distance was computed.
  */
  std::pair<int,int>
  computeClosestPoint(const Lattice& lattice, 
		      std::vector<Grid>* grids, 
		      Number maximumDistance) const;

  //! Calculate the closest point transform with unsigned distance to this b-rep.
  /*!
    Calculate the unsigned distance, closest point, etc. to this b-rep for all 
    the points in the grid.

    \param lattice is the lattice on which the grids lie.
    \param grids is the container of grids.  Each one holds the distance, 
    closest point, etc. arrays.
    \param maximumDistance is the distance to calculate distance away from the 
    surface.

    \return
    Return the number of points scan converted and the number of points
    for which the distance was set.
  */
  std::pair<int,int>
  computeClosestPointUnsigned(const Lattice& lattice, 
			      std::vector<Grid>* grids, 
			      Number maximumDistance) const;

  //! Use bounding boxes around the characteristic polyhedra instead of polyhedron scan conversion.
  std::pair<int,int>
  computeClosestPointUsingBBox(const Lattice& lattice, 
			       std::vector<Grid>* grids, 
			       Number maximumDistance) const;

  //! Use bounding boxes around the characteristic polyhedra instead of polyhedron scan conversion.
  std::pair<int,int>
  computeClosestPointUnsignedUsingBBox(const Lattice& lattice, 
				       std::vector<Grid>* grids, 
				       Number maximumDistance) const;

  //! Use bounding boxes around the primitives instead of polyhedron scan conversion.
  std::pair<int,int>
  computeClosestPointUsingBruteForce(const Lattice& lattice, 
				     std::vector<Grid>* grids, 
				     Number maximumDistance) const;

  //! Use bounding boxes around the primitives instead of polyhedron scan conversion.
  std::pair<int,int>
  computeClosestPointUnsignedUsingBruteForce(const Lattice& lattice, 
					     std::vector<Grid>* grids, 
					     Number maximumDistance) const;

  //! Return the bounding box that contains the mesh.
  BBox
  computeBBox() const {
    BBox box;
    box.bound(getVerticesBeginning(), getVerticesEnd());
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
  displayInformation(std::ostream& out) const;

  //! Display the b-rep.
  void 
  display(std::ostream& out) const;

  //@}

private:

  //
  // Accessors
  //

  // Make a Face out of the face specified by the index.
  void 
  getFace(int index, FaceDistance* face) const;

  // Make a bounding box around the face specified by the index.
  // Enlarge the bounding box by the maximumDistance.
  void 
  getFaceBBox(int index, Number maximumDistance, BBox* box) const;

  // Make the edge.
  // Return true if this is an interior edge.
  bool 
  getEdge(EdgeIterator iter, EdgeDistance* edge) const;

  // Make the edge.
  // The edge may be in the interior or on the boundary.
  void
  getEdgeUnsigned(EdgeIterator iter, EdgeDistance* edge) const;

  // Make a bounding box around the edge specified by the index.
  // Enlarge the bounding box by the maximumDistance.
  void 
  getEdgeBBox(EdgeIterator iter, Number maximumDistance, 
	      BBox* box) const;

  // Make the vertex.
  // Return true iff the vertex is not on the boundary.
  bool 
  getVertex(int index, VertexDistance* vertex) const; 

  // Make the vertex for computing unsigned distance.
  // Return true if any of the neigboring vertices are known.
  // Otherwise return false.
  bool 
  getVertexUnsigned(int index, VertexDistance* vertex) const; 

  // Make a bounding box around the vertex specified by the index.
  // Enlarge the bounding box by the maximumDistance.
  void 
  getVertexBBox(int index, Number maximumDistance,
		BBox* box) const;
};


//
// Constructors, Destructors
//


template<typename T>
inline
BRep<3,T>::
BRep(const BRep& other) : 
  Base(other),
  _faceNormals(other._faceNormals),
  _vertexNormals(other._vertexNormals),
  _faceIdentifiers(other._faceIdentifiers)
{}


template<typename T>
inline
BRep<3,T>& 
BRep<3,T>::
operator=(const BRep& other) {
  // Avoid assignment to self
  if (&other != this) {
    Base::operator=(other);
    _faceNormals = other._faceNormals;
    _vertexNormals = other._vertexNormals;
    _faceIdentifiers = other._faceIdentifiers;
  }
  // Return *this so assignments can chain
  return *this;
}


// Make the b-rep from vertex coordinates and face indices.
template<typename T>
inline
void 
BRep<3,T>::
make(const SizeType verticesSize, 
     const void* vertices,
     const SizeType simplicesSize, 
     const void* indexedSimplices) {
  // The indexed simplex set.
  Base::build(verticesSize, vertices, simplicesSize, indexedSimplices);
  // The face and vertex normals.
  _faceNormals.resize(simplicesSize);
  _vertexNormals.resize(verticesSize);
  geom::computeSimplexAndVertexNormals(*this, &_faceNormals, 
				       &_vertexNormals);
  // The face identifiers.
  _faceIdentifiers.resize(simplicesSize);
  for (int n = 0; n != _faceIdentifiers.size(); ++n) {
    _faceIdentifiers[n] = n;
  }
}



template<typename T>
inline
void 
BRep<3,T>::
make(const SizeType verticesSize, 
     const void* vertices,
     const SizeType simplicesSize, 
     const void* indexedSimplices,
     const BBox& cartesianDomain,
     const Number maximumDistance) {
  // Determine the bounding box containing the points of interest.
  BBox bbox(cartesianDomain.getLowerCorner() - maximumDistance, 
	    cartesianDomain.getUpperCorner() + maximumDistance);

  // Build a mesh from the vertices and indexed simplices.
  geom::IndSimpSet<3,2,true,T> mesh(verticesSize, vertices, simplicesSize, 
				    indexedSimplices);

  // Determine the set of overlapping faces.
  std::vector<int> overlappingFaces;
  geom::determineOverlappingSimplices
    (mesh, bbox, std::back_inserter(overlappingFaces));

  // Build this mesh from the subset of overlapping faces.
  geom::buildFromSubsetSimplices(mesh, overlappingFaces.begin(), 
				 overlappingFaces.end(), this);

  // The face and vertex normals.
  _faceNormals.resize(getSimplicesSize());
  _vertexNormals.resize(getVerticesSize());
  geom::computeSimplexAndVertexNormals(*this, &_faceNormals, &_vertexNormals);

  // The overlapping faces are the face identifiers.
  _faceIdentifiers.rebuild(overlappingFaces.begin(), overlappingFaces.end());
}


//
// Accesors
//

 
// Make a Face out of the face specified by the index.
template<typename T>
inline
void 
BRep<3,T>::
getFace(const int index, FaceDistance* face) const {
  face->make(getSimplexVertex(index, 0),
	     getSimplexVertex(index, 1),
	     getSimplexVertex(index, 2),
	     _faceNormals[index],
	     _faceIdentifiers[index]);
}


// Make a bounding box around the face specified by the index.
template<typename T>
inline
void 
BRep<3,T>::
getFaceBBox(const int index, const Number maximumDistance, BBox* box) const {
  box->bound(getSimplexVertex(index, 0),
	     getSimplexVertex(index, 1),
	     getSimplexVertex(index, 2));
  box->setLowerCorner(box->getLowerCorner() - maximumDistance);
  box->setUpperCorner(box->getUpperCorner() + maximumDistance);
}


// Make the edge.
// Return true if the edge has two known adjacent faces.
// Otherwise return false.
template<typename T>
inline
bool 
BRep<3,T>::
getEdge(const EdgeIterator iter, EdgeDistance* edge) const {
  // We don't compute signed distance for boundary edges.
  if (isOnBoundary(iter)) {
    return false;
  }
  // The simplex index.
  const int si = iter->first;
  // The local face index.
  const int i = iter->second;
  // Make the edge.
  edge->make(getSimplexVertex(si, (i+1)%3), // source
	     getSimplexVertex(si, (i+2)%3), // target
	     _faceNormals[si], // left normal
	     _faceNormals[getAdjacent(si, i)], // right normal
	     _faceIdentifiers[si]); // face identifier
  return true;
}


// Make the edge.
template<typename T>
inline
void
BRep<3,T>::
getEdgeUnsigned(const EdgeIterator iter, EdgeDistance* edge) const {
  // The simplex index.
  const int si = iter->first;
  // The local face index.
  const int i = iter->second;
  // If this is an interior edge.
  if (getAdjacent(si, i) != -1) {
    // Make the edge.
    edge->make(getSimplexVertex(si, (i+1)%3), // source
	       getSimplexVertex(si, (i+2)%3), // target
	       _faceNormals[si], // left normal
	       _faceNormals[getAdjacent(si, i)], // right normal
	       _faceIdentifiers[si]); // face identifier
  }
  // Else, this is a boundary edge.
  else {
    // Make the edge.
    edge->make(getSimplexVertex(si, (i+1)%3), // source
	       getSimplexVertex(si, (i+2)%3), // target
	       _faceNormals[si], // left normal
	       Point(0.0), // null value for the right normal
	       _faceIdentifiers[si]); // face identifier
  }
}


// Make a bounding box around the edge specified by the index.
template<typename T>
inline
void 
BRep<3,T>::
getEdgeBBox(const EdgeIterator iter, const Number maximumDistance,
	    BBox* box) const {
  // The simplex index.
  const int si = iter->first;
  // The local face index.
  const int i = iter->second;
  box->bound(getSimplexVertex(si, (i+1)%3), // source
	     getSimplexVertex(si, (i+2)%3)); // target
  box->setLowerCorner(box->getLowerCorner() - maximumDistance);
  box->setUpperCorner(box->getUpperCorner() + maximumDistance);
}


// Make the vertex.
// Return true iff the vertex is not on the boundary.
template<typename T>
inline
bool 
BRep<3,T>::
getVertex(const int index, VertexDistance* vert) const {
  // Boundary vertices are not used in computing signed distance.
  if (isVertexOnBoundary(index)) {
    return false;
  }

  // The number of incident triangle faces.
  const int incidentSize = getIncidentSize(index);

  int i;
  // The neighboring vertices.
  std::vector<Point> neighbors(incidentSize);
  // We can get these in any order.
  for (int n = 0; n != incidentSize; ++n) {
    // The face as an indexed simplex.
    const IndexedSimplex& is = getIndexedSimplex(getIncident(index, n));
    // The local index of this vertex in the indexed simplex.
    i = is.getVertexIndex(index);
    // Get the vertex in the positive direction from this one.
    neighbors[n] = getVertex(is[(i+1)%3]);
  }

  // CONTINUE: I don't think they need to be in any particular order.
  // The normals of the adjacent faces. 
  std::vector<Point> normals(incidentSize);  
  // We need to walk around the vertex in the positive direction.
  // The current simplex index.
  int si = getIncident(index, 0);
  for (int n = 0; n != incidentSize; ++n) {
    normals[n] = _faceNormals[si];
    // The local index of this vertex in the indexed simplex.
    i = getIndexedSimplex(si).getVertexIndex(index);
    // Move to the next face.
    si = getAdjacent(si, (i+1)%3);
  }

#ifdef DEBUG_cpt
  // We should have come back to the starting face.
  assert(si == getIncident(index, 0));
#endif

  // Make the vertex.
  vert->make(getVertex(index), _vertexNormals[index], neighbors, normals, 
	     _faceIdentifiers[si]);

  return true;
}


// Make the vertex for computing unsigned distance.
// Return true if any of the neigboring vertices are known.
// Otherwise return false.
template<typename T>
inline
bool 
BRep<3,T>::
getVertexUnsigned(const int index, VertexDistance* vert) const {
  // If there are no incident faces.
  if (isIncidentEmpty(index)) {
    return false;
  }

  // If this vertex is not on the boundary of the surface, we can use the 
  // same function as is used in computing unsigned distance.
  if (! getVertex(index, vert)) {
    // Otherwise, make the vertex without using any b-rep information.
    vert->make(getVertex(index), _faceIdentifiers[getIncident(index, 0)]);
  }

  return true;
}


// Make a bounding box around the vertex specified by the index.
template<typename T>
inline
void 
BRep<3,T>::
getVertexBBox(const int index, const Number maximumDistance, BBox* box) const {
  box->setLowerCorner(getVertex(index) - maximumDistance);
  box->setUpperCorner(getVertex(index) + maximumDistance);
}

//
// Mathematical Operations
//


// CONTINUE: In scan converting, only use points in the lattice.
template<typename T>
inline
std::pair<int,int>
BRep<3,T>::
computeClosestPoint(const Lattice& lattice, 
		    std::vector<Grid>* grids, 
		    const Number maximumDistance) const {
  typedef geom::BBox<3,int> IndexBBox;

  const int gridsSize = int(grids->size());
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  std::vector<Index> indices;
  std::vector<Point> cartesianPoints;
  BBox box;
  IndexBBox indexBox;
  Polyhedron poly;
  geom::IndexedEdgePolyhedron<Number> cartesianPolyhedron;

#ifdef CPT_PERFORMANCE
  std::pair<int,int> countPair;
  ads::Timer timer;
#endif

  // Store index bounding boxes for each grid.
  std::vector<IndexBBox> gridIndexBBoxes(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    gridIndexBBoxes[n].setLowerCorner((*grids)[n].getRanges().lbounds());
    gridIndexBBoxes[n].setUpperCorner((*grids)[n].getRanges().ubounds() - 1);
  }

  // Compute Cartesian bounding boxes around each grid.
  std::vector<BBox> gridDomains(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    lattice.convertBBoxIndicesToLocations(gridIndexBBoxes[n], &gridDomains[n]);
  }

  //
  // Find the closest points and distance for the faces.
  //
  FaceDistance face;
  for (int i = 0; i != getSimplicesSize(); ++i) {
    // Get a bounding box around the face.
    getFaceBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next face.
    if (firstGrid == gridsSize) {
      continue;
    }
#ifdef CPT_PERFORMANCE
    timer.tic();
#endif
    // Get the i_th face.
    getFace(i, &face);
    // Make the polyhedron containing the closest points.
    face.buildCharacteristicPolyhedron(&cartesianPolyhedron, maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
    performance::timeMakeFacePolyhedra += timer.toc();
    timer.tic();
#endif
    // Make the scan conversion polyhedron data structure.
    poly = cartesianPolyhedron;
    // Scan convert the polyhedron.
    indices.clear();
    poly.scanConvert(std::back_inserter(indices), lattice);
    scanConversionCount += int(indices.size());
#ifdef CPT_PERFORMANCE
    performance::countFaceScanConverted += indices.size();
#endif
    // Make an index bounding box around the scan converted points.
    indexBox.bound(indices.begin(), indices.end());
    // Compute the Cartesian coordinates of the scan converted points.
    cartesianPoints.clear();
    std::copy(indices.begin(), indices.end(), 
	      std::back_inserter(cartesianPoints));
    lattice.convertIndicesToLocations(cartesianPoints.begin(), 
				      cartesianPoints.end());
#ifdef CPT_PERFORMANCE
    performance::timeScanConvertFacePolyhedra += timer.toc();
    timer.tic();
#endif
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid points.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (indices, cartesianPoints, face, maximumDistance);
	performance::countFaceDistancesComputed += countPair.first;
	performance::countFaceDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (indices, cartesianPoints, face, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeFaceCpt += timer.toc();
    distanceCount += performance::countFaceDistancesComputed;
#endif
  }

  //
  // Find the closest points and distance for the edges.
  //
  EdgeDistance edge;
  const EdgeIterator edgesEnd = getFacesEnd();
  for (EdgeIterator iter = getFacesBeginning(); iter != edgesEnd; ++iter) {
    // If the adjacent faces are known and the surface is either
    // concave or convex at the edge.
    if (getEdge(iter, &edge) && edge.getSignOfDistance() != 0) {
      // Get a bounding box around the edge.
      getEdgeBBox(iter, maximumDistance, &box);
      // Find the first relevant grid.
      firstGrid = 0;
      for (; firstGrid != gridsSize; ++firstGrid) {
	if (geom::doOverlap(gridDomains[firstGrid], box)) {
	  break;
	}
      }
      // If there are no relevant grids, continue with the next edge.
      if (firstGrid == gridsSize) {
	continue;
      }
#ifdef CPT_PERFORMANCE
      timer.tic();
#endif
      // Make the polyhedron containing the closest points.
      edge.buildCharacteristicPolyhedron(&cartesianPolyhedron, 
					 maximumDistance);
      // Convert the polyhedron to grid coordinates.
      lattice.convertLocationsToIndices
	(cartesianPolyhedron.getVerticesBeginning(),
	 cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
      performance::timeMakeEdgePolyhedra += timer.toc();
      timer.tic();
#endif
      // Make the scan conversion polyhedron data structure.
      poly = cartesianPolyhedron;
      // Scan convert the polyhedron.
      indices.clear();
      poly.scanConvert(std::back_inserter(indices), lattice);
      scanConversionCount += int(indices.size());
#ifdef CPT_PERFORMANCE
      performance::countEdgeScanConverted += indices.size();
#endif
      // Make an index bounding box around the scan converted points.
      indexBox.bound(indices.begin(), indices.end());
      // Compute the Cartesian coordinates of the scan converted points.
      cartesianPoints.clear();
      std::copy(indices.begin(), indices.end(), 
		std::back_inserter(cartesianPoints));
      lattice.convertIndicesToLocations(cartesianPoints.begin(), 
					cartesianPoints.end());
#ifdef CPT_PERFORMANCE
      performance::timeScanConvertEdgePolyhedra += timer.toc();
      timer.tic();
#endif
      // Loop over the grids.
      for (int n = firstGrid; n != gridsSize; ++n) {
	// If the edge could influence this grid.
	if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	  // Compute closest points and distance for scan converted grid pts.
#ifdef CPT_PERFORMANCE
	  countPair = (*grids)[n].computeClosestPointTransform
	    (indices, cartesianPoints, edge, maximumDistance);
	  performance::countEdgeDistancesComputed += countPair.first;
	  performance::countEdgeDistancesSet += countPair.second;
#else
	  distanceCount += (*grids)[n].computeClosestPointTransform
	    (indices, cartesianPoints, edge, maximumDistance).second;
#endif
	}
      }
#ifdef CPT_PERFORMANCE
      performance::timeEdgeCpt += timer.toc();
      distanceCount += performance::countEdgeDistancesComputed;
#endif
    }
  }

  //
  // Find the closest points and distance for the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // If the vertex is not on the boundary.
    if (getVertex(i, &vert)) {
      // Get a bounding box around the vertex.
      getVertexBBox(i, maximumDistance, &box);
      // Find the first relevant grid.
      firstGrid = 0;
      for (; firstGrid != gridsSize; ++firstGrid) {
	if (geom::doOverlap(gridDomains[firstGrid], box)) {
	  break;
	}
      }
      // If there are no relevant grids, continue with the next vertex.
      if (firstGrid == gridsSize) {
	continue;
      }
      indices.clear();
      if (! vert.isConcave()) {
#ifdef CPT_PERFORMANCE
	timer.tic();
#endif
	// Make the polyhedron containing the closest points of positive
	// distance.
	vert.buildCharacteristicPolyhedronPositive(&cartesianPolyhedron, 
						   maximumDistance);
	// Convert the polyhedron to grid coordinates.
	lattice.convertLocationsToIndices
	  (cartesianPolyhedron.getVerticesBeginning(), 
	   cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
	performance::timeMakeVertexPolyhedra += timer.toc();
	timer.tic();
#endif
	// Make the scan conversion polyhedron data structure.
	poly = cartesianPolyhedron;
	// Scan convert the polyhedron.
	poly.scanConvert(std::back_inserter(indices), lattice);
#ifdef CPT_PERFORMANCE
	performance::timeScanConvertVertexPolyhedra += timer.toc();
#endif
      }
      if (! vert.isConvex()) {
#ifdef CPT_PERFORMANCE
	timer.tic();
#endif
	// Make the polyhedron containing the closest points of negative
	// distance.
	vert.buildCharacteristicPolyhedronNegative(&cartesianPolyhedron, 
						   maximumDistance);
	// Convert the polyhedron to grid coordinates.
	lattice.convertLocationsToIndices
	  (cartesianPolyhedron.getVerticesBeginning(), 
	   cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
	performance::timeMakeVertexPolyhedra += timer.toc();
	timer.tic();
#endif
	// Make the scan conversion polyhedron data structure.
	poly = cartesianPolyhedron;
	// Scan convert the polyhedron.
	poly.scanConvert(std::back_inserter(indices), lattice);
#ifdef CPT_PERFORMANCE
	performance::timeScanConvertVertexPolyhedra += timer.toc();
#endif
      }
      scanConversionCount += int(indices.size());
#ifdef CPT_PERFORMANCE
      performance::countVertexScanConverted += indices.size();
      timer.tic();
#endif
      // Make an index bounding box around the scan converted points.
      indexBox.bound(indices.begin(), indices.end());
      // Compute the Cartesian coordinates of the scan converted points.
      cartesianPoints.clear();
      std::copy(indices.begin(), indices.end(), 
		std::back_inserter(cartesianPoints));
      lattice.convertIndicesToLocations(cartesianPoints.begin(), 
					cartesianPoints.end());
#ifdef CPT_PERFORMANCE
      performance::timeScanConvertVertexPolyhedra += timer.toc();
      timer.tic();
#endif

      // Loop over the grids.
      for (int n = firstGrid; n != gridsSize; ++n) {
	// If the vertex could influence this grid.
	if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	  // Compute cpt for scan converted grid points.
#ifdef CPT_PERFORMANCE
	  countPair = (*grids)[n].computeClosestPointTransform
	    (indices, cartesianPoints, vert, maximumDistance);
	  performance::countVertexDistancesComputed += countPair.first;
	  performance::countVertexDistancesSet += countPair.second;
#else
	  distanceCount += (*grids)[n].computeClosestPointTransform
	    (indices, cartesianPoints, vert, maximumDistance).second;
#endif
	}
      }
#ifdef CPT_PERFORMANCE
      performance::timeVertexCpt += timer.toc();
      distanceCount += performance::countVertexDistancesComputed;
#endif
    }
  }

  return std::pair<int,int>(scanConversionCount, distanceCount);
}








template<typename T>
inline
std::pair<int,int>
BRep<3,T>::
computeClosestPointUnsigned(const Lattice& lattice, 
			    std::vector<Grid>* grids, 
			    const Number maximumDistance) const {
  typedef geom::BBox<3,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  std::vector<Index> indices;
  std::vector<Point> cartesianPoints;
  BBox box;
  IndexBBox indexBox;
  Polyhedron poly;
  geom::IndexedEdgePolyhedron<Number> cartesianPolyhedron;

  // Store index bounding boxes for each grid.
  std::vector<IndexBBox> gridIndexBBoxes(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    gridIndexBBoxes[n].setLowerCorner((*grids)[n].getRanges().lbounds());
    gridIndexBBoxes[n].setUpperCorner((*grids)[n].getRanges().ubounds() - 1);
  }

  // Compute Cartesian bounding boxes around each grid.
  std::vector<BBox> gridDomains(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    lattice.convertBBoxIndicesToLocations(gridIndexBBoxes[n], &gridDomains[n]);
  }

  //
  // Find the closest points and distance for the faces.
  //
  FaceDistance face;
  for (int i = 0; i != getSimplicesSize(); ++i) {
    // Get a bounding box around the face.
    getFaceBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next face.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Get the i_th face.
    getFace(i, &face);
    // Make the polyhedron containing the closest points.
    face.buildCharacteristicPolyhedron(&cartesianPolyhedron, maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
    // Make the scan conversion polyhedron data structure.
    poly = cartesianPolyhedron;
    // Scan convert the polyhedron.
    indices.clear();
    poly.scanConvert(std::back_inserter(indices), lattice);
    scanConversionCount += indices.size();
    // Make an index bounding box around the scan converted points.
    indexBox.bound(indices.begin(), indices.end());
    // Compute the Cartesian coordinates of the scan converted points.
    cartesianPoints.clear();
    std::copy(indices.begin(), indices.end(), 
	      std::back_inserter(cartesianPoints));
    lattice.convertIndicesToLocations(cartesianPoints.begin(), 
				      cartesianPoints.end());
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (indices, cartesianPoints, face, maximumDistance).second;
      }
    }
  }

  //
  // Find the closest points and distance for the edges.
  //
  EdgeDistance edge;
  const EdgeIterator edgesEnd = getFacesEnd();
  for (EdgeIterator iter = getFacesBeginning(); iter != edgesEnd; ++iter) {
    // Get the edge.
    getEdgeUnsigned(iter, &edge);
    // Get a bounding box around the edge.
    getEdgeBBox(iter, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next edge.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Make the polyhedron containing the closest points.
    edge.buildCharacteristicPolyhedronUnsigned(&cartesianPolyhedron, 
					       maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
    // Make the scan conversion polyhedron data structure.
    poly = cartesianPolyhedron;
    // Scan convert the polyhedron.
    indices.clear();
    poly.scanConvert(std::back_inserter(indices), lattice);
    scanConversionCount += indices.size();
    // Make an index bounding box around the scan converted points.
    indexBox.bound(indices.begin(), indices.end());
    // Compute the Cartesian coordinates of the scan converted points.
    cartesianPoints.clear();
    std::copy(indices.begin(), indices.end(), 
	      std::back_inserter(cartesianPoints));
    lattice.convertIndicesToLocations(cartesianPoints.begin(), 
				      cartesianPoints.end());
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the edge could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid pts.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (indices, cartesianPoints, edge, maximumDistance).second;
      }
    }
  }

  //
  // Find the closest points and distance for the vertices.
  //
  BBox bbox;

  // The closed index range of the grid.
  geom::BBox<3,int> latticeIndexRange;
  latticeIndexRange.setLowerCorner(Index(0,0,0));
  latticeIndexRange.setUpperCorner(lattice.getExtents() - 1);
  
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // Don't compute the distance if none of the neighboring vertices 
    // are known.
    if (! getVertexUnsigned(i, &vert)) {
      continue;
    }
    // Get a bounding box around the vertex.
    getVertexBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }
    indices.clear();

    // If this is an interior vertex.
    if (! vert.getEdgeDirections().empty()) {
      if (! vert.isConcave()) {
	// Make the polyhedron containing the closest points of positive
	// distance.
	vert.buildCharacteristicPolyhedronPositive(&cartesianPolyhedron, 
						   maximumDistance);
	// Convert the polyhedron to grid coordinates.
	lattice.convertLocationsToIndices
	  (cartesianPolyhedron.getVerticesBeginning(), 
	   cartesianPolyhedron.getVerticesEnd());
	// Make the scan conversion polyhedron data structure.
	poly = cartesianPolyhedron;
	// Scan convert the polyhedron.
	poly.scanConvert(std::back_inserter(indices), lattice);
      }
      if (! vert.isConvex()) {
	// Make the polyhedron containing the closest points of negative
	// distance.
	vert.buildCharacteristicPolyhedronNegative(&cartesianPolyhedron, 
						   maximumDistance);
	// Convert the polyhedron to grid coordinates.
	lattice.convertLocationsToIndices
	  (cartesianPolyhedron.getVerticesBeginning(), 
	   cartesianPolyhedron.getVerticesEnd());
	// Make the scan conversion polyhedron data structure.
	poly = cartesianPolyhedron;
	// Scan convert the polyhedron.
	poly.scanConvert(std::back_inserter(indices), lattice);
      }
    }
    else { // If this is a boundary vertex.
      // Make the bounding box containing the closest points.
      bbox.setCorners(vert.getLocation() - maximumDistance,
		      vert.getLocation() + maximumDistance);
      lattice.convertBBoxLocationsToIndices(&bbox);
      // Scan convert the bounding box.
      geom::scanConvert(std::back_inserter(indices), bbox, 
			latticeIndexRange);
    }

    scanConversionCount += indices.size();
    // Make an index bounding box around the scan converted points.
    indexBox.bound(indices.begin(), indices.end());
    // Compute the Cartesian coordinates of the scan converted points.
    cartesianPoints.clear();
    std::copy(indices.begin(), indices.end(), 
	      std::back_inserter(cartesianPoints));
    lattice.convertIndicesToLocations(cartesianPoints.begin(), 
				      cartesianPoints.end());

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the vertex could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute the cpt for the scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (indices, cartesianPoints, vert, maximumDistance).second;
      }
    }
  }

  return std::pair<int,int>(scanConversionCount, distanceCount);
}










template<typename T>
inline
std::pair<int,int>
BRep<3,T>::
computeClosestPointUsingBBox(const Lattice& lattice, 
			     std::vector<Grid>* grids, 
			     const Number maximumDistance) const {
  typedef geom::BBox<3,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box, characteristicBox;
  Range indexRange;
  IndexBBox indexBox;
  geom::IndexedEdgePolyhedron<Number> cartesianPolyhedron;

#ifdef CPT_PERFORMANCE
  std::pair<int,int> countPair;
  ads::Timer timer;
#endif

  // Store index bounding boxes for each grid.
  std::vector<IndexBBox> gridIndexBBoxes(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    gridIndexBBoxes[n].setLowerCorner((*grids)[n].getRanges().lbounds());
    gridIndexBBoxes[n].setUpperCorner((*grids)[n].getRanges().ubounds() - 1);
  }

  // Compute Cartesian bounding boxes around each grid.
  std::vector<BBox> gridDomains(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    lattice.convertBBoxIndicesToLocations(gridIndexBBoxes[n], &gridDomains[n]);
  }

  //
  // Find the closest points and distance for the faces.
  //
  FaceDistance face;
  for (int i = 0; i != getSimplicesSize(); ++i) {
    // Get a bounding box around the face.
    getFaceBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next face.
    if (firstGrid == gridsSize) {
      continue;
    }
#ifdef CPT_PERFORMANCE
    timer.tic();
#endif
    // Get the i_th face.
    getFace(i, &face);
    // Make the polyhedron containing the closest points.
    face.buildCharacteristicPolyhedron(&cartesianPolyhedron, maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
    performance::timeMakeFacePolyhedra += timer.toc();
#endif
    // Make a bounding box around the polyhedra.
    characteristicBox.bound(cartesianPolyhedron.getVerticesBeginning(), cartesianPolyhedron.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(characteristicBox.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(characteristicBox.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
#ifdef CPT_PERFORMANCE
    performance::countFaceScanConverted += indexRange.content();
    timer.tic();
#endif
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for the grid points in the 
	// index range.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, face, maximumDistance);
	performance::countFaceDistancesComputed += countPair.first;
	performance::countFaceDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, face, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeFaceCpt += timer.toc();
    distanceCount += performance::countFaceDistancesComputed;
#endif
  }

  //
  // Find the closest points and distance for the edges.
  //
  EdgeDistance edge;
  const EdgeIterator edgesEnd = getFacesEnd();
  for (EdgeIterator iter = getFacesBeginning(); iter != edgesEnd; ++iter) {
    // Only compute the distance if the adjacent faces are known and the 
    // surface is either concave or convex at the edge.
    if (! getEdge(iter, &edge) || edge.getSignOfDistance() == 0) {
      continue;
    }
    // Get a bounding box around the edge.
    getEdgeBBox(iter, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next edge.
    if (firstGrid == gridsSize) {
      continue;
    }
#ifdef CPT_PERFORMANCE
    timer.tic();
#endif
    // Make the polyhedron containing the closest points.
    edge.buildCharacteristicPolyhedron(&cartesianPolyhedron, maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
    performance::timeMakeEdgePolyhedra += timer.toc();
#endif
    // Make a bounding box around the polyhedra.
    characteristicBox.bound(cartesianPolyhedron.getVerticesBeginning(), cartesianPolyhedron.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(characteristicBox.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(characteristicBox.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
#ifdef CPT_PERFORMANCE
    performance::countEdgeScanConverted += indexRange.content();
    timer.tic();
#endif
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the edge could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid pts.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, edge, maximumDistance);
	performance::countEdgeDistancesComputed += countPair.first;
	performance::countEdgeDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, edge, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeEdgeCpt += timer.toc();
    distanceCount += performance::countEdgeDistancesComputed;
#endif
  }

  //
  // Find the closest points and distance for the vertices.
  //
  BBox positiveBox, negativeBox;
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // Don't compute the distance if the vertex is on the boundary.
    if (! getVertex(i, &vert)) {
      continue;
    }
    // Get a bounding box around the vertex.
    getVertexBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }
    if (! vert.isConcave()) {
#ifdef CPT_PERFORMANCE
      timer.tic();
#endif
      // Make the polyhedron containing the closest points of positive
      // distance.
      vert.buildCharacteristicPolyhedronPositive(&cartesianPolyhedron, 
						 maximumDistance);
      // Convert the polyhedron to grid coordinates.
      lattice.convertLocationsToIndices
	(cartesianPolyhedron.getVerticesBeginning(), 
	 cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
      performance::timeMakeVertexPolyhedra += timer.toc();
#endif
      // Make a bounding box around the polyhedra.
      positiveBox.bound(cartesianPolyhedron.getVerticesBeginning(), 
			cartesianPolyhedron.getVerticesEnd());
    }
    if (! vert.isConvex()) {
#ifdef CPT_PERFORMANCE
      timer.tic();
#endif
      // Make the polyhedron containing the closest points of negative
      // distance.
      vert.buildCharacteristicPolyhedronNegative(&cartesianPolyhedron, 
						 maximumDistance);
      // Convert the polyhedron to grid coordinates.
      lattice.convertLocationsToIndices
	(cartesianPolyhedron.getVerticesBeginning(), 
	 cartesianPolyhedron.getVerticesEnd());
#ifdef CPT_PERFORMANCE
      performance::timeMakeVertexPolyhedra += timer.toc();
#endif
      // Make a bounding box around the polyhedra.
      negativeBox.bound(cartesianPolyhedron.getVerticesBeginning(), 
			cartesianPolyhedron.getVerticesEnd());
    }
    if (! vert.isConcave() && ! vert.isConvex()) {
      characteristicBox = positiveBox;
      characteristicBox.add(negativeBox.getLowerCorner());
      characteristicBox.add(negativeBox.getUpperCorner());
    }
    else if (! vert.isConcave()) {
      characteristicBox = positiveBox;
    }
    else if (! vert.isConvex()) {
      characteristicBox = negativeBox;
    }
    else {
      continue;
    }
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(characteristicBox.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(characteristicBox.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
#ifdef CPT_PERFORMANCE
    performance::countVertexScanConverted += indexRange.content();
    timer.tic();
#endif

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the vertex could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute cpt for scan converted grid points.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, vert, maximumDistance);
	performance::countVertexDistancesComputed += countPair.first;
	performance::countVertexDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, vert, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeVertexCpt += timer.toc();
    distanceCount += performance::countVertexDistancesComputed;
#endif
  }

  return std::pair<int,int>(scanConversionCount, distanceCount);
}










template<typename T>
inline
std::pair<int,int>
BRep<3,T>::
computeClosestPointUnsignedUsingBBox(const Lattice& lattice, 
				     std::vector<Grid>* grids, 
				     const Number maximumDistance) const {
  typedef geom::BBox<3,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box, characteristicBox;
  Range indexRange;
  IndexBBox indexBox;
  geom::IndexedEdgePolyhedron<Number> cartesianPolyhedron;

  // Store index bounding boxes for each grid.
  std::vector<IndexBBox> gridIndexBBoxes(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    gridIndexBBoxes[n].setLowerCorner((*grids)[n].getRanges().lbounds());
    gridIndexBBoxes[n].setUpperCorner((*grids)[n].getRanges().ubounds() - 1);
  }

  // Compute Cartesian bounding boxes around each grid.
  std::vector<BBox> gridDomains(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    lattice.convertBBoxIndicesToLocations(gridIndexBBoxes[n], &gridDomains[n]);
  }

  //
  // Find the closest points and distance for the faces.
  //
  FaceDistance face;
  for (int i = 0; i != getSimplicesSize(); ++i) {
    // Get a bounding box around the face.
    getFaceBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next face.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Get the i_th face.
    getFace(i, &face);
    // Make the polyhedron containing the closest points.
    face.buildCharacteristicPolyhedron(&cartesianPolyhedron, maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
    // Make a bounding box around the polyhedra.
    characteristicBox.bound(cartesianPolyhedron.getVerticesBeginning(), 
			    cartesianPolyhedron.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(characteristicBox.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(characteristicBox.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for the grid points in the 
	// index range.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, face, maximumDistance).second;
      }
    }
  }

  //
  // Find the closest points and distance for the edges.
  //
  EdgeDistance edge;
  const EdgeIterator edgesEnd = getFacesEnd();
  for (EdgeIterator iter = getFacesBeginning(); iter != edgesEnd; ++iter) {
    // Get the edge.
    getEdgeUnsigned(iter, &edge);
    // Get a bounding box around the edge.
    getEdgeBBox(iter, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next edge.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Make the polyhedron containing the closest points.
    edge.buildCharacteristicPolyhedronUnsigned(&cartesianPolyhedron, 
					       maximumDistance);
    // Convert the polyhedron to grid coordinates.
    lattice.convertLocationsToIndices
      (cartesianPolyhedron.getVerticesBeginning(), 
       cartesianPolyhedron.getVerticesEnd());
    // Make a bounding box around the polyhedra.
    characteristicBox.bound(cartesianPolyhedron.getVerticesBeginning(), 
			    cartesianPolyhedron.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(characteristicBox.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(characteristicBox.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the edge could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid pts.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, edge, maximumDistance).second;
      }
    }
  }

  //
  // Find the closest points and distance for the vertices.
  //

  BBox positiveBox, negativeBox;
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // Don't compute the distance if none of the neighboring vertices 
    // are known.
    if (! getVertexUnsigned(i, &vert)) {
      continue;
    }
    // Get a bounding box around the vertex.
    getVertexBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // If this is an interior vertex.
    if (! vert.getEdgeDirections().empty()) {
      if (! vert.isConcave()) {
	// Make the polyhedron containing the closest points of positive
	// distance.
	vert.buildCharacteristicPolyhedronPositive(&cartesianPolyhedron, 
						   maximumDistance);
	// Convert the polyhedron to grid coordinates.
	lattice.convertLocationsToIndices
	  (cartesianPolyhedron.getVerticesBeginning(), 
	   cartesianPolyhedron.getVerticesEnd());
	// Make a bounding box around the polyhedra.
	positiveBox.bound(cartesianPolyhedron.getVerticesBeginning(), 
			  cartesianPolyhedron.getVerticesEnd());
      }
      if (! vert.isConvex()) {
	// Make the polyhedron containing the closest points of negative
	// distance.
	vert.buildCharacteristicPolyhedronNegative(&cartesianPolyhedron, 
						   maximumDistance);
	// Convert the polyhedron to grid coordinates.
	lattice.convertLocationsToIndices
	  (cartesianPolyhedron.getVerticesBeginning(), 
	   cartesianPolyhedron.getVerticesEnd());
	// Make a bounding box around the polyhedra.
	negativeBox.bound(cartesianPolyhedron.getVerticesBeginning(), 
			  cartesianPolyhedron.getVerticesEnd());
      }
      if (! vert.isConcave() && ! vert.isConvex()) {
	characteristicBox = positiveBox;
	characteristicBox.add(negativeBox.getLowerCorner());
	characteristicBox.add(negativeBox.getUpperCorner());
      }
      else if (! vert.isConcave()) {
	characteristicBox = positiveBox;
      }
      else if (! vert.isConvex()) {
	characteristicBox = negativeBox;
      }
      else {
	continue;
      }
    }
    else { // If this is a boundary vertex.
      // Start with the box of radius maximumDistance around the vertex.
      // Then convert to grid coordinates.
      characteristicBox = box;
      lattice.convertBBoxLocationsToIndices(&characteristicBox);
    }

    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(characteristicBox.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(characteristicBox.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the vertex could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute the cpt for the scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, vert, maximumDistance).second;
      }
    }
  }

  return std::pair<int,int>(scanConversionCount, distanceCount);
}






template<typename T>
inline
std::pair<int,int>
BRep<3,T>::
computeClosestPointUsingBruteForce(const Lattice& lattice, 
				   std::vector<Grid>* grids, 
				   const Number maximumDistance) const {
  typedef geom::BBox<3,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box;
  Range indexRange;
  IndexBBox indexBox;
  geom::IndexedEdgePolyhedron<Number> cartesianPolyhedron;

#ifdef CPT_PERFORMANCE
  std::pair<int,int> countPair;
  ads::Timer timer;
#endif

  // Store index bounding boxes for each grid.
  std::vector<IndexBBox> gridIndexBBoxes(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    gridIndexBBoxes[n].setLowerCorner((*grids)[n].getRanges().lbounds());
    gridIndexBBoxes[n].setUpperCorner((*grids)[n].getRanges().ubounds() - 1);
  }

  // Compute Cartesian bounding boxes around each grid.
  std::vector<BBox> gridDomains(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    lattice.convertBBoxIndicesToLocations(gridIndexBBoxes[n], &gridDomains[n]);
  }

  //
  // Find the closest points and distance for the faces.
  //
  FaceDistance face;
  for (int i = 0; i != getSimplicesSize(); ++i) {
    // Get a bounding box around the face.
    getFaceBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next face.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Get the i_th face.
    getFace(i, &face);
    // Convert the face bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
#ifdef CPT_PERFORMANCE
    performance::countFaceScanConverted += indexRange.content();
    timer.tic();
#endif
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for the grid points in the 
	// index range.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, face, maximumDistance);
	performance::countFaceDistancesComputed += countPair.first;
	performance::countFaceDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, face, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeFaceCpt += timer.toc();
    distanceCount += performance::countFaceDistancesComputed;
#endif
  }

  //
  // Find the closest points and distance for the edges.
  //
  EdgeDistance edge;
  const EdgeIterator edgesEnd = getFacesEnd();
  for (EdgeIterator iter = getFacesBeginning(); iter != edgesEnd; ++iter) {
    // Only compute the distance if the adjacent faces are known and the 
    // surface is either concave or convex at the edge.
    if (! getEdge(iter, &edge) || edge.getSignOfDistance() == 0) {
      continue;
    }
    // Get a bounding box around the edge.
    getEdgeBBox(iter, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next edge.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Convert the edge bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
#ifdef CPT_PERFORMANCE
    performance::countEdgeScanConverted += indexRange.content();
    timer.tic();
#endif
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the edge could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid pts.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, edge, maximumDistance);
	performance::countEdgeDistancesComputed += countPair.first;
	performance::countEdgeDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, edge, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeEdgeCpt += timer.toc();
    distanceCount += performance::countEdgeDistancesComputed;
#endif
  }

  //
  // Find the closest points and distance for the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // Don't compute the distance if the vertex is on the boundary.
    if (! getVertex(i, &vert)) {
      continue;
    }
    // Get a bounding box around the vertex.
    getVertexBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Convert the vertex bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
#ifdef CPT_PERFORMANCE
    performance::countVertexScanConverted += indexRange.content();
    timer.tic();
#endif

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the vertex could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute cpt for scan converted grid points.
#ifdef CPT_PERFORMANCE
	countPair = (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, vert, maximumDistance);
	performance::countVertexDistancesComputed += countPair.first;
	performance::countVertexDistancesSet += countPair.second;
#else
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, vert, maximumDistance).second;
#endif
      }
    }
#ifdef CPT_PERFORMANCE
    performance::timeVertexCpt += timer.toc();
    distanceCount += performance::countVertexDistancesComputed;
#endif
  }

  return std::pair<int,int>(scanConversionCount, distanceCount);
}










template<typename T>
inline
std::pair<int,int>
BRep<3,T>::
computeClosestPointUnsignedUsingBruteForce(const Lattice& lattice, 
					   std::vector<Grid>* grids, 
					   const Number maximumDistance) const {
  typedef geom::BBox<3,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box;
  Range indexRange;
  IndexBBox indexBox;
  geom::IndexedEdgePolyhedron<Number> cartesianPolyhedron;

  // Store index bounding boxes for each grid.
  std::vector<IndexBBox> gridIndexBBoxes(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    gridIndexBBoxes[n].setLowerCorner((*grids)[n].getRanges().lbounds());
    gridIndexBBoxes[n].setUpperCorner((*grids)[n].getRanges().ubounds() - 1);
  }

  // Compute Cartesian bounding boxes around each grid.
  std::vector<BBox> gridDomains(gridsSize);
  for (int n = 0; n != gridsSize; ++n) {
    lattice.convertBBoxIndicesToLocations(gridIndexBBoxes[n], &gridDomains[n]);
  }

  //
  // Find the closest points and distance for the faces.
  //
  FaceDistance face;
  for (int i = 0; i != getSimplicesSize(); ++i) {
    // Get a bounding box around the face.
    getFaceBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next face.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Get the i_th face.
    getFace(i, &face);
    // Convert the face bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for the grid points in the 
	// index range.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, face, maximumDistance).second;
      }
    }
  }

  //
  // Find the closest points and distance for the edges.
  //
  EdgeDistance edge;
  const EdgeIterator edgesEnd = getFacesEnd();
  for (EdgeIterator iter = getFacesBeginning(); iter != edgesEnd; ++iter) {
    // Get the edge.
    getEdgeUnsigned(iter, &edge);
    // Get a bounding box around the edge.
    getEdgeBBox(iter, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next edge.
    if (firstGrid == gridsSize) {
      continue;
    }
    // Convert the edge bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();
    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the edge could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid pts.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, edge, maximumDistance).second;
      }
    }
  }

  //
  // Find the closest points and distance for the vertices.
  //

  BBox positiveBox, negativeBox;
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // Don't compute the distance if none of the neighboring vertices 
    // are known.
    if (! getVertexUnsigned(i, &vert)) {
      continue;
    }
    // Get a bounding box around the vertex.
    getVertexBBox(i, maximumDistance, &box);
    // Find the first relevant grid.
    firstGrid = 0;
    for (; firstGrid != gridsSize; ++firstGrid) {
      if (geom::doOverlap(gridDomains[firstGrid], box)) {
	break;
      }
    }
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Convert the vertex bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 3; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the vertex could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute the cpt for the scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, vert, maximumDistance).second;
      }
    }
  }

  return std::pair<int,int>(scanConversionCount, distanceCount);
}







//
// File I/O
//

template<typename T>
inline
void 
BRep<3,T>::
displayInformation(std::ostream& out) const {
  geom::printInformation(out, *this);
}

template<typename T>
inline
void 
BRep<3,T>::
display(std::ostream& out) const {
  geom::writeAscii(out, *this);
  out << _faceNormals << _vertexNormals << _faceIdentifiers;
}

//
// Private Member Functions
//

//
// This grid function is defined here because it uses BRep.
//

// CONTINUE
#if 0
template<typename T>
template<bool A1, bool A2>
inline
void 
Grid<3,T>::
flood_fill(Number far_away,
	   const ads::Array<1,const Point,A1>& vertices,
	   const ads::Array<1,const Index,A2>& faces)
{
  // First try to determine the sign of the distance from the known distances
  // and then flood fill.
  if (flood_fill(far_away)) {
    return;
  }
  // If the above did not succeed then there are no known distances.

  // Ensure that the mesh is not degenerate.
  assert(vertices.size() != 0 && faces.size() != 0);

  // Find the Cartesian location of one of the grid points.
  Point location(0, 0, 0);
  index_to_location(location);

  // We will find the closest point on the mesh to the grid location.

  // Compute the distances to the vertices.
  ads::Array<1,Number> vertex_distance(vertices.size());
  for (int i = 0; i != vertices.size(); ++i) {
    vertex_distance[i] = geom::distance(location, vertices[i]);
  }

  // Find the vertex that is closest to the grid location.
  // We use this to determine an upper bound on the distance.
  const Number upper_bound_distance = ads::min(vertex_distance);

  // Determine the faces that are relevant.
  std::vector<Index> close_faces;
  {
    Number max_edge_length, min_vertex_distance;
    for (int i = 0; i != faces.size(); ++i) {
      max_edge_length = 
	ads::max(geom::distance(vertices[faces[i][0]], 
				vertices[faces[i][1]]),
		 geom::distance(vertices[faces[i][1]], 
				vertices[faces[i][2]]),
		 geom::distance(vertices[faces[i][2]], 
				vertices[faces[i][0]]));
      min_vertex_distance = ads::min(vertex_distance[faces[i][0]],
				     vertex_distance[faces[i][1]],
				     vertex_distance[faces[i][2]]);
      if (min_vertex_distance <= upper_bound_distance + max_edge_length) {
	close_faces.push_back(faces[i]);
      }
    }
  }

  // Make a set of the vertex indices that comprise the close faces.
  std::set<int> index_set;
  {
    const int sz = close_faces.size();
    for (int i = 0; i != sz; ++i) {
      index_set.insert(close_faces[i][0]);
      index_set.insert(close_faces[i][1]);
      index_set.insert(close_faces[i][2]);
    }
  }
  std::vector<int> close_vertex_indices(index_set.begin(), 
					index_set.end());

  // Make an array of the close vertices.
  ads::Array<1,Point> close_vertices(close_vertex_indices.size());
  {
    const int sz = close_vertex_indices.size();
    for (int i = 0; i != sz; ++i) {
      close_vertices[i] = vertices[close_vertex_indices[i]];
    }
  }

  // Adjust the indices of the close faces.
  {
    const int sz = close_faces.size();
    for (int i = 0; i != sz; ++i) {
      for (int j = 0; j != 3; ++j) {
	close_faces[i][j] = 
	  std::lower_bound(close_vertex_indices.begin(), 
			   close_vertex_indices.end(), close_faces[i][j]) -
	  close_vertex_indices.begin();
      }
    }
  }

  // Make a b-rep from the close faces.
  BRep<3,Number> brep;
  brep.make(close_vertices.begin(), close_vertices.end(), 
	    close_faces.begin(), close_faces.end());

  // Make one grid with a single point.
  BBox dom(location, location);
  ads::Array<3,Number> dist(1, 1, 1);
  ads::Array<3,Point> gd;
  ads::Array<3,Point> cp;
  ads::Array<3,int> cf;
  std::vector<Grid> grids;
  grids.push_back(Grid(dom, dist, gd, cp, cf));
  
  // Compute the distance from the grid point to the mesh.
  grids[0].initialize();
  brep.computeClosestPoint(grids, upper_bound_distance * 1.1);
  Number d = dist(0, 0, 0);

  // Set the distance to +- far_away.
  assert(d != std::numeric_limits<Number>::max());
  int sign_distance = (d >= 0 ? 1 : -1);
  distance() = sign_distance * far_away;
}
#endif

END_NAMESPACE_CPT
