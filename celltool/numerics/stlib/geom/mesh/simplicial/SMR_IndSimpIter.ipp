// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_SMR_IndSimpIter_ipp__)
#error This file is an implementation detail of SimpMeshRed::IndSimpIter.
#endif

//! An indexed simplex iterator for a SimpMeshRed.
/*!
  This is a const iterator.
*/
class IndSimpIter :
  public std::iterator< 
  typename std::iterator_traits<CellConstIterator>::iterator_category,
  const IndexedSimplex,
  typename std::iterator_traits<CellConstIterator>::difference_type,
  const IndexedSimplex*,
  const IndexedSimplex&> {
  //
  // Private types.
  //

private:
  
  typedef 
  std::iterator< 
    typename std::iterator_traits<CellConstIterator>::iterator_category,
    const IndexedSimplex,
    typename std::iterator_traits<CellConstIterator>::difference_type,
    const IndexedSimplex*,
    const IndexedSimplex&>
  Base;

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

  //
  // Member data.
  //

private:

  CellConstIterator _cell;
  mutable IndexedSimplex _indexedSimplex;

  //
  // Not implemented.
  //

private:

  // Default constructor not implemented.
  IndSimpIter();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Construct an indexed simplex iterator from a const cell iterator.
  IndSimpIter(CellConstIterator cell) :
    Base(),
    _cell(cell),
    _indexedSimplex()
  {}

  //! Copy constructor.
  IndSimpIter(const IndSimpIter& other) :
    Base(),
    _cell(other._cell),
    _indexedSimplex()
  {}    

  //! Assignment operator.
  IndSimpIter&
  operator=(const IndSimpIter& other) {
    if (&other != this) {
      _cell = other._cell;
    }
    return *this;
  }

  //! Destructor.
  ~IndSimpIter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Forward iterator requirements
  //@{

  //! Dereference.
  reference
  operator*() const {
    // Update the indexed simplex.
    update();
    // Then return a constant reference to it.
    return _indexedSimplex;
  }

  //! Pointer dereference.
  pointer
  operator->() const {
    // Update the indexed simplex.
    update();
    // Then return a constant pointer to it.
    return &_indexedSimplex;
  }

  //! Pre-increment.
  IndSimpIter&
  operator++() { 
    ++_cell; 
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  IndSimpIter
  operator++(int) {
    IndSimpIter x(*this); 
    ++*this;
    return x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  IndSimpIter&
  operator--() { 
    --_cell; 
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  IndSimpIter
  operator--(int) {
    IndSimpIter x(*this); 
    --*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Base.
  //@{

  //! Return the const cell iterator.
  CellConstIterator
  base() const {
    return _cell;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{

  //! Return true if the cells are the same.
  bool
  operator==(const IndSimpIter& x) { 
    return base() == x.base(); 
  }

  //! Return true if the cells are not the same.
  bool
  operator!=(const IndSimpIter& x) { 
    return !(*this == x);
  }

  //@}

private:

  //
  // Private member functions
  //

  // Update the indexed simplex for the current cell.
  void
  update() const {
    for (int m = 0; m != M + 1; ++m) {
      _indexedSimplex[m] = _cell->getNode(m)->getIdentifier();
    }
  }
};

// End of file.
