// -*- C++ -*-

/*! 
  \file HalfedgeDS.h
  \brief Class for a halfedge data structure.
*/

#if !defined(__HalfedgeDS_h__)
#define __HalfedgeDS_h__

#include "../defs.h"

#include "circulator.h"

#include <vector>

#include <cassert>

BEGIN_NAMESPACE_ADS
  
//! A halfedge data structure.
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
class HalfedgeDS
{
  //
  // Types
  //

private:

  typedef HalfedgeDS<Vertex, Halfedge, Face> HDS;

public:

  //! A vertex in the HDS.
  typedef Vertex<HDS> Vertex_type;
  //! A halfedge in the HDS.
  typedef Halfedge<HDS> Halfedge_type;
  //! A face in the HDS.
  typedef Face<HDS> Face_type;

private:

  typedef std::vector<Vertex_type> Vertex_container;
  typedef std::vector<Halfedge_type> Halfedge_container;
  typedef std::vector<Face_type> Face_container;
    
public:

  //! Vertex iterator.
  typedef typename Vertex_container::iterator Vertex_iterator;
  //! Halfedge iterator.
  typedef typename Halfedge_container::iterator Halfedge_iterator;
  //! Face iterator.
  typedef typename Face_container::iterator Face_iterator;

  //! Vertex const iterator.
  typedef typename Vertex_container::const_iterator Vertex_const_iterator;
  //! Halfedge const iterator.
  typedef typename Halfedge_container::const_iterator 
  Halfedge_const_iterator;
  //! Face const iterator.
  typedef typename Face_container::const_iterator Face_const_iterator;

  //! Vertex handle.
  typedef typename Vertex_container::iterator Vertex_handle;
  //! Halfedge handle.
  typedef typename Halfedge_container::iterator Halfedge_handle;
  //! Face handle.
  typedef typename Face_container::iterator Face_handle;

  //! const Vertex handle.
  typedef typename Vertex_container::const_iterator Vertex_const_handle;
  //! const Halfedge handle.
  typedef typename Halfedge_container::const_iterator Halfedge_const_handle;
  //! const Face handle.
  typedef typename Face_container::const_iterator Face_const_handle;

  //! The size type.
  typedef typename Vertex_container::size_type size_type;
  //! The pointer difference type.
  typedef typename Vertex_container::difference_type difference_type;

  //! Halfedge around Face circulator.
  typedef Face_Halfedge_circ<Halfedge_handle> Face_Halfedge_circulator;
  //! Halfedge around Face const circulator.
  typedef Face_Halfedge_circ<Halfedge_const_handle> 
  Face_Halfedge_const_circulator;

private:
      
  //
  // Data
  //
    
  // Containers.
  Vertex_container   _vertices;
  Halfedge_container _halfedges;
  Face_container     _faces;

  // Null handles.
  Vertex_handle   _null_vertex_handle;
  Halfedge_handle _null_halfedge_handle;
  Face_handle     _null_face_handle;

public:

  //
  // Constructors and Destructor
  //

  //! Default constructor.  Empty containers.
  HalfedgeDS() :
    _vertices(),
    _halfedges(),
    _faces(),
    _null_vertex_handle( 0 ),
    _null_halfedge_handle( 0 ),
    _null_face_handle( 0 )
  {}

  //! Size constructor.  
  /*!
    Reserve memory for \c v vertices, \c h halfedges and \c f faces.
  */
  HalfedgeDS( size_type v, size_type h, size_type f ) :
    _vertices(),
    _halfedges(),
    _faces(),
    _null_vertex_handle( 0 ),
    _null_halfedge_handle( 0 ),
    _null_face_handle( 0 )
  {
    _vertices.reserve( v );
    _halfedges.reserve( h );
    _faces.reserve( f );
  }

  //! Copy constructor.
  HalfedgeDS( const HalfedgeDS& x );

  //! Destructor.
  ~HalfedgeDS()
  {}

  //! Assignment operator.
  HalfedgeDS& 
  operator=( const HalfedgeDS& x );

  //  
  // Accessors
  //

  //! Return a const iterator to the beginning of the vertices.
  Vertex_const_iterator
  vertices_begin() const
  {
    return _vertices.begin();
  }

