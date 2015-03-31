// -*- C++ -*-

/*! 
  \file geom/mesh/simplex/geometry.h
  \brief Geometric functions for simplices.
*/

#if !defined(__geom_mesh_simplex_geometry_h__)
#define __geom_mesh_simplex_geometry_h__

#include "Simplex.h"

#include "../../kernel/Plane.h"

#include "../../../numerical/constants.h"

BEGIN_NAMESPACE_GEOM

//---------------------------------------------------------------------------
// Angles
//---------------------------------------------------------------------------

//! The dihedral angle between two faces.
/*!
  \relates Simplex
*/
template<typename T>
T
computeAngle(const Simplex<3,ads::FixedArray<3,T>,T>& s, int a, int b);


//! The solid angle at a vertex.
/*!
  \relates Simplex
*/
template<typename T>
T
computeAngle(const Simplex<3,ads::FixedArray<3,T>,T>& s, int n);


//---------------------------------------------------------------------------
// Project
//---------------------------------------------------------------------------


//! Project the simplex to a lower dimension.
/*!
  \relates Simplex
*/
template<typename T>
void
projectToLowerDimension(const Simplex<1,ads::FixedArray<2,T>,T>& s,
			Simplex<1,ads::FixedArray<1,T>,T>* t);


//! Project the simplex to a lower dimension.
/*!
  \relates Simplex
*/
template<typename T>
void
projectToLowerDimension(const Simplex<1,ads::FixedArray<3,T>,T>& s,
			Simplex<1,ads::FixedArray<1,T>,T>* t);


//! Project the simplex to a lower dimension.
/*!
  \relates Simplex
*/
template<typename T>
void
projectToLowerDimension(const Simplex<2,ads::FixedArray<3,T>,T>& s,
			Simplex<2,ads::FixedArray<2,T>,T>* t);


END_NAMESPACE_GEOM

#define __geom_mesh_simplex_geometry_ipp__
#include "geometry.ipp"
#undef __geom_mesh_simplex_geometry_ipp__

#endif
