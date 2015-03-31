// -*- C++ -*-

#if !defined(__numerical_random_DiscreteFiniteGeneratorBinarySearch_ipp__)
#error This file is an implementation detail of DiscreteFiniteGeneratorBinarySearch.
#endif

BEGIN_NAMESPACE_NUMERICAL


// Update the data structure following calls to setPmf() .
template<class Generator, typename T>
inline
void
DiscreteFiniteGeneratorBinarySearch<true, Generator, T>::
updatePmf() {
  Base::updatePmf();

  // If we need to recompute the CDF for all probabilities.
  if (_firstModifiedProbability == 0) {
    // Compute the cumulative distribution function.
    std::partial_sum(Base::getPmfBeginning(), Base::getPmfEnd(), _cdf.begin());
  }
  else {
    // The CDF is correct up to _cdf[_firstModifiedProbability - 1].
    // Update the cumulative distribution function for the modified 
    // probabilities.
    std::partial_sum(Base::getPmfBeginning() + _firstModifiedProbability,
		     Base::getPmfEnd(),
		     _cdf.begin() + _firstModifiedProbability);
    const Number offset = _cdf[_firstModifiedProbability - 1];
    for (typename ads::Array<1, Number>::iterator i 
	   = _cdf.begin() + _firstModifiedProbability; i != _cdf.end(); 
	 ++i) {
      *i += offset;
    }
  }
  _firstModifiedProbability = _cdf.size();
}


END_NAMESPACE_NUMERICAL

// End of file.
