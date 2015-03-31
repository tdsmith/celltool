// -*- C++ -*-

/*! 
  \file Node_VertSelfId.h
  \brief A node in a simplicial mesh that stores a vertex, an iterator to itself, and an identifier.
*/

#if !defined(__geom_mesh_simplicial_Node_VertSelfId_h__)
#define __geom_mesh_simplicial_Node_VertSelfId_h__

#include "../../defs.h"

#include "../../../ads/array/FixedArray.h"

#if defined(DEBUG_geom) && !defined(DEBUG_Node_VertSelfId)
#define DEBUG_Node_VertSelfId
#endif

BEGIN_NAMESPACE_GEOM

//! A node in a simplicial mesh that stores a vertex, an iterator to itself, and an identifier.
/*!
  \param Mesh is the simplicial mesh class.

  This vertex class stores a vertex (a Cartesian point), an iterator to itself,
  and an integer identifier
 */
template <class Mesh>
class Node_VertSelfId
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
  //! A Cartesian point.
  typedef ads::FixedArray<N,number_type> vertex_type;

  //
  // Data
  //
      
private:
      
  //! The vertex (a Cartesian point).
  vertex_type _vertex;
  //! An iterator to the derived vertex.
  node_iterator _self;
  //! The identifier of this vertex.
  mutable int _identifier;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Uninitialized point, null iterator, id = -1.
  Node_VertSelfId() :
    _vertex(),
    _self( 0 ),
    _identifier( -1 )
  {}

  //! Construct from a point and an identifier.
  Node_VertSelfId( const vertex_type& vertex, const int identifier = -1 ) :
    _vertex( vertex ),
    _self( 0 ),
    _identifier( identifier )
  {}

  //! Build from a point, an identifier and a vertex iterator.
  void
  build( const vertex_type& vertex, const int identifier = -1,
	 const node_iterator self = 0 )
  {
    _vertex = vertex;
    _self = self;
    _identifier = identifier;
  }

  //! Copy constructor.
  Node_VertSelfId( const Node_VertSelfId& x ) :
    _vertex( x._vertex ),
    _self( x._self ),
    _identifier( x._identifier )
  {}

  //! Destructor.
  ~Node_VertSelfId()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  //! @{
      
  //! Assignment operator.
  Node_VertSelfId& 
  operator=( const Node_VertSelfId& x )
  {
    if ( &x != this ) {
      _vertex = x._vertex;
      _self = x._self;
      _identifier = x._identifier;
    }
    return *this;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the Cartesian point.
  const vertex_type&
  vertex() const
  { 
    return _vertex;
  }

  //! Return a const iterator to the derived vertex.
  node_const_iterator
  self() const
  { 
    return _self;
  }

  //! Return the identifier of this vertex.
  /*!
    Typically, the identifier is in the range [0...num_vertices).
    and a value of -1 indicates that the identifier has not been calculated.
  */
  int
  identifier() const
  {
    return _identifier;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the point.
  void
  set_vertex( const vertex_type& vertex )
  { 
    _vertex = vertex;
  }

  //! Return an iterator to the derived vertex.
  node_iterator
  self()
  { 
    return _self;
  }

  //! Set the iterator to the derived vertex.
  void
  set_self( const node_iterator self )
  { 
    _self = self;
  }

  //! Set the identifier.
  /*!
    \note This member function is const because the identifier is mutable.
    It is intended to be modified as the mesh changes.
  */
  void
  set_identifier( const int identifier ) const
  { 
    _identifier = identifier;
  }

  //! This is here for compatibility with the other node classes.  The function does nothing.
  template <typename CellIterator>
  void
  add_cell( const CellIterator c )
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //! @{
  
  //! Return true if this is equal to \c x.
  bool
  operator==( const Node_VertSelfId& x ) const
  {
    return _self == x._self && _vertex == x._vertex && 
      _identifier == x._identifier;
  }

  //! Return true if this is not equal to \c x.
  bool
  operator!=( const Node_VertSelfId& x ) const
  {
    return ! operator==( x );
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //! @{

  //! Write the vertex point and the identifier.
  void
  put( std::ostream& out ) const
  {
    out << "Vertex = " << _vertex << " Id = " << identifier();
  }

  //! @}
};

END_NAMESPACE_GEOM

#endif
