// -*- C++ -*-

#if !defined(__geom_mesh_iss_geometry_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Return the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
typename IndSimpSetIncAdj<2,1,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<2,1,A,T,V,IS>& mesh, const int n) {
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  Vertex x;
  computeVertexNormal(mesh, n, &x);
  return x;
}

// Compute the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
void
computeVertexNormal(const IndSimpSetIncAdj<2,1,A,T,V,IS>& mesh, const int n,
		    typename IndSimpSetIncAdj<2,1,A,T,V,IS>::Vertex* 
		    vertexNormal) {
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  // The vertex should have two incident faces.
  assert(mesh.getIncidentSize(n) == 2);

  *vertexNormal = 0.0;

  // The first incident edge.
  int i = mesh.getIncident(n, 0);
  Vertex a = mesh.getVertex(mesh.getIndexedSimplex(i)[1]);
  a -= mesh.getVertex(mesh.getIndexedSimplex(i)[0]);
  rotateMinusPiOver2(&a);
  normalize(&a);
  *vertexNormal += a;

  // The second incident edge.
  i = mesh.getIncident(n, 1);
  a = mesh.getVertex(mesh.getIndexedSimplex(i)[1]);
  a -= mesh.getVertex(mesh.getIndexedSimplex(i)[0]);
  rotateMinusPiOver2(&a);
  normalize(&a);
  *vertexNormal += a;

  normalize(vertexNormal);
}



// Return the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
typename IndSimpSetIncAdj<2,2,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<2,2,A,T,V,IS>& mesh, const int n) {
  typedef IndSimpSetIncAdj<2,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  Vertex x;
  computeVertexNormal(mesh, n, &x);
  return x;
}



// Compute the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
void
computeVertexNormal(const IndSimpSetIncAdj<2,2,A,T,V,IS>& mesh, const int n,
		    typename IndSimpSetIncAdj<2,2,A,T,V,IS>::Vertex* 
		    vertexNormal) {
  typedef IndSimpSetIncAdj<2,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::IndexedSimplex IndexedSimplex;
  typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;

#ifdef DEBUG_geom
  // This should be a boundary vertex.
  assert(mesh.isVertexOnBoundary(n));
  // The vertex should have two incident faces.
  assert(mesh.getIncidentSize(n) >= 1);
#endif

  const int M = 2;

  int i;
  Vertex v;

  *vertexNormal = 0.0;
  // For each incident simplex.
  const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
  for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n);
	iter != iterEnd; ++iter) {
    // The indexed simplex that defines the face.
    const IndexedSimplex& is = mesh.getIndexedSimplex(*iter);
    // The local index of the n_th vertex in the face.
    i = is.getVertexIndex(n);

    if (mesh.getAdjacent(*iter, (i+2)%(M+1)) == -1) {
      v = mesh.getVertex(is[(i+1)%(M+1)]);
      v -= mesh.getVertex(n);
      rotateMinusPiOver2(&v);
      normalize(&v);
      *vertexNormal += v;
    }

    if (mesh.getAdjacent(*iter, (i+1)%(M+1)) == -1) {
      v = mesh.getVertex(is[(i+2)%(M+1)]);
      v -= mesh.getVertex(n);
      rotatePiOver2(&v);
      normalize(&v);
      *vertexNormal += v;
    }
  }
  normalize(vertexNormal);
}



// Return the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
typename IndSimpSetIncAdj<3,2,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, const int n) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  Vertex x;
  computeVertexNormal(mesh, n, &x);
  return x;
}



