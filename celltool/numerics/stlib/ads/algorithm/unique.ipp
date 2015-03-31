// -*- C++ -*-

#if !defined(__ads_algorithm_unique_ipp__)
#error This file is an implementation detail of unique.
#endif

BEGIN_NAMESPACE_ADS

// Return true if the elements are unique.
template<typename InputIterator>
inline
bool
areElementsUnique(InputIterator first, InputIterator last) {
  typedef typename std::iterator_traits<InputIterator>::value_type Value;

  // Call the function below which the usual less than and equal to.
  return areElementsUnique(first, last, std::less<Value>(), 
			   std::equal_to<Value>());
}

// Return true if the elements are unique.
template<typename InputIterator, typename StrictWeakOrdering, 
	 typename BinaryPredicate>
inline
bool
areElementsUnique(InputIterator first, InputIterator last, 
		  StrictWeakOrdering ordering, BinaryPredicate pred) {
  typedef typename std::iterator_traits<InputIterator>::value_type Value;
  
  // Copy the elements into vector.
  std::vector<Value> v(first, last);

  // Sort the elements.
  std::sort(v.begin(), v.end(), ordering);

  // Return true if all of the elements are unique.
  return std::unique(v.begin(), v.end(), pred) == v.end();
}

END_NAMESPACE_ADS
