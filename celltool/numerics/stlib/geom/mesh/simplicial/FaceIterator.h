// -*- C++ -*-

/*! 
  \file FaceIterator.h
  \brief A face iterator in SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_FaceIterator_h__)
#define __geom_mesh_simplicial_FaceIterator_h__

#if defined(DEBUG_geom) && !defined(DEBUG_FaceIterator)
#define DEBUG_FaceIterator
#endif

BEGIN_NAMESPACE_GEOM

//! An iterator of faces in a simplicial mesh.
template<int M, typename _Face, typename _CellHandle>
class 
FaceIterator  :
  public std::iterator<std::bidirectional_iterator_tag, // Iterator tag.
		       _Face, // Value type.
		       std::ptrdiff_t, // Pointer difference type.
		       const _Face*, // Pointer type.
		       const _Face&> { // Reference type. 
  //
  // Private types.
  //

private:

  //! The base type.
  typedef std::iterator<std::bidirectional_iterator_tag,
			_Face,
			std::ptrdiff_t,
			const _Face*,
			const _Face&> Base;
  
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

  //! A face in the mesh.
  typedef _Face Face;
  //! A handle to a cell.
  typedef _CellHandle CellHandle;

  //
  // Member data.
  //

private:
  
  //! The face.
  Face _face;
  //! The end of the list of cells.
  CellHandle _cellsEnd;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  FaceIterator();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors for the mesh to call.
  //@{

  //! Construct a face const iterator for the mesh.
  FaceIterator(const CellHandle c, const CellHandle cellsEnd) :
    Base(),
    _face(c, 0),
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
  FaceIterator(const FaceIterator& other) :
    Base(),
    _face(other._face),
    _cellsEnd(other._cellsEnd)
  {}    

  //! Assignment operator.
  FaceIterator&
  operator=(const FaceIterator& other) {
    if (&other != this) {
      _face = other._face;
      _cellsEnd = other._cellsEnd;
    }
    return *this;
  }

  //! Destructor.
  ~FaceIterator()
  {}

  //! The face.
  /*!
    This is needed for the conversion constructor.
  */
  const Face&
  getFace() const {
    return _face;
  }

  //! The end of the list of cells.
  /*!
    This is needed for the conversion constructor.
  */
  const CellHandle&
  getCellsEnd() const {
    return _cellsEnd;
  }

  //! Conversion from iterator to const iterator
  template<typename AnyFace, typename AnyCellHandle>
  FaceIterator(const FaceIterator<M, AnyFace, AnyCellHandle>& other) :
    Base(),
    _face(other.getFace()),
    _cellsEnd(other.getCellsEnd())
  {}    

  //@}
  //--------------------------------------------------------------------------
  //! \name Forward iterator requirements
  //@{

  //! Dereference.
  reference
  operator*() const {
    // Return a constant reference to the face.
    return _face;
  }

  //! Return a const pointer to the face.
  pointer
  operator->() const {
    // Return a constant pointer to the face.
    return &_face;
  }
  
  //! Pre-increment.
  FaceIterator&
  operator++() { 
    increment();
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  FaceIterator
  operator++(int) {
    FaceIterator x(*this); 
    ++*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  FaceIterator&
  operator--() { 
    decrement();
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  FaceIterator
  operator--(int) {
    FaceIterator x(*this); 
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

  //! Return true if x is an iterator to the same face.
  bool
  operator==(const FaceIterator& x) {
#ifdef DEBUG_FaceIterator
    // These must be iterators over the same mesh.
    assert(_cellsEnd == x._cellsEnd);
#endif
    return _face == x._face;
  }

  //! Return true if x is not an iterator to the same face.
  bool
  operator!=(const FaceIterator& x) {
    return ! operator==(x);
  }

  //@}


private:

  // If there is no adjacent cell through the face or
  // if the identifier of this cell is less than the identifier of the 
  // adjacent cell through the face.
  bool
  isValid() {
    return (_face.first->isFaceOnBoundary(_face.second) || 
	     _face.first->getIdentifier() < 
	     _face.first->getNeighbor(_face.second)->getIdentifier());
  }

  // Increment the iterator.
  void
  increment() {
    // While we have not gone through all of the cells.
    while (_face.first != _cellsEnd) {

      // Advance to the next face.

      // If we have not gone through all the faces of this cell.
      if (_face.second != M) {
	// Advance to the next face within the cell.
	++_face.second;
      }
      else {
	// Advance to the next cell.
	++_face.first;
	_face.second = 0;
      }

      // First check that we have not gone through all of the cells.
      // If there is no adjacent cell through the face or
      // if the identifier of this cell is less than the identifier of the 
      // adjacent cell through the face.
      if (_face.first != _cellsEnd && isValid()) {
	// Then we have a face.  Break out of the loop and return.
	break;
      }
    }
  }

  // Decrement the iterator.
  void
  decrement() {
    // While we have not gone through all of the cells.
    while (_face.first != _cellsEnd) {

      // Move to the previous face.

      // If we have not gone through all the faces of this cell.
      if (_face.second != 0) {
	// Go to the previous face within the cell.
	--_face.second;
      }
      else {
	// Go to the previous cell.
	--_face.first;
	_face.second = M;
      }

      // First check that we have not gone through all of the cells.
      // If there is no adjacent cell through the face or
      // if the identifier of this cell is less than the identifier of the 
      // adjacent cell through the face.
      if (_face.first != _cellsEnd && isValid()) {
	// Then we have a face.  Break out of the loop and return.
	break;
      }
    }
  }

};

END_NAMESPACE_GEOM

#endif
