// -*- C++ -*-

#if !defined(__numerical_specialFunctions_LogarithmOfFactorial_ipp__)
#error This file is an implementation detail of LogarithmOfFactorial.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
T
computeLogarithmOfFactorial(const int n) {
  assert(n >= 0);
  //
  // I use the property that log(product(n)) = sum(log(n)).
  //
  T result = 0;
  // Below I use floating point types to avoid converting when taking the
  // logarithm.
  const T size(n);
  for (T i = 1; i <= size; ++i) {
    result += std::log(i);
  }
  return result;
}

END_NAMESPACE_NUMERICAL
