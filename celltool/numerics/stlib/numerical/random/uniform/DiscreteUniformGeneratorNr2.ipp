// -*- C++ -*-

#if !defined(__numerical_random_DiscreteUniformGeneratorNr2_ipp__)
#error This file is an implementation detail of DiscreteUniformGeneratorNr2.
#endif

BEGIN_NAMESPACE_NUMERICAL

// Seed this random number generator.
inline
void
DiscreteUniformGeneratorNr2::
seed(const int value) {
  // These values are duplicated from operator().
  const int 
    IM1 = 2147483563,
    IA1 = 40014,
    IQ1 = 53668,
    IR1 = 12211,
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
  _idum2 = _idum;

  int k;
  // Load the shuffle table (after 8 warm-ups).
  for (int j = NTAB + 7; j >= 0; j--) {
    k = _idum / IQ1;
    _idum = IA1 * (_idum - k * IQ1) - IR1 * k;
    if (_idum < 0) {
      _idum += IM1;
    }
    if (j < NTAB) {
      _iv[j] = _idum;
    }
  }
  _iy = _iv[0];
}


inline
DiscreteUniformGeneratorNr2::result_type
DiscreteUniformGeneratorNr2::
operator()() {
  const int 
    IM1 = 2147483563,
    IM2 = 2147483399,
    IA1 = 40014,
    IA2 = 40692,
    IQ1 = 53668,
    IQ2 = 52774,
    IR1 = 12211,
    IR2 = 3791,
    NTAB = 32,
    IMM1 = IM1 - 1,
    NDIV = 1 + IMM1 / NTAB;

  int j, k;
  
  k = _idum / IQ1;
  _idum = IA1 * (_idum - k * IQ1) - IR1 * k;
  if (_idum < 0) {
    _idum += IM1;
  }

  k = _idum2 / IQ2;
  _idum2 = IA2 * (_idum2 - k * IQ2) - IR2 * k;
  if (_idum2 < 0) {
    _idum2 += IM2;
  }

  j = _iy / NDIV;
  _iy = _iv[j] - _idum2;
  _iv[j] = _idum;
  if (_iy < 1) {
    _iy += IMM1;
  }

  return _iy;
}


END_NAMESPACE_NUMERICAL

// End of file.
