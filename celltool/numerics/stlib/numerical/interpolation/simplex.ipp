// -*- C++ -*-

#if !defined(__numerical_interpolation_simplex_ipp__)
#error This file is an implementation detail of numerical/interpolation/simplex.
#endif

BEGIN_NAMESPACE_NUMERICAL

//
// 1-D space.
//

// 1-D simplex.  1-D space.  Reduced problem.
namespace internal {

  template <typename T, typename F>
  inline
  void
  lin_interp_reduced( const T b, const F& beta, const T x, F& f )
  {
    assert( b != 0 );
    // f = (x / b) * beta;
    f = beta;
    f *= (x / b);
  }

}

// 1-D simplex.  1-D space.
template <typename T, typename F>
inline
const F&
linear_interpolation( const T a, T b, 
		      const F& alpha, const F& _beta, 
		      T x )
{
  static F field;
  static F beta;

  // Translate the nodes and x by -a.
  b -= a;
  x -= a;
  // Translate the values by -alpha.
  beta = _beta;
  beta -= alpha;
  // Interpolate
  internal::lin_interp_reduced( b, beta, x, field );
  field += alpha;
  return field;
}

// 1-D simplex.  1-D space.
// Implementation of general interface.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray< 2, ads::FixedArray<1,T> >& 
		      positions,
		      const ads::FixedArray<2,F>& values,
		      const ads::FixedArray<1,T>& location )
{
  return linear_interpolation( positions[0][0], positions[1][0], 
			       values[0], values[1], location[0] );
}

// 1-D simplex.  2-D space.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray<2,T>& a, 
		      const ads::FixedArray<2,T>& b, 
		      const F& alpha, const F& beta,
		      const ads::FixedArray<2,T>& x )
{
  static ads::FixedArray<2,T> c;
  c[0] = a[0] + a[1] - b[1];
  c[1] = a[1] + b[0] - a[0];
  return linear_interpolation( a, b, c, alpha, beta, alpha, x );
}

// 1-D simplex.  2-D space.
// Implementation of general interface.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray< 2, ads::FixedArray<2,T> >& 
		      positions,
		      const ads::FixedArray<2,F>& values,
		      const ads::FixedArray<2,T>& location )
{
  return linear_interpolation( positions[0], positions[1],
			       values[0], values[1], location );
}


//
// 2-D.
//

// 2-D simplex.  2-D space.  Reduced problem.
namespace internal {

  template <typename T, typename F>
  inline
  void
  lin_interp_reduced( const ads::FixedArray<2,T>& b,
		      const ads::FixedArray<2,T>& c,
		      const F& beta, const F& gamma,
		      const ads::FixedArray<2,T>& x,
		      F& field )
  {
    // Used in computing the interpolated field value.
    static F s, t;

    const T den = b[0] * c[1] - b[1] * c[0];
    assert( den != 0 );

    /*
    field = ( (c[1] * beta - b[1] * gamma) * x[0] +
	      (- c[0] * beta + b[0] * gamma) * x[1] ) / den;
    */
    field = beta;
    field *= c[1];
    s = gamma;
    s *= b[1];
    field -= s;
    field *= x[0];
    
    s = beta;
    s *= - c[0];
    t = gamma;
    t *= b[0];
    s += t;
    s *= x[1];
    
    field += s;
    field /= den;
  }

}


// 2-D simplex.  2-D space.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray<2,T>& a,
		      const ads::FixedArray<2,T>& _b,
		      const ads::FixedArray<2,T>& _c,
		      const F& alpha, 
		      const F& _beta, 
		      const F& _gamma,
		      const ads::FixedArray<2,T>& _x )
{
  static ads::FixedArray<2,T> b, c, x;
  static F beta, gamma, field;
  b = _b;
  c = _c;
  x = _x;
  beta = _beta;
  gamma = _gamma;

  // Translate the nodes and x by -a.
  b -= a;
  c -= a;
  x -= a;
  // Translate the values by -alpha.
  beta -= alpha;
  gamma -= alpha;
  // Interpolate.
  internal::lin_interp_reduced( b, c, beta, gamma, x, field );
  field += alpha;
  return field;
}

// 2-D simplex.  2-D space.
// Implementation of general interface.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray< 3, ads::FixedArray<2,T> >& 
		      positions,
		      const ads::FixedArray< 3, F >& values,
		      const ads::FixedArray<2,T>& location )
{
  return linear_interpolation( positions[0], positions[1], positions[2],
			       values[0], values[1], values[2], location );
}

//
// 3-D.
//

// 3-D simplex.  3-D space.  Reduced problem.
namespace internal {

