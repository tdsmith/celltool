// -*- C++ -*-

/*! 
  \file PointsOnManifoldN11.h
  \brief Represent the features of a N-1 mesh.
*/

#if !defined(__geom_PointsOnManifoldN11_h__)
#define __geom_PointsOnManifoldN11_h__


BEGIN_NAMESPACE_GEOM


//! Feature-based manifold data structure.  
/*!
  <!--I put an anchor here because I cannot automatically reference this 
  class. -->
  \anchor PointsOnManifoldN11T

  \param T is the number type.  By default it is double.
*/
template<int _N, typename T>
class PointsOnManifold<_N,1,1,T> {
  //
  // Public enumerations.
  //

public:

  //! The space dimension, simplex dimension, and spline degree.
  enum {N = _N, M = 1, SD = 1};

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
  enum Feature {NullFeature, CornerFeature, SurfaceFeature};

  //
  // Nested classes.
  //

private:
  
  //! A face handle. (1-face or 0-face)
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
  //! The size type.
  typedef int SizeType;
  //! The container for the registered points on the manifold.
  typedef std::map<int,FaceHandle> PointMap;
  //! The representation for a point.
  typedef typename PointMap::value_type Point;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef T Number;
  //! A vertex.
  typedef typename SurfaceManifold::Vertex Vertex;

  //
  // Member data.
  //

private:
  
  //! The surface manifold.
  SurfaceManifold _surfaceManifold;
  //! The indices of the corner vertices.
  ads::Array<1,int> _cornerIndices;
  //! The vertex features.
  ads::Array<1,Feature> _vertexFeatures;
  //! The registered points on the surface.
  PointMap _points;
  //! The data structure for simplex queries.
  ISS_SimplexQuery<SurfaceManifold> _simplexQuery;
  //! The maximum distance between a point and a corner vertex for the point to be placed there.
  Number _maxCornerDistance;

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
    \param cornersBegin The beginning of the range of corner indices.
    \param cornersEnd The end of the range of corner indices.

    Boundary vertices (with only one incident simplex) will also be set as 
    corner vertices.

