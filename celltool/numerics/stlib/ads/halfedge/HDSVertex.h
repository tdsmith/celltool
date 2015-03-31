// -*- C++ -*-

/*! 
  \file HDSVertex.h
  \brief Class for a vertex in a halfedge data structure.
*/

#if !defined(__HDSVertex_h__)
#define __HDSVertex_h__

#include "HDSNode.h"

BEGIN_NAMESPACE_ADS

//! A vertex in a halfedge data structure.
/*!
  Implements the minimum functionality for a vertex in a halfedge data 
  structure.  Vertices in ads::HalfedgeDS should derive from 
  this class.
*/
template <class HDS>
class HDSVertex :
  public HDSNode<HDS>
{
private:

  typedef HDSNode<HDS> base_type;

public:

  //
  // Types
  //

  //! A handle to a halfedge.
  typedef typename base_type::Halfedge_handle Halfedge_handle;

  //! A handle to a const halfedge.
  typedef typename base_type::Halfedge_const_handle Halfedge_const_handle;

public:

  //
  // Constructors and Destructor
  //

  //! Default constructor.  Unititialized memory.
  HDSVertex() :
    base_type()
  {}

  //! Construct from a halfedge handle.
  HDSVertex( Halfedge_handle h ) :
    base_type( h )
  {}

  //! Copy constructor.
  HDSVertex( const HDSVertex& x ) :
    base_type( x )
  {}

  //! Trivial destructor.
  ~HDSVertex()
  {}

  //
  // Assignment operators.
  //
      
  //! Assignment operator.
  HDSVertex& 
  operator=( const HDSVertex& x )
  {
    base_type::operator=( x );
    return *this;
  }

};

END_NAMESPACE_ADS

#endif
