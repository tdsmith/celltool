// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionMaximumMean_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionMaximumMean.
#endif

BEGIN_NAMESPACE_NUMERICAL


//! Maximum allowed mean for using the PoissonGeneratorInversion class.
/*!
  If the mean is too large, we will get underflow in computing the
  probability density function.
*/
template<typename T>
class PoissonGeneratorInversionMaximumMean {
public:
  //! Invalid value for an unknown number type.
  enum {Value = -1};
};

template<>
class PoissonGeneratorInversionMaximumMean<double> {
 public:
  //! - std::log(std::numeric_limits<double>::min()) == 708.396
  enum {Value = 708};
};

template<>
class PoissonGeneratorInversionMaximumMean<float> {
 public:
  //! - std::log(std::numeric_limits<float>::min()) == 87.3365
  /*!
    Here I assume that the arithmetic is actually done in single precision.
    However, floats are typically converted to double's.
  */
  enum {Value = 87};
};


END_NAMESPACE_NUMERICAL

// End of file.
