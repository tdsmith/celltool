// -*- C++ -*-

#if !defined(__numerical_random_NormalGeneratorBoxMullerNr_ipp__)
#error This file is an implementation detail of NormalGeneratorBoxMullerNr.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Generator>
inline
typename NormalGeneratorBoxMullerNr<T,Generator>::result_type
NormalGeneratorBoxMullerNr<T,Generator>::
operator()() {
  // If there is no cached deviate.
  if (! _haveCachedGenerator) {
    //
    // Generate two standard normal deviates.
    //
    Number x, y, radiusSquared;
    // Loop until we get a random point in the unit circle. 
    do {
      // A random point in the square [-1..1] x [-1..1].
      x = 2.0 * _continuousUniformGenerator() - 1.0;
      y = 2.0 * _continuousUniformGenerator() - 1.0;
      radiusSquared = x * x + y * y;
      // We exclude the exact center of the cirle to avoid numerical problems.
    } while (radiusSquared >= 1.0 || radiusSquared == 0.0);
    const Number factor = 
      std::sqrt(-2.0 * std::log(radiusSquared) / radiusSquared);
    // Save one deviate for next time.
    _haveCachedGenerator = true;
    _cachedGenerator = x * factor;
    // Return the other deviate.
    return y * factor;
  }
  // Otherwise, we have a cached deviate.
  _haveCachedGenerator = false;
  return _cachedGenerator;
}


END_NAMESPACE_NUMERICAL

// End of file.
