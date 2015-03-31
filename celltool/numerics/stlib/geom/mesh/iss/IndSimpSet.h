// -*- C++ -*-

/*! 
  \file IndSimpSet.h
  \brief Implements a mesh that stores vertices and indexed simplices.
*/

#if !defined(__geom_IndSimpSet_h__)
#define __geom_IndSimpSet_h__

#include "../../defs.h"

#include "SimplexIterator.h"

#include "../simplex/Simplex.h"

#include "../../../ads/array/Array.h"

#include <cassert>

#if defined(DEBUG_geom) && !defined(DEBUG_IndSimpSet)
#define DEBUG_IndSimpSet
#endif

BEGIN_NAMESPACE_GEOM

// CONTINUE: Perhaps Mutable should be a template parameter.
//! Class for a mesh that stores vertices and indexed simplices.
/*!
  \param N is the space dimension.
  \param M is the simplex dimension  By default it is N.
  \param A determines whether the mesh will allocate its own memory
  or use externally allocated memory.  By default \c A is true.
  \param T is the number type.  By default it is double.
  \param V is the vertex type, an N-tuple of the number type.  It must be 
  subscriptable.  By default it is ads::FixedArray<N,T>.
  \param IS is the indexed simplex type, a tuple of M+1 integers.  
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
*/
template<int _N,
	 int _M = _N,
	 bool _A = true,
	 typename T = double,
	 typename V = ads::FixedArray<_N,T>,
	 typename IS = Simplex<_M,int> >
class IndSimpSet {
  //
  // Enumerations.
  //

public:

  //! The space dimension, simplex dimension and allocation.
  enum {N = _N, M = _M, A = _A};

  //
  // Public types.
  //

public:

  //! The number type.
  typedef T Number;
  //! A vertex.
  typedef V Vertex;
  //! A simplex of vertices.
  typedef Simplex<M,Vertex> Simplex;
  //! The face of a simplex of vertices.
  typedef typename Simplex::Face SimplexFace;

  //! An indexed simplex.  (A simplex of indices.)
  typedef IS IndexedSimplex;
  //! The face of an indexed simplex.
  typedef typename IndexedSimplex::Face IndexedSimplexFace;

  //! The vertex container.
  typedef ads::Array<1,Vertex,A> VertexContainer;
  //! A vertex const iterator.
  typedef typename VertexContainer::const_iterator VertexConstIterator;
  //! A vertex iterator.
  typedef typename VertexContainer::iterator VertexIterator;

  //! The indexed simplex container.
  typedef ads::Array<1,IndexedSimplex,A> IndexedSimplexContainer;
  //! An indexed simplex const iterator.
  typedef typename IndexedSimplexContainer::const_iterator 
  IndexedSimplexConstIterator;
  //! An indexed simplex iterator.
  typedef typename IndexedSimplexContainer::iterator 
  IndexedSimplexIterator;

  //! A simplex const iterator.
  typedef SimplexIterator<IndSimpSet> SimplexConstIterator;

  //! The size type.
  typedef typename VertexContainer::size_type SizeType;

private:

  //
  // Data
  //

  //! The vertices.
  VertexContainer _vertices;

  //! An indexed simplex is determined by the indices of M+1 vertices.
  IndexedSimplexContainer _indexedSimplices;

public:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    Suppose that we are dealing with a tetrahedron mesh in 3-D.  Below
    we instantiate a mesh that allocates its own memory for the vertices
    and indexed simplices.
    \code
    geom::IndSimpSet<3,3> mesh;
    \endcode
    We can construct the mesh from vertices and indexed simplices stored in
    ADS arrays:
    \code
    typedef geom::IndSimpSet<3,3> ISS;
    typedef typename ISS:Vertex Vertex;
    typedef typename ISS:IndexedSimplex IndexedSimplex;
    ads::Array<1,Vertex> vertices(numberOfVertices);
    ads::Array<1,IndexedSimplex> indexedSimplices(numberOfSimplices);
    ...
    geom::IndSimpSet<3,3> mesh(vertices, indexedSimplices);
    \endcode
    or use C arrays:
    \code
    double* vertices = new[3 * numberOfVertices]
    int* indexedSimplices = new[4 * numberOfSimplices];
    ...
    geom::IndSimpSet<3,3> mesh(numberOfVertices, vertices, numberOfSimplices, simplices);
    \endcode


    We can also make meshes that borrow externally allocated memory.
    One can use ADS arrays:
    \code
    geom::IndSimpSet<3,3,false> mesh(vertices, indexedSimplices);
    \endcode
    or C arrays:
    \code
    geom::IndSimpSet<3,3,false> mesh(numberOfVertices, vertices, numberOfSimplices, indexedSimplices);
    \endcode
  */
  //! @{

