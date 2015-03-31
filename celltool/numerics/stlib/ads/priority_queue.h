// -*- C++ -*-

/*! 
  \file priority_queue.h
  \brief Includes the priority queue classes.
*/

/*!
  \page ads_priority_queue Priority Queue Package

  A priority queue is a container that stores elements which have keys
  which can be compared.  
  See "Introduction to Algorithms" by Cormen, Leiserson, Rivest and
  Stein for a discussion of priority queues.

  This package provides priority queues implemented with
  binary heaps and an approximate priority queue implemented with a cell array.
  Include the file priority_queue.h to use them.
  Most of the data structures in this package assume that the key can be
  determined from the element.  Here are some examples of this:
  - The element type is a pointer to a number type and the key is that number
  type.
  - The element type is a structure that has the key as a member.
  - The element type is a pointer to a structure that has the key as a member.
  - The element is a number type and the key of the element is the value 
  of the element.
  .
  You specify how to obtain the key from the element by specifying an 
  appropriate functor as a template parameter to the priority queue.

  Priority queues have the following functionality:
  - \c size() returns the number of elements in the queue.
  - \c empty() returns true if the queue is empty.
  - \c top() returns a const reference to the element at the top of the queue.
  - \c pop() removes the element at the top of the queue.
  - \c push( e ) inserts the element \c e into the queue.  Here we assume that
  the key can be determined from the element.
  - \c push( e, k ) inserts the element \c e with key \c k into the queue.  
  Here it is not necessary that the key can be determined from the element.
  
  Priority queue with dynamic keys have the additional function:
  \c decrease( e ).  If the key of \c e has changed, call \c decrease( e ) to
  adjust the position of the element in the queue.  (Here we assume that the
  key can be determined from the element.)

  This package has two priority queues designed for static keys.  That is, 
  while an element is in the queue, its key does not change.  Both 
  implementations use the STL heap utility functions.  
  - ads::PriorityQueueBinaryHeap only stores the elements.
  - ads::PriorityQueueBinaryHeapStoreKeys stores the elements and the keys.

  The ads::PriorityQueueBinaryHeapDynamicKeys class is designed for dynamic
  keys.  It does not use the STL heap utility functions.  To use this class,
  you must store handles to the heap elements in some other data structure 
  and provide a functor which takes an element and returns a
  reference to the handle to that element.  ads::HeapHandleArray is an 
  example of a such a functor.

  ads::PriorityQueueBinaryHeapArray is a priority queue with dynamic keys
  that is designed for storing handles into an array.

  ads::PriorityQueueCellArray is an approximate priority queue with static 
  keys.  It cell sorts the elements by their key.  Thus the keys must be 
  numeric.  Each cell is a container with elements whose keys are in a 
  certain range.  The interface is a little different than that of a 
  regular priority queue.
  - \c top() returns a const reference to the container at the top of 
  the queue.
  - \c pop() clears the container at the top of the queue.
 */

#if !defined(__ads_priority_queue_h__)
#define __ads_priority_queue_h__

#include "priority_queue/PriorityQueueBinaryHeap.h"
#include "priority_queue/PriorityQueueBinaryHeapStoreKeys.h"
#include "priority_queue/PriorityQueueBinaryHeapDynamicKeys.h"
#include "priority_queue/PriorityQueueBinaryHeapArray.h"

#include "priority_queue/HeapHandleArray.h"

#include "priority_queue/PriorityQueueCellArray.h"

#endif
