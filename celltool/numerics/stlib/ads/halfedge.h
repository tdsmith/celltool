// -*- C++ -*-

/*! 
  \file halfedge.h
  \brief Includes the halfedge data structure classes.
*/

/*!
  \page ads_halfedge Halfedge Data Structure Package

  The halfedge package has the halfedge data structure, ads::HalfedgeDS.
  Vertices, halfedges and faces should derive from the classes:
  - ads::HDSVertex
  - ads::HDSHalfedge
  - ads::HDSFace
  .
  To use these, include the file halfedge.h.
 */

#if !defined(__ads_halfedge_h__)
#define __ads_halfedge_h__

#include "halfedge/HalfedgeDS.h"
#include "halfedge/HDSVertex.h"
#include "halfedge/HDSHalfedge.h"
#include "halfedge/HDSFace.h"

#endif
