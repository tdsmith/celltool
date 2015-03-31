// -*- C++ -*-

/*! 
  \file SimplexModCondNum.h
  \brief Implements the modified condition number quality metric.
*/

#if !defined(__geom_SimplexModCondNum_h__)
#define __geom_SimplexModCondNum_h__

#include "SimplexCondNum.h"
#include "SimplexModDet.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexModCondNum)
#define DEBUG_SimplexModCondNum
#endif

BEGIN_NAMESPACE_GEOM

//! Implements the modified condition number quality metric.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This class implements the modified condition number metric.
  - operator()()
  returns the modified condition number metric.
  - operator()(const Simplex& simplex)
  returns the modified condition number metric as a function of the simplex.
  - operator()(const Number minDeterminant)
  returns the modified metric.
  - computeGradient(Vertex& grad)
  calculates the gradient of the metric.  If the determinant is positive,
  it is the gradient of the unmodified metric.  Otherwise it is 
  the modified metric.
  - computeGradient(const Number minDeterminant,Vertex& gradient)
  is useful when this simplex is part of a complex if simplices.

  Before evaluating the metric, you must set the 
  Jacobian matrix with \c setFunction() or \c set().
  Before evaluating the gradient of the metric, you must 
  set the Jacobian matrix and its gradient with \c set().
*/
template<int N, typename T = double>
class SimplexModCondNum :
  public SimplexCondNum<N,T> {
private:

  typedef SimplexCondNum<N,T> Base;

public:

  //
  // public typedefs
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
  SimplexModCondNum() :
    Base()
  {}

  //! Copy constructor.
  SimplexModCondNum(const SimplexModCondNum& other) :
    Base(other)
  {}


  //! Construct from a simplex.
  SimplexModCondNum(const Simplex& s) :
    Base(s)
  {}

  //! Assignment operator.
  SimplexModCondNum& 
  operator=(const SimplexModCondNum& other) {
    if (&other != this) {
      Base::operator=(other);
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexModCondNum()
  {}

  //!@}
  //--------------------------------------------------------------------------
  /*! \name Accessors.
    Functionality inherited from SimplexJacQF.
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

  //! Calculate or return the gradient of the content (hypervolume) of the simplex.
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

  //! Return the modified condition number (kappa) quality metric.
  /*!
    \pre The Jacobian determinant need not be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S| |\Sigma| }{ N h(\sigma) }. \f]
  */
  Number
  operator()() const {
    return operator()(getDeterminant());
  }

  //! Return the modified condition number (kappa) quality metric.
  /*!
    \pre The Jacobian determinant need not be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  Return
    \f[ \frac{ |S| |\Sigma| }{ N h(\sigma) }. \f]
  */
  Number
  operator()(const Simplex& simplex) const {
    setFunction(simplex);
    return operator()(getDeterminant());
  }

  //! Return the modified condition number (kappa) quality metric.
  /*!
    \param minDeterminant is the minimum determinant of the simplices
    currently being considered.  If the quality of a single simplex 
    is being computed, then \c min_determinant should be the Jacobian
    determinant of this simplex.  If the quality of the simplices
    adjacent to a vertex is being considered, then \c min_determinant
    should be the minimum determinant among these simplices.

    \pre The Jacobian determinant need not be positive.

    \return 
    Let \f$ S \f$ be the Jacobian matrix, 
    \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian determinant, 
    \f$ \sigma_m \f$ be the minimum Jacobian determinant, 
    \f$ \epsilon \f$ be the value of epsilon()
    and \f$ | \cdot | \f$ be the Frobenius norm.  
    If \f$ \sigma_m \geq \epsilon \f$ then return the kappa quality metric:
    \f[ \frac{ |S| |\Sigma| }{ N \sigma }. \f]
    Otherwise return the modified kappa quality metric:
    \f[ \frac{ |S| |\Sigma| }{ N h(\sigma_m) }. \f]
   */
  Number
  operator()(Number minDeterminant) const;

  //! Calculate the gradient of the modified condition number (kappa) quality metric.
  /*!
    \pre The Jacobian determinant need not be positive.

    Let \f$ S \f$ be the Jacobian matrix, \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian
    determinant and \f$ | \cdot | \f$ be the Frobenius norm.  The modified
    kappa function is
    \f[ \frac{ |S| |\Sigma| }{ N h(\sigma) }. \f]
  */
  void
  computeGradient(Vertex* gradient) const {
    return computeGradient(getDeterminant(), gradient);
  }

  //! Calculate the gradient of the modified condition number (kappa) quality metric.
  /*!
    \c min_determinant is the minimum determinant of the simplices
    currently being considered.  If the quality of a single simplex 
    is being computed, then \c min_determinant should be the Jacobian
    determinant of this simplex.  If the quality of the simplices
    adjacent to a vertex is being considered, then \c min_determinant
    should be the minimum determinant among these simplices.

    \pre The Jacobian determinant need not be positive.

    Let \f$ S \f$ be the Jacobian matrix, 
    \f$ \Sigma \f$ be its scaled inverse,
    \f$ \sigma \f$ be the Jacobian determinant, 
    \f$ \sigma_m \f$ be the minimum Jacobian determinant, 
    \f$ \epsilon \f$ be the value of epsilon()
    and \f$ | \cdot | \f$ be the Frobenius norm.  
    If \f$ \sigma_m \geq \epsilon \f$ then the kappa quality metric is
    \f[ \frac{ |S| |\Sigma| }{ N \sigma }. \f]
    Otherwise the modified kappa quality metric is
    \f[ \frac{ |S| |\Sigma| }{ N h(\sigma_m) }. \f]
   */
  void
  computeGradient(Number minDeterminant, Vertex* gradient) const;

  //!@}

protected:

  //! Return the modified quality metric given \f$ |S|^2 \f$ and \f$ |\Sigma|^2 \f$.
  Number
  computeFunction(const Number snj, const Number sna) const {
    return computeFunction(getDeterminant(), snj, sna);
  }

  //! Return the modified quality metric given \f$ |S|^2 \f$ and \f$ |\Sigma|^2 \f$.
  Number
  computeFunction(Number minDeterminant, Number snj, Number sna) const;
  
private:

  Number
  getH(const Number minDeterminant) const {
    return SimplexModDet<T>::getH(getDeterminant(), minDeterminant);
  }

};

END_NAMESPACE_GEOM

#define __geom_SimplexModCondNum_ipp__
#include "SimplexModCondNum.ipp"
#undef __geom_SimplexModCondNum_ipp__

#endif
