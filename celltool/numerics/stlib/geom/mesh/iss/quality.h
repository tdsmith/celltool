// -*- C++ -*-

/*! 
  \file geom/mesh/iss/quality.h
  \brief Implements quality measures for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_quality_h__)
#define __geom_mesh_iss_quality_h__

#include "IndSimpSetIncAdj.h"
#include "topology.h"

#include "../simplex/SimplexModMeanRatio.h"
#include "../simplex/SimplexModCondNum.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_quality Quality 
  These function measure quality statistics for simplicial meshes.
*/
//@{

//! Calculate the adjacency counts for the simplices in the mesh.
/*!
  \relates IndSimpSet
  Each simplex has between 0 and M+1 (inclusive) adjacent simplices.
*/
template<int N, int M, bool A, typename T, typename V, typename IS>
void
countAdjacencies(const IndSimpSetIncAdj<N,M,A,T,V,IS>& iss,
		 ads::FixedArray<M+2,int>* counts);

// CONTINUE: edge length statistics.
// Add the edge length to the functions that print quality.



//! Compute edge length statistics.
/*! 
  With a bucket of simplices, one can compute the minimum and maximum, but
  not the mean.
*/
template<int M, typename SimpInIter, typename T>
void
computeEdgeLengthStatistics(SimpInIter simplicesBeginning, 
			    SimpInIter simplicesEnd,
			    T* minimumLength,
			    T* maximumLength);



//! Compute edge length statistics.
template<int M, typename VertRAIter, typename ISInIter, typename T>
void
computeEdgeLengthStatistics(VertRAIter vertices,
			    ISInIter indexedSimplicesBeginning, 
			    ISInIter indexedSimplicesEnd,
			    T* minimumLength,
			    T* maximumLength);



//! Compute edge length statistics.
template<int N, bool A, typename T, typename V, typename IS>
void
computeEdgeLengthStatistics(const IndSimpSetIncAdj<N,2,A,T,V,IS>& mesh,
			    T* minimumLength,
			    T* maximumLength,
			    T* meanLength);


//! Return the minimum edge length.
template<int M, typename T, typename SimpInIter>
T
computeMinimumEdgeLength(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd);



//! Return the minimum edge length.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
T
computeMinimumEdgeLength(const IndSimpSet<N,M,A,T,V,IS>& mesh) {
  return computeMinimumEdgeLength<M,T>(mesh.getSimplicesBeginning(), 
				       mesh.getSimplicesEnd());
}


//! Return the maximum edge length.
template<int M, typename T, typename SimpInIter>
T
computeMaximumEdgeLength(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd);


//! Return the maximum edge length.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
T
computeMaximumEdgeLength(const IndSimpSet<N,M,A,T,V,IS>& mesh) {
  return computeMaximumEdgeLength<M,T>(mesh.getSimplicesBeginning(), 
				   mesh.getSimplicesEnd());
}

//! Compute edge length statistics.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
T
computeMeanEdgeLength(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh) {
  T minimumLength, maximumLength, meanLength;
  computeEdgeLengthStatistics(mesh, &minimumLength, &maximumLength, 
			      &meanLength);
  return meanLength;
}


//! Return the total content of the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
T
computeContent(VertRAIter vertices,
	       ISInIter indexedSimplicesBeginning, ISInIter indexedSimplicesEnd);

//! Return the total content of the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
T
computeContent(SimpInIter simplicesBeginning, SimpInIter simplicesEnd);

//! Return the total content of the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
T
computeContent(const IndSimpSet<N,M,A,T,V,IS>& iss);


//! Calculate content (hypervolume) statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
void
computeContentStatistics(VertRAIter vertices,
			 ISInIter indexedSimplicesBeginning, 
			 ISInIter indexedSimplicesEnd,
			 T* minimumContent, 
			 T* maximumContent,
			 T* meanContent);


//! Calculate content (hypervolume) statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
void
computeContentStatistics(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd,
			 T* minimumContent,
			 T* maximumContent,
			 T* meanContent);


//! Calculate content (hypervolume) statistics for the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
computeContentStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			 T* minimumContent, 
			 T* maximumContent,
			 T* meanContent);



//! Calculate determinant statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
void
computeDeterminantStatistics(VertRAIter vertices,
			     ISInIter indexedSimplicesBeginning, 
			     ISInIter indexedSimplicesEnd,
			     T* minimumDeterminant, 
			     T* maximumDeterminant,
			     T* meanDeterminant);

//! Calculate determinant statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
void
computeDeterminantStatistics(SimpInIter simplicesBeginning, 
			     SimpInIter simplicesEnd,
			     T* minimumDeterminant, 
			     T* maximumDeterminant,
			     T* meanDeterminant);

