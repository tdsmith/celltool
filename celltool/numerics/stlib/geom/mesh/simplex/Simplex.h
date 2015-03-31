// -*- C++ -*-

/*! 
  \file geom/mesh/simplex/Simplex.h
  \brief A simplex in N dimensions.
*/

#if !defined(__geom_Simplex_h__)
#define __geom_Simplex_h__

#include "../../defs.h"

#include "../../kernel/content.h"
#include "../../kernel/BBox.h"

#include "../../../ads/array/FixedArray.h"

#include <cassert>

#if defined(DEBUG_geom) && !defined(DEBUG_Simplex)
#define DEBUG_Simplex
#endif

BEGIN_NAMESPACE_GEOM

//! A simplex in N dimensions.
/*!
  \param N is the dimension.
  \param V is the vertex type.
  \param T is the number type.  By default it is double.

  This simplex stores N+1 vertices.  It does not store them in lexicographical
  order.  This has the advantage that we do not need to store a sign.  
  Then we can treat a container of N+1 vertices as a simplex.  It 
  has the disadvantage that it is difficult to compare simplices for 
  equivalence.  (Simplices are equivalent if the vertices of one are an even
  permutation of the vertices of the other.)  

  <b>Usage</b>

  One can represent a N-simplex in with Cartesian points as vertices 
  by choosing \c ads::FixedArray<N,double> for the vertex type.
  \code
  geom::Simplex< N, ads::FixedArray<N,double> > simplex;
  \endcode
  One can represent an indexed simplex for use in an indexed simplex set
  (see geom::IndexedSimplexSet) by choosing an \c int for the vertex type.
  \code
  geom::Simplex<N,int> indexedSimplex;
  \endcode
*/
template<int N, typename V, typename T = double>
class Simplex {
  //
  // Public types.
  //

public:

  //! The vertex type.
  typedef V Vertex;

  //! The number type.
  typedef T Number;

  //! The container for the vertices.
  typedef ads::FixedArray<N+1,Vertex> VertexContainer;

  //! The vertex type.
  typedef typename VertexContainer::value_type Value;

  //! An iterator on the vertices.
  typedef typename VertexContainer::iterator Iterator;

  //! A const iterator on the vertices.
  typedef typename VertexContainer::const_iterator ConstIterator;
  
  //! A reference to a vertex.
  typedef typename VertexContainer::reference Reference;

  //! A const reference to a vertex.
  typedef typename VertexContainer::const_reference ConstReference;

  //! A parameter type for a vertex.
  typedef typename VertexContainer::parameter_type Parameter;

  //! The (N-1)-face of an N-simplex.
  typedef Simplex<N-1,V,T> Face;

private:

  //
  // Member data.
  //

  VertexContainer _vertices;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  Simplex() :
    _vertices()
  {}

  //! Copy constructor.
  Simplex(const Simplex& other) :
    _vertices(other._vertices)
  {}

  //! Construct from the vertices.
  explicit
  Simplex(const VertexContainer& vertices) :
    _vertices(vertices)
  {}

  //! Build from the vertices.
  void
  build(const VertexContainer& vertices) {
    _vertices = vertices;
  }

  //! Construct a 0-D simplex from a vertex.
  explicit
  Simplex(Parameter v) :
    _vertices(v) {
    LOKI_STATIC_CHECK(N == 0, N_is_0);
  }

  //! Build a 0-D simplex from a vertex.
  void
  build(Parameter v) {
    LOKI_STATIC_CHECK(N == 0, N_is_0);
    _vertices[0] = v;
  }

  //! Construct a 1-D simplex from two vertices.
  Simplex(Parameter v0, Parameter v1) :
    _vertices(v0, v1) {
    LOKI_STATIC_CHECK(N == 1, N_is_1);
  }

  //! Build a 1-D simplex from two vertices.
  void
  build(Parameter v0, Parameter v1) {
    LOKI_STATIC_CHECK(N == 1, N_is_1);
    _vertices[0] = v0;
    _vertices[1] = v1;
  }

  //! Construct a 2-D simplex from three vertices.
  Simplex(Parameter v0, Parameter v1, Parameter v2) :
    _vertices(v0, v1, v2) {
    LOKI_STATIC_CHECK(N == 2, N_is_2);
  }

