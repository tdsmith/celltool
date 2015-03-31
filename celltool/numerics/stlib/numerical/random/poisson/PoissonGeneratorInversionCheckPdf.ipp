// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionCheckPdf_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionCheckPdf.
#endif

#define __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__
#include "PoissonGeneratorInversionMaximumMean.ipp"
#undef __numerical_random_PoissonGeneratorInversionMaximumMean_ipp__

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionCheckPdf<T, Uniform>::result_type
PoissonGeneratorInversionCheckPdf<T, Uniform>::
operator()(const argument_type mean) {
#ifdef DEBUG_numerical_PoissonGeneratorInversionCheckPdf
  // If the mean is too large, we will get underflow in computing p.
  // The algorithm will give incorrect results.
  assert(mean < Number(PoissonGeneratorInversionMaximumMean<Number>::Value));
#endif

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

  // If the random number is very close to 1, then the cumulative distribution
  // function f may never exceed its value because of round-off error in
  // calculating it.  
  // Let M = PoissonGeneratorInversionCheckPdfMaximumMean::Value and
  // E = std::numeric_limits<Number>::epsilon().  Consider the simple version
  // of the loop below.  The loop below will never
  // never iterate more than 2 * M times.  This is because p will fall below 
  // E before that happens.  For double precision numbers, 
  // p = e^(-708) 708^(2 * 708) / (2 * 708)! = 1.76e-121 < E.
  // For single precision numbers, 
  // p = e^(-87) 87^(2 * 87) / (2 * 87)! = 7.67e-17 < E.
  // Thus we can bound the round-off error by 2 M E.
  const Number threshholdForCheckingRoundOffError = 1 - 
    2 * PoissonGeneratorInversionMaximumMean<Number>::Value * 
    std::numeric_limits<Number>::epsilon();

  // Probability density function.
  Number p = std::exp(-mean);
  // Cumulative distribution function.
  Number f = p;
  // I considered storing n as a floating point number to avoid converting 
  // it in the following loop.  However, it does not affect performance so
  // I left it as an integer.
  int n = 0;

  if (r < threshholdForCheckingRoundOffError) {
    while (f < r) {
      ++n;
      p *= mean / n;
      f += p;
    }
  }
  else {
    // If p drops below epsilon in the upper tail of the distribution, 
    // then f will not further increase.  This is because f is close to 1 so 
    // adding a number less than epsilon to it does not change its value.
    // Without this break, the algorithm would enter an infinite loop.
    while (f < r && 
	   ! (n > mean && p < std::numeric_limits<Number>::epsilon())) {
      ++n;
      p *= mean / n;
      f += p;
    }
  }

  return n;
}


END_NAMESPACE_NUMERICAL

// End of file.
