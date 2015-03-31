// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/tile.h
  \brief Tile a rectilinear region with a simplicial mesh.
*/

#if !defined(__geom_mesh_simplicial_tile_h__)
#define __geom_mesh_simplicial_tile_h__

#include "../iss/tile.h"
#include "SimpMeshRed.h"

#include <iostream>

#include <cassert>
#include <cmath>

BEGIN_NAMESPACE_GEOM

//! Tile the rectilinear region.
/*!
  \param domain is the rectilinear domain to tile.
  \param length is the maximum tetrahedron edge length.
  \param mesh is the indexed simplex set.

  In 2-D, tile the rectangular region with equilateral triangles.
  In 3-D, tile with a body-centered cubic lattice.

  CONTINUE: draw a picture of the 2-D mesh and 3-D block.

  The template parameters can be deduced from the arguments.
*/
template<int N, 
	 int M,
	 typename T,
	 template<class> class Vertex,
	 template<class> class Cell,
	 template<class,class> class Container>
inline
void
tile(const BBox<N,T>& domain, const T length,
     SimpMeshRed<N,M,T,Vertex,Cell,Container>* mesh) {
  IndSimpSet<N,M,true,T> iss;
  tile(domain, length, &iss);
  // Build the mesh from the indexed simplex set.
  mesh->build(iss);
}


//! Tile the object.
/*!
  \param domain is the rectilinear domain to tile.
  \param length is the maximum tetrahedron edge length.
  \param f is the level set description of the object.
  \param mesh is the indexed simplex set.

  In 2-D, tile the rectangular region with equilateral triangles.
  In 3-D, tile with a body-centered cubic lattice.

  The template parameters can be deduced from the arguments.
*/
template<int N, 
	 int M,
	 typename T,
	 template<class> class Vertex,
	 template<class> class Cell,
	 template<class,class> class Container,
	 class LSF>
inline
void
tile(const BBox<N,T>& domain, const T length, const LSF& f,
     SimpMeshRed<N,M,T,Vertex,Cell,Container>* mesh) {
  IndSimpSet<N,M,true,T> iss;
  tile(domain, length, f, &iss);
  // Build the mesh from the indexed simplex set.
  mesh->build(iss);
}

END_NAMESPACE_GEOM

#endif
