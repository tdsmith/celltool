// -*- C++ -*-

/*! 
  \file ISS_SignedDistance.h
  \brief Signed distance to a simplicial mesh.
*/

#if !defined(__geom_ISS_SignedDistance_h__)
#define __geom_ISS_SignedDistance_h__

#include "SimplexAdj.h"
#include "ISS_SimplexQuery.h"

#include "../simplex/simplex_distance.h"

#include "../../tree/BBoxTree.h"

#include "../../../ads/algorithm/sign.h"

#include <set>
#include <map>

#if defined(DEBUG_geom) && !defined(DEBUG_ISS_SignedDistance)
#define DEBUG_ISS_SignedDistance
#endif

BEGIN_NAMESPACE_GEOM

//! Signed distance to a triangle faceted surface in 3-D.
/*!
  \param ISS is the indexed simplex set.

  This class stores a constant reference to an indexed simplex set.
*/
template <class ISS, int N = ISS::N>
class ISS_SignedDistance;

END_NAMESPACE_GEOM

#define __geom_ISS_SignedDistance_2_ipp__
#include "ISS_SignedDistance_2.ipp"
#undef __geom_ISS_SignedDistance_2_ipp__

#define __geom_ISS_SignedDistance_3_ipp__
#include "ISS_SignedDistance_3.ipp"
#undef __geom_ISS_SignedDistance_3_ipp__

BEGIN_NAMESPACE_GEOM


//! Functor that returns the distance to a mesh.
/*!
  If M = N, the signed distance is used.  If M < N, the we use the unsigned
  distance.
*/
template<class ISS>
class ISS_Distance :
  public std::unary_function<typename ISS_SimplexQuery<ISS>::Vertex,
			     typename ISS_SimplexQuery<ISS>::Number> {
  // Types.
  typedef std::unary_function<typename ISS_SimplexQuery<ISS>::Vertex,
			      typename ISS_SimplexQuery<ISS>::Number> Base;

  // Data.
  const ISS_SimplexQuery<ISS>& _sq;

  // Not implemented.
  ISS_Distance();
  ISS_Distance& operator=(const ISS_Distance&);

public:
  //! The argument type is a vertex.
  typedef typename Base::argument_type argument_type;
  //! The result type is a number.
  typedef typename Base::result_type result_type;

  //! Make from a simplex query data structure.
  explicit
  ISS_Distance(const ISS_SimplexQuery<ISS>& sq) :
    _sq(sq)
  {}

  //! Copy constructor.
  ISS_Distance(const ISS_Distance& other) :
    _sq(other._sq)
  {}

  //! Compute the distance to the mesh.
  result_type
  operator()(const argument_type& x) const {
    return _sq.computeMinimumDistance(x);
  }
};


// CONTINUE: Fix the argument type. (as above)

//! Functor that returns the signed distance to an (N-1)-D mesh in N-D space.
template<class ISS>
class ISS_SD_Distance :
  public std::unary_function<const typename 
ISS_SignedDistance<ISS>::Vertex&,
			     typename ISS_SignedDistance<ISS>::Number>
{
  // Types.
  typedef std::unary_function<const typename 
			      ISS_SignedDistance<ISS>::Vertex&,
			      typename ISS_SignedDistance<ISS>::Number> Base;

  // Data.
  const ISS_SignedDistance<ISS>& _sd;

  // Not implemented.
  ISS_SD_Distance();
  ISS_SD_Distance& operator=(const ISS_SD_Distance&);

public:
  //! The argument type is a vertex.
  typedef typename Base::argument_type argument_type;
  //! The result type is a number.
  typedef typename Base::result_type result_type;
  
  //! Construct from the signed distance data structure.
  ISS_SD_Distance(const ISS_SignedDistance<ISS>& sd) :
    _sd(sd)
  {}

  //! Copy constructor.
  ISS_SD_Distance(const ISS_SD_Distance& other) :
    _sd(other._sd)
  {}

  //! Compute the signed distance.
  result_type
  operator()(argument_type x) const {
    return _sd(x);
  }
};



//! Functor that returns the closest point to an (N-1)-D mesh in N-D space.
template<class ISS>
class ISS_SD_ClosestPoint :
  public std::unary_function<const typename 
ISS_SignedDistance<ISS>::Vertex&,
			     const typename 
