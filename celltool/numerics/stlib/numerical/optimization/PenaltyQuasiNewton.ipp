// -*- C++ -*-

#if !defined(__PenaltyQuasiNewton_ipp__)
#error This file is an implementation detail of the class PenaltyQuasiNewton.
#endif

BEGIN_NAMESPACE_NUMERICAL

//
// Constructors
//

template <int N, class F, class C, typename T, typename P>
inline
PenaltyQuasiNewton<N,F,C,T,P>::
PenaltyQuasiNewton( const function_type& function,
		    const constraint_type& constraint,
		    const number_type max_constraint_error,
		    const number_type x_tolerance,
		    const number_type gradient_tolerance,
		    const int max_function_calls ) :
  _function_with_penalty( function, constraint ),
  _optimization( _function_with_penalty, x_tolerance, gradient_tolerance,
		 max_function_calls ),
  _max_constraint_error( max_constraint_error ),
  _initial_penalty_parameter( 1 )
{
  assert( _max_constraint_error > 0 );
}

template <int N, class F, class C, typename T, typename P>
inline
void
PenaltyQuasiNewton<N,F,C,T,P>::
find_minimum( point_type& x, number_type& value, int& num_iterations,
	      number_type max_step,
	      const number_type x_tolerance,
	      const number_type gradient_tolerance )
{
  number_type constraint_error;
  int num_iterations_per_step;

  if ( x_tolerance != 0 ) {
    _optimization.set_x_tolerance( x_tolerance );
  }
  if ( gradient_tolerance != 0 ) {
    _optimization.set_gradient_tolerance( gradient_tolerance );
  }

  _function_with_penalty.set_penalty_parameter( _initial_penalty_parameter );
  num_iterations = 0;

  do {
    // Do an optimization.
    try {
      _optimization.find_minimum( x, value, num_iterations_per_step, 
				  max_step );
    }
    catch ( OptimizationException<N>& ex ) {
      std::cerr << "In PenaltyQuasiNewton::find_minimum():\n"
		<< "constraint = " 
		<< _optimization.function().constraint()( x ) << '\n'
		<< "_max_constraint_error = " << _max_constraint_error 
		<< '\n'
		<< "num_iterations = " << num_iterations_per_step << '\n'
		<< "max_step = " << max_step << '\n'
		<< "penalty parameter = " << penalty_parameter() << '\n';
      throw ex;
    }
    num_iterations += num_iterations_per_step;
    constraint_error = std::abs( _optimization.function().constraint()( x ) );
    if ( constraint_error > _max_constraint_error ) {
      // Increase the constraint penalty.
      _function_with_penalty.increase_penalty();
    }
  } while ( constraint_error > _max_constraint_error );
}

END_NAMESPACE_NUMERICAL

// End of file.