  //! Return a const iterator to the end of the vertices.
  Vertex_const_iterator
  vertices_end() const
  {
    return _vertices.end();
  }

  //! Return a const iterator to the beginning of the half-edges.
  Halfedge_const_iterator
  halfedges_begin() const
  {
    return _halfedges.begin();
  }

  //! Return a const iterator to the end of the half-edges.
  Halfedge_const_iterator
  halfedges_end() const
  {
    return _halfedges.end();
  }

  //! Return a const iterator to the beginning of the faces.
  Face_const_iterator
  faces_begin() const
  {
    return _faces.begin();
  }

  //! Return a const iterator to the end of the faces.
  Face_const_iterator
  faces_end() const
  {
    return _faces.end();
  }

  //! Return the number of vertices.
  size_type
  vertices_size() const
  {
    return _vertices.size();
  }
    
  //! Return the number of half-edges.
  size_type
  halfedges_size() const
  {
    return _halfedges.size();
  }
    
  //! Return the number of faces.
  size_type
  faces_size() const
  {
    return _faces.size();
  }

  //  
  // Manipulators
  //

  //! Return an iterator to the beginning of the vertices.
  Vertex_iterator
  vertices_begin()
  {
    return _vertices.begin();
  }

  //! Return an iterator to the end of the vertices.
  Vertex_iterator
  vertices_end()
  {
    return _vertices.end();
  }

  //! Return an iterator to the beginning of the half-edges.
  Halfedge_iterator
  halfedges_begin()
  {
    return _halfedges.begin();
  }

  //! Return an iterator to the end of the half-edges.
  Halfedge_iterator
  halfedges_end()
  {
    return _halfedges.end();
  }

  //! Return an iterator to the beginning of the faces.
  Face_iterator
  faces_begin()
  {
    return _faces.begin();
  }

  //! Return an iterator to the end of the faces.
  Face_iterator
  faces_end()
  {
    return _faces.end();
  }

  //
  // Utility
  //

  //! Return true if the data structure is valid.
  /*!
    To be valid, all the handles must be valid or null.
  */
  bool
  is_valid() const
  {
    // Check the vertices.
    for ( Vertex_const_iterator i = vertices_begin(); 
	  i != vertices_end(); ++i ) {
      if ( ! is_valid( i->halfedge() ) ) {
	return false;
      }
    }
    // Check the halfedges.
    for ( Halfedge_const_iterator i = halfedges_begin(); 
	  i != halfedges_end(); ++i ) {
      if ( ! ( is_valid( i->opposite() ) && 
	       is_valid( i->prev() ) && 
	       is_valid( i->next() ) && 
	       is_valid( i->vertex() ) && 
	       is_valid( i->face() ) ) ) {
	return false;
      }
    }
    // Check the faces.
    for ( Face_const_iterator i = faces_begin(); 
	  i != faces_end(); ++i ) {
      if ( ! is_valid( i->halfedge() ) ) {
	return false;
      }
    }
    return true;
  }
    
  //! Return true if the vertex handle is null or points to a vertex.
  bool
  is_valid( Vertex_const_handle h ) const
  {
    if ( is_null( h ) || ( vertices_begin() <= h && h < vertices_end() ) ) {
      return true;
    }
    return false;
  }
    
  //! Return true if the halfedge handle is null or points to a halfedge.
  bool
  is_valid( Halfedge_const_handle h ) const
  {
    if ( is_null( h ) || ( halfedges_begin() <= h && h < halfedges_end() ) ){
      return true;
    }
    return false;
  }
    
  //! Return true if the face handle is null or points to a face.
  bool
  is_valid( Face_const_handle h ) const
  {
    if ( is_null( h ) || ( faces_begin() <= h && h < faces_end() ) ){
      return true;
    }
    return false;
  }
    
  //! Return the index of a vertex specified by its handle.
  difference_type
  index( Vertex_const_handle h ) const
  {
    if ( h == _null_vertex_handle ) {
      return std::distance( vertices_begin(), vertices_end() );
    }
    return std::distance( vertices_begin(), h );
  }
    
