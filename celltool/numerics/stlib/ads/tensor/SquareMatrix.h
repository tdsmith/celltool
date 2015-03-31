// -*- C++ -*-

/*! 
  \file ads/tensor/SquareMatrix.h
  \brief A square matrix class.
*/

#if !defined(__SquareMatrix_h__)
#define __SquareMatrix_h__

#include "TensorTypes.h"

#include "../array/FixedArray.h"

#include "../../third-party/loki/static_check.h"

#include <iosfwd>
#include <algorithm>
#include <functional>
#include <numeric>

#include <cassert>
#include <cmath>

#if defined(DEBUG_ads) && !defined(DEBUG_SquareMatrix)
#define DEBUG_SquareMatrix
#endif

BEGIN_NAMESPACE_ADS

//
// NxN matrices.
//

//! An NxN matrix.
/*!
  \param N: The matrix is NxN.
  \param T is the number type.  By default it is double.
*/
template<int N, typename T = double>
class SquareMatrix :
  public TensorTypes<T> {
private:

  //
  // Private types.
  //

  typedef TensorTypes<T> base_type;

public:

  //
  // public types
  //

  //! The element type of the tensor.
  typedef typename base_type::value_type value_type;
  //! A pointer to a tensor element.
  typedef typename base_type::pointer pointer;
  //! A const pointer to a tensor element.
  typedef typename base_type::const_pointer const_pointer;
  //! An iterator in the tensor.
  typedef typename base_type::iterator iterator;
  //! A const iterator in the tensor.
  typedef typename base_type::const_iterator const_iterator;
  //! A reference to a tensor element.
  typedef typename base_type::reference reference;
  //! A const reference to a tensor element.
  typedef typename base_type::const_reference const_reference;
  //! The size type.
  typedef typename base_type::size_type size_type;
  //! Pointer difference type.
  typedef typename base_type::difference_type difference_type;
  //! An index into the tensor.
  typedef typename base_type::index_type index_type;

protected:

  //
  // Data
  //

  //! The elements of the matrix.
  value_type _elem[N][N];

public:

  //
  // Constructors
  //

  //! Default constructor.  Leave the data uninitialized.
  SquareMatrix() {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  }

  //! Trivial destructor.
  ~SquareMatrix() {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  }

  //! Copy constructor.
  SquareMatrix(const SquareMatrix& x);

  //! Construct from a matrix with different number type.
  template<typename T2>
  SquareMatrix(const SquareMatrix<N,T2>& x);

  //! Construct from an array.
  SquareMatrix(const_pointer x);

  //! Initialize all the elements with the given value.
  SquareMatrix(const value_type x);

  //
  // Assignment operators
  //

  //! Assignment operator.
  SquareMatrix& 
  operator=(const SquareMatrix& x);

  //! Assignment operator.  Assign all elements the given value
  SquareMatrix& 
  operator=(const value_type x);

  //! Assignment operator for a SquareMatrix with different number type.
  template<typename T2> 
  SquareMatrix&
  operator=(const SquareMatrix<N,T2>& x);

  //
  // Accessors and Manipulators
  //

  //! Return an iterator to the beginning of the data.
  iterator
  begin() { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return &(_elem[0][0]);
  }

  //! Return an iterator to the end of the data.
  iterator 
  end() { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return begin() + N * N;
  }

  //! Return a const_iterator to the beginning of the data.
  const_iterator 
  begin() const { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return &(_elem[0][0]);
  }

  //! Return a const_iterator to the end of the data.
  const_iterator 
  end() const { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return begin() + N * N;
  }

  //! Return a pointer to the data.
  pointer
  data() { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return &(_elem[0][0]);
  }

  //! Return a const_pointer to the data.
  const_pointer
  data() const { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return &(_elem[0][0]);
  }

  //! Return the size of the tensor.
  size_type
  size() const { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
    return N * N;
  }

  //! Subscripting.  Return the i_th element.
  value_type
  operator()(const index_type i) const {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < size());
#endif
    return *(begin() + i);
  }

  //! Subscripting.  Return a reference to the i_th element.
  reference
  operator()(const index_type i) { 
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < size());
#endif
    return *(begin() + i);
  }

  //! Subscripting.  Return the i_th element.
  value_type 
  operator[](const index_type i) const {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < size());
#endif
    return *(begin() + i);
  }

  //! Subscripting.  Return a reference to the i_th element.
  value_type& 
  operator[](const index_type i) {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < size());
#endif
    return *(begin() + i);
  }

  //! Indexing.  Return element in the i_th row and j_th column.
  value_type
  operator()(const index_type i, const index_type j) const {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < N && 0 <= j && j < N);
#endif
    return _elem[i][j];
  }

  //! Indexing.  Return a reference to the element in the i_th row and j_th column.
  reference
  operator()(const index_type i, const index_type j) {
    LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < N && 0 <= j && j < N);
#endif
    return _elem[i][j];
  }

  //! Get the elements in row-major order.
  void
  get(pointer x) const;

  //! Set the elements in row-major order.
  void
  set(const_pointer x);

  //! Get the specified minor of the matrix.
  /*! 
    The matrix with the i_th row and the j_th column removed.
  */
  void
  getMinor(const int i, const int j, SquareMatrix<N-1,T>& minor) const;

  //! Negate each element.
  void
  negate();

  //! Transpose the matrix.
  void
  transpose();

  //
  // Assignment operators with scalar operand.
  //

  //! Add \c x to each element.
  SquareMatrix& 
  operator+=(const value_type x);

  //! Subtract \c x from each element.
  SquareMatrix& 
  operator-=(const value_type x);

  //! Multiply each element by \c x.
  SquareMatrix& 
  operator*=(const value_type x);

  //! Divide each element by \c x.
  SquareMatrix& 
  operator/=(const value_type x);

  //! Mod each element by \c x.
  SquareMatrix& 
  operator%=(const value_type x);

  //
  // Assignment operators with SquareMatrix operand
  //

  //! Element-wise addition.
  template<typename T2>
  SquareMatrix& 
  operator+=(const SquareMatrix<N,T2> & x);

  //! Element-wise subtraction.
  template<typename T2>
  SquareMatrix& 
  operator-=(const SquareMatrix<N,T2> & x);

  //! Matrix product.
  template<typename T2>
  SquareMatrix& 
  operator*=(const SquareMatrix<N,T2> & x);

  //
  // Unary operators
  //

  //! Unary positive operator.
  SquareMatrix
  operator+() {
    return *this;
  }
    
  //! Unary negate operator.
  SquareMatrix
  operator-();

};

//
// Binary operators
//
    
//! SquareMatrix-scalar addition.
template<typename T, int N>
inline
SquareMatrix<N,T>
operator+(const SquareMatrix<N,T>& m, const T x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(m);
  result += x;
  return result;
}

//! Scalar-SquareMatrix addition.
template<typename T, int N>
inline
SquareMatrix<N,T>
operator+(const T x, const SquareMatrix<N,T>& m) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return m + x;
}

//! SquareMatrix-SquareMatrix addition.
template<typename T, int N>
inline
SquareMatrix<N,T>
operator+(const SquareMatrix<N,T>& x, const SquareMatrix<N,T>& y) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(x);
  result += y;
  return result;
}

