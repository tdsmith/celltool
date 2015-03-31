// -*- C++ -*-

#if !defined(__ads_SparseArray1_ipp__)
#error This file is an implementation detail of the class SparseArray.
#endif

BEGIN_NAMESPACE_ADS

//
// Constructors etc.
//

// Construct a 1-D array sparse array from the values and indices.
template<typename T>
template<typename IndexForwardIter, typename ValueForwardIter>
inline
SparseArray<1, T>::
SparseArray(IndexForwardIter indicesBeginning, IndexForwardIter indicesEnd,
	    ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd,
	    parameter_type nullValue) :
  Base(valuesBeginning, valuesEnd),
  _indices(indicesBeginning, indicesEnd),
  _null(nullValue){
  assert(_indices.size() == size());
}


// Rebuild a 1-D array sparse array from the values and indices.
template<typename T>
template<typename IndexForwardIter, typename ValueForwardIter>
inline
void
SparseArray<1, T>::
rebuild(IndexForwardIter indicesBeginning, IndexForwardIter indicesEnd,
	ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd,
	parameter_type nullValue) {
  Base::rebuild(valuesBeginning, valuesEnd);
  _indices.rebuild(indicesBeginning, indicesEnd);
  assert(_indices.size() == size());
  _null = nullValue;
}


// Construct a 1-D sparse array from a 1-D dense array of possibly 
// different value type.
template<typename T>
template<typename T2, bool A>
inline
SparseArray<1, T>::
SparseArray(const Array<1,T2,A>& array, parameter_type nullValue) :
  // Start with an empty sparse array.
  Base(),
  _indices(),
  _null(nullValue) {
  operator=(array);
}


  //! Assignment operator for dense arrays.
template<typename T>
template<typename T2, bool A>
inline
SparseArray<1, T>& 
SparseArray<1, T>::
operator=(const Array<1,T2,A>& array) {
  // The non-null indices and values.
  std::vector<int> indices;
  std::vector<value_type> values;

  // The array index range.
  const int iBegin = array.lbound(0);
  const int iEnd = array.ubound(0);

  // For each element in the dense array.
  for (int i = iBegin; i != iEnd; ++i) {
    // If the value is non-null.
    if (array(i) != _null) {
      // Record the index and value.
      indices.push_back(i);
      values.push_back(array(i));
    }
  }

  // Rebuild the sparse array.
  rebuild(indices.begin(), indices.end(), values.begin(), values.end());

  return *this;
}


template<typename T>
inline
bool
SparseArray<1, T>::
isNull(const int i) const {
  // Do a binary search to find the index.
  Array<1,int>::const_iterator j = 
    std::lower_bound(_indices.begin(), _indices.end(), i);
  // If the index does not exist.
  if (j == _indices.end() || *j != i) {
    // The element is null.
    return true;
  }
  // If the index does exist, the element is non-null.
  return false;
}


template<typename T>
inline
typename SparseArray<1, T>::parameter_type
SparseArray<1, T>::
operator()(const int i) const {
  // Do a binary search to find the index.
  Array<1,int>::const_iterator j = 
    std::lower_bound(_indices.begin(), _indices.end(), i);
  // If the index does not exist.
  if (j == _indices.end() || *j != i) {
    // Return the null value;
    return _null;
  }
  // If the index does exist, return the value.
  return operator[](int(j - _indices.begin()));
}


template<typename T>
template<typename T2, bool A>
inline
void
SparseArray<1, T>::
fill(ads::Array<1,T2,A>* array) const {
  // First set all the elements to the null value.
  *array = getNull();
  // Then fill in the non-null values.
  fillNonNull(array);
}


template<typename T>
template<typename T2, bool A>
inline
void
SparseArray<1, T>::
fillNonNull(ads::Array<1,T2,A>* array) const {
  // The range for the dense array.
  const int lb = array->lbound(0);
  const int ub = array->ubound(0);
  // Do a binary search to find the lower bound of the index range.
  // Index iterator.
  Array<1,int>::const_iterator ii = 
    std::lower_bound(_indices.begin(), _indices.end(), lb);
  // Initialize the value iterator.
  typename Array<1,value_type>::const_iterator vi = begin() + 
    (ii - _indices.begin());
  // Loop over the index range.
  for (; ii != _indices.end() && *ii < ub; ++ii, ++vi) {
    (*array)(*ii) = *vi;
  }
}


template<typename T>
inline
void
SparseArray<1, T>::
put(std::ostream& out) const {
  out << _null << '\n'
      << size() << '\n';
  _indices.write_elements_ascii(out);
  write_elements_ascii(out);
}


