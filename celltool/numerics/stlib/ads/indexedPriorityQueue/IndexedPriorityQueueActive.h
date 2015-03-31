// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueueActive.h
  \brief Indexed priority queue that partitions the active and inactive elements.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueActive_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueActive_h__

#include "IndexedPriorityQueueBase.h"

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that partitions the active and inactive elements.
/*!
  \param Key is the key type.
*/
template<typename _Key = double>
class IndexedPriorityQueueActive :
  public IndexedPriorityQueueBase<_Key> {
  //
  // Public types.
  //
public:

  //! The key type.
  typedef _Key Key;

  //
  // Private types.
  //
private:

  typedef IndexedPriorityQueueBase<Key> Base;
  typedef typename Base::Iterator Iterator;

  //
  // Member data.
  //
protected:

  using Base::_keys;
  using Base::_indices;
  using Base::_queue;
  using Base::_compare;

  //! The end of the active elements.
  typename ads::Array<1, Iterator>::iterator _activeEnd;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueueActive(const int size) :
    Base(size),
    _activeEnd(_queue.begin()) {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Return the key of the specified element.
  using Base::get;

  //! Return the beginning of the queue.
  using Base::getQueueBeginning;

  //! Return the end of the queue.
  typename ads::Array<1, Iterator>::const_iterator
  getQueueEnd() const {
    return _activeEnd;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Return the end of the queue.
  typename ads::Array<1, Iterator>::iterator
  getQueueEnd() {
    return _activeEnd;
  }

  //! Swap the two elements' positions in the queue.
  using Base::swap;

  //! Recompute the indices after the queue has changed.
  using Base::recomputeIndices;

  //! Pop the top element off the queue.
  void 
  popTop() {
#ifdef DEBUG_ads
    assert(_keys[Base::_topIndex] != std::numeric_limits<Key>::max());
#endif
    Base::popTop();
    moveToInactive(Base::_topIndex);
  }

  //! Pop the element off the queue.
  void 
  pop(const int index) {
    // If it is currently in the active queue.
    if (_keys[index] != std::numeric_limits<Key>::max()) {
      _keys[index] = std::numeric_limits<Key>::max();
      moveToInactive(index);
    }
  }

  //! Push the top value into the queue.
  using Base::pushTop;

  //! Push the value into the queue.
  void
  push(const int index, const Key key) {
    if (_keys[index] == std::numeric_limits<Key>::max()) {
#ifdef DEBUG_ads
      assert(key != std::numeric_limits<Key>::max());
#endif
      moveToActive(index);
    }
    _keys[index] = key;
  }

  //! Change the value in the queue.
  void
  set(const int index, const Key key) {
#ifdef DEBUG_ads
      assert(_keys[index] != std::numeric_limits<Key>::max() &&
	     key != std::numeric_limits<Key>::max());
#endif
    _keys[index] = key;
  }

  //! Clear the queue.
  void
  clear() {
    Base::clear();
    _activeEnd = _queue.begin();
  }

  //! Recompute the indices after the queue has changed.
  void
  recomputeIndices() {
    Base::recomputeIndices(getQueueBeginning(), getQueueEnd());
  }

private:

  //! Move the element into the inactive partition.
  /*!
    \pre The element must be in the active partition.
  */
  void
  moveToInactive(const int index) {
#ifdef DEBUG_ads
    assert(_indices[index] < _activeEnd - _queue.begin());
#endif
    --_activeEnd;
    Base::swap(_queue.begin() + _indices[index], _activeEnd);
  }

  //! Move the element into the active partition.
  /*!
    \pre The element must be in the inactive partition.
  */
  void
  moveToActive(const int index) {
#ifdef DEBUG_ads
    assert(_indices[index] >= _activeEnd - _queue.begin());
#endif
    Base::swap(_queue.begin() + _indices[index], _activeEnd);
    ++_activeEnd;
  }

  //@}
};

END_NAMESPACE_ADS

#endif
