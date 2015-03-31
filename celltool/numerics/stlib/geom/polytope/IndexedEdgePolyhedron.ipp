// -*- C++ -*-

#if !defined(__geom_IndexedEdgePolyhedron_ipp__)
#error This file is an implementation detail of the class IndexedEdgePolyhedron.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors and destructor.
//

template<typename T>
inline
IndexedEdgePolyhedron<T>& 
IndexedEdgePolyhedron<T>::
operator=(const IndexedEdgePolyhedron& other) {
  // Avoid assignment to self
  if (&other != this){
    _vertices = other._vertices;
    _edges = other._edges;
  }
  // Return *this so assignments can chain
  return *this;
}

//
// Mathematical Member Functions
//

template<typename T>
inline
void 
IndexedEdgePolyhedron<T>::
computeBBox(BBox<3,Number>* bb) const {
  // if this polyhedron has any vertices.
  if (! _vertices.empty()) {
    // Bound the vertices.
    bb->bound(_vertices.begin(), _vertices.end());
  }
  else {
    // Make an empty bounding box.
    bb->setLowerCorner(Point(0.0));
    bb->setUpperCorner(Point(-1.0));
  }
}

END_NAMESPACE_GEOM

// End of file.
