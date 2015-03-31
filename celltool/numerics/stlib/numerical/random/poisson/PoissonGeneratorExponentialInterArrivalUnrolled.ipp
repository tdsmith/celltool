// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorExponentialInterArrivalUnrolled_ipp__)
#error This file is an implementation detail of PoissonGeneratorExponentialInterArrivalUnrolled.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, 
	 class Uniform,
	 template<typename, class> class Exponential>
inline
typename PoissonGeneratorExponentialInterArrivalUnrolled<T, Uniform, Exponential>::result_type
PoissonGeneratorExponentialInterArrivalUnrolled<T, Uniform, Exponential>::
operator()(argument_type mean) {
  mean -= (*_exponentialGenerator)();
  if (mean < 0) {
    return 0;
  }
  mean -= (*_exponentialGenerator)();
  if (mean < 0) {
    return 1;
  }

  return 2;
}


END_NAMESPACE_NUMERICAL

// End of file.
