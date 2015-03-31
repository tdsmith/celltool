// -*- C++ -*-

/*! 
  \file Node_AllIncCells.h
  \brief Class for a vertex in a mesh.
*/

#if !defined(__geom_mesh_simplicial_Node_AllIncCells_h__)
#define __geom_mesh_simplicial_Node_AllIncCells_h__

#include "Node_VertSelfId.h"

#include "../../../ads/iterator/IndirectIterator.h"

#include <vector>

#if defined(DEBUG_geom) && !defined(DEBUG_Node_AllIncCells)
#define DEBUG_Node_AllIncCells
#endif

BEGIN_NAMESPACE_GEOM

//! Node in a simplicial mesh that stores iterators to all incident cells.
/*!
  \param Mesh is the simplicial mesh.

  The base class has the vertex, an iterator to itself, and an identifier.
  This class stores iterators to all incident cells.
 */
template <class Mesh>
class Node_AllIncCells :
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
  // Private types.
  //

private:

  //! The container for incident cell iterators.
  typedef std::vector<cell_iterator> cell_iterator_container;

  //
  // More public types.
  //

public:

  //! An iterator for incident cell iterators.
  typedef typename cell_iterator_container::iterator cell_iter_iterator;
  //! A const iterator for incident cell iterators.
  typedef typename cell_iterator_container::const_iterator 
  cell_iter_const_iterator;

  //! An iterator for incident cells.
  typedef ads::IndirectIterator<typename cell_iterator_container::iterator> 
  cell_inc_to_node_iterator;
  //! A const iterator for incident cells.
  typedef ads::IndirectIterator<typename 
  cell_iterator_container::const_iterator> 
  cell_inc_to_node_const_iterator;

  //
  // Data
  //
      
private:
      
  //! The incident cells.
  cell_iterator_container _cells;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Uninitialized vertex, and null identifier and iterators.
  Node_AllIncCells() :
    base_type(),
    _cells()
  {}

  //! Construct from a vertex and an identifier.
  Node_AllIncCells( const vertex_type& vertex, const int identifier = -1 ) :
    base_type( vertex, identifier ),
    _cells()
  {}

  //! Build from a vertex and an identifier.
  void
  build( const vertex_type& vertex, const int identifier = -1,
	 const node_iterator self = 0 )
  {
    base_type::build( vertex, identifier, self );
    _cells.clear();
  }

#if 0
  //! Construct from a point, an identifier, and a cell iterator.
  Node_AllIncCells( const vertex_type& vertex, const int identifier = -1, 
		    const cell_iterator cell = 0 ) :
    base_type( point, identifier ),
    _cells()
  {
    if ( cell != 0 ) {
      add_cell( cell );
    }
  }

  //! Build from a point, an identifier, and a cell iterator.
  void
  build( const vertex_type& point, const int identifier = -1, 
	 const cell_iterator cell = 0 )
  {
    base_type::build( point, identifier );
    _cells.clear();
    if ( cell != 0 ) {
      add_cell( cell );
    }
  }
