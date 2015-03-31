// -*- C++ -*-

/*! 
  \file IndexedEdgePolyhedron.h
  \brief Implements a class for an indexed edge polyhedron in 3-D.
*/

#if !defined(__geom_IndexedEdgePolyhedron_h__)
#define __geom_IndexedEdgePolyhedron_h__

#include "../kernel/BBox.h"

#include <vector>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_IndexedEdgePolyhedron)
#define DEBUG_IndexedEdgePolyhedron
#endif

BEGIN_NAMESPACE_GEOM

//! Class for an indexed edge polyhedron in 3-D.
/*! 
  \param T is the number type.  By default it is double.

  This polyhedron stores vertices and indexed edges.  (An edge is described
  by a pair of vertex indices.)  With this representation, one can efficiently
  transform the vertices.
*/
template<typename T = double>
class IndexedEdgePolyhedron {
  //
  // Public types.
  //

public:

  //! The floating point number type.
  typedef T Number;
  //! The size type is a signed integer.
  typedef int SizeType;
  //! The representation of a point in 3 dimensions.
  typedef ads::FixedArray<3,Number> Point;
  //! The representation of an indexed edge.
  typedef ads::FixedArray<2,int> IndexedEdge;

  //
  // Private types.
  //

private:

  //! The vertices of the polyhedron.
  typedef std::vector<Point> VertexContainer;

  //! The edges of the polyhedron.
  typedef std::vector<IndexedEdge> EdgeContainer;

  //
  // More public types.
  //

public:

  //! A const iterator on vertices.
  typedef typename VertexContainer::const_iterator VertexConstIterator;
  //! An iterator on vertices.
  typedef typename VertexContainer::iterator VertexIterator;
  //! A const iterator on indexed edges.
  typedef typename EdgeContainer::const_iterator EdgeConstIterator;
  //! An iterator on indexed edges.
  typedef typename EdgeContainer::iterator EdgeIterator;

  // 
  // Data
  //

private:

  //! The vertices of the polyhedron.
  VertexContainer _vertices;

  //! The edges of the polyhedron.
  EdgeContainer _edges;
  
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Uninitialized memory.
  IndexedEdgePolyhedron() :
    _vertices(),
    _edges() 
  {}
  
  //! Copy constructor.
  IndexedEdgePolyhedron(const IndexedEdgePolyhedron& other) : 
    _vertices(other._vertices),
    _edges(other._edges) 
  {}

  //! Assignment operator.
  IndexedEdgePolyhedron& 
  operator=(const IndexedEdgePolyhedron& other);
  
  //! Trivial destructor.
  ~IndexedEdgePolyhedron()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions.
  // @{

  //! Make a BBox containing the polyhedron.
  void 
  computeBBox(BBox<3,Number>* bb) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the number of vertices.
  SizeType
  getVerticesSize() const {
    return SizeType(_vertices.size());
  }

  //! Return the number of edges.
  SizeType
  getEdgesSize() const {
    return SizeType(_edges.size());
  }

  //! Return a const reference to the specified vertex.
  const Point& 
  getVertex(const int n) const { 
    return _vertices[n]; 
  }

  //! Return a const reference to the specified edge.
  const IndexedEdge& 
  getEdge(const int n) const { 
    return _edges[n];
  }

  //! Return a const reference to the specified edge source.
  const Point&
  getEdgeSource(const int n) const {
    return _vertices[_edges[n][0]];
  }

  //! Return a const reference to the specified edge target.
  const Point&
  getEdgeTarget(const int n) const {
    return _vertices[_edges[n][1]];
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  // Return the beginning of the vertices.
  VertexIterator
  getVerticesBeginning() {
    return _vertices.begin();
  }

  // Return the end of the vertices.
  VertexIterator
  getVerticesEnd() {
    return _vertices.end();
  }

  // Return the beginning of the indexed edges.
  EdgeIterator
  getEdgesBeginning() {
    return _edges.begin();
  }

  // Return the end of the indexed edges.
  EdgeIterator
  getEdgesEnd() {
    return _edges.end();
  }

  // CONTINUE REMOVE
#if 0
  //! Return a reference to the vertices.
  std::vector<Point>& 
  vertices() { 
    return _vertices; 
  }

  //! Return a reference to the edges.
  std::vector<IndexedEdge>& 
  edges() { 
    return _edges; 
  }
#endif

  //! Add a vertex.
  void
  insertVertex(const Point& x) {
    _vertices.push_back(x);
  }

  //! Add an edge.
  void 
  insertEdge(const int i, const int j) {
#ifdef DEBUG_IndexedEdgePolyhedron
    assert(0 <= i && i < getVerticesSize() && 
	   0 <= j && j < getVerticesSize());
#endif
    _edges.push_back(IndexedEdge(i, j));
  }

  //! Clear the vertices and edges.
  void
  clear() {
    _vertices.clear();
    _edges.clear();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.

  bool
  isEqualTo(const IndexedEdgePolyhedron& x) const {
    return _vertices == x._vertices && _edges == x._edges; 
  }

  //@}

};


//-----------------------------------------------------------------------------
// Equality Operators
//-----------------------------------------------------------------------------


//! Return true if the polyhedra are equal.
/*! \relates IndexedEdgePolyhedron */
template<typename T>
inline
bool 
operator==(const IndexedEdgePolyhedron<T>& a, 
	   const IndexedEdgePolyhedron<T>& b) { 
  return a.isEqualTo(b);
}

//! Return true if the polyhedra are not equal.
/*! \relates IndexedEdgePolyhedron */
template<typename T>
inline
bool 
operator!=(const IndexedEdgePolyhedron<T>& a, 
	   const IndexedEdgePolyhedron<T>& b) { 
  return !(a == b);
}

END_NAMESPACE_GEOM

#define __geom_IndexedEdgePolyhedron_ipp__
#include "IndexedEdgePolyhedron.ipp"
#undef __geom_IndexedEdgePolyhedron_ipp__

#endif
