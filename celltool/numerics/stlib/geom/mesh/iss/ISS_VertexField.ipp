// -*- C++ -*-

#if !defined(__geom_ISS_VertexField_ipp__)
#error This file is an implementation detail of the class ISS_VertexField.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical Member Functions
//

template <class ISS,typename F>
inline
typename ISS_VertexField<ISS,F>::FieldParameterType
ISS_VertexField<ISS,F>::
interpolate(const int n, const Vertex& x) const {
  int i;
  for (int m = 0; m != ISS::M + 1; ++m) {
    i = _iss.getIndexedSimplices()[n][m];
    _pos[m] = _iss.getVertices()[i];
    _val[m] = _fields[i];
  }
  return numerical::linear_interpolation(_pos, _val, x);
}

END_NAMESPACE_GEOM

// End of file.
