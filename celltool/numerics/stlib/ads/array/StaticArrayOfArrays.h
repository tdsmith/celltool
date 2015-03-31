// -*- C++ -*-

/*! 
  \file StaticArrayOfArrays.h
  \brief A class for a static array of arrays.
*/

#if !defined(__ads_StaticArrayOfArrays_h__)
#define __ads_StaticArrayOfArrays_h__

#include "Array.h"

// If we are debugging the whole ads package.
#if defined(DEBUG_ads) && !defined(DEBUG_StaticArrayOfArrays)
#define DEBUG_StaticArrayOfArrays
#endif

BEGIN_NAMESPACE_ADS

//! A static array of arrays.
/*!
  \param T is the value type.  By default it is double.
*/
template<typename T = double>
class StaticArrayOfArrays :
  public ArrayContainer<T> {

  //
  // Private types.
  //

private:

  typedef ArrayTypes<T> Types;
  typedef ArrayContainer<T> Base;

  //
  // Public Types.
  //

public:

  //! The element type.
  typedef typename Types::value_type value_type;
  //! The parameter type.
  /*! 
    This is used for passing the value type as an argument.
  */
  typedef typename Types::parameter_type parameter_type;
  //! The unqualified value type.
  /*!
    The value type with top level \c const and \c volatile qualifiers removed.
  */
  typedef typename Types::unqualified_value_type unqualified_value_type;

  //! A pointer to an array element.
  typedef typename Types::pointer pointer;
  //! A pointer to a constant array element.
  typedef typename Types::const_pointer const_pointer;

  //! An iterator in the array.
  typedef typename Types::iterator iterator;
  //! A iterator on constant elements in the array.
  typedef typename Types::const_iterator const_iterator;

  //! A reference to an array element.
  typedef typename Types::reference reference;
  //! A reference to a constant array element.
  typedef typename Types::const_reference const_reference;

  //! The size type is a signed integer.
  /*!
    Having \c std::size_t (which is an unsigned integer) as the size type
    causes minor problems.  Consult "Large Scale C++ Software Design" by 
    John Lakos for a discussion of using unsigned integers in a class 
    interface.
  */
  typedef typename Types::size_type size_type;
  //! Pointer difference type.
  typedef typename Types::difference_type difference_type;

  //
  // Data.
  //

private:

  //! Pointers that determine the beginning and end of each array.
  Array<1,iterator> _pointers;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Empty data structure.
  StaticArrayOfArrays() :
    Base(),
    _pointers(1) {
    // begin is qualified for the benefit of the icc compiler.
    _pointers[0] = Base::begin();
  }

  //! Construct from the array sizes and the values.
  template<typename IntForwardIter, typename ValueForwardIter>
  StaticArrayOfArrays(IntForwardIter sizesBeginning, IntForwardIter sizesEnd,
		      ValueForwardIter valuesBeginning, 
		      ValueForwardIter valuesEnd);

  //! Rebuild from the array sizes.
  template<typename IntForwardIter>
  void
  rebuild(const size_type numberOfElements, 
	  IntForwardIter sizesBeginning, IntForwardIter sizesEnd);

  //! Rebuild from the array sizes and the values.
  template<typename IntForwardIter, typename ValueForwardIter>
  void
  rebuild(IntForwardIter sizesBeginning, IntForwardIter sizesEnd,
	  ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd) {
    rebuild(std::distance(valuesBeginning, valuesEnd), sizesBeginning, 
	    sizesEnd);
    std::copy(valuesBeginning, valuesEnd, begin());
  }

  //! Copy constructor.
  StaticArrayOfArrays(const StaticArrayOfArrays& other) :
    Base(other),
    _pointers(other._pointers) {
    _pointers += begin() - other.begin();
  }

  //! Destructor.
  ~StaticArrayOfArrays()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  // @{

  //! Assignment operator.
  StaticArrayOfArrays& 
  operator=(const StaticArrayOfArrays& other) {
    if (&other != this) {
      Base::operator=(other);
      _pointers = other._pointers;
      _pointers += begin() - other.begin();
    }
    return *this;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors for the whole set of elements.
  // @{

  //! Return the number of arrays.
  size_type
  getNumberOfArrays() const {
    return _pointers.size() - 1;
  }

  // CONTINUE: I do this to placate the icc compiler.
  //! Return the total number of elements.
  //using Base::size;
  size_type 
  size() const {
    return Base::size();
  }

  //! Return true if the total number of elements is empty.
  using Base::empty;

  //! Return the size of the largest possible array.
  using Base::max_size;

  //! Return the memory size.
  size_type 
  getMemoryUsage() const {
    return Base::getMemoryUsage() + _pointers.getMemoryUsage();
  }

  // CONTINUE: I do this to placate the icc compiler.
  //! Return a const iterator to the first value.
  //using Base::begin;
  //! Return a const iterator to the first value.
  const_iterator 
  begin() const { 
    return Base::begin();
  }

  // CONTINUE: I do this to placate the icc compiler.
  //! Return a const iterator to one past the last value.
  //using Base::end;
  const_iterator 
  end() const { 
    return Base::end();
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors for individual arrays.
  // @{

  //! Return the number of elements in the n_th array.
  size_type
  size(const int n) const {
    return size_type(_pointers[n + 1] - _pointers[n]);
  }

  //! Return true if the n_th array is empty.
  bool 
  empty(const int n) const { 
    return size(n) == 0;
  }

  //! Return a const iterator to the first value in the n_th array.
  const_iterator 
  begin(const int n) const { 
    return _pointers[n];
  }

  //! Return a const iterator to one past the last value in the n_th array.
  const_iterator 
  end(const int n) const { 
    return _pointers[n + 1];
  }

  // CONTINUE: Inherit different functionality from Base.
  //! Return a const iterator to the first element of the n_th array.
  const_iterator
  operator[](const int n) const {
    return begin(n);
  }

  //! Return a const iterator to the first element of the n_th array.
  const_iterator
  operator()(const int n) const {
    return operator[](n);
  }

  //! Return the m_th element of the n_th array.
  parameter_type
  operator()(const int n, const int m) const {
#ifdef DEBUG_StaticArrayOfArrays
    assert(0 <= m && m < size(n));
#endif
    return *(begin(n) + m);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators for the whole set of elements.
  // @{

  // CONTINUE: I do this to placate the icc compiler.
  //! Return an iterator to the first value.
  //using Base::begin;
  //! Return an iterator to the first value.
  iterator 
  begin() { 
    return Base::begin();
  }

  // CONTINUE: I do this to placate the icc compiler.
  //! Return an iterator to one past the last value.
  //using Base::end;
  iterator 
  end() { 
    return Base::end();
  }

  //! Negate each component.
  using Base::negate;

  //! Swaps data with another StaticArrayOfArrays.
  void
  swap(StaticArrayOfArrays& other) {
    if (&other != this) {
      Base::swap(other);
      _pointers.swap(other._pointers);
    }
  }

  //! Clear the array of arrays.
  void
  clear() {
    Base::resize(0);
    _pointers.resize(1);
    _pointers[0] = begin();
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators for individual arrays.
  // @{

  //! Return an iterator to the first value in the n_th array.
  iterator 
  begin(const int n) { 
    return _pointers[n];
  }

  //! Return an iterator to one past the last value in the n_th array.
  iterator 
  end(const int n) { 
    return _pointers[n + 1];
  }

  //! Return an iterator to the first element of the n_th array.
  iterator
  operator[](const int n) {
    return begin(n);
  }

  //! Return an iterator to the first element of the n_th array.
  iterator
  operator()(const int n) {
    return operator[](n);
  }

  //! Return the m_th element of the n_th array.
  reference
  operator()(const int n, const int m) {
#ifdef DEBUG_StaticArrayOfArrays
    assert(0 <= m && m < size(n));
#endif
    return *(begin(n) + m);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators with scalar operand.
  // @{

  //! Set each element to \c x.
  StaticArrayOfArrays& 
  operator=(parameter_type x) {
    Base::operator=(x);
    return *this;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  // @{

  //! Return true if the arrays are equal.
  bool
  operator==(const StaticArrayOfArrays<T>& x) const {
    if (Base::operator!=(x)) {
      return false;
    }
    for (int n = 0; n != getNumberOfArrays(); ++n) {
      if (size(n) != x.size(n)) {
	return false;
      }
    }
    return true;
  }

  //! Return true if the arrays are not equal.
  bool
  operator!=(const StaticArrayOfArrays<T>& x) const {
    return ! operator==(x);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Write to a file stream in ascii format.
  void
  put(std::ostream& out) const;

  //! Read from a file stream in ascii format.
  void
  get(std::istream& in);

  // @}
};

//
// File I/O.
//

//! Write a StaticArrayOfArrays in ascii format.
/*!
  \relates StaticArrayOfArrays

  Below is the file format.
  \verbatim
  number_of_arrays number_of_elements
  array_0_size
  array_0_value_0 array_0_value_1 ... 
  array_1_size
  array_1_value_0 array_1_value_1 ... 
  ... \endverbatim
*/
template<typename T>
inline
std::ostream&
operator<<(std::ostream& out, const StaticArrayOfArrays<T>& x) {
  x.put(out);
  return out;
}

//! Read a StaticArrayOfArrays in ascii format.
/*!
  \relates StaticArrayOfArrays

  Below is the file format.
  \verbatim
  number_of_arrays number_of_elements
  array_0_size
  array_0_value_0 array_0_value_1 ... 
  array_1_size
  array_1_value_0 array_1_value_1 ... 
  ... \endverbatim
*/
template<typename T>
inline
std::istream&
operator>>(std::istream& in, StaticArrayOfArrays<T>& x) {
  x.get(in);
  return in;
}

END_NAMESPACE_ADS

#define __StaticArrayOfArrays_ipp__
#include "StaticArrayOfArrays.ipp"
#undef __StaticArrayOfArrays_ipp__

#endif
