// -*- C++ -*-

#if !defined(__ads_algorithm_extremeElement_ipp__)
#error This file is an implementation detail of extremeElement.
#endif

BEGIN_NAMESPACE_ADS


// Return the minimum element in a range of even length.
template<typename _RandomAccessIterator>
inline
_RandomAccessIterator
findMinimumElementUnrolledEven(_RandomAccessIterator begin, _RandomAccessIterator end) {
  return findExtremeElementUnrolledEven
    (begin, end, 
     std::less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

// Return the minimum element in a range of even length.
template<typename _RandomAccessIterator>
inline
_RandomAccessIterator
findMinimumElementUnrolledOdd(_RandomAccessIterator begin, _RandomAccessIterator end) {
  return findExtremeElementUnrolledOdd
    (begin, end, 
     std::less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

// Return the minimum element in a range.
template<typename _RandomAccessIterator>
inline
_RandomAccessIterator
findMinimumElementUnrolled(_RandomAccessIterator begin, 
			   _RandomAccessIterator end) {
  return findExtremeElementUnrolled
    (begin, end, 
     std::less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

// Return the maximum element in a range of even length.
template<typename _RandomAccessIterator>
inline
_RandomAccessIterator
findMaximumElementUnrolledEven(_RandomAccessIterator begin, _RandomAccessIterator end) {
  return findExtremeElementUnrolledEven
    (begin, end, 
     std::greater<typename 
     std::iterator_traits<_RandomAccessIterator>::value_type>());
}

// Return the maximum element in a range of even length.
template<typename _RandomAccessIterator>
inline
_RandomAccessIterator
findMaximumElementUnrolledOdd(_RandomAccessIterator begin, _RandomAccessIterator end) {
  return findExtremeElementUnrolledOdd
    (begin, end, 
     std::greater<typename 
     std::iterator_traits<_RandomAccessIterator>::value_type>());
}

// Return the maximum element in a range.
template<typename _RandomAccessIterator>
inline
_RandomAccessIterator
findMaximumElementUnrolled(_RandomAccessIterator begin, 
			   _RandomAccessIterator end) {
  return findExtremeElementUnrolled
    (begin, end, 
     std::greater<typename 
     std::iterator_traits<_RandomAccessIterator>::value_type>());
}

// Return the extreme element in a range of even length.
template<typename _RandomAccessIterator, typename _BinaryPredicate>
inline
_RandomAccessIterator
findExtremeElementUnrolledEven(_RandomAccessIterator begin,
			       _RandomAccessIterator end,
			       _BinaryPredicate compare) {
#ifdef DEBUG_ads
  // Check for invalid range.
  assert(end - begin >= 2);
  assert((end - begin) % 2 == 0);
#endif
  
  _RandomAccessIterator even = begin, odd = begin + 1;
  for (begin += 2 ; begin != end; begin += 2) {
    if (compare(*begin, *even)) {
      even = begin;
    }
    if (compare(*(begin + 1), *odd)) {
      odd = begin + 1;
    }
  }

  if (compare(*even, *odd)) {
    return even;
  }
  else {
    return odd;
  }

#if 0
  // Unrolling to a depth of 4 is not as efficient.
#ifdef DEBUG_ads
  // Check for invalid range.
  assert(end - begin >= 4);
  assert((end - begin) % 4 == 0);
#endif
  
  _RandomAccessIterator i0 = begin, i1 = begin + 1, i2 = begin + 2, 
    i3 = begin + 3;
  for (begin += 4 ; begin != end; begin += 4) {
    if (compare(*begin, *i0)) {
      i0 = begin;
    }
    if (compare(*(begin + 1), *i1)) {
      i1 = begin + 1;
    }
    if (compare(*(begin + 2), *i2)) {
      i2 = begin + 2;
    }
    if (compare(*(begin + 3), *i3)) {
      i3 = begin + 3;
    }
  }

  if (compare(*i1, *i0)) {
    i0 = i1;
  }
  if (compare(*i3, *i2)) {
    i2 = i3;
  }

  if (compare(*i0, *i2)) {
    return i0;
  }
  else {
    return i2;
  }
#endif

#if 0
  // Not as efficient for larger sizes.
  _RandomAccessIterator minimum = begin;
  _RandomAccessIterator i;
  for ( ; begin != end; begin += 2) {
    i = begin + 1;
    if (compare(*begin, *i)) {
      if (compare(*begin, *minimum)) {
	minimum = begin;
      }
    }
    else {
      if (compare(*i, *minimum)) {
	minimum = i;
      }
    }
  }

  return minimum;
#endif
}

// Return the extreme element in a range of odd length.
template<typename _RandomAccessIterator, typename _BinaryPredicate>
inline
_RandomAccessIterator
findExtremeElementUnrolledOdd(_RandomAccessIterator begin,
			      _RandomAccessIterator end,
			      _BinaryPredicate compare) {
#ifdef DEBUG_ads
  // Check for invalid range.
  assert(end - begin >= 1);
  assert((end - begin) % 2 == 1);
#endif

  _RandomAccessIterator even = begin, odd = begin;
  for (++begin; begin != end; begin += 2) {
    if (compare(*begin, *odd)) {
      odd = begin;
    }
    if (compare(*(begin + 1), *even)) {
      even = begin + 1;
    }
  }

  if (compare(*even, *odd)) {
    return even;
  }
  else {
    return odd;
  }

#if 0
  // Not as efficient for larger sizes.
  _RandomAccessIterator minimum = begin;
  _RandomAccessIterator i;
  for (++begin ; begin != end; begin += 2) {
    i = begin + 1;
    if (compare(*begin, *i)) {
      if (compare(*begin, *minimum)) {
	minimum = begin;
      }
    }
    else {
      if (compare(*i, *minimum)) {
	minimum = i;
      }
    }
  }
  return minimum;
#endif
}

// Return the extreme element in a range.
template<typename _RandomAccessIterator, typename _BinaryPredicate>
inline
_RandomAccessIterator
findExtremeElementUnrolled(_RandomAccessIterator begin, 
			   _RandomAccessIterator end,
			   _BinaryPredicate compare) {
#ifdef DEBUG_ads
  // Check for invalid range.
  assert(end - begin >= 1);
#endif

  _RandomAccessIterator even = begin, odd = begin;
  // If the size of the range is odd.
  if ((end - begin) % 2) {
    ++begin;
  }
  for ( ; begin != end; begin += 2) {
    if (compare(*begin, *even)) {
      even = begin;
    }
    if (compare(*(begin + 1), *odd)) {
      odd = begin + 1;
    }
  }

  if (compare(*even, *odd)) {
    return even;
  }
  else {
    return odd;
  }

#if 0
  // Not as efficient for larger sizes.
  _RandomAccessIterator minimum = begin;
  // If the size of the range is odd.
  if (std::distance(begin, end) % 2) {
    ++begin;
  }
  _RandomAccessIterator i;
  for ( ; begin != end; begin += 2) {
    i = begin + 1;
    if (compare(*begin, *i)) {
      if (compare(*begin, *minimum)) {
	minimum = begin;
      }
    }
    else {
      if (compare(*i, *minimum)) {
	minimum = i;
      }
    }
  }
  return minimum;
#endif
}

#if 0
    // This has about the same performance as unrolling to depth of 2.
    typename ads::Array<1, Key>::const_iterator minimum = _keys.begin();
    typename ads::Array<1, Key>::const_iterator i1, i2, i3;
    for (typename ads::Array<1, Key>::const_iterator i0 = _keys.begin();
	 i0 != _keys.end(); i0 += 4) {
      i1 = i0 + 1;
      i2 = i0 + 2;
      i3 = i0 + 3;
      if (*i0 < *i1) {
	if (*i2 < *i3) {

	  if (*i0 < *i2) {
	    if (*i0 < *minimum) {
	      minimum = i0;
	    }
	  }
	  else {
	    if (*i2 < *minimum) {
	      minimum = i2;
	    }
	  }

	}
	else { // *i2 >= *i3

	  if (*i0 < *i3) {
	    if (*i0 < *minimum) {
	      minimum = i0;
	    }
	  }
	  else {
	    if (*i3 < *minimum) {
	      minimum = i3;
	    }
	  }

	}
      }
      else { // *i0 >= *i1
	if (*i2 < *i3) {

	  if (*i1 < *i2) {
	    if (*i1 < *minimum) {
	      minimum = i1;
	    }
	  }
	  else {
	    if (*i2 < *minimum) {
	      minimum = i2;
	    }
	  }

	}
	else { // *i2 >= *i3

	  if (*i1 < *i3) {
	    if (*i1 < *minimum) {
	      minimum = i1;
	    }
	  }
	  else {
	    if (*i3 < *minimum) {
	      minimum = i3;
	    }
	  }

	}
      }
    }
    return _topIndex = minimum - _keys.begin();
#endif
#if 0
    // Unrolling to a depth of 4 does not perform as well as unrolling to a 
    // depth of 2.
    typename ads::Array<1, Key>::const_iterator minimum = _keys.begin();
    typename ads::Array<1, Key>::const_iterator i1, i2, i3;
    for (typename ads::Array<1, Key>::const_iterator i0 = _keys.begin();
	 i0 != _keys.end(); i0 += 4) {
      i1 = i0 + 1;
      i2 = i0 + 2;
      i3 = i0 + 3;
      if (*i0 < *i1) {
	i1 = i0;
      }
      if (*i2 < *i3) {
	i3 = i2;
      }
      if (*i1 < *i3) {
	if (*i1 < *minimum) {
	  minimum = i1;
	}
      }
      else {
	if (*i3 < *minimum) {
	  minimum = i3;
	}
      }
    }
    return _topIndex = minimum - _keys.begin();
#endif
#if 0
    // Unrolling to a depth of 4 does not perform as well as unrolling to a 
    // depth of 2.
    typename ads::Array<1, Key>::const_iterator minimum = _keys.begin();
    typename ads::Array<1, Key>::const_iterator i1, i2, i3;
    for (typename ads::Array<1, Key>::const_iterator i0 = _keys.begin();
	 i0 != _keys.end(); i0 += 4) {
      i1 = i0 + 1;
      i2 = i0 + 2;
      i3 = i0 + 3;
      if (*i0 < *i2) {
	i2 = i0;
      }
      if (*i1 < *i3) {
	i3 = i1;
      }
      if (*i2 < *i3) {
	if (*i2 < *minimum) {
	  minimum = i2;
	}
      }
      else {
	if (*i3 < *minimum) {
	  minimum = i3;
	}
      }
    }
    return _topIndex = minimum - _keys.begin();
#endif

END_NAMESPACE_ADS
