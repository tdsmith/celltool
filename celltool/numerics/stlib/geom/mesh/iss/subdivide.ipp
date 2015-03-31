// -*- C++ -*-

#if !defined(__geom_mesh_iss_subdivide_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Subdivide by splitting each simplex in half.
template<int N, bool A, typename T, typename V, typename IS>
inline
void
subdivide(const IndSimpSet<N,1,A,T,V,IS>& in, 
	  IndSimpSet<N,1,true,T,V,IS>* out) {
  // Resize the output mesh.
  out->build(in.getVerticesSize() + in.getSimplicesSize(), 
	     2 * in.getSimplicesSize());

  //
  // Build the vertices.
  //

  // The old vertices.
  int vertexIndex = 0;
  for (int i = 0; i != in.getVerticesSize(); ++i, ++vertexIndex) {
    out->getVertices()[vertexIndex] = in.getVertex(i);
  }
  // The midpoints.
  for (int i = 0; i != in.getSimplicesSize(); ++i, ++vertexIndex) {
    out->getVertices()[vertexIndex] = in.getSimplexVertex(i, 0);
    out->getVertices()[vertexIndex] += in.getSimplexVertex(i, 1);
    out->getVertices()[vertexIndex] /= 2.0;
  }
  
  //
  // Build the indexed simplices.
  //
  for (int i = 0; i != in.getSimplicesSize(); ++i) {
    out->getIndexedSimplices()[2 * i][0] = in.getIndexedSimplices()[i][0];
    out->getIndexedSimplices()[2 * i][1] = in.getVerticesSize() + i;
    out->getIndexedSimplices()[2 * i + 1][0] = in.getVerticesSize() + i;
    out->getIndexedSimplices()[2 * i + 1][1] = in.getIndexedSimplices()[i][1];
  }

  // Update the topology if necessary.
  out->updateTopology();
}



// Subdivide by splitting each simplex into four similar simplices.
template<int N, bool A, typename T, typename V, typename IS>
inline
void
subdivide(const IndSimpSetIncAdj<N,2,A,T,V,IS>& in, 
	  IndSimpSet<N,2,true,T,V,IS>* out) {
  typedef IndSimpSet<N,2,true,T,V,IS> OutputMesh;
  typedef typename OutputMesh::Vertex Vertex;
  typedef typename OutputMesh::IndexedSimplex IndexedSimplex;

  const int M = 2;

  // The vertices in the subdivided mesh.
  std::vector<Vertex> vertices;
  // The indexed midpoint vertices for each simplex.
  ads::Array<1, IndexedSimplex> 
    midpointIndexedSimplices(in.getSimplicesSize());


  // Add the vertices from the input mesh.
  for (int i = 0; i != in.getVerticesSize(); ++i ) {
    vertices.push_back(in.getVertex(i));
  }

  //  
  // Loop over the simplices, determining the midpoint vertices.
  //
  int adjacentSimplex;
  // For each simplex.
  for (int i = 0; i != in.getSimplicesSize(); ++i) {
    // For each vertex of the simplex.
    for (int m = 0; m != M + 1; ++m) {
      adjacentSimplex = in.getAdjacent(i, m);
      // If there is no adjacent simplex or if this simplices' index is less 
      // than the adjacent simplices' index.
      if (adjacentSimplex == -1 || i < adjacentSimplex) {
	// Record the index of the new midpoint vertex.
	midpointIndexedSimplices[i][m] = int(vertices.size());
	// Make a new midpoint vertex.
	vertices.push_back((in.getSimplexVertex(i, (m + 1) % (M + 1)) + 
			    in.getSimplexVertex(i, (m + 2) % (M + 1))) / 2.0);
      }
      // Otherwise, the adjacent simplex already added the midpoint vertex.
      else {
	// Record the index of that midpoint vertex.
	midpointIndexedSimplices[i][m] = 
	  midpointIndexedSimplices[adjacentSimplex][in.getMirrorIndex(i, m)];
      }
    }
  }


  // Resize the output mesh.
  out->build(int(vertices.size()), 4 * in.getSimplicesSize());

  // Set the vertices.
  for (int i = 0; i != out->getVerticesSize(); ++i) {
    out->getVertices()[i] = vertices[i];
  }

  //
  // Build the indexed simplices in the output mesh from the input simplices
  // and midpoint simplices.
  //
  for (int i = 0; i != in.getSimplicesSize(); ++i) {
    out->getIndexedSimplices()[4 * i][0] = in.getIndexedSimplices()[i][0];
    out->getIndexedSimplices()[4 * i][1] = midpointIndexedSimplices[i][2];
    out->getIndexedSimplices()[4 * i][2] = midpointIndexedSimplices[i][1];

    out->getIndexedSimplices()[4 * i + 1][0] = in.getIndexedSimplices()[i][1];
    out->getIndexedSimplices()[4 * i + 1][1] = midpointIndexedSimplices[i][0];
    out->getIndexedSimplices()[4 * i + 1][2] = midpointIndexedSimplices[i][2];

    out->getIndexedSimplices()[4 * i + 2][0] = in.getIndexedSimplices()[i][2];
    out->getIndexedSimplices()[4 * i + 2][1] = midpointIndexedSimplices[i][1];
    out->getIndexedSimplices()[4 * i + 2][2] = midpointIndexedSimplices[i][0];

    out->getIndexedSimplices()[4 * i + 3][0] = midpointIndexedSimplices[i][0];
    out->getIndexedSimplices()[4 * i + 3][1] = midpointIndexedSimplices[i][1];
    out->getIndexedSimplices()[4 * i + 3][2] = midpointIndexedSimplices[i][2];
  }

  // Update the topology if necessary.
  out->updateTopology();
}

END_NAMESPACE_GEOM

// End of file.
