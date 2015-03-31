// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_SMR_SimpIter_ipp__)
#error This file is an implementation detail of SimpMeshRed::SimpIter.
#endif

//! A simplex iterator for a SimpMeshRed.
/*!
  This is a const iterator.
*/
class SimpIter :
  public std::iterator< 
  typename std::iterator_traits<CellConstIterator>::iterator_category,
  const Simplex,
  typename std::iterator_traits<CellConstIterator>::difference_type,
  const Simplex*,
  const Simplex&> {
  //
  // Private types.
  //

private:
  
  typedef 
  std::iterator< 
    typename std::iterator_traits<CellConstIterator>::iterator_category,
    const Simplex,
    typename std::iterator_traits<CellConstIterator>::difference_type,
    const Simplex*,
    const Simplex&>
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
  mutable Simplex _simplex;

  //
  // Not implemented.
  //

private:

  // Default constructor not implemented.
  SimpIter();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Construct a simplex iterator from a const cell iterator.
  SimpIter(CellConstIterator cell) :
    Base(),
    _cell(cell),
    _simplex()
  {}

  //! Copy constructor.
  SimpIter(const SimpIter& other) :
    Base(),
    _cell(other._cell),
    _simplex()
  {}    

  //! Assignment operator.
  SimpIter&
  operator=(const SimpIter& other) {
    if (&other != this) {
      _cell = other._cell;
    }
    return *this;
  }

  //! Destructor.
  ~SimpIter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Forward iterator requirements
  //@{

  //! Dereference.
  reference
  operator*() const {
    // Update the simplex.
    update();
    // Then return a constant reference to it.
    return _simplex;
  }

  //! Pointer dereference.
  pointer
  operator->() const {
    // Update the simplex.
    update();
    // Then return a constant pointer to it.
    return &_simplex;
  }

  //! Pre-increment.
  SimpIter&
  operator++() { 
    ++_cell; 
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  SimpIter
  operator++(int) {
    SimpIter x(*this); 
    ++*this;
    return x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  SimpIter&
  operator--() { 
    --_cell; 
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  SimpIter
  operator--(int) {
    SimpIter x(*this); 
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
  operator==(const SimpIter& x) { 
    return base() == x.base(); 
  }

  //! Return true if the cells are not the same.
  bool
  operator!=(const SimpIter& x) { 
    return !(*this == x);
  }

  //@}

private:

  //
  // Private member functions
  //

  // Update the simplex for the current cell.
  void
  update() const {
    for (int m = 0; m != M + 1; ++m) {
      _simplex[m] = _cell->getNode(m)->getVertex();
    }
  }
};

// End of file.
