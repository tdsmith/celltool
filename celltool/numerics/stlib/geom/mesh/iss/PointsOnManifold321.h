// -*- C++ -*-

/*! 
  \file PointsOnManifold321.h
  \brief Represent the features of a 3-2 mesh.
*/

#if !defined(__geom_PointsOnManifold321_h__)
#define __geom_PointsOnManifold321_h__


BEGIN_NAMESPACE_GEOM

// CONTINUE:  I need the ability to have corner features defined only by
// the solid angle and at the same time have edge features that fall below
// that solid angle.  Do this by defining a vertex with small solid angle
// and two incident edges as on edge feature.

// CONTINUE: Replace the term surface feature with smooth feature.

// CONTINUE: Consider a node with two incident edge features in which
// the angle between these two edges has a large deviation from pi.  In
// this case the node should be a corner feature.  Add this functionality
// to the code and the documentation.

//! Feature-based manifold data structure.
/*!
  <!--I put an anchor here because I cannot automatically reference this 
  class. -->
  \anchor PointsOnManifold321T

  \param T is the number type.  By default it is double.

  <b>Features of Manifolds.</b>

  This class represents a 2-manifold in 3-D space.  It models the manifold
  with a triangle mesh.  Consider the Enterprise model shown below.  The 
  boundary of the model is a 2-manifold.  Consider a point on this 
  manifold.  If a neighborhood of the point is diffeomorphic a neighborhood
  of the origin in the plane \f$\{x = 0\}\f$,
  then we call the point a <em>smooth feature</em>.  For those who have 
  forgotten the definition of diffeomorphic (or for those who don't care
  what it means), we can proceed concurrently with intuitive definitions.
  If a point is part of a smooth portion of the surface, then it is on a 
  smooth feature.  
  
  \image html enterprise.jpg "The Enterprise."
  \image latex enterprise.pdf "The Enterprise."

  Next we define edges.  Consider the manifold \f$E\f$ that is the union of 
  the two
  half planes \f$\{z = 0, y \geq 0\}\f$ and \f$\{y = 0, z \geq 0\}\f$.
  The two half planes meet along the \f$x\f$ axis.  We call this line an edge.
  We use this as our canonical example of an edge.
  If a neighborhood of a point on a manifold is diffeomorphic to a 
  neighborhood around the origin on \f$E\f$, then the point is on an 
  <em>edge feature</em>.  Of course you already knew what an edge was, 
  but wasn't it fun to define it?

  Finally we consider corners.  This one is easy.  If a point is not on 
  a smooth feature nor an edge feature, then it is on a 
  <em>corner feature</em>.

  Smooth features are 2-manifolds (surfaces) and edge features are 1-manifolds
  (curves).  Corner features are typically 0-manifolds (points).  I have 
  to say "typically" because one can conceive of weird 2-manifolds where the
  corner features cluster to form higher dimensional manifolds.  Go ahead 
  and ponder such possibilities if you like, but I am going to steer clear
  of the esoteric and show some simple examples.

  Consider the boundary of a cube 
  (shown below).  The interiors of its 6 square faces are smooth feature.  
  The interiors of its 12 line segment edges are edge features.  
  The cube has 8 corner features.  The corner features arise from intersecting
  edge features.  

  \image html cube.jpg "A cube."
  \image latex cube.pdf "A cube."

  Next consider the boundary of a cone (shown below).  It has a circular 
  edge feature and one corner feature.  The remainer is composed of two 
  smooth features.
  Now you can see why we defined corner features in terms of what they are not
  (smooth features and edge features) instead of what they are.  Since we 
  defined edge features by intersecting two planes we could have tried to 
  define corner features 
  by intersecting several planes.  This would work for corners that arise
  from intersecting edges, but it does not work for the corner feature 
  of the cone.  

  \image html coneAbove.jpg "A cone.  View from above."
  \image latex coneAbove.pdf "A cone.  View from above."

  \image html coneBelow.jpg "A cone.  View from below."
  \image latex coneBelow.pdf "A cone.  View from below."

  Now we have defined features for 2-manifolds in 3-D space.  Pretty easy,
  wasn't it?  We even used mathematical definitions which could easily be 
  made rigorous.  The problem is that these manifolds are mythical beasts
  which only exist in our imaginations.  In real life, we deal with triangle
  meshes, which are 2-manifolds as well, but which represent (approximate)
  the mythical manifolds.  Next we will consider surface, edge, and corner
  features in the context of triangle meshes.  

  CONTINUE.

  \image html edgeTerminatesAtCorner.jpg "An edge feature that terminates at corner features."
  \image latex edgeTerminatesAtCorner.pdf "An edge feature that terminates at corner features."

  CONTINUE.

  \image html cornerWithTwoIncidentEdges.jpg "A corner feature with two incident edge features."
  \image latex cornerWithTwoIncidentEdges.pdf "A corner feature with two incident edge features."

  <b>Features of Meshes.</b>

  Below is a triangle mesh that represents the boundary of the enterprise.
  A triangle mesh is defined in terms of nodes (points in space) and elements
  (triples of nodes that form triangles).  Because the define the geometry
  of the mesh, we would like to characterize the nodes.  If we treat the 
  mesh as a (peicewise linear) 2-manifold and apply the results from above
  we will be dissapointed.  With this approach, almost all of the nodes in the
  Enterprise example are corner features.  This is because the nodes are 
  where multiple planes (defined by the incident triangles) intersect.  
  There is a special case when all of the incident triangles are co-planar.
  Then the node is a surface feature.  One could also get an edge feature when
  the incident triangles each lie in one of two planes.  As predicted, this
  is vexing.  The classification of the node has nothing to do with how the 
  associated point on the mythical manifold would be classified.  In going 
  from the mythical manifold to the triangle mesh, we have lost information.
  We certainly don't have enough information to get derivatives, hence the
  whole diffeomorphic thing is right out the window.  We cannot determine for
  sure whether a node corresponds to a surface, edge, or corner feature.  
  The best that we will be able to do is make a reasonable guess.

  \image html enterpriseBoundary.jpg "A triangle mesh that represents the boundary of the Enterprise."
  \image latex enterpriseBoundary.pdf "A triangle mesh that represents the boundary of the Enterprise."
  
  To get the ball rolling, we will need to consider edges in the triangle mesh
  instead of single nodes.  An interior edge in the triangle mesh has two 
  incident
  triangle faces.  Consider the dihedral angle defined by these two triangle
  faces.  If the angle has a large deviation from \f$\pi\f$ there are two 
  possibilities.
  -# The points on the interior of the line segment correspond to points 
  on an edge 
  feature on the mythical manifold.  The edge in the mesh is then a linear 
  approximation of the associated portion of this edge feature.
  -# The mesh is a poor approximation of the mythical manifold near the edge.
  This could happen because the mesh is too coarse to capture the features 
  of the manifold.  For example, the points on the edge could correspond 
  to points on a surface 
  feature where the surface has relatively high curvature (relative to the 
  edge lengths in the mesh).

  To keep the ball rolling, we need to be optimistic.  If the dihedral angle
  at an edge in the mesh has a large deviation from \f$\pi\f$, we \e hope
  that the edge corresponds to a portion of an edge feature in the mythical
  manifold.  If not, the mesh is a poor approximation of the manifold, and 
  that makes anything that we do with the mesh rather pointless.

  Note the vague language of "large deviation."  This vagueness is a necessity.
  What is large will depend on the mesh and how well we think it approximates
  the mythical manifold.  We will explore this issue with some examples later 
  on.

  Now we have a method of determining which edges in a mesh we are going to
  call edge features.  First we pick a deviation \f$\alpha\f$.  Then if 
  \f$|\theta - \pi| > \alpha\f$ where \f$\theta\f$ is the dihedral angle at
  an edge, then we have an edge feature.  What does this tell us about the
  two nodes at the ends of the edge?  Well, they are certainly not surface
  features.  But they may be either edge features or corner features.
  If a specified interior node has two incident edge features, then the 
  node is on 
  an edge feature.  If it has more than two incident edge features,
  then it is on a corner feature.  For instance, the node at the corner of
  a cube would have three incident edge features.  If the node has one incident
  edge feature, it is also on a corner feature.  To justify this, we go back 
  to the mythical manifold and consider an edge feature that has a "free end",
  i.e. it terminates at a point where it does not intersect other edge
  features.  Since this endpoint is clearly not a surface feature or
  an edge feature, it must be a corner feature.  In summary:
  - If a node has 0 incident edge features we will, for the time being,
  consider it to be on a surface feature.  (We are being non-commital because
  we have only considered corner features that arise from intersecting or
  terminating edges.)
  - If a node has 2 incident edge features, it is on an edge feature.
  - Otherwise, the node is on a corner feature.

  Now we deal with corner features that do not arise from intersecting
  or terminating edge features, for example the point of a cone.  For this
  we examine the solid angle an an interior node.  If this angle has 
  a large deviation from \f$2 \pi\f$, then the node is on a corner feature.
  
  If the triangle mesh is not closed, then we need to classify its boundary
  nodes.  In this case, the boundary of the triangle mesh is a line segment
  mesh.  To classify the boundary nodes of the triangle mesh we 
  will use our classification scheme for 1-manifolds in
  3-D space (see \ref PointsOnManifoldN11T).  We measure the angle
  between the two incident boundary edges.  If it has a large deviation 
  from \f$\pi\f$ then the node is on a corner feature.  If the boundary
  node has any incident edge features, it is also on a corner feature.
  Otherwise the node is on a surface feature.

  To summarize, three parameters give us a reasonable way of determining
  which edges of a triangle mesh are edge features and classifying each
  of the nodes as being on a surface feature, an edge feature or a corner
  feature.  These three parameters are the dihedral devation, the solid
  angle deviation, and the boundary angle deviation.  

  We have just done quite a bit of deriving.  It's time to give our brains
  a rest and look at some pictures.  Consider the Enterprise mesh above.
  This was generated in Cubit using a target edge length of 20 meters.
  We will use a range of dihedral angle deviations to extract the features
  of the mesh and hopefully extract features of the model.  In the figures 
  below we show the edge features in white and the corner features in red.

  We start with a dihedral angle deviation of 0.1 (radians).  This picks up
  a lot of spurious edge and corner features (along with the "genuine" 
  features).
  
  \image html enterpriseFeatures0.1.jpg "Features derived from a dihedral angle deviation of 0.1."
  \image latex enterpriseFeatures0.1.pdf "Features derived from a dihedral angle deviation of 0.1."

  When we increase the dihedral angle deviation to 0.2, we get fewer
  spurious features.

  \image html enterpriseFeatures0.2.jpg "Features derived from a dihedral angle deviation of 0.2."
  \image latex enterpriseFeatures0.2.pdf "Features derived from a dihedral angle deviation of 0.2."
  
  Likewise for 0.3.

  \image html enterpriseFeatures0.3.jpg "Features derived from a dihedral angle deviation of 0.3."
  \image latex enterpriseFeatures0.3.pdf "Features derived from a dihedral angle deviation of 0.3."
  
  When we increase the dihedral angle deviation to 0.4, the spurious features
  are almost gone, but we have also lost a legitimate edge feature.  We 
  no longer detect one of the large circular edge features.

  \image html enterpriseFeatures0.4.jpg "Features derived from a dihedral angle deviation of 0.4."
  \image latex enterpriseFeatures0.4.pdf "Features derived from a dihedral angle deviation of 0.4."
  
  When we go to 0.5, there are no spurious features.

  \image html enterpriseFeatures0.5.jpg "Features derived from a dihedral angle deviation of 0.5."
  \image latex enterpriseFeatures0.5.pdf "Features derived from a dihedral angle deviation of 0.5."

  As we increase the dihedral angle deviation more, we lose more of the 
  legitimate features.
  
  \image html enterpriseFeatures1.0.jpg "Features derived from a dihedral angle deviation of 1.0."
  \image latex enterpriseFeatures1.0.pdf "Features derived from a dihedral angle deviation of 1.0."
  
  \image html enterpriseFeatures1.5.jpg "Features derived from a dihedral angle deviation of 1.5."
  \image latex enterpriseFeatures1.5.pdf "Features derived from a dihedral angle deviation of 1.5."
  
  \image html enterpriseFeatures2.0.jpg "Features derived from a dihedral angle deviation of 2.0."
  \image latex enterpriseFeatures2.0.pdf "Features derived from a dihedral angle deviation of 2.0."
  
*/
template<typename T>
class PointsOnManifold<3,2,1,T> {
  //
  // Public enumerations.
  //

public:

