// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionChopDownUnrolled_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionChopDownUnrolled.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionChopDownUnrolled<T, Uniform>::result_type
PoissonGeneratorInversionChopDownUnrolled<T, Uniform>::
operator()(const argument_type mean) {
#ifdef DEBUG_numerical_PoissonGeneratorInversionChopDownUnrolled
  // If the mean is too large, we will get underflow in computing p.
  // The algorithm will give incorrect results.
  assert(mean < Number(PoissonGeneratorInversionMaximumMean<Number>::Value));
#endif

#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  if (mean != _oldMean) {
    _oldMean = mean;
    _oldExponential = _expNeg(mean);
  }
#else
  const Number exponential = _expNeg(mean);
#endif

  const Number mean4 = mean * mean * mean * mean;

  Number sum;
  while (true) {
    typename ads::Array<1, Number>::const_iterator iterator = _inverse.begin();
    // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    Number pdf0 = _oldExponential;
#else
    Number pdf0 = exponential;
#endif
    Number pdf1 = exponential * mean;
    Number pdf2 = exponential * mean * mean * (1.0 / 2.0);
    Number pdf3 = exponential * mean * mean * mean * (1.0 / 6.0);
    // Uniform deviate.
    Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
    do {
      sum = pdf0 + pdf1 + pdf2 + pdf3;
      if (u < sum) {
	u -= pdf0;
	if (u < 0) {
	  return iterator - _inverse.begin();
	}
	u -= pdf1;
	if (u < 0) {
	  return iterator - _inverse.begin() + 1;
	}
	u -= pdf2;
	if (u < 0) {
	  return iterator - _inverse.begin() + 2;
	}
	return iterator - _inverse.begin() + 3;
      }
      u -= sum;
      iterator += 4;
      pdf0 = pdf0 * mean4 * *iterator;
      pdf1 = pdf1 * mean4 * *(iterator + 1);
      pdf2 = pdf2 * mean4 * *(iterator + 2);
      pdf3 = pdf3 * mean4 * *(iterator + 3);
    } while (iterator < _inverse.end() - 4);
  }
}


END_NAMESPACE_NUMERICAL

// End of file.
