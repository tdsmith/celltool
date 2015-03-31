// -*- C++ -*-

#if !defined(__numerical_interp_extrap_ipp__)
#error This file is an implementation detail of interp_extrap.
#endif

BEGIN_NAMESPACE_NUMERICAL

// Routine for 1-D linear interpolation/extrapolation.
// OUTPUT:
// value - The tuple of the interpolated fields.
// is_value_known - If the value can be determined.  This will be false
// iff the fields are known at none of the four points.
// INPUT:
// position - The position in index coordinates where the field will be 
//            interpolated.  This lies between the second and third 
//            grid points.
// field - The value of the field at the four grid points.
// is_known - If the value of the field is known at these grid points.
// lower_index - The index of the first grid point.
//
// I use FieldTuple instead of templating on the tuple dimension.  This way
// the argument can be a scalar or a tuple.
template<typename FieldTuple, typename T>
inline
void
int_ext(FieldTuple& value, 
	bool& is_value_known,
	const ads::FixedArray<4,FieldTuple>& field,
	const ads::FixedArray<4,bool>& is_known,
	const T position,
	const int lower_index) {
  assert(lower_index + 1 <= position && position <= lower_index + 2);

  is_value_known = true;
  if (is_known[1] && is_known[2]) {
    // Linear interpolation.
    value = field[1] + (field[2] - field[1]) * 
      (position - (lower_index + 1));
  }
  else if (is_known[1]) {
    // Constant extrapolation.
    value = field[1];
  }
  else if (is_known[2]) {
    // Constant extrapolation.
    value = field[2];
  }
  else if (is_known[0] && is_known[3]) {
    // Choose the closer one.
    if (position - lower_index < 1.5) {
      // Constant extrapolation.
      value = field[0];
    }
    else {
      // Constant extrapolation.
      value = field[3];
    }
  }
  else if (is_known[0]) {
    // Constant extrapolation.
    value = field[0];
  }
  else if (is_known[3]) {
    // Constant extrapolation.
    value = field[3];
  }
  else {
    is_value_known = false;
  }
}

/*
  template<typename FieldTuple, typename T>
  inline
  void
  int_ext(FieldTuple& value, 
  bool& is_value_known,
  const ads::FixedArray<4,FieldTuple>& field,
  const ads::FixedArray<4,bool>& is_known,
  const T position,
  const int lower_index)
  {
  assert(lower_index + 1 <= position && position <= lower_index + 2);

  is_value_known = true;
  if (is_known[1] && is_known[2]) {
  // Linear interpolation.
  value = field[1] + (field[2] - field[1]) * 
  (position - (lower_index + 1));
  }
  else if (is_known[1]) {
  if (is_known[0]) {
  // Linear extrapolation.
  value = field[0] + (field[1] - field[0]) * 
  (position - lower_index);
  }
  else {
  // Constant extrapolation.
  value = field[1];
  }
  }
  else if (is_known[2]) {
  if (is_known[3]) {
  // Linear extrapolation.
  value = field[2] + (field[3] - field[2]) * 
  (position - (lower_index + 2));
  }
  else {
  // Constant extrapolation.
  value = field[2];
  }
  }
  else if (is_known[0] && is_known[3]) {
  // Choose the closer one.
  if (position - lower_index < 1.5) {
  // Constant extrapolation.
  value = field[0];
  }
  else {
  // Constant extrapolation.
  value = field[3];
  }
  }
  else if (is_known[0]) {
  // Constant extrapolation.
  value = field[0];
  }
  else if (is_known[3]) {
  // Constant extrapolation.
  value = field[3];
  }
  else {
  is_value_known = false;
  }
  }
*/


template<int M, typename T>
inline
void
make_fields_tuple(ads::FixedArray< 4, ads::FixedArray<M,T> >& fields_tuple,
		  const ads::FixedArray< M, ads::Array<1,const T,false> >& 
		  fields,
		  const int i) {
  for (int m = 0; m != M; ++m) {
    for (int x = 0; x != 4; ++x) {
      fields_tuple[x][m] = fields[m](i + x);
    }
  }
}