// Compute the outward normal at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
void
computeVertexNormal(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, const int n,
		    typename IndSimpSetIncAdj<3,2,A,T,V,IS>::Vertex* 
		    vertexNormal) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::IndexedSimplex IndexedSimplex;
  typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;

  // The vertex should have at least 3 incident faces.
  assert(mesh.getIncidentSize(n) >= 3);

  *vertexNormal = 0.0;
  int i;
  // CONTINUE
  //Simplex s;
  Vertex x, y, faceNormal;

  // For each incident face.
  const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
  for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n);
       iter != iterEnd; ++iter) {
    // The indexed simplex that defines the face.
    const IndexedSimplex& is = mesh.getIndexedSimplex(*iter);
    // The local index of the n_th vertex in the face.
    i = is.getVertexIndex(n);
    // Construct the face.
    // CONTINUE
    /*
    s[0] = mesh.getVertex(is[i]);
    s[1] = mesh.getVertex(is[(i+1)%3]);
    s[2] = mesh.getVertex(is[(i+2)%3]);
    */

    // Compute the face normal.
    x = mesh.getVertex(is[(i+1)%3]);
    x -= mesh.getVertex(is[i]);
    normalize(&x);
    y = mesh.getVertex(is[(i+2)%3]);
    y -= mesh.getVertex(is[i]);
    normalize(&y);
    computeCrossProduct(x, y, &faceNormal);
    normalize(&faceNormal);

    // Contribute to the vertex normal.
    // Multiply by the angle between the edges.
    faceNormal *= std::acos(computeDotProduct(x, y));
    *vertexNormal += faceNormal;
  }
  normalize(vertexNormal);
}



// Return the outward normal at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
inline
typename IndSimpSetIncAdj<3,3,A,T,V,IS>::Vertex
computeVertexNormal(const IndSimpSetIncAdj<3,3,A,T,V,IS>& mesh, const int n) {
  typedef IndSimpSetIncAdj<3,3,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  Vertex x;
  computeVertexNormal(mesh, n, &x);
  return x;
}



// Compute the outward normal at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
inline
void
computeVertexNormal(const IndSimpSetIncAdj<3,3,A,T,V,IS>& mesh, const int n,
		    typename IndSimpSetIncAdj<3,3,A,T,V,IS>::Vertex*
		    vertexNormal) {
  typedef IndSimpSetIncAdj<3,3,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::IndexedSimplex IndexedSimplex;
  typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;
  typedef typename ISS::IndexedSimplexFace IndexedSimplexFace;

#ifdef DEBUG_geom
  // This should be a boundary vertex.
  assert(mesh.isVertexOnBoundary(n));
  // The vertex should have two incident faces.
  assert(mesh.getIncidentSize(n) >= 1);
#endif

  const int M = 3;

  int i, j;
  Vertex x, y, faceNormal;
  IndexedSimplexFace face;

  *vertexNormal = 0.0;
  // For each incident simplex.
  const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
  for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n);
	iter != iterEnd; ++iter) {
    // The incident indexed simplex.
    const IndexedSimplex& is = mesh.getIndexedSimplex(*iter);
    // The local index of the n_th vertex in the incident simplex.
    i = is.getVertexIndex(n);
    
    // Loop over the incident faces.
    for (j = 1; j != M + 1; ++j) {
      // If this is a boundary face.
      if (mesh.getAdjacent(*iter, (i+j)%(M+1)) == -1) {
	// Get the indexed face.
	is.getFace((i+j)%(M+1), &face);

	// Compute the face normal.
	x = mesh.getVertex(face[1]);
	x -= mesh.getVertex(face[0]);
	normalize(&x);
	y = mesh.getVertex(face[2]);
	y -= mesh.getVertex(face[0]);
	normalize(&y);
	computeCrossProduct(x, y, &faceNormal);
	normalize(&faceNormal);

	// Contribute to the vertex normal.
	// Multiply by the angle between the edges.
	faceNormal *= std::acos(computeDotProduct(x, y));
	*vertexNormal += faceNormal;
      }
    }
  }
  normalize(vertexNormal);
}








