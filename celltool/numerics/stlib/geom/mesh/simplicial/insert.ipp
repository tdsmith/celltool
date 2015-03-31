// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_insert_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//! Insert the specified cell into the 1-D mesh.
template<int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont>
inline
typename SimpMeshRed<N,1,T,_Node,_Cell,Cont>::CellIterator
insertCell
(SimpMeshRed<N,1,T,_Node,_Cell,Cont>* mesh,
 const typename SimpMeshRed<N,1,T,_Node,_Cell,Cont>::NodeIterator n0, 
 const typename SimpMeshRed<N,1,T,_Node,_Cell,Cont>::NodeIterator n1,
 const typename SimpMeshRed<N,1,T,_Node,_Cell,Cont>::CellIterator c0, 
 const typename SimpMeshRed<N,1,T,_Node,_Cell,Cont>::CellIterator c1) {
  typedef typename SimpMeshRed<N,1,T,_Node,_Cell,Cont>::Cell Cell;
  return mesh->insertCell(Cell(n0, n1, c0, c1));
}




//! Insert the specified cell into the 2-D mesh.
template<int N, typename T,
	 template<class> class _Node,
	 template<class> class _Cell,
	 template<class,class> class Cont>
inline
typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::CellIterator
insertCell
(SimpMeshRed<N,2,T,_Node,_Cell,Cont>* mesh,
 const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::NodeIterator n0, 
 const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::NodeIterator n1,
 const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::NodeIterator n2, 
 const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::CellIterator c0, 
 const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::CellIterator c1, 
 const typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::CellIterator c2) {
  typedef typename SimpMeshRed<N,2,T,_Node,_Cell,Cont>::Cell Cell;
  return mesh->insertCell(Cell(n0, n1, n2, c0, c1, c2));
}




//! Insert the specified cell into the 3-D mesh.
template<int N, typename T,
	   template<class> class _Node,
	   template<class> class _Cell,
	   template<class,class> class Cont>
inline
typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::CellIterator
insertCell
(SimpMeshRed<N,3,T,_Node,_Cell,Cont>* mesh,
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::NodeIterator n0, 
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::NodeIterator n1,
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::NodeIterator n2, 
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::NodeIterator n3,
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::CellIterator c0, 
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::CellIterator c1, 
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::CellIterator c2, 
 const typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::CellIterator c3) {
  typedef typename SimpMeshRed<N,3,T,_Node,_Cell,Cont>::Cell Cell;
  return mesh->insertCell(Cell(n0, n1, n2, n3, c0, c1, c2, c3));
}
 

END_NAMESPACE_GEOM

// End of file.