template<int M, typename T>
inline
void
make_fields_tuple(ads::FixedArray< 4, ads::FixedArray<M,T> >& fields_tuple,
		  const ads::Array< 1, const ads::FixedArray<M,T>, false >& 
		  fields,
		  const int i) {
  for (int x = 0; x != 4; ++x) {
    fields_tuple[x] = fields(i + x);
  }
}


// 1-D interpolation/extrapolation at a single point.  There are M scalar
// field arrays.
// OUTPUT: 
//   values - The values of the interpolated fields.
// INPUT:
//   position - The Cartesian position at which to interpolate.
//   field - An array of values to be interpolated.
//   distance - The array of distances.
//   grid - Holds domain and grid information.
//   default_values - If no neigboring grid points are known, values is set
//     to default_values.
// TEMPLATE PARAMETERS:
//   M is the number of fields.
//   T in the number type.
// NOTE:
// I use Arrays so that this function can be called from a function 
// that is templated on the dimension, N.
template<int M, typename T, class FieldContainer>
inline
void
int_ext(ads::FixedArray<M,T>& values,
	const ads::FixedArray<1,T>& position,
	const ads::FixedArray<M,T>& default_values,
	const geom::RegularGrid<1,T>& grid,
	const ads::Array<1,const T,false>& distance,
	const FieldContainer& fields) {
  //
  // The position is surrounded by 4 grid points.
  // First calculate the index of the left point.
  //
  ads::FixedArray<1,T> continuous_index = position;
  grid.convertLocationToIndex(&continuous_index);
  const int i = static_cast<int>(std::floor(continuous_index[0])) - 1;
  
  //
  // Only do the interpolation/extrapolation if the 4 grid points are 
  // within this grid.
  //
  if (!(0 <= i && i <= grid.getExtents()[0] - 4)) {
    return;
  }

  ads::FixedArray< 4, ads::FixedArray<M,T> > fields_tuple;
  make_fields_tuple(fields_tuple, fields, i);

  ads::FixedArray<4,bool> is_known_tuple;
  for (int x = 0; x != 4; ++x) {
    is_known_tuple[x] = (0 <= distance(i + x));
  }

  //    
  // Interpolate/extrapolate.
  //
  bool is_value_known;
  int_ext(values, is_value_known,
	  fields_tuple, is_known_tuple, continuous_index[0], i);
  
  if (! is_value_known) {
    values = default_values;
  }
}




template<int M, typename T>
inline
void
make_fields_tuple(ads::FixedArray< 4, ads::FixedArray<M,T> >& fields_tuple,
		  const ads::FixedArray< M, ads::Array<2,const T,false> >& 
		  fields,
		  const int i, const int j) {
  for (int x = 0; x != 4; ++x) {
    for (int m = 0; m != M; ++m) {
      fields_tuple[x][m] = fields[m](i + x, j);
    }
  }
}

template<int M, typename T>
inline
void
make_fields_tuple(ads::FixedArray< 4, ads::FixedArray<M,T> >& fields_tuple,
		  const ads::Array< 2, const ads::FixedArray<M,T>, false >& 
		  fields,
		  const int i, const int j) {
  for (int x = 0; x != 4; ++x) {
    fields_tuple[x] = fields(i + x, j);
  }
}