//! SquareMatrix-scalar subtraction.
template<typename T, int N>
inline
SquareMatrix<N,T>
operator-(const SquareMatrix<N,T>& m, const T x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(m);
  result -= x;
  return result;
}

//! Scalar-SquareMatrix subtraction.
template<typename T, int N>
inline
SquareMatrix<N,T>
operator-(const T x, const SquareMatrix<N,T>& m) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(x);
  result -= m;
  return result;
}

//! SquareMatrix-SquareMatrix subtraction.
template<typename T, int N>
inline
SquareMatrix<N,T>
operator-(const SquareMatrix<N,T>& x, const SquareMatrix<N,T>& y) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(x);
  result -= y;
  return result;
}

//! SquareMatrix-scalar product.
template<int N, typename T>
inline
SquareMatrix<N,T>
operator*(const SquareMatrix<N,T>& m, const T x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(m);
  result *= x;
  return result;
}

//! Scalar-SquareMatrix product.
template<int N, typename T>
inline
SquareMatrix<N,T>
operator*(const T x, const SquareMatrix<N,T>& m) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return m * x;
}

//! SquareMatrix-SquareMatrix product.
template<int N, typename T>
SquareMatrix<N,T>
operator*(const SquareMatrix<N,T>& x, const SquareMatrix<N,T>& y);

//! SquareMatrix-scalar division.
template<int N, typename T>
inline
SquareMatrix<N,T>
operator/(const SquareMatrix<N,T>& m, const T x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  SquareMatrix<N,T> result(m);
  result /= x;
  return result;
}

//! Scalar-SquareMatrix division.
template<int N, typename T>
SquareMatrix<N,T>
operator/(const T x, const SquareMatrix<N,T>& m);

//
// Math operators.
//

//! Return the sum of the elements.
template<int N, typename T>
inline
T
computeSum(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return std::accumulate(x.begin(), x.end(), T(0));
}
    
//! Return the product of the elements.
template<int N, typename T>
inline
T
computeProduct(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return std::accumulate(x.begin(), x.end(), T(1), std::multiplies<T>());
}

//! Return the minimum element.  Use < for comparison.
template<int N, typename T>
inline
T
computeMinimum(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return *std::min_element(x.begin(), x.end());
}
    
//! Return the maximum element.  Use > for comparison.
template<int N, typename T>
inline
T
computeMaximum(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return *std::max_element(x.begin(), x.end());
}

//! Return the determinant of the matrix.
template<int N, typename T>
T
computeDeterminant(const SquareMatrix<N,T>& x);

//! Return the trace of the matrix.
template<int N, typename T>
T
computeTrace(const SquareMatrix<N,T>& x);

//! Return the transpose of the matrix.
template<int N, typename T>
SquareMatrix<N,T>
computeTranspose(const SquareMatrix<N,T>& x);

//! Return the inverse of the matrix.
template<int N, typename T>
SquareMatrix<N,T>
computeInverse(const SquareMatrix<N,T>& x);

//! Compute the inverse of the matrix.
template<int N, typename T>
void
computeInverse(const SquareMatrix<N,T>& x, SquareMatrix<N,T>* y);

//! Calculate the scaled inverse of the matrix: determinant * inverse.
template<int N, typename T>
void
computeScaledInverse(const SquareMatrix<N,T>& x, SquareMatrix<N,T>* si);

//! Return the scaled inverse of the matrix: determinant * inverse.
template<int N, typename T>
SquareMatrix<N,T>
computeScaledInverse(const SquareMatrix<N,T>& x);

//! Return the frobenius norm of the matrix.
template<int N, typename T>
T
computeFrobeniusNormSquared(const SquareMatrix<N,T>& x);

//! Return the frobenius norm of the matrix.
template<int N, typename T>
inline
T
computeFrobeniusNorm(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return std::sqrt(computeFrobeniusNormSquared(x));
}

//! Return the inner product of the matrices.  computeTrace(transpose(x) * y)
template<int N, typename T>
inline
T
computeInnerProduct(const SquareMatrix<N,T>& x, const SquareMatrix<N,T>& y) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  T result = 0;
  const typename SquareMatrix<N,T>::const_iterator finish = x.end();
  for (typename SquareMatrix<N,T>::const_iterator i = x.begin(), 
	 j = y.begin(); i != finish; ++i, ++j) {
    result += *i * *j;
  }
  return result;
}


//! Compute the matrix-vector product. <tt>x = m * v</tt>.
template<int N, typename T>
inline
void
computeProduct(const SquareMatrix<N,T>& m, const FixedArray<N,T>& v, 
	       FixedArray<N,T>* x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  *x = 0;
  typename SquareMatrix<N,T>::const_iterator mi = x->begin();
  typename FixedArray<N,T>::const_iterator vi;
  const typename FixedArray<N,T>::const_iterator vi_end = v.end();
  // Loop over the rows.
  for (int m = 0; m != N; ++m) {
    // Loop over the columns.
    for (vi = v.begin(); vi != vi_end; ++vi, ++mi) {
      (*x)[m] += *mi * *vi;
    }
  }
}


//
// Equality
//

//! Return true if the matrices are equal.
template<int N, typename T1, typename T2>
inline
bool
operator==(const SquareMatrix<N,T1>& a, const SquareMatrix<N,T2>& b) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return std::equal(a.begin(), a.end(), b.begin());
}

//! Return true if the tensors are not equal.
template<int N, typename T1, typename T2>
inline
bool
operator!=(const SquareMatrix<N,T1>& a, const SquareMatrix<N,T2>& b) { 
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  return !(a == b); 
}

//
// I/O
//
    
//! Write a matrix as rows with space-separated numbers.
template<int N, typename T>
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<N,T>& x);
    
//! Read white space-separated numbers into a matrix.
template<int N, typename T>
std::istream&
operator>>(std::istream& in, SquareMatrix<N,T>& x);




//----------------------------------------------------------------------------
// 1x1 matrices.
//----------------------------------------------------------------------------