  //! The space dimension, simplex dimension, and spline degree.
  enum {N = 3, M = 2, SD = 1};

  //
  // Private enumerations.
  //

  // CONTINUE: With the Intel compiler, private members are not accessible in
  // nested classes.
#ifdef __INTEL_COMPILER
public:
#else
private:
#endif

  //! The features of the manifold at the vertices.
  enum Feature {NullFeature, CornerFeature, EdgeFeature, SurfaceFeature};

  //
  // Nested classes.
  //

private:
  
  //! A face handle.  (2-face, 1-face, or 0-face)
  class FaceHandle {
  private:

    Feature _feature;
    int _index;

  public:

    //
    // Constructors, etc.
    //

    //! Default constructor.  Make an invalid face handle.
    FaceHandle() :
      _feature(NullFeature),
      _index(-1)
    {}

    //! Copy constructor.
    FaceHandle(const FaceHandle& other) :
      _feature(other._feature),
      _index(other._index)
    {}

    //! Assignment operator.
    FaceHandle&
    operator=(const FaceHandle& other) {
      // Avoid assignment to self.
      if (this != &other) {
	_feature = other._feature;
	_index = other._index;
      }
      // Return a reference to this so assignments can chain.
      return *this;
    }
    
    //! Construct from a feature and an index.
    FaceHandle(const Feature feature, const int index) :
      _feature(feature),
      _index(index)
    {}

