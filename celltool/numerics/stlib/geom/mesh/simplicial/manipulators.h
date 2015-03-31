// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/manipulators.h
  \brief Implements operations that manipulators a SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_manipulators_h__)
#define __geom_mesh_simplicial_manipulators_h__

#include "SimpMeshRed.h"

#include "../simplex/SimplexJac.h"

BEGIN_NAMESPACE_GEOM


//! Orient each simplex so it has non-negative volume.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
orientPositive(SimpMeshRed<N,M,T,Node,Cell,Cont>* x);


//! Erase the nodes that do not have an incident cells.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
eraseUnusedNodes(SimpMeshRed<N,M,T,Node,Cell,Cont>* x);


//! Erase cells until there are none with minimum adjacencies less than specified.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
eraseCellsWithLowAdjacencies(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh,
			     int minimumAdjacencies);


//! Re-number the node and cell identifiers so they start at 0 and are contiguous.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
renumberIdentifiers(SimpMeshRed<N,M,T,Node,Cell,Cont>* x);


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_manipulators_ipp__
#include "manipulators.ipp"
#undef __geom_mesh_simplicial_manipulators_ipp__

#endif
