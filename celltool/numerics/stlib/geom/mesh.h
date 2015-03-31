// -*- C++ -*-

/*! 
  \file mesh.h
  \brief Includes the data structures and algorithms for generating and optimizing simplicial (triangle, tetrahedral, etc.) meshes.
*/

// CONTINUE: Use the medial axis to get information on the local feature size.
// Check out papers by Marshal Bern.

/* CONTINUE: Move this documentation to a refinement page.
  There are three ways to use the refinement and coarsening operations:
  - Specify a maximum/minimum allowed edge length function.  
  - Specify a set of cells.
  - Specify an element quality criterion.  This can be used in conjuction
  with the edge length function.  One can specify a <em>minimum allowed 
  element quality</em> (split or collapse only when the resulting elements meet
  the specified minimum quality) and/or a <em>quality factor</em> (proceed 
  only when the quality of the resulting elements is no less that the quality 
  of the initial elements times the factor). 
*/


//----------------------------------------------------------------------------
/*!
  \page geom_mesh Simplicial Mesh Package

  This package has data structures for representing simplicial meshes
  and algorithms for improving the quality of these meshes.  See the
  \ref geom_mesh_ingredients page for an introduction to mesh optimization.

  The documentation is distributed among a number of pages:
  There is brief \ref geom_mesh_overview "overview" and a discussion of 
  the \ref geom_mesh_organization "organization" of the code.
  Another page gives more details on the \ref geom_mesh_data "data structures".
  There are many \ref geom_mesh_example "example programs" which provide 
  useful functionality and show how to use this package.
  Finally, there are a number of pages that show 
  \ref geom_mesh_results "results" of using the example programs.
*/


//----------------------------------------------------------------------------
/*!
  \page geom_mesh_ingredients The Five Essential Ingredients of Mesh Optimization

  There are five essential ingredients to any program of mesh optimization:
  - Element quality metric.
  - Geometric optimization.
  - Topological optimization.
  - Element size control.
  - Boundary description.


  <b>Element Quality</b>
  
  The quality of a mesh is measured in terms of the quality of its
  elements.  Thus one needs a metric for assessing element quality.
  Simply put, an element that is close to the ideal shape (perhaps an 
  equilateral tetrahedron) has good quality and an element that is 
  far from the ideal shape has poor quality.  Below are poor quality
  tetrahedra that are nearly co-linear or co-planar.

  \image html poor_quality_tetrahedra.jpg "Poor quality tetrahedra."
  \image latex poor_quality_tetrahedra.pdf "Poor quality tetrahedra."
  

  <b>Geometry</b>


  The positions of the nodes defines the geometry of a mesh.  If the nodes
  are not well-spaced, then it is not possible to connect them to make 
  quality elements.  Any mesh optimization program must be able to improve 
  the geometry of the mesh.

  Below is a mesh with good geometry.  The nodes are connected to form 
  quality triangles.

  \image html ing_geom_good.jpg 
  \image latex ing_geom_good.pdf

  Next consider a mesh with bad geometry.  Note that there is no way of 
  forming quality elements with these node positions.  One is forced to 
  make skinny triangles along the bottom of the square.  

  \image html ing_geom_bad.jpg 
  \image latex ing_geom_bad.pdf



  <b>Topology</b>

  The connectivity of the nodes to form elements defines the topology 
  of a mesh.  If a mesh has poor topology, then no course of geometry 
  optimization alone could yield a high quality mesh.  Below is 
  a mesh with good topology and relatively poor topology.

  \image html ing_top_good.jpg 
  \image latex ing_top_good.pdf

  \image html ing_top_bad.jpg 
  \image latex ing_top_bad.pdf



  <b>Element Size</b>

  For most applications, it is not sufficient that a mesh have well-shaped 
  elements. the elements must also satisy size requirements.  For finite 
  element applications, elements that are too large increase the error
  and elements that are too small increase the cost of the computation.

  Consider the following three meshes.  In terms of element shape, they have
  the same quality.  However, the meshes are clearly not equivalent.

  \image html ing_size_large.jpg 
  \image latex ing_size_large.pdf

  \image html ing_size_small.jpg 
  \image latex ing_size_small.pdf

  \image html ing_size_non_unif.jpg 
  \image latex ing_size_non_unif.pdf



  <b>Boundary</b>

  Finally, consider the following mesh of the circle.

  \image html ing_boundary_initial.jpg 
  \image latex ing_boundary_initial.pdf 

  If we were to refine the triangles, it would no longer be the mesh of a 
  circle; it would be the mesh of a hexagon.

  \image html ing_boundary_refined.jpg 
  \image latex ing_boundary_refined.pdf 
  
  It is apparent that if we change the mesh, we need a means of keeping the
  boundary nodes on the boundary curve.

  \image html ing_boundary_moved.jpg 
  \image latex ing_boundary_moved.pdf 

*/



