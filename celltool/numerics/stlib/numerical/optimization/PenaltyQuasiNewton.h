// -*- C++ -*-

/*! 
  \file PenaltyQuasiNewton.h
  \brief The penalty method for optimization with an equality constraint using a quasi-Newton method .
*/

#if !defined(__numerical_PenaltyQuasiNewton_h__)
#define __numerical_PenaltyQuasiNewton_h__

#include "QuasiNewton.h"
#include "FunctionWithQuadraticPenalty.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PenaltyQuasiNewton)
#define DEBUG_PenaltyQuasiNewton
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The penalty method for optimization with an equality constraint using a quasi-Newton method .
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param Constraint is the equality constraint.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template <int N, 
	  class _Function, 
	  class Constraint, 
	  typename T = typename _Function::result_type,
	  typename Point = typename _Function::argument_type>
class PenaltyQuasiNewton
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
  typedef QuasiNewton<N, function_with_penalty_type, number_type, point_type> 
  optimization_type;

private:

  //
  // Member data.
  //

  // The objective function with a quadratic penalty.
  function_with_penalty_type _function_with_penalty;

  // The unconstrained optimization method.
  optimization_type _optimization;

  // The maximum allowed error in the constraint.
  number_type _max_constraint_error;

  // The initial value of the penalty parameter.
  number_type _initial_penalty_parameter;

  //
  // Not implemented.
  //
  
  // Default constructor not implemented.
  PenaltyQuasiNewton();

  // Copy constructor not implemented.
  PenaltyQuasiNewton( const PenaltyQuasiNewton& );

  // Assignment operator not implemented.
  PenaltyQuasiNewton&
  operator=( const PenaltyQuasiNewton& );

public:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The copy constructor and the assignment operator are not implemented.
  */
  // @{

  //! Construct from the objective function, the constraint and optional parameters.
  PenaltyQuasiNewton( const function_type& function,
		      const constraint_type& constraint,
		      const number_type max_constraint_error = 
		      std::pow( std::numeric_limits<number_type>::epsilon(), 
				0.25 ),
		      const number_type x_tolerance 
		      = 4 * std::numeric_limits<number_type>::epsilon(),
		      const number_type gradient_tolerance 
		      = 4 * std::numeric_limits<number_type>::epsilon(),
		      const int max_function_calls = 10000 );

  //! Destructor.
  virtual
  ~PenaltyQuasiNewton()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Minimization.
  // @{

  //! Find the minimum to within the tolerances.
  void
  find_minimum( point_type& x, number_type& value, int& num_iterations,
		number_type max_step = 0,
		const number_type x_tolerance = 0,
		const number_type gradient_tolerance = 0 );

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

  //! Return the initial penalty parameter.
  number_type
  initial_penalty_parameter() const
  {
    return _initial_penalty_parameter;
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

  //! Set the maximum allowed constraint error.
  void
  set_max_constraint_error( const number_type max_constraint_error )
  {
    _max_constraint_error = max_constraint_error;
  }

  //! Set the penalty parameter.
  void
  set_initial_penalty_parameter( const number_type initial_penalty_parameter )
  {
    _initial_penalty_parameter = initial_penalty_parameter;
  }

  // @}

};

END_NAMESPACE_NUMERICAL

#define __PenaltyQuasiNewton_ipp__
#include "PenaltyQuasiNewton.ipp"
#undef __PenaltyQuasiNewton_ipp__

#endif
