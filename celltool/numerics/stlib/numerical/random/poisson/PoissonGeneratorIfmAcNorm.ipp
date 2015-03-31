// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorIfmAcNorm_ipp__)
#error This file is an implementation detail of PoissonGeneratorIfmAcNorm.
#endif

BEGIN_NAMESPACE_NUMERICAL


//! Threshhold for whether one should use the inversion from the mode method or the acceptance-complement method in computing a Poisson deviate.
/*!
  For the specializations I tested the code on an Intel core duo, compiled 
  with GNU g++ 4.0 using the flags: -O3 -funroll-loops -fstrict-aliasing.  
*/
template<typename T, class Generator>
class PdianIfmVsAc {
public:
  //! Use the inversion from the mode method for means less than this value.
  static
  T
  getThreshhold() {
    return 45;
  }
};



// Construct using the normal generator and the threshhold.
template<typename T,
	 class Uniform,
	 template<typename, class> class Normal>
PoissonGeneratorIfmAcNorm<T, Uniform, Normal>::
PoissonGeneratorIfmAcNorm(NormalGenerator* normalGenerator,
			Number normalThreshhold) :
  _inversionFromTheMode(normalGenerator->getDiscreteUniformGenerator(),
			PdianIfmVsAc<T, Uniform>::getThreshhold()),
  _acceptanceComplementWinrand(normalGenerator),
  _normal(normalGenerator),
  _normalThreshhold(normalThreshhold)
{}


template<typename T, 
	 class Uniform,
	 template<typename, class> class Normal>
inline
typename PoissonGeneratorIfmAcNorm<T, Uniform, Normal>::result_type
PoissonGeneratorIfmAcNorm<T, Uniform, Normal>::
operator()(const argument_type mean) {
  // If the mean is small, use the inversion from the mode method.
  if (mean < PdianIfmVsAc<T, Uniform>::getThreshhold()) {
    return _inversionFromTheMode(mean);
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
