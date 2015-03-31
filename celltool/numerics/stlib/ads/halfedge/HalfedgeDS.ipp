// -*- C++ -*-

#if !defined(__HalfedgeDS_ipp__)
#error This file is an implementation detail of the class HalfedgeDS.
#endif

BEGIN_NAMESPACE_ADS

//
// Constructors
//

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
HalfedgeDS<Vertex,Halfedge,Face>::
HalfedgeDS( const HalfedgeDS& x ) :
  _vertices( x._vertices ),
  _halfedges( x._halfedges ),
  _faces( x._faces ),
  _null_vertex_handle( x._null_vertex_handle ),
  _null_halfedge_handle( x._null_halfedge_handle ),
  _null_face_handle( x._null_face_handle )
{
  shift_vertex_handles( _vertices.begin() - x._vertices.begin() );
  shift_halfedge_handles( _halfedges.begin() - x._halfedges.begin() );
  shift_face_handles( _faces.begin() - x._faces.begin() );
}

//
// Assignment operators.
//

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
HalfedgeDS<Vertex,Halfedge,Face>&
HalfedgeDS<Vertex,Halfedge,Face>::
operator=( const HalfedgeDS& x )
{
  if ( this != &x ) {
    // Copy the containers.
    _vertices = x._vertices;
    _halfedges = x._halfedges;
    _faces = x._faces;
    // Copy the null handles.
    _null_vertex_handle = x._null_vertex_handle;
    _null_halfedge_handle = x._null_halfedge_handle;
    _null_face_handle = x._null_face_handle;
    // Fix the handles.
    shift_vertex_handles( _vertices.begin() - x._vertices.begin() );
    shift_halfedge_handles( _halfedges.begin() - x._halfedges.begin() );
    shift_face_handles( _faces.begin() - x._faces.begin() );
  }
  return *this;
}
  