// Compute the outward normal for the specified simplex (triangle face).
template<bool A, typename T, typename V, typename IS>
inline
void
computeSimplexNormal(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		     const int simplexIndex, V* simplexNormal) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  assert(0 <= simplexIndex && simplexIndex < mesh.getSimplicesSize());

  Vertex x, y;
  // Compute the face normal.
  x = mesh.getSimplexVertex(simplexIndex, 2);
  x -= mesh.getSimplexVertex(simplexIndex, 1);
  y = mesh.getSimplexVertex(simplexIndex, 0);
  y -= mesh.getSimplexVertex(simplexIndex, 1);
  computeCrossProduct(x, y, simplexNormal);
  normalize(simplexNormal);
}




// Compute the outward normals for the simplices (triangle faces).
template<bool A, typename T, typename V, typename IS, bool AA>
inline
void
computeSimplexNormals(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		      ads::Array<1,V,AA>* simplexNormals) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Simplex Simplex;
  typedef typename ISS::Vertex Vertex;

  assert(mesh.getSimplicesSize() == simplexNormals->size());

  Simplex simplex;
  Vertex normal, x, y;
  // For each simplex.
  for (int n = 0; n != mesh.getSimplicesSize(); ++n) {
    // Get the simplex.
    mesh.getSimplex(n, &simplex);
    // Compute the face normal.
    x = simplex[2];
    x -= simplex[1];
    y = simplex[0];
    y -= simplex[1];
    computeCrossProduct(x, y, &normal);
    normalize(&normal);
    (*simplexNormals)[n] = normal;
  }
}





// Compute the outward normals for the simplices (line segments).
template<bool A, typename T, typename V, typename IS, bool AA>
inline
void
computeSimplexNormals(const IndSimpSetIncAdj<2,1,A,T,V,IS>& mesh, 
		      ads::Array<1,V,AA>* simplexNormals)
{
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> ISS;
  typedef typename ISS::Simplex Simplex;
  typedef typename ISS::Vertex Vertex;

  assert(mesh.getSimplicesSize() == simplexNormals->size());

  Simplex simplex;
  Vertex x;
  // For each simplex.
  for (int n = 0; n != mesh.getSimplicesSize(); ++n) {
    // The tangent direction.
    x = mesh.getSimplexVertex(n, 1);
    x -= mesh.getSimplexVertex(n, 0);
    // The normal direction.
    rotateMinusPiOver2(&x);
    // Normalize to unit length.
    normalize(&x);
    (*simplexNormals)[n] = x;
  }
}



// Compute the outward normals for the vertices.
template<bool A, typename T, typename V, typename IS, bool AA>
inline
void
computeVertexNormals(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		     const ads::Array<1,V,AA>& simplexNormals,
		     ads::Array<1,V,AA>* vertexNormals) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Simplex Simplex;
  typedef typename ISS::Vertex Vertex;

  assert(mesh.getSimplicesSize() == simplexNormals.size() &&
	 mesh.getVerticesSize() == vertexNormals->size());

  *vertexNormals = Vertex(0, 0, 0);
  Vertex normal, x, y;
  Simplex simplex;
  int i, m;
  // For each simplex.
  for (int n = 0; n != mesh.getSimplicesSize(); ++n) {
    // Get the simplex.
    mesh.getSimplex(n, &simplex);
    // Contribute to the vertex normal.
    for (m = 0; m != 3; ++m) {
      x = simplex[(m+1)%3];
      x -= simplex[m];
      normalize(&x);
      y = simplex[(m+2)%3];
      y -= simplex[m];
      normalize(&y);
      i = mesh.getIndexedSimplices()[n][m];
      // Get the simplex normal.
      normal = simplexNormals[n];
      // Multiply by the angle between the edges.
      normal *= std::acos(computeDotProduct(x, y));
      (*vertexNormals)[i] += normal;
    }
  }

  // Normalize the vertex directions.
  for (int n = 0; n != vertexNormals->size(); ++n) {
    normalize(&(*vertexNormals)[n]);
  }
}





