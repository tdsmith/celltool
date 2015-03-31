// -*- C++ -*-

/*! 
  \file SimpMeshRed.h
  \brief Class for a tetrahedral mesh with topological optimization capabilities.
*/

#if !defined(__geom_mesh_simplicial_SimpMeshRed_h__)
#define __geom_mesh_simplicial_SimpMeshRed_h__

#include "SmrNode.h"
#include "SmrCell.h"
#include "FaceIterator.h"
#include "EdgeIterator.h"

#include "../iss/IndSimpSetIncAdj.h"
#include "../simplex/Simplex.h"

#include "../../../ads/functor/composite_compare.h"
#include "../../../ads/iterator/MemFunIterator.h"
#include "../../../ads/algorithm/Triplet.h"

#include <list>
#include <set>
#include <map>

#include <cassert>

BEGIN_NAMESPACE_GEOM

//! A simplicial mesh data structure.
/*!
  \param _N is the space dimension.
  \param _M is the simplex dimension  By default it is _N.
  \param T is the number type.  By default it is double.
  \param _Node is the node type.
  \param _Cell is the cell (simplex) type.
  \param Container is the container for storing the vertices and cells.
*/
template<int _N, 
	 int _M = _N,
	 typename T = double,
	 template<class> class _Node = SmrNode,
	 template<class> class _Cell = SmrCell,
	 // CONTINUE
	 // Had to add the allocator class to appease MSVC++.
	 // See notes below.
	 template<class _Elem,
		  class = std::allocator<_Elem> > class Container =
	 std::list>
class SimpMeshRed {
  //
  // Enumerations.
  //

public:

  //! The space dimension and simplex dimension.
  enum {N = _N, M = _M};
  
  //
  // Types.
  //

private:

  // CONTINUE
  // I should be able to do this more cleanly.  See page 112 of
  // "C++ Templates, The Complete Guide".  I have to add the allocator to
  // get MSVC++ to compile it.
  typedef Container<_Node<SimpMeshRed>, 
		    std::allocator<_Node<SimpMeshRed> > > NodeContainer;
  typedef Container<_Cell<SimpMeshRed>,
		    std::allocator<_Cell<SimpMeshRed> > > CellContainer;

public:

  //
  // Nodes.
  //

  //! A node.
  typedef typename NodeContainer::value_type Node;
  //! Node iterator.
  typedef typename NodeContainer::iterator NodeIterator;
  //! Vertex const iterator.
  typedef typename NodeContainer::const_iterator NodeConstIterator;

  //
  // Cells.
  //

  //! A cell (simplex).
  typedef typename CellContainer::value_type Cell;
  //! Cell iterator.
  typedef typename CellContainer::iterator CellIterator;
  //! Cell const iterator.
  typedef typename CellContainer::const_iterator CellConstIterator;

  //
  // Faces.
  //
  
  //! A const face of a cell is determined by a cell and a node index.
  typedef std::pair<CellConstIterator,int> ConstFace;
  //! A bidirectional, constant iterator on the faces.
  typedef FaceIterator<M,ConstFace,CellConstIterator> 
  FaceConstIterator;
  //! A face of a cell is determined by a cell and a node index.
  typedef std::pair<CellIterator,int> Face;
  //! A bidirectional, iterator on the faces.
  typedef FaceIterator<M,Face,CellIterator> FaceIterator;

  //
  // Edges.
  //

  //! A const edge of a cell is determined by a cell and two node indices.
  typedef ads::Triplet<CellConstIterator,int,int> ConstEdge;
  //! A bidirectional, constant iterator on the edges.
  typedef EdgeIterator<SimpMeshRed,true> EdgeConstIterator;
  //! An edge of a cell is determined by a cell and two node indices.
  typedef ads::Triplet<CellIterator,int,int> Edge;
  //! A bidirectional, iterator on the edges.
  typedef EdgeIterator<SimpMeshRed,false> EdgeIterator;

