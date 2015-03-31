// -*- C++ -*-

/*! 
  \file QuasiNewton.h
  \brief The quasi-Newton BFGS method.
*/

#if !defined(__numerical_QuasiNewton_h__)
#define __numerical_QuasiNewton_h__

#include "Opt.h"

#include "../../ads/tensor/SquareMatrix.h"

#include "../../geom/kernel/Point.h"

#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_QuasiNewton)
#define DEBUG_QuasiNewton
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The quasi-Newton BFGS method.
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template <int N, 
	  class _Function,
	  typename T = typename _Function::result_type,
	  typename Point = typename _Function::argument_type>
class QuasiNewton :
  public Opt<N,_Function,T,Point>
{
private:

  // 
  // Private types
  //

  typedef Opt<N,_Function,T,Point> base_type;

  typedef ads::SquareMatrix<N,T> matrix_type;

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
  // Member data.
  //

  number_type _x_tolerance;

  number_type _gradient_tolerance;

  //
  // Not implemented.
  //
  
  // Default constructor not implemented.
  QuasiNewton();

  // Copy constructor not implemented.
  QuasiNewton( const QuasiNewton& );

  // Assignment operator not implemented.
  QuasiNewton&
  operator=( const QuasiNewton& );

public:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The default constructor, the copy constructor and the assignment 
    operator are not implemented.
  */
  // @{

  //! Construct from the objective function.
  QuasiNewton( const function_type& function,
	       const number_type x_tolerance 
	       = 4 * std::numeric_limits<number_type>::epsilon(),
	       const number_type gradient_tolerance 
	       = 4 * std::numeric_limits<number_type>::epsilon(),
	       const int max_function_calls = 10000 ) :
    base_type( function, max_function_calls ),
    _x_tolerance( x_tolerance ),
    _gradient_tolerance( gradient_tolerance )
  {}

  //! Destructor.
  virtual
  ~QuasiNewton()
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
    \param num_iterations is the number of iterations of the method.
    \param max_step is the maximum step used in the line search.
    \param x_tolerance is the x tolerance for convergence.  The function
    returns if the line search takes a smaller step than this.
    \param gradient_tolerance is the gradient tolerance for convergence.  
    The function returns if the magnitude of the gradient falls below this 
    value.
  */
  void
  find_minimum( point_type& x, number_type& value, int& num_iterations,
		number_type max_step = 0,
		const number_type x_tolerance = 0,
		const number_type gradient_tolerance = 0 );
  
  //! Find the minimum.
  /*!
    \param x  The input value is the starting point.  The output value
    is the minimum point found.
  */
  void
  find_minimum( point_type& x )
  {
    number_type value;
    int num_iterations;
    find_minimum( x, value, num_iterations );
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
  dimension()
  {
    return base_type::dimension();
  }

  //! Return a constant reference to the objective function.
  const function_type&
  function() const
  {
    return base_type::function();
  }

  //! Return the number of function calls required to find the minimum.
  int
  num_function_calls() const
  {
    return base_type::num_function_calls();
  }

  //
  // New.
  //

  //! Return the x tolerance for convergence.
  number_type
  x_tolerance() const
  {
    return _x_tolerance;
  }

  //! Return the gradient tolerance for convergence.
  number_type
  gradient_tolerance() const
  {
    return _gradient_tolerance;
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
  set_max_function_calls( const int max_function_calls )
  {
    base_type::set_max_function_calls( max_function_calls );
  }

  //! Reset the number of function calls to zero.
  void
  reset_num_function_calls()
  {
    base_type::reset_num_function_calls();
  }

  //
  // New.
  //
  
  //! Set the x tolerance for convergence.
  void
  set_x_tolerance( const number_type x_tolerance )
  {
    _x_tolerance = x_tolerance;
  }

  //! Set the gradient tolerance for convergence.
  void
  set_gradient_tolerance( const number_type gradient_tolerance )
  {
    _gradient_tolerance = gradient_tolerance;
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
  evaluate_function( const point_type& x )
  {
    return base_type::evaluate_function( x );
  }

  //! Evaluate the gradient.
  /*!
    Increment the count of the number of function calls.
  */
  void
  evaluate_gradient( const point_type& x, point_type& gradient )
  {
    base_type::evaluate_gradient( x, gradient );
  }

  //! Numerically evaluate the gradient.
  /*!
    Increment the count of the number of function calls.
  */
  void
  evaluate_numeric_gradient( const point_type& x, 
			     point_type& gradient,
			     const number_type delta = 0.0 )
  {
    base_type::evaluate_numeric_gradient( x, gradient, delta );
  }

  // @}

private:

  void
  line_search( const point_type& x_old, const number_type f_old,
	       point_type& gradient, point_type& direction, point_type& x,
	       number_type& f, const number_type max_step );

};

END_NAMESPACE_NUMERICAL

#define __QuasiNewton_ipp__
#include "QuasiNewton.ipp"
#undef __QuasiNewton_ipp__

#endif
