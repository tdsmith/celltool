// -*- C++ -*-

#if !defined(__numerical_random_DiscreteUniformGeneratorNr1_ipp__)
#error This file is an implementation detail of DiscreteUniformGeneratorNr1.
#endif

BEGIN_NAMESPACE_NUMERICAL

// Seed this random number generator.
inline
void
DiscreteUniformGeneratorNr1::
seed(const int value) {
  // These values are duplicated from operator().
  const int 
    IA = 16807,
    IM = 2147483647,
    IQ = 127773,
    IR = 2836,
    NTAB = 32;

  // Set the seed.
  _idum = value;

  // Correct for bad seed values.
  if (_idum == 0) {
    _idum = 1;
  }
  else if (_idum < 0) {
    _idum = - _idum;
  }

  int k;
  // Load the shuffle table (after 8 warm-ups).
  for (int j = NTAB + 7; j >= 0; j--) {
    k = _idum / IQ;
    _idum = IA * (_idum - k * IQ) - IR * k;
    if (_idum < 0) {
      _idum += IM;
    }
    if (j < NTAB) {
      _iv[j] = _idum;
    }
  }
  _iy = _iv[0];
}


inline
DiscreteUniformGeneratorNr1::result_type
DiscreteUniformGeneratorNr1::
operator()() {
  const int 
    IA = 16807,
    IM = 2147483647,
    IQ = 127773,
    IR = 2836,
    NTAB = 32;
  const int NDIV = (1 + (IM - 1) / NTAB);
  int j, k;
  
  k = _idum / IQ;
  _idum = IA * (_idum - k * IQ) - IR * k;
  if (_idum < 0) {
    _idum += IM;
  }
  j = _iy / NDIV;
  _iy = _iv[j];
  _iv[j] = _idum;

  return _iy;
}


END_NAMESPACE_NUMERICAL

// End of file.
