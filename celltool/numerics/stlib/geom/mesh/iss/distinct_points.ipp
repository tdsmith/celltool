// -*- C++ -*-

#if !defined(__geom_mesh_iss_distinct_points_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


template<int N, typename PtForIter, typename PtOutIter, typename IntOutIter,
	 typename T>
inline
void
buildDistinctPoints(PtForIter pointsBeginning, PtForIter pointsEnd,
		    PtOutIter distinctPointsOutput,
		    IntOutIter indicesOutput,
		    const T minDistance) {
  // Get the point type from the input iterator.
  typedef typename std::iterator_traits<PtForIter>::value_type Point;
  // The container for the points.
  typedef std::vector<Point> PointContainer;
  // A record is a const iterator on points.
  typedef typename PointContainer::const_iterator Record;
  // The ORQ data structure.
  typedef geom::CellArray<N,Record> ORQDS;
  // The bounding box type.
  typedef typename ORQDS::BBox BBox;
  // A Cartesian point.
  typedef ads::FixedArray<N,T> CartesianPoint;

  // A container to hold the distinct points.
  PointContainer distinctPoints;
  // Reserve memory for these records.  This is necessary because the ORQ 
  // data structure stores pointers to the records.  Resizing the array 
  // would invalidate the pointers.
  distinctPoints.reserve(std::distance(pointsBeginning, pointsEnd));

  //
  // Make the ORQ data structure.
  //

  // Make a bounding box around the points.
  BBox box;
  box.bound(pointsBeginning, pointsEnd);
  // Choose a box size so that there will be about 10 boxes in each dimension.
  CartesianPoint delta = box.getUpperCorner();
  delta -= box.getLowerCorner();
  delta /= 9.0;
  // Make a semi-open interval that contains the points.
  box.setUpperCorner(box.getUpperCorner() + delta);
  typename ORQDS::SemiOpenInterval domain(box.getLowerCorner(), 
					  box.getUpperCorner());
  // Make the ORQ data structure.
  ORQDS orqds(delta, domain);

  // The window containing close records.
  BBox window;
  // The radius of the search window.
  const CartesianPoint offset(minDistance);
  // The vector to hold the candidate records.
  std::vector<Record> candidates;

  Point p;
  int m;
  // Loop over the points.
  for (; pointsBeginning != pointsEnd; ++pointsBeginning) {
    // The point.
    p = *pointsBeginning;
    // Make the query window.
    window.setLowerCorner(p - offset);
    window.setUpperCorner(p + offset);
    // Do the window query.
    candidates.clear();
    orqds.computeWindowQuery(std::back_inserter(candidates), window);
    // Loop over the candidate points.
    const int size = int(candidates.size());
    for (m = 0; m != size; ++m) {
      // Note: I use the abs so this works in 1-D.
      if (std::abs(geom::computeDistance(p, *candidates[m])) <= minDistance) {
	break;
      }
    }
    // If we did not find a close point.
    if (m == size) {
      // The index of the distinct point.
      *indicesOutput++ = int(distinctPoints.size());
      // Add another distinct point.
      distinctPoints.push_back(p);
      orqds.insert(distinctPoints.end() - 1);
    }
    else {
      // The index of the distinct point.
      *indicesOutput++ = int(candidates[m] - distinctPoints.begin());
    }
  }
  // Write the distinct points to the output iterator.
  const int size = int(distinctPoints.size());
  for (m = 0; m != size; ++m) {
    *distinctPointsOutput++ = distinctPoints[m];
  }
}




// Internal function for computing the maximum distance from the origin.
// The return type is the number type.
template<typename PtForIter>
inline
typename std::iterator_traits<PtForIter>::value_type::value_type
_computeMaxDistanceFromOrigin(PtForIter pointsBeginning, 
			      PtForIter pointsEnd) {
  // Get the point type from the forward iterator.
  typedef typename std::iterator_traits<PtForIter>::value_type Point;
  // Get the number type from the point type.
  typedef typename Point::value_type Number;

  //
  // Determine the max distance from the origin.
  //
  Point p;
  p = 0.0;
  Number d;
  Number maxDistance = 0;
  for (PtForIter i = pointsBeginning; i != pointsEnd; ++i) {
    d = computeSquaredDistance(p, *i);
    if (d > maxDistance) {
      maxDistance = d;
    }
  }
 return std::sqrt(maxDistance);
}




template<int N, typename PtForIter, typename PtOutIter, typename IntOutIter>
inline
void
buildDistinctPoints(PtForIter pointsBeginning, PtForIter pointsEnd,
		    PtOutIter distinctPoints, IntOutIter indices) {
  // Get the point type from the forward iterator.
  typedef typename std::iterator_traits<PtForIter>::value_type Point;
  // Get the number type from the point type.
  typedef typename Point::value_type Number;

  // Determine the max distance from the origin.
  const Number maxDistance = 
    _computeMaxDistanceFromOrigin(pointsBeginning, pointsEnd);

  // Call distinct_points() with a minimum distance.
  buildDistinctPoints<N>(pointsBeginning, pointsEnd, 
			 distinctPoints, indices,
			 maxDistance * 
			 std::sqrt(std::numeric_limits<Number>::epsilon()));
}



// Remove duplicate vertices.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
removeDuplicateVertices(IndSimpSet<N,M,A,T,V,IS>* x, const T minDistance) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;
  typedef typename ISS::Vertex Vertex;
  typedef typename ISS::IndexedSimplexIterator IndexedSimplexIterator;
  typedef typename ISS::VertexContainer VertexContainer;

  //
  // Get the indexed set of distinct vertices.
  //
  std::vector<Vertex> vertices;
  std::vector<int> indices;
  buildDistinctPoints<N>(x->getVerticesBeginning(), x->getVerticesEnd(), 
			 std::back_inserter(vertices), 
			 std::back_inserter(indices));

  // If all of the vertices are distinct.
  if (int(vertices.size()) == x->getVerticesSize()) {
    // Do nothing.
    return;
  }

  assert(int(vertices.size()) < x->getVerticesSize());
  assert(int(indices.size()) == x->getVerticesSize());

  //
  // Update the vertices.
  //
  {
    VertexContainer tmp(int(vertices.size()), vertices.begin(), 
			vertices.end());
    tmp.swap(x->getVertices());
  }
  
  //
  // Update the indexed simplices.
  //
  int m = 0;
  const IndexedSimplexIterator iEnd = x->getIndexedSimplicesEnd();
  // For each indexed simplex.
  for (IndexedSimplexIterator i = x->getIndexedSimplicesBeginning(); 
       i != iEnd; ++i) {
    // For each vertex index.
    for (m = 0; m != M+1; ++m) {
      // Update the index.
      (*i)[m] = indices[(*i)[m]];
    }
  }

  //
  // Update the topology.
  //
  x->updateTopology();
}


// Remove duplicate vertices.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
removeDuplicateVertices(IndSimpSet<N,M,A,T,V,IS>* x) {
  // Determine the max distance from the origin.
  const T maxDistance = 
    _computeMaxDistanceFromOrigin(x->getVerticesBeginning(), 
				  x->getVerticesEnd());

  // Call the above function with an approprate minimum distance.
  removeDuplicateVertices
    (x, maxDistance * std::sqrt(std::numeric_limits<T>::epsilon()));
}

END_NAMESPACE_GEOM

// End of file.
