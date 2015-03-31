// -*- C++ -*-

#if !defined(__numerical_random_PoissonPdfAtTheMode_ipp__)
#error This file is an implementation detail of PoissonPdfAtTheMode.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Construct from the range of means and the number of patches per unit.
// I increase openUpperBound by 1 to allow for round-off error in determining
// the array index.
template<typename T>
inline
PoissonPdfAtTheMode<T>::
PoissonPdfAtTheMode(const int closedLowerBound, const int openUpperBound, 
		    const int numberOfPatchesPerUnit) :
  _lowerBound(closedLowerBound),
  _scaleToIndex(numberOfPatchesPerUnit),
  // Four coefficients for each cubic polynomial.
  _coefficients(4 * numberOfPatchesPerUnit * 
		((openUpperBound + 1) - closedLowerBound)) {
  // Functor for evaluating the PDF.
  PoissonPdf<Number> pdf;
  // Arrays for the function and derivative values.
  ads::Array<1, Number> f(numberOfPatchesPerUnit + 1);
  ads::Array<1, Number> d(numberOfPatchesPerUnit + 1);
  const Number patchLength = 1.0 / numberOfPatchesPerUnit;
  int mode;
  Number mean;
  int index = 0;
  // For each continuous portion of the PDF.
  for (int i = closedLowerBound; i != openUpperBound + 1; ++i) {
    mode = closedLowerBound + i;
    // First compute the values of the function and its derivatives.
    for (int j = 0; j != numberOfPatchesPerUnit + 1; ++j) {
      mean = _lowerBound + i + j * patchLength;
      f[j] = pdf(mean, mode);
      // Scale the derivative by the patch length.
      d[j] = pdf.computeDerivative(mean, mode) * patchLength;
    }
    // From these, compute the coefficients of the cubic polynomials:
    // a + b t + c t^2 + d t^3
    for (int j = 0; j != numberOfPatchesPerUnit; ++j) {
      computeHermitePolynomialCoefficients(f[j], f[j + 1], d[j], d[j + 1],
					   _coefficients.begin() + 4 * index++);
    }
  }
}


// Copy constructor.
template<typename T>
inline
PoissonPdfAtTheMode<T>::
PoissonPdfAtTheMode(const PoissonPdfAtTheMode& other) :
  _lowerBound(other._lowerBound),
  _scaleToIndex(other._scaleToIndex),
  _coefficients(other._coefficients)
{}


// Assignment operator.
template<typename T>
inline
PoissonPdfAtTheMode<T>&
PoissonPdfAtTheMode<T>::
operator=(const PoissonPdfAtTheMode& other) {
  if (this != &other) {
    _lowerBound = other._lowerBound;
    _scaleToIndex = other._scaleToIndex;
    _coefficients = other._coefficients;
  }
  return *this;
}


// Return the probability density function evaluated at the mode.
template<typename T>
inline
typename PoissonPdfAtTheMode<T>::result_type
PoissonPdfAtTheMode<T>::
operator()(argument_type x) const {
  // Scale the argument to the array index range.
  x = (x - _lowerBound) * _scaleToIndex;
  // The index of the patch.
  const int index = int(x);
  // Perform the Hermite interpolation.
  x -= index;
  return evaluatePolynomial<3>(x, _coefficients.begin() + 4 * index);
}


END_NAMESPACE_NUMERICAL

// End of file.
