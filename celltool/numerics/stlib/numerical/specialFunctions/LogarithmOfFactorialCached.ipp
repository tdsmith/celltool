// -*- C++ -*-

#if !defined(__numerical_specialFunctions_LogarithmOfFactorialCached_ipp__)
#error This file is an implementation detail of LogarithmOfFactorialCached.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
LogarithmOfFactorialCached<T>::
LogarithmOfFactorialCached(const int size) :
  _values(size) {
  // First fill the table with log(n).
  if (size != 0) {
    _values[0] = 0;
  }
  for (int i = 1; i < size; ++i) {
    _values[i] = std::log(Number(i));
  }
  // Next use partial sums to get log(n!).
  std::partial_sum(_values.begin(), _values.end(), _values.begin());
}


END_NAMESPACE_NUMERICAL