//! A 1x1 matrix.
/*!
  \param T is the number type.  By default it is double.
*/
template<typename T>
class SquareMatrix<1,T> :
  public TensorTypes<T> {
private:

  //
  // private types
  //

  typedef TensorTypes<T> base_type;

public:

  //
  // public types
  //

  //! The element type of the tensor.
  typedef typename base_type::value_type value_type;
  //! A pointer to a tensor element.
  typedef typename base_type::pointer pointer;
  //! A const pointer to a tensor element.
  typedef typename base_type::const_pointer const_pointer;
  //! An iterator in the tensor.
  typedef typename base_type::iterator iterator;
  //! A const iterator in the tensor.
  typedef typename base_type::const_iterator const_iterator;
  //! A reference to a tensor element.
  typedef typename base_type::reference reference;
  //! A const reference to a tensor element.
  typedef typename base_type::const_reference const_reference;
  //! The size type.
  typedef typename base_type::size_type size_type;
  //! Pointer difference type.
  typedef typename base_type::difference_type difference_type;
  //! An index into the tensor.
  typedef typename base_type::index_type index_type;

protected:

  //
  // Data
  //

  //! The element of the matrix.
  value_type _elem;

public:

  //
  // Constructors
  //

  //! Default constructor.  Leave the data uninitialized.
  SquareMatrix()
  {}

  //! Trivial destructor.
  ~SquareMatrix() 
  {}

  //! Copy constructor.
  SquareMatrix(const SquareMatrix& x);

  //! Construct from a matrix with different number type.
  template<typename T2>
  SquareMatrix(const SquareMatrix<1,T2>& x);

  //! Construct from the matrix element.
  SquareMatrix(const value_type e);

  //! Construct from an array.
  SquareMatrix(const_pointer x);

  //
  // Assignment operators
  //

  //! Assignment operator.
  SquareMatrix& 
  operator=(const SquareMatrix& x);

  //! Assignment operator.  Assign all elements the given value
  SquareMatrix& 
  operator=(const value_type x);

  //! Assignment operator for a SquareMatrix with different number type.
  template<typename T2> 
  SquareMatrix&
  operator=(const SquareMatrix<1,T2>& x);

  //
  // Accessors and Manipulators
  //

  //! Return an iterator to the beginning of the data.
  iterator
  begin() { 
    return &_elem; 
  }

  //! Return an iterator to the end of the data.
  iterator 
  end() { 
    return &_elem + 1;
  }

  //! Return a const_iterator to the beginning of the data.
  const_iterator 
  begin() const { 
    return &_elem;
  }

  //! Return a const_iterator to the end of the data.
  const_iterator 
  end() const { 
    return &_elem + 1;
  }

  //! Return a pointer to the data.
  pointer
  data() { 
    return &_elem;
  }

  //! Return a const_pointer to the data.
  const_pointer
  data() const { 
    return &_elem; 
  }

  //! Return the size of the tensor.
  size_type
  size() const { 
    return 1;
  }

  //! Subscripting.  Return the i_th element.
  value_type
  operator()(const index_type i) const {
#ifdef DEBUG_SquareMatrix
    assert(i == 0);
#endif
    return _elem;
  }

  //! Subscripting.  Return a reference to the i_th element.
  reference
  operator()(const index_type i) { 
#ifdef DEBUG_SquareMatrix
    assert(i == 0);
#endif
    return _elem;
  }

  //! Subscripting.  Return the i_th element.
  value_type 
  operator[](const index_type i) const {
#ifdef DEBUG_SquareMatrix
    assert(i == 0);
#endif
    return _elem;
  }

  //! Subscripting.  Return a reference to the i_th element.
  value_type& 
  operator[](const index_type i) {
#ifdef DEBUG_SquareMatrix
    assert(i == 0);
#endif
    return _elem;
  }

  //! Indexing.  Return element in the i_th row and j_th column.
  value_type
  operator()(const index_type i, const index_type j) const {
#ifdef DEBUG_SquareMatrix
    assert(i == 0 && j == 0);
#endif
    return _elem;
  }

  //! Indexing.  Return a reference to the element in the i_th row and j_th column.
  reference
  operator()(const index_type i, const index_type j) {
#ifdef DEBUG_SquareMatrix
    assert(i == 0 && j == 0);
#endif
    return _elem;
  }

  //! Get the elements in row-major order.
  void
  get(pointer x) const;

  //! Set the elements in row-major order.
  void
  set(const_pointer x);

  //! Get the elements in row-major order.
  void
  get(reference e) const;

  //! Set the elements in row-major order.
  void
  set(const value_type e);

  //! Negate each element.
  void
  negate();

  //! Transpose the matrix.
  void
  transpose();

  //
  // Assignment operators with scalar operand.
  //

  //! Add \c x to each element.
  SquareMatrix& 
  operator+=(const value_type x);

  //! Subtract \c x from each element.
  SquareMatrix& 
  operator-=(const value_type x);

  //! Multiply each element by \c x.
  SquareMatrix& 
  operator*=(const value_type x);

  //! Divide each element by \c x.
  SquareMatrix& 
  operator/=(const value_type x);

  //! Mod each element by \c x.
  SquareMatrix& 
  operator%=(const value_type x);

  //
  // Assignment operators with SquareMatrix operand
  //

  //! Element-wise addition.
  template<typename T2>
  SquareMatrix& 
  operator+=(const SquareMatrix<1,T2>& x);

  //! Element-wise subtraction.
  template<typename T2>
  SquareMatrix& 
  operator-=(const SquareMatrix<1,T2>& x);

  //! Matrix product.
  template<typename T2>
  SquareMatrix& 
  operator*=(const SquareMatrix<1,T2>& x);

  //
  // Unary operators
  //

  //! Unary positive operator.
  SquareMatrix
  operator+() {
    return *this;
  }
    
  //! Unary negate operator.
  SquareMatrix
  operator-();

};

//
// Binary operators
//
    
//! SquareMatrix-scalar addition.
template<typename T>
inline
SquareMatrix<1,T>
operator+(const SquareMatrix<1,T>& m, const T x) {
  return SquareMatrix<1,T>(m[0] + x);
}

//! Scalar-SquareMatrix addition.
template<typename T>
inline
SquareMatrix<1,T>
operator+(const T x, const SquareMatrix<1,T>& m) {
  return m + x;
}

//! SquareMatrix-SquareMatrix addition.
template<typename T>
inline
SquareMatrix<1,T>
operator+(const SquareMatrix<1,T>& x, const SquareMatrix<1,T>& y) {
  return SquareMatrix<1,T>(x[0] + y[0]);
}

//! SquareMatrix-scalar subtraction.
template<typename T>
inline
SquareMatrix<1,T>
operator-(const SquareMatrix<1,T>& m, const T x) {
  return SquareMatrix<1,T>(m[0] - x);
}

//! Scalar-SquareMatrix subtraction.
template<typename T>
inline
SquareMatrix<1,T>
operator-(const T x, const SquareMatrix<1,T>& m) {
  return SquareMatrix<1,T>(x - m[0]);
}

//! SquareMatrix-SquareMatrix subtraction.
template<typename T>
inline
SquareMatrix<1,T>
operator-(const SquareMatrix<1,T>& x, const SquareMatrix<1,T>& y) {
  return SquareMatrix<1,T>(x[0] - y[0]);
}

//! SquareMatrix-scalar product.
template<typename T>
inline
SquareMatrix<1,T>
operator*(const SquareMatrix<1,T>& m, const T x) {
  return SquareMatrix<1,T>(m[0] * x);
}

//! Scalar-SquareMatrix product.
template<typename T>
inline
SquareMatrix<1,T>
operator*(const T x, const SquareMatrix<1,T>& m) {
  return m * x;
}

//! SquareMatrix-SquareMatrix product.
template<typename T>
inline
SquareMatrix<1,T>
operator*(const SquareMatrix<1,T>& x, const SquareMatrix<1,T>& y) {
  return SquareMatrix<1,T>(x[0]*y[0]);
}

//! SquareMatrix-scalar division.
template<typename T>
inline
SquareMatrix<1,T>
operator/(const SquareMatrix<1,T>& m, const T x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  return SquareMatrix<1,T>(m[0] / x);
}

//! Scalar-SquareMatrix division.
template<typename T>
inline
SquareMatrix<1,T>
operator/(const T x, const SquareMatrix<1,T>& m) {
#ifdef DEBUG_SquareMatrix
  assert(m[0] != 0);
#endif
  return SquareMatrix<1,T>(x / m[0]);
}

//
// Math operators.
//

//! Return the sum of the elements.
template<typename T>
inline
T
computeSum(const SquareMatrix<1,T>& x) {
  return (x[0]);
}
    
