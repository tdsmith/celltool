// -*- C++ -*-

/*!
  \file ads/indexedPriorityQueue/IndexedPriorityQueuePartitionLinearSearchPropensities.h
  \brief Indexed priority queue that uses linear search on a partition.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueuePartitionLinearSearchPropensities_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueuePartitionLinearSearchPropensities_h__

#include "IndexedPriorityQueuePartitionLinearSearch.h"

#include <algorithm>

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses linear search on a fixed size partition.
/*!
  \param _Base is the base class.
*/
template<class _Base = IndexedPriorityQueueBase<> >
class IndexedPriorityQueuePartitionLinearSearchPropensities :
  public IndexedPriorityQueuePartitionLinearSearch<_Base> {
  //
  // Enumerations.
  //
public:

  enum {UsesPropensities = true};

  //
  // Private types.
  //
private:

  typedef IndexedPriorityQueuePartitionLinearSearch<_Base> Base;

  //
  // Public types.
  //
public:

  //! The key type.
  typedef typename Base::Key Key;

  //
  // Member data.
  //
private:

  using Base::_keys;
  using Base::_indices;
  using Base::_queue;
  using Base::_compare;
  using Base::_topIndex;
  using Base::_partitionEnd;
  using Base::_splittingValue;

  const ads::Array<1, Key>* _propensities;
  Key _costConstant;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueuePartitionLinearSearchPropensities(const int size) :
    Base(size),
    _propensities(0),
    // sqrt((partition) / (search and update))
    // I determined this constant with a test on 1000 unit propensities.
    _costConstant(0) {
    setCostConstant(1.75);
  }

  //! Store a pointer to the propensities array.
  void
  setPropensities(const ads::Array<1, Key>* propensities) {
    _propensities = propensities;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Return the key of the specified element.
  using Base::get;

private:

  //! Return the beginning of the queue.
  using Base::getQueueBeginning;

  //! Return the end of the queue.
  using Base::getQueueEnd;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Return the index of the top element.
  int
  top() {
    // While the partition is empty.
    while (_partitionEnd == getQueueBeginning()) {
      // Generate a new partition.
      partition();
    }
    // Return the top element.
    return Base::top();
  }

  //! Pop the top element off the queue.
  using Base::popTop;

  //! Pop the element off the queue.
  using Base::pop;

  //! Push the top value into the queue.
  using Base::pushTop;

  //! Push the value into the queue.
  using Base::push;

  //! Change the value in the queue.
  using Base::set;

  //! Clear the queue.
  using Base::clear;
  
  //! Set the constant used to balance costs.
  void
  setCostConstant(const Key costConstant) {
    _costConstant = std::sqrt(Key(_keys.size())) * costConstant;
  }

private:

  //! Generate a new partitioning of the queue.
  void
  partition() {
#ifdef DEBUG_ads
    assert(_propensities != 0);
#endif
    // If this is the first time we are generating a partition.
    if (_splittingValue == - std::numeric_limits<Key>::max()) {
      // We don't have an old value for _splittingValue so we compute it from
      // the keys.
      _splittingValue = *std::min_element(_keys.begin(), _keys.end());
    }

    const Key sum = std::accumulate(_propensities->begin(),
				    _propensities->end(), Key(0));

    // If there are no non-zero propensities.
    if (sum == 0) {
      // Put one element in the partition and return.
      _partitionEnd = getQueueBeginning() + 1;
      return;
    }

    // Balance the costs of partitioning and updating.
    _splittingValue += _costConstant / sum;

    // Put the elements in the lower partition in the queue.
    Base::buildLowerPartition();
  }

  //@}
};

END_NAMESPACE_ADS

#endif
