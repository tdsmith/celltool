// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/build.h
  \brief Functions to build edges in a SimpMeshRed<2,2>.
*/

#if !defined(__geom_mesh_simplicial_build_h__)
#define __geom_mesh_simplicial_build_h__

#include "SimpMeshRed.h"

BEGIN_NAMESPACE_GEOM

//! Make an indexed simplex set from the mesh.
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
			       IndSimpSet<N,M,true,T,ISSV,ISSIS>* iss);

END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_build_ipp__
#include "build.ipp"
#undef __geom_mesh_simplicial_build_ipp__

#endif