  //! Default constructor.  Empty simplex set.
  IndSimpSet() :
    _vertices(),
    _indexedSimplices()
  {}

  //! Construct from arrays of vertices and indexed simplices.
  /*!
    If \c A is true, the arrays will be copied.
    If \c A is false, the memory is adopted.  It will not be freed when 
    the class destructor is called.

    \param vertices is the array of vertices.
    \param indexedSimplices is the array of indexed simplices.
  */
  template<bool A1, bool A2>
  IndSimpSet
  (const ads::Array<1,Vertex,A1>& vertices, 
   const ads::Array<1,IndexedSimplex,A2>& indexedSimplices) :
    _vertices(vertices),
    _indexedSimplices(indexedSimplices)
  {}

  //! Build from arrays of vertices and indexed simplices.
  /*!
    Performs same actions as the constructor.

    \param vertices is the array of vertices.
    \param indexedSimplices is the array of indexed simplices.
  */
  template<bool A1, bool A2>
  void
  build(const ads::Array<1,Vertex,A1>& vertices, 
	const ads::Array<1,IndexedSimplex,A2>& indexedSimplices) {
    _vertices = vertices;
    _indexedSimplices = indexedSimplices;
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
    \param vertices points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplices points to the data for the indexed simplices.
  */
  IndSimpSet(const SizeType numVertices, 
	     void* vertices,
	     const SizeType numSimplices, 
	     void* indexedSimplices) :
    _vertices(numVertices, vertices),
    _indexedSimplices(numSimplices, indexedSimplices)
  {}

  //! Build from pointers to the vertices and indexed simplices.
  /*!
    Performs same actions as the constructor.

    \param numVertices is the number of vertices.
    \param vertices points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplices points to the data for the indexed simplices.
  */
  void
  build(const SizeType numVertices, void* vertices,
	const SizeType numSimplices, void* indexedSimplices);

  //! Construct from pointers to the vertices and indexed simplices.
  /*!
    \c A must be true to use this constructor.

    The objects to which 
    \c vertices and \c indexedSimplices point will be cast to 
    \c Vertex and \c IndexedSimplex, respectively.  Thus they 
    should have the same memory layout as these classes.

    \param numVertices is the number of vertices.
    \param vertices points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplices points to the data for the indexed simplices.
  */
  IndSimpSet(const SizeType numVertices, 
	     const void* vertices,
	     const SizeType numSimplices, 
	     const void* indexedSimplices) :
    _vertices(numVertices, vertices),
    _indexedSimplices(numSimplices, indexedSimplices)
  {
    LOKI_STATIC_CHECK(A == true, MustAllocateOwnMemory);
  }

  //! Build from pointers to the vertices and indexed simplices.
  /*!
    Performs same actions as the constructor.

    \param numVertices is the number of vertices.
    \param vertices points to the data for the vertices.
    \param numSimplices is the number of simplices.
    \param indexedSimplices points to the data for the indexed simplices.
  */
  void
  build(const SizeType numVertices, const void* vertices,
	const SizeType numSimplices, const void* indexedSimplices);

  //! Construct from the number of vertices and simplices.
  /*!
    \c A must be true.

    The vertices and indexed simplices are left uninitialized.
    
    \param numVertices is the number of vertices.
    \param numSimplices is the number of simplices.
  */
  IndSimpSet(const SizeType numVertices, const SizeType numSimplices) :
    _vertices(numVertices),
    _indexedSimplices(numSimplices)
  {}

  //! Build from the number of vertices and simplices.
  /*!
    Performs same actions as the constructor.

    \param numVertices is the number of vertices.
    \param numSimplices is the number of simplices.
  */
  void
  build(const SizeType numVertices, const SizeType numSimplices) {
    _vertices.resize(numVertices);
    _indexedSimplices.resize(numSimplices);
  }

  //! Swap data with another mesh.
  void
  swap(IndSimpSet& x) {
    _vertices.swap(x._vertices);
    _indexedSimplices.swap(x._indexedSimplices);
  }

  //! Copy constructor.
  IndSimpSet(const IndSimpSet& other) :
    _vertices(other._vertices),
    _indexedSimplices(other._indexedSimplices)
  {}

  //! Assignment operator.
  IndSimpSet&
  operator=(const IndSimpSet& other);

  //! Destructor.  Deletes memory only if it was allocated internally.
  virtual
  ~IndSimpSet()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Vertex Accessors
  //! @{

  //! Return the dimension of the space.
  int
  getSpaceDimension() const {
    return N;
  }

  //! Return the number of vertices.
  SizeType
  getVerticesSize() const {
    return _vertices.size();
  }

  //! Return true if there are no vertices.
  SizeType
  areVerticesEmpty() const {
    return getVerticesSize() == 0;
  }

  //! Return a const iterator to the beginning of the vertices.
  VertexConstIterator
  getVerticesBeginning() const { 
    return _vertices.begin();
  }

  //! Return a const iterator to the end of the vertices.
  VertexConstIterator
  getVerticesEnd() const { 
    return _vertices.end();
  }

  //! Return a const reference to the n_th vertex.
  const Vertex&
  getVertex(const int n) const {
    return _vertices[n];
  }

  //! Return a const reference to the vertex container.
  const VertexContainer&
  getVertices() const {
    return _vertices;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Simplex Accessors
  //! @{

  //! Return the dimension of the simplices.
  int
  getSimplexDimension() const {
    return M;
  }

  //! Return the number of simplices.
  SizeType
  getSimplicesSize() const {
    return _indexedSimplices.size();
  }

  //! Return true if there are no simplices.
  SizeType
  areSimplicesEmpty() const {
    return getSimplicesSize() == 0;
  }

  //! Return true if there are no vertices or simplices.
  SizeType
  isEmpty() const {
    return areVerticesEmpty() && areSimplicesEmpty();
  }

  //! Return a const iterator to the beginning of the indexed simplices.
  IndexedSimplexConstIterator
  getIndexedSimplicesBeginning() const { 
    return _indexedSimplices.begin();
  }

  //! Return a const iterator to the end of the indexed simplices.
  IndexedSimplexConstIterator
  getIndexedSimplicesEnd() const { 
    return _indexedSimplices.end();
  }

  //! Return a const reference to the n_th indexed simplex.
  const IndexedSimplex&
  getIndexedSimplex(const int n) const {
    return _indexedSimplices[n];
  }

  //! Return a const reference to the indexed simplex container.
  const IndexedSimplexContainer&
  getIndexedSimplices() const {
    return _indexedSimplices;
  }

  //! Return a const iterator to the beginning of the simplices.
  SimplexConstIterator
  getSimplicesBeginning() const {
    return SimplexConstIterator(*this);
  }

  //! Return a const iterator to the end of the simplices.
  SimplexConstIterator
  getSimplicesEnd() const {
    SimplexConstIterator i(*this);
    i += getSimplicesSize();
    return i;
  }

  //! Return a const reference to the m_th vertex of the n_th simplex.
  const Vertex&
  getSimplexVertex(const int n, const int m) const {
    return _vertices[_indexedSimplices[n][m]];
  }

  //! Get the n_th simplex.
  void
  getSimplex(const int n, Simplex* s) const {
    getSimplex(_indexedSimplices.begin() + n, s);
  }

  //! Get the simplex given an iterator to the indexed simplex.
  void
  getSimplex(IndexedSimplexConstIterator i, Simplex* s) const {
    for (int m = 0; m != M+1; ++m) {
      (*s)[m] = _vertices[(*i)[m]];
    }
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Vertex Manipulators
  //! @{

  //! Return an iterator to the beginning of the vertices.
  VertexIterator
  getVerticesBeginning() { 
    return _vertices.begin();
  }

  //! Return an iterator to the end of the vertices.
  VertexIterator
  getVerticesEnd() { 
    return _vertices.end();
  }

  //! Set the specified vertex.
  void
  setVertex(const int n, const Vertex& vertex) {
    _vertices[n] = vertex;
  }

  //! Return a reference to the vertex container.
  VertexContainer&
  getVertices() {
    return _vertices;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Simplex Manipulators
  //! @{

  //! Return an iterator to the beginning of the indexed simplices.
  IndexedSimplexIterator
  getIndexedSimplicesBeginning() { 
    return _indexedSimplices.begin();
  }

  //! Return an iterator to the end of the indexed simplices.
  IndexedSimplexIterator
  getIndexedSimplicesEnd() { 
    return _indexedSimplices.end();
  }

  //! Return a reference to the indexed simplex container.
  IndexedSimplexContainer&
  getIndexedSimplices() {
    return _indexedSimplices;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Update the topology.
  //! @{

  //! Update the data structure following a change in the topology.
  /*!
    For this class, this function does nothing.  For derived classes,
    it updates data structures that hold auxillary topological information.
  */
  virtual
  void
  updateTopology()
  {}
  
  //! @}
};

END_NAMESPACE_GEOM

#define __geom_IndSimpSet_ipp__
#include "IndSimpSet.ipp"
#undef __geom_IndSimpSet_ipp__

#endif