template<typename T>
inline
void
SparseArray<1, T>::
get(std::istream& in) {
  in >> _null;
  size_type s;
  in >> s;
  // Resize the indices array.
  _indices.resize(s);
  // Resize the values array.
  Base::rebuild(s);
  _indices.read_elements_ascii(in);
  read_elements_ascii(in);
}


//
// Free functions
//


// Compute the sum of the two arrays.
template<typename T>
inline
void
computeSum(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
	   SparseArray<1, T>* result) {
  computeBinaryOperation(x, y, result, std::plus<T>());
}


// Compute the difference of the two arrays.
template<typename T>
inline
void
computeDifference(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
		  SparseArray<1, T>* result) {
  computeBinaryOperation(x, y, result, std::minus<T>());
}


// Compute the product of the two arrays.
template<typename T>
inline
void
computeProduct(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
	       SparseArray<1, T>* result) {
  computeBinaryOperation(x, y, result, std::multiplies<T>());
}


// Use the binary function to compute the result.
template<typename T, typename BinaryFunction>
inline
void
computeBinaryOperation(const SparseArray<1, T>& x, const SparseArray<1, T>& y, 
		       SparseArray<1, T>* result, 
		       const BinaryFunction& function) {
  assert(x.getNull() == y.getNull());
  const T Null = x.getNull();

  std::vector<int> indices;
  std::vector<T> values;

  const int mEnd = x.size();
  const int nEnd = y.size();
  int i, j;
  T f;
  // Loop over the common index range.
  int m = 0, n = 0;
  while (m != mEnd && n != nEnd) {
    i = x.getIndices()[m];
    j = y.getIndices()[n];
    // If the first index is less.
    if (i < j) {
      f = function(x[m], Null);
      if (f != Null) {
	indices.push_back(i);
	values.push_back(f);
      }
      ++m;
    }
    // If the second index is less.
    else if (j < i) {
      f = function(Null, y[n]);
      if (f != Null) {
	indices.push_back(j);
	values.push_back(f);
      }
      ++n;
    }
    // If the indices are equal.
    else {
      f = function(x[m], y[n]);
      if (f != Null) {
	indices.push_back(i);
	values.push_back(f);
      }
      ++m;
      ++n;
    }
  }
  // Loop over the remaining indices of x.
  while (m != mEnd) {
    i = x.getIndices()[m];
    f = function(x[m], Null);
    if (f != Null) {
      indices.push_back(i);
      values.push_back(f);
    }
    ++m;
  }
  // Loop over the remaining indices of y.
  while (n != nEnd) {
    j = y.getIndices()[n];
    f = function(Null, y[n]);
    if (f != Null) {
      indices.push_back(j);
      values.push_back(f);
    }
    ++n;
  }

  // Build the result.
  result->rebuild(indices.begin(), indices.end(), 
		  values.begin(), values.end(), Null);
}





template<typename T>
inline
int
countNonNullElementsInUnion(const SparseArray<1, T>& a, 
			    const SparseArray<1, T>& b) {
  Array<1,int>::const_iterator i = a.getIndices().begin();
  const Array<1,int>::const_iterator i_end = a.getIndices().end();
  Array<1,int>::const_iterator j = b.getIndices().begin();
  const Array<1,int>::const_iterator j_end = b.getIndices().end();

  int count = 0;

  // Loop over the common index range.
  for (; i != i_end && j != j_end; ++count) {
    if (*i < *j) {
      ++i;
    }
    else if (*j < *i) {
      ++j;
    }
    else { // *i == *j
      ++i;
      ++j;
    }
  }

  // Count any remaining elements from a and b.  Only one of these terms
  // can be non-zero.
  count += int(i_end - i) + int(j_end - j);

  return count;
}


//---------------------------------------------------------------------------
// Operations with arrays and sparse arrays.
//---------------------------------------------------------------------------

// += on the non-null elements.
template<typename T1, bool A, typename T2>
inline
Array<1,T1,A>& 
operator+=(Array<1,T1,A>& x, const SparseArray<1,T2> & y) {
  typename SparseArray<1,T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1,T2>::const_iterator v = y.begin();
  const typename SparseArray<1,T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    x[*i] += *v;
  }
  return x;
}

// -= on the non-null elements.
template<typename T1, bool A, typename T2>
inline
Array<1,T1,A>& 
operator-=(Array<1,T1,A>& x, const SparseArray<1,T2> & y) {
  typename SparseArray<1,T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1,T2>::const_iterator v = y.begin();
  const typename SparseArray<1,T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    x[*i] -= *v;
  }
  return x;
}

