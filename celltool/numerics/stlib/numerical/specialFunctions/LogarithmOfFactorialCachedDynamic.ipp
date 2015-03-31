// -*- C++ -*-

#if !defined(__numerical_specialFunctions_LogarithmOfFactorialCachedDynamic_ipp__)
#error This file is an implementation detail of LogarithmOfFactorialCachedDynamic.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
void
LogarithmOfFactorialCachedDynamic<T>::
fillTable(const int maximumArgument) const{
#ifdef DEBUG_numerical_LogarithmOfFactorialCachedDynamic
  assert(maximumArgument >= int(_values.size()));
#endif

  // Make sure the table has been initialized.
  if (_values.empty()) {
    _values.push_back(0);
  }

  // Part of the old table is already filled with correct values.
  // Fill in the rest of the table with log(n!).
  for (int i = _values.size(); i <= maximumArgument; ++i) {
    _values.push_back(_values[i-1] + std::log(Number(i)));
  }
}


END_NAMESPACE_NUMERICAL

// End of file.
