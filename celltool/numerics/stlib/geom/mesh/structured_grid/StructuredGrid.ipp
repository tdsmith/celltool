// -*- C++ -*-

#if !defined(__geom_StructuredGrid_ipp__)
#error This file is an implementation detail of the class StructuredGrid.
#endif

BEGIN_NAMESPACE_GEOM

//
// File I/O member functions.
//

// Forward declaration.
template<int N, int M, typename T>
void
structuredGridWriteIndexedSimplexSet(std::ostream& out,
				     const StructuredGrid<N,M,T>& x);


// Implementation for 2-D grids.
template<int N, typename T>
inline
void
structuredGridWriteIndexedSimplexSet(std::ostream& out,
				     const StructuredGrid<N,2,T>& sg) {
  // The number of triangles is twice the number of quadrilaterals.
  const int numTriangles = 2 * (sg.getExtents()[0] - 1) * 
    (sg.getExtents()[1] - 1);
  // Write the number of nodes and the number of triangles.
  out << sg.getSize() << " " << numTriangles << '\n';
  // Write the nodes.
  sg.getGrid().write_elements_ascii(out);
  // Write the element indices.
  const int iEnd = sg.getExtents()[0] - 1;
  const int jEnd = sg.getExtents()[1] - 1;
  for (int i = 0; i != iEnd; ++i) {
    for (int j = 0; j != jEnd; ++j) {
      // Choose the shorter diagonal of the quadrilateral.
      if (geom::computeDistance(sg(i, j), sg(i+1, j+1)) <
	  geom::computeDistance(sg(i+1, j), sg(i, j+1))) {
	std::cout << sg.getGrid().index(i, j) << " "
		  << sg.getGrid().index(i+1, j) << " "
		  << sg.getGrid().index(i+1, j+1) << '\n'
		  << sg.getGrid().index(i+1, j+1) << " "
		  << sg.getGrid().index(i, j+1) << " "
		  << sg.getGrid().index(i, j) << '\n';
      }
      else {
	std::cout << sg.getGrid().index(i, j) << " "
		  << sg.getGrid().index(i+1, j) << " "
		  << sg.getGrid().index(i, j+1) << '\n'
		  << sg.getGrid().index(i+1, j+1) << " "
		  << sg.getGrid().index(i, j+1) << " "
		  << sg.getGrid().index(i+1, j) << '\n';
      }
    }
  }
}


template<int N, int M, typename T>
inline
void
StructuredGrid<N,M,T>::
writeIndexedSimplexSet(std::ostream& out) const {
  structuredGridWriteIndexedSimplexSet(out, *this);
}

END_NAMESPACE_GEOM

// End of file.
