// -*- C++ -*-

/*! 
  \file HeapHandleArray.h
  \brief Implements a functor for getting handles into a heap.
*/

#if !defined(__ads_HeapHandleArray_h__)
#define __ads_HeapHandleArray_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! A functor for getting handles into a heap.
/*!
  \param DataConstIterator is a random access const iterator into the 
  data array.
  \param HeapIterator is a random access iterator into the heap.
*/
template <typename DataConstIterator, typename HeapIterator>
class HeapHandleArray
{
 public:

  //! A const iterator into the data array.
  typedef DataConstIterator data_const_iterator;
  //! An iterator into the heap.
  typedef HeapIterator heap_iterator;

 private:

  //
  // Member data
  //

  data_const_iterator _data_begin;
  heap_iterator* _handles_begin;

 private:

  // Assignment operator not implemented.
  HeapHandleArray&
  operator=( const HeapHandleArray& );

 public:

  //
  // Constructors
  //
    
  //! Default constructor.  Invalid pointers.
  HeapHandleArray() :
    _data_begin( 0 ),
    _handles_begin( 0 )
  {}

  //! Construct from the data array and the handle array.
  template <class DataArray, class HandleArray>
  HeapHandleArray( const DataArray& data, HandleArray& handles ) :
    _data_begin( data.begin() ),
    _handles_begin( handles.begin() )
  {}

  //! Copy constructor.
  HeapHandleArray( const HeapHandleArray& x ) :
    _data_begin( x._data_begin ),
    _handles_begin( x._handles_begin )
  {}

  //
  // Accessors
  //
    
  //! Return the heap iterator for the handle.
  heap_iterator
  operator()( data_const_iterator h ) const
  {
    return _handles_begin[ h - _data_begin ];
  }

  //
  // Manipulators
  //

  //! Return a reference to the heap iterator for the handle.
  heap_iterator&
  operator()( data_const_iterator h )
  {
    return _handles_begin[ h - _data_begin ];
  }

  //! Initialize from the data array and the handle array.
  template <class DataArray, class HandleArray>
  void
  initialize( const DataArray& data, HandleArray& handles )
  {
    _data_begin = data.begin();
    _handles_begin = handles.begin();
  }

};

END_NAMESPACE_ADS

#endif
