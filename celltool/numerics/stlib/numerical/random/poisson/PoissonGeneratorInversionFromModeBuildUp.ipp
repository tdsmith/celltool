// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionFromModeBuildUp_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionFromModeBuildUp.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionFromModeBuildUp<T, Uniform>::result_type
PoissonGeneratorInversionFromModeBuildUp<T, Uniform>::
operator()(const argument_type mean) {
  // Greatest integer function (floor).
  const int mode = int(mean);

  // CONTINUE
  const int NumericalFailureBound = 2 * mode + 100;

  // Probability density function and cumulative distribution function.
#ifdef NUMERICAL_POISSON_HERMITE_APPROXIMATION
  Number pdfAtTheMode, cdfAtTheMode;
  _pdfCdf.evaluate(mean, &pdfAtTheMode, &cdfAtTheMode);
#else
  const Number pdfAtTheMode = _pdf(mean, mode);
  const Number cdfAtTheMode = _cdfAtTheMode(mean);
#endif

  while (true) {
    // Probability density function.
    Number pdf = pdfAtTheMode;
    // Compute the cdf at the mean minus a uniform random deviate.
    Number cdf = cdfAtTheMode - 
      transformDiscreteDeviateToContinuousDeviateClosed<Number>
      ((*_discreteUniformGenerator)());
    int n = mode;
    if (cdf < 0) {
      while (n != NumericalFailureBound) {
	++n;
	pdf *= mean;
	cdf *= n;
	cdf += pdf;
	if (cdf >= 0) {
	  return n;
	}
      }
    }
    else {
      while (n >= 0) {
	cdf -= pdf;
	if (cdf <= 0) {
	  return n;
	}
	// I don't need to worry about division by zero.  If the mean were zero,
	// the other branch of the if statment would execute.
	pdf *= n;
	cdf *= mean;
	--n;
      }
    }
  }


  // CONTINUE
#if 0
  if (cdf < r) {
    // If pdf drops below epsilon, then cdf will not further increase.  
    // This is because cdf is close to 1 so 
    // adding a number less than epsilon to it does not change its value.
    // Without this break, the algorithm would enter an infinite loop.
    while (cdf < r && pdf > std::numeric_limits<Number>::epsilon()) {
      ++n;
      pdf *= mean / n;
      cdf += pdf;
    }
  }
  else {
    while (cdf > r && pdf > std::numeric_limits<Number>::epsilon()) {
      cdf -= pdf;
      // I don't need to worry about division by zero.  If the mean were zero,
      // the other branch of the if statment would execute.
      pdf *= n / mean;
      --n;
    }
    // Take one step forward so cdf > r.
    ++n;
  }
  return n;
#endif
}


END_NAMESPACE_NUMERICAL

// End of file.
