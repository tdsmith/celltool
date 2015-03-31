// -*- C++ -*-

/*!
  \file ads/indexedPriorityQueue/IndexedPriorityQueuePartitionLinearSearchSizeAdaptive.h
  \brief Indexed priority queue that uses linear search on a partition.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueuePartitionLinearSearchSizeAdaptive_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueuePartitionLinearSearchSizeAdaptive_h__

#include "IndexedPriorityQueuePartitionLinearSearch.h"

#include <algorithm>

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses linear search on a fixed size partition.
/*!
  \param _Base is the base class.
*/
template<class _Base = IndexedPriorityQueueBase<> >
class IndexedPriorityQueuePartitionLinearSearchSizeAdaptive :
  public IndexedPriorityQueuePartitionLinearSearch<_Base> {
  //
  // Enumerations.
  //
public:

  enum {UsesPropensities = false};

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

  Key _splittingOffset;
  int _partitionSize;
  int _targetPartitionSize;
  Key _costConstant;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueuePartitionLinearSearchSizeAdaptive(const int size) :
    Base(size),
    _splittingOffset(0),
    // Set these two so partition will do something reasonable to start with.
    _partitionSize(1),
    _targetPartitionSize(0),
    // sqrt((partition) / (search and update))
    // I determined this constant with a test on 1000 unit propensities.
    _costConstant(0.8) {
    computeTargetPartitionSize();
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
    // If the partition is empty.
    if (_partitionEnd == getQueueBeginning()) {
      // Generate a new partition.
      partition();
    }
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
    _costConstant = costConstant;
    computeTargetPartitionSize();
  }

private:

  //! Compute the initial partition size.
  /*!
    We choose a partition size to balance the costs of searching and 
    partitioning.  Let M be the number of elements and m be the partition
    size.  The cost of search and updating is O(m).  We could expect 
    O(m) searches before the lower partition is empty.  The cost of
    partitioning in O(M).  We balance the two costs.
    \f[
    (\mathrm{search and update}) m^2 = (\mathrm{partition}) M
    \f]
    \f[
    m = \sqrt{(\mathrm{partition})}{(\mathrm{search and update})} \sqrt{M}
    \f]
   */
  void
  computeTargetPartitionSize() {
    _targetPartitionSize = 
      std::max(2, std::min(int(_costConstant * std::sqrt(_keys.size())), 
			   _keys.size()));
  }

  //! Generate a new partitioning of the queue.
  void
  partition() {
    // Determine a new splitting offset to try to get the desired 
    // partition size.
    _splittingOffset *= Key(_targetPartitionSize) / Key(_partitionSize);
    _splittingValue += _splittingOffset;
    // Put the elements in the lower partition in the queue.
    Base::buildLowerPartition();
    // If the partition is empty.
    if (_partitionEnd == getQueueBeginning()) {
      // Generate a partition with sqrt(n) elements.
      partitionInitial();
    }
    _partitionSize = _partitionEnd - getQueueBeginning();
  }

  //! Generate a new partitioning of the queue.
  void
  partitionInitial() {
    // Copy the keys.
    ads::Array<1, Key> nthElement(_keys);
    // Determine the n_th value.
    std::nth_element(nthElement.begin(), 
		     nthElement.begin() + _targetPartitionSize - 1,
		     nthElement.end());

    // Use the n_th value as the splitting value.
    _splittingValue = nthElement[_targetPartitionSize - 1];

    // Put the elements in the lower partition in the queue.
    Base::buildLowerPartition();

    // Determine the splitting offset.
    _splittingOffset = _splittingValue - 
      **std::min_element(getQueueBeginning(), _partitionEnd,
			 _compare);
    // If we got a bad splitting offset value.
    if (_splittingOffset == 0) {
      // Set it to unity.  It will adapt to something reasonable after a 
      // few partitions.
      _splittingOffset = 1;
    }
  }

  //@}
};

END_NAMESPACE_ADS

#endif