//! Return the product of the elements.
template<typename T>
inline
T
computeProduct(const SquareMatrix<1,T>& x) {
  return (x[0]);
}

//! Return the minimum element.  Use < for comparison.
template<typename T>
inline
T
computeMinimum(const SquareMatrix<1,T>& x) {
  return x[0];
}
    
//! Return the maximum element.  Use > for comparison.
template<typename T>
inline
T
computeMaximum(const SquareMatrix<1,T>& x) {
  return x[0];
}

//! Return the determinant of the matrix.
template<typename T>
inline
T
computeDeterminant(const SquareMatrix<1,T>& x) {
  return x[0];
}

//! Return the trace of the matrix.
template<typename T>
inline
T
computeTrace(const SquareMatrix<1,T>& x) {
  return x[0];
}

//! Return the transpose of the matrix.
template<typename T>
inline
SquareMatrix<1,T>
computeTranspose(const SquareMatrix<1,T>& x) {
  return x;
}

//! Return the inverse of the matrix.
template<typename T>
inline
SquareMatrix<1,T>
computeInverse(const SquareMatrix<1,T>& x) {
  SquareMatrix<1,T> y;
  computeInverse(x, &y);
  return y;
}

//! Compute the inverse of the matrix.
template<typename T>
inline
void
computeInverse(const SquareMatrix<1,T>& x, SquareMatrix<1,T>* y) {
#ifdef DEBUG_SquareMatrix
  assert(x[0] != 0);
#endif
  y[0] = 1 / x[0];
}

//! Calculate the scaled inverse of the matrix: determinant * inverse.
template<typename T>
inline
void
computeScaledInverse(const SquareMatrix<1,T>& x, SquareMatrix<1,T>* si) {
  *si = x;
}

//! Return the scaled inverse of the matrix: determinant * inverse.
template<typename T>
inline
SquareMatrix<1,T>
computeScaledInverse(const SquareMatrix<1,T>& x) {
  return SquareMatrix<1,T>(1);
}

//! Return the frobenius norm of the matrix.
template<typename T>
inline
T
computeFrobeniusNorm(const SquareMatrix<1,T>& x) {
  return std::abs(x[0]);
}

//! Return the frobenius norm of the matrix.
template<typename T>
inline
T
computeFrobeniusNormSquared(const SquareMatrix<1,T>& x) {
  return (x[0]*x[0]);
}

//! Return the inner product of the matrices.  computeTrace(transpose(x) * y)
template<typename T>
inline
T
computeInnerProduct(const SquareMatrix<1,T>& x, const SquareMatrix<1,T>& y) {
  return x * y;
}

//! Compute the matrix-vector product. <tt>x = m * v</tt>.
template<typename T>
inline
void
computeProduct(const SquareMatrix<1,T>& m, const FixedArray<1,T>& v, 
		FixedArray<1,T>* x) {
  (*x)[0] = m[0] * v[0];
}

//
// Equality
//

//! Return true if the matrices are equal.
template<typename T1, typename T2>
inline
bool
operator==(const SquareMatrix<1,T1>& a, const SquareMatrix<1,T2>& b) {
  return (a[0] == b[0]);
}

//! Return true if the tensors are not equal.
template<typename T1, typename T2>
inline
bool
operator!=(const SquareMatrix<1,T1>& a, const SquareMatrix<1,T2>& b) { 
  return !(a == b); 
}

//
// I/O
//
    
//! Write a matrix as rows with space-separated numbers.
template<typename T>
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<1,T>& x);
    
//! Read white space-separated numbers into a matrix.
template<typename T>
std::istream&
operator>>(std::istream& in, SquareMatrix<1,T>& x);



//----------------------------------------------------------------------------
// 2x2 matrices.
//----------------------------------------------------------------------------

//! A 2x2 matrix.
/*!
  \param T is the number type.  By default it is double.
*/
template<typename T>
class SquareMatrix<2,T> :
  public TensorTypes<T> {
private:

  //
  // private types
  //

  typedef TensorTypes<T> base_type;

public:

  //
  // public types
  //

  //! The element type of the tensor.
  typedef typename base_type::value_type value_type;
  //! A pointer to a tensor element.
  typedef typename base_type::pointer pointer;
  //! A const pointer to a tensor element.
  typedef typename base_type::const_pointer const_pointer;
  //! An iterator in the tensor.
  typedef typename base_type::iterator iterator;
  //! A const iterator in the tensor.
  typedef typename base_type::const_iterator const_iterator;
  //! A reference to a tensor element.
  typedef typename base_type::reference reference;
  //! A const reference to a tensor element.
  typedef typename base_type::const_reference const_reference;
  //! The size type.
  typedef typename base_type::size_type size_type;
  //! Pointer difference type.
  typedef typename base_type::difference_type difference_type;
  //! An index into the tensor.
  typedef typename base_type::index_type index_type;

protected:

  //
  // Data
  //

  //! The elements of the matrix.
  value_type _elem[4];

public:

  //
  // Constructors
  //

  //! Default constructor.  Leave the data uninitialized.
  SquareMatrix()
  {}

  //! Trivial destructor.
  ~SquareMatrix() 
  {}

  //! Copy constructor.
  SquareMatrix(const SquareMatrix& x);

  //! Construct from a matrix with different number type.
  template<typename T2>
  SquareMatrix(const SquareMatrix<2,T2>& x);

  //! Construct from the matrix elements.
  SquareMatrix(const value_type e00, const value_type e01,
	       const value_type e10, const value_type e11);

  //! Construct from an array.
  SquareMatrix(const_pointer x);

  //! Initialize all the elements with the given value.
  SquareMatrix(const value_type x);

  //
  // Assignment operators
  //

  //! Assignment operator.
  SquareMatrix& 
  operator=(const SquareMatrix& x);

  //! Assignment operator.  Assign all elements the given value
  SquareMatrix& 
  operator=(const value_type x);

  //! Assignment operator for a SquareMatrix with different number type.
  template<typename T2> 
  SquareMatrix&
  operator=(const SquareMatrix<2,T2>& x);

  //
  // Accessors and Manipulators
  //

  //! Return an iterator to the beginning of the data.
  iterator
  begin() { 
    return _elem; 
  }

  //! Return an iterator to the end of the data.
  iterator 
  end() { 
    return _elem + 4;
  }

  //! Return a const_iterator to the beginning of the data.
  const_iterator 
  begin() const { 
    return _elem;
  }

  //! Return a const_iterator to the end of the data.
  const_iterator 
  end() const { 
    return _elem + 4;
  }

  //! Return a pointer to the data.
  pointer
  data() { 
    return _elem;
  }

  //! Return a const_pointer to the data.
  const_pointer
  data() const { 
    return _elem; 
  }

  //! Return the size of the tensor.
  size_type
  size() const { 
    return 4;
  }

  //! Subscripting.  Return the i_th element.
  value_type
  operator()(const index_type i) const {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 4);
#endif
    return _elem[i];
  }

  //! Subscripting.  Return a reference to the i_th element.
  reference
  operator()(const index_type i) { 
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 4);
#endif
    return _elem[i];
  }

  //! Subscripting.  Return the i_th element.
  value_type 
  operator[](const index_type i) const {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 4);
#endif
    return _elem[i];
  }

  //! Subscripting.  Return a reference to the i_th element.
  value_type& 
  operator[](const index_type i) {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 4);
