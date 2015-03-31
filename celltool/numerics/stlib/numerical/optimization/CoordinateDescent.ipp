// -*- C++ -*-

#if !defined(__CoordinateDescent_ipp__)
#error This file is an implementation detail of the class CoordinateDescent.
#endif

BEGIN_NAMESPACE_NUMERICAL

//
// Constructors
//

template <int N, class F, typename T, typename P>
inline
CoordinateDescent<N,F,T,P>::
CoordinateDescent(const function_type& function, 
		  const number_type initial_step_size,
		  const number_type final_step_size,
		  const int max_function_calls) :
  base_type(function, max_function_calls),
  _initial_step_size(initial_step_size),
  _final_step_size(final_step_size),
  _step_size_reduction_factor(0.5),
  _step_limit(2 * int(std::ceil(1 / _step_size_reduction_factor)) + 1)
{
}

template <int N, class F, typename T, typename P>
inline
bool
CoordinateDescent<N,F,T,P>::
find_minimum(point_type& x, number_type& value, int& num_steps)
{
  point_type delta;
  number_type trial_value;
  bool found_descent_direction;

  num_steps = 0;
  reset_num_function_calls();

  _step_size = _initial_step_size;
  try {
    value = evaluate_function(x);
  }
  catch (OptimizationException<N>& ex) {
    std::cerr << "Exception in CoordinateDescent::find_minimum().\n"
	      << "Error in evaluating function.\n";
    throw ex;
  }

  while (_step_size >= _final_step_size) {
    // REMOVE
    /*
      if (is_max_function_calls_exceeded()) {
      std::cout << "Max function calls exceeded.\n"
      << "step size = " << _step_size << '\n'
      << "final step size = " << _final_step_size << '\n'
      << "delta = " << delta << '\n';
      return false;
      }
    */

    // REMOVE
    /*
      std::cout << "After " << num_function_calls() 
      << " function evaluations, f(x) = " << value
      << '\n'
      << "x = " << x << '\n';
    */

    // Find a descent direction by searching in each coordinate direction.
    trial_value = value;
    try {
      found_descent_direction = descent_direction(x, value, delta);
    }
    catch (OptimizationException<N>& ex) {
      std::cerr << "Exception in CoordinateDescent::find_minimum().\n"
		<< "Error in finding descent direction.\n";
      throw ex;
    }
    if (found_descent_direction) {
      ++num_steps;
      // Since we made some improvement, pursue that direction.
      trial_value = value;
      int acceleration_step_count = 0;
      do {
	// If we have taken too many steps.
	if (acceleration_step_count != 0 && 
	    acceleration_step_count % _step_limit == 0) {
	  // Increase the step size;
	  delta /= _step_size_reduction_factor;
	}
	++acceleration_step_count;

	// Move further in the descent direction.
	value = trial_value;
	x += delta;
	try {
	  trial_value = evaluate_function(x);
	}
	catch (OptimizationException<N>& ex) {
	  std::cerr << "Exception in CoordinateDescent::find_minimum().\n"
		    << "Error in acceleration step.\n"
		    << "_step_size = " << _step_size << '\n'
		    << "_final_step_size = " << _final_step_size << '\n'
		    << "_step_size_reduction_factor = " 
		    << _step_size_reduction_factor << '\n'
		    << "delta = " << delta << '\n'
		    << "acceleration_step_count = " << acceleration_step_count;
	  throw ex;
	}
	// REMOVE
	/*
	  if (acceleration_step_count % 100 == 0) {
	  std::cout << "Lots of acceleration steps.\n"
	  << "step size = " << _step_size << '\n'
	  << "final step size = " << _final_step_size << '\n'
	  << "delta = " << delta << '\n';
	  }
	*/
      } while (trial_value < value);
      // Undo the last bad step.
      x -= delta;
    }
    else {
      // We failed to find a descent direction.  Reduce the step size.
      _step_size *= _step_size_reduction_factor;
    }
  }
  // If we make it here, then we did not exceed the maximum allowed number
  // of function calls.
  return true;
}

//
// Private member functions.
//

template <int N, class F, typename T, typename P>
inline
bool
CoordinateDescent<N,F,T,P>::
descent_direction(point_type& x, number_type& value, point_type& delta)
{
  bool result = false;
  delta = 0;
  for (int i = 0; i != N; ++i) {
    // Try searching in the positive direction.
    if (coordinate_search(x, value, delta, i, 1)) {
      result = true;
    }
    // If that didn't work, search in the negative direction.
    else if (coordinate_search(x, value, delta, i, -1)) {
      result = true;
    }
  }
  return result;
}

template <int N, class F, typename T, typename P>
inline
bool
CoordinateDescent<N,F,T,P>::
coordinate_search(point_type& x, number_type& value, point_type& delta, 
		  const int i, const int sign)
{
#ifdef DEBUG_CoordinateDescent
  assert(delta[i] == 0);
  assert(sign == 1 || sign == -1);
#endif

  number_type trial_value = value;
  // Start at -1 because we step until the function no longer decreases.
  // Thus the last step is a bad one and we don't use it.
  int step_count = -1;
  do {
    // If we have taken too many steps.
    if (step_count != 0 && step_count % _step_limit == 0) {
      // Increase the step size;
      _step_size /= _step_size_reduction_factor;
    }
    ++step_count;

    value = trial_value;
    x[i] += sign * _step_size;
    delta[i] += sign * _step_size;
    try {
      trial_value = evaluate_function(x);
    }
    catch (OptimizationException<N>& ex) {
      std::cerr << "\nIn coordinate_search():\n"
		<< "delta = " << delta << '\n'
		<< "i = " << i << '\n'
		<< "step_count = " << step_count << '\n';
      throw ex;
    }
  } while (trial_value < value);
  // Undo the last bad step.
  x[i] -= sign * _step_size;
  delta[i] -= sign * _step_size;

  return step_count != 0;
}

END_NAMESPACE_NUMERICAL

// End of file.
