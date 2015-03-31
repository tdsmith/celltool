// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_flip_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Flip faces for as long as the quality of the mesh is improved.
template<class DistortionFunction, 
	 int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont>
inline
int
flip(SimpMeshRed<N,2,T,_Node,_Cell,Cont>* mesh) {
  typedef SimpMeshRed<N,2,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::FaceIterator FaceIterator;

  DistortionFunction distortionFunction;
  int count = 0, c;

  // Sweep over the faces until none are flipped.
  do {
    c = 0;
    // Loop over the faces.
    for (FaceIterator face = mesh->getFacesBeginning(); 
	 face != mesh->getFacesEnd(); ++face) {
      // Try flipping this face.
      if (flip(mesh, *face, distortionFunction)) {
	++c;
      }
    }
    count += c;
  } while (c != 0);
  return count;
}




// Flip faces for as long as the quality of the mesh is improved.
template<class DistortionFunction, 
	 int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont>
inline
int
flip(SimpMeshRed<N,2,T,_Node,_Cell,Cont>* mesh, const T maxAngle) {
  typedef SimpMeshRed<N,2,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::FaceIterator FaceIterator;

  const T minCosine = std::cos(maxAngle);

  DistortionFunction distortionFunction;
  int count = 0, c;

  // Sweep over the faces until none are flipped.
  do {
    c = 0;
    // Loop over the faces.
    for (FaceIterator face = mesh->getFacesBeginning(); 
	 face != mesh->getFacesEnd(); ++face) {
      // Try flipping this face.
      if (flip(mesh, *face, distortionFunction, minCosine)) {
	++c;
      }
    }
    count += c;
  } while (c != 0);
  return count;
}




// Flip the specified face if it improves the quality of the mesh.
template<int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
inline
bool
flip(SimpMeshRed<N,2,T,_Node,_Cell,Cont>* mesh,
     const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::Face& face, 
     DistortionFunction& distortionFunction) {
  typedef SimpMeshRed<N,2,T,_Node,_Cell,Cont> SMR;
  if (isOnBoundary<SMR>(face)) {
    return false;
  }
  if (shouldFlip(*mesh, face, distortionFunction)) {
    flip<SMR>(face);
    return true;
  }
  return false;
}




// Flip the specified face if it improves the quality of the mesh.
// Return true if the face is flipped.
template<int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
inline
bool
flip(SimpMeshRed<N,2,T,_Node,_Cell,Cont>* mesh,
     const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::Face& face, 
     DistortionFunction& distortionFunction, const T minCosine) {
  typedef SimpMeshRed<N,2,T,_Node,_Cell,Cont> SMR;
  if (isOnBoundary<SMR>(face)) {
    return false;
  }
  if (shouldFlip(*mesh, face, distortionFunction, minCosine)) {
    flip<SMR>(face);
    return true;
  }
  return false;
}




// Return true if flipping the specified interior face will improve the quality of the mesh.
template<int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
inline
bool
shouldFlip(const SimpMeshRed<N,2,T,_Node,_Cell,Cont>& mesh,
	   const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::Face& face,
	   DistortionFunction& distortionFunction) {
  typedef SimpMeshRed<N,2,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::Simplex Simplex;
  typedef typename SMR::CellIterator CellIterator;

  const int M = 2;
  
  Simplex s;
  // The current distortion and the flipped distortion.
  T current, flipped;

  // The first simplex.
  CellIterator a = face.first;
  // The node index opposite the face in the first simplex.   
  const int i = face.second;
  // The second simplex.
  CellIterator b = a->getNeighbor(i);
  // The node index opposite the face in the second simplex.
  const int j = a->getMirrorIndex(i);

  //
  // Assess the current max distortion.
  //

  mesh.getSimplex(a, &s);
  distortionFunction.setFunction(s);
  current = distortionFunction();

  mesh.getSimplex(b, &s);
  distortionFunction.setFunction(s);
  current = std::max(current, distortionFunction());

  //
  // Assess the flipped max distortion.
  //

  s[0] = a->getNode(i)->getVertex();
  s[1] = a->getNode((i+1)%(M+1))->getVertex();
  s[2] = b->getNode(j)->getVertex();
  distortionFunction.setFunction(s);
  flipped = distortionFunction();

  s[0] = b->getNode(j)->getVertex();
  s[1] = b->getNode((j+1)%(M+1))->getVertex();
  s[2] = a->getNode(i)->getVertex();
  distortionFunction.setFunction(s);
  flipped = std::max(flipped, distortionFunction());

  // If the max flipped distortion is less than the current max distortion.
  if (flipped < current) {
    // We should flip the face.
    return true;
  }
  // We should not flip the face.
  return false;
}




