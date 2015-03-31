// -*- C++ -*-

#if !defined(__numerical_random_PoissonPdfCdfAtTheMode_ipp__)
#error This file is an implementation detail of PoissonPdfCdfAtTheMode.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Construct from the range of means and the number of patches per unit.
// I increase openUpperBound by 1 to allow for round-off error in determining
// the array index.
template<typename T>
inline
PoissonPdfCdfAtTheMode<T>::
PoissonPdfCdfAtTheMode(const int closedLowerBound, const int openUpperBound, 
		       const int numberOfPatchesPerUnit) :
  _lowerBound(closedLowerBound),
  _scaleToIndex(numberOfPatchesPerUnit),
  // Four coefficients for each cubic polynomial.  Two polynomial per patch.
  _coefficients(8 * numberOfPatchesPerUnit * 
		((openUpperBound + 1) - closedLowerBound)) {
  // Functors for evaluating the PDF and CDF.
  PoissonPdf<Number> pdf;
  PoissonCdf<Number> cdf;
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
    //
    // PDF
    //
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
					   _coefficients.begin() + 
					   8 * (index + j));
    }
    //
    // CDF
    //
    // First compute the values of the function and its derivatives.
    for (int j = 0; j != numberOfPatchesPerUnit + 1; ++j) {
      mean = _lowerBound + i + j * patchLength;
      f[j] = cdf(mean, mode);
      // Scale the derivative by the patch length.
      d[j] = - pdf(mean, mode) * patchLength;
    }
    // From these, compute the coefficients of the cubic polynomials:
    // a + b t + c t^2 + d t^3
    for (int j = 0; j != numberOfPatchesPerUnit; ++j) {
      computeHermitePolynomialCoefficients(f[j], f[j + 1], d[j], d[j + 1],
					   _coefficients.begin() + 
					   8 * (index + j) + 4);
    }
    index += numberOfPatchesPerUnit;
  }
}


// Copy constructor.
template<typename T>
inline
PoissonPdfCdfAtTheMode<T>::
PoissonPdfCdfAtTheMode(const PoissonPdfCdfAtTheMode& other) :
  _lowerBound(other._lowerBound),
  _scaleToIndex(other._scaleToIndex),
  _coefficients(other._coefficients)
{}


// Assignment operator.
template<typename T>
inline
PoissonPdfCdfAtTheMode<T>&
PoissonPdfCdfAtTheMode<T>::
operator=(const PoissonPdfCdfAtTheMode& other) {
  if (this != &other) {
    _lowerBound = other._lowerBound;
    _scaleToIndex = other._scaleToIndex;
    _coefficients = other._coefficients;
  }
  return *this;
}


// Evaluate the probability density function and cumulative distribution 
// function at the mode.
template<typename T>
inline
void
PoissonPdfCdfAtTheMode<T>::
evaluate(Number x, Number* pdf, Number* cdf) const {
  // Scale the argument to the array index range.
  x = (x - _lowerBound) * _scaleToIndex;
  // The index of the patch.
  const int index = int(x);
  // Perform the Hermite interpolation.
  x -= index;
  *pdf = evaluatePolynomial<3>(x, _coefficients.begin() + 8 * index);
  *cdf = evaluatePolynomial<3>(x, _coefficients.begin() + 8 * index + 4);
}


END_NAMESPACE_NUMERICAL

// End of file.
