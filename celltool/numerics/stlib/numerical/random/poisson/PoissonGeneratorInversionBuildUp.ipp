// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionBuildUp_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionBuildUp.
#endif

#define __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__
#include "PoissonGeneratorInversionMaximumMean.ipp"
#undef __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionBuildUp<T, Uniform>::result_type
PoissonGeneratorInversionBuildUp<T, Uniform>::
operator()(const argument_type mean) {
#ifdef DEBUG_numerical_PoissonGeneratorInversionBuildUp
  // If the mean is too large, we will get underflow in computing p.
  // The algorithm will give incorrect results.
  assert(mean < Number(PoissonGeneratorInversionMaximumMean<Number>::Value));
#endif

#ifdef NUMERICAL_POISSON_SMALL_MEAN
  // This helps if the mean is zero a significant fraction of the
  // time.  This lets us skip computing a random number.
  if (mean == 0) {
    return 0;
  }

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
    _oldExponential = std::exp(-mean);
  }
#else
  const Number exponential = std::exp(-mean);
#endif

  // CONTINUE
  const int NumericalFailureBound = 
    2 * PoissonGeneratorInversionMaximumMean<Number>::Value;

#ifdef NUMERICAL_POISSON_SMALL_MEAN
  // Poisson random deviate.
  int deviate = 0;
  // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
  Number scaledPdf = _oldExponential;
#else
  Number scaledPdf = exponential;
#endif
  // Transformed cumulative distribution function.
  Number transformedCdf = scaledPdf - r;

  while (true) {
    if (transformedCdf >= 0) {
      return deviate;
    }
    ++deviate;
    scaledPdf *= mean;
    transformedCdf *= deviate;
    transformedCdf += scaledPdf;
    if (deviate == NumericalFailureBound) {
      // Start over.
      deviate = 0;
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
      scaledPdf = _oldExponential;
#else
      scaledPdf = exponential;
#endif
      transformedCdf = scaledPdf - 
	transformDiscreteDeviateToContinuousDeviateClosed<Number>
	((*_discreteUniformGenerator)());
    }
  }
#else
  while (true) {
    // Poisson random deviate.
    int deviate = 0;
    // Scaled probability density function.
#ifdef NUMERICAL_POISSON_CACHE_OLD_MEAN
    Number scaledPdf = _oldExponential;
#else
    Number scaledPdf = exponential;
#endif
    // Transformed cumulative distribution function.
    Number transformedCdf = scaledPdf - 
      transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
    do {
      if (transformedCdf >= 0) {
	return deviate;
      }
      ++deviate;
      scaledPdf *= mean;
      transformedCdf *= deviate;
      transformedCdf += scaledPdf;
    } while (deviate != NumericalFailureBound);
  }
#endif
}


END_NAMESPACE_NUMERICAL

// End of file.
