// -*- C++ -*-

#if !defined(__numerical_specialFunctions_HarmonicNumber_ipp__)
#error This file is an implementation detail of HarmonicNumber.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Return the n_th harmonic number.
template<typename T>
inline
T
computeHarmonicNumber(int n) {
  assert(n >= 0);
  T result = 0;
  while (n != 0) {
    result += 1.0 / n;
    --n;
  }
  return result;
}


//! Compute the difference of two harmonic numbers.
template<typename T>
inline
T
computeDifferenceOfHarmonicNumbers(const int m, const int n) {
  assert(m >= 0 && n >= 0);

  if (m < n) {
    return - computeDifferenceOfHarmonicNumbers<T>(n, m);
  }

  // For efficiency, we don't call the computeHarmonicNumber() function,
  // We directly compute the difference.
  T result = 0;
  for (int i = n + 1; i <= m; ++i) {
    result += 1.0 / i;
  }
  return result;
}


END_NAMESPACE_NUMERICAL

// End of file.
