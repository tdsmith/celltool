// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/valid.h
  \brief Functions to valid edges in a SimpMeshRed<2,2>.
*/

#if !defined(__geom_mesh_simplicial_valid_h__)
#define __geom_mesh_simplicial_valid_h__

#include "SimpMeshRed.h"

#include "../simplex/SimplexJac.h"

#include "ads/algorithm/unique.h"

BEGIN_NAMESPACE_GEOM

//! Return true if the mesh is valid.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
bool
isValid(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh);

END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_valid_ipp__
#include "valid.ipp"
#undef __geom_mesh_simplicial_valid_ipp__

#endif
