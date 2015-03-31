// -*- C++ -*-

#if !defined(__geom_SimplexMeanRatio_ipp__)
#error This file is an implementation detail of the class SimplexMeanRatio.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical Member Functions
//

// Return the eta quality metric.
template<int N, typename T>
inline
T
SimplexMeanRatio<N,T>::
operator()() const {
  return computeFunctionGivenS2(computeFrobeniusNormSquared(getMatrix()));
}


// Return the gradient of the mean ratio (eta) quality metric.
template<int N, typename T>
inline
void
SimplexMeanRatio<N,T>::
computeGradient(Vertex* gradient) const {
  // Ensure that the determinant of the Jacobian matrix is positive.
  assert(getDeterminant() > 0);
  // |S|^2, S is the Jacobian matrix.
  const Number s2 = computeFrobeniusNormSquared(getMatrix());
  // Calculate eta given the squared norm of S.
  const Number e = computeFunctionGivenS2(s2);
  
  // Compute the gradient of eta.
  for (int n = 0; n != N; ++n) {
    (*gradient)[n] = 2.0 * e * 
      (computeInnerProduct(getGradientMatrix()[n], getMatrix()) / s2 -
       getGradientDeterminant()[n] / (N * getDeterminant()));
  }
}


//
// Protected member functions.
//


// Return the eta quality metric given \f$ |S|^2 \f$.
template<int N, typename T>
inline
T
SimplexMeanRatio<N,T>::
computeFunctionGivenS2(const Number s2) const {
  if (getDeterminant() <= 0.0) {
    return std::numeric_limits<Number>::infinity();
  }
  return s2 / (N * std::pow(getDeterminant(), 2.0 / N));
}

END_NAMESPACE_GEOM

// End of file.
