// -*- C++ -*-

/*! 
  \file ads/array/SparseArraySigned1.h
  \brief A class for a 1-D sparse array with signed values.
*/

#if !defined(__ads_SparseArraySigned1_h__)
#error This file is an implementation detail of the class SparseArraySigned.
#endif

#include "../defs.h"

BEGIN_NAMESPACE_ADS


//! A sparse array of type T in 1 dimension.
/*!
  <!--I put an anchor here because I cannot automatically reference this 
  class. -->
  \anchor SparseArraySigned1

  \param T is the value type.  By default it is double.

  The free functions are grouped into the following categories.
  - \ref ads_array_ArrayContainerFunctions
  - \ref ads_array_SparseArrayFunctions
  - \ref ads_array_SparseArray1Functions
  - \ref ads_array_SparseArraySignedFunctions

  Note that \c operator[] is container indexing.  \c a[i] is the i_th non-null
  element.  However, \c operator() is array indexing.  \c a(i) is the element
  with index \c i.
*/
template<typename T>
class SparseArraySigned<1,T> : public SparseArray<1,T> {
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

  //! An index into the array.
  typedef int index_type;

  //
  // Data.
  //

protected:

  //! The sign.  This is used if there are no non-null elements.
  int _sign;

  //
  // Using
  //

public:

  // Static members.
  using Base::getRank;

  // Accessors.
  using Base::size;
  using Base::empty;
  using Base::max_size;
  using Base::begin;
  using Base::end;
  using Base::data;
  using Base::operator[];
  using Base::getNull;
  using Base::isNull;
  using Base::isNonNull;
  using Base::fillNonNull;
  using Base::getIndices;

  // Manipulators.
  //using Base::begin;
  //using Base::end;
  //using Base::data;

  // File I/O.
  using Base::write_elements_ascii;
  using Base::write_elements_binary;
  using Base::read_elements_ascii;
  using Base::read_elements_binary;

protected:

  // Data.
  using Base::_indices;
  using Base::_null;

  //
  // Friends.
  //

  //! Merge two arrays.
  friend void merge<>(const SparseArraySigned<1,T>& a, 
		      const SparseArraySigned<1,T>& b, 
		      SparseArraySigned<1,T>* c);

  //! Remove the unecessary elements in an array.
  friend void removeUnecessaryElements<>(SparseArraySigned<1,T>* a);

  //! Compute the union of two implicit functions.
  friend void computeUnion<>(const SparseArraySigned<1,T>& a, 
			     const SparseArraySigned<1,T>& b, 
			     SparseArraySigned<1,T>* c);

  //! Compute the intersection of two implicit functions.
  friend void computeIntersection<>(const SparseArraySigned<1,T>& a, 
				    const SparseArraySigned<1,T>& b, 
				    SparseArraySigned<1,T>* c);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.
  SparseArraySigned(parameter_type nullValue 
		    = std::numeric_limits<value_type>::max()) :
    Base(nullValue),
    _sign(1)
  {}

  //! Construct a 1-D array sparse array from the values and indices.
  template<typename IndexForwardIter, typename ValueForwardIter>
  SparseArraySigned(IndexForwardIter indicesBeginning, 
		    IndexForwardIter indicesEnd,
		    ValueForwardIter valuesBeginning, 
		    ValueForwardIter valuesEnd,
		    parameter_type nullValue
		    = std::numeric_limits<value_type>::max());

  //! Construct a 1-D sparse array from a 1-D dense array.
  template<typename T2, bool A>
  SparseArraySigned(const Array<1,T2,A>& array, parameter_type nullValue);

  //! Copy constructor.  Deep copy.
  SparseArraySigned(const SparseArraySigned& other) :
    Base(other),
    _sign(other._sign)
  {}

  //! Destructor.
  ~SparseArraySigned()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  // @{

  //! Assignment operator.
  SparseArraySigned& 
  operator=(const SparseArraySigned& other) {
    if (&other != this) {
      Base::operator=(other);
      _sign = other._sign;
    }
    return *this;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the sign.
  int
  getSign() const {
    return _sign;
  }

  //! Return the memory size.
  size_type 
  getMemoryUsage() const {
    return Base::getMemoryUsage() + sizeof(int);
  }

  //! Return the specified element.
  value_type
  operator()(const int i) const;

  //! Return true if this sparse array is equal to the argument.
  bool
  operator==(const SparseArraySigned& x) const {
    return (Base::operator==(x) && _sign == x._sign);
  }

  //! Fill a dense array with the elements from this sparse array.
  template<typename T2, bool A>
  void
  fill(ads::Array<1,T2,A>* array) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Swaps data with another SparseArraySigned.
  void
  swap(SparseArraySigned& x) {
    Base::swap(x);
    std::swap(_sign, x._sign);
  }

  //! Negate each component.
  void
  negate() {
    if (empty()) {
      _sign = - _sign;
    }
    Base::negate();
  }

  //! Set the sign.
  void
  setSign(const int sign) {
    _sign = sign;
  }

  // @}
  //--------------------------------------------------------------------------
  /*! \name Assignment operators with scalar operand.
    These need to be defined to get the correct return type.  I can't just
    inherit the functions from the base class.
  */
  // @{

  //! Set each component to \c x.
  SparseArraySigned& 
  operator=(parameter_type x) {
    Base::operator=(x);
    return *this;
  }

  //! Add \c x to each component.
  SparseArraySigned& 
  operator+=(parameter_type x) {
    Base::operator+=(x);
    return *this;
  }

  //! Subtract \c x from each component.
  SparseArraySigned& 
  operator-=(parameter_type x) {
    Base::operator-=(x);
    return *this;
  }

  //! Multiply each component by \c x.
  SparseArraySigned& 
  operator*=(parameter_type x) {
    Base::operator*=(x);
    return *this;
  }

  //! Divide each component by \c x.
  SparseArraySigned& 
  operator/=(parameter_type x) {
    Base::operator/=(x);
    return *this;
  }

  //! Mod each component by \c x.
  SparseArraySigned& 
  operator%=(parameter_type x) {
    Base::operator%=(x);
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

#define __ads_SparseArraySigned1_ipp__
#include "SparseArraySigned1.ipp"
#undef __ads_SparseArraySigned1_ipp__
