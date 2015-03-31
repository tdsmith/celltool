// -*- C++ -*-

/*! 
  \file SmrCell.h
  \brief Class for a cell in a simplicial mesh that stores the handles to the adjacent cells.
*/

#if !defined(__geom_mesh_simplicial_SmrCell_h__)
#define __geom_mesh_simplicial_SmrCell_h__

#include "../../defs.h"

#include "../simplex/topology.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SmrCell)
#define DEBUG_SmrCell
#endif

BEGIN_NAMESPACE_GEOM

//! Cell in a simplicial mesh that stores handles to the adjacent cells.
/*!
  \param SMR is the simplicial mesh data structure.
*/
template<class SMR>
class SmrCell {
  //
  // Enumerations.
  //

public:

  //! The simplex dimension.
  enum {M = SMR::M};

  //
  // Public types.
  //

public:

  //! The simplicial mesh.
  typedef SMR Mesh;

  //! An iterator to a cell.
  typedef typename Mesh::CellIterator CellIterator;
  //! An iterator to a const cell.
  typedef typename Mesh::CellConstIterator CellConstIterator;

  //! An iterator to a node.
  typedef typename Mesh::NodeIterator NodeIterator;
  //! An iterator to a const node.
  typedef typename Mesh::NodeConstIterator NodeConstIterator;
  //! The vertex (a Cartesian point).
  typedef typename Mesh::Vertex Vertex;
  //! The number type.
  typedef typename Mesh::Number Number;

  //! The simplex of node iterators.
  typedef Simplex<M,NodeIterator> NodeIteratorSimplex;
  //! The simplex of cell iterators.
  typedef Simplex<M,CellIterator> CellIteratorSimplex;

  //
  // Private types.
  //

private:

  //
  // Public types.
  //

public:

  //! A face of the cell is a simplex of \c M vertex iterators.
  typedef typename NodeIteratorSimplex::Face Face;
      
  //
  // Data
  //

private:
  
  //! The incident nodes.
  NodeIteratorSimplex _nodes;
  //! The adjacent cells.
  CellIteratorSimplex _neighbors;
  //! The identifier of this simplex.
  mutable int _identifier;
  //! An iterator to this cell.
  CellIterator _self;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Null iterators.
  SmrCell() :
    _nodes(),
    _neighbors(),
    _identifier(-1),
    _self(0) {
    _nodes.setEach(NodeIterator(0));
    _neighbors.setEach(CellIterator(0));
  }

  //! Construct a 1-D cell from the vertices and neighbors.
  SmrCell(const NodeIterator v0, const NodeIterator v1, 
	  const CellIterator c0 = CellIterator(0), 
	  const CellIterator c1 = CellIterator(0),
	  const int identifier = -1) :
    _nodes(v0, v1),
    _neighbors(c0, c1),
    _identifier(identifier),
    _self(0) {
    LOKI_STATIC_CHECK(M == 1, TheSimplexDimensionMustBe1);
  }
  
  //! Construct a 2-D cell from the vertices and neighbors.
  SmrCell(const NodeIterator v0, const NodeIterator v1, 
	  const NodeIterator v2, 
	  const CellIterator c0 = CellIterator(0), 
	  const CellIterator c1 = CellIterator(0), 
	  const CellIterator c2 = CellIterator(0),
	  const int identifier = -1) :
    _nodes(v0, v1, v2),
    _neighbors(c0, c1, c2),
    _identifier(identifier),
    _self(0) {
    LOKI_STATIC_CHECK(M == 2, TheSimplexDimensionMustBe2);
  }
  
  //! Construct a 3-D cell from the vertices and neighbors.
  SmrCell(const NodeIterator v0, const NodeIterator v1, 
	  const NodeIterator v2, const NodeIterator v3, 
	  const CellIterator c0 = CellIterator(0), 
	  const CellIterator c1 = CellIterator(0), 
	  const CellIterator c2 = CellIterator(0), 
	  const CellIterator c3 = CellIterator(0),
	  const int identifier = -1) :
    _nodes(v0, v1, v2, v3),
    _neighbors(c0, c1, c2, c3),
    _identifier(identifier),
    _self(0) {
    LOKI_STATIC_CHECK(M == 3, TheSimplexDimensionMustBe3);
  }
  
