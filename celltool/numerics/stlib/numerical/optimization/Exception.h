// -*- C++ -*-

/*! 
  \file Exception.h
  \brief Base class for optimization methods.
*/

#if !defined(__numerical_Exception_h__)
#define __numerical_Exception_h__

#include "../defs.h"

#include "../../ads/array/FixedArray.h"

#include <iostream>
#include <string>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_Exception)
#define DEBUG_Exception
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Optimization exception.
template<int N, 
	 typename T = double,
	 typename Point = ads::FixedArray<N> >
class OptimizationException {
public:

  //
  // Types.
  //

  //! Number type.
  typedef T number_type;
  //! Point type.
  typedef Point point_type;

private:

  //
  // Member data.
  //

  std::string _message;
  point_type _location;
  number_type _value;
  int _num_function_calls;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  OptimizationException();

  // Assignment operator not implemented.
  OptimizationException&
  operator=(const OptimizationException&);

public:

  //
  // Constructor and destructor.
  //

  //! Constructor.
  OptimizationException(const char* message, const point_type& location,
			 const number_type value, 
			 const int num_function_calls) :
    _message(message),
    _location(location),
    _value(value),
    _num_function_calls(num_function_calls)
  {}

  //! Destructor.
  virtual
  ~OptimizationException()
  {}

  //! Print an error message.
  virtual
  void
  print() {
    std::cerr << message() << '\n'
	      << "location = " << location() << '\n'
	      << "function value = " << value() << '\n'
	      << "number of function calls = " << num_function_calls() 
	      << '\n';
  }

  //
  // Accessors.
  //

  //! Return the message.
  const std::string&
  message() const {
    return _message;
  }

  //! Return the location.
  const point_type&
  location() const {
    return _location;
  }

  //! Return the function value.
  number_type
  value() const {
    return _value;
  }

  //! Return the number of function calls.
  int
  num_function_calls() const {
    return _num_function_calls;
  }

};



//! Penalty method exception.
template<int N, 
	 typename T = double,
	 typename Point = ads::FixedArray<N> >
class PenaltyException {
public:

  //
  // Types.
  //

  //! The number type.
  typedef T number_type;
  //! A Cartesian point.
  typedef Point point_type;

private:

  //
  // Member data.
  //

  std::string _message;
  point_type _location;
  number_type _function;
  number_type _constraint;
  number_type _step_size;
  number_type _max_constraint_error;
  number_type _penalty_parameter;

  //
  // Not implemented.
  //

  // Default constructor not implemented.
  PenaltyException();

  // Assignment operator not implemented.
  PenaltyException&
  operator=(const PenaltyException&);

public:

  //
  // Constructor and destructor.
  //

  //! Constructor.
  PenaltyException(const char* message, 
			 const point_type& location,
			 const number_type function,
			 const number_type constraint,
			 const number_type step_size,
			 const number_type max_constraint_error,
			 const number_type penalty_parameter) :
    _message(message),
    _location(location),
    _function(function),
    _constraint(constraint),
    _step_size(step_size),
    _max_constraint_error(max_constraint_error),
    _penalty_parameter(penalty_parameter)
  {}

  //! Destructor.
  virtual
  ~PenaltyException()
  {}

  //! Print an error message.
  virtual
  void
  print() {
    std::cerr << _message << '\n'
	      << "location = " << _location << '\n'
	      << "function value = " << _function << '\n'
	      << "constraint value = " << _constraint << '\n'
	      << "step size = " << _step_size << '\n'
	      << "max constraint error = " << _max_constraint_error 
	      << '\n'
	      << "penalty parameter = " << _penalty_parameter << '\n'
	      << '\n';
  }

};

END_NAMESPACE_NUMERICAL

#endif
