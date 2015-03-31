// -*- C++ -*-

/*! 
  \file SimplexAdj.h
  \brief Simplex-simplex adjacencies in an indexed simplex set in N-D.
*/

#if !defined(__geom_SimplexAdj_h__)
#define __geom_SimplexAdj_h__

#include "../../defs.h"

#include "VertexSimplexInc.h"

#include <vector>
#include <iosfwd>

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexAdj)
#define DEBUG_SimplexAdj
#endif

BEGIN_NAMESPACE_GEOM

//! Simplex-simplex adjacencies in an M-D indexed simplex set.
/*!
  \param N is the simplex dimension.

  This class is used in IndSimpSetIncAdj to store the simplex-simplex 
  adjacencies.  Note that the space dimension is not relevant.  
  This class deals only with topological information.
*/
template<int M>
class SimplexAdj {
  //
  // Private types.
  //

private:

  //! The container for the adjacent simplex indices.
  typedef ads::FixedArray<M+1,int> IndexContainer;

  //! The container for the simplex-simplex adjacencies.
  typedef ads::Array<1,IndexContainer> AdjacenciesContainer;

  //
  // Data
  //

private:

  // The array of simplex-simplex adjacencies.
  AdjacenciesContainer _adj;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Empty adjacency data.
  SimplexAdj() :
    _adj()
  {}

  //! Copy constructor.
  SimplexAdj(const SimplexAdj& other) :
    _adj(other._adj)
  {}

  //! Assignment operator.
  SimplexAdj& 
  operator=(const SimplexAdj& other) {
    if (&other != this) {
      _adj = other._adj;
    }
    return *this;
  }
  
  //! Construct from the array of indexed simplices and the vertex-simplex incidences.
  /*!
    \c IS is an indexed simplex type, a tuple of M+1 integers.  
  */
  template<typename IS, bool A>
  SimplexAdj(const ads::Array<1,IS,A>& simplices,
	     const VertexSimplexInc<M>& vertexSimplexInc) {
    build(simplices, vertexSimplexInc);
  }

  //! Build the vertex-simplex adjacencies structure.
  template<typename IS, bool A>
  void
  build(const ads::Array<1,IS,A>& simplices,
	const VertexSimplexInc<M>& vertexSimplexInc);

  //! Construct from the number of vertices and the array of indexed simplices.
  /*!
    \c IS is an indexed simplex type, a tuple of M+1 integers.  
  */
  template<typename IS, bool A>
  SimplexAdj(const int numVertices, const ads::Array<1,IS,A>& simplices) {
    build(numVertices, simplices);
  }

  //! Build the vertex-simplex adjacencies structure.
  template<typename IS, bool A>
  void
  build(const int numVertices, const ads::Array<1,IS,A>& simplices) {
    VertexSimplexInc<M> vsi(numVertices, simplices);
    build(simplices, vsi);
  }

  //! Swap data.
  void
  swap(SimplexAdj& x) {
    _adj.swap(x._adj);
  }

  //! Destructor.  Leave cleaning up to the containers.
  ~SimplexAdj()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return adjacencies of the n_th simplex.
  const ads::FixedArray<M+1,int>&
  operator()(const int n) const {
    return _adj[n];
  }

  //! Return m_th adjacent simplex to the n_th simplex.
  int
  operator()(const int n, const int m) const {
    return _adj[n][m];
  }

  //! Return number of simplices.
  int
  getSize() const {
    return _adj.size();
  }

  //! Return number of simplices adjacent to the n_th simplex.
  int
  getSize(const int n) const {
    return M + 1 - int(std::count(_adj[n].begin(), _adj[n].end(), -1));
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Return adjacencies of the n_th simplex.
  ads::FixedArray<M+1,int>&
  operator()(const int n) {
    return _adj[n];
  }

  //! Set the m_th adjacent simplex to the n_th simplex.
  void
  set(const int n, const int m, const int index) {
    _adj[n][m] = index;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{

  //! Return true if the adjacencies are the same.
  bool
  operator==(const SimplexAdj& x) const {
    return _adj == x._adj;
  }

  //! Return true if the adjacencies are not the same.
  bool
  operator!=(const SimplexAdj& x) const {
    return ! operator==(x);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Write the simplex-simplex adjacencies.
  void
  put(std::ostream& out) const {
    out << _adj;
  }

  //@}
};


//
// File output.
//


//! Write the simplex adjacencies.
template<int M>
inline
std::ostream&
operator<<(std::ostream& out, const SimplexAdj<M>& x) {
  x.put(out);
  return out;
}

END_NAMESPACE_GEOM

#define __geom_SimplexAdj_ipp__
#include "SimplexAdj.ipp"
#undef __geom_SimplexAdj_ipp__

#endif
