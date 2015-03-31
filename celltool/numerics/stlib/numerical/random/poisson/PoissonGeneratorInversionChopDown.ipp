// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionChopDown_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionChopDown.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionChopDown<T, Uniform>::result_type
PoissonGeneratorInversionChopDown<T, Uniform>::
operator()(const argument_type mean) {
#ifdef DEBUG_numerical_PoissonGeneratorInversionChopDown
  // If the mean is too large, we will get underflow in computing p.
  // The algorithm will give incorrect results.
  assert(mean < Number(PoissonGeneratorInversionMaximumMean<Number>::Value));
#endif

#ifdef NUMERICAL_POISSON_ZERO_MEAN
  // This helps if the mean is zero a significant fraction of the
  // time.  This lets us skip computing a random number.
  if (mean == 0) {
    return 0;
  }
#endif

#ifdef NUMERICAL_POISSON_SMALL_MEAN
  // Calculate a uniform random deviate.
  const Number r = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_discreteUniformGenerator)());

  // Recall that exp(-x) = 1 - x + x^2 / 2! - x^3 / 3! + ...
  // Note that 1 - x <= exp(-x) for all x.  The two functions and their first
  // derivatives are equal at x = 0, and the latter is convex (f''(x) > 0).
  // Thus r <= 1 - mean implies that r <= exp(-mean).  We check this condition
  // to avoid having to compute the exponential function for small means.
  if (r <= 1 - mean) {
    return 0;
  }
#endif

#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  if (mean != _oldMean) {
    _oldMean = mean;
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    _oldExponential = expNeg(mean);
#else
    _oldExponential = std::exp(-mean);
#endif
  }
#else
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
  const Number exponential = _expNeg(mean);
#else
  const Number exponential = std::exp(-mean);
#endif
#endif

#ifndef NUMERICAL_POISSON_STORE_INVERSE
  // CONTINUE
  const int NumericalFailureBound = 
    2 * PoissonGeneratorInversionMaximumMean<Number>::Value;
#endif

#ifdef NUMERICAL_POISSON_SMALL_MEAN

#ifdef NUMERICAL_POISSON_STORE_INVERSE

  typename ads::Array<1, Number>::const_iterator iterator = _inverse.begin();
  // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  Number scaledPdf = _oldExponential;
#else
  Number scaledPdf = exponential;
#endif
  // Uniform deviate.
  Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_discreteUniformGenerator)());

  while (true) {
    u -= scaledPdf;
    if (u <= 0) {
      return iterator - _inverse.begin();
    }
    scaledPdf *= mean * *++iterator;
    if (iterator == _inverse.end()) {
      // Start over.
      iterator = _inverse.begin();
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
      scaledPdf = _oldExponential;
#else
      scaledPdf = exponential;
#endif
      u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_discreteUniformGenerator)());
    }
  }

#else // NUMERICAL_POISSON_STORE_INVERSE

  // Poisson random deviate.
  int deviate = 0;
  // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  Number scaledPdf = _oldExponential;
#else
  Number scaledPdf = exponential;
#endif
  // Uniform deviate.
  Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_discreteUniformGenerator)());

  while (true) {
    u -= scaledPdf;
    if (u <= 0) {
      return deviate;
    }
    ++deviate;
    scaledPdf *= mean;
    u *= deviate;
    if (deviate == NumericalFailureBound) {
      // Start over.
      deviate = 0;
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
      scaledPdf = _oldExponential;
#else
      scaledPdf = exponential;
#endif
      u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_discreteUniformGenerator)());
    }
  }

#endif // NUMERICAL_POISSON_STORE_INVERSE

#else // NUMERICAL_POISSON_SMALL_MEAN

#ifdef NUMERICAL_POISSON_STORE_INVERSE

  while (true) {
    typename ads::Array<1, Number>::const_iterator iterator = _inverse.begin();
    // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    Number scaledPdf = _oldExponential;
#else
    Number scaledPdf = exponential;
#endif
    // Uniform deviate.
    Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)()) - scaledPdf;
    do {
      if (u <= 0) {
	return iterator - _inverse.begin();
      }
      scaledPdf *= mean * *++iterator;
      u -= scaledPdf;
    } while (iterator != _inverse.end());
  }

#else // NUMERICAL_POISSON_STORE_INVERSE

  while (true) {
    // Poisson random deviate.
    int deviate = 0;
    // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    Number scaledPdf = _oldExponential;
#else
    Number scaledPdf = exponential;
#endif
    // Uniform deviate.
    Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)()) - scaledPdf;
    do {
      if (u <= 0) {
	return deviate;
      }
      ++deviate;
      scaledPdf *= mean;
      u *= deviate;
      u -= scaledPdf;
    } while (deviate != NumericalFailureBound);
  }

#endif // NUMERICAL_POISSON_STORE_INVERSE

#endif // NUMERICAL_POISSON_SMALL_MEAN
}


END_NAMESPACE_NUMERICAL

// End of file.