// 2-D
// OUTPUT: 
//   values - The values of the interpolated fields.
// INPUT:
//   position - The Cartesian position at which to interpolate.
//   field - An array of values to be interpolated.
//   distance - The array of distances.
//   grid - Holds domain and grid information.
//   default_values - If no neigboring grid points are known, values is set
//     to default_values.
// TEMPLATE PARAMETERS
//   M is the number of fields.
//   T in the number type.
template<int M, typename T, class FieldContainer>
inline
void
int_ext(ads::FixedArray<M,T>& values,
	const ads::FixedArray<2,T>& position,
	const ads::FixedArray<M,T>& default_values,
	const geom::RegularGrid<2,T>& grid,
	const ads::Array<2,const T,false>& distance,
	const FieldContainer& fields) {
  //
  // The position lies in a square defined by 16 grid points.
  // First calculate the indices of the lower corner.
  //
  ads::FixedArray<2,T> continuous_index = position;
  grid.convertLocationToIndex(&continuous_index);
  const int i = static_cast<int>(std::floor(continuous_index[0])) - 1;
  const int j = static_cast<int>(std::floor(continuous_index[1])) - 1;

  //
  // Only do the interpolation/extrapolation if the 16 grid points are 
  // within this grid.
  //
  if (!(0 <= i && i <= grid.getExtents()[0] - 4 &&
	0 <= j && j <= grid.getExtents()[1] - 4)) {
    return;
  }

  ads::FixedArray< 4, ads::FixedArray<M,T> > fields_tuple;
  ads::FixedArray<4,bool> is_known_tuple;

  //
  // Interpolate in the x direction.
  //
  ads::FixedArray< 4, ads::FixedArray<M,T> > x_interp_fields;
  ads::FixedArray<4, bool> x_interp_is_known;
  int x, y, jj;

  for (y = 0; y != 4; ++y) {
    jj = j + y;
    make_fields_tuple(fields_tuple, fields, i, jj);
    for (x = 0; x != 4; ++x) {
      is_known_tuple[x] = (0 <= distance(i + x, jj));
    }
    int_ext(x_interp_fields[y], x_interp_is_known[y],
	    fields_tuple, is_known_tuple, continuous_index[0], i);
  }
  
  //    
  // Interpolate in the y direction.
  //
  bool is_value_known;
  int_ext(values, is_value_known,
	  x_interp_fields, x_interp_is_known, continuous_index[1], j);
  
  if (! is_value_known) {
    values = default_values;
  }
}


template<int M, typename T>
inline
void
make_fields_tuple(ads::FixedArray< 4, ads::FixedArray<M,T> >& fields_tuple,
		  const ads::FixedArray< M, ads::Array<3,const T,false> >& 
		  fields,
		  const int i, const int j, const int k) {
  for (int x = 0; x != 4; ++x) {
    for (int m = 0; m != M; ++m) {
      fields_tuple[x][m] = fields[m](i + x, j, k);
    }
  }
}

template<int M, typename T>
inline
void
make_fields_tuple(ads::FixedArray< 4, ads::FixedArray<M,T> >& fields_tuple,
		  const ads::Array< 3, const ads::FixedArray<M,T>, false >& 
		  fields,
		  const int i, const int j, const int k) {
  for (int x = 0; x != 4; ++x) {
    fields_tuple[x] = fields(i + x, j, k);
  }
}

// 3-D
// OUTPUT: 
//   values - The values of the interpolated fields.
// INPUT:
//   position - The Cartesian position at which to interpolate.
//   field - An array of values to be interpolated.
//   distance - The array of distances.
//   grid - Holds domain and grid information.
//   default_values - If no neigboring grid points are known, values is set
//     to default_values.
// TEMPLATE PARAMETERS
//   M is the number of fields.
//   T in the number type.
template<int M, typename T, class FieldContainer>
inline
void
int_ext(ads::FixedArray<M,T>& values,
	const ads::FixedArray<3,T>& position,
	const ads::FixedArray<M,T>& default_values,
	const geom::RegularGrid<3,T>& grid,
	const ads::Array<3,const T,false>& distance,
	const FieldContainer& fields) {
  //
  // The position lies in a cuboid defined by 64 grid points.
  // First calculate the indices of the lower corner of this octet.
  //
  ads::FixedArray<3,T> continuous_index = position;
  grid.convertLocationToIndex(&continuous_index);
  const int i = static_cast<int>(std::floor(continuous_index[0])) - 1;
  const int j = static_cast<int>(std::floor(continuous_index[1])) - 1;
  const int k = static_cast<int>(std::floor(continuous_index[2])) - 1;

  //
  // Only do the interpolation/extrapolation if the 64 grid points are 
  // within this grid.
  //
  if (!(0 <= i && i <= grid.getExtents()[0] - 4 &&
	0 <= j && j <= grid.getExtents()[1] - 4 &&
	0 <= k && k <= grid.getExtents()[2] - 4)) {
    return;
  }

  ads::FixedArray< 4, ads::FixedArray<M,T> > fields_tuple;
  ads::FixedArray<4,bool> is_known_tuple;

  //
  // Interpolate in the x direction.
  //
  ads::FixedArray<M,T> x_interp_fields[4][4];
  bool x_interp_is_known[4][4];
  int x, y, z, jj, kk;

  for (z = 0; z != 4; ++z) {
    for (y = 0; y != 4; ++y) {
      kk = k + z;
      jj = j + y;
      make_fields_tuple(fields_tuple, fields, i, jj, kk);
      for (x = 0; x != 4; ++x) {
	is_known_tuple[x] = (0 <= distance(i + x, jj, kk));
      }
      int_ext(x_interp_fields[y][z], x_interp_is_known[y][z],
	      fields_tuple, is_known_tuple, continuous_index[0], i);
    }
  }
  
  //    
  // Interpolate in the y direction.
  //
  ads::FixedArray< 4, ads::FixedArray<M,T> > y_interp_fields;
  ads::FixedArray<4, bool> y_interp_is_known;

  for (z = 0; z != 4; ++z) {
    for (y = 0; y != 4; ++y) {
      fields_tuple[y] = x_interp_fields[y][z];
      is_known_tuple[y] = x_interp_is_known[y][z];
    }
    int_ext(y_interp_fields[z], y_interp_is_known[z],
	    fields_tuple, is_known_tuple, continuous_index[1], j);
  }

  //    
  // Interpolate in the z direction.
  //
  bool is_value_known;
  int_ext(values, is_value_known,
	  y_interp_fields, y_interp_is_known, continuous_index[2], k);
  
  if (! is_value_known) {
    values = default_values;
  }
}


