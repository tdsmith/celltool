// -*- C++ -*-

#if !defined(__geom_mesh_iss_topology_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Count the connected components of the mesh.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
int
countComponents(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh) {
  // Which simplices have been identified as belonging to a particular 
  // component.
  std::vector<bool> used(mesh.getSimplicesSize(), false);

  // Simplex indices in a single component.
  std::vector<int> indices;
  int i, iEnd;
  int n = 0;
  // The number of components.
  int numComponents = 0;
  // The number of simplices accounted for so far.
  int numSimplices = 0;
  do {
    ++numComponents;
    // Get the first unused simplex.
    while (used[n]) {
      ++n;
    }
    // Get the component that contains the n_th simplex.
    determineSimplicesInComponent(mesh, n, std::back_inserter(indices));

    // Record the simplices that are used in this component.
    iEnd = indices.size();
    for (i = 0; i != iEnd; ++i) {
      used[indices[i]] = true;
    }

    numSimplices += indices.size();
    indices.clear();
  } while (numSimplices != mesh.getSimplicesSize());
  
  return numComponents;
}

END_NAMESPACE_GEOM

// End of file.
