// -*- C++ -*-

#if !defined(__numerical_specialFunctions_Gamma_ipp__)
#error This file is an implementation detail of Gamma.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
typename LogarithmOfGamma<T>::Number
LogarithmOfGamma<T>::
operator()(const Number x) const {
  Number y, tmp, ser;

  y = x;
  tmp = x + 5.5;
  tmp -= (x + 0.5) * std::log(tmp);
  ser = 1.000000000190015;
  for (int j = 0; j != _cof.size(); ++j) {
    ser += _cof[j] / ++y;
  }
  return - tmp + std::log(2.5066282746310005 * ser / x);
}

END_NAMESPACE_NUMERICAL

// End of file.