    //! Trivial destructor.
    ~FaceHandle()
    {}

    //
    // Accessors.
    //

    //! Get the feature.
    Feature
    getFeature() const {
      return _feature;
    }

    //! Get the index.
    int
    getIndex() const {
      return _index;
    }

    //
    // Manipulators
    //

    //! Set the feature.
    void
    setFeature(const Feature feature) {
      _feature = feature;
    }

    //! Set the index.
    void
    setIndex(const int index) {
      _index = index;
    }

    //! Set the face handle to an invalid value.
    void
    setToNull() {
      _feature = NullFeature;
      _index = -1;
    }
  };

  //
  // Private types.
  //

private:

  //! The representation of the surface.
  typedef IndSimpSetIncAdj<N,M,true,T> SurfaceManifold;
  //! An edge in the surface.
  typedef typename SurfaceManifold::Face SurfaceEdge;
  //! The size type.
  typedef int SizeType;
  //! The container for the registered points on the manifold.
  typedef std::map<int,FaceHandle> PointMap;
  //! An edge between two registered points.
  typedef ads::OrderedPair<int> RegisteredEdge;
  //! The container for the registered edges (along edge features).
  typedef std::set<RegisteredEdge> EdgeSet;
  //! The representation for a point: std::pair<const int, FaceHandle>.
  typedef typename PointMap::value_type Point;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef T Number;
  //! A vertex.
  typedef typename SurfaceManifold::Vertex Vertex;
  // CONTINUE: I could wrap the vertex array from the surface to save memory.
  //! The representation of the edges.
  typedef IndSimpSetIncAdj<N,M-1,true,T> EdgeManifold;

