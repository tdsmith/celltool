// -*- C++ -*-

/*! 
  \file ads/array/SparseArray1.h
  \brief A class for a 1-D sparse array.
*/

#if !defined(__ads_SparseArray1_h__)
#error This file is an implementation detail of the class SparseArray.
#endif

#include "../defs.h"

#include <vector>

BEGIN_NAMESPACE_ADS

//! A sparse array of type T in 1 dimension.
/*!
  <!--I put an anchor here because I cannot automatically reference this 
  class. -->
  \anchor SparseArray1

  \param T is the value type.  By default it is double.

  The free functions are grouped into the following categories.
  - \ref ads_array_ArrayContainerFunctions
  - \ref ads_array_SparseArrayFunctions
  - \ref ads_array_SparseArray1Functions

  Note that \c operator[] is container indexing.  \c a[i] is the i_th non-null
  element.  However, \c operator() is array indexing.  \c a(i) is the element
  with index \c i.
*/
template<typename T>
class SparseArray<1, T> : public ArrayContainer<T> {
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


  //! A const iterator over the indices.
  typedef Array<1,int>::const_iterator IndexConstIterator;

  //
  // Data.
  //

protected:

  //! The array indices.
  Array<1,int> _indices;
  //! The null value.
  value_type _null;

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

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.
  SparseArray() :
    Base(),
    _indices(),
    _null()
  {}

  //! Construct a 1-D array sparse array from the values and indices.
  /*!
    If the null value is not specified, the default value for the type is 
    used.  For built-in types, \c value_type() is equivalent to 
    \c value_type(0).
  */
  template<typename IndexForwardIter, typename ValueForwardIter>
  SparseArray(IndexForwardIter indicesBeginning, IndexForwardIter indicesEnd,
	      ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd,
	      parameter_type nullValue = value_type());

  //! Rebuild a 1-D sparse array from the values, the indices, and the null value.
  template<typename IndexForwardIter, typename ValueForwardIter>
  void
  rebuild(IndexForwardIter indicesBeginning, IndexForwardIter indicesEnd,
	  ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd,
	  parameter_type nullValue);

  //! Rebuild a 1-D sparse array from the values and indices.
  template<typename IndexForwardIter, typename ValueForwardIter>
  void
  rebuild(IndexForwardIter indicesBeginning, IndexForwardIter indicesEnd,
	  ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd) {
    rebuild(indicesBeginning, indicesEnd, valuesBeginning, valuesEnd, 
	    getNull());
  }

  //! Construct a 1-D sparse array from a 1-D dense array of possibly different value type.
  template<typename T2, bool A>
  SparseArray(const Array<1,T2,A>& array, parameter_type nullValue);

  //! Construct from the array size (number of non-null elements).
  /*!
    Leave the data uninitialized.
  */
  explicit
  SparseArray(const size_type size) :
    Base(size),
    _indices(size),
    _null()
  {}

  //! Rebuild from the array size (number of non-null elements).
  /*!
    Leave the data uninitialized.
  */
  void
  rebuild(const size_type size) {
    Base::rebuild(size);
    _indices.rebuild(size);
  }

