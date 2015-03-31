// -*- C++ -*-

/*! 
  \file geom/mesh/iss/tile.h
  \brief Tile a rectilinear region with simplices.
*/

#if !defined(__geom_mesh_iss_tile_h__)
#define __geom_mesh_iss_tile_h__

#include "buildFromSimplices.h"
#include "transform.h"

#include "../../../ads/functor/constant.h"

#include <iostream>

#include <cassert>
#include <cmath>

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup iss_tile Tiling 
  These functions use the cookie-cutter algorithm to mesh an object.  
  The object is described implicitly with a level set function.

  Future work: Tile inside and around the object.  Refine the mesh 
  near the boundary.  Then apply the cookie cutter algorithm.  This will 
  increase fidelity at the boundary while maintaining the desired edge
  length in the interior.
*/
//@{

//! Tile the object with equilateral triangles.
/*!
  \relates IndSimpSet

  \param domain is the rectangular domain that contains the object.
  \param length is the triangle edge length.
  \param f is the level set description of the object.
  \param mesh is the indexed simplex set.

  The template parameters can be deduced from the arguments.
  - \c T is the number type.
  - \c V is the vertex type, an 2-tuple of the number type.  
    It must be subscriptable.
  - \c IS is the indexed simplex type, a tuple of 3 integers.  
    It must be subscriptable.
  - \c LSF is the level set function that describes the object.  Negative
    values are inside.
*/
template<typename T,    // number type
	 typename V,    // Vertex
	 typename IS,   // Indexed Simplex
	 class LSF>    // Level Set Function
void
tile(const BBox<2,T>& domain, T length, const LSF& f,
     IndSimpSet<2,2,true,T,V,IS>* mesh);


//! Tile the rectangular region with equilateral triangles.
/*!
  \relates IndSimpSet

  \param domain is the rectangular domain to tile.
  \param length is the triangle edge length.
  \param mesh is the indexed simplex set.

  The template parameters can be deduced from the arguments.
  - \c T is the number type.
  - \c V is the vertex type, an 2-tuple of the number type.  
    It must be subscriptable.
  - \c IS is the indexed simplex type, a tuple of 3 integers.  
    It must be subscriptable.

  \note This function simply calls the above tiling function with a trivial
  level set function.
*/
template<typename T,    // number type
	 typename V,    // Vertex
	 typename IS>  // Indexed Simplex
inline
void
tile(const BBox<2,T>& domain, const T length,
     IndSimpSet<2,2,true,T,V,IS>* mesh) {
  tile(domain, length, ads::constructUnaryConstant<V>(-1.0), mesh);
}




//! Tile the object with a body-centered cubic lattice.
/*!
  \relates IndSimpSet

  \param domain is the rectilinear domain to tile.
  \param length is the maximum tetrahedron edge length.
  \param f is the level set description of the object.
  \param mesh is the indexed simplex set.

  \image html bcc0.jpg "A BCC block with 12 tetrahedra."
  \image latex bcc0.jpg "A BCC block with 12 tetrahedra." width=0.5\textwidth

  The template parameters can be deduced from the arguments.
  - \c T is the number type.
  - \c V is the vertex type, an 3-tuple of the number type.  
    It must be subscriptable.
  - \c IS is the indexed simplex type, a tuple of 4 integers.  
    It must be subscriptable.
  - \c LSF is the level set function that describes the object.  Negative
    values are inside.
*/
template<typename T,    // number type
	 typename V,    // Vertex
	 typename IS,   // Indexed Simplex
	 class LSF>    // Level Set Function
void
tile(const BBox<3,T>& domain, const T length, const LSF& f,
     IndSimpSet<3,3,true,T,V,IS>* mesh);


//! Tile the rectilinear region with a body-centered cubic lattice.
/*!
  \relates IndSimpSet

  \param domain is the rectilinear domain to tile.
  \param length is the maximum tetrahedron edge length.
  \param mesh is the indexed simplex set.

  The template parameters can be deduced from the arguments.
  - \c T is the number type.
  - \c V is the vertex type, an 3-tuple of the number type.  
    It must be subscriptable.
  - \c IS is the indexed simplex type, a tuple of 4 integers.  
    It must be subscriptable.

  \note This function simply calls the above tiling function with a trivial
  level set function.
*/
template<typename T,    // number type
	 typename V,    // Vertex
	 typename IS>  // Indexed Simplex
inline
void
tile(const BBox<3,T>& domain, const T length,
     IndSimpSet<3,3,true,T,V,IS>* mesh) {
  tile(domain, length, ads::constructUnaryConstant<V>(-1.0), mesh);
}

//@}

END_NAMESPACE_GEOM

#define __geom_mesh_iss_tile_ipp__
#include "tile.ipp"
#undef __geom_mesh_iss_tile_ipp__

#endif
