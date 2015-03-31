// -*- C++ -*-

#if !defined(__cpt_BRep2_ipp__)
#error This file is an implementation detail of the class BRep.
#endif

BEGIN_NAMESPACE_CPT

//! A class for a b-rep in 2-D.
template<typename T>
class BRep<2,T> :
  public geom::IndSimpSetIncAdj<2,1,true,T> {
  // 
  // Private types.
  //

private:

  //! The base is an indexed simplex set with incidence and adjacency information.
  typedef geom::IndSimpSetIncAdj<2,1,true,T> Base;

  // 
  // Public types.
  //

public:

  //! The size type.
  typedef typename Base::SizeType SizeType;
  //! The number type.
  typedef T Number;
  //! A point in 2-D.
  typedef ads::FixedArray<2,Number> Point;
  //! A multi-index in 2-D.
  typedef ads::FixedArray<2,int> Index;
  //! A bounding box.
  typedef geom::BBox<2,Number> BBox;
  // CONTINUE REMOVE
  //! Three indices determine a face.
  //typedef ads::FixedArray<2,int> IndexedFace;
  //! The lattice.
  typedef geom::RegularGrid<2,T> Lattice;
  //! The grid type.
  typedef Grid<2,Number> Grid;
    
  // 
  // Private types.
  //

private:

  typedef cpt::Vertex<2,T> VertexDistance;
  typedef cpt::Face<2,T> FaceDistance;
  // CONTINUE REMOVE
  //! The segment type.
  //typedef geom::Segment<2,T> Segment;
  //! The polygon type.
  typedef geom::ScanConversionPolygon<T> Polygon;
  //! An index range.
  typedef typename Grid::Range Range;

  //
  // Member data.
  //

private:

  //! The unit outward normals of the faces.
  ads::Array<1,Point> _faceNormals;
  //! The face identifiers.
  ads::Array<1,int> _faceIdentifiers;

  //
  // Private using the base member functions.
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
  // \name Constructors etc.
  //@{
    
  //! Default constructor.  An empty b-rep.
  BRep() : 
    Base(),
    _faceNormals(),
    _faceIdentifiers()
  {}

  //! Copy constructor.
  BRep(const BRep& other);

  //! Assignment operator.
  BRep& 
  operator=(const BRep& other);
  
  //! Trivial destructor.
  ~BRep()
  {}
    
  //! Make from vertices and faces.  Throw away irrelevant faces.
  /*!
    \param verticesSize The number of vertices.
    \param vertices The vertices.
    \param simplicesSize The number of faces.
    \param indexedSimplices The indexed faces.
    \param cartesianDomain is the domain of interest.
    \param maximumDistance is how far the distance will be computed.

    Make the b-rep from vertex coordinates and face indices.
    Clip the b-rep so that faces outside the relevant Cartesian domain 
    are thrown away.  (Any face within \c maximumDistance of 
    \c cartesianDomain is regarded as relevant.)

    This constructor calls \c make() with the same arguments.
  */  
  BRep(const SizeType verticesSize, 
       const void* vertices,
       const SizeType simplicesSize, 
       const void* indexedSimplices,
       const BBox& cartesianDomain,
       const Number maximumDistance) : 
    Base(),
    _faceNormals(),
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
    \param simplicesSize The number of faces.
    \param indexedSimplices The indexed faces.
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
  getMaximimFaceIdentifier() const {
    if (_faceIdentifiers.empty()) {
      return -1;
    }
    return *(_faceIdentifiers.end() - 1);
  }

  //@}
  //--------------------------------------------------------------------------
  // \name Accessors for the vertices.
  //@{

  int 
  getRightFace(const int vertexIndex) const {
    if (getIndexedSimplex(getIncident(vertexIndex, 0))[1] == vertexIndex) {
      return getIncident(vertexIndex, 0);
    }
    if (getIncidentSize(vertexIndex) == 2) {
      return getIncident(vertexIndex, 1);
    }
    return -1;
  }

  int 
  getLeftFace(const int vertexIndex) const {
    if (getIndexedSimplex(getIncident(vertexIndex, 0))[0] == vertexIndex) {
      return getIncident(vertexIndex, 0);
    }
    if (getIncidentSize(vertexIndex) == 2) {
      return getIncident(vertexIndex, 1);
    }
    return -1;
  }

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
  // \name Mathematical Operations
  //@{

  //! Calculate the signed distance, closest point, etc. for all the points in the grid.
  /*!
    \param lattice is the lattice on which the grids lie.
    \param grids is the container of grids.  Each one holds the distance, 
    closest point, etc. arrays.
    \param maximumDistance is the distance to calculate distance away from the 
    curve.
    \param arePerformingLocalClipping determines whether local clipping 
    will be performed on the characteristic polygons.
    \param arePerformingGlobalClipping can be 0, 1 or 2.  0 indicates that 
    no global clipping will be done.  1 indicates limited global clipping; 
    2 indicates full global clipping.
    \param globalPoints is the set of points used in global clipping.

    \return the number of points scan converted (counting multiplicities) 
    and the number of distances set.
  */
  std::pair<int,int>
  computeClosestPoint(const Lattice& lattice, 
		      std::vector<Grid>* grids, Number maximumDistance,
		      bool arePerformingLocalClipping, 
		      int arePerformingGlobalClipping, 
		      const std::vector<Point>& globalPoints) const;

  //! Calculate the unsigned distance, closest point, etc. for all the points in the grid.
  /*!
    \param lattice is the lattice on which the grids lie.
    \param grids is the container of grids.  Each one holds the distance, 
    closest point, etc. arrays.
    \param maximumDistance is the distance to calculate distance away from the 
    curve.
    \param arePerformingLocalClipping determines whether local clipping will be performed
    on the characteristic polygons.
    \param arePerformingGlobalClipping can be 0, 1 or 2.  0 indicates that no global 
    clipping will be done.  1 indicates limited global clipping; 2 indicates
    full global clipping.
    \param globalPoints is the set of points used in global clipping.

    \return the number of points scan converted (counting multiplicities) 
    and the number of distances set.
  */
  std::pair<int,int>
  computeClosestPointUnsigned(const Lattice& lattice, 
			      std::vector<Grid>* grids, 
			      Number maximumDistance, 
			      bool arePerformingLocalClipping, 
			      int arePerformingGlobalClipping,
			      const std::vector<Point>& globalPoints) const;

  //! Use bounding boxes around the characteristic polygons instead of polygon scan conversion.
  std::pair<int,int>
  computeClosestPointUsingBBox(const Lattice& lattice, 
			       std::vector<Grid>* grids, 
			       Number maximumDistance) const;

  //! Use bounding boxes around the characteristic polygons instead of polygon scan conversion.
  std::pair<int,int>
  computeClosestPointUnsignedUsingBBox(const Lattice& lattice, 
				       std::vector<Grid>* grids, 
				       Number maximumDistance) const;

  //! Use bounding boxes around the primitives instead of polygon scan conversion.
  std::pair<int,int>
  computeClosestPointUsingBruteForce(const Lattice& lattice, 
				     std::vector<Grid>* grids, 
				     Number maximumDistance) const;

  //! Use bounding boxes around the primitives instead of polygon scan conversion.
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
  // Private member functions.
  //

  //! Make a vertex for computing the signed distance.
  /*! 
    If the vertex has two neighboring faces, make the vertex and return true.
    Otherwise return false.
  */
  bool
  getVertex(int index, VertexDistance* vertex) const;

  //! Make a vertex for computing the unsigned distance.
  /*! 
    If the vertex has one or more neighboring faces, make the vertex and 
    return true. Otherwise return false.
  */
  bool
  getVertexUnsigned(int index, VertexDistance* vertex) const;

  // Make a bounding box around the vertex specified by the index.
  // Enlarge the bounding box by the maximumDistance.
  void 
  getVertexBBox(int index, Number maximumDistance, BBox* box) const;

  //! Make a face.
  void 
  getFace(int index, FaceDistance* face) const;

  // Make a bounding box around the face specified by the index.
  // Enlarge the bounding box by the maximumDistance.
  void 
  getFaceBBox(int index, Number maximumDistance, BBox* box) const;
};





