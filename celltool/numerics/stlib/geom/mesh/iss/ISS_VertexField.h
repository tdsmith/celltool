// -*- C++ -*-

/*! 
  \file ISS_VertexField.h
  \brief Interpolation on an indexed simplex set.
*/

#if !defined(__geom_ISS_VertexField_h__)
#define __geom_ISS_VertexField_h__

#include "../../defs.h"

#include "../../../ads/array/Array.h"
// Interpolation functions.
#include "../../../numerical/interpolation/simplex.h"

#if defined(DEBUG_geom) && !defined(DEBUG_ISS_VertexField)
#define DEBUG_ISS_VertexField
#endif

BEGIN_NAMESPACE_GEOM

//! Indexed simplex set with fields at the vertices.
/*!
  \param ISS is the indexed simplex set.
  \param F is the field type.  By default it is the number type of the mesh.

  This class stores a reference to an indexed simplex set and a reference
  to an array of fields.
*/
template<class ISS, 
	 typename F = typename ISS::Number>
class ISS_VertexField
{
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
  typedef typename FieldContainer::parameter_type FieldParameterType;
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

  //! The indexed simplex set.
  const IssType& _iss;
  //! The field values at the vertices.
  FieldContainer _fields;
  //! The simplex of positions.
  mutable ads::FixedArray<ISS::M + 1,Vertex> _pos;
  //! The simplex of vertex field values.
  mutable ads::FixedArray<ISS::M + 1,Field> _val;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  ISS_VertexField();

  //! Assignment operator not implemented.
  ISS_VertexField&
  operator=(const ISS_VertexField& x);

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
  ISS_VertexField(const IssType& iss,
		  const ads::Array<1,Field,A2>& fields) :
    _iss(iss),
    _fields(fields),
    _pos(),
    _val() {
    assert(_iss.getVerticesSize() == _fields.size());
  }

  //! Construct from pointers to the vertices and simplices.
  /*!
    \param iss is the indexed simplex set.
    \param numVertices is the number of vertices.
    \param fields is the array of fields defined at each vertex.
   */
  ISS_VertexField(const IssType& iss,
		  const SizeType numVertices, 
		  FieldVoidPointer fields) :
    _iss(iss),
    _fields(numVertices, fields),
    _pos(),
    _val() {
    assert(_iss.getVerticesSize() == numVertices);
  }

  //! Copy constructor.
  ISS_VertexField(const ISS_VertexField& other) :
    _iss(other._iss),
    _fields(other._fields),
    _pos(),
    _val()
  {}

  //! Destructor has no effect on the indexed simplex set.
  ~ISS_VertexField()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical Functions.
  //! @{

  //! Return the interpolated field for the n_th simplex and the point \c x.
  FieldParameterType
  interpolate(int n, const Vertex& x) const;

  //! @}
};

END_NAMESPACE_GEOM

#define __geom_ISS_VertexField_ipp__
#include "ISS_VertexField.ipp"
#undef __geom_ISS_VertexField_ipp__

#endif
