// -*- C++ -*-

#if !defined(__geom_ScanConversionPolygon_ipp__)
#error This file is an implementation detail of the class ScanConversionPolygon.
#endif

BEGIN_NAMESPACE_GEOM


//
// Constructors and destructor.
//


template<typename T>
inline
ScanConversionPolygon<T>::
ScanConversionPolygon(const SizeType size) : 
  _vertices() {
  _vertices.reserve(size);
}


template<typename T>
inline
ScanConversionPolygon<T>::
ScanConversionPolygon(const ScanConversionPolygon& other) : 
  _vertices(other._vertices)
{}


template<typename T>
inline
ScanConversionPolygon<T>& 
ScanConversionPolygon<T>::
operator=(const ScanConversionPolygon& other) {
  // Avoid assignment to self
  if (&other != this){
    _vertices = other._vertices;
  }
  // Return *this so assignments can chain
  return *this;
}


//
// Mathematical operations
//


template<typename T>
inline
void 
ScanConversionPolygon<T>::
orderVertices() {
  // If the polygon is not degenerate.
  if (_vertices.size() >= 3) {

    // Find the vertex with minimum y coordinate and put it in the 
    // first position.
    Point temp;
    Iterator min = _vertices.begin();
    Iterator iter = _vertices.begin() + 1;
    for (; iter != _vertices.end(); ++iter) {
      if ((*iter)[1] < (*min)[1]) {
	min = iter;
      }
    }
    if (min != _vertices.begin()) {
      temp = *_vertices.begin();
      *_vertices.begin() = *min;
      *min = temp;
    }
    
    // Calculate the pseudo-angle from the bottom point.
    std::vector<Number> angle(_vertices.size());
    angle.clear();
    const Point bottom = *_vertices.begin();
    for (iter = _vertices.begin(); iter != _vertices.end(); ++iter) {
      temp = *iter - bottom;
      angle.push_back(computePseudoAngle(temp));
    }

    // Sort the vertices by angle.
    int i, j;
    Number angleTemp;
    for (i = int(_vertices.size()) - 1; i >= 0; --i) {
      for (j = 1; j <= i; ++j) {
	if (angle[j-1] > angle[j]) {
	  temp = _vertices[j-1];
	  _vertices[j-1] = _vertices[j];
	  _vertices[j] = temp;
	  angleTemp = angle[j-1];
	  angle[j-1] = angle[j];
	  angle[j] = angleTemp;
	}
      }
    }
  }
}


template<typename T>
inline
void 
ScanConversionPolygon<T>::
removeDuplicates() {
  // The square of the floating point precision.
  static Number eps = 
    std::pow(10 * std::numeric_limits<Number>::epsilon(), 2);

  // If there is more than one vertex.
  if (_vertices.size() > 1) {
    Iterator prev = _vertices.begin();
    Iterator iter = _vertices.begin() + 1;
    while (iter != _vertices.end()) {
      if (computeSquaredDistance(*iter, *prev) < eps) {
	_vertices.erase(iter);
	iter = prev + 1;
      }
      else {
	++prev;
	++iter;
      }
    }
  }
}


template<typename T>
inline
int 
ScanConversionPolygon<T>::
computeBottomAndTop(Number* bottom, Number* top) const {
  // Sanity check: The polygon should not be degenerate.
  assert(_vertices.size() >= 3);

  int minIndex = 0;
  *bottom = *top = _vertices[0][1];
  Number y;
  const int size(int(_vertices.size()));
  for (int i = 1; i < size; ++i) {
    if ((y = _vertices[i][1]) < *bottom) {
      minIndex = i;
      *bottom = y;
    }
    else if (y > *top) {
      *top = y;
    }
  }
  return minIndex;
}


