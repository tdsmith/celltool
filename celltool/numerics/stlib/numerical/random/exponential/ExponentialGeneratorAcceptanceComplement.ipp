// -*- C++ -*-

#if !defined(__numerical_random_exponential_ExponentialGeneratorAcceptanceComplement_ipp__)
#error This file is an implementation detail of ExponentialGeneratorAcceptanceComplement.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Generator>
inline
typename ExponentialGeneratorAcceptanceComplement<T,Generator>::result_type
ExponentialGeneratorAcceptanceComplement<T,Generator>::
operator()() {
  using ExponentialGeneratorAcceptanceComplementConstants::TableSize;
  using ExponentialGeneratorAcceptanceComplementConstants::IndexMask;

  const unsigned Random = (*_discreteUniformGenerator)();
  const unsigned Index = Random & IndexMask;
  Number result = _we[Index] * Random; 
  if(_te < result) {
    _te = computeAlternateGenerator();
    result = _ae[TableSize] + computeAlternateGenerator();
  } 
  else {
    _te -= result;
    result += _ae[Index];
  } 
  return result; 
}



template<typename T, class Generator>
inline
void
ExponentialGeneratorAcceptanceComplement<T,Generator>::
copy(const ExponentialGeneratorAcceptanceComplement& other) {  
  using ExponentialGeneratorAcceptanceComplementConstants::TableSize;

  _te = other._te;
  _t1 = other._t1;
  for (int i = 0; i != TableSize; ++i) {
    _we[i] = other._we[i];
  }
  for (int i = 0; i != TableSize + 1; ++i) {
    _ae[i] = other._ae[i];
  }
}


template<typename T, class Generator>
inline
typename ExponentialGeneratorAcceptanceComplement<T,Generator>::result_type
ExponentialGeneratorAcceptanceComplement<T,Generator>::
computeAlternateGenerator() { 
  using ExponentialGeneratorAcceptanceComplementConstants::TableSize;
  using ExponentialGeneratorAcceptanceComplementConstants::IndexMask;

  const unsigned Random = (*_discreteUniformGenerator)(); 
  const unsigned Index = Random & IndexMask; 
  const Number D = _we[Index] * Random; 
  if(_t1 < D) { 
    _t1 = - std::log(transformDiscreteDeviateToContinuousDeviateOpen<Number>
		     ((*_discreteUniformGenerator)()));
    return _ae[TableSize] + computeAlternateGenerator();
  } 
  else { 
    _t1 -= D;
    return _ae[Index] + D;
  }
}


template<typename T, class Generator>
inline
void
ExponentialGeneratorAcceptanceComplement<T,Generator>::
computeTables() {  
  using ExponentialGeneratorAcceptanceComplementConstants::TableSize;

  int i; 
  for(_ae[0] = 0.0, i = 0; i < TableSize; ++i) { 
    _ae[i + 1] = _ae[i] + std::exp(_ae[i]) / TableSize; 
    _we[i] = (_ae[i + 1] - _ae[i]) / 
      Number(std::numeric_limits<unsigned>::max()); 
  } 
  _t1 = - std::log(transformDiscreteDeviateToContinuousDeviateOpen<Number>
		     ((*_discreteUniformGenerator)()));
  _te = computeAlternateGenerator();
}

END_NAMESPACE_NUMERICAL

// End of file.
