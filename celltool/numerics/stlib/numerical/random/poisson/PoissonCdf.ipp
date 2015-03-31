// -*- C++ -*-

#if !defined(__numerical_random_PoissonCdf_ipp__)
#error This file is an implementation detail of PoissonCdf.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Return the Poisson cumulative distribution function.
template<typename T>
inline
typename PoissonCdf<T>::result_type
PoissonCdf<T>::
operator()(first_argument_type mean, second_argument_type n) {
  assert(mean >= 0);
  assert(n >= 0);
  
  // A number that is a little bit bigger that the smallest number that
  // can be represented.
  const Number VerySmall = 100 * std::numeric_limits<Number>::min();

  // Probability density function.
  Number pdf = std::exp(-mean);
  // Cumulative distribution function.
  Number cdf = pdf;
  // If we can represent the first pdf with a floating point number.
  if (pdf > VerySmall) {
    // CONTINUE: I may get a more accurate answer by summing the lower and
    // upper halves from the tail toward the center.
    for (int i = 1; i <= n; ++i) {
      pdf *= mean / i;
      cdf += pdf;
    }
  }
  // Otherwise, computing the pdf causes underflow.
  else {
    PoissonPdf<Number> f;
    int i = 1;
    // Loop until the pdf is large enough to be represented.
    for (; i <= n && pdf < VerySmall; ++i) {
      pdf = f(mean, i);
      cdf += pdf;
    }
    // Then use the recursive formula for the pdf.
    for (; i <= n && pdf >= VerySmall; ++i) {
      pdf *= mean / i;
      cdf += pdf;
    }
  }
  return cdf;
}


END_NAMESPACE_NUMERICAL

// End of file.