  //! Copy constructor.  Deep copy.
  SparseArray(const SparseArray& other) :
    Base(other),
    _indices(other._indices),
    _null(other._null)
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
      _indices = other._indices;
      _null = other._null;
    }
    return *this;
  }

  //! Assignment operator for dense arrays.
  template<typename T2, bool A>
  SparseArray& 
  operator=(const Array<1,T2,A>& array);

  // @}
  //--------------------------------------------------------------------------
  //! \name Static members.
  // @{

  //! Return the rank (number of dimensions) of the array.
  static
  int
  getRank() {
    return 1;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the memory size.
  size_type 
  getMemoryUsage() const {
    return Base::getMemoryUsage() + _indices.getMemoryUsage();
  }

  //! Return the null value.
  parameter_type
  getNull() const {
    return _null;
  }

  //! Return true if the element is null.
  bool
  isNull(const int i) const;

  //! Return true if the element is non-null.
  bool
  isNonNull(const int i) const {
    return ! isNull(i);
  }

  //! Return the element with the specified index.
  parameter_type
  operator()(const int i) const;

  //! Return true if this sparse array is equal to the argument.
  bool
  operator==(const SparseArray& x) const {
    return (Base::operator==(x) && _indices == x._indices && _null == x._null);
  }

  //! Return true if this sparse array is not equal to the argument.
  bool
  operator!=(const SparseArray& x) const {
    return ! operator==(x);
  }

  //! Fill a dense array with the elements from this sparse array.
  template<typename T2, bool A>
  void
  fill(ads::Array<1,T2,A>* array) const;

  //! Fill a dense array using only the non-null elements from this sparse array.
  template<typename T2, bool A>
  void
  fillNonNull(ads::Array<1,T2,A>* array) const;

  //! Return the array of indices.
  const Array<1,int>&
  getIndices() const {
    return _indices;
  }

  //! Return the index of the specified element.
  int
  getIndex(const int n) const {
    return _indices[n];
  }

  //! Get a const iterator to the beginning of the indices.
  IndexConstIterator
  getIndicesBeginning() const {
    return _indices.begin();
  }

  //! Get a const iterator to the end of the indices.
  IndexConstIterator
  getIndicesEnd() const {
    return _indices.end();
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Swaps data with another SparseArray.
  void
  swap(SparseArray& other) {
    Base::swap(other);
    _indices.swap(other._indices);
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


//-----------------------------------------------------------------------------
/*! \defgroup ads_array_SparseArray1Functions Free functions for SparseArray<1, T>. */
//@{


//! Count the number of non-null elements in the union of the arrays.
template<typename T>
int
countNonNullElementsInUnion(const SparseArray<1, T>& a, 
			    const SparseArray<1, T>& b);


//! Compute the sum of the two arrays.
template<typename T>
void
computeSum(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
	   SparseArray<1, T>* result);


//! Compute the difference of the two arrays.
template<typename T>
void
computeDifference(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
		  SparseArray<1, T>* result);


//! Compute the product of the two arrays.
template<typename T>
void
computeProduct(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
	       SparseArray<1, T>* result);


//! Use the binary function to compute the result.
/*!
  Where one of the the arrays has a null value, the null value will be 
  an argument to the function.
*/
template<typename T, typename BinaryFunction>
void
computeBinaryOperation(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
		       SparseArray<1, T>* result, 
		       const BinaryFunction& function);


//---------------------------------------------------------------------------
// Operations with arrays and sparse arrays.
//---------------------------------------------------------------------------

//! += on the non-null elements.
template<typename T1, bool A, typename T2>
Array<1,T1,A>& 
operator+=(Array<1,T1,A>& x, const SparseArray<1,T2> & y);

//! -= on the non-null elements.
template<typename T1, bool A, typename T2>
Array<1,T1,A>& 
operator-=(Array<1,T1,A>& x, const SparseArray<1,T2> & y);

//! *= on the non-null elements.
template<typename T1, bool A, typename T2>
Array<1,T1,A>& 
operator*=(Array<1,T1,A>& x, const SparseArray<1,T2> & y);

//! /= on the non-null elements.
template<typename T1, bool A, typename T2>
Array<1,T1,A>& 
operator/=(Array<1,T1,A>& x, const SparseArray<1,T2> & y);

//! %= on the non-null elements.
template<typename T1, bool A, typename T2>
Array<1,T1,A>& 
operator%=(Array<1,T1,A>& x, const SparseArray<1,T2> & y);

//! Perform x += a * y on the non-null elements.
template<typename T1, bool A, typename T2, typename T3>
void
scaleAdd(Array<1,T1,A>* x, const T2 a, const SparseArray<1,T3> & y);



//---------------------------------------------------------------------------
// Operations with FixedArray's and sparse arrays.
//---------------------------------------------------------------------------

//! += on the non-null elements.
template<int _N, typename _T1, typename _T2>
FixedArray<_N, _T1>& 
operator+=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y);

//! -= on the non-null elements.
template<int _N, typename _T1, typename _T2>
FixedArray<_N, _T1>& 
operator-=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y);

//! *= on the non-null elements.
template<int _N, typename _T1, typename _T2>
FixedArray<_N, _T1>& 
operator*=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y);

//! /= on the non-null elements.
template<int _N, typename _T1, typename _T2>
FixedArray<_N, _T1>& 
operator/=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y);

//! %= on the non-null elements.
template<int _N, typename _T1, typename _T2>
FixedArray<_N, _T1>& 
operator%=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y);

//! Perform x += a * y on the non-null elements.
template<int _N, typename _T1, typename _T2, typename _T3>
void
scaleAdd(FixedArray<_N, _T1>* x, const _T2 a, const SparseArray<1, _T3>& y);

//@}

END_NAMESPACE_ADS

#define __ads_SparseArray1_ipp__
#include "SparseArray1.ipp"
#undef __ads_SparseArray1_ipp__
