// -*- C++ -*-

#if !defined(__numerical_polynomial_evaluate_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_NUMERICAL


namespace {

  //
  // C++ does not allow partial specializations of functions.  Here I use 
  // the trick of converting an integer to a type in order to achieve
  // the specializations.
  //

  // Evaluate the polynomial of the specified order.
  template<int N, typename T>
  inline
  T
  evaluatePolynomial(const T x, const T* coefficients, 
		     Loki::Int2Type<N> /*dummy*/) {
    LOKI_STATIC_CHECK(N > 3, BadOrder);
    int n = N;
    T result = coefficients[n--];
    while (n >= 0) {
      result = result * x + coefficients[n--];
    }
    return result;
  }


  // Evaluate the cubic polynomial.
  template<typename T>
  inline
  T
  evaluatePolynomial(const T x, const T* coefficients,
		     Loki::Int2Type<3> /*dummy*/) {
    return ((coefficients[3] * x + coefficients[2]) * x
	    + coefficients[1]) * x + coefficients[0];
  }


  // Evaluate the quadratic polynomial.
  template<typename T>
  inline
  T
  evaluatePolynomial(const T x, const T* coefficients,
		     Loki::Int2Type<2> /*dummy*/) {
    return (coefficients[2] * x + coefficients[1]) * x + coefficients[0];
  }


  // Evaluate the linear polynomial.
  template<typename T>
  inline
  T
  evaluatePolynomial(const T x, const T* coefficients,
		     Loki::Int2Type<1> /*dummy*/) {
    return coefficients[1] * x + coefficients[0];
  }


  // Evaluate the constant polynomial.
  template<typename T>
  inline
  T
  evaluatePolynomial(const T x, const T* coefficients,
		     Loki::Int2Type<0> /*dummy*/) {
    return coefficients[0];
  }
}


// Evaluate the polynomial of the specified order.
template<int N, typename T>
inline
T
evaluatePolynomial(const T x, const T* coefficients) {
  return evaluatePolynomial(x, coefficients, Loki::Int2Type<N>());
}


END_NAMESPACE_NUMERICAL

// End of file.
