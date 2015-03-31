// -*- C++ -*-

#if !defined(__geom_mesh_iss_build_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Build from a quadrilateral mesh.
template<int N, bool A, typename T, typename V, typename IF, typename IS>
inline
void
buildFromQuadMesh(const QuadMesh<N,A,T,V,IF>& quadMesh,
		  IndSimpSet<N,2,true,T,V,IS>* mesh) {
  typedef typename QuadMesh<N,A,T,V,IF>::IndexedFace IndexedFace;

  // The simplicial mesh will have the same vertices, but twice the number of
  // faces as the quad mesh.
  mesh->getVertices() = quadMesh.getVertices();
  mesh->getIndexedSimplices().resize(2 * quadMesh.getFacesSize());

  // Make two triangles from each quadrilateral.
  for (int faceIndex = 0; faceIndex != quadMesh.getFacesSize(); 
       ++faceIndex) {
    const T diagonal02 = 
      computeSquaredDistance(quadMesh.getFaceVertex(faceIndex, 0),
			     quadMesh.getFaceVertex(faceIndex, 2));
    const T diagonal13 = 
      computeSquaredDistance(quadMesh.getFaceVertex(faceIndex, 1),
			     quadMesh.getFaceVertex(faceIndex, 3));
    const IndexedFace& indexedFace = quadMesh.getIndexedFace(faceIndex);
    // Choose the smaller diagonal to split the quadrilateral.
    if (diagonal02 < diagonal13) {
      mesh->getIndexedSimplices()[2 * faceIndex][0] = indexedFace[0];
      mesh->getIndexedSimplices()[2 * faceIndex][1] = indexedFace[1];
      mesh->getIndexedSimplices()[2 * faceIndex][2] = indexedFace[2];
      mesh->getIndexedSimplices()[2 * faceIndex + 1][0] = indexedFace[2];
      mesh->getIndexedSimplices()[2 * faceIndex + 1][1] = indexedFace[3];
      mesh->getIndexedSimplices()[2 * faceIndex + 1][2] = indexedFace[0];
    }
    else {
      mesh->getIndexedSimplices()[2 * faceIndex][0] = indexedFace[1];
      mesh->getIndexedSimplices()[2 * faceIndex][1] = indexedFace[2];
      mesh->getIndexedSimplices()[2 * faceIndex][2] = indexedFace[3];
      mesh->getIndexedSimplices()[2 * faceIndex + 1][0] = indexedFace[3];
      mesh->getIndexedSimplices()[2 * faceIndex + 1][1] = indexedFace[0];
      mesh->getIndexedSimplices()[2 * faceIndex + 1][2] = indexedFace[1];
    }
  }

  // Update any auxilliary topological information.
  mesh->updateTopology();
}



template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter>
inline
void
buildFromSubsetVertices(const IndSimpSet<N,M,A,T,V,IS>& in,
			IntForIter verticesBeginning, 
			IntForIter verticesEnd,
			IndSimpSet<N,M,true,T,V,IS>* out) {
  typedef IndSimpSet<N,M,true,T,V,IS> ISS;
  typedef typename ISS::IndexedSimplex IndexedSimplex;

  // Flag whether each vertex is in the subset.
  ads::Array<1,bool> subset(in.getVerticesSize(), false);
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    subset[*verticesBeginning] = true;
  }

  // Determine the simplices that only use the subset of vertices.
  int m;
  std::vector<int> simplexIndices;
  for (int i = 0; i != in.getSimplicesSize(); ++i) {
    const IndexedSimplex& s = in.getIndexedSimplex(i);
    // Check to see if each vertex is in the subset.
    for (m = 0; m != ISS::M + 1; ++m) {
      if (subset[s[m]] == false) {
	break;
      }
    }
    // If all vertices are in the subset.
    if (m == M + 1) {
      simplexIndices.push_back(i);
    }
  }
  
  // Make the mesh based on the subset of simplex indices.
  buildFromSubsetSimplices(in, simplexIndices.begin(), simplexIndices.end(), 
			   out);
}



