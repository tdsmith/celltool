// -*- C++ -*-

/*! 
  \file RegularGrid.ipp
  \brief A class for a regular grid.
*/

#if !defined(__geom_RegularGrid_ipp__)
#error This file is an implementation detail of the class RegularGrid.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//


template<int N, typename T>
inline
RegularGrid<N,T>::
RegularGrid(const Index& extents, const BBox& domain) : 
  _extents(extents),
  _domain(domain),
  _length(domain.getUpperCorner() - domain.getLowerCorner()),
  _delta(),
  _indexEpsilon(ads::computeMaximum(extents)* 
		std::numeric_limits<T>::epsilon()),
  _cartesianEpsilon(ads::computeMaximum(_length) * 
		    std::numeric_limits<T>::epsilon()) {
  Point den;
  den = _extents;
  den -= SizeType(1);
  if (ads::computeProduct(den) != 0) {
    _delta = _length / den;
  }
  else {
    _delta = 1;
    _cartesianEpsilon = std::numeric_limits<T>::epsilon();
  }
}

  
template<int N, typename T>
inline
RegularGrid<N,T>::
RegularGrid(const RegularGrid<N,T>& other) :
  _extents(other._extents),
  _domain(other._domain),
  _length(other._length),
  _delta(other._delta),
  _indexEpsilon(other._indexEpsilon),
  _cartesianEpsilon(other._cartesianEpsilon)
{}
    

template<int N, typename T>
inline
RegularGrid<N,T>& 
RegularGrid<N,T>::
operator=(const RegularGrid<N,T>& other) {
  // Avoid assignment to self
  if (&other != this) {
    _extents = other._extents;
    _domain = other._domain;
    _length = other._length;
    _delta = other._delta;
    _indexEpsilon = other._indexEpsilon;
    _cartesianEpsilon = other._cartesianEpsilon;
  }
  // Return *this so assignments can chain
  return *this;
}


//
// Equality
//

    
//! Return true if the true RegularGrid's are equal.
template<int N, typename T>
inline
bool 
operator==(const RegularGrid<N,T>& a, const RegularGrid<N,T>& b) {
  if (a.getExtents() != b.getExtents()) {
    return false;
  }
  if (a.getDelta() != b.getDelta()) {
    return false;
  }
  if (a.getDomain() != b.getDomain()) {
    return false;
  }
  if (a.getIndexEpsilon() != b.getIndexEpsilon()) {
    return false;
  }
  if (a.getCartesianEpsilon() != b.getCartesianEpsilon()) {
    return false;
  }
  return true;
}


//
// File I/O
//


template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const RegularGrid<N,T>& grid) {
  out << "Dimensions: " << grid.getExtents() << '\n';
  out << "Domain:" << '\n' << grid.getDomain() << '\n';
  return out;
}


//! Read from a file stream.
/*! \relates RegularGrid */
template<int N, typename T>
inline
std::istream& 
operator>>(std::istream& in, RegularGrid<N,T>& grid) {
  typename RegularGrid<N, T>::Index extents;
  typename RegularGrid<N, T>::BBox domain;
  in >> extents >> domain;
  grid = RegularGrid<N, T>(extents, domain);
  return in;
}

END_NAMESPACE_GEOM
