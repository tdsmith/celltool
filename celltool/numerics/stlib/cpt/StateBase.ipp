// -*- C++ -*-

#if !defined(__cpt_StateBase_ipp__)
#error This file is an implementation detail of the class StateBase.
#endif

BEGIN_NAMESPACE_CPT

template<int N, typename T>
inline
void
StateBase<N,T>::
displayInformation(std::ostream& out) const {
  out << "The domain containing the grids is " 
      << getDomain() << '\n'
      << "The distance transform will be computed up to " 
      << getMaximumDistance() << '\n'
      << "There are " << getNumberOfGrids() << " grids.\n";

  if (hasBRepBeenSet()) {
    out << "The b-rep has been set." << '\n';
    _brep.displayInformation(out);
  }
  else {
    out << "The b-rep has not been set." << '\n';
  }

  if (_hasCptBeenComputed) {
    out << "The closest point transform has been computed." << '\n';
    int numberKnown = 0;
    Number minimum = std::numeric_limits<Number>::max();
    Number maximum = -std::numeric_limits<Number>::max();
    Number minimumValue, maximumValue;
    // Loop over the grids.
    for (int n = 0; n != getNumberOfGrids(); ++n) {
      numberKnown += _grids[n].countKnownDistances(_maximumDistance);
      _grids[n].computeMinimumAndMaximumDistances(_maximumDistance, 
						  &minimumValue, 
						  &maximumValue);
      if (minimumValue < minimum) {
	minimum = minimumValue;
      }
      if (maximumValue > maximum) {
	maximum = maximumValue;
      }
    }
    out << "There are " << numberKnown << " known distances.\n"
	<< "The minimum known distance is " << minimum << ".\n"
	<< "The maximum known distance is " << maximum << ".\n";
  }
  else {
    out << "The closest point transform has not been computed." << '\n';
  }
}


template<int N, typename T>
inline
void
StateBase<N,T>::
setParameters(const BBox& domain, const Number maximumDistance) {
  // Sanity check.
  assert(! domain.isEmpty());
  assert(maximumDistance > 0);

  _domain = domain;
  _maximumDistance = maximumDistance;
}


template<int N, typename T>
inline
void
StateBase<N,T>::
setLattice(const Index& extents, const BBox& domain) {
  // Sanity check.
  for (int n = 0; n != N; ++n) {
    assert(extents[n] > 1);
  }
  assert(! domain.isEmpty());

  _lattice = Lattice(extents, domain);
}


template<int N, typename T>
template<bool A1, bool A2, bool A3, bool A4>
inline
void
StateBase<N,T>::
insertGrid(ads::Array<N,Number,A1>* distance,
	   ads::Array<N,Point,A2>* gradientOfDistance,
	   ads::Array<N,Point,A3>* closestPoint,
	   ads::Array<N,int,A4>* closestFace) {
  // Make the grid.  Initially the distance, the gradient of the distance,
  // the closest point and closest face are undefined.
  Grid tmp(distance, gradientOfDistance, closestPoint, closestFace);

  // Add the grid.
  _grids.push_back(tmp);
}


template<int N, typename T>
inline
void
StateBase<N,T>::
insertGrid(const int* indexLowerBounds,
	   const int* indexUpperBounds,
	   Number* distance,
	   Number* gradientOfDistance,
	   Number* closestPoint,
	   int* closestFace) {
  Range emptyRange;
  // CONTINUE: For some reason it won't compile using the copy constructor.
  Range distanceRange = Range(Index(indexLowerBounds), 
			      Index(indexUpperBounds));
  
  Range gradientOfDistanceRange = distanceRange;
  if (gradientOfDistance == 0) {
    gradientOfDistanceRange = emptyRange;
  }
  Range closestPointRange = distanceRange;
  if (closestPoint == 0) {
    closestPointRange = emptyRange;
  }
  Range closestFaceRange = distanceRange;
  if (closestFace == 0) {
    closestFaceRange = emptyRange;
  }

  ads::Array<N,Number,false> distanceArray(distanceRange, distance);
  ads::Array<N,Point,false>
    gradientOfDistanceArray(gradientOfDistanceRange, gradientOfDistance);
  ads::Array<N,Point,false>
    closestPointArray(closestPointRange, closestPoint);
  ads::Array<N,int,false> 
    closestFaceArray(closestFaceRange, closestFace);

  insertGrid(&distanceArray, &gradientOfDistanceArray, 
	     &closestPointArray, &closestFaceArray);
}


template<int N, typename T>
inline
std::pair<int,int>
StateBase<N,T>::
computeClosestPointTransformUsingBBox() {
  // CONTINUE: Remove the requirement that num_grids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Compute the closest point transforms.
  return _brep.computeClosestPointUsingBBox(_lattice, &_grids, 
					    _maximumDistance);
}


template<int N, typename T>
inline
std::pair<int,int>
StateBase<N,T>::
computeClosestPointTransformUsingBruteForce() {
  // CONTINUE: Remove the requirement that num_grids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Compute the closest point transforms.
  return _brep.computeClosestPointUsingBruteForce(_lattice, &_grids, 
						  _maximumDistance);
}