template<typename T>
template<typename IndexOutputIterator, int N>
inline
void 
ScanConversionPolygon<T>::
scanConvertTriangle(IndexOutputIterator coordinates,
		    const ads::FixedArray<N,int>& extents,
		    ads::FixedArray<N,int> multiIndex) const {
  assert(_vertices.size() == 3);

  //
  // Determine the bottom, left and right vertices.
  //
  int bottomIndex, rightIndex, leftIndex;
  if (_vertices[0][1] < _vertices[1][1]) {
    if (_vertices[0][1] < _vertices[2][1]) {
      bottomIndex = 0;
      rightIndex = 1;
      leftIndex = 2;
    }
    else {
      bottomIndex = 2;
      rightIndex = 0;
      leftIndex = 1;
    }
  }
  else {
    if (_vertices[1][1] < _vertices[2][1]) {
      bottomIndex = 1;
      rightIndex = 2;
      leftIndex = 0;
    }
    else {
      bottomIndex = 2;
      rightIndex = 0;
      leftIndex = 1;
    }
  }
  const Point& bottomVertex = _vertices[bottomIndex];
  const Point& rightVertex = _vertices[rightIndex];
  const Point& leftVertex = _vertices[leftIndex];

  //
  // The scan conversion proceeds in two stages.
  // Do the first stage.
  //

  // Get the starting row.
  int row = bottomVertex[1] > 0 ? int(bottomVertex[1]) + 1 : 0;
  // Get the ending row.
  int topRow = std::min(int(std::floor(std::min(rightVertex[1],
						leftVertex[1]))), 
			extents[1] - 1);

  Number leftDxDy, leftIntersection;
  Number rightDxDy, rightIntersection;

  // Find the intersection of the left edge with the first row.
  Number dy = leftVertex[1] - bottomVertex[1];
  if (dy > 1e-5) {
    leftDxDy = (leftVertex[0] - bottomVertex[0]) / dy;
    leftIntersection = (bottomVertex[0] 
			+ (row - bottomVertex[1]) * leftDxDy);
  }
  else {
    leftDxDy = 0;
    leftIntersection = std::min(bottomVertex[0], leftVertex[0]); 
  }

  // Find the intersection of the right edge with the first row.
  dy = rightVertex[1] - bottomVertex[1];
  if (dy > 1e-5) {
    rightDxDy = (rightVertex[0] - bottomVertex[0]) / dy;
    rightIntersection = (bottomVertex[0] 
			 + (row - bottomVertex[1]) * rightDxDy);
  }
  else {
    rightDxDy = 0;
    rightIntersection = std::min(bottomVertex[0], rightVertex[0]); 
  }
  
  // Loop until all rows in the first stage have been scanned.
  while(row <= topRow) {  
    // Scan convert the row.
    const int end = 
      std::min(rightIntersection > 0 ? int(rightIntersection) : -1,
	       extents[0] - 1);
    for (int col = leftIntersection > 0 ? int(leftIntersection) + 1 : 0;
	 col <= end; ++col) {
      multiIndex[0] = col;
      multiIndex[1] = row;
      *coordinates++ = multiIndex;
    }

    // Increment the row.
    ++row;  
    // Adjust the left and right intersections.
    leftIntersection += leftDxDy;
    rightIntersection += rightDxDy;
  }

  //
  // Do the second stage of the scan conversion.
  //

  // Get the ending row.
  topRow = std::min(int(std::floor(std::max(rightVertex[1], leftVertex[1]))), 
		    extents[1] - 1);

  // If this row passes through the triangle.
  if (row <= topRow) {
    
    if (leftVertex[1] < rightVertex[1]) {
      // Find the intersection of the left edge with this row.
      Number dy = rightVertex[1] - leftVertex[1];
      if (dy > 1e-5) {
	leftDxDy = (rightVertex[0] - leftVertex[0]) / dy;
	leftIntersection = (leftVertex[0] 
			    + (row - leftVertex[1]) * leftDxDy);
      }
      else {
	leftDxDy = 0;
	leftIntersection = std::min(leftVertex[0], rightVertex[0]); 
      }
    }
    else {
      // Find the intersection of the right edge with this row.
      Number dy = leftVertex[1] - rightVertex[1];
      if (dy > 1e-5) {
	rightDxDy = (leftVertex[0] - rightVertex[0]) / dy;
	rightIntersection = (rightVertex[0] 
			     + (row - rightVertex[1]) * rightDxDy);
      }
      else {
	rightDxDy = 0;
	rightIntersection = std::min(rightVertex[0], leftVertex[0]); 
      }
    }
    // Loop until all rows in the second stage have been scanned.
    while(row <= topRow) {  
      // Scan convert the row.
      const int end = 
	std::min(rightIntersection > 0 ? int(rightIntersection) : -1,
		 extents[0] - 1);
      for (int col = leftIntersection > 0 ? int(leftIntersection) + 1 : 0;
	 col <= end; ++col) {
	multiIndex[0] = col;
	multiIndex[1] = row;
	*coordinates++ = multiIndex;
      }

      // Increment the row.
      ++row;  
      // Adjust the left and right intersections.
      if (row <= topRow) {
	leftIntersection += leftDxDy;
	rightIntersection += rightDxDy;
      }
    }
  }
}


