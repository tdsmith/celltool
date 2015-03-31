// -*- C++ -*-

/*! 
  \file ads/algorithm/statistics.h
  \brief Statistics functions.
*/

#if !defined(__ads_algorithm_statistics_h__)
#define __ads_algorithm_statistics_h__

#include "../defs.h"

#include <algorithm>
#include <iostream>
#include <limits>

#include <cmath>
#include <cassert>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup algorithm_statistics Algorithm: Statistics functions */
// @{


//! Compute the minimum value for the elements in the range.
template<typename InputIterator>
typename std::iterator_traits<InputIterator>::value_type
computeMinimum(InputIterator beginning, InputIterator end);


//! Compute the maximum value for the elements in the range.
template<typename InputIterator>
typename std::iterator_traits<InputIterator>::value_type
computeMaximum(InputIterator beginning, InputIterator end);


//! Compute the minimum and maximum values for the elements in the range.
template<typename InputIterator, typename T>
void
computeMinimumAndMaximum(InputIterator beginning, InputIterator end,
			 T* minimum, T* maximum);


//! Compute the mean value for the elements in the range.
template<typename InputIterator>
typename std::iterator_traits<InputIterator>::value_type
computeMean(InputIterator beginning, InputIterator end);


//! Compute the minimum, maximum, and mean for the elements in the range.
template<typename InputIterator, typename T>
void
computeMinimumMaximumAndMean(InputIterator beginning, InputIterator end,
			     T* minimum, T* maximum, T* mean);


//! Compute the mean and variance for the elements in the range.
/*!
  To compute the variance, I use the <em>corrected two-pass algorithm</em>
  presented in "Numerical Recipes."
  \f[
  \mathrm{var}(x) = \frac{1}{N - 1} \left( 
  \sum_{j = 0}^{N - 1} (x_j - \bar{x})^2
  - \frac{1}{N} \left( \sum_{j = 0}^{N - 1} (x_j - \bar{x}) \right)^2 \right)
  \f]
  Note that with exact arithmetic, the second sum is zero.  With finite 
  precision arithmetic, the term reduces the round-off error.
  CONTINUE.
*/
template<typename ForwardIterator, typename T>
void
computeMeanAndVariance(ForwardIterator beginning, ForwardIterator end,
		       T* mean, T* variance);


//! Compute the variance for the elements in the range.
template<typename ForwardIterator>
inline
typename std::iterator_traits<ForwardIterator>::value_type
computeVariance(ForwardIterator beginning, ForwardIterator end) {
  typename std::iterator_traits<ForwardIterator>::value_type mean, variance;
  computeMeanAndVariance(beginning, end, &mean, &variance);
  return variance;
}


//! Compute the mean, absolute deviation, variance, skew and curtosis for the elements in the range.
/*!
  To compute the variance, I use the <em>corrected two-pass algorithm</em>
  presented in "Numerical Recipes."
  \f[
  \mathrm{variance}(x) = \frac{1}{N - 1} \left( 
  \sum_{j = 0}^{N - 1} (x_j - \bar{x})^2
  - \frac{1}{N} \left( \sum_{j = 0}^{N - 1} (x_j - \bar{x}) \right)^2 \right)
  \f]
  Note that with exact arithmetic, the second sum is zero.  With finite 
  precision arithmetic, the term reduces the round-off error.  The 
  mean absolute deviation, skew, and curtosis are defined below.
  \f[
  \mathrm{absoluteDeviation}(x) = \frac{1}{N} 
  \sum_{j = 0}^{N - 1} |x_j - \bar{x}|
  \f]
  \f[
  \mathrm{skew}(x) = \frac{1}{N \sigma^3} 
  \sum_{j = 0}^{N - 1} (x_j - \bar{x})^3
  \f]
  \f[
  \mathrm{curtosis}(x) = \left( \frac{1}{N \sigma^4} 
  \sum_{j = 0}^{N - 1} (x_j - \bar{x})^4 \right) - 3
  \f]
  Note that the skew and curtosis are not defined when the variance is zero.
  in this case, the function prints a warning.
*/
template<typename ForwardIterator, typename T>
void
computeMeanAbsoluteDeviationVarianceSkewAndCurtosis
(ForwardIterator beginning, ForwardIterator end,
 T* mean, T* absoluteDeviation, T* variance, T* skew, T* curtosis);


// @}

END_NAMESPACE_ADS

#define __ads_algorithm_statistics_ipp__
#include "statistics.ipp"
#undef __ads_algorithm_statistics_ipp__

#endif
