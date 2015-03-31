// -*- C++ -*-

/*! 
  \file PriorityQueueBinaryHeapDynamicKeys.h
  \brief Implements a class for a binary heap with dynamic keys.
*/

#if !defined(__ads_PriorityQueueBinaryHeapDynamicKeys_h__)
#define __ads_PriorityQueueBinaryHeapDynamicKeys_h__

#include "PriorityQueue.h"

#include "../functor/compose.h"

#include <vector>

BEGIN_NAMESPACE_ADS

//! A priority queue with dynamic keys implemented with a binary heap.
/*!
  This priority queue is similar to ads::PriorityQueueBinaryHeap,
  but it supports dynamic keys and the implementation does not use
  the STL heap functions.  It has the additional template parameter
  \c GetHandle.  This class is useful when the keys changes while
  their associated elements are in the heap.

  The element type and the functor to get heap handles are
  required parameters.  The remaining parameters have default
  values.  By default, it is assumed that the element type is a
  handle and that the key is obtained by dereferencing this handle.

  \param T is the element type.
  \param GetHandle is a functor that takes an element and returns a
  reference to the handle to that element.  This handle must be stored 
  in some data structure.
  \param Key is the key type.
  \param GetKey is the functor that gets the key from the element.
  \param CompareKeys is a functor that takes two keys as arguments 
  and returns a boolean.  It is used to order the objects in the 
  priority queue.  For less than comparisons, the top of the priority
  queue holds the element with minimum key.  This is the default behavior.
  \param Sequence is the container for the binary heap.  It is 
  std::vector<T> by default.
*/
template < typename T,
	   class GetHandle,
	   typename Key = typename std::iterator_traits<T>::value_type,
	   class GetKey = Dereference<T>,
	   class CompareKeys = std::less<Key>, 
	   class Sequence = std::vector<T> >
