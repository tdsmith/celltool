// -*- C++ -*-

/*! 
  \file Penalty.h
  \brief The penalty method for optimization with an equality constraint.
*/

#if !defined(__numerical_Penalty_h__)
#define __numerical_Penalty_h__

#include "CoordinateDescent.h"
#include "FunctionWithQuadraticPenalty.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_Penalty)
#define DEBUG_Penalty
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The penalty method for optimization with an equality constraint.
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param Constraint is the equality constraint.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template <int N, class _Function, class Constraint, 
	  typename T = typename _Function::result_type,
	  typename Point = typename _Function::argument_type>
class Penalty
{
public:

  //
  // Public types.
  //
  
  //! The function type.
  typedef _Function function_type;

  //! The constraint type.
  typedef Constraint constraint_type;

  //! The number type.
  typedef T number_type;

  //! A point in N dimensions.
  typedef Point point_type;

private:

  // The objective function with a quadratic penalty.
  typedef FunctionWithQuadraticPenalty<N, function_type, constraint_type,
				       number_type, point_type>
  function_with_penalty_type;

  // The unconstrained optimization method.
  typedef CoordinateDescent<N, function_with_penalty_type, number_type, 
			    point_type> 
  optimization_type;

private:

  //
  // Member data.
  //

  // The objective function with a quadratic penalty.
  function_with_penalty_type _function_with_penalty;

  // The unconstrained optimization method.
  optimization_type _optimization;

  // The initial step size;
  number_type _initial_step_size;

  // The final step size;
  number_type _final_step_size;

  // The maximum allowed error in the constraint.
  number_type _max_constraint_error;

  // The initial value of the penalty parameter.
  number_type _initial_penalty_parameter;

  //
  // Not implemented.
  //
  
  // Default constructor not implemented.
  Penalty();

  // Copy constructor not implemented.
  Penalty( const Penalty& );

  // Assignment operator not implemented.
  Penalty&
  operator=( const Penalty& );

public:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The copy constructor and the assignment operator are not implemented.
  */
  // @{

  //! Construct from the objective function, the constraint and many optional parameters.
  Penalty( const function_type& function,
	   const constraint_type& constraint,
	   const number_type initial_step_size = 
	   std::pow( std::numeric_limits<number_type>::epsilon(), 0.25 ),
	   const number_type final_step_size = 
	   std::sqrt( std::numeric_limits<number_type>::epsilon() ),
	   const number_type max_constraint_error = 
	   std::pow( std::numeric_limits<number_type>::epsilon(), 0.25 ),
	   const int max_function_calls = 10000 );

  //! Destructor.
  virtual
  ~Penalty()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Minimization.
  // @{

  //! Find the minimum to within the tolerances.
  void
  find_minimum( point_type& x );

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the penalty parameter.
  number_type
  penalty_parameter() const
  {
    return _function_with_penalty.penalty_parameter();
  }

  //! Return the maximum allowed constraint error.
  number_type
  max_constraint_error() const
  {
    return _max_constraint_error;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Set the initial step size.
  void
  set_initial_step_size( const number_type initial_step_size )
  {
    _initial_step_size = initial_step_size;
  }

  //! Set the stepsize at which to halt optimization.
  void
  set_final_step_size( const number_type final_step_size )
  {
    _final_step_size = final_step_size;
  }

  //! Set the stepsize reduction factor.
  void
  set_step_size_reduction_factor( const number_type 
				  step_size_reduction_factor )
  {
    _optimization.set_step_size_reduction_factor( step_size_reduction_factor );
  }

  //! Set the maximum allowed constraint error.
  void
  set_max_constraint_error( const number_type max_constraint_error )
  {
    _max_constraint_error = max_constraint_error;
  }

  //! Set the penalty parameter.
  void
  set_penalty_parameter( const number_type penalty_parameter )
  {
    _optimization.function().set_penalty_parameter( penalty_parameter );
  }

  // @}

};

END_NAMESPACE_NUMERICAL

#define __Penalty_ipp__
#include "Penalty.ipp"
#undef __Penalty_ipp__

#endif