  //! Return the index of a half-edge specified by its handle.
  difference_type
  index( Halfedge_const_handle h ) const
  {
    if ( h == _null_halfedge_handle ) {
      return std::distance( halfedges_begin(), halfedges_end() );
    }
    return std::distance( halfedges_begin(), h );
  }
    
  //! Return the index of a face specified by its handle.
  difference_type
  index( Face_const_handle h ) const
  {
    if ( h == _null_face_handle ) {
      return std::distance( faces_begin(), faces_end() );
    }
    return std::distance( faces_begin(), h );
  }

  //! Return true if the Vertex_handle is null.
  bool
  is_null( Vertex_const_handle h ) const
  {
    return ( h == _null_vertex_handle );
  }
    
  //! Return true if the Halfedge_handle is null.
  bool
  is_null( Halfedge_const_handle h ) const
  {
    return ( h == _null_halfedge_handle );
  }
    
  //! Return true if the Face_handle is null.
  bool
  is_null( Face_const_handle h ) const
  {
    return ( h == _null_face_handle );
  }
    
  //
  // Insert items
  //

  //! Reserve memory for \c v vertices, \c h halfedges and \c f faces.
  void
  reserve( size_type v, size_type h, size_type f );

  //! Clear the data structure.
  void
  clear();
    
  //! Add a vertex with a null halfedge and return its handle.
  Vertex_handle
  insert_vertex();
    
  //! Add a copy of \c x and return its handle.
  Vertex_handle
  insert_vertex( const Vertex_type& x );
    
  //! Add a halfedge and its opposite. Return a handle to the former.
  /*! 
    Use the default constructor. 
  */
  Halfedge_handle
  insert_halfedge();
    
  //! Add a copy of \c x, (and its opposite), and return its handle.
  Halfedge_handle
  insert_halfedge( const Halfedge_type& x );
    
  //! Add a copy of \c x and its opposite \c y, and return its handle.
  Halfedge_handle
  insert_halfedge( const Halfedge_type& x, const Halfedge_type& y );
    
  //! Add a face with a null halfedge and return its handle.
  Face_handle
  insert_face();
    
  //! Add a copy of \c x and return its handle.
  Face_handle
  insert_face( const Face_type& x );

  //
  // I/O
  //

  //! Write indices for each of the handles.
  void
  put( std::ostream& out ) const;

  //! Read indices and convert them to handles.
  void
  get( std::istream& in );

private:

  //
  // private member functions
  //

  //! Increase the capacity for the vertices and update the handles.
  void
  increase_vertex_capacity();

  //! Increase the capacity for the halfedges and update the handles.
  void
  increase_halfedge_capacity();

  //! Increase the capacity for the faces and update the handles.
  void
  increase_face_capacity();

  //! Shift the vertex handles of each halfedge by \c d.
  void 
  shift_vertex_handles( difference_type d );

  //! Shift the halfedge handles of each vertex, halfedge and face by \c d.
  void 
  shift_halfedge_handles( difference_type d );

  //! Shift the face handles of each halfedge by \c d.
  void 
  shift_face_handles( difference_type d );
};

//
// Equality.
//

//! Return true if the half-edge data structures are equal.
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
bool
operator==( const HalfedgeDS<Vertex,Halfedge,Face>& a,
	    const HalfedgeDS<Vertex,Halfedge,Face>& b );

//! Return true if the half-edge data structures are not equal.
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
bool
operator!=( const HalfedgeDS<Vertex,Halfedge,Face>& a,
	    const HalfedgeDS<Vertex,Halfedge,Face>& b )
{
  return !(a == b );
}

//
// I/O
//

//! Write to a stream using the put() member function.
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
std::ostream&
operator<<( std::ostream& out, const HalfedgeDS<Vertex,Halfedge,Face>& x )
{
  x.put( out );
  return out;
}
  
//! Read from a stream using the get() member function.
template < template<class> class Vertex, 
	   template<class> class Halfedge, 
	   template<class> class Face >
inline
std::istream&
operator>>( std::istream& in, HalfedgeDS<Vertex,Halfedge,Face>& x )
{
  x.get( in );
  return in;
}
  
END_NAMESPACE_ADS

#define __HalfedgeDS_ipp__
#include "HalfedgeDS.ipp"
#undef __HalfedgeDS_ipp__

#endif
