// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/topologicalOptimize.h
  \brief Functions to topologicalOptimize the cells in a SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_topologicalOptimize_h__)
#define __geom_mesh_simplicial_topologicalOptimize_h__

#include "SimpMeshRed.h"
#include "EdgeRemoval.h"
#include "FaceRemoval.h"
#include "set.h"

#include "../iss/PointsOnManifold.h"

#include "../../../ads/algorithm/skipElements.h"

BEGIN_NAMESPACE_GEOM


//! Use edge and face removal to optimize the mesh.
/*!
  \param mesh The simplicial mesh.
  \param manifold The manifold data structure.  We pass this by const 
  reference, because the topological optimization will not change the
  manifold data structure.  
  \param edgeRemovalOperations Multi-set to record the edge removal operations.
  \param faceRemovalOperations Multi-set to record the face removal operations.
  \param maximumSteps The maximum allowed number of steps.

  Use the specified metric.

  \return The number of edge and face removal operations.
*/
template<class _QualityMetric,
	 typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int SD>
int
topologicalOptimize
(SimpMeshRed<3,3,T,Node,Cell,Cont>* mesh, 
 const PointsOnManifold<3,2,SD,T>* manifold,
 std::multiset<std::pair<int,int> >* edgeRemovalOperations = 0,
 std::multiset<std::pair<int,int> >* faceRemovalOperations = 0,
 int maximumSteps = std::numeric_limits<int>::max());


  
//! Use edge and face removal to optimize the mesh.
/*!
  \param mesh The simplicial mesh.
  \param manifold The manifold data structure.
  \param edgeRemovalOperations Multi-set to record the edge removal operations.
  \param faceRemovalOperations Multi-set to record the face removal operations.
  \param maximumSteps The maximum allowed number of steps.

  Use the modified mean ratio metric.

  \return The number of edge and face removal operations.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD>
inline
int
topologicalOptimizeUsingMeanRatio
(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
 const PointsOnManifold<N,MM,SD,T>* manifold,
 std::multiset<std::pair<int,int> >* edgeRemovalOperations = 0,
 std::multiset<std::pair<int,int> >* faceRemovalOperations = 0,
 const int maximumSteps = std::numeric_limits<int>::max()) {
  return topologicalOptimize<SimplexModMeanRatio<N,T> >(mesh, manifold, 
							edgeRemovalOperations,
							faceRemovalOperations,
							maximumSteps);
}


  
//! Use edge and face removal to optimize the mesh.
/*!
  \param mesh The simplicial mesh.
  \param manifold The manifold data structure.
  \param edgeRemovalOperations Multi-set to record the edge removal operations.
  \param faceRemovalOperations Multi-set to record the face removal operations.
  \param maximumSteps The maximum allowed number of steps.

  Use the modified mean ratio metric.

  \return The number of edge and face removal operations.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 int MM, int SD>
inline
int
topologicalOptimizeUsingConditionNumber
(SimpMeshRed<N,M,T,Node,Cell,Cont>* mesh, 
 const PointsOnManifold<N,MM,SD,T>* manifold,
 std::multiset<std::pair<int,int> >* edgeRemovalOperations = 0,
 std::multiset<std::pair<int,int> >* faceRemovalOperations = 0,
 const int maximumSteps = std::numeric_limits<int>::max()) {
  return topologicalOptimize<SimplexModCondNum<N,T> >(mesh, manifold, 
						      edgeRemovalOperations,
						      faceRemovalOperations,
						      maximumSteps);
}

  
END_NAMESPACE_GEOM


#define __geom_mesh_simplicial_topologicalOptimize3_ipp__
#include "topologicalOptimize3.ipp"
#undef __geom_mesh_simplicial_topologicalOptimize3_ipp__


#endif
