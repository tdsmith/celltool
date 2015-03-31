// -*- C++ -*-

/*! 
  \file ComplexWithFreeVertex.h
  \brief A local simplicial complex with a free node at the center.
*/

#if !defined(__geom_ComplexWithFreeVertex_h__)
#define __geom_ComplexWithFreeVertex_h__

#include "SimplexWithFreeVertex.h"
#include "SimplexModDet.h"

#include <vector>
#include <iostream>

#include <cassert>

#if defined(DEBUG_geom) && !defined(DEBUG_ComplexWithFreeVertex)
#define DEBUG_ComplexWithFreeVertex
#endif

BEGIN_NAMESPACE_GEOM

//! A local simplicial complex with a free node at the center.
/*!
  \param QF is the quality functor for the simplices.
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This class implements the complex of simplices that surround a free 
  node.  It provides functions to aid in the optimization of the 
  location of this node.  The faces in the complex that are not incident
  to the free node are fixed.  You can set the fixed faces with the
  ComplexWithFreeVertex(FaceIterator begin,FaceIterator end) constructor
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
	 typename T = double>
class ComplexWithFreeVertex {
private:

  //
  // Private types.
  //

  typedef SimplexWithFreeVertex<QF,N,T> SWFV;
  typedef std::vector<SWFV> SimplexContainer;
  typedef typename SimplexContainer::iterator SimplexIterator;
  typedef typename SimplexContainer::const_iterator SimplexConstIterator;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! A vertex.
  typedef typename SWFV::Vertex Vertex;

  //! A face of the simplex
  typedef typename SWFV::Face Face;

private:

  //
  // Data
  //

  SimplexContainer _simplices;

  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  ComplexWithFreeVertex(const ComplexWithFreeVertex&);

  // Assignment operator not implemented.
  ComplexWithFreeVertex& 
  operator=(const ComplexWithFreeVertex&);
  
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  ComplexWithFreeVertex()
  {}

  //! Construct from the fixed faces.
  template<typename FaceIterator>
  ComplexWithFreeVertex(FaceIterator beginning, FaceIterator end) {
    set(beginning, end);
  }

  //! Trivial destructor.
  ~ComplexWithFreeVertex()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the free vertex.
  Vertex
  getFreeVertex() const {
    assert(! _simplices.empty());
    return _simplices[0].getFreeVertex();
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the fixed vertices.
  template<typename FaceIterator>
  void
  set(FaceIterator begin, FaceIterator end);

  //! @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Calculate the bounding box for the complex.
  void
  computeBBox(const Vertex& v, BBox<N,T>* bb) {
    set(v);
    computeBBox(bb);
  }

  //! Return the content for the given free node.
  Number
  computeContent(const Vertex& v);

  //! Calculate the gradient of the content for the given free node.
  void
  computeGradientOfContent(const Vertex& v, Vertex* gradient);

  //! Return the 2-norm of the quality metric for the given free node.
  Number
  computeNorm2(const Vertex& v);

  //! Calculate the gradient of the 2-norm of the quality metric for the given free node.
  void
  computeGradientOfNorm2(const Vertex& v, Vertex* gradient);

  //! Return the 2-norm of the modified quality metric for the given free node.
  Number
  computeNorm2Modified(const Vertex& v);

  //! Calculate the gradient of the 2-norm of the modified quality metric for the given free node.
  void
  computeGradientOfNorm2Modified(const Vertex& v, Vertex* gradient);

  //! @}

protected:

  //! Set the free vertex in the simplices.
  /*! 
    For effeciency, it checks if the free vertex has already been set to v.
  */
  void
  set(const Vertex& v);

