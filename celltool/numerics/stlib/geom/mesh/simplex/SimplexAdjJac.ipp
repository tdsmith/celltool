// -*- C++ -*-

#if !defined(__geom_SimplexAdjJac_ipp__)
#error This file is an implementation detail of the class SimplexAdjJac.
#endif

BEGIN_NAMESPACE_GEOM

//
// Manipulators.
//

namespace internal {
  
  // Consult algebraic_simplex_quality.nb.

  template<typename T>
  inline
  void
  set(const ads::SquareMatrix<2,T>& j,
      ads::FixedArray<2, ads::SquareMatrix<2,T> >* matrixGradient)
  {
    const T sqrt3 = 1.7320508075688772;

    (*matrixGradient)[0](0,0) = 0.0;
    (*matrixGradient)[0](1,0) = 0.0;
    (*matrixGradient)[0](0,1) = 1.0 / sqrt3;
    (*matrixGradient)[0](1,1) = - 1.0;

    (*matrixGradient)[1](0,0) = - 1.0 / sqrt3;
    (*matrixGradient)[1](1,0) = 1.0;
    (*matrixGradient)[1](0,1) = 0.0;
    (*matrixGradient)[1](1,1) = 0.0;
  }


  template<typename T>
  inline
  void
  set(const ads::SquareMatrix<3,T>& j, 
      ads::FixedArray<3, ads::SquareMatrix<3,T> >* matrixGradient) {
    const T sqrt2 = 1.4142135623730951;
    const T sqrt3 = 1.7320508075688772;
    const T sqrt6 = 2.4494897427831779;

    // \frac{ \partial }{ \partial x }
    (*matrixGradient)[0](0,0) = 0.0;
    (*matrixGradient)[0](1,0) = 0.0;
    (*matrixGradient)[0](2,0) = 0.0;

    (*matrixGradient)[0](0,1) = j(2,2) / sqrt3 - j(2,1) / sqrt6;
    (*matrixGradient)[0](1,1) = j(2,0) / sqrt6 - j(2,2);
    (*matrixGradient)[0](2,1) = j(2,1) - j(2,0) / sqrt3;

    (*matrixGradient)[0](0,2) = (j(1,1) - sqrt2 * j(1,2)) / sqrt6;
    (*matrixGradient)[0](1,2) = j(1,2) - j(1,0) / sqrt6;
    (*matrixGradient)[0](2,2) = j(1,0) / sqrt3 - j(1,1);


    // \frac{ \partial }{ \partial y }
    (*matrixGradient)[1](0,0) = (j(2,1) - sqrt2 * j(2,2)) / sqrt6;
    (*matrixGradient)[1](1,0) = j(2,2) - j(2,0) / sqrt6;
    (*matrixGradient)[1](2,0) = j(2,0) / sqrt3 - j(2,1);

    (*matrixGradient)[1](0,1) = 0.0;
    (*matrixGradient)[1](1,1) = 0.0;
    (*matrixGradient)[1](2,1) = 0.0;

    (*matrixGradient)[1](0,2) = j(0,2) / sqrt3 - j(0,1) / sqrt6;
    (*matrixGradient)[1](1,2) = j(0,0) / sqrt6 - j(0,2);
    (*matrixGradient)[1](2,2) = j(0,1) - j(0,0) / sqrt3;


    // \frac{ \partial }{ \partial z }
    (*matrixGradient)[2](0,0) = j(1,2) / sqrt3 - j(1,1) / sqrt6;
    (*matrixGradient)[2](1,0) = j(1,0) / sqrt6 - j(1,2);
    (*matrixGradient)[2](2,0) = j(1,1) - j(1,0) / sqrt3;

    (*matrixGradient)[2](0,1) = (j(0,1) - sqrt2 * j(0,2)) / sqrt6;
    (*matrixGradient)[2](1,1) = j(0,2) - j(0,0) / sqrt6;
    (*matrixGradient)[2](2,1) = j(0,0) / sqrt3 - j(0,1);

    (*matrixGradient)[2](0,2) = 0.0;
    (*matrixGradient)[2](1,2) = 0.0;
    (*matrixGradient)[2](2,2) = 0.0;
  }
}


template<int N, typename T>
inline
void
SimplexAdjJac<N,T>::
setFunction(const Matrix& jacobian) {
  ads::computeScaledInverse(jacobian, &_matrix);
}


template<int N, typename T>
inline
void
SimplexAdjJac<N,T>::
set(const Matrix& jacobian) {
  setFunction(jacobian);
  internal::set(jacobian, &_gradientMatrix);
}

END_NAMESPACE_GEOM

// End of file.
