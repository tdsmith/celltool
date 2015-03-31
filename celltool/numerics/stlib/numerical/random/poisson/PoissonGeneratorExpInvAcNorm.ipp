// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorExpInvAcNorm_ipp__)
#error This file is an implementation detail of PoissonGeneratorExpInvAcNorm.
#endif

BEGIN_NAMESPACE_NUMERICAL


//! Threshhold for whether one should use the exponential inter-arrival method or the inversion method in computing a Poisson deviate.
template<typename T, class Generator>
class PdeianExpVsInv {
public:
  //! Use the exponential inter-arrival method for means less than this value.
  static
  T
  getThreshhold() {
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    return 0.4;
#else
    return 2.0;
#endif
  }
};


//! Threshhold for whether one should use the inversion method or the acceptance-complement method in computing a Poisson deviate.
template<typename T, class Generator>
class PdeianInvVsAc {
public:
  //! Use the inversion method for means less than this value.
  static
  T
  getThreshhold() {
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
    return 13;
#else
    return 6.5;
#endif
  }
};


// Construct using the exponential generator, the normal generator, 
// and the threshhold.
template<typename T, 
	 class Uniform,
	 template<typename, class> class Exponential,
	 template<typename, class> class Normal>
inline
PoissonGeneratorExpInvAcNorm<T, Uniform, Exponential, Normal>::
PoissonGeneratorExpInvAcNorm(ExponentialGenerator* exponentialGenerator,
			   NormalGenerator* normalGenerator,
			   Number normalThreshhold) :
  _exponentialInterArrival(exponentialGenerator),
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
  _inversion(exponentialGenerator->getDiscreteUniformGenerator(), 
	     PdeianInvVsAc<T, Uniform>::getThreshhold()),
#else
  _inversion(exponentialGenerator->getDiscreteUniformGenerator()),
#endif
  _acceptanceComplementWinrand(normalGenerator),
  _normal(normalGenerator),
  _normalThreshhold(normalThreshhold)
{}


template<typename T, 
	 class Uniform,
	 template<typename, class> class Exponential,
	 template<typename, class> class Normal>
inline
typename PoissonGeneratorExpInvAcNorm<T, Uniform, Exponential, Normal>::result_type
PoissonGeneratorExpInvAcNorm<T, Uniform, Exponential, Normal>::
operator()(const argument_type mean) {
  // If the mean is very small, use the exponential inter-arrival method.
  if (mean < PdeianExpVsInv<T, Uniform>::getThreshhold()) {
    return _exponentialInterArrival(mean);
  }
  // Use the inversion method for small means.
  if (mean < PdeianInvVsAc<T, Uniform>::getThreshhold()) {
    return _inversion(mean);
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
