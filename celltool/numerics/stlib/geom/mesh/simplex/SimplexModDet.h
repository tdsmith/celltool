// -*- C++ -*-

/*! 
  \file SimplexModDet.h
  \brief Implements operations for modifying the determinant of the Jacobian.
*/

#if !defined(__geom_SimplexModDet_h__)
#define __geom_SimplexModDet_h__

#include "../../defs.h"

#include <limits>

#include <cmath>
#include <cassert>

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexModDet)
#define DEBUG_SimplexModDet
#endif

BEGIN_NAMESPACE_GEOM

//! Implements operations for modifying the determinant of the Jacobian.
/*!
  \param T is the number type.  By default it is double.

  This class cannot be constructed.  Its member data and public member
  functions are static.

  <b>The Modified Determinant</b>

  See the documentation of the geom::SimplexJac class for information on
  the Jacobian matrix of a simplex.
  When the content of a simplex vanishes, its Jacobian matrix becomes
  singular.  That is, its determinant vanishes.  This presents a problem 
  for some algebraic quality metrics.  The condition number metric and the
  mean ratio metric (implemented in geom::SimplexCondNum and
  geom::SimplexMeanRatio) become singular as the content vanishes.
  In order to define these metrics for simplices with vanishing and
  negative content, we use a modified value \f$ h \f$ of the Jacobian
  determinant \f$ \sigma \f$ which has the following properties:
  - \f$ h \f$ is close to \f$ \sigma \f$ when
  \f$ \sigma \f$ is positive and \f$ \sigma \gg 1 \f$.  
  - \f$ h \f$ is small and positive when the determinant is negative.
  - \f$ h \f$ is differentiable.

  Let \f$ \epsilon \f$ be a number that is a little bigger than the 
  machine precision.  (We use 100 times the machine precision.)
  Define
  \f[ 
  \delta = \sqrt{ \epsilon (\epsilon - \sigma) }. 
  \f]
  The modified value of the determinant is
  \f[ 
  h = \frac{ \sigma + \sqrt{ \sigma^2 + 4 \delta^2 } }{ 2 }. 
  \f]

  <b>Usage</b>

  Consider a complex of simplices.  (Perhaps the tetrahedra that are 
  adjacent to a vertex.)  Let \c minDeterminant be the minimum determinant
  of the simplices in the complex and \c determinant be the determinant
  of a given simplex.  
  \c h(determinant,minDeterminant) returns the modified determinant.
  If the minimum determinant is no less than \f$ \epsilon \f$ then
  \c h() returns the un-modified determinant.
*/
template<typename T = double>
class SimplexModDet {
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

private:

  //
  // Static member data.
  //

  // A little bigger than the machine epsilon.
  static Number _epsilon;

  //
  // Not implemented.
  //

  // Default constructor.
  SimplexModDet();

  // Copy constructor.
  SimplexModDet(const SimplexModDet&);

  // Assignment operator.
  SimplexModDet& 
  operator=(const SimplexModDet&);
  
  // Destructor.
  ~SimplexModDet();

public:

  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Return epsilon.
  static
  Number
  getEpsilon() {
    return _epsilon;
  }

  //! Return delta.
  static
  Number
  getDelta(const Number minDeterminant) {
    if (minDeterminant < _epsilon) {
      const Number argument = _epsilon * (_epsilon - minDeterminant);
#ifdef DEBUG_geom
      assert(argument >= 0);
#endif
      return std::sqrt(argument);
    }
    return 0.0;
  }

  //! Return a number that is close to the determinant when it is positive and small and positive when the determinant is negative.
  /*!
    \return
    Let \f$ \epsilon \f$ be the value of epsilon() and
    \f$ \sigma \f$ be the Jacobian determinant.
    Define 
    \f[ \delta = \sqrt{ \epsilon (\epsilon - \sigma) }. \f]
    Return
    \f[ \frac{ \sigma + \sqrt{ \sigma^2 + 4 \delta^2 } }{ 2 }. \f]
  */
  static
  Number
  getH(Number determinant, Number minDeterminant);

  //! @}
};

END_NAMESPACE_GEOM

#define __geom_SimplexModDet_ipp__
#include "SimplexModDet.ipp"
#undef __geom_SimplexModDet_ipp__

#endif
