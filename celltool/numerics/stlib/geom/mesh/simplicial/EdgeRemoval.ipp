// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_EdgeRemoval_ipp__)
#error This file is an implementation detail of the class EdgeRemoval.
#endif

BEGIN_NAMESPACE_GEOM

//
// Tetrahedralization.
//

template<class _QualityMetric, class _Point, typename _Number>
inline
bool
EdgeRemoval<_QualityMetric,_Point,_Number>::
solve() {
  // Calculate the best quality if we remove the edge.
  fillTables();
  // If the new quality is no better than the old quality with the edge.
  // CONTINUE
  if (_quality(0, int(_ring.size()) - 1) <= computeQualityWithEdge() *
       (1.0 + std::sqrt(std::numeric_limits<Number>::epsilon()))) {
    return false;
  }
  
  // REMOVE
  /*
  std::cout << _quality(0, _ring.size() - 1) << " >= " 
	    << computeQualityWithEdge() 
	    << "   " << _ring.size() << " -> " << 2 * (_ring.size() - 2)
	    << '\n';
  */

  // Otherwise, removing the edge improves the quality.
  // Build the triangulation of the ring.
  buildTriangles();
  return true;
}


//
// Private member functions.
//


template<class _QualityMetric, class _Point, typename _Number>
inline
void
EdgeRemoval<_QualityMetric,_Point,_Number>::
fillTables() {
  const int N = int(_ring.size());
  Number q;
  for (int i = N - 3; i >= 0; --i) {
    for (int j = i + 2; j != N; ++j) {
      for (int k = i + 1; k <= j - 1; ++k) {
	q = computeQuality(i, k, j);
	if (k < j - 1) {
	  q = std::min(q, _quality(k, j));
	}
	if (k > i + 1) {
	  q = std::min(q, _quality(i, k));
	}
	if (k == i + 1 || q > _quality(i, j)) {
	  _quality(i, j) = q;
	  _index(i, j) = k;
	}
      }
    }
  }
}


template<class _QualityMetric, class _Point, typename _Number>
inline
void
EdgeRemoval<_QualityMetric,_Point,_Number>::
buildTriangles() {
  _triangles.clear();
  buildTrianglesRecurse(0, int(_ring.size()) - 1);
#ifdef DEBUG_EdgeRemoval
  assert(_triangles.size() == _ring.size() - 2);
#endif
}


template<class _QualityMetric, class _Point, typename _Number>
inline
void
EdgeRemoval<_QualityMetric,_Point,_Number>::
buildTrianglesRecurse(const int i, const int j) {
#ifdef DEBUG_EdgeRemoval
  assert(0 <= i && i < j && j < int(_ring.size()));
#endif

  if (i + 1 < j) {
    // Get the third index of the triangle.
    const int k = _index(i, j);
#ifdef DEBUG_EdgeRemoval
    assert(i < k && k < j);
#endif
    // Add the triangle.
    _triangles.push_back(ads::FixedArray<3,int>(i, k, j));
    // Build the rest of the triangles.
    buildTrianglesRecurse(i, k);
    buildTrianglesRecurse(k, j);
  }
}


// Return the worse quality of the tetrahedra: 
// _ring[i], _ring[k], _ring[j], _target
// and
// _source, _ring[i], _ring[k], _ring[j]
template<class _QualityMetric, class _Point, typename _Number>
inline
typename EdgeRemoval<_QualityMetric,_Point,_Number>::Number
EdgeRemoval<_QualityMetric,_Point,_Number>::
computeQuality(const int i, const int k, const int j) const {
#ifdef DEBUG_EdgeRemoval
  assert(0 <= i && i <=  int(_ring.size()) - 3);
  assert(j >= i + 2 && j < int(_ring.size()));
  assert(k >= i + 1 && k <= j - 1);
#endif
  static Simplex tet;

  // Make a simplex from the first tetrahedra.
  tet.build(_ring[i], _ring[k], _ring[j], _target);
  // Calculate the Jacobian.
  _qualityFunction.setFunction(tet);
  // Calculate the mean ratio quality function.
  const Number q1 = 1.0 / _qualityFunction();

  // Make a simplex from the first tetrahedra.
  tet.build(_source, _ring[i], _ring[k], _ring[j]);
  // Calculate the Jacobian.
  _qualityFunction.setFunction(tet);
  // Calculate the mean ratio quality function.
  const Number q2 = 1.0 / _qualityFunction();

  return std::min(q1, q2);
}


// Return the quality of the complex with the center edge.
// The quality of the complex is the quality of the worst tetrahedron.
template<class _QualityMetric, class _Point, typename _Number>
inline
typename EdgeRemoval<_QualityMetric,_Point,_Number>::Number
EdgeRemoval<_QualityMetric,_Point,_Number>::
computeQualityWithEdge() const {
#ifdef DEBUG_EdgeRemoval
  assert(_ring.size() >= 3);
#endif

  Number qualityNew;
  Number q = computeQuality(0);
  const int end = int(_ring.size()) - 1;
  for (int i = 1; i != end; ++i) {
    qualityNew = computeQuality(i);
    if (qualityNew < q) {
      q = qualityNew;
    }
  }
  return q;
}


// Return the quality of the tetrahedra: 
// _source, _target, _ring[i], _ring[i+1]
template<class _QualityMetric, class _Point, typename _Number>
inline
typename EdgeRemoval<_QualityMetric,_Point,_Number>::Number
EdgeRemoval<_QualityMetric,_Point,_Number>::
computeQuality(const int i) const {
#ifdef DEBUG_EdgeRemoval
  assert(0 <= i && i <  int(_ring.size()) - 1);
#endif

  static Simplex tet;

  // Make the simplex.
  tet.build(_source, _target, _ring[i], _ring[i+1]);
  // Calculate the Jacobian.
  _qualityFunction.setFunction(tet);
  // REMOVE
  /*
  Number q = 1.0 / _qualityFunction();
  if (q < 0.1) {
    std::cout << "Low quality: " << q << '\n';
  }
  */
  // Return the quality.
  return 1.0 / _qualityFunction();
}

END_NAMESPACE_GEOM

// End of file.