template<typename T>
inline
BRep<2,T>::
BRep(const BRep& other) : 
  Base(other),
  _faceNormals(other._faceNormals),
  _faceIdentifiers(other._faceIdentifiers)
{}

template<typename T>
inline
BRep<2,T>& 
BRep<2,T>::
operator=(const BRep& other) {
  // Avoid assignment to self
  if (&other != this) {
    Base::operator=(other);
    _faceNormals = other._faceNormals;
    _faceIdentifiers = other._faceIdentifiers;
  }
  // Return *this so assignments can chain
  return *this;
}


template<typename T>
inline
void 
BRep<2,T>::
make(const SizeType verticesSize, const void* vertices,
     const SizeType simplicesSize, const void* indexedSimplices) {
  // The indexed simplex set.
  Base::build(verticesSize, vertices, simplicesSize, indexedSimplices);
  // The face normals.
  _faceNormals.resize(simplicesSize);
  geom::computeSimplexNormals(*this, &_faceNormals);
  // The face identifiers.
  _faceIdentifiers.resize(simplicesSize);
  for (int n = 0; n != _faceIdentifiers.size(); ++n) {
    _faceIdentifiers[n] = n;
  }
}


template<typename T>
inline
void 
BRep<2,T>::
make(const SizeType verticesSize, const void* vertices,
     const SizeType simplicesSize, const void* indexedSimplices,
     const BBox& cartesianDomain, const Number maximumDistance) {
  // Determine the bounding box containing the points of interest.
  BBox bbox(cartesianDomain.getLowerCorner() - maximumDistance,
	    cartesianDomain.getUpperCorner() + maximumDistance);

  // Build a mesh from the vertices and indexed simplices.
  geom::IndSimpSet<2,1,true,T> mesh(verticesSize, vertices, simplicesSize, 
				    indexedSimplices);

  // Determine the set of overlapping faces.
  std::vector<int> overlappingFaces;
  geom::determineOverlappingSimplices
    (mesh, bbox, std::back_inserter(overlappingFaces));

  // Build this mesh from the subset of overlapping faces.
  geom::buildFromSubsetSimplices(mesh, overlappingFaces.begin(), 
				 overlappingFaces.end(), this);

  // The face normals.
  _faceNormals.resize(getSimplicesSize());
  geom::computeSimplexNormals(*this, &_faceNormals);

  // The overlapping faces are the face identifiers.
  _faceIdentifiers.rebuild(overlappingFaces.begin(), overlappingFaces.end());
}


