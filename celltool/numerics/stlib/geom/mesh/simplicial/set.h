// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/set.h
  \brief Implements operations that set a SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_set_h__)
#define __geom_mesh_simplicial_set_h__

#include "SimpMeshRed.h"
#include "accessors.h"

BEGIN_NAMESPACE_GEOM


//! Get the nodes that are outside the object.
/*!
  \param begin is the beginning of a range of node input iterators.
  \param end is the end of a range of node input iterators.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param iter is an output iterator for the node const iterators.
*/
template<typename NodeInIter, class LSF, typename OutIter>
void
determineNodesOutside(NodeInIter begin, NodeInIter end,
		      const LSF& f, OutIter iter);


//! Get the nodes that are outside the object.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param iter is an output iterator for the node const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class LSF, typename OutIter>
inline
void
determineNodesOutside(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		      const LSF& f, OutIter iter) {
  determineNodesOutside(mesh.getNodesBeginning(), mesh.getNodesEnd(), f, iter);
}


//! Get the nodes that are outside the object.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param iter is an output iterator for the node iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class LSF, typename OutIter>
inline
void
determineNodesOutside(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		      const LSF& f, OutIter iter) {
  determineNodesOutside(mesh.getNodesBeginning(), mesh.getNodesEnd(), f, iter);
}




//! Get the cells whose centroids are outside the object.
/*!
  \param begin is the beginning of a range of cell input iterators.
  \param end is the end of a range of cell input iterators.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param iter is an output iterator for the cell const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<typename CellInIter, class LSF, typename OutIter>
void
determineCellsOutside(CellInIter begin, CellInIter end,
		      const LSF& f, OutIter iter);


//! Get the cells whose centroids are outside the object.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param iter is an output iterator for the cell const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class LSF, typename OutIter>
inline
void
determineCellsOutside(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		      const LSF& f, OutIter iter) {
  determineCellsOutside(mesh.getCellsBeginning(), mesh.getCellsEnd(), f, iter);
}


//! Get the cells whose centroids are outside the object.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param f is the level set function that describes the object.
  Points inside/outside the object have negative/positive values.
  \param iter is an output iterator for the cell iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 class LSF, typename OutIter>
inline
void
determineCellsOutside(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		      const LSF& f, OutIter iter) {
  determineCellsOutside(mesh.getCellsBeginning(), mesh.getCellsEnd(), f, iter);
}




//! Get the node iterators for the all of the nodes in the mesh.
/*!
  \param begin is the beginning of a range of node input iterators.
  \param end is the end of a range of node input iterators.
  \param iter is an output iterator for the node iterators.
*/
template<typename NodeInIter, typename OutIter>
void
getNodes(NodeInIter begin, NodeInIter end, OutIter iter);


//! Get the node const iterators for the all of the nodes in the mesh.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param iter is an output iterator for the node const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
getNodes(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, OutIter iter) {
  getNodes(mesh.getNodesBeginning(), mesh.getNodesEnd(), iter);
}


//! Get the node iterators for the all of the nodes in the mesh.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param iter is an output iterator for the node iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
getNodes(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, OutIter iter) {
  getNodes(mesh.getNodesBeginning(), mesh.getNodesEnd(), iter);
}




//! Get the node iterators for the interior nodes.
/*!
  \param begin is the beginning of a range of node input iterators.
  \param end is the end of a range of node input iterators.
  \param iter is an output iterator for the node iterators.
*/
template<typename NodeInIter, typename OutIter>
void
determineInteriorNodes(NodeInIter begin, NodeInIter end, OutIter iter);


//! Get the node const iterators for the interior nodes.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param iter is an output iterator for the node const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
determineInteriorNodes(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
		       OutIter iter) {
  determineInteriorNodes(mesh.getNodesBeginning(), mesh.getNodesEnd(), iter);
}


//! Get the node iterators for the interior nodes.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param iter is an output iterator for the node iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont,
	   typename OutIter>
inline
void
determineInteriorNodes(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
		       OutIter iter) {
  determineInteriorNodes(mesh.getNodesBeginning(), mesh.getNodesEnd(), iter);
}




//! Get the node iterators for the boundary nodes.
/*!
  \param begin is the beginning of a range of node input iterators.
  \param end is the end of a range of node input iterators.
  \param iter is an output iterator for the node iterators.
*/
template<typename NodeInIter, typename OutIter>
void
determineBoundaryNodes(NodeInIter begin, NodeInIter end, OutIter iter);


//! Get the node const iterators for the boundary nodes.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param iter is an output iterator for the node const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
determineBoundaryNodes(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
		       OutIter iter) {
  determineBoundaryNodes(mesh.getNodesBeginning(), mesh.getNodesEnd(), iter);
}


//! Get the node iterators for the boundary nodes.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param iter is an output iterator for the node iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont,
	   typename OutIter>
inline
void
determineBoundaryNodes(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
		       OutIter iter) {
  determineBoundaryNodes(mesh.getNodesBeginning(), mesh.getNodesEnd(), iter);
}




