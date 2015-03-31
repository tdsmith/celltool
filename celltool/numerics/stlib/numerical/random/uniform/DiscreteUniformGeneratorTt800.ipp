// -*- C++ -*-

#if !defined(__numerical_random_DiscreteUniformGeneratorTt800_ipp__)
#error This file is an implementation detail of DiscreteUniformGeneratorTt800.
#endif

BEGIN_NAMESPACE_NUMERICAL

inline
void
DiscreteUniformGeneratorTt800::
seed(const unsigned s) {
  unsigned seeds[tt800::N];
  // Construct a multiplicative congruential generator using the single seed.
  DiscreteUniformGeneratorMc32<> generator(s);
  // Generate 25 seeds.
  for (int i = 0; i != tt800::N; ++i) {
    seeds[i] = generator();
  }
  // Set the 25 seeds.
  seed(seeds);
}

// Set the 25 seeds.
inline
void
DiscreteUniformGeneratorTt800::
seed(const unsigned* seeds) {
  k = 0;
  for (int i = 0; i != tt800::N; ++i) {
    x[i] = seeds[i];
  }
  // this is magic vector `a', don't change.
  mag01[0] = 0x0;
  mag01[1] = 0x8ebfd028;
}

inline
DiscreteUniformGeneratorTt800::result_type
DiscreteUniformGeneratorTt800::
operator()() {
  // generate N words at one time.
  if (k == tt800::N) {
    int kk;
    for (kk = 0; kk < tt800::N - tt800::M; ++kk) {
      x[kk] = x[kk + tt800::M] ^ (x[kk] >> 1) ^ mag01[x[kk] % 2];
    }
    for (; kk < tt800::N; ++kk) {
      x[kk] = x[kk + (tt800::M - tt800::N)] ^ (x[kk] >> 1) ^ mag01[x[kk] % 2];
    }
    k = 0;
  }
  unsigned y = x[k];
  // s and b, magic vectors.
  y ^= (y << 7) & 0x2b5b2500; 
  // t and c, magic vectors.
  y ^= (y << 15) & 0xdb8b0000; 
  // you may delete this line if word size = 32.
  //y &= 0xffffffff; 
  // added to the 1994 version.
  y ^= (y >> 16); 
  ++k;
  return y;
}

END_NAMESPACE_NUMERICAL

// End of file.
