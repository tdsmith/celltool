// -*- C++ -*-

/*! 
  \file PriorityQueueBinaryHeapStoreKeys.h
  \brief Implements a class for a binary heap.  
*/

#if !defined(__ads_PriorityQueueBinaryHeapStoreKeys_h__)
#define __ads_PriorityQueueBinaryHeapStoreKeys_h__

#include "PriorityQueue.h"

#include "../functor/compose.h"
#include "../functor/select.h"

#include <algorithm>
#include <vector>

BEGIN_NAMESPACE_ADS

//! A priority queue implemented with a binary heap that stores the keys.
/*!
  This priority queue is similar to ads::PriorityQueueBinaryHeap, but 
  it stores the key values along with the elements in the heap.  The
  template parameters are the same as for ads::PriorityQueueBinaryHeap.
  However, this class does not necessarily use the GetKey functor.
  Thus this class is useful when the key cannot be determined from 
  the element.

  Again, the element type is the only required parameter.  The 
  remaining parameters have default values.  By default, it is assumed
  that the element type is a handle and that the key is obtained by 
  dereferencing this handle.
  
  The implementation uses the following STL heap functions.
  - std::make_heap()
  - std::push_heap()
  - std::pop_heap()

  This priority queue does not support dynamic keys.  The key values
  are not allowed to change while an element is in the queue.
  
  \param T is the element type.
  \param Key is the key type.
  \param GetKey is the functor that gets the key from the element.
  By default it is dereference<T>.  This functor is used only if the
  push( element_type x ) member function is used.
  \param CompareKeys is a functor that takes two keys as arguments 
  and returns a boolean.  It is used to order the objects in the 
  priority queue.  For greater than comparisons, the top of the priority
  queue holds the element with minimum key.  This is the default behavior.
  \param Sequence is the container for the binary heap.  It is 
  std::vector< std::pair<Key,T> > by default.
*/
template < typename T, 
	   typename Key = typename std::iterator_traits<T>::value_type,
	   class GetKey = Dereference<T>,
	   class CompareKeys = std::greater<Key>, 
	   class Sequence = std::vector< std::pair<Key,T> > >
class PriorityQueueBinaryHeapStoreKeys :
  public PriorityQueue<T, Key, GetKey, CompareKeys, Sequence>
{
 private:

  typedef PriorityQueue<T, Key, GetKey, CompareKeys, Sequence> base_type;
  typedef typename base_type::get_key_functor get_key_functor;
  typedef typename base_type::compare_keys_functor compare_keys_functor;
  typedef typename base_type::sequence_type sequence_type;

 public:

  //
  // public typedefs
  //


  //! The element type.
  typedef typename base_type::element_type element_type;
  //! A const reference to the element type.
  typedef typename base_type::const_reference const_reference;
  //! The key type.
  typedef typename base_type::key_type key_type;
  //! The size type.
  typedef typename base_type::size_type size_type;
  //! The type stored in the binary heap.
  typedef typename base_type::value_type value_type;

 private:

  typedef 
  ads::binary_compose_binary_unary< compare_keys_functor, 
				    Select1st<value_type>,
				    Select1st<value_type> > 
  compare_values_functor;

 protected:

  //
  // Member data.
  //

  //! The functor for getting a key from the element.
  get_key_functor _get_key;
  //! The value type comparison functor.
  compare_values_functor _compare;
  //! The container for storing the values.
  sequence_type _container;

 private:

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  PriorityQueueBinaryHeapStoreKeys( const 
				    PriorityQueueBinaryHeapStoreKeys& );

  // Assignment operator not implemented.
  const PriorityQueueBinaryHeapStoreKeys& 
  operator=( const PriorityQueueBinaryHeapStoreKeys& );

 public:

  //
  // Constructors, Destructor.
  //

  //! Make from a container of values.  
  /*!
    The default constructor makes an empty queue.
  */
  explicit
  PriorityQueueBinaryHeapStoreKeys( const sequence_type& 
				    container = sequence_type() ) :
    _get_key(),
    _compare(),
    _container( container )
  {
    std::make_heap( _container.begin(), _container.end(), _compare ); 
  }

  //! Construct and reserve memory for n elements.
  explicit 
  PriorityQueueBinaryHeapStoreKeys( size_type n ) :
    _get_key(),
    _compare(),
    _container()
  {
    _container.reserve( n );
  }

  //! Add the values in the range to the container then make the heap.
  template <class InputIterator>
  PriorityQueueBinaryHeapStoreKeys( InputIterator first, InputIterator last,
				    const sequence_type& 
				    container = sequence_type() ) :
    _get_key(),
    _compare(),
    _container( container )
  { 
    _container.insert( _container.end(), first, last);
    std::make_heap( _container.begin(), _container.end(), _compare ); 
  }

  //! Destructor.
  virtual 
  ~PriorityQueueBinaryHeapStoreKeys()
  {}
  
  //
  // Accessors
  //

  //! Return the number of elements in the priority queue.

  size_type
  size() const
  {
    return _container.size();
  }

  //! Return true if the priority queue is empty.
  bool
  empty() const
  {
    return _container.empty();
  }

  //! Return the element at the top of the priority queue.
  const_reference
  top() const
  { 
    return _container.front().second;
  }

  //
  // Manipulators
  //

  //! Add an element to the priority queue.
  void 
  push( element_type x )
  {
    _container.push_back( value_type( _get_key( x ), x ) );
    std::push_heap( _container.begin(), _container.end(), _compare );
  }

  //! Add an element with the specified key to the priority queue.
  void 
  push( element_type x, key_type k )
  {
    _container.push_back( value_type( k, x ) );
    std::push_heap( _container.begin(), _container.end(), _compare );
  }

  //! Remove the element at the top of the priority queue.
  void 
  pop()
  {
    std::pop_heap( _container.begin(), _container.end(), _compare );
    _container.pop_back();
  }
};

END_NAMESPACE_ADS

#endif