// CONTINUE
#if 0
template<typename T>
template<bool A1, bool A2>
inline
void 
Grid<2,T>::
floodFill(Number farAway,
	  const ads::Array<1,const Point,A1>& vertices,
	  const ads::Array<1,const Index,A2>& faces) {
  // First try to determine the sign of the distance from the known distances
  // and then flood fill.
  if (floodFill(farAway)) {
    return;
  }
  // If the above did not succeed then there are no known distances.

  // Ensure that the mesh is not degenerate.
  assert(vertices.size() != 0 && faces.size() != 0);

  // Find the Cartesian location of one of the grid points.
  Point location(0, 0);
  convertIndexToLocation(location);

  // We will find the closest point on the mesh to the grid location.

  // Compute the distances to the vertices.
  ads::Array<1,Number> vertexDistance(vertices.size());
  for (int i = 0; i != vertices.size(); ++i) {
    vertexDistance[i] = geom::computeDistance(location, vertices[i]);
  }

  // Find the vertex that is closest to the grid location.
  // We use this to determine an upper bound on the distance.
  const Number upperBoundDistance = ads::min(vertexDistance);

  // Determine the faces that are relevant.
  std::vector<Index> closeFaces;
  {
    Number edgeLength, minimumVertexDistance;
    for (int i = 0; i != faces.size(); ++i) {
      edgeLength = geom::computeDistance(vertices[faces[i][0]], 
					 vertices[faces[i][1]]);
      minimumVertexDistance = std::min(vertexDistance[faces[i][0]],
				       vertexDistance[faces[i][1]]);
      if (minimumVertexDistance <= upperBoundDistance + edgeLength) {
	closeFaces.push_back(faces[i]);
      }
    }
  }

  // Make a set of the vertex indices that comprise the close faces.
  std::set<int> indexSet;
  {
    const int sz = closeFaces.size();
    for (int i = 0; i != sz; ++i) {
      indexSet.insert(closeFaces[i][0]);
      indexSet.insert(closeFaces[i][1]);
    }
  }
  std::vector<int> closeVertexIndices(indexSet.begin(), indexSet.end());

  // Make an array of the close vertices.
  ads::Array<1,Point> closeVertices(closeVertexIndices.size());
  {
    const int sz = closeVertexIndices.size();
    for (int i = 0; i != sz; ++i) {
      closeVertices[i] = vertices[closeVertexIndices[i]];
    }
  }

  // Adjust the indices of the close faces.
  {
    const int sz = closeFaces.size();
    for (int i = 0; i != sz; ++i) {
      for (int j = 0; j != 2; ++j) {
	closeFaces[i][j] = 
	  std::lower_bound(closeVertexIndices.begin(), 
			   closeVertexIndices.end(), closeFaces[i][j]) -
	  closeVertexIndices.begin();
      }
    }
  }

  // Make a b-rep from the close faces.
  BRep<2,Number> brep;
  brep.make(closeVertices.begin(), closeVertices.end(), 
	    closeFaces.begin(), closeFaces.end());

  // Make a grid with a single point.
  BBox dom(location, location);
  ads::Array<2,Number> dist(1, 1);
  ads::Array<2,Point> gd;
  ads::Array<2,Point> cp;
  ads::Array<2,int> cf;
  std::vector<Grid> grids;
  grids.push_back(Grid(dom, dist, gd, cp, cf));
    
  // Compute the distance from the grid point to the mesh.
  grids[0].initialize();
  {
    std::vector<Point> globalPoints;
    brep.computeClosestPoint(grids, upperBoundDistance * 1.1, true, 0, 
			     globalPoints);
  }
  Number d = dist(0, 0);

  // Set the distance to +- farAway.
  assert(d != std::numeric_limits<Number>::max());
  const int signDistance = (d >= 0 ? 1 : -1);
  distance() = signDistance * farAway;
}
#endif