  //
  // Member data.
  //

private:
  
  //! The surface manifold.
  SurfaceManifold _surfaceManifold;
  //! The array that records if a 1-face is an edge feature.
  /*!
    The array has size simplicesSize * (M + 1).
  */
  ads::Array<2,bool> _isAnEdge;
  //! The edge manifold.
  EdgeManifold _edgeManifold;
  //! The indices of the corner vertices.
  ads::Array<1,int> _cornerIndices;
  //! The vertex features.
  ads::Array<1,Feature> _vertexFeatures;
  //! The registered points on the surface.
  PointMap _points;
  //! The registered edges.
  EdgeSet _edges;
  //! The data structure for surface simplex queries.
  ISS_SimplexQuery<SurfaceManifold> _surfaceQuery;
  //! The data structure for edge simplex queries.
  ISS_SimplexQuery<EdgeManifold> _edgeQuery;
  //! The maximum distance between a point and a corner vertex for the point to be placed there.
  Number _maxCornerDistance;
  //! The maximum distance between a point and an edge for the point to be placed there.
  Number _maxEdgeDistance;

  //! A cached point identifier.
  mutable int _cachedIdentifier;
  //! A cached face.
  mutable FaceHandle _cachedFace;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  PointsOnManifold();

  //! Copy constructor not implemented
  PointsOnManifold(const PointsOnManifold&);

  //! Assignment operator not implemented
  PointsOnManifold& 
  operator=(const PointsOnManifold&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //!@{

  //! Construct from the mesh and the corners.
  /*!
    \param iss is the indexed simplex set.
    \param edgesBegin The beginning of the range of edges (simplex index and 
    local index).
    \param edgesEnd The end of the range of edges.
    \param cornersBegin The beginning of the range of corner indices.
    \param cornersEnd The end of the range of corner indices.

    Boundary edges (with only one incident simplex) will also be set as 
    edges features.  Any vertex with other than 0 or 2 incident edge features
    will also be set as a corner feature.

    The maximum corner distance and the maximum edge distance will be 
    set to 0.1 times the minimum edge length in surface mesh.  You can change
    these default values with setMaxCornerDistance() and 
    setMaxEdgeDistance().
  */
  template<bool A, typename V, typename IS, typename EdgeInIter,
	   typename IntInIter>
  PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		   EdgeInIter edgesBegin, EdgeInIter edgesEnd,
		   IntInIter cornersBegin, IntInIter cornersEnd);

