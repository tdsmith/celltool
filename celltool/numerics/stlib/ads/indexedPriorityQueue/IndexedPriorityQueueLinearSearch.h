// -*- C++ -*-

/*! 
  \file ads/indexedPriorityQueue/IndexedPriorityQueueLinearSearch.h
  \brief Indexed priority queue that uses linear search.
*/

#if !defined(__ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearch_h__)
#define __ads_indexedPriorityQueue_IndexedPriorityQueueLinearSearch_h__

#include "IndexedPriorityQueueBase.h"

BEGIN_NAMESPACE_ADS

//! Indexed priority queue that uses linear search.
/*!
  \param _Base is the base class.
*/
template<class _Base = IndexedPriorityQueueBase<> >
class IndexedPriorityQueueLinearSearch :
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

  //
  // Public types.
  //
public:

  //! The key type.
  typedef typename Base::Key Key;

  //
  // Using member data.
  //
private:

  using Base::_keys;
  //using Base::_indices;
  //using Base::_queue;
  using Base::_compare;
  using Base::_topIndex;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the size.
  IndexedPriorityQueueLinearSearch(const int size) :
    Base(size) {
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
#ifdef DEBUG_ads
    assert(! _keys.empty());
#endif
    return _topIndex = 
      *std::min_element(getQueueBeginning(), getQueueEnd(), _compare) - 
      _keys.begin();
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

  //@}
};

END_NAMESPACE_ADS

#endif