//
// Free functions for performing clipping.
//


// Make this line equidistant from the two points and oriented so that 
// p is above the line.
template<typename T>
void
makeEquidistantLine(geom::Line_2<T>* line, 
		    const ads::FixedArray<2,T>& p, 
		    const ads::FixedArray<2,T>& q) {
  assert(p != q);
  ads::FixedArray<2,T> t = T(0.5) * (p - q);
  ads::FixedArray<2,T> n = t;
  geom::rotatePiOver2(&n);
  *line = geom::Line_2<T>(q + t, q + t + n);
}


// Used for clipping the characteristic polygon for a vertex.
// Clip the polygon using the lines that are equidistant from the vertex
// and the points in clip_points.
template<typename T>
void 
clip(geom::ScanConversionPolygon<T>* poly, const Vertex<2,T>& vertex, 
     const std::vector< ads::FixedArray<2,T> >& clipPoints) {
  geom::Line_2<T> line;
  ads::FixedArray<2,T> displacement;
  for (typename std::vector< ads::FixedArray<2,T> >::const_iterator 
	 i = clipPoints.begin(); i != clipPoints.end(); ++i) {
    displacement = *i;
    displacement -= vertex.getLocation();
    // If the vertex is not at the clipping point and the clipping point
    // is above the two neighboring faces.
    if (vertex.getLocation() != *i && 
	geom::computeDotProduct(displacement, vertex.getRightNormal()) > 0 && 
	geom::computeDotProduct(displacement, vertex.getLeftNormal()) > 0) {
      makeEquidistantLine(&line, vertex.getLocation(), *i);
      poly->clip(line);
    }
  }
}


// Find the best clipping point for a characteristic polygon of a vertex
// or face.  The point on the surface has the specified normal.
template<typename T>
bool
computeBestClipPoint(const ads::FixedArray<2,T>& point, 
		     const ads::FixedArray<2,T>& normal, 
		     const std::vector< ads::FixedArray<2,T> >& clipPoints,
		     ads::FixedArray<2,T>* bestPoint) {
  T minimumValue = std::numeric_limits<T>::max();
  T val, den;
  ads::FixedArray<2,T> vec;

  for (typename std::vector< ads::FixedArray<2,T> >::const_iterator 
	 i = clipPoints.begin(); i != clipPoints.end(); ++i) {
    vec = *i - point;
    den = geom::computeDotProduct(vec, normal);
    if (den > std::numeric_limits<T>::epsilon()) {
      val = geom::computeDotProduct(vec, vec) / den;
      if (val < minimumValue) {
	minimumValue = val;
	*bestPoint = *i;
      }
    }
  }
  if (minimumValue != std::numeric_limits<T>::max()) {
    return true;
  }
  return false;
}


// Used for clipping the characteristic polygon for a vertex.
// Clip the polygon once using the best point in clipPoints.
template<typename T>
void
oneClip(geom::ScanConversionPolygon<T>* poly, const Vertex<2,T>& vertex, 
	const std::vector<ads::FixedArray<2,T> >& clipPoints) {
  ads::FixedArray<2,T> normal = vertex.getRightNormal() + 
    vertex.getLeftNormal();
  geom::normalize(&normal);
  ads::FixedArray<2,T> bestPoint;
  if (computeBestClipPoint(vertex.getLocation(), normal, clipPoints, 
			   &bestPoint)) {
    geom::Line_2<T> line;
    makeEquidistantLine(&line, vertex.getLocation(), bestPoint);
    poly->clip(line);
  }
}