// Interplotation/extrapolation for an array of points.
template<int N, int M, typename T, class FieldContainer>
inline
void
grid_interp_extrap(ads::ArrayContainer< ads::FixedArray<M,T>, false >& values,
		   const ads::ArrayContainer< const ads::FixedArray<N,T>, false >&
		   positions,
		   const ads::FixedArray<M,T>& default_values,
		   const geom::RegularGrid<N,T>& grid,
		   const ads::Array<N,const T,false>& distance,
		   const FieldContainer& fields) {
  assert(values.size() == positions.size());

  for (int i = 0; i != positions.size(); ++i) {
    int_ext(values[i], positions[i], default_values, grid, distance, fields);
  }
}


template<int N, int M, typename T>
inline
void
grid_interp_extrap(const int num_points, 
		   T _values[],
		   const T _positions[],
		   const T _default_values[M],
		   const int _extents[N],
		   const T _domain[2 * N],
		   const T* _distance,
		   const T* _fields[M]) {
  //
  // Wrap the arguments in the proper classes.
  //

  ads::Array< 1, ads::FixedArray<M,T>, false > values(num_points, _values);

  const ads::Array< 1, const ads::FixedArray<N,T>, false > 
    positions(num_points, _positions);

  const ads::FixedArray<M,T> default_values(_default_values);

  const ads::FixedArray<N,int> extents(_extents);
  
  const geom::BBox<N,T> domain(_domain);

  const geom::RegularGrid<N,T> grid(extents, domain);

  const ads::Array<N,const T,false> distance(extents, _distance);

  ads::FixedArray< M, ads::Array<N,const T,false> > fields;
  for (int m = 0; m != M; ++m) {
    ads::Array<N,const T,false> tmp(extents, _fields[m]);
    fields[m] = tmp;
  }
  
  // Do the interpolation/extrapolation.
  grid_interp_extrap(values, positions, default_values, grid, distance, 
		     fields);
}


template<int N, int M, typename T>
inline
void
grid_interp_extrap(const int num_points, 
		   T _values[],
		   const T _positions[],
		   const T _default_values[M],
		   const int _extents[N],
		   const T _domain[2 * N],
		   const T* _distance,
		   const T* _fields) {
  //
  // Wrap the arguments in the proper classes.
  //

  ads::Array< 1, ads::FixedArray<M,T>, false > values(num_points, _values);

  const ads::Array< 1, const ads::FixedArray<N,T>, false > 
    positions(num_points, _positions);

  const ads::FixedArray<M,T> default_values(_default_values);

  const ads::FixedArray<N,int> extents(_extents);
  
  const geom::BBox<N,T> domain(_domain);

  const geom::RegularGrid<N,T> grid(extents, domain);

  const ads::Array<N,const T,false> distance(extents, _distance);

  const ads::Array<N, const ads::FixedArray<M,T>, false > 
    fields(extents, _fields);

  // Do the interpolation/extrapolation.
  grid_interp_extrap(values, positions, default_values, grid, distance, 
		     fields);
}

END_NAMESPACE_NUMERICAL

// End of file.
