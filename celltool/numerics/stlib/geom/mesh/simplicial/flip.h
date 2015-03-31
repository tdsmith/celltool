// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/flip.h
  \brief Functions to flip edges in a SimpMeshRed<2,2>.
*/

#if !defined(__geom_mesh_simplicial_flip_h__)
#define __geom_mesh_simplicial_flip_h__

#include "SimpMeshRed.h"
#include "geometry.h"

#include "../simplex/SimplexModCondNum.h"
#include "../simplex/SimplexModMeanRatio.h"

BEGIN_NAMESPACE_GEOM

//! Flip faces for as long as the quality of the mesh is improved.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return the number of faces flipped.
*/
template<class DistortionFunction, 
	 int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
int
flip(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh);


//! Flip faces for as long as the quality of the mesh is improved.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.  The edge will only be flipped if 
  the angle between the incident face normals is no greater than
  \c maxAngle.
    
  \return the number of faces flipped.
*/
template<class DistortionFunction, 
	 int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
int
flip(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, T maxAngle);


//! Flip faces using the modified mean ratio metric for as long as the quality of the mesh is improved.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return the number of faces flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
int
flipUsingModifiedMeanRatio(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh) {
  return flip<SimplexModMeanRatio<2,T> >(mesh);
}


//! Flip faces using the modified mean ratio metric for as long as the quality of the mesh is improved.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return the number of faces flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
int
flipUsingModifiedMeanRatio(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, 
			   const T maxAngle) {
  return flip<SimplexModMeanRatio<2,T> >(mesh, maxAngle);
}


//! Flip faces using the modified condition number metric for as long as the quality of the mesh is improved.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return the number of faces flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
int
flipUsingModifiedConditionNumber(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh) {
  return flip<SimplexModCondNum<2,T> >(mesh);
}


//! Flip faces using the modified condition number metric for as long as the quality of the mesh is improved.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return the number of faces flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
inline
int
flipUsingModifiedConditionNumber(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh, 
				 const T maxAngle) {
  return flip<SimplexModCondNum<2,T> >(mesh, maxAngle);
}


//! Flip the specified face if it improves the quality of the mesh.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return true if the face is flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
bool
flip(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh,
     const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::Face& face, 
     DistortionFunction& distortionFunction);


//! Flip the specified face if it improves the quality of the mesh.
/*!
  \relates SimpMeshRed

  Boundary faces will not be flipped.
    
  \return true if the face is flipped.
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
bool
flip(SimpMeshRed<N,2,T,Node,Cell,Cont>* mesh,
     const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::Face& face, 
     DistortionFunction& distortionFunction, T minCosine);


//! Return true if flipping the specified interior face will improve the quality of the mesh.
/*!
  \relates SimpMeshRed
*/
template<int N, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class DistortionFunction>
bool
shouldFlip(const SimpMeshRed<N,2,T,Node,Cell,Cont>& mesh,
	   const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::Face& face,
	   DistortionFunction& distortionFunction);


//! Return true if flipping the specified interior face will improve the quality of the mesh.
/*!
  \relates SimpMeshRed
*/
template<int N, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont,
	   class DistortionFunction >
bool
shouldFlip(const SimpMeshRed<N,2,T,Node,Cell,Cont>& mesh,
	   const typename SimpMeshRed<N,2,T,Node,Cell,Cont>::Face& face,
	   DistortionFunction& distortionFunction, T minCosine);


//! Flip the face between \c ch and \c ch->neighbor(i).
/*!
  \relates SimpMeshRed

  \image html SimpMeshRed_2_flip.jpg "Flipping an edge between two triangle cells."
  \image latex SimpMeshRed_2_flip.pdf "Flipping an edge between two triangle cells."
*/
template<typename SMR>
void
flip(typename SMR::CellIterator cell, int faceIndex);


//! Flip the specified face.
/*!
  \relates SimpMeshRed

  This function just calls flip(CellIterator, int).
*/
template<typename SMR>
inline
void
flip(const typename SMR::Face& face) {
  flip<SMR>(face.first, face.second);
}


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_flip_ipp__
#include "flip.ipp"
#undef __geom_mesh_simplicial_flip_ipp__

#endif