//----------------------------------------------------------------------------
/*!
  \page geom_mesh_overview An Overview of the Simplicial Mesh Package.

  \section geom_mesh_overview_introduction Introduction.

  This package contains a suite of hill-climbing methods for mesh optimization.
  That is, each method sweeps over the mesh, investigating local changes.  
  If the proposed transformation locally improves the quality of the mesh, 
  then the change is accepted.  The local changes are comprised of 
  geometric transformations (moving vertices)
  and topological transformations (changing the connectivities).
  
  There are also refinement and coarsening capabilities.  
  Refinement is done with edge splitting; coarsening with edge collapse.
  These are used for error control and efficiency, respectively.  

  A \ref geom_mesh_advantagesMWF "feature-based representation" 
  of the boundary is used in many of the mesh
  optimization algorithms.  For a 3-D mesh, the boundary is represented
  as a triangle mesh that has surface features, edge features, and corner 
  features.  Sharp edges and corners are preserved as nodes are moved 
  and cells are modified.

  One can use either an explicit (parametric) or an 
  implicit (level set) description of the boundary.  The explicit 
  description is useful for moving points along the boundary.  
  For the level set description, the boundary
  is stored in a bounding box tree, which supports efficient minimum
  distance queries.  In this manner the distance to the boundary and
  the closest point on the boundary may be determined for any point in
  space.  This representation of the boundary enables moving nodes
  onto the boundary.

  \section geom_mesh_overview_algorithms Algorithms.

  The following algorithms are currently implemented in 2-D and 3-D.

  - Algebraic quality metrics.  
  These metrics are functions of the Jacobian 
  matrix  of the mapping from the equilateral simplex to the physical simplex.
  They are differentiable and sensitive to all types of degeneracies.
  - Geometric optimization of vertex positions.   
  A vertex is moved to 
  optimize the quality of the incident simplices.  Boundary vertices 
  are moved along the feature-based description of the boundary.
  Alterntively, boundary vertices may be 
  optimized subject to a constant content constraint or may be 
  optimized and then constrained to remain on a specified curve/surface.
  - Topological optimization.  
  Local transformations (edge flips in 2-D,
  edge removal and face removal in 3-D) are applied in a hill-climbing
  strategy.
  - \ref geom_mesh_refinement "Refinement".  
  One can refine a set of cells or refine the mesh based on a maximum 
  allowed edge length function.
  If a cell is targeted for refinement, its longest edge
  will be split.  However, only mutual longest edges are allowed to be split.
  This means that adjacent cells may need to be (recursively) refined
  before the targeted cell is refined.
  - Coarsening.  
  The mesh is coarsened by removing cells through edge collapse.
  One can coarsen the mesh by collapsing edges for a set of cells or by 
  specifying a minimum allowed edge length function.  There are 
  user-defined parameters for controlling the quality of the mesh while
  performing coarsening.
  - Conversion of a simplicial mesh to a level set function.  
  There is a data 
  structure that allows a mesh to be treated as a distance function or a 
  closest point function.  It combines a lower-upper bound 
  query on a bounding box tree with distance and closest point calculations
  to the elements of the mesh.
  - Mesh transfer.  
  There is a robust mesh transfer algorithm
  for transfering fields from one simplicial mesh to another.  Given a
  point and a field, the algorithm first determines the simplex to
  which the point has minimum distance.  The distance could be
  negative or positive depending on whether the point is inside or
  outside the simplex.  Then one can perform interpolation or extrapolation
  with the fields defined in the simplex.  The minimum distance
  calculation uses a lower-upper bound query on a bounding box tree
  which has expected complexity \f$\log N\f$ where \f$N\f$ is the
  number of simplices.
  
  \section geom_mesh_overview_dataStructures Data Structures.

  There are mesh data structures for both static topology and dynamic topology.
  (See the \ref geom_mesh_data "data structures page" for details and 
  comparisons.)
  The static topology data structures 
  (geom::IndSimpSet and geom::IndSimpSetIncAdj) 
  are used in geometric optimization, the level
  set capability, and boundary description.  The dynamic topology data 
  structure (geom::SimpMeshRed) is used
  in topological optimization, refinement and coarsening.  The data structures
  are light-weight, and flexible.   The space dimension, simplex dimension
  (triangle, tetrahedron, etc.), as well as the node and element types
  are specified as template parameters.  We anticipate that
  these data structures will be sufficient for future algorithmic 
  development.

  The boundary manifold data structures are 
  \ref PointsOnManifold321T "geom::PointsOnManifold<3,2,1,T>" and
  \ref PointsOnManifoldN11T "geom::PointsOnManifold<N,1,1,T>", 
  for 3-D and 2-D, respectively.
  They capture the features of the boundary.  In 3-D, the boundary has
  surface, edge, and corner features.  Nodes may be moved along the boundary,
  inserted in the boundary, or deleted from the boundary.  
  
  

  \section geom_mesh_overview_futureWork Future Work.

  - Add more quality functions.
  - Combine geometric optimization with topological optimization to form 
  more general transformations for the hill-climbing optimization approach.
  - Make refinement more efficient by avoiding deep recursion.
*/




//----------------------------------------------------------------------------
/*!
  \page geom_mesh_organization Organization of the Simplicial Mesh Package.

  Algorithms for mesh optimization fall into three categories.
  - Geometric optimization: Vertices are moved to improve mesh quality.
  The topology is unchanged.
  - Topological optimization: Reconnect the vertices to form different cells
  with better quality.  This changes the topology of the mesh, but the 
  geometry remains constant.
  - Coarsening and refinement: Coarsen the mesh by collapsing edges; 
  refine the mesh by splitting edges.  These operations change both
  the geometry and topology of the mesh.

  The simplicial mesh package is composed of three sub-packages:
  - The \ref geom_mesh_simplex has quality functions for simplices.
  - The \ref iss has support for mesh generation and geometric optimization.
    It also has data structures for effeciently computing distance to meshes.
  - The \ref simplicial has support for topological optimization, coarsening, 
    and refinement of simplicial meshes.

  Each of these sub-packages has a header file in the \c geom/mesh directory.
  You can include all of the sub-packages by including the file 
  \c geom/mesh.h .
*/



//----------------------------------------------------------------------------
/*!
  \page geom_mesh_results Results for the Simplicial Mesh Package.

  - \ref geom_mesh_cubeStretchTwist
  - \ref geom_mesh_cylinderTwist
  - \ref geom_mesh_cubeBend
  - \ref geom_mesh_cubePullCorner
  - \ref geom_mesh_enterpriseBoundarySurvey
  - \ref geom_mesh_advantagesMWF 
*/




