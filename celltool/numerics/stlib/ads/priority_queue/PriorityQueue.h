// -*- C++ -*-

/*! 
  \file PriorityQueue.h
  \brief Implements a base class for priority queues.
*/

#if !defined(__ads_PriorityQueue_h__)
#define __ads_PriorityQueue_h__

#include "../functor/Dereference.h"

#include <vector>

BEGIN_NAMESPACE_ADS

//! A base class for priority queues.
/*!
  PriorityQueue is a base class that defines types common to all 
  priority queues.

  \param T is the element type.
  \param Key is the key type.  By default, we assume T is an iterator
  and Key is the value type of this iterator.
*/
template < typename T, 
	   typename Key = typename std::iterator_traits<T>::value_type,
	   class GetKey = Dereference<T>,
	   class CompareKeys = std::greater<Key>, 
	   class Sequence = std::vector<T> >
class PriorityQueue
{
 public:

  //! The element type.
  typedef T element_type;
  //! A const reference to the element type.
  typedef const element_type& const_reference;

  //! The key type.
  typedef Key key_type;

  //! The functor to get a key from an element.
  typedef GetKey get_key_functor;

  //! The functor to compare keys.
  typedef CompareKeys compare_keys_functor;

  //! The sequence to store elements.
  typedef Sequence sequence_type;

  //! The type stored in the priority queue.
  typedef typename sequence_type::value_type value_type;
  //! An iterator on the value type.
  typedef typename sequence_type::iterator iterator;
  //! Difference between iterators.
  typedef typename sequence_type::difference_type difference_type;
  //! The size type.
  typedef int size_type;

  // I don't currently use the virtual function capability.
  // It does not affect the performance much either way.
  // The FM method is a little better without virtual functions.

  /*
    public:

    //
    // Accessors
    //

    //! Return the number of elements in the priority queue.
    virtual
    size_type
    size() const = 0;

    //! Return true if the priority queue is empty.
    virtual
    bool
    empty() const = 0;

    //
    // Manipulators
    //

    //! Add an element to the priority queue.
    virtual
    void 
    push( element_type x ) = 0;

    //! Remove the element or element container at the top of the queue.
    virtual
    void
    pop() = 0;
  */
};

END_NAMESPACE_ADS

#endif
