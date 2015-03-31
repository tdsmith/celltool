// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueueBase.h
  \brief Base class for indexed priority queues.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueBase_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueBase_h__

#include "../defs.h"

#include "../array/Array.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//! Base class for indexed priority queues.
/*!
  \param Key is the key type.
*/
template<typename _Key = double>
class IndexedPriorityQueueBase {
  //
  // Public types.
  //
public:

  //! The key type.
  typedef _Key Key;

  //
  // Protected types.
  //
protected:

  //! Iterator on the key type.
  typedef typename ads::Array<1, Key>::const_iterator Iterator;

  //
  // Nested classes.
  //
private:

  //! Compare two iterators.
  struct Compare {
    bool
    operator()(const Iterator x, const Iterator y) const {
      return *x < *y;
    }
  };

  //
  // Member data.
  //
protected:

  //! The array of keys.
  ads::Array<1, Key> _keys;
  //! The array of indices.
  ads::Array<1, int> _indices;
  //! The priority queue array.
  ads::Array<1, Iterator> _queue;
  //! The index of the top element.
  int _topIndex;
  //! Comparison functor.
  Compare _compare;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueueBase(const int size) :
    _keys(size),
    _indices(size),
    _queue(size),
    // Invalid index.
    _topIndex(-1) {
    clear();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Return the key of the specified element.
  Key
  get(const int index) const {
    return _keys[index];
  }

  //! Return the beginning of the queue.
  typename ads::Array<1, Iterator>::const_iterator
  getQueueBeginning() const {
    return _queue.begin();
  }

  //! Return the end of the queue.
  typename ads::Array<1, Iterator>::const_iterator
  getQueueEnd() const {
    return _queue.end();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Return the beginning of the queue.
  typename ads::Array<1, Iterator>::iterator
  getQueueBeginning() {
    return _queue.begin();
  }

  //! Return the end of the queue.
  typename ads::Array<1, Iterator>::iterator
  getQueueEnd() {
    return _queue.end();
  }

  //! Pop the top element off the queue.
  void 
  popTop() {
    pop(_topIndex);
  }

  //! Pop the element off the queue.
  void 
  pop(const int index) {
    _keys[index] = std::numeric_limits<Key>::max();
  }

  //! Push the top value into the queue.
  void
  pushTop(const Key key) {
    push(_topIndex, key);
  }

  //! Push the value into the queue.
  void
  push(const int index, const Key key) {
#ifdef DEBUG_ads
    assert(key != std::numeric_limits<Key>::max());
#endif
    _keys[index] = key;
  }

  //! Change the value in the queue.
  void
  set(const int index, const Key key) {
#ifdef DEBUG_ads
    assert(key != std::numeric_limits<Key>::max());
#endif
    _keys[index] = key;
  }

  //! Clear the priority queue.
  void
  clear() {
    for (int i = 0; i != _keys.size(); ++i) {
      _keys[i] = std::numeric_limits<Key>::max();
    }
    for (int i = 0; i != _indices.size(); ++i) {
      _indices[i] = i;
    }
    for (int i = 0; i != _queue.size(); ++i) {
      _queue[i] = _keys.begin() + i;
    }
    _topIndex = -1;
  }

  //! Swap the two elements' positions in the queue.
  void
  swap(const int i, const int j) {
    std::swap(_queue[_indices[i]], _queue[_indices[j]]);
    std::swap(_indices[i], _indices[j]);
  }

  //! Swap the two elements' positions in the queue.
  void
  swap(const typename ads::Array<1, Iterator>::iterator i,
       const typename ads::Array<1, Iterator>::iterator j) {
    std::swap(_indices[*i - _keys.begin()], _indices[*j - _keys.begin()]);
    std::swap(*i, *j);
  }

  //! Recompute the indices after the queue has changed.
  void
  recomputeIndices() {
    recomputeIndices(_queue.begin(), _queue.end());
  }

  //! Recompute the indices after the queue has changed.
  void
  recomputeIndices(typename ads::Array<1, Iterator>::const_iterator begin,
		   const typename ads::Array<1, Iterator>::const_iterator end) {
    for (int i = begin - _queue.begin(); begin != end; ++begin, ++i) {
      _indices[*begin - _keys.begin()] = i;
    }
  }

  //@}
};

END_NAMESPACE_ADS

#endif