//----------------------------------------------------------------------------
/*!
  \page geom_mesh_example Example Programs in the Simplicial Mesh Package.

  The following programs in the example directory show how to use the data
  structures and algorithms to generate and optimize meshes.


  <b>Element Quality</b>

  - The \ref examples_geom_mesh_utility_quality "quality" program prints quality
  statistics for a mesh.
  - \ref examples_geom_mesh_simplexQuality "simplexQuality" assesses the
  quality of a single simplex.
  - \ref examples_geom_mesh_jacobianDecomposition "jacobianDecomposition"
  calculates the decomposition of the Jacobian of mapping from the 
  ideal simplex to the physical simplex.
  - \ref examples_geom_mesh_mapSimplexJac "mapSimplexJac" maps a simplex 
  using a Jacobian matrix.
  - \ref examples_geom_mesh_mapSimplexOSA "mapSimplexOSA" 
  maps a simplex using the orientation, skew and aspect ratio matrices.


  <b>Geometric Optimization</b>

  - \ref examples_geom_mesh_geometricOptimize "geometricOptimize" uses 
  geometric optimization to optimize the position of interior vertices in 
  a mesh.
  - \ref examples_geom_mesh_geomOptBoundaryCondition "geomOptBoundaryCondition"
  uses geometric optimization subject to the constraint that the boundary
  vertices remain on a specified curve/surface.
  - \ref examples_geom_mesh_geomOptContentConstraint "geomOptContentConstraint"
  uses geometric optimization of vertex positions subject to a constant
  content constraint.  That is, the area/volume of the mesh remains constant.
  - \ref examples_geom_mesh_laplacianSolve "laplacianSolve"
  applies Laplacian smoothing to the interior vertices.
  - \ref examples_geom_mesh_laplacian "laplacian"
  applies sweeps of Laplacian smoothing to the interior vertices.
  - \ref examples_geom_mesh_laplacianBoundary "laplacianBoundary"
  applies sweeps of Laplacian smoothing to the boundary vertices.


  <b>Topological Optimization</b>

  - The \ref examples_geom_mesh_flip "flip" program performs edges flips 
  in a 2-D mesh to improve quality.
  - \ref examples_geom_mesh_topologicalOptimize "topologicalOptimize" 
  does topological optimization of 3-D meshes.  It performs edge removal and 
  face removal operations to improve the mesh quality.
  - \ref examples_geom_mesh_incidenceOptimize "incidenceOptimize" optimizes
  the cell-node incidence relations.


  <b>Coarsening and Refinement</b>

  - The \ref examples_geom_mesh_coarsen "coarsen" program coarsens a mesh
  by collapsing edges.
  - The \ref examples_geom_mesh_refine "refine" program refines a mesh
  by splitting edges.
  - \ref examples_geom_mesh_coarsenCells "coarsenCells" removes
  a specified set of cells by collapsing edges.
  - \ref examples_geom_mesh_refineCells "refineCells" refines
  a specified set of cells by splitting edges.
  - \ref examples_geom_mesh_refineBoundaryDistance "refineBoundaryDistance"
  refines a mesh according to the distance from the boundary.
  - The \ref examples_geom_mesh_refineBoundary "refineBoundary" program 
  refines the boundary cells of a mesh so the boundary more closely matches
  a specified curve.


  <b>Penetration</b>

  - The \ref examples_geom_mesh_penetration "penetration" program 
  takes a simplicial mesh and a list of points and detects which points
  penetrate the mesh. For each of these points it reports the simplex
  that contains the point and the closest point on the surface of the mesh.

  <b>Subspace Methods</b>

  - The \ref examples_geom_mesh_subspace "subspace" program 
  coarsens and refines a mesh by collapsing and splitting edges.
  During this process the node-cell incidence relationships are optimized
  and the node positions are updated with Laplacian smoothing.


  <b>Boundary Operations</b>

  - The \ref examples_geom_mesh_moveBoundary "moveBoundary" program
  moves the boundary vertices of a 2-D/3-D mesh to lie on a curve/surface.
  - \ref examples_geom_mesh_fitBoundary "fitBoundary" fits the boundary 
  of a mesh to a specified curve.
  - \ref examples_geom_mesh_fitMesh "fitMesh" fits a mesh to a specified 
  curve.


  <b>Mesh Generation</b>

  - \ref examples_geom_mesh_tile "tile" meshes a curve/surface by tiling
  the interior with triangles/tetrahedra.
  - \ref examples_geom_mesh_centerPointMesh "centerPointMesh" creates 
  a mesh from the boundary and a center point.

  <b>Utilities</b>

  - The \ref examples_geom_mesh_boundary "boundary" program extracts the 
  boundary of a simplicial mesh.
  - \ref examples_geom_mesh_utility_iss2vtk "iss2vtk" converts a text file for 
  a simplicial mesh to a VTK file.  (VTK files can be viewed with Paraview.)
  - The \ref examples_geom_mesh_orientPositive "orientPositive" orients
  the simplices of a mesh to have non-negative content.
  - \ref examples_geom_mesh_randomize "randomize" randomly moves
  the interior vertices of a mesh to produce a distorted mesh.
  - The \ref examples_geom_mesh_removeLowAdjacencies "removeLowAdjacencies"
  progam removes the simplices that have low adjacencies.
  - \ref examples_geom_mesh_reverseOrientation "reverseOrientation"
  reverses the orientation of a mesh.
  - \ref examples_geom_mesh_orient "orient" tries to orient the simplices 
  in a mesh.
*/




//----------------------------------------------------------------------------
/*!
  \page geom_mesh_data Data Structures in the Simplicial Mesh Package.

  <!-- CONTINUE: Define nodes, cells, simplices and simplicial meshes. -->

  <!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
  \section geom_mesh_data_mesh Simplicial Mesh Data Structures.

  The position of the nodes (vertices) defines the \em geometry of a mesh.  
  The connection of the nodes to form elements, faces and edges defines the 
  \em topology of a mesh.  


  Let N be the space dimension and M be the simplex 
  dimension.  For a tetrahedral mesh in 3-D, N = 3 and M = 3.  The boundary
  of this tetrahdral mesh is a triangle mesh with N = 3 and M = 2.
  There are three mesh data structures: 
  - geom::IndSimpSet, Indexed Simplex Set.
  - geom::IndSimpSetIncAdj, Indexed Simplex Set with Incidence and Adjacency 
  information.
  - geom::SimpMeshRed, Simplicial Mesh, Reduced representation.
  .
  These data structures have
  the space dimension and the simplex dimension as template parameters.

  geom::IndSimpSet implements an indexed simplex set.  This is a economical
  representation of a mesh.  It is composed of an array of vertices
  and an array of indexed simplices.  A \em vertex is an N-tuple of numbers (a
  Cartesian point).  An \em indexed \em simplex
  is represented by an (M+1)-tuple of vertex indices.  
  From these, one can make a \em simplex: an (M+1)-tuple of vertices.
  The indexed simplex set is well suited for
  implementing algorithms in which the topology of the mesh does not
  change.  The ``cookie cutter'' mesh generation and file I/O are implemented
  using the geom::IndSimpSet data structure.

  Note that the only topological information stored in geom::IndSimpSet
  is the simplex-vertex incidencies.  One needs more information
  to perform geometric optimization of vertices.  
  There are auxillary data structures for
  storing vertex-simplex incidencies (geom::VertexSimplexInc)
  and simplex-simplex adjacencies (geom::SimplexAdj) 
  The geom::IndSimpSetIncAdj class inherits from geom::IndSimpSet and 
  has geom::VertexSimplexInc and geom::SimplexAdj as data members.
  Geometric mesh optimization capabilities are implemented using this
  augmented data structure.

  geom::SimpMeshRed is a \em reduced mesh representation.  A
  \em full mesh representation stores all of the topological entities.
  For a tetrahedral mesh, there are tetrahedral cells, triangle faces,
  edges, and vertices.  geom::SimpMeshRed (short for Simplicial Mesh
  Reduced) stores only the top and bottom level entities.  The
  intermediate entities (edges for triangle meshes, faces and edges
  for tetrahedral meshes) are represented implictly.  This approach
  has the advantage that the data structure can be general in the
  space dimension and simplex dimension.  It also reduces the storage
  requirements.
  
  geom::SimpMeshRed supports algorithms in which the topology of the 
  mesh changes.  The data structure is composed of a container of nodes 
  and container of cells.  The \em node type is a template parameter
  for the mesh.  A node stores at least a vertex and an iterator to itself.
  In addition, it may store an integer identifier and iterators to one or all
  of the incident cells.  The \em cell type is also a template parameter
  for the mesh.  A cell stores at least a simplex of its nodes 
  (technically a simplex of node iterators) and an 
  iterator to itself.  In addition, it may store an integer identifier and
  iterators to the adjacent cells.  The container type is also a template
  parameter of the mesh.  It is required that inserting and removing
  elements does not invalidate iterators.  By default, the container type
  is std::list.

  <!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
  \section geom_mesh_data_manifold Manifold with Features Data Structures.

  To represent the boundary of a model, one uses a "manifold with features"
  data structure.  The boundary of a triangle mesh in 2-D is a line segment
  mesh.  In this case, one can represent the boundary with 
  \ref PointsOnManifoldN11T.  The boundary of a tetrahedral mesh in 3-D
  is a triangle mesh in 3-D.  Here one can represent the boundary with 
  \ref PointsOnManifold321T.  One can also use \ref PointsOnManifold321T
  to represent the geometry of your model when working with 3-2 meshes
  (triangle meshes in 3-D).
*/




