// -*- C++ -*-

#if !defined(__ads_SparseArray2_ipp__)
#error This file is an implementation detail of the class SparseArray.
#endif

BEGIN_NAMESPACE_ADS

template<typename T>
template<typename IndexBiDirIter, typename ValueForwardIter>
inline
SparseArray<2,T>::
SparseArray(IndexBiDirIter indices_begin, IndexBiDirIter indices_end,
	    ValueForwardIter values_begin, ValueForwardIter values_end,
	    parameter_type nullValue) :
  Base(ads::TransformIterator< IndexBiDirIter, 
       ads::SelectElement< typename 
       std::iterator_traits<IndexBiDirIter>::value_type, 0 > >
       (indices_begin),
       ads::TransformIterator< IndexBiDirIter, 
       ads::SelectElement< typename 
       std::iterator_traits<IndexBiDirIter>::value_type, 0 > >
       (indices_end),
       values_begin, values_end, nullValue),
  _offsets() {
  // I do this here instead of in the initializer list because the array 
  // might be empty.
  if (indices_begin != indices_end) {
    IndexBiDirIter i = indices_end;
    --i;
    _offsets.resize(ads::IndexRange<1>((*indices_begin)[1], (*i)[1] + 2));
  }

  // Set the offsets.
  int n = 0;
  for (int i = _offsets.lbound(); i != _offsets.ubound(); ++i) {
    while ((*indices_begin)[1] < i && indices_begin != indices_end) {
      ++indices_begin;
      ++n;
    }
    _offsets(i) = n;
  }
}


template<typename T>
inline
bool
SparseArray<2,T>::
isNull(const index_type& index) const {
  // First check the second component of the index.
  if (index[1] < _offsets.lbound() || _offsets.ubound() - 1 <= index[1]) {
    return true;
  }

  // Then do a binary search to find the first component of the index.
  const Array<1,int>::const_iterator begin 
    = _indices.begin() + _offsets(index[1]);
  const Array<1,int>::const_iterator end 
    = _indices.begin() + _offsets(index[1] + 1);
  Array<1,int>::const_iterator i = std::lower_bound(begin, end, index[0]);

  // If the index does not exist.
  if (i == end || *i != index[0]) {
    // The element is null.
    return true;
  }
  // If the index does exist, the element is non-null.
  return false;
}


template<typename T>
inline
typename SparseArray<2,T>::parameter_type
SparseArray<2,T>::
operator()(const index_type& index) const {
  // First check the second component of the index.
  if (index[1] < _offsets.lbound() || _offsets.ubound() - 1 <= index[1]) {
    return _null;
  }

  // Then do a binary search to find the first component of the index.
  const Array<1,int>::const_iterator begin 
    = _indices.begin() + _offsets(index[1]);
  const Array<1,int>::const_iterator end 
    = _indices.begin() + _offsets(index[1] + 1);
  Array<1,int>::const_iterator i = std::lower_bound(begin, end, index[0]);

  // If the index does not exist.
  if (i == end || *i != index[0]) {
    // Return the null value;
    return _null;
  }
  // If the index does exist, return the value.
  return operator[](int(i - _indices.begin()));
}


template<typename T>
template<typename T2, bool A>
inline
void
SparseArray<2,T>::
fill(ads::Array<2,T2,A>* array) const {
  // First set all the elements to the null value.
  *array = getNull();
  // Then fill in the non-null values.
  fillNonNull(array);
}


template<typename T>
template<typename T2, bool A>
inline
void
SparseArray<2,T>::
fillNonNull(ads::Array<2,T2,A>* array) const {
  Array<1,int>::const_iterator ii_begin, ii_end;
  typename Array<1,value_type>::const_iterator vi;

  const int j_begin = std::max(array->lbound(1), _offsets.lbound());
  const int j_end = std::min(array->ubound(1), _offsets.ubound() - 1);
  for (int j = j_begin; j < j_end; ++j) {
    // Do a binary search to find the lower bound of the first component 
    // of the index range.
    ii_begin = _indices.begin() + _offsets(j);
    ii_end = _indices.begin() + _offsets(j+1);
    vi = begin() + _offsets(j);
    Array<1,int>::const_iterator ii = 
      std::lower_bound(ii_begin, ii_end, array->lbound(0));
    for (; ii != ii_end && *ii < array->ubound(0); ++ii, ++vi) {
      (*array)(*ii, j) = *vi;
    }
  }
}


template<typename T>
inline
void
SparseArray<2,T>::
put(std::ostream& out) const {
  // CONTINUE.
  // Set the precision.  Now I assume the number type is double.
  const int oldPrecision = 
    out.precision(std::numeric_limits<double>::digits10);

  Base::put(out);
  out << _offsets;

  // Restore the old precision.
  out.precision(oldPrecision);
}


template<typename T>
inline
void
SparseArray<2,T>::
get(std::istream& in) {
  Base::get(in);
  in >> _offsets;
}

END_NAMESPACE_ADS