    The maximum corner distance will be 
    set to 0.1 times the minimum edge length in surface mesh.  You can change
    this default value with setMaxCornerDistance().
  */
  template<bool A, typename V, typename IS, typename IntInIter>
  PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		   IntInIter cornersBegin, IntInIter cornersEnd);

  //! Construct from the mesh and an angle to define corners.
  /*!
    \param iss is the indexed simplex set.
    \param maxAngleDeviation The maximum angle deviation (from straight)
    for a surface vertex.  The rest are corner vertices.  If not specified,
    all interior vertices will be set as surface vertices.

    The maximum corner distance will be 
    set to 0.1 times the minimum edge length in surface mesh.  You can change
    this default value with setMaxCornerDistance().
  */
  template<bool A, typename V, typename IS>
  PointsOnManifold(const IndSimpSet<N,M,A,T,V,IS>& iss,
		   Number maxAngleDeviation = -1);

  //! Destructor.  Free internally allocated memory.
  ~PointsOnManifold()
  {}

  //!@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //!@{

  //! Count the number of registered points.
  SizeType
  countNumberOfPoints() const {
    return SizeType(_points.size());
  }

  //! Count the number of registered points on each feature.
  void
  countNumberOfPointsOnFeatures(SizeType* surfaceCount, SizeType* cornerCount)
    const;

  //! Count the number of points on surface features.
  SizeType
  countNumberOfSurfacePoints() const;

  //! Count the number of points on corner features.
  SizeType
  countNumberOfCornerPoints() const;

  //! Return true if the vertex is a surface feature.
  bool
  isVertexASurfaceFeature(const int n) const {
    return _vertexFeatures[n] == SurfaceFeature;
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

  //! Get the simplex index associated with the point.
  /*!
    The point must be on a surface feature.
  */
  int
  getSimplexIndex(const int identifier) const;
  
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

  //! Get the simplices in the neighborhood of the face. (0-face or 1-face)
  template<typename IntInsertIterator>
  void
  getNeighborhood(const int identifier, IntInsertIterator iter) const {
    // If the vertex is a corner feature.
    if (isOnCorner(identifier)) {
      getCornerNeighborhood(getVertexIndex(identifier), iter);
    }
    // Otherwise, it is a surface feature.
    else {
      getSurfaceNeighborhood(getSimplexIndex(identifier), iter);
    }
  }

  //! Get the specified vertex of the specified surface simplex.
  const Vertex&
  getSurfaceSimplexVertex(const int simplexIndex, const int m) const {
    return _surfaceManifold.getSimplexVertex(simplexIndex, m);
  }

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

  //! Insert a point at the closest point to the specified position that is near the existing point.
  /*!
    Return the point's position on the manifold.
  */
  Vertex
  insertNearPoint(int newPointID, const Vertex& position, int existingPointID);

  //! Insert a point at the closest point to the specified position that is in the neighborhood of one of the existing points.
  /*!
    Return the point's position on the manifold.
  */
  Vertex
  insertNearPoints(int newPointID, const Vertex& position, 
		   int existingPointID1, int existingPointID2);

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

  //! Insert the boundary vertices of the mesh.
  /*!
    The point identifiers will be the vertex indices.  The boundary vertices
    are moved to lay on the manifold.
  */
  template<bool A, typename V, typename IS>
  void
  insertBoundaryVertices(IndSimpSetIncAdj<N,M+1,A,T,V,IS>* mesh);
  
  //! Insert the boundary vertices of the mesh.
  /*!
    The point identifiers will be the vertex identifiers.  The boundary 
    vertices are moved to lay on the manifold.
  */
  template<template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Container>
  void
  insertBoundaryVertices(SimpMeshRed<N,M+1,T,Node,Cell,Container>* mesh);
  
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

  //! Insert the point in the specified simplex.
  /*!
    \return The closest point in the simplex.
  */
  Vertex
  insertInSimplex(int pointIdentifier, const Vertex& position, 
		  int simplexIndex);

  //! Try to find a corner vertex that is very close to the position.
  /*!
    If there is a close corner, return the index of the vertex.  
    Otherwise, return -1.
  */
  int
  findCornerVertex(const Vertex& position) const;

  //! Compute the closest point in the set of simplices.  
  /*!
    Cache the face of the closest point.
  */
  template<typename IntInputIterator>
  Vertex
  computeClosestPointInSimplices(IntInputIterator indicesBegin, 
				 IntInputIterator indicesEnd, 
				 const Vertex& position) const;

  //! Compute the closest point to the simplex.  Return the unsigned distance.
  Number
  computeClosestPointInSimplex(int simplexIndex, const Vertex& x, 
			       Vertex* closestPoint) const;

  //! Return the closest point to the n_th simplex.
  Vertex
  computeClosestPointInSimplex(int simplexIndex, const Vertex& x) const;

  //! Return true if the point is on the specified feature.
  bool
  isOnFeature(int identifier, Feature feature) const;

  //! Return true if the face is valid.
  bool
  isValid(const FaceHandle& face) const;
  
  //! Get the simplices in the neighborhood of the face. (0-face or 1-face)
  template<typename IntInsertIterator>
  void
  getNeighborhood(const FaceHandle& face, IntInsertIterator iter) const;

  //! Get the simplex index and the adjacent simplex indices.
  /*!
    Do not step across corners.
  */
  template<typename IntInsertIterator>
  void
  getSurfaceNeighborhood(const int simplexIndex, IntInsertIterator iter) const;

  //! Get the incident simplex indices.
  template<typename IntInsertIterator>
  void
  getCornerNeighborhood(const int vertexIndex, IntInsertIterator iter) const;

  //! Determine the corners from the maximum allowed angle deviation.
  void
  determineCorners(Number maxAngleDeviation);

  //! Determine the boundary corners.
  void
  determineBoundaryCorners();
  
  //! Record the indices of the corner vertices.
  void
  recordCorners();
};

END_NAMESPACE_GEOM

#define __geom_PointsOnManifoldN11_ipp__
#include "PointsOnManifoldN11.ipp"
#undef __geom_PointsOnManifoldN11_ipp__

#endif
