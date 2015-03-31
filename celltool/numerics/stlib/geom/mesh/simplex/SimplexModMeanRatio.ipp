// -*- C++ -*-

#if !defined(__geom_SimplexModMeanRatio_ipp__)
#error This file is an implementation detail of the class SimplexModMeanRatio.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical Member Functions
//


// Return the eta quality metric.
template<int N, typename T>
inline
T
SimplexModMeanRatio<N,T>::
operator()(const Number minDeterminant) const {
  return computeFunctionGivenS2(minDeterminant, 
				computeFrobeniusNormSquared(getMatrix()));
}


// Calculate the gradient of the modified eta quality metric.
template<int N, typename T>
inline
void
SimplexModMeanRatio<N,T>::
computeGradient(const Number minDeterminant, Vertex* gradient) const {
  // |S|^2, S is the Jacobian matrix.
  const Number s2 = computeFrobeniusNormSquared(getMatrix());
  assert(s2 != 0);
  // Calculate eta_m given the squared norm of S.
  const Number e = computeFunctionGivenS2(minDeterminant, s2);
  
  // Compute the gradient of eta.
  const Number d = SimplexModDet<T>::getDelta(minDeterminant);
  const Number den = 
    N * std::sqrt(getDeterminant() * getDeterminant() + 4.0 * d * d);
  for (int n = 0; n != N; ++n) {
    (*gradient)[n] = 2.0 * e * 
      (computeInnerProduct(getGradientMatrix()[n], getMatrix()) / s2 -
       getGradientDeterminant()[n] / den);
  }
}


//
// Protected member functions.
//


// Return the eta quality metric given \f$ |S|^2 \f$.
template<int N, typename T>
inline
T
SimplexModMeanRatio<N,T>::
computeFunctionGivenS2(const Number minDeterminant, const Number s2) const {
  // If none of the determinants are small.
  if (minDeterminant >= SimplexModDet<T>::getEpsilon()) {
    // Return the unmodified quality metric.
    return Base::computeFunctionGivenS2(s2);
  }
  // Else, some of the determinants are small or negative.
  // CONTINUE: What should I do when the numerator vanishes?
  if (s2 != 0) {
    return s2 / (N * std::pow(getH(minDeterminant), 2.0 / N));
  }
  return std::numeric_limits<Number>::max();
}


END_NAMESPACE_GEOM

// End of file.
