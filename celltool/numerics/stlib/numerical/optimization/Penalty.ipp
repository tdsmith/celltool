// -*- C++ -*-

#if !defined(__Penalty_ipp__)
#error This file is an implementation detail of the class Penalty.
#endif

BEGIN_NAMESPACE_NUMERICAL

//
// Constructors
//

template <int N, class F, class C, typename T, typename P>
inline
Penalty<N,F,C,T,P>::
Penalty( const function_type& function,
	 const constraint_type& constraint,
	 const number_type initial_step_size,
	 const number_type final_step_size,
	 const number_type max_constraint_error,
	 const int max_function_calls ) :
  _function_with_penalty( function, constraint ),
  _optimization( _function_with_penalty,
		 initial_step_size, final_step_size, 
		 max_function_calls ),
  _initial_step_size( initial_step_size ),
  _final_step_size( final_step_size ),
  _max_constraint_error( max_constraint_error ),
  _initial_penalty_parameter( 1 )
{
  assert( _initial_step_size >= _final_step_size );
  assert( _max_constraint_error > 0 );
}

#if 0
// This is very slow.  It does the full precision optimization for each
// penalty parameter.
template <int N, class F, class C, typename T, typename P>
inline
bool 
Penalty<N,F,C,T,P>::
find_minimum( point_type& x )
{
  number_type constraint_error = 0;
  int num_steps;
  _function_with_penalty.set_penalty_parameter( _initial_penalty_parameter );

  do {
    // Do an optimization.
    try {
      _optimization.find_minimum( x, num_steps );
    }
    catch ( OptimizationException<N>& ex ) {
      std::cerr << "In Penalty::find_minimum():\n"
		<< "constraint = " 
		<< _optimization.function().constraint()( x ) << '\n'
		<< "_max_constraint_error = " << _max_constraint_error 
		<< '\n'
		<< "num_steps = " << num_steps << '\n'
		<< "penalty parameter = " << penalty_parameter() << '\n';
      throw ex;
    }
    constraint_error = std::abs( _optimization.function().constraint()( x ) );
    if ( constraint_error > _max_constraint_error ) {
      if ( num_steps == 0 ) {
	std::cout << "Can not satisfy constraint to given tolerance.\n";
	return false;
      }
      // Increase the constraint penalty.
      _function_with_penalty.increase_penalty();
    }
  } while ( constraint_error > _max_constraint_error );
  return true;
}
#endif

#if 0
template <int N, class F, class C, typename T, typename P>
inline
bool 
Penalty<N,F,C,T,P>::
find_minimum( point_type& x )
{
  for ( number_type step_size = _initial_step_size;
	step_size >= _final_step_size;
	step_size *= _optimization.step_size_reduction_factor() ) {
    // Set the step size.
    _optimization.set_initial_step_size( step_size );
    _optimization.set_final_step_size( step_size );

    do {
      if ( ! _optimization.find_minimum( x, num_steps ) ) {
	// If the maximum number of function calls was exceeded, return false.
	return false;
      }
      if ( num_steps != 0 ) {
	// Increase the constraint penalty.
	_optimization.function().increase_penalty();
      }
    } while ( num_steps != 0 );
  }
  /*
  number_type constraint_error = 0;
  int num_steps;
  _function_with_penalty.set_penalty_parameter( _initial_penalty_parameter );

  do {
    // Do an optimization.
    try {
      _optimization.find_minimum( x, num_steps );
    }
    catch ( OptimizationException<N>& ex ) {
      std::cerr << "In Penalty::find_minimum():\n"
		<< "constraint = " 
		<< _optimization.function().constraint()( x ) << '\n'
		<< "_max_constraint_error = " << _max_constraint_error 
		<< '\n'
		<< "num_steps = " << num_steps << '\n'
		<< "penalty parameter = " << penalty_parameter() << '\n';
      throw ex;
    }
    constraint_error = std::abs( _optimization.function().constraint()( x ) );
    if ( constraint_error > _max_constraint_error ) {
      if ( num_steps == 0 ) {
	std::cout << "Can not satisfy constraint to given tolerance.\n";
	return false;
      }
      // Increase the constraint penalty.
      _function_with_penalty.increase_penalty();
    }
  } while ( constraint_error > _max_constraint_error );
  return true;
  */
}
#endif

template <int N, class F, class C, typename T, typename P>
inline
void
Penalty<N,F,C,T,P>::
find_minimum( point_type& x )
{
  int num_steps;
  number_type constraint_error = std::numeric_limits<number_type>::max();
  number_type step_size = _initial_step_size;

  //  _function_with_penalty.set_penalty_parameter( _initial_penalty_parameter );

  // REMOVE
  //  std::cout << "Initial = " << penalty_parameter();

  while ( step_size >= _final_step_size ) {
    // Do an optimization with the current step size.
    _optimization.set_initial_step_size( step_size );
    _optimization.set_final_step_size( step_size );
    try {
      _optimization.find_minimum( x, num_steps );
    }
    catch ( OptimizationException<N>& ex ) {
      std::cerr << "In Penalty::find_minimum():\n"
		<< "constraint = " 
		<< _function_with_penalty.constraint()( x ) << '\n'
		<< "_max_constraint_error = " << _max_constraint_error 
		<< '\n'
		<< "num_steps = " << num_steps << '\n'
		<< "penalty parameter = " << penalty_parameter() << '\n';
      throw ex;
    }

    constraint_error = std::abs( _function_with_penalty.constraint()( x ) );

    // If the optimization was unable to take any successful steps.
    if ( num_steps == 0 ) {
      // Decrease the step size;
      step_size *= _optimization.step_size_reduction_factor();
    }
    // CONTINUE
    // If the optimization took successful steps, but the constraint error
    // is too large.
    else if ( constraint_error > _max_constraint_error ) {
      // Increase the constraint penalty.
      _function_with_penalty.increase_penalty();
    }
  }
  if ( constraint_error > _max_constraint_error ) {
    PenaltyException<N> ex( "Could not satisfy constraint.",
			    x, _function_with_penalty.function()( x ),
			    _function_with_penalty.constraint()( x ),
			    _final_step_size, _max_constraint_error,
			    penalty_parameter() );
    ex.print();
    throw ex;
  }
  // REMOVE
  //  std::cout << " Final = " << penalty_parameter() << '\n';
}

END_NAMESPACE_NUMERICAL

// End of file.
