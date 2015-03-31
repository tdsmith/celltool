// -*- C++ -*-

#if !defined(__geom_Simplex_ipp__)
#error This file is an implementation detail of the class Simplex.
#endif

BEGIN_NAMESPACE_GEOM


template<int N, typename V, typename T>
template<typename Comparable>
inline
bool
Simplex<N,V,T>::
hasVertex(const Comparable& v) const {
  for (ConstIterator i = begin(); i != end(); ++i) {
    if (v == *i) {
      return true;
    }
  }
  return false;
}


template<int N, typename V, typename T>
template<typename Comparable>
inline
bool
Simplex<N,V,T>::
hasVertex(const Comparable& v, int* i) const {
  for (*i = 0; *i != size(); ++*i) {
    if (v == _vertices[*i]) {
      return true;
    }
  }
  return false;
}


template<int N, typename V, typename T>
template<int M>
inline
bool
Simplex<N,V,T>::
hasFace(const Simplex<M,V,T>& f) const {
  // Loop over the vertices of the face.
  for (typename Simplex<M,V,T>::ConstIterator i = f.begin(); i != f.end(); 
       ++i) {
    // If the vertex of the face is not in the simplex.
    if (! hasVertex(*i)) {
      return false;
    }
  }
  // If we get here then all the vertices in the face are in the simplex.
  return true;
}


template<int N, typename V, typename T>
template<typename Comparable>
inline
int
Simplex<N,V,T>::
getVertexIndex(const Comparable& v) const {
  for (int i = 0; i != N+1; ++i) {
    if (v == _vertices[i]) {
      return i;
    }
  }
  assert(false);
  return -1;
}


template<int N, typename V, typename T>
inline
void
Simplex<N,V,T>::
getFace(const int n, Face* f) const {
#ifdef DEBUG_Simplex
  assert(0 <= n && n <= N);
#endif
  typename Face::Iterator iter = f->begin();
  for (int i = 0; i != N + 1; ++i) {
    if (i != n) {
      *iter++ = _vertices[i];
    }
  }
  if (n % 2 == 1) {
    f->negate();
  }
}


END_NAMESPACE_GEOM

// End of file.