//! Get the cell iterators with at least the specified number of adjacencies.
/*!
  \param begin is the beginning of a range of cell input iterators.
  \param end is the end of a range of cell input iterators.
  \param minimumRequiredAdjacencies The minimum required adjacencies.
  \param iter is an output iterator for the cell iterators.
*/
template<typename CellInIter, typename OutIter>
void
determineCellsWithRequiredAdjacencies(CellInIter begin, CellInIter end, 
				      int minimumRequiredAdjacencies, 
				      OutIter iter);


//! Get the cell const iterators with at least the specified number of adjacencies.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param minimumRequiredAdjacencies The minimum required adjacencies.
  \param iter is an output iterator for the cell const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
determineCellsWithRequiredAdjacencies
(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
 const int minimumRequiredAdjacencies, OutIter iter) {
  determineCellsWithRequiredAdjacencies(mesh.getCellsBeginning(), 
					mesh.getCellsEnd(), 
					minimumRequiredAdjacencies, iter);
}


//! Get the cell iterators with at least the specified number of adjacencies.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param minimumRequiredAdjacencies This function gets the cells that have at least as many
  adjacencies as minimumRequiredAdjacencies.
  \param iter is an output iterator for the cell iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
determineCellsWithRequiredAdjacencies(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
				      const int minimumRequiredAdjacencies, 
				      OutIter iter)
{
  determineCellsWithRequiredAdjacencies(mesh.getCellsBeginning(), 
					mesh.getCellsEnd(), 
					minimumRequiredAdjacencies, iter);
}





//! Get the cell iterators with adjacencies less than specified.
/*!
  \param begin is the beginning of a range of cell input iterators.
  \param end is the end of a range of cell input iterators.
  \param minimumRequiredAdjacencies This function gets the cells that have fewer adjacencies 
  than minimumRequiredAdjacencies.
  \param iter is an output iterator for the cell iterators.
*/
template<typename CellInIter, typename OutIter>
void
determineCellsWithLowAdjacencies(CellInIter begin, CellInIter end, 
				 const int minimumRequiredAdjacencies, 
				 OutIter iter);


//! Get the cell const iterators with adjacencies less than specified.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param minimumRequiredAdjacencies The minimum required adjacencies.
  \param iter is an output iterator for the cell const iterators.

  This function calls the function of the same name with
  const iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
determineCellsWithLowAdjacencies(const SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
				 const int minimumRequiredAdjacencies, 
				 OutIter iter) {
  determineCellsWithLowAdjacencies(mesh.getCellsBeginning(), 
				   mesh.getCellsEnd(), 
				   minimumRequiredAdjacencies, iter);
}


//! Get the cell iterators with adjacencies less than specified.
/*!
  \relates SimpMeshRed

  \param mesh is the simplicial mesh.
  \param minimumRequiredAdjacencies The minimum required adjacencies.
  \param iter is an output iterator for the cell iterators.

  This function calls the function of the same name with
  iterators as the initial arguments.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont,
	 typename OutIter>
inline
void
determineCellsWithLowAdjacencies(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh, 
				 const int minimumRequiredAdjacencies, 
				 OutIter iter) {
  determineCellsWithLowAdjacencies(mesh.getCellsBeginning(), 
				   mesh.getCellsEnd(), 
				   minimumRequiredAdjacencies, iter);
}





//! Get the neighboring nodes of a node.
/*!
  The set of nodes (not including the specified node) that share a cell with
  the specified node.
*/
template<int N, int M, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont>
void
determineNeighbors(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		   NodeIterator node,
		   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		   NodeIteratorSet* neighbors);


//! Get the neighboring boundary nodes of a node.
/*!
  The set of boundary nodes (not including the specified node) that share 
  a cell with the specified node.
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
determineBoundaryNeighbors(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			   NodeIterator node,
			   typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			   NodeIteratorSet* neighbors);


//! Get all the nodes within the specified radius of the specified node.
/*!
  The set includes the specified node.
*/
template<int N, int M, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont>
void
determineNeighbors(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
		  typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		  NodeIterator node,
		  int radius,
		  typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
		  NodeIteratorSet* neighbors);



//! Get the faces of the incident cells.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
determineFacesOfIncidentCells(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			      typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			      NodeIterator node,
			      typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			      FaceSet* faces);



//! Build a set of cell iterators from a range of cell identifiers.
/*!
  \param mesh The simplicial mesh.  It is not modified, but because we are 
  getting cell iterators (not cell const iterators) we pass it by reference
  (not const reference).
  \param begin The beginning of the range of identifiers.
  \param end The end of the range of identifiers.
  \param cells The set of cell iterators.

  The cell identifiers may not be the same as the cell indices.  (Cell indices
  would be in the range [ 0 .. mesh.cells_size()).
*/
template<int N, int M, typename T,
	   template<class> class Node,
	   template<class> class Cell,
	   template<class,class> class Cont,
	   typename IntInIter>
void
convertIdentifiersToIterators(SimpMeshRed<N,M,T,Node,Cell,Cont>& mesh,
			      IntInIter begin, IntInIter end,
			      typename SimpMeshRed<N,M,T,Node,Cell,Cont>::
			      CellIteratorSet* cells);

END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_set_ipp__
#include "set.ipp"
#undef __geom_mesh_simplicial_set_ipp__

#endif