  template <typename T, typename F>
  inline
  void
  lin_interp_reduced( const ads::FixedArray<3,T>& b,
		      const ads::FixedArray<3,T>& c,
		      const ads::FixedArray<3,T>& d,
		      const F& beta, const F& gamma, const F& delta,
		      const ads::FixedArray<3,T>& x,
		      F& field )
  {
    static F s, t;
    const T den = ( (c[2]*d[1] - c[1]*d[2]) * b[0] +
		    (b[1]*d[2] - b[2]*d[1]) * c[0] +
		    (b[2]*c[1] - b[1]*c[2]) * d[0] );
    assert( den != 0 );

    /*
    const F p0 = ( (c[2]*d[1] - c[1]*d[2]) * beta +
		   (b[1]*d[2] - b[2]*d[1]) * gamma +
		   (b[2]*c[1] - b[1]*c[2]) * delta );
    const F p1 = ( (c[0]*d[2] - c[2]*d[0]) * beta +
		   (b[2]*d[0] - b[0]*d[2]) * gamma +
		   (b[0]*c[2] - b[2]*c[0]) * delta );
    const F p2 = ( (c[1]*d[0] - c[0]*d[1]) * beta +
		   (b[0]*d[1] - b[1]*d[0]) * gamma +
		   (b[1]*c[0] - b[0]*c[1]) * delta );
    field = ( p0 * x[0] + p1 * x[1] + p2 * x[2] ) / den;
    */

    s = beta;
    s *= c[2]*d[1] - c[1]*d[2];
    t = gamma;
    t *= b[1]*d[2] - b[2]*d[1];
    s += t;
    t = delta;
    t *= b[2]*c[1] - b[1]*c[2];
    s += t;
    s *= x[0];
    field = s;

    s = beta;
    s *= c[0]*d[2] - c[2]*d[0];
    t = gamma;
    t *= b[2]*d[0] - b[0]*d[2];
    s += t;
    t = delta;
    t *= b[0]*c[2] - b[2]*c[0];
    s += t;
    s *= x[1];
    field += s;

    s = beta;
    s *= c[1]*d[0] - c[0]*d[1];
    t = gamma;
    t *= b[0]*d[1] - b[1]*d[0];
    s += t;
    t = delta;
    t *= b[1]*c[0] - b[0]*c[1];
    s += t;
    s *= x[2];
    field += s;

    field /= den;
  }

}

// 3-D simplex.  3-D space.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray<3,T>& a,
		      const ads::FixedArray<3,T>& _b,
		      const ads::FixedArray<3,T>& _c,
		      const ads::FixedArray<3,T>& _d,
		      const F& alpha, 
		      const F& _beta, 
		      const F& _gamma, 
		      const F& _delta,
		      const ads::FixedArray<3,T>& _x )
{
  static ads::FixedArray<3,T> b, c, d, x;
  static F beta, gamma, delta, field;
  b = _b;
  c = _c;
  d = _d;
  x = _x;
  beta = _beta;
  gamma = _gamma;
  delta = _delta;

  // Translate the nodes and x by -a.
  b -= a;
  c -= a;
  d -= a;
  x -= a;
  // Translate the values by -alpha.
  beta -= alpha;
  gamma -= alpha;
  delta -= alpha;
  // Interpolate.
  internal::lin_interp_reduced( b, c, d, beta, gamma, delta, x, field );
  field += alpha;
  return field;
}

// 3-D simplex.  3-D space.
// Implementation of general interface.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray< 4, ads::FixedArray<3,T> >& 
		      positions,
		      const ads::FixedArray< 4, F >& values,
		      const ads::FixedArray<3,T>& location )
{
  return linear_interpolation( positions[0], positions[1], 
			       positions[2], positions[3],
			       values[0], values[1], values[2], values[3], 
			       location );
}

// 2-D simplex.  3-D space.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray<3,T>& a, 
		      const ads::FixedArray<3,T>& _b, 
		      const ads::FixedArray<3,T>& _c, 
		      const F& alpha, 
		      const F& _beta, 
		      const F& _gamma,
		      const ads::FixedArray<3,T>& _x )
{
  static ads::FixedArray<3,T> b, c, x, n;
  static F beta, gamma, field;
  static F zero( 0.0 );
  b = _b;
  c = _c;
  x = _x;
  beta = _beta;
  gamma = _gamma;

  // Translate the nodes and x by -a.
  b -= a;
  c -= a;
  x -= a;
  // Translate the values by -alpha.
  beta -= alpha;
  gamma -= alpha;
  // Normal to the triangle.
  n[0] = -b[2] * c[1] + b[1] * c[2];
  n[1] = b[2] * c[0] - b[0] * c[2];
  n[2] = -b[1] * c[0] + b[0] * c[1];
  // Interpolate.
  internal::lin_interp_reduced( b, c, n, beta, gamma, zero, x, field );
  field += alpha;
  return field;
}

// 2-D simplex.  3-D space.
// Implementation of general interface.
template <typename T, typename F>
inline
const F&
linear_interpolation( const ads::FixedArray< 3, ads::FixedArray<3,T> >& 
		      positions,
		      const ads::FixedArray< 3, F >& values,
		      const ads::FixedArray<3,T>& location )
{
  return linear_interpolation( positions[0], positions[1], positions[2],
			       values[0], values[1], values[2], location );
}

END_NAMESPACE_NUMERICAL

// End of file.
