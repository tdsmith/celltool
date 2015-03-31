// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorDirectRejectionNr_ipp__)
#error This file is an implementation detail of PoissonGeneratorDirectRejectionNr.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorDirectRejectionNr<T, Uniform>::result_type
PoissonGeneratorDirectRejectionNr<T, Uniform>::
operator()(const argument_type mean) {
  // If the mean is small, use the direct method.
  if (mean < 12.0) {
    return _directNr(mean);
  }
  // Otherwise, use the rejection method.
  return _rejectionNr(mean);
}


END_NAMESPACE_NUMERICAL

// End of file.
