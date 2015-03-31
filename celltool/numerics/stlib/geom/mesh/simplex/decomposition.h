// -*- C++ -*-

/*! 
  \file decomposition.h
  \brief Functions for decomposing the Jacobian.
*/

#if !defined(__geom_decomposition_h__)
#define __geom_decomposition_h__

#include "../../../ads/tensor/SquareMatrix.h"

#if defined(DEBUG_geom) && !defined(DEBUG_decomposition)
#define DEBUG_decomposition
#endif

BEGIN_NAMESPACE_GEOM


//! Decompose the jacobian into orientation * skew * aspectRatio.
template<typename T>
void
decompose(const ads::SquareMatrix<2,T>& jacobian, 
	  ads::SquareMatrix<2,T>* orientation,
	  ads::SquareMatrix<2,T>* skew,
	  ads::SquareMatrix<2,T>* aspectRatio);


//! Decompose the jacobian into orientation * skew * aspectRatio.
template<typename T>
void
decompose(const ads::SquareMatrix<3,T>& jacobian, 
	  ads::SquareMatrix<3,T>* orientation,
	  ads::SquareMatrix<3,T>* skew,
	  ads::SquareMatrix<3,T>* aspectRatio);


END_NAMESPACE_GEOM

#define __geom_decomposition_ipp__
#include "decomposition.ipp"
#undef __geom_decomposition_ipp__

#endif
