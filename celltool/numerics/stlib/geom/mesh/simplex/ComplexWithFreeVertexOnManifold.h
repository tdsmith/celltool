// -*- C++ -*-

/*! 
  \file ComplexWithFreeVertexOnManifold.h
  \brief A local simplicial complex with a free node at the center.
*/

#if !defined(__geom_ComplexWithFreeVertexOnManifold_h__)
#define __geom_ComplexWithFreeVertexOnManifold_h__

#include "ComplexWithFreeVertex.h"

#if defined(DEBUG_geom) && !defined(DEBUG_ComplexWithFreeVertexOnManifold)
#define DEBUG_ComplexWithFreeVertexOnManifold
#endif

BEGIN_NAMESPACE_GEOM

//! A base class for a local simplicial complex with a free node on a manifold.
/*!
  \param QF is the quality functor for the simplices.
  \param N is the space dimension.
  \param M is the manifold dimension.
  \param Manifold is the parametrized manifold.
  \param T is the number type.  By default it is double.

  This class implements the complex of simplices that surround a free 
  node that lies on a manifold.  It provides functions to aid in the 
  optimization of the 
  location of this node.  The faces in the complex that are not incident
  to the free node are fixed.  You can set the fixed faces with the
  ComplexWithFreeVertexOnManifold(FaceIterator begin,FaceIterator end) 
  constructor
  or with set(FaceIterator begin,FaceIterator end).  You can evaluate
  various quantities as a function of the position of the free node:
  - computeBBox(const Vertex& v,BBox<N,T>* bb)
  calculates the bounding box for the complex.
  - computeContent(const Vertex& v)
  returns the content of the complex.
  - computeGradientOfContent(const Vertex& v,Vertex* gradient)
  calculates the gradient of the content.
  - computeNorm2(const Vertex& v)
  returns the 2-norm of the quality metric.
  - computeGradientOfNorm2(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the quality metric.
  - computeNorm2Modified(const Vertex& v)
  returns the 2-norm of the modified quality metric.
  - computeGradientOfNorm2Modified(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the modified quality metric.
*/
template<template<int,typename> class QF,
	 int N, int M,
	 class _Manifold,
	 typename T = double>
