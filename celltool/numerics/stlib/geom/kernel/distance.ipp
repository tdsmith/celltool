// -*- C++ -*-

#if !defined(__geom_distance_ipp__)
#error This file is an implementation detail of distance.
#endif

BEGIN_NAMESPACE_GEOM

//
// N-D
//

template<int N, typename T>
inline
T
computeUpperBoundOnSignedDistance
(const BBox<N,T>& box, 
 const typename BBox<N,T>::Point& x,
 const Loki::Int2Type<true> /*General case*/) {
  typedef typename BBox<N,T>::Point Point;
  Point p;
  // The mid-point.
  Point mp;

#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif

  // Compute the mid point.
  mp = box.getLowerCorner();
  mp += box.getUpperCorner();
  mp *= 0.5;

  // Determine the closest vertex of the box.
  for (int n = 0; n != N; ++n) {
    if (x[n] < mp[n]) {
      p[n] = box.getLowerCorner()[n];
    }
    else {
      p[n] = box.getUpperCorner()[n];
    }
  }

  // Examine the N neighbors of the closest vertex to find the second 
  // closest vertex.
  T d = std::numeric_limits<T>::max();
  T t;
  for (int n = 0; n != N; ++n) {
    if (p[n] == box.getUpperCorner()[n]) {
      p[n] = box.getLowerCorner()[n];
      t = geom::computeDistance(x, p);
      if (t < d) {
	d = t;
      }
      p[n] = box.getUpperCorner()[n];
    }
    else {
      p[n] = box.getUpperCorner()[n];
      t = geom::computeDistance(x, p);
      if (t < d) {
	d = t;
      }
      p[n] = box.getLowerCorner()[n];
    }
  }
  return d;
}



template<int N, typename T>
inline
T
computeLowerBoundOnSignedDistance
(const BBox<N,T>& box, 
 const typename BBox<N,T>::Point& x,
 const Loki::Int2Type<true> /*General case*/) {
  typedef typename BBox<N,T>::Point Point;
  Point p;
  
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif

  //
  // If the point is inside the box.
  //
  if (box.isIn(x)) {
    // The negative distance to the nearest wall is a lower bound on the
    // distance.
    T d = -std::numeric_limits<T>::max();
    T t;
    // For each coordinate direction.
    for (int n = 0; n != N; ++n) {
      // Check the lower wall distance.
      t = box.getLowerCorner()[n] - x[n];
      if (t > d) {
	d = t;
      }
      // Check the upper wall distance.
      t = x[n] - box.getUpperCorner()[n];
      if (t > d) {
	d = t;
      }
    }
    return d;
  }

  //
  // Else the point is outside the box.
  //

  // For each coordinate direction.
  for (int n = 0; n != N; ++n) {
    // If below the box in the x coordinate.
    if (x[n] < box.getLowerCorner()[n]) {
      p[n] = box.getLowerCorner()[n];
    }
    // If above the box in the x coordinate.
    else if (box.getUpperCorner()[n] < x[n]) {
      p[n] = box.getUpperCorner()[n];
    }
    // If in the box in the x coordinate.
    else {
      p[n] = x[n];
    }
  }
  return geom::computeDistance(x, p);
}







//
// 1-D
//

template<typename T>
inline
T
computeUpperBoundOnSignedDistance
(const BBox<1,T>& box, 
 const typename BBox<1,T>::Point& x,
 const Loki::Int2Type<false> /*Special case*/) {
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif
  if (x[0] <= box.getLowerCorner()[0]) {
    return box.getLowerCorner()[0] - x[0];
  }
  if (x[0] >= box.getUpperCorner()[0]) {
    return x[0] - box.getUpperCorner()[0];
  }
  return std::min(box.getUpperCorner()[0] - x[0], 
		  x[0] - box.getLowerCorner()[0]);
}


template<typename T>
inline
T
computeLowerBoundOnSignedDistance
(const BBox<1,T>& box, 
 const typename BBox<1,T>::Point& x,
 const Loki::Int2Type<false> /*Special case*/) {
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif
  if (x[0] <= box.getLowerCorner()[0]) {
    return box.getLowerCorner()[0] - x[0];
  }
  if (x[0] >= box.getUpperCorner()[0]) {
    return x[0] - box.getUpperCorner()[0];
  }
  return std::max(x[0] - box.getUpperCorner()[0], 
		  box.getLowerCorner()[0] - x[0]);
}






//
// N-D
//

// CONTINUE: These are not declared in the header.
// Are they used or tested?
template<int N, typename T>
inline
T
computeUpperBoundOnUnsignedDistance(const BBox<N,T>& box, 
				    const typename BBox<N,T>::Point& x) {
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif

  // Determine the closest face.
  int min_n = 0;
  T min_d = std::numeric_limits<T>::max();
  T d;
  for (int n = 0; n != N; ++n) {
    d = std::min(std::abs(x[n] - box.getUpperCorner()[n]), 
		  std::abs(x[n] - box.getLowerCorner()[n]));
    if (d < min_d) {
      min_n = n;
      min_d = d;
    }
  }
  
  // Compute the distance to the farthest point on the closest face.
  T dist = d * d;
  for (int n = 0; n != N; ++n) {
    if (n != min_n) {
      d = std::max(std::abs(x[n] - box.getUpperCorner()[n]), 
		   std::abs(x[n] - box.getLowerCorner()[n]));
      dist += d * d;
    }
  }
  return std::sqrt(dist);
}


template<int N, typename T>
inline
T
computeLowerBoundOnUnsignedDistance(const BBox<N,T>& box, 
				    const typename BBox<N,T>::Point& x) {
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif

  T d;
  T dist = 0;
  for (int n = 0; n != N; ++n) {
    if (x[n] <= box.getLowerCorner()[n]) {
      d = box.getLowerCorner()[n] - x[n];
    }
    else if (x[n] >= box.getUpperCorner()[n]) {
      d = x[n] - box.getUpperCorner()[n];
    }
    else {
      d = 0;
    }
    dist += d * d;
  }
  return std::sqrt(dist);
}



//
// 1-D
//

// CONTINUE
#if 0
template<typename T>
inline
T
computeUpperBoundOnUnsignedDistance(const BBox<1,T>& box, 
				    const typename BBox<1,T>::Point& x) {
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif
  if (x[0] <= box.getLowerCorner()[0]) {
    return box.getLowerCorner()[0] - x[0];
  }
  if (x[0] >= box.getUpperCorner()[0]) {
    return x[0] - box.getUpperCorner()[0];
  }
  return std::min(box.getUpperCorner()[0] - x[0], 
		  x[0] - box.getLowerCorner()[0]);
}


template<typename T>
inline
T
computeLowerBoundOnUnsignedDistance(const BBox<1,T>& box, 
				    const typename BBox<1,T>::Point& x) {
#ifdef DEBUG_geom
  // The box should not be degenerate.
  assert(! box.isEmpty());
#endif
  if (x[0] <= box.getLowerCorner()[0]) {
    return box.getLowerCorner()[0] - x[0];
  }
  if (x[0] >= box.getUpperCorner()[0]) {
    return x[0] - box.getUpperCorner()[0];
  }
  return 0;
}
#endif

END_NAMESPACE_GEOM
