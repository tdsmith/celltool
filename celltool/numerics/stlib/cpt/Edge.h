// -*- C++ -*-

/*! 
  \file Edge.h
  \brief Class for an edge on a b-rep.
*/

#if !defined(__cpt_Edge_h__)
#define __cpt_Edge_h__

#include "defs.h"

#include "../ads/array/FixedArray.h"

#include "../geom/grid/RegularGrid.h"
#include "../geom/kernel/BBox.h"
#include "../geom/kernel/SegmentMath.h"
#include "../geom/polytope/IndexedEdgePolyhedron.h"

#include <vector>

#include <cmath>

BEGIN_NAMESPACE_CPT

template<int N, typename T = double>
class Edge;

//! An edge on a b-rep in 3-D.
template<typename T>
class Edge<3,T> {
public:
	
  //
  // Public types.
  //
	
  //! The number type.
  typedef T Number;
	
  //! A Cartesian point.
  typedef ads::FixedArray<3,T> Point;
	
  //! A bounding box.
  typedef geom::BBox<3,Number> BBox;
	
  //! The representation of a regular grid.
  typedef geom::RegularGrid<3,Number> Grid;
	
  //! An indexed edge polyhedron type.
  typedef geom::IndexedEdgePolyhedron<Number> Polyhedron;

private:
		
  //
  // Private types.
  //
		
  //! A line segment in 3 dimensions.
  typedef geom::SegmentMath<3,Number> Segment;
	
private:
		
  //
  // Data
  //
		
  //! The line segment that makes up the edge.
  Segment _segment;
	
  //! The normals to the neighboring faces.
  Point _leftFaceNormal, _rightFaceNormal;
	
  //! The normals to the lateral sides of the wedge.
  /*! The points that are closest to the edge lie in a wedge. */
  Point _leftSideNormal, _rightSideNormal;
	
  //! The index of an adjacent face.
  int _adjacentFaceIndex;
	
  //! The sign of the distance.
  int _signOfDistance;
	
  //! An epsilon that is appropriate for the edge length.
  Number _epsilon;
	
public:
		
  //-------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{
		
  //! Default constructor.  Unititialized memory.
  Edge() :
    _segment(),
    _leftFaceNormal(),
    _rightFaceNormal(),
    _leftSideNormal(),
    _rightSideNormal(),
    _adjacentFaceIndex(),
    _signOfDistance(),
    _epsilon()
  {}
	
  //! Construct from points and neighboring face normals.
  Edge(const Point& source, const Point& target, 
       const Point& leftNormal, const Point& rightNormal, 
       const int adjacentFaceIndex);
	
  //! Make from points and neighboring face normals.
  void
  make(const Point& source, const Point& target, 
       const Point& leftNormal, const Point& rightNormal, 
       const int adjacentFaceIndex);
	
  //! Copy constructor.
  Edge(const Edge& other);
	
  //! Assignment operator.
  Edge& 
  operator=(const Edge& other);
	
