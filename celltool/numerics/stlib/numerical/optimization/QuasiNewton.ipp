// -*- C++ -*-

#if !defined(__QuasiNewton_ipp__)
#error This file is an implementation detail of the class QuasiNewton.
#endif

BEGIN_NAMESPACE_NUMERICAL

/*
  Given a starting point x The BFGS variant of the DFP minimization is 
  performed on the function using its gradient.  The convergence requirement
  on zeroing the gradient is input as gradient_tolerance.  
  Returned quantities are 
  x (the location of the minimum), 
  value (the minimum value of the function)
  and num_iterations (the number of iterations performed).
*/
template <int N, class F, typename T, typename P>
inline
void
QuasiNewton<N,F,T,P>::
find_minimum( point_type& x, number_type& value, int& num_iterations,
	      number_type max_step,
	      const number_type x_tolerance,
	      const number_type gradient_tolerance )
{
  const int MAX_ITERATIONS = 200;
  const number_type MAX_STEP = 100;

  number_type den, fac, fad, fae, fx, sumdg, sumxi, temp, 
    test;

  point_type dg, g, hdg, xnew, xi;
  matrix_type inverse_hessian;

  // See if they have changed the convergence tolerances.
  if ( x_tolerance != 0.0 ) {
    _x_tolerance = x_tolerance;
  }
  if ( gradient_tolerance != 0.0 ) {
    _gradient_tolerance = gradient_tolerance;
  }

  reset_num_function_calls();

  // Calculate the starting function value and gradient.
  fx = evaluate_function( x );
  evaluate_gradient( x, g );
  // Initialize the inverse Hessian to the unit matrix.
  inverse_hessian = 0.0;
  for ( int i = 0; i != N; ++i ) {
    inverse_hessian(i,i) = 1;
  }
  // Initial line direction.
  xi = - g;
  // If the maximum step size was not provided, set it to something reasonable.
  if ( max_step == 0.0 ) {
    max_step = MAX_STEP * std::max( geom::computeMagnitude( x ), 
				    number_type( N ) );
  }

  // Main loop over the iterations.
  for ( num_iterations = 0; num_iterations != MAX_ITERATIONS; 
	++num_iterations ) {
    line_search( x, fx, g, xi, xnew, value, max_step );
    // The new function evaluation occurs in line_search.  Save the function
    // value in fx for the next line search.
    fx = value;
    // Update the line direction.
    xi = xnew; 
    xi -= x;
    x = xnew;

    // Test for convergence on delta x.
    test = 0.0;
    for ( int i = 0; i != N; ++i ) {
      temp = std::abs( xi[i] ) / std::max( std::abs( x[i] ), 1.0 );
      if ( temp > test ) {
	test = temp;
      }
    }
    if ( test < _x_tolerance ) {
      return;
    }

    // Save the old gradient.
    dg = g;
    // Get the new gradient.
    evaluate_gradient( x, g );

    // Test for convergence on zero gradient.
    test = 0;
    den = std::max( value, 1.0 );
    for ( int i = 0; i != N; ++i ) {
      temp = std::abs( g[i] ) * std::max( std::abs( x[i] ), 1.0 ) / den;
      if ( temp > test ) {
	test = temp;
      }
    }
    if ( test < _gradient_tolerance ) {
      return;
    }

    // Compute the difference of gradients.
    dg.negate();
    dg += g;
    // Compute the difference times the current matrix.
    hdg = 0;
    for ( int i = 0; i != N; ++i ) {
      for ( int j = 0; j != N; ++j ) {
	hdg[i] += inverse_hessian(i,j) * dg[j];
      }
    }

    // Calculate dot products for the denominators.
    fac = geom::computeDotProduct( dg, xi );
    fae = geom::computeDotProduct( dg, hdg );
    sumdg = geom::computeDotProduct( dg, dg );
    sumxi = geom::computeDotProduct( xi, xi );

    // Skip update if fac is not sufficiently positive.
    if ( fac > std::sqrt( std::numeric_limits<number_type>::epsilon() * 
			  sumdg * sumxi ) ) {
      fac = 1.0 / fac;
      fad = 1.0 / fae;
      // The vector that makes BFGS different from DFP.
      for ( int i = 0; i != N; ++i ) {
	dg[i] = fac * xi[i] - fad * hdg[i];
      }
      // The BFGS updating formula.
      for ( int i = 0; i != N; ++i ) {
	for ( int j = i; j != N; ++j ) {
	  inverse_hessian(i,j) += fac * xi[i] * xi[j] - fad * hdg[i] * hdg[j]
	    + fae * dg[i] * dg[j];
	  inverse_hessian(j,i) = inverse_hessian(i,j);
	}
      }
    }
    
    // Calculate the next direction.
    xi = 0.0;
    for ( int i = 0; i != N; ++i ) {
      for ( int j = 0; j != N; ++j ) {
	xi[i] -= inverse_hessian(i,j) * g[j];
      }
    }
  }
  OptimizationException<N,number_type,point_type>
    ex( "Too many iterations in QuasiNewton::find_minimum.",
	x, value, num_function_calls() );
  throw ex;
}

//
// Private member functions.
//

/*
  Given a point x_old, the value of the function f_old and gradient there,
  and a direction, this function finds a new point x along the 
  given direction from x_old where the function has decreased "sufficiently".
  The new function value is returned in f.  max_step is an input quantity
  that limits the length of the steps so that you do not try to 
  evaluate the function in regions where it is undefined or subject to 
  overflow.  The direction is the Newton direction.  
 */