#endif
    return _elem[i];
  }

  //! Indexing.  Return element in the i_th row and j_th column.
  value_type
  operator()(const index_type i, const index_type j) const {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 2 && 0 <= j && j < 2);
#endif
    return _elem[i * 2 + j];
  }

  //! Indexing.  Return a reference to the element in the i_th row and j_th column.
  reference
  operator()(const index_type i, const index_type j) {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 2 && 0 <= j && j < 2);
#endif
    return _elem[i * 2 + j];
  }

  //! Get the elements in row-major order.
  void
  get(pointer x) const;

  //! Set the elements in row-major order.
  void
  set(const_pointer x);

  //! Get the elements in row-major order.
  void
  get(reference e00, reference e01,
      reference e10, reference e11) const;

  //! Set the elements in row-major order.
  void
  set(const value_type e00, const value_type e01,
      const value_type e10, const value_type e11);

  //! Negate each element.
  void
  negate();

  //! Transpose the matrix.
  void
  transpose();

  //
  // Assignment operators with scalar operand.
  //

  //! Add \c x to each element.
  SquareMatrix& 
  operator+=(const value_type x);

  //! Subtract \c x from each element.
  SquareMatrix& 
  operator-=(const value_type x);

  //! Multiply each element by \c x.
  SquareMatrix& 
  operator*=(const value_type x);

  //! Divide each element by \c x.
  SquareMatrix& 
  operator/=(const value_type x);

  //! Mod each element by \c x.
  SquareMatrix& 
  operator%=(const value_type x);

  //
  // Assignment operators with SquareMatrix operand
  //

  //! Element-wise addition.
  template<typename T2>
  SquareMatrix& 
  operator+=(const SquareMatrix<2,T2>& x);

  //! Element-wise subtraction.
  template<typename T2>
  SquareMatrix& 
  operator-=(const SquareMatrix<2,T2>& x);

  //! Matrix product.
  template<typename T2>
  SquareMatrix& 
  operator*=(const SquareMatrix<2,T2>& x);

  //
  // Unary operators
  //

  //! Unary positive operator.
  SquareMatrix
  operator+() {
    return *this;
  }
    
  //! Unary negate operator.
  SquareMatrix
  operator-();

};

//
// Binary operators
//
    
//! SquareMatrix-scalar addition.
template<typename T>
inline
SquareMatrix<2,T>
operator+(const SquareMatrix<2,T>& m, const T x) {
  return SquareMatrix<2,T>(m[0] + x, m[1] + x, 
			   m[2] + x, m[3] + x);
}

//! Scalar-SquareMatrix addition.
template<typename T>
inline
SquareMatrix<2,T>
operator+(const T x, const SquareMatrix<2,T>& m) {
  return m + x;
}

//! SquareMatrix-SquareMatrix addition.
template<typename T>
inline
SquareMatrix<2,T>
operator+(const SquareMatrix<2,T>& x, const SquareMatrix<2,T>& y) {
  return SquareMatrix<2,T>(x[0] + y[0], x[1] + y[1], 
			   x[2] + y[2], x[3] + y[3]);
}

//! SquareMatrix-scalar subtraction.
template<typename T>
inline
SquareMatrix<2,T>
operator-(const SquareMatrix<2,T>& m, const T x) {
  return SquareMatrix<2,T>(m[0] - x, m[1] - x, 
			   m[2] - x, m[3] - x);
}

//! Scalar-SquareMatrix subtraction.
template<typename T>
inline
SquareMatrix<2,T>
operator-(const T x, const SquareMatrix<2,T>& m) {
  return SquareMatrix<2,T>(x - m[0], x - m[1], 
			   x - m[2], x - m[3]);
}

//! SquareMatrix-SquareMatrix subtraction.
template<typename T>
inline
SquareMatrix<2,T>
operator-(const SquareMatrix<2,T>& x, const SquareMatrix<2,T>& y) {
  return SquareMatrix<2,T>(x[0] - y[0], x[1] - y[1], 
			   x[2] - y[2], x[3] - y[3]);
}

//! SquareMatrix-scalar product.
template<typename T>
inline
SquareMatrix<2,T>
operator*(const SquareMatrix<2,T>& m, const T x) {
  return SquareMatrix<2,T>(m[0] * x, m[1] * x, 
			   m[2] * x, m[3] * x);
}

//! Scalar-SquareMatrix product.
template<typename T>
inline
SquareMatrix<2,T>
operator*(const T x, const SquareMatrix<2,T>& m) {
  return m * x;
}

//! SquareMatrix-SquareMatrix product.
template<typename T>
inline
SquareMatrix<2,T>
operator*(const SquareMatrix<2,T>& x, const SquareMatrix<2,T>& y) {
  return SquareMatrix<2,T>(x[0]*y[0]+x[1]*y[2],
			   x[0]*y[1]+x[1]*y[3],
			   x[2]*y[0]+x[3]*y[2],
			   x[2]*y[1]+x[3]*y[3]);
}

//! SquareMatrix-scalar division.
template<typename T>
inline
SquareMatrix<2,T>
operator/(const SquareMatrix<2,T>& m, const T x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  return SquareMatrix<2,T>(m[0] / x, m[1] / x, 
			   m[2] / x, m[3] / x);
}

//! Scalar-SquareMatrix division.
template<typename T>
inline
SquareMatrix<2,T>
operator/(const T x, const SquareMatrix<2,T>& m) {
#ifdef DEBUG_SquareMatrix
  assert(m[0] != 0 && m[1] != 0 && 
	 m[2] != 0 && m[3] != 0);
#endif
  return SquareMatrix<2,T>(x / m[0], x / m[1], 
			   x / m[2], x / m[3]);
}

//
// Math operators.
//

//! Return the sum of the elements.
template<typename T>
inline
T
computeSum(const SquareMatrix<2,T>& x) {
  return (x[0] + x[1] + 
	  x[2] + x[3]);
}
    
//! Return the product of the elements.
template<typename T>
inline
T
computeProduct(const SquareMatrix<2,T>& x) {
  return (x[0] * x[1] * 
	  x[2] * x[3]);
}

//! Return the minimum element.  Use < for comparison.
template<typename T>
inline
T
computeMinimum(const SquareMatrix<2,T>& x) {
  return *std::min_element(x.begin(), x.end());
}
    
//! Return the maximum element.  Use > for comparison.
template<typename T>
inline
T
computeMaximum(const SquareMatrix<2,T>& x) {
  return *std::max_element(x.begin(), x.end());
}

//! Return the determinant of the matrix.
template<typename T>
inline
T
computeDeterminant(const SquareMatrix<2,T>& x) {
  return (x[0] * x[3] - x[1] * x[2]);
}

//! Return the trace of the matrix.
template<typename T>
inline
T
computeTrace(const SquareMatrix<2,T>& x) {
  return (x[0] + x[3]);
}

//! Return the transpose of the matrix.
template<typename T>
inline
SquareMatrix<2,T>
computeTranspose(const SquareMatrix<2,T>& x) {
  return SquareMatrix<2,T>(x[0], x[2],
			   x[1], x[3]);
}

//! Return the inverse of the matrix.
template<typename T>
inline
SquareMatrix<2,T>
computeInverse(const SquareMatrix<2,T>& x) {
  SquareMatrix<2,T> y;
  computeInverse(x, &y);
  return y;
}

