// -*- C++ -*-

/*! 
  \file geom/mesh/simplicial/FaceRemoval.h
  \brief Class for edge removal in a tetrahedral mesh.
*/

#if !defined(__geom_mesh_simplicial__FaceRemoval_h__)
#define __geom_mesh_simplicial__FaceRemoval_h__

#include "../simplex/SimplexModCondNum.h"

#include "../../../ads/array/Array.h"

#include <cassert>

#if defined(DEBUG_geom) && !defined(DEBUG_FaceRemoval)
#define DEBUG_FaceRemoval
#endif

BEGIN_NAMESPACE_GEOM

//! Edge removal in a tetrahedral mesh.
template<class _QualityMetric = SimplexModCondNum<3>,
	 class _Point = ads::FixedArray<3>,
	 typename _Number = typename _Point::value_type>
class FaceRemoval {

public:

  //
  // Public types.
  //

  //! The tetrahedron quality metric.
  typedef _QualityMetric QualityMetric;
  //! The point type.
  typedef _Point Point;
  //! The number type;
  typedef _Number Number;

private:
      
  //
  // Private types.
  //

  typedef typename QualityMetric::Simplex Simplex;

  //
  // Data
  //

  // The source and target of the edge.
  Point _source, _target;
  // The three vertices in the face shared by the two initial tetrahedra.
  ads::FixedArray<3,Point> _face;
  // The quality function.
  mutable QualityMetric _qualityFunction;
  
  //
  // Not implemented.
  //

  // Copy constructor not implemented.
  FaceRemoval(const FaceRemoval&);

  // Assignment operator.
  FaceRemoval& 
  operator=(const FaceRemoval&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and Destructor.
  //! @{

  //! Default constructor.  Unititialized memory.
  FaceRemoval() :
    _source(),
    _target(),
    _face(),
    _qualityFunction()
  {}

  //! Destructor.
  ~FaceRemoval()
  {}

  //! @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //! @{

  //! Set the source vertex of the proposed edge.
  void
  setSource(const Point& src) {
    _source = src;
  }

  //! Set the target vertex of the proposed edge.
  void
  setTarget(const Point& tgt) {
    _target = tgt;
  }

  //! Set the vertices of the shared face.
  /*!
    The vertices should go around the proposed edge in the positive direction.
  */
  void
  setFace(const Point& a, const Point& b, const Point& c) {
    _face[0] = a;
    _face[1] = b;
    _face[2] = c;
  }

  //! @}
  //--------------------------------------------------------------------------
  //! \name Tetrahedralization.
  //! @{

  //! Return true if a 2-3 flip improves the quality of the mesh.
  bool
  flip23() {
    return computeQuality3() > computeQuality2();
  }

  //! @}

  //
  // Private member functions.
  //
  // CONTINUE: Should these be private?
  
  // Return the worse quality of the 2 tetrahedra: 
  // _face[0], _face[1], _face[2], _target
  // and
  // _source, _face[0], _face[1], _face[2]
  //! CONTINUE
  Number
  computeQuality2() const;

  // Return the worst quality of the 3 tetrahedra: 
  // _source, _target, _face[0], _face[1]
  // _source, _target, _face[1], _face[2]
  // _source, _target, _face[2], _face[0]
  //! CONTINUE
  Number
  computeQuality3() const;
};


END_NAMESPACE_GEOM


#define __geom_mesh_simplicial_FaceRemoval_ipp__
#include "FaceRemoval.ipp"
#undef __geom_mesh_simplicial_FaceRemoval_ipp__

#endif