template <int N, class F, typename T, typename P>
inline
void
QuasiNewton<N,F,T,P>::
line_search( const point_type& x_old, const number_type f_old,
	     point_type& gradient, point_type& direction, point_type& x,
	     number_type& f, const number_type max_step )
{
  // alpha ensures sufficient decrease in function value.
  const number_type alpha = 1.0e-4;
  // x_tolerance is the convergence criterion on delta x.
  const number_type x_tolerance = std::numeric_limits<number_type>::epsilon();

  number_type a, lambda, lambda2 = 0.0, lambda_min, b, disc, f2 = 0.0;
  number_type rhs1, rhs2, slope, temp, test, temp_lambda;
  bool first_time = true;

  // Scale if the attempted step is too big.
  const number_type length_direction = geom::computeMagnitude( direction );
  if ( length_direction > max_step ) {
    direction *= max_step / length_direction;
  }
  
  slope = geom::computeDotProduct( gradient, direction );
  if ( slope >= 0.0 ) {
    OptimizationException<N,number_type,point_type>
      ex( "Bad slope in QuasiNewton::line_search.",
	  x_old, f_old, num_function_calls() );
    throw ex;
  }
  test = 0.0;
  for ( int i = 0; i != N; ++i ) {
    temp = std::abs( direction[i] ) / std::max( std::abs( x_old[i] ), 1.0 );
    if ( temp > test ) {
      test = temp;
    }
  }
  
  lambda_min = x_tolerance / test;

  // REMOVE
  /*
  point_type num_grad;
  evaluate_numeric_gradient( x, num_grad );
  std::cout << "x_old = " << x_old << '\n'
	    << "f_old = " << f_old << '\n'
	    << "gradient = " << gradient << '\n'
	    << "num_grad = " << num_grad << '\n'
	    << "direction = " << direction << '\n'
	    << "slope = " << slope << '\n'
	    << "lambda_min = " << lambda_min << '\n'
	    << "max_step = " << max_step << '\n' << '\n';
  */

  // Always try a full Newton step first.
  lambda = 1.0;
  // Start of iteration loop.
  for(;;) {
    // Convergence on delta x.
    if ( lambda < lambda_min ) {
      x = x_old;
      return;
    }

    for ( int i = 0; i != N; ++i ) {
      x[i] = x_old[i] + lambda * direction[i];
    }
    // REMOVE
    //std::cerr << "evaluate " << x << '\n';
    f = evaluate_function( x );
    // REMOVE
    //std::cerr << "f = " << f << '\n';
    
    // REMOVE
    /*
    std::cout << "lambda = " << lambda << '\n'
	      << "x = " << x << '\n'
	      << "f = " << f << '\n'
	      << "alpha * lambda * slope = " << alpha * lambda * slope 
	      << '\n';
    */

    /* CONTINUE: this has compilation problems.  Fix and add back in.
    if ( ! std::isfinite( f ) ) {

      // REMOVE
      std::cerr << "Problem: f is not finite.\n"
		<< "x_old = " << x_old << '\n'
		<< "f_old = " << f_old << '\n'
		<< "lambda = " << lambda << '\n'
		<< "direction = " << direction << '\n'
		<< "x = " << x << '\n'
		<< "f = " << f << '\n';

      // If f is not finite, reduce lambda.
      lambda *= 0.1;
      continue;
    }
    */

    // Sufficient function decrease.
    if ( f <= f_old + alpha * lambda * slope ) {
      return;
    }
    // Else backtrack.
    // First time.
    if ( first_time ) {
      temp_lambda = - slope / ( 2.0 * ( f - f_old - slope ) );
      first_time = false;
    }
    else {
      rhs1 = f - f_old - lambda * slope;
      rhs2 = f2 - f_old - lambda2 * slope;
      a = ( rhs1 / ( lambda * lambda ) - rhs2 / ( lambda2 * lambda2 ) ) / 
	( lambda - lambda2 );
      b = ( - lambda2 * rhs1 / ( lambda * lambda ) + lambda * rhs2 / 
	    ( lambda2 * lambda2 ) ) / ( lambda - lambda2 );
      if ( a == 0.0 ) {
	temp_lambda = - slope / ( 2.0 * b );
      }
      else {
	disc = b * b - 3.0 * a * slope;
	if ( disc < 0.0 ) {
	  temp_lambda = lambda / 2.0;
	}
	else if ( b <= 0.0 ) {
	  temp_lambda = ( - b + std::sqrt( disc ) ) / ( 3.0 * a );
	}
	else {
	  temp_lambda = - slope / ( b + std::sqrt( disc ) );
	}
      }
      if ( temp_lambda > lambda / 2.0 ) {
	temp_lambda = lambda / 2;
      }
      // REMOVE
      /*
      std::cout << "lambda = " << lambda << '\n'
		<< "lambda2 = " << lambda2 << '\n'
		<< "f = " << f << '\n'
		<< "f2 = " << f2 << '\n'
		<< "f_old = " << f_old << '\n'
		<< "slope = " << slope << '\n'
		<< "rhs1 = " << rhs1 << '\n'
		<< "rhs2  = " << rhs2 << '\n'
		<< "a = " << a << '\n'
		<< "b = " << b << '\n'
		<< "temp_lambda = " << temp_lambda << '\n'
		<< "disc = " << disc << '\n' << '\n';
      */
    }
    lambda2 = lambda;
    f2 = f;
    lambda = std::max( temp_lambda, 0.1 * lambda );
  }
}

END_NAMESPACE_NUMERICAL

// End of file.
