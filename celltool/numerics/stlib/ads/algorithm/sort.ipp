// -*- C++ -*-

#if !defined(__ads_algorithm_sort_ipp__)
#error This file is an implementation detail of sort.
#endif

BEGIN_NAMESPACE_ADS


// Sort the two ranges together, using the first for comparisons.
template<typename RandomAccessIterator1, typename RandomAccessIterator2,
	 typename Compare>
inline
void
sortTogether(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
	     RandomAccessIterator2 begin2, RandomAccessIterator2 end2,
	     Compare compare1) {
  typedef typename std::iterator_traits<RandomAccessIterator1>::value_type
    Value1;
  typedef typename std::iterator_traits<RandomAccessIterator2>::value_type
    Value2;
  typedef std::pair<Value1, Value2> Pair;

  // The ranges should be of equal length.
  assert(std::distance(begin1, end1) == std::distance(begin2, end2));

  // Make a vector of the pairs.
  std::vector<Pair> data(std::distance(begin1, end1));
  {
    RandomAccessIterator1 i1 = begin1;
    RandomAccessIterator2 i2 = begin2;
    for (typename std::vector<Pair>::iterator i = data.begin(); 
	 i != data.end(); ++i, ++i1, ++i2) {
      i->first = *i1;
      i->second = *i2;
    }
  }
  
  // Sort the data by the first component.
  /* CONTINUE
  ads::binary_compose_binary_unary<std::less<Value1>, ads::Select1st<Pair>,
    ads::Select1st<Pair> > compare;
  */
  ads::binary_compose_binary_unary<Compare, ads::Select1st<Pair>,
    ads::Select1st<Pair> > compare;
  std::sort(data.begin(), data.end(), compare);
  
  // Copy the sorted data into the two ranges.
  {
    RandomAccessIterator1 i1 = begin1;
    RandomAccessIterator2 i2 = begin2;
    for (typename std::vector<Pair>::iterator i = data.begin(); 
	 i != data.end(); ++i, ++i1, ++i2) {
      *i1 = i->first;
      *i2 = i->second;
    }
  }
}


// Sort the three ranges together, using the first for comparisons.
template<typename RandomAccessIterator1, typename RandomAccessIterator2,
	 typename RandomAccessIterator3, typename Compare>
inline
void
sortTogether(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
	     RandomAccessIterator2 begin2, RandomAccessIterator2 end2,
	     RandomAccessIterator3 begin3, RandomAccessIterator3 end3,
	     Compare compare1) {
  typedef typename std::iterator_traits<RandomAccessIterator1>::value_type
    Value1;
  typedef typename std::iterator_traits<RandomAccessIterator2>::value_type
    Value2;
  typedef typename std::iterator_traits<RandomAccessIterator3>::value_type
    Value3;
  typedef ads::Triplet<Value1, Value2, Value3> Triplet;

  // The ranges should be of equal length.
  assert(std::distance(begin1, end1) == std::distance(begin2, end2) &&
	 std::distance(begin1, end1) == std::distance(begin3, end3));

  // Make a vector of the triplets.
  std::vector<Triplet> data(std::distance(begin1, end1));
  {
    RandomAccessIterator1 i1 = begin1;
    RandomAccessIterator2 i2 = begin2;
    RandomAccessIterator3 i3 = begin3;
    for (typename std::vector<Triplet>::iterator i = data.begin(); 
	 i != data.end(); ++i, ++i1, ++i2, ++i3) {
      i->first = *i1;
      i->second = *i2;
      i->third = *i3;
    }
  }
  
  // Sort the data by the first component.
  ads::binary_compose_binary_unary<Compare, ads::Select1st<Triplet>,
    ads::Select1st<Triplet> > compare;
  std::sort(data.begin(), data.end(), compare);
  
  // Copy the sorted data into the three ranges.
  {
    RandomAccessIterator1 i1 = begin1;
    RandomAccessIterator2 i2 = begin2;
    RandomAccessIterator3 i3 = begin3;
    for (typename std::vector<Triplet>::iterator i = data.begin(); 
	 i != data.end(); ++i, ++i1, ++i2, ++i3) {
      *i1 = i->first;
      *i2 = i->second;
      *i3 = i->third;
    }
  }
}


// Compute the order for the elements.
template<typename InputIterator, typename IntOutputIterator>
inline
void
computeOrder(InputIterator begin, InputIterator end, IntOutputIterator order) {
  // The element type.
  typedef typename std::iterator_traits<InputIterator>::value_type Value;

  // Copy the elements.
  std::vector<Value> copy(begin, end);
  // The index vector.
  std::vector<int> index(copy.size());
  for (std::size_t i = 0; i != index.size(); ++i) {
    index[i] = i;
  }

  // Sort the copy and the index vector together to get the order.
  sortTogether(copy.begin(), copy.end(), index.begin(), index.end());

  // Record the order.
  for (std::vector<int>::const_iterator i = index.begin(); i != index.end();
       ++i) {
    *order++ = *i;
  }
}

// Order the elements by rank.
template<typename RandomAccessIterator, typename IntInputIterator>
inline
void
orderByRank(RandomAccessIterator begin, RandomAccessIterator end,
	    IntInputIterator ranks) {
  // The element type.
  typedef typename std::iterator_traits<RandomAccessIterator>::value_type 
    Value;

  // Copy the elements.
  std::vector<Value> copy(begin, end);

  // Order the elements by rank.
  const int size = copy.size();
  for (typename std::vector<Value>::const_iterator i = copy.begin();
       i != copy.end(); ++i, ++ranks) {
    // Make sure the rank is in the correct range.
    assert(0 <= *ranks && *ranks < size);
    begin[*ranks] = *i;
  }
}

END_NAMESPACE_ADS
