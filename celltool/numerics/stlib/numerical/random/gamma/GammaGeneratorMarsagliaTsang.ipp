// -*- C++ -*-

#if !defined(__numerical_random_GammaGeneratorMarsagliaTsang_ipp__)
#error This file is an implementation detail of GammaGeneratorMarsagliaTsang.
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T,
	 class Uniform,
	 template<typename, class> class Normal>
inline
typename GammaGeneratorMarsagliaTsang<T, Uniform, Normal>::result_type
GammaGeneratorMarsagliaTsang<T, Uniform, Normal>::
operator()(const Number a) {
#ifdef DEBUG_GammaGeneratorMarsagliaTsang
  assert(a >= 1);
#endif
  const Number d = a - 1. / 3.;
  const Number c = 1 / std::sqrt(9 * d);
  Number x, v, u;
  for (;;) {
    do {
      x = ((*_normalGenerator)());
      v = 1 + c * x;
    } while (v <= 0);
    v = v * v * v;
    u = transformDiscreteDeviateToContinuousDeviateOpen<Number>
      ((*_normalGenerator->getDiscreteUniformGenerator())());
    if (u < 1 - 0.331 * x * x * x * x) {
      return d * v;
    }
    if (std::log(u) < 0.5 * x * x + d * (1 - v + std::log(v))) {
      return d * v;
    }
  }
}

END_NAMESPACE_NUMERICAL

// End of file.
