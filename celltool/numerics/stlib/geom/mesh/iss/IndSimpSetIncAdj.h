// -*- C++ -*-

/*! 
  \file IndSimpSetIncAdj.h
  \brief An indexed simplex set in N-D that optimizes simplex quality.
*/

#if !defined(__geom_IndSimpSetIncAdj_h__)
#define __geom_IndSimpSetIncAdj_h__

#include "IndSimpSet.h"
#include "SimplexAdj.h"
#include "VertexSimplexInc.h"
#include "IssiaFaceIterator.h"

#if defined(DEBUG_geom) && !defined(DEBUG_IndSimpSetIncAdj)
#define DEBUG_IndSimpSetIncAdj
#endif

BEGIN_NAMESPACE_GEOM

//! An indexed simplex set that stores vertex-simplex incidences and simplex adjacencies.
/*!
  \param N is the space dimension.
  \param M is the simplex dimension  By default it is N.
  \param A determines whether memory will be allocated for the vertices and
  the indexed simplices.  By default \c A is true.
  (Memory will always be allocated for the vertex-simplex incidences and the
  simplex adjacencies.)
  \param T is the number type.  By default it is double.
  \param V is the vertex type, an N-tuple of the number type.  It must be 
  subscriptable.  By default it is ads::FixedArray<N,T>.
  \param IS is the Indexed Simplex type, a tuple of M+1 integers.  
  It must be subscriptable.  By default it is Simplex<M,int>.

  Note that the indices for indexed simplices follow the C convention of 
  starting at 0.  

  The free functions that operate on this class are grouped into the 
  following categories:
  - \ref iss_accessors
  - \ref iss_build
  - \ref iss_equality
  - \ref iss_file_io
  - \ref iss_optimize
  - \ref iss_quality
  - \ref iss_set
  - \ref iss_tile
  - \ref iss_transfer
  - \ref iss_transform

  This class derives from geom::IndSimpSet.  Any function that takes a 
  geom::IndSimpSet as an argument may also take this class as an argument.
  This includes function that build the mesh or modify the topology.
  This functionality is made possible with the update_topology() virtual
  function.  Any free function that modifies the topology of the mesh
  calls update_topology().  In the base class, the function has no effect,
  but in this class, it builds/rebuilds the vertex-simplex incidences
  and the simplex adjacencies.
*/
template<int _N,
	 int _M = _N,
	 bool _A = true,
	 typename T = double,
	 typename V = ads::FixedArray<_N,T>,
	 typename IS = Simplex<_M,int> >
