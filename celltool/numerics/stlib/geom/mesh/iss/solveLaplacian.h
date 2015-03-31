// -*- C++ -*-

/*! 
  \file geom/mesh/iss/solveLaplacian.h
  \brief Implements Laplacian solve.
*/

#if !defined(__geom_mesh_iss_solveLaplacian_h__)
#define __geom_mesh_iss_solveLaplacian_h__

#include "laplacian.h"

#include "../../../third-party/tnt/tnt.h"
#include "../../../third-party/jama/jama_lu.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_solveLaplacian Laplacian Smoothing with a Solve
*/
//@{


//! Perform Laplacian smoothing on the interior vertices.
/*!
  \relates IndSimpSetIncAdj

  \param mesh Pointer to the simplicial mesh.

  \note This function is for testing purposes.  Do not use this function 
  for large meshes.  This makes an n x n matrix, where n is the number 
  of interior nodes.
*/
template<int N, int M, bool A, typename T, typename V, typename IS>
void
solveLaplacian(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh);


//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_solveLaplacian_ipp__
#include "solveLaplacian.ipp"
#undef __geom_mesh_iss_solveLaplacian_ipp__

#endif
