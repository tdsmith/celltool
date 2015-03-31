// -*- C++ -*-

/*! 
  \file HDSHalfedge.h
  \brief Class for a halfedge in a halfedge data structure.
*/

#if !defined(__HDSHalfedge_h__)
#define __HDSHalfedge_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! A halfedge in a halfedge data structure.
/*!
  Implements the minimum functionality for a halfedge in a halfedge data 
  structure.  Halfedges in ads::HalfedgeDS should derive from 
  this class.
*/
template <class HDS>
class HDSHalfedge
{
public:

  //
  // Types
  //

  //! A handle to a vertex.
  typedef typename HDS::Vertex_handle Vertex_handle;
  //! A handle to a const vertex.
  typedef typename HDS::Vertex_const_handle Vertex_const_handle;

  //! A handle to a halfedge.
  typedef typename HDS::Halfedge_handle Halfedge_handle;
  //! A handle to a const halfedge.
  typedef typename HDS::Halfedge_const_handle Halfedge_const_handle;

  //! A handle to a face.
  typedef typename HDS::Face_handle Face_handle;
  //! A handle to a const face.
  typedef typename HDS::Face_const_handle Face_const_handle;

private:
      
  //
  // Data
  //
      
  Halfedge_handle _opposite, _prev, _next;
  Vertex_handle _vertex;
  Face_handle _face;

public:

  //
  // Constructors and Destructor
  //

  //! Default constructor.  Unititialized memory.
  HDSHalfedge()
  {}

  //! Copy constructor.
  HDSHalfedge( const HDSHalfedge& x ) :
    _opposite( x._opposite ),
    _prev( x._prev ),
    _next( x._next ),
    _vertex( x._vertex ),
    _face( x._face )
  {}

  //! Trivial destructor.
  ~HDSHalfedge()
  {}

  //
  // Assignment operators.
  //
      
  //! Assignment operator.
  HDSHalfedge& 
  operator=( const HDSHalfedge& x )
  {
    if ( &x != this ) {
      _opposite = x._opposite;
      _prev = x._prev;
      _next = x._next;
      _vertex = x._vertex;
      _face = x._face;
    }
    return *this;
  }

  //  
  // Accessors
  //

  //! Return a const handle to the opposite half-edge.
  Halfedge_const_handle
  opposite() const
  { 
    return _opposite;
  }

  //! Return a const handle to the previous half-edge.
  Halfedge_const_handle
  prev() const
  { 
    return _prev;
  }

  //! Return a const handle to the next half-edge.
  Halfedge_const_handle
  next() const
  { 
    return _next;
  }

  //! Return a const handle to the incident vertex.
  Vertex_const_handle
  vertex() const
  { 
    return _vertex;
  }

  //! Return a const handle to the incident face.
  Face_const_handle
  face() const
  { 
    return _face;
  }

  //  
  // Manipulators
  //

  //! Return a handle to the opposite half-edge.
  Halfedge_handle&
  opposite()
  { 
    return _opposite;
  }

  //! Return a handle to the previous half-edge.
  Halfedge_handle&
  prev()
  { 
    return _prev;
  }

  //! Return a handle to the next half-edge.
  Halfedge_handle&
  next()
  { 
    return _next;
  }

  //! Return a handle to the incident vertex.
  Vertex_handle&
  vertex()
  { 
    return _vertex;
  }

  //! Return a handle to the incident face.
  Face_handle&
  face()
  { 
    return _face;
  }

};

//
// Equality operators.
//

/* REMOVE
//! Equality operator
template <class HDS>
bool 
operator==( const HDSHalfedge<HDS>& a, const HDSHalfedge<HDS>& b )
{
return ( a.index() == b.index() &&
a.opposite()->index() == b.opposite()->index() &&
a.prev()->index() == b.prev()->index() &&
a.next()->index() == b.next()->index() &&
a.vertex()->index() == b.vertex()->index() &&
a.face()->index() == b.face()->index() );
}

//! Inequality operator
template <class HDS>
bool 
operator!=( const HDSHalfedge<HDS>& a, const HDSHalfedge<HDS>& b )
{
  return !(a == b );
}
*/

END_NAMESPACE_ADS

#endif
