// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueueLinearSearchUnrolled.h
  \brief Indexed priority queue that uses a linear search.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearchUnrolled_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearchUnrolled_h__

#include "../defs.h"

#include "../array/Array.h"
#include "../algorithm/extremeElement.h"
#include "../../third-party/loki/TypeManip.h"

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses a linear search.
/*!
  \param Key is the key type.
*/
template<typename _Key = double>
class IndexedPriorityQueueLinearSearchUnrolled {
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
  // Member data.
  //
private:

  ads::Array<1, Key> _keys;
  int _topIndex;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueueLinearSearchUnrolled(const int size) :
    // No valid keys.
    // Pad to an even size to enable loop unrolling.
    _keys(size + size % 2, std::numeric_limits<Key>::max()),
    // Invalid index.
    _topIndex(-1) {
  }

  // Default copy constructor, assignment operator, and destructor are fine.

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the key of the specified element.
  Key
  get(const int index) const {
    return _keys[index];
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Return the index of the top element.
  int
  top() {
#ifdef DEBUG_ads
    assert(! _keys.empty());
#endif
    return _topIndex = 
      ads::findMinimumElementUnrolledEven(_keys.begin(), _keys.end()) -
      _keys.begin();
#if 0
    // This is about the same speed as calling the above function.
    typename ads::Array<1, Key>::const_iterator minimum = _keys.begin();
    typename ads::Array<1, Key>::const_iterator j;
    for (typename ads::Array<1, Key>::const_iterator i = 
	   _keys.begin(); i != _keys.end(); i += 2) {
      j = i + 1;
      if (*i < *j) {
	if (*i < *minimum) {
	  minimum = i;
	}
      }
      else {
	if (*j < *minimum) {
	  minimum = j;
	}
      }
    }
    return _topIndex = minimum - _keys.begin();
#endif
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

  //! Clear the queue.
  void
  clear() {
    _keys = std::numeric_limits<Key>::max();
    _topIndex = -1;
  }

  //@}
};

END_NAMESPACE_ADS

#endif
