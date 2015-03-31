// -*- C++ -*-

/*! 
  \file Node_OneIncCell.h
  \brief Class for a vertex in a mesh.
*/

#if !defined(__geom_mesh_simplicial_Node_OneIncCell_h__)
#define __geom_mesh_simplicial_Node_OneIncCell_h__

#include "Node_VertSelfId.h"

#if defined(DEBUG_geom) && !defined(DEBUG_Node_OneIncCell)
#define DEBUG_Node_OneIncCell
#endif

BEGIN_NAMESPACE_GEOM

//! Vertex in a simplicial mesh that stores one cell incidence.
/*!
  \param Mesh is the simplicial mesh.

  The base class has the Cartesian point and a vertex iterator.
  This class stores an iterator to one incident cell.
 */
template <class Mesh>
class Node_OneIncCell :
  public Node_VertSelfId<Mesh>
{
  //
  // Private types.
  //

private:

  typedef Node_VertSelfId<Mesh> base_type;

  //
  // Enumerations.
  //

public:

  //! The space dimension.
  enum { N = base_type::N };

  //
  // Public types.
  //

public:

  //! The simplicial mesh.
  typedef Mesh mesh_type;

  //! An iterator to a cell.
  typedef typename mesh_type::cell_iterator cell_iterator;
  //! An iterator to a const cell.
  typedef typename mesh_type::cell_const_iterator cell_const_iterator;

  //! An iterator to a node.
  typedef typename base_type::node_iterator node_iterator;
  //! An iterator to a const node.
  typedef typename base_type::node_const_iterator node_const_iterator;

  //! The number type.
  typedef typename base_type::number_type number_type;
  //! A vertex (a Cartesian point).
  typedef typename base_type::vertex_type vertex_type;

  //
  // Data
  //
      
private:
      
  //! An incident cell.
  cell_iterator _cell;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Uninitialized point, and null identifier and iterators.
  Node_OneIncCell() :
    base_type(),
    _cell( 0 )
  {}

  //! Construct from a point, an identifier, and a cell iterator.
  Node_OneIncCell( const vertex_type& point, const int identifier = -1,
		   const cell_iterator cell = 0 ) :
    base_type( point, identifier ),
    _cell( cell )
  {}

  //! Build from a point, an identifier, and a cell iterator.
  void
  build( const vertex_type& point, const int identifier = -1, 
	 const cell_iterator cell = 0 )
  {
    base_type::build( point, identifier );
    _cell = cell;
  }

  //! Copy constructor.
  Node_OneIncCell( const Node_OneIncCell& x ) :
    base_type( x ),
    _cell( x._cell )
  {}

  //! Destructor.
  ~Node_OneIncCell()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  //! @{
      
  //! Assignment operator.
  Node_OneIncCell& 
  operator=( const Node_OneIncCell& x )
  {
    if ( &x != this ) {
      base_type::operator=( x );
      _cell = x._cell;
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
    return base_type::vertex();
  }

  //! Return the identifier of this node.
  /*!
    Typically, the identifier is in the range [0...num_vertices).
    and a value of -1 indicates that the identifier has not been calculated.
  */
  int
  identifier() const
  {
    return base_type::identifier();
  }

  //! Return a const iterator to itself.
  node_const_iterator
  self() const
  { 
    return base_type::self();
  }

  //! Return a const iterator to an incident cell.
  cell_const_iterator
  cell() const
  { 
    return _cell;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the vertex.
  void
  set_vertex( const vertex_type& vertex )
  { 
    base_type::set_vertex( vertex );
  }

  //! Set the identifier.
  /*!
    \note This member function is const because the identifier is mutable.
    It is intended to be modified as the mesh changes.
  */
  void
  set_identifier( const int identifier ) const
  { 
    base_type::set_identifier( identifier );
  }

  //! Return an iterator to itself.
  node_iterator
  self()
  { 
    return base_type::self();
  }

  //! Set the iterator to the derived node.
  void
  set_self( const node_iterator v )
  { 
    base_type::set_self( v );
  }

  //! Return an iterator to an incident cell.
  cell_iterator
  cell()
  { 
    return _cell;
  }

  //! Add an adjacent cell.
  void
  add_cell( const cell_iterator c )
  { 
    _cell = c;
  }

  //! Remove an adjacent cell.
  void
  remove_cell( const cell_iterator c )
  { 
    // Look for another adjacent cell.
    // CONTINUE;
    assert( false );
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //! @{
  
  //! Return true if this is equal to \c x.
  bool
  operator==( const Node_OneIncCell& x ) const
  {
    return base_type::operator==( x ) && _cell == x._cell;
  }

  //! Return true if this is not equal to \c x.
  bool
  operator!=( const Node_OneIncCell& x ) const
  {
    return ! operator==( x );
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //! @{

  //! Write the vertex, identifier, and the incident cell identifier.
  void
  put( std::ostream& out ) const
  {
    // Write the point and the vertex identifier.
    base_type::put( out );
    // Write the incident cell identifier.
    out << " " << _cell->identifier();
  }

  //! @}
};

END_NAMESPACE_GEOM

#endif
