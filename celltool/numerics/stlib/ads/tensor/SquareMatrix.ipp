// -*- C++ -*-

#if !defined(__SquareMatrix_ipp__)
#error This file is an implementation detail of the class SquareMatrix.
#endif

BEGIN_NAMESPACE_ADS

//
// 1x1 matrices.
//

//
// Constructors
//

// Copy constructor.
template<typename T>
inline
SquareMatrix<1,T>::
SquareMatrix(const SquareMatrix& other) :
  _elem(other._elem)
{}

// Construct from a matrix with different number type.
template<typename T>
template<typename T2>
inline
SquareMatrix<1,T>::
SquareMatrix(const SquareMatrix<1,T2>& x) :
  _elem(x._elem)
{}

// Construct from the matrix elements.
template<typename T>
inline
SquareMatrix<1,T>::
SquareMatrix(const value_type e) :
  _elem(e)
{}

// Construct from an array.
template<typename T>
inline
SquareMatrix<1,T>::
SquareMatrix(const_pointer x) :
  _elem(*x)
{}

//
// Assignment operators
//

template<typename T>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator=(const SquareMatrix& x) {
  if (this != &x) {
    _elem = x._elem;
  }
  return *this;
}

template<typename T> 
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator=(const value_type x) {
  _elem = x;
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator=(const SquareMatrix<1,T2>& x) {
  _elem = x._elem;
  return *this;
}

//
// Accessors and manipulators.
//

//! Get the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<1,T>::
get(pointer x) const {
  *x = _elem;
}

//! Set the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<1,T>::
set(const_pointer x) {
  _elem = *x;
}

//! Get the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<1,T>::
get(reference e) const {
  e = _elem;
}

//! Set the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<1,T>::
set(const value_type e) {
  _elem = e;
}

template<typename T>
inline
void
SquareMatrix<1,T>::
negate() {
  _elem = - _elem;
}

template<typename T>
inline
void
SquareMatrix<1,T>::
transpose()
{}

//
// Assignment operators with scalar operand.
//

template<typename T>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator+=(const value_type x) {
  _elem += x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator-=(const value_type x) {
  _elem -= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator*=(const value_type x) {
  _elem *= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator/=(const value_type x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  _elem /= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator%=(const value_type x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  _elem %= x;
  return *this;
}

//
// Assignment operators with SquareMatrix operand
//

template<typename T> 
template<typename T2>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator+=(const SquareMatrix<1,T2>& x) {
  _elem += x._elem;
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator-=(const SquareMatrix<1,T2>& x) {
  _elem -= x._elem;
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<1,T>&
SquareMatrix<1,T>::
operator*=(const SquareMatrix<1,T2>& x) {
  _elem *= x._elem;
  return *this;
}

//
// Unary operators.
//

template<typename T>
inline
SquareMatrix<1,T>
SquareMatrix<1,T>::
operator-() {
  return SquareMatrix<1,T>(- _elem);
}
     
//
// I/O
//
    
template<typename T>
inline
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<1,T>& x) {
  return out << x[0] << '\n';
}
    
template<typename T>
inline
std::istream&
operator>>(std::istream& in, SquareMatrix<1,T>& x) {
  return in >> x[0];
}



//
// 2x2 matrices.
//

//
// Constructors
//

// Copy constructor.
template<typename T>
inline
SquareMatrix<2,T>::
SquareMatrix(const SquareMatrix& x) {
  _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; 
  _elem[2] = x._elem[2]; _elem[3] = x._elem[3];
}

// Construct from a matrix with different number type.
template<typename T>
template<typename T2>
inline
SquareMatrix<2,T>::
SquareMatrix(const SquareMatrix<2,T2>& x) {
  _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; 
  _elem[2] = x._elem[2]; _elem[3] = x._elem[3];
}

// Construct from the matrix elements.
template<typename T>
inline
SquareMatrix<2,T>::
SquareMatrix(const value_type e00, const value_type e01,
	      const value_type e10, const value_type e11) {
  _elem[0] = e00; _elem[1] = e01;
  _elem[2] = e10; _elem[3] = e11;
}

// Construct from an array.
template<typename T>
inline
SquareMatrix<2,T>::
SquareMatrix(const_pointer x) {
  _elem[0] = x[0]; _elem[1] = x[1]; 
  _elem[2] = x[2]; _elem[3] = x[3];
}

// Initialize all the elements with the given value.
template<typename T>
inline
SquareMatrix<2,T>::
SquareMatrix(const value_type x) {
  _elem[0] = x; _elem[1] = x; 
  _elem[2] = x; _elem[3] = x;
}

//
// Assignment operators
//

template<typename T>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator=(const SquareMatrix& x) {
  if (this != &x) {
    _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; 
    _elem[2] = x._elem[2]; _elem[3] = x._elem[3];
  }
  return *this;
}

template<typename T> 
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator=(const value_type x) {
  _elem[0] = x; _elem[1] = x; 
  _elem[2] = x; _elem[3] = x;
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator=(const SquareMatrix<2,T2>& x) {
  _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; 
  _elem[2] = x._elem[2]; _elem[3] = x._elem[3];
  return *this;
}

//
// Accessors and manipulators.
//

//! Get the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<2,T>::
get(pointer x) const {
  x[0] = _elem[0]; x[1] = _elem[1]; 
  x[2] = _elem[2]; x[3] = _elem[3];
}

//! Set the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<2,T>::
set(const_pointer x) {
  _elem[0] = x[0]; _elem[1] = x[1]; 
  _elem[2] = x[2]; _elem[3] = x[3];
}

//! Get the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<2,T>::
get(reference e00, reference e01,
     reference e10, reference e11) const {
  e00 = _elem[0]; e01 = _elem[1];
  e10 = _elem[2]; e11 = _elem[3];
}

//! Set the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<2,T>::
set(const value_type e00, const value_type e01,
     const value_type e10, const value_type e11) {
  _elem[0] = e00; _elem[1] = e01;
  _elem[2] = e10; _elem[3] = e11;
}

template<typename T>
inline
void
SquareMatrix<2,T>::
negate() {
  _elem[0] = - _elem[0]; _elem[1] = - _elem[1]; 
  _elem[2] = - _elem[2]; _elem[3] = - _elem[3];
}

template<typename T>
inline
void
SquareMatrix<2,T>::
transpose() {
  std::swap(_elem[1], _elem[2]);
}

//
// Assignment operators with scalar operand.
//

template<typename T>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator+=(const value_type x) {
  _elem[0] += x; _elem[1] += x; 
  _elem[2] += x; _elem[3] += x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator-=(const value_type x) {
  _elem[0] -= x; _elem[1] -= x; 
  _elem[2] -= x; _elem[3] -= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator*=(const value_type x) {
  _elem[0] *= x; _elem[1] *= x; 
  _elem[2] *= x; _elem[3] *= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator/=(const value_type x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  _elem[0] /= x; _elem[1] /= x; 
  _elem[2] /= x; _elem[3] /= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator%=(const value_type x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  _elem[0] %= x; _elem[1] %= x; 
  _elem[2] %= x; _elem[3] %= x;
  return *this;
}

//
// Assignment operators with SquareMatrix operand
//

template<typename T> 
template<typename T2>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator+=(const SquareMatrix<2,T2>& x) {
  _elem[0] += x._elem[0]; _elem[1] += x._elem[1]; 
  _elem[2] += x._elem[2]; _elem[3] += x._elem[3];
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator-=(const SquareMatrix<2,T2>& x) {
  _elem[0] -= x._elem[0]; _elem[1] -= x._elem[1]; 
  _elem[2] -= x._elem[2]; _elem[3] -= x._elem[3];
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<2,T>&
SquareMatrix<2,T>::
operator*=(const SquareMatrix<2,T2>& x) {
  value_type v0 = _elem[0];
  value_type v1 = _elem[1];
  _elem[0] = v0 * x._elem[0] + v1 * x._elem[2];
  _elem[1] = v0 * x._elem[1] + v1 * x._elem[3];
  v0 = _elem[2];
  v1 = _elem[3];
  _elem[2] = v0 * x._elem[0] + v1 * x._elem[2];
  _elem[3] = v0 * x._elem[1] + v1 * x._elem[3];

  return *this;
}

//
// Unary operators.
//

template<typename T>
inline
SquareMatrix<2,T>
SquareMatrix<2,T>::
operator-() {
  return SquareMatrix<2,T>(- _elem[0], - _elem[1], 
			    - _elem[2], - _elem[3]);
}
     
//
// I/O
//
    
template<typename T>
inline
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<2,T>& x) {
  return out << x[0] << " "<< x[1] << '\n'
	     << x[2] << " "<< x[3] << '\n';
}
    
template<typename T>
inline
std::istream&
operator>>(std::istream& in, SquareMatrix<2,T>& x) {
  return in >> x[0] >> x[1] 
	    >> x[2] >> x[3];
}

//
// 3x3 matrices.
//

//
// Constructors
//

// Copy constructor.
template<typename T>
inline
SquareMatrix<3,T>::
SquareMatrix(const SquareMatrix& x) {
  _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; _elem[2] = x._elem[2];
  _elem[3] = x._elem[3]; _elem[4] = x._elem[4]; _elem[5] = x._elem[5];
  _elem[6] = x._elem[6]; _elem[7] = x._elem[7]; _elem[8] = x._elem[8];
}

// Construct from a matrix with different number type.
template<typename T>
template<typename T2>
inline
SquareMatrix<3,T>::
SquareMatrix(const SquareMatrix<3,T2>& x) {
  _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; _elem[2] = x._elem[2];
  _elem[3] = x._elem[3]; _elem[4] = x._elem[4]; _elem[5] = x._elem[5];
  _elem[6] = x._elem[6]; _elem[7] = x._elem[7]; _elem[8] = x._elem[8];
}

// Construct from the matrix elements.
template<typename T>
inline
SquareMatrix<3,T>::
SquareMatrix(const value_type e00, const value_type e01, const value_type e02, 
	     const value_type e10, const value_type e11, const value_type e12, 
	     const value_type e20, const value_type e21, const value_type e22) {
  _elem[0] = e00; _elem[1] = e01; _elem[2] = e02;
  _elem[3] = e10; _elem[4] = e11; _elem[5] = e12;
  _elem[6] = e20; _elem[7] = e21; _elem[8] = e22;
}

// Construct from an array.
template<typename T>
inline
SquareMatrix<3,T>::
SquareMatrix(const_pointer x) {
  _elem[0] = x[0]; _elem[1] = x[1]; _elem[2] = x[2];
  _elem[3] = x[3]; _elem[4] = x[4]; _elem[5] = x[5];
  _elem[6] = x[6]; _elem[7] = x[7]; _elem[8] = x[8];
}

// Initialize all the elements with the given value.
template<typename T>
inline
SquareMatrix<3,T>::
SquareMatrix(const value_type x) {
  _elem[0] = x; _elem[1] = x; _elem[2] = x;
  _elem[3] = x; _elem[4] = x; _elem[5] = x;
  _elem[6] = x; _elem[7] = x; _elem[8] = x;
}

//
// Assignment operators
//

template<typename T>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator=(const SquareMatrix& x) {
  if (this != &x) {
    _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; _elem[2] = x._elem[2];
    _elem[3] = x._elem[3]; _elem[4] = x._elem[4]; _elem[5] = x._elem[5];
    _elem[6] = x._elem[6]; _elem[7] = x._elem[7]; _elem[8] = x._elem[8];
  }
  return *this;
}

template<typename T> 
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator=(const value_type x) {
  _elem[0] = x; _elem[1] = x; _elem[2] = x;
  _elem[3] = x; _elem[4] = x; _elem[5] = x;
  _elem[6] = x; _elem[7] = x; _elem[8] = x;
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator=(const SquareMatrix<3,T2>& x) {
  _elem[0] = x._elem[0]; _elem[1] = x._elem[1]; _elem[2] = x._elem[2];
  _elem[3] = x._elem[3]; _elem[4] = x._elem[4]; _elem[5] = x._elem[5];
  _elem[6] = x._elem[6]; _elem[7] = x._elem[7]; _elem[8] = x._elem[8];
  return *this;
}

//
// Accessors and manipulators.
//

//! Get the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<3,T>::
get(pointer x) const {
  x[0] = _elem[0]; x[1] = _elem[1]; x[2] = _elem[2];
  x[3] = _elem[3]; x[4] = _elem[4]; x[5] = _elem[5];
  x[6] = _elem[6]; x[7] = _elem[7]; x[8] = _elem[8];
}

//! Set the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<3,T>::
set(const_pointer x) {
  _elem[0] = x[0]; _elem[1] = x[1]; _elem[2] = x[2];
  _elem[3] = x[3]; _elem[4] = x[4]; _elem[5] = x[5];
  _elem[6] = x[6]; _elem[7] = x[7]; _elem[8] = x[8];
}

//! Get the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<3,T>::
get(reference e00, reference e01, reference e02, 
     reference e10, reference e11, reference e12, 
     reference e20, reference e21, reference e22) const {
  e00 = _elem[0]; e01 = _elem[1]; e02 = _elem[2];
  e10 = _elem[3]; e11 = _elem[4]; e12 = _elem[5];
  e20 = _elem[6]; e21 = _elem[7]; e22 = _elem[8];
}

//! Set the elements in row-major order.
template<typename T>
inline
void
SquareMatrix<3,T>::
set(const value_type e00, const value_type e01, const value_type e02, 
     const value_type e10, const value_type e11, const value_type e12, 
     const value_type e20, const value_type e21, const value_type e22) {
  _elem[0] = e00; _elem[1] = e01; _elem[2] = e02;
  _elem[3] = e10; _elem[4] = e11; _elem[5] = e12;
  _elem[6] = e20; _elem[7] = e21; _elem[8] = e22;
}

template<typename T>
inline
void
SquareMatrix<3,T>::
negate() {
  _elem[0] = - _elem[0]; _elem[1] = - _elem[1]; _elem[2] = - _elem[2];
  _elem[3] = - _elem[3]; _elem[4] = - _elem[4]; _elem[5] = - _elem[5];
  _elem[6] = - _elem[6]; _elem[7] = - _elem[7]; _elem[8] = - _elem[8];
}

template<typename T>
inline
void
SquareMatrix<3,T>::
transpose() {
  std::swap(_elem[1], _elem[3]);
  std::swap(_elem[2], _elem[6]);
  std::swap(_elem[5], _elem[7]);
}

//
// Assignment operators with scalar operand.
//

template<typename T>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator+=(const value_type x) {
  _elem[0] += x; _elem[1] += x; _elem[2] += x;
  _elem[3] += x; _elem[4] += x; _elem[5] += x;
  _elem[6] += x; _elem[7] += x; _elem[8] += x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator-=(const value_type x) {
  _elem[0] -= x; _elem[1] -= x; _elem[2] -= x;
  _elem[3] -= x; _elem[4] -= x; _elem[5] -= x;
  _elem[6] -= x; _elem[7] -= x; _elem[8] -= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator*=(const value_type x) {
  _elem[0] *= x; _elem[1] *= x; _elem[2] *= x;
  _elem[3] *= x; _elem[4] *= x; _elem[5] *= x;
  _elem[6] *= x; _elem[7] *= x; _elem[8] *= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator/=(const value_type x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  _elem[0] /= x; _elem[1] /= x; _elem[2] /= x;
  _elem[3] /= x; _elem[4] /= x; _elem[5] /= x;
  _elem[6] /= x; _elem[7] /= x; _elem[8] /= x;
  return *this;
}

template<typename T>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator%=(const value_type x) {
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  _elem[0] %= x; _elem[1] %= x; _elem[2] %= x;
  _elem[3] %= x; _elem[4] %= x; _elem[5] %= x;
  _elem[6] %= x; _elem[7] %= x; _elem[8] %= x;
  return *this;
}

//
// Assignment operators with SquareMatrix operand
//

template<typename T> 
template<typename T2>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator+=(const SquareMatrix<3,T2>& x) {
  _elem[0] += x._elem[0]; _elem[1] += x._elem[1]; _elem[2] += x._elem[2];
  _elem[3] += x._elem[3]; _elem[4] += x._elem[4]; _elem[5] += x._elem[5];
  _elem[6] += x._elem[6]; _elem[7] += x._elem[7]; _elem[8] += x._elem[8];
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator-=(const SquareMatrix<3,T2>& x) {
  _elem[0] -= x._elem[0]; _elem[1] -= x._elem[1]; _elem[2] -= x._elem[2];
  _elem[3] -= x._elem[3]; _elem[4] -= x._elem[4]; _elem[5] -= x._elem[5];
  _elem[6] -= x._elem[6]; _elem[7] -= x._elem[7]; _elem[8] -= x._elem[8];
  return *this;
}

template<typename T> 
template<typename T2>
inline 
SquareMatrix<3,T>&
SquareMatrix<3,T>::
operator*=(const SquareMatrix<3,T2>& x) {
  // First row.
  value_type v0 = _elem[0];
  value_type v1 = _elem[1];
  value_type v2 = _elem[2];
  _elem[0] = v0 * x._elem[0] + v1 * x._elem[3] + v2 * x._elem[6];
  _elem[1] = v0 * x._elem[1] + v1 * x._elem[4] + v2 * x._elem[7];
  _elem[2] = v0 * x._elem[2] + v1 * x._elem[5] + v2 * x._elem[8];

  // Second row.
  v0 = _elem[3];
  v1 = _elem[4];
  v2 = _elem[5];
  _elem[3] = v0 * x._elem[0] + v1 * x._elem[3] + v2 * x._elem[6];
  _elem[4] = v0 * x._elem[1] + v1 * x._elem[4] + v2 * x._elem[7];
  _elem[5] = v0 * x._elem[2] + v1 * x._elem[5] + v2 * x._elem[8];

  // Third row.
  v0 = _elem[6];
  v1 = _elem[7];
  v2 = _elem[8];
  _elem[6] = v0 * x._elem[0] + v1 * x._elem[3] + v2 * x._elem[6];
  _elem[7] = v0 * x._elem[1] + v1 * x._elem[4] + v2 * x._elem[7];
  _elem[8] = v0 * x._elem[2] + v1 * x._elem[5] + v2 * x._elem[8];

  return *this;
}

//
// Unary operators.
//

template<typename T>
inline
SquareMatrix<3,T>
SquareMatrix<3,T>::
operator-() {
  return SquareMatrix<3,T>(- _elem[0], - _elem[1], - _elem[2],
			    - _elem[3], - _elem[4], - _elem[5],
			    - _elem[6], - _elem[7], - _elem[8]);
}
     
//
// I/O
//
    
template<typename T>
inline
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<3,T>& x) {
  return out << x[0] << " "<< x[1] << " "<< x[2] << '\n'
	     << x[3] << " "<< x[4] << " "<< x[5] << '\n'
	     << x[6] << " "<< x[7] << " "<< x[8] << '\n';
}
    
template<typename T>
inline
std::istream&
operator>>(std::istream& in, SquareMatrix<3,T>& x) {
  return in >> x[0] >> x[1] >> x[2]
	    >> x[3] >> x[4] >> x[5]
	    >> x[6] >> x[7] >> x[8];
}




//-----------------------------------------------------------------------------
// NxN matrices.
//-----------------------------------------------------------------------------

//
// Constructors
//

// Copy constructor.
template<int N, typename T>
inline
SquareMatrix<N,T>::
SquareMatrix(const SquareMatrix& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::copy(x.begin(), x.end(), begin());
}

// Construct from a matrix with different number type.
template<int N, typename T>
template<typename T2>
inline
SquareMatrix<N,T>::
SquareMatrix(const SquareMatrix<N,T2>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::copy(x.begin(), x.end(), begin());
}

// Construct from an array.
template<int N, typename T>
inline
SquareMatrix<N,T>::
SquareMatrix(const_pointer x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::copy(x, x + size(), begin());
}

// Initialize all the elements with the given value.
template<int N, typename T>
inline
SquareMatrix<N,T>::
SquareMatrix(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::fill(begin(), end(), x);
}

//
// Assignment operators
//

template<int N, typename T>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator=(const SquareMatrix& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  if (this != &x) {
    std::copy(x.begin(), x.end(), begin());
  }
  return *this;
}

template<int N, typename T> 
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator=(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::fill(begin(), end(), x);
  return *this;
}

template<int N, typename T> 
template<typename T2>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator=(const SquareMatrix<N,T2>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::copy(x.begin(), x.end(), begin());
  return *this;
}

//
// Accessors and manipulators.
//

// Get the elements in row-major order.
template<int N, typename T>
inline
void
SquareMatrix<N,T>::
get(pointer x) const {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::copy(begin(), end(), x);
}

// Set the elements in row-major order.
template<int N, typename T>
inline
void
SquareMatrix<N,T>::
set(const_pointer x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  std::copy(x, x + size(), begin());
}

  // Get the specified minor of the matrix.
template<int N, typename T>
inline
void
SquareMatrix<N,T>::
getMinor(const int i, const int j, SquareMatrix<N-1,T>& minor) const {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  int x, y, a, b;
  for (x = 0, a = 0; x != N-1; ++x, ++a) {
    if (a == i) {
      ++a;
    }
    for (y = 0, b = 0; y != N-1; ++y, ++b) {
      if (b == j) {
	++b;
      }
      (minor)(x,y) = _elem[a][b];
    }
  }
}

template<int N, typename T>
inline
void
SquareMatrix<N,T>::
negate() {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i) {
    *i = - *i;
  }
}

template<int N, typename T>
inline
void
SquareMatrix<N,T>::
transpose() {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  int i, j;
  for (i = 0; i != N; ++i) {
    for (j = i+1; j != N; ++j) {
      std::swap(_elem[i][j], _elem[j][i]);
    }
  }
}

//
// Assignment operators with scalar operand.
//

template<int N, typename T>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator+=(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i) {
    *i += x;
  }
  return *this;
}

template<int N, typename T>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator-=(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i) {
    *i -= x;
  }
  return *this;
}

template<int N, typename T>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator*=(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i) {
    *i *= x;
  }
  return *this;
}

template<int N, typename T>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator/=(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i) {
    *i /= x;
  }
  return *this;
}

template<int N, typename T>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator%=(const value_type x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
#ifdef DEBUG_SquareMatrix
  assert(x != 0);
#endif
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i) {
    *i %= x;
  }
  return *this;
}

//
// Assignment operators with SquareMatrix operand
//

template<int N, typename T> 
template<typename T2>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator+=(const SquareMatrix<N,T2>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  typename SquareMatrix<N,T2>::const_iterator j = x.begin();
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i, ++j) {
    *i += *j;
  }
  return *this;
}

template<int N, typename T> 
template<typename T2>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator-=(const SquareMatrix<N,T2>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  typename SquareMatrix<N,T2>::const_iterator j = x.begin();
  const iterator finish = end();
  for (iterator i = begin(); i != finish; ++i, ++j) {
    *i -= *j;
  }
  return *this;
}

template<int N, typename T> 
template<typename T2>
inline 
SquareMatrix<N,T>&
SquareMatrix<N,T>::
operator*=(const SquareMatrix<N,T2>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  value_type s;
  ads::FixedArray<N,value_type> row;
  for (int i = 0; i != N; ++i) {
    row.copy(&_elem[i][0], &_elem[i][0] + N);
    for (int j = 0; j != N; ++j) {
      s = 0;
      for (int k = 0; k != N; ++k) {
	s += row[k] * x(k,j);
      }
      _elem[i][j] = s;
    }
  }
  return *this;
}

//
// Unary operators.
//

template<int N, typename T>
inline
SquareMatrix<N,T>
SquareMatrix<N,T>::
operator-() {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(*this);
  result.negate();
  return result;
}
     
//
// Binary operators
//
    
// SquareMatrix-SquareMatrix product.
template<int N, typename T>
inline
SquareMatrix<N,T>
operator*(const SquareMatrix<N,T>& x, const SquareMatrix<N,T>& y) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result(0);
  int i, j, k;
  for (i = 0; i != N; ++i) {
    for (j = 0; j != N; ++j) {
      for (k = 0; k != N; ++k) {
	result(i, j) += x(i, k) * y(k, j);
      }
    }
  }
  return result;
}

//! Scalar-SquareMatrix division.
template<int N, typename T>
inline
SquareMatrix<N,T>
operator/(const T x, const SquareMatrix<N,T>& m) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result;
  int i, j;
  for (i = 0; i != N; ++i) {
    for (j = 0; j != N; ++j) {
#ifdef DEBUG_SquareMatrix
      assert(m(i,j) != 0);
#endif
      result(i,j) = x / m(i,j);
    }
  }
  return result;
}

//
// Math operators.
//

// Return the determinant of the matrix.
template<int N, typename T>
inline
T
computeDeterminant(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  T result = 0;
  int sign = 1;
  SquareMatrix<N-1,T> minor;
  for (int j = 0; j != N; ++j, sign = -sign) {
    x.getMinor(0, j, minor);
    result += sign * x(0,j) * computeDeterminant(minor);
  }
  return result;
}

// Return the trace of the matrix.
template<int N, typename T>
inline
T
computeTrace(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  T result = 0;
  for (int i = 0; i != N; ++i) {
    result += x(i,i);
  }
  return result;
}

// Return the transpose of the matrix.
template<int N, typename T>
inline
SquareMatrix<N,T>
computeTranspose(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  SquareMatrix<N,T> result;
  int i, j;
  for (i = 0; i != N; ++i) {
    for (j = 0; j != N; ++j) {
      result(i,j) = x(j,i);
    }
  }
  return result;
}

// Return the inverse of the matrix.
template<int N, typename T>
inline
SquareMatrix<N,T>
computeInverse(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  T det = computeDeterminant(x);
  assert(det != 0);

  SquareMatrix<N,T> result = computeScaledInverse(x);
  result /= det;
  return result;
}

// Calculate the scaled inverse of the matrix: determinant * inverse.
template<int N, typename T>
inline
void
computeScaledInverse(const SquareMatrix<N,T>& x, SquareMatrix<N,T>* si) {
  SquareMatrix<N-1,T> minor;
  int i, j;
  for (i = 0; i != N; ++i) {
    for (j = 0; j != N; ++j) {
      x.getMinor(i, j, minor);
      if ((i + j) % 2 == 0) {
	(*si)(i,j) = computeDeterminant(minor);
      }
      else {
	(*si)(i,j) = - computeDeterminant(minor);
      }
    }
  }
  si->transpose();
}

// Return the scaled inverse of the matrix: determinant * inverse.
template<int N, typename T>
inline
SquareMatrix<N,T>
computeScaledInverse(const SquareMatrix<N,T>& x) {
  SquareMatrix<N,T> result;
  computeScaledInverse(x, &result);
  return result;
}

// Return the frobenius norm of the matrix.
template<int N, typename T>
inline
T
computeFrobeniusNormSquared(const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  T result = 0;
  const typename SquareMatrix<N,T>::const_iterator finish = x.end();
  for (typename SquareMatrix<N,T>::const_iterator i = x.begin(); i != finish;
	++i) {
    result += *i * *i;
  }
  return result;
}

//
// I/O
//
    
template<int N, typename T>
inline
std::ostream&
operator<<(std::ostream& out, const SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N-1; ++j) {
      out << x(i,j) << " ";
    }
    out << x(i,N-1) << '\n';
  }
  return out;
}
    
template<int N, typename T>
inline
std::istream&
operator>>(std::istream& in, SquareMatrix<N,T>& x) {
  LOKI_STATIC_CHECK(N > 3, N_greater_than_3);
  for (typename SquareMatrix<N,T>::iterator i = x.begin(); i != x.end(); 
	++i) {
    in >> *i;
  }
  return in;
}

END_NAMESPACE_ADS
