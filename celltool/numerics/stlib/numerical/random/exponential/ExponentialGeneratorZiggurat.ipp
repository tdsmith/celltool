// -*- C++ -*-

#if !defined(__numerical_random_exponential_ExponentialGeneratorZiggurat_ipp__)
#error This file is an implementation detail of ExponentialGeneratorZiggurat.
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T, class Generator>
inline
typename ExponentialGeneratorZiggurat<T,Generator>::Number
ExponentialGeneratorZiggurat<T,Generator>::
operator()() {
  _jz = (*_discreteUniformGenerator)();
  _iz = _jz & 255;
  return _jz < _ke[_iz] ? _jz * _we[_iz] : fix();
}

// fix() generates variates from the residue when rejection occurs.
template<typename T, class Generator>
inline
typename ExponentialGeneratorZiggurat<T,Generator>::result_type
ExponentialGeneratorZiggurat<T,Generator>::
fix() {
  Number x;
  for(;;) {  
    if(_iz == 0) {
      return 7.69711 - 
	std::log(transformDiscreteDeviateToContinuousDeviateOpen<Number>
		 ((*_discreteUniformGenerator)()));
    }
    x = _jz*_we[_iz]; 
    if (_fe[_iz] + 
	transformDiscreteDeviateToContinuousDeviateOpen<Number>
	((*_discreteUniformGenerator)()) * 
	(_fe[_iz-1] - _fe[_iz]) < std::exp(-x)) {
      return x;
    }
    // Initiate, try to exit the loop.
    _jz = (*_discreteUniformGenerator)();
    _iz = _jz & 255;
    if (_jz < _ke[_iz]) {
      return _jz * _we[_iz];
    }
  }
}


template<typename T, class Generator>
inline
void
ExponentialGeneratorZiggurat<T,Generator>::
copy(const ExponentialGeneratorZiggurat& other) {  
  _jz = other._jz;
  _iz = other._iz;
  for (int i = 0; i != 256; ++i) {
    _ke[i] = other._ke[i];
    _we[i] = other._we[i];
    _fe[i] = other._fe[i];
  }
}


template<typename T, class Generator>
inline
void
ExponentialGeneratorZiggurat<T,Generator>::
computeTables() {  
  const Number m2 = 4294967296.;
  Number q;
  Number de = 7.697117470131487, te = de, ve = 3.949659822581572e-3;
  int i;

  // Set up the tables.
  q = ve / std::exp(-de);
  _ke[0] = unsigned((de / q) * m2);
  _ke[1] = 0;

  _we[0] = q / m2;
  _we[255] = de / m2;

  _fe[0] = 1.;
  _fe[255] = std::exp(-de);

  for(i = 254; i>=1; i--) {
    de = - std::log(ve / de + std::exp(-de));
    _ke[i + 1] = unsigned((de / te) * m2);
    te = de;
    _fe[i] = std::exp(-de);
    _we[i] = de / m2;
  }
}

END_NAMESPACE_NUMERICAL

// End of file.