// Return true if flipping the specified interior face will improve the quality of the mesh.
template<int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
inline
bool
shouldFlip(const SimpMeshRed<N,2,T,_Node,_Cell,Cont>& mesh,
	   const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::Face& face,
	   DistortionFunction& distortionFunction, const T minCosine) {
  typedef SimpMeshRed<N,2,T,_Node,_Cell,Cont> SMR;
  typedef typename SMR::CellConstIterator CellConstIterator;
  typedef typename SMR::Vertex Vertex;
  
  const int M = 2;

  Vertex n1, n2;

  // The first incident cell.
  const CellConstIterator a = face.first;
  // The node index opposite the face in the first simplex.   
  const int i = face.second;
  // Compute the normal to the first face.
  computeCellNormal<SMR>(a, &n1);

  // The second simplex.
  const CellConstIterator b = a->getNeighbor(i);
  // The node index opposite the face in the second simplex.
  const int j = a->getMirrorIndex(i);
  // Compute the normal to the second face.
  computeCellNormal<SMR>(b, &n2);

  // If the angle is too sharp, return false.
  if (computeDotProduct(n1, n2) < minCosine) {
    return false;
  }

  //  
  // Check the angle between the faces if we flip the edge.
  //

  Vertex u = a->getNode((i+1)%(M+1))->getVertex();;
  u -= a->getNode(i)->getVertex();
  normalize(&u);
  Vertex v = b->getNode(j)->getVertex();
  v -= a->getNode(i)->getVertex();
  normalize(&v);
  computeCrossProduct(u, v, &n1);
  // Check if the vertices of the flipped face are nearly co-linear.
  T mag = computeMagnitude(n1);
  if (mag < std::sqrt(std::numeric_limits<T>::epsilon())) {
    return false;
  }
  n1 /= mag;

  u = b->getNode((j+1)%(M+1))->getVertex();
  u -= b->getNode(j)->getVertex();
  normalize(&u);
  v = a->getNode(i)->getVertex();
  v -= b->getNode(j)->getVertex();
  normalize(&v);
  computeCrossProduct(u, v, &n2);
  // Check if the vertices of the flipped face are nearly co-linear.
  mag = computeMagnitude(n2);
  if (mag < std::sqrt(std::numeric_limits<T>::epsilon())) {
    return false;
  }
  n2 /= mag;

  // If the angle is too sharp, return false.
  if (computeDotProduct(n1, n2) < minCosine) {
    return false;
  }

  // If the angle are not too sharp, see if flipping the edge would 
  // improve the quality.
  return shouldFlip(mesh, face, distortionFunction);
}




// Flip the face between \c ch and \c ch->getNeighbor(i).
template<typename SMR>
inline
void
flip(const typename SMR::CellIterator cell, const int i) {
  typedef typename SMR::Cell Cell;
  typedef typename SMR::CellIterator CellIterator;

  const int M = 2;

  Cell a, b;

  // The neighbor.
  CellIterator nh = cell->getNeighbor(i);
  // The neighbor should not be null.
  assert(nh != CellIterator(0));

  // Copy the old cells.
  a = *cell;
  b = *nh;

  //
  // Remove the old node-cell incidences.
  //
  cell->getNode(0)->removeCell(cell);
  cell->getNode(1)->removeCell(cell);
  cell->getNode(2)->removeCell(cell);
  nh->getNode(0)->removeCell(nh);
  nh->getNode(1)->removeCell(nh);
  nh->getNode(2)->removeCell(nh);
    
  //
  // Build the new cells.
  //

  // Set the vertices.
  const int j = cell->getMirrorIndex(i);
  cell->setNode(0, a.getNode(i));
  cell->setNode(1, a.getNode((i+1)%(M+1)));
  cell->setNode(2, b.getNode(j));
  nh->setNode(0, b.getNode(j));
  nh->setNode(1, b.getNode((j+1)%(M+1)));
  nh->setNode(2, a.getNode(i));

  // Set the neigbors of the two new cells.
  cell->setNeighbor(0, b.getNeighbor((j+1)%(M+1)));
  cell->setNeighbor(1, nh);
  cell->setNeighbor(2, a.getNeighbor((i+2)%(M+1)));
  nh->setNeighbor(0, a.getNeighbor((i+1)%(M+1)));
  nh->setNeighbor(1, cell);
  nh->setNeighbor(2, b.getNeighbor((j+2)%(M+1)));

  //
  // Fix the adjacencies for the neighbors.
  //

  int k = (i+1)%(M+1);
  CellIterator c = a.getNeighbor(k);
  if (c != CellIterator(0)) {
    c->setNeighbor(a.getMirrorIndex(k), nh);
  }

  k = (i+2)%(M+1);
  c = a.getNeighbor(k);
  if (c != CellIterator(0)) {
    c->setNeighbor(a.getMirrorIndex(k), cell);
  }

  k = (j+1)%(M+1);
  c = b.getNeighbor(k);
  if (c != CellIterator(0)) {
    c->setNeighbor(b.getMirrorIndex(k), cell);
  }

  k = (j+2)%(M+1);
  c = b.getNeighbor(k);
  if (c != CellIterator(0)) {
    c->setNeighbor(b.getMirrorIndex(k), nh);
  }

  //
  // Fix the incidences for the vertices.
  //
    
  cell->getNode(0)->insertCell(cell);
  cell->getNode(1)->insertCell(cell);
  cell->getNode(2)->insertCell(cell);
  nh->getNode(0)->insertCell(nh);
  nh->getNode(1)->insertCell(nh);
  nh->getNode(2)->insertCell(nh);
}

END_NAMESPACE_GEOM

// End of file.
