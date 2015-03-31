// -*- C++ -*-

/*!
  \file ads/indexedPriorityQueue/IndexedPriorityQueueLinearSearchPartitionPropensities.h
  \brief Indexed priority queue that uses linear search.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearchPartitionPropensities_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearchPartitionPropensities_h__

#include "IndexedPriorityQueueBase.h"

#include <algorithm>

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses linear search.
/*!
  \param _Base is the base class.
*/
template<class _Base = IndexedPriorityQueueBase<> >
class IndexedPriorityQueueLinearSearchPartitionPropensities :
  public _Base {
  //
  // Enumerations.
  //
public:

  enum {UsesPropensities = true};

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
  const ads::Array<1, Key>* _propensities;
  PartitionPredicate _partitionPredicate;
  Key _costConstant;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the propensities and the initial time.
  IndexedPriorityQueueLinearSearchPartitionPropensities(const int size) :
    Base(size),
    _partitionEnd(getQueueBeginning()),
    _splittingValue(- std::numeric_limits<Key>::max()),
    _propensities(0),
    _partitionPredicate(),
    _costConstant() {
    // sqrt((partition) / (search and update))
    // I determined this constant with a test on 1000 unit propensities.
    setCostConstant(2.75);
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
    // While the partition is empty.
    while (_partitionEnd == getQueueBeginning()) {
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
    // The element is in the lower partition.
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
    // Partition the elements in the queue.
    _partitionPredicate._value = _splittingValue;
    _partitionEnd = std::partition(getQueueBeginning(), getQueueEnd(),
				   _partitionPredicate);
    // Recompute the indices.
    Base::recomputeIndices();
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
