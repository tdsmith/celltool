// -*- C++ -*-

/*! 
  \file geom/mesh/iss/set.h
  \brief Implements set operations for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_set_h__)
#define __geom_mesh_iss_set_h__

#include "IndSimpSetIncAdj.h"
#include "ISS_SimplexQuery.h"

#include <set>
#include <stack>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_set Set Operations 
  These functions build sets of vertices or simplices.
*/
//@{

//! Determine the vertices that are inside the object.
/*!
  \relates IndSimpSet

  \param mesh is the simplicial mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param indexIterator is an output iterator on vertex indices.
    
  \c LSF is the level set functor.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 class LSF, typename IntOutIter>
void
determineVerticesInside(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			const LSF& f, IntOutIter indexIterator);


//! Determine the simplices that are inside the object.
/*!
  \relates IndSimpSet

  \param mesh is the simplicial mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param indexIterator is an output iterator on simplex indices.
    
  \c LSF is the level set functor.  A simplex is determined to be inside the
  object if its centroid is inside.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 class LSF, typename IntOutIter>
void
determineSimplicesInside(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			 const LSF& f, IntOutIter indexIterator);


//! Determine the simplices which satisfy the specified condition.
/*!
  \relates IndSimpSet

  \param mesh is the simplicial mesh.
  \param f is the boolean function on simplices.
  \param indexIterator is an output iterator on simplex indices.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	  class UnaryFunction, typename IntOutIter>
void
determineSimplicesThatSatisfyCondition(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
				       const UnaryFunction& f, 
				       IntOutIter indexIterator);


//! Determine the simplices whose bounding boxes overlap the domain.
/*!
  \relates IndSimpSet

  \param mesh is the simplicial mesh.
  \param domain A Cartesian rectiliniar domain.
  \param indexIterator is an output iterator on simplex indices.

  Determine the set of simplices whose bounding boxes overlap the domain.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
void
determineOverlappingSimplices(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			      const BBox<N,T>& domain, 
			      IntOutIter indexIterator);


//! Add the simplices with at least the specified number of adjacencies to the set.
/*!
  \relates IndSimpSetIncAdj

  \param mesh is the simplicial mesh.
  \param minRequiredAdjacencies is the minimim required number of adjacencies.
  \param indexIterator is an output iterator for the simplex indices
  that have at least specified number of adjacencies.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
void
determineSimplicesWithRequiredAdjacencies
(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
 int minRequiredAdjacencies, IntOutIter indexIterator);


//! Add the interior vertex indices to the set.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
void
determineInteriorVertices(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
			  IntOutIter indexIterator);


//! Add the boundary vertex indices to the set.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
void
determineBoundaryVertices(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
			  IntOutIter indexIterator);


//! Add the vertices which are incident to the simplices.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntInIter, typename IntOutIter>
void
determineIncidentVertices(const IndSimpSet<N,M,A,T,V,IS>& mesh, 
			  IntInIter simplexIndicesBeginning, 
			  IntInIter simplexIndicesEnd,
			  IntOutIter vertexIndicesIterator);


//! Add the simplices (simplex indices) which are incident to the vertices.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntInIter, typename IntOutIter>
void
determineIncidentSimplices(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh, 
			   IntInIter vertexIndicesBeginning, 
			   IntInIter vertexIndicesEnd,
			   IntOutIter simplexIndicesIterator);


//! Make the complement set of indices.
/*!
  \c begin and \c end form a range of indices in sorted order.
 */
template<typename IntForIter, typename IntOutIter>
void
determineComplementSetOfIndices(const int upperBound,
				IntForIter beginning, IntForIter end,
				IntOutIter indexIterator);


//! Add the simplices in the component with the n_th simplex to the set.
/*! \relates IndSimpSetIncAdj */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutIter>
void
determineSimplicesInComponent(const IndSimpSetIncAdj<N,M,A,T,V,IS>& mesh,
			      int index, IntOutIter indexIterator);


//! Separate the connected components of the mesh.
/*! 
  \relates IndSimpSetIncAdj 

  \param mesh The input/output mesh.  The simplices will be rearranged
  so that the simplices in a connected component are in a contiguous index
  range.
  \param delimiterIterator The \c delimiters define the components.  
  Its values are the semi-open index ranges.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutputIterator>
void
separateComponents(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
		   IntOutputIterator delimiterIterator);


//! Separate the connected components of the mesh.
/*! 
  \relates IndSimpSetIncAdj 

  \param mesh The input/output mesh.  The simplices will be rearranged
  so that the simplices in a connected component are in a contiguous index
  range.
  \param delimiterIterator The \c delimiters define the components.  
  Its values are the semi-open index ranges.
  \param permutationIterator The permutation of the simplices will be 
  output to this iterator.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntOutputIterator1, typename IntOutputIterator2>
void
separateComponents(IndSimpSetIncAdj<N,M,A,T,V,IS>* mesh,
		   IntOutputIterator1 delimiterIterator,
		   IntOutputIterator2 permutationIterator);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_set_ipp__
#include "set.ipp"
#undef __geom_mesh_iss_set_ipp__

#endif