//
// Insert items
//
  
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
reserve( size_type v, size_type h, size_type f )
{
  // Record the address of the old vertex memory.
  Vertex_iterator vb = _vertices.begin();
  // Reserve memory for the vertices.
  _vertices.reserve( v );
  // If new memory was allocated.
  if ( vb != _vertices.begin() ) {
    // Update the vertex handles.
    shift_vertex_handles( std::distance( vb, _vertices.begin() ) );
  }

  // Record the address of the old halfedge memory.
  Halfedge_iterator hb = _halfedges.begin();
  // Reserve memory for the halfedges.
  _halfedges.reserve( h );
  // If new memory was allocated.
  if ( hb != _halfedges.begin() ) {
    // Update the halfedge handles.
    shift_halfedge_handles( std::distance( hb, _halfedges.begin() ) );
  }

  // Record the address of the old face memory.
  Face_iterator fb = _faces.begin();
  // Reserve memory for the faces.
  _faces.reserve( f );
  // If new memory was allocated.
  if ( fb != _faces.begin() ) {
    // Update the face handles.
    shift_face_handles( std::distance( fb, _faces.begin() ) );
  }
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
clear()
{
  _vertices.clear();
  _halfedges.clear();
  _faces.clear();
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Vertex_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_vertex()
{
  return insert_vertex( Vertex_type( _null_halfedge_handle ) );
}
    
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Vertex_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_vertex( const Vertex_type& x )
{
  if ( _vertices.size() == _vertices.capacity() ) {
    increase_vertex_capacity();
  }
  _vertices.push_back( x );
  return _vertices.end() - 1;
}
    
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Halfedge_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_halfedge()
{
  Halfedge_type h;
  h.prev() = _null_halfedge_handle;
  h.next() = _null_halfedge_handle;
  h.vertex() = _null_vertex_handle;
  h.face() = _null_face_handle;
  return insert_halfedge( h, h );
}
    
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Halfedge_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_halfedge( const Halfedge_type& x )
{
  Halfedge_type h;
  h.prev() = _null_halfedge_handle;
  h.next() = _null_halfedge_handle;
  h.vertex() = _null_vertex_handle;
  h.face() = _null_face_handle;
  return insert_halfedge( x, h );
}
    
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Halfedge_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_halfedge( const Halfedge_type& x, const Halfedge_type& y )
{
  // Add x.
  if ( _halfedges.size() == _halfedges.capacity() ) {
    increase_halfedge_capacity();
  }
  _halfedges.push_back( x );
  // Add y.
  if ( _halfedges.size() == _halfedges.capacity() ) {
    increase_halfedge_capacity();
  }
  _halfedges.push_back( y );
  // The handles to the x and y.
  Halfedge_handle xh = _halfedges.end() - 2;
  Halfedge_handle yh = _halfedges.end() - 1;
  // link the two halfedges together.
  xh->opposite() = yh;
  yh->opposite() = xh;
  // Return a handle to the first halfedge.
  return xh;
}
    
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Face_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_face()
{
  return insert_face( Face_type( _null_halfedge_handle ) );
}
    
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
typename HalfedgeDS<Vertex,Halfedge,Face>::Face_handle
HalfedgeDS<Vertex,Halfedge,Face>::
insert_face( const Face_type& x )
{
  if ( _faces.size() == _faces.capacity() ) {
    increase_face_capacity();
  }
  _faces.push_back( x );
  return _faces.end() - 1;
}

//
// I/O
//


template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
put( std::ostream& out ) const
{
  // Write the number of vertices, halfedges and faces.
  out << vertices_size() << " "
      << halfedges_size() << " "
      << faces_size() << '\n';
  // Data for vertices.
  for ( Vertex_const_iterator i = vertices_begin(); 
	i != vertices_end(); ++i ) {
    out << index( i->halfedge() ) << '\n';
  }
  // Data for halfedges.
  for ( Halfedge_const_iterator i = halfedges_begin(); 
	i != halfedges_end(); ++i ){
    out << index( i->opposite() ) << " "
	<< index( i->prev() ) << " "
	<< index( i->next() ) << " "
	<< index( i->vertex() ) << " "
	<< index( i->face() ) << '\n';
  }
  // Data for faces.
  for ( Face_const_iterator i = faces_begin(); i != faces_end(); ++i ) {
    out << index( i->halfedge() ) << '\n';
  }
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
get( std::istream& in )
{
  // Clear the halfedge data structure.
  clear();
  // Get the number of vertices, halfedges and faces.
  size_type v_size, h_size, f_size;
  in >> v_size >> h_size >> f_size;
  // Reserve memory.
  reserve( v_size, h_size, f_size );
  // Read the vertices.
  {
    Vertex_type v;
    int halfedge_index;
    for ( size_type i = 0; i != v_size; ++i ) {
      in >> halfedge_index;
      v.halfedge() = _halfedges.begin() + halfedge_index;
      _vertices.push_back( v );
    }
  }
  // Read the halfedges.
  {
    Halfedge_type h;
    int opposite_index, prev_index, next_index, vertex_index, face_index;
    for ( size_type i = 0; i != h_size; ++i ) {
      in >> opposite_index >> prev_index >> next_index >> vertex_index 
	 >> face_index;
      h.opposite() = _halfedges.begin() + opposite_index;
      h.prev() = _halfedges.begin() + prev_index;
      h.next() = _halfedges.begin() + next_index;
      h.vertex() = _vertices.begin() + vertex_index;
      h.face() = _faces.begin() + face_index;
      _halfedges.push_back( h );
    }
  }
  // Read the faces.
  {
    Face_type f;
    int halfedge_index;
    for ( size_type i = 0; i != f_size; ++i ) {
      in >> halfedge_index;
      f.halfedge() = _halfedges.begin() + halfedge_index;
      _faces.push_back( f );
    }
  }
}

//
// private member functions
//

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
increase_vertex_capacity()
{
  assert( _vertices.size() == _vertices.capacity() );
  // Record the address of the old memory.
  Vertex_iterator begin = _vertices.begin();
  // Add and delete an item to make the vector resize.
  _vertices.push_back( Vertex_type() );
  _vertices.pop_back();
  // Update the handles for the new memory location.
  shift_vertex_handles( std::distance( begin, _vertices.begin() ) );
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
increase_halfedge_capacity()
{
  assert( _halfedges.size() == _halfedges.capacity() );
  // Record the address of the old memory.
  Halfedge_iterator begin = _halfedges.begin();
  // Add and delete an item to make the vector resize.
  _halfedges.push_back( Halfedge_type() );
  _halfedges.pop_back();
  // Update the handles for the new memory location.
  shift_halfedge_handles( std::distance( begin, _halfedges.begin() ) );
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
increase_face_capacity()
{
  assert( _faces.size() == _faces.capacity() );
  // Record the address of the old memory.
  Face_iterator begin = _faces.begin();
  // Add and delete an item to make the vector resize.
  _faces.push_back( Face_type() );
  _faces.pop_back();
  // Update the handles for the new memory location.
  shift_face_handles( std::distance( begin, _faces.begin() ) );
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
shift_vertex_handles( difference_type d )
{
  for ( Halfedge_iterator i = halfedges_begin(); 
	i != halfedges_end(); ++i ) {
    if ( ! is_null( i->vertex() ) ) {
      i->vertex() += d;
    }
  }
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
shift_halfedge_handles( difference_type d )
{
  for ( Vertex_iterator i = vertices_begin(); 
	i != vertices_end(); ++i ) {
    if ( ! is_null( i->halfedge() ) ) {
      i->halfedge() += d;
    }
  }
  for ( Halfedge_iterator i = halfedges_begin(); 
	i != halfedges_end(); ++i ) {
    if ( ! is_null( i->opposite() ) ) {
      i->opposite() += d;
    }
    if ( ! is_null( i->prev() ) ) {
      i->prev() += d;
    }
    if ( ! is_null( i->next() ) ) {
      i->next() += d;
    }
  }
  for ( Face_iterator i = faces_begin(); 
	i != faces_end(); ++i ) {
    if ( ! is_null( i->halfedge() ) ) {
      i->halfedge() += d;
    }
  }
}

template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
void
HalfedgeDS<Vertex,Halfedge,Face>::
shift_face_handles( difference_type d )
{
  for ( Halfedge_iterator i = halfedges_begin(); 
	i != halfedges_end(); ++i ) {
    if ( ! is_null( i->face() ) ) {
      i->face() += d;
    }
  }
}

//
// Equality.
//

template < template<class> class V, 
	   template<class> class H, 
	   template<class> class F >
bool
operator==( const HalfedgeDS<V,H,F>& a,
	    const HalfedgeDS<V,H,F>& b )
{
  // Check the sizes.
  if ( a.vertices_size() != b.vertices_size() ||
       a.halfedges_size() != b.halfedges_size() ||
       a.faces_size() != b.faces_size() ) {
    return false;
  }
  // Check the vertices.
  for ( typename HalfedgeDS<V,H,F>::Vertex_const_iterator 
	  i = a.vertices_begin(), j = b.vertices_begin();
	i != a.vertices_end(); ++i, ++j ) {
    if ( index( i->halfedge() ) != index( j->halfedge() ) ) {
      return false;
    }
  }
  // Check the halfedges.
  for ( typename HalfedgeDS<V,H,F>::Halfedge_const_iterator 
	  i = a.halfedges_begin(), j = b.halfedges_begin();
	i != a.halfedges_end(); ++i, ++j ) {
    if ( index( i->opposite() ) != index( j->opposite() ) ||
	 index( i->prev() ) != index( j->prev() ) ||
	 index( i->next() ) != index( j->next() ) ||
	 index( i->vertex() ) != index( j->vertex() ) ||
	 index( i->face() ) != index( j->face() ) ) {
      return false;
    }
  }
  // Check the faces.
  for ( typename HalfedgeDS<V,H,F>::Face_const_iterator 
	  i = a.faces_begin(), j = b.faces_begin();
	i != a.faces_end(); ++i, ++j ) {
    if ( index( i->halfedge() ) != index( j->halfedge() ) ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_ADS
