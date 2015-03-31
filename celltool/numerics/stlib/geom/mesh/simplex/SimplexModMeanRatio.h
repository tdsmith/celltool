// -*- C++ -*-

/*! 
  \file SimplexModMeanRatio.h
  \brief Implements the mean ratio quality metric.
*/

#if !defined(__geom_SimplexModMeanRatio_h__)
#define __geom_SimplexModMeanRatio_h__

#include "SimplexMeanRatio.h"
#include "SimplexModDet.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexModMeanRatio)
#define DEBUG_SimplexModMeanRatio
#endif

BEGIN_NAMESPACE_GEOM

//! Implements the mean ratio quality metric.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This class implements the modified mean ratio quality metric.
  - operator()()
  returns the modified mean ratio metric.
  - operator()(const Simplex& simplex)
  returns the modified mean ratio metric as a function of the simplex.
  - operator()(const Number minDeterminant)
  returns the modified metric.
  - computeGradient(Vertex& grad)
  calculates the gradient of the metric.  If the determinant is positive,
  it is the gradient of the unmodified metric.  Otherwise it is 
  the modified metric.
  - computedGradient(const Number minDeterminant,Vertex& gradient)
  is useful when this simplex is part of a complex of simplices.

  Before evaluating the metric, you must set the 
  Jacobian matrix with \c setFunction() or \c set().
  Before evaluating the gradient of the metric, you must 
  set the Jacobian matrix and its gradient with \c set().
*/
template <int N, typename T = double>
class SimplexModMeanRatio :
  public SimplexMeanRatio<N,T> {
private:

  typedef SimplexMeanRatio<N,T> Base;

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
  SimplexModMeanRatio() :
    Base()
  {}

  //! Copy constructor.
  SimplexModMeanRatio(const SimplexModMeanRatio& other) :
    Base(other)
  {}


  //! Construct from a simplex.
  SimplexModMeanRatio(const Simplex& s) :
    Base(s)
  {}

  //! Assignment operator.
  SimplexModMeanRatio& 
  operator=(const SimplexModMeanRatio& other) {
    if (&other != this) {
      Base::operator=(other);
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexModMeanRatio()
  {}

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

  //! Calculate or return the gradient of the content (hypervolume) of the simplex.
  using Base::computeGradientContent;

  //! Return the space dimension.
  using Base::getDimension;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Return the modified mean ratio (eta) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S|^2 }{ N \sigma^{2/N} }. \f]
  */
  Number
  operator()() const {
    return operator()(getDeterminant());
  }

  //! Return the modified mean ratio (eta) quality metric.
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
    return operator()(getDeterminant());
  }

  //! Return the modified mean ratio (eta) quality metric.
  /*!
    \param minDeterminant is the minimum determinant of the simplices
    currently being considered.  If the quality of a single simplex 
    is being computed, then \c minDeterminant should be the Jacobian
    determinant of this simplex.  If the quality of the simplices
    adjacent to a vertex is being considered, then \c minDeterminant
    should be the minimum determinant among these simplices.

    \pre The Jacobian determinant need not be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, 
    \f$ \sigma \f$ be the Jacobian determinant, 
    \f$ \sigma_m \f$ be the minimum Jacobian determinant, 
    \f$ \epsilon \f$ be the value of epsilon()
    and \f$ | \cdot | \f$ be the Frobenius norm.  
    If \f$ \sigma_m \geq \epsilon \f$ then return the eta quality metric:
    \f[ \frac{ |S|^2 }{ N \sigma^{2/N} }. \f]
    Otherwise return the modified eta quality metric:
    \f[ \frac{ |S|^2 }{ N (h(\sigma_m))^{2/N} }. \f]
   */
  Number
  operator()(Number minDeterminant) const;

  //! Calculate the gradient of the modified mean ratio (eta) quality metric.
  /*!
    \pre The Jacobian determinant need not be positive.

    Let \f$ S \f$ be the Jacobian matrix, \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  The modified
    eta function is
    \f[ \frac{ |S|^2 }{ N (h(\sigma))^{2/N} }. \f]
  */
  void
  computeGradient(Vertex* gradient) const {
    return computeGradient(getDeterminant(), gradient);
  }

  //! Calculate the gradient of the modified mean ratio (eta) quality metric.
  /*!
    \c minDeterminant is the minimum determinant of the simplices
    currently being considered.  If the quality of a single simplex 
    is being computed, then \c minDeterminant should be the Jacobian
    determinant of this simplex.  If the quality of the simplices
    adjacent to a vertex is being considered, then \c minDeterminant
    should be the minimum determinant among these simplices.

    \pre The Jacobian determinant need not be positive.

    Let \f$ S \f$ be the Jacobian matrix, 
    \f$ \sigma \f$ be the Jacobian determinant, 
    \f$ \sigma_m \f$ be the minimum Jacobian determinant, 
    \f$ \epsilon \f$ be the value of epsilon()
    and \f$ | \cdot | \f$ be the Frobenius norm.  
    If \f$ \sigma_m \geq \epsilon \f$ then the eta quality metric is
    \f[ \frac{ |S|^2 }{ N \sigma^{2/N} }. \f]
    Otherwise the modified eta quality metric is
    \f[ \frac{ |S|^2 }{ N (h(\sigma_m))^{2/N} }. \f]
   */
  void
  computeGradient(Number minDeterminant, Vertex* gradient) const;

  //! @}

protected:

  //! Return the modified mean ratio quality metric given \f$ |S|^2 \f$.
  Number
  computeFunctionGivenS2(const Number s2) const {
    return computeFunctionGivenS2(getDeterminant(), s2);
  }

  //! Return the modified mean ratio quality metric given \f$ |S|^2 \f$.
  Number
  computeFunctionGivenS2(Number minDeterminant, Number s2) const;

private:

  Number
  getH(const Number minDeterminant) const {
    return SimplexModDet<T>::getH(getDeterminant(), minDeterminant);
  }

};

END_NAMESPACE_GEOM

#define __geom_SimplexModMeanRatio_ipp__
#include "SimplexModMeanRatio.ipp"
#undef __geom_SimplexModMeanRatio_ipp__

#endif
