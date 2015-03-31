// -*- C++ -*-

#if !defined(__StaticArrayOfArrays_ipp__)
#error This file is an implementation detail of the class StaticArrayOfArrays.
#endif

BEGIN_NAMESPACE_ADS

template<typename T>
template<typename IntForwardIter, typename ValueForwardIter>
inline
StaticArrayOfArrays<T>::
StaticArrayOfArrays(IntForwardIter sizesBeginning, IntForwardIter sizesEnd,
		    ValueForwardIter valuesBeginning, 
		    ValueForwardIter valuesEnd) :
  Base(valuesBeginning, valuesEnd),
  _pointers(std::distance(sizesBeginning, sizesEnd) + 1) {
  _pointers[0] = begin();
  for (int n = 0; n != getNumberOfArrays(); ++n, ++sizesBeginning) {
    _pointers[n+1] = _pointers[n] + *sizesBeginning;
  }
  assert(_pointers[_pointers.size() - 1] == end());
}


template<typename T>
template<typename IntForwardIter>
inline
void
StaticArrayOfArrays<T>::
rebuild(const size_type numberOfElements, 
	IntForwardIter sizesBeginning, IntForwardIter sizesEnd) {
  Base::resize(numberOfElements);
  _pointers.resize(std::distance(sizesBeginning, sizesEnd) + 1);
  _pointers[0] = begin();
  for (int n = 0; n != getNumberOfArrays(); ++n, ++sizesBeginning) {
    _pointers[n+1] = _pointers[n] + *sizesBeginning;
  }
  assert(_pointers[_pointers.size() - 1] == end());
}


template<typename T>
inline
void
StaticArrayOfArrays<T>::
put(std::ostream& out) const {
  const_iterator i, iEnd;
  out << getNumberOfArrays() << " " << size() << '\n';
  for (int n = 0; n != getNumberOfArrays(); ++n) {
    out << size(n) << '\n';
    iEnd = end(n);
    for (i = begin(n); i != iEnd; ++i) {
      out << *i << " ";
    }
    out << '\n';
  }
}

template<typename T>
inline
void
StaticArrayOfArrays<T>::
get(std::istream& in) {
  int numberOfArrays, numberOfElements, sz;
  in >> numberOfArrays >> numberOfElements;
  Base::resize(numberOfElements);
  _pointers.resize(numberOfArrays + 1);

  _pointers[0] = begin();
  for (int n = 0; n != numberOfArrays; ++n) {
    in >> sz;
    _pointers[n + 1] = _pointers[n] + sz;
    for (int m = 0; m != sz; ++m) {
      in >> operator()(n, m);
    }
  }
}

END_NAMESPACE_ADS