template<int N, typename T>
inline
std::pair<int,int>
StateBase<N,T>::
computeClosestPointTransformUsingTree() {
  // CONTINUE: Remove the requirement that num_grids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Make the bounding box tree.
  //CONTINUE HERE;
  assert(false);

  // Compute the closest point transforms for each grid.
  std::pair<int,int> counts, c;
  counts.first = 0;
  counts.second = 0;
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    //CONTINUE HERE;
    //c = _grids[n].closestPoint_transform(_lattice, TREE, _maximumDistance);
    counts.first += c.first;
    counts.second += c.second;
  }
  return counts;
}


template<int N, typename T>
inline
std::pair<int,int>
StateBase<N,T>::
computeClosestPointTransformUnsignedUsingBBox() {
  // CONTINUE: Remove the requirement that num_grids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Compute the closest point transforms.
  return _brep.computeClosestPointUnsignedUsingBBox(_lattice, &_grids, 
						    _maximumDistance);
}



template<int N, typename T>
inline
std::pair<int,int>
StateBase<N,T>::
computeClosestPointTransformUnsignedUsingBruteForce() {
  // CONTINUE: Remove the requirement that num_grids > 0.
  // Make sure everything is set.
  assert(getNumberOfGrids() > 0 && hasBRepBeenSet());

  // Initialize the grids.
  initializeGrids();

  // Signify that the cpt has been computed.
  _hasCptBeenComputed = true;

  // Compute the closest point transforms.
  return _brep.computeClosestPointUnsignedUsingBruteForce(_lattice, &_grids, 
							  _maximumDistance);
}



template<int N, typename T>
inline
void
StateBase<N,T>::
floodFillAtBoundary(const Number farAway) {
  // Make sure the cpt has been computed first.
  assert(_hasCptBeenComputed);

  // For each grid.
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    // Flood fill the distance grid.
    _grids[n].floodFill(farAway);
  }
}



template<int N, typename T>
inline
void
StateBase<N,T>::
floodFillDetermineSign(const Number farAway) {
  // Make sure the cpt has been computed first.
  assert(_hasCptBeenComputed);

  std::vector<int> farAwayGrids;
  // For each grid.
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    // Flood fill the distance grid.
    if (! _grids[n].floodFill(farAway)) {
      // Record this grid if there are no known distances.
      farAwayGrids.push_back(n);
    }
  }

  if (farAwayGrids.empty()) {
    return;
  }

  // Determine a Cartesian point that lies in each far away grid.
  std::vector<Point> lowerCorners(farAwayGrids.size());
  for (int n = 0; n != int(farAwayGrids.size()); ++n) {
    lowerCorners[n] = _grids[farAwayGrids[n]].getRanges().lbounds();
    _lattice.convertIndexToLocation(&lowerCorners[n]);
  }

  // Determine the signed distance to the lower corners.
  std::vector<Number> distances;
  geom::computeSignedDistance(_brep, lowerCorners.begin(), lowerCorners.end(),
			      std::back_inserter(distances));

  // Set the distances for the far away grids to +- farAway.
  for (int n = 0; n != int(farAwayGrids.size()); ++n) {
    _grids[farAwayGrids[n]].getDistance() = 
      (distances[n] > 0 ? 1 : -1) * farAway;
  }
}



template<int N, typename T>
inline
void
StateBase<N,T>::
floodFillUnsigned(const Number farAway) {
  // Make sure the cpt has been computed first.
  assert(_hasCptBeenComputed);

  // For each grid.
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    // Flood fill the distance grid.
    _grids[n].floodFillUnsigned(farAway);
  }
}



template<int N, typename T>
inline
bool 
StateBase<N,T>::
areGridsValid() {
  // Check that there are grids.
  if (getNumberOfGrids() == 0) {
    return false;
  }

  const int MaximumFaceIdentifier = _brep.getMaximumFaceIdentifier();
  // For each grid.
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    // Check the grid.
    if (! _grids[n].isValid(_lattice, _maximumDistance, 
			    MaximumFaceIdentifier)) {
      std::cerr << "Grid number " << n << " is not valid.\n";
      return false;
    }
  }

  // If we got here, then all the grids are valid.
  return true;
}


template<int N, typename T>
inline
bool 
StateBase<N,T>::
areGridsValidUnsigned() {
  // Check that there are grids.
  if (getNumberOfGrids() == 0) {
    return false;
  }

  const int MaximumFaceIdentifier = _brep.getMaximumFaceIdentifier();
  // For each grid.
  for (int n = 0; n != getNumberOfGrids(); ++n) {
    // Check the grid.
    if (! _grids[n].isValidUnsigned(_lattice, _maximumDistance, 
				    MaximumFaceIdentifier)) {
      std::cerr << "Grid number " << n << " is not valid.\n";
      return false;
    }
  }

  // If we got here, then all the grids are valid.
  return true;
}


template<int N, typename T>
inline
void 
StateBase<N,T>::
setBRepWithNoClipping(const int verticesSize, const void* vertices,
		      const int facesSize, const void* faces) {
  _hasBRepBeenSet = true;
  // Make the BRep.
  _brep.make(verticesSize, vertices, facesSize, faces);
}


template<int N, typename T>
inline
void 
StateBase<N,T>::
setBRep(const int verticesSize, const void* vertices,
	const int facesSize, const void* faces) {
  _hasBRepBeenSet = true;

  // Check that the cartesian domain has been set.
  assert(! _domain.isEmpty() && _maximumDistance > 0);

  // Make the BRep.
  _brep.make(verticesSize, vertices, facesSize, faces,
	     getDomain(), _maximumDistance);
}

END_NAMESPACE_CPT
