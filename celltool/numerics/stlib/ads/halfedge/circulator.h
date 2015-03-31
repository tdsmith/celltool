// -*- C++ -*-

/*! 
  \file circulator.h
  \brief Halfedge circulators for the halfedge data structure.
*/

#if !defined(__ads_circulator_h__)
#define __ads_circulator_h__

#include "../defs.h"

#include <iterator>

BEGIN_NAMESPACE_ADS

//! Halfedge circulator for the halfedges around a face.
/*!
  This is a bi-directional circulator for the incident halfedges of a face
  in the halfedge data structure.  The halfedges are traversed in the
  positive (counter-clockwise) direction.
*/
template <class Iterator>
class Face_Halfedge_circ
{
private:

  typedef  std::iterator_traits<Iterator> traits;
    
public:

  //! The value type.
  typedef typename traits::value_type      value_type;
  //! Pointer difference type.
  typedef typename traits::difference_type difference_type;
  //! Reference to a value_type.
  typedef typename traits::reference       reference;
  //! Pointer to a value_type.
  typedef typename traits::pointer         pointer;

  // I'm leaving the category blank for now.

private:

  //
  // Member data.
  //

  Iterator _i;

public:

  //
  // Constructors
  //

  //! Default constructor.
  Face_Halfedge_circ() :
    _i()
  {}

  //! Construct from at iterator.
  explicit 
  Face_Halfedge_circ( Iterator i ) : 
    _i( i ) 
  {}

  //! Circulator to const circulator conversion.
  template <class Iterator2>
  Face_Halfedge_circ( const Face_Halfedge_circ<Iterator2>& c ) : 
    _i( c.base() )
  {}

  //
  // Dereferencing.
  //

  //! Dereference.
  reference
  operator*() const 
  { 
    return *_i; 
  }
  
  //! Member access.
  pointer
  operator->() const 
  { 
    return _i; 
  }

  //
  // Increment
  //

  //! Pre-increment.
  Face_Halfedge_circ& 
  operator++() 
  {
    _i = _i->next();
    return *this;
  }

  //! Post-increment.
  Face_Halfedge_circ 
  operator++( int ) 
  {
    Face_Halfedge_circ tmp = *this;
    ++*this;
    return tmp;
  }

  //
  // Decrement
  //

  //! Pre-decrement.
  Face_Halfedge_circ& 
  operator--() 
  {
    _i = _i->prev();
    return *this;
  }

  //! Post-decrement.
  Face_Halfedge_circ 
  operator--(int) 
  {
    Face_Halfedge_circ tmp = *this;
    --*this;
    return tmp;
  }

  //
  // Base iterator.
  //
  
  //! Return a const reference to the base iterator.
  const Iterator&
  base() const
  {
    return _i;
  }
  
};

//
// Equality
//

//! Return true if the circulators point to the same object.
template <class Iterator>
inline
bool 
operator==( const Face_Halfedge_circ<Iterator>& x, 
	    const Face_Halfedge_circ<Iterator>& y ) 
{ 
  return x.base() == y.base();
}

//! Return true if the circulators don't point to the same object.
template <class Iterator>
inline
bool 
operator!=( const Face_Halfedge_circ<Iterator>& x, 
	    const Face_Halfedge_circ<Iterator>& y ) 
{ 
  return !( x == y );
}

END_NAMESPACE_ADS

#endif
