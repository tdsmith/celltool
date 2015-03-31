// -*- C++ -*-

/*! 
  \file Opt.h
  \brief Base class for optimization methods.
*/

#if !defined(__numerical_Opt_h__)
#define __numerical_Opt_h__

#include "Exception.h"

#include "../derivative/centered_difference.h"

#include "../../ads/array/FixedArray.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_Opt)
#define DEBUG_Opt
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Base class for optimization methods.
/*!
  \param N is the problem dimension.
  \param _Function is the functor to minimize.
  \param T is the number type.  By default it is _Function::result_type;
  \param Point is the point type.  By default it is _Function::argument_type;
*/
template<int N, class _Function, typename T = typename _Function::result_type,
	 typename Point = typename _Function::argument_type>
class Opt {
protected:

  //
  // Protected types.
  //
  
  //! The function type.
  typedef _Function function_type;

  //! The number type.
  typedef T number_type;

  //! A point in N dimensions.
  typedef Point point_type;

  //
  // Member data.
  //

protected:

  //! The objective function.
  const function_type& _function;

  //! The maximum allowed number of function calls.
  int _max_function_calls;

  //! The number of function calls required to find the minimum.
  int _num_function_calls;

  //! If we are checking the number of function calls.
  bool _are_checking_function_calls;

  //
  // Not implemented.
  //
  
private:

  // Default constructor not implemented.
  Opt();

  // Copy constructor not implemented.
  Opt(const Opt&);

  // Assignment operator not implemented.
  Opt&
  operator=(const Opt&);

protected:

  //--------------------------------------------------------------------------
  /*! \name Constructors etc.
    The default constructor, the copy constructor and the assignment 
    operator are not implemented.  The implemented constructor and 
    destructor are protected.
  */
  // @{

  //! Construct from the objective function.
  Opt(const function_type& function, 
      const int max_function_calls = 10000,
      const bool are_checking_function_calls = true) :
    _function(function),
    _max_function_calls(max_function_calls),
    // Do this so they remember to call reset_num_function_calls().
    _num_function_calls(std::numeric_limits<int>::max()),
    _are_checking_function_calls(are_checking_function_calls)
  {}

  //! Destructor.
  virtual
  ~Opt()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Calling the objective function.
  // @{

  //! Evaluate the objective function and return the result.  
  /*!
    Increment the count of the number of function calls.
  */
  number_type
  evaluate_function(const point_type& x) {
    increment_function_calls(x);
    return _function(x);
  }

  //! Evaluate the gradient.
  /*!
    Increment the count of the number of function calls.
  */
  void
  evaluate_gradient(const point_type& x, point_type& gradient) {
    increment_function_calls(x);
    _function.gradient(x, gradient);
  }

  //! Numerically evaluate the gradient.
  /*!
    Increment the count of the number of function calls.
  */
  void
  evaluate_numeric_gradient(const point_type& x, 
			    point_type& gradient,
			    const number_type delta = 0.0) {
    increment_function_calls(x, 2 * N);
    if (delta != 0.0) {
      numerical::gradient_centered_difference<N,function_type,number_type>
	(_function, x, gradient, delta);
    }
    else {
      numerical::gradient_centered_difference<N,function_type,number_type>
	(_function, x, gradient);
    }
  }

  // @}

public:

  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the dimension, N.
  static
  int
  dimension() {
    return N;
  }

  //! Return a constant reference to the objective function.
  const function_type&
  function() const {
    return _function;
  }

  //! Return the maximum allowed number of function calls.
  int 
  max_function_calls() {
    return _max_function_calls;
  }

  //! Return the number of function calls required to find the minimum.
  int
  num_function_calls() const {
    return _num_function_calls;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Set the maximum number of function call allowed per optimization.
  void
  set_max_function_calls(const int max_function_calls) {
    _max_function_calls = max_function_calls;
  }

  //! Reset the number of function calls to zero.
  void
  reset_num_function_calls() {
    _num_function_calls = 0;
  }

  //! Set whether we are checking the number of function calls.
  void
  set_are_checking_function_calls(const bool are_checking) {
    _are_checking_function_calls = are_checking;
  }

  // @}
  
private:

  // CONTINUE: Remove the x parameter.
  void
  increment_function_calls(const point_type& x, const int increment = 1) {
    _num_function_calls += increment;
    if (_are_checking_function_calls &&
	_num_function_calls > _max_function_calls) {
      OptimizationException<N,number_type,point_type>
	ex("In Opt: max function calls exceeded.",
	    x, _function(x), _num_function_calls);
      ex.print();
      throw ex;
    }
  }

};

END_NAMESPACE_NUMERICAL

#endif
