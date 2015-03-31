// -*- C++ -*-

/*! 
  \file SimplexJac.h
  \brief Implements operations for the Jacobian matrix of a simplex.
*/

#if !defined(__geom_SimplexJac_h__)
#define __geom_SimplexJac_h__

#include "geometry.h"

#include "../../../ads/tensor/SquareMatrix.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexJac)
#define DEBUG_SimplexJac
#endif

BEGIN_NAMESPACE_GEOM

//! Implements operations for the Jacobian matrix of a simplex.
/*!
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  <b>The Jacobian Matrix</b>

  Consider a simplex \f$T\f$ with vertices 
  \f$ \{ \mathbf{x}_0, \ldots \mathbf{x}_{N-1} \} \f$.  We call this
  the physical simplex.
  The identity simplex \f$T_I\f$ may be mapped to the physical simplex
  \f$T\f$ by an affine transformation and a translation.
  \f[ T = S T_I + \mathbf{x}_0 \f]
  \f$S\f$ is the Jacobian matrix of the transformation.

  In 2-D, the identity triangle has vertices:
  \f$(0,0)\f$, \f$(1,0)\f$ and \f$(1/2,\sqrt{3}/2)\f$.
  In 3-D, the identity tetrahedron has vertices:
  \f$(0,0,0)\f$, \f$(1,0,0)\f$, \f$(1/2,\sqrt{3}/2,0)\f$ and
  \f$(1/2,\sqrt{3}/6,\sqrt{2} / \sqrt{3})\f$.

  The logical simplex, \f$T_L\f$ is the simplex whose vertices are the origin
  and unit displacements in each coordinate direction.
  In 2-D, the logical triangle has vertices:
  \f$(0,0)\f$, \f$(1,0)\f$ and \f$(0,1)\f$.
  In 3-D, the logical tetrahedron has vertices:
  \f$(0,0,0)\f$, \f$(1,0,0)\f$, \f$(0,1,0)\f$, \f$(0,0,1)\f$.
  It is easy to map the logical simplex to the physical simplex.
  \f[ T = A T_L + \mathbf{x}_0 \f]
  The columns of \f$A\f$ are the displacements of the vertices from 
  the first vertex \f$ \mathbf{x}_0 \f$.  In 3-D, this is
  \f[
  A = 
  \left(
  \begin{array}{ccc}
  x_1 - x_0 & x_2 - x_0 & x_3 - x_0 \\
  y_1 - y_0 & y_2 - y_0 & y_3 - y_0 \\
  z_1 - z_0 & z_2 - z_0 & z_3 - z_0 
  \end{array}
  \right).
  \f]
  It is also easy to map the logical simplex to the identity simplex.
  \f[
  T_I = W T_L
  \f]
  The columns of \f$W\f$ are the vertices of \f$T_I\f$.  In 3-D, this is
  \f[
  W = 
  \left(
  \begin{array}{ccc}
  1 & 1/2 & 1/2 \\
  0 & \sqrt{3} / 2 & \sqrt{3} / 6 \\
  0 & 0 & \sqrt{2} / \sqrt{3}
  \end{array}
  \right).
  \f]
  By combining the former transformation with the inverse of the latter,
  we can map the identity simplex to the physical simplex.
  \f[
  T = A W^{-1} T_I + \mathbf{x}_0
  \f]
  The Jacobian matrix of the transformation is \f$ S = A W^{-1} \f$.

  <b>Usage</b>

  Construct a \c SimplexJac with the default constructor or from a 
  \c SimplexJac::Simplex.
  \code
  typedef geom::SimplexJac<3> TetJac;
  typedef TetJac::Vertex Vertex;
  typedef TetJac::Simplex Tetrahedron;
  // Default constructor.
  TetJac tet;
  \endcode
  \code
  // The identity tetrahedron.
  Tetrahedron t(Vertex(0, 0, 0),
                Vertex(1, 0, 0),
	        Vertex(1./2, std::sqrt(3.)/2, 0),
	        Vertex(1./2, std::sqrt(3.)/6, std::sqrt(2./3.)));
  TetJac tet(t);
  \endcode
  The \c Simplex constructor calls \c set() to enable evaluation of the
  determinant, the content and their gradients.

  To evaluate the determinant or the content of the simplex, first call 
  \c setFunction() to set the Jacobian matrix and then use the getDeterminant()
  and computeContent() member functions.
  \code
  tet.setFunction(t);
  std::cout << "Identity tetrahedron:\n"
            << "determinant = " << tet.getDeterminant()
            << "\nvolume = " << tet.computeContent()
            << '\n';
  \endcode
  To evaluate the determinant and content and/or their gradients, first call 
  \c set() to set the Jacobian matrix and its gradient.  Then use the 
  member functions to access the appropriate qantities.
  \code
  tet.set(t);
  std::cout << "Identity tetrahedron:\n"
            << "determinant = " << tet.getDeterminant()
            << "\ngrad determinant = " << tet.getGradientDeterminant()
            << "\nvolume = " << tet.computeContent()
            << "\ngrad volume = " << tet.computeGradientContent() 
            << '\n';
  \endcode
*/
template<int N, typename T = double>
class SimplexJac {
public:

