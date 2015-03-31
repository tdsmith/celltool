// -*- C++ -*-

// CONTINUE: add more documentation.

/*! 
  \file PriorityQueueCellArray.h
  \brief Implements a class for a priority queue using a cell array.
*/

#if !defined(__ads_PriorityQueueCellArray_h__)
#define __ads_PriorityQueueCellArray_h__

#include "PriorityQueue.h"

#include "../array/Array.h"

#include <vector>

BEGIN_NAMESPACE_ADS

//! A priority queue utilizing a cell array.
/*!
  This priority queue stores elements that have numeric keys.
  For example, the value type might be a handle to an object and the key 
  type might be a floating point number.
    
  This is an approximate priority queue.  You specify the span of a
  cell in the constructor.

  \param T is the element type.
  \param Key is the key type.
  \param GetKey is the functor that gets the key from the element.
  \param Container is a container for the value type.  It must have a
  push_back() member function.
*/
template<typename T, 
	 typename Key = typename std::iterator_traits<T>::value_type,
	 class GetKey = Dereference<T>,
	 class Container = std::vector<T> >
class PriorityQueueCellArray :
  public PriorityQueue<T, Key> {
 private:

  //
  // private typedefs.
  //

  typedef PriorityQueue<T, Key> base_type;
  typedef GetKey get_key_functor;

 public:

  //
  // public typedefs.
  //

  //! The element type.
  typedef typename base_type::element_type element_type;
  //! The key type.
  typedef typename base_type::key_type key_type;
  //! The size type.
  typedef typename base_type::size_type size_type;

  //! The container type.
  typedef Container container_type;
  //! A const reference to a container.
  typedef const container_type& container_const_reference;
  //! The value type.
  typedef typename container_type::value_type value_type;
  //! A reference to the value type.
  typedef typename container_type::reference reference;
  //! A const reference to the value type.
  typedef typename container_type::const_reference const_reference;
  //! An iterator in the container.
  typedef typename container_type::iterator iterator;
  //! A const iterator in the container.
  typedef typename container_type::const_iterator const_iterator;

 private:

  // 
  // private typedefs
  //

  // The cell array.
  typedef Array< 1, container_type > cell_array_type;
  // An iterator over cells.
  typedef typename cell_array_type::iterator cell_iterator;

 private:

  //
  // Member data.
  //

  // The cell array.
  cell_array_type _array;
  // The top cell in the priority queue.
  cell_iterator _top;
  // The number of elements in the priority queue.
  size_type _size;
  // The minimum key that will be stored in the queue.
  key_type _min_key;
  // The span of a single cell.
  key_type _delta;
  // A lower bound on the keys stored in the top cell.
  key_type _lower_bound;
  // The number of cells.
  int _num_cells;

  // The functor to get the key from an element.
  get_key_functor _get_key;

#ifdef DEBUG_PriorityQueueCellArray
  // The index of the top cell (non-cyclic).
  int _top_index;
#endif

 public:

  //
  // Constructor and destructor.
  //

  //! Make an empty priority queue.
  /*!
    \param min_key is a lower bound on the keys to be stored.
    \param delta is the span of a single cell.
    \param span is the difference between the upper and lower bounds on 
    the keys that are held at any one time.
  */
  PriorityQueueCellArray(key_type min_key, key_type delta, key_type span) :
    _array(int(span / delta) + 3),
    _top(_array.begin()),
    _size(0),
    // Adjust it down by delta, because we are not allowed to push into
    // the top cell.
    _min_key(min_key - delta),
    _delta(delta),
    _lower_bound(_min_key),
    _num_cells(_array.size()),
    _get_key()
#ifdef DEBUG_PriorityQueueCellArray
									    ,_top_index(0)
#endif
  {}

  //! Destructor
  virtual
  ~PriorityQueueCellArray()
  {}

  //
  // Accessors
  //

  //! Return the container at the top of the priority queue.
  container_const_reference 
  top() const {
    return *_top;
  }

  //! Return the number of elements in the priority queue.
  size_type
  size() const {
    return _size;
  }

  //! Return true if the priority queue is empty.
  bool
  empty() const {
    return _size == 0;
  }

  //! A lower bound on the keys stored in the top cell.
  key_type
  lower_bound() const {
    return _lower_bound;
  }

  //
  // Manipulators
  //

  //! Insert an element into the priority queue.
  void 
  push(element_type x) {
    ++_size;
    _array[ index(_get_key(x)) ].push_back(x);
  }

  //! Add an element with the specified key to the priority queue.
  void 
  push(element_type x, key_type k) {
    ++_size;
    _array[ index(k) ].push_back(x);
  }

  //! Clear the container at the top of the priority queue.
  void
  pop() {
    // Adjust the number of elements in the priority queue.
    _size -= _top->size();
    // Clear the container.
    _top->clear();
    // Increment the top iterator.
    ++_top;
    if (_top == _array.end()) {
      _top = _array.begin();
    }
    // Increment the lower bound on keys.
    _lower_bound += _delta;
#ifdef DEBUG_PriorityQueueCellArray
    ++_top_index;
#endif
  }

 private:

  //
  // Private member functions.
  //

  // Return the index of the appropriate cell.
  int
  index(key_type k) const {
#ifdef DEBUG_PriorityQueueCellArray
    int index = int((k - _min_key) / _delta);
    assert(k >= _min_key + _delta && _top_index < index &&
	    index < _top_index + _num_cells);
#endif
    return int((k - _min_key) / _delta) % _num_cells;
  }

};

END_NAMESPACE_ADS

#endif
