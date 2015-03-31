// -*- C++ -*-

/*! 
  \file geom/mesh/iss/cellAttributes.h
  \brief Implements cellAttributes measures for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_cellAttributes_h__)
#define __geom_mesh_iss_cellAttributes_h__

#include "IndSimpSetIncAdj.h"
#include "topology.h"

#include "../simplex.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_cellAttributes CellAttributes 
  These functions compute cell attributes for simplicial meshes.
*/
//@{


//----------------------------------------------------------------------------
// Mean ratio.
//----------------------------------------------------------------------------


//! Calculate the mean ratio function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
void
computeMeanRatio(VertRAIter vertices,
		 ISInIter indexedSimplicesBeginning, 
		 ISInIter indexedSimplicesEnd,
		 OutputIterator output);

//! Calculate the mean ratio function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
void
computeMeanRatio(SimpInIter simplicesBeginning, 
		 SimpInIter simplicesEnd,
		 OutputIterator output);

//! Calculate the mean ratio function for each simplex in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
void
computeMeanRatio(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 OutputIterator output);



//----------------------------------------------------------------------------
// Modified mean ratio.
//----------------------------------------------------------------------------


//! Calculate the modified mean ratio function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
void
computeModifiedMeanRatio(VertRAIter vertices,
			 ISInIter indexedSimplicesBeginning, 
			 ISInIter indexedSimplicesEnd,
			 OutputIterator output);

//! Calculate the modified mean ratio function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
void
computeModifiedMeanRatio(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd,
			 OutputIterator output);

//! Calculate the modified mean ratio function for each simplex in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
void
computeModifiedMeanRatio(const IndSimpSet<N,M,A,T,V,IS>& iss,
			 OutputIterator output);


//----------------------------------------------------------------------------
// Condition number.
//----------------------------------------------------------------------------


//! Calculate the condition number function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
void
computeConditionNumber(VertRAIter vertices,
		       ISInIter indexedSimplicesBeginning, 
		       ISInIter indexedSimplicesEnd,
		       OutputIterator output);

//! Calculate the condition number function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
void
computeConditionNumber(SimpInIter simplicesBeginning, 
		       SimpInIter simplicesEnd,
		       OutputIterator output);

//! Calculate the condition number function for each simplex in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
void
computeConditionNumber(const IndSimpSet<N,M,A,T,V,IS>& iss,
		       OutputIterator output);



//----------------------------------------------------------------------------
// Modified condition number.
//----------------------------------------------------------------------------


//! Calculate the modified condition number function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
void
computeModifiedConditionNumber(VertRAIter vertices,
			       ISInIter indexedSimplicesBeginning, 
			       ISInIter indexedSimplicesEnd,
			       OutputIterator output);

//! Calculate the modified condition number function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
void
computeModifiedConditionNumber(SimpInIter simplicesBeginning, 
			       SimpInIter simplicesEnd,
			       OutputIterator output);

//! Calculate the modified condition number function for each simplex in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
void
computeModifiedConditionNumber(const IndSimpSet<N,M,A,T,V,IS>& iss,
			       OutputIterator output);


//----------------------------------------------------------------------------
// Content.
//----------------------------------------------------------------------------


//! Calculate the content for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
void
computeContent(VertRAIter vertices,
	       ISInIter indexedSimplicesBeginning, 
	       ISInIter indexedSimplicesEnd,
	       OutputIterator output);

//! Calculate the content for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
void
computeContent(SimpInIter simplicesBeginning, 
	       SimpInIter simplicesEnd,
	       OutputIterator output);

//! Calculate the content for each simplex in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
void
computeContent(const IndSimpSet<N,M,A,T,V,IS>& iss,
	       OutputIterator output);



//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_cellAttributes_ipp__
#include "cellAttributes.ipp"
#undef __geom_mesh_iss_cellAttributes_ipp__

#endif