  //! Trivial destructor.
  ~Edge()
  {}
	
  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{
	
  const Point& 
  getSource() const { 
    return _segment.getSource(); 
  }
	
  const Point& 
  getTarget() const { 
    return _segment.getTarget(); 
  }
	
  const Point& 
  getTangent() const { 
    return _segment.getTangent(); 
  }
	
  Number
  getLength() const { 
    return _segment.getLength(); 
  }
	
  const Point& 
  getLeftFaceNormal() const { 
    return _leftFaceNormal; 
  }
	
  const Point& 
  getRightFaceNormal() const { 
    return _rightFaceNormal; 
  }
	
  const Point& 
  getLeftSideNormal() const { 
    return _leftSideNormal; 
  }
	
  const Point& 
  getRightSideNormal() const { 
    return _rightSideNormal; 
  }
	
  int 
  getFaceIndex() const { 
    return _adjacentFaceIndex; 
  }
	
  int 
  getSignOfDistance() const
  { 
    return _signOfDistance; 
  }
	
  //@}
  //-------------------------------------------------------------------------
  //! \name Mathematical operations.
  //@{
	
  //! Return true if the edge is valid.
  bool 
  isValid() const;
	
  //! Return the distance to the supporting line of the edge.
  Number 
  computeDistance(const Point& p) const;
	
  //! Compute distance with checking that the point is in the wedge.
  Number
  computeDistanceChecked(const Point& p) const;
	
  //! Return the unsigned distance to the supporting line of the edge.
  Number 
  computeDistanceUnsigned(const Point& p) const;
	
  //! Return the unsigned distance to the supporting line of the edge.
  Number 
  computeDistanceUnsignedChecked(const Point& p) const;
	
	
	
  //! Return the distance and find the closest point.
  Number 
  computeClosestPoint(const Point& p, Point* cp) const;
	
  //! Return the distance and find the closest point.
  Number 
  computeClosestPointChecked(const Point& p, Point* cp) const;
	
  //! Return the unsigned distance and find the closest point.
  Number 
  computeClosestPointUnsigned(const Point& p, Point* cp) const;
	
  //! Return the unsigned distance and find the closest point.
  Number 
  computeClosestPointUnsignedChecked(const Point& p, Point* cp) const;
	
	
	
  //! Return the distance and find the gradient of the distance.
  Number 
  computeGradient(const Point& p, Point* grad) const;
	
  //! Return the distance and find the gradient of the distance.
  Number 
  computeGradientChecked(const Point& p, Point* grad) const;
	
  //! Return the unsigned distance and find the gradient of this distance.
  Number 
  computeGradientUnsigned(const Point& p, Point* grad) const;
	
  //! Return the unsigned distance and find the gradient of this distance.
  Number 
  computeGradientUnsignedChecked(const Point& p, Point* grad) const;
	
	
	
  //! Return the distance and find the closest point and gradient of distance
  Number 
  computeClosestPointAndGradient(const Point& p, Point* cp, 
				 Point* grad) const;
	
  //! Return the distance and find the closest point and gradient of distance
  Number 
  computeClosestPointAndGradientChecked(const Point& p, Point* cp, 
					Point* grad) const;
	
  //! Return the distance and find the closest point and gradient of distance
  Number 
  computeClosestPointAndGradientUnsigned(const Point& p, Point* cp, 
					 Point* grad) const;
	
  //! Return the distance and find the closest point and gradient of distance
  Number 
  computeClosestPointAndGradientUnsignedChecked(const Point& p, Point* cp,
						Point* grad) const;
	
	
  //! Make the polyhedron on the grid that contains the closest points for signed distance.
  void 
  buildCharacteristicPolyhedron(Polyhedron* polyhedron, 
				Number height) const;
	
  //! Make the polyhedron on the grid that contains the closest points for unsigned distance.
  void 
  buildCharacteristicPolyhedronUnsigned(Polyhedron* polyhedron, 
					Number height) const;
	
  //@}
	
private:
		
  //! Return true if the point is within delta of being inside.
  bool 
  isInside(const Point& pt, Number delta) const;
	
  //! Return true if the point is (close to being) inside the wedge of closest points
  bool 
  isInside(const Point& p) const { 
    return isInside(p, _epsilon);
  }
	
	
};

//! Equality operator
/*! \relates Edge<3,T> */
template<typename T>
bool 
operator==(const Edge<3,T>& a, const Edge<3,T>& b);

//! Inequality operator
/*! \relates Edge<3,T> */
template<typename T>
bool 
operator!=(const Edge<3,T>& a, const Edge<3,T>& b);

//! File output.
/*! \relates Edge<3,T> */
template<typename T>
std::ostream& 
operator<<(std::ostream& out, const Edge<3,T>& edge);

END_NAMESPACE_CPT

#define __cpt_Edge_ipp__
#include "Edge.ipp"
#undef __cpt_Edge_ipp__

#endif
