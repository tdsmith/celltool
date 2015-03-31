// -*- C++ -*-

/*! 
  \file Node_VertSelf.h
  \brief A node in a simplicial mesh that stores a vertex and an iterator to itself.
*/

#if !defined(__geom_mesh_simplicial_Node_VertSelf_h__)
#define __geom_mesh_simplicial_Node_VertSelf_h__

#include "../../defs.h"

#include "../../../ads/array/FixedArray.h"

#if defined(DEBUG_geom) && !defined(DEBUG_Node_VertSelf)
#define DEBUG_Node_VertSelf
#endif

BEGIN_NAMESPACE_GEOM

//! A node in a simplicial mesh that stores a vertex and an iterator to itself.
/*!
  This class stores a vertex (a Cartesian point) and an iterator to itself.
  (The iterator could point to a derived node class.)
 */
template <class Mesh>
class Node_VertSelf
{
  //
  // Enumerations.
  //

public:

  //! The space dimension.
  enum { N = Mesh::N };
  
  //
  // Public types.
  //

public:

  //! The simplicial mesh.
  typedef Mesh mesh_type;

  //! An iterator to a node.
  typedef typename mesh_type::node_iterator node_iterator;
  //! An iterator to a const node.
  typedef typename mesh_type::node_const_iterator node_const_iterator;

  //! The number type.
  typedef typename mesh_type::number_type number_type;
  //! The vertex (a Cartesian point).
  typedef ads::FixedArray<N,number_type> vertex_type;

  //
  // Data
  //
      
private:
      
  //! The vertex (a Cartesian point).
  vertex_type _vertex;
  //! An iterator to the derived node.
  node_iterator _self;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Uninitialized point, null iterator.
  Node_VertSelf() :
    _vertex(),
    _self( 0 )
  {}

  //! Construct from a vertex and a node iterator.
  Node_VertSelf( const vertex_type& vertex, const node_iterator self = 0 ) :
    _vertex( vertex ),
    _self( self )
  {}

  //! Build from a vertex and a node iterator
  void
  build( const vertex_type& vertex, const node_iterator self = 0 )
  {
    _vertex = vertex;
    _self = self;
  }

  //! Copy constructor.
  Node_VertSelf( const Node_VertSelf& x ) :
    _vertex( x._vertex ),
    _self( x._self )
  {}

  //! Destructor.
  ~Node_VertSelf()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  //! @{
      
  //! Assignment operator.
  Node_VertSelf& 
  operator=( const Node_VertSelf& x )
  {
    if ( &x != this ) {
      _vertex = x._vertex;
      _self = x._self;
    }
    return *this;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the vertex.
  const vertex_type&
  vertex() const
  { 
    return _vertex;
  }

  //! Return a const iterator to the (possibly derived) node.
  node_const_iterator
  self() const
  { 
    return _self;
  }

  //! Return the identifier (rank) of this node.
  /*!
    The identifier is in the range [0...num_vertices).
    If the self iterator is null, return -1.
    
    \note This function may be very inefficient.  If the vertices are stored 
    in a list, it will count from the beginning of the list to determine 
    the identifier.
  */
  int
  identifier( const node_const_iterator nodes_begin ) const
  {
    if ( _self == 0 ) {
      return -1;
    }
    return std::distance( nodes_begin, node_const_iterator( _self ) );
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Return an iterator to the (possibly derived) node.
  node_iterator
  self()
  { 
    return _self;
  }

  //! Set the point
  void
  set_vertex( const vertex_type& vertex )
  { 
    _vertex = vertex;
  }

  //! Set the iterator to the derived node.
  void
  set_self( const node_iterator self )
  { 
    _self = self;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //! @{
  
  //! Return true if this is equal to \c x.
  bool
  operator==( const Node_VertSelf& x ) const
  {
    return _self == x._self && _vertex == x._vertex;
  }

  //! Return true if this is not equal to \c x.
  bool
  operator!=( const Node_VertSelf& x ) const
  {
    return ! operator==( x );
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //! @{

  //! Write the vertex and the node identifier.
  /*!
    \note Unless the nodes are stored in a random access container, 
    this function is very inefficient.
  */
  void
  put( std::ostream& out, const node_const_iterator nodes_begin ) const
  {
    out << _vertex << " " << identifier( nodes_begin );
  }

  //! @}
};

END_NAMESPACE_GEOM

#endif
