// -*- C++ -*-

#if !defined(__numerical_random_DiscreteFiniteGeneratorRejectionBinsSplitting_ipp__)
#error This file is an implementation detail of DiscreteFiniteGeneratorRejectionBinsSplitting.
#endif

BEGIN_NAMESPACE_NUMERICAL

//----------------------------------------------------------------------------
// Static class.
//----------------------------------------------------------------------------

template<bool ExactlyBalance, class BinConstants, class Generator>
inline
typename DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::result_type
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
operator()() {
  // Loop until the point is not rejected.
  for (;;) {
    const unsigned random = (*_discreteUniformGenerator)();
    // Use the first bits for indexing.
    const unsigned binIndex = random & getIndexMask();
    // Use the remaining bits for the height deviate.
    const unsigned heightGenerator = random >> getIndexBits();
    const Number height = heightGenerator * _heightUpperBound * 
      getMaxHeightInverse();
    const int deviate = _deviateIndices[binIndex];
    // If we have a hit for the PMF in this bin.
    if (height < computeBinHeight(deviate)) {
      return deviate;
    }
  }
}

// Initialize the probability mass function.
template<bool ExactlyBalance, class BinConstants, class Generator>
template<typename ForwardIterator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
initialize(ForwardIterator begin, ForwardIterator end) {
  // Build the array for the PMF.
  _pmf.rebuild(begin, end);
  _inverseSizes.resize(_pmf.size());
  _sortedPmf.resize(_pmf.size());
  if (ExactlyBalance) {
    _binIndices.resize(_pmf.size());
  }

  // Make sure that there are an adequate number of bins.
  // CONTINUE: This won't work if he bin constants class is static.
  if(_deviateIndices.size() < 2 * _pmf.size()) {
    int indexBits = 1;
    while(1 << indexBits < 2 * _pmf.size()) {
      ++indexBits;
    }
    setIndexBits(indexBits);
  }
  assert(2 * _pmf.size() <= _deviateIndices.size());

  // Rebuild the data structure by splitting the PMF across the bins.
  rebuild();
}


// Rebuild the bins.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
rebuild() {
  _pmfSum = ads::computeSum(_pmf);
  packIntoBins();
  // Compute an upper bound on the bin height.
  computeUpperBound();
}


// Pack the blocks into bins.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
_packIntoBins(Loki::Int2Type<false> /*ExactlyBalance*/) {
  // Get a sorted array of the PMF.
  computeSortedPmf();
  
  //
  // Determine how many bins to use for each probability.
  //
  Number sum = _pmfSum;
  int remainingBins = getNumberOfBins();
  // For all except the largest probability.
  for (int i = 0; i != _sortedPmf.size() - 1; ++i) {
    // Determine how many bins to use for this probability.  We must use
    // at least one.  Below I round to the nearest integer.
    const int count = std::max(1, 
			       int(*_sortedPmf[i] / sum * remainingBins + 0.5));
    // The index of the probability.
    const int index = _sortedPmf[i] - _pmf.begin();
    // The inverse of the number of bins for this probability.
    _inverseSizes[index] = 1.0 / count;
    for (int n = 0; n != count; ++n) {
      _deviateIndices[--remainingBins] = index;
    }
    sum -= _pmf[index];
  }
  // Give the largest probability the remaining bins.
#ifdef DEBUG_DiscreteFiniteGeneratorRejectionBinsSplitting
  assert(remainingBins != 0);
#endif
  // The index of the largest probability.
  const int index = *(_sortedPmf.end() - 1) - _pmf.begin();
  // The inverse of the number of bins for this probability.
  _inverseSizes[index] = 1.0 / remainingBins;
  while (remainingBins != 0) {
    _deviateIndices[--remainingBins] = index;
  }
}


// Pack the blocks into bins.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
_packIntoBins(Loki::Int2Type<true> /*ExactlyBalance*/) {
  // Clear the old bins.
  for (int i = 0; i != _binIndices.size(); ++i) {
    _binIndices[i].clear();
  }

  // Get a sorted array of the PMF.
  computeSortedPmf();
  
  //
  // Determine how many bins to use for each probability.
  //
  Number sum = _pmfSum;
  int remainingBins = getNumberOfBins();
  // For all except the largest probability.
  for (int i = 0; i != _sortedPmf.size() - 1; ++i) {
    // Determine how many bins to use for this probability.  We must use
    // at least one.  Below I round to the nearest integer.
    const int count = std::max(1, 
			       int(*_sortedPmf[i] / sum * remainingBins + 0.5));
    // The index of the probability.
    const int index = _sortedPmf[i] - _pmf.begin();
    std::vector<int>& indices = _binIndices[index];
    for (int n = 0; n != count; ++n) {
      indices.push_back(--remainingBins);
      _deviateIndices[remainingBins] = index;
    }
    sum -= _pmf[index];
  }
  // Give the largest probability the remaining bins.
#ifdef DEBUG_DiscreteFiniteGeneratorRejectionBinsSplitting
  assert(remainingBins != 0);
#endif
  // The index of the largest probability.
  const int index = *(_sortedPmf.end() - 1) - _pmf.begin();
  std::vector<int>& indices = _binIndices[index];
  while (remainingBins != 0) {
    indices.push_back(--remainingBins);
    _deviateIndices[remainingBins] = index;
  }

  // Balance to minimize the maximum bin height.
  balance();

  // Compute the inverse of the number of bins for each probability.
  computeInverseSizes();
}