template<typename T>
template<typename IndexOutputIterator, int N>
inline
void 
ScanConversionPolygon<T>::
scanConvert(IndexOutputIterator coordinates,
	    const ads::FixedArray<N,int>& extents,
	    ads::FixedArray<N,int> multiIndex) const {
  // If the polygon is degenerate, do nothing.
  if (_vertices.size() < 3) {
    return;
  }

  // Special case of a triangle.
  if (_vertices.size() == 3) {
    scanConvertTriangle(coordinates, extents, multiIndex);
    return;
  }

  Number bottomY, topY;
  // Get bottom vertex.
  int bottom = computeBottomAndTop(&bottomY, &topY);  

  // Get the starting row.
  int row = bottomY > 0 ? int(bottomY) + 1 : 0;
  // Get the ending row.
  int topRow;
  if (topY < 0) {
    topRow = -1;
  }
  else{
    topRow = std::min(int(topY), extents[1] - 1);
  }
  // The indices that track the left and right Line segments.
  CyclicIndex 
    leftBottom(int(_vertices.size())), 
    leftTop(int(_vertices.size())), 
    rightBottom(int(_vertices.size())), 
    rightTop(int(_vertices.size()));
  leftBottom.set(bottom);
  rightBottom.set(bottom);
  leftTop.set(bottom);
  --leftTop;
  rightTop.set(bottom);
  ++rightTop;
  bool newLeftEdge = true;
  bool newRightEdge = true;

  Number leftDxDy = 0, 
    leftIntersection = 0, 
    rightDxDy = 0, 
    rightIntersection = 0;

  while(row <= topRow) {  // loop until all rows have been scanned.

    // CONTINUE
#if 0
    std::cerr << "row = " << row << " topRow = " << topRow << "\n";
    if (row == 0 && topRow ==0) {
      put(std::cerr);
    }
#endif

    // Get the left edge for this row
    // Loop until we get an edge that crosses the row.  Skip horizontal edges.
    while (_vertices[leftTop()][1] < row || 
	   _vertices[leftTop()][1] <= _vertices[leftBottom()][1]) {
      --leftTop;
      --leftBottom;
      newLeftEdge = true;
    }

    //std::cerr << "Get the right edge for this row.\n";
    // Get the right edge for this row
    while (_vertices[rightTop()][1] < row || 
	   _vertices[rightTop()][1] <= _vertices[rightBottom()][1]) {
      ++rightTop;
      ++rightBottom;
      newRightEdge = true;
    }

    // Find the intersection of the left edge with this row
    if (newLeftEdge) {
      Point p = _vertices[leftBottom()]; 
      Point q = _vertices[leftTop()];
      Number dy = q[1] - p[1];
      if (dy > 1e-5) {
	leftDxDy = (q[0] - p[0]) / dy;
	leftIntersection = p[0] + (row - p[1]) * leftDxDy;
      }
      else {
	leftDxDy = 0;
	leftIntersection = std::min(p[0], q[0]); 
      }
      newLeftEdge = false;
    }
    else {
      leftIntersection += leftDxDy;
    }

    // Find the intersection of the right edge with this row
    if (newRightEdge) {
      Point p = _vertices[rightBottom()]; 
      Point q = _vertices[rightTop()];
      Number dy = q[1] - p[1];
      if (dy > 1.0e-5) {
	rightDxDy = (q[0] - p[0]) / dy;
	rightIntersection = p[0] + (row - p[1]) * rightDxDy;
      }
      else {
	rightDxDy = 0;
	rightIntersection = std::max(p[0], q[0]);
      }
      newRightEdge = false;
    }
    else {
      rightIntersection += rightDxDy;
    }

    //std::cerr << "Scan convert the row.\n";
    // Scan convert the row.
    const int end = 
      std::min(rightIntersection > 0 ? int(rightIntersection) : -1,
	       extents[0] - 1);
    for (int col = leftIntersection > 0 ? int(leftIntersection) + 1 : 0;
	 col <= end; ++col) {
      multiIndex[0] = col;
      multiIndex[1] = row;
      *coordinates++ = multiIndex;
    }
	
    // Increment the row.
    ++row;  
  }
}


