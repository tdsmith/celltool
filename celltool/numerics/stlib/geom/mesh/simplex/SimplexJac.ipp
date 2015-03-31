// -*- C++ -*-

#if !defined(__geom_SimplexJac_ipp__)
#error This file is an implementation detail of the class SimplexJac.
#endif

BEGIN_NAMESPACE_GEOM

namespace internal {
  
  // Consult algebraic_simplex_quality.nb.

  // I tried to do this with typenames, but it would not compile.

  template<typename T>
  inline
  void
  set(const Simplex<1, ads::FixedArray<1,T> >& s, 
      ads::FixedArray<1,T>* determinantGradient) {
    (*determinantGradient)[0] = s[1][0];
  }

  template<typename T>
  inline
  void
  set(const Simplex<2, ads::FixedArray<2,T> >& s, 
      ads::FixedArray<2,T>* determinantGradient) {
    // 2 / sqrt(3)
    const T twoOverSqrt3 = 1.1547005383792517;
    (*determinantGradient)[0] = (s[1][1] - s[2][1]) * twoOverSqrt3;
    (*determinantGradient)[1] = (s[2][0] - s[1][0]) * twoOverSqrt3;
  }

  template<typename T>
  inline
  void
  set(const Simplex<3, ads::FixedArray<3,T> >& s, 
      ads::FixedArray<3,T>* determinantGradient) {
    const T sqrt2 = 1.4142135623730951;

    (*determinantGradient)[0] = sqrt2 * 
      (s[1][1] * (s[3][2] - s[2][2]) +
       s[2][1] * (s[1][2] - s[3][2]) +
       s[3][1] * (s[2][2] - s[1][2]));
    (*determinantGradient)[1] = sqrt2 * 
      (s[1][0] * (s[2][2] - s[3][2]) +
       s[2][0] * (s[3][2] - s[1][2]) +
       s[3][0] * (s[1][2] - s[2][2]));
    (*determinantGradient)[2] = sqrt2 * 
      (s[1][0] * (s[3][1] - s[2][1]) +
       s[2][0] * (s[1][1] - s[3][1]) +
       s[3][0] * (s[2][1] - s[1][1]));
  }

}


//
// Manipulators.
//


template<int N, typename T>
inline
void
SimplexJac<N,T>::
setFunction(const Simplex& s) {
  // The coordinates of the simplex after the first vertex has been 
  // translated to the origin.  This Jacobian matrix maps the reference 
  // simplex to this simplex.
  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) {
      _matrix(i, j) = s[j+1][i] - s[0][i];
    }
  }
  // Add the part of the transformation: identity to reference.
  _matrix *= _identityToReference;
  // Compute the determinant.
  _determinant = ads::computeDeterminant(_matrix);
}


template<int N, typename T>
inline
void
SimplexJac<N,T>::
set(const Simplex& s) {
  setFunction(s);
  internal::set(s, &_gradientDeterminant);
}


template<int N, typename T>
inline
void
SimplexJac<N,T>::
setFunction(const geom::Simplex<N,ads::FixedArray<N+1,Number>,Number>& s) {
  Simplex t;
  projectToLowerDimension(s, &t);
  setFunction(t);
}


template<int N, typename T>
inline
void
SimplexJac<N,T>::
set(const geom::Simplex<N,ads::FixedArray<N+1,Number>,Number>& s) {
  Simplex t;
  projectToLowerDimension(s, &t);
  set(t);
}


//
// Static member data.
//

template<>
SimplexJac<1,double>::Matrix 
SimplexJac<1,double>::
_identityToReference(1.0);

template<>
SimplexJac<1,float>::Matrix 
SimplexJac<1,float>::
_identityToReference(1.0);

template<>
SimplexJac<2,double>::Matrix 
SimplexJac<2,double>::
_identityToReference(1.0, - std::sqrt(3.0) / 3.0,
                        0.0, 2.0 * std::sqrt(3.0) / 3.0);

template<>
SimplexJac<2,float>::Matrix 
SimplexJac<2,float>::
_identityToReference(1.0f, - std::sqrt(3.0f) / 3.0f,
                        0.0f, 2.0f * std::sqrt(3.0f) / 3.0f);

template<>
SimplexJac<3,double>::Matrix 
SimplexJac<3,double>::
_identityToReference(1.0, - std::sqrt(3.0) / 3.0,   - std::sqrt(6.0) / 6.0,
                        0.0, 2.0 * std::sqrt(3.0)/3.0, - std::sqrt(6.0)/6.0,
                        0.0, 0.0,                      std::sqrt(6.0) / 2.0);

