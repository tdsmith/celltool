// -*- C++ -*-

#if !defined(__numerical_random_PoissonPdfCached_ipp__)
#error This file is an implementation detail of PoissonPdfCached.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Return the Poisson cumulative distribution function.
template<typename T>
inline
typename PoissonPdfCached<T>::result_type
PoissonPdfCached<T>::
operator()(first_argument_type mean, second_argument_type n) {
  assert(mean >= 0);
  assert(n >= 0);

  // Check this special case so we don't take the logarithm of 0.
  if (mean == 0) {
    if (n == 0) {
      return 1;
    }
    // else
    return 0;
  }

  // Compute the result by taking the logarithm and then the exponential.
  return std::exp(-mean + n * std::log(mean) - _logarithmOfFactorial(n));
}


END_NAMESPACE_NUMERICAL

// End of file.
