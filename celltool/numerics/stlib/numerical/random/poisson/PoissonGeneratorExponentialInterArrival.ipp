// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorExponentialInterArrival_ipp__)
#error This file is an implementation detail of PoissonGeneratorExponentialInterArrival.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, 
	 class Uniform,
	 template<typename, class> class Exponential>
inline
typename PoissonGeneratorExponentialInterArrival<T, Uniform, Exponential>::result_type
PoissonGeneratorExponentialInterArrival<T, Uniform, Exponential>::
operator()(argument_type mean) {
  int deviate = -1;
  do {
    ++deviate;
    mean -= (*_exponentialGenerator)();
  } while (mean > 0);
  return deviate;
}


END_NAMESPACE_NUMERICAL

// End of file.