  //
  // Miscellaneous.
  //

  //! The number type.
  typedef T Number;
  //! A node (a Cartesian point).
  typedef typename Node::Vertex Vertex;
  //! A bounding box.
  typedef BBox<N,Number> BBox;

  //! The size type.
  typedef int SizeType;
  //! The pointer difference type.
  typedef typename NodeContainer::difference_type DifferenceType;

  //
  // Simplex
  //

  //! A simplex of indices.
  typedef Simplex<M,int> IndexedSimplex;
  //! A simplex of vertices.
  typedef Simplex<M,Vertex> Simplex;

  //
  // Node member function iterators.
  //

  //! Vertex point iterator.
  typedef ads::MemFunIterator<NodeConstIterator,Node, const Vertex&,true>
  VertexIterator;
  //! Node identifier iterator.
  typedef ads::MemFunIterator<NodeConstIterator,Node,int,true>
  NodeIdentifierIterator;
  //! Cell identifier iterator.
  typedef ads::MemFunIterator<CellConstIterator,Cell,int,true>
  CellIdentifierIterator;

  //
  // Indexed simplex iterator.
  //

#define __geom_mesh_simplicial_SMR_IndSimpIter_ipp__
#include "SMR_IndSimpIter.ipp"
#undef __geom_mesh_simplicial_SMR_IndSimpIter_ipp__

  //! A const iterator over indexed simplices.
  typedef IndSimpIter IndexedSimplexIterator;

  //
  // Simplex iterator.
  //

#define __geom_mesh_simplicial_SMR_SimpIter_ipp__
#include "SMR_SimpIter.ipp"
#undef __geom_mesh_simplicial_SMR_SimpIter_ipp__

  //! A const iterator over simplices.
  typedef SimpIter SimplexIterator;

  //! Functor for comparing node iterators by their identifiers.
  struct NodeIteratorCompare : 
    public std::binary_function<NodeIterator, NodeIterator, bool> {
    //! Compare node iterators by their identifiers.
    bool
    operator()(const NodeIterator& x, const NodeIterator& y) const {
      return x->getIdentifier() < y->getIdentifier();
    }
  };

  //! Functor for comparing cell iterators by their identifiers.
  struct CellIteratorCompare : 
    public std::binary_function<CellIterator, CellIterator, bool> {
    //! Compare cell iterators by their identifiers.
    bool
    operator()(const CellIterator& x, const CellIterator& y) const {
      return x->getIdentifier() < y->getIdentifier();
    }
  };

  //! Functor for comparing faces.
  struct FaceCompare : 
    public std::binary_function<Face, Face, bool> {
    //! Compare the faces.
    bool
    operator()(const Face& x, const Face& y) const {
      return x.first->getIdentifier() < y.first->getIdentifier() ||
	(x.first->getIdentifier() == y.first->getIdentifier() &&
	  x.second < y.second);
    }
  };

  //! Functor for comparing face iterators.
  struct FaceIteratorCompare : 
    public std::binary_function<FaceIterator, FaceIterator, bool> {
    //! Compare the face iterators.
    bool
    operator()(const FaceIterator& x, const FaceIterator& y) const {
      return x->first->getIdentifier() < y->first->getIdentifier() ||
	(x->first->getIdentifier() == y->first->getIdentifier() &&
	  x->second < y->second);
    }
  };

  //! A set of node iterators.
  typedef std::set<NodeIterator, NodeIteratorCompare> NodeIteratorSet;
  //! A set of cell iterators.
  typedef std::set<CellIterator, CellIteratorCompare> CellIteratorSet;
  //! A set of faces.
  typedef std::set<Face, FaceCompare> FaceSet;
  //! A set of face iterators.
  typedef std::set<FaceIterator, FaceIteratorCompare> FaceIteratorSet;


  //
  // Data
  //
    
private:
      
