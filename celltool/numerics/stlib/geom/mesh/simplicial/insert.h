// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/insert.h
  \brief Insert cells into a SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_insert_h__)
#define __geom_mesh_simplicial_insert_h__

#include "SimpMeshRed.h"

BEGIN_NAMESPACE_GEOM


//! Insert the specified cell into the 1-D mesh.
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
typename SimpMeshRed<N,1,T,Node,Cell,Cont>::CellIterator
insertCell
(SimpMeshRed<N,1,T,Node,Cell,Cont>* mesh,
  const typename SimpMeshRed<N,1,T,Node,Cell,Cont>::NodeIterator n0, 
  const typename SimpMeshRed<N,1,T,Node,Cell,Cont>::NodeIterator n1,
  const typename SimpMeshRed<N,1,T,Node,Cell,Cont>::CellIterator c0 = 
 typename SimpMeshRed<N,1,T,Node,Cell,Cont>::CellIterator(0), 
  const typename SimpMeshRed<N,1,T,Node,Cell,Cont>::CellIterator c1 = 
 typename SimpMeshRed<N,1,T,Node,Cell,Cont>::CellIterator(0));


//! Insert the specified cell into the 2-D mesh.
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator
insertCell
(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh,
  const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::NodeIterator n0, 
  const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::NodeIterator n1,
  const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::NodeIterator n2, 
  const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator c0 = 
 typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator(0), 
  const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator c1 = 
 typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator(0), 
  const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator c2 = 
 typename SimpMeshRed<N,2,T,Node,Cell,Cont>::CellIterator(0));


//! Insert the specified cell into the 3-D mesh.
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator
insertCell
(SimpMeshRed<N,3,T,Node,Cell,Cont>* mesh,
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::NodeIterator n0, 
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::NodeIterator n1,
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::NodeIterator n2, 
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::NodeIterator n3,
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator c0 = 
 typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator(0), 
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator c1 = 
 typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator(0), 
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator c2 = 
 typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator(0), 
  const typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator c3 = 
 typename SimpMeshRed<N,3,T,Node,Cell,Cont>::CellIterator(0));


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_insert_ipp__
#include "insert.ipp"
#undef __geom_mesh_simplicial_insert_ipp__

#endif