// If the point is above the segment:
// Find a line that lies above the portion of the equidistant parabola
// above the segment.  The line is oriented so the segment is above
// the line.  Return true.
// Else:
// Return false.
template<typename T>
bool
makeEquidistantLine(geom::Line_2<T>* line, 
		    const ads::FixedArray<2,T>& source, 
		    const ads::FixedArray<2,T>& target,
		    const ads::FixedArray<2,T>& tangent, 
		    const ads::FixedArray<2,T>& normal,
		    const ads::FixedArray<2,T>& point) {
  const T d = geom::computeDotProduct(normal, point - source);
  // Return false if the point is not above the segment.
  if (d <= 0) {
    return false;
  }
  
  T x = geom::computeDotProduct(point - target, tangent); 
  const ads::FixedArray<2,T> p1 = target + ((x*x + d*d) / (2 * d)) * normal;
  x = geom::computeDotProduct(point - source, tangent); 
  const ads::FixedArray<2,T> p2 = source + ((x*x + d*d) / (2 * d)) * normal;
  *line = geom::Line_2<T>(p1, p2);
  return true;
}


// Clip this polygon using the lines that are equidistant from 
// the face and the points in clipPoints.
template<typename T>
void 
clip(geom::ScanConversionPolygon<T>* poly, const Face<2,T>& face,
     const std::vector< ads::FixedArray<2,T> >& clipPoints) {
  geom::Line_2<T> line;
  typename std::vector< ads::FixedArray<2,T> >::const_iterator 
    i = clipPoints.begin();
  const typename std::vector< ads::FixedArray<2,T> >::const_iterator 
    i_end = clipPoints.end();
  for (; i != i_end; ++i) {
    if (face.getSource() != *i && face.getTarget() != *i) {
      // Clip for points above the face.
      if (makeEquidistantLine(&line, face.getSource(), face.getTarget(),
			      face.getTangent(), face.getNormal(), *i)) {
	poly->clip(line);
      }
      // Clip for points below the face.
      if (makeEquidistantLine(&line, face.getTarget(), face.getSource(),
			      -face.getTangent(), -face.getNormal(), *i)) {
	poly->clip(line);
      }
    }
  }
}


// Clip the polygon using the two best points in clipPoints.
template<typename T>
void
oneClip(geom::ScanConversionPolygon<T>* poly, const Face<2,T>& face,
	const std::vector< ads::FixedArray<2,T> >& clipPoints) {
  ads::FixedArray<2,T> bestPoint;
  const ads::FixedArray<2,T> mid_point = T(0.5) * 
    (face.getSource() + face.getTarget());
  geom::Line_2<T> line;
  // Clip for points above the face.
  if (computeBestClipPoint(mid_point, face.getNormal(), clipPoints, 
			   &bestPoint)) {
    if (makeEquidistantLine(&line, face.getSource(), face.getTarget(), 
			    face.getTangent(), face.getNormal(), 
			    bestPoint)) {
      poly->clip(line);
    }
  }
  // Clip for points below the face.
  if (computeBestClipPoint(mid_point, -face.getNormal(), clipPoints, 
			   &bestPoint)) {
    if (makeEquidistantLine(&line, face.getTarget(), face.getSource(),
			    - face.getTangent(), - face.getNormal(), 
			    bestPoint)) {
      poly->clip(line);
    }
  }
}