  //! The nodes.
  NodeContainer _nodes;
  //! The cells.
  CellContainer _cells;
     
public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Empty containers.
  SimpMeshRed() :
    _nodes(),
    _cells()
  {}

  //! Copy constructor.
  SimpMeshRed(const SimpMeshRed& other) :
    _nodes(other._nodes),
    _cells(other._cells)
  {}

  //! Construct from an indexed simplex set.
  template< bool A, typename V, typename IS >
  SimpMeshRed(const IndSimpSet<N,M,A,Number,V,IS>& iss) :
    _nodes(),
    _cells() {
    build(iss);
  }

  //! Assignment operator.
  SimpMeshRed& 
  operator=(const SimpMeshRed& other) {
    if (&other != this) {
      _nodes = other._nodes;
      _cells = other._cells;
    }
    return *this;
  }

  //! Build from an indexed simplex set.
  /*!
    The value type for the vertices must be \c ads::FixedArray<N,T>.
    The value type for the simplices must be subscriptable.
  */
  template<typename VertInIter, typename SimpInIter>
  void
  build(VertInIter verticesBeginning, VertInIter verticesEnd, 
	SimpInIter simplicesBeginning, SimpInIter simplicesEnd);
  
  //! Build from an indexed simplex set.
  template< bool A, typename V, typename IS >
  void
  build(const IndSimpSet<N,M,A,Number,V,IS>& iss) {
    build(iss.getVerticesBeginning(), iss.getVerticesEnd(),
	  iss.getIndexedSimplicesBeginning(), iss.getIndexedSimplicesEnd());
  }

  //! Swap.
  void
  swap(SimpMeshRed& x) {
    _nodes.swap(x._nodes);
    _cells.swap(x._cells);
  }

  //! Clear the mesh.
  void
  clear() {
    _nodes.clear();
    _cells.clear();
  }

  //! Destructor.
  ~SimpMeshRed()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Dimension accessors.
  //! @{

  //! Return the space dimension.
  int
  getSpaceDimension() const {
    return N;
  }

  //! Return the simplex dimension.
  int
  getSimplexDimension() const {
    return M;
  }
  
  //! @}
  //--------------------------------------------------------------------------
  //! \name Node accessors.
  //! @{

  //! Return true if there are no nodes.
  bool
  areNodesEmpty() const {
    return _nodes.empty();
  }

  //! Return the number of nodes.
  /*!
    \note This is a slow function.  It counts the nodes.
  */
  SizeType
  computeNodesSize() const {
    return SizeType(_nodes.size());
  }

  //! Return the beginning of the nodes.
  NodeConstIterator
  getNodesBeginning() const {
    return _nodes.begin();
  }

  //! Return the end of the nodes.
  NodeConstIterator
  getNodesEnd() const {
    return _nodes.end();
  }

  //! Return the beginning of the node vertices.
  VertexIterator
  getVerticesBeginning() const {
    return VertexIterator(&Node::getVertex, _nodes.begin());
  }

  //! Return the end of the node vertices.
  VertexIterator
  getVerticesEnd() const {
    return VertexIterator(&Node::getVertex, _nodes.end());
  }

  //! Return the beginning of the node identifiers.
  NodeIdentifierIterator
  getNodeIdentifiersBeginning() const {
    return NodeIdentifierIterator(&Node::getIdentifier, _nodes.begin());
  }

  //! Return the end of the vertex identifiers.
  NodeIdentifierIterator
  getNodeIdentifiersEnd() const {
    return NodeIdentifierIterator(&Node::getIdentifier, _nodes.end());
  }

