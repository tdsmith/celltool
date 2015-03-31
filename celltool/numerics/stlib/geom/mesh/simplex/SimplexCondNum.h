// -*- C++ -*-

/*! 
  \file SimplexCondNum.h
  \brief Implements the condition number quality metric.
*/

#if !defined(__geom_SimplexCondNum_h__)
#define __geom_SimplexCondNum_h__

#include "SimplexAdjJacQF.h"

#include <limits>

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexCondNum)
#define DEBUG_SimplexCondNum
#endif

BEGIN_NAMESPACE_GEOM

//! Implements the condition number quality metric.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This class implements the condition number quality metric.
  Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its adjoint
  (scaled inverse), \f$ \sigma \f$ be the Jacobian
  determinant and \f$ | \cdot | \f$ be the Frobenius norm.
  The \c operator()() member function returns the
  condition number quality metric:
  \f[ 
  \kappa = \frac{ |S| |\Sigma| }{ N \sigma }. 
  \f]
  This quality metric is only defined for simplices with positive content.
  (The Jacobian determinant must be positive.)
  
  \c computeGradient() calculates the gradient of the condition number metric.

  Before evaluating the condition number metric, you must set the 
  Jacobian matrix with \c setFunction() or \c set().
  Before evaluating the gradient of the metric, you must 
  set the Jacobian matrix and its gradient with \c set().
*/
template<int N, typename T = double>
class SimplexCondNum :
  public SimplexAdjJacQF<N,T> {
private:

  typedef SimplexAdjJacQF<N,T> Base;

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
  //!@{

  //! Default constructor.  Un-initialized memory.
  SimplexCondNum() :
    Base()
  {}

  //! Copy constructor.
  SimplexCondNum(const SimplexCondNum& other) :
    Base(other)
  {}

  //! Construct from a simplex.
  SimplexCondNum(const Simplex& s) :
    Base(s)
  {}

  //! Assignment operator.
  SimplexCondNum& 
  operator=(const SimplexCondNum& other) {
    if (&other != this) {
      Base::operator=(other);
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexCondNum()
  {}

  //!@}
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from SimplexAdjJacQF.
  */
  //!@{

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

  //! Return a const reference to the adjoint Jacobian matrix.
  using Base::getAdjointMatrix;

  //! Return a const reference to the gradient of the adjoint Jacobian matrix.
  using Base::getAdjointGradientMatrix;

  //!@}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //!@{

  //! Set the vertices in preparation for a function call.
  using Base::setFunction;

  //! Set the vertices in preparation for a function call or a gradient call.
  using Base::set;

  //!@}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //!@{

  //! Return the condition number (kappa) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S| |\Sigma| }{ N \sigma }. \f]
  */
  Number
  operator()() const;

  //! Return the condition number (kappa) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S| |\Sigma| }{ N \sigma }. \f]
  */
  Number
  operator()(const Simplex& simplex) const {
    setFunction(simplex);
    return operator()();
  }

  //! Calculate the gradient of the condition number (kappa) quality metric.
  /*!
    \pre The Jacobian determinant must be positive.

    Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  The kappa 
    function is
    \f[ \frac{ |S| |\Sigma| }{ N \sigma }. \f]
  */
  void
  computeGradient(Vertex* gradient) const;

  //!@}

protected:

  //! Return the quality metric given \f$ | S |^2 \f$ and \f$ | \Sigma |^2 \f$.
  Number
  computeFunction(Number snj, Number sna) const;

};

END_NAMESPACE_GEOM

#define __geom_SimplexCondNum_ipp__
#include "SimplexCondNum.ipp"
#undef __geom_SimplexCondNum_ipp__

#endif
