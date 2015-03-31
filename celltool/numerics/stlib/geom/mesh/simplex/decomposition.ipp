// -*- C++ -*-

#if !defined(__geom_decomposition_ipp__)
#error This file is an implementation detail of the decomposition.
#endif

BEGIN_NAMESPACE_GEOM


// Decompose the jacobian into orientation * skew * aspectRatio.
template<typename T>
inline
void
decompose(const ads::SquareMatrix<2,T>& jacobian, 
	  ads::SquareMatrix<2,T>* orientation,
	  ads::SquareMatrix<2,T>* skew,
	  ads::SquareMatrix<2,T>* aspectRatio) {
  // Compute the determinant.
  const T determinant = ads::computeDeterminant(jacobian);

  // Compute the metric tensor.
  // The metric tensor is 
  ads::SquareMatrix<2,T> metricTensor = jacobian;
  metricTensor.transpose();
  metricTensor *= jacobian;
  assert(metricTensor(0, 0) >= 0 && metricTensor(1, 1) >= 0);

  // Orientation.
  (*orientation)(0, 0) = jacobian(0, 0);
  (*orientation)(0, 1) = - jacobian(1, 0);
  (*orientation)(1, 0) = jacobian(1, 0);
  (*orientation)(1, 1) = jacobian(0, 0);
  (*orientation) /= std::sqrt(metricTensor(0, 0));

  // Skew.
  const T den = std::sqrt(metricTensor(0, 0) * metricTensor(1, 1));
  assert(den != 0);
  (*skew)(0, 0) = 1;
  (*skew)(0, 1) = metricTensor(0,1) / den;
  (*skew)(1, 0) = 0;
  (*skew)(1, 1) = determinant / den;

  // Aspect ratio.
  (*aspectRatio)(0, 0) = std::sqrt(metricTensor(0, 0));
  (*aspectRatio)(0, 1) = 0;
  (*aspectRatio)(1, 0) = 0;
  (*aspectRatio)(1, 1) = std::sqrt(metricTensor(1, 1));
}



// Decompose the jacobian into orientation * skew * aspectRatio.
template<typename T>
inline
void
decompose(const ads::SquareMatrix<3,T>& jacobian, 
	  ads::SquareMatrix<3,T>* orientation,
	  ads::SquareMatrix<3,T>* skew,
	  ads::SquareMatrix<3,T>* aspectRatio) {
  // Compute the determinant.
  const T determinant = ads::computeDeterminant(jacobian);

  // Compute the metric tensor.
  // The metric tensor is 
  ads::SquareMatrix<3,T> metricTensor = jacobian;
  metricTensor.transpose();
  metricTensor *= jacobian;
  assert(metricTensor(0, 0) >= 0 && metricTensor(1, 1) >= 0 && 
	 metricTensor(2, 2) >= 0);

  // The columns of the Jacobian.  x[n] is the n_th column.
  ads::FixedArray<3, ads::FixedArray<3,T> > x;
  for (int c = 0; c != 3; ++c) { // Column.
    for (int r = 0; r != 3; ++r) { // Row.
      x[c][r] = jacobian(r, c);
    }
  }
  

  // Orientation.
  ads::FixedArray<3,T> x0x1;
  geom::computeCrossProduct(x[0], x[1], &x0x1);
  const T nx0x1 = geom::computeMagnitude(x0x1);
  assert(nx0x1 != 0);
  for (int r = 0; r != 3; ++r) { // Row.
    (*orientation)(r, 0) = x[0][r] / std::sqrt(metricTensor(0, 0));
    (*orientation)(r, 1) = (metricTensor(0, 0) * x[1][r] - 
			    metricTensor(0, 1) * x[0][r]) / 
      (std::sqrt(metricTensor(0, 0)) * nx0x1);
    (*orientation)(r, 2) = x0x1[r] / nx0x1;
  }
  
  // Skew.
  (*skew)(0, 0) = 1;
  T den = std::sqrt(metricTensor(0, 0) * metricTensor(1, 1));
  assert(den != 0);
  (*skew)(0, 1) = metricTensor(0,1) / den;
  den = std::sqrt(metricTensor(0, 0) *	metricTensor(2, 2));
  assert(den != 0);
  (*skew)(0, 2) = metricTensor(0,2) / den;
  (*skew)(1, 0) = 0;
  den = std::sqrt(metricTensor(0, 0) * metricTensor(1, 1));
  assert(den != 0);
  (*skew)(1, 1) = nx0x1 / den;
  den = std::sqrt(metricTensor(0, 0) * metricTensor(2, 2)) * nx0x1;
  assert(den != 0);
  (*skew)(1, 2) = (metricTensor(0, 0) * metricTensor(1, 2) -
		   metricTensor(0, 1) * metricTensor(0, 2)) / den;
  (*skew)(2, 0) = 0;
  (*skew)(2, 1) = 0;
  den = std::sqrt(metricTensor(2, 2)) * nx0x1;
  assert(den != 0);
  (*skew)(2, 2) = determinant / den;
  
  // Aspect ratio.
  (*aspectRatio) = 0;
  (*aspectRatio)(0, 0) = std::sqrt(metricTensor(0, 0));
  (*aspectRatio)(1, 1) = std::sqrt(metricTensor(1, 1));
  (*aspectRatio)(2, 2) = std::sqrt(metricTensor(2, 2));
}


END_NAMESPACE_GEOM

// End of file.