  //! Construct from the mesh and angles to define edge and corner features.
  /*!
    \param iss is the indexed simplex set.
    \param maxDihedralAngleDeviation The maximum dihedral angle deviation 
    (from straight) for a surface feature.  The rest are edge features.  
    If not specified, all interior edges will be set as surface features.
    \param maxSolidAngleDeviation Solid angles that deviate 
    more than this value (from \f$2 \pi\f$) are corner features.  
    If not specified, this criterion will not be used to identify corners.
    \param maxBoundaryAngleDeviation 
    If the angle deviation (from \f$\pi\f$) between two boundary edges 
    exceeds this value, it will be set as a corner feature.  If not specified,
    this criterion will not be used to identify corners on the boundary.

    The maximum corner distance and the maximum edge distance will be 
    set to 0.1 times the minimum edge length in surface mesh.  You can change
    these default values with setMaxCornerDistance() and 
    setMaxEdgeDistance().
  */
  template<bool A, typename V, typename IS>
  PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		   Number maxDihedralAngleDeviation = -1,
		   Number maxSolidAngleDeviation = -1,
		   Number maxBoundaryAngleDeviation = -1);

  //! Destructor.  Free internally allocated memory.
  ~PointsOnManifold()
  {}

  //!@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //!@{

  //! Return the number of registered points.
  SizeType
  countNumberOfPoints() const {
    return _points.size();
  }

  //! Count the number of registered points on each feature.
  void
  countNumberOfPointsOnFeatures(SizeType* surfaceCount, SizeType* edgeCount, 
				SizeType* cornerCount) const;

  //! Count the number of points on surface features.
  SizeType
  countNumberOfSurfacePoints() const;

  //! Count the number of points on edge features.
  SizeType
  countNumberOfEdgePoints() const;

  //! Count the number of points on corner features.
  SizeType
  countNumberOfCornerPoints() const;

  //! Return the number of registered edges.
  SizeType
  countNumberOfEdges() const {
    return _edges.size();
  }

  //! Return true if the vertex is a surface feature.
  bool
  isVertexASurfaceFeature(const int n) const {
    return _vertexFeatures[n] == SurfaceFeature;
  }

  //! Return true if the vertex is an edge feature.
  bool
  isVertexAnEdgeFeature(const int n) const {
    return _vertexFeatures[n] == EdgeFeature;
  }

  //! Return true if the vertex is a corner feature.
  bool
  isVertexACornerFeature(const int n) const {
    return _vertexFeatures[n] == CornerFeature;
  }

  //! Return true if the point is on a surface.
  bool
  isOnSurface(const int identifier) const {
    return isOnFeature(identifier, SurfaceFeature);
  }
  
  //! Return true if the point is on an edge.
  bool
  isOnEdge(const int identifier) const {
    return isOnFeature(identifier, EdgeFeature);
  }
  
  //! Return true if the point is on a corner.
  bool
  isOnCorner(const int identifier) const {
    return isOnFeature(identifier, CornerFeature);
  }

  //! Return true if the point has been registered.
  bool
  hasPoint(const int identifier) const {
    return _points.count(identifier) == 1;
  }

  //! Return true if the edge between the two registered points has been registered.
  bool
  hasEdge(int pointId1, int pointId2) const;

  //! Get the surface simplex index associated with the point.
  /*!
    The point must be on a surface feature.
  */
  int
  getSurfaceSimplexIndex(const int identifier) const;
  
  //! Get the edge simplex index associated with the point.
  /*!
    The point must be on an edge feature.
  */
  int
  getEdgeSimplexIndex(const int identifier) const;
  
  //! Get the vertex index associated with the point.
  /*!
    The point must be on a corner feature.
  */
  int
  getVertexIndex(const int identifier) const;

  //! Get the maximum distance between a point and a corner vertex for the point to be placed there.
  Number
  getMaxCornerDistance() const {
    return _maxCornerDistance;
  }

  //! Get the maximum distance between a point and an edge for the point to be placed there.
  Number
  getMaxEdgeDistance() const {
    return _maxEdgeDistance;
  }

  //! Get the simplices in the neighborhood of the face. (0, 1 or 2-face)
  template<typename IntInsertIterator>
  void
  getNeighborhood(const int identifier, IntInsertIterator iter) const {
    // If the vertex is a corner feature.
    if (isOnCorner(identifier)) {
      getCornerNeighborhood(getVertexIndex(identifier), iter);
    }
    // If the vertex is an edge feature.
    else if (isOnEdge(identifier)) {
      getEdgeNeighborhood(getEdgeSimplexIndex(identifier), iter);
    }
    // Otherwise, it is a surface feature.
    else {
      getSurfaceNeighborhood(getSurfaceSimplexIndex(identifier), iter);
    }
  }

  //! Get the specified vertex of the specified edge simplex.
  const Vertex&
  getEdgeSimplexVertex(const int simplexIndex, const int m) const {
    // No need to test the validity of the parameters.  This is done in the
    // following call.
    return _edgeManifold.getSimplexVertex(simplexIndex, m);
  }

  //! Get the specified vertex of the specified surface simplex.
  const Vertex&
  getSurfaceSimplexVertex(const int simplexIndex, const int m) const {
    // No need to test the validity of the parameters.  This is done in the
    // following call.
    return _surfaceManifold.getSimplexVertex(simplexIndex, m);
  }

  //! Get the edge manifold.
  const EdgeManifold&
  getEdgeManifold() const {
    return _edgeManifold;
  }

  //! Get the corner indices.
  const ads::Array<1,int>&
  getCornerIndices() const {
    return _cornerIndices;
  }

  //! Build a mesh of the registered edges.
  /*!
    The vertices of the edge mesh will be those of the solid mesh.  You can
    get rid of the unused vertices by packing the mesh.
  */
  template<bool A33, typename V33, typename IS33, typename V31, typename IS31>
  void
  getEdgesOnEdgeFeatures(const IndSimpSet<3,3,A33,Number,V33,IS33>& solidMesh,
			 IndSimpSet<3,1,true,Number,V31,IS31>* edgeMesh) const;

  //!@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //!@{

  //! Set the maximum distance between a point and a corner vertex for the point to be placed there.
  /*!
    In the constructor, the maximum corner distance is initialized to 
    0.1 times the minimum edge length in the surface manifold.  If this is not
    an appropriate value, change it with this function.
  */
  void
  setMaxCornerDistance(const Number x) {
    _maxCornerDistance = x;
  }

  //! Set the maximum distance between a point and an edge for the point to be placed there.
  /*!
    In the constructor, the maximum corner distance is initialized to 
    0.1 times the minimum edge length in the surface manifold.  If this is not
    an appropriate value, change it with this function.
  */
  void
  setMaxEdgeDistance(const Number x) {
    _maxEdgeDistance = x;
  }

  //! Change the identifier of a registered point.
  void
  changeIdentifier(int existingIdentifier, int newIdentifier);

  //! Change the location of a registered point.
  /*!
    Return the new point on the manifold.
  */
  Vertex
  changeLocation(int pointIdentifier, const Vertex& newLocation);

  //! Change the surface simplex for a registered surface feature.
  void
  changeSurfaceSimplex(int pointIdentifier, int simplexIndex);

  //! Change the edge simplex for a registered edge feature.
  void
  changeEdgeSimplex(int pointIdentifier, int simplexIndex);

  //!@}
  //--------------------------------------------------------------------------
  //! \name Insert/Erase Points.
  //!@{

  //! Insert a point at the specified vertex.
  void
  insertAtVertex(int pointIdentifier, int vertexIndex);

  //! Insert a point at each vertex.
  /*!
    The point identifiers will be the vertex indices.
  */
  void
  insertAtVertices();

  //! Insert a point at each vertex and an edge at each edge feature.
  /*!
    The point identifiers will be the vertex indices.
  */
  void
  insertAtVerticesAndEdges();

