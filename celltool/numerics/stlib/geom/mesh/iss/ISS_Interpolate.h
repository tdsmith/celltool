// -*- C++ -*-

/*! 
  \file ISS_Interpolate.h
  \brief Interpolation on an indexed simplex set.
*/

#if !defined(__geom_ISS_Interpolate_h__)
#define __geom_ISS_Interpolate_h__

#include "ISS_SimplexQuery.h"
#include "ISS_VertexField.h"

#if defined(DEBUG_geom) && !defined(DEBUG_ISS_Interpolate)
#define DEBUG_ISS_Interpolate
#endif

BEGIN_NAMESPACE_GEOM

//! Interpolation for an indexed simplex set with fields at the vertices.
/*!
  \param ISS is the indexed simplex set.
  \param F is the field type.  By default it is the number type of the mesh.

  This class stores a reference to an ISS_SimplexQuery for determining
  which which simplex to use in the interpolation and a reference 
  to an ISS_VertexField for performing the linear interpolation.
*/
template<class ISS, 
	 typename F = typename ISS::Number>
class ISS_Interpolate {
  //
  // Private types.
  //

private:

  //! The indexed simplex set.
  typedef ISS IssType;
  //! The size type.
  typedef typename IssType::SizeType SizeType;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename IssType::Number Number;
  //! A vertex.
  typedef typename IssType::Vertex Vertex;

  //
  // Field types.
  //

  //! The field type.
  typedef F Field;
  //! The field container.
  typedef ads::Array<1,Field,false> FieldContainer;
  //! The constant parameter type.
  /*! 
    This is used for passing the value type as an argument or returning 
    the value type.
  */
  typedef typename FieldContainer::parameter_type 
  FieldParameterType;
  //! A void pointer for the field type.
  /*!
    If the field type has a top level const qualifier, this is 
    <tt>const void*</tt>.  Otherwise it is \c void*.
  */
  typedef typename FieldContainer::void_pointer FieldVoidPointer;

  //
  // Data.
  //

private:

  //! The simplex query data structure.
  ISS_SimplexQuery<ISS> _simplexQuery;
  //! The interpolation data structure.
  ISS_VertexField<ISS,Field> _vertexField;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  ISS_Interpolate();

  //! Assignment operator not implemented.
  ISS_Interpolate&
  operator=(const ISS_Interpolate&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Construct from the indexed simplex set and the fields.
  /*!
    \param iss is the indexed simplex set.
    \param fields is the array of fields.
  */
  template <bool A2>
  ISS_Interpolate(const IssType& iss,
		  const ads::Array<1,Field,A2>& fields) :
    _simplexQuery(iss),
    _vertexField(iss, fields)
  {}

  //! Construct from pointers to the vertices and simplices.
  /*!
    \param iss is the indexed simplex set.
    \param num_vertices is the number of vertices.
    \param fields is the array of fields defined at each vertex.
   */
  ISS_Interpolate(const IssType& iss,
		  const SizeType num_vertices, 
		  FieldVoidPointer fields) :
    _simplexQuery(iss),
    _vertexField(iss, num_vertices, fields)
  {}

  //! Copy constructor.
  ISS_Interpolate(const ISS_Interpolate& other) :
    _simplexQuery(other._simplexQuery),
    _vertexField(other._vertexField)
  {}

  //! The destructor has no effect on the simplex query are the vertex field data structures.
  ~ISS_Interpolate()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical Functions.
  //! @{

  //! Return the interpolated field for the point \c x.
  FieldParameterType
  operator()(const Vertex& x) const {
    return _vertexField.interpolate
      (_simplexQuery.computeMinimumDistanceIndex(x), x);
  }

  //! @}
};

END_NAMESPACE_GEOM

#endif
