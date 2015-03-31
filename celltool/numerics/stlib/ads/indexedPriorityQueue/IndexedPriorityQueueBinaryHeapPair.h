// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueueBinaryHeapPair.h
  \brief Indexed priority queue that partitions the active and inactive elements.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueBinaryHeapPair_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueBinaryHeapPair_h__

#include "IndexedPriorityQueueBase.h"

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that partitions the active and inactive elements.
/*!
  \param Key is the key type.
*/
template<typename _Key = double>
class IndexedPriorityQueueBinaryHeapPair {
  //
  // Enumerations.
  //
public:

  enum {UsesPropensities = false};

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

  typedef std::pair<int, Key> Value;
  typedef ads::Array<1, Value> Queue;
  typedef typename Queue::const_iterator ConstIterator;
  typedef typename Queue::iterator Iterator;

  //
  // Nested classes.
  //
private:

  //! Compare two queue elements.
  struct Compare {
    bool
    operator()(const Value& x, const Value& y) const {
      return x.second < y.second;
    }
  };

  //
  // Member data.
  //
private:

  Queue _queue;
  ads::Array<1, Iterator> _pointers;
  Iterator _heapEnd;
  Compare _compare;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueueBinaryHeapPair(const int size) :
    _queue(size),
    _pointers(size),
    _heapEnd(_queue.begin()) {
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
    return _pointers[index]->second;
  }

private:

  //! Return the beginning of the queue.
  ConstIterator
  getQueueBeginning() const {
    return _queue.begin();
  }

  //! Return the end of the queue.
  ConstIterator
  getQueueEnd() const {
    return _heapEnd;
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
    assert(! _queue.empty());
#endif
    return _queue.begin()->first;
  }

  //! Pop the top element off the queue.
  void 
  popTop() {
#ifdef DEBUG_ads
    assert(_queue.begin()->second != std::numeric_limits<Key>::max());
#endif
    _queue.begin()->second = std::numeric_limits<Key>::max();
    remove(0);
  }

  //! Pop the element off the queue.
  void
  pop(const int index) {
#ifdef DEBUG_ads
    // It must be in the active queue.
    assert(_pointers[index]->second != std::numeric_limits<Key>::max());
#endif
    _pointers[index]->second = std::numeric_limits<Key>::max();
    remove(_pointers[index] - _queue.begin());
  }

  //! Push the value into the queue.
  void
  push(const int index, const Key key) {
#ifdef DEBUG_ads
    assert(_pointers[index]->second == std::numeric_limits<Key>::max() &&
	   key != std::numeric_limits<Key>::max());
#endif
    _pointers[index]->second = key;
    insert(index);
  }

  //! Push the value at the top into the queue.
  void
  pushTop(const Key key) {
#ifdef DEBUG_ads
    assert(key != std::numeric_limits<Key>::max());
#endif
    _queue[0].second = key;
#ifdef GIBSON_BRUCK_UPDATE
    updateRecursive(0);
#else
    pushDown(0);
#endif
  }

  //! Change the value in the queue.
  void
  set(const int index, const Key key) {
#ifdef DEBUG_ads
    assert(_pointers[index]->second != std::numeric_limits<Key>::max() &&
	   key != std::numeric_limits<Key>::max());
#endif
    // If we are using the Gibson and Bruck updating algorithm.
#ifdef GIBSON_BRUCK_UPDATE
    _pointers[index]->second = key;
    updateRecursive(_pointers[index] - getQueueBeginning());
#else
    if (key < _pointers[index]->second) {
      _pointers[index]->second = key;
      pushUp(_pointers[index] - getQueueBeginning());
    }
    else {
      _pointers[index]->second = key;
      pushDown(_pointers[index] - getQueueBeginning());
    }
#endif
  }

  //! Clear the queue.
  void
  clear() {
    for (int i = 0; i != _queue.size(); ++i) {
      _queue[i].first = i;
      _queue[i].second = std::numeric_limits<Key>::max();
    }
    for (int i = 0; i != _queue.size(); ++i) {
      _pointers[i] = _queue.begin() + i;
    }
    _heapEnd = _queue.begin();
  }

private:

  //! Return the beginning of the queue.
  Iterator
  getQueueBeginning() {
    return _queue.begin();
  }

  //! Return the end of the queue.
  Iterator
  getQueueEnd() {
    return _heapEnd;
  }

  //! Remove the element from the heap.
  /*!
    \pre The element must be in the heap.
  */
  void
  remove(const int n) {
#ifdef DEBUG_ads
    assert(n < _heapEnd - getQueueBeginning());
#endif
    --_heapEnd;
    swap(_queue.begin() + n, _heapEnd);
#ifdef GIBSON_BRUCK_UPDATE
    updateRecursive(n);
#else
    pushDown(n);
#endif
  }

  //! Insert the element in the heap.
  /*!
    \pre The element must not be in the heap.
  */
  void
  insert(const int index) {
#ifdef DEBUG_ads
    assert(_pointers[index] >= _heapEnd);
#endif
    swap(_pointers[index], _heapEnd);
#ifdef GIBSON_BRUCK_UPDATE
    updateRecursive(_heapEnd - getQueueBeginning());
#else
    pushUp(_heapEnd - getQueueBeginning());
#endif
    ++_heapEnd;
  }

  void
  pushUp(ptrdiff_t child) {
    ptrdiff_t parent = (child - 1) / 2;
    while (child > 0 && _compare(_queue[child], _queue[parent])) {
      swap(getQueueBeginning() + child, getQueueBeginning() + parent);
      child = parent;
      parent = (child - 1) / 2;
    }
  }

  void
  pushDown(ptrdiff_t parent) {
    ptrdiff_t child = getSmallerChild(parent);
    while (child != 0 && _compare(_queue[child], _queue[parent])) {
      swap(getQueueBeginning() + child, getQueueBeginning() + parent);
      parent = child;
      child = getSmallerChild(parent);
    }
  }

  //! The Gibson and Bruck updating algorithm.
  void
  updateRecursive(ptrdiff_t n) {
    ptrdiff_t parent = (n - 1) / 2;
    if (n > 0 && _compare(_queue[n], _queue[parent])) {
      swap(getQueueBeginning() + n, getQueueBeginning() + parent);
      updateRecursive(parent);
    }
    else {
      ptrdiff_t child = getSmallerChild(n);
      if (child != 0 && _compare(_queue[child], _queue[n])) {
	swap(getQueueBeginning() + child, getQueueBeginning() + n);
	updateRecursive(child);
      }
    }
  }

  //! Return the index of the smaller child or 0 if there are no children.
  ptrdiff_t
  getSmallerChild(ptrdiff_t parent) const {
    const ptrdiff_t size = getQueueEnd() - getQueueBeginning();
    ptrdiff_t child = 2 * parent + 1;
    // If there are no children.
    if (child >= size) {
      return 0;
    }
    // If the second child is smaller.
    if (child + 1 < size && _compare(_queue[child + 1], _queue[child])) {
      ++child;
    }
    return child;
  }

  //! Swap the two elements' positions in the queue.
  void
  swap(const Iterator i, const Iterator j) {
    std::swap(_pointers[i->first], _pointers[j->first]);
    std::swap(*i, *j);
  }

  //@}
};

END_NAMESPACE_ADS

#endif