class IndSimpSetIncAdj :
  public IndSimpSet<_N,_M,_A,T,V,IS> {
  //
  // The base type.
  //

public:

  //! The base type.
  typedef IndSimpSet<_N,_M,_A,T,V,IS> Base;

  //
  // Enumerations.
  //

public:

  //! The space dimension, simplex dimension and allocation.
  enum {N = Base::N, M = Base::M, A = Base::A};

  //
  // Public types.
  //

public:

  //
  // Inherited from IndSimpSet.
  //

  //! The number type.
  typedef typename Base::Number Number;
  //! A vertex.
  typedef typename Base::Vertex Vertex;
  //! A simplex of vertices.
  typedef typename Base::Simplex Simplex;
  //! The face of a simplex of vertices.
  typedef typename Base::SimplexFace SimplexFace;

  //! An indexed simplex.  (A simplex of indices.)
  typedef typename Base::IndexedSimplex IndexedSimplex;
  //! The face of an indexed simplex.
  typedef typename Base::IndexedSimplexFace IndexedSimplexFace;

  //! The vertex container.
  typedef typename Base::VertexContainer VertexContainer;
  //! A vertex const iterator.
  typedef typename Base::VertexConstIterator VertexConstIterator;
  //! A vertex iterator.
  typedef typename Base::VertexIterator VertexIterator;

  //! The indexed simplex container.
  typedef typename Base::IndexedSimplexContainer IndexedSimplexContainer;
  //! An indexed simplex const iterator.
  typedef typename Base::IndexedSimplexConstIterator
  IndexedSimplexConstIterator;
  //! An indexed simplex iterator.
  typedef typename Base::IndexedSimplexIterator IndexedSimplexIterator;

  //! A simplex const iterator.
  typedef typename Base::SimplexConstIterator SimplexConstIterator;

  //! The size type.
  typedef typename Base::SizeType SizeType;

  //
  // New types.
  //

  //! The vertex-simplex incidences.
  typedef VertexSimplexInc<M> IncidenceContainer;
  //! Iterator over the vertex-simplex incidences.
  typedef typename IncidenceContainer::ConstIterator IncidenceConstIterator;

  //! The simplex adjacencies.
  typedef SimplexAdj<M> AdjacencyContainer;

  // Faces.
  
  //! A face is determined by a simplex index and an integer in [0..M].
  typedef std::pair<int,int> Face;
  //! A bidirectional, iterator on the faces.
  typedef IssiaFaceIterator<IndSimpSetIncAdj> FaceIterator;

  //
  // Member data.
  //

private:

  //! The vertex-simplex incidences.
  IncidenceContainer _vertexSimplexIncidence;
  //! The simplex adjacencies.
  AdjacencyContainer _simplexAdjacencies;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.
  IndSimpSetIncAdj() :
    Base(),
    _vertexSimplexIncidence(),
    _simplexAdjacencies()
  {}

  //! Copy constructor.
  IndSimpSetIncAdj(const IndSimpSetIncAdj& other) :
    Base(other),
    _vertexSimplexIncidence(other._vertexSimplexIncidence),
    _simplexAdjacencies(other._simplexAdjacencies)
  {}

  //! Assignment operator.
  IndSimpSetIncAdj& 
  operator=(const IndSimpSetIncAdj& other) {
    if (&other != this) {
      Base::operator=(other);
      _vertexSimplexIncidence = other._vertexSimplexIncidence;
      _simplexAdjacencies = other._simplexAdjacencies;
    }
    return *this;
  }

  //! Construct from arrays of vertices and indexed simplices.
  /*!
    If \c A is true, the arrays will be copied.
    If \c A is false, the memory is adopted.  It will not be freed when 
    the class destructor is called.

    \param vertices is the array of vertices.
    \param indexedSimplices is the array of indexed simplices.
  */
  template <bool A1, bool A2>
  IndSimpSetIncAdj(const ads::Array<1,Vertex,A1>& vertices, 
		   const ads::Array<1,IndexedSimplex,A2>& indexedSimplices) :
    Base(vertices, indexedSimplices),
    _vertexSimplexIncidence(getVerticesSize(), getIndexedSimplices()),
    _simplexAdjacencies(getIndexedSimplices(), _vertexSimplexIncidence)
  {}

  //! Build from arrays of vertices and indexed simplices.
  /*!
    Performs same actions as the constructor.

    \param vertices is the array of vertices.
    \param indexedSimplices is the array of indexed simplices.
  */
  template <bool A1, bool A2>
  void
  build(const ads::Array<1,Vertex,A1>& vertices, 
	const ads::Array<1,IndexedSimplex,A2>& indexedSimplices) {
    Base::build(vertices, indexedSimplices);
    updateTopology();
  }

  //! Construct from pointers to the vertices and indexed simplices.
  /*!
    If \c A is true, the arrays will be copied.
    If \c A is false, the memory is adopted.  It will not be freed
    when the class destructor is called.  The objects to which 
    \c vertices and \c indexedSimplices point will be cast to 
    \c Vertex and \c IndexedSimplex, respectively.  Thus they 
    should have the same memory layout as these classes.

    \param numVertices is the number of vertices.
    \param verticesData points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplicesData points to the data for the indexed simplices.
   */
  IndSimpSetIncAdj(const SizeType numVertices, 
		   void* verticesData,
		   const SizeType numSimplices, 
		   void* indexedSimplicesData) :
    Base(numVertices, verticesData, numSimplices, 
	      indexedSimplicesData),
    _vertexSimplexIncidence(getVerticesSize(), getIndexedSimplices()),
    _simplexAdjacencies(getIndexedSimplices(), _vertexSimplexIncidence)
  {}

  //! Build from pointers to the vertices and indexed simplices.
  /*!
    Performs the same action as the constructor.

    \param numVertices is the number of vertices.
    \param verticesData points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplicesData points to the data for the indexed simplices.
  */
  void
  build(const SizeType numVertices, void* verticesData,
	const SizeType numSimplices, void* indexedSimplicesData) {
    Base::build(numVertices, verticesData, numSimplices, indexedSimplicesData);
    updateTopology();
  }

  //! Construct from pointers to the vertices and indexed simplices.
  /*!
    \c A must be  true to use this constructor.
    The objects to which 
    \c vertices and \c indexedSimplices point will be cast to 
    \c Vertex and \c IndexedSimplex, respectively.  Thus they 
    should have the same memory layout as these classes.

    \param numVertices is the number of vertices.
    \param verticesData points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplicesData points to the data for the indexed simplices.
   */
  IndSimpSetIncAdj(const SizeType numVertices, 
		   const void* verticesData,
		   const SizeType numSimplices, 
		   const void* indexedSimplicesData) :
    Base(numVertices, verticesData, numSimplices, 
	      indexedSimplicesData),
    _vertexSimplexIncidence(getVerticesSize(), getIndexedSimplices()),
    _simplexAdjacencies(getIndexedSimplices(), _vertexSimplexIncidence)
  {}

  //! Build from pointers to the vertices and indexed simplices.
  /*!
    Performs the same action as the constructor.

    \param numVertices is the number of vertices.
    \param verticesData points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplicesData points to the data for the indexed simplices.
  */
  void
  build(const SizeType numVertices, const void* verticesData,
	const SizeType numSimplices, const void* indexedSimplicesData) {
    Base::build(numVertices, verticesData, numSimplices, indexedSimplicesData);
    updateTopology();
  }

  //! Construct from the number of vertices and simplices.
  /*!
    \c A must be true.

    The vertices and indexed simplices are left uninitialized.  The incidence
    and adjacency relations are not built.

    \param numVertices is the number of vertices.
    \param numSimplices is the number of simplices.
  */
  IndSimpSetIncAdj(const SizeType numVertices, const SizeType numSimplices) :
    Base(numVertices, numSimplices),
    _vertexSimplexIncidence(),
    _simplexAdjacencies()
  {}

  //! Build from the number of vertices and simplices.
  /*!
    Performs the same action as the constructor.

    \param numVertices is the number of vertices.
    \param numSimplices is the number of simplices.
  */
  void
  build(const SizeType numVertices, const SizeType numSimplices) {
    Base::build(numVertices, numSimplices);
    {
      // Clear the vertex-simplex incidence relations.
      IncidenceContainer empty;
      _vertexSimplexIncidence.swap(empty);
    }
    {
      // Clear the adjacency relations.
      AdjacencyContainer empty;
      _simplexAdjacencies.swap(empty);
    }
  }

  //! Swap data with another mesh.
  void
  swap(IndSimpSetIncAdj& x) {
    Base::swap(x);
    _vertexSimplexIncidence.swap(x._vertexSimplexIncidence);
    _simplexAdjacencies.swap(x._simplexAdjacencies);
  }

  //! Construct from an indexed simplex set.
  /*!
    \param iss is the indexed simplex set.
  */
  IndSimpSetIncAdj(const Base& iss) :
    Base(iss),
    _vertexSimplexIncidence(getVerticesSize(), getIndexedSimplices()),
    _simplexAdjacencies(getIndexedSimplices(), _vertexSimplexIncidence)
  {}

  //! Destructor.  Free any memory that was allocated.
  virtual
  ~IndSimpSetIncAdj()
  {}

  //! @}
  //--------------------------------------------------------------------------
  /*! \name Vertex Accessors
    Inherited from IndSimpSet.
  */
  //! @{

  //! Return the dimension of the space.
  using Base::getSpaceDimension;

  //! Return the number of vertices.
  using Base::getVerticesSize;

  //! Return true if there are no vertices.
  using Base::areVerticesEmpty;

  //! Return a const iterator to the beginning of the vertices.
  using Base::getVerticesBeginning;

  //! Return a const iterator to the end of the vertices.
  using Base::getVerticesEnd;

  //! Return a const reference to the n_th vertex.
  using Base::getVertex;

  //! Return a const reference to the vertex container.
  using Base::getVertices;

  //! @}
  //--------------------------------------------------------------------------
  /*! \name Simplex Accessors
    Inherited from IndSimpSet.
   */
  //! @{

  //! Return the dimension of the simplices.
  using Base::getSimplexDimension;

  //! Return the number of simplices.
  using Base::getSimplicesSize;

  //! Return true if there are no simplices.
  using Base::areSimplicesEmpty;

  //! Return true if there are no vertices or simplices.
  using Base::isEmpty;

  //! Return a const iterator to the beginning of the indexed simplices.
  using Base::getIndexedSimplicesBeginning;

  //! Return a const iterator to the end of the indexed simplices.
  using Base::getIndexedSimplicesEnd;

  //! Return a const reference to the n_th indexed simplex.
  using Base::getIndexedSimplex;

  //! Return a const reference to the indexed simplex container.
  using Base::getIndexedSimplices;

  //! Return a const iterator to the beginning of the simplices.
  using Base::getSimplicesBeginning;

  //! Return a const iterator to the end of the simplices.
  using Base::getSimplicesEnd;

  //! Return a const reference to the m_th vertex of the n_th simplex.
  using Base::getSimplexVertex;

  //! Get the n_th simplex.
  using Base::getSimplex;

  //! Get the simplex given an iterator to the indexed simplex.
  //using Base::getSimplex;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Vertex-Simplex Incidence Accessors
  //! @{

  //! A const reference to the vertex-simplex incidence.
  const IncidenceContainer&
  getVertexSimplexIncidence() const {
    return _vertexSimplexIncidence;
  }

  //! Return the number of incident simplices to the n_th vertex.
  SizeType
  getIncidentSize(const int n) const {
    return _vertexSimplexIncidence.getSize(n);
  }

  //! Return true if the n_th vertex has no incident simplices.
  bool 
  isIncidentEmpty(const int n) const { 
    return _vertexSimplexIncidence.isEmpty(n);
  }

  //! Return a const iterator on simplex indices to the first incident simplex index of the n_th vertex.
  IncidenceConstIterator 
  getIncidentBeginning(const int n) const { 
    return _vertexSimplexIncidence.getBeginning(n);
  }

  //! Return a const iterator on simplex indices to one past the last incident simplex index of the n_th vertex.
  IncidenceConstIterator 
  getIncidentEnd(const int n) const { 
    return _vertexSimplexIncidence.getEnd(n);
  }

  //! Return the m_th incident simplex index of the n_th vertex.
  int
  getIncident(const int n, const int m) const {
    return _vertexSimplexIncidence(n, m);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Simplex Adjacency Accessors
  //! @{

  //! Return a const reference to the simplex adjacencies.
  const AdjacencyContainer&
  getSimplexAdjacencies() const {
    return _simplexAdjacencies;
  }

  //! Return number of simplices adjacent to the n_th simplex.
  int
  getAdjacentSize(const int n) const {
    return _simplexAdjacencies.getSize(n);
  }

  //! Return the index of the m_th adjacent simplex to the n_th simplex.
  /*!
    An index of -1 indicates that there is no adjacent simplex.
  */
  int
  getAdjacent(const int n, const int m) const {
    return _simplexAdjacencies(n, m);
  }

  //! Return the index of the n_th simplex in its m_th adjacent neighbor.
  int
  getMirrorIndex(const int n, const int m) const {
    const int a = getAdjacent(n, m);
    if (a == -1) {
      return -1;
    }
#ifdef DEBUG_geom
    const int mi = _simplexAdjacencies(a).find_index(n);
    assert(0 <= mi && mi < M + 1);
    return mi;
#else
    return _simplexAdjacencies(a).find_index(n);
#endif
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Face accessors.
  //! @{

  //! Return the number of faces.
  /*!
    \note This is a slow function.  It counts the faces.
  */
  SizeType
  computeFacesSize() const {
    return int(std::distance(getFacesBeginning(), getFacesEnd()));
  }

  //! Return the beginning of the faces.
  FaceIterator
  getFacesBeginning() const {
    FaceIterator x(this, 0);
    return x;
  }

  //! Return the end of the faces.
  FaceIterator
  getFacesEnd() const {
    return FaceIterator(this, getSimplicesSize());
  }

  //! Return true if the face is on the boundary.
  bool
  isOnBoundary(const Face& f) const {
    return getAdjacent(f.first, f.second) == -1;
  }

  //! Return true if the face is on the boundary.
  bool
  isOnBoundary(const FaceIterator& f) const {
    return isOnBoundary(*f);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Other Accessors
  //! @{

  //! Return true if the vertex is on the boundary of the mesh.
  /*!
    \param index is the index of a vertex.
  */
  bool
  isVertexOnBoundary(int index) const;

  //! @}
  //--------------------------------------------------------------------------
  // Vertex Manipulators
  // Inherited from IndSimpSet.
  // These are already listed with the vertex accessors.

  // Return an iterator to the beginning of the vertices.
  //using Base::getVerticesBeginning;

  // Return an iterator to the end of the vertices.
  //using Base::getVerticesEnd;

  // Return a reference to the vertex container.
  //using Base::getVertices;

  //--------------------------------------------------------------------------
  //! \name Simplex Manipulators
  //! @{

  // Return an iterator to the beginning of the indexed simplices.
  //using Base::getIndexedSimplicesBeginning;

  // Return an iterator to the end of the indexed simplices.
  //using Base::getIndexedSimplicesEnd;

  // Return a reference to the indexed simplex container.
  //using Base::getIndexedSimplices;

  //! Reverse the orientation of the n_th simplex.
  void
  reverseOrientation(const int n) {
    std::swap(getIndexedSimplices()[n][0], getIndexedSimplices()[n][1]);
    std::swap(_simplexAdjacencies(n)[0], _simplexAdjacencies(n)[1]);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Update the topology.
  //! @{

  //! Update the data structure following a change in the topology.
  /*!
    Update the vertex-simplex incidences and simplex adjacencies following
    a change in the topology.
  */
  virtual
  void
  updateTopology() {
    _vertexSimplexIncidence.build(getVerticesSize(), getIndexedSimplices());
    _simplexAdjacencies.build(getIndexedSimplices(), _vertexSimplexIncidence);
  }
  
  //! @}
};

END_NAMESPACE_GEOM

#define __geom_IndSimpSetIncAdj_ipp__
#include "IndSimpSetIncAdj.ipp"
#undef __geom_IndSimpSetIncAdj_ipp__

#endif
