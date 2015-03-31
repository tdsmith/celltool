// -*- C++ -*-

/*! 
  \file CoordinateDescent.h
  \brief The coordinate descent method of Hooke and Jeeves.
*/

#if !defined(__numerical_CoordinateDescent_h__)
#define __numerical_CoordinateDescent_h__

#include "Opt.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_CoordinateDescent)
#define DEBUG_CoordinateDescent
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The coordinate descent method of Hooke and Jeeves.
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template<int N, class _Function, typename T = typename _Function::result_type,
	 typename Point = typename _Function::argument_type>
class CoordinateDescent :
  public Opt<N,_Function,T,Point> {
private:

  // 
  // Private types
  //

  typedef Opt<N,_Function,T,Point> base_type;

public:

  //
  // Public types.
  //
  
  //! The function type.
  typedef typename base_type::function_type function_type;

  //! The number type.
  typedef typename base_type::number_type number_type;

  //! A point in N dimensions.
  typedef typename base_type::point_type point_type;

private:

  //
  // Member data that the user can set.
  //

  // The initial step size.
  number_type _initial_step_size;

  // The stepsize at which to halt optimization.
  number_type _final_step_size;

  // The stepsize reduction factor.
  number_type _step_size_reduction_factor;

  //
  // Other member data.
  //

  // The number of steps taken before the step size is increased.
  int _step_limit;

  // The step size.
  number_type _step_size;

  //
  // Not implemented.
  //
  
  // Default constructor not implemented.
  CoordinateDescent();

  // Copy constructor not implemented.
  CoordinateDescent(const CoordinateDescent&);

  // Assignment operator not implemented.
  CoordinateDescent&
  operator=(const CoordinateDescent&);

public:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The default constructor, the copy constructor and the assignment 
    operator are not implemented.
  */
  // @{

  //! Construct from the objective function.
  CoordinateDescent(const function_type& function, 
		    const number_type initial_step_size = 
		    std::pow(std::numeric_limits<number_type>::epsilon(), 
			     0.25),
		    const number_type final_step_size = 
		    std::sqrt(std::numeric_limits<number_type>::epsilon()),
		    const int max_function_calls = 10000);

  //! Destructor.
  virtual
  ~CoordinateDescent()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Minimization.
  // @{

  //! Find the minimum.
  /*!
    \param x  The input value is the starting point.  The output value
    is the minimum point found.
    \param value is the value of the objective function at the minimum point.
    \param num_steps is the number of time that a descent direction is 
    successfully found.
  */
  bool 
  find_minimum(point_type& x, number_type& value, int& num_steps);

  //! Find the minimum.
  /*!
    \param x  The input value is the starting point.  The output value
    is the minimum point found.
    \param value is the value of the objective function at the minimum point.
  */
  bool 
  find_minimum(point_type& x, number_type& value) {
    int num_steps;
    return find_minimum(x, value, num_steps);
  }

  //! Find the minimum.
  /*!
    \param x  The input value is the starting point.  The output value
    is the minimum point found.
    \param num_steps is the number of time that a descent direction is 
    successfully found.
  */
  bool 
  find_minimum(point_type& x, int& num_steps) {
    number_type value;
    return find_minimum(x, value, num_steps);
  }

  //! Find the minimum.
  /*!
    \param x  The input value is the starting point.  The output value
    is the minimum point found.
  */
  bool 
  find_minimum(point_type& x) {
    int num_steps;
    number_type value;
    return find_minimum(x, value, num_steps);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //
  // Inherited from Opt.
  //

  //! Return the dimension, N.
  static
  int
  dimension() {
    return base_type::dimension();
  }

  //! Return a constant reference to the objective function.
  const function_type&
  function() const {
    return base_type::function();
  }

  //! Return the number of function calls required to find the minimum.
  int
  num_function_calls() const {
    return base_type::num_function_calls();
  }

  //
  // New.
  //

  // Return the stepsize reduction factor.
  number_type 
  step_size_reduction_factor() const {
    return _step_size_reduction_factor;
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //
  // Inherited from Opt
  //

  //! Set the maximum number of function call allowed per optimization.
  void
  set_max_function_calls(const int max_function_calls) {
    base_type::set_max_function_calls(max_function_calls);
  }

  //! Reset the number of function calls to zero.
  void
  reset_num_function_calls() {
    base_type::reset_num_function_calls();
  }

  //
  // New.
  //
  
  //! Set the initial step size.
  void
  set_initial_step_size(const number_type initial_step_size) {
    _initial_step_size = initial_step_size;
  }

  //! Set the stepsize at which to halt optimization.
  void
  set_final_step_size(const number_type final_step_size) {
    _final_step_size = final_step_size;
  }

  //! Set the stepsize reduction factor.
  void
  set_step_size_reduction_factor(const number_type 
				 step_size_reduction_factor) {
    _step_size_reduction_factor = step_size_reduction_factor;
  }

  // @}

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
  number_type
  evaluate_function(const point_type& x) {
    return base_type::evaluate_function(x);
  }

  // @}

private:

  // Find a descent direction by moving in the coordinate directions.
  /*
    \param x is the starting point.  It will be set to the new position 
    found by the search.
    \param value is initially equal to _function(x).  It will be set to 
    the value of the objective function at the new position.
    \param delta will be set to the difference between the new and old 
    position.

    \return Return true if the search takes one or more successful steps.  
    Otherwise return false.
  */
  bool
  descent_direction(point_type& x, number_type& value, point_type& delta);

  // Search in the specified coordinate and direction.
  /*
    \param x is the starting point.  It will be set to the new position 
    found by the search.
    \param value is initially equal to _function(x).  It will be set to 
    the value of the objective function at the new position.
    \param delta will be set to the difference between the new and old 
    position.
    \param i is the coordinate in which to search.
    \param sign is the direction to search (1 or -1).

    \return Return true if the search takes one or more successful steps.  
    Otherwise return false.
  */
  bool
  coordinate_search(point_type& x, number_type& value, point_type& delta, 
		    const int i, const int sign);
};

END_NAMESPACE_NUMERICAL

#define __CoordinateDescent_ipp__
#include "CoordinateDescent.ipp"
#undef __CoordinateDescent_ipp__

#endif
