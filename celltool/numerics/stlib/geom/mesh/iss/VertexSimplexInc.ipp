// -*- C++ -*-

#if !defined(__geom_VertexSimplexInc_ipp__)
#error This file is an implementation detail of the class VertexSimplexInc.
#endif

BEGIN_NAMESPACE_GEOM


//
// Manipulators
//


template<int M>
template<typename IS, bool A>
inline
void
VertexSimplexInc<M>::
build(const int numVertices, const ads::Array<1,IS,A>& simplices) {
  // Clear the old incidence information.
  clear();

  //
  // Determine the number of simplices incident to each vertex.
  //

  ads::Array<1,int> numIncident(numVertices, int(0));
  int m;
  // Loop over the simplices.
  for (int i = 0; i != simplices.size(); ++i) {
    // Loop over the vertices of this simplex.
    for (m = 0; m != M+1; ++m) {
      ++numIncident[simplices[i][m]];
    }
  }

  //
  // Build the incidence array with the size information.
  //

  _inc.rebuild((M + 1) * simplices.size(), 
	       numIncident.begin(), numIncident.end());

  //
  // Fill the incidence array with the simplex indices.
  //

  // Vertex-simplex incidences.
  ads::Array<1,Iterator> vsi(numVertices);
  for (int i = 0; i != numVertices; ++i) {
    vsi[i] = _inc[i];
  }
  // vertex index.
  int vi;
  // Loop over the simplices.
  for (int i = 0; i != simplices.size(); ++i) {
    // Loop over the vertices of this simplex.
    for (m = 0; m != M+1; ++m) {
      // Add the simplex to the vertex incidence array.
      vi = simplices[i][m];
      *vsi[vi] = i;
      ++vsi[vi];
    }
  }

#ifdef DEBUG_VertexSimplexInc
  // Check that we added the correct number of incidences for each vertex.
  for (int i = 0; i != numVertices; ++i) {
    assert(vsi[i] == _inc.end(i));
  }
#endif
}


//
// File output.
//


template<int M>
std::ostream&
operator<<(std::ostream& out, const VertexSimplexInc<M>& x) {
  x.put(out);
  return out;
}

END_NAMESPACE_GEOM

// End of file.
