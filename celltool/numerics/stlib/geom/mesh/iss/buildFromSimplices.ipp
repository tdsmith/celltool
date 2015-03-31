// -*- C++ -*-

#if !defined(__geom_mesh_iss_buildFromSimplices_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<int N, int M, typename T, typename V, typename IS,
	 typename VertexForIter>
inline
void
buildFromSimplices(VertexForIter verticesBeginning, 
		   VertexForIter verticesEnd,
		   IndSimpSet<N,M,true,T,V,IS>* mesh) {
  typedef IndSimpSet<N,M,true,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  // Used to hold the distinct vertices.
  std::vector<Vertex> distinct;
  
  // Resize the simplices array.
  const int numInputVertices = 
    int(std::distance(verticesBeginning, verticesEnd));
  assert(numInputVertices % (M + 1) == 0);
  mesh->getIndexedSimplices().resize(numInputVertices / (M + 1));

  // Make the indexed set of distinct vertices.
  buildDistinctPoints<N>
    (verticesBeginning, verticesEnd, 
      std::back_inserter(distinct), 
      reinterpret_cast<int*>(mesh->getIndexedSimplices().data()));
  
  // Copy the distinct vertices into the vertex array.
  mesh->getVertices().resize(int(distinct.size()));
  std::copy(distinct.begin(), distinct.end(), mesh->getVertices().begin());

  // Update any auxilliary topological information.
  mesh->updateTopology();
}

END_NAMESPACE_GEOM

// End of file.
