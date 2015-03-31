// -*- C++ -*-

/*! 
  \file SimplexJacQF.h
  \brief Simplex quality functions of the Jacobian matrix.
*/

#if !defined(__geom_SimplexJacQF_h__)
#define __geom_SimplexJacQF_h__

#include "SimplexJac.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexJacQF)
#define DEBUG_SimplexJacQF
#endif

BEGIN_NAMESPACE_GEOM

//! Simplex quality functions of the Jacobian matrix.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This is a base class for simplex quality functions that uses the Jacobian
  matrix.  It has member functions for evaluating the determinant and 
  content of the simplex.
  - \c getDeterminant() returns the determinant of the Jacobian matrix.
  - \c computeContent() returns the content.
  - \c computeGradientContent() calculates the gradient of the content.

  Before calling \c getDeterminant() or \c computeContent(), you must set the 
  Jacobian matrix with \c setFunction() or \c set().
  Before calling \c computeGradientContent(), you must 
  set the Jacobian matrix and its gradient with \c set().
*/
template<int N, typename T = double>
class SimplexJacQF {
private:

  typedef SimplexJac<N,T> Jacobian;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! The class for a vertex.
  typedef typename Jacobian::Vertex Vertex;

  //! The simplex type.
  typedef typename Jacobian::Simplex Simplex;

  //! An NxN matrix.
  typedef typename Jacobian::Matrix Matrix;

private:

  //
  // Member data.
  //

  //! The Jacobian of the transformation from the identity simplex.
  Jacobian _jacobian;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //!@{

  //! Default constructor.  Un-initialized memory.
  SimplexJacQF() :
    _jacobian()
  {}

  //! Copy constructor.
  SimplexJacQF(const SimplexJacQF& other) :
    _jacobian(other._jacobian)
  {}

  //! Construct from a simplex.
  SimplexJacQF(const Simplex& s) :
    _jacobian(s)
  {}

  //! Assignment operator.
  SimplexJacQF& 
  operator=(const SimplexJacQF& other) {
    if (&other != this) {
      _jacobian = other._jacobian;
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexJacQF()
  {}

  //!@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //!@{

  //! Return a const reference to the Jacobian matrix.
  const Matrix&
  getMatrix() const {
    return _jacobian.getMatrix();
  }

  //! Return a const reference to the gradient of the Jacobian matrix.
  const ads::FixedArray<N,Matrix>&
  getGradientMatrix() const {
    return _jacobian.getGradientMatrix();
  }

  //! Return the determinant of the Jacobian matrix.
  Number
  getDeterminant() const {
    return _jacobian.getDeterminant();
  }

  //! Return a const reference to the gradient of the determinant of the Jacobian matrix.
  const Vertex&
  getGradientDeterminant() const {
    return _jacobian.getGradientDeterminant();
  }

  //! Return the content (hypervolume) of the simplex.
  Number
  computeContent() const {
    return _jacobian.computeContent();
  }

  //! Calculate the gradient of the content (hypervolume) of the simplex.
  void
  computeGradientContent(Vertex* grad) const {
    _jacobian.computeGradientContent(grad);
  }

  //! Return the gradient of the content (hypervolume) of the simplex.
  Vertex
  computeGradientContent() const {
    return _jacobian.computeGradientContent();
  }

  //! Return the space dimension.
  static
  int
  getDimension() {
    return N;
  }

  //!@}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //!@{

  //! Set the vertices in preparation for a function call.
  void
  setFunction(const Simplex& s) {
    _jacobian.setFunction(s);
  }

  //! Set the vertices in preparation for a function call or a gradient call.
  void
  set(const Simplex& s) {
    _jacobian.set(s);
  }

  //! Set the vertices in preparation for a function call.
  /*!
    This first projects the simplex to N-D and then calls the above 
    set_function().
  */
  void
  setFunction(const geom::Simplex<N,ads::FixedArray<N+1,Number>,Number>& s) {
    Simplex t;
    projectToLowerDimension(s, &t);
    setFunction(t);
  }

  //! Set the vertices in preparation for a function call.
  /*!
    This first projects the simplex to N-D and then calls the above 
    set_function().
  */
  void
  setFunction(const geom::Simplex<N,ads::FixedArray<N+2,Number>,Number>& s) {
    Simplex t;
    projectToLowerDimension(s, &t);
    setFunction(t);
  }

  //! Set the vertices in preparation for a function call or a gradient call.
  /*!
    This first projects the simplex to N-D and then call the above set().
  */
  void
  set(const geom::Simplex<N,ads::FixedArray<N+1,Number>,Number>& s) {
    Simplex t;
    projectToLowerDimension(s, &t);
    set(t);
  }

  //!@}
};

END_NAMESPACE_GEOM

#endif