class PriorityQueueBinaryHeapDynamicKeys :
  public PriorityQueue<T, Key, GetKey, CompareKeys, Sequence>
{
 protected:

  //! The base priority queue.
  typedef PriorityQueue<T, Key, GetKey, CompareKeys, Sequence> base_type;
  //! Functor that gets a handle to an element.
  typedef GetHandle get_handle_functor;
  //! Functor for getting the key of an element.
  typedef typename base_type::get_key_functor get_key_functor;
  //! Functor for comparing keys.
  typedef typename base_type::compare_keys_functor compare_keys_functor;
  //! The container type.
  typedef typename base_type::sequence_type sequence_type;
  //! Functor for comparing elements.
  typedef ads::binary_compose_binary_unary< compare_keys_functor, 
					    get_key_functor, 
					    get_key_functor > 
  compare_values_functor;

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
  //! An iterator on the value type.
  typedef typename base_type::iterator iterator;
  //! Difference between iterators.
  typedef typename base_type::difference_type difference_type;

 protected:

  //
  // Member data.
  //

  //! The functor for getting an elements handle.
  get_handle_functor _get_handle;
  //! The value type comparison functor.
  compare_values_functor _compare;
  //! The container for storing the values.
  sequence_type _container;

 private:

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  PriorityQueueBinaryHeapDynamicKeys( const 
				      PriorityQueueBinaryHeapDynamicKeys& );

  // Assignment operator not implemented.
  const PriorityQueueBinaryHeapDynamicKeys& 
  operator=( const PriorityQueueBinaryHeapDynamicKeys& );

 public:

  //
  // Constructors, Destructor.
  //

  //! Make from a container of values.  
  /*!
    The default constructor makes an empty queue.
  */
  explicit
  PriorityQueueBinaryHeapDynamicKeys( const get_handle_functor& 
				      get_handle = get_handle_functor(),
				      const sequence_type& 
				      container = sequence_type() ) :
    _get_handle( get_handle ),
    _compare(),
    _container( container )
  {
    make();
  }

  //! Construct and reserve memory for n elements.
  explicit 
  PriorityQueueBinaryHeapDynamicKeys( size_type n,
				      const get_handle_functor& 
				      get_handle = get_handle_functor() ) :
    _get_handle( get_handle ),
    _compare(),
    _container()
  {
    _container.reserve( n ); 
  }

  //! Add the values in the range to the container then make the heap.
  template <class InputIterator>
  PriorityQueueBinaryHeapDynamicKeys( InputIterator first, 
				      InputIterator last,
				      const get_handle_functor& 
				      get_handle = get_handle_functor(),
				      const sequence_type& 
				      container = sequence_type() ) :
    _get_handle( get_handle ),
    _compare(),
    _container( container )
  { 
    _container.insert( _container.end(), first, last);
    make();
  }

  //! Destructor.
  virtual 
  ~PriorityQueueBinaryHeapDynamicKeys()
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
    return _container.front(); 
  }

  //
  // Manipulators
  //

  //! Add an element to the priority queue.
  void 
  push( element_type x )
  {
    // If we don't have to resize the container.
    if ( _container.size() < _container.capacity() ) {
      // Add the element.
      _container.push_back( x );
      // Set the element's handle.
      _get_handle( _container.back() ) = _container.end() - 1;
    }
    // The container will be resized when the element is added.
    else {
      // Add the element.
      _container.push_back( x );
      // Fix the element handles.
      set_handles();
    }
    // Insert the element in the heap.
    decrease( _container.end() - 1 );
  }

  //! Remove the element at the top of the priority queue.
  void 
  pop()
  {
    // Store and erase the last element.
    value_type tmp( _container.back() );
    _container.pop_back();

    // Adjust the heap.
    difference_type parent = 0;
    difference_type child = small_child( parent );
    while ( child >= 0 && 
	    _compare( *( _container.begin() + child ), tmp ) ) {
      copy( _container.begin() + parent, _container.begin() + child );
      parent = child;
      child = small_child( parent );
    }

    // Insert the last element.
    copy( _container.begin() + parent, iterator(&tmp) );
  }

  //! The key of the element has decreased, adjust its position in the heap.
  void 
  decrease( element_type x )
  {
    decrease( _get_handle( x ) );
  }

 protected:

  //! Make a heap from the elements in the container.
  void
  make()
  {
    for ( iterator i = _container.begin(); i != _container.end(); ++i ) {
      _get_handle( *i ) = i;
      decrease( i );
    }
  }

  //! The key of the element has decreased, adjust its position in the heap.
  void 
  decrease( iterator iter )
  {
    difference_type child = iter - _container.begin();
    difference_type parent = (child - 1) / 2;

    while ( child > 0 && 
	    _compare( *( _container.begin() + child ), 
		      *( _container.begin() + parent ) ) ) {
      swap( _container.begin() + child, _container.begin() + parent );
      child = parent;
      parent = (child - 1) / 2;
    }
  }

  //! Swap two elements in the container.
  void 
  swap( iterator a, iterator b )
  {
    std::swap( _get_handle( *a ), _get_handle( *b ) );
    std::swap( *a, *b );
  }

  //! Copy b into a.
  void 
  copy( iterator a, iterator b )
  {
    *a = *b;
    _get_handle( *a ) = a;
  }

  //! Set the heap pointers of the elements.
  void 
  set_handles()
  {
    for ( iterator i = _container.begin(); i != _container.end(); ++i ) {
      _get_handle( *i ) = i;
    }
  }

  //! Return the index of the smaller child.
  difference_type 
  small_child( difference_type parent )
  {
    difference_type child = 2 * parent + 1;
    if ( child + 1 < static_cast<difference_type>( size() ) &&
	 _compare( *( _container.begin() + child + 1 ),
		   *( _container.begin() + child ) ) ) {
      ++child;
    }
    if ( child < static_cast<difference_type>( size() ) ) {
      return child;
    }
    return -1;
  }

};

END_NAMESPACE_ADS

#endif
