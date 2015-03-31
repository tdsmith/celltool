// -*- C++ -*-

#if !defined(__numerical_specialFunctions_BinomialCoefficient_ipp__)
#error This file is an implementation detail of BinomialCoefficient.
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T>
inline
T
computeBinomialCoefficient(int n, int k) {
  if (n == 0) {
    if (k == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
#ifdef DEBUG_numerical_BinomialCoefficient
  assert(k >= 0 && k <= n);
#endif
  T result = 1;
  for (int i = 0; i != k; ++i) {
    result *= (n - i);
    result /= (i + 1);
  }
  return result;
}


// Return the derivative (with respect to the first argument of the binomial 
// coefficient.
// D[Binomial[n,k],n] == 
// Binomial[n,k] (HarmonicNumber[n] - HarmonicNumber[n-k])
template<typename T>
inline
T
computeBinomialCoefficientDerivative(int n, int k) {
  return computeDifferenceOfHarmonicNumbers<T>(n, n - k) * 
    computeBinomialCoefficient<T>(n, k);
}


END_NAMESPACE_NUMERICAL

// End of file.
