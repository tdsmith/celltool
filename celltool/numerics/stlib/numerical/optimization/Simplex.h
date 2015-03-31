// -*- C++ -*-

/*! 
  \file numerical/optimization/Simplex.h
  \brief The downhill simplex method.
*/

// CONTINUE: Write tests for the diameter functionality.

#if !defined(__numerical_Simplex_h__)
#define __numerical_Simplex_h__

#include "Opt.h"

#include "../../geom/mesh/simplex/Simplex.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_Simplex)
#define DEBUG_Simplex
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The downhill simplex method.
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template<int N, class _Function, typename T = typename _Function::result_type,
	 typename Point = typename _Function::argument_type>
class Simplex :
  public Opt<N,_Function,T,Point> {
private:

  // 
  // Private types
  //

  typedef Opt<N,_Function,T,Point> Base;

public:

  //
  // Public types.
  //
  
  //! The function type.
  typedef typename Base::function_type function_type;

  //! The number type.
  typedef typename Base::number_type number_type;

  //! A point in N dimensions.
  typedef typename Base::point_type point_type;

private:

  //
  // Private types.
  //

  //! A container of points.
  typedef ads::FixedArray<N+1,point_type> point_container;

  //! A const iterator on points.
  typedef typename point_container::const_iterator point_const_iterator;

  //
  // Member data that the user can set.
  //

  // The fractional error tolerance.
  number_type _tolerance;

  // The initial offset size used to generate the simplex.
  number_type _offset;

  //
  // Other member data.
  //

  // The vertices of the simplex.
  ads::FixedArray<N+1,point_type> _vertices;

  // The objective function values at the vertices.
  ads::FixedArray<N+1,number_type> _values;

  // The sums of the coordinates of the vertices in the simplex.
  point_type _coordinate_sums;

  // The diameter of a hypercube that has the same volume as the simplex.
  number_type _diameter;

  //
  // Not implemented.
  //
  
  // Default constructor not implemented.
  Simplex();

  // Copy constructor not implemented.
  Simplex(const Simplex&);

  // Assignment operator not implemented.
  Simplex&
  operator=(const Simplex&);

public:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The default constructor, the copy constructor and the assignment 
    operator are not implemented.
  */
  // @{

  //! Construct from the objective function.
  Simplex(const function_type& function, 
	  const number_type tolerance = 
	  std::sqrt(std::numeric_limits<number_type>::epsilon()),
	  const number_type offset = 
	  std::pow(std::numeric_limits<number_type>::epsilon(), 0.25),
	  const int max_function_calls = 10000);

  //! Destructor.
  virtual
  ~Simplex()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //
  // Inherited from Opt.
  //

#if 0
  //! Return the dimension, N.
  static
  int
  dimension() {
    return Base::dimension();
  }
#endif

  //! Return the dimension, N.
  using Base::dimension;

  //! Return a constant reference to the objective function.
  using Base::function;

  //! Return the maximum allowed number of function calls.
  using Base::max_function_calls;

  //! Return the number of function calls required to find the minimum.
  using Base::num_function_calls;

  // @}
  //--------------------------------------------------------------------------
  //! \name Minimization.
  // @{

  //! Find the minimum to within the tolerance.
  bool 
  find_minimum(const point_type& starting_point);

  //! Find the minimum to within the tolerance.
  bool 
  find_minimum(const ads::FixedArray<N + 1,point_type>& vertices);

  //! Return the minimum point.
  point_type
  minimum_point() const {
    return _vertices[0];
  }

  //! Return the function value at the minimum point.
  number_type
  minimum_value() const {
    return _values[0];
  }

  //! Return the diameter of a hypercube that has the same volume as the simplex.
  number_type
  diameter() const {
    return _diameter;
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //
  // Inherited from Opt
  //

  //! Set the maximum number of function call allowed per optimization.
  using Base::set_max_function_calls;

  //! Reset the number of function calls to zero.
  using Base::reset_num_function_calls;

  //! Set whether we are checking the number of function calls.
  using Base::set_are_checking_function_calls;

  //
  // New.
  //
  
  //! Set the tolerance.
  void
  set_tolerance(const number_type tolerance) {
    _tolerance = tolerance;
  }

  //! Set the offset used in generating the initial simplex.
  void
  set_offset(const number_type offset) {
    _offset = offset;
  }

  // @}

private:

  // Initialize the simplex.
  void 
  initialize(const point_type& starting_point, const number_type offset);

  // Initialize the simplex.
  void 
  initialize(const ads::FixedArray<N + 1,point_type>& vertices);

  // Initialize given the vertices of the simplex.
  void 
  initialize_given_vertices();

  // Sum the coordinates of the vertices.
  void
  sum_coordinates();

  bool
  find_minimum();

  // Move the high point.
  /*
    Extrapolate by the given factor through the face of the simplex across
    from the high point.  Replace the high point if the new point is better.
    \param ihi is the index of the high point.
    \param factor is the scaling factor on where to move the point.
    For example, -1 reflects the point and 0.5 contracts the point.
  */
  number_type
  move_high_point(const int ihi, const number_type factor);

  // Compute the diameter
  void
  compute_diameter();

protected:

  //--------------------------------------------------------------------------
  /*! \name Calling the objective function.
    Functionality inherited from Opt.
  */
  // @{

  //! Evaluate the objective function and return the result.  
  /*!
    Increment the count of the number of function calls.
  */
  using Base::evaluate_function;

  // @}

};

END_NAMESPACE_NUMERICAL

#define __Simplex_ipp__
#include "Simplex.ipp"
#undef __Simplex_ipp__

#endif