//! Return the inverse of the matrix given the matrix and its determinant.
template<typename T>
inline
SquareMatrix<2,T>
computeInverse(const SquareMatrix<2,T>& x, const T det) {
  SquareMatrix<2,T> y;
  computeInverse(x, det, &y);
  return y;
}

//! Compute the inverse of the matrix.
template<typename T>
inline
void
computeInverse(const SquareMatrix<2,T>& x, SquareMatrix<2,T>* y) {
  const T det = computeDeterminant(x);
  computeInverse(x, det, y);
}

//! Compute the inverse of the matrix given its determinant.
template<typename T>
inline
void
computeInverse(const SquareMatrix<2,T>& x, const T det, SquareMatrix<2,T>* y) {
#ifdef DEBUG_SquareMatrix
  assert(det != 0);
#endif
  y->set(x[3] / det, - x[1] / det,
	 - x[2] / det, x[0] / det);
}

//! Calculate the scaled inverse of the matrix: determinant * inverse.
template<typename T>
inline
void
computeScaledInverse(const SquareMatrix<2,T>& x, SquareMatrix<2,T>* si) {
  (*si)[0] = x[3];  (*si)[1] = -x[1];
  (*si)[2] = -x[2]; (*si)[3] = x[0];
}

//! Return the scaled inverse of the matrix: determinant * inverse.
template<typename T>
inline
SquareMatrix<2,T>
computeScaledInverse(const SquareMatrix<2,T>& x) {
  return SquareMatrix<2,T>(x[3], - x[1],
			   - x[2], x[0]);
}

//! Return the frobenius norm of the matrix.
template<typename T>
inline
T
computeFrobeniusNorm(const SquareMatrix<2,T>& x) {
  return std::sqrt(computeFrobeniusNormSquared(x));
}

//! Return the frobenius norm of the matrix.
template<typename T>
inline
T
computeFrobeniusNormSquared(const SquareMatrix<2,T>& x) {
  return (x[0]*x[0] + x[1]*x[1] + 
	  x[2]*x[2] + x[3]*x[3]);
}

//! Return the inner product of the matrices.  computeTrace(transpose(x) * y)
template<typename T>
inline
T
computeInnerProduct(const SquareMatrix<2,T>& x, const SquareMatrix<2,T>& y) {
  return x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3];
}

//! Compute the matrix-vector product. <tt>x = m * v</tt>.
template<typename T>
inline
void
computeProduct(const SquareMatrix<2,T>& m, const FixedArray<2,T>& v, 
		FixedArray<2,T>* x) {
  (*x)[0] = m[0] * v[0] + m[1] * v[1];
  (*x)[1] = m[2] * v[0] + m[3] * v[1];
}

//
// Equality
//

//! Return true if the matrices are equal.
template<typename T1, typename T2>
inline
bool
operator==(const SquareMatrix<2,T1>& a, const SquareMatrix<2,T2>& b) {
  return (a[0] == b[0] && a[1] == b[1] && 
	  a[2] == b[2] && a[3] == b[3]);
}

//! Return true if the tensors are not equal.
template<typename T1, typename T2>
inline
bool
operator!=(const SquareMatrix<2,T1>& a, const SquareMatrix<2,T2>& b) { 
  return !(a == b); 
}

//
// I/O
//
    
//! Write a matrix as rows with space-separated numbers.
template<typename T>
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<2,T>& x);
    
//! Read white space-separated numbers into a matrix.
template<typename T>
std::istream&
operator>>(std::istream& in, SquareMatrix<2,T>& x);



//----------------------------------------------------------------------------
// 3x3 matrices.
//----------------------------------------------------------------------------

//! A 3x3 matrix.
/*!
  \param T is the number type.  By default it is double.
*/
template<typename T>
class SquareMatrix<3,T> :
  public TensorTypes<T> {
private:

  //
  // private types
  //

  typedef TensorTypes<T> base_type;

public:

  //
  // public types
  //

  //! The element type of the tensor.
  typedef typename base_type::value_type value_type;
  //! A pointer to a tensor element.
  typedef typename base_type::pointer pointer;
  //! A const pointer to a tensor element.
  typedef typename base_type::const_pointer const_pointer;
  //! An iterator in the tensor.
  typedef typename base_type::iterator iterator;
  //! A const iterator in the tensor.
  typedef typename base_type::const_iterator const_iterator;
  //! A reference to a tensor element.
  typedef typename base_type::reference reference;
  //! A const reference to a tensor element.
  typedef typename base_type::const_reference const_reference;
  //! The size type.
  typedef typename base_type::size_type size_type;
  //! Pointer difference type.
  typedef typename base_type::difference_type difference_type;
  //! An index into the tensor.
  typedef typename base_type::index_type index_type;

protected:

  //
  // Data
  //

  //! The elements of the matrix.
  value_type _elem[9];

public:

  //
  // Constructors
  //

  //! Default constructor.  Leave the data uninitialized.
  SquareMatrix()
  {}

  //! Trivial destructor.
  ~SquareMatrix() 
  {}

  //! Copy constructor.
  SquareMatrix(const SquareMatrix& x);

  //! Construct from a matrix with different number type.
  template<typename T2>
  SquareMatrix(const SquareMatrix<3,T2>& x);

  //! Construct from the matrix elements.
  SquareMatrix(const value_type e00, const value_type e01, const value_type e02, 
	       const value_type e10, const value_type e11, const value_type e12, 
	       const value_type e20, const value_type e21, const value_type e22);

  //! Construct from an array.
  SquareMatrix(const_pointer x);

  //! Initialize all the elements with the given value.
  SquareMatrix(const value_type x);

  //
  // Assignment operators
  //

  //! Assignment operator.
  SquareMatrix& 
  operator=(const SquareMatrix& x);

  //! Assignment operator.  Assign all elements the given value
  SquareMatrix& 
  operator=(const value_type x);

  //! Assignment operator for a SquareMatrix with different number type.
  template<typename T2> 
  SquareMatrix&
  operator=(const SquareMatrix<3,T2>& x);

  //
  // Accessors and Manipulators
  //

  //! Return an iterator to the beginning of the data.
  iterator
  begin() { 
    return _elem; 
  }

  //! Return an iterator to the end of the data.
  iterator 
  end() { 
    return _elem + 9;
  }

  //! Return a const_iterator to the beginning of the data.
  const_iterator 
  begin() const { 
    return _elem;
  }

  //! Return a const_iterator to the end of the data.
  const_iterator 
  end() const { 
    return _elem + 9;
  }

  //! Return a pointer to the data.
  pointer
  data() { 
    return _elem;
  }

  //! Return a const_pointer to the data.
  const_pointer
  data() const { 
    return _elem; 
  }

  //! Return the size of the tensor.
  size_type
  size() const { 
    return 9;
  }

  //! Subscripting.  Return the i_th element.
  value_type
  operator()(const index_type i) const {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 9);
#endif
    return _elem[i];
  }

  //! Subscripting.  Return a reference to the i_th element.
  reference
  operator()(const index_type i) { 
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 9);
#endif
    return _elem[i];
  }

  //! Subscripting.  Return the i_th element.
  value_type 
  operator[](const index_type i) const {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 9);
#endif
    return _elem[i];
  }

  //! Subscripting.  Return a reference to the i_th element.
  value_type& 
  operator[](const index_type i) {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 9);