class ComplexWithFreeVertexOnManifoldBase :
  public ComplexWithFreeVertex<QF,N,T> {
private:

  //
  // Private types.
  //

  typedef ComplexWithFreeVertex<QF,N,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef typename Base::Number Number;

  //! A vertex.
  typedef typename Base::Vertex Vertex;

  //! A parametrized manifold.
  typedef _Manifold Manifold;

  //! A point in the parameter space.
  typedef ads::FixedArray<M,Number> ManifoldPoint;

  //! A face of the simplex
  typedef typename Base::Face Face;

protected:

  //
  // Data
  //

  //! The manifold data structure.
  const Manifold* _manifold;

private:

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  ComplexWithFreeVertexOnManifoldBase
  (const ComplexWithFreeVertexOnManifoldBase&);

  // Assignment operator not implemented.
  ComplexWithFreeVertexOnManifoldBase& 
  operator=(const ComplexWithFreeVertexOnManifoldBase&);
  
protected:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  ComplexWithFreeVertexOnManifoldBase() :
    Base()
  {}

  //! Construct from the fixed faces.
  template<typename FaceIterator>
  ComplexWithFreeVertexOnManifoldBase(FaceIterator beginning, 
				      FaceIterator end) :
    Base(beginning, end),
    _manifold(0)
  {}

  //! Trivial destructor.
  ~ComplexWithFreeVertexOnManifoldBase()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the free vertex.
  using Base::getFreeVertex;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the fixed vertices.
  using Base::set;

  //! Set the manifold.
  void
  setManifold(const Manifold* manifold) {
    assert(manifold != 0);
    _manifold = manifold;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Calculate the bounding box for the complex.
  void
  computeBBox(const ManifoldPoint& point, BBox<N,T>* bb) {
    Base::computeBBox(computePosition(point), bb);
  }

  //! Return the content for the specified parameter point.
  Number
  computeContent(const ManifoldPoint& point) {
    return Base::computeContent(computePosition(point));
  }

  //! Calculate the gradient of the content for the given free node.
  void
  computeGradientOfContent(const Vertex& v, Vertex* gradient) {
    Base::computeGradientOfContent(v, gradient);
  }

  //! Return the 2-norm of the quality metric for the given free node.
  Number
  computeNorm2(const Vertex& v) {
    return Base::computeNorm2(v);
  }

  //! Return the 2-norm of the quality metric for the given free node.
  Number
  computeNorm2(const ManifoldPoint& point) {
    return Base::computeNorm2(computePosition(point));
  }

  //! Calculate the gradient of the 2-norm of the quality metric for the given free node.
  void
  computeGradientOfNorm2(const Vertex& v, Vertex* gradient) {
    Base::computeGradientOfNorm2(v, gradient);
  }

  //! Return the 2-norm of the modified quality metric for the given free node.
  Number
  computeNorm2Modified(const Vertex& v) {
    return Base::computeNorm2Modified(v);
  }

  //! Return the 2-norm of the modified quality metric for the given free node.
  Number
  computeNorm2Modified(const ManifoldPoint& point) {
    return Base::computeNorm2Modified(computePosition(point));
  }

  //! Calculate the gradient of the 2-norm of the modified quality metric for the given free node.
  void
  computeGradientOfNorm2Modified(const Vertex& v, Vertex* gradient) {
    Base::computeGradientOfNorm2Modified(v, gradient);
  }

  // Compute a Euclidean position from parameter coordinates.
  Vertex
  computePosition(const ManifoldPoint& point) {
    assert(_manifold != 0);
    return _manifold->computePosition(point);
  }

  //! @}
};









//! A local simplicial complex with a free node on a manifold.
/*!
  \param QF is the quality functor for the simplices.
  \param N is the space dimension.
  \param M is the Manifold dimension.
  \param Manifold is the parametrized manifold.
  \param T is the number type.  By default it is double.

  This class implements the complex of simplices that surround a free 
  node that lies on a manifold.  It provides functions to aid in the 
  optimization of the 
  location of this node.  The faces in the complex that are not incident
  to the free node are fixed.  You can set the fixed faces with the
  ComplexWithFreeVertexOnManifold(FaceIterator begin,FaceIterator end) 
  constructor
  or with set(FaceIterator begin,FaceIterator end).  You can evaluate
  various quantities as a function of the position of the free node:
  - computeBBox(const Vertex& v,BBox<N,T>* bb)
  calculates the bounding box for the complex.
  - computeContent(const Vertex& v)
  returns the content of the complex.
  - computeGradientOfContent(const Vertex& v,Vertex* gradient)
  calculates the gradient of the content.
  - computeNorm2(const Vertex& v)
  returns the 2-norm of the quality metric.
  - computeGradientOfNorm2(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the quality metric.
  - computeNorm2Modified(const Vertex& v)
  returns the 2-norm of the modified quality metric.
  - computeGradientOfNorm2Modified(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the modified quality metric.
*/
template<template<int,typename> class QF,
	 int N, int M,
	 class _Manifold,
	 typename T = double>
class ComplexWithFreeVertexOnManifold;






//! A local simplicial complex with a free node on a manifold.
/*!
  \param QF is the quality functor for the simplices.
  \param N is the space dimension.
  \param Manifold is the parametrized manifold.
  \param T is the number type.

  Specialization for M == 1 (M is the manifold dimension).

  This class implements the complex of simplices that surround a free 
  node that lies on a manifold.  It provides functions to aid in the 
  optimization of the 
  location of this node.  The faces in the complex that are not incident
  to the free node are fixed.  You can set the fixed faces with the
  ComplexWithFreeVertexOnManifold(FaceIterator begin,FaceIterator end) 
  constructor
  or with set(FaceIterator begin,FaceIterator end).  You can evaluate
  various quantities as a function of the position of the free node:
  - computeBBox(const Vertex& v,BBox<N,T>* bb)
  calculates the bounding box for the complex.
  - computeContent(const Vertex& v)
  returns the content of the complex.
  - computeGradientOfContent(const Vertex& v,Vertex* gradient)
  calculates the gradient of the content.
  - computeNorm2(const Vertex& v)
  returns the 2-norm of the quality metric.
  - computeGradientOfNorm2(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the quality metric.
  - computeNorm2Modified(const Vertex& v)
  returns the 2-norm of the modified quality metric.
  - computeGradientOfNorm2Modified(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the modified quality metric.
*/
template<template<int,typename> class QF,
	 int N,
	 class _Manifold,
	 typename T>
class ComplexWithFreeVertexOnManifold<QF,N,1,_Manifold,T> :
  public ComplexWithFreeVertexOnManifoldBase<QF,N,1,_Manifold,T> {
private:

  //
  // Private types.
  //

  typedef ComplexWithFreeVertexOnManifoldBase<QF,N,1,_Manifold,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef typename Base::Number Number;

  //! A vertex.
  typedef typename Base::Vertex Vertex;

  //! A parametrized manifold.
  typedef typename Base::Manifold Manifold;

  //! A point in the parameter space.
  typedef typename Base::ManifoldPoint ManifoldPoint;

  //! A face of the simplex
  typedef typename Base::Face Face;

private:

  //
  // Member data.
  //

  using Base::_manifold;

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  ComplexWithFreeVertexOnManifold(const ComplexWithFreeVertexOnManifold&);

  // Assignment operator not implemented.
  ComplexWithFreeVertexOnManifold& 
  operator=(const ComplexWithFreeVertexOnManifold&);
  
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  ComplexWithFreeVertexOnManifold()
  {}

  //! Construct from the fixed faces.
  template<typename FaceIterator>
  ComplexWithFreeVertexOnManifold(FaceIterator beginning, FaceIterator end) :
    Base(beginning, end)
  {}

  //! Trivial destructor.
  ~ComplexWithFreeVertexOnManifold()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the free vertex.
  using Base::getFreeVertex;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the fixed vertices.
  using Base::set;

  //! Set the manifold.
  using Base::setManifold;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Calculate the bounding box for the complex.
  using Base::computeBBox;

  //! Return the content for the specified parameter point.
  using Base::computeContent;

  //! Calculate the gradient of the content for the specified parameter point.
  void
  computeGradientOfContent(const ManifoldPoint& point, 
			   ManifoldPoint* gradient) {
    // Compute the gradient in Euclidean space.
    Vertex spaceGradient;
    Base::computeGradientOfContent(computePosition(point), &spaceGradient);
    // Compute the gradient in parameter space.
    (*gradient)[0] = computeDotProduct(spaceGradient, 
				       computeDerivative(point));
  }

  //! Return the 2-norm of the quality metric for the given free node.
  using Base::computeNorm2;

  //! Calculate the gradient of the 2-norm of the quality metric for the given free node.
  void
  computeGradientOfNorm2(const ManifoldPoint& point, 
			 ManifoldPoint* gradient) {
    // Compute the gradient in Euclidean space.
    Vertex spaceGradient;
    Base::computeGradientOfNorm2(computePosition(point), &spaceGradient);
    // Compute the gradient in parameter space.
    (*gradient)[0] = computeDotProduct(spaceGradient, 
				       computeDerivative(point));
  }

  //! Return the 2-norm of the modified quality metric for the given free node.
  using Base::computeNorm2Modified;

  //! Calculate the gradient of the 2-norm of the modified quality metric for the given free node.
  void
  computeGradientOfNorm2Modified(const ManifoldPoint& point, 
				 ManifoldPoint* gradient) {
    // Compute the gradient in Euclidean space.
    Vertex spaceGradient;
    Base::computeGradientOfNorm2Modified(computePosition(point), 
					 &spaceGradient);
    // Compute the gradient in parameter space.
    (*gradient)[0] = computeDotProduct(spaceGradient, 
				       computeDerivative(point));
  }

  //! @}

private:

  // Compute a Euclidean position from parameter coordinates.
  using Base::computePosition;

  // Compute the derivative of position.
  Vertex
  computeDerivative(const ManifoldPoint& point) {
    assert(_manifold != 0);
    return _manifold->computeDerivative(point);
  }
};








//! A local simplicial complex with a free node on a manifold.
/*!
  \param QF is the quality functor for the simplices.
  \param N is the space dimension.
  \param Manifold is the parametrized manifold.
  \param T is the number type.

  Specialization for M == 2 (M is the manifold dimension).

  This class implements the complex of simplices that surround a free 
  node that lies on a manifold.  It provides functions to aid in the 
  optimization of the 
  location of this node.  The faces in the complex that are not incident
  to the free node are fixed.  You can set the fixed faces with the
  ComplexWithFreeVertexOnManifold(FaceIterator begin,FaceIterator end) 
  constructor
  or with set(FaceIterator begin,FaceIterator end).  You can evaluate
  various quantities as a function of the position of the free node:
  - computeBBox(const Vertex& v,BBox<N,T>* bb)
  calculates the bounding box for the complex.
  - computeContent(const Vertex& v)
  returns the content of the complex.
  - computeGradientOfContent(const Vertex& v,Vertex* gradient)
  calculates the gradient of the content.
  - computeNorm2(const Vertex& v)
  returns the 2-norm of the quality metric.
  - computeGradientOfNorm2(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the quality metric.
  - computeNorm2Modified(const Vertex& v)
  returns the 2-norm of the modified quality metric.
  - computeGradientOfNorm2Modified(const Vertex& v,Vertex* gradient)
  calculates the gradient of the 2-norm of the modified quality metric.
*/
template<template<int,typename> class QF,
	 int N,
	 class _Manifold,
	 typename T>
class ComplexWithFreeVertexOnManifold<QF,N,2,_Manifold,T> :
  public ComplexWithFreeVertexOnManifoldBase<QF,N,2,_Manifold,T> {
private:

  //
  // Private types.
  //

  typedef ComplexWithFreeVertexOnManifoldBase<QF,N,2,_Manifold,T> Base;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef typename Base::Number Number;

  //! A vertex.
  typedef typename Base::Vertex Vertex;

  //! A parametrized manifold.
  typedef typename Base::Manifold Manifold;

  //! A point in the parameter space.
  typedef typename Base::ManifoldPoint ManifoldPoint;

  //! A face of the simplex
  typedef typename Base::Face Face;

private:

  //
  // Member data.
  //

  using Base::_manifold;

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  ComplexWithFreeVertexOnManifold(const ComplexWithFreeVertexOnManifold&);

  // Assignment operator not implemented.
  ComplexWithFreeVertexOnManifold& 
  operator=(const ComplexWithFreeVertexOnManifold&);
  
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  ComplexWithFreeVertexOnManifold()
  {}

  //! Construct from the fixed faces.
  template<typename FaceIterator>
  ComplexWithFreeVertexOnManifold(FaceIterator beginning, FaceIterator end) :
    Base(beginning, end)
  {}

  //! Trivial destructor.
  ~ComplexWithFreeVertexOnManifold()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the free vertex.
  using Base::getFreeVertex;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the fixed vertices.
  using Base::set;

  //! Set the manifold.
  using Base::setManifold;

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Calculate the bounding box for the complex.
  using Base::computeBBox;

  //! Return the content for the specified parameter point.
  using Base::computeContent;

  //! Calculate the gradient of the content for the specified parameter point.
  void
  computeGradientOfContent(const ManifoldPoint& point, 
			   ManifoldPoint* gradient) {
    // Compute the gradient in Euclidean space.
    Vertex spaceGradient;
    Base::computeGradientOfContent(computePosition(point), &spaceGradient);
    // Compute the gradient in parameter space.
    Vertex dxds, dxdt;
#ifdef __INTEL_COMPILER
    // CONTINUE: For some reason icc does not like the line below.
    // I replaced it with the contents of the function in order get it 
    // to compile.
    assert(_manifold != 0);
    _manifold->computeDerivative(point, &dxds, &dxdt)
#else
    computeDerivative(point, &dxds, &dxdt)
#endif
    (*gradient)[0] = computeDotProduct(spaceGradient, dxds);
    (*gradient)[1] = computeDotProduct(spaceGradient, dxdt);
  }

  //! Return the 2-norm of the quality metric for the given free node.
  using Base::computeNorm2;

  //! Calculate the gradient of the 2-norm of the quality metric for the given free node.
  void
  computeGradientOfNorm2(const ManifoldPoint& point, 
			 ManifoldPoint* gradient) {
    // Compute the gradient in Euclidean space.
    Vertex spaceGradient;
    Base::computeGradientOfNorm2(computePosition(point), &spaceGradient);
    // Compute the gradient in parameter space.
    Vertex dxds, dxdt;
#ifdef __INTEL_COMPILER
    // CONTINUE: For some reason icc does not like the line below.
    // I replaced it with the contents of the function in order get it 
    // to compile.
    assert(_manifold != 0);
    _manifold->computeDerivative(point, &dxds, &dxdt)
#else
    computeDerivative(point, &dxds, &dxdt)
#endif
    (*gradient)[0] = computeDotProduct(spaceGradient, dxds);
    (*gradient)[1] = computeDotProduct(spaceGradient, dxdt);
  }

  //! Return the 2-norm of the modified quality metric for the given free node.
  using Base::computeNorm2Modified;

  //! Calculate the gradient of the 2-norm of the modified quality metric for the given free node.
  void
  computeGradientOfNorm2Modified(const ManifoldPoint& point, 
				 ManifoldPoint* gradient) {
    // Compute the gradient in Euclidean space.
    Vertex spaceGradient;
    Base::computeGradientOfNorm2Modified(computePosition(point), 
					 &spaceGradient);
    // Compute the gradient in parameter space.
    Vertex dxds, dxdt;
    computeDerivative(point, &dxds, &dxdt);
    (*gradient)[0] = computeDotProduct(spaceGradient, dxds);
    (*gradient)[1] = computeDotProduct(spaceGradient, dxdt);
  }

  //! @}

private:

  // Compute a Euclidean position from parameter coordinates.
  using Base::computePosition;

  // Compute the derivative of position.
  void
  computeDerivative(const ManifoldPoint& point, Vertex* dxds, Vertex* dxdt) {
    assert(_manifold != 0);
    _manifold->computeDerivative(point, dxds, dxdt);
  }
};








//! Functor that evaluates the 2-norm of the quality metric for a local simplicial mesh.
template<class Complex>
class ComplexManifoldNorm2 :
  public std::unary_function<typename Complex::ManifoldPoint, 
			     typename Complex::Number> {
private:

  typedef std::unary_function<typename Complex::ManifoldPoint, 
			      typename Complex::Number> Base;

public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:

  mutable Complex& _sc;

  // Default constructor not implemented.
  ComplexManifoldNorm2();

  // Assignment operator not implemented.
  ComplexManifoldNorm2&
  operator=(const ComplexManifoldNorm2&);

public:

  //! Construct from a \c ComplexWithFreeVertexOnManifold.
  ComplexManifoldNorm2(Complex& sc) :
    _sc(sc)
  {}

  //! Copy constructor.
  ComplexManifoldNorm2(const ComplexManifoldNorm2& other) :
    _sc(other._sc)
  {}

  //! Get the complex.
  Complex&
  getComplex() const {
    return _sc;
  }

  //! Return the 2-norm of the quality function.
  result_type
  operator()(const argument_type& x) const {
    return _sc.computeNorm2(x);
  }

  //! Calculate the gradient of the 2-norm of the quality function.
  void
  gradient(const argument_type& x, argument_type& grad) const {
    _sc.computeGradientOfNorm2(x, &grad);
  }
};




//! Functor that evaluates the 2-norm of the modified quality metric for a local simplicial mesh.
template<class Complex>
class ComplexManifoldNorm2Mod :
  public std::unary_function<typename Complex::ManifoldPoint, 
			     typename Complex::Number> {
private:

  typedef std::unary_function<typename Complex::ManifoldPoint, 
			      typename Complex::Number> Base;

public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:

  mutable Complex& _sc;

  // Default constructor not implemented.
  ComplexManifoldNorm2Mod();

  // Assignment operator not implemented.
  ComplexManifoldNorm2Mod&
  operator=(const ComplexManifoldNorm2Mod&);

public:

  //! Construct from a \c ComplexWithFreeVertexOnManifold.
  ComplexManifoldNorm2Mod(Complex& sc) :
    _sc(sc)
  {}

  //! Copy constructor.
  ComplexManifoldNorm2Mod(const ComplexManifoldNorm2Mod& x) :
    _sc(x._sc)
  {}

  //! Get the complex.
  Complex&
  getComplex() const {
    return _sc;
  }

  //! Return the 2-norm of the modified quality function.
  result_type
  operator()(const argument_type& x) const {
    return _sc.computeNorm2Modified(x);
  }

  //! Calculate the gradient of the 2-norm of the modified quality function.
  void
  gradient(const argument_type& x, argument_type& grad) const {
    _sc.computeGradientOfNorm2Modified(x, &grad);
  }
};

END_NAMESPACE_GEOM

#define __geom_ComplexWithFreeVertexOnManifold_ipp__
#include "ComplexWithFreeVertexOnManifold.ipp"
#undef __geom_ComplexWithFreeVertexOnManifold_ipp__

#endif
