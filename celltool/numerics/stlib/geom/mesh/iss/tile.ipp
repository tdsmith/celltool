// -*- C++ -*-

#if !defined(__geom_mesh_iss_tile_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<typename T, typename V, typename IS, class LSF>
inline
void
tile(const BBox<2,T>& domain, const T length, const LSF& f,
     IndSimpSet<2,2,true,T,V,IS>* mesh) {
  assert(! domain.isEmpty() && length > 0);

  // The height of the equilateral triangle.
  const T height = std::sqrt(3.) * length / 2;
  
  // The number of triangles in the x direction.
  const int numX = int(std::ceil((domain.getUpperCorner()[0] - 
				  domain.getLowerCorner()[0] + 
				  length / 2.) / length));
  // The number of triangles in the y direction.
  const int numY = int(std::ceil((domain.getUpperCorner()[1] - 
				  domain.getLowerCorner()[1]) / 
				 height));
  
  // The number of vertices and simplices in the mesh.
  const int numVertices = (numX + 1) * (numY + 1);

  // Resize the arrays.
  mesh->getVertices().resize(numVertices);
  
  //
  // Make the vertices.
  //

  int i, j;
  V x;
  int n = 0;
  // For each row of vertices.
  for (j = 0; j <= numY; ++j) {
    // Calculate the first vertex.
    x = domain.getLowerCorner();
    if (j % 2 == 1) {
      x[0] -= length / 2.0;
    }
    x[1] += j * height;
    // For each vertex in the row.
    for (i = 0; i <= numX; ++i, ++n, x[0] += length) {
      mesh->getVertices()[n] = x;
    }
  }
  
  //
  // Make the simplices.
  //

  int row, col;
  IS s;
  std::vector<IS> simp;
  int m;
  for (row = 0; row != numY; ++row) {
    i = row * (numX + 1);
    j = i + numX + 1;
    for (col = 0; col != numX; ++col, ++i, ++j) {
      if (row % 2 == 0) {
	// The indexed simplex.
	s[0] = i;
	s[1] = i + 1;
	s[2] = j + 1;
	// Compute the centroid.
	x = 0;
	for (m = 0; m != 3; ++m) {
	  x += mesh->getVertices()[ s[m] ];
	}
	x /= 3.0;
	// If the centroid is inside.
	if (f(x) <= 0) {
	  // Add the indexed simplex.
	  simp.push_back(s);
	}

	// The indexed simplex.
	s[0] = i;
	s[1] = j + 1;
	s[2] = j;
	// Compute the centroid.
	x = 0;
	for (m = 0; m != 3; ++m) {
	  x += mesh->getVertices()[ s[m] ];
	}
	x /= 3.0;
	// If the centroid is inside.
	if (f(x) <= 0) {
	  // Add the indexed simplex.
	  simp.push_back(s);
	}
      }
      else {
	// The indexed simplex.
	s[0] = i;
	s[1] = i + 1;
	s[2] = j;
	// Compute the centroid.
	x = 0;
	for (m = 0; m != 3; ++m) {
	  x += mesh->getVertices()[ s[m] ];
	}
	x /= 3.0;
	// If the centroid is inside.
	if (f(x) <= 0) {
	  // Add the indexed simplex.
	  simp.push_back(s);
	}

	// The indexed simplex.
	s[0] = i + 1;
	s[1] = j + 1;
	s[2] = j;
	// Compute the centroid.
	x = 0;
	for (m = 0; m != 3; ++m) {
	  x += mesh->getVertices()[ s[m] ];
	}
	x /= 3.0;
	// If the centroid is inside.
	if (f(x) <= 0) {
	  // Add the indexed simplex.
	  simp.push_back(s);
	}
      }
    }
  }
  // Add the simplices whose centroids are inside the object to the mesh.
  mesh->getIndexedSimplices().resize(int(simp.size()));
  std::copy(simp.begin(), simp.end(), mesh->getIndexedSimplices().begin());
  // Pack the mesh to get rid of unused vertices.
  pack(mesh);
}



template<typename T, typename V, typename IS, class LSF>
inline
void
tile(const BBox<3,T>& domain, const T length, const LSF& f,
     IndSimpSet<3,3,true,T,V,IS>* mesh) {
  typedef Simplex<3,V> Simplex;

  assert(! domain.isEmpty() && length > 0);

  // The number of blocks in the x direction.
  const int numX = int(std::ceil((domain.getUpperCorner()[0] - 
				  domain.getLowerCorner()[0] + 
				  length / 2.) / length));
  // The number of blocks in the y direction.
  const int numY = int(std::ceil((domain.getUpperCorner()[1] - 
				  domain.getLowerCorner()[1] + 
				  length / 2.) / length));
  // The number of blocks in the z direction.
  const int num_z = int(std::ceil((domain.getUpperCorner()[2] - 
				   domain.getLowerCorner()[2] + 
				   length / 2.) / length));
  
  // Half length.
  const T hl = length / 2.0;

  //
  // Make the simplex set.
  //
  
  // The (un-indexed) simplex set.
  std::vector<V> simplices;
  // The lower corner of the block.
  V corner;
  // Axis
  V a0, a1;
  //Ring
  ads::FixedArray<4,V> r;
  // The simplex.
  Simplex s;
  // The centroid of the simplex.
  V centroid;
  // Loop over the blocks.
  int x, y, z, n, m, mm;
  for (z = 0; z != num_z; ++z) {
    corner[2] = domain.getLowerCorner()[2] + z * length;
    for (y = 0; y != numY; ++y) {
      corner[1] = domain.getLowerCorner()[1] + y * length;
      for (x = 0; x != numX; ++x) {
	corner[0] = domain.getLowerCorner()[0] + x * length;

	// For each dimension.
	for (n = 0; n != 3; ++n) {
	  int i = n;
	  int j = (n+1)%3;
	  int k = (n+2)%3;
	  a1 = corner;
	  a1 += hl;
	  a0 = a1;
	  a0[i] -= length;
	  r[0] = corner;
	  r[1] = r[0];
	  r[1][j] += length;
	  r[2] = r[1];
	  r[2][k] += length;
	  r[3] = r[2];
	  r[3][j] -= length;

	  // For the four simplices whose axis is in this direction.
	  for (m = 0; m != 4; ++m) {
	    // Make the simplex.
	    s[0] = a0;
	    s[1] = a1;
	    s[2] = r[m];
	    s[3] = r[(m+1)%4];
	    // If the centroid is inside the object.
	    s.computeCentroid(&centroid);
	    if (f(centroid) <= 0) {
	      // Add the four vertices of the simplex to the set.
	      for (mm = 0; mm != 4; ++mm) {
		simplices.push_back(s[mm]);
	      }
	    }
	  }
	}
      }
    }
  }

  // Build the mesh from the simplex set.
  buildFromSimplices(simplices.begin(), simplices.end(), mesh);
}

END_NAMESPACE_GEOM

// End of file.
