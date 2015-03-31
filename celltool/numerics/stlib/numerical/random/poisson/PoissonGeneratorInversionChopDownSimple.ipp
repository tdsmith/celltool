// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionChopDownSimple_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionChopDownSimple.
#endif

#define __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__
#include "PoissonGeneratorInversionMaximumMean.ipp"
#undef __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionChopDownSimple<T, Uniform>::result_type
PoissonGeneratorInversionChopDownSimple<T, Uniform>::
operator()(const argument_type mean) {
#ifdef DEBUG_numerical_PoissonGeneratorInversionChopDownSimple
  // If the mean is too large, we will get underflow in computing p.
  // The algorithm will give incorrect results.
  assert(mean < Number(PoissonGeneratorInversionMaximumMean<Number>::Value));
#endif

  // CONTINUE
  const int NumericalFailureBound = 
    2 * PoissonGeneratorInversionMaximumMean<Number>::Value;
  const Number exponential = std::exp(-mean);
  while (true) {
    // Poisson random deviate.
    int deviate = 0;
    // Scaled probability density function.
    Number pdf = exponential;
    // Uniform deviate.
    Number u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
    do {
      u -= pdf;
      if (u <= 0) {
	return deviate;
      }
      ++deviate;
      pdf *= mean / deviate;
    } while (deviate != NumericalFailureBound);
  }
}


END_NAMESPACE_NUMERICAL

// End of file.
