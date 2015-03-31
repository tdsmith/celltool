// -*- C++ -*-

/*! 
  \file geom/mesh/simplex.h
  \brief Includes the classes for algebraic simplex quality measures.
*/

/*!
  \page geom_mesh_simplex The Simplex Package

  \section simplex_Quality Simplicial Quality Measures

  For the finite element method, the simplicial elements of the mesh should
  be ``well-shaped'' to ensure accuracy and stability of the solution.  

  Large angles reduce interpolation accuracy.  Small or large angles
  increase the condition number of the stiffness matrix.  Poorly
  conditioned elements slow down solvers and introduce large roundoff
  errors.

  Below are the nine ways the vertices of a tetrahedron may become nearly 
  colinear or coplanar.

  \image html poor_quality_tetrahedra.jpg "Degenerate tetrahedra."
  \image latex poor_quality_tetrahedra.pdf "Degenerate tetrahedra." width=0.5\textwidth


  \subsection mesh_Geometric_Metrics Geometric Quality Metrics

  Geometric quality metrics are a function of the geometry of the simplex
  - Maximum or minimum dihedral angle (angle between faces).
  - Aspect ratio (ratio of inscribed to circumscribed sphere).

  \image html geometric_quality.jpg "The dihedral angle and the insphere and circumsphere used to calculate the aspect ratio."
  \image latex geometric_quality.pdf "The dihedral angle and the insphere and circumsphere used to calculate the aspect ratio." width=0.6\textwidth


  \subsection mesh_Algebraic_Metrics Algebraic Quality Metrics

  Algebraic quality metrics are a function of the Jacobian matrix of the
  affine map that transforms the reference (equilateral) simplex to
  the physical simplex.  The \f$ N \times N \f$ Jacobian matrix \f$ S \f$ 
  has information about the volume, shape and orientation of the element.

  In 3-D, the condition number metric is
  \f$ \displaystyle \kappa(S) = \frac{ |S| }{ |S^{-1}| } \f$.  
  \f$ S \f$ becomes singular when the element volume vanishes.  
  \f$ \kappa(S) \f$ measures the distance from singular matrices.

  In 3-D, the mean ratio metric is 
  \f$ \eta(S) = \displaystyle \frac{ |S|^2 }{ 3 \mathrm{det}(S)^{2/3} } \f$.

  These metrics are unity for the equilateral tetrahedron and are singular
  for tetrahedron of zero volume.

  These two algebraic quality metrics have a number of desirable properties.
  - They are sensitive to all types of degeneracies.  (Freitag and Knupp.)
  For example, the dihedral angle 
  quality metric does not detect spears, spindles or spires.
  - Their definition is dimension independent.  (In this implementation
  the dimension is a template parameter.)
  - They have continuous derivatives.  This enables optimization methods
  that use the gradient and Hessian.


  \subsection mesh_Inverted Metrics for Inverted Elements

  Because of their singularities, these algebraic metrics cannot be used 
  to optimize meshes with inverted elements.  Even for good quality meshes, 
  the optimization algorithm may assess the quality of
  inverted elements in trying to improve the mesh.

  We implement the condition number and mean ratio metrics
  presented in ``Simultaneous Untangling and
  Smoothing of Tetrahedral Meshes'' by Escobar et al.
  They modified the metrics to be defined for inverted elements.

  \image html triangle_height.jpg "Iscosoles triangle of varying height." 
  \image latex triangle_height.pdf "Iscosoles triangle of varying height." height=0.15\textheight

  \image html mod_mean_ratio.jpg "The modified and unmodified mean ratio metrics for the triangle are shown in blue and red, respectively."
  \image latex mod_mean_ratio.pdf "The modified and unmodified mean ratio metrics for the triangle are shown in blue and red, respectively." height=0.3\textheight


  \section simplex_Content Content

  The Simplex package has algebraic quality metrics for simplices (triangles, 
  tetrahedra, etc.).  There are four quality functions: condition number,
  modified condition number, mean ratio and modified mean ratio.  
  The condition number and mean ratio metrics are defined for simplices
  with positive content (hypervolume).  
  The modified versions are defined for simplices with positive, zero
  and negative content.

  This package has implementations of the work presented in 
  ``Simultaneous Untangling and Smoothing of Tetrahedral Meshes''
  by Escobar et al. in Computer Methods in Applied Mechanics and Engineering,
  192 (2003) 2775-2787.  
  Also see:
  - "Algebraic Mesh Quality Metrics", by Patrick M. Knupp,
    SIAM Journal of Scientific Computing, Volume 23, Number 1, pages 193-218.
  - ``Tetrahedral Mesh Improvement via Optimization of the Element
    Condition Number'' by Lori A. Freitag and Patrick M. Knupp.

  \section simplex_Classes Classes

  The four classes which implement the quality metrics are:
  - geom::SimplexMeanRatio is the mean ratio quality function.
  - geom::SimplexModMeanRatio is the modified mean ratio quality function.
  - geom::SimplexCondNum is the condition number quality function.
  - geom::SimplexModCondNum is the modified condition number quality function.
  
  These quality metric clases inherit or use functionality from the following
  classes.
  - geom::Simplex is a simplex in N-D.
  - geom::SimplexJac implements the Jacobian matrix of the transformation
    from the identity simplex.
  - geom::SimplexAdjJac implements the adjoint of the Jacobian matrix of 
    the transformation from the identity simplex.
  - geom::SimplexModDet modifies the determinant of the Jacobian matrix so 
    that quality functions can be defined Jacobian matrices with 
    non-positive determinants.
  - geom::SimplexJacQF is a base class for quality functions using the 
    Jacobian.
  - geom::SimplexAdjJacQF is a base class for quality functions using the 
    adjoint of the Jacobian.

  Finally, there are classes for assessing the quality of a complex of
  simplices that are adjacent to a vertex.
  - geom::SimplexWithFreeVertex is a quality function applied to a simplex
    with a free vertex.
  - geom::ComplexWithFreeVertex holds the simplices
    which are incident to a free vertex.

  Use these classes by including the file \c geom/mesh/simplex.h .
*/

#if !defined(__geom_mesh_simplex_h__)
#define __geom_mesh_simplex_h__

#include "simplex/Simplex.h"
#include "simplex/SimplexJac.h"
#include "simplex/SimplexAdjJac.h"

#include "simplex/SimplexModDet.h"

#include "simplex/SimplexJacQF.h"
#include "simplex/SimplexAdjJacQF.h"

#include "simplex/SimplexMeanRatio.h"
#include "simplex/SimplexModMeanRatio.h"

#include "simplex/SimplexCondNum.h"
#include "simplex/SimplexModCondNum.h"

#include "simplex/SimplexWithFreeVertex.h"
#include "simplex/ComplexWithFreeVertex.h"
#include "simplex/ComplexWithFreeVertexOnManifold.h"

#include "simplex/functor.h"
#include "simplex/simplex_distance.h"
#include "simplex/decomposition.h"
#include "simplex/geometry.h"
#include "simplex/topology.h"

#endif