template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter>
inline
void
buildFromSubsetSimplices(const IndSimpSet<N,M,A,T,V,IS>& in,
			 IntForIter simplicesBeginning, 
			 IntForIter simplicesEnd,
			 IndSimpSet<N,M,true,T,V,IS>* out) {
  typedef IndSimpSet<N,M,true,T,V,IS> ISS;
  typedef typename ISS::IndexedSimplexIterator IndexedSimplexIterator;

  // Copy the subset of indexed simplices.
  const int numSimplices = int(std::distance(simplicesBeginning, 
					     simplicesEnd));
  out->getIndexedSimplices().resize(numSimplices);
  for (IndexedSimplexIterator s = out->getIndexedSimplicesBeginning(); 
	s != out->getIndexedSimplicesEnd(); ++s, ++simplicesBeginning) {
    // Make sure the simplex indices are valid.
    assert(0 <= *simplicesBeginning && 
	   *simplicesBeginning < in.getSimplicesSize());
    // Copy the indexed simplex.
    *s = in.getIndexedSimplex(*simplicesBeginning);
  }

  // Copy the vertices.
  out->getVertices() = in.getVertices();
  // Pack the mesh to get rid of unused vertices.
  pack(out);
}



template<int N, int M, bool A, typename T, typename V, typename IS, class LSF>
inline
void
buildFromVerticesInside(const IndSimpSet<N,M,A,T,V,IS>& in,
			const LSF& f,
			IndSimpSet<N,M,true,T,V,IS>* out) {
  // Determine the vertex indices that are inside.
  std::vector<int> insideIndices;
  determineVerticesInside(in, f, std::back_inserter(insideIndices));
  // Select the portion of this mesh whose vertices are inside.
  buildFromSubsetVertices(in, insideIndices.begin(), insideIndices.end(), out);
}



template<int N, int M, bool A, typename T, typename V, typename IS, class LSF>
inline
void
buildFromSimplicesInside(const IndSimpSet<N,M,A,T,V,IS>& in,
			 const LSF& f,
			 IndSimpSet<N,M,true,T,V,IS>* out) {
  // Determine the simplex indices that are inside.
  std::vector<int> insideIndices;
  determineSimplicesInside(in, f, std::back_inserter(insideIndices));
  // Select the portion of this mesh whose simplices are inside.
  buildFromSubsetSimplices(in, insideIndices.begin(), insideIndices.end(), 
			   out);
}



template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator>
inline
void
buildBoundary(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
	      IndSimpSet<N,M-1,true,T,V,IFace>* out,
	      IntOutputIterator usedVertexIndices) {
  // Build the boundary without packing.
  buildBoundaryWithoutPacking(in, out);

  // Pack the mesh to get rid of the interior vertices.
  pack(out, usedVertexIndices);
}



template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator>
inline
void
buildBoundaryWithoutPacking(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
			    IndSimpSet<N,M-1,true,T,V,IFace>* out,
			    IntOutputIterator incidentSimplices) {
  typedef typename IndSimpSetIncAdj<N,M,A,T,V,ISimp>::IndexedSimplexFace
    IndexedSimplexFace;

  // The set of indexed faces that lie on the boundary.
  std::vector<IndexedSimplexFace> boundaryFaces;

  int m;
  // Loop over the simplices.
  for (int n = 0; n != in.getSimplicesSize(); ++n) {
    // Loop over the faces of the simplex.
    for (m = 0; m != M + 1; ++m) {
      // If the face is on the boundary.
      if (in.getAdjacent(n, m) == -1) {
	// Add the indexed face.
	boundaryFaces.push_back(in.getIndexedSimplices()[n].getFace(m));
	// Record the incident simplex for the face.
	*incidentSimplices++ = n;
      }
    }
  }

  // Build the boundary with all of the vertices and the boundary faces.
  ads::Array<1,IndexedSimplexFace,false> faces(boundaryFaces.size(),
					       &boundaryFaces[0]);
  out->build(in.getVertices(), faces);
}




// Make a mesh (separated into connected components) that is the boundary of 
// the input mesh.
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator1, typename IntOutputIterator2>
inline
void
buildBoundaryOfComponentsWithoutPacking
(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
 IndSimpSet<N,M-1,true,T,V,IFace>* out,
 IntOutputIterator1 delimiterIterator,
 IntOutputIterator2 incidentSimplices) {
  // First get the boundary of the mesh.
  std::vector<int> incident;
  buildBoundaryWithoutPacking(in, out, std::back_inserter(incident));

  // Then separate the boundary into connected components.
  std::vector<int> permutation;
  {
    IndSimpSetIncAdj<N,M-1,true,T,V,IFace> tmp(*out);
    separateComponents(&tmp, delimiterIterator, 
		       std::back_inserter(permutation));
    *out = tmp;
  }

  // Permute the values for the incident simplices.
  for (std::vector<int>::const_iterator i = permutation.begin();
       i != permutation.end(); ++i) {
    *incidentSimplices++ = incident[*i];
  }
}



