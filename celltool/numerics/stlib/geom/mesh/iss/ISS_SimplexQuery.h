// -*- C++ -*-

/*! 
  \file ISS_SimplexQuery.h
  \brief Simplex queries on an indexed simplex set.
*/

#if !defined(__geom_ISS_SimplexQuery_h__)
#define __geom_ISS_SimplexQuery_h__

#include "../simplex/simplex_distance.h"

#include "../../tree/BBoxTree.h"

#include "../../../ads/array/Array.h"

#if defined(DEBUG_geom) && !defined(DEBUG_ISS_SimplexQuery)
#define DEBUG_ISS_SimplexQuery
#endif

BEGIN_NAMESPACE_GEOM

//! Simplex queries on an indexed simplex set.
/*!
  \param ISS is the indexed simplex set.

  This class stores a constant reference to an indexed simplex set.
*/
template<class ISS>
class ISS_SimplexQuery {
  //
  // Private types.
  //

private:

  //! The indexed simplex set.
  typedef ISS IssType;

  //! The (un-indexed) simplex type.
  typedef typename IssType::Simplex Simplex;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename IssType::Number Number;
  //! A vertex.
  typedef typename IssType::Vertex Vertex;
  //! A bounding box.
  typedef geom::BBox<ISS::N,Number> BBox;

  //
  // Member data.
  //

private:

  //! The indexed simplex set.
  const IssType& _iss;
  //! Bounding box tree.
  BBoxTree<ISS::N,Number> _bboxTree;
  

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  ISS_SimplexQuery();

  //! Copy constructor not implemented
  ISS_SimplexQuery(const ISS_SimplexQuery&);

  //! Assignment operator not implemented
  ISS_SimplexQuery& 
  operator=(const ISS_SimplexQuery&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Construct from the indexed simplex set.
  /*!
    \param iss is the indexed simplex set.
  */
  ISS_SimplexQuery(const IssType& iss) :
    _iss(iss),
    _bboxTree() {
    build();
  }

  //! Build the bounding box tree.  Call this after the simplicial complex changes.
  void
  build();

  //! Destructor has no effect on the indexed simplex set.
  ~ISS_SimplexQuery()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Queries
  //! @{

  //! Get the indices of the simplices that contain the point.
  template<typename IntOutIter>
  void
  computePointQuery(IntOutIter iter, const Vertex& x) const;
  
  //! Get the indices of the simplices whose bounding boxes overlap the window.
  template<typename IntOutIter>
  void
  computeWindowQuery(IntOutIter iter, const BBox& window) const;

  //! Return the index of the simplex of minimum distance.
  /*!
    If M = N, the signed distance is used.  If M < N, we use the unsigned
    distance.
  */
  int
  computeMinimumDistanceAndIndex(const Vertex& x, Number* minDistance) const;

  //! Return the index of the simplex of minimum distance.
  /*!
    If M = N, the signed distance is used.  If M < N, we use the unsigned
    distance.
  */
  int
  computeMinimumDistanceIndex(const Vertex& x) const {
    Number minDistance;
    return computeMinimumDistanceAndIndex(x, &minDistance);
  }

  //! Return the minimum distance.
  /*!
    If M = N, the signed distance is used.  If M < N, we use the unsigned
    distance.
  */
  Number
  computeMinimumDistance(const Vertex& x) const;

  //! @}
};

END_NAMESPACE_GEOM

#define __geom_ISS_SimplexQuery_ipp__
#include "ISS_SimplexQuery.ipp"
#undef __geom_ISS_SimplexQuery_ipp__

#endif
