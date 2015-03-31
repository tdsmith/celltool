// -*- C++ -*-

#if !defined(__geom_mesh_iss_file_io_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Write an indexed simplex set in ascii format.
template<int N, int M, typename VertForIter, typename ISForIter>
inline
void
writeIssAscii(std::ostream& out, 
	      VertForIter verticesBeginning, VertForIter verticesEnd,
	      ISForIter indexedSimplicesBeginning, 
	      ISForIter indexedSimplicesEnd) {
  // Set the precision.
  typedef typename std::iterator_traits<VertForIter>::value_type Value;
  typedef typename Loki::Select< Loki::TypeTraits<Value>::isReference,
    typename Loki::TypeTraits<Value>::ReferredType,
    Value >::Result Vertex;
  typedef typename Vertex::value_type Number;
  const int oldPrecision = 
    out.precision(std::numeric_limits<Number>::digits10);

  // Write the space dimension and simplex dimension.
  out << N << " " << M << '\n';

  // Write the number of vertices.
  out << int(std::distance(verticesBeginning, verticesEnd)) << '\n';
  // Write the vertices.
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    out << *verticesBeginning << '\n';
  }

  // Write the number of simplices.
  out << int(std::distance(indexedSimplicesBeginning, indexedSimplicesEnd)) 
      << '\n';
  // Write the connectivities.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    out << *indexedSimplicesBeginning << '\n';
  }

  // Restore the old precision.
  out.precision(oldPrecision);
}


// Write an indexed simplex set in binary format.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
writeBinary(std::ostream& out, const IndSimpSet<N,M,A,T,V,IS>& x) {
  // Write the space dimension and simplex dimension.
  int dim = N;
  out.write(reinterpret_cast<const char*>(&dim), sizeof(int));
  dim = M;
  out.write(reinterpret_cast<const char*>(&dim), sizeof(int));

  // Write the number of vertices.
  int sz = x.getVerticesSize();
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the vertices.
  x.getVertices().write_elements_binary(out);

  // Write the number of simplices.
  sz = x.getSimplicesSize();
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the connectivities.
  x.getIndexedSimplices().write_elements_binary(out);
}


// Write an indexed simplex set in binary format.
template<int N, int M, typename VertForIter, typename ISForIter>
inline
void
writeIssBinary(std::ostream& out, 
	       VertForIter verticesBeginning, VertForIter verticesEnd,
	       ISForIter indexedSimplicesBeginning, 
	       ISForIter indexedSimplicesEnd) {
  typedef typename std::iterator_traits<VertForIter>::value_type Vertex;
  typedef typename std::iterator_traits<ISForIter>::value_type 
    IndexedSimplex;

  // Write the space dimension and simplex dimension.
  int dim = N;
  out.write(reinterpret_cast<const char*>(&dim), sizeof(int));
  dim = M;
  out.write(reinterpret_cast<const char*>(&dim), sizeof(int));

  // Write the number of vertices.
  int sz = std::distance(verticesBeginning, verticesEnd);
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the vertices.
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    out.write(reinterpret_cast<const char*>(&*verticesBeginning), 
	       sizeof(Vertex));
  }

  // Write the number of simplices.
  sz = std::distance(indexedSimplicesBeginning, indexedSimplicesEnd);
  out.write(reinterpret_cast<const char*>(&sz), sizeof(int));
  // Write the connectivities.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    out.write(reinterpret_cast<const char*>(&*indexedSimplicesBeginning), 
	       sizeof(IndexedSimplex));
  }
}


// Read an indexed simplex set in ascii format.
template<int N, int M, typename T, typename V, typename IS>
inline
void
readAscii(std::istream& in, IndSimpSet<N,M,true,T,V,IS>* x) {
  // Read the space dimension and the simplex dimension.
  int n;
  in >> n;
  assert(n == N);
  in >> n;
  assert(n == M);
  
  // Read the number of vertices.
  in >> n;
  x->getVertices().resize(n);
  // Read the vertices.
  x->getVertices().read_elements_ascii(in);

  // Read the number of simplices.
  in >> n;
  x->getIndexedSimplices().resize(n);
  // Read the simplices.
  x->getIndexedSimplices().read_elements_ascii(in);

  // Update any auxilliary topological information.
  x->updateTopology();
}


// Read an indexed simplex set in binary format.
template<int N, int M, typename T, typename V, typename IS>
inline
void
readBinary(std::istream& in, IndSimpSet<N,M,true,T,V,IS>* x) {
  typedef typename IndSimpSet<N,M,true,T,V,IS>::SizeType SizeType;

  // Read the space dimension and the simplex dimension.
  int dim;
  in.read(reinterpret_cast<char*>(&dim), sizeof(int));
  assert(dim == N);
  in.read(reinterpret_cast<char*>(&dim), sizeof(int));
  assert(dim == M);
  
  // Read the number of vertices.
  SizeType size;
  in.read(reinterpret_cast<char*>(&size), sizeof(SizeType));
  x->getVertices().resize(size);
  // Read the vertices.
  x->getVertices().read_elements_binary(in);

  // Read the number of simplices.
  in.read(reinterpret_cast<char*>(&size), sizeof(SizeType));
  x->getIndexedSimplices().resize(size);
  // Read the simplices.
  x->getIndexedSimplices().read_elements_binary(in);

  // Update any auxilliary topological information.
  x->updateTopology();
}



