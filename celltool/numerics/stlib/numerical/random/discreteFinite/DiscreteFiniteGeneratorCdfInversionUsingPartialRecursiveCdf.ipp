// -*- C++ -*-

#if !defined(__numerical_random_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf_ipp__)
#error This file is an implementation detail of DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf.
#endif

BEGIN_NAMESPACE_NUMERICAL

// Return a discrete, finite deviate.
template<bool UseImmediateUpdate, class Generator, typename T>
inline
typename DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
<UseImmediateUpdate, Generator, T>::result_type
DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
<UseImmediateUpdate, Generator, T>::
operator()() {
  Number r = _continuousUniformGenerator() * getPmfSum();
  Number x;
  int index = -1;
  int step = _partialRecursiveCdf.size() / 2;
  // The for loop is slightly faster.
  for (int i = _indexBits; i != 0; --i) {
  //while (step != 0) {
    x = _partialRecursiveCdf[index + step];

    // It is faster whithout the branch.
#if 0
    if (x <= r) {
      r -= x;
      index += step;
    }
#endif
    const bool shouldMove = x <= r;
    r -= shouldMove * x;
    index += shouldMove * step;

    step /= 2;
  }
  // This is the weighted probability that exceeds r.
  ++index;

  // Step back over zero probabilities if necessary.
  while (getPmf(index) == 0) {
    --index;
  }

  return index;
}


// Update the data structure following calls to setPmf() .
template<bool UseImmediateUpdate, class Generator, typename T>
inline
void
DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
<UseImmediateUpdate, Generator, T>::
repair() {
  // Start with the (zero-padded) PMF.
  std::copy(getPmfBeginning(), getPmfEnd(), _partialRecursiveCdf.begin());
  std::fill(_partialRecursiveCdf.begin() + getSize(), 
	    _partialRecursiveCdf.end(), Number(0));

  // Compute the partial, recursive sums.
  for (int step = 2, offset = 1; step <= _partialRecursiveCdf.size(); 
       step *= 2, offset *= 2) {
    for (int i = step - 1, j = step - 1- offset; 
	 i < _partialRecursiveCdf.size(); i += step, j += step) {
      _partialRecursiveCdf[i] += _partialRecursiveCdf[j];
    }
  }
  
  RepairBase::resetRepairCounter();
}


//! Update the CDF.
template<bool UseImmediateUpdate, class Generator, typename T>
inline
void
DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
<UseImmediateUpdate, Generator, T>::
updateCdf(const int index, const Number difference) {
  // Shift back by one so we can use offsets.
  typename ads::Array<1, Number>::iterator array 
    = _partialRecursiveCdf.begin() - 1;

  int offset = _partialRecursiveCdf.size();
  for (int shift = _indexBits; shift >= 0; --shift, offset /= 2) {
    //for (; offset != 0; offset /= 2) {
    // Check the appropriate bit.
    if (index & offset) {
      array += offset;
    }
    else {
      array[offset] += difference;
    }
#if 0
    // Little performance difference.
    const bool bit = index & offset;
    array[offset] += (! bit) * difference;
    array += bit * offset;
#endif
  }

  // Not as fast.
#if 0
  int offset = _partialRecursiveCdf.size();
  //int bit;
  for (int shift = _indexBits; shift >= 0; --shift, offset /= 2) {
    // Extract the shift_th digit.
    if ((index >> shift) & 1) {
      array += offset;
    }
    else {
      array[offset] += difference;
    }
    // This is not as fast.
#if 0
    const bool bit = (index >> shift) & 1;
    array[offset] += (! bit) * difference;
    array += bit * offset;
#endif
  }
#endif
}

END_NAMESPACE_NUMERICAL

// End of file.