// CONTINUE: Implement this or something like it.
#if 0
  //! Insert a point at the closest point to the specified position that is near the existing point.
  /*!
    Return the point's position on the manifold.
  */
  Vertex
  insertNearPoint(int newPointID, const Vertex& position, int existingPointID);
#endif

  //! Insert a point at the closest point on an edge feature.
  Vertex
  insertOnAnEdge(int pointIdentifier, const Vertex& position);

  //! Insert a point at the closest point on an edge or corner feature.
  /*!
    First check if the point is close to a corner.  If not, insert it
    on an edge feature.
  */
  Vertex
  insertOnAnEdgeOrCorner(int pointIdentifier, const Vertex& position);

  //! Insert a point at the closest point on a surface feature.
  Vertex
  insertOnASurface(int pointIdentifier, const Vertex& position);

  //! Insert a point at the closest point to the specified position.
  /*!
    Return the point's position on the manifold.
  */
  Vertex
  insert(int pointIdentifier, const Vertex& position);

  //! Insert a range of points at their closest points.
  /*!
    The point identifiers will be in the range [0...numPoints).
    Put the positions at the closest points on the manifold to the output 
    iterator.
  */
  template<typename PointInputIterator, typename PointOutputIterator>
  void
  insert(PointInputIterator locationsBegin, PointInputIterator locationsEnd,
	 PointOutputIterator closestPoints);

  //! Insert the boundary vertices of the mesh.  Register the edge features.
  /*!
    The point identifiers will be the vertex identifiers.  The boundary 
    vertices are moved to lay on the manifold.
    
    Consider an edge on the boundary of mesh, whose vertices are inserted
    on edge or corner features.  (We have to include corner features because
    the whole edge feature may be composed of a single line segment.)
    If the dihedral angle deviates more than
    maxDihedralAngleDeviation from straight, the edge will be registered
    as on edge feature.  If the maximum dihedral angle deviation is not 
    specified, no edge features will be registered.
  */
  template<bool A, typename V, typename IS>
  void
  insertBoundaryVerticesAndEdges(IndSimpSetIncAdj<N,M+1,A,T,V,IS>* mesh,
				 Number maxDihedralAngleDeviation = -1);

  //! Insert the boundary vertices of the mesh.  Register the edge features.
  /*!
    The point identifiers will be the vertex identifiers.  The boundary 
    vertices are moved to lay on the manifold.
    
    Consider an edge on the boundary of mesh, whose vertices are inserted
    on edge or corner features.  (We have to include corner features because
    the whole edge feature may be composed of a single line segment.)
    If the dihedral angle deviates more than
    maxDihedralAngleDeviation from straight, the edge will be registered
    as on edge feature.  If the maximum dihedral angle deviation is not 
    specified, no edge features will be registered.
  */
  template<template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Container>
  void
  insertBoundaryVerticesAndEdges
  (SimpMeshRed<N,M+1,T,Node,Cell,Container>* mesh,
   Number maxDihedralAngleDeviation = -1);

  //! Insert the vertices of the mesh.  Register the edge features.
  /*!
    The point identifiers will be the vertex identifiers.  The 
    vertices are moved to lay on the manifold.
    
    Consider an edge on mesh, whose vertices are inserted
    on edge or corner features.  (We have to include corner features because
    the whole edge feature may be composed of a single line segment.)
    If the dihedral angle deviates more than
    maxDihedralAngleDeviation from straight, the edge will be registered
    as on edge feature.  If the maximum dihedral angle deviation is not 
    specified, no edge features will be registered.
  */
  template<template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Container>
  void
  insertVerticesAndEdges(SimpMeshRed<N,M,T,Node,Cell,Container>* mesh,
			 Number maxDihedralAngleDeviation = -1);

  //! Erase a point.
  void
  erase(int pointIdentifier);

  //! Erase all of the points.
  void
  clearPoints() {
    _points.clear();
  }

  //!@}
  //--------------------------------------------------------------------------
  //! \name Insert/Erase Edges.
  //!@{

  //! Insert an edge.
  void
  insert(int pointId1, int pointId2);

  //! Insert an edge at each edge feature.
  /*! 
    There must already be a point at each vertex of an edge feature.
  */
  void
  insertAtEdges();

  //! Erase an edge.
  void
  erase(int pointId1, int pointId2);

  //! Erase all of the edges.
  void
  clearEdges() {
    _edges.clear();
  }

  //! Split an edge.
  /*!
    Erase the edge between the source and the target.  Insert two edges 
    using the mid-point
  */
  void
  splitEdge(int source, int target, int middle);

  //!@}
  //--------------------------------------------------------------------------
  //! \name Closest Point.
  //!@{

  //! Return the closest point on the manifold to the specified position.
  /*!
    \param pointIdentifier The identifier of the registered point.
    \param position The position of the point.

    Cache the new face for the closest point.
  */
  Vertex
  computeClosestPoint(int pointIdentifier, const Vertex& position) const;

  //! Update the face of a registered point from the cached value.
  void
  updatePoint();

  //!@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //!@{

  //! Print information about the data structure.
  void
  printInformation(std::ostream& out) const;

  //!@}

  //--------------------------------------------------------------------------
  // Private member functions.
  //

