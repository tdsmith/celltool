// -*- C++ -*-

/*! 
  \file HDSFace.h
  \brief Class for a face in a halfedge data structure.
*/

#if !defined(__HDSFace_h__)
#define __HDSFace_h__

#include "../defs.h"

#include "HDSNode.h"

BEGIN_NAMESPACE_ADS

//! A face in a halfedge data structure.
/*!
  Implements the minimum functionality for a face in a halfedge data 
  structure.  Faces in ads::HalfedgeDS should derive from 
  this class.
*/
template <class HDS>
class HDSFace :
  public HDSNode<HDS>
{
private:

  typedef HDSNode<HDS> base_type;
  typedef typename HDS::Face_Halfedge_circulator Face_Halfedge_circulator;
  typedef typename HDS::Face_Halfedge_const_circulator 
  Face_Halfedge_const_circulator;

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
  HDSFace() :
    base_type()
  {}

  //! Construct from a halfedge handle.
  HDSFace( Halfedge_handle h ) :
    base_type( h )
  {}

  //! Copy constructor.
  HDSFace( const HDSFace& x ) :
    base_type( x )
  {}

  //! Trivial destructor.
  ~HDSFace()
  {}

  //  
  // Accessors
  //

  //! Return a const handle to one of the incident half-edges.
  Halfedge_const_handle
  halfedge() const
  { 
    return base_type::halfedge();
  }

  //  
  // Manipulators
  //

  //! Return a reference to the handle to one of the incident half-edges.
  Halfedge_handle&
  halfedge()
  { 
    return base_type::halfedge();
  }

  //
  // Assignment operators.
  //
      
  //! Assignment operator.
  HDSFace& 
  operator=( const HDSFace& x )
  {
    base_type::operator=( x );
    return *this;
  }

  //
  // Circulators
  //

  //! Return a halfedge circulator.
  Face_Halfedge_circulator
  halfedges_begin()
  {
    return Face_Halfedge_circulator( halfedge() );
  }

  //! Return a halfedge const circulator.
  Face_Halfedge_const_circulator
  halfedges_begin() const
  {
    return Face_Halfedge_const_circulator( halfedge() );
  }

};

END_NAMESPACE_ADS

#endif