// *= on the non-null elements.
template<typename T1, bool A, typename T2>
inline
Array<1,T1,A>& 
operator*=(Array<1,T1,A>& x, const SparseArray<1,T2> & y) {
  typename SparseArray<1,T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1,T2>::const_iterator v = y.begin();
  const typename SparseArray<1,T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    x[*i] *= *v;
  }
  return x;
}

// /= on the non-null elements.
template<typename T1, bool A, typename T2>
inline
Array<1,T1,A>& 
operator/=(Array<1,T1,A>& x, const SparseArray<1,T2> & y) {
  typename SparseArray<1,T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1,T2>::const_iterator v = y.begin();
  const typename SparseArray<1,T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
#ifdef DEBUG_SparseArray
    assert(*v != 0);
#endif
    x[*i] /= *v;
  }
  return x;
}

// %= on the non-null elements.
template<typename T1, bool A, typename T2>
inline
Array<1,T1,A>& 
operator%=(Array<1,T1,A>& x, const SparseArray<1,T2> & y) {
  typename SparseArray<1,T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1,T2>::const_iterator v = y.begin();
  const typename SparseArray<1,T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
#ifdef DEBUG_SparseArray
    assert(*v != 0);
#endif
    x[*i] %= *v;
  }
  return x;
}

// Perform x += a * y on the non-null elements.
template<typename T1, bool A, typename T2, typename T3>
inline
void
scaleAdd(Array<1,T1,A>* x, const T2 a, const SparseArray<1,T3> & y) {
  typename SparseArray<1,T3>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1,T3>::const_iterator v = y.begin();
  const typename SparseArray<1,T3>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    (*x)[*i] += a * *v;
  }
}



//---------------------------------------------------------------------------
// Operations with FixedArray's and sparse arrays.
//---------------------------------------------------------------------------

// += on the non-null elements.
template<int _N, typename _T1, typename _T2>
inline
FixedArray<_N, _T1>& 
operator+=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y) {
  typename SparseArray<1, _T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1, _T2>::const_iterator v = y.begin();
  const typename SparseArray<1, _T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    x[*i] += *v;
  }
  return x;
}

// -= on the non-null elements.
template<int _N, typename _T1, typename _T2>
inline
FixedArray<_N, _T1>& 
operator-=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y) {
  typename SparseArray<1, _T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1, _T2>::const_iterator v = y.begin();
  const typename SparseArray<1, _T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    x[*i] -= *v;
  }
  return x;
}

// *= on the non-null elements.
template<int _N, typename _T1, typename _T2>
inline
FixedArray<_N, _T1>& 
operator*=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y) {
  typename SparseArray<1, _T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1, _T2>::const_iterator v = y.begin();
  const typename SparseArray<1, _T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    x[*i] *= *v;
  }
  return x;
}

// /= on the non-null elements.
template<int _N, typename _T1, typename _T2>
inline
FixedArray<_N, _T1>& 
operator/=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y) {
  typename SparseArray<1, _T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1, _T2>::const_iterator v = y.begin();
  const typename SparseArray<1, _T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
#ifdef DEBUG_SparseArray
    assert(*v != 0);
#endif
    x[*i] /= *v;
  }
  return x;
}

// %= on the non-null elements.
template<int _N, typename _T1, typename _T2>
inline
FixedArray<_N, _T1>& 
operator%=(FixedArray<_N, _T1>& x, const SparseArray<1, _T2>& y) {
  typename SparseArray<1, _T2>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1, _T2>::const_iterator v = y.begin();
  const typename SparseArray<1, _T2>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
#ifdef DEBUG_SparseArray
    assert(*v != 0);
#endif
    x[*i] %= *v;
  }
  return x;
}

// Perform x += a * y on the non-null elements.
template<int _N, typename _T1, typename _T2, typename _T3>
inline
void
scaleAdd(FixedArray<_N, _T1>* x, const _T2 a, const SparseArray<1, _T3>& y) {
  typename SparseArray<1, _T3>::IndexConstIterator i = y.getIndicesBeginning();
  typename SparseArray<1, _T3>::const_iterator v = y.begin();
  const typename SparseArray<1, _T3>::const_iterator vEnd = y.end();
  for (; v != vEnd; ++i, ++v) {
    (*x)[*i] += a * *v;
  }
}

END_NAMESPACE_ADS
