// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/accessors.h
  \brief Implements accessors operations for SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_accessors_h__)
#define __geom_mesh_simplicial_accessors_h__

#include "build.h"

#include "../iss/accessors.h"

BEGIN_NAMESPACE_GEOM


//! Get the incident cells of the edge.
template<typename SMR, typename CellIteratorOutputIterator>
void
getIncidentCells(const typename SMR::CellIterator cell, 
		 int i, int j,
		 CellIteratorOutputIterator out);


//! For a 2-simplex cell, a pair of nodes defines a 1-face.  Return the index of this 1-face.
template<class CellIterator, class NodeIterator>
int
getFaceIndex(const CellIterator& cell, 
	     const NodeIterator& a, const NodeIterator& b);


//! Return true if the simplices of the mesh have consistent orientations.
/*! \relates SimpMeshRed */
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
bool
isOriented(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh);


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_accessors_ipp__
#include "accessors.ipp"
#undef __geom_mesh_simplicial_accessors_ipp__

#endif