ISS_SignedDistance<ISS>::Vertex&> {
  // Types.
  typedef std::unary_function<const typename 
			      ISS_SignedDistance<ISS>::Vertex&,
			      const typename 
			      ISS_SignedDistance<ISS>::Vertex&> Base;

  // Data.
  const ISS_SignedDistance<ISS>& _sd;

  // Not implemented.
  ISS_SD_ClosestPoint();
  ISS_SD_ClosestPoint& operator=(const ISS_SD_ClosestPoint&);

public:
  //! The argument type is a point.
  typedef typename Base::argument_type argument_type;
  //! The result type is a point.
  typedef typename Base::result_type result_type;
  
  //! Construct from the signed distance data structure.
  ISS_SD_ClosestPoint(const ISS_SignedDistance<ISS>& sd) :
    _sd(sd)
  {}

  //! Copy constructor.
  ISS_SD_ClosestPoint(const ISS_SD_ClosestPoint& other) :
    _sd(other._sd)
  {}

  //! Compute the closest point.
  result_type
  operator()(argument_type x) const {
    return _sd.computeClosestPoint(x);
  }
};



//! Functor that returns the closest point along a specified direction to an (N-1)-D mesh in N-D space.
template<class ISS>
class ISS_SD_ClosestPointDirection :
  public std::unary_function<const typename 
			     ISS_SignedDistance<ISS>::Vertex&,
			     const typename 
			     ISS_SignedDistance<ISS>::Vertex&> {
  // Types.
  typedef std::unary_function<const typename 
			      ISS_SignedDistance<ISS>::Vertex&,
			      const typename 
			      ISS_SignedDistance<ISS>::Vertex&> Base;

  typedef typename ISS_SignedDistance<ISS>::Vertex Vertex;

public:
  //! The argument type is a point.
  typedef typename Base::argument_type argument_type;
  //! The result type is a point.
  typedef typename Base::result_type result_type;
  //! The number type.
  typedef typename ISS_SignedDistance<ISS>::Number Number;

private:

  // Data.
  const ISS_SignedDistance<ISS>& _sd;
  int _maxIterations;
  Number _tolerance;
  mutable Vertex _p, _q, _offset;

  // Not implemented.
  ISS_SD_ClosestPointDirection();
  ISS_SD_ClosestPointDirection& 
  operator=(const ISS_SD_ClosestPointDirection&);

public:
  
  //! Construct from the signed distance data structure.
  ISS_SD_ClosestPointDirection
  (const ISS_SignedDistance<ISS>& sd,
    const int max_iterations = 5,
    const Number tolerance = 
    std::sqrt(std::numeric_limits<Number>::epsilon())) :
    _sd(sd),
    _maxIterations(max_iterations),
    _tolerance(tolerance)
  {}

  //! Copy constructor.
  ISS_SD_ClosestPointDirection(const ISS_SD_ClosestPointDirection& other) :
    _sd(other._sd),
    _maxIterations(other._maxIterations),
    _tolerance(other._tolerance)
  {}

  //! Return the closest point along the direction.
  /*!
    \param x The position.
    \param dir The direction (normalized).
   */
  result_type
  operator()(argument_type x, argument_type dir) const {
    _p = x;
    Number distance = _tolerance;
    for (int iter = 0; iter != _maxIterations && distance >= _tolerance; 
	  ++iter) {
      // Compute the distance and closest point.
      distance = std::abs(_sd(_p, &_q));
      // The vector from the current point to the closest point.
      _q -= _p;
      _offset = dir;
      _offset *= ads::sign(geom::computeDotProduct(_q, dir)) * 
	geom::computeMagnitude(_q);
      _p += _offset;
    }
    return _sd.computeClosestPoint(_p);
  }
};





