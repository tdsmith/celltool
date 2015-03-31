// -*- C++ -*-

/*!
  \file ads/indexedPriorityQueue/IndexedPriorityQueueHashing.h
  \brief Indexed priority queue with a hash table.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueHashing_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueHashing_h__

#include "HashingChaining.h"

BEGIN_NAMESPACE_ADS

//! Indexed priority queue with a hash table.
/*!
  \param Key is the key type.
*/
template<typename _Key = double,
	 class _HashTable = HashingChaining<typename ads::Array<1, _Key>::const_iterator> >
class IndexedPriorityQueueHashing {
  //
  // Enumerations.
  //
public:

  enum {UsesPropensities = true};

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

  typedef typename ads::Array<1, Key>::const_iterator Iterator;
  typedef _HashTable HashTable;

  //
  // Member data.
  //
private:

  ads::Array<1, Key> _keys;
  HashTable _hashTable;
  int _topIndex;
  const ads::Array<1, Key>* _propensities;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the initial time and the hash table parameters.
  IndexedPriorityQueueHashing(const int size,
			      const int hashTableSize,
			      const Key targetLoad) :
    _keys(size),
    _hashTable(hashTableSize, targetLoad),
    // Invalid value.
    _topIndex(-1),
    _propensities(0) {
    clear();
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
  Key
  get(const int index) const {
    return _keys[index];
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
    assert(_propensities != 0);
#endif
    if (_hashTable.isEmpty()) {
      const Key sum = std::accumulate(_propensities->begin(),
				      _propensities->end(), Key(0));
      // If there are no non-zero propensities.
      if (sum == 0) {
	// Return the index of any reaction.  The first will do fine.
	return _topIndex = 0;
      }
#ifdef DEBUG_ads
      // There must be finite times left.
      assert(*std::min_element(_keys.begin(), _keys.end()) !=
	     std::numeric_limits<Key>::max());
#endif
      while (_hashTable.isEmpty()) {
	_hashTable.rebuild(_keys.begin(), _keys.end(), 1 / sum);
      }
    }
    return _topIndex = _hashTable.pop() - _keys.begin();
  }

  //! Pop the top element off the queue.
  /*!
    \pre The minimum element was removed from the hash table with top().
  */
  void 
  popTop() {
    _keys[_topIndex] = std::numeric_limits<Key>::max();
  }

  //! Pop the element off the queue.
  void 
  pop(const int index) {
    _hashTable.erase(_keys.begin() + index);
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
    _hashTable.push(_keys.begin() + index);
  }

  //! Change the value in the queue.
  void
  set(const int index, const Key key) {
#ifdef DEBUG_ads
    assert(key != std::numeric_limits<Key>::max());
#endif
    const Key oldValue = _keys[index];
    _keys[index] = key;
    _hashTable.set(_keys.begin() + index, oldValue);
  }

  //! Clear the priority queue.
  void
  clear() {
    for (int i = 0; i != _keys.size(); ++i) {
      _keys[i] = std::numeric_limits<Key>::max();
    }
    _hashTable.clear();
    _topIndex = -1;
  }

  //@}
};

END_NAMESPACE_ADS

#endif