private:

  //! Build the edge and corner data structures.
  /*!
    This is called from the constructor.

    In addition to the edges specified in the range, all boundary edges
    will be set as edge features.

    In addition to the corners specified in the range, any vertex with 
    other than 0 or 2 incident edge features will be set as a corner feature.
  */
  template<typename EdgeInIter, typename IntInIter>
  void
  buildEdgesAndCorners(EdgeInIter edgesBegin, EdgeInIter edgesEnd,
		       IntInIter cornersBegin, IntInIter cornersEnd);

  //! Insert the point in the specified surface simplex.
  /*!
    \return The closest point in the simplex.
  */
  Vertex
  insertInSurfaceSimplex(int pointIdentifier, const Vertex& position, 
			 int simplexIndex);

  //! Insert the point in the specified edge simplex.
  /*!
    \return The closest point in the simplex.
  */
  Vertex
  insertInEdgeSimplex(int pointIdentifier, const Vertex& position, 
		      int simplexIndex);

  //! Try to find a corner vertex that is very close to the position.
  /*!
    If there is a close corner, return the index of the vertex.  
    Otherwise, return -1.
  */
  int
  findCornerVertex(const Vertex& position) const;

  //! Try to find an edge that is very close to the position.
  /*!
    If there is a close edge, return the index of the edge simplex.  
    Otherwise, return -1.
  */
  int
  findEdgeSimplex(const Vertex& position) const;

  //! Compute the closest point in the set of surface simplices.  
  /*!
    Cache the face of the closest point.
  */
  template<typename IntInputIterator>
  Vertex
  computeClosestPointInSurfaceSimplices(IntInputIterator indicesBegin, 
					IntInputIterator indicesEnd, 
					const Vertex& position) const;

  //! Compute the closest point in the set of edge simplices.  
  /*!
    Cache the face of the closest point.
  */
  template<typename IntInputIterator>
  Vertex
  computeClosestPointInEdgeSimplices(IntInputIterator indicesBegin, 
				     IntInputIterator indicesEnd, 
				     const Vertex& position) const;

  //! Compute the closest point to the surface simplex.  Return the unsigned distance.
  Number
  computeClosestPointInSurfaceSimplex(int simplexIndex, const Vertex& x, 
				      Vertex* closestPoint) const;

  //! Compute the closest point to the edge simplex.  Return the unsigned distance.
  Number
  computeClosestPointInEdgeSimplex(int simplexIndex, const Vertex& x, 
				   Vertex* closestPoint) const;

  //! Return the closest point to the specified surface simplex.
  Vertex
  computeClosestPointInSurfaceSimplex(int simplexIndex, const Vertex& x) const;

  //! Return the closest point to the specified edge simplex.
  Vertex
  computeClosestPointInEdgeSimplex(int simplexIndex, const Vertex& x) const;

  //! Return true if the point is on the specified feature.
  bool
  isOnFeature(int identifier, Feature feature) const;

  //! Return true if the face is valid.
  bool
  isValid(const FaceHandle& face) const;
  
  //! Get the simplices in the neighborhood of the face. (0, 1 or 2-face)
  template<typename IntInsertIterator>
  void
  getNeighborhood(const FaceHandle& face, IntInsertIterator iter) const;

  //! Get the indices of the surface simplices in the neighborhood.
  /*!
    Do not step across edges.  Only take one step in each direction around 
    corners.
  */
  template<typename IntInsertIterator>
  void
  getSurfaceNeighborhood(const int simplexIndex, IntInsertIterator iter) const;

  //! Get the indices of the edge simplices in the neighborhood.
  /*!
    Do not step across corners.
  */
  template<typename IntInsertIterator>
  void
  getEdgeNeighborhood(const int simplexIndex, IntInsertIterator iter) const;

  //! Get the incident surface simplex indices.
  template<typename IntInsertIterator>
  void
  getCornerNeighborhood(const int vertexIndex, IntInsertIterator iter) const;
};

END_NAMESPACE_GEOM

#define __geom_PointsOnManifold321_ipp__
#include "PointsOnManifold321.ipp"
#undef __geom_PointsOnManifold321_ipp__

#endif
