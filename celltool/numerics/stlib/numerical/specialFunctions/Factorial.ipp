// -*- C++ -*-

#if !defined(__numerical_specialFunctions_Factorial_ipp__)
#error This file is an implementation detail of Factorial.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
T
computeFactorial(int n) {
  assert(n >= 0);
  T result = 1;
  while (n > 1) {
    result *= n;
    --n;
  }
  return result;
}


END_NAMESPACE_NUMERICAL

// End of file.
