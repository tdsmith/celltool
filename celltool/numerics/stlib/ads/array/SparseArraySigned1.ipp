// -*- C++ -*-

#if !defined(__ads_SparseArraySigned1_ipp__)
#error This file is an implementation detail of the class SparseArraySigned.
#endif

BEGIN_NAMESPACE_ADS

template<typename T>
template<typename IndexForwardIter, typename ValueForwardIter>
inline
SparseArraySigned<1,T>::
SparseArraySigned(IndexForwardIter indicesBeginning, 
		  IndexForwardIter indicesEnd,
		  ValueForwardIter valuesBeginning, ValueForwardIter valuesEnd,
		  parameter_type nullValue) :
  Base(indicesBeginning, indicesEnd, valuesBeginning, valuesEnd, nullValue),
  _sign(indicesBeginning == indicesEnd ? 1 : 0)
{}


template<typename T>
inline
typename SparseArraySigned<1,T>::value_type
SparseArraySigned<1,T>::
operator()(const int i) const {
  // If there are no non-null elements.
  if (_sign != 0) {
#ifdef DEBUG_ads_SparseArraySigned
    assert(empty());
#endif
    return _sign * _null;;
  }

#ifdef DEBUG_ads_SparseArraySigned
  assert(! empty());
#endif

  // Do a binary search to find the index.
  Array<1,int>::const_iterator iter = 
    std::lower_bound(_indices.begin(), _indices.end(), i);

  // If there are no non-null elements with index <= i.
  if (iter == _indices.end()) {
    // Check the last non-null element value.
    if (*(end() - 1) > 0) {
      return _null; 
    }
    else {
      return - _null;
    }
  }

  parameter_type val = operator[](int(iter - _indices.begin()));
  // If the i_th element is null.
  if (i != *iter) {
    return (val > 0 ? _null : - _null);
  }
  
  return val;
}


template<typename T>
template<typename T2, bool A>
inline
void
SparseArraySigned<1,T>::
fill(ads::Array<1,T2,A>* array) const {
  // If there are no non-null elements.
  if (_sign != 0) {
    assert(empty());
    *array = _sign * _null;
    return;
  }

  assert(! empty());

  // The range for the dense array.
  const int lb = array->lbound(0);
  const int ub = array->ubound(0);
  // Do a binary search to find the lower bound of the index range.
  // Index iterator.
  Array<1,int>::const_iterator ii = 
    std::lower_bound(_indices.begin(), _indices.end(), lb);

  // If there are no non-null elements with index <= lb.
  if (ii == _indices.end()) {
    // Check the last non-null element value.
    *array = (*(_values.end() - 1) > 0 ? _null : - _null);
    return;
  }

  // Initialize the value iterator.
  typename Array<1,value_type>::const_iterator vi = _values.begin() + 
    (ii - _indices.begin());
  int sign = (*vi > 0 ? 1 : -1);
  // Loop over the dense array.
  for (int n = lb; n != ub; ++n) {
    if (n < *ii) {
      (*array)(n) = sign * _null;
    }
    else {
      (*array)(n) = *vi;
      sign = (*vi > 0 ? 1 : -1);
      ++ii;
      ++vi;
    }
  }
}


template<typename T>
inline
void
SparseArraySigned<1,T>::
put(std::ostream& out) const {
  out << _sign << '\n';
  Base::put(out);
}


template<typename T>
inline
void
SparseArraySigned<1,T>::
get(std::istream& in) {
  in >> _sign;
  Base::get(in);
}


//
// Free functions
//