// Compute the outward normals for the simplices and vertices.
template<bool A, typename T, typename V, typename IS, bool A1, bool A2>
inline
void
computeSimplexAndVertexNormals(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh,
			       ads::Array<1,V,A1>* simplexNormals,
			       ads::Array<1,V,A2>* vertexNormals) {
  computeSimplexNormals(mesh, simplexNormals);
  computeVertexNormals(mesh, *simplexNormals, vertexNormals);
}





// Return the cosine of the interior angle at the specified vertex.
template<int N, bool A, typename T, typename V, typename IS>
inline
T
computeCosineAngle(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, 
		   const int vertexIndex) {
  typedef IndSimpSetIncAdj<2,1,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  // The vertex should have two incident faces.
  assert(mesh.getIncidentSize(vertexIndex) == 2);

  // The two incident edges.
  int i = mesh.getIncident(vertexIndex, 0);
  int j = mesh.getIncident(vertexIndex, 1);
  if (mesh.getIndexedSimplex(i)[0] == vertexIndex) {
    std::swap(i, j);
  }
  assert(mesh.getIndexedSimplex(i)[1] == vertexIndex &&
	 mesh.getIndexedSimplex(j)[0] == vertexIndex);
  // Now i is the previous edge and j is the next edge.

  // Make two unit vectors with tails at the n_th vertex and heads in the 
  // directions of the neighboring vertices.
  Vertex a = mesh.getSimplexVertex(i, 0);
  a -= mesh.getVertex(vertexIndex);
  normalize(&a);
  Vertex b = mesh.getSimplexVertex(j, 1);
  b -= mesh.getVertex(vertexIndex);
  normalize(&b);

  // Return the cosine of the interior angle.
  return computeDotProduct(a, b);
}



// Return the cosine of the interior angle at the specified 1-face.
template<bool A, typename T, typename V, typename IS>
inline
T
computeCosineAngle(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
		   const typename IndSimpSetIncAdj<3,2,A,T,V,IS>::Face& face) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;

  // The simplex dimension.
  const int M = 2;

  // Check that the face is valid.
  assert(0 <= face.first && face.first < mesh.getSimplicesSize());
  assert(0 <= face.second && face.second < M + 1);
  // It must be an internal face.
  assert(! mesh.isOnBoundary(face));
  
  // The cosine of the angle is the negative of the dot product of the 
  // incident simplex normals.
  // n0 . n1 == cos(pi - a) == - cos(a)
  Vertex n0, n1;
  computeSimplexNormal(mesh, face.first, &n0);
  computeSimplexNormal(mesh, mesh.getAdjacent(face.first, face.second), &n1);
  return - computeDotProduct(n0, n1);
}



// Return the cosine of the interior angle at the specified boundary vertex.
template<bool A, typename T, typename V, typename IS>
inline
T
computeCosineBoundaryAngle(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, 
			   const int vertexIndex) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::Face Face;

  // The simplex dimension.
  const int M = 2;

  // It should be a boundary vertex.
  assert(mesh.isVertexOnBoundary(vertexIndex));

  //
  // Get the two neighboring boundary vertices.
  //
  int neighbors[2];
  {
    int neighborCount = 0;
    int simplexIndex;
    int i1, i2;
    // For each incident simplex.
    for (int n = 0; n != mesh.getIncidentSize(vertexIndex); ++n) {
      // The index of the simplex.
      simplexIndex = mesh.getIncident(vertexIndex, n);
      // For each face of the simplex.
      for (int m = 0; m != M + 1; ++m) {
	// Skip the face that is opposite the specified vertex.
	if (mesh.getIndexedSimplex(simplexIndex)[m] != vertexIndex) {
	  // If this is a boundary face.
	  if (mesh.isOnBoundary(Face(simplexIndex, m))) {
	    // The vertex (other than the specified one) is a neighboring 
	    // boundary vertex.
	    i1 = mesh.getIndexedSimplex(simplexIndex)[(m + 1) % (M + 1)];
	    i2 = mesh.getIndexedSimplex(simplexIndex)[(m + 2) % (M + 1)];
	    assert(neighborCount != 2);
	    if (i1 != vertexIndex) {
	      neighbors[neighborCount++] = i1;
	    }
	    else if (i2 != vertexIndex) {
	      neighbors[neighborCount++] = i2;
	    }
	    else {
	      assert(false);
	    }
	  }
	}
      }
    }
    assert(neighborCount == 2);
  }

  // Make two unit vectors with tails at the specified vertex and heads in the 
  // directions of the neighboring vertices.
  Vertex a = mesh.getVertex(neighbors[0]);
  a -= mesh.getVertex(vertexIndex);
  normalize(&a);
  Vertex b = mesh.getVertex(neighbors[1]);
  b -= mesh.getVertex(vertexIndex);
  normalize(&b);

  // Return the cosine of the interior angle.
  return computeDotProduct(a, b);
}