//--------------------------------------------------------------------------
// VTK XML
//--------------------------------------------------------------------------


// Internal function.
template<typename FieldIterator>
inline
void
writeDataArray(std::ostream& out, 
	       FieldIterator beginning,
	       FieldIterator end,
	       const std::string& name) {
  out << "<DataArray type=\"Float32\" Name=\"" << name << "\">\n";
  while (beginning != end) {
    out << *beginning << "\n";
    ++beginning;
  }
  out << "</DataArray>\n";
}


// Write in VTK XML unstructured grid format with a cell field.
template<int N, int M, typename VertForIter, typename ISForIter, 
	 typename ContainerIter, typename StringIter>
inline
void
writeIssAndCellDataVtkXml(std::ostream& out, 
			  VertForIter verticesBeginning, 
			  VertForIter verticesEnd,
			  ISForIter indexedSimplicesBeginning, 
			  ISForIter indexedSimplicesEnd,
			  ContainerIter cellDataContainersBeginning,
			  ContainerIter cellDataContainersEnd,
			  StringIter dataNamesBeginning,
			  StringIter dataNamesEnd) {
  // Set the precision.
  typedef typename std::iterator_traits<VertForIter>::value_type Value;
  typedef typename Loki::Select< Loki::TypeTraits<Value>::isReference,
    typename Loki::TypeTraits<Value>::ReferredType,
    Value >::Result Vertex;
  typedef typename Vertex::value_type Number;
  const int oldPrecision = 
    out.precision(std::numeric_limits<Number>::digits10);

  // Sanity check for the cell data and cell data names.
  assert(std::distance(cellDataContainersBeginning, cellDataContainersEnd) ==
	 std::distance(dataNamesBeginning, dataNamesEnd));

  // Determine the number of vertices and simplices.
  const int numVertices = int(std::distance(verticesBeginning, verticesEnd));
  const int numSimplices = int(std::distance(indexedSimplicesBeginning, 
					     indexedSimplicesEnd));

  // Header.
  out << "<?xml version=\"1.0\"?>\n";
  // Begin VTKFile.
  out << "<VTKFile type=\"UnstructuredGrid\">\n";
  // Begin UnstructuredGrid.
  out << "<UnstructuredGrid>\n";
  // Begin Piece.
  out << "<Piece NumberOfPoints=\"" << numVertices 
      << "\" NumberOfCells=\"" << numSimplices << "\">\n";

  // Begin PointData.
  out << "<PointData>\n";
  // End PointData.
  out << "</PointData>\n";

  // Begin CellData.
  out << "<CellData>\n";
  // For each cell data field.
  for (; cellDataContainersBeginning != cellDataContainersEnd; 
       ++cellDataContainersBeginning, ++dataNamesBeginning) {
    // The size of the field array should be the number of simplices.
    assert(std::distance(cellDataContainersBeginning->begin(),
			 cellDataContainersBeginning->end()) == numSimplices);
    // Write the field data.
    writeDataArray(out, cellDataContainersBeginning->begin(),
		   cellDataContainersBeginning->end(),
		   *dataNamesBeginning);
  }
  // End CellData.
  out << "</CellData>\n";

  // Begin Points.
  out << "<Points>\n";
  out << "<DataArray type=\"Float32\" NumberOfComponents=\"3\">\n";
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    out << *verticesBeginning;
    if (N == 3) {
      out << "\n";
    }
    else if (N == 2) {
      out << " 0\n";
    }
    else {
      assert(false);
    }
  }
  out << "</DataArray>\n";
  // End Points.
  out << "</Points>\n";

  // Begin Cells.
  out << "<Cells>\n";
  out << "<DataArray type=\"Int32\" Name=\"connectivity\">\n";
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    out << *indexedSimplicesBeginning << "\n";
  }
  out << "</DataArray>\n";
  out << "<DataArray type=\"Int32\" Name=\"offsets\">\n";
  for (int i = 1; i <= numSimplices; ++i) {
    out << (M + 1) * i << "\n";
  }
  out << "</DataArray>\n";
  out << "<DataArray type=\"UInt8\" Name=\"types\">\n";
  if (M == 3) {
    // Each cell is a tetrahedron.
    for (int i = 0; i != numSimplices; ++i) {
      out << "10\n";
    }  
  }
  else if (M == 2) {
    // Each cell is a triangle.
    for (int i = 0; i != numSimplices; ++i) {
      out << "5\n";
    }  
  }
  else if (M == 1) {
    // Each cell is a line segment.
    for (int i = 0; i != numSimplices; ++i) {
      out << "3\n";
    }  
  }
  else if (M == 0) {
    // Each cell is a point.
    for (int i = 0; i != numSimplices; ++i) {
      out << "1\n";
    }  
  }
  else {
    // Unknown simplex type.
    assert(false);
  }
  out << "</DataArray>\n";
  // End Cells.
  out << "</Cells>\n";

  // End Piece.
  out << "</Piece>\n";

  // End UnstructuredGrid.
  out << "</UnstructuredGrid>\n";

  // End VTKFile.
  out << "</VTKFile>\n";

  // Restore the old precision.
  out.precision(oldPrecision);
}