#endif
    return _elem[i];
  }

  //! Indexing.  Return element in the i_th row and j_th column.
  value_type
  operator()(const index_type i, const index_type j) const {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 3 && 0 <= j && j < 3);
#endif
    return _elem[i * 3 + j];
  }

  //! Indexing.  Return a reference to the element in the i_th row and j_th column.
  reference
  operator()(const index_type i, const index_type j) {
#ifdef DEBUG_SquareMatrix
    assert(0 <= i && i < 3 && 0 <= j && j < 3);
#endif
    return _elem[i * 3 + j];
  }

  //! Get the elements in row-major order.
  void
  get(pointer x) const;

  //! Set the elements in row-major order.
  void
  set(const_pointer x);

  //! Get the elements in row-major order.
  void
  get(reference e00, reference e01, reference e02, 
      reference e10, reference e11, reference e12, 
      reference e20, reference e21, reference e22) const;

  //! Set the elements in row-major order.
  void
  set(const value_type e00, const value_type e01, const value_type e02, 
      const value_type e10, const value_type e11, const value_type e12, 
      const value_type e20, const value_type e21, const value_type e22);

  //! Negate each element.
  void
  negate();

  //! Transpose the matrix.
  void
  transpose();

  //
  // Assignment operators with scalar operand.
  //

  //! Add \c x to each element.
  SquareMatrix& 
  operator+=(const value_type x);

  //! Subtract \c x from each element.
  SquareMatrix& 
  operator-=(const value_type x);

  //! Multiply each element by \c x.
  SquareMatrix& 
  operator*=(const value_type x);

  //! Divide each element by \c x.
  SquareMatrix& 
  operator/=(const value_type x);

  //! Mod each element by \c x.
  SquareMatrix& 
  operator%=(const value_type x);

  //
  // Assignment operators with SquareMatrix operand
  //

  //! Element-wise addition.
  template<typename T2>
  SquareMatrix& 
  operator+=(const SquareMatrix<3,T2>& x);

  //! Element-wise subtraction.
  template<typename T2>
  SquareMatrix& 
  operator-=(const SquareMatrix<3,T2>& x);

  //! Matrix product.
  template<typename T2>
  SquareMatrix& 
  operator*=(const SquareMatrix<3,T2>& x);

  //
  // Unary operators
  //

  //! Unary positive operator.
  SquareMatrix
  operator+() {
    return *this;
  }
    
  //! Unary negate operator.
  SquareMatrix
  operator-();

};

//
// Binary operators
//
    
//! SquareMatrix-scalar addition.
template<typename T>
inline
SquareMatrix<3,T>
operator+(const SquareMatrix<3,T>& m, const T x) {
  return SquareMatrix<3,T>(m[0] + x, m[1] + x, m[2] + x, 
			   m[3] + x, m[4] + x, m[5] + x, 
			   m[6] + x, m[7] + x, m[8] + x);
}

//! Scalar-SquareMatrix addition.
template<typename T>
inline
SquareMatrix<3,T>
operator+(const T x, const SquareMatrix<3,T>& m) {
  return m + x;
}

//! SquareMatrix-SquareMatrix addition.
template<typename T>
inline
SquareMatrix<3,T>
operator+(const SquareMatrix<3,T>& x, const SquareMatrix<3,T>& y) {
  return SquareMatrix<3,T>(x[0] + y[0], x[1] + y[1], x[2] + y[2],
			   x[3] + y[3], x[4] + y[4], x[5] + y[5],
			   x[6] + y[6], x[7] + y[7], x[8] + y[8]);
}

//! SquareMatrix-scalar subtraction.
template<typename T>
inline
SquareMatrix<3,T>
operator-(const SquareMatrix<3,T>& m, const T x) {
  return SquareMatrix<3,T>(m[0] - x, m[1] - x, m[2] - x, 
			   m[3] - x, m[4] - x, m[5] - x, 
			   m[6] - x, m[7] - x, m[8] - x);
}

//! Scalar-SquareMatrix subtraction.
template<typename T>
inline
SquareMatrix<3,T>
operator-(const T x, const SquareMatrix<3,T>& m) {
  return SquareMatrix<3,T>(x - m[0], x - m[1], x - m[2], 
			   x - m[3], x - m[4], x - m[5], 
			   x - m[6], x - m[7], x - m[8]);
}

//! SquareMatrix-SquareMatrix subtraction.
template<typename T>
inline
SquareMatrix<3,T>
operator-(const SquareMatrix<3,T>& x, const SquareMatrix<3,T>& y) {
  return SquareMatrix<3,T>(x[0] - y[0], x[1] - y[1], x[2] - y[2],
			   x[3] - y[3], x[4] - y[4], x[5] - y[5],
			   x[6] - y[6], x[7] - y[7], x[8] - y[8]);
}

//! SquareMatrix-scalar product.
template<typename T>
inline
SquareMatrix<3,T>
operator*(const SquareMatrix<3,T>& m, const T x) {
  return SquareMatrix<3,T>(m[0] * x, m[1] * x, m[2] * x, 
			   m[3] * x, m[4] * x, m[5] * x, 
			   m[6] * x, m[7] * x, m[8] * x);
}

//! Scalar-SquareMatrix product.
template<typename T>
inline
SquareMatrix<3,T>
operator*(const T x, const SquareMatrix<3,T>& m) {
  return m * x;
}

//! SquareMatrix-SquareMatrix product.
template<typename T>
inline
SquareMatrix<3,T>
operator*(const SquareMatrix<3,T>& x, const SquareMatrix<3,T>& y) {
  return SquareMatrix<3,T>
    (x[0]*y[0]+x[1]*y[3]+x[2]*y[6],
     x[0]*y[1]+x[1]*y[4]+x[2]*y[7],
     x[0]*y[2]+x[1]*y[5]+x[2]*y[8],
     x[3]*y[0]+x[4]*y[3]+x[5]*y[6],
     x[3]*y[1]+x[4]*y[4]+x[5]*y[7],
     x[3]*y[2]+x[4]*y[5]+x[5]*y[8],
     x[6]*y[0]+x[7]*y[3]+x[8]*y[6],
     x[6]*y[1]+x[7]*y[4]+x[8]*y[7],
     x[6]*y[2]+x[7]*y[5]+x[8]*y[8]);
}

//! SquareMatrix-scalar division.
template<typename T>
inline
SquareMatrix<3,T>
operator/(const SquareMatrix<3,T>& m, const T x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  return SquareMatrix<3,T>(m[0] / x, m[1] / x, m[2] / x, 
			   m[3] / x, m[4] / x, m[5] / x, 
			   m[6] / x, m[7] / x, m[8] / x);
}

//! Scalar-SquareMatrix division.
template<typename T>
inline
SquareMatrix<3,T>
operator/(const T x, const SquareMatrix<3,T>& m) {
#ifdef DEBUG_SquareMatrix
  assert(m[0] != 0 && m[1] != 0 && m[2] != 0 &&
	 m[3] != 0 && m[4] != 0 && m[5] != 0 &&
	 m[6] != 0 && m[7] != 0 && m[8] != 0);
#endif
  return SquareMatrix<3,T>(x / m[0], x / m[1], x / m[2], 
			   x / m[3], x / m[4], x / m[5], 
			   x / m[6], x / m[7], x / m[8]);
}

//
// Math operators.
//