  //! Build a 2-D simplex from three vertices.
  void
  build(Parameter v0, Parameter v1, Parameter v2) {
    LOKI_STATIC_CHECK(N == 2, N_is_2);
    _vertices[0] = v0;
    _vertices[1] = v1;
    _vertices[2] = v2;
  }

  //! Construct a 3-D simplex from four vertices.
  Simplex(Parameter v0, Parameter v1, Parameter v2, Parameter v3) :
    _vertices(v0, v1, v2, v3) {
    LOKI_STATIC_CHECK(N == 3, N_is_3);
  }

  //! Build a 3-D simplex from four vertices.
  void
  build(Parameter v0, Parameter v1, Parameter v2, Parameter v3) {
    LOKI_STATIC_CHECK(N == 3, N_is_3);
    _vertices[0] = v0;
    _vertices[1] = v1;
    _vertices[2] = v2;
    _vertices[3] = v3;
  }

  //! Construct from an array of vertices and and indexed simplex.
  template<typename VertRAIter, typename IndSimp>
  Simplex(VertRAIter vertices, const IndSimp& indexed_simplex) :
    _vertices() {
    for (int n = 0; n != N + 1; ++n) {
      _vertices[n] = vertices[indexed_simplex[n]];
    }
  }

  //! Build from an array of vertices and and indexed simplex.
  template<typename VertRAIter, typename IndSimp>
  void
  build(VertRAIter vertices, const IndSimp& indexed_simplex) {
    for (int n = 0; n != N + 1; ++n) {
      _vertices[n] = vertices[indexed_simplex[n]];
    }
  }

