// -*- C++ -*-

#if !defined(__Simplex_ipp__)
#error This file is an implementation detail of the class Simplex.
#endif

BEGIN_NAMESPACE_NUMERICAL


//
// Constructors
//


template<int N, class F, typename T, typename P>
inline
Simplex<N,F,T,P>::
Simplex(const function_type& function, 
	const number_type tolerance,
	const number_type offset,
	const int max_function_calls) :
  Base(function, max_function_calls),
  _tolerance(tolerance),
  _offset(offset)
{}


//
// Minimization.
//


template<int N, class F, typename T, typename P>
inline
bool
Simplex<N,F,T,P>::
find_minimum(const point_type& starting_point) {
  // Initialize the simplex.
  initialize(starting_point, _offset);
  // Find the minimum.
  return find_minimum();
}


// Find the minimum to within the tolerance.
template<int N, class F, typename T, typename P>
inline
bool
Simplex<N,F,T,P>::
find_minimum(const ads::FixedArray<N + 1,point_type>& vertices) {
  // Initialize the simplex.
  initialize(vertices);
  // Find the minimum.
  return find_minimum();
}


//
// Private member functions.
//


template<int N, class F, typename T, typename P>
inline
void
Simplex<N,F,T,P>::
sum_coordinates() {
  const point_const_iterator end = _vertices.end();
  point_const_iterator i;
  number_type sum;

  // For each dimension.
  for (int dim = 0; dim != N; ++dim) {
    sum = 0;
    // For each vertex.
    for (i = _vertices.begin(); i != end; ++i) {
      sum += (*i)[dim];
    }
    _coordinate_sums[dim] = sum;
  }
}


template<int N, class F, typename T, typename P>
inline
void
Simplex<N,F,T,P>::
initialize(const point_type& starting_point, const number_type offset) {
  reset_num_function_calls();
  // Make the simplex.
  point_type point = starting_point;
  _vertices[0] = point;
  _values[0] = evaluate_function(point);
  for (int i = 0; i != N; ++i) {
    point[i] += offset;
    _vertices[i+1] = point;
    _values[i+1] = evaluate_function(point);
    point[i] -= offset;
  }
  // Compute the diameter.
  // Note: One can compute the volume of a simplex with the Cayley-Menger 
  // determinant.  Here I use that the offsets are in orthogonal directions.
  // The volume is offset^N / N!.  The diameter is the N^th root of this.
  number_type nf = 1; 
  for (int i = 2; i <= N; ++i) {
    nf *= i;
  }
  _diameter = offset / std::pow(nf, 1.0 / N);
}


template<int N, class F, typename T, typename P>
inline
void
Simplex<N,F,T,P>::
initialize(const ads::FixedArray<N + 1,point_type>& vertices) {
  // Make the simplex.
  _vertices = vertices;
  initialize_given_vertices();
}


template<int N, class F, typename T, typename P>
inline
void
Simplex<N,F,T,P>::
initialize_given_vertices() {
  reset_num_function_calls();
  // Set the values.
  for (int i = 0; i != N + 1; ++i) {
    _values[i] = evaluate_function(_vertices[i]);
  }
  // The diameter of a hypercube that has the same volume as the simplex.
  _diameter = std::pow(geom::computeContent(_vertices), 1.0 / N);
}


template<int N, class F, typename T, typename P>
inline
bool
Simplex<N,F,T,P>::
find_minimum() {
  int i, ihi, ilo, inhi;
  number_type rtol, save_value, trial_value;

  reset_num_function_calls();
  sum_coordinates();

  for(;;) {

    // First we must determine which point is the highest (worst), 
    // next-highest, and lowest (best), by looping over the vertices of
    // the simplex.
    ilo = 0;
    ihi = 1;
    inhi = 0;
    for (i = 0; i != int(_values.size()); ++i) {
      if (_values[i] < _values[ilo]) {
	ilo = i;
      }
      else if (_values[i] > _values[ihi] ) {
	inhi = ihi;
	ihi = i;
      }
      else if (_values[i] > _values[inhi] && i != ihi) {
	inhi = i;
      }
    }
    
    // Compute the fractional range from highest to lowest.
    rtol = 2 * std::abs(_values[ihi] - _values[ilo]) /
      (std::abs(_values[ihi] + _values[ilo]) + _tolerance);
    // If the fractional range is acceptable.
    if (rtol < _tolerance) {
      // Move the best point and value to the beginning.
      std::swap(_vertices[0], _vertices[ilo]);
      std::swap(_values[0], _values[ilo]);
      // Indicate that the minimization was successful.
      return true;
    }

    if (num_function_calls() > max_function_calls()) {
      // Indicate that the minimization failed to converge.
      return false;
    }

    // Begin a new iteration.  First extrapolate by a factor -1 through
    // the face of the simplex across from the high point, i.e. reflect
    // the simplex from the high point.
    trial_value = move_high_point(ihi, -1);
    
    // If the trial value is better than the best point.
    if (trial_value <= _values[ilo]) {
      // Try an additional extrapolation by a factor of 2.
      trial_value = move_high_point(ihi, 2);
    }
    // Else if the refected point is worse than the second-highest, look for
    // an intermediate lower point, i.e., do a one-dimensional contraction.
    else if (trial_value >= _values[inhi]) {
      save_value = _values[ihi];
      trial_value = move_high_point(ihi, 0.5);
      // If we can't get rid of the high point.
      if (trial_value >= save_value) {
	// Contract around the lowest (best) point.
	for (i = 0; i != int(_vertices.size()); ++i) {
	  if (i != ilo) {
	    _vertices[i] = 0.5 * (_vertices[i] + _vertices[ilo]);
	    _values[i] = evaluate_function(_vertices[i]);
	  }
	}
	// Update the simplex diameter.
	_diameter /= 2;
	// Recompute the coordinate sums.
	sum_coordinates();
      }
    }
  }
}


template<int N, class F, typename T, typename P>
inline
typename Simplex<N,F,T,P>::number_type
Simplex<N,F,T,P>::
move_high_point(const int ihi, const number_type factor) {
  int j;
  
  point_type trial_point;
  const number_type fac1 = (1 - factor) / N;
  const number_type fac2 = fac1 - factor;
  for (j = 0; j != N; ++j) {
    trial_point[j] = _coordinate_sums[j] * fac1 - _vertices[ihi][j] * fac2;
  }
  // Evaluate the function at the trial point.
  const number_type trial_value = evaluate_function(trial_point);
  // If it's better than the highest, then replace the highest.
  if (trial_value < _values[ihi]) {
    _values[ihi] = trial_value;
    for (j = 0; j != N; ++j) {
      _coordinate_sums[j] += trial_point[j] - _vertices[ihi][j];
      _vertices[ihi][j] = trial_point[j];
    }
    // Update the simplex volume.
    _diameter *= std::pow(std::abs(factor), 1.0 / N);
  }
  return trial_value;
}


END_NAMESPACE_NUMERICAL

// End of file.
