// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionTable_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionTable.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionTable<T, Uniform>::result_type
PoissonGeneratorInversionTable<T, Uniform>::
operator()(const argument_type mean) {
  static Number oldMean = -1.0;
  static Number p, q, p0, pp[36];
  static int ll, m;
  Number u;
  int k,i;

#ifdef DEBUG_numerical_PoissonGeneratorInversionTable
  assert(mean <= 10);
#endif
  if (mean != oldMean) {
    oldMean = mean;
    m = (mean > 1.0)? int(mean) : 1;
    ll = 0;
    p0 = q = p = std::exp(-mean);
  }
  for(;;) {
    // Uniform sample
    u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
    k = 0;
    if (u <= p0) {
      return k;
    }
    // Table comparison
    if (ll != 0) {              
      i = (u > 0.458) ? std::min(ll,m) : 1;
      for (k = i; k <=ll; k++) {
	if (u <= pp[k]) {
	  return k;
	}
      }
      if (ll == 35) {
	continue;
      }
    }
    // Step C. Creation of new prob.
    for (k = ll +1; k <= 35; k++) { 
      p *= mean/Number(k);
      q += p;
      pp[k] = q;
      if (u <= q) {
	ll = k;
	return k;
      }
    }
    ll = 35;
  }
}


END_NAMESPACE_NUMERICAL

// End of file.
