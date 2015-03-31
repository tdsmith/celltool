// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorRejectionNr_ipp__)
#error This file is an implementation detail of PoissonGeneratorRejectionNr.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorRejectionNr<T, Uniform>::result_type
PoissonGeneratorRejectionNr<T, Uniform>::
operator()(const argument_type mean) {
  Number em, t, y;

  // If the mean is new then compute some functions that are used below.
  if (mean != _oldm) {
    _oldm = mean;
    _sq = std::sqrt(2.0 * mean);
    _alxm = std::log(mean);
    _g = mean * _alxm - _logarithmOfGamma(mean + 1.0);
  }

  do {
    do {
      y = std::tan(numerical::Constants<Number>::Pi() * 
		   transformDiscreteDeviateToContinuousDeviateOpen<Number>
		   ((*_discreteUniformGenerator)()));
      em = _sq * y + mean;
    } while (em < 0.0);
    // The floor function is costly.
    //em = std::floor(em);
    em = int(em);
    t = 0.9 * (1.0 + y * y) * std::exp(em * _alxm - 
				       _logarithmOfGamma(em + 1.0) - _g);
  } while (transformDiscreteDeviateToContinuousDeviateClosed<Number>
	   ((*_discreteUniformGenerator)()) > t);

  return int(em);
}


END_NAMESPACE_NUMERICAL

// End of file.