//----------------------------------------------------------------------------
/*!
  \page geom_mesh_refinement Mesh Refinement

  Mesh refinement is accomplished through edge splitting.  In 2-D, an edge 
  is incident to either one or two triangles (boundary edges and 
  internal edges, respectively).  To split the edge, we insert a node at
  the midpoint of the edge.  The midpoint is connected to the opposite
  nodes of the incident triangles.  In the figure below we split an internal
  edge.

  \image html edge_split_2.jpg "Splitting an edge in 2-D."
  \image latex edge_split_2.pdf "Splitting an edge in 2-D."
  
  In 3-D, an edge is incident to one or more tetrahedra.  To split the
  edge, we insert a node at the midpoint of the edge.  The midpoint is
  connected to each of the nodes of the incident tetrahedra which are
  not incident to the edge being split.  (If the edge is incident to
  \f$n\f$ tetrahedra, the the midpoint of the split node is incident
  to \f$2 n\f$ tetrahedra.)  In the figure below we split an edge of a
  tetrahedron.

  \image html edge_split_3.jpg "Splitting an edge in 3-D."
  \image latex edge_split_3.pdf "Splitting an edge in 3-D."
*/




//---------------------------------------------------------------------------
/*!
  \page geom_mesh_cubeStretchTwist A Stretched and Twisted Cube.

  <!------------------------------------------------------------------------->
  \section geom_mesh_cubeStretchTwistDistorted The Distorted Mesh.

  We start with a mesh of a unit cube.  It has 8,007 cells.
  The edges of the tetrahedra have lengths close to 0.1.
  (Each of the figures below show the modified condition number of the 
  elements.)

  \verbatim
  cp ../../../data/geom/mesh/33/cube_1_1_1_0.1.txt mesh.txt \endverbatim

  \image html cubeStretchTwistMesh.jpg "The initial mesh of the cube.  The minimum modified condition number is 0.58; the mean is 0.86."
  \image latex cubeStretchTwistMesh.pdf "The initial mesh of the cube.  The minimum modified condition number is 0.58; the mean is 0.86."

  Then we distort the mesh by stretching it along the z axis by a factor of 2
  and twisting it around the z axis by an angle of \f$\pi\f$.
  
  \verbatim
  python.exe ../../../data/geom/mesh/utilities/vertices_map.py stretch mesh.txt distorted.txt
  python.exe ../../../data/geom/mesh/utilities/vertices_map.py twist distorted.txt distorted.txt \endverbatim

  \image html cubeStretchTwistDistorted.jpg "The distorted mesh.  The minimum modified condition number is 0.16; the mean is 0.59."
  \image latex cubeStretchTwistDistorted.pdf "The distorted mesh.  The minimum modified condition number is 0.16; the mean is 0.59."



  <!------------------------------------------------------------------------->
  \section geom_mesh_cubeStretchTwistGT Topological and Geometric Optimization.

  First we apply topological optimization.  We use a dihedral angle deviation
  of 0.5 to define edge features.  Intersecting edge features are corner
  features.
  
  \verbatim
  utility/boundary33.exe distorted.txt boundary.txt
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 r1.txt r1t1.txt \endverbatim

  \image html cubeStretchTwistT1.jpg "The mesh after topological optimization.  The minimum modified condition number is 0.19; the mean is 0.68."
  \image latex cubeStretchTwistT1.pdf "The mesh after topological optimization.  The minimum modified condition number is 0.19; the mean is 0.68."
  
  Next we apply one sweep of geometric optimization.  
  
  \verbatim
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 r1t1.txt r1t1g1.txt \endverbatim

  \image html cubeStretchTwistT1G1.jpg "The mesh after geometric optimization.  The minimum modified condition number is 0.28; the mean is 0.73."
  \image latex cubeStretchTwistT1G1.pdf "The mesh after geometric optimization.  The minimum modified condition number is 0.28; the mean is 0.73."

  Finally, we apply an additional cycle of topological and geometric 
  optimization.
  
  \verbatim
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 r1t1g1.txt r1t2g1.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 r1t2g1.txt r1t2g2.txt \endverbatim

  \image html cubeStretchTwistT2G2.jpg "The mesh after two cycles of topological and geometric optimization.  The minimum modified condition number is 0.41; the mean is 0.78."
  \image latex cubeStretchTwistT2G2.pdf "The mesh after two cycles of topological and geometric optimization.  The minimum modified condition number is 0.41; the mean is 0.78."

  We see that the optimization has significantly improved the quality of the
  mesh.  However, because of the distortion, the cells are on average about 
  twice as large as in the original mesh.  The following sections use 
  refinement to deal with this.  


  <!------------------------------------------------------------------------->
  \section geom_mesh_cubeStretchTwistRL Refinement Based on Edge Length.

  We refine the mesh to deal with the stretching.  We split any edges that 
  are longer than 0.2.  This results in a mesh with 20,867 cells.
  
  \verbatim
  optimization/refine33.exe -length=0.2 -manifold=boundary.txt -angle=0.5 distorted.txt r1.txt \endverbatim

  \image html cubeStretchTwistRL1.jpg "The mesh after refinement based on edge length.  The minimum modified condition number is 0.18; the mean is 0.63."
  \image latex cubeStretchTwistRL1.pdf "The mesh after refinement based on edge length.  The minimum modified condition number is 0.18; the mean is 0.63."

  Next, as we did above, we apply two cycles of topological and geometric
  optimization.

  \image html cubeStretchTwistRL1T2G2.jpg "The mesh after refinement based on edge length and two cycles of topological and geometric optimization.  The minimum modified condition number is 0.40; the mean is 0.80."
  \image latex cubeStretchTwistRL1T2G2.pdf "The mesh after refinement based on edge length and two cycles of topological and geometric optimization.  The minimum modified condition number is 0.40; the mean is 0.80."

  We see that the refinement has little affect on the final quality of the 
  mesh.  It only reduces the size of the cells.

  <!------------------------------------------------------------------------->
  \section geom_mesh_cubeStretchTwistRQ Refinement Based on Cell Quality.

  A different strategy is to refine the mesh based on cell quality.  
  We refine cells 
  that have have been significantly distorted.  To do this, we select the
  cells with modified condition number less than 0.5.
  This approach results in fewer edge splits than in the above section.
  The refined mesh has 13,034 cells.
  
  \verbatim
  utility/cellAttributes33.exe -mcn distorted.txt mcn.txt
  utility/selectCells.exe -upper=0.5 mcn.txt indices.txt
  optimization/refineCells33.exe indices.txt distorted.txt rq1.txt \endverbatim

  \image html cubeStretchTwistRQ1.jpg "The mesh after refinement based on cell quality.  The minimum modified condition number is 0.19; the mean is 0.60."
  \image latex cubeStretchTwistRQ1.pdf "The mesh after refinement based on cell quality.  The minimum modified condition number is 0.19; the mean is 0.60."

  Next, as we did above, we apply two cycles of topological and geometric
  optimization.

  \image html cubeStretchTwistRQ1T2G2.jpg "The mesh after refinement based on cell quality and two cycles of topological and geometric optimization.  The minimum modified condition number is 0.41; the mean is 0.80."
  \image latex cubeStretchTwistRQ1T2G2.pdf "The mesh after refinement based on cell quality and two cycles of topological and geometric optimization.  The minimum modified condition number is 0.41; the mean is 0.80."

  In terms of mesh quality, refinement based on cell quality is little 
  different than no refinement or refinement based on edge length.
*/





