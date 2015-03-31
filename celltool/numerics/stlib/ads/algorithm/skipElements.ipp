// -*- C++ -*-

#if !defined(__ads_algorithm_skipElements_ipp__)
#error This file is an implementation detail of skipElements.
#endif

BEGIN_NAMESPACE_ADS


// Advance the iterator while it's value is equal to any of the elements 
// in the range.  Return the advanced iterator.
template<typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1
skipElementsUsingIteration(ForwardIterator1 iterator,
			   ForwardIterator2 beginning,
			   ForwardIterator2 end) {
  bool didChange;
  ForwardIterator2 i;
  do {
    didChange = false;
    for (i = beginning; i != end; ++i) {
      if (*i == *iterator) {
	++iterator;
	didChange = true;
      }
    }
  } while (didChange);
  return iterator;
}


// Advance the iterator while it is equal to any of the elements in the range.
// Return the advanced iterator.
template<typename ForwardIterator, typename IteratorForwardIterator>
inline
ForwardIterator
skipIteratorsUsingIteration(ForwardIterator iterator,
			    IteratorForwardIterator beginning,
			    IteratorForwardIterator end) {
  bool didChange;
  IteratorForwardIterator i;
  do {
    didChange = false;
    for (i = beginning; i != end; ++i) {
      if (*i == iterator) {
	++iterator;
	didChange = true;
      }
    }
  } while (didChange);
  return iterator;
}


END_NAMESPACE_ADS
