// -*- C++ -*-

/*! 
  \file ads/array/SparseArray2.h
  \brief A class for a 2-D sparse array.
*/

#if !defined(__ads_SparseArray2_h__)
#error This file is an implementation detail of the class SparseArray.
#endif

#include "../defs.h"

#include "../functor/select.h"

BEGIN_NAMESPACE_ADS


//! A sparse array of type T in 2-D.
/*!
  <!--I put an anchor here because I cannot automatically reference this 
  class. -->
  \anchor SparseArray2

  \param T is the value type.  By default it is double.
*/
template<typename T>
class SparseArray<2,T> : public SparseArray<1,T> {
  //
  // Private types.
  //

private:

  typedef ArrayTypes<T> Types;
  typedef SparseArray<1,T> Base;

  //
  // Public Types.
  //

public:

  //! The element type of the array.
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

  //! A multi-index.  Index in 2 dimensions.
  typedef FixedArray<2,int> index_type;

  //
  // Data.
  //

private:

  //! The index offsets.
  Array<1,int> _offsets;

  //
  // Using
  //

public:

  // Accessors.
  using Base::size;
  using Base::empty;
  using Base::max_size;
  using Base::begin;
  using Base::end;
  using Base::data;
  using Base::operator[];
  using Base::getNull;

  // Manipulators.
  //using Base::begin;
  //using Base::end;
  //using Base::data;
  using Base::negate;

  // File I/O.
  using Base::write_elements_ascii;
  using Base::write_elements_binary;
  using Base::read_elements_ascii;
  using Base::read_elements_binary;

private:

  // Using base data.
  using Base::_indices;
  using Base::_null;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.
  SparseArray() :
    Base(),
    _offsets()
  {}

  //! Construct a 2-D array sparse array from the values and indices.
  /*!
    If the null value is not specified, the default value for the type is 
    used.  For built-in types, \c value_type() is equivalent to 
    \c value_type(0).
  */
  template<typename IndexBiDirIter, typename ValueForwardIter>
  SparseArray(IndexBiDirIter indicesBeginning, IndexBiDirIter indicesEnd,
	      ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd,
	      parameter_type nullValue = value_type());

  //! Construct a 2-D sparse array from a 2-D dense array.
  template<typename T2, bool A>
  SparseArray(const Array<2,T2,A>& array, parameter_type nullValue);

  //! Copy constructor.  Deep copy.
  SparseArray(const SparseArray& x) :
    Base(x),
    _offsets(x._offsets)
  {}

  //! Destructor.
  ~SparseArray()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  // @{

  //! Assignment operator.
  SparseArray& 
  operator=(const SparseArray& other) {
    if (&other != this) {
      Base::operator=(other);
      _offsets = other._offsets;
    }
    return *this;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Static members.
  // @{

  //! Return the rank (number of dimensions) of the array.
  static
  int
  getRank() {
    return 2;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the memory size.
  size_type 
  getMemoryUsage() const {
    return Base::getMemoryUsage() + _offsets.getMemoryUsage();
  }

  //! Return true if the element is null.
  bool
  isNull(const index_type& index) const;

  //! Return true if the element is non-null.
  bool
  isNonNull(const index_type& index) const {
    return ! isNull(index);
  }

  //! Return the specified element.
  parameter_type
  operator()(const index_type& index) const;

  //! Return true if this sparse array is equal to the argument.
  bool
  operator==(const SparseArray& x) const {
    return Base::operator==(x) && _offsets == x._offsets;
  }

  //! Return true if this sparse array is not equal to the argument.
  bool
  operator!=(const SparseArray& x) const {
    return ! operator==(x);
  }

  //! Fill a dense array with the elements from this sparse array.
  template<typename T2, bool A>
  void
  fill(ads::Array<2,T2,A>* array) const;

  //! Fill a dense array using only the non-null elements from this sparse array.
  template<typename T2, bool A>
  void
  fillNonNull(ads::Array<2,T2,A>* array) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Swaps data with another SparseArray.
  void
  swap(SparseArray& other) {
    if (&other != this) {
      Base::swap(other);
      _offsets.swap(other._offsets);
    }
  }

  // @}
  //--------------------------------------------------------------------------
  /*! \name Assignment operators with scalar operand.
  */
  // @{

  //! Set each component to \c x.
  SparseArray& 
  operator=(parameter_type x) {
    Base::operator=(x);
    return *this;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Write the array to a file stream in ascii format.
  void
  put(std::ostream& out) const;

  //! Read from a file stream in ascii format.
  void
  get(std::istream& in);

  // @}
};

END_NAMESPACE_ADS

#define __ads_SparseArray2_ipp__
#include "SparseArray2.ipp"
#undef __ads_SparseArray2_ipp__
