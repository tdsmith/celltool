// -*- C++ -*-

/*! 
  \file geom/mesh/iss/subdivide.h
  \brief Subdivision (Uniform in 1-D and 2-D.)
*/

#if !defined(__geom_mesh_iss_subdivide_h__)
#define __geom_mesh_iss_subdivide_h__

#include "IndSimpSetIncAdj.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_subdivide Subdivision for IndSimpSet
*/
//@{

//! Subdivide by splitting each simplex in half.
/*! \relates IndSimpSet */
template<int N, bool A, typename T, typename V, typename IS>
void
subdivide(const IndSimpSet<N,1,A,T,V,IS>& in, 
	  IndSimpSet<N,1,true,T,V,IS>* out);


//! Subdivide by splitting each simplex into four similar simplices.
/*! \relates IndSimpSet */
template<int N, bool A, typename T, typename V, typename IS>
void
subdivide(const IndSimpSetIncAdj<N,2,A,T,V,IS>& in, 
	  IndSimpSet<N,2,true,T,V,IS>* out);


// CONTINUE: Write a function for N-3 meshes.  Consult "Geometry and Topology
// for mesh Generation" by Edelsbrunner.

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_subdivide_ipp__
#include "subdivide.ipp"
#undef __geom_mesh_iss_subdivide_ipp__

#endif
