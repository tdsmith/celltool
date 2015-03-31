// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_build_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Make an IndSimpSet from a SimpMeshRed.
/*!
  \c ISSV is the Indexed Simplex Set Vertex type.
  \c ISSIS is the Indexed Simplex Set Indexed Simplex type.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename ISSV, typename ISSIS>
void
buildIndSimpSetFromSimpMeshRed(const SimpMeshRed<N,M,T,Node,Cell,Cont>& smr,
			       IndSimpSet<N,M,true,T,ISSV,ISSIS>* iss) {
  typedef IndSimpSet<N,M,true,T,ISSV,ISSIS> ISS;
  typedef typename ISS::VertexIterator VertexIterator;
  typedef typename ISS::IndexedSimplexIterator IndexedSimplexIterator;

  typedef SimpMeshRed<N,M,T,Node,Cell,Cont> SMR;
  typedef typename SMR::NodeConstIterator NodeConstIterator;
  typedef typename SMR::CellConstIterator CellConstIterator;

  // Set the vertices.
  iss->getVertices().resize(smr.computeNodesSize());
  VertexIterator vertexIterator = iss->getVerticesBeginning();
  for (NodeConstIterator i = smr.getNodesBeginning(); i != smr.getNodesEnd(); 
	++i, ++vertexIterator) {
    *vertexIterator = i->getVertex();
  }

  // Set the indexed simplices.
  smr.setNodeIdentifiers();
  iss->getIndexedSimplices().resize(smr.computeCellsSize());
  IndexedSimplexIterator simplexIterator = iss->getIndexedSimplicesBeginning();
  for (CellConstIterator i = smr.getCellsBeginning(); i != smr.getCellsEnd(); 
	++i, ++simplexIterator) {
    for (int m = 0; m != M+1; ++m) {
      (*simplexIterator)[m] = i->getNode(m)->getIdentifier();
    }
  }
  
  // Update the topology.
  iss->updateTopology();
}

END_NAMESPACE_GEOM

// End of file.
