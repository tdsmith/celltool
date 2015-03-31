// -*- C++ -*-

/*! 
  \file geom/spatialIndexing/DistributedOrthtree.h
  \brief An orthtree that uses distributed-memory concurrency.
*/

#if !defined(__geom_spatialIndexing_DistributedOrthtree_h__)
#define __geom_spatialIndexing_DistributedOrthtree_h__

#include "SpatialIndex.h"

#include "../../ads/array/Array.h"
#include "../../numerical/partition.h"
#include "../../third-party/loki/static_check.h"

#include <mpi.h>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_DistributedOrthtree)
#define DEBUG_geom_spatialIndexing_DistributedOrthtree
#endif

BEGIN_NAMESPACE_GEOM

//! An orthtree that uses distributed-memory concurrency.
/*!
  \param _Orthtree The orthtree data structure.
*/
template<class _Orthtree>
class
DistributedOrthtree :
  public _Orthtree {
  //
  // Private types.
  //
private:

  typedef _Orthtree Base;

  //
  // Public types.
  //
public:

  //! The key type is the spatial index.
  typedef typename Base::key_type Key;
  //! The element type.
  typedef typename Base::mapped_type Element;

  //
  // More public types.
  //
public:

  //! The number type.
  typedef typename Base::Number Number;
  //! A Cartesian point.
  typedef typename Base::Point Point;
  //! The refinement functor.
  typedef typename Base::Split Split;
  //! The coarsening functor.
  typedef typename Base::Merge Merge;
  //! The default refinement predicate.
  typedef typename Base::Refine Refine;
  //! The default coarsening predicate.
  typedef typename Base::Coarsen Coarsen;
  //! The default action on elements.
  typedef typename Base::Action Action;

  //! The value type for the map.
  typedef typename Base::value_type value_type;
  //! An iterator in the map.
  typedef typename Base::iterator iterator;
  //! A const iterator in the map.
  typedef typename Base::const_iterator const_iterator;

  //
  // Protected types.
  //
protected:

  typedef typename Base::Level Level;
  typedef typename Base::Coordinate Coordinate;
  typedef typename Base::Code Code;

  //
  // Member data.
  //
private:

  MPI::Intracomm _communicator;
  ads::Array<1, Code> _delimiters;
  // CONTINUE

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Make an empty tree.
  DistributedOrthtree(const MPI::Intracomm& communicator,
		      const Point& lowerCorner, const Point& extents,
		      Split split = Split(),
		      Merge merge = Merge(),
		      Refine refine = Refine(),
		      Coarsen coarsen = Coarsen(),
		      Action action = Action());

  //! Destructor.
  ~DistributedOrthtree() {
    _communicator.Free();
  }

private:

  //! Copy constructor not implemented.
  DistributedOrthtree(const DistributedOrthtree&);

  //! Assignment operator not implemented.
  DistributedOrthtree&
  operator=(const DistributedOrthtree&);

  //@}
  //--------------------------------------------------------------------------
  //! \name Operations on all nodes.
  //@{
public:

  //! Apply the default function to the element of each node.
  using Base::apply;

  //! Perform refinement.
  using Base::refine;

  //! Perform coarsening.
  using Base::coarsen;

  //! Perform refinement to balance the tree.
  using Base::balance;

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor accessors.
  //@{
public:

  //! Get a const reference to the default splitting functor.
  using Base::getSplit;

  //! Get a const reference to the default merging functor.
  using Base::getMerge;

  //! Get a const reference to the default refinement predicate.
  using Base::getRefine;

  //! Get a const reference to the default coarsening predicate.
  using Base::getCoarsen;

  //! Get a const reference to the default action.
  using Base::getAction;

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor manipulators.
  //@{
public:

  //! Get a reference to the default splitting functor.
  //using Base::getSplit;

  //! Get a reference to the default merging functor.
  //using Base::getMerge;

  //! Get a reference to the default refinement predicate.
  //using Base::getRefine;

  //! Get a reference to the default coarsening predicate.
  //using Base::getCoarsen;

  //! Get a reference to the default action.
  //using Base::getAction;

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  using Base::begin;
  using Base::end;
  using Base::size;
  using Base::max_size;
  using Base::empty;

  //! Compute the lower corner of the leaf.
  using Base::computeLowerCorner;

  //! Get the Cartesian extents of the leaf.
  using Base::getExtents;

  //! Return true if the element can be refined.
  using Base::canBeRefined;
  
  //! Get the keys that are parents of 2^Dimension leaves.
  using Base::getParentKeys;

  //! Get the keys that are parents of 2^Dimension leaves and would result in a balanced tree under merging.
  using Base::getParentKeysBalanced;

  //! Return true if the tree is balanced.
  using Base::isBalanced;

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  //! Return true if the data structures are equal.
  bool
  operator==(const DistributedOrthtree& other) {
    return static_cast<const Base&>(*this) == static_cast<const Base&>(other);
    // CONTINUE
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Insert a value type.
  using Base::insert;

  //! Erase a value.
  using Base::erase;

  //@}
  //--------------------------------------------------------------------------
  //! \name Search.
  //@{
public:

  //! Find a value.
  using Base::find;

  //! Count the occurences of the value.
  using Base::count;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print the keys and the elements.
  using Base::print;

  //@}
};

END_NAMESPACE_GEOM

#define __geom_spatialIndexing_DistributedOrthtree_ipp__
#include "DistributedOrthtree.ipp"
#undef __geom_spatialIndexing_DistributedOrthtree_ipp__

#endif
