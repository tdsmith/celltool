// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_EdgeIterator_h__)
#define __geom_mesh_simplicial_EdgeIterator_h__

#if defined(DEBUG_geom) && !defined(DEBUG_EdgeIterator)
#define DEBUG_EdgeIterator
#endif


BEGIN_NAMESPACE_GEOM

//! An iterator on edges in a simplicial mesh.
template<class _Mesh, bool Const = true>
class 
EdgeIterator :
  public std::iterator< 
  // Iterator tag.
  std::bidirectional_iterator_tag,
  // Value type.
  typename 
  Loki::Select<Const, typename _Mesh::ConstEdge,
	       typename _Mesh::Edge>::Result,
  // Pointer difference type.
  std::ptrdiff_t, 
  // Pointer type.
  typename 
  Loki::Select<Const, const typename _Mesh::ConstEdge*,
	       typename _Mesh::Edge*>::Result,
  // Reference type.
  typename 
  Loki::Select<Const, const typename _Mesh::ConstEdge&,
	       typename _Mesh::Edge&>::Result> {
  //
  // Enumerations.
  //

private:

  //! The simplex dimension.
  enum {M = _Mesh::M};

  //
  // Private types.
  //

private:

  //! The base type.
  typedef std::iterator< 
    // Iterator tag.
    std::bidirectional_iterator_tag,
    // Value type.
    typename 
    Loki::Select<Const, typename _Mesh::ConstEdge,
		 typename _Mesh::Edge>::Result,
    // Pointer difference type.
    std::ptrdiff_t, 
    // Pointer type.
    typename 
    Loki::Select<Const, const typename _Mesh::ConstEdge*,
		 typename _Mesh::Edge*>::Result,
    // Reference type.
    typename 
    Loki::Select<Const, const typename _Mesh::ConstEdge&,
		 typename _Mesh::Edge&>::Result> 
  Base;

  //! The simplicial mesh type.
  typedef _Mesh Mesh;
  //! The node type.
  typedef typename Mesh::Node Node;
  //! Node iterator.
  typedef typename Loki::Select<Const,
				typename Mesh::NodeConstIterator,
				typename Mesh::NodeIterator>::Result
  NodeIterator;

  //! Iterator on the cells incident to a node.
  typedef 
  typename Loki::Select<Const,
			typename Node::CellIncidentToNodeConstIterator,
			typename Node::CellIncidentToNodeIterator>::Result
  CellIncidentToNodeIterator;

  //
  // Public types.
  //

public:

  //! Iterator category.
  typedef typename Base::iterator_category iterator_category;
  //! Value type.
  typedef typename Base::value_type value_type;
  //! Pointer difference type.
  typedef typename Base::difference_type difference_type;
  //! Pointer to the value type.
  typedef typename Base::pointer pointer;
  //! Reference to the value type.
  typedef typename Base::reference reference;

  //! An edge in the mesh.
  typedef value_type Edge;
  //! Cell iterator.
  typedef typename Loki::Select<Const,
				typename Mesh::CellConstIterator,
				typename Mesh::CellIterator>::Result
  CellIterator;

  //
  // Member data.
  //

private:
  
  //! The edge.
  Edge _edge;
  //! The end of the list of cells.
  CellIterator _cellsEnd;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  EdgeIterator();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors for the mesh to call.
  //@{

  //! Construct a edge const iterator for the mesh.
  EdgeIterator(const CellIterator c, const CellIterator cellsEnd) :
    Base(),
    _edge(c, 0, 1),
    _cellsEnd(cellsEnd) {
    if (c != cellsEnd && ! isValid()) {
      increment();
    }
  }

  //@}

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Copy constructor.
  EdgeIterator(const EdgeIterator& other) :
    Base(),
    _edge(other._edge),
    _cellsEnd(other._cellsEnd)
  {}    

  //! Assignment operator.
  EdgeIterator&
  operator=(const EdgeIterator& other) {
    if (&other != this) {
      _edge = other._edge;
      _cellsEnd = other._cellsEnd;
    }
    return *this;
  }

  //! Destructor.
  ~EdgeIterator()
  {}

  //! The edge.
  /*!
    This is needed for the conversion constructor.
  */
  const Edge&
  getEdge() const {
    return _edge;
  }

  //! The end of the list of cells.
  /*!
    This is needed for the conversion constructor.
  */
  const CellIterator&
  getCellsEnd() const {
    return _cellsEnd;
  }

  //! Conversion from iterator to const iterator
  template<bool _Const>
  EdgeIterator(const EdgeIterator<Mesh,_Const>& other) :
    Base(),
    _edge(other.getEdge()),
    _cellsEnd(other.getCellsEnd())
  {}    

  //@}
  //--------------------------------------------------------------------------
  //! \name Forward iterator requirements
  //@{

  //! Dereference.
  reference
  operator*() const {
    // Return a constant reference to the edge.
    return _edge;
  }

  //! Return a const pointer to the edge.
  pointer
  operator->() const {
    // Return a constant pointer to the edge.
    return &_edge;
  }
  
  //! Pre-increment.
  EdgeIterator&
  operator++() { 
    increment();
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  EdgeIterator
  operator++(int) {
    EdgeIterator x(*this); 
    ++*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  EdgeIterator&
  operator--() { 
    decrement();
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  EdgeIterator
  operator--(int) {
    EdgeIterator x(*this); 
    --*this;
    return x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
      
  //
  // Forward iterator requirements
  //

  //! Return true if x is an iterator to the same edge.
  bool
  operator==(const EdgeIterator& x) {
#ifdef DEBUG_EdgeIterator
    // These must be iterators over the same mesh.
    assert(_cellsEnd == x._cellsEnd);
#endif
    return _edge == x._edge;
  }

  //! Return true if x is not an iterator to the same edge.
  bool
  operator!=(const EdgeIterator& x) {
    return ! operator==(x);
  }

  //@}


private:

  // If the address of this cell is less than the addresses of the other
  // cells incident to the edge.
  bool
  isValid() {
    if (_edge.first != _cellsEnd) {
      // The source node of the edge.
      NodeIterator a = _edge.first->getNode(_edge.second);
      // The target node of the edge.
      NodeIterator b = _edge.first->getNode(_edge.third);
      // For each incident cell of the source node.
      for (CellIncidentToNodeIterator c = a->getCellsBeginning(); 
	   c != a->getCellsEnd(); ++c) {
	// If this cell is incident to the edge and its identifier is less 
	// than this cell's identifier.
	if (c->hasNode(b) && 
	    c->getIdentifier() < _edge.first->getIdentifier()) {
	  return false;
	}
      }
    }
    return true;
  }

  // Increment the iterator.
  void
  increment() {
    // While we have not gone through all of the cells.
    while (_edge.first != _cellsEnd) {
      // Go to the next edge.
      ++_edge.third;
      if (_edge.third == M + 1) {
	++_edge.second;
	_edge.third = _edge.second + 1;
      }
      if (_edge.second == M) {
	++_edge.first;
	_edge.second = 0;
	_edge.third = 1;
      }
      // If this edge is valid.
      if (isValid()) {
	// Then we have a edge.  Break out of the loop and return.
	break;
      }
    }
  }

  // Decrement the iterator.
  void
  decrement() {
    // While we have not gone through all of the cells.
    do {
      // Go to the previous edge.
      --_edge.third;
      if (_edge.third == _edge.second) {
	--_edge.second;
	_edge.third = M;
      }
      if (_edge.second == -1) {
	--_edge.first;
	_edge.second = M-1;
	_edge.third = M;
      }
      // If this edge is valid.
      if (isValid()) {
	// Then we have a edge.  Break out of the loop and return.
	break;
      }
    } while (_edge.first != _cellsEnd);
  }

};

END_NAMESPACE_GEOM

#endif
