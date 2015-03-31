// -*- C++ -*-

/*! 
  \file geom/mesh/iss/fit.h
  \brief Fit a mesh to a level-set description.
*/

#if !defined(__geom_mesh_iss_fit_h__)
#define __geom_mesh_iss_fit_h__

#include "IndSimpSetIncAdj.h"
#include "ISS_SignedDistance.h"
#include "build.h"

#include <set>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_fit Fit a mesh to a level-set description.
*/
//@{


//! Fit a mesh to a level-set description.
/*!
  \relates IndSimpSetIncAdj
*/
template<bool A, typename T, typename V, typename IS, class ISS>
void
fit(IndSimpSetIncAdj<2,1,A,T,V,IS>* mesh, 
    const ISS_SignedDistance<ISS,2>& signedDistance, 
    T deviationTangent, int numSweeps);


//! Fit the boundary of a mesh to a level-set description.
/*!
  \relates IndSimpSet
*/
template<bool A, typename T, typename V, typename IS, class ISS>
void
fit(IndSimpSetIncAdj<2,2,A,T,V,IS>* mesh, 
    const ISS_SignedDistance<ISS,2>& signedDistance, 
    T deviationTangent, int numSweeps);


//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_fit_ipp__
#include "fit.ipp"
#undef __geom_mesh_iss_fit_ipp__

#endif