//---------------------------------------------------------------------------
/*!
  \page geom_mesh_cylinderTwist Twisted Cylinder Example

  We start with a mesh of a cylinder.  It has unit height and radius 0.5.  
  The edges of the tetrahedra have lengths close to 0.1.  In the figures 
  below, we show the modified condition number of the elements.

  \verbatim
  cp ../../../data/geom/mesh/33/cylinder_0.5_1_0.1.txt mesh.txt \endverbatim

  \image html cylinderTwistMesh.jpg "The initial mesh of the cylinder."
  \image latex cylinderTwistMesh.pdf "The initial mesh of the cylinder."

  For the boundary of the cylinder, we use a mesh that has triangle edge
  lengths close to 0.05.  We define any dihedral angle that deviates more 
  than 0.5 from pi to be an edge feature.

  \verbatim
  cp ../../../data/geom/mesh/32/cylinder_0.5_1_0.05.txt boundary.txt \endverbatim

  \image html cylinderTwistBoundary.jpg "The boundary mesh and the edge features."
  \image latex cylinderTwistBoundary.pdf "The boundary mesh and the edge features."

  Next we twist the mesh along its axis of symmetry by an angle of pi.

  \verbatim
  python.exe ../../../data/geom/mesh/utilities/vertices_map.py twist mesh.txt twisted.txt \endverbatim

  \image html cylinderTwistTwisted.jpg "The twisted mesh."
  \image latex cylinderTwistTwisted.pdf "The twisted mesh."

  First we will try optimizing the twisted mesh by treating the boundary as
  a surface.  That is, vertices are only constrained to remain on the 
  boundary.  We apply five sweeps of geometric optimization.
  
  \verbatim
  optimization/geomOptBoundaryCondition3.exe -sweeps=5 boundary.txt twisted.txt surfaceOptimized.txt \endverbatim

  \image html cylinderTwistSurfaceG1T0.jpg "Geometric optimization using the boundary as a surface."
  \image latex cylinderTwistSurfaceG1T0.pdf "Geometric optimization using the boundary as a surface."

  Next we apply topological optimization.

  \verbatim
  optimization/topologicalOptimize3.exe -angle=3.15 surfaceOptimized.txt surfaceOptimized.txt \endverbatim

  \image html cylinderTwistSurfaceG1T1.jpg "Geometric and topological optimization using the boundary as a surface."
  \image latex cylinderTwistSurfaceG1T1.pdf "Geometric and topological optimization using the boundary as a surface."

  Applying three cycles of geometric and topological optimization yields the 
  mesh below.

  \image html cylinderTwistSurfaceG3T3.jpg "Three cycles of geometric and topological optimization using the boundary as a surface."
  \image latex cylinderTwistSurfaceG3T3.pdf "Three cycles of geometric and topological optimization using the boundary as a surface."




  Next we will try optimizing the twisted mesh by treating the boundary as
  a manifold with edge features.  That is, vertices on edge features must
  remain on edge features and vertices on surface features may not cross 
  edge features.  We apply five sweeps of geometric optimization.
  
  \verbatim
  optimization/geometricOptimize3.exe -sweeps=5 -boundary=boundary.txt -featureDistance=0.01 -dihedralAngle=0.5 twisted.txt manifoldOptimized.txt \endverbatim

  \image html cylinderTwistManifoldG1T0.jpg "Geometric optimization using the boundary as a manifold with edge features."
  \image latex cylinderTwistManifoldG1T0.pdf "Geometric optimization using the boundary as a manifold with edge features."

  Next we apply topological optimization.

  \verbatim
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 manifoldOptimized.txt manifoldOptimized.txt \endverbatim

  \image html cylinderTwistManifoldG1T1.jpg "Geometric and topological optimization using the boundary as a manifold with edge features."
  \image latex cylinderTwistManifoldG1T1.pdf "Geometric and topological optimization using the boundary as a manifold with edge features."

  Applying three cycles of geometric and topological optimization yields the 
  mesh below.

  \image html cylinderTwistManifoldG3T3.jpg "Three cycles of geometric and topological optimization using the boundary as a manifold with edge features."
  \image latex cylinderTwistManifoldG3T3.pdf "Three cycles of geometric and topological optimization using the boundary as a manifold with edge features."
*/







//---------------------------------------------------------------------------
/*!
  \page geom_mesh_cubeBend Bent Cube Example

  We start with a mesh of the unit cube.  
  The edges of the tetrahedra have lengths close to 0.1.  In the figures below,
  we show the modified condition number of the elements.

  \verbatim
  cp ../../../data/geom/mesh/33/cube_1_1_1_0.1.txt mesh.txt \endverbatim

  \image html cubeBendMesh.jpg "The initial mesh of the cube.  The minimum modified condition number is 0.58; the mean is 0.86."
  \image latex cubeBendMesh.pdf "The initial mesh of the cube.  The minimum modified condition number is 0.58; the mean is 0.86."

  We distort the mesh by moving the vertices according to the following
  function:

  \verbatim
  x = x
  y = y
  z = z + 0.75 - 1.5 * x * x - 1.5 * y * y \endverbatim

  The distortion reduces the quality of the mesh.

  \verbatim
  python.exe ../../../data/geom/mesh/utilities/vertices_map.py bend mesh.txt distorted.txt \endverbatim

  \image html cubeBendDistorted.jpg "The distorted mesh.  The minimum modified condition number is 0.15; the mean is 0.61."
  \image latex cubeBendDistorted.pdf "The distorted mesh.  The minimum modified condition number is 0.15; the mean is 0.61."

  We apply cycles of topological optimization followed by a sweep of 
  geometric optimization to improve the quality of the 
  distorted mesh.  We use a dihedral angle deviation of 0.5 to define edge 
  features on the boundary.  Intersecting edge features form corner features.

  \verbatim
  utility/boundary33.exe distorted.txt boundary.txt
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 distorted.txt t1.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 t1.txt t1g1.txt
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 t1g1.txt t2g1.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 t2g1.txt t2g2.txt
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 t2g2.txt t3g2.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 t3g2.txt t3g3.txt \endverbatim

  \image html cubeBendT1G1.jpg "1 optimization cycle.  The minimum modified condition number is 0.22; the mean is 0.79."
  \image latex cubeBendT1G1.pdf "1 optimization cycle.  The minimum modified condition number is 0.22; the mean is 0.79."

  \image html cubeBendT2G2.jpg "2 optimization cycles.  The minimum modified condition number is 0.37; the mean is 0.82."
  \image latex cubeBendT2G2.pdf "2 optimization cycles.  The minimum modified condition number is 0.37; the mean is 0.82."

  \image html cubeBendT3G3.jpg "3 optimization cycles.  The minimum modified condition number is 0.38; the mean is 0.84."
  \image latex cubeBendT3G3.pdf "3 optimization cycles.  The minimum modified condition number is 0.38; the mean is 0.84."

  The optimization improves the quality of the mesh while retaining its edge 
  and corner features.  Vertices on surface features have been allowed to 
  move within their surface patches.  Vertices on edge features have 
  been allowed to move along edges.  Vertices on corner features have 
  been fixed.
*/