// Return the solid interior angle at the specified vertex.
template<bool A, typename T, typename V, typename IS>
inline
T
computeAngle(const IndSimpSetIncAdj<3,2,A,T,V,IS>& mesh, const int n) {
  typedef IndSimpSetIncAdj<3,2,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::IndexedSimplex IndexedSimplex;
  typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;
  typedef Simplex<3,V,T> Tetrahedron;

  // Get the inward pointing normal.
  Vertex inwardNormal;
  computeVertexNormal(mesh, n, &inwardNormal);
  inwardNormal.negate();


  int i;
  Tetrahedron t;
  T solidAngle = 0;

  // For each incident face.
  const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
  for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n);
	iter != iterEnd; ++iter) {
    // The indexed simplex that defines the face.
    const IndexedSimplex& is = mesh.getIndexedSimplex(*iter);
    // The local index of the n_th vertex in the face.
    i = is.getVertexIndex(n);
    // Construct the tetrahedron defined by the inward normal and the face.
    t[0] = mesh.getVertex(is[i]);
    t[0] += inwardNormal;
    t[1] = mesh.getVertex(is[i]);
    t[2] = mesh.getVertex(is[(i+1)%3]);
    t[3] = mesh.getVertex(is[(i+2)%3]);

    solidAngle += computeAngle(t, 1);
  }
  return solidAngle;
}



// Project the line segments to 1-D and collect them.
template<bool A, typename T, typename V, typename IS, typename OutputIterator>
inline
void
projectAndGetSimplices(const IndSimpSet<2,1,A,T,V,IS>& mesh, 
		       OutputIterator simplices) {
  typedef IndSimpSet<2,1,A,T,V,IS> ISS;
  typedef Simplex<1,ads::FixedArray<1,T>,T> Segment;
  typedef typename ISS::Simplex Simplex;
  Simplex s;
  Segment t;

  const int size = mesh.getSimplicesSize();
  // For each simplex.
  for (int n = 0; n != size; ++n) {
    // Get the n_th simplex.
    mesh.getSimplex(n, &s);
    // Project the line segment in 2-D to a line segment in 1-D.
    projectToLowerDimension(s, &t);
    // Add the line segment to the sequence of simplices.
    *simplices++ = t;
  }
}



// Project the triangle simplices to 2-D and collect them.
template<bool A, typename T, typename V, typename IS, typename OutputIterator>
inline
void
projectAndGetSimplices(const IndSimpSet<3,2,A,T,V,IS>& mesh, 
		       OutputIterator simplices) {
  typedef IndSimpSet<3,2,A,T,V,IS> ISS;
  typedef Simplex<2,ads::FixedArray<2,T>,T> Triangle;
  typedef typename ISS::Simplex Simplex;
  Simplex s;
  Triangle t;

  const int size = mesh.getSimplicesSize();
  // For each simplex.
  for (int n = 0; n != size; ++n) {
    // Get the n_th simplex.
    mesh.getSimplex(n, &s);
    // Project the triangle in 3-D to a triangle in 2-D.
    projectToLowerDimension(s, &t);
    // Add the triangle to the sequence of simplices.
    *simplices++ = t;
  }
}

END_NAMESPACE_GEOM

// End of file.