#endif

  //! Copy constructor.
  Node_AllIncCells( const Node_AllIncCells& x ) :
    base_type( x ),
    _cells( x._cells )
  {}

  //! Destructor.
  ~Node_AllIncCells()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Assignment operators.
  //! @{
      
  //! Assignment operator.
  Node_AllIncCells& 
  operator=( const Node_AllIncCells& x )
  {
    if ( &x != this ) {
      base_type::operator=( x );
      _cells = x._cells;
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
    // If there are no incident cells.
    if ( _cells.empty() ) {
      // Return a null iterator.
      return cell_const_iterator( 0 );
    }
    // Return the first adjacent cell in the container.
    return _cells.front();
  }

  //! Return the number of incident cells.
  int
  cells_size() const
  {
    return int(_cells.size());
  }

  //! Return the begining of the incident cells.
  cell_inc_to_node_const_iterator
  cells_begin() const
  {
    return cell_inc_to_node_const_iterator( _cells.begin() );
  }

  //! Return the end of the incident cells.
  cell_inc_to_node_const_iterator
  cells_end() const
  {
    return cell_inc_to_node_const_iterator( _cells.end() );
  }

  //! Return the begining of the incident cells iterators.
  cell_iter_const_iterator
  cell_iters_begin() const
  {
    return _cells.begin();
  }

  //! Return the end of the incident cell iterators.
  cell_iter_const_iterator
  cell_iters_end() const
  {
    return _cells.end();
  }

  //! Return true if the node is on the boundary.
  /*!
    The node is on the boundary if there are any incident boundary faces.
    To determine this, we examine each of the incident cells.
  */
  bool
  is_on_boundary() const
  {
    // For each simplex incident to this vertex.
    for ( typename cell_iterator_container::const_iterator cii = 
	    _cells.begin();
	  cii != _cells.end(); ++cii ) {
      // If this cell has a face incident to this vertex and on the boundary.
      if ( (*cii)->has_incident_boundary_face( self() ) ) {
	// This vertex is on the boundary.
	return true;
      }
    }
    // We did not encounter any incident boundary faces.
    return false;
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

  //! Set the iterator to the derived vertex.
  void
  set_self( const node_iterator self )
  { 
    base_type::set_self( self );
  }

  //! Return an iterator to an incident cell.
  cell_iterator
  cell()
  { 
    // If there are no incident cells.
    if ( _cells.empty() ) {
      // Return a null iterator.
      return cell_iterator( 0 );
    }
    // Return the first adjacent cell in the container.
    return _cells.front();
  }


  //! Add an adjacent cell.
  void
  add_cell( const cell_iterator c )
  {
    // The cell iterator should not be null.
#ifdef DEBUG_Node_AllIncCells
    assert( c != 0 );
#endif    
    _cells.push_back( c );
  }

  //! Add a range of adjacent cells.
  template <typename CellIterInIter>
  void
  add_cells( CellIterInIter begin, CellIterInIter end )
  {
    for ( ; begin != end; ++begin ) {
      add_cell( *begin );
    }
  }

  //! Remove an incident cell.
  void
  remove_cell( const cell_iterator c )
  {
    // Find the cell.
    typename cell_iterator_container::iterator i;
    i = std::find( _cells.begin(), _cells.end(), c );
    // The specified cell must be incident to this vertex.
    assert( i != _cells.end() );
    // Remove the incident cell.
    _cells.erase( i );
  }

  //! Return the begining of the incident cells.
  cell_inc_to_node_iterator
  cells_begin()
  {
    return cell_inc_to_node_iterator( _cells.begin() );
  }

  //! Return the end of the incident cells.
  cell_inc_to_node_iterator
  cells_end()
  {
    return cell_inc_to_node_iterator( _cells.end() );
  }

  //! Return the begining of the incident cell iterators.
  cell_iter_iterator
  cell_iters_begin()
  {
    return _cells.begin();
  }

  //! Return the end of the incident cell iterators.
  cell_iter_iterator
  cell_iters_end()
  {
    return _cells.end();
  }

  //! Replace this node with the specified one.
  /*!
    This amounts to changing the simplex-vertex incidences.
  */
  void
  replace( node_iterator x )
  {
    // Vertex Index.
    int vi;
    // Loop over the incident cells.
    cell_inc_to_node_iterator i = cells_begin();
    cell_inc_to_node_iterator i_end = cells_end();
    for ( ; i != i_end; ++i ) {
      // The index of this node.
      vi = i->index( self() );
      // Replace this node with the specified one.
      i->set_node( vi, x );
    }
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //! @{
  
  //! Return true if this is equal to \c x.
  bool
  operator==( const Node_AllIncCells& x ) const
  {
    return base_type::operator==( x ) && _cells == x._cells;
  }

  //! Return true if this is not equal to \c x.
  bool
  operator!=( const Node_AllIncCells& x ) const
  {
    return ! operator==( x );
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //! @{

  //! Write the vertex point, identifier, and the iterators.
  void
  put( std::ostream& out ) const
  {
    // Write the point and the vertex identifier.
    base_type::put( out );
    // Write the incident cell identifiers.
    const int i_end = int(_cells.size());
    // CONTINUE REMOVE
    out << " cells = ";
    for ( int i = 0; i != i_end; ++i ) {
      out << " " << _cells[i]->identifier();
    }
    out << "\n";
  }

  //! @}
};

END_NAMESPACE_GEOM

#endif