// CONTINUE: Just call above function.
// Write in VTK XML unstructured grid format.
template<int N, int M, typename VertForIter, typename ISForIter>
inline
void
writeIssVtkXml(std::ostream& out, 
	       VertForIter verticesBeginning, VertForIter verticesEnd,
	       ISForIter indexedSimplicesBeginning, 
	       ISForIter indexedSimplicesEnd) {
  const ads::Array<1,double>* TrivialContainerIterator(0);
  const std::string* TrivialStringIterator(0);
  writeIssAndCellDataVtkXml<N,M>(out, verticesBeginning, verticesEnd,
				 indexedSimplicesBeginning, 
				 indexedSimplicesEnd,
				 TrivialContainerIterator, 
				 TrivialContainerIterator, // No cell data.
				 TrivialStringIterator, 
				 TrivialStringIterator); // No cell data names.
}


//--------------------------------------------------------------------------
// VTK Legacy
//--------------------------------------------------------------------------


// Write in legacy VTK unstructured grid format.
template<int N, int M, typename VertForIter, typename ISForIter>
inline
void
writeIssVtkLegacy(std::ostream& out, 
		  VertForIter verticesBeginning, VertForIter verticesEnd,
		  ISForIter indexedSimplicesBeginning, 
		  ISForIter indexedSimplicesEnd,
		  std::string title) {
  // Set the precision.
  typedef typename std::iterator_traits<VertForIter>::value_type Value;
  typedef typename Loki::Select< Loki::TypeTraits<Value>::isReference,
    typename Loki::TypeTraits<Value>::ReferredType,
    Value >::Result Vertex;
  typedef typename Vertex::value_type Number;
  const int oldPrecision = 
    out.precision(std::numeric_limits<Number>::digits10);

  // Determine the number of vertices and simplices.
  const int numVertices = int(std::distance(verticesBeginning, verticesEnd));
  const int numSimplices = int(std::distance(indexedSimplicesBeginning, 
					     indexedSimplicesEnd));

  //
  // The header.
  //
  out << "# vtk DataFile Version 3.0\n";

  //
  // The title.
  //
  assert(title.size() < 256);
  out << title;
  if (title.size() == 0 || title[ title.size() - 1] != '\n') {
    out << '\n';
  }

  //
  // The data type.
  //
  out << "ASCII\n";

  //
  // Geometry/topology.
  //
  out << "DATASET UNSTRUCTURED_GRID\n";

  //
  // Dataset attributes.
  //

  // The nodes.
  out << "POINTS " << numVertices << " double\n";
  for (; verticesBeginning != verticesEnd; ++verticesBeginning) {
    out << *verticesBeginning;
    if (N == 3) {
      out << "\n";
    }
    else if (N == 2) {
      out << " 0\n";
    }
    else {
      assert(false);
    }
  }

  // The cells.
  out << "CELLS " << numSimplices << " " << (M + 2) * numSimplices << "\n";
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    out << M + 1 << " " << *indexedSimplicesBeginning << "\n";
  }

  // The cell types.
  // See page 296 of "The VTK User's Guide."
  out << "CELL_TYPES " << numSimplices << "\n";
  if (M == 3) {
    // Each cell is a tetrahedron.
    for (int n = 0; n != numSimplices; ++n) {
      out << "10\n";
    }
  }
  else if (M == 2) {
    // Each cell is a triangle.
    for (int n = 0; n != numSimplices; ++n) {
      out << "5\n";
    }
  }
  else if (M == 1) {
    // Each cell is a line segment.
    for (int n = 0; n != numSimplices; ++n) {
      out << "3\n";
    }
  }
  else if (M == 0) {
    // Each cell is a point.
    for (int n = 0; n != numSimplices; ++n) {
      out << "1\n";
    }
  }
  else {
    // Unknown cell type.
    assert(false);
  }

  // Restore the old precision.
  out.precision(oldPrecision);
}

END_NAMESPACE_GEOM

// End of file.
