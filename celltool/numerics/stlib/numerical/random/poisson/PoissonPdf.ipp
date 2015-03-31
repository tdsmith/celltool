// -*- C++ -*-

#if !defined(__numerical_random_PoissonPdf_ipp__)
#error This file is an implementation detail of PoissonPdf.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Return the Poisson cumulative distribution function.
template<typename T>
inline
typename PoissonPdf<T>::result_type
PoissonPdf<T>::
operator()(first_argument_type mean, second_argument_type n) {
#ifdef DEBUG_numerical_PoissonPdf
  assert(mean >= 0);
  assert(n >= 0);
#endif

  // Check this special case so we don't take the logarithm of 0.
  if (mean == 0) {
    if (n == 0) {
      return 1;
    }
    // else
    return 0;
  }

  // Compute the result by taking the logarithm and then the exponential.
  return std::exp(-mean + n * std::log(mean) - _logarithmOfGamma(n + 1));
}


// Return the derivative (with respect to the mean) of the Poisson 
// probability density function.
template<typename T>
inline
typename PoissonPdf<T>::result_type
PoissonPdf<T>::
computeDerivative(first_argument_type mean, second_argument_type n) {
  if (mean == 0) {
    if (n == 0) {
      return -1;
    }
    // else
    return 0;
  }
  return  (n / mean - 1) * operator()(mean, n);
}

END_NAMESPACE_NUMERICAL

// End of file.
