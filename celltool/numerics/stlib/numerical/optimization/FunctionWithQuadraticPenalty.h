// -*- C++ -*-

/*! 
  \file FunctionWithQuadraticPenalty.h
  \brief An objective function with a quadratic penalty.
*/

#if !defined(__numerical_FunctionWithQuadraticPenalty_h__)
#define __numerical_FunctionWithQuadraticPenalty_h__

#include "../defs.h"

#include <functional>

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_FunctionWithQuadraticPenalty)
#define DEBUG_FunctionWithQuadraticPenalty
#endif

BEGIN_NAMESPACE_NUMERICAL

//! An objective function with a quadratic penalty.
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param Constraint is the equality constraint functor.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template <int N, class _Function, class Constraint, 
	  typename T = typename _Function::result_type,
	  typename Point = typename _Function::argument_type>
class FunctionWithQuadraticPenalty :
  public std::unary_function<Point,T> {
private:

  typedef std::unary_function<Point,T> base_type;

public:

  //
  // Public types.
  //
  
  //! The argument type.
  typedef typename base_type::argument_type argument_type;

  //! The result type.
  typedef typename base_type::result_type result_type;

private:

  //
  // Private types.
  //
  
  // The function type.
  typedef _Function function_type;

  // The constraint type.
  typedef Constraint constraint_type;

  // The number type.
  typedef result_type number_type;

  // A point in N dimensions.
  typedef argument_type point_type;

private:

  //
  // Member data.
  //

  // The objective function.
  const function_type& _function;

  // The constraint function.
  const constraint_type& _constraint;

  // The penalty parameter.
  mutable number_type _penalty_parameter;

  // The penalty parameter reduction factor.
  mutable number_type _reduction_factor;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  FunctionWithQuadraticPenalty();

  // Assignment operator not implemented.
  FunctionWithQuadraticPenalty&
  operator=(const FunctionWithQuadraticPenalty&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Construct from the objective function, the constraint and the penalty parameter.
  FunctionWithQuadraticPenalty(const function_type& function, 
				const constraint_type& constraint, 
				const number_type penalty_parameter = 1,
				const number_type reduction_factor = 0.1);

  //! Copy constructor.
  FunctionWithQuadraticPenalty(const FunctionWithQuadraticPenalty& x);

  //! Destructor.
  virtual
  ~FunctionWithQuadraticPenalty()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Functor.
  // @{

  //! Return the value of the objective function with a quadratic penalty.
  result_type
  operator()(const argument_type& x) const;

  //! Calculate the gradient of the objective function with a quadratic penalty.
  void
  gradient(const argument_type& x, argument_type& gradient) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return a const reference to the objective function.
  const function_type&
  function() const {
    return _function;
  }

  //! Return a const reference to the constraint function.
  const constraint_type&
  constraint() const {
    return _constraint;
  }

  //! Return the value of the penalty parameter.
  number_type 
  penalty_parameter() const {
    return _penalty_parameter;
  }

  //! Return the value of the penalty parameter reduction factor.
  number_type 
  reduction_factor() const {
    return _reduction_factor;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Increase the penalty be decreasing the penalty parameter by the reduction factor.
  void
  increase_penalty() const {
    _penalty_parameter *= _reduction_factor;
  }

  //! Set the penalty parameter.
  void
  set_penalty_parameter(const number_type penalty_parameter) const {
    _penalty_parameter = penalty_parameter;
  }

  //! Set the reduction factor.
  void
  set_reduction_factor(const number_type reduction_factor) const {
    _reduction_factor = reduction_factor;
  }

  // @}
};

END_NAMESPACE_NUMERICAL

#define __FunctionWithQuadraticPenalty_ipp__
#include "FunctionWithQuadraticPenalty.ipp"
#undef __FunctionWithQuadraticPenalty_ipp__

#endif