//---------------------------------------------------------------------------
/*!
  \page geom_mesh_cubePullCorner Cube with a Pulled Corner

  We start with a mesh of the unit cube.  
  The edges of the tetrahedra have lengths close to 0.1.  We move the cube to
  lie in the first octant.  In the figures below,
  we show the modified condition number of the elements.

  \verbatim
  cp ../../../data/geom/mesh/33/cube_1_1_1_0.1.txt mesh.txt
  python.exe ../../../data/geom/mesh/utilities/vertices_map.py translate mesh.txt mesh.txt \endverbatim

  \image html cubePullCornerMesh.jpg "The initial mesh of the cube.  The minimum modified condition number is 0.58; the mean is 0.86."
  \image latex cubePullCornerMesh.pdf "The initial mesh of the cube.  The minimum modified condition number is 0.58; the mean is 0.86."

  We distort the mesh by moving the vertices according to the following
  function:

  \verbatim
  x = x + x * x + y * y + z * z
  y = y + x * x + y * y + z * z
  z = z + x * x + y * y + z * z \endverbatim

  The distortion reduces the quality of the mesh.

  \verbatim
  python.exe ../../../data/geom/mesh/utilities/vertices_map.py distort mesh.txt distorted.txt \endverbatim

  \image html cubePullCornerDistorted.jpg "The distorted mesh.  The minimum modified condition number is 0.22; the mean is 0.82."
  \image latex cubePullCornerDistorted.pdf "The distorted mesh.  The minimum modified condition number is 0.22; the mean is 0.82."

  First we use refinement to deal with the edges that have been stretched by 
  pulling out the corner.  We split any edge longer than 0.2.

  \verbatim
  utility/boundary33.exe distorted.txt boundary.txt
  optimization/refine33.exe -length=0.2 -manifold=boundary.txt -dihedralAngle=0.5 distorted.txt r1t0g0.txt \endverbatim

  \image html cubePullCornerR1T0G0.jpg "The mesh after refinement based on edge length.  There are 10,785 cells.  The minimum modified condition number is 0.20; the mean is 0.77."
  \image latex cubePullCornerR1T0G0.pdf "The mesh after refinement based on edge length.  There are 10,785 cells.  The minimum modified condition number is 0.20; the mean is 0.77."

  We apply cycles of topological optimization followed by a sweep of 
  geometric optimization to improve the quality of the 
  distorted mesh.  We use a dihedral angle deviation of 0.5 to define edge 
  features on the boundary.  Intersecting edge features form corner features.

  \verbatim
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 r1t0g0.txt r1t1g0.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 r1t1g0.txt r1t1g1.txt
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 r1t1g1.txt r1t2g1.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 r1t2g1.txt r1t2g2.txt
  optimization/topologicalOptimize3.exe -manifold=boundary.txt -angle=0.5 r1t2g2.txt r1t3g2.txt
  optimization/geometricOptimize3.exe -boundary=boundary.txt -dihedralAngle=0.5 r1t3g2.txt r1t3g3.txt \endverbatim

  \image html cubePullCornerR1T1G1.jpg "1 optimization cycle.  The minimum modified condition number is 0.27; the mean is 0.83."
  \image latex cubePullCornerR1T1G1.pdf "1 optimization cycle.  The minimum modified condition number is 0.27; the mean is 0.83."

  \image html cubePullCornerR1T2G2.jpg "2 optimization cycles.  The minimum modified condition number is 0.39; the mean is 0.85."
  \image latex cubePullCornerR1T2G2.pdf "2 optimization cycles.  The minimum modified condition number is 0.39; the mean is 0.85."

  \image html cubePullCornerR1T3G3.jpg "3 optimization cycles.  The minimum modified condition number is 0.44; the mean is 0.86."
  \image latex cubePullCornerR1T3G3.pdf "3 optimization cycles.  The minimum modified condition number is 0.44; the mean is 0.86."

  The optimization improves the quality of the mesh while retaining its edge 
  and corner features.  Vertices on surface features have been allowed to 
  move within their surface patches.  Vertices on edge features have 
  been allowed to move along edges.  Vertices on corner features have 
  been fixed.


  Another refinement approach is to refine the cells that have become 
  distorted.  We refine the cells whose modified condition number is less
  than 0.5.

  \verbatim
  utility/cellAttributes33.exe -mcn distorted.txt mcn.txt
  utility/selectCells.exe -upper=0.5 mcn.txt indices.txt
  optimization/refineCells33.exe indices.txt distorted.txt rq1t0g0.txt \endverbatim

  \image html cubePullCornerRQ1T0G0.jpg "The mesh after refinement based on cell quality.  There are 8,608 cells.  The minimum modified condition number is 0.21; the mean is 0.80."
  \image latex cubePullCornerRQ1T0G0.pdf "The mesh after refinement based on cell quality.  There are 8,608 cells.  The minimum modified condition number is 0.21; the mean is 0.80."

  We note that refining based on quality results in fewer edge splits
  than the refinement above based on edge length.  Again we apply
  cycles of topological optimization followed by a sweep of geometric
  optimization to improve the quality of the distorted mesh.  In terms
  of mesh quality, we obtain similar results to the refinement based
  on edge length.

  \image html cubePullCornerRQ1T3G3.jpg "3 optimization cycles.  The minimum modified condition number is 0.39; the mean is 0.87."
  \image latex cubePullCornerRQ1T3G3.pdf "3 optimization cycles.  The minimum modified condition number is 0.39; the mean is 0.87."
*/