template<typename T>
inline
void
merge(const SparseArraySigned<1,T>& a, const SparseArraySigned<1,T>& b, 
      SparseArraySigned<1,T>* c) {
  typedef typename Array<1,int>::const_iterator index_const_iterator;
  typedef typename Array<1,int>::iterator index_iterator;
  typedef typename Array<1,T>::const_iterator value_const_iterator;
  typedef typename Array<1,T>::iterator value_iterator;

  assert(a.getNull() == b.getNull());
  c->_null = a.getNull();

  const int size = countNonNullElementsInUnion(a, b);
  c->rebuild(size);

  if (size == 0) {
    assert(a._sign == b._sign);
    c->_sign = a._sign;
    return;
  }
  else {
    c->_sign = 0;
  }

  // Index iterators.
  index_const_iterator i = a.getIndices().begin();
  const index_const_iterator i_end = a.getIndices().end();
  index_const_iterator j = b.getIndices().begin();
  const index_const_iterator j_end = b.getIndices().end();
  index_iterator k = c->_indices.begin();

  // Value iterators.
  value_const_iterator x = a.begin();
  value_const_iterator y = b.begin();
  value_iterator z = c->begin();

  // Loop over the common index range.
  while (i != i_end && j != j_end) {
    if (*i < *j) {
      *k = *i;
      ++i;
      *z = *x;
      ++x;
    }
    else if (*j < *i) {
      *k = *j;
      ++j;
      *z = *y;
      ++y;
    }
    else { // *i == *j
      *k = *i;
      ++i;
      ++j;
      if (std::abs(*x) < std::abs(*y)) {
	*z = *x;
      }
      else {
	*z = *y;
      }
      ++x;
      ++y;
    }
    ++k;
    ++z;
  }

  // Get any remaining elements of a.
  while (i != i_end) {
    *k = *i;
    ++i;
    ++k;
    *z = *x;
    ++x;
    ++z;
  }

  // Get any remaining elements of b.
  while (j != j_end) {
    *k = *j;
    ++j;
    ++k;
    *z = *y;
    ++y;
    ++z;
  }

  assert(k == c->_indices.end());
  assert(z == c->end());
}




// Remove the unecessary elements.
/*
 Remove positive elements that do not have an adjacent non-positive neighbor
 and vice-versa.
*/
template<typename T>
void
removeUnecessaryElements(SparseArraySigned<1,T>* a) {
  typedef typename Array<1,int>::const_iterator index_const_iterator;
  typedef typename Array<1,int>::iterator index_iterator;
  typedef typename Array<1,T>::const_iterator value_const_iterator;
  typedef typename Array<1,T>::iterator value_iterator;

  const value_const_iterator x_begin = a->begin();
  const value_const_iterator x_end = a->end();

  // Count the necessary elements.
  int size = 0;
  for (value_const_iterator x = x_begin; x != x_end; ++x) {
    if (x != x_begin) {
      if (*x > 0 && *(x-1) <= 0 || *x <= 0 && *(x-1) > 0) {
	++size;
	continue;
      }
    }
    if (x + 1 != x_end) {
      if (*x > 0 && *(x+1) <= 0 || *x <= 0 && *(x+1) > 0) {
	++size;
      }
    }
  }

  // If all of the elements are necessary, there is no need to alter the array.
  if (size == a->size()) {
    return;
  }

  // If the array will be resized to zero.
  if (size == 0) {
    // Each element must have the same sign.  Use the first element to 
    // determine this sign.
    if ((*a)[0] > 0) {
      a->_sign = 1;
    }
    else {
      a->_sign = -1;
    }
  }

  // Allocate memory for the new values and indices.
  ads::Array<1,T> values(size);
  ads::Array<1,int> indices(size);

  // Compute the new values and indices.
  value_iterator y = values.begin();
  index_iterator j = indices.begin();
  index_const_iterator i = a->getIndices().begin();
  for (value_const_iterator x = x_begin; x != x_end; ++i, ++x) {
    if (x != x_begin) {
      if (*x > 0 && *(x-1) <= 0 || *x <= 0 && *(x-1) > 0) {
	*y = *x;
	++y;
	*j = *i;
	++j;
	continue;
      }
    }
    if (x + 1 != x_end) {
      if (*x > 0 && *(x+1) <= 0 || *x <= 0 && *(x+1) > 0) {
	*y = *x;
	++y;
	*j = *i;
	++j;
      }
    }
  }

  // Rebuild the array.
  a->rebuild(indices.begin(), indices.end(), values.begin(), values.end());
}