//! Return the sum of the elements.
template<typename T>
inline
T
computeSum(const SquareMatrix<3,T>& x) {
  return (x[0] + x[1] + x[2] + 
	  x[3] + x[4] + x[5] + 
	  x[6] + x[7] + x[8]);
}
    
//! Return the product of the elements.
template<typename T>
inline
T
computeProduct(const SquareMatrix<3,T>& x) {
  return (x[0] * x[1] * x[2] * 
	  x[3] * x[4] * x[5] * 
	  x[6] * x[7] * x[8]);
}

//! Return the minimum element.  Use < for comparison.
template<typename T>
inline
T
computeMinimum(const SquareMatrix<3,T>& x) {
  return *std::min_element(x.begin(), x.end());
}
    
//! Return the maximum element.  Use > for comparison.
template<typename T>
inline
T
computeMaximum(const SquareMatrix<3,T>& x) {
  return *std::max_element(x.begin(), x.end());
}

//! Return the determinant of the matrix.
template<typename T>
inline
T
computeDeterminant(const SquareMatrix<3,T>& x) {
  return (x[0]*x[4]*x[8] + x[1]*x[5]*x[6] + x[2]*x[3]*x[7] -
	  x[2]*x[4]*x[6] - x[1]*x[3]*x[8] - x[0]*x[5]*x[7]);
}

//! Return the trace of the matrix.
template<typename T>
inline
T
computeTrace(const SquareMatrix<3,T>& x) {
  return (x[0] + x[4] + x[8]);
}

//! Return the transpose of the matrix.
template<typename T>
inline
SquareMatrix<3,T>
computeTranspose(const SquareMatrix<3,T>& x) {
  return SquareMatrix<3,T>(x[0], x[3], x[6],
			   x[1], x[4], x[7],
			   x[2], x[5], x[8]);
}

//! Return the inverse of the matrix.
template<typename T>
inline
SquareMatrix<3,T>
computeInverse(const SquareMatrix<3,T>& x) {
  SquareMatrix<3,T> y;
  computeInverse(x, &y);
  return y;
}

//! Return the inverse of the matrix given the matrix and its determinant.
template<typename T>
inline
SquareMatrix<3,T>
computeInverse(const SquareMatrix<3,T>& x, const T det) {
  SquareMatrix<3,T> y;
  computeInverse(x, det, &y);
  return y;
}

//! Compute the inverse of the matrix.
template<typename T>
inline
void
computeInverse(const SquareMatrix<3,T>& x, SquareMatrix<3,T>* y) {
  const T det = computeDeterminant(x);
  computeInverse(x, det, y);
}

//! Compute the inverse of the matrix given its determinant.
template<typename T>
inline
void
computeInverse(const SquareMatrix<3,T>& x, const T det, SquareMatrix<3,T>* y) {
#ifdef DEBUG_SquareMatrix
  assert(det != 0);
#endif
  y->set((x[4] * x[8] - x[5] * x[7]) / det,
	(x[2] * x[7] - x[1] * x[8]) / det,
	(x[1] * x[5] - x[2] * x[4]) / det,
	(x[5] * x[6] - x[3] * x[8]) / det,
	(x[0] * x[8] - x[2] * x[6]) / det,
	(x[2] * x[3] - x[0] * x[5]) / det,
	(x[3] * x[7] - x[4] * x[6]) / det,
	(x[1] * x[6] - x[0] * x[7]) / det,
	(x[0] * x[4] - x[1] * x[3]) / det);
}

//! Calculate the scaled inverse of the matrix: determinant * inverse.
template<typename T>
inline
void
computeScaledInverse(const SquareMatrix<3,T>& x, SquareMatrix<3,T>* si) {
  (*si)[0] = x[4] * x[8] - x[5] * x[7];
  (*si)[1] = x[2] * x[7] - x[1] * x[8];
  (*si)[2] = x[1] * x[5] - x[2] * x[4];
  (*si)[3] = x[5] * x[6] - x[3] * x[8];
  (*si)[4] = x[0] * x[8] - x[2] * x[6];
  (*si)[5] = x[2] * x[3] - x[0] * x[5];
  (*si)[6] = x[3] * x[7] - x[4] * x[6];
  (*si)[7] = x[1] * x[6] - x[0] * x[7];
  (*si)[8] = x[0] * x[4] - x[1] * x[3];
}

//! Return the scaled inverse of the matrix: determinant * inverse.
template<typename T>
inline
SquareMatrix<3,T>
computeScaledInverse(const SquareMatrix<3,T>& x) {
  return SquareMatrix<3,T>(x[4] * x[8] - x[5] * x[7],
			   x[2] * x[7] - x[1] * x[8],
			   x[1] * x[5] - x[2] * x[4],
			   x[5] * x[6] - x[3] * x[8],
			   x[0] * x[8] - x[2] * x[6],
			   x[2] * x[3] - x[0] * x[5],
			   x[3] * x[7] - x[4] * x[6],
			   x[1] * x[6] - x[0] * x[7],
			   x[0] * x[4] - x[1] * x[3]);
}

//! Return the frobenius norm of the matrix.
template<typename T>
inline
T
computeFrobeniusNorm(const SquareMatrix<3,T>& x) {
  return std::sqrt(computeFrobeniusNormSquared(x));
}

//! Return the frobenius norm of the matrix.
template<typename T>
inline
T
computeFrobeniusNormSquared(const SquareMatrix<3,T>& x) {
  return (x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + 
	  x[3]*x[3] + x[4]*x[4] + x[5]*x[5] + 
	  x[6]*x[6] + x[7]*x[7] + x[8]*x[8]);
}

//! Return the inner product of the matrices.  computeTrace(transpose(x) * y)
template<typename T>
inline
T
computeInnerProduct(const SquareMatrix<3,T>& x, const SquareMatrix<3,T>& y) {
  return x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3] + x[4] * y[4]
    + x[5] * y[5] + x[6] * y[6] + x[7] * y[7] + x[8] * y[8];
}

//! Compute the matrix-vector product. <tt>x = m * v</tt>.
template<typename T>
inline
void
computeProduct(const SquareMatrix<3,T>& m, const FixedArray<3,T>& v, 
		FixedArray<3,T>* x) {
  (*x)[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
  (*x)[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
  (*x)[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}

//
// Equality
//

//! Return true if the matrices are equal.
template<typename T1, typename T2>
inline
bool
operator==(const SquareMatrix<3,T1>& a, const SquareMatrix<3,T2>& b) {
  return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] &&
	  a[3] == b[3] && a[4] == b[4] && a[5] == b[5] &&
	  a[6] == b[6] && a[7] == b[7] && a[8] == b[8]);
}

//! Return true if the tensors are not equal.
template<typename T1, typename T2>
inline
bool
operator!=(const SquareMatrix<3,T1>& a, const SquareMatrix<3,T2>& b) { 
  return !(a == b); 
}

//
// I/O
//
    
//! Write a matrix as rows with space-separated numbers.
template<typename T>
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<3,T>& x);
    
//! Read white space-separated numbers into a matrix.
template<typename T>
std::istream&
operator>>(std::istream& in, SquareMatrix<3,T>& x);

END_NAMESPACE_ADS

#define __SquareMatrix_ipp__
#include "SquareMatrix.ipp"
#undef __SquareMatrix_ipp__

#endif
