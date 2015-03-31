// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/inc_opt.h
  \brief Incidence optimization.
*/

#if !defined(__geom_mesh_simplicial_inc_opt_h__)
#define __geom_mesh_simplicial_inc_opt_h__

#include "SimpMeshRed.h"
#include "flip.h"
#include "geometry.h"

#include "../../../ads/algorithm/min_max.h"

#include "../../../numerical/constants.h"

BEGIN_NAMESPACE_GEOM

//! Modify the topology to optimize the cell-node incidences.
/*!
  \param mesh The simplicial mesh.
  \param norm The norm used in incidence optimization.  May be 0, 1, or 2.
  \param numSweeps The number of sweeps over the faces.  By default, the 
  number of sweeps is not limited.

  \return the number of edges flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
int
incidenceOptimize(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh,
		  int norm,
		  int numSweeps = 0);
  
END_NAMESPACE_GEOM

//#define __geom_mesh_simplicial_inc_opt2_ipp__
//#include "inc_opt2.ipp"
//#undef __geom_mesh_simplicial_inc_opt2_ipp__

//#define __geom_mesh_simplicial_inc_opt3_ipp__
//#include "inc_opt3.ipp"
//#undef __geom_mesh_simplicial_inc_opt3_ipp__

#define __geom_mesh_simplicial_inc_opt_ipp__
#include "inc_opt.ipp"
#undef __geom_mesh_simplicial_inc_opt_ipp__

#endif
