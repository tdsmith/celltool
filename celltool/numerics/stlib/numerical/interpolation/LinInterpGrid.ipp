// -*- C++ -*-

#if !defined(__numerical_interpolation_LinInterpGrid_ipp__)
#error This file is an implementation detail of LinInterpGrid.
#endif

BEGIN_NAMESPACE_NUMERICAL

namespace internal {

  template<typename F, bool A, typename T>
  typename Loki::TypeTraits<F>::ParameterType
  lin_interp(const ads::Array<1,F,A>& fields, const ads::FixedArray<1,int>& i,
	      const ads::FixedArray<1,T>& t) {
    // Make these static in case F is not a fundamental type.
    static F f, g;
#ifdef DEBUG_LinInterpGrid
    assert(0 <= t[0] && t[0] <= 1);
#endif
    // return t[0] * fields(i[0]) + (1 - t[0]) * fields(i[0] + 1);

    // The following is more efficient if F is not a fundamental type.

    f = fields(i[0]);
    f *= (1 - t[0]);

    g = fields(i[0] + 1);
    g *= t[0];
    f += g;

    return f;
  }

  template<typename F, bool A, typename T>
  typename Loki::TypeTraits<F>::ParameterType
  lin_interp(const ads::Array<2,F,A>& fields, const ads::FixedArray<2,int>& i,
	      const ads::FixedArray<2,T>& t) {
    // Make these static in case F is not a fundamental type.
    static F f, g;
#ifdef DEBUG_LinInterpGrid
    assert(0 <= t[0] && t[0] <= 1 && 0 <= t[1] && t[1] <= 1);
#endif
    // return t[0] * t[1] * fields(i[0], i[1]) + 
    //  (1 - t[0]) * t[1] * fields(i[0] + 1, i[1]) +
    // t[0] * (1 - t[1]) * fields(i[0], i[1] + 1) + 
    //  (1 - t[0]) * (1 - t[1]) * fields(i[0] + 1, i[1] + 1);

    // The following is more efficient if F is not a fundamental type.

    f = fields(i[0], i[1]);
    f *= (1 - t[0]) * (1 - t[1]);

    g = fields(i[0] + 1, i[1]);
    g *= t[0] * (1 - t[1]);
    f += g;

    g = fields(i[0], i[1] + 1);
    g *= (1 - t[0]) * t[1];
    f += g;

    g = fields(i[0] + 1, i[1] + 1);
    g *= t[0] * t[1];
    f += g;

    return f;
  }

  template<typename F, bool A, typename T>
  typename Loki::TypeTraits<F>::ParameterType
  lin_interp(const ads::Array<3,F,A>& fields, const ads::FixedArray<3,int>& i,
	      const ads::FixedArray<3,T>& t) {
    // Make these static in case F is not a fundamental type.
    static F f, g;
#ifdef DEBUG_LinInterpGrid
    assert(0 <= t[0] && t[0] <= 1 && 0 <= t[1] && t[1] <= 1 && 
	    0 <= t[2] && t[2] <= 1);
#endif

    f = fields(i[0], i[1], i[2]);
    f *= (1 - t[0]) * (1 -t[1]) * (1 - t[2]);

    g = fields(i[0] + 1, i[1], i[2]);
    g *= t[0] * (1 - t[1]) * (1 - t[2]);
    f += g;

    g = fields(i[0], i[1] + 1, i[2]);
    g *= (1 - t[0]) * t[1] * (1 - t[2]);
    f += g;

    g = fields(i[0] + 1, i[1] + 1, i[2]);
    g *= t[0] * t[1] * (1 - t[2]);
    f += g;

    g = fields(i[0], i[1], i[2] + 1);
    g *= (1 - t[0]) * (1 -t[1]) * t[2];
    f += g;

    g = fields(i[0] + 1, i[1], i[2] + 1 );
    g *= t[0] * (1 - t[1]) * t[2];
    f += g;

    g = fields(i[0], i[1] + 1, i[2] + 1 );
    g *= (1 - t[0]) * t[1] * t[2];
    f += g;

    g = fields(i[0] + 1, i[1] + 1, i[2] + 1 );
    g *= t[0] * t[1] * t[2];
    f += g;

    return f;
  }

}

template<int N, typename F, bool A, typename T>
inline
typename LinInterpGrid<N,F,A,T>::result_type
LinInterpGrid<N,F,A,T>::
operator()(argument_type x) const {
  static Point p, q;
  static index_type i;

  //
  // Convert the Cartesian point to an index.
  //

  // Store the location.  
  p = x;
  // Convert the location to a continuous index.
  _grid.convertLocationToIndex(&p);
  q = p;
  // Floor to get the integer index.
  ads::applyFloor(&q);
  for (int n = 0; n != N; ++n) {
    i[n] = int(q[n]);
  }
  // The scaled offsets.  Each coordinate is in the range [0..1).
  p -= q;

  // Check that the index is in the grid.
  for (int n = 0; n != N; ++n) {
    assert(0 <= i[n] && i[n] < _grid.getExtents()[n]);
  }
  
  return internal::lin_interp(_fields, i, p);
}

END_NAMESPACE_NUMERICAL

// End of file.