//! Calculate determinant statistics for the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
computeDeterminantStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			     T* minimumDeterminant, 
			     T* maximumDeterminant,
			     T* meanDeterminant);




//! Calculate modified mean ratio function statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
void
computeModifiedMeanRatioStatistics(VertRAIter vertices,
			      ISInIter indexedSimplicesBeginning, 
			      ISInIter indexedSimplicesEnd,
			      T* minimumModMeanRatio, 
			      T* maximumModMeanRatio,
			      T* meanModMeanRatio);

//! Calculate modified mean ratio function statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
void
computeModifiedMeanRatioStatistics(SimpInIter simplicesBeginning, 
			      SimpInIter simplicesEnd,
			      T* minimumModMeanRatio, 
			      T* maximumModMeanRatio,
			      T* meanModMeanRatio);

//! Calculate modified mean ratio function statistics for the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
computeModifiedMeanRatioStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			      T* minimumModMeanRatio, 
			      T* maximumModMeanRatio,
			      T* meanModMeanRatio);


//! Calculate modified condition number function statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
void
computeModifiedConditionNumberStatistics(VertRAIter vertices,
			    ISInIter indexedSimplicesBeginning, 
			    ISInIter indexedSimplicesEnd,
			    T* minimumModCondNum, 
			    T* maximumModCondNum,
			    T* meanModCondNum);

//! Calculate modified condition number function statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
void
computeModifiedConditionNumberStatistics(SimpInIter simplicesBeginning, 
			    SimpInIter simplicesEnd,
			    T* minimumModCondNum, 
			    T* maximumModCondNum,
			    T* meanModCondNum);

//! Calculate modified condition number function statistics for the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
computeModifiedConditionNumberStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			    T* minimumModCondNum, 
			    T* maximumModCondNum,
			    T* meanModCondNum);


//! Calculate quality statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
void
computeQualityStatistics(VertRAIter vertices,
			 ISInIter indexedSimplicesBeginning, 
			 ISInIter indexedSimplicesEnd,
			 T* minimumContent, 
			 T* maximumContent,
			 T* meanContent,
			 T* minimumDeterminant, 
			 T* maximumDeterminant,
			 T* meanDeterminant,
			 T* minimumModMeanRatio,
			 T* maximumModMeanRatio,
			 T* meanModMeanRatio,
			 T* minimumModCondNum, 
			 T* maximumModCondNum,
			 T* meanModCondNum);

//! Calculate quality statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
void
computeQualityStatistics(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd,
			 T* minimumContent, 
			 T* maximumContent,
			 T* meanContent,
			 T* minimumDeterminant, 
			 T* maximumDeterminant,
			 T* meanDeterminant,
			 T* minimumModMeanRatio,
			 T* maximumModMeanRatio,
			 T* meanModMeanRatio,
			 T* minimumModCondNum, 
			 T* maximumModCondNum,
			 T* meanModCondNum);

//! Calculate quality statistics for the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
computeQualityStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			 T* minimumContent, 
			 T* maximumContent,
			 T* meanContent,
			 T* minimumDeterminant, 
			 T* maximumDeterminant,
			 T* meanDeterminant,
			 T* minimumModMeanRatio,
			 T* maximumModMeanRatio,
			 T* meanModMeanRatio,
			 T* minimumModCondNum, 
			 T* maximumModCondNum,
			 T* meanModCondNum);


//! Print quality statistics for the simplices in the mesh.
template<int N, int M, typename T, typename VertRAIter, typename ISInIter>
void
printQualityStatistics(std::ostream& out,
		       VertRAIter verticesBeginning, VertRAIter verticesEnd,
		       ISInIter indexedSimplicesBeginning, 
		       ISInIter indexedSimplicesEnd);

//! Print quality statistics for the simplices in the mesh.
template<int N, int M, typename T, typename SimpInIter>
void
printQualityStatistics(std::ostream& out,
		       SimpInIter simplicesBeginning, SimpInIter simplicesEnd);

//! Print quality statistics for the simplices in the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
printQualityStatistics(std::ostream& out,
		       const IndSimpSet<N,M,A,T,V,IS>& mesh);

//! Print information about the mesh.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
printInformation(std::ostream& out,
		 const IndSimpSet<N,M,A,T,V,IS>& mesh);

//! Print information about the mesh.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
printInformation(std::ostream& out,
		 const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_quality_ipp__
#include "quality.ipp"
#undef __geom_mesh_iss_quality_ipp__

#endif
