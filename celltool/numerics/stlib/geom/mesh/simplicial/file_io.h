// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/file_io.h
  \brief Implements file I/O operations for SimpMeshRed.
*/

#if !defined(__geom_mesh_simplicial_file_io_h__)
#define __geom_mesh_simplicial_file_io_h__

#include "SimpMeshRed.h"

#include "../iss/file_io.h"

#include <string>

BEGIN_NAMESPACE_GEOM

//! Write a mesh as an indexed simplex set in ascii format.
/*!
  First the space dimension and the simplex dimension are written.
  Then the vertex coordinates are written, followed by the tuples of 
  vertex indices that comprise the simplices.  The file format is:
  \verbatim
  space_dimension simplex_dimension
  num_vertices 
  vertex_0_coord_0 vertex_0_coord_1 ... vertex_0_coord_N-1
  vertex_1_coord_0 vertex_1_coord_1 ... vertex_1_coord_N-1
  ...
  num_simplices
  simplex_0_index_0 simplex_0_index_1 ... simplex_0_index_M
  simplex_1_index_0 simplex_1_index_1 ... simplex_1_index_M
  ..
  \endverbatim
*/
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
writeAscii(std::ostream& out, const SimpMeshRed<N,M,T,Node,Cell,Cont>& x);


//! Print detailed information about the mesh.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
print(std::ostream& out, const SimpMeshRed<N,M,T,Node,Cell,Cont>& x);


//! Write a mesh as an indexed simplex set in binary format.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
writeBinary(std::ostream& out, const SimpMeshRed<N,M,T,Node,Cell,Cont>& x);


//! Read a mesh as an indexed simplex set in ascii format.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
readAscii(std::istream& in, SimpMeshRed<N,M,T,Node,Cell,Cont>* x);


//! Read an indexed simplex set in binary format.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
readBinary(std::istream& in, SimpMeshRed<N,M,T,Node,Cell,Cont>* x);


//! Write in VTK XML unstructured grid format.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
writeVtkXml(std::ostream& out, const SimpMeshRed<N,M,T,Node,Cell,Cont>& x);


//! Write in legacy VTK unstructured grid format.
template<int N, int M, typename T,
	 template<class> class Node,
	 template<class> class Cell,
	 template<class,class> class Cont>
void
writeVtkLegacy(std::ostream& out, 
		  const SimpMeshRed<N,M,T,Node,Cell,Cont>& x,
		  std::string title = "");


END_NAMESPACE_GEOM

#define __geom_mesh_simplicial_file_io_ipp__
#include "file_io.ipp"
#undef __geom_mesh_simplicial_file_io_ipp__

#endif