  //! Assignment operator.
  Simplex& 
  operator=(const Simplex& other) {
    if (&other != this) {
      _vertices = other._vertices;
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~Simplex()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name STL-style Accessors.
  //! @{

  //! Return a const iterator to the beginning of the vertices.
  ConstIterator
  begin() const {
    return _vertices.begin();
  }
  
  //! Return a const iterator to the end of the vertices.
  ConstIterator
  end() const {
    return _vertices.end();
  }

  //! Return the number of vertices.
  int
  size() const {
    return N + 1;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors
  //! @{

  //! Return a const reference to the vertex container.
  const VertexContainer&
  getVertices() const {
    return _vertices;
  }

  //! Return the n_th vertex.
  Parameter
  operator[](const int n) const {
    return _vertices[n];
  }

  //! Return a const iterator to the beginning of the vertices.
  ConstIterator
  getBeginning() const {
    return _vertices.begin();
  }
  
  //! Return a const iterator to the end of the vertices.
  ConstIterator
  getEnd() const {
    return _vertices.end();
  }

  //! Return the number of vertices.
  int
  getSize() const {
    return N + 1;
  }

  //! Return true if the simplex has the given vertex.
  template<typename Comparable>
  bool
  hasVertex(const Comparable& v) const;
  
  //! Return true if the simplex has the given vertex.
  /*!
    If true, compute the index of the vertex.
  */
  template<typename Comparable>
  bool
  hasVertex(const Comparable& v, int* n) const;
  
  //! Return true if the simplex has the given face as a sub-simplex.
  /*!
    This function does not check the orientation of the face.  It returns 
    true if the simplex has each of the vertices in the face.
  */
  template<int M>
  bool
  hasFace(const Simplex<M,V,T>& f) const;
  
  //! Return the index of the given vertex.
  template<typename Comparable>
  int
  getVertexIndex(const Comparable& v) const;

  //! Return the face obtained by removing the n_th vertex.
  /*!
    For the simplex (v[0], ... v[N]) return
    (-1)^n (v[0], ..., v[n-1], v[n+1], ..., v[N]).
  */
  Face
  getFace(const int n) const {
    Face f;
    getFace(n, &f);
    return f;
  }

  //! Get the face obtained by removing the n_th vertex.
  /*!
    For the simplex (v[0], ... v[N]) the face is 
    (-1)^n (v[0], ..., v[n-1], v[n+1], ..., v[N]).
  */
  void
  getFace(const int n, Face* f) const;

  //! Calculate a bounding box around the simplex.
  void
  computeBBox(BBox<N,Number>* bb) const {
    bb->bound(begin(), end());
  }

  //! Calculate the centroid of the simplex.
  void
  computeCentroid(Vertex* centroid) const {
    *centroid = Number(0);
    for (int n = 0; n != N+1; ++n) {
      *centroid += _vertices[n];
    }
    *centroid /= Number(N + 1);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name STL-style Manipulators.
  //! @{

  //! Return an iterator to the beginning of the vertices.
  Iterator
  begin() {
    return _vertices.begin();
  }
  
  //! Return an iterator to the end of the vertices.
  Iterator
  end() {
    return _vertices.end();
  }
  
  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //! @{

  // CONTINUE: I think I should remove this.
#if 0
  //! Return a reference to the vertex container.
  VertexContainer&
  vertices() {
    return _vertices;
  }
#endif

  //! Return a reference to the n_th vertex.
  Reference
  operator[](const int n) {
    return _vertices[n];
  }
  
  //! Return an iterator to the beginning of the vertices.
  Iterator
  getBeginning() {
    return _vertices.begin();
  }
  
  //! Return an iterator to the end of the vertices.
  Iterator
  getEnd() {
    return _vertices.end();
  }
  
  //! Set the vertices.
  void
  set(const VertexContainer& vertices) {
    _vertices = vertices;
  }

  //! Set each vertex to the specified value.
  void
  setEach(Parameter value) {
    _vertices = value;
  }

  //! Reverse the orientation of the simplex.
  void
  negate() {
    if (N > 0) {
      std::swap(_vertices[0], _vertices[1]);
    }
  }

  //! @}
};

//
// Equality Tests
//

//! Return true if the vertices are given in the same order.
/*!
  \relates Simplex

  Note: This does not check if the vertices of \c x are an even permutation
  of the vertices of \c y.
 */
template<int N, typename V, typename T>
bool
operator==(const Simplex<N,V,T>& x, const Simplex<N,V,T>& y) {
  return (x.getVertices() == y.getVertices());
}

//! Return true if the vertices are not given in the same order.
/*!
  \relates Simplex
*/
template<int N, typename V, typename T>
bool
operator!=(const Simplex<N,V,T>& x, const Simplex<N,V,T>& y) {
  return !(x == y);
}

//! Return true if the two simplices have the same orientation.
/*!
  \pre \c x and \c y must have the same vertices.

  \relates Simplex
*/
template<typename V, typename T>
bool
haveSameOrientation(const Simplex<0,V,T>& x, const Simplex<0,V,T>& y) {
#ifdef DEBUG_geom
  assert(x[0] == y[0]);
#endif
  return true;
}

//! Return true if the two simplices have the same orientation.
/*!
  \pre \c x and \c y must have the same vertices.

  \relates Simplex
*/
template<typename V, typename T>
bool
haveSameOrientation(const Simplex<1,V,T>& x, const Simplex<1,V,T>& y) {
#ifdef DEBUG_geom
  assert(x.hasVertex(y[0]) && x.hasVertex(y[1]));
#endif
  return x[0] == y[0];
}

//! Return true if the two simplices have the same orientation.
/*!
  \pre \c x and \c y must have the same vertices.

  \relates Simplex
*/
template<typename V, typename T>
bool
haveSameOrientation(const Simplex<2,V,T>& x, const Simplex<2,V,T>& y) {
#ifdef DEBUG_geom
  assert(x.hasVertex(y[0]) && x.hasVertex(y[1]) && x.hasVertex(y[2]));
#endif
  if (x[0] == y[0]) {
    return x[1] == y[1];
  }
  else if (x[0] == y[1]) {
    return x[1] == y[2];
  }
  // else x[0] == y[2]
  return x[1] == y[0];
}


//
// File I/O
//

//! Write the vertices.
/*!
  \relates Simplex
*/
template<int N, typename V, typename T>
inline
std::ostream&
operator<<(std::ostream& out, const Simplex<N,V,T>& x) {
  return out << x.getVertices();
}

//! Read the vertices.
/*!
  \relates Simplex
*/
template<int N, typename V, typename T>
inline
std::istream&
operator>>(std::istream& in, Simplex<N,V,T>& x) {
  for (int n = 0; n != N + 1; ++n) {
    in >> x[n];
  }
  return in;
}

END_NAMESPACE_GEOM

#define __geom_Simplex_ipp__
#include "Simplex.ipp"
#undef __geom_Simplex_ipp__

#endif
