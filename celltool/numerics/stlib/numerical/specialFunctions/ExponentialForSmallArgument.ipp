// -*- C++ -*-

#if !defined(__numerical_specialFunctions_ExponentialForSmallArgument_ipp__)
#error This file is an implementation detail of ExponentialForSmallArgument.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
typename ExponentialForSmallArgument<T>::Number
ExponentialForSmallArgument<T>::
operator()(const Number x) const {
  const Number absX = std::abs(x);

  if (absX >= _threshhold7) {
    return std::exp(x);
  }
  else if (absX < _threshhold1) {
    return 1.0;
  }
  else if (absX < _threshhold2) {
    return 1.0 + x;
  }
  else if (absX < _threshhold3) {
    return 1.0 + x + x * x / 2;
  }
  else if (absX < _threshhold4) {
    return 1.0 + x + x * x / 2 + x * x * x / 6;
  }
  else if (absX < _threshhold5) {
    return 1.0 + x + x * x / 2 + x * x * x / 6 + x * x * x * x / 24;
  }
  else if (absX < _threshhold6) {
    return 1.0 + x + x * x / 2 + x * x * x / 6 + x * x * x * x / 24 
      + x * x * x * x * x / 120;
  }
  // else if (absX < _threshhold7)
  return 1.0 + x + x * x / 2 + x * x * x / 6 + x * x * x * x / 24 
    + x * x * x * x * x / 120 + x * x * x * x * x * x / 720;
}


template<typename T>
inline
void
ExponentialForSmallArgument<T>::
printThreshholds(std::ostream& out) const {
  out << _threshhold1 << "\n"
      << _threshhold2 << "\n"
      << _threshhold3 << "\n"
      << _threshhold4 << "\n"
      << _threshhold5 << "\n"
      << _threshhold6 << "\n"
      << _threshhold7 << "\n";
}


END_NAMESPACE_NUMERICAL

// End of file.