//! Functor that returns the closest point along a specified direction to an (N-1)-D mesh in N-D space.
template<class ISS>
class ISS_SD_CloserPointDirection :
  public std::unary_function<const typename 
			     ISS_SignedDistance<ISS>::Vertex&,
			     const typename 
			     ISS_SignedDistance<ISS>::Vertex&> {
  // Types.
  typedef std::unary_function<const typename 
			      ISS_SignedDistance<ISS>::Vertex&,
			      const typename 
			      ISS_SignedDistance<ISS>::Vertex&> Base;

  typedef typename ISS_SignedDistance<ISS>::Vertex Vertex;

public:
  //! The argument type is a point.
  typedef typename Base::argument_type argument_type;
  //! The result type is a point.
  typedef typename Base::result_type result_type;
  //! The number type.
  typedef typename ISS_SignedDistance<ISS>::Number Number;

private:

  // Data.
  const ISS_SignedDistance<ISS>& _sd;
  Number _max_distance;
  int _maxIterations;
  Number _tolerance;
  mutable Vertex _p, _q, _offset;

  // Not implemented.
  ISS_SD_CloserPointDirection();
  ISS_SD_CloserPointDirection& 
  operator=(const ISS_SD_CloserPointDirection&);

public:

  //! Construct from the signed distance data structure.  
  ISS_SD_CloserPointDirection
  (const ISS_SignedDistance<ISS>& sd,
    const Number max_distance,
    const int max_iterations = 5,
    const Number tolerance = 
    std::sqrt(std::numeric_limits<Number>::epsilon())) :
    _sd(sd),
    _max_distance(max_distance),
    _maxIterations(max_iterations),
    _tolerance(tolerance)
  {}

  //! Copy constructor.
  ISS_SD_CloserPointDirection(const ISS_SD_CloserPointDirection& other) :
    _sd(other._sd),
    _max_distance(other._max_distance),
    _maxIterations(other._maxIterations),
    _tolerance(other._tolerance)
  {}

  //! Return a closer point along the direction.
  /*!
    \param x The position.
    \param dir The direction (normalized).
   */
  result_type
  operator()(argument_type x, argument_type dir) const {
    //
    // Compute the closest point on the manifold in the given direction.
    //
    _p = x;
    Number distance = _tolerance;
    for (int iter = 0; iter != _maxIterations && distance >= _tolerance; 
	  ++iter) {
      // Compute the distance and closest point.
      distance = std::abs(_sd(_p, &_q));
      // The vector from the current point to the closest point.
      _q -= _p;
      _offset = dir;
      _offset *= ads::sign(geom::computeDotProduct(_q, dir)) * 
	geom::computeMagnitude(_q);
      _p += _offset;
    }
    _p = _sd.computeClosestPoint(_p);

    //
    // Determine the closer point.
    //

    distance = geom::computeDistance(x, _p);
    // If the distance is more than the maximum allowed.
    if (distance > _max_distance) {
      // The vector between x and the closest point.
      _p -= x;
      // Make the vector have length _max_distance.
      _p *= (_max_distance / distance);
      // The closer point.
      _p += x;
    }

    // Return the closest point or the closer point.
    return _p;
  }
};





//! Functor that returns a closer point to an (N-1)-D mesh in N-D space.
template<class ISS>
class ISS_SD_CloserPoint :
  public std::unary_function<const typename 
			     ISS_SignedDistance<ISS>::Vertex&,
			     const typename 
			     ISS_SignedDistance<ISS>::Vertex&> {
  // Types.
  typedef typename ISS_SignedDistance<ISS>::Vertex Vertex;
  typedef typename ISS_SignedDistance<ISS>::Number Number;
  typedef std::unary_function<const Vertex&, const Vertex&> Base;

  // Data.
  const ISS_SignedDistance<ISS>& _sd;
  Number _maxDistance;
  mutable Vertex _cp;

  // Not implemented.
  ISS_SD_CloserPoint();
  ISS_SD_CloserPoint& operator=(const ISS_SD_CloserPoint&);

public:
  //! The argument type is a point.
  typedef typename Base::argument_type argument_type;
  //! The result type is a point.
  typedef typename Base::result_type result_type;

  //! Construct from the signed distance data structure.  
  ISS_SD_CloserPoint(const ISS_SignedDistance<ISS>& sd, 
		     const Number max_distance) :
    _sd(sd),
    _maxDistance(max_distance)
  {}

  //! Copy constructor.
  ISS_SD_CloserPoint(const ISS_SD_CloserPoint& other) :
    _sd(other._sd),
    _maxDistance(other._maxDistance)
  {}

  //! Return a closer point.
  result_type
  operator()(argument_type x) const {
    // Compute the distance and closest point.
    Number d = _sd(x, &_cp);

    // If the distance is more than the maximum allowed.
    if (d > _maxDistance) {
      // The vector between x and the closest point.
      _cp -= x;
      // Make the vector have length _maxDistance.
      _cp *= (_maxDistance / d);
      // The closer point.
      _cp += x;
    }

    // Return the closest point or the closer point.
    return _cp;
  }
};


END_NAMESPACE_GEOM


#endif
