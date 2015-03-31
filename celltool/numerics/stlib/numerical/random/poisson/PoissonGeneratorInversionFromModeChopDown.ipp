// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionFromModeChopDown_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionFromModeChopDown.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionFromModeChopDown<T, Uniform>::result_type
PoissonGeneratorInversionFromModeChopDown<T, Uniform>::
operator()(const argument_type mean) {
#ifdef NUMERICAL_POISSON_SMALL_MEAN
  // This helps if the mean is zero a significant fraction of the
  // time.  This lets us skip computing a random number.
  if (mean == 0) {
    return 0;
  }

  // Calculate a uniform random deviate.
  Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_discreteUniformGenerator)());

  // Recall that exp(-x) = 1 - x + x^2 / 2! - x^3 / 3! + ...
  // Note that 1 - x <= exp(-x) for all x.  The two functions and their first
  // derivatives are equal at x = 0, and the latter is convex (f''(x) > 0).
  // Thus u <= 1 - mean implies that u <= exp(-mean).  We check this condition
  // to avoid having to compute the exponential function for small means.
  if (u <= 1 - mean) {
    return 0;
  }
#endif

  // Greatest integer function (floor).
  const int mode = int(mean);

#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  if (mean != _oldMean) {
    _oldMean = mean;
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    _oldPdf = _pdf(mean);
#else
    _oldPdf = _pdf(mean, mode);
#endif
  }
#else
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
  const Number pdf = _pdf(mean);
#else
  const Number pdf = _pdf(mean, mode);
#endif
#endif

  // CONTINUE
  const int NumericalFailureBound = 2 * mode + 100;

  while (true) {
    // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    Number lowerPdf = _oldPdf, upperPdf = _oldPdf;
#else
    Number lowerPdf = pdf, upperPdf = pdf;
#endif
#ifdef NUMERICAL_POISSON_SMALL_MEAN
    u -= lowerPdf;
#else
    // Uniform deviate.
    Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)()) - lowerPdf;
#endif
    // First check the mode.
    if (u <= 0) {
      return mode;
    }

    // Then do an alternating search from the mode.
    int deviate;
    for (int i = 1; i <= mode; ++i) {
      // Lower.
      deviate = mode - i;
      lowerPdf *= deviate + 1;
      upperPdf *= mean;
      u *= mean;
      u -= lowerPdf;
      if (u <= 0) {
	return deviate;
      }
      // Upper.
      deviate = mode + i;
      upperPdf *= mean;
      lowerPdf *= deviate;
      u *= deviate;
      u -= upperPdf;
      if (u <= 0) {
	return deviate;
      }
    } 

    // Finally do an upward search from 2 * mode + 1.
    for (deviate = 2 * mode + 1; deviate != NumericalFailureBound; ++deviate) {
      upperPdf *= mean;
      u *= deviate;
      u -= upperPdf;
      if (u <= 0) {
	return deviate;
      }
    }
#ifdef NUMERICAL_POISSON_SMALL_MEAN
    u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
#endif
  }
}


END_NAMESPACE_NUMERICAL

// End of file.
