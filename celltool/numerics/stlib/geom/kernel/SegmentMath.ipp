// -*- C++ -*-

#if !defined(__geom_SegmentMath_ipp__)
#error This file is an implementation detail of the class SegmentMath.
#endif

BEGIN_NAMESPACE_GEOM

//
// Constructors
//
  

template<int N, typename T>
inline
SegmentMath<N,T>::
SegmentMath(const Point& source, const Point& target) {
  make(source, target);
}


template<int N, typename T>
inline
SegmentMath<N,T>& 
SegmentMath<N,T>::
operator=(const SegmentMath& other) {
  // Avoid assignment to self
  if (&other != this) {
    // Copy member data.
    Base::operator=(other);
    _tangent = other._tangent;
    _length = other._length;
  }
  // Return *this so assignments can chain
  return *this;
}


template<int N, typename T>
inline
void
SegmentMath<N,T>::
make(const Point& source, const Point& target) {
  Segment<N,T>::make(source, target);
  _tangent = target; 
  _tangent -= source;
  _length = computeMagnitude(_tangent);
  if (_length != 0)
    _tangent /= _length;
  else
    _tangent[0] = 1;
}


//
// Arithmetic member operators
//


template<int N, typename T>
inline
SegmentMath<N,T>& 
SegmentMath<N,T>::
operator+=(const Point& p) {
  Segment<N,T>::operator +=(p);
  return (*this);
}


template<int N, typename T>
inline
SegmentMath<N,T>& 
SegmentMath<N,T>::
operator-=(const Point& p) {
  Segment<N,T>::operator -=(p);
  return (*this);
}


//
// Binary free Operators
//


// Addition
template<int N, typename T>
inline
SegmentMath<N,T>
operator+(const SegmentMath<N,T>& s, 
	  const typename SegmentMath<N,T>::Point& p) {
  SegmentMath<N,T> seg(s);
  seg += p;
  return seg;
}


// Subtraction
template<int N, typename T>
inline
SegmentMath<N,T>
operator-(const SegmentMath<N,T>& s, 
	  const typename SegmentMath<N,T>::Point& p) {
  SegmentMath<N,T> seg(s);
  seg -= p;
  return seg;
}
  

//
// Mathematical member functions
//


template<int N, typename T>
inline
bool
SegmentMath<N,T>::
isValid() const {
  Point tangent(getTarget() - getSource());
  normalize(&tangent);
  return (geom::computeDistance(getTangent(), tangent) < 
	  10 * std::numeric_limits<Number>::epsilon() &&
	  std::abs(geom::computeDistance(getSource(), getTarget()) - 
		   getLength()) 
	  < 10 * std::numeric_limits<Number>::epsilon());
}



//
// Mathematical Free Functions
//

template<int N, typename T>
inline
T 
computeDistance(const SegmentMath<N,T>& segment, 
		const typename SegmentMath<N,T>::Point& x) {
  typename SegmentMath<N,T>::Point closestPoint;
  return computeDistanceAndClosestPoint(segment, x, &closestPoint);
}


template<int N, typename T>
inline
void
computeClosestPoint(const SegmentMath<N,T>& segment, 
		    const typename SegmentMath<N,T>::Point& x, 
		    typename SegmentMath<N,T>::Point* closestPoint) {
  T proj = computeDotProduct(x - segment.getSource(), segment.getTangent());
  if (proj >= 0 && proj <= segment.getLength()) {
    *closestPoint = segment.getSource() + proj * segment.getTangent();
  }
  else {
    if (proj < 0) {
      *closestPoint = segment.getSource();
    }
    else {
      *closestPoint = segment.getTarget();
    }
  }
}


template<int N, typename T>
inline
T 
computeDistanceAndClosestPoint
(const SegmentMath<N,T>& segment, 
 const typename SegmentMath<N,T>::Point& x, 
 typename SegmentMath<N,T>::Point* closestPoint) {
  // Compute the closest point.
  computeClosestPoint(segment, x, closestPoint);
  // Return the distance.
  return geom::computeDistance(x, *closestPoint);
}


template<int N, typename T>
inline
T 
computeUnsignedDistanceToSupportingLine
(const SegmentMath<N,T>& segment, 
 const typename SegmentMath<N,T>::Point& x) {
  typename SegmentMath<N,T>::Point closestPoint;
  return computeUnsignedDistanceAndClosestPointToSupportingLine
    (segment, x, &closestPoint);
}


template<int N, typename T>
inline
T 
computeUnsignedDistanceAndClosestPointToSupportingLine
(const SegmentMath<N,T>& segment, const typename SegmentMath<N,T>::Point& x, 
 typename SegmentMath<N,T>::Point* closestPoint) {
  const T proj = computeDotProduct(x - segment.getSource(), 
				   segment.getTangent());
  *closestPoint = segment.getSource() + proj * segment.getTangent();
  return geom::computeDistance(x, *closestPoint);
}


template<typename T>
bool 
computeZIntersection(const SegmentMath<3,T>& segment, T* x, T* y, T z) {
  assert(segment.getSource()[2] <= segment.getTarget()[2]);

  // If the segment intersects the z plane.
  if (segment.getSource()[2] <= z && z <= segment.getTarget()[2]) {
    if (segment.getTangent()[2] > 1e-8) {
      const T a = (z - segment.getSource()[2]) / segment.getTangent()[2];
      *x = segment.getSource()[0] + a * segment.getTangent()[0];
      *y = segment.getSource()[1] + a * segment.getTangent()[1];
    }
    else {
      *x = segment.getSource()[0];
      *y = segment.getSource()[1];
    }
    return true;
  }
  return false;
}


template<typename T>
bool
computeIntersection(const SegmentMath<2,T>& s1, const SegmentMath<2,T>& s2,
		    typename SegmentMath<2,T>::Point* intersectionPoint) {
  typedef typename SegmentMath<2,T>::Point Point;
  const Point& p1 = s1.getSource();
  const Point& p2 = s2.getSource();
  const Point& t1 = s1.getTangent();
  const Point& t2 = s2.getTangent();

  const T den = computeDiscriminant(t1, t2);
  // If the segments are parallel.
  if (den == 0) {
    return false;
  }

  const T a = (t2[0] * (p1[1] - p2[1]) - t2[1] * (p1[0] - p2[0])) / den;
  *intersectionPoint = p1 + a * t1;
  const T b = computeDotProduct(*intersectionPoint - p2, t2);

  if (a >= 0 && a <= s1.getLength() && b >= 0 && b <= s2.getLength()) {
    return true;
  }

  return false;
}


//
// File IO
//

  
template<int N, typename T>
inline
std::istream& 
operator>>(std::istream& in, SegmentMath<N,T>& s) {
  typename SegmentMath<N,T>::Point source, target;
  in >> source >> target;
  s = SegmentMath<N,T>(source, target);
  return in;
}


template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const SegmentMath<N,T>& s) {
  return out << s.getSource() << "\n" 
	     << s.getTarget() << "\n"
	     << s.getTangent() << "\n"
	     << s.getLength() << "\n";
}


END_NAMESPACE_GEOM