private:

  // Return the minimum Jacobian determinant.
  Number
  computeMinDeterminant() const;

  // Calculate the bounding box for the complex.
  void
  computeBBox(BBox<N,T>* bb) const;

  // Return the content.
  // The free vertex must be set before calling this function.
  Number
  computeContent() const;

  // Calculate the gradient of the content.
  // The free vertex must be set before calling this function.
  void
  computeGradientOfContent(Vertex* gradient) const;

  // Return the 2-norm of the quality metric.
  // The free vertex must be set before calling this function.
  Number
  computeNorm2() const;

  // Calculate the gradient of the 2-norm of the quality metric.
  // The free vertex must be set before calling this function.
  void
  computeGradientOfNorm2(Vertex* gradient) const;

  // Return the 2-norm of the modified quality metric.
  // The free vertex must be set before calling this function.
  Number
  computeNorm2Modified(Number minDetermintant) const;

  // Calculate the gradient of the 2-norm of the modified quality metric.
  // The free vertex must be set before calling this function.
  void
  computeGradientOfNorm2Modified(Number minDetermintant, Vertex* gradient) 
    const;
};


//! Functor that evaluates the content constraint for a local simplicial mesh.
/*!
  The functor returns
  (current_content - initial_content) / initial_content.
 */
template<class Complex>
class ComplexContentConstraint :
  public std::unary_function<typename Complex::Vertex, 
			     typename Complex::Number>
{
private:

  typedef std::unary_function<typename Complex::Vertex, 
			      typename Complex::Number> base_type;

public:

  //! The argument type.
  typedef typename base_type::argument_type argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

private:

  mutable Complex& _sc;

  // The content of the complex for the initial position of the free vertex.
  result_type _initialContent;

  // Default constructor not implemented.
  ComplexContentConstraint();

  // Assignment operator not implemented.
  ComplexContentConstraint&
  operator=(const ComplexContentConstraint&);

public:

  //! Construct from a \c Complex.
  ComplexContentConstraint(Complex& sc) :
    _sc(sc)
  {}

  //! Copy constructor.
  ComplexContentConstraint(const ComplexContentConstraint& other) :
    _sc(other._sc)
  {}

  //! Initialize the content.
  void
  initialize(const argument_type& x) {
    _initialContent = _sc.computeContent(x);
  }

  //! Return the scaled difference from the initial content.
  result_type
  operator()(const argument_type& x) const {
#ifdef DEBUG_ComplexWithFreeVertex
    assert(_initialContent != 0);
#endif
    return (_sc.computeContent(x) - _initialContent) / _initialContent;
  }

  //! Calculate the gradient of the scaled difference from the initial content.
  void
  gradient(const argument_type& x, argument_type& grad) const {
#ifdef DEBUG_ComplexWithFreeVertex
    assert(_initialContent != 0);
#endif
    _sc.computeGradientOfContent(x, &grad);
    grad /= _initialContent;
  }
};


//! Functor that evaluates the 2-norm of the quality metric for a local simplicial mesh.
template<class Complex>
class ComplexNorm2 :
  public std::unary_function<typename Complex::Vertex, 
			     typename Complex::Number>
{
private:

  typedef std::unary_function<typename Complex::Vertex, 
			      typename Complex::Number> Base;

public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:

  mutable Complex& _sc;

  // Default constructor not implemented.
  ComplexNorm2();

  // Assignment operator not implemented.
  ComplexNorm2&
  operator=(const ComplexNorm2&);

public:

  //! Construct from a \c ComplexWithFreeVertex.
  ComplexNorm2(Complex& sc) :
    _sc(sc)
  {}

  //! Copy constructor.
  ComplexNorm2(const ComplexNorm2& other) :
    _sc(other._sc)
  {}

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
class ComplexNorm2Mod :
  public std::unary_function<typename Complex::Vertex, 
			     typename Complex::Number>
{
private:

  typedef std::unary_function<typename Complex::Vertex, 
			      typename Complex::Number> Base;

public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:

  mutable Complex& _sc;

  // Default constructor not implemented.
  ComplexNorm2Mod();

  // Assignment operator not implemented.
  ComplexNorm2Mod&
  operator=(const ComplexNorm2Mod&);

public:

  //! Construct from a \c ComplexWithFreeVertex.
  ComplexNorm2Mod(Complex& sc) :
    _sc(sc)
  {}

  //! Copy constructor.
  ComplexNorm2Mod(const ComplexNorm2Mod& x) :
    _sc(x._sc)
  {}

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

#define __geom_ComplexWithFreeVertex_ipp__
#include "ComplexWithFreeVertex.ipp"
#undef __geom_ComplexWithFreeVertex_ipp__

#endif
