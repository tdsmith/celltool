// -*- C++ -*-

/*!
  \file numerical/interpolation/LinInterpGrid.h
  \brief Functor for linear interpolation on a regular grid.
*/

#if !defined(__numerical_interpolation_linear_h__)
#define __numerical_interpolation_linear_h__

#include "../defs.h"

#include "../../ads/array/Array.h"
#include "../../geom/grid/RegularGrid.h"

#include <functional>

BEGIN_NAMESPACE_NUMERICAL

//! Functor for linear interpolation on a regular grid.
/*!
  \param N is the space dimension.
  \param F is the field type.  By default it is double.
  \param A determines whether the grid will allocate memory its own memory
  or use externally allocated memory.  By default \c A is true.
  \param T is the number type.  By default it is double.
*/
template<int N, typename F = double, bool A = true, typename T = double>
class LinInterpGrid :
  // The following is complicated, but the argument type is a Cartesian point 
  // and the return type is the field.
  public std::unary_function< 
  const typename geom::RegularGrid<N,T>::Point&, 
  typename Loki::TypeTraits<F>::ParameterType> {
  //
  // Private types.
  //

private:

  //! The base class.
  typedef std::unary_function< 
    const typename geom::RegularGrid<N,T>::Point&, 
    typename Loki::TypeTraits<F>::ParameterType>
  base_type;

  //
  // Public types.
  //

public:

  //! The argument type is a Cartesian point.
  typedef typename base_type::argument_type argument_type;
  //! The result type is the field.
  typedef typename base_type::result_type result_type;

  //! The number type.
  typedef T Number;
  //! The field type.
  typedef F Field;

  //! A regular grid.
  typedef geom::RegularGrid<N,Number> Grid;
  //! The field array.
  typedef ads::Array<N,Field,A> FieldArray;

  //! A Cartesian point.
  typedef typename Grid::Point Point;
  //! A bounding box.
  typedef typename Grid::BBox BBox;

  //! The (multi) index type.
  typedef typename FieldArray::index_type index_type;
  //! The size type.
  typedef typename FieldArray::size_type size_type;

  //! The unqalified field type.
  typedef typename FieldArray::unqualified_value_type
  unqualified_Field;

  //
  // Data.
  //

private:
  
  //! The field array.
  FieldArray _fields;
  //! The Cartesian grid.
  Grid _grid;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.
  LinInterpGrid() :
    _fields(),
    _grid()
  {}

  //! Construct from the field array and the Cartesian domain.
  /*!
    \param fields is the array of fields.
    \param domain is the Cartesian domain.
  */
  template <bool A2>
  LinInterpGrid(const ads::Array<N,Field,A2>& fields,
		 const BBox domain) :
    _fields(fields),
    _grid(_fields.extents(), domain)
  {}

  //! Build from the field array and the Cartesian domain.
  /*!
    \param fields is the array of fields.
    \param domain is the Cartesian domain.
  */
  template <bool A2>
  void
  build(const ads::Array<N,Field,A2>& fields, const BBox domain) {
    _fields = fields;
    _grid = Grid(_fields.extents(), domain);
  }

  //! Copy constructor.
  LinInterpGrid(const LinInterpGrid& x) :
    _fields(x._fields),
    _grid(x._grid)
  {}

  //! Assignment operator.
  LinInterpGrid&
  operator=(const LinInterpGrid& x) {
    if (this != &x) {
      _fields = x._fields;
      _grid = x._grid;
    }
    return *this;
  }

  //! Destructor.  Deletes memory only if it was allocated internally.
  ~LinInterpGrid()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Interpolation.
  //! @{

  //! Interpolate the field at the specified point.
  result_type
  operator()(argument_type x) const;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Static member functions.
  //! @{

  //! Return the dimension of the space.
  static
  int
  space_dimension() {
    return N;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the field array.
  const FieldArray&
  fields() const {
    return _fields;
  }

  //! Return the Cartesian domain.
  const BBox&
  domain() const {
    return _grid.domain();
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //! @{

  //! Return the field array.
  /*!
    \warning Don't resize the fields array using this accessors.  Use the 
    resize() member function instead.
  */
  FieldArray&
  fields() {
    return _fields;
  }

  //! Set the Cartesian domain.
  void
  set_domain(const BBox& domain);

  //! Resize the fields array.
  void
  resize(const index_type& extents);

  //! @}
};

END_NAMESPACE_NUMERICAL

#define __numerical_interpolation_LinInterpGrid_ipp__
#include "LinInterpGrid.ipp"
#undef __numerical_interpolation_LinInterpGrid_ipp__

#endif