//----------------------------------------------------------------------------
/*!
  \page geom_mesh_advantagesMWF Advantages of the Manifold With Features Approach.

  Here we consider an example that demonstrates the advantages of the 
  "manifold with features" approach.  We start with a mesh of the Enterprise.
  (The following commands can be executed in \c stlib/examples/geom/mesh.
  The \c stlib/results/geom/mesh/3/enterpriseManifold directory contains 
  a makefile which generates the figures in this example.)

  \verbatim
  cp ../../../data/geom/mesh/33/enterpriseL50.txt enterprise.txt \endverbatim

  \image html advantagesMWFEnterprise.jpg "The initial mesh.  The minimum modified condition number is 0.069 the mean is 0.82."
  \image latex advantagesMWFEnterprise.pdf "The initial mesh.  The minimum modified condition number is 0.069 the mean is 0.82."

  We have a higher resolution mesh to describe the boundary.

  \verbatim
  cp ../../../data/geom/mesh/32/enterpriseL20.txt enterpriseBoundary.txt \endverbatim

  \image html advantagesMWFEnterpriseBoundary.jpg "A higher resolution surface mesh of the Enterprise."
  \image latex advantagesMWFEnterpriseBoundary.pdf "A higher resolution surface mesh of the Enterprise."

  We apply two cycles of geometric and topological optimization to the mesh.
  We optimize the modified condition number quality metric.
  We use the boundary mesh, without using the features capability.  That is,
  the boundary nodes of the solid mesh are only constrained to remain on the 
  surface mesh.

  \verbatim
  optimization/topologicalOptimize3.exe -function=c -manifold=enterpriseBoundary.txt enterprise.txt enterpriseNoFeatures.txt
  optimization/geometricOptimize3.exe -function=c -boundary=enterpriseBoundary.txt enterpriseNoFeatures.txt enterpriseNoFeatures.txt
  optimization/topologicalOptimize3.exe -function=c -manifold=enterpriseBoundary.txt enterpriseNoFeatures.txt enterpriseNoFeatures.txt
  optimization/geometricOptimize3.exe -function=c -boundary=enterpriseBoundary.txt enterpriseNoFeatures.txt enterpriseNoFeatures.txt \endverbatim

  \image html advantagesMWFEnterpriseNFTGTG.jpg "The mesh after two cycles of geometric and topological optimization.  Boundary features are not used.  The minimum modified condition number is 0.23 the mean is 0.87."
  \image latex advantagesMWFEnterpriseNFTGTG.pdf "The mesh after two cycles of geometric and topological optimization.  Boundary features are not used.  The minimum modified condition number is 0.23 the mean is 0.87."

  We see that the optimization has improved the quality of the mesh elements.
  However, the result is not very satisfactory.  The shape of the object has 
  changed.  The distortion is most pronounced on the thin rectangular beams 
  and along the circular rim.

  To rectify this problem, we utilize the edge features of the boundary mesh.
  We define any edge with a dihedral angle which deviates more than 0.5 from
  \f$ \pi \f$ to be an edge feature.  In the figure below we see that this 
  choice is large enough to capture the edge features of the boundary mesh
  and small enough to not introduce spurious edge features.  Intersecting edge
  features are corner features (shown in red).

  \verbatim
  utility/extractFeatures32.exe -angle=0.5 enterpriseBoundary.txt enterpriseEdges.txt enterpriseCorners.txt \endverbatim

  \image html advantagesMWFEnterpriseEdgesCorners.jpg "The edge and corner features of the boundary mesh for a dihedral angle deviation of 0.5."
  \image latex advantagesMWFEnterpriseEdgesCorners.pdf "The edge and corner features of the boundary mesh for a dihedral angle deviation of 0.5."

  Again we apply two cycles of geometric and topological optimization to the 
  mesh, but this time we use the features capability.  In short, this means:
  - Boundary nodes on corner features cannot be moved by the geometric 
  optimization.
  - Boundary edges cannot be modified by the topological optimization if
  they lie along an edge feature.
  - When boundary nodes on edge features are moved by the geometric 
  optimization, they must remain on edge features.
  - When boundary nodes on surface features are moved by the geometric 
  optimization, they must not cross edge features.
  
  \image html advantagesMWFEnterpriseTGTG.jpg "The mesh after two cycles of geometric and topological optimization.  Boundary features are used.  The minimum modified condition number is 0.11 the mean is 0.86."
  \image latex advantagesMWFEnterpriseTGTG.pdf "The mesh after two cycles of geometric and topological optimization.  Boundary features are used.  The minimum modified condition number is 0.11 the mean is 0.86."

  We see that the optimization has not improved the quality of the elements
  as much as before, but the shape of the object has been accurately 
  maintained.

*/


//----------------------------------------------------------------------------
// CONTINUE: Update this after I revert geomOptBoundaryCondition to the old
// method.
/*!
  \page geom_mesh_enterpriseBoundarySurvey A Survey of Different Boundary Conditions in Geometric Optimization of the Enterprise Mesh.

  Here we apply geometric optimization to a mesh using a variety of boundary
  constraints.  We start with a mesh of the Enterprise that has 1,438 nodes
  and 4,926 elements.
  (The following commands can be executed in \c stlib/examples/geom/mesh.
  The \c stlib/results/geom/mesh/3/enterpriseBoundarySurvey directory contains 
  a makefile which generates the figures in this example.)

  \verbatim
  cp ../../../data/geom/mesh/33/enterpriseL50.txt enterprise.txt \endverbatim

  \image html ebsMesh.jpg "The initial mesh.  The minimum modified condition number is 0.069 the mean is 0.82."
  \image latex ebsMesh.pdf "The initial mesh.  The minimum modified condition number is 0.069 the mean is 0.82."

  We have a higher resolution mesh to describe the boundary.

  \verbatim
  cp ../../../data/geom/mesh/32/enterpriseL20.txt enterpriseBoundary.txt \endverbatim

  \image html ebsBoundary.jpg "A higher resolution surface mesh of the Enterprise."
  \image latex ebsBoundary.pdf "A higher resolution surface mesh of the Enterprise."

  
  We apply apply one sweep of geometric optimization to the mesh.
  We optimize the modified condition number quality metric.
  First, we use an implicit description of the boundary.  To optimize a 
  boundary node, its position is first optimized in 3-D space and then
  projected back onto its closest point on the boundary.  If the resulting
  change locally improves the quality of the mesh, it is accepted.

  \verbatim
  optimization/geomOptBoundaryCondition3.exe -function=c enterpriseBoundary.txt enterprise.txt enterpriseImplicitSurface.txt \endverbatim

  \image html ebsImplicitSurface.jpg "The mesh after optimization using the implicit surface method.  The minimum modified condition number is 0.19 the mean is 0.85."
  \image latex ebsImplicitSurface.pdf "The mesh after optimization using the implicit surface method.  The minimum modified condition number is 0.19 the mean is 0.85."

  \image html ebsImplicitSurfaceBack.jpg "The mesh after optimization using the implicit surface method.  View from the back."
  \image latex ebsImplicitSurfaceBack.pdf "The mesh after optimization using the implicit surface method.  View from the back."

  We see that the optimization has improved the quality of the mesh elements.
  However, the result is not very satisfactory.  The shape of the object has 
  changed.  The distortion is most pronounced on the thin rectangular beams 
  and along the circular rim.



  Next we try using a content constraint.  That is, when moving boundary
  nodes the volume of the object remains constant.

  \verbatim
  optimization/geomOptContentConstraint3.exe -function=c enterprise.txt enterpriseContentConstraint.txt \endverbatim

  \image html ebsContentConstraint.jpg "The mesh after optimization using the content constraint method.  The minimum modified condition number is 0.31 the mean is 0.87."
  \image latex ebsContentConstraint.pdf "The mesh after optimization using the content constraint method.  The minimum modified condition number is 0.31 the mean is 0.87."

  \image html ebsContentConstraintBack.jpg "The mesh after optimization using the content constraint method.  View from the back."
  \image latex ebsContentConstraintBack.pdf "The mesh after optimization using the content constraint method.  View from the back."

  Again the optimization has improved the quality of the mesh elements.
  In this respect, it is more successful than with using the implicit 
  boundary method.  But again, the result is not very satisfactory.  
  The shape of the object has been preserved along its its smooth surfaces,
  but it has been distorted near sharp edges in the model.



  To rectify this problem, we utilize the edge features of the boundary mesh.
  We define any edge with a dihedral angle which deviates more than 0.5 from
  \f$ \pi \f$ to be an edge feature.  In the figure below we see that this 
  choice is large enough to capture the edge features of the boundary mesh
  and small enough to not introduce spurious edge features.  Intersecting edge
  features are corner features (shown in red).

  \verbatim
  utility/extractFeatures32.exe -angle=0.5 enterpriseBoundary.txt enterpriseEdges.txt enterpriseCorners.txt \endverbatim

  \image html ebsEdgesCorners.jpg "The edge and corner features of the boundary mesh for a dihedral angle deviation of 0.5."
  \image latex ebsEdgesCorners.pdf "The edge and corner features of the boundary mesh for a dihedral angle deviation of 0.5."

  Now we optimize the mesh using the manifold with features approach to 
  constrain the boundary nodes.  In short, this means that for boundary nodes:
  - Nodes on corner features cannot be moved.
  - When nodes on edge features are moved, they must remain on that 
  edge feature.
  - When boundary nodes on surface features are moved, they must not cross 
  edge features.
  
  \image html ebsManifoldWithFeatures.jpg "The mesh after optimization using the manifold with features approach.  The minimum modified condition number is 0.10 the mean is 0.85."
  \image latex ebsManifoldWithFeatures.pdf "The mesh after optimization using the manifold with features approach.  The minimum modified condition number is 0.10 the mean is 0.85."

  \image html ebsManifoldWithFeaturesBack.jpg "The mesh after optimization using the manifold with features approach.  View from the back."
  \image latex ebsManifoldWithFeaturesBack.pdf "The mesh after optimization using the manifold with features approach.  View from the back."

  We see that the optimization has not improved the quality of the elements
  as much as before, but the shape of the object has been accurately 
  maintained.

  For this example, it is difficult to see how the mesh changed when using
  the manifold with features approach.  One might ask whether the shape of
  the model was preserved because the nodes were moved well or whether they 
  just were not moved.  The following figures show the edges for the initial
  mesh in red and the edges for the optimized mesh in blue.  They show that
  nodes on surface features were allowed to move along the surface and nodes
  on edge features were allowed to move along edge features.

  \image html ebsManifoldWithFeaturesEdges.jpg "The edges of the initial mesh (in red) and the edges of the optimized mesh using the manifold with features approach (in blue)."
  \image latex ebsManifoldWithFeaturesEdges.pdf "The edges of the initial mesh (in red) and the edges of the optimized mesh using the manifold with features approach (in blue)."

  \image html ebsManifoldWithFeaturesEdgesClose.jpg "Close-up view."
  \image latex ebsManifoldWithFeaturesEdgesClose.pdf "Close-up view."
*/








