// -*- C++ -*-

/*! 
  \file geom/mesh/iss/topology.h
  \brief Topological functions for indexed simplex sets.
*/

#if !defined(__geom_mesh_iss_topology_h__)
#define __geom_mesh_iss_topology_h__

#include "IndSimpSetIncAdj.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_topology Topology Functions for IndSimpSet
*/
//@{


//! Count the connected components of the mesh.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS>
int
countComponents(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh);


//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_topology_ipp__
#include "topology.ipp"
#undef __geom_mesh_iss_topology_ipp__

#endif