template<typename T>
inline
int
countElementsInUnion(const SparseArraySigned<1,T>& a, 
			const SparseArraySigned<1,T>& b) {
  typedef typename Array<1,int>::const_iterator index_const_iterator;
  typedef typename Array<1,T>::const_iterator value_const_iterator;

  // Index iterators.
  index_const_iterator i = a.getIndices().begin();
  const index_const_iterator i_end = a.getIndices().end();
  index_const_iterator j = b.getIndices().begin();
  const index_const_iterator j_end = b.getIndices().end();

  // Value iterators.
  value_const_iterator x = a.begin();
  value_const_iterator y = b.begin();

  int size = 0;

  // Loop over the common index range.
  while (i != i_end && j != j_end) {
    // If the a array has the next element.
    if (*i < *j) {
      // If the b array does not have a value of -infinity at this point, 
      // the element will be in the union.
      if (*y > 0) {
	++size;
      }
      ++i;
      ++x;
    }
    // If the b array has the next element.
    else if (*j < *i) {
      // If the a array does not have a value of -infinity at this point, 
      // the element will be in the union.
      if (*x > 0) {
	++size;
      }
      ++j;
      ++y;
    }
    // If the a and b arrays both have the next element.
    else { // *i == *j
      // The element will be in the union.
      ++size;
      ++i;
      ++j;
      ++x;
      ++y;
    }
  }

  // The sign of the values for a and b.
  int p, q;
  // Initialize the signs.
  if (a.empty()) {
    p = a.getSign();
  }
  else {
    p = (*(a.end()-1) > 0 ? 1 : -1);
  }
  if (b.empty()) {
    q = b.getSign();
  }
  else {
    q = (*(b.end()-1) > 0 ? 1 : -1);
  }

  // Get any remaining elements of a.
  if (q > 0) {
    size += int(i_end - i);
  }

  // Get any remaining elements of b.
  if (p > 0) {
    size += int(j_end - j);
  }

  return size;
}




