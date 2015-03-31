// -*- C++ -*-

/*! 
  \file geom/mesh/iss.h
  \brief Includes the classes for indexed simplex sets.
*/

/*!
  \page iss Indexed Simplex Set Package

  <!------------------------------------------------------------------------->
  \section iss_functions Mesh Functions

  The functions which operate on indexed simplex sets are categorized as 
  follows:
  - \ref iss_accessors
  - \ref iss_build
  - \ref iss_buildFromSimplices
  - \ref iss_boundaryCondition
  - \ref iss_cellAttributes
  - \ref iss_distinct_points
  - \ref iss_equality
  - \ref iss_file_io
  - \ref iss_fit
  - \ref iss_geometry
  - \ref iss_laplacian
  - \ref iss_onManifold
  - \ref iss_optimize
  - \ref iss_quality
  - \ref iss_set
  - \ref iss_solveLaplacian
  - \ref iss_subdivide
  - \ref iss_tile
  - \ref iss_topology
  - \ref iss_transfer
  - \ref iss_transform




  <!------------------------------------------------------------------------->
  \section iss_Data Mesh Data Structures

  geom::IndSimpSet implements an indexed simplex set.  It is composed of an 
  array of vertices and an array of indexed simplices.  Below is a mesh
  of the unit square.  The vertex indices are labelled outside the square.
  The indices inside indicate the order of the vertices for the three 
  triangles.

  \image html square.jpg "A mesh of the unit square."
  \image latex square.pdf "A mesh of the unit square."

  This mesh is represented as an array of vertices:
  \verbatim
  0.0 0.0
  1.0 0.0
  1.0 0.5
  1.0 1.0
  0.0 1.0 \endverbatim
  and an array of indexed simplices.
  \verbatim
  0 1 2
  0 2 4
  2 3 4 \endverbatim

  The geom::IndSimpSetIncAdj class inherits from geom::IndSimpSet and 
  stores additional topological information.  It uses 
  geom::VertexSimplexInc to store vertex-simplex incidences and 
  geom::SimplexAdj to store simplex adjacencies.
  Geometric mesh optimization capabilities are implemented using this
  augmented data structure.

  For the above unit square, the vertex-simplex incidences are:
  \verbatim
  0 1
  0
  0 1 2
  2
  1 2 \endverbatim
  These are stored in a packed array.
  The simplex adjacencies are represented as:
  \verbatim
  -1 1  -1
  2  -1 0
  -1 1  -1 \endverbatim
  where -1 indicates no adacent neighbor.  For example, triangle 0 has no 
  adjacent neighbors opposite its first and third vertices and has triangle 1
  opposite its second vertex.

  Consider an (N-1)-D manifold \f$ s \f$ in N-D space (perhaps a curve in
  2-D or a surface in 3-D.)  The manifold can be described \em explicitly 
  or \em implicitly.  An explicit description could be
  parametric.  For example, the unit circle is:
  \f[
  \{ (x,y) : x = \cos(\theta), y = \sin(\theta), 0 \leq \theta < 2 \pi \}
  \f]
  A simplicial mesh is also an explicit description of a manifold.  

  For some purposes an implicit description of a manifold is more 
  useful.  One common implicit representation is a 
  <em>level set function</em>.  
  For example, the unit circle is the set of points on which the function
  \f$ f(x,y) = x^2 + y^2 - 1 \f$ is zero.
  This level set function is useful for determining if a point is inside 
  or outside the circle.  If \f$ f(x,y) \f$ is negative/positive then the
  point \f$ (x,y) \f$ is inside/outside the circle.

  One special kind of level set function is a 
  <em>signed distance function</em>.
  \f$ d(x,y) = \sqrt{x^2 + y^2} - 1\f$ is a signed 
  distance function for the unit cirle.  That is, the value of the function
  is the distance to the circle.  The distance is negative inside the 
  circle and positive outside.

  A related concept is a <em>closest point function</em>.  As the name 
  suggests, the function evaluates to the closest point on the manifold.
  For the unit circle, the closest point function is
  \f[
  c(x,y) = \frac{(x,y)}{ \sqrt{x^2+y^2} } \ \mathrm{ for }\  (x,y) \neq (0,0)
  \f]
  \f[
  c(x,y) = (1,0) \ \mathrm{for}\ (x,y) = (0,0)
  \f]
  (Note that the closest point is not uniquely determined for 
  \f$ (x,y) = (0,0) \f$.)


  Consider a manifold that is the boundary of an object.
  A distance function for the manifold can be used to determine if a
  point is inside or outside the object.  Or it can be used to determine
  if a point is close the boundary of the object.  The closest point function
  is useful if points need to be moved onto the boundary.

  The geom::ISS_SignedDistance class computes the distance to a simplicial 
  mesh and the closest point on the simplicial mesh.  It stores the simplices
  is a bounding box tree.  It efficiently computes distance and closest point
  by using a technique called a lower/upper bound query.  This class is
  used both in mesh generation and in geometric optimization of boundary
  vertices.

  Use the indexed simplex set classes by including the file geom/mesh/iss.h or 
  by including geom/mesh.h .


  <!------------------------------------------------------------------------->
  \section iss_Mesh_Quality Mesh Quality

  We assess the quality of a mesh in terms of the quality of its
  elements.  The \f$ \ell_p \f$ norm of the quality metric over all
  tetrahedra gives the quality of the mesh.  We can use the \f$ \ell_2
  \f$ norm to obtain an average quality or the \f$ \ell_\infty \f$
  norm to measure the worst element quality.

  The operations we apply to optimize the mesh are local.  They only 
  affect the complex of tetrahedra which are adjacent to a node, edge or face.
  We use the \f$ \ell_p \f$ norm to measure the quality of the complex.  


  <!------------------------------------------------------------------------->
  \section iss_Node_Location Geometric Optimization of a Node Location

  The tetrahedra adjacent to an interior vertex \f$ v \f$ form a complex.  
  We move \f$ v \f$ to optimize the \f$ \ell_p \f$ norm of the quality 
  metrics of the simplices in the complex.

  \image html geom_opt_complex.jpg "2-D illustration.  The complex is shown in green."
  \image latex geom_opt_complex.pdf "2-D illustration.  The complex is shown in green." width=0.5\textwidth

  We have investigated using optimization methods that do not require
  derivative information.  These have the advantage that one can
  optimize the \f$ \ell_\infty \f$ (max norm) of the quality metric.
  We implemented the downhill simplex method and the coordinate
  descent method.  However, we have found that optimizing the \f$ \ell_2 \f$ 
  norm with a quasi-Newton method (BFGS) is more robust and much
  more efficient.


  <!------------------------------------------------------------------------->
  \section iss_Boundary_Nodes Geometric Optimization of Boundary Nodes

  For impact/penetration problems, the greatest deformation occurs at 
  the boundary.  It is not possible to arrange the interior nodes to obtain
  a quality mesh if the boundary nodes have poor geometry.  Thus we 
  extended the geometric optimization to boundary nodes.


  One approach is to optimize the position of the boundary node and then
  project it onto the boundary curve/surface.  We use an implicit 
  representation of the boundary.  The projection is done with the 
  closest point function.

  \image html surface_constraint.jpg "Boundary node optimization subject to a surface constraint."
  \image latex surface_constraint.pdf "Boundary node optimization subject to a surface constraint." width=0.5\textwidth

  
  <!------------------------------------------------------------------------->
  \section iss_Content_Constraint Constant Content Constraint

  Another approach is to conserve the content (volume in 3-D) of the complex 
  of simplices
  adjacent to the boundary node.  This is easy to implement and conserves
  mass.  We use a penalty method combined with a quasi-Newton optimization.
  (We use the same optimization method as for interior nodes.  Only the
  objective function changes by adding a penalty.)

  \image html volume_constraint.jpg "Boundary node optimization subject to a constant volume constraint."
  \image latex volume_constraint.pdf "Boundary node optimization subject to a constant volume constraint." width=0.7\textwidth

  A boundary node should be moved only if the surface is approximately
  planar at the node.
  In the first example, the surface at the boundary node is planar.  Thus 
  the boundary shape is unchanged.  In the second example, there is a large
  difference between the adjacent boundary edge normals.  This leads to 
  a large change in the shape of the boundary.


  <!------------------------------------------------------------------------->
  \section iss_Optimization Geometric Optimization

  The user chooses the movable set of nodes.  They may choose all or a subset
  of the interior or boundary nodes.
  The user may use the condition number or mean ration metrics or supply
  their own metric.

  We have implemented several hill climbing methods for sweeping over the 
  nodes of the mesh.  One can sweep over all movable nodes.
  Or one can sweep over movable nodes that have an adjacent 
  tetrahedron with poor quality.

  These hill climbing methods apply local changes to the mesh.  They rapidly
  improve deformations that are local in nature.  It may take many iterations
  to converge if the nodes are widely re-distributed.
*/

#if !defined(__geom_mesh_iss_h__)
#define __geom_mesh_iss_h__

#include "iss/ISS_Interpolate.h"
#include "iss/ISS_SignedDistance.h"
#include "iss/ISS_SimplexQuery.h"
#include "iss/ISS_VertexField.h"
#include "iss/IndSimpSet.h"
#include "iss/IndSimpSetIncAdj.h"
#include "iss/PointsOnManifold.h"
#include "iss/accessors.h"
#include "iss/build.h"
#include "iss/buildFromSimplices.h"
#include "iss/boundaryCondition.h"
#include "iss/cellAttributes.h"
#include "iss/distinct_points.h"
#include "iss/equality.h"
#include "iss/file_io.h"
#include "iss/fit.h"
#include "iss/geometry.h"
#include "iss/laplacian.h"
#include "iss/onManiold.h"
#include "iss/optimize.h"
#include "iss/quality.h"
#include "iss/set.h"
// I intentionally do not include solveLaplacian.h because it uses uses 
// the third-party packages TNT and Jama.  If you want to use solveLaplacian,
// you have to include it directly.
#include "iss/subdivide.h"
#include "iss/tile.h"
#include "iss/topology.h"
#include "iss/transfer.h"
#include "iss/transform.h"

#endif
