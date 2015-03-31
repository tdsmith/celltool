// -*- C++ -*-

/*! 
  \file PriorityQueueBinaryHeapArray.h
  \brief A binary heap priority queue for the data in an array.
*/

#if !defined(__ads_PriorityQueueBinaryHeapArray_h__)
#define __ads_PriorityQueueBinaryHeapArray_h__

#include "PriorityQueueBinaryHeapDynamicKeys.h"
#include "HeapHandleArray.h"

#include "../array/Array.h"

BEGIN_NAMESPACE_ADS

//! A binary heap priority queue for the data in an array.
/*!
  This priority queue is designed for storing handles into an array.
  It derives functionality from ads::PriorityQueueBinaryHeapDynamicKeys
  and thus supports dynamic keys.  The element type is the only
  required parameter.  The remaining parameters have default values.
  By default, it is assumed that the element type is a handle and
  that the key is obtained by dereferencing this handle.
  
  \param T is the element type.
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
	   typename Key = typename std::iterator_traits<T>::value_type,
	   class GetKey = Dereference<T>,
	   class CompareKeys = std::less<Key>, 
	   class Sequence = std::vector<T> >
class PriorityQueueBinaryHeapArray :
  public PriorityQueueBinaryHeapDynamicKeys
  < T, HeapHandleArray<T, typename Sequence::iterator>, Key,
    GetKey, CompareKeys, Sequence >
{
 private:

  typedef PriorityQueueBinaryHeapDynamicKeys
  < T, HeapHandleArray<T, typename Sequence::iterator>, Key,
    GetKey, CompareKeys, Sequence > base_type;
    
  typedef typename base_type::iterator heap_handle;
  typedef ads::Array<1,heap_handle> heap_handle_array_type;

  typedef HeapHandleArray<T, heap_handle> get_handle_functor;

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
  //! An iterator on the value type.
  typedef typename base_type::iterator iterator;

 protected:

  //
  // Member data.
  //

  //! The array of heap handles.
  heap_handle_array_type _heap_handles;

 private:

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  PriorityQueueBinaryHeapArray( const PriorityQueueBinaryHeapArray& );

  // Assignment operator not implemented.
  const PriorityQueueBinaryHeapArray& 
  operator=( const PriorityQueueBinaryHeapArray& );

 public:

  //
  // Constructors, Destructor.
  //

  //! Make from a container of values.  
  /*!
    The default constructor makes an empty queue.
  */
  template <class DataArray>
  PriorityQueueBinaryHeapArray( const DataArray& data_array, 
				const sequence_type& 
				container = sequence_type() ) :
    base_type( get_handle_functor(), container ),
    _heap_handles( data_array.size() )
  {
    base_type::_get_handle.initialize( data_array, _heap_handles );
  }

  //! Construct and reserve memory for n elements.
  template <class DataArray>
  PriorityQueueBinaryHeapArray( const DataArray& data_array, size_type n ) :
    base_type( n ),
    _heap_handles( data_array.size() )
  {
    base_type::_get_handle.initialize( data_array, _heap_handles );
  }

  //! Add the values in the range to the container then make the heap.
  template <class DataArray, class InputIterator>
  PriorityQueueBinaryHeapArray( const DataArray& data_array,
				InputIterator first, 
				InputIterator last,
				const sequence_type& 
				container = sequence_type() ) :
    base_type( first, last, get_handle_functor(), container ),
    _heap_handles( data_array.size() )
  {
    base_type::_get_handle.initialize( data_array, _heap_handles );
  }

  //! Destructor.
  virtual 
  ~PriorityQueueBinaryHeapArray()
  {}
  
};

END_NAMESPACE_ADS

#endif