template<typename T>
inline
std::pair<int,int>
BRep<2,T>::
computeClosestPoint(const Lattice& lattice, 
		    std::vector<Grid>* grids, 
		    const Number maximumDistance,
		    const bool arePerformingLocalClipping, 
		    const int arePerformingGlobalClipping, 
		    const std::vector<Point>& globalPoints) const {
  typedef geom::BBox<2,int> IndexBBox;

  const int gridsSize = int(grids->size());
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  std::vector<Index> indices;
  std::vector<Point> cartesianPoints;
  BBox box;
  IndexBBox indexBox;
  Polygon poly;

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
  // Find the distance, closest points and closest faces for the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    
    // If the i_th vertex has two adjacent faces
    // and the curve is either convex or concave here.
    if (getVertex(i, &vert) && vert.isConvexOrConcave()) {

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

      // Make the polygon containing the closest points.
      vert.buildCharacteristicPolygon(&poly, maximumDistance);

      // If doing limited global clipping
      if (arePerformingGlobalClipping == 1) {  
	oneClip(&poly, vert, globalPoints);
      }
      // Else if doing full global clipping
      else if (arePerformingGlobalClipping == 2) { 
	clip(&poly, vert, globalPoints);
      }
	
      // Convert to index coordinates.
      lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
					poly.getVerticesEnd());

      // Scan convert the polygon.
      indices.clear();
      poly.scanConvert(std::back_inserter(indices), lattice.getExtents());
      scanConversionCount += int(indices.size());

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
	  // Compute closest points and distance for scan converted grid pts.
	  distanceCount += (*grids)[n].computeClosestPointTransform
	    (indices, cartesianPoints, vert, maximumDistance).second;
	}
      }
    }
  } 

  //
  // Find the distance, closest points and closest faces for the faces.
  //

  FaceDistance face, prev, next;
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
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Get the i_th face.
    getFace(i, &face);

    if (arePerformingLocalClipping && getAdjacentSize(i) == 2) {
      // Get the previous and next face.
      getFace(getAdjacent(i, 1), &prev);
      getFace(getAdjacent(i, 0), &next);
      // Make the polygon that contains the points with positive and 
      // negative distance.  Do local clipping.
      face.buildCharacteristicPolygon(&poly, prev, next, maximumDistance);
    }
    // Make the characteristic polygon and don't do local clipping.
    else { 
      // Make the polygon that contains the points with positive and 
      // negative distance.
      face.buildCharacteristicPolygon(&poly, maximumDistance);
    }

    // If doing limited global clipping
    if (arePerformingGlobalClipping == 1) {  
      oneClip(&poly, face, globalPoints);
    }
    // Else if doing full global clipping
    else if (arePerformingGlobalClipping == 2) { 
      clip(&poly, face, globalPoints);
    }

    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Scan convert the polygon.
    indices.clear();
    poly.scanConvert(std::back_inserter(indices), lattice.getExtents());
    scanConversionCount += int(indices.size());

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
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (indices, cartesianPoints, face, maximumDistance).second;
      }
    }
  }
  
  return std::pair<int,int>(scanConversionCount, distanceCount);
}










template<typename T>
inline
std::pair<int,int>
BRep<2,T>::
computeClosestPointUnsigned(const Lattice& lattice, 
			    std::vector<Grid>* grids, 
			    const Number maximumDistance,
			    const bool arePerformingLocalClipping, 
			    const int arePerformingGlobalClipping, 
			    const std::vector<Point>& globalPoints) const {
  typedef geom::BBox<2,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  std::vector<Index> indices;
  std::vector<Point> cartesianPoints;
  BBox box;
  IndexBBox indexBox;
  Polygon poly;

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
  // Find the distance, gradient, closest points and closest faces for 
  // the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // If the i_th vertex has one more adjacent faces, compute the distance.
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

    // Make the polygon containing the closest points.
    vert.buildCharacteristicPolygonUnsigned(&poly, maximumDistance);

    // If doing limited global clipping
    if (arePerformingGlobalClipping == 1) {  
      oneClip(&poly, vert, globalPoints);
    }
    // Else if doing full global clipping
    else if (arePerformingGlobalClipping == 2) { 
      clip(&poly, vert, globalPoints);
    }
	
    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Scan convert the polygon.
    indices.clear();
    poly.scanConvert(std::back_inserter(indices), lattice.getExtents());
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
	// Compute closest points and distance for scan converted grid pts.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (indices, cartesianPoints, vert, maximumDistance).second;
      }
    }
  } 

  //
  // Find the distance, closest points and closest faces for the faces.
  //

  FaceDistance face, prev, next;
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
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Get the i_th face.
    getFace(i, &face);

    if (arePerformingLocalClipping && getAdjacentSize(i) == 2) {
      // Get the previous and next face.
      getFace(getAdjacent(i, 1), &prev);
      getFace(getAdjacent(i, 0), &next);
      // Make the polygon that contains the points with positive and 
      // negative distance.  Do local clipping.
      face.buildCharacteristicPolygon(&poly, prev, next, maximumDistance);
    }
    // Make the characteristic polygon and don't do local clipping.
    else { 
      // Make the polygon that contains the points with positive and 
      // negative distance.
      face.buildCharacteristicPolygon(&poly, maximumDistance);
    }

    // If doing limited global clipping
    if (arePerformingGlobalClipping == 1) {  
      oneClip(&poly, face, globalPoints);
    }
    // Else if doing full global clipping
    else if (arePerformingGlobalClipping == 2) { 
      clip(&poly, face, globalPoints);
    }

    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Scan convert the polygon.
    indices.clear();
    poly.scanConvert(std::back_inserter(indices), lattice.getExtents());
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
  
  return std::pair<int,int>(scanConversionCount, distanceCount);
}