template<>
SimplexJac<3,float>::Matrix 
SimplexJac<3,float>::
_identityToReference(1.0f, - std::sqrt(3.0f) / 3.0f,   - std::sqrt(6.0f) / 6.0f,
		     0.0f, 2.0f * std::sqrt(3.0f)/3.0f, - std::sqrt(6.0f)/6.0f,
		     0.0f, 0.0f,                      std::sqrt(6.0f) / 2.0f);



template<>
ads::FixedArray<1,SimplexJac<1,double>::Matrix>
SimplexJac<1,double>::
_gradientMatrix(SimplexJac<1,double>::Matrix(1.0));

template<>
ads::FixedArray<1,SimplexJac<1,float>::Matrix>
SimplexJac<1,float>::
_gradientMatrix(SimplexJac<1,float>::Matrix(1.0));

template<>
ads::FixedArray<2,SimplexJac<2,double>::Matrix>
SimplexJac<2,double>::
_gradientMatrix(SimplexJac<2,double>::Matrix
		  (-1.0, - std::sqrt(3.0) / 3.0,
		    0.0, 0.0),
		  SimplexJac<2,double>::Matrix
		  (0.0, 0.0,
		    -1.0, - std::sqrt(3.0) / 3.0));

template<>
ads::FixedArray<2,SimplexJac<2,float>::Matrix>
SimplexJac<2,float>::
_gradientMatrix(SimplexJac<2,float>::Matrix
		  (-1.0f, - std::sqrt(3.0f) / 3.0f,
		    0.0f, 0.0f),
		  SimplexJac<2,float>::Matrix
		  (0.0f, 0.0f,
		    -1.0f, - std::sqrt(3.0f) / 3.0f));

template<>
ads::FixedArray<3,SimplexJac<3,double>::Matrix>
SimplexJac<3,double>::
_gradientMatrix(SimplexJac<3,double>::Matrix
		  (-1.0, - std::sqrt(3.0) / 3.0, - std::sqrt(6.0) / 6.0,
		    0.0, 0.0, 0.0,
		    0.0, 0.0, 0.0),
		  SimplexJac<3,double>::Matrix
		  (0.0, 0.0, 0.0,
		    -1.0, - std::sqrt(3.0) / 3.0, - std::sqrt(6.0) / 6.0,
		    0.0, 0.0, 0.0),
		  SimplexJac<3,double>::Matrix
		  (0.0, 0.0, 0.0,
		    0.0, 0.0, 0.0, 
		    -1.0, - std::sqrt(3.0) / 3.0, - std::sqrt(6.0) / 6.0)
		 );

template<>
ads::FixedArray<3,SimplexJac<3,float>::Matrix>
SimplexJac<3,float>::
_gradientMatrix(SimplexJac<3,float>::Matrix
		(-1.0f, - std::sqrt(3.0f) / 3.0f, - std::sqrt(6.0f) / 6.0f,
		 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f),
		SimplexJac<3,float>::Matrix
		(0.0f, 0.0f, 0.0f,
		 -1.0f, - std::sqrt(3.0f) / 3.0f, - std::sqrt(6.0f) / 6.0f,
		 0.0f, 0.0f, 0.0f),
		SimplexJac<3,float>::Matrix
		(0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 
		 -1.0f, - std::sqrt(3.0f) / 3.0f, - std::sqrt(6.0f) / 6.0f)
		);



template<>
SimplexJac<1,double>::Number 
SimplexJac<1,double>::
_determinantIdentityToReference(1.0);

template<>
SimplexJac<1,float>::Number 
SimplexJac<1,float>::
_determinantIdentityToReference(1.0);

template<>
SimplexJac<2,double>::Number 
SimplexJac<2,double>::
_determinantIdentityToReference(2.0 * std::sqrt(3.0) / 3.0);

template<>
SimplexJac<2,float>::Number 
SimplexJac<2,float>::
_determinantIdentityToReference(2.0f * std::sqrt(3.0f) / 3.0f);

template<>
SimplexJac<3,double>::Number 
SimplexJac<3,double>::
_determinantIdentityToReference(std::sqrt(2.0));

template<>
SimplexJac<3,float>::Number 
SimplexJac<3,float>::
_determinantIdentityToReference(std::sqrt(2.0f));



template<>
SimplexJac<1,double>::Number 
SimplexJac<1,double>::
_nFactorial(1.0);

template<>
SimplexJac<1,float>::Number 
SimplexJac<1,float>::
_nFactorial(1.0);

template<>
SimplexJac<2,double>::Number 
SimplexJac<2,double>::
_nFactorial(2.0);

template<>
SimplexJac<2,float>::Number 
SimplexJac<2,float>::
_nFactorial(2.0);

template<>
SimplexJac<3,double>::Number 
SimplexJac<3,double>::
_nFactorial(6.0);

template<>
SimplexJac<3,float>::Number 
SimplexJac<3,float>::
_nFactorial(6.0);

END_NAMESPACE_GEOM

// End of file.
