// -*- C++ -*-

/*! 
  \file MemFunIterator.h
  \brief An iterator that calls a member function in \c operator*().
*/

#if !defined(__ads_MemFunIterator_h__)
#define __ads_MemFunIterator_h__

#include "AdaptedIterator.h"

#include "../../third-party/loki/TypeManip.h"

BEGIN_NAMESPACE_ADS

//! An iterator that calls a member function in \c operator*().
/*!
  CONTINUE
*/
template<typename _Iterator, class Pointee, typename Result, bool Const = true>
class MemFunIterator :
  public AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  Result,
  typename std::iterator_traits<_Iterator>::difference_type,
  void,  // We don't use the pointer type.
  void>  { // We don't use the reference type.
  //
  // Private types.
  //

private:

  typedef AdaptedIterator<
  _Iterator,
  typename std::iterator_traits<_Iterator>::iterator_category,
  Result,
  typename std::iterator_traits<_Iterator>::difference_type,
  void,
  void>
  Base;

  //
  // Public types.
  //

public:

  // The following five types are required to be defined for any iterator.

  //! The iterator category.
  typedef typename Base::iterator_category iterator_category;
  //! The type "pointed to" by the iterator.  This is the function's return type: Result.
  typedef typename Base::value_type value_type;
  //! Distance between iterators is represented as this type.
  typedef typename Base::difference_type difference_type;
  //! This type represents a pointer-to-value_type.
  typedef typename Base::pointer pointer;
  //! This type represents a reference-to-value_type.
  typedef typename Base::reference reference;

  //! The base iterator type.
  typedef typename Base::Iterator Iterator;

  // CONTINUE:
  // I would like to define the pointee type as below and not have it as 
  // a template parameter, but MSVC++ does not like the typedef below.
  //! The pointee type.  The class to which the iterator points.
  //typedef typename std::iterator_traits<Iterator>::value_type pointee_type;

  //
  // Private types.
  //

private:

  //! A pointer to a non-const member function.
  typedef value_type (Pointee::*mem_fun)();
  //! A pointer to a const member function.
  typedef value_type (Pointee::*mem_fun_const)() const;

  //
  // Public types.
  //

public:

  //! A pointer to a member function.
  typedef typename Loki::Select<Const, mem_fun_const, mem_fun>::Result 
  MemberFunction;
  
  //
  // Member data.
  //

private:

  //! The member function.
  MemberFunction _f;

public:
  
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  MemFunIterator() :
    Base(),
    _f()
  {}

  //! Construct from a pointer to a member function.
  MemFunIterator(MemberFunction f) :
    Base(),
    _f(f)
  {}

  //! Construct from a pointer to a member function and an iterator.
  MemFunIterator(MemberFunction f, const Iterator& i) :
    Base(i),
    _f(f)
  {}

  //! Copy constructor.
  MemFunIterator(const MemFunIterator& x) :
    Base(x),
    _f(x._f)
  {}

  //! Assignment operator.
  MemFunIterator& 
  operator=(const MemFunIterator& other) {
    Base::operator=(other);
    _f = other._f;
    return *this;
  }

  //! Assignment from an iterator.
  MemFunIterator& 
  operator=(const Iterator& i) {
    Base::operator=(i);
    return *this;
  }

  //! Set the pointer to a member function.
  MemFunIterator& 
  operator=(MemberFunction f) {
    _f = f;
    return *this;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the base iterator.
  const Iterator&
  base() const {
    return Base::base();
  }

  //! Convert to the base iterator.
  operator Iterator() const {
    return Base::base();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Iterator Functionality.
  //@{

  //
  // Trivial iterator requirements.
  //

  //! Call the member function of the base iterator.
  value_type
  operator*() const {
    // CONTINUE: Try to understand this.
    //return (Base::_iterator->*_f)();
    return ((*Base::_iterator).*_f)();
  }

  /* CONTINUE
  //! 
  pointer
  operator->() const
  {
    return *Base::_iterator;
  }
  */

  //
  // Forward iterator requirements.
  //

  //! Pre-increment.
  MemFunIterator&
  operator++() {
    ++Base::_iterator;
    return *this;
  }

  //! Post-increment.  Warning: This is not as efficient as the pre-increment.
  MemFunIterator
  operator++(int) {
    MemFunIterator tmp = *this;
    ++Base::_iterator;
    return tmp;
  }

  //
  // Bi-directional iterator requirements.
  //

  //! Pre-decrement.
  MemFunIterator&
  operator--() {
    --Base::_iterator;
    return *this;
  }

  //! Post-decrement.  Warning: This is not as efficient as the pre-decrement.
  MemFunIterator
  operator--(int) {
    MemFunIterator tmp = *this;
    --Base::_iterator;
    return tmp;
  }

  //
  // Random access iterator requirements.
  //

  //! Sub-scripting.
  value_type
  operator[](const difference_type& n) const {
    return Base::_iterator[n]._f();
  }

  //! Addition assignment.
  MemFunIterator&
  operator+=(const difference_type& n) {
    Base::_iterator += n;
    return *this;
  }

  //! Addition.
  MemFunIterator
  operator+(const difference_type& n) const {
    MemFunIterator x(*this);
    x += n;
    return x;
  }

  //! Subtraction assignment.
  MemFunIterator&
  operator-=(const difference_type& n) {
    Base::_iterator -= n;
    return *this;
  }

  //! Subtraction.
  MemFunIterator
  operator-(const difference_type& n) const {
    MemFunIterator x(*this);
    x -= n;
    return x;
  }

  //@}
};

//
// Random access iterator requirements.
//

//! Offset from an iterator.
/*!
  \relates MemFunIterator
*/
template<typename Iterator, class Pointee, typename Result, bool Const>
inline
MemFunIterator<Iterator,Pointee,Result,Const>
operator+(typename MemFunIterator<Iterator,Pointee,Result,Const>::
	   difference_type n,
	   const MemFunIterator<Iterator,Pointee,Result,Const>& x) {
  return x + n;
}

// CONTINUE
#if 0
//! Convenience function for instantiating a MemFunIterator.
/*!
  \relates MemFunIterator
*/
template<typename _Iterator, typename Result>
inline
MemFunIterator<_Iterator,Result>
constructMemFunIterator
(Result ((typename MemFunIterator<_Iterator,Result>::pointee_type)::*f)()) {
  return MemFunIterator<_Iterator,Result>(f);
}

//! Convenience function for instantiating a MemFunIterator.
/*!
  \relates MemFunIterator
*/
template<typename _Iterator, typename Result>
inline
MemFunIterator<_Iterator,Result>
constructMemFunIterator
(Result ((typename MemFunIterator<_Iterator,Result>::pointee_type)::*f)(),
  const _Iterator& i) {
  return MemFunIterator<_Iterator,Result>(f, i);
}
#endif

END_NAMESPACE_ADS

#endif