  //
  // public typedefs
  //

  //! The number type.
  typedef T Number;

  //! The class for a vertex.
  typedef ads::FixedArray<N,Number> Vertex;

  //! The simplex type.
  typedef Simplex<N,Vertex,Number> Simplex;

  //! An NxN matrix.
  typedef ads::SquareMatrix<N,Number> Matrix;

private:

  //
  // Member data.
  //

  // The Jacobian matrix that maps the identity simplex to this 
  // simplex (after the first vertex has been translated to the origin.)
  Matrix _matrix;

  // The determinant of _matrix.
  Number _determinant;

  // The gradient of the determinant of the Jacobian matrix.
  Vertex _gradientDeterminant;

  //
  // Static member data.
  //

  // The Jacobian matrix that maps the identity simplex to the reference 
  // simplex.  In 2-D, it maps the identity triangle:
  // (0,0), (1,0), (1/2,sqrt(3)/2)
  // to the reference triangle:
  // (0,0), (1,0), (0,1).
  // In 3-D it maps the identity tetrahedron:
  // (0,0,0), (1,0,0), (1/2,sqrt(3)/2,0), (1/2,sqrt(3)/6,sqrt(2)/sqrt(3))
  // to the reference tetrahedron:
  // (0,0,0), (1,0,0), (0,1,0), (0,0,1).
  static Matrix _identityToReference;

  // The gradient of the Jacobian matrix.
  static ads::FixedArray<N,Matrix> _gradientMatrix;

  // The determinant of _identityToReference;
  static Number _determinantIdentityToReference;

  // N!
  static Number _nFactorial;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  SimplexJac() :
    _matrix(),
    _determinant(),
    _gradientDeterminant()
  {}

  //! Copy constructor.
  SimplexJac(const SimplexJac& other) :
    _matrix(other._matrix),
    _determinant(other._determinant),
    _gradientDeterminant(other._gradientDeterminant)
  {}


  //! Construct from a simplex.
  SimplexJac(const Simplex& s) {
    set(s);
  }

  //! Assignment operator.
  SimplexJac& 
  operator=(const SimplexJac& other) {
    if (&other != this) {
      _matrix = other._matrix;
      _determinant = other._determinant;
      _gradientDeterminant = other._gradientDeterminant;
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexJac()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors
  //! @{

  //! Return a const reference to the Jacobian matrix.
  const Matrix&
  getMatrix() const {
    return _matrix;
  }

  //! Return a const reference to the gradient of the Jacobian matrix.
  const ads::FixedArray<N,Matrix>&
  getGradientMatrix() const {
    return _gradientMatrix;
  }

  //! Return the determinant of the Jacobian matrix.
  Number
  getDeterminant() const {
    return _determinant;
  }

  //! Return a const reference to the gradient of the determinant of the Jacobian matrix.
  const Vertex&
  getGradientDeterminant() const {
    return _gradientDeterminant;
  }

  //! Return the content (hypervolume) of the simplex.
  Number
  computeContent() const {
    return _determinant / _nFactorial / _determinantIdentityToReference;
  }

  //! Calculate the gradient of the content (hypervolume) of the simplex.
  void
  computeGradientContent(Vertex* grad) const {
    *grad = _gradientDeterminant;
    *grad /= (_nFactorial * _determinantIdentityToReference);
  }

  //! Return the gradient of the content (hypervolume) of the simplex.
  Vertex
  computeGradientContent() const {
    Vertex grad;
    computeGradientContent(&grad);
    return grad;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators
  //! @{

  //! Set the vertices.  Calculate the Jacobian matrix and determinant.
  void
  setFunction(const Simplex& s);

  //! Set the vertices.  Calculate the Jacobian matrix and the determinant and its gradient.
  void
  set(const Simplex& s);

  // CONTINUE: These kind of functions do not know the orientation of the 
  // simplex.  A 3-2 mesh could become tangled and still have high quality.
  //! Set the vertices.  Calculate the Jacobian matrix and determinant.
  /*!
    This first projects the simplex to N-D and then call the above 
    setFunction().
  */
  void
  setFunction(const geom::Simplex<N,ads::FixedArray<N+1,Number>,Number>& s);

  //! Set the vertices.  Calculate the Jacobian matrix and the determinant and its gradient.
  /*!
    This first projects the simplex to N-D and then call the above set().
  */
  void
  set(const geom::Simplex<N,ads::FixedArray<N+1,Number>,Number>& s);

  //! @}

};

END_NAMESPACE_GEOM

#define __geom_SimplexJac_ipp__
#include "SimplexJac.ipp"
#undef __geom_SimplexJac_ipp__

#endif
