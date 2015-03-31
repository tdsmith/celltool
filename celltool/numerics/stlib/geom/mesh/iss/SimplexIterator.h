// -*- C++ -*-

/*! 
  \file geom/mesh/iss/SimplexIterator.h
  \brief A simplex iterator for an indexed simplex set.
*/

#if !defined(__geom_mesh_iss_SimplexIterator_h__)
#define __geom_mesh_iss_SimplexIterator_h__

#include <iterator>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SimplexIterator)
#define DEBUG_SimplexIterator
#endif

BEGIN_NAMESPACE_GEOM

// CONTINUE: Make this a nested class.  It is only used through the 
// IndSimpSet::simplex_const_iterator type.

//! A simplex iterator for a mesh.
/*!
  \param ISS is the mesh.  

  This is a const iterator.
*/
template<class ISS>
class SimplexIterator :
  public std::iterator<std::random_access_iterator_tag,
		       typename ISS::Simplex,
		       std::ptrdiff_t,
		       const typename ISS::Simplex*,
		       const typename ISS::Simplex&> {
  //
  // Private types.
  //

private:
  
  typedef std::iterator<std::random_access_iterator_tag,
			typename ISS::Simplex,
			std::ptrdiff_t,
			const typename ISS::Simplex*,
			const typename ISS::Simplex&> Base;

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

  //! Indexed simplex set (mesh).
  typedef ISS IssType;

  //
  // Member data.
  //

private:

  int _index;
  mutable value_type _simplex;
  const IssType& _iss;

  //
  // Not implemented.
  //

private:

  // Default constructor not implemented.
  SimplexIterator();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Construct a simplex iterator for a mesh.
  /*!
    It is initialized to point to the first simplex.
  */
  SimplexIterator(const IssType& iss) :
    Base(),
    _index(0),
    _simplex(),
    _iss(iss)
  {}

  //! Copy constructor.
  SimplexIterator(const SimplexIterator& other) :
    Base(),
    _index(other._index),
    _simplex(),
    _iss(other._iss)
  {}    

  //! Assignment operator.
  SimplexIterator&
  operator=(const SimplexIterator& other)
  {
    if (&other != this) {
      _index = other._index;
    }
    return *this;
  }

  //! Destructor.
  ~SimplexIterator()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Forward iterator requirements
  //@{

  //! Dereference.
  reference
  operator*() const {
    // Update the simplex.
    update(_index);
    // Then return a constant reference to it.
    return _simplex;
  }

  //! Pointer dereference.
  pointer
  operator->() const {
    // Update the simplex.
    update(_index);
    // Then return a constant reference to it.
    return &_simplex;
  }

  //! Pre-increment.
  SimplexIterator&
  operator++() { 
    ++_index; 
    return *this; 
  }
      
  //! Post-increment.
  /*!
    \note This is not efficient.  If possible, use the pre-increment operator
    instead.
  */
  SimplexIterator
  operator++(int) {
    SimplexIterator x(*this); 
    ++*this;
    return x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Bidirectional iterator requirements
  //@{

  //! Pre-decrement.
  SimplexIterator&
  operator--() { 
    --_index; 
    return *this; 
  }
      
  //! Post-decrement.
  /*!
    \note This is not efficient.  If possible, use the pre-decrement operator
    instead.
  */
  SimplexIterator
  operator--(int) {
    SimplexIterator x(*this); 
    --*this;
    return x;
  }
      
  //@}
  //--------------------------------------------------------------------------
  //! \name Random access iterator requirements
  //@{

  //! Iterator indexing.
  reference
  operator[](const difference_type n) const { 
    // Update the simplex with the requested offset value.
    update(_index + n);
    // Then return a constant reference to it.
    return _simplex;
  }
  
  //! Positive offseting.
  SimplexIterator&
  operator+=(const difference_type n) { 
    _index += n; 
    return *this; 
  }

  //! Positive offseting.
  /*!
    \note This is not efficient.  If possible, use \c += instead.
  */
  SimplexIterator
  operator+(const difference_type n) const { 
    SimplexIterator x(*this); 
    x += n;
    return x;
  }
      
  //! Negative offseting.
  SimplexIterator&
  operator-=(const difference_type n) { 
    _index -= n; 
    return *this; 
  }

  //! Negative offseting.
  /*!
    \note This is not efficient.  If possible, use \c -= instead.
  */
  SimplexIterator
  operator-(const difference_type n) const { 
    SimplexIterator x(*this); 
    x -= n;
    return x;
  }

  //! Return the index.
  int
  getBase() const {
    return _index;
  }

  //@}

  //
  // Private member functions
  //

private:

  // Update the simplex for the current index.
  void
  update(const int index) const {
    for (int m = 0; m != IssType::M + 1; ++m) {
      _simplex[m] = _iss.getSimplexVertex(index, m);
    }
  }
};


//
// Forward iterator requirements
//


//! Return true if the iterators have a handle to the same index.
template<class ISS>
inline 
bool
operator==(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return x.getBase() == y.getBase(); 
}


//! Return true if the iterators do not have a handle to the same index.
template<class ISS>
inline 
bool
operator!=(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return !(x == y);
}


//
// Random access iterator requirements
//


//! Return true if the index of \c x precedes that of \c y.
template<class ISS>
inline 
bool 
operator<(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return x.getBase() < y.getBase(); 
}


//! Return true if the index of \c x follows that of \c y.
template<class ISS>
inline 
bool
operator>(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return x.getBase() > y.getBase(); 
}


//! Return true if the index of \c x precedes or is equal to that of \c y.
template<class ISS>
inline 
bool
operator<=(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return x.getBase() <= y.getBase(); 
}


//! Return true if the index of \c x follows or is equal to that of \c y.
template<class ISS>
inline 
bool
operator>=(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return x.getBase() >= y.getBase(); 
}


//! The difference of two iterators.
template<class ISS>
inline 
typename SimplexIterator<ISS>::difference_type
operator-(const SimplexIterator<ISS>& x, const SimplexIterator<ISS>& y) { 
  return x.getBase() - y.getBase(); 
}


//! Iterator advance.
template<class ISS>
inline 
SimplexIterator<ISS>
operator+(typename SimplexIterator<ISS>::difference_type n,
	  const SimplexIterator<ISS>& i) { 
  SimplexIterator<ISS> x(i); 
  x += n;
  return x;
}

END_NAMESPACE_GEOM

#endif