/*
Below are a few cases.  I only indicate the sign of the values.

Disjoint:

+--+
    +--+
+--++--+


Touching:

+--+
   +--+
+--+--+

+--+
  +--+
+----+


Intersecting:

+--+
 +--+
+---+

Subset:

+----+
 +--+
+----+

+-
   +-+
+-
*/
template<typename T>
inline
void
computeUnion(const SparseArraySigned<1,T>& a, 
	     const SparseArraySigned<1,T>& b, 
	     SparseArraySigned<1,T>* c) {
  typedef typename Array<1,int>::const_iterator index_const_iterator;
  typedef typename Array<1,int>::iterator index_iterator;
  typedef typename Array<1,T>::const_iterator value_const_iterator;
  typedef typename Array<1,T>::iterator value_iterator;

  assert(a.getNull() == b.getNull());
  c->_null = a.getNull();

  const int size = countElementsInUnion(a, b);
  c->rebuild(size);

  if (size == 0) {
    if (a.getSign() == -1 || b.getSign() == -1) {
      c->setSign(-1);
    }
    else {
      c->setSign(1);
    }
    return;
  }
  else {
    c->setSign(0);
  }

  // Index iterators.
  index_const_iterator i = a.getIndices().begin();
  const index_const_iterator i_end = a.getIndices().end();
  index_const_iterator j = b.getIndices().begin();
  const index_const_iterator j_end = b.getIndices().end();
  index_iterator k = c->_indices.begin();

  // Value iterators.
  value_const_iterator x = a.begin();
  value_const_iterator y = b.begin();
  value_iterator z = c->begin();

  // Loop over the common index range.
  while (i != i_end && j != j_end) {
    // If the a array has the next element.
    if (*i < *j) {
      // If the b array does not have a value of -infinity at this point, 
      // add the element to c.
      if (*y > 0) {
	*k = *i;
	++k;
	*z = *x;
	++z;
      }
      // If the b array has a value of -infinity at this point, we skip the 
      // element.  (In either case, we advance the iterators.)
      ++i;
      ++x;
    }
    // If the b array has the next element.
    else if (*j < *i) {
      // If the a array does not have a value of -infinity at this point, 
      // add the element to c.
      if (*x > 0) {
	*k = *j;
	++k;
	*z = *y;
	++z;
      }
      // If the a array has a value of -infinity at this point, we skip the 
      // element.  (In either case, we advance the iterators.)
      ++j;
      ++y;
    }
    // If the a and b arrays both have the next element.
    else { // *i == *j
      *k = *i;
      ++i;
      ++j;
      ++k;
      if (*x <= 0 && *y <= 0) {
	*z = std::max(*x, *y);
      }
      else {
	*z = std::min(*x, *y);
      }
      ++x;
      ++y;
      ++z;
    }
  }

  // The sign of the values for a and b.
  int p, q;
  // Initialize the signs.
  if (a.empty()) {
    p = a.getSign();
  }
  else {
    p = (*(a.end()-1) > 0 ? 1 : -1);
  }
  if (b.empty()) {
    q = b.getSign();
  }
  else {
    q = (*(b.end()-1) > 0 ? 1 : -1);
  }

  // Get any remaining elements of a.
  if (q > 0) {
    while (i != i_end) {
      *k = *i;
      ++i;
      ++k;
      *z = *x;
      ++x;
      ++z;
    }
  }

  // Get any remaining elements of b.
  if (p > 0) {
    while (j != j_end) {
      *k = *j;
      ++j;
      ++k;
      *z = *y;
      ++y;
      ++z;
    }
  }

  assert(k == c->_indices.end());
  assert(z == c->end());
  
  removeUnecessaryElements(c);
}




template<typename T>
inline
int
countElementsInIntersection(const SparseArraySigned<1,T>& a, 
			    const SparseArraySigned<1,T>& b) {
  typedef typename Array<1,int>::const_iterator index_const_iterator;
  typedef typename Array<1,T>::const_iterator value_const_iterator;

  // Index iterators.
  index_const_iterator i = a.getIndices().begin();
  const index_const_iterator i_end = a.getIndices().end();
  index_const_iterator j = b.getIndices().begin();
  const index_const_iterator j_end = b.getIndices().end();

  // Value iterators.
  value_const_iterator x = a.begin();
  value_const_iterator y = b.begin();

  int size = 0;

  // Loop over the common index range.
  while (i != i_end && j != j_end) {
    // If the a array has the next element.
    if (*i < *j) {
      // If the b array does not have a value of -infinity at this point, 
      // the element will be in the union.
      if (*y <= 0) {
	++size;
      }
      ++i;
      ++x;
    }
    // If the b array has the next element.
    else if (*j < *i) {
      // If the a array does not have a value of -infinity at this point, 
      // the element will be in the union.
      if (*x <= 0) {
	++size;
      }
      ++j;
      ++y;
    }
    // If the a and b arrays both have the next element.
    else { // *i == *j
      // The element will be in the union.
      ++size;
      ++i;
      ++j;
      ++x;
      ++y;
    }
  }

  // The sign of the values for a and b.
  int p, q;
  // Initialize the signs.
  if (a.empty()) {
    p = a.getSign();
  }
  else {
    p = (*(a.end()-1) > 0 ? 1 : -1);
  }
  if (b.empty()) {
    q = b.getSign();
  }
  else {
    q = (*(b.end()-1) > 0 ? 1 : -1);
  }

  // Get any remaining elements of a.
  if (q < 0) {
    size += int(i_end - i);
  }

  // Get any remaining elements of b.
  if (p < 0) {
    size += int(j_end - j);
  }

  return size;
}