  //! Copy constructor.
  SmrCell(const SmrCell& other) :
    _nodes(other._nodes),
    _neighbors(other._neighbors),
    _identifier(other._identifier),
    _self(other._self)
  {}

  //! Trivial destructor.
  ~SmrCell()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  //! @{
      
  //! Assignment operator.
  SmrCell& 
  operator=(const SmrCell& other) {
    if (&other != this) {
      _nodes = other._nodes;
      _neighbors = other._neighbors;
      _identifier = other._identifier;
      _self = other._self;
    }
    return *this;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the identifier of this simplex.
  /*!
    Typically, the identifier is in the range [0...num_simplices).
    and a value of -1 indicates that the identifier has not been calculated.
  */
  int
  getIdentifier() const {
    return _identifier;
  }

  //! Return a const iterator to this cell.
  CellConstIterator
  getSelf() const {
    return _self;
  }

  //! Return a const iterator to the m_th node.
  NodeConstIterator
  getNode(const int m) const {
    return _nodes[m];
  }

  //! Return the simplex of node iterators.
  const NodeIteratorSimplex&
  getNodes() const {
    return _nodes;
  }

  //! Return the index of the specified node.
  int
  getIndex(NodeConstIterator node) const {
    return _nodes.getVertexIndex(node);
  }

  //! Return the index of the specified node.
  int
  getIndex(NodeIterator node) const {
    return _nodes.getVertexIndex(node);
  }

  //! Return the index of the specified vertex.
  /*!
    The vertex is specified by a face of the cell.
  */
  int
  getIndex(const Face& f) const {
    for (int m = 0; m != M + 1; ++m) {
      if (! f.hasVertex(_nodes[m])) {
	return m;
      }
    }
    assert(false);
    return -1;
  }

  //! Return true if the cell has the specified node.
  bool
  hasNode(NodeConstIterator node) const {
    return _nodes.hasVertex(node);
  }

  //! Return true if the cell has the specified node.
  /*!
    If true, compute the index of the node.
  */
  bool
  hasNode(NodeConstIterator node, int* m) const {
    return _nodes.hasVertex(node, m);
  }

  //! Return true if this cell has a boundary face that is incident to the specified node.
  bool
  hasIncidentBoundaryFace(NodeConstIterator node) const {
#ifdef DEBUG_SmrCell
    assert(hasNode(node));
#endif
    return hasIncidentBoundaryFace(getIndex(node));
  }

  //! Return true if this cell has a boundary face that is incident to the specified node.
  bool
  hasIncidentBoundaryFace(const int n) const {
#ifdef DEBUG_SmrCell
    assert(0 <= n && n < M + 1);
#endif    
    // For each face incident to the node.
    for (int m = 0; m != M+1; ++m) {
      // Exclude the face opposite this node.
      if (m != n) {
	// If the face is on the boundary.
	if (isFaceOnBoundary(m)) {
	  return true;
	}
      }
    }
    // We did not encounter any incident boundary faces.
    return false;
  }

  //! Calculate the centroid.
  void
  getCentroid(Vertex* centroid) const {
    // CONTINUE: This is the arithmetic mean.  Look up the formula for 
    // the centroid.
    *centroid = _nodes[0]->getVertex();
    for (int m = 1; m != M+1; ++m) {
      *centroid += _nodes[m]->getVertex();
    }
    *centroid /= (M + 1);
  }

  //! Return a const iterator to the n_th neighboring cell.
  CellConstIterator
  getNeighbor(const int m) const {
    return _neighbors[m];
  }

  //! Return true if the specified face is on the boundary.
  bool
  isFaceOnBoundary(const int m) const {
    return getNeighbor(m) == CellConstIterator(0);
  }

  //! Return the number of neighbors.
  int
  getNumberOfNeighbors() const {
    return M + 1 - int(std::count(_neighbors.begin(), _neighbors.end(), 
				  CellConstIterator(0)));
  }

  //! Return the index of the specified neighbor.
  int
  getIndex(CellConstIterator c) const {
    return _neighbors.getVertexIndex(c);
  }

  //! Return true if the cell has the specified neighbor.
  bool
  hasNeighbor(CellConstIterator c) const {
    return _neighbors.hasVertex(c);
  }

  //! Return true if the cell has the specified neighbor.
  /*!
    If true, compute the index of the neighbor.
  */
  bool
  hasNeighbor(CellConstIterator c, int* m) const {
    return _neighbors.hasVertex(c, m);
  }

  //! Return the index of this cell in the m_th neighbor.
  int
  getMirrorIndex(const int m) const {
    if (isFaceOnBoundary(m)) {
      return -1;
    }
    return _neighbors[m]->getIndex(_self);
  }

  //! Return the minimum edge length.
  /*!
    Set \c a and \c b to the vertex indices that define the minimum edge.
  */
  Number
  computeMinimumEdgeLength(int* a, int* b) const {
    Number x = std::numeric_limits<Number>::max();
    Number d;
    int j;
    // For each edge.
    for (int i = 0; i != M; ++i) {
      for (j = i+1; j != M + 1; ++j) {
	d = geom::computeDistance(_nodes[i]->getVertex(), 
				  _nodes[j]->getVertex());
	if (d < x) {
	  x = d;
	  *a = i;
	  *b = j;
	}
      }
    }
    return x;
  }
  
  //! Return the maximum edge length.
  /*!
    Set \c a and \c b to the vertex indices that define the maximum edge.
  */
  Number
  computeMaximumEdgeLength(int* a, int* b) const {
    Number x = 0;
    Number d;
    int j;
    // For each edge.
    for (int i = 0; i != M; ++i) {
      for (j = i+1; j != M + 1; ++j) {
	d = geom::computeDistance(_nodes[i]->getVertex(), 
				  _nodes[j]->getVertex());
	if (d > x) {
	  x = d;
	  *a = i;
	  *b = j;
	}
      }
    }
    return x;
  }
  
  //! Return the minimum edge length.
  Number
  computeMinimumEdgeLength() const {
    int a, b;
    return computeMinimumEdgeLength(&a, &b);
  }
  
  //! Return the maximum edge length.
  Number
  computeMaximumEdgeLength() const {
    int a, b;
    return computeMaximumEdgeLength(&a, &b);
  }

  //! Get the (vertex) simplex that comprises the cell.
  void
  getSimplex(Simplex<M,Vertex>* simplex) {
    for (int m = 0; m != M + 1; ++m) {
      (*simplex)[m] = getNode(m)->getVertex();
    }
  }
 
  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the identifier.
  /*!
    \note This member function is const because the identifier is mutable.
    It is intended to be modified as the mesh changes.
  */
  void
  setIdentifier(const int identifier) const { 
    _identifier = identifier;
  }

  //! Return an iterator to this cell.
  CellIterator
  getSelf() {
    return _self;
  }

  //! Set the self iterator for this cell.
  void
  setSelf(const CellIterator self) {
    _self = self;
  }

  //! Return an iterator to the m_th node.
  NodeIterator
  getNode(const int m) {
    return _nodes[m];
  }

  //! Set the m_th node.
  void
  setNode(const int m, const NodeIterator node) {
    _nodes[m] = node;
  }

  //! Return an iterator to the m_th neighboring cell.
  CellIterator
  getNeighbor(const int m) {
    return _neighbors[m];
  }

  //! Set the m_th neighbor.
  void
  setNeighbor(const int m, const CellIterator c) {
    _neighbors[m] = c;
  }

  //! Return the vertex of the m_th neighbor that is opposite this cell.
  NodeIterator
  getMirrorVertex(const int m) {
    if (_neighbors[m] == CellIterator(0)) {
      return NodeIterator(0);
    }
    return _neighbors[m]->getNode(getMirrorIndex(m));
  }

  //! Get the face opposite the m_th vertex.
  void
  getFace(const int m, Face* f) {
    _nodes.getFace(m, f);
  }

  //! Unlink this cell from the mesh.
  void
  unlink() {
    // For each vertex.
    for (typename NodeIteratorSimplex::Iterator i = _nodes.getBeginning();
	  i != _nodes.getEnd(); ++i) {
      // Remove the vertices' link to this cell.
      (*i)->removeCell(getSelf());
      // Remove the link to the vertex.
      *i = NodeIterator(0);
    }
    // For each neigbor.
    for (typename CellIteratorSimplex::Iterator i = _neighbors.getBeginning();
	  i != _neighbors.getEnd(); ++i) {
      // If there is a neighbor.
      if (*i != CellIterator(0)) {
	// Remove the neighbor's link to this cell.
	(*i)->removeNeighbor(getSelf());
	// Remove the link to the neighbor.
	*i = CellIterator(0);
      }
    }
  }

  //! Remove the link to the specified neighbor.  
  /*!
    The shared face becomes a boundary face.
  */
  void
  removeNeighbor(const CellIterator c) {
    // For each neigbor.
    typename CellIteratorSimplex::Iterator i = _neighbors.getBeginning();
    for (; i != _neighbors.getEnd(); ++i) {
      // If this is the specified neighbor.
      if (*i == c) {
	// Remove the link to the neighbor.
	*i = CellIterator(0);
	break;
      }
    }
    // The specified cell must be a neighbor.
    assert(i != _neighbors.getEnd());
  }

  //! Reverse the orientation of this cell.
  void
  negate() {
    _nodes.negate();
    _neighbors.negate();
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //! @{

  //! Write the identifier, vertex identifiers and neighbor simplex identifiers.
  void
  put(std::ostream& out) const {
    // Write the simplex identifier.
    out << "Id = "<< getIdentifier() << " Vertices = ";
    // Write the incident nodes.
    for (int m = 0; m != M+1; ++m) {
      assert(_nodes[m] != NodeIterator(0));
      out << _nodes[m]->getIdentifier() << " ";
    }
    out << " Neighbors = ";
    // Write the neighbors.
    for (int m = 0; m != M+1; ++m) {
      if (_neighbors[m] != CellIterator(0)) {
	out << _neighbors[m]->getIdentifier() << " ";
      }
      else {
	out << "-1 ";
      }
    }
    out << "\n";
  }

  //! @}
};


//! Return true if the cell has a boundary face incident to the edge.
/*!
  \pre SMR::M == 3.

  \relates SmrCell
*/
template<class SMR>
bool
doesCellHaveIncidentFaceOnBoundary(const typename SMR::CellConstIterator& c, 
				   int i, int j);


//! Return true if the edge is on the boundary.
/*!
  \pre SMR::M == 3.

  \c i and \c j are the indices of the edge in the cell.

  An edge is on the boundary iff an incident face is on the boundary.

  \relates SmrCell
*/
template<class SMR>
bool
isOnBoundary(const typename SMR::CellConstIterator& c, int i, int j);


//! Return true if the edge is on the boundary.
/*!
  \pre SMR::M == 3.

  \c i and \c j are the indices of the edge in the cell.

  An edge is on the boundary iff an incident face is on the boundary.

  \relates SmrCell
*/
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::Edge& edge) {
  return isOnBoundary<SMR>(edge.first, edge.second, edge.third);
}


//! Return true if the cell has the specified face.  Set the face index.
/*!
  \relates SmrCell
*/
template<class SMR>
inline
bool
hasFace(typename SMR::CellConstIterator cell, 
	typename SMR::Cell::Face& face,
	int* faceIndex) {
  return hasFace(cell->getNodes(), face, faceIndex);
}


//! Return true if the cell has the specified face.  Set the face index.
/*!
  \relates SmrCell
*/
template<class SMR>
inline
bool
hasFace(typename SMR::CellConstIterator cell, 
	typename SMR::Cell::Face& face) {
  int faceIndex;
  return hasFace<SMR>(cell, face, &faceIndex);
}


//! Return true if the cell has the specified face.  Set the face index.
/*!
  \pre SMR::M == 3.

  \relates SmrCell
*/
template<class SMR>
inline
bool
hasFace(typename SMR::CellConstIterator cell, 
	typename SMR::NodeConstIterator a,
	typename SMR::NodeConstIterator b,
	typename SMR::NodeConstIterator c,
	int* faceIndex) {
  return hasFace(cell->getNodes(), a, b, c, faceIndex);
}


//! Return true if the cell has the specified face.
/*!
  \pre SMR::M == 3.

  \relates SmrCell
*/
template<class SMR>
inline
bool
hasFace(typename SMR::CellConstIterator cell, 
	typename SMR::NodeConstIterator a,
	typename SMR::NodeConstIterator b,
	typename SMR::NodeConstIterator c) {
  int dummy;
  return hasFace<SMR>(cell, a, b, c, &dummy);
}


//! Return true if the cell has the specified face.  Set the face index.
/*!
  \pre SMR::M == 3.

  \relates SmrCell
*/
template<class SMR>
inline
bool
hasFace(typename SMR::CellIterator cell, 
	typename SMR::NodeIterator a,
	typename SMR::NodeIterator b,
	typename SMR::NodeIterator c,
	int* faceIndex) {
  return hasFace(cell->getNodes(), a, b, c, faceIndex);
}


//! Return true if the cell has the specified face.
/*!
  \pre SMR::M == 3.

  \relates SmrCell
*/
template<class SMR>
inline
bool
hasFace(typename SMR::CellIterator cell, 
	typename SMR::NodeIterator a,
	typename SMR::NodeIterator b,
	typename SMR::NodeIterator c) {
  int dummy;
  return hasFace<SMR>(cell, a, b, c, &dummy);
}


// Below are the even and odd permutations of the nodes.  We use even 
// permutations to get the next node and odd permutations to get the 
// previous node.
//
// Even Odd
// 0123 0132
// 0231 0213
// 0312 0321
// 1032 1032
// 1203 1230
// 1320 1302
// 2013 2031
// 2130 2103
// 2301 2310
// 3021 3012
// 3102 3120
// 3210 3201


// CONTINUE: Make this a singleton.  Static data may be a bad idea.
// Also, I should move this to Simplex.
//! Get the next node index.
inline
int
getNextNodeIndex(const int i, const int j) {
  const int Size = 12;
  static int even[Size][3] = 
    {{0,1,2},
     {0,2,3},
     {0,3,1},
     {1,0,3},
     {1,2,0},
     {1,3,2},
     {2,0,1},
     {2,1,3},
     {2,3,0},
     {3,0,2},
     {3,1,0},
     {3,2,1}};
  for (int n = 0; n != Size; ++n) {
    if (even[n][0] == i && even[n][1] == j) {
      return even[n][2];
    }
  }
  // Here we check that i and j have sensible values.
  assert(false);
  return -1;
}


// CONTINUE: Make this a singleton.  Static data may be a bad idea.
//! Get the previous node index.
inline
int
getPreviousNodeIndex(const int i, const int j) {
  const int Size = 12;
  static int odd[Size][3] = 
    {{0,1,3},
     {0,2,1},
     {0,3,2},
     {1,0,2},
     {1,2,3},
     {1,3,0},
     {2,0,3},
     {2,1,0},
     {2,3,1},
     {3,0,1},
     {3,1,2},
     {3,2,0}};
  for (int n = 0; n != Size; ++n) {
    if (odd[n][0] == i && odd[n][1] == j) {
      return odd[n][2];
    }
  }
  // Here we check that i and j have sensible values.
  assert(false);
  return -1;
}


//! Get the next node index.
template <typename SMR>
inline
int
getNextNodeIndex(const typename SMR::CellConstIterator cell, 
		 const typename SMR::NodeConstIterator a, 
		 const typename SMR::NodeConstIterator b) {
  assert(cell->hasNode(a) && cell->hasNode(b));
  return getNextNodeIndex(cell->getIndex(a), cell->getIndex(b));
}


//! Get the previous node index.
template <typename SMR>
inline
int
getPreviousNodeIndex(const typename SMR::CellConstIterator cell, 
		     const typename SMR::NodeConstIterator a, 
		     const typename SMR::NodeConstIterator b) {
  assert(cell->hasNode(a) && cell->hasNode(b));
  return getPreviousNodeIndex(cell->getIndex(a), cell->getIndex(b));
}


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_SmrCell_ipp__
#include "SmrCell.ipp"
#undef __geom_mesh_simplicial_SmrCell_ipp__

#endif
