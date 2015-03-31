// -*- C++ -*-

#if !defined(__ads_insertion_sort_ipp__)
#error This file is an implementation detail of insertion_sort.
#endif

BEGIN_NAMESPACE_ADS

template<typename RandomAccessIterator, typename T>
inline
void
unguarded_linear_insert(RandomAccessIterator last, T val) {
  RandomAccessIterator next = last;
  --next;
  while (val < *next) {
    *last = *next;
    last = next;
    --next;
  }
  *last = val;
}

template<typename RandomAccessIterator, typename T, typename Compare>
inline
void
unguarded_linear_insert(RandomAccessIterator last, T val, Compare comp) {
  RandomAccessIterator next = last;
  --next;
  while (comp(val, *next)) {
    *last = *next;
    last = next;
    --next;
  }
  *last = val;
}

template<typename RandomAccessIterator>
inline
void
insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first == last) {
    return;
  }

  for (RandomAccessIterator i = first + 1; i != last; ++i) {
    typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
    if (val < *first) {
      std::copy_backward(first, i, i + 1);
      *first = val;
    }
    else {
      unguarded_linear_insert(i, val);
    }
  }
}

template<typename RandomAccessIterator, typename Compare>
inline
void
insertion_sort(RandomAccessIterator first, RandomAccessIterator last,
	       Compare comp) {
  if (first == last) {
    return;
  }

  for (RandomAccessIterator i = first + 1; i != last; ++i) {
    typename std::iterator_traits<RandomAccessIterator>::value_type val = *i;
    if (comp(val, *first)) {
      std::copy_backward(first, i, i + 1);
      *first = val;
    }
    else {
      unguarded_linear_insert(i, val, comp);
    }
  }
}

END_NAMESPACE_ADS