template<int N, bool A, typename T, typename V, typename ISimp, typename IFace>
inline
void
centerPointMeshSetSimplices(const IndSimpSet<N,1,A,T,V,IFace>& boundary,
			    IndSimpSet<N,2,true,T,V,ISimp>* mesh) {
  const int M = 2;
  int m;
  for (int n = 0; n != mesh->getSimplicesSize(); ++n) {
    for (m = 0; m != M; ++m) {
      mesh->getIndexedSimplices()[n][m] = boundary.getIndexedSimplices()[n][m];
    }
    mesh->getIndexedSimplices()[n][M] = mesh->getVerticesSize() - 1;
  }
}



template<int N, bool A, typename T, typename V, typename ISimp, typename IFace>
inline
void
centerPointMeshSetSimplices(const IndSimpSet<N,2,A,T,V,IFace>& boundary,
			    IndSimpSet<N,3,true,T,V,ISimp>* mesh) {
  const int M = 3;
  int m;
  for (int n = 0; n != mesh->getSimplicesSize(); ++n) {
    mesh->getIndexedSimplices()[n][0] = mesh->getVerticesSize() - 1;
    for (m = 0; m != M; ++m) {
      mesh->getIndexedSimplices()[n][m+1] = 
	boundary.getIndexedSimplices()[n][m];
    }
  }
}



// Make a mesh by connecting the boundary nodes to a new center point.
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace>
inline
void
centerPointMesh(const IndSimpSet<N,M-1,A,T,V,IFace>& boundary,
		IndSimpSet<N,M,true,T,V,ISimp>* mesh) {
  typedef IndSimpSet<N,M,true,T,V,ISimp> ISS;
  typedef typename ISS::Vertex Vertex;

  // Sanity check.
  assert(boundary.getVerticesSize() != 0);
  assert(boundary.getSimplicesSize() != 0);

  // Resize the mesh.
  mesh->getVertices().resize(boundary.getVerticesSize() + 1);
  mesh->getIndexedSimplices().resize(boundary.getSimplicesSize());

  // Set the vertices.
  {
    Vertex p(0.0);
    for (int n = 0; n != boundary.getVerticesSize(); ++n) {
      p += mesh->getVertices()[n] = boundary.getVertices()[n];
    }
    p /= boundary.getVerticesSize();
    mesh->getVertices()[mesh->getVerticesSize() - 1] = p;
  }

  // Set the indexed simplices.
  centerPointMeshSetSimplices(boundary, mesh);

  // Update any auxilliary topological information.
  mesh->updateTopology();
}



// Merge a range of meshes to make a single mesh.
template<int N, int M, typename T, typename V, typename IS, 
	 typename MeshInputIterator>
inline
void
merge(MeshInputIterator beginning, MeshInputIterator end,
      IndSimpSet<N,M,true,T,V,IS>* out) {
  typedef IndSimpSet<N,M,true,T,V,IS> Mesh;
  typedef typename Mesh::Vertex Vertex;
  typedef typename Mesh::IndexedSimplex IndexedSimplex;

  std::vector<Vertex> vertices;
  std::vector<IndexedSimplex> indexedSimplices;
  
  int indexOffset = 0;
  IndexedSimplex s;
  // For each input mesh.
  for (; beginning != end; ++beginning) {
    // Get the vertices.
    for (int i = 0; i != beginning->getVerticesSize(); ++i) {
      vertices.push_back(beginning->getVertex(i));
    }
    // Get the indexed simplices.
    for (int i = 0; i != beginning->getSimplicesSize(); ++i) {
      // Get the indexed simplex.
      s = beginning->getIndexedSimplex(i);
      // Offset the vertex indices.
      for (int m = 0; m != M + 1; ++m) {
	s[m] += indexOffset;
      }
      indexedSimplices.push_back(s);
    }
    // Update the index offset.
    indexOffset += beginning->getVerticesSize();
  }

  // Build the mesh.
  out->build(int(vertices.size()), &vertices[0], int(indexedSimplices.size()),
	     &indexedSimplices[0]);
}



// Merge two meshes to make a single mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
merge2(const IndSimpSet<N,M,A,T,V,IS>& a, const IndSimpSet<N,M,A,T,V,IS>& b,
       IndSimpSet<N,M,true,T,V,IS>* out) {
  typedef IndSimpSet<N,M,A,T,V,IS> Mesh;

  // Make an array of the two meshes.
  ads::FixedArray<2,const Mesh*> meshes(&a, &b);
  // Call the above merge function.
  merge(ads::constructIndirectIterator(meshes.begin()), 
	ads::constructIndirectIterator(meshes.end()), out);
}

END_NAMESPACE_GEOM

// End of file.
