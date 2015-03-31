// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueueLinearSearchPartitionAdaptive.h
  \brief Indexed priority queue that uses linear search.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearchPartitionAdaptive_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearchPartitionAdaptive_h__

#include "IndexedPriorityQueueBase.h"

// For partition.
#include <algorithm>

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses linear search.
/*!
  \param _Base is the base class.
*/
template<class _Base = IndexedPriorityQueueBase<> >
class IndexedPriorityQueueLinearSearchPartitionAdaptive :
  public _Base {
  //
  // Enumerations.
  //
public:

  enum {UsesPropensities = false};

  //
  // Private types.
  //
private:

  typedef _Base Base;
  typedef typename Base::Iterator Iterator;

  //
  // Public types.
  //
public:

  //! The key type.
  typedef typename Base::Key Key;

  //
  // Nested classes.
  //
private:

  struct PartitionPredicate {
    Key _value;

    bool
    operator()(Iterator i) const {
      return *i < _value;
    }
  };

  //
  // Member data.
  //
private:

  using Base::_keys;
  using Base::_indices;
  //using Base::_queue;
  using Base::_compare;
  using Base::_topIndex;

  typename ads::Array<1, Iterator>::iterator _partitionEnd;
  Key _splittingValue;
  Key _splittingOffset;
  int _partitionSize;
  int _targetPartitionSize;
  PartitionPredicate _partitionPredicate;
  Key _costConstant;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueueLinearSearchPartitionAdaptive(const int size) :
    Base(size),
    _partitionEnd(getQueueBeginning()),
    _splittingValue(- std::numeric_limits<Key>::max()),
    _splittingOffset(0),
    _partitionSize(0),
    _targetPartitionSize(0),
    _partitionPredicate(),
    // sqrt((partition) / (search and update))
    // I determined this constant with a test on 1000 unit propensities.
    _costConstant(1.75) {
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

  //! Return true if the element is in the lower partition.
  bool
  isInLower(const int index) const {
    return getQueueBeginning() + _indices[index] < _partitionEnd;
  }

  //! Return true if the element should be in the lower partition.
  bool
  shouldBeInLower(const int index) const {
    // Use < instead of <= because the splitting value might be infinity.
    return _keys[index] < _splittingValue;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Return the index of the top element.
  int
  top() {
#ifdef DEBUG_ads
    assert(! _keys.empty());
#endif
    // If the partition is empty.
    if (_partitionEnd == getQueueBeginning()) {
      // Generate a new partition.
      partition();
    }
    // Find the minimum element with a linear search.
    return _topIndex = 
      *std::min_element(getQueueBeginning(), _partitionEnd, _compare) - 
      _keys.begin();
  }

  //! Pop the top element off the queue.
  void 
  popTop() {
#ifdef DEBUG_ads
    // If the element is in the lower partition.
    assert(isInLower(_topIndex));
#endif
    Base::popTop();
    // Move it into the upper partition.
    moveToUpper(_topIndex);
  }

  //! Pop the element off the queue.
  void 
  pop(const int index) {
    Base::pop(index);
    // If the element is in the lower partition.
    if (isInLower(index)) {
      // Move it into the upper partition.
      moveToUpper(index);
    }
  }

  //! Push the top value into the queue.
  void
  pushTop(const Key key) {
    push(_topIndex, key);
  }

  //! Push the value into the queue.
  void
  push(const int index, const Key key) {
    Base::push(index, key);
    moveToCorrectPartition(index);
  }

  //! Change the value in the queue.
  void
  set(const int index, const Key key) {
    Base::set(index, key);
    moveToCorrectPartition(index);
  }

  //! Clear the queue.
  void
  clear() {
    Base::clear();
    _partitionEnd = getQueueBeginning();
  }

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
      std::max(1, std::min(int(_costConstant * std::sqrt(_keys.size())), 
			   _keys.size()));
  }

  //! Generate a new partitioning of the queue.
  void
  partition() {
    // Determine a new splitting offset to try to get the desired 
    // partition size.
    _splittingOffset *= Key(_targetPartitionSize) / Key(_partitionSize);
    _splittingValue += _splittingOffset;
    // Partition the elements in the queue.
    _partitionPredicate._value = _splittingValue;
    _partitionEnd = std::partition(getQueueBeginning(), getQueueEnd(),
				   _partitionPredicate);
    // If the partition is empty.
    if (_partitionEnd == getQueueBeginning()) {
      // Generate a partition with sqrt(n) elements.
      partitionInitial();
    }
    _partitionSize = _partitionEnd - getQueueBeginning();

    // Recompute the indices.
    Base::recomputeIndices();
  }

  //! Generate an initial partitioning of the queue.
  void
  partitionInitial() {
    // Partition the elements in the queue.
    std::nth_element(getQueueBeginning(), 
		     getQueueBeginning() + _targetPartitionSize - 1,
		     getQueueEnd(), _compare);

    _partitionEnd = getQueueBeginning() + _targetPartitionSize;

    _splittingValue = **(_partitionEnd - 1);
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

  //! Move the element into the upper partition.
  /*!
    \pre The element must be in the lower partition.
  */
  void
  moveToUpper(const int index) {
#ifdef DEBUG_ads
    assert(_indices[index] < _partitionEnd - getQueueBeginning());
#endif
    --_partitionEnd;
    Base::swap(getQueueBeginning() + _indices[index], _partitionEnd);
  }

  //! Move the element into the lower partition.
  /*!
    \pre The element must be in the upper partition.
  */
  void
  moveToLower(const int index) {
#ifdef DEBUG_ads
    assert(_indices[index] >= _partitionEnd - getQueueBeginning());
#endif
    Base::swap(getQueueBeginning() + _indices[index], _partitionEnd);
    ++_partitionEnd;
  }

  //! Move the element into the correct partition.
  void
  moveToCorrectPartition(const int index) {
    if (isInLower(index)) {
      if (! shouldBeInLower(index)) {
	moveToUpper(index);
      }
    }
    else {
      if (shouldBeInLower(index)) {
	moveToLower(index);
      }
    }
  }

  //@}
};

END_NAMESPACE_ADS

#endif
