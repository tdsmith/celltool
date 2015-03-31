// -*- C++ -*-

#if !defined(__geom_mesh_iss_accessors_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Get the previous vertex index.
template<int N, bool A, typename T, typename V, typename IS>
inline
int
getPreviousVertexIndex(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, 
		       const int n) {
  typedef IndSimpSetIncAdj<N,1,A,T,V,IS> ISS;
  typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;

  const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
  int simplexIndex;
  // For each incident simplex.
  for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n); 
       iter != iterEnd; ++iter) {
    simplexIndex = *iter;
    if (mesh.getIndexedSimplex(simplexIndex)[1] == n) {
      return mesh.getIndexedSimplex(simplexIndex)[0];
    }
  }
  assert(false);
  return -1;
}


// Get the next vertex index.
template<int N, bool A, typename T, typename V, typename IS>
inline
int
getNextVertexIndex(const IndSimpSetIncAdj<N,1,A,T,V,IS>& mesh, const int n) {
  typedef IndSimpSetIncAdj<N,1,A,T,V,IS> ISS;
  typedef typename ISS::IncidenceConstIterator IncidenceConstIterator;

  const IncidenceConstIterator iterEnd = mesh.getIncidentEnd(n);
  int simplexIndex;
  // For each incident simplex.
  for (IncidenceConstIterator iter = mesh.getIncidentBeginning(n); 
       iter != iterEnd; ++iter) {
    simplexIndex = *iter;
    if (mesh.getIndexedSimplex(simplexIndex)[0] == n) {
      return mesh.getIndexedSimplex(simplexIndex)[1];
    }
  }
  assert(false);
  return -1;
}


template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
getFace(const IndSimpSet<N,M,A,T,V,IS>& mesh,
	const int simplexIndex, const int vertexIndex, 
	typename IndSimpSet<N,M,A,T,V,IS>::SimplexFace* face) {
  typedef typename IndSimpSet<N,M,A,T,V,IS>::IndexedSimplexFace 
    IndexedSimplexFace;

  IndexedSimplexFace indexedFace;
  // Get the vertex indices of the face.
  getIndexedFace(mesh, simplexIndex, vertexIndex, &indexedFace);
  // Assign the vertices of the face.
  for (int m = 0; m != M; ++m) {
    (*face)[m] = mesh.getVertices()[indexedFace[m]];
  }
}


template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
getIndexedFace(const IndSimpSet<N,M,A,T,V,IS>& mesh,
	       const int simplexIndex, const int vertexIndex, 
	       typename IndSimpSet<N,M,A,T,V,IS>::IndexedSimplexFace* face) {
  // The number of the vertex in the simplex.
  const int n = mesh.getIndexedSimplices()[simplexIndex].getVertexIndex
    (vertexIndex);
  // Get the vertex indices of the face.
  mesh.getIndexedSimplices()[simplexIndex].getFace(n, face);
}


template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
getCentroid(const IndSimpSet<N,M,A,T,V,IS>& mesh, const int n, 
	    typename IndSimpSet<N,M,A,T,V,IS>::Vertex* x) {
  // CONTINUE: This is the arithmetic mean.  Look up the formula for 
  // the centroid.

  typedef typename IndSimpSet<N,M,A,T,V,IS>::IndexedSimplex IndexedSimplex;

  const IndexedSimplex& s = mesh.getIndexedSimplices()[n];
  *x = mesh.getVertices()[s[0]];
  for (int m = 1; m != M + 1; ++m) {
    *x += mesh.getVertices()[s[m]];
  }
  *x /= (M + 1);
}


template<int N, int M, bool A, typename T, typename V, typename IS>
inline
bool
isOriented(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh) {
  typedef IndSimpSetIncAdj<N,M,A,T,V,IS> ISS;
  typedef typename ISS::IndexedSimplexFace IndexedSimplexFace;

  int m, nu, mu;
  IndexedSimplexFace f, g;

  // For each simplex.
  const int simplicesSize = mesh.getSimplicesSize();
  for (int n = 0; n != simplicesSize; ++n) {
    // For each adjacent simplex.
    for (m = 0; m != M + 1; ++m) {
      // The m_th adjacent simplex.
      nu = mesh.getAdjacent(n, m);
      // If this is not a boundary face.
      if (nu != -1) {
	mu = mesh.getMirrorIndex(n, m);
	mesh.getIndexedSimplex(n).getFace(m, &f);
	mesh.getIndexedSimplex(nu).getFace(mu, &g);
	g.negate();
	if (! haveSameOrientation(f, g)) {
	  return false;
	}
      }
    }
  }
  
  return true;
}

END_NAMESPACE_GEOM

// End of file.
