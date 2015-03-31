// -*- C++ -*-

/*! 
  \file geom/mesh/iss/file_io.h
  \brief Implements file I/O operations for IndSimpSet.
*/

#if !defined(__geom_mesh_iss_file_io_h__)
#define __geom_mesh_iss_file_io_h__

#include "IndSimpSet.h"

#include <string>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_file_io File I/O for IndSimpSet
  An indexed simplex set can be written to a file in ascii or binary format.
  In addition, it can be exported to VTK XML or VTK legacy format.

  For ascii and binary files, the first two numbers identify the space 
  dimension and the simplex dimension.
  Then the vertex coordinates are enumerated, followed by the tuples of 
  vertex indices that comprise the simplices.  These indices are in the range
  [0..numberOfVertices).
  The file format is:
  \verbatim
  spaceDimension simplexDimension
  numberOfVertices 
  vertex_0_coord_0 vertex_0_coord_1 ... vertex_0_coord_N-1
  vertex_1_coord_0 vertex_1_coord_1 ... vertex_1_coord_N-1
  ...
  numberOfSimplices
  simplex_0_index_0 simplex_0_index_1 ... simplex_0_index_M
  simplex_1_index_0 simplex_1_index_1 ... simplex_1_index_M
  ..
  \endverbatim

  For example, the boundary of the unit square in 2-D is:
  \verbatim
  2 1
  4
  0.0 0.0
  1.0 0.0
  1.0 1.0
  0.0 1.0
  4
  0 1
  1 2
  2 3
  3 0
  \endverbatim

  A triangle mesh of the unit square is:
  \verbatim
  2 2
  4
  0.0 0.0
  1.0 0.0
  1.0 1.0
  0.0 1.0
  2
  0 1 2
  0 2 3
  \endverbatim
*/
//@{

// CONTINUE: I should raise an assertion if I can't write/read the file.
// CONTINUE: Add support for comment lines starting with a #.


//! Write an indexed simplex set in ascii format.
template<int N, int M, typename VertForIter, typename ISForIter>
void
writeIssAscii(std::ostream& out, 
	      VertForIter verticesBeginning, VertForIter verticesEnd,
	      ISForIter indexedSimplicesBeginning, 
	      ISForIter indexedSimplicesEnd);


//! Write an indexed simplex set in ascii format.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
writeAscii(std::ostream& out, const IndSimpSet<N,M,A,T,V,IS>& x) {
  writeIssAscii<N,M>(out, x.getVerticesBeginning(), x.getVerticesEnd(),
		     x.getIndexedSimplicesBeginning(), 
		     x.getIndexedSimplicesEnd());
}


//! Write an indexed simplex set in binary format.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
void
writeBinary(std::ostream& out, const IndSimpSet<N,M,A,T,V,IS>& x);


//! Write an indexed simplex set in binary format.
template<int N, int M, typename VertForIter, typename ISForIter>
void
writeIssBinary(std::ostream& out, 
	       VertForIter verticesBeginning, VertForIter verticesEnd,
	       ISForIter indexedSimplicesBeginning, 
	       ISForIter indexedSimplicesEnd);


//! Read an indexed simplex set in ascii format.
/*! \relates IndSimpSet */
template<int N, int M, typename T, typename V, typename IS>
void
readAscii(std::istream& in, IndSimpSet<N,M,true,T,V,IS>* x);


//! Read an indexed simplex set in binary format.
/*! \relates IndSimpSet */
template<int N, int M, typename T, typename V, typename IS>
void
readBinary(std::istream& in, IndSimpSet<N,M,true,T,V,IS>* x);


//--------------------------------------------------------------------------
// VTK XML
//--------------------------------------------------------------------------

//! Write in VTK XML unstructured grid format with a cell field.
template<int N, int M, typename VertForIter, typename ISForIter, 
	 typename ContainerIter, typename StringIter>
void
writeIssAndCellDataVtkXml(std::ostream& out, 
			  VertForIter verticesBeginning, 
			  VertForIter verticesEnd,
			  ISForIter indexedSimplicesBeginning, 
			  ISForIter indexedSimplicesEnd,
			  ContainerIter cellDataContainersBeginning,
			  ContainerIter cellDataContainersEnd,
			  StringIter dataNamesBeginning,
			  StringIter dataNamesEnd);


//! Write in VTK XML unstructured grid format.
template<int N, int M, typename VertForIter, typename ISForIter>
void
writeIssVtkXml(std::ostream& out, 
	       VertForIter verticesBeginning, VertForIter verticesEnd,
	       ISForIter indexedSimplicesBeginning, 
	       ISForIter indexedSimplicesEnd);


//! Write in VTK XML unstructured grid format.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
writeVtkXml(std::ostream& out, const IndSimpSet<N,M,A,T,V,IS>& x) {
  writeIssVtkXml<N,M>(out, x.getVerticesBeginning(), x.getVerticesEnd(), 
		      x.getIndexedSimplicesBeginning(), 
		      x.getIndexedSimplicesEnd());
}


//! Write the mesh and the cell data in VTK XML unstructured grid format.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename F, bool A2>
inline
void
writeVtkXml(std::ostream& out, 
	    const IndSimpSet<N,M,A,T,V,IS>& x,
	    const ads::Array<1,F,A2>& cellData,
	    std::string dataName) {
  writeIssAndCellDataVtkXml<N,M>(out, x.getVerticesBeginning(), 
				 x.getVerticesEnd(), 
				 x.getIndexedSimplicesBeginning(), 
				 x.getIndexedSimplicesEnd(),
				 &cellData, &cellData,
				 &dataName, &dataName);
}


//! Write the mesh and the cell data in VTK XML unstructured grid format.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename ContainerIter, typename StringIter>
inline
void
writeVtkXml(std::ostream& out, 
	    const IndSimpSet<N,M,A,T,V,IS>& x,
	    ContainerIter cellDataContainersBeginning,
	    ContainerIter cellDataContainersEnd,
	    StringIter dataNamesBeginning,
	    StringIter dataNamesEnd) {
  writeIssAndCellDataVtkXml<N,M>(out, x.getVerticesBeginning(), 
				 x.getVerticesEnd(), 
				 x.getIndexedSimplicesBeginning(), 
				 x.getIndexedSimplicesEnd(),
				 cellDataContainersBeginning,
				 cellDataContainersEnd,
				 dataNamesBeginning,
				 dataNamesEnd);
}


//--------------------------------------------------------------------------
// VTK Legacy
//--------------------------------------------------------------------------

//! Write in legacy VTK unstructured grid format.
template<int N, int M, typename VertForIter, typename ISForIter>
void
writeIssVtkLegacy(std::ostream& out, 
		  VertForIter verticesBeginning, VertForIter verticesEnd,
		  ISForIter indexedSimplicesBeginning, 
		  ISForIter indexedSimplicesEnd,
		  std::string title = "");


//! Write in legacy VTK unstructured grid format.
/*! \relates IndSimpSet */
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
writeVtkLegacy(std::ostream& out, const IndSimpSet<N,M,A,T,V,IS>& x,
	       std::string title = "") {
  writeIssVtkLegacy<N,M>(out, x.getVerticesBeginning(), 
			 x.getVerticesEnd(), 
			 x.getIndexedSimplicesBeginning(), 
			 x.getIndexedSimplicesEnd(), title);
}

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_file_io_ipp__
#include "file_io.ipp"
#undef __geom_mesh_iss_file_io_ipp__

#endif
