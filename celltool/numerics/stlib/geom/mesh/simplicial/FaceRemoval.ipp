// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_FaceRemoval_ipp__)
#error This file is an implementation detail of the class FaceRemoval.
#endif

BEGIN_NAMESPACE_GEOM

//
// Private member functions.
//


// Return the worse quality of the 2 tetrahedra: 
// _face[0], _face[1], _face[2], _target
// and
// _source, _face[0], _face[1], _face[2]
template <class _QualityMetric, class _Point, typename _Number>
inline
typename FaceRemoval<_QualityMetric,_Point,_Number>::Number
FaceRemoval<_QualityMetric,_Point,_Number>::
computeQuality2() const {
  Number q1, q2;
  {
    // Make a simplex from the first tetrahedra.
    Simplex tet(_face[0], _face[1], _face[2], _target);
    // Calculate the Jacobian.
    _qualityFunction.setFunction(tet);
    // Calculate the mean ratio quality function.
    q1 = 1.0 / _qualityFunction();
  }
  {
    // Make a simplex from the second tetrahedra.
    Simplex tet(_source, _face[0], _face[1], _face[2]);
    // Calculate the Jacobian.
    _qualityFunction.setFunction(tet);
    // Calculate the mean ratio quality function.
    q2 = 1.0 / _qualityFunction();
  }
  // REMOVE
  //std::cout << "quality2(): " << q1 << " " << q2 << '\n';
  return std::min(q1, q2);
}


// Return the worst quality of the 3 tetrahedra: 
// _source, _target, _face[0], _face[1]
// _source, _target, _face[1], _face[2]
// _source, _target, _face[2], _face[0]
template <class _QualityMetric, class _Point, typename _Number>
inline
typename FaceRemoval<_QualityMetric,_Point,_Number>::Number
FaceRemoval<_QualityMetric,_Point,_Number>::
computeQuality3() const {
  Number q1, q2, q3;
  {
    // Make a simplex from the first tetrahedra.
    Simplex tet(_source, _target, _face[0], _face[1]);
    // Calculate the Jacobian.
    _qualityFunction.setFunction(tet);
    // Calculate the mean ratio quality function.
    q1 = 1.0 / _qualityFunction();
  }
  {
    // Make a simplex from the second tetrahedra.
    Simplex tet(_source, _target, _face[1], _face[2]);
    // Calculate the Jacobian.
    _qualityFunction.setFunction(tet);
    // Calculate the mean ratio quality function.
    q2 = 1.0 / _qualityFunction();
  }
  {
    // Make a simplex from the third tetrahedra.
    Simplex tet(_source, _target, _face[2], _face[0]);
    // Calculate the Jacobian.
    _qualityFunction.setFunction(tet);
    // Calculate the mean ratio quality function.
    q3 = 1.0 / _qualityFunction();
  }
  // REMOVE
  //std::cout << "quality3(): " << q1 << " " << q2 << " " << q3 << '\n';
  return std::min(q1, std::min(q2, q3));
}


END_NAMESPACE_GEOM

// End of file.
