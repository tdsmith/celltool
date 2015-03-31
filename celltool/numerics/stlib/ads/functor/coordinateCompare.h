// -*- C++ -*-

/*! 
  \file coordinateCompare.h
  \brief Implements functions and classes for a comparing composite numbers.
*/

#if !defined(__ads_coordinateCompare_h__)
#define __ads_coordinateCompare_h__

#include "../defs.h"

#include <functional>

#include <cassert>

// If we are debugging the whole ads package.
#if defined(DEBUG_ads) && !defined(DEBUG_coordinateCompare)
#define DEBUG_coordinateCompare
#endif

BEGIN_NAMESPACE_ADS
  
//-----------------------------------------------------------------------------
/*! \defgroup functor_coordinateCompare Functor: Coordinate Comparison */
// @{

//! Less than comparison in a specified coordinate.
/*!
  This class is templated on the point type.
*/
template<typename Point>
class LessThanCompareCoordinate :
  public std::binary_function<Point, Point, bool> {
private:

  typedef std::binary_function<Point, Point, bool> Base;

  int _n;

public:

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;
  
  //! Constructor.  By default, the coordinate to compare has an invalid value.
  LessThanCompareCoordinate(const int n = -1) :
    _n(n)
  {}
  
  // We use the default copy constructor, assignment operator, and destructor.

  //! Set the coordinate to compare.
  void
  setCoordinate(const int n) {
    _n = n;
  }

  //! Less than comparison in a specified coordinate.
  bool 
  operator()(const first_argument_type& x, const second_argument_type& y) 
    const {
#ifdef DEBUG_coordinateCompare
    assert(_n >= 0);
#endif
    return x[_n] < y[_n];
  }
};


//! Convenience function for constructing a LessThanCompareCoordinate functor.
/*!
  \param n The index of the coordinate to compare.
 */
template<typename Point>
LessThanCompareCoordinate<Point>
constructLessThanCompareCoordinate(const int n = -1) {
  return LessThanCompareCoordinate<Point>(n);
}


// @}

END_NAMESPACE_ADS

#endif