template<typename T>
inline
void 
ScanConversionPolygon<T>::
clip(const Line_2<Number>& line) {
  // Initially indicate that there are no points above or below the Line.
  int above = -1, below = -1;

  // Calculate the distance of the vertices from the line.
  std::vector<Number> dist;
  dist.reserve(_vertices.size());
  const int verticesSize = int(_vertices.size());
  for (int i = 0; i < verticesSize; i++) {
    dist[i] = line.computeSignedDistance(_vertices[i]);
  }

  // Try to find a vertex above and below the Line.
  for (int i = 0; i < verticesSize 
	 && (above == -1 || below == -1); i++) {
    if (above == -1 && dist[i] > 0) {
      above = i;
    }
    if (below == -1 && dist[i] < 0) {
      below = i;
    }
  }

  // If there are no points below the line do nothing.
  if (below == -1) {
    return;
  }
    
  // If there are no points above the line the polygon is annihilated
  if (above == -1) {
    _vertices.clear();
    return;
  }

  // There are points above and below the line.  Find the points of 
  // transition and clip the polygon.
  CyclicIndex left(int(_vertices.size()));
  CyclicIndex right(int(_vertices.size()));
  left.set(above);
  right.set(above);

  // Find the transition on one side.
  for (++left; dist[left()] > 0; ++left)
    ;
  int leftBelow = left();
  --left;
  int leftAbove = left();

  // Find the point of intersection.
  assert(dist[leftAbove] > 0 && dist[leftBelow] <= 0);
  Point leftInt;
  line.computeIntersection(_vertices[leftBelow], _vertices[leftAbove], 
			   &leftInt);

  // Find the transition on the other side.
  for (--right; dist[right()] > 0; --right)
    ;
  int rightBelow = right();
  ++right;
  int rightAbove = right();

  // Find the point of intersection.
  assert(dist[rightAbove] > 0.0 && dist[rightBelow] <= 0.0);
  Point rightInt;
  line.computeIntersection(_vertices[rightBelow], _vertices[rightAbove], 
			   &rightInt);

  //
  // Make the new polygon.
  //

  // Copy the old vertices.
  Container oldVertices(_vertices);  
  // Erase the vertices of this polygon.
  _vertices.clear();  
  // Add the vertices above the line.
  for (right.set(rightAbove); right() != leftBelow; ++right) {
    _vertices.push_back(oldVertices[right()]);
  }
  // Add the intersection points.
  if (oldVertices[leftAbove] != leftInt && 
       oldVertices[rightAbove] != leftInt) {
    _vertices.push_back(leftInt);
  }
  if (oldVertices[leftAbove] != rightInt && 
       oldVertices[rightAbove] != rightInt &&
       leftInt != rightInt) {
    _vertices.push_back(rightInt);
  }
}


template<typename T>
inline
bool 
ScanConversionPolygon<T>::
isValid() const {
  const int size = _vertices.size();
  if (size < 3) {
    return false;
  }
  for (int i = 0; i < size; i++) {
    if (_vertices[i] == _vertices[(i+1) % size]) {
      return false;
    }
  }
  return true;
}


//
// Equality
//

    
template<typename T>
inline
bool 
operator==(const ScanConversionPolygon<T>& a, 
	   const ScanConversionPolygon<T>& b) {
  // Check that a and b have the same number of vertices.
  if (a.getVerticesSize() != b.getVerticesSize()) {
    return false;
  }

  // Check each vertex.
  int size = a.getVerticesSize();
  for (int i = 0; i < size; ++i) {
    if (a.getVertex(i) != b.getVertex(i)) {
      return false;
    }
  }
  return true;
}


//
// File I/O
//


template<typename T>
inline
void
ScanConversionPolygon<T>::
get(std::istream& in) {
  // Clear the vertices.
  _vertices.clear();
  // Get the number of vertices.
  int nv;
  in >> nv;
  // Get the vertices.
  Point p;
  for (; nv > 0; --nv) {
    in >> p;
    _vertices.push_back(p);
  }
}


template<typename T>
inline
void
ScanConversionPolygon<T>::
put(std::ostream& out) const {
  ConstIterator iter;
  for (iter = _vertices.begin(); iter != _vertices.end(); ++iter)
    out << *iter << '\n';
}


template<typename T>
inline
void
ScanConversionPolygon<T>::
mathematicaPrint(std::ostream& out) const {
  out << "Line[{";
  ConstIterator iter;
  for (iter = _vertices.begin(); iter != _vertices.end(); ++iter) {
    out << "{" << (*iter)[0] << "," << (*iter)[1] << "},";
  }
  iter = _vertices.begin();
  out << "{" << (*iter)[0] << "," << (*iter)[1] << "}}]," 
      << '\n';
}


END_NAMESPACE_GEOM

// End of file.
