// -*- C++ -*-

#if !defined(__geom_mesh_quadrilateral_file_io_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Write an indexed simplex set in ascii format.
template<int N, typename VertForIter, typename CellForIter>
inline
void
writeQuadMeshAscii(std::ostream& out, 
		   VertForIter verticesBeginning, VertForIter verticesEnd,
		   CellForIter cellsBeginning, CellForIter cellsEnd) {
  // Set the precision.
  typedef typename std::iterator_traits<VertForIter>::value_type Value;
  typedef typename Loki::Select< Loki::TypeTraits<Value>::isReference,
    typename Loki::TypeTraits<Value>::ReferredType,
    Value >::Result Vertex;
  typedef typename Vertex::value_type Number;
  const int oldPrecision = 
    out.precision(std::numeric_limits<Number>::digits10);

  // Write the space dimension.
  out << N << "\n";

  // Write the number of vertices.
  out << int(std::distance(verticesBeginning, verticesEnd)) << "\n";
  // Write the vertices.
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    out << *verticesBeginning << "\n";
  }

  // Write the number of simplices.
  out << int(std::distance(cellsBeginning, cellsEnd)) 
      << "\n";
  // Write the connectivities.
  for (; cellsBeginning != cellsEnd; ++cellsBeginning) {
    out << *cellsBeginning << "\n";
  }

  // Restore the old precision.
  out.precision(oldPrecision);
}


// Write a quad mesh in binary format.
template<int N, bool A, typename T, typename V, typename IF>
inline
void
writeBinary(std::ostream& out, const QuadMesh<N,A,T,V,IF>& x) {
  // Write the space dimension.
  int dim = N;
  out.write(reinterpret_cast<const char*>(&dim), sizeof(int));

  // Write the number of vertices.
  int sz = x.getVerticesSize();
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the vertices.
  x.getVertices().write_elements_binary(out);

  // Write the number of simplices.
  sz = x.getFacesSize();
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the connectivities.
  x.getIndexedFaces().write_elements_binary(out);
}


// Write a quad mesh in binary format.
template<int N, typename VertForIter, typename CellForIter>
inline
void
writeQuadMeshBinary(std::ostream& out, 
		    VertForIter verticesBeginning, VertForIter verticesEnd,
		    CellForIter cellsBeginning, CellForIter cellsEnd) {
  typedef typename std::iterator_traits<VertForIter>::value_type Vertex;
  typedef typename std::iterator_traits<CellForIter>::value_type 
    IndexedFace;

  // Write the space dimension.
  int dim = N;
  out.write(reinterpret_cast<const char*>(&dim), sizeof(int));

  // Write the number of vertices.
  int sz = std::distance(verticesBeginning, verticesEnd);
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the vertices.
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    out.write(reinterpret_cast<const char*>(&*verticesBeginning), 
	       sizeof(Vertex));
  }

  // Write the number of cells.
  sz = std::distance(cellsBeginning, cellsEnd);
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the connectivities.
  for (; cellsBeginning != cellsEnd; ++cellsBeginning) {
    out.write(reinterpret_cast<const char*>(&*cellsBeginning), 
	      sizeof(IndexedFace));
  }
}


// Read a quad mesh in ascii format.
template<int N, typename T, typename V, typename IF>
inline
void
readAscii(std::istream& in, QuadMesh<N,true,T,V,IF>* x) {
  // Read the space dimension.
  int n;
  in >> n;
  assert(n == N);
  
  // Read the number of vertices.
  in >> n;
  x->getVertices().resize(n);
  // Read the vertices.
  x->getVertices().read_elements_ascii(in);

  // Read the number of faces.
  in >> n;
  x->getIndexedFaces().resize(n);
  // Read the faces.
  x->getIndexedFaces().read_elements_ascii(in);

  // Update any auxilliary topological information.
  x->updateTopology();
}


// Read a quad mesh in binary format.
template<int N, typename T, typename V, typename IF>
inline
void
readBinary(std::istream& in, QuadMesh<N,true,T,V,IF>* x) {
  typedef typename QuadMesh<N,true,T,V,IF>::SizeType SizeType;

  // Read the space dimension.
  int dim;
  in.read(reinterpret_cast<char*>(&dim), sizeof(int));
  assert(dim == N);
  
  // Read the number of vertices.
  SizeType size;
  in.read(reinterpret_cast<char*>(&size), sizeof(SizeType));
  x->getVertices().resize(size);
  // Read the vertices.
  x->getVertices().read_elements_binary(in);

  // Read the number of simplices.
  in.read(reinterpret_cast<char*>(&size), sizeof(SizeType));
  x->getIndexedFaces().resize(size);
  // Read the simplices.
  x->getIndexedFaces().read_elements_binary(in);

  // Update any auxilliary topological information.
  x->updateTopology();
}

END_NAMESPACE_GEOM

// End of file.
