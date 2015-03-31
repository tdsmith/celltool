// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorDirectNr_ipp__)
#error This file is an implementation detail of PoissonGeneratorDirectNr.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorDirectNr<T, Uniform>::result_type
PoissonGeneratorDirectNr<T, Uniform>::
operator()(const argument_type mean) {
  // If the mean is new, compute the exponential.
  if (mean != _oldm) {
    _oldm = mean;
    _g = std::exp(- mean);
  }

  Number em = -1;
  Number t = 1.0;
  do {
    ++em;
    t *= transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
  } while (t > _g);

  return int(em);
}


END_NAMESPACE_NUMERICAL

// End of file.
