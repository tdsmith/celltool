// -*- C++ -*-

#if !defined(__ads_algorithm_statistics_ipp__)
#error This file is an implementation detail of statistics.
#endif

BEGIN_NAMESPACE_ADS


// Compute the minimum value for the elements in the range.
template<typename InputIterator>
inline
typename std::iterator_traits<InputIterator>::value_type
computeMinimum(InputIterator beginning, InputIterator end) {
  typedef typename std::iterator_traits<InputIterator>::value_type Number;
  Number minimum = std::numeric_limits<Number>::max(), value;
  // For each element.
  for (; beginning != end; ++beginning) {
    value = *beginning;
    if (value < minimum) {
      minimum = value;
    }
  }
  return minimum;
}


// Compute the maximum value for the elements in the range.
template<typename InputIterator>
inline
typename std::iterator_traits<InputIterator>::value_type
computeMaximum(InputIterator beginning, InputIterator end) {
  typedef typename std::iterator_traits<InputIterator>::value_type Number;
  Number maximum = -std::numeric_limits<Number>::max(), value;
  // For each element.
  for (; beginning != end; ++beginning) {
    value = *beginning;
    if (value > maximum) {
      maximum = value;
    }
  }
  return maximum;
}


// Compute the minimum and maximum values for the elements in the range.
template<typename InputIterator, typename T>
inline
void
computeMinimumAndMaximum(InputIterator beginning, InputIterator end,
			 T* minimum, T* maximum) {
  *minimum = std::numeric_limits<T>::max();
  *maximum = -std::numeric_limits<T>::max();
  T value;
  // For each element.
  for (; beginning != end; ++beginning) {
    value = *beginning;
    if (value < *minimum) {
      *minimum = value;
    }
    if (value > *maximum) {
      *maximum = value;
    }
  }
}


// Compute the mean value for the elements in the range.
template<typename InputIterator>
inline
typename std::iterator_traits<InputIterator>::value_type
computeMean(InputIterator beginning, InputIterator end) {
  typedef typename std::iterator_traits<InputIterator>::value_type Number;
  Number mean = 0, value;
  int size = 0;
  // For each element.
  for (; beginning != end; ++beginning) {
    value = *beginning;
    mean += value;
    ++size;
  }
  if (size != 0) {
    mean /= size;
  }
  return mean;
}



// Compute the minimum, maximum, and mean for the elements in the range.
template<typename InputIterator, typename T>
inline
void
computeMinimumMaximumAndMean(InputIterator beginning, InputIterator end,
			     T* minimum, T* maximum, T* mean) {
  *minimum = std::numeric_limits<T>::max();
  *maximum = -std::numeric_limits<T>::max();
  *mean = 0;
  T value;
  int size = 0;
  // For each element.
  for (; beginning != end; ++beginning) {
    value = *beginning;
    if (value < *minimum) {
      *minimum = value;
    }
    if (value > *maximum) {
      *maximum = value;
    }
    *mean += value;
    ++size;
  }
  if (size != 0) {
    *mean /= size;
  }
}


// Compute the mean and variance for the elements in the range.
template<typename ForwardIterator, typename T>
void
computeMeanAndVariance(ForwardIterator beginning, ForwardIterator end,
		       T* mean, T* variance) {
  *mean = computeMean(beginning, end);
  const T mn = *mean;
  T var = 0, eps = 0, diff;
  int size = 0;
  for (; beginning != end; ++beginning) {
    diff = *beginning - mn;
    eps += diff;
    var += diff * diff;
    ++size;
  }
  assert(size > 1);
  *variance = (var - eps * eps / size) / (size - 1);
}


// Compute the mean, variance, skew and curtosis for the elements in the range.
template<typename ForwardIterator, typename T>
inline
void
computeMeanAbsoluteDeviationVarianceSkewAndCurtosis
(ForwardIterator beginning, ForwardIterator end,
 T* meanPtr, T* absoluteDeviationPtr, T* variancePtr, T* skewPtr, 
 T* curtosisPtr) {
  // Compute the mean.
  const T mean = computeMean(beginning, end);

  T absoluteDeviation = 0, variance = 0, skew = 0, curtosis = 0;
  T epsilon = 0, difference, t;
  int size = 0;
  for (; beginning != end; ++beginning) {
    // The difference between the element and the mean.
    difference = t = *beginning - mean;
    absoluteDeviation += std::abs(difference);
    epsilon += difference;
    variance += (t *= difference);
    skew += (t *= difference);
    curtosis += (t *= difference);
    ++size;
  }
  assert(size > 1);

  absoluteDeviation /= size;
  variance = (variance - epsilon * epsilon / size) / (size - 1);
  const T standardDeviation = std::sqrt(variance);
  if (variance == 0) {
    std::cerr << "Warning: The skew and curtosis are not defined when the variance is zero.\n";
  }
  else {
    skew /= size * variance * standardDeviation;
    curtosis = curtosis / (size * variance * variance) - 3.;
  }

  *meanPtr = mean;
  *absoluteDeviationPtr = absoluteDeviation;
  *variancePtr = variance;
  *skewPtr = skew;
  *curtosisPtr = curtosis;
}

END_NAMESPACE_ADS
