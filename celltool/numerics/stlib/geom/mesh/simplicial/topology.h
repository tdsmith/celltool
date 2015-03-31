// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/topology.h
  \brief Topological functions.
*/

#if !defined(__geom_mesh_simplicial_topology_h__)
#define __geom_mesh_simplicial_topology_h__

#include "SimpMeshRed.h"

BEGIN_NAMESPACE_GEOM

//! Return true if the face is on the boundary.
/*!
  \relates SimpMeshRed
*/
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::ConstFace& f) {
  return f.first->isFaceOnBoundary(f.second);
}


//! Return true if the face is on the boundary.
/*!
  \relates SimpMeshRed
*/
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::Face& f) {
  return f.first->isFaceOnBoundary(f.second);
}


//! Return true if the face is on the boundary.
/*!
  \relates SimpMeshRed
*/
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::FaceConstIterator& f) {
  return isOnBoundary<SMR>(*f);
}


//! Return true if the face is on the boundary.
/*!
  \relates SimpMeshRed
*/
template<class SMR>
inline
bool
isOnBoundary(const typename SMR::FaceIterator& f) {
  return isOnBoundary<SMR>(*f);
}


//! Return true if they are mirror nodes.
/*!
  \relates SimpMeshRed

  They are mirror nodes if:
  - They are opposite a common face.
  - One is on the boundary and one is in the interior.  There is a boundary
  face opposite the interior node that is not incident to the boundary node.

  This function is used to determine if an edge may be collapsed.  If
  not for the second condition, it would be possible for collapsing an
  edge to create a sliver on the boundary.  If a tetrahedran has a
  face on the boundary and an edge incident to the remaining node is
  collapsed, that node could be moved to the boundary.  This creates a
  tetrahedron with four nodes on the boundary.  (There is an analogous
  case in 2-D.)
*/
template<class SMR>
bool
areMirrorNodes(typename SMR::NodeConstIterator x, 
	       typename SMR::NodeConstIterator y);


//! Return true if the nodes are incident to a common cell.
template<class SMR>
bool
doNodesShareACell(typename SMR::NodeConstIterator x, 
		  typename SMR::NodeConstIterator y);


//! Determine the cells incident to the edge.
/*!
  \relates SimpMeshRed

  \pre SMR::M == 3.
  \note SMR must be specified explicitly as a template parameter.
*/
template<typename SMR, typename CellIteratorOutputIterator>
void
determineCellsIncidentToEdge(typename SMR::CellIterator c, 
			     const int i, const int j,
			     CellIteratorOutputIterator output);


//! Determine the nodes in the link of the specified node.
/*!
  \relates SimpMeshRed

  \note SMR must be specified explicitly as a template parameter.
*/
template<typename SMR, typename NodeIteratorInsertIterator>
void
determineNodesInLink(typename SMR::NodeIterator node, 
		     NodeIteratorInsertIterator nodesInLink);


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_topology_ipp__
#include "topology.ipp"
#undef __geom_mesh_simplicial_topology_ipp__

#endif