// Compute the inverses.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
computeInverseSizes() {
  for (int i = 0; i != _binIndices.size(); ++i) {
    _inverseSizes[i] = 1.0 / _binIndices[i].size();
  }
}

// Balance to minimize the maximum bin height.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
balance() {
  // Trade bins until we have minimized the maximum bin height.
  while (tradeBins())
    ;
}

// Trade bins to reduce the maximum bin height.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
bool
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
tradeBins() {
  // Get the maximum bin height and the minimum height obtained by removing 
  // a bin.
  Number maximumHeight = 0;
  Number minimumHeight = std::numeric_limits<Number>::max();
  Number height;
  int maximumIndex = -1, minimumIndex = -1;
  for (int i = 0; i != _pmf.size(); ++i) {
    height = _pmf[i] / _binIndices[i].size();
    if (height > maximumHeight) {
      maximumHeight = height;
      maximumIndex = i;
    }
    if (_binIndices[i].size() != 1) {
      height = _pmf[i] / (_binIndices[i].size() - 1);
      if (height < minimumHeight) {
	minimumHeight = height;
	minimumIndex = i;
      }
    }
  }
  // If we can reduce the maximum height by trading bins.
  if (minimumHeight < maximumHeight && minimumIndex != maximumIndex) {
    _deviateIndices[_binIndices[minimumIndex].back()] = maximumIndex;
    _binIndices[maximumIndex].push_back(_binIndices[minimumIndex].back());
    _binIndices[minimumIndex].pop_back();
    return true;
  }
  return false;
}

// Print information about the data structure.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<false, ExactlyBalance, BinConstants, Generator>::
print(std::ostream& out) const {
  out << "Bin data:\n\n"
      << "Height upper bound = " << _heightUpperBound << "\n"
      << "Deviate indices = \n" << _deviateIndices << "\n"
      << "\nPMF data:\n\n"
      << "PMF sum = " << _pmfSum << "\n"
      << "PMF = \n" << _pmf << "\n"
      << "Inverse sizes = \n" << _inverseSizes << "\n"
      << "Bin indices = \n";
  for (int i = 0; i != _binIndices.size(); ++i) {
    for (size_t j = 0; j != _binIndices[i].size(); ++j) {
      out << _binIndices[i][j] << " ";
    }
    out << "\n";
  }
}


//----------------------------------------------------------------------------
// Dynamic class.
//----------------------------------------------------------------------------

// Initialize the probability mass function.
template<bool ExactlyBalance, class BinConstants, class Generator>
template<typename ForwardIterator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<true, ExactlyBalance, BinConstants, Generator>::
initialize(ForwardIterator begin, ForwardIterator end) {
  RepairBase::resetRepairCounter();
  Base::initialize(begin, end);
  updateMinimumAllowedEfficiency();
}


// Rebuild the bins.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<true, ExactlyBalance, BinConstants, Generator>::
rebuild() {
  Base::rebuild();
  updateMinimumAllowedEfficiency();
  // Rebuilding also repairs the data structure, so we reset that counter.
  resetRepairCounter();
}



// Set the probability mass function with the specified index.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<true, ExactlyBalance, BinConstants, Generator>::
setPmf(const int index, const Number value) {
  // If the value has not changed, do nothing.  I need this check; otherwise
  // the following branch could be expensive.
  if (_pmf[index] == value) {
    return;
  }

  // If the PMF has become zero. (It was not zero before.)
  if (value == 0) {
    // Set the PMF to zero.
    _pmf[index] = 0;
    // Repair the data structure.  This is necessary to ensure that the 
    // binned PMF are correct.  They must be exactly zero.  Likewise, the 
    // sum of the PMF may have become zero.  
    repair();
    return;
  }

  // The remainder of this function is the standard case.  Update the data 
  // structure using the difference between the new and old values.

  const Number difference = value - _pmf[index];
  // Update the sum of the PMF.
  _pmfSum += difference;
  // Update the PMF array.
  _pmf[index] = value;

  // Update the upper bound on the bin height.
  const Number height = computeBinHeight(index);
  if (height > _heightUpperBound) {
    _heightUpperBound = height;
  }

  decrementRepairCounter();
}


// Update the data structure following calls to setPmf().
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<true, ExactlyBalance, BinConstants, Generator>::
updatePmf() {
  // If it is time for a repair.
  if (shouldRepair()) {
    repair();
  }
}




// Repair the data structure.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<true, ExactlyBalance, BinConstants, Generator>::
repair() {
  // Compute the sum of the PMF.
  Base::computePmfSum();

  resetRepairCounter();
}


// Print information about the data structure.
template<bool ExactlyBalance, class BinConstants, class Generator>
inline
void
DiscreteFiniteGeneratorRejectionBinsSplitting<true, ExactlyBalance, BinConstants, Generator>::
print(std::ostream& out) const {
  Base::print(out);
  out << "Minimum efficiency factor = " << _minimumEfficiencyFactor << "\n"
      << "Minimum efficiency = " << _minimumEfficiency << "\n"
      << "Efficiency = " << computeEfficiency() << "\n";
  RepairBase::print(out);
}

END_NAMESPACE_NUMERICAL

// End of file.