template<typename T>
inline
std::pair<int,int>
BRep<2,T>::
computeClosestPointUsingBBox(const Lattice& lattice, 
			     std::vector<Grid>* grids, 
			     const Number maximumDistance) const {
  typedef geom::BBox<2,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box, characteristicBox;
  Range indexRange;
  IndexBBox indexBox;
  Polygon poly;

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
  // Find the distance, closest points and closest faces for the faces.
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
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Get the i_th face.
    getFace(i, &face);
    // Make the polygon that contains the points with positive and 
    // negative distance.
    face.buildCharacteristicPolygon(&poly, maximumDistance);
    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Make a bounding box around the polygon.
    characteristicBox.bound(poly.getVerticesBeginning(), poly.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 2; ++n) {
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
	// Compute closest points and distance for scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, face, maximumDistance).second;
      }
    }
  }
  
  //
  // Find the distance, closest points and closest faces for the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // If the i_th vertex has two adjacent faces and the curve is either 
    // convex or concave here, then compute the distance.
    if (! (getVertex(i, &vert) && vert.isConvexOrConcave())) {
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

    // Make the polygon containing the closest points.
    vert.buildCharacteristicPolygon(&poly, maximumDistance);
    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Make a bounding box around the polygon.
    characteristicBox.bound(poly.getVerticesBeginning(), poly.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 2; ++n) {
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
	// Compute closest points and distance for scan converted grid pts.
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, vert, maximumDistance).second;
      }
    }
  } 

  return std::pair<int,int>(scanConversionCount, distanceCount);
}








template<typename T>
inline
std::pair<int,int>
BRep<2,T>::
computeClosestPointUnsignedUsingBBox(const Lattice& lattice, 
				     std::vector<Grid>* grids, 
				     const Number maximumDistance) const {
  typedef geom::BBox<2,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box, characteristicBox;
  Range indexRange;
  IndexBBox indexBox;
  Polygon poly;

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
  // Find the distance, closest points and closest faces for the faces.
  //

  FaceDistance face, prev, next;
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
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Get the i_th face.
    getFace(i, &face);
    // Make the polygon that contains the points with positive and 
    // negative distance.
    face.buildCharacteristicPolygon(&poly, maximumDistance);
    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Make a bounding box around the polygon.
    characteristicBox.bound(poly.getVerticesBeginning(), 
			    poly.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 2; ++n) {
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
	// Compute closest points and distance for scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, face, maximumDistance).second;
      }
    }
  }
  
  //
  // Find the distance, gradient, closest points and closest faces for 
  // the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // If the i_th vertex has one more adjacent faces, compute the distance.
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

    // Make the polygon containing the closest points.
    vert.buildCharacteristicPolygonUnsigned(&poly, maximumDistance);
    // Convert to index coordinates.
    lattice.convertLocationsToIndices(poly.getVerticesBeginning(), 
				      poly.getVerticesEnd());

    // Make a bounding box around the polygon.
    characteristicBox.bound(poly.getVerticesBeginning(), 
			    poly.getVerticesEnd());
    // Convert to an integer index range.
    for (int n = 0; n != 2; ++n) {
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
	// Compute closest points and distance for scan converted grid pts.
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
BRep<2,T>::
computeClosestPointUsingBruteForce(const Lattice& lattice, 
				   std::vector<Grid>* grids, 
				   const Number maximumDistance) const {
  typedef geom::BBox<2,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box;
  Range indexRange;
  IndexBBox indexBox;
  Polygon poly;

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
  // Find the distance, closest points and closest faces for the faces.
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
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Get the i_th face.
    getFace(i, &face);
    // Convert the face bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 2; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the face could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, face, maximumDistance).second;
      }
    }
  }
  
  //
  // Find the distance, closest points and closest faces for the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // If the i_th vertex has two adjacent faces and the curve is either 
    // convex or concave here, then compute the distance.
    if (! (getVertex(i, &vert) && vert.isConvexOrConcave())) {
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
    for (int n = 0; n != 2; ++n) {
      // Ceiling.
      indexBox.setLowerCoordinate
	(n, int(box.getLowerCorner()[n]) + 1);
      // Floor + 1 for open range.
      indexBox.setUpperCoordinate
	(n, int(box.getUpperCorner()[n]) + 1);
    }
    indexRange.set_lbounds(indexBox.getLowerCorner());
    indexRange.set_ubounds(indexBox.getUpperCorner());
    indexBox.setUpperCorner(indexBox.getUpperCorner() - 1);
    scanConversionCount += indexRange.content();

    // Loop over the grids.
    for (int n = firstGrid; n != gridsSize; ++n) {
      // If the vertex could influence this grid.
      if (geom::doOverlap(gridIndexBBoxes[n], indexBox)) {
	// Compute closest points and distance for scan converted grid pts.
	distanceCount += (*grids)[n].computeClosestPointTransform
	  (lattice, indexRange, vert, maximumDistance).second;
      }
    }
  } 

  return std::pair<int,int>(scanConversionCount, distanceCount);
}








