// -*- C++ -*-

#if !defined(__numerical_random_DiscreteUniformGeneratorMt19937_ipp__)
#error This file is an implementation detail of DiscreteUniformGeneratorMt19937.
#endif

BEGIN_NAMESPACE_NUMERICAL

// initializes mt[N] with a seed
inline
void
DiscreteUniformGeneratorMt19937::
seed(const unsigned s) {
  generateState(s, mt);
  mti = mt19937mn::N + 1;
#if 0
  mt[0] = s & 0xffffffffUL;
  for (mti = 1; mti < mt19937mn::N; mti++) {
    mt[mti] = 
      (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mt[mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
#endif
}


// Generate a state vector.
inline
unsigned
DiscreteUniformGeneratorMt19937::
generateState(unsigned seed, unsigned state[]) {
  state[0] = seed & 0xffffffffUL;
  int i;
  for (i = 1; i < mt19937mn::N; i++) {
    state[i] = 
      (1812433253UL * (state[i-1] ^ (state[i-1] >> 30)) + i); 
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    state[i] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
  seed = (1812433253UL * (state[i-1] ^ (state[i-1] >> 30)) + i); 
  seed &= 0xffffffffUL;
  return seed;
}


inline
DiscreteUniformGeneratorMt19937::result_type
DiscreteUniformGeneratorMt19937::
operator()() {
  result_type y;

  if (mti >= mt19937mn::N) { /* generate N words at one time */
    int kk;

    for (kk = 0; kk < mt19937mn::N - mt19937mn::M; kk++) {
      y = (mt[kk] & mt19937mn::UPPER_MASK) | 
	(mt[kk + 1] &mt19937mn::LOWER_MASK);
      mt[kk] = mt[kk + mt19937mn::M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < mt19937mn::N - 1; kk++) {
      y = (mt[kk] & mt19937mn::UPPER_MASK) | 
	(mt[kk + 1] & mt19937mn::LOWER_MASK);
      mt[kk] = mt[kk + (mt19937mn::M - mt19937mn::N)] ^ (y >> 1) ^ 
	mag01[y & 0x1UL];
    }
    y = (mt[mt19937mn::N - 1] & mt19937mn::UPPER_MASK) | 
      (mt[0] & mt19937mn::LOWER_MASK);
    mt[mt19937mn::N - 1] = mt[mt19937mn::M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mti = 0;
  }
  
  y = mt[mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

END_NAMESPACE_NUMERICAL

// End of file.
