// -*- C++ -*-

/*! 
  \file HDSNode.h
  \brief Class for a node in a halfedge data structure.  

  Nodes are the base for vertices and faces.
*/

#if !defined(__HDSNode_h__)
#define __HDSNode_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! A node in a halfedge data structure.
/*!
  Implements the minimum functionality for a node in a halfedge data 
  structure.  
*/
template <class HDS>
class HDSNode
{
  //
  // Public types.
  //

public:

  //! A handle to a halfedge.
  typedef typename HDS::Halfedge_handle Halfedge_handle;

  //! A handle to a const halfedge.
  typedef typename HDS::Halfedge_const_handle Halfedge_const_handle;

  //
  // Data
  //
      
private:
      
  Halfedge_handle _halfedge;

public:

  //
  // Constructors and Destructor
  //

  //! Default constructor.  Unititialized memory.
  HDSNode()
  {}

  //! Construct from a halfedge handle.
  HDSNode( Halfedge_handle h ) :
    _halfedge( h )
  {}

  //! Copy constructor.
  HDSNode( const HDSNode& x ) :
    _halfedge( x._halfedge )
  {}

  //! Trivial destructor.
  ~HDSNode()
  {}

  //
  // Assignment operators.
  //
      
  //! Assignment operator.
  HDSNode& 
  operator=( const HDSNode& x )
  {
    if ( &x != this ) {
      _halfedge = x._halfedge;
    }
    return *this;
  }

  //  
  // Accessors
  //

  //! Return a const handle to one of the incident half-edges.
  Halfedge_const_handle
  halfedge() const
  { 
    return _halfedge;
  }

  //  
  // Manipulators
  //

  //! Return a reference to the handle to one of the incident half-edges.
  Halfedge_handle&
  halfedge()
  { 
    return _halfedge;
  }

};

/* REMOVE
//! Equality operator
template <class HDS>
bool 
operator==( const HDSNode<HDS>& a, const HDSNode<HDS>& b )
{
return ( a.index() == b.index() &&
a.halfedge()->index() == b.halfedge()->index() );
}

//! Inequality operator
template <class HDS>
bool 
operator!=( const HDSNode<HDS>& a, const HDSNode<HDS>& b )
{
  return !(a == b );
}
*/

END_NAMESPACE_ADS

#endif
