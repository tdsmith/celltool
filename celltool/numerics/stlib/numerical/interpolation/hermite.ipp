// -*- C++ -*-

#if !defined(__numerical_interpolation_hermite_ipp__)
#error This file is an implementation detail of numerical/interpolation/hermite.
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T>
inline
T
hermiteInterpolate(const T t, const T value0, const T value1, 
		   const T derivative0, const T derivative1) {
  const T t2 = t * t;
  const T t3 = t2 * t;
  return value0 * (2 * t3 - 3 * t2 + 1) +
    value1 * (-2 * t3 + 3 * t2) +
    derivative0 * (t3 - 2 * t2 + t) +
    derivative1 * (t3 - t2);
}


// Compute the polynomial coefficients for Hermite interpolation.
template<typename T>
inline
void
computeHermitePolynomialCoefficients(const T f0, const T f1, 
				     const T d0, const T d1,
				     T* coefficients) {
  coefficients[0] = f0;
  coefficients[1] = d0;
  coefficients[2] = -3 * f0 + 3 * f1 - 2 * d0 - d1;
  coefficients[3] = 2 * f0 - 2 * f1 + d0 + d1;
}


//----------------------------------------------------------------------------
// Hermite
//----------------------------------------------------------------------------


// Construct from the functor, its derivative, the range, and the number of patches.
template<typename T>
template<typename _Function, typename Derivative>
inline
Hermite<T>::
Hermite(const _Function& function, const Derivative& derivative,
	Number closedLowerBound, Number openUpperBound, int numberOfPatches) :
  _lowerBound(closedLowerBound),
  _scaleToIndex(numberOfPatches / (openUpperBound - closedLowerBound)),
  // Four coefficients for each cubic polynomial.
  _coefficients(4 * numberOfPatches) {
  // First compute the values of the function and its derivatives.
  ads::Array<1, Number> f(numberOfPatches + 1);
  ads::Array<1, Number> d(numberOfPatches + 1);
  const Number patchLength = (openUpperBound - closedLowerBound) / 
    numberOfPatches;
  Number x;
  for (int i = 0; i != numberOfPatches + 1; ++i) {
    x = _lowerBound + i * patchLength;
    f[i] = function(x);
    d[i] = derivative(x) * patchLength;
  }
  // From these, compute the coefficients of the cubic polynomials:
  // a + b t + c t^2 + d t^3
  for (int i = 0; i != numberOfPatches; ++i) {
    computeHermitePolynomialCoefficients(f[i], f[i + 1], d[i], d[i + 1],
					 _coefficients.begin() + 4 * i);
  }
}


// Copy constructor.
template<typename T>
inline
Hermite<T>::
Hermite(const Hermite& other) :
  _lowerBound(other._lowerBound),
  _scaleToIndex(other._scaleToIndex),
  _coefficients(other._coefficients)
{}


// Assignment operator.
template<typename T>
inline
Hermite<T>&
Hermite<T>::
operator=(const Hermite& other) {
  if (this != &other) {
    _lowerBound = other._lowerBound;
    _scaleToIndex = other._scaleToIndex;
    _coefficients = other._coefficients;
  }
  return *this;
}


// Return the interpolated function value.
template<typename T>
inline
typename Hermite<T>::result_type
Hermite<T>::
operator()(argument_type x) const {
  // Scale the argument to the range [0..N).
  x = (x - _lowerBound) * _scaleToIndex;
  // The index of the patch.
  const int index = int(x);
  // Perform the Hermite interpolation.
  x -= index;
  return evaluatePolynomial<3>(x, _coefficients.begin() + 4 * index);
}



//----------------------------------------------------------------------------
// HermiteFunctionDerivative
//----------------------------------------------------------------------------



// Construct from the functor, its derivative, the range, and the number of patches.
template<typename T>
template<typename _Function, typename Derivative>
inline
HermiteFunctionDerivative<T>::
HermiteFunctionDerivative(const _Function& function, 
			  const Derivative& derivative,
			  const Number closedLowerBound, 
			  const Number openUpperBound, 
			  const int numberOfPatches) :
  _lowerBound(closedLowerBound),
  _scaleToIndex(numberOfPatches / (openUpperBound - closedLowerBound)),
  _functionAndDerivativeValues(2 * (numberOfPatches + 1)) {
  const Number patchLength = (openUpperBound - closedLowerBound) / 
    numberOfPatches;
  Number x;
  for (int i = 0; i != numberOfPatches + 1; ++i) {
    x = _lowerBound + i * patchLength;
    _functionAndDerivativeValues[2 * i] = function(x);
    _functionAndDerivativeValues[2 * i + 1] = derivative(x) * patchLength;
  }
}


// Return the interpolated function value.
template<typename T>
inline
typename HermiteFunctionDerivative<T>::result_type
HermiteFunctionDerivative<T>::
operator()(argument_type x) const {
  // Scale the argument to the range [0..N).
  x = (x - _lowerBound) * _scaleToIndex;
  // The index of the patch.
  const int index = int(x);
  // Perform the HermiteFunctionDerivative interpolation.
  return hermiteInterpolate(x - index, 
			    _functionAndDerivativeValues[2 * index],
			    _functionAndDerivativeValues[2 * index + 2],
			    _functionAndDerivativeValues[2 * index + 1],
			    _functionAndDerivativeValues[2 * index + 3]);
}

END_NAMESPACE_NUMERICAL

// End of file.
