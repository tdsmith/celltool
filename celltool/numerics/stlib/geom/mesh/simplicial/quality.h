// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/quality.h
  \brief Implements quality measures for SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_quality_h__)
#define __geom_mesh_simplicial_quality_h__

#include "SimpMeshRed.h"

#include "../iss/quality.h"

BEGIN_NAMESPACE_GEOM

//! Calculate the adjacency counts for the simplices in the mesh.
/*!
  Each simplex has between 0 and M+1 (inclusive) adjacent simplices.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
countAdjacencies(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		 ads::FixedArray<M+2,int>* counts);


//! Return the total content of the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
T
computeContent(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh) {
  return computeContent<M,T>(mesh.getSimplicesBeginning(), 
			     mesh.getSimplicesEnd());
}


//! Calculate content (hypervolume) statistics for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
computeContentStatistics(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent) {
  computeContentStatistics<M,T>(mesh.getSimplicesBeginning(), 
				mesh.getSimplicesEnd(),
				minContent, maxContent, meanContent);
}


//! Calculate edge length statistics.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
computeEdgeLengthStatistics(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			    T* minLength, T* maxLength, T* meanLength);


//! Print edge length statistics.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
printEdgeLengthStatistics(std::ostream& out,
			  const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh)
{
  T minLength, maxLength, meanLength;
  computeEdgeLengthStatistics(mesh, &minLength, &maxLength, &meanLength);
  out << "edge lengths:"
      << " min = " << minLength
      << " max = " << maxLength
      << " mean = " << meanLength << "\n";
}


//! Calculate determinant statistics for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
computeDeterminantStatistics(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			     T* minDeterminant, 
			     T* maxDeterminant,
			     T* meanDeterminant) {
  computeDeterminantStatistics<M,T>(mesh.getSimplicesBeginning(), 
				    mesh.getSimplicesEnd(),
				    minDeterminant, 
				    maxDeterminant,
				    meanDeterminant);
}



//! Calculate modified mean ratio function statistics for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
computeModifiedMeanRatioStatistics
(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
 T* minModifiedMeanRatio, 
 T* maxModifiedMeanRatio,
 T* meanModifiedMeanRatio) {
  computeModifiedMeanRatioStatistics<M,T>(mesh.getSimplicesBeginning(), 
					  mesh.getSimplicesEnd(),
					  minModifiedMeanRatio, 
					  maxModifiedMeanRatio,
					  meanModifiedMeanRatio);
}



//! Calculate modified condition number function statistics for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
computeModifiedConditionNumberStatistics
(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
 T* minModifiedConditionNumber, 
 T* maxModifiedConditionNumber,
 T* meanModifiedConditionNumber) {
  computeModifiedMeanRatioStatistics<M,T>(mesh.getSimplicesBeginning(), 
					  mesh.getSimplicesEnd(),
					  minModifiedConditionNumber, 
					  maxModifiedConditionNumber,
					  meanModifiedConditionNumber);
}



//! Calculate quality statistics for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
computeQualityStatistics(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent,
			 T* minDeterminant, 
			 T* maxDeterminant,
			 T* meanDeterminant,
			 T* minModifiedMeanRatio,
			 T* maxModifiedMeanRatio,
			 T* meanModifiedMeanRatio,
			 T* minModifiedConditionNumber, 
			 T* maxModifiedConditionNumber,
			 T* meanModifiedConditionNumber) {
  computeQualityStatistics<M,T>(mesh.getSimplicesBeginning(), 
				mesh.getSimplicesEnd(),
				minContent, 
				maxContent,
				meanContent,
				minDeterminant, 
				maxDeterminant,
				meanDeterminant,
				minModifiedMeanRatio,
				maxModifiedMeanRatio,
				meanModifiedMeanRatio,
				minModifiedConditionNumber, 
				maxModifiedConditionNumber,
				meanModifiedConditionNumber);
}



//! Print quality statistics for the simplices in the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
void
printQualityStatistics(std::ostream& out,
		       const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh) {
  out << "Number of vertices = " << mesh.computeNodesSize() << "\n";
  printQualityStatistics<N,M,T>(out, mesh.getSimplicesBeginning(), 
				mesh.getSimplicesEnd());
}


//! Print quality statistics for the simplices in the mesh.
// CONTINUE
/*
template<int N, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont>
inline
void
printQualityStatistics(std::ostream& out,
	       const SimpMeshRed<N,N-1,T,Node,Cell,Cont>& mesh)
{
  typedef Simplex<N-1,ads::FixedArray<N-1,T>,T> Simp;
  std::vector<Simp> simplices;
  simplices.reserve(mesh.cells_size());
  project_and_get_simplices(mesh, std::back_inserter(simplices));
  printQualityStatistics<N-1,T>(out, simplices.begin(), simplices.end());
}
*/

END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_quality_ipp__
#include "quality.ipp"
#undef __geom_mesh_simplicial_quality_ipp__

#endif