/*
Below are a few cases.  I only indicate the sign of the values.

Disjoint:

+--+
    +--+
+

Touching:

+--+
   +--+
+

+--+
  +--+
+

Intersecting:

+--+
 +--+
 +-+

Subset:

+----+
 +--+
 +--+

+-
   +-+
   +-+
*/
template<typename T>
inline
void
computeIntersection(const SparseArraySigned<1,T>& a, 
		    const SparseArraySigned<1,T>& b, 
		    SparseArraySigned<1,T>* c) {
  typedef typename Array<1,int>::const_iterator index_const_iterator;
  typedef typename Array<1,int>::iterator index_iterator;
  typedef typename Array<1,T>::const_iterator value_const_iterator;
  typedef typename Array<1,T>::iterator value_iterator;

  assert(a.getNull() == b.getNull());
  c->_null = a.getNull();

  const int size = countElementsInIntersection(a, b);
  c->rebuild(size);

  if (size == 0) {
    // If all of the elements from both arrays are inside.
    if (a.getSign() == -1 && b.getSign() == -1) {
      // All of the elements are inside.
      c->set_sign(-1);
    }
    else {
      // No elements are inside.
      c->set_sign(1);
    }
    return;
  }
  else {
    c->set_sign(0);
  }

  // Index iterators.
  index_const_iterator i = a.getIndices().begin();
  const index_const_iterator i_end = a.getIndices().end();
  index_const_iterator j = b.getIndices().begin();
  const index_const_iterator j_end = b.getIndices().end();
  index_iterator k = c->_indices.begin();

  // Value iterators.
  value_const_iterator x = a.begin();
  value_const_iterator y = b.begin();
  value_iterator z = c->begin();

  // Loop over the common index range.
  while (i != i_end && j != j_end) {
    // If the a array has the next element.
    if (*i < *j) {
      // If the b array does not have a value of infinity at this point, 
      // add the element to c.
      if (*y <= 0) {
	*k = *i;
	++k;
	*z = *x;
	++z;
      }
      // If the b array has a value of infinity at this point, we skip the 
      // element.  (In either case, we advance the iterators.)
      ++i;
      ++x;
    }
    // If the b array has the next element.
    else if (*j < *i) {
      // If the a array does not have a value of infinity at this point, 
      // add the element to c.
      if (*x <= 0) {
	*k = *j;
	++k;
	*z = *y;
	++z;
      }
      // If the a array has a value of infinity at this point, we skip the 
      // element.  (In either case, we advance the iterators.)
      ++j;
      ++y;
    }
    // If the a and b arrays both have the next element.
    else { // *i == *j
      *k = *i;
      ++i;
      ++j;
      ++k;
      *z = std::max(*x, *y);
      ++x;
      ++y;
      ++z;
    }
  }

  // The sign of the values for a and b.
  int p, q;
  // Initialize the signs.
  if (a.empty()) {
    p = a.getSign();
  }
  else {
    p = (*(a.end()-1) > 0 ? 1 : -1);
  }
  if (b.empty()) {
    q = b.getSign();
  }
  else {
    q = (*(b.end()-1) > 0 ? 1 : -1);
  }

  // Get any remaining elements of a.
  if (q < 0) {
    while (i != i_end) {
      *k = *i;
      ++i;
      ++k;
      *z = *x;
      ++x;
      ++z;
    }
  }

  // Get any remaining elements of b.
  if (p < 0) {
    while (j != j_end) {
      *k = *j;
      ++j;
      ++k;
      *z = *y;
      ++y;
      ++z;
    }
  }

  assert(k == c->_indices.end());
  assert(z == c->end());
  
  removeUnecessaryElements(c);
}

END_NAMESPACE_ADS
