// -*- C++ -*-

#if !defined(__geom_mesh_iss_IssiaFaceIterator_h__)
#define __geom_mesh_iss_IssiaFaceIterator_h__

#if defined(DEBUG_geom) && !defined(DEBUG_IssiaFaceIterator)
#define DEBUG_IssiaFaceIterator
#endif

BEGIN_NAMESPACE_GEOM

//! Face iterator in an indexed simplex set (mesh).
/*!
  The value type is \c std::pair<int,int>.  The first component is the 
  simplex index.  The second component is the local index of the face.
*/
template<class ISSIA>
class 
IssiaFaceIterator  :
  public std::iterator<std::bidirectional_iterator_tag, // Iterator tag.
		       std::pair<int,int>, // Value type.
		       std::ptrdiff_t, // Pointer difference type.
		       const std::pair<int,int>*, // Pointer type.
		       const std::pair<int,int>&> { // Reference type.
  //
  // Private types.
  //

private:

  //! The base type.
  typedef std::iterator<std::bidirectional_iterator_tag,
			std::pair<int,int>,
			std::ptrdiff_t,
			const std::pair<int,int>*,
			const std::pair<int,int>&> Base;
  
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
  
  //! A const pointer to the mesh.
  const ISSIA* _mesh;
  //! The face.
  value_type _face;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  IssiaFaceIterator();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors for the mesh to call.
  //@{

  //! Construct a face const iterator for the mesh.
  IssiaFaceIterator(const ISSIA* mesh, const int simplexIndex) :
    Base(),
    _mesh(mesh),
    _face(simplexIndex, 0) {
    // We allow it to be equal to simplices_size for the past-the-end iterator.
    assert(0 <= simplexIndex && simplexIndex <= _mesh->getSimplicesSize());
    if (simplexIndex != _mesh->getSimplicesSize() && ! isValid()) {
      increment();
    }
  }

  //@}

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Copy constructor.
  IssiaFaceIterator(const IssiaFaceIterator& other) :
    Base(),
    _mesh(other._mesh),
    _face(other._face)
  {}    

  //! Assignment operator.
  IssiaFaceIterator&
  operator=(const IssiaFaceIterator& other) {
    if (&other != this) {
      _mesh = other._mesh;
      _face = other._face;
    }
    return *this;
  }

  //! Trivial destructor.
  ~IssiaFaceIterator()
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
  IssiaFaceIterator&
  operator++() { 
    increment();
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  IssiaFaceIterator
  operator++(int) {
    IssiaFaceIterator x(*this); 
    ++*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  IssiaFaceIterator&
  operator--() { 
    decrement();
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  IssiaFaceIterator
  operator--(int) {
    IssiaFaceIterator x(*this); 
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
  operator==(const IssiaFaceIterator& x) {
#ifdef DEBUG_IssiaFaceIterator
    // These must be iterators over the same mesh.
    assert(_mesh == x._mesh);
#endif
    return _face == x._face;
  }

  //! Return true if x is not an iterator to the same face.
  bool
  operator!=(const IssiaFaceIterator& x) {
    return ! operator==(x);
  }

  //@}


private:

  // If there is no adjacent cell through the face or
  // if the address of this cell is less than the address of the 
  // adjacent cell through the face.
  bool
  isValid() {
    // The index of the adjacent neighbor.
    int neighbor = _mesh->getAdjacent(_face.first, _face.second);
    // Return true if there is no neighbor or if our index is less than 
    // the neighbor.
    return (neighbor == -1 || _face.first < neighbor);
  }

  // Increment the iterator.
  void
  increment() {
    assert(0 <= _face.first && _face.first < _mesh->getSimplicesSize());
    assert(0 <= _face.second && _face.second <= ISSIA::M);
    // While we have not gone through all of the cells.
    while (_face.first != _mesh->getSimplicesSize()) {
      // Advance to the next face.

      // If we have not gone through all the faces of this cell.
      if (_face.second != ISSIA::M) {
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
      // if the index of this cell is less than the index of the 
      // adjacent cell through the face.
      if (_face.first != _mesh->getSimplicesSize() && isValid()) {
	// Then we have a face.  Break out of the loop and return.
	break;
      }
    }
  }

  // Decrement the iterator.
  void
  decrement() {
    // While we have not gone through all of the cells.
    while (_face.first != -1) {

      // Move to the previous face.

      // If we have not gone through all the faces of this cell.
      if (_face.second != 0) {
	// Go to the previous face within the cell.
	--_face.second;
      }
      else {
	// Go to the previous cell.
	--_face.first;
	_face.second = ISSIA::M;
      }

      // First check that we have not gone through all of the cells.
      // If there is no adjacent cell through the face or
      // if the index of this cell is less than the index of the 
      // adjacent cell through the face.
      if (_face.first != -1 && isValid()) {
	// Then we have a face.  Break out of the loop and return.
	break;
      }
    }
  }

};

END_NAMESPACE_GEOM

#endif
