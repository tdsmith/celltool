// -*- C++ -*-

/*! 
  \file SimplexAdjJac.h
  \brief Implements operations for the adjoint Jacobian matrix of a simplex.
*/

#if !defined(__geom_SimplexAdjJac_h__)
#define __geom_SimplexAdjJac_h__

#include "../../defs.h"

#include "../../../ads/tensor/SquareMatrix.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexAdjJac)
#define DEBUG_SimplexAdjJac
#endif

BEGIN_NAMESPACE_GEOM

//! Implements operations for the adjoint Jacobian matrix of a simplex.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  <b>The Adjoint of a Matrix</b>

  Consult the documentation of \c geom::SimplexJac for information on the 
  Jacobian matrix of a simplex.  This class implements operations on the
  adjoint of this matrix.  In this context, the adjoint is the scaled 
  inverse of a matrix.  The adjoint of a non-singular matrix \f$M\f$ is
  \f$ M^{-1} / \mathrm{det}(M) \f$.  For the general case, we have:
  \f[
  M \cdot \mathrm{adj}(M) = \mathrm{det}(M) I
  \f]
  where \f$ I \f$ is the identity matrix.

  The adjoint of the Jacobian matrix of a simplex is used in the 
  condition number quality metric.  (See \c geom::SimplexCondNum and
  \c geom::SimplexModCondNum.)

  <b>Usage</b>

  Make a SimplexAdjJac by using the default constructor or the Matrix
  constructor.
  \code
  typedef geom::SimplexJac<3> TetJac;
  typedef geom::SimplexAdjJac<3> TetAdjJac;
  typedef TetJac::Vertex Vertex;
  typedef TetJac::Simplex Tetrahedron;
  // Default constructor.
  TetAdjJac adjoint;
  \endcode
  \code
  // Identity tetrahedron
  Tetrahedron t(Vertex(0, 0, 0),
                Vertex(1, 0, 0),
                Vertex(1./2, std::sqrt(3.)/2, 0),
                Vertex(1./2, std::sqrt(3.)/6, std::sqrt(2./3.)));
  // The Jacobian matrix.	      
  TetJac jacobian(t);
  // The adjoint of the Jacobian matrix.
  TetAdjJac adjoint(jacobian.getMatrix());
  \endcode
  The latter constructor calls the \c set() member function.

  To evaluate the adjoint of the Jacobian, first call the 
  \c setFunction() manipulator and then use the getMatrix() accessor.
  \code
  adjoint.setFunction(jacobian.getMatrix());
  std::cout << "Adjoint = \n"
            << adjoint.getMatrix() << '\n';
  \endcode

  To evaluate the adjoint of the Jacobian and its gradient, first call the 
  \c set() manipulator and then use the getMatrix() and getGradientMatrix()
  accessors.
  \code
  adjoint.set(jacobian.getMatrix());
  std::cout << "Adjoint = \n"
            << adjoint.getMatrix()
	    << "\nGradient of adjoint = \n"
	    << adjoint.getGradientMatrix() << '\n';
  \endcode
*/
template<int N, typename T = double>
class SimplexAdjJac
{
public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! An NxN matrix.
  typedef ads::SquareMatrix<N,Number> Matrix;

private:

  //
  // Member data.
  //

  // The adjoint (scaled inverse) of the Jacobian matrix.
  Matrix _matrix;

  // The gradient of the scaled inverse of the Jacobian matrix.
  ads::FixedArray<N,Matrix> _gradientMatrix;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  SimplexAdjJac() :
    _matrix(),
    _gradientMatrix()
  {}

  //! Copy constructor.
  SimplexAdjJac(const SimplexAdjJac& other) :
    _matrix(other._matrix),
    _gradientMatrix(other._gradientMatrix)
  {}

  //! Construct from the Jacobian matrix.
  SimplexAdjJac(const Matrix& jacobian) {
    set(jacobian);
  }

  //! Assignment operator.
  SimplexAdjJac& 
  operator=(const SimplexAdjJac& other) {
    if (&other != this) {
      _matrix = other._matrix;
      _gradientMatrix = other._gradientMatrix;
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexAdjJac()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors
  //! @{

  //! Return a const reference to the adjoint Jacobian matrix.
  const Matrix&
  getMatrix() const {
    return _matrix;
  }

  //! Return a const reference to the gradient of the adjoint Jacobian matrix.
  const ads::FixedArray<N,Matrix>&
  getGradientMatrix() const {
    return _gradientMatrix;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //! @{

  //! Calculate the adjoint Jacobian matrix.
  void
  setFunction(const Matrix& jacobian);

  //! Calculate the adjoint Jacobian matrix and its gradient.
  void
  set(const Matrix& jacobian);

  //! @}

};

END_NAMESPACE_GEOM

#define __geom_SimplexAdjJac_ipp__
#include "SimplexAdjJac.ipp"
#undef __geom_SimplexAdjJac_ipp__

#endif
