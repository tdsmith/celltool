// -*- C++ -*-

#if !defined(__geom_SimplexCondNum_ipp__)
#error This file is an implementation detail of the class SimplexCondNum.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical Member Functions
//

// Return the condition number quality metric.
template<int N, typename T>
inline
T
SimplexCondNum<N,T>::
operator()() const {
  // Calculate the quality metric using the matrix norms.
  return computeFunction(computeFrobeniusNormSquared(getMatrix()),
			 computeFrobeniusNormSquared(getAdjointMatrix()));
}


// Calculate the gradient of the condition number quality metric.
template<int N, typename T>
inline
void
SimplexCondNum<N,T>::
computeGradient(Vertex* gradient) const {
  // Ensure that the determinant of the Jacobian matrix is positive.
  assert(getDeterminant() > 0);
  // Calculate the squared norm of the matrix and its adjoint.
  const Number snj = computeFrobeniusNormSquared(getMatrix());
  const Number sna = computeFrobeniusNormSquared(getAdjointMatrix());
  // Calculate kapppa.
  const Number k = computeFunction(snj, sna);

  // Calculate the gradient.
  for (int n = 0; n != N; ++n) {
    (*gradient)[n] = k * 
      (computeInnerProduct(getGradientMatrix()[n], getMatrix()) / snj +
       computeInnerProduct(getAdjointGradientMatrix()[n], getAdjointMatrix()) / 
       sna - getGradientDeterminant()[n] / getDeterminant());
  }
}


// Return the quality metric given \f$ | S |^2 \f$ and \f$ | \Sigma |^2 \f$.
template<int N, typename T>
inline
T
SimplexCondNum<N,T>::
computeFunction(const Number snj, const Number sna) const {
  if (getDeterminant() <= 0.0) {
    return std::numeric_limits<Number>::infinity();
  }
#ifdef DEBUG_geom
  assert(snj >= 0 && sna >= 0);
#endif
  return (std::sqrt(snj) * std::sqrt(sna) / (N * getDeterminant()));
}

END_NAMESPACE_GEOM

// End of file.
