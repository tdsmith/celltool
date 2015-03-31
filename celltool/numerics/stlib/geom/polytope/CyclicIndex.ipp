// -*- C++ -*-

#if !defined(__geom_CyclicIndex_ipp__)
#error This file is an implementation detail of the class CyclicIndex.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors and destructor.
//

inline
CyclicIndex& 
CyclicIndex::operator=(const CyclicIndex& other) {
  // Avoid assignment to self
  if (&other != this){
    _index = other._index;
    _n = other._n;
  }
  // Return *this so assignments can chain
  return * this;
}

// 
// Manipulators
//

inline
void 
CyclicIndex::set(int i) {
  if (i < 0) {
    i += _n * (- i / _n + 1);
  }
  _index = i % _n;
}

//
// Increment and decrement operators
//

inline
CyclicIndex&
operator++(CyclicIndex& ci) {
  ci._index = (ci._index + 1) % ci._n;
  return ci;
}

inline
CyclicIndex&
operator--(CyclicIndex& ci) {
  ci._index = (ci._index + ci._n - 1) % ci._n;
  return ci;
}

END_NAMESPACE_GEOM

// End of file.
