// -*- C++ -*-

#if !defined(__numerical_random_PoissonCdfAtTheMode_ipp__)
#error This file is an implementation detail of PoissonCdfAtTheMode.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T>
inline
PoissonCdfAtTheMode<T>::
PoissonCdfAtTheMode(const int tableSize) :
  _function0(tableSize),
  _function1(tableSize),
  _derivative0(tableSize),
  _derivative1(tableSize) {
  assert(tableSize > 0);

  // CONTINUE: Reconcile the numerical differences between the two methods.
  PoissonPdf<Number> pdf;
  PoissonCdf<Number> cdf;
  _function0[0] = cdf(0, 0);
  _function1[0] = cdf(1, 0);
  _derivative0[0] = - pdf(0, 0);
  _derivative1[0] = - pdf(1, 0);
  Number pdfii;
  for (int i = 1; i != _function0.size(); ++i) {
    pdfii = pdf(i, i);
    _function0[i] = _function1[i-1] + pdfii;
    _function1[i] = cdf(i + 1, i);
    _derivative0[i] = - pdfii;
    _derivative1[i] = - pdf(i + 1, i);
  }
#if 0
  CONTINUE;
  for (int i = 0; i != _function0.size(); ++i) {
    _function0[i] = cdf(i, i);
    _function1[i] = cdf(i + 1, i);
    _derivative0[i] = - pdf(i, i);
    _derivative1[i] = - pdf(i + 1, i);
  }
#endif
}


template<typename T>
inline
typename PoissonCdfAtTheMode<T>::result_type
PoissonCdfAtTheMode<T>::
operator()(const argument_type mean) {
  // Greatest integer function (floor).
  const int mode = int(mean);
#ifdef DEBUG_PoissonCdfAtTheMode
  assert(mode >= 0 && mode < _function0.size());
#endif
#if 0
  // CONTINUE: REMOVE
  // Use linear interpolation.
  const Number x = mean - mode;
  return (1 - x) * _function0[mode] + x * _function1[mode];
#endif
  return hermiteInterpolate(mean - mode, _function0[mode], _function1[mode],
			    _derivative0[mode], _derivative1[mode]);
}


END_NAMESPACE_NUMERICAL

// End of file.
