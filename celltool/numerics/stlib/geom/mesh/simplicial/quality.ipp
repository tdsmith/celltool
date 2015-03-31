// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_quality_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Calculate the adjacency counts for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
countAdjacencies(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		 ads::FixedArray<M+2,int>* counts) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::CellConstIterator CellConstIterator;

  *counts = 0;
  // For each cell.
  CellConstIterator i = mesh.getCellsBeginning();
  const CellConstIterator iEnd = mesh.getCellsEnd();
  for (; i != iEnd; ++i) {
    ++(*counts)[i->getNumberOfNeighbors()];
  }
}



//! Calculate edge length statistics.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
computeEdgeLengthStatistics(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			    T* minLength, T* maxLength, T* meanLength) {
  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::EdgeConstIterator EdgeConstIterator;

  *minLength = std::numeric_limits<T>::max();
  *maxLength = 0;
  *meanLength = 0;

  int num = 0;
  T d;
  for (EdgeConstIterator i = mesh.getEdgesBeginning(); 
       i != mesh.getEdgesEnd(); ++i, ++num) {
    d = geom::computeDistance(i->first->getNode(i->second)->getVertex(), 
			      i->first->getNode(i->third)->getVertex());
    if (d < *minLength) {
      *minLength = d;
    }
    if (d > *maxLength) {
      *maxLength = d;
    }
    *meanLength += d;
  }

  if (num != 0) {
    *meanLength /= num;
  }
}

END_NAMESPACE_GEOM

// End of file.