//----------------------------------------------------------------------------
/*!
\page geom_mesh_bibliography Bibliography for geom/mesh.


<!-------------------------------------------------------------------------->
\section geom_mesh_bibliography_data Data Structures

\anchor geom_mesh_garimella2002,
Rau V. Garimella,
"Mesh data structure selection for mesh generation and FEA applications"
International Journal for Numerical Methods in Engineering,
Vol. 55, 2002, 451-478.



<!-------------------------------------------------------------------------->
\section geom_mesh_bibliography_geometric Geometric Optimization

\anchor geom_mesh_escobar2003
J. M. Escobar, E. Rodriguez, R. Montenegro, G. Montero, and J. M. Gonzalez-Yuste
"Simultaneous untangling and smoothing of tetrahedral meshes"
Computat. Methods Appl. Mech. Engrg., Vol. 192, 2003, 2775-2787.

\anchor geom_mesh_knupp2001
Patrick M. Knupp,
"Algebraic Mesh Quality Metrics"
SIAM Journal of Scientific Computing,
Vol. 23, Num. 1, 2001, 193-218.

\anchor geom_mesh_freitag2002
Lori A. Freitag and Patrick M. Knupp,
"Tetrahedral Mesh Improvement via Optimization of the Element Condition Number"
International Journal of Numerical Methods in Engineering,
Vol. 53, 2002, 1377-1391.

\anchor geom_mesh_press2002
William H. Press, Saul A. Teukolsky, William T. Vetterling, and Brian P. Flannery,
"Numerical Recipes in C++"
Cambridge University Press, Cambridge, UK, 2002.



<!-------------------------------------------------------------------------->
\section geom_mesh_bibliography_topological Topological Optimization


\anchor geom_mesh_shewchuk2002
Jonathan Richard Shewchuk,
"Two Discrete Optimization Algorithms for the Topological Improvement of Tetrahedral Meshes"
Unpublished manuscript, http://www.cs.cmu.edu/~jrs/
University of California at Berkeley



<!-------------------------------------------------------------------------->
\section geom_mesh_bibliography_refinement Refinement and Coarsening


\anchor geom_mesh_rivara1997
Maria-Cecilia Rivara,
"New Longest-Edge Algorithms for the Refinement and/or Improvement of Unstructured Triangulations"
International Journal for Numerical Methods in Engineering,
Vol. 40, 1997, 3313-3324.

*/






#if !defined(__geom_mesh_h__)
#define __geom_mesh_h__

#include "mesh/iss.h"
#include "mesh/simplex.h"
#include "mesh/simplicial.h"
#include "mesh/structured_grid.h"
#include "mesh/tetrahedral.h"

BEGIN_NAMESPACE_GEOM

//-----------------------------------------------------------------------------
/*! \defgroup geom_mesh_api API for the geom/mesh sub-package.
  This group defines an API for the geom/mesh sub-package.  It wraps 
  functionality for the iss and simplicial sub-sub-packages.

  All of the functions take IndSimpSet<N,M,true,T> as parameters.
*/
//@{

// CONTINUE: I will probably abandon the API and remove these functions.

//! Read a mesh in ascii format.
template<int N, int M, typename T>
inline
void 
readAscii(std::istream& in, IndSimpSet<N,M,true,T>* mesh) {
  typedef IndSimpSet<N,M,true,T> ISS;
  typedef typename ISS::Vertex V;
  typedef typename ISS::IndexedSimplex IS;
  readAscii<N,M,T,V,IS>(in, mesh);
}

//! Write a mesh in ascii format.
template<int N, int M, typename T>
inline
void
writeAscii(std::ostream& out, const IndSimpSet<N,M,true,T>& mesh) {
  typedef IndSimpSet<N,M,true,T> ISS;
  typedef typename ISS::Vertex V;
  typedef typename ISS::IndexedSimplex IS;
  writeAscii<N,M,true,T,V,IS>(out, mesh);
}

//@}

END_NAMESPACE_GEOM

#endif
