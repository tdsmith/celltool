// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueuePartitionLinearSearch.h
  \brief Indexed priority queue that uses linear search on a partition.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueuePartitionLinearSearch_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueuePartitionLinearSearch_h__

#include "IndexedPriorityQueueBase.h"

#include <algorithm>

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses linear search.
/*!
  \param _Base is the base class.
*/
template<class _Base = IndexedPriorityQueueBase<> >
class IndexedPriorityQueuePartitionLinearSearch :
  public _Base {
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
  // Member data.
  //
protected:

  using Base::_keys;
  using Base::_indices;
  using Base::_queue;
  using Base::_compare;
  using Base::_topIndex;

  //! The end of the partition.
  typename ads::Array<1, Iterator>::iterator _partitionEnd;
  //! The splitting value for the partition.
  Key _splittingValue;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueuePartitionLinearSearch(const int size) :
    Base(size),
    _partitionEnd(getQueueBeginning()),
    _splittingValue(- std::numeric_limits<Key>::max()) {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Return the key of the specified element.
  using Base::get;

protected:

  //! Return the beginning of the queue.
  using Base::getQueueBeginning;

  //! Return the end of the queue.
  using Base::getQueueEnd;

private:

  //! Return true if the value is in the lower partition.
  bool
  isInLower(const Key key) const {
    // Use < instead of <= because the splitting value might be infinity.
    return key < _splittingValue;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Return the index of the top element.
  /*!
    The derived class must re-partition if the partition is empty.
  */
  int
  top() {
#ifdef DEBUG_ads
    assert(! _keys.empty());
#endif
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
    assert(isInLower(_keys[_topIndex]));
#endif
    // Move it into the upper partition.
    moveToUpper(_topIndex);
    // Set the value to infinity.
    Base::popTop();
  }

  //! Pop the element off the queue.
  void 
  pop(const int index) {
    // If the element is in the lower partition.
    if (isInLower(_keys[index])) {
      // Move it into the upper partition.
      moveToUpper(index);
    }
    // Set the value to infinity.
    Base::pop(index);
  }

  //! Push the top value into the queue.
  void
  pushTop(const Key key) {
    push(_topIndex, key);
  }

  //! Push the value into the queue.
  void
  push(const int index, const Key key) {
    // Move to the correct partition for the new key value.
    moveToCorrectPartition(index, key);
    // Set the value.
    Base::push(index, key);
  }

  //! Change the value in the queue.
  void
  set(const int index, const Key key) {
    // Move to the correct partition for the new key value.
    moveToCorrectPartition(index, key);
    // Set the value.
    Base::set(index, key);
  }

  //! Clear the queue.
  void
  clear() {
    Base::clear();
    _partitionEnd = getQueueBeginning();
    _splittingValue = - std::numeric_limits<Key>::max();
  }

protected:

  //! Put the elements in the lower partition in the queue.
  void
  buildLowerPartition() {
    _partitionEnd = getQueueBeginning();
#if 0
    // The method below is faster.
    for (int i = 0; i != _keys.size(); ++i) {
      if (_keys[i] < _splittingValue) {
	_indices[i] = _partitionEnd - getQueueBeginning();
	*_partitionEnd = _keys.begin() + i;
	++_partitionEnd;
      }
    }
#endif
    int index = 0;
    ads::Array<1, int>::iterator indices = _indices.begin();
    Iterator end = _keys.end();
    for (Iterator i = _keys.begin(); i != end; ++i, ++indices) {
      if (*i < _splittingValue) {
	*_partitionEnd++ = i;
	*indices = index++;
      }
    }
  }

private:

  //! Move the element into the upper partition.
  /*!
    \pre The element must be in the lower partition.
  */
  void
  moveToUpper(const int index) {
#ifdef DEBUG_ads
    assert(isInLower(_keys[index]));
#endif
    --_partitionEnd;
    // The index of the last element in the lower partition.
    const int i = *_partitionEnd - _keys.begin();
    // Replace the element that is being moved to the upper partition.
    _indices[i] = _indices[index];
    _queue[_indices[i]] = _keys.begin() + i;
  }

  //! Move the element into the lower partition.
  /*!
    \pre The element must be in the upper partition.
  */
  void
  moveToLower(const int index) {
#ifdef DEBUG_ads
    assert(! isInLower(_keys[index]));
#endif
    // Put the element in the lower partition.
    *_partitionEnd = _keys.begin() + index;
    // Set the index of the element in the queue.
    _indices[index] = _partitionEnd - getQueueBeginning();
    ++_partitionEnd;
  }

  //! Move the element into the correct partition.
  void
  moveToCorrectPartition(const int index, const Key key) {
    // If the old element value was in the lower partition.
    if (isInLower(_keys[index])) {
      // If the new element value is in the upper partition.
      if (! isInLower(key)) {
	moveToUpper(index);
      }
    }
    // If the old element value was in the upper partition.
    else {
      // If the new element value is in the lower partition.
      if (isInLower(key)) {
	moveToLower(index);
      }
    }
  }

  //@}
};

END_NAMESPACE_ADS

#endif
