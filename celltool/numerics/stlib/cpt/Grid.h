// -*- C++ -*-

/*! 
  \file Grid.h
  \brief Implements a class for the grid data.
*/

#if !defined(__cpt_Grid_h__)
#define __cpt_Grid_h__

// Local
#include "GridBase.h"
#include "Vertex.h"
#include "Face.h"

BEGIN_NAMESPACE_CPT


//! A class to hold the grid data.
template<int N, typename T = double>
class Grid;


//
// File I/O
//


//! Print the grid.
/*! \relates Grid */
template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const Grid<N,T>& g) {
  g.put(out);
  return out;
}


//
// Equality operators
//


//! Return true if the grids are equal.
/*! \relates Grid */
template<int N, typename T>
inline
bool 
operator==(const Grid<N,T>& a, const Grid<N,T>& b) {
  return (static_cast<const GridBase<N,T>&>(a) ==
	  static_cast<const GridBase<N,T>&>(b));
}


//! Return true if the grids are not equal.
/*! \relates Grid */
template<int N, typename T>
inline
bool 
operator!=(const Grid<N,T>& a, const Grid<N,T>& b) {
  return !(a == b);
}


END_NAMESPACE_CPT

#define __Grid1_ipp__
#include "Grid1.ipp"
#undef __Grid1_ipp__

#define __Grid2_ipp__
#include "Grid2.ipp"
#undef __Grid2_ipp__

#define __Grid3_ipp__
#include "Grid3.ipp"
#undef __Grid3_ipp__

#endif