  //! Return the maximum node identifier.
  /*!
    CONTINUE: when I implement a scheme for managing identifiers I won't need
    this.
  */
  int
  computeMaximumNodeIdentifier() const {
    if (areNodesEmpty()) {
      // Return -1 because one more than the "maximum" is then 0.
      return -1;
    }
    return *std::max_element(getNodeIdentifiersBeginning(),
			     getNodeIdentifiersEnd());
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Cell accessors.
  //! @{

  //! Return true if there are no cells.
  bool
  areCellsEmpty() const {
    return _cells.empty();
  }

  //! Return the number of cells.
  /*!
    \note This is a slow function.  It counts the cells.
  */
  SizeType
  computeCellsSize() const {
    return SizeType(_cells.size());
  }

  //! Return the beginning of the cells.
  CellConstIterator
  getCellsBeginning() const {
    return _cells.begin();
  }

  //! Return the end of the cells.
  CellConstIterator
  getCellsEnd() const {
    return _cells.end();
  }

  //! Get the simplex given a const iterator to the cell.
  void
  getSimplex(CellConstIterator i, Simplex* s) const {
    for (int m = 0; m != M+1; ++m) {
      (*s)[m] = i->getNode(m)->getVertex();
    }
  }

  //! Return the beginning of the cell identifiers.
  CellIdentifierIterator
  getCellIdentifiersBeginning() const {
    return CellIdentifierIterator(&Cell::getIdentifier, _cells.begin());
  }

  //! Return the end of the cell identifiers.
  CellIdentifierIterator
  getCellIdentifiersEnd() const {
    return CellIdentifierIterator(&Cell::getIdentifier, _cells.end());
  }

  //! Return the maximum cell identifier.
  /*!
    CONTINUE: when I implement a scheme for managing identifiers I won't need
    this.
  */
  int
  computeMaximumCellIdentifier() const {
    if (areCellsEmpty()) {
      // Return -1 because one more than the "maximum" is then 0.
      return -1;
    }
    return *std::max_element(getCellIdentifiersBeginning(),
			     getCellIdentifiersEnd());
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Simplex accessors.
  //! @{

  //! Return the beginning of the indexed simplices
  IndexedSimplexIterator
  getIndexedSimplicesBeginning() const {
    return IndexedSimplexIterator(getCellsBeginning());
  }

  //! Return the end of the indexed simplices
  IndexedSimplexIterator
  getIndexedSimplicesEnd() const {
    return IndexedSimplexIterator(getCellsEnd());
  }

  //! Return the beginning of the simplices
  SimplexIterator
  getSimplicesBeginning() const {
    return SimplexIterator(getCellsBeginning());
  }

  //! Return the end of the simplices
  SimplexIterator
  getSimplicesEnd() const {
    return SimplexIterator(getCellsEnd());
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
    return SizeType(std::distance(getFacesBeginning(), getFacesEnd()));
  }

  //! Return the beginning of the faces.
  FaceConstIterator
  getFacesBeginning() const {
    FaceConstIterator x(getCellsBeginning(), getCellsEnd());
    return x;
  }

  //! Return the end of the faces.
  FaceConstIterator
  getFacesEnd() const {
    return FaceConstIterator(getCellsEnd(), getCellsEnd());
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Edge accessors.
  //! @{

  //! Return the number of edges.
  /*!
    \note This is a slow function.  It counts the edges.
  */
  SizeType
  computeEdgesSize() const {
    return std::distance(getEdgesBeginning(), getEdgesEnd());
  }

  //! Return the beginning of the edges.
  EdgeConstIterator
  getEdgesBeginning() const {
    EdgeConstIterator x(getCellsBeginning(), getCellsEnd());
    return x;
  }

  //! Return the end of the edges.
  EdgeConstIterator
  getEdgesEnd() const {
    return EdgeConstIterator(getCellsEnd(), getCellsEnd());
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Node manipulators.
  //! @{

  //! Return the beginning of the nodes.
  NodeIterator
  getNodesBeginning() {
    return _nodes.begin();
  }

  //! Return the end of the nodes.
  NodeIterator
  getNodesEnd() {
    return _nodes.end();
  }

  //! Set the node identifiers.
  /*!
    \note This is a const member function because the node identifier is
    mutable.
  */
  void
  setNodeIdentifiers() const;

  //! Set the locations of the vertices.
  template<typename VertexInIter>
  void
  setVertices(VertexInIter begin, VertexInIter end) {
    for (NodeIterator i = getNodesBeginning(); i != getNodesEnd(); 
	 ++i, ++begin) {
#ifdef DEBUG_SimpMeshRed
      assert(begin != end);
#endif
      i->setVertex(*begin);
    }
    assert(begin == end);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Cell manipulators.
  //! @{

  //! Return the beginning of the cells.
  CellIterator
  getCellsBeginning() {
    return _cells.begin();
  }

  //! Return the end of the cells.
  CellIterator
  getCellsEnd() {
    return _cells.end();
  }
  
  //! Set the cell identifiers.
  /*!
    \note This is a const member function because the cell identifier is
    mutable.
  */
  void
  setCellIdentifiers() const;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Face manipulators.
  //! @{

  //! Return the beginning of the faces.
  FaceIterator
  getFacesBeginning() {
    return FaceIterator(getCellsBeginning(), getCellsEnd());
  }

  //! Return the end of the faces.
  FaceIterator
  getFacesEnd() {
    return FaceIterator(getCellsEnd(), getCellsEnd());
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Edge manipulators.
  //! @{

  //! Return the beginning of the edges.
  EdgeIterator
  getEdgesBeginning() {
    return EdgeIterator(getCellsBeginning(), getCellsEnd());
  }

  //! Return the end of the edges.
  EdgeIterator
  getEdgesEnd() {
    return EdgeIterator(getCellsEnd(), getCellsEnd());
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Insert/erase nodes.
  //! @{

  //! Insert the node into the mesh.
  /*!
    Set the self iterator and the identifier.
  */
  NodeIterator
  insertNode(const Node& node = Node());

  //! Insert a copy of the node into the mesh.
  NodeIterator
  insertNode(const NodeIterator node) {
    return insert_vertex(*node);
  }

  //! Erase a vertex.
  /*!
    No cell should be incident to this vertex.
  */
  void
  eraseNode(const NodeIterator node) {
    _nodes.erase(node);
  }

  //! Merge two nodes.  Erase the second.
  /*!
    The two nodes should not have any incident cells in common.
  */
  void
  merge(NodeIterator x, NodeIterator y) {
#if 0
    // CONTINUE
    std::cerr << "Merge " << x->getIdentifier() << " "
	      << y->getIdentifier() << "\n";
#endif
    assert(x != y);
    // Merge the vertex-simplex incidences.
    x->insertCells(y->getCellIteratorsBeginning(), y->getCellIteratorsEnd());
    // Fix the simplex-vertex incidences.
    y->replace(x);
    // Erase the second vertex.
    eraseNode(y);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Insert/erase cells.
  //! @{

  //! Insert the cell into the mesh and set the self iterator and identifier.
  CellIterator
  insertCell(const Cell& c = Cell());

  //! Insert a copy of the cell into the mesh.
  CellIterator
  insertCell(const CellIterator c) {
    return insertCell(*c);
  }

  //! Erase a cell.
  /*!
    Unlink the cell and erase it from the mesh.
  */
  void
  eraseCell(const CellIterator c) {
    c->unlink();
    _cells.erase(c);
  }

  //! Erase a range of cells.
  /*!
    Unlink the cells and erase them from the mesh.
    
    \c InIter is an input iterator for cell iterators.
  */
  template<typename InIter>
  void
  eraseCells(InIter begin, InIter end) {
    for (; begin != end; ++begin) {
      eraseCell(*begin);
    }
  }

  //! @}

private:

  /* CONTINUE REMOVE
  template<class Map>
  void
  map_NodeIterators_to_indices(Map& x) const;
  */

  void
  buildCellAdjacencies();
  
};

END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_SimpMeshRed_ipp__
#include "SimpMeshRed.ipp"
#undef __geom_mesh_simplicial_SimpMeshRed_ipp__

#endif