template<typename T>
inline
std::pair<int,int>
BRep<2,T>::
computeClosestPointUnsignedUsingBruteForce(const Lattice& lattice, 
					   std::vector<Grid>* grids, 
					   const Number maximumDistance) const {
  typedef geom::BBox<2,int> IndexBBox;

  const int gridsSize = grids->size();
  assert(gridsSize > 0);

  int firstGrid;
  int scanConversionCount = 0;
  int distanceCount = 0;
  BBox box;
  Range indexRange;
  IndexBBox indexBox;
  Polygon poly;

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
  // Find the distance, closest points and closest faces for the faces.
  //

  FaceDistance face, prev, next;
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
    // If there are no relevant grids, continue with the next vertex.
    if (firstGrid == gridsSize) {
      continue;
    }

    // Get the i_th face.
    getFace(i, &face);
    // Convert the face bounding box to index coordinates.
    lattice.convertBBoxLocationsToIndices(&box);
    // Convert to an integer index range.
    for (int n = 0; n != 2; ++n) {
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
	// Compute closest points and distance for scan converted grid points.
	distanceCount += (*grids)[n].computeClosestPointTransformUnsigned
	  (lattice, indexRange, face, maximumDistance).second;
      }
    }
  }
  
  //
  // Find the distance, gradient, closest points and closest faces for 
  // the vertices.
  //
  VertexDistance vert;
  for (int i = 0; i != getVerticesSize(); ++i) {
    // If the i_th vertex has one more adjacent faces, compute the distance.
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
    for (int n = 0; n != 2; ++n) {
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
	// Compute closest points and distance for scan converted grid pts.
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
BRep<2,T>::
displayInformation(std::ostream& out) const {
  geom::printInformation(out, *this);
}

template<typename T>
inline
void 
BRep<2,T>::
display(std::ostream& out) const {
  geom::writeAscii(out, *this);
  out << _faceNormals << _faceIdentifiers;
}


//
// Private Member Functions
//


template<typename T>
inline
bool
BRep<2,T>::
getVertex(const int index, VertexDistance* vert) const {
  if (getIncidentSize(index) != 2) {
    return false;
  }
  
  const int right = getRightFace(index);
  const int left = getLeftFace(index);
  vert->make(getVertex(index), _faceNormals[right], _faceNormals[left], 
	     right);
  return true;
}


template<typename T>
inline
bool
BRep<2,T>::
getVertexUnsigned(const int index, VertexDistance* vert) const {
  static Point zero(0.0);
  const int right = getRightFace(index);
  const int left = getLeftFace(index);
  if (right != -1 && left != -1) {
    vert->make(getVertex(index), _faceNormals[right], 
	       _faceNormals[left], right);
    return true;
  }
  else if (right != -1) {
    vert->make(getVertex(index), _faceNormals[right], 
	       Point(0.0), right);
    return true;
  }
  else if (left != -1) {
    vert->make(getVertex(index), Point(0.0), 
	       _faceNormals[left], left);
    return true;
  }
  assert(false);
  return false;
}


// Make a bounding box around the vertex specified by the index.
template<typename T>
inline
void 
BRep<2,T>::
getVertexBBox(const int index, const Number maximumDistance, BBox* box) const {
  box->setLowerCorner(getVertex(index) - maximumDistance);
  box->setUpperCorner(getVertex(index) + maximumDistance);
}


template<typename T>
inline
void 
BRep<2,T>::
getFace(int index, FaceDistance* face) const {
  face->make(getSimplexVertex(index, 0), getSimplexVertex(index, 1),
	     _faceNormals[index], index);
}


// Make a bounding box around the face specified by the index.
template<typename T>
inline
void 
BRep<2,T>::
getFaceBBox(const int index, const Number maximumDistance, BBox* box) const {
  box->bound(getSimplexVertex(index, 0),
	     getSimplexVertex(index, 1));
  box->setLowerCorner(box->getLowerCorner() - maximumDistance);
  box->setUpperCorner(box->getUpperCorner() + maximumDistance);
}


END_NAMESPACE_CPT
