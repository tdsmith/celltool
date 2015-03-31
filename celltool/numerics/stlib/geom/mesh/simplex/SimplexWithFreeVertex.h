// -*- C++ -*-

/*! 
  \file SimplexWithFreeVertex.h
  \brief Implements a class for a simplex with a free vertex.
*/

#if !defined(__geom_SimplexWithFreeVertex_h__)
#define __geom_SimplexWithFreeVertex_h__

#include "Simplex.h"

#if defined(DEBUG_geom) && !defined(DEBUG_SimplexWithFreeVertex)
#define DEBUG_SimplexWithFreeVertex
#endif

BEGIN_NAMESPACE_GEOM

//! A simplex with a free vertex.
/*!
  \param QF is the quality functor for the simplex.
  \param N is the dimension.
  \param T is the number type.  By default it is double.

  This class implements a simplex with a free vertex.  
  It is used to optimize the position of the free vertex.
  One can set the fixed vertices with the 
  SimplexWithFreeVertex(const Face& face) constructor or with
  set(const Face& face).

  If you are going to use the gradient of the quality function, 
  Jacobian determinant, content, etc. then set the free vertex 
  with set(const Vertex& v).  If you don't need gradient 
  information, then use setFunction(const Vertex& v).

  This class provides the following mathematical functions.
  The gradients are with respect to the free vertex.
  - operator()()
  return the quality metric.
  - computeGradient(Vertex* gradient)
  calculates the gradient of the quality metric.
  - operator()(const Number minimumDeterminant)
  returns the modified quality metric.
  - computeGradient(const Number minimumDeterminant,Vertex* gradient)
  calculates the gradient of the modified quality metric.
  - computeDeterminant()
  returns the determinant.
  - computeContent()
  returns the content.
  - computeGradientOfContent(Vertex* gradient)
  calculates the gradient of the content.
*/
template<template<int,typename> class QF,
	 int N,
	 typename T = double>
class SimplexWithFreeVertex {
private:

  //
  // Private types.
  //

  typedef QF<N,T> QualityFunction;

public:

  //
  // Public types.
  //

  //! The number type.
  typedef T Number;

  //! The class for a vertex.
  typedef typename QualityFunction::Vertex Vertex;

  //! A simplex of vertices.
  typedef Simplex<N,Vertex,Number> Simplex;

  //! The face of a simplex.
  typedef typename Simplex::Face Face;

private:

  // The simplex of vertices.
  Simplex _simplex;
  // The quality function.
  QualityFunction _qualityFunction;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //! @{

  //! Default constructor.  Un-initialized memory.
  SimplexWithFreeVertex() :
    _simplex(),
    _qualityFunction()
  {}

  //! Construct from a face.
  SimplexWithFreeVertex(const Face& face) :
    _simplex(),
    _qualityFunction() {
    set(face);
  }

  //! Copy constructor.
  SimplexWithFreeVertex(const SimplexWithFreeVertex& other) :
    _simplex(other._simplex),
    _qualityFunction(other._qualityFunction)
  {}

  //! Assignment operator.
  SimplexWithFreeVertex& 
  operator=(const SimplexWithFreeVertex& other) {
    if (&other != this) {
      _simplex = other._simplex;
      _qualityFunction = other._qualityFunction;
    }
    return *this;
  }
  
  //! Trivial destructor.
  ~SimplexWithFreeVertex()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //! @{

  //! Return the free vertex.
  Vertex
  getFreeVertex() const {
    return _simplex[0];
  }

  //! Calculate a bounding box around the simplex.
  void
  computeBBox(BBox<N,T>* bb) const {
    _simplex.computeBBox(bb);
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the fixed face.
  void
  set(const Face& face) {
    for (int i = 0; i != N; ++i) {
      _simplex[i+1] = face[i];
    }
  }

  //! Set the free vertex.  Prepare for function calls.
  void
  setFunction(const Vertex& v) {
    _simplex[0] = v;
    _qualityFunction.setFunction(_simplex);
  }

  //! Set the free vertex.  Prepare for function and/or gradient calls.
  void
  set(const Vertex& v) {
    _simplex[0] = v;
    _qualityFunction.set(_simplex);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Mathematical functions
  //! @{

  //! Return the quality metric.
  Number
  operator()() const {
    return _qualityFunction();
  }

  //! Calculate the gradient of the quality metric.
  void
  computeGradient(Vertex* gradient) const {
    _qualityFunction.computeGradient(gradient);
  }

  //! Return the modified quality metric.
  Number
  operator()(const Number minimumDeterminant) const {
    return _qualityFunction(minimumDeterminant);
  }

  //! Calculate the gradient of the modified quality metric.
  void
  computeGradient(const Number minimumDeterminant, Vertex* gradient) const {
    _qualityFunction.computeGradient(minimumDeterminant, gradient);
  }

  //! Return the determinant.
  Number
  getDeterminant() const {
    return _qualityFunction.getDeterminant();
  }

  //! Return the content.
  Number
  computeContent() const {
    return _qualityFunction.computeContent();
  }

  //! Calculate the gradient of the content.
  void
  computeGradientOfContent(Vertex* gradient) const {
    _qualityFunction.computeGradientContent(gradient);
  }

  // @}
};

END_NAMESPACE_GEOM

#endif
