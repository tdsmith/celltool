// -*- C++ -*-

/*! 
  \file geom/mesh/iss/build.h
  \brief Implements builders for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_build_h__)
#define __geom_mesh_iss_build_h__

#include "IndSimpSetIncAdj.h"
#include "set.h"
#include "transform.h"

#include "../quadrilateral/QuadMesh.h"

#include "../../../ads/iterator/IndirectIterator.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_build Builders 
  These functions build indexed simplex sets from various inputs.
*/
//@{

//! Build from a quadrilateral mesh.
/*!
  \relates IndSimpSet

  \param quadMesh The input quadrilateral mesh.
  \param mesh The output simplicial mesh.

  Each quadrilateral is split to form two triangles.  In the splitting, 
  the shorter diagonal is chosen.
*/
template<int N, bool A, typename T, typename V, typename IF, typename IS>
void
buildFromQuadMesh(const QuadMesh<N,A,T,V,IF>& quadMesh,
		  IndSimpSet<N,2,true,T,V,IS>* mesh);

//! Make a mesh from a subset of vertices of a mesh.
/*!
  \relates IndSimpSet

  \param in The input mesh.
  \param verticesBeginning The beginning of the range of vertex indices.
  \param verticesEnd The end of the range of vertex indices.
  \param out The output mesh.

  \c IntForIter is an integer forward iterator.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter>
void
buildFromSubsetVertices(const IndSimpSet<N,M,A,T,V,IS>& in,
			IntForIter verticesBeginning, 
			IntForIter verticesEnd,
			IndSimpSet<N,M,true,T,V,IS>* out);
  

//! Make a new mesh from the subset of simplices.
/*!
  \relates IndSimpSet

  \param in is the input mesh.
  \param simplicesBeginning is the beginning of the range of simplex indices.
  \param simplicesEnd is the end of the range of simplex indices.
  \param out is the output mesh.

  \c IntForIter is an integer forward iterator.
*/
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename IntForIter>
void
buildFromSubsetSimplices(const IndSimpSet<N,M,A,T,V,IS>& in,
			 IntForIter simplicesBeginning, 
			 IntForIter simplicesEnd,
			 IndSimpSet<N,M,true,T,V,IS>* out);


//! Make a mesh by selecting vertices from the input mesh that are inside the object.
/*!
  \relates IndSimpSet

  \param in is the input mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param out is the output mesh.
    
  \c LSF is the level set functor.
*/
template<int N, int M, bool A, typename T, typename V, typename IS, class LSF>
void
buildFromVerticesInside(const IndSimpSet<N,M,A,T,V,IS>& in,
			const LSF& f,
			IndSimpSet<N,M,true,T,V,IS>* out);
  

//! Make a mesh by selecting simplices from the input mesh that are inside the object.
/*!
  \relates IndSimpSet

  \param in is the input mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param out is the output mesh.
    
  \c LSF is the level set functor.  A simplex is determined to be inside the
  object if its centroid is inside.
*/
template<int N, int M, bool A, typename T, typename V, typename IS, class LSF>
void
buildFromSimplicesInside(const IndSimpSet<N,M,A,T,V,IS>& in,
			 const LSF& f,
			 IndSimpSet<N,M,true,T,V,IS>* out);


//! Make a mesh that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  \param in The input mesh.
  \param out The output mesh.
  \param usedVertexIndices The vertex indices that are used in the boundary.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator>
void
buildBoundary(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
	      IndSimpSet<N,M-1,true,T,V,IFace>* out,
	      IntOutputIterator usedVertexIndices);


//! Make a mesh that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  \param in The input mesh.
  \param out The output mesh.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace>
inline
void
buildBoundary(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
	      IndSimpSet<N,M-1,true,T,V,IFace>* out) {
  buildBoundary(in, out, ads::TrivialOutputIterator());
}


//! Make a mesh that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  This function does not pack the output mesh.  That is, it does not remove
  the unused interior vertices.

  \param in The input mesh.
  \param out The output mesh.
  \param incidentSimplices The incident simplex index of each boundary face
  is recorded in this output iterator.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator>
void
buildBoundaryWithoutPacking(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
			    IndSimpSet<N,M-1,true,T,V,IFace>* out,
			    IntOutputIterator incidentSimplices);


//! Make a mesh that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  This function does not pack the output mesh.  That is, it does not remove
  the unused interior vertices.

  \param in The input mesh.
  \param out The output mesh.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace>
