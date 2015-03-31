// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorExpAcNorm_ipp__)
#error This file is an implementation detail of PoissonGeneratorExpAcNorm.
#endif

BEGIN_NAMESPACE_NUMERICAL


//! Threshhold for whether one should use the exponential inter-arrival method or the acceptance-complement method in computing a Poisson deviate.
/*!
  For the specializations I tested the code on an Intel core duo, compiled 
  with GNU g++ 4.0 using the flags: -O3 -funroll-loops -fstrict-aliasing.  
*/
template<class Generator>
class PoissonExpVsAc {
public:
  //! Use the exponential inter-arrival method for means less than this value.
  enum {Threshhold = 3};
};



template<typename T, 
	 class Uniform,
	 template<typename, class> class Exponential,
	 template<typename, class> class Normal>
inline
typename PoissonGeneratorExpAcNorm<T, Uniform, Exponential, Normal>::result_type
PoissonGeneratorExpAcNorm<T, Uniform, Exponential, Normal>::
operator()(const argument_type mean) {
  // If the mean is small, use the exponential inter-arrival method.
  if (mean < PoissonExpVsAc<DiscreteUniformGenerator>::Threshhold) {
    return _exponentialInterArrival(mean);
  }
  // Use acceptance-complement for medium values.
  if (mean < _normalThreshhold) {
    return _acceptanceComplementWinrand(mean);
  }
  // Use normal approximation for large means.
  return _normal(mean);
}


END_NAMESPACE_NUMERICAL

// End of file.
