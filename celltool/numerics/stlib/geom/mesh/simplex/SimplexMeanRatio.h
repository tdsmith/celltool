// -*- C++ -*-

/*! 
  \file SimplexMeanRatio.h
  \brief Implements the mean ratio quality metric.
*/

#if !defined(__geom_SimplexMeanRatio_h__)
#define __geom_SimplexMeanRatio_h__

#include "SimplexJacQF.h"

#include <limits>

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexMeanRatio)
#define DEBUG_SimplexMeanRatio
#endif

BEGIN_NAMESPACE_GEOM

//! Implements the mean ratio quality metric.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This class implements the mean ratio quality metric.
  Let \f$ S \f$ be the Jacobian matrix, \f$ \sigma \f$ be the Jacobian
  determinant and \f$ | \cdot | \f$ be the Frobenius norm.  The 
  \c operator() member function returns the
  mean ratio quality metric:
  \f[ 
  \eta = \frac{ |S|^2 }{ N \sigma^{2/N} }. 
  \f]
  This quality metric is only defined for simplices with positive content.
  (The Jacobian determinant must be positive.)
  
  \c computeGradient() calculates the gradient of the mean ratio metric.

  Before evaluating the mean ratio metric, you must set the 
  Jacobian matrix with \c setFunction() or \c set().
  Before evaluating the gradient of the metric, you must 
  set the Jacobian matrix and its gradient with \c set().
*/
template<int N, typename T = double>
class SimplexMeanRatio :
  public SimplexJacQF<N,T> {
private:

  typedef SimplexJacQF<N,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! The class for a vertex.
  typedef typename Base::Vertex Vertex;

  //! The simplex type.
  typedef typename Base::Simplex Simplex;

  //! An NxN matrix.
  typedef typename Base::Matrix Matrix;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  SimplexMeanRatio() :
    Base()
  {}

  //! Copy constructor.
  SimplexMeanRatio(const SimplexMeanRatio& other) :
    Base(other)
  {}

  //! Construct from a simplex.
  SimplexMeanRatio(const Simplex& s) :
    Base(s)
  {}

  //! Assignment operator.
  SimplexMeanRatio& 
  operator=(const SimplexMeanRatio& other) {
    if (&other != this) {
      Base::operator=(other);
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexMeanRatio()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Return the mean ratio (eta) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S|^2 }{ N \sigma^{2/N} }. \f]
  */
  Number
  operator()() const;

  //! Return the mean ratio (eta) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S|^2 }{ N \sigma^{2/N} }. \f]
  */
  Number
  operator()(const Simplex& simplex) const {
    setFunction(simplex);
    return operator()();
  }

  //! Calculate the gradient of the mean ratio (eta) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    Let \f$ S \f$ be the Jacobian matrix, \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  The eta function
    is 
    \f[ \frac{ |S|^2 }{ N \sigma^{2/N} }. \f]
  */
  void
  computeGradient(Vertex* gradient) const;

  //! @}
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from SimplexJacQF.
  */
  //! @{

  //! Return a const reference to the Jacobian matrix.
  using Base::getMatrix;

  //! Return a const reference to the gradient of the Jacobian matrix.
  using Base::getGradientMatrix;

  //! Return the determinant of the Jacobian matrix.
  using Base::getDeterminant;

  //! Return a const reference to the gradient of the determinant of the Jacobian matrix.
  using Base::getGradientDeterminant;

  //! Return the content (hypervolume) of the simplex.
  using Base::computeContent;

  //! Calculate the gradient of the content (hypervolume) of the simplex.
  using Base::computeGradientContent;

  //! Return the space dimension.
  using Base::getDimension;

  //! @}

protected:

  //! Return the eta quality metric given \f$ |S|^2 \f$.
  Number
  computeFunctionGivenS2(Number s2) const;

};

END_NAMESPACE_GEOM

#define __geom_SimplexMeanRatio_ipp__
#include "SimplexMeanRatio.ipp"
#undef __geom_SimplexMeanRatio_ipp__

#endif