inline
void
buildBoundaryWithoutPacking(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
			    IndSimpSet<N,M-1,true,T,V,IFace>* out) {
  buildBoundaryWithoutPacking(in, out, ads::constructTrivialOutputIterator());
}



//! Make a mesh (separated into connected components) that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  This function does not pack the output mesh.  That is, it does not remove
  the unused interior vertices.

  \param in The input mesh.
  \param out The output mesh.
  \param delimiterIterator The \c delimiters define the components.  
  Its values are the semi-open index ranges.
  \param incidentSimplices The incident simplex index of each boundary face
  is recorded in this output iterator.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator1, typename IntOutputIterator2>
void
buildBoundaryOfComponentsWithoutPacking
(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
 IndSimpSet<N,M-1,true,T,V,IFace>* out,
 IntOutputIterator1 delimiterIterator,
 IntOutputIterator2 incidentSimplices);



//! Make a mesh (separated into connected components) that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  This function does not pack the output mesh.  That is, it does not remove
  the unused interior vertices.

  \param in The input mesh.
  \param out The output mesh.
  \param delimiterIterator The \c delimiters define the components.  
  Its values are the semi-open index ranges.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator>
inline
void
buildBoundaryOfComponentsWithoutPacking
(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
 IndSimpSet<N,M-1,true,T,V,IFace>* out,
 IntOutputIterator delimiterIterator) {
  buildBoundaryOfComponentsWithoutPacking
    (in, out, delimiterIterator, ads::constructTrivialOutputIterator());
}



//! Make a mesh (separated into connected components) that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  \param in The input mesh.
  \param out The output mesh.
  \param delimiterIterator The \c delimiters define the components.  
  Its values are the semi-open index ranges.
  \param incidentSimplices The incident simplex index of each boundary face
  is recorded in this output iterator.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator1, typename IntOutputIterator2>
inline
void
buildBoundaryOfComponents(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
			  IndSimpSet<N,M-1,true,T,V,IFace>* out,
			  IntOutputIterator1 delimiterIterator,
			  IntOutputIterator2 incidentSimplices) {
  // First do it without packing.
  buildBoundaryOfComponentsWithoutPacking(in, out, delimiterIterator,
					  incidentSimplices);
  // Pack the mesh to get rid of the interior vertices.
  pack(out);
}




//! Make a mesh (separated into connected components) that is the boundary of the input mesh.
/*!
  \relates IndSimpSet

  \param in The input mesh.
  \param out The output mesh.
  \param delimiterIterator The \c delimiters define the components.  
  Its values are the semi-open index ranges.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace,
	 typename IntOutputIterator>
inline
void
buildBoundaryOfComponents(const IndSimpSetIncAdj<N,M,A,T,V,ISimp>& in,
			  IndSimpSet<N,M-1,true,T,V,IFace>* out,
			  IntOutputIterator delimiterIterator) {
  buildBoundaryOfComponents(in, out, delimiterIterator, 
			    ads::constructTrivialOutputIterator());
}




//! Make a mesh by connecting the boundary nodes to a new center point.
/*!
  \relates IndSimpSet

  \param boundary The input boundary mesh.
  \param mesh The output solid mesh.
*/
template<int N, int M, bool A, typename T, typename V, 
	 typename ISimp, typename IFace>
void
centerPointMesh(const IndSimpSet<N,M-1,A,T,V,IFace>& boundary,
		IndSimpSet<N,M,true,T,V,ISimp>* mesh);


//! Merge a range of meshes to make a single mesh.
/*!
  \relates IndSimpSet

  \param beginning The beginning of a range of meshes.
  \param end The end of a range of meshes.
  \param out The output mesh.

  The meshes are simply concatenated.  Duplicate vertices (if any) are 
  not removed.
*/
template<int N, int M, typename T, typename V, typename IS, 
	 typename MeshInputIterator>
void
merge(MeshInputIterator beginning, MeshInputIterator end,
      IndSimpSet<N,M,true,T,V,IS>* out);


//! Merge two meshes to make a single mesh.
/*!
  \relates IndSimpSet

  \param a The first input mesh.
  \param b The second input mesh.
  \param out The output mesh.

  The meshes are simply concatenated.  Duplicate vertices (if any) are 
  not removed.
*/
template<int N, int M, bool A, typename T, typename V, typename IS>
void
merge2(const IndSimpSet<N,M,A,T,V,IS>& a, const IndSimpSet<N,M,A,T,V,IS>& b,
       IndSimpSet<N,M,true,T,V,IS>* out);

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_build_ipp__
#include "build.ipp"
#undef __geom_mesh_iss_build_ipp__

#endif
