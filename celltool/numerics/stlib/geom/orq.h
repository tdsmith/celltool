// -*- C++ -*-

/*! 
  \file orq.h
  \brief Includes the Orthogonal Range Query in N-D classes.
*/

//=============================================================================
//=============================================================================
/*!
  \page geom_orq ORQ Package

  The Orthogonal Range Query sub-package has many classes for performing orthogonal range queries in N-D:
  - geom::CellArray
  - geom::CellBinarySearch
  - geom::CellForwardSearch
  - geom::CellForwardSearchKey
  - geom::KDTree
  - geom::Octree
  - geom::Placebo
  - geom::PlaceboCheck
  - geom::SequentialScan
  - geom::SortFirst
  - geom::SortFirstDynamic
  - geom::SortProject
  - geom::SortRankProject
  - geom::SparseCellArray

  Use this sub-package by including the desired class or by including
  the file orq.h.

  The documentation below has a thorough description of the algorithms for
  ORQ's and performance analyses of the above data structures.  Consult
  the \ref geom_orq_usage "final section" for usage information.

  - \ref geom_orq_introduction
  - \ref geom_orq_rq
  - \ref geom_orq_orq
  - \ref geom_orq_querySizes
  - \ref geom_orq_mrq
  - \ref geom_orq_morq
  - \ref geom_orq_cc
  - \ref geom_orq_fileSizes
  - \ref geom_orq_conclusions
  - \ref geom_orq_usage
*/



//=============================================================================
//=============================================================================
/*!
\page geom_orq_introduction Introduction

Consider a database whose entries have multiple searchable attributes.
Perhaps a personnel database that stores employees' names, addresses,
salaries and dates of birth, or a map that stores the population and
location of cities, or a mesh that stores the Cartesian locations of
points.  In database terminology, the entries are called
\em records and the attributes are called \em keys.  A
collection of records is a \em file.  If there are \em K keys then
one can identify each key with a coordinate direction in Cartesian
space.  Then each record represents a point in <em>K</em>-dimensional space.
Searching a file for records whose keys satisfy certain criteria is a
\em query.  A query for which the keys must lie in specified
ranges is called an <em>orthogonal range query</em>.  This is because
each of the ranges correspond to intervals in orthogonal directions.
The records which satisfy the criteria lie in a box in <em>K</em>-dimensional
space.  The process of finding these records is called 
<em>range searching</em>.  
For example, one could search for cities which lie
between certain coordinates longitude and latitude and have
populations between 10,000 and 100,000.  This orthogonal range query is
depicted in the figure below.  The box is projected onto
the three coordinate planes.


\image html orq/orq_city.jpg "An orthogonal range query for cities."
\image latex orq/orq_city.pdf "An orthogonal range query for cities." width=0.5\textwidth




<!---------------------------------------------------------------------------->
\section geom_orq_introduction_cpt Example Application: CPT on an Irregular Mesh


Many query problems can be aided by using orthogonal range queries.
Consider a file which holds points in 3-D space.  Suppose we wish to 
find the points which lie inside a polyhedron.  The brute force 
approach would be to check each point to see if it is inside.  An efficient
algorithm would make a bounding box around the polyhedron.  To check 
if a point is inside the polyhedron, one would first check that it 
is inside the bounding box since that is a much simpler test.  In this
way one could rule out most of the points before doing the complicated
test to see which points are inside the polyhedron.  A better approach
(for most scenarios) would be to do an orthogonal range query to determine
which points lie inside the bounding box.  Then one could do the more 
detailed check on those points.  (More generally, one could compute a set
of boxes that together contain the polyhedron.  This would be more efficient
if the volume of the bounding box were much greater than the volume 
of the polyhedron.)

Consider computing the closest point transform presented in \ref cpt
not on the points of a regular grid, but on the points of an irregular 
mesh (perhaps the vertices of a tetrahedral mesh).  To do this
one would have to do polyhedron scan conversion for these irregularly 
spaced points.  That is, given a characteristic polyhedron of a face,
edge or vertex, we must determine which mesh points are inside.  This can be 
implemented efficiently using orthogonal range queries.




<!---------------------------------------------------------------------------->
\section geom_orq_introduction_contact Example Application: Contact Detection

In this section we consider the finite deformation contact problem for
finite element meshes.  A detailed account of this problem is given
in 
\ref geom_orq_laursen_1991 "On the formulation and numerical treatment of finite deformation frictional contact problems."  
We will follow the treatment in
\ref geom_orq_attaway_1998 "A parallel contact detection algorithm for transient solid dynamics simulations using PRONTO3D."  
Consider a finite element tetrahedron mesh
modeling an object (or objects).  The boundary of this mesh is a
triangle mesh that comprises the surface of the object.  The vertices
on the surface are called <em>contact nodes</em> and the triangle
faces on the surface are called <em>contact surfaces</em>.  During the
course of a simulation the object may come in contact with itself or
other objects.  Unless restoring forces are applied on the boundary,
the objects will inter-penetrate.  In order to prevent this, the
contact constraint is applied at the contact nodes.  Contact forces
are applied to those nodes that have penetrated contact surfaces.
Below is an outline of a time step in the simulation.

-# Define the contact surface.
-# Predict the location of nodes assuming no contacts by integrating 
the equations of motion.
-# Search for potential contacts between nodes and surfaces.
-# Perform a detailed contact check on the potential contacts.
-# Enforce the contacts by applying forces to remove the overlap.

The contact search in the third step is
the most time consuming part of the contact detection algorithm.  At
each time step, each triangle face on the surface is displaced.  Nodes
which are close to volumes swept out by this motion could potentially
contact the surface.  One can find these nodes with the following
three steps.  1) Compute a bounding box around the two positions (the
position at the beginning of the time step and the predicted position
at the end of the time step) of the contact surface.  2) Enlarge the
bounding box to account for motion of the nodes.  This is called the
capture box.  3) Perform an orthogonal range query on the surface
nodes to find those in the capture box.  The contact search is
depicted the figure below.



\image html orq/contact_search_labeled.jpg "Contact search for a face.  The orthogonal range query returns the nodes in the capture box."
\image latex orq/contact_search_labeled.pdf "Contact search for a face.  The orthogonal range query returns the nodes in the capture box." width=0.5\textwidth


Following the contact search, the detailed contact check, 
step 4, is performed on the potential
contacts.  In this step, contact is detected by determining if the 
node penetrates the contact surface during the time step.  This is
depicted below.

\image html orq/contact_check_labeled.jpg "The contact check for a single contact surface and node.  The node penetrates the face."
\image latex orq/contact_check_labeled.pdf "The contact check for a single contact surface and node.  The node penetrates the face." width=0.4\textwidth


Since the contact search is the most time consuming part of contact 
detection, the performance of the algorithm depends heavily on efficient
methods for doing orthogonal range queries.  The contact detection problem
has more structure than many orthogonal range query problems.  Firstly,
there are many range queries.  Since there is a query for every face,
the number of orthogonal range queries is of the same order as the number
of nodes.  Secondly, the problem is dynamic.  At each time step, the 
nodes and faces move a small amount.  From one time step to the next,
the nodes and ranges are slightly different.

We will first consider the problem of doing a single range query,
noting which methods are easily adapted to dynamic problems.  For this we 
will collect and compare previously developed algorithms and data structures.
Then we will consider the problem of performing a set of orthogonal range 
queries.  The multiple query problem has not previously been studied.
*/













//=============================================================================
//=============================================================================
/*!
\page geom_orq_rq Range Queries


As a stepping stone to orthogonal range queries in <em>K</em>-dimensional
space, we consider the problem of 1-D range queries.  We
will analyze the methods for doing range queries and see which ideas
carry over well to higher dimensions.  Consider a file of \em N records.
Some of the methods will require only that the records are comparable.
Other methods will require that the records can be mapped to numbers
so that we can use arithmetic methods to divide or hash them.  In this
case, let the records lie in the interval \f$[\alpha .. \beta]\f$.  We
wish to do a range query for the interval \f$[a .. b]\f$.  Let there be
\em I records in the query range.

We introduce the following notations for the complexity of the algorithms.
- Preprocess(\em N) denotes the preprocessing time to build the data structures.
- Reprocess(\em N) denotes the reprocessing time.  That is if the
  records change by small amounts, Reprocess(\em N) is the time to
  rebuild or repair the data structures.
- Storage(\em N) denotes the storage required by the data structures.
- Query(\em N, \em I) is the time required for a range query if there
  are \em I records in the query range.  For some methods,
  Query() will depend upon additional parameters.
- For some data structures, the average case performance is much better 
  than the worst-case complexity.  Let AverageQuery(\em N,\em I) denote
  the average case performance.

<!--
CONTINUE
All of the range query algorithms first identify a super-set \f$\tilde{I}\f$
of the records that lie in the query range.  Then the records in this set
are check for inclusion in \f$[a .. b]\f$.
-->

<!---------------------------------------------------------------------------->
\section geom_orq_rq_sequential Sequential Scan


The simplest approach to the range query problem is to examine each record
and test if it is in the range.  Below is the 
<em>sequential scan algorithm</em>.
See \ref geom_orq_bentley_1979 "Data Structures for Range Searching."
(Functions for performing range queries will 
have the RQ prefix.)

<pre>
RQ_sequential_scan(file, range):
  included_records = \f$\emptyset\f$
  <b>for</b> record <b>in</b> file:
    <b>if</b> record \f$\in\f$ range:
      included_records += record
  <b>return</b> included_records
</pre>

The algorithm has the advantage that it is trivial to implement and
trivial to adapt to higher dimensions and dynamic problems.  However,
the performance is acceptable only if the file is small or most of the
records lie in the query range.
\f[
\mathrm{Preprocess} = \mathcal{O}(N),
\quad
\mathrm{Reprocess} = 0,
\quad
\mathrm{Storage} = \mathcal{O}(N),
\quad
\mathrm{Query} = \mathcal{O}(N)
\f]




<!--------------------------------------------------------------------------->
\section geom_orq_rq_bsosd Binary Search on Sorted Data


If the records are sorted, then we can find any given record with a
binary search at a cost of \f$\mathcal{O}(\log N)\f$.  To do a range query
for the interval \f$[a .. b]\f$, we use a binary search to find the first
record \em x that satisfies \f$x \geq a\f$.  Then we collect records \em x in
order while \f$x \leq b\f$.  Alternatively, we could also do a binary
search to find the last record in the interval.  Then we could iterate
from the first included record to the last without checking the
condition \f$x \leq b\f$.  Either way, the computational complexity of the
range query is \f$\mathcal{O}(\log N + I)\f$.


To find the first record in the range we use
binary_search_lower_bound().  \em begin and
\em end are random access iterators to the sorted records.  The
function returns the first iterator whose key is greater than or equal
to \em value.  (Note that this binary search is implemented in the 
C++ STL function std::lower_bound().
See \ref geom_orq_austern_1999 "Generic programming and the STL: using and extending the C++ Standard Template Library.")

<pre>
binary_search_lower_bound(begin, end, value):
  <b>if</b> begin == end:
    <b>return</b> end
  middle = (begin + end) / 2
  <b>if</b> *middle < value:
    <b>return</b> binary_search_lower_bound(middle + 1, end, value)
  <b>else</b>:
    <b>return</b> binary_search_lower_bound(begin, middle, value)
</pre>

To find the last record in the range we use 
binary_search_upper_bound(), which returns
the last iterator whose key is less than or equal to \em value.
(This binary search is implemented in the C++ STL function
std::upper_bound().)

<pre>
binary_search_upper_bound(begin, end, value):
  <b>if</b> begin == end:
    <b>return</b> end
  middle = (begin + end) / 2
  <b>if</b> value < *middle:
    <b>return</b> binary_search_lower_bound(begin, middle, value)
  <b>else</b>:
    <b>return</b> binary_search_lower_bound(middle + 1, end, value)
</pre>

Below are the two methods of performing a range query with a binary
search on sorted records.  If the number of records in the interval is
small, specifically \f$I \ll \log N\f$ then
RQ_binary_search_single will be more efficient.
RQ_binary_search_double has better performance when
there are many records in the query range.

<pre>
RQ_binary_search_single(sorted_records, range = [a..b]):
  included_records = \f$\emptyset\f$
  iter = binary_search_lower_bound(sorted_records.begin, sorted_records.end, a)
  <b>while</b> *iter \f$\leq\f$ b:
    included_records += iter
    ++iter
  <b>return</b> included_records
</pre>

<pre>
RQ_binary_search_double(sorted_records, range = [a..b]):
  included_records = \f$\emptyset\f$
  begin = binary_search_lower_bound(sorted_records.begin, sorted_records.end, a)
  end = binary_search_upper_bound(sorted_records.begin, sorted_records.end, b)
  <b>for</b> iter <b>in</b> [begin..end):
    included_records += iter
  <b>return</b> included_records
</pre>

The preprocessing time is \f$\mathcal{O}(N \log N)\f$ because the records
must be sorted.  The reprocessing time is \f$\mathcal{O}(N)\f$, because a
nearly sorted sequence can be sorted in linear time with an insertion
sort.
(See \ref geom_orq_cormen_2001 "Introduction to Algorithms, Second Edition.")
The storage requirement is linear because
the data structure is simply an array of pointers to the records.
\f[
  \mathrm{Preprocess} = \mathcal{O}(N \log N),
  \quad
  \mathrm{Reprocess} = \mathcal{O}(N),
\f]
\f[
  \mathrm{Storage} = \mathcal{O}(N),
  \quad
  \mathrm{Query} = \mathcal{O}(\log N + I)
\f]













<!---------------------------------------------------------------------------->
\section geom_orq_rq_trees Trees


The records in the file can be stored in a binary search tree data
structure.
(See \ref geom_orq_bentley_1979 "Data Structures for Range Searching" and
\ref geom_orq_cormen_2001 "Introduction to Algorithms, Second Edition.")
The records are stored in the leaves.  Each branch of the tree has a
discriminator.  Records with keys less than the discriminator are
stored in the left branch, the other records are stored in the right
branch.  There are a couple of sensible criteria for determining
a discriminator which splits the records.  We can split by the median,
in which case half the records go to the left branch and half to the
right.  We recursively split until we have no more than a certain
number of records.  Let this number be leaf_size.  These records,
stored in a list or an array, make up a leaf of the tree.  The depth
of the tree depends only on the number of records.

We can also choose the discriminator to be the midpoint of the
interval.  If the records span the interval \f$[\alpha .. \beta]\f$ then all
records \em x that satisfy \f$x < (\alpha + \beta)/2\f$ go to the left branch and the
other records go to the right.  Again, we recursively split until
there are no more than leaf_size records at which point we store the
records in a leaf.  Note that the depth of the tree depends on the
distribution of the records.

Below is the code for constructing a binary search tree.  The function
returns the root of the tree.

<pre>
tree_make(records):
  <b>if</b> records.size \f$\leq\f$ leaf_size:
    Make a leaf.
    leaf.insert(records)
    <b>return</b> leaf
  <b>else</b>:
    Make a branch.
    Choose the branch.discriminator.
    left_records = {x \f$\in\f$ records | x < discriminator}
    branch.left = tree_make(left_records)
    right_records = {x \f$\in\f$ records | x \f$\geq\f$ discriminator}
    branch.right = tree_make(right_records)
    <b>return</b> branch
</pre>

We now consider range queries on records stored in binary search
trees.  Given a branch and a query range \f$[a .. b]\f$, the domain of the
left sub-tree overlaps the query range if the discriminator is greater
than or equal to \em a.  In this case, the left sub-tree is searched.
If the discriminator is less than or equal to \em b, then the domain of
the right sub-tree overlaps the query range and the right sub-tree
must be searched.  This gives us a prescription for recursively
searching the tree.  When a leaf is reached, the records are checked
with a sequential scan.  RQ_tree() performs a range query when
called with the root of the binary search tree.

<pre>
RQ_tree(node, range = [a..b]):
  <b>if</b> node is a leaf:
    <b>return</b> RQ_sequential_scan(node.records, range)
  <b>else</b>:
    included_records = \f$\emptyset\f$
    <b>if</b> node.discriminator \f$\geq\f$ a:
      included_records += RQ_tree(node.left, range)
    <b>if</b> node.discriminator \f$\leq\f$ b:
      included_records += RQ_tree(node.right, range)
    <b>return</b> included_records
</pre>

If the domain of a leaf or branch is a subset of the query range then it 
is not necessary to check the records for inclusion.  We can simply report
all of the records in the leaf or sub-tree.  (See the tree_report()
function below.)  This requires that we store 
the domain at each node (or compute the domain as we traverse the tree).
The RQ_tree_domain() function first checks if the domain of the node
is a subset of the query range and if not, then checks if the domain 
overlaps the query range.

<pre>
tree_report(node):
  <b>if</b> node is a leaf:
    <b>return</b> node.records
  <b>else</b>:
    <b>return</b> (tree_report(node.left) + tree_report(node.right))
</pre>

<pre>
RQ_tree_domain(node, range = [a..b]):
  <b>if</b> node.domain \f$\subseteq\f$ range:
    <b>return</b> tree_report(node)
  <b>else</b>:
    <b>if</b> node is a leaf:
      <b>return</b> RQ_sequential_scan(node.records, range)
    <b>else</b>:
      included_records = \f$\emptyset\f$
      <b>if</b> node.discriminator \f$\geq\f$ a:
        included_records += RQ_tree(node.left, range)
      <b>if</b> node.discriminator \f$\leq\f$ b:
        included_records += RQ_tree(node.right, range)
      <b>return</b> included_records
</pre>

RQ_tree() and RQ_tree_domain() have the same
computational complexity.  The former has better performance when the
query range contains few records.  The latter performs better when the
number of records in the range is larger than leaf_size.

For median splitting, the depth of the tree depends only on the number of 
records.  The computational complexity depends only on the total number 
of records, \em N, and the number of records which are reported or checked 
for inclusion, \f$\tilde{I}\f$.
\f[
  \mathrm{Preprocess} = \mathcal{O}(N \log N),
  \quad
  \mathrm{Reprocess} = \mathcal{O}(N),
\f]
\f[
  \mathrm{Storage} = \mathcal{O}(N),
  \quad
  \mathrm{Query} = \mathcal{O}(\log N + \tilde{I})
\f]


For midpoint splitting, the depth of the tree \em D depends on the 
distribution of records.  Thus the computational complexity depends on this 
parameter.  The average case performance of a range query is usually
much better than the worst-case computational complexity.  
\f[
  \mathrm{Preprocess} = \mathcal{O}((D + 1) N),
  \quad
  \mathrm{Reprocess} = \mathcal{O}((D + 1) N),
  \quad
  \mathrm{Storage} = \mathcal{O}((D + 1) N),
\f]
\f[
  \mathrm{Query} = \mathcal{O}(N),
  \quad
  \mathrm{AverageQuery} = \mathcal{O}(D + \tilde{I})
\f]











<!---------------------------------------------------------------------------->
\section geom_orq_rq_cell Cells or Bucketing


We can apply a bucketing strategy to the range query problem.
(See \ref geom_orq_bentley_1979 "Data Structures for Range Searching.")
Consider a uniform partition of the
interval \f$[\alpha .. \beta]\f$ with the points \f$x_0, \ldots, x_M\f$.
\f[
x_0 = \alpha, \quad x_M > \beta, \quad x_{m+1} - x_m = \frac{x_M - x_0}{M}
\f]
The \f$m^{\mathrm{th}}\f$ cell (or bucket) \f$C_m\f$ holds the records in the interval
\f$[x_m .. x_{m+1})\f$.  We have an array of \em M cells, each of which holds a list 
or an array of the records in its interval.  We can place a record in a 
cell by converting the key to a cell index.  Let the cell array data 
structure have the attribute \em min which returns the minimum 
key in the interval \f$\alpha\f$ and the attribute \em delta which
returns the size of a cell.  The process of putting the records in the 
cell array is called a <em>cell sort</em>.

<pre>
cell_sort(cells, file):
  <b>for</b> record <b>in</b> file:
    cells[key_to_cell_index(cells, record.key)] += record
</pre>

<pre>
key_to_cell_index(cells, key):
  <b>return</b> \f$\lfloor\f$(key - cells.min) / cells.delta\f$\rfloor\f$
</pre>


We perform a range query by determining the range of cells \f$[i .. j]\f$
whose intervals overlap the query range \f$[a .. b]\f$.  Let \em J be the
number of overlapping cells.  The contents of the cells in the range
\f$[i+1 .. j-1]\f$ lie entirely in the query range. The contents of the
two boundary cells \f$C_i\f$ and \f$C_j\f$ lie partially in the query range.
We must check the records in these two cells for inclusion in the query
range.

<pre>
RQ_cell(cells, range = [a .. b]):
  included_records = \f$\emptyset\f$
  i = key_to_cell_index(cells, a)
  j = key_to_cell_index(cells, b)
  <b>for</b> x <b>in</b> cells[i]:
    <b>if</b> x \f$\geq\f$ a:
      included_records += x
  <b>for</b> k <b>in</b> [i+1 .. j-1]:
    <b>for</b> x <b>in</b> cells[k]:
      included_records += x
  <b>for</b> x <b>in</b> cells[j]:
    <b>if</b> x \f$\leq\f$ b:
      included_records += x
  <b>return</b> included_records
</pre>

The preprocessing time is linear in the number of records \em N and the
number of cells \em M and is accomplished by a cell sort.  Reprocessing
is done by scanning the contents of each cell and moving records when
necessary.  Thus reprocessing has linear complexity as well.  Since
the data structure is an array of cells each containing a list or
array of records, the storage requirement is \f$\mathcal{O}(N + M)\f$.
Let \f$\tilde{I}\f$ be the number records in the \em J cells that overlap
the query range.  The computational complexity of a query is
\f$\mathcal{O}(J + \tilde{I})\f$.  If the cell size is no larger than the
query range, then we expect that \f$\tilde{I} \approx I\f$.  
If the number of records is greater than the number of cells,
then the expected computational complexity is \f$\mathcal{O}(I)\f$.
\f[
  \mathrm{Preprocess} = \mathcal{O}(N + M),
  \quad
  \mathrm{Reprocess} = \mathcal{O}(N + M),
  \quad
  \mathrm{Storage} = \mathcal{O}(N + M), 
\f]
\f[
  \mathrm{Query} = \mathcal{O}(J + \tilde{I}),
  \quad
  \mathrm{AverageQuery} = \mathcal{O}(I)
\f]
*/













//=============================================================================
//=============================================================================
/*!
\page geom_orq_orq Orthogonal Range Queries

In this section we develop methods for doing orthogonal range queries
in <em>K</em>-dimensional space as extensions of the methods for doing
1-D range queries.  We will consider several standard
methods.  In addition, we introduce a new method of using cells
coupled with a binary search.


The execution time and memory usage of tree methods and cell methods
depend on the leaf size and cell size, respectively.  For these
methods we will examine their performance in 3-D on two test problems.



<!---------------------------------------------------------------------------->
\section geom_orq_orq_test Test Problems


The records in the test problems are points in 3-D.  We 
do an orthogonal range query around each record.  The query range is a 
small cube.  



<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_orq_test_chair Chair



For the chair problem, the points are vertices in the surface mesh of a 
chair.  See the figure below for a plot of the vertices in a 
low resolution mesh.  For the tests in this section, the mesh has 
116,232 points.  There is unit spacing 
between adjacent vertices.  The query size is 8 in each dimension.
The orthogonal range queries return a total of 11,150,344 records.


\image html orq/chair.jpg "Points in the surface mesh of a chair.  7200 points."
\image latex orq/chair.pdf "Points in the surface mesh of a chair.  7200 points." width=0.4\textwidth




<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_orq_test_random Random Points


For the random points problem, the points are uniformly randomly
distributed in the unit cube, \f$[0 .. 1]^3\f$.  There are 100,000
points.  To test the effect of varying the leaf size or cell size, the  
query range will have a size of 0.1 in each dimension.  For this case,
the orthogonal range queries return a total of 9,358,294 records.
To show how the best leaf size or cell size varies with different query 
ranges, we will use ranges which vary in size from 0.01 to 0.16.





<!---------------------------------------------------------------------------->
\section geom_orq_orq_sequential Sequential Scan


The simplest approach to the orthogonal range query problem is to
examine each record and test if it is in the range.  Below is the
sequential scan algorithm.

<pre>
ORQ_sequential_scan(file, range)
  included_records = \f$\emptyset\f$
  <b>for</b> record <b>in</b> file:
    <b>if</b> record \f$\in\f$ range:
      included_records += record
  <b>return</b> included_records
</pre>

The sequential scan algorithm is implemented by storing pointers to
the records in an array or list.  Thus the preprocessing time and
storage complexity is \f$\mathcal{O}(N)\f$.  Since each record is examined
once during an orthogonal range query, the complexity is
\f$\mathcal{O}(N)\f$.  The performance of the sequential scan algorithm is
acceptable only if the file is small.  However, the sequential scan
(or a modification of it) is used in all of the orthogonal range
query algorithms to be presented.
\f[
\mathrm{Preprocess} = \mathcal{O}(N),
\quad
\mathrm{Reprocess} = 0,
\quad
\mathrm{Storage} = \mathcal{O}(N),
\quad
\mathrm{Query} = \mathcal{O}(N)
\f]








<!---------------------------------------------------------------------------->
\section geom_orq_orq_projection Projection


We extend the idea of a binary search on sorted data presented 
\ref geom_orq_rq_bsosd "previously" to higher dimensions.  We simply sort the
records in each dimension.  Let \em sorted[k] be an array of
pointers to the records, sorted in the \f$k^{\mathrm{th}}\f$
dimension.  This is called the projection method because the records
are successively projected onto each coordinate axis before each sort.
Doing a range query in one coordinate direction gives us all the
records that lie in a slice of the domain.  This is depicted below in three
dimensions.  The orthogonal range
along with the slices obtained by doing range queries in each
direction are shown.  To perform an orthogonal range query, we
determine the number of records in each slice by finding the beginning
and end of the slice with binary searches.  Then we choose the slice
with the fewest records and perform the sequential scan on those
records.

\image html orq/projection.jpg "The projection method.  The query range is the intersection of the three slices."
\image latex orq/projection.pdf "The projection method.  The query range is the intersection of the three slices." width=0.5\textwidth


<pre>
ORQ_projection(range):
  // Do binary searches in each direction to find the size of the slices.
  <b>for</b> k \f$\in\f$ [0..K):
    slice[k].begin = binary_search_lower_bound(sorted[k].begin, sorted[k].end, range.min[k])
    slice[k].end = binary_search_upper_bound(sorted[k].begin, sorted[k].end, range.max[k])
  smallest_slice = slice with the fewest elements.
  <b>return</b> ORQ_sequential_scan(smallest_slice, range)
</pre>

The projection method requires storing \em K arrays of pointers to the records
so the storage requirement is \f$\mathcal{O}(K N)\f$.
Preprocessing is comprised of sorting in each direction and so has
complexity \f$\mathcal{O}(K N \log N)\f$.  Reprocessing can be
accomplished by insertion sorting the \em K
arrays of pointers to records.  
(See \ref geom_orq_cormen_2001 "Introduction to Algorithms, Second Edition.")
Thus it has complexity \f$\mathcal{O}(K N)\f$.

The orthogonal range query is comprised of two steps: 1) Determine the
\em K slices with 2<em>K</em> binary searches on the the \em N records at a cost
of \f$\mathcal{O}(K \log N)\f$.  2) Perform a sequential scan on the
smallest slice.  Thus the computational complexity for a query is
\f$\mathcal{O}(K \log N + \text{smallest slice size})\f$.  Typically the
number of records in the smallest slice is much greater than \f$K \log N\f$,
so the sequential scan is more costly than the binary searches.
Consider the case that the records are uniformly distributed in
\f$[0..1]^K\f$.  The expected distance between adjacent records is of the
order \f$N^{-1/K}\f$.  Suppose that the query range is small and has
length \f$\mathcal{O}(N^{-1/K})\f$ in each dimension.  Then the volume of
any of the slices is of the order \f$N^{-1/K}\f$ and thus contains
\f$\mathcal{O}(N^{1 - 1/K})\f$ records.  The sequential scan on these
records will be more costly than the binary searches.  Below we give
the expected cost for this case.  In general, the projection method
has acceptable performance only if the total number of records is
small or if the number of records in some slice is small.

\f[
\mathrm{Preprocess} = \mathcal{O}(K N \log N),
\quad
\mathrm{Reprocess} = \mathcal{O}(K N),
\quad
\mathrm{Storage} = \mathcal{O}(K N),
\f]
\f[
\mathrm{Query} = \mathcal{O}(K \log N + \text{smallest slice size}),
\mathrm{AverageQuery} = \mathcal{O}\left( K \log N + N^{1 - 1 / K} \right)
\f]









<!---------------------------------------------------------------------------->
\section geom_orq_orq_point Point-in-Box Method


A modification of the projection method was developed by J. W. Swegle.
(See \ref geom_orq_attaway_1998 "A parallel contact detection algorithm for transient solid dynamics simulations using PRONTO3D" and
\ref geom_orq_heinstein_1993 "A general-purpose contact detection algorithm for nonlinear structural analysis codes" where
they apply the method to the contact detection problem.)
In addition to sorting the records in each coordinate direction, the
rank of each record is stored for each direction.  When iterating
through the records in the smallest slice the rank arrays are used so
that one can compare the rank of keys instead of the keys themselves.
This allows one to do integer comparisons instead of floating point
comparisons.  On some architectures, like a Cray Y-MP, this
modification will significantly improve performance, on others, like
most x86 processors, it has little effect.  Also, during the
sequential scan step, the records are not accessed, only their ranks
are compared.  This improves the performance of the sequential scan.

The projection method requires \em K arrays of pointers to records.  For the 
Point-in-Box method, there is a single array of pointers to the records.
There are \em K arrays of pointers to the record pointers which sort the records
in each coordinate direction.  Finally there are \em K arrays of integers
which hold the rank of each record in the given coordinate.  Thus the 
storage requirement is \f$\mathcal{O}((2 K + 1) N)\f$.  The point-in-box method
has the same computational complexity as the projection method, but has
a higher storage overhead.  
Below are the methods
for initializing the arrays and performing an orthogonal range query.

<pre>
initialize():
  // Initialize the vectors of sorted pointers.
  <b>for</b> i \f$\in\f$ [0..num_records):
    <b>for</b> k \f$\in\f$ [0..K):
      sorted[k][i] = record_pointers.begin + i
  // Sort in each direction.
  <b>for</b> k \f$\in\f$ [0..K):
    sort_by_coordinate(sorted[k].begin, sorted[k].end, k)
  // Make the rank vectors.
  <b>for</b> i \f$\in\f$ [0..num_records):
    <b>for</b> k \f$\in\f$ [0..K):
      rank[k][sorted[k][i] - record_pointers.begin] = i
  <b>return</b>
</pre>

<pre>
ORQ_point_in_box(range):
  // Do binary searches in each direction to find the size of the slices.
  <b>for</b> k \f$\in\f$ [0..K):
    slice[k].begin = binary_search_lower_bound(sorted[k].begin, sorted[k].end, range.min[k])
    slice[k].end = binary_search_upper_bound(sorted[k].begin, sorted[k].end, range.max[k])
    rank_range.min[k] = slice[k].begin - sorted[k].begin
    rank_range.max[k] = slice[k].end - sorted[k].end
  smallest_slice = slice with the fewest elements.
  // Do a sequential scan on the smallest slice.
  included_records = \f$\emptyset\f$
  <b>for</b> ptr \f$\in\f$ [smallest_slice.begin .. smallest_slice.end):
    <b>for</b> k \f$\in\f$ [0..K):
      record_rank[k] = rank[k][*ptr - record_pointers.begin]
    <b>if</b> record_rank \f$\in\f$ rank_range:
      included_records += **ptr
  <b>return</b> included_records
</pre>

\f[
\mathrm{Preprocess} = \mathcal{O}(K N \log N),
\quad
\mathrm{Reprocess} = \mathcal{O}(K N),
\quad
\mathrm{Storage} = \mathcal{O}((2 K + 1) N),
\f]
\f[
\mathrm{Query} = \mathcal{O}(K \log N + \mathrm{smallest slice size}),
\quad
\mathrm{AverageQuery} = \mathcal{O}\left( K \log N + N^{1 - 1 / K} \right)
\f]





<!---------------------------------------------------------------------------->
\section geom_orq_orq_kdtree Kd-Trees


We generalize the trees with median splitting presented 
\ref geom_orq_rq_trees "previously" to higher dimensions.  Now instead of a
single median, there are \em K medians, one for each coordinate.  We
split in the key with the largest spread.  (We could use the distance
from the minimum to maximum keys or some other measure of how spread
out the records are.)  The records are recursively split by choosing a
key (direction), and putting the records less than the median in the
left branch and the other records in the right branch.  The recursion
stops when there are no more than leaf_size records.  These records
are then stored in a leaf.  The figure below depicts a
kd-tree in 2-D with a leaf size of unity.  Horizontal and
vertical lines are drawn through the medians to show the successive
splitting.  A kd-tree divides a domain into hyper-rectangles.  Note
that the depth of the kd-tree is determined by the number of records
alone and is \f$\lceil \log_2 N \rceil\f$.



\image html orq/kdtree.jpg "A kd-tree in 2-D."
\image latex orq/kdtree.pdf "A kd-tree in 2-D." width=\textwidth


Below is the function construct(), which constructs the
kd-tree and returns its root.  Leaves in the tree simply store
records.  Branches in the tree must store the dimension with the
largest spread, split_dimension, the median value in that
dimension, \em discriminator, and the \em left and
\em right sub-trees.

<pre>
construct(file):
  <b>if</b> file.size > leaf_size:
    <b>return</b> construct_branch(file)
  <b>else</b>:
    <b>return</b> construct_leaf(file)
</pre>

<pre>
construct_branch(file):
  branch.split_dimension = dimension with largest spread
  branch.discriminator = median key value in split_dimension
  left_file = records with key < discriminator in split_dimension
  <b>if</b> left_file.size > leaf_size:
    branch.left = construct_branch(left_file)
  <b>else</b>:
    branch.left = construct_leaf(left_file)
  right_file = records with key \f$\geq\f$ discriminator in split_dimension
  <b>if</b> right_file.size > leaf_size:
    branch.right = construct_branch(right_file)
  <b>else</b>:
    branch.right = construct_leaf(right_file)
  <b>return</b> branch
</pre>

<pre>
construct_leaf(file):
  leaf.records = file
  <b>return</b> leaf
</pre>


We define the orthogonal range query recursively.  When we are at a branch
in the tree, we check if the domains of the left and right sub-trees
intersect the query range.  We can do this by examining the discriminator.
If the discriminator is less than the lower bound of the query range
(in the splitting dimension), then only the right tree intersects 
the query range so we return the ORQ on that tree.
If the discriminator is greater than the upper bound of the query range,
then only the left tree intersects the query range so we return the ORQ 
on the left tree.  Otherwise we return the union of the ORQ's on the 
left and right trees.  When we reach a leaf in the tree, we use the 
sequential scan algorithm to check the records.

<pre>
ORQ_KDTree(node, range):
  <b>if</b> node is a leaf:
    <b>return</b> ORQ_sequential_scan(node.records, range)
  <b>else</b>:
    <b>if</b> node.discriminator < range.min[node.split_dimension]:
      <b>return</b> ORQ_KDTree(node.right, range)
    <b>else if</b> node.discriminator > range.max[node.split_dimension]:
      <b>return</b> ORQ_KDTree(node.left, range)
    <b>else</b>:
      <b>return</b> (ORQ_KDTree(node.left, range) + ORQ_KDTree(node.right, range))
</pre>

Note that with the above implementation of ORQ's, every record that is 
returned is checked for inclusion in the query range with the sequential 
scan algorithm.  Thus the kd-tree identifies the records that might be 
in the query range and then these records are checked with the brute 
force algorithm.  If the query range contains most of the records, then
we expect that the kd-tree will perform no better than the sequential scan
algorithm.  Below we give an algorithm that has better performance for 
large queries.  As we traverse the tree, we keep track of the \em domain
containing the records in the current sub-tree.  If the current domain
is a subset of the query range, then we can simply report the records
and avoid checking them with a sequential scan.  Note that this modification
does not affect the computational complexity of the algorithm but 
it will affect performance.  The additional work to maintain the domain
will increase the query time for small queries (small meaning that
the number of records returned is not much greater than the leaf size).
However, this additional bookkeeping will pay off when the query range
spans many leaves.

<pre>
ORQ_KDTree_domain(node, range, domain):
  <b>if</b> node is a leaf:
    <b>if</b> domain \f$\subseteq\f$ range:
      <b>return</b> node.records
    <b>else</b>:
      <b>return</b> ORQ_sequential_scan(node.records, range)
  <b>else</b>:
    <b>if</b> node.discriminator \f$\geq\f$ range.min[node.split_dimension]:
      domain_max = domain.max[node.split_dimension]
      domain.max[node.split_dimension] = node.discriminator
      <b>if</b> domain \f$\subseteq\f$ range:
        included_records += report(node.left)
      <b>else</b>:
        included_records += ORQ_KDTree(node.left, domain, range)
      domain.max[node.split_dimension] = domain_max
    <b>if</b> node.discriminator \f$\leq\f$ range.max[node.split_dimension]:
      domain_min = domain.min[node.split_dimension]
      domain.min[node.split_dimension] = node.discriminator
      <b>if</b> domain \f$\subseteq\f$ range:
        included_records += report(node.right)
      <b>else</b>:
        included_records += ORQ_KDTree(node.right, domain, range)
      domain.min[node.split_dimension] = domain_min
    <b>return</b> included_records
</pre>

The worst-case query time for kd-trees is \f$\mathcal{O}(N^{1 - 1/K} + I)\f$,
which is not very encouraging.  
(See \ref geom_orq_bentley_1979 "Data Structures for Range Searching.")
However, if the query range is nearly cubical
and contains few elements the average case performance is much
better:
\f[
\mathrm{Preprocess} = \mathcal{O}(N \log N),
\quad
\mathrm{Reprocess} = \mathcal{O}(N \log N),
\quad
\mathrm{Storage} = \mathcal{O}(N),
\f]
\f[
\mathrm{Query} = \mathcal{O}\left( N^{1 - 1 / k} + I \right),
\quad
\mathrm{AverageQuery} = \mathcal{O}(\log N + I)
\f]


The figures below show the
execution times and storage requirements for the chair
problem. The best execution times are obtained for leaf sizes of 4 or 8.
There is a moderately high memory overhead for small leaf sizes.
For the random points problem, a leaf size of 8 gives the best 
performance and has a modest memory overhead.



\image html orq/KDTreeLeafSizeChairTime.jpg "The effect of leaf size on the execution time for the kd-tree and the chair problem."
\image latex orq/KDTreeLeafSizeChairTime.pdf "The effect of leaf size on the execution time for the kd-tree and the chair problem." width=0.5\textwidth

\image html orq/KDTreeLeafSizeChairMemory.jpg "The effect of leaf size on the memory usage for the kd-tree and the chair problem."
\image latex orq/KDTreeLeafSizeChairMemory.pdf "The effect of leaf size on the memory usage for the kd-tree and the chair problem." width=0.5\textwidth

\image html orq/KDTreeLeafSizeRandomTime.jpg "The effect of leaf size on the execution time for the kd-tree and the random points problem."
\image latex orq/KDTreeLeafSizeRandomTime.pdf "The effect of leaf size on the execution time for the kd-tree and the random points problem." width=0.5\textwidth

\image html orq/KDTreeLeafSizeRandomMemory.jpg "The effect of leaf size on the memory usage for the kd-tree and the random points problem."
\image latex orq/KDTreeLeafSizeRandomMemory.pdf "The effect of leaf size on the memory usage for the kd-tree and the random points problem." width=0.5\textwidth






In the figure below we show
the best leaf size versus the average number of records in a query 
for the random points
problem.  We see that the best
leaf size is correlated to the number of records in a query.  
For small query sizes, the best leaf size is on the order of the number of
records in a query.  For larger queries, it is best to choose a larger 
leaf size. However, the best leaf size is much smaller
than the number of records in a query.  This leaf size balances the 
costs of accessing leaves and testing records for inclusion in the range.
It reflects that the cost of accessing many leaves is amortized by the 
structure of the tree.


\image html orq/KDTreeLeafSizeRandomBestSize.jpg "The best leaf size as a function of records per query for the kd-tree and the random points problem."
\image latex orq/KDTreeLeafSizeRandomBestSize.pdf "The best leaf size as a function of records per query for the kd-tree and the random points problem." width=0.5\textwidth

\image html orq/KDTreeLeafSizeRandomBestRatio.jpg "The ratio of the number of records per query and the leaf size."
\image latex orq/KDTreeLeafSizeRandomBestRatio.pdf "The ratio of the number of records per query and the leaf size." width=0.5\textwidth








<!---------------------------------------------------------------------------->
\section geom_orq_orq_octree Quadtrees and Octrees


We can also generalize the trees with midpoint splitting presented 
\ref geom_orq_rq_trees "previously" to higher dimensions.   Now instead 
of splitting an interval in two, we split a <em>K</em>-dimensional 
domain into \f$2^K\f$ equal size
hyper-rectangles.  Each non-leaf node of the tree has \f$2^K\f$ branches. 
We recursively split the domain until there are no more than leaf_size
records, which we store at a leaf.  In 2-D these trees are called
quadtrees, in 3-D they are octrees.
The figure below depicts a quadtree.  Note that the depth of
these trees depends on the distribution of records.  If some records are very
close, the tree could be very deep.


\image html orq/quadtree.jpg "A quadtree in 2-D."
\image latex orq/quadtree.pdf "A quadtree in 2-D." width=\textwidth



<!--Hanan Samet: The Quadtree and Related Hierarchical Data Structures. ACM Computing Surveys 16(2):187-260(1984)-->


Let \em D be the depth of the octree.  The worst-case query time is as bad as 
sequential scan, but in practice the octree has much better performance.
\f[
\mathrm{Preprocess} = \mathcal{O}((D + 1) N),
\quad
\mathrm{Storage} = \mathcal{O}((D + 1) N),
\f]
\f[
\mathrm{Query} = \mathcal{O}\left( N + I \right)
\quad
\mathrm{AverageQuery} = \mathcal{O}\left( \log N + I \right)
\f]





The figures below show the execution
times and storage requirements for the chair problem. The best
execution times are obtained for a leaf size of 16.  There is a high
memory overhead for small leaf sizes.  For the random points problem,
leaf sizes of 8 and 16 give the best performance.  The execution
time is moderately sensitive to the leaf size.  Compared with the
kd-tree, the octree's memory usage is higher and more sensitive to the
leaf size.


\image html orq/OctreeLeafSizeChairTime.jpg "The effect of leaf size on the execution time for the octree and the chair problem."
\image latex orq/OctreeLeafSizeChairTime.pdf "The effect of leaf size on the execution time for the octree and the chair problem." width=0.5\textwidth

\image html orq/OctreeLeafSizeChairMemory.jpg "The effect of leaf size on the memory usage for the octree and the chair problem."
\image latex orq/OctreeLeafSizeChairMemory.pdf "The effect of leaf size on the memory usage for the octree and the chair problem." width=0.5\textwidth

\image html orq/OctreeLeafSizeRandomTime.jpg "The effect of leaf size on the execution time for the octree and the random points problem."
\image latex orq/OctreeLeafSizeRandomTime.pdf "The effect of leaf size on the execution time for the octree and the random points problem." width=0.5\textwidth

\image html orq/OctreeLeafSizeRandomMemory.jpg "The effect of leaf size on the memory usage for the octree and the random points problem."
\image latex orq/OctreeLeafSizeRandomMemory.pdf "The effect of leaf size on the memory usage for the octree and the random points problem." width=0.5\textwidth





In the figure below we show the best
leaf size versus the average number of records in a query for the
random points problem.  We see that the best leaf size is correlated
to the number of records in a query.  The results are similar to those
for a kd-tree, but for octrees the best leaf size is a little larger.

\image html orq/OctreeLeafSizeRandomBestSize.jpg "The best leaf size as a function of records per query for the octree for the random points problem."
\image latex orq/OctreeLeafSizeRandomBestSize.pdf "The best leaf size as a function of records per query for the octree for the random points problem." width=0.5\textwidth

\image html orq/OctreeLeafSizeRandomBestRatio.jpg "The ratio of the number of records per query and the leaf size."
\image latex orq/OctreeLeafSizeRandomBestRatio.pdf "The ratio of the number of records per query and the leaf size." width=0.5\textwidth











<!---------------------------------------------------------------------------->
\section geom_orq_orq_cell Cells


The \ref geom_orq_rq_cell "cell method presented for range queries" is easily
generalized to higher dimensions.
(See \ref geom_orq_bentley_1979 "Data Structures for Range Searching.")
Consider an array of cells that spans the domain containing the
records.  Each cell spans a rectilinear domain of the same size and
contains a list or an array of pointers to the records in the cell.
The figure below shows a depiction of a 2-D
<em>cell array</em> (also called a <em>bucket array</em>).

We cell sort the records by converting their multikeys to cell
indices.  Let the cell array data structure have the attribute
\em min which returns the minimum multikey in the domain and the
attribute \em delta which returns the size of a cell.  Below are
the functions for this initialization of the cell data structure.

<pre>
multikey_to_cell_index(cells, multikey):
  <b>for</b> k \f$\in\f$ [0 .. K):
    index[k] = \f$\lfloor\f$(multikey[k] - cells.min[k]) / cells.delta[k]\f$\rfloor\f$
  <b>return</b> index
</pre>

<pre>
cell_sort(cells, file):
  <b>for</b> record <b>in</b> file:
    cells[multikey_to_cell_index(cells, record.multikey)] += record
</pre>


\image html orq/bucket.jpg "First we depict a 2-D cell array.  The 8 by 8 array of cells contains records depicted as points.  Next we show an orthogonal range query.  The query range is shown as a rectangle with thick lines.  There are eight boundary cells and one interior cell."
\image latex orq/bucket.pdf "First we depict a 2-D cell array.  The 8 by 8 array of cells contains records depicted as points.  Next we show an orthogonal range query.  The query range is shown as a rectangle with thick lines.  There are eight boundary cells and one interior cell." width=\textwidth


An orthogonal range query consists of accessing cells and testing
records for inclusion in the range.  The query range partially
overlaps <em>boundary cells</em> and completely overlaps
<em>interior cells</em>.  See the figure above.  For the
boundary cells we must test for inclusion in the range; for interior
cells we don't.  Below is the orthogonal range query algorithm.


<pre>
ORQ_cell(cells, range):
  included_records = \f$\emptyset\f$
  <b>for</b> each boundary_cell:
    <b>for</b> record <b>in</b> boundary_cell:
      <b>if</b> record \f$\in\f$ range:
        included_records += record
  <b>for</b> each interior_cell:
    <b>for</b> record <b>in</b> interior_cell:
      included_records += record
  <b>return</b> included_records
</pre>

The query performance depends on the size of the cells and the query
range.  If the cells are too large, the boundary cells will likely
contain many records which are not in the query range.  We will waste
time doing inclusion tests on records that are not close to the range.
If the cells are too small, we will spend a lot of time accessing
cells.  The cell method is particularly suited to problems in which
the query ranges are approximately the same size.  Then the cell size
can be chosen to give good performance.  Let \em M be the total number
of cells.  Let \em J be the number of cells which overlap the query
range and \f$\tilde{I}\f$ be the number of records in the overlapping
cells.  Suppose that the cells are no larger than the query range and
that both are roughly cubical.  Let \em R be the ratio of the length of
a query range to the length of a cell in a given coordinate.  In this
case we expect \f$J \approx (R + 1)^K\f$.  The number of records in these
cells will be about \f$\tilde{I} \approx (1 + 1/R)^K I\f$, where \em I is
the number of records in the query range.  Let AverageQuery
be the expected computational complexity for this case.

\f[
\mathrm{Preprocess} = \mathcal{O}(M + N),
\quad
\mathrm{Reprocess} = \mathcal{O}(M + N),
\quad
\mathrm{Storage} = \mathcal{O}(M + N),
\f]
\f[
\mathrm{Query} = \mathcal{O}(J + \tilde{I}),
\quad
\mathrm{AverageQuery} = \mathcal{O}\left( (R + 1)^K + (1 + 1/R)^K I \right)
\f]

The figure below shows the execution
times and storage requirements for the chair problem.
The best execution times are obtained when the ratio
of cell length to query range length, \em R, is between 1/4 and 1/2.
There is a large storage overhead for \f$R \gtrsim 3/8\f$.
For the random points problem,
the best execution times are obtained when \em R
is between 1/5 and 1/2.  There is a large storage overhead for 
\f$R \gtrsim 1/5\f$.

\image html orq/CellArrayCellSizeChairTime.jpg "The effect of leaf size on the execution time for the cell array and the chair problem."
\image latex orq/CellArrayCellSizeChairTime.pdf "The effect of leaf size on the execution time for the cell array and the chair problem." width=0.5\textwidth

\image html orq/CellArrayCellSizeChairMemory.jpg "The effect of leaf size on the memory usage for the cell array and the chair problem."
\image latex orq/CellArrayCellSizeChairMemory.pdf "The effect of leaf size on the memory usage for the cell array and the chair problem." width=0.5\textwidth

\image html orq/CellArrayCellSizeRandomTime.jpg "The effect of leaf size on the execution time for the cell array and the random points problem."
\image latex orq/CellArrayCellSizeRandomTime.pdf "The effect of leaf size on the execution time for the cell array and the random points problem." width=0.5\textwidth

\image html orq/CellArrayCellSizeRandomMemory.jpg "The effect of leaf size on the memory usage for the cell array and the random points problem."
\image latex orq/CellArrayCellSizeRandomMemory.pdf "The effect of leaf size on the memory usage for the cell array and the random points problem." width=0.5\textwidth

<!-- CONTINUE HERE with the merging.-->


In the figure below we show the best cell
size versus the query range size for the random points problem.  
We see that the best cell size is correlated to the 
query size.  For small query sizes which return only a few records, the 
best cell size is a little larger than the query size.  The ratio of 
best cell size to query size decreases with increasing query size.

\image html orq/CellArrayCellSizeRandomBestSize.jpg "The best cell size versus the query range size for the cell array on the random points problem."
\image latex orq/CellArrayCellSizeRandomBestSize.pdf "The best cell size versus the query range size for the cell array on the random points problem." width=0.5\textwidth

\image html orq/CellArrayCellSizeRandomBestSizeRatio.jpg "The ratio of the cell size to the query range size."
\image latex orq/CellArrayCellSizeRandomBestSizeRatio.pdf "The ratio of the cell size to the query range size." width=0.5\textwidth

\image html orq/CellArrayCellSizeRandomBestRecordRatio.jpg "The average number of records in a cell versus the average number of records returned by an orthogonal range query as a ratio."
\image latex orq/CellArrayCellSizeRandomBestRecordRatio.pdf "The average number of records in a cell versus the average number of records returned by an orthogonal range query as a ratio." width=0.5\textwidth








<!---------------------------------------------------------------------------->
\section geom_orq_orq_sparse Sparse Cells


Consider the cell method presented \ref geom_orq_orq_cell "above".
If the cell size and distribution of records is such that many of the
cells are empty then the storage requirement for the cells may exceed
that of the records.  Also, the computational cost of accessing cells
may dominate the orthogonal range query. In such cases it may be
advantageous to use a sparse cell data structure in which only
non-empty cells are stored and use hashing to access cells.

As an example, one could 
use a sparse array data structure.  The figure below
depicts a cell array that is sparse in the \em x coordinate.  For
cell arrays that are sparse in one coordinate, we can access a cell by 
indexing an array and then performing a binary search in the sparse 
direction.  The orthogonal range query algorithm is essentially the same 
as that for the dense cell array.

\image html orq/sparsebucket.jpg "A sparse cell array in 2-D.  The array is sparse in the \em x coordinate.  Only the non-empty cells are stored."
\image latex orq/sparsebucket.pdf "A sparse cell array in 2-D.  The array is sparse in the \em x coordinate.  Only the non-empty cells are stored." width=0.5\textwidth





As with dense cell arrays, the query performance depends on the size
of the cells and the query range.  The same results carry
over. However, accessing a cell is more expensive because of the
binary search in the sparse direction.  One would choose a sparse cell
method when the memory overhead of the dense cell method is
prohibitive.  Let \em M be the number of cells used with the dense cell
method.  The sparse cell method has an array of sparse cell data
structures.  The array spans \em K - 1 dimensions and thus has size
\f$\mathcal{O}(M^{1 - 1/K})\f$.  The binary searches are performed on the
sparse cell data structures.  The total number of non-empty cells is
bounded by \em N.  Thus the storage requirement is \f$\mathcal{O}(M^{1 -
1/K} + N)\f$.  The data structure is built by cell sorting the records.
Thus the preprocessing and reprocessing times are \f$\mathcal{O}(M^{1
- 1/K} + N)\f$.


Let \em J be the number of non-empty cells which overlap the query range
and \f$\tilde{I}\f$ be the number of records in the overlapping cells.
There will be at most \em J binary searches to access the cells.
There are \f$\mathcal{O}(M^{1/K})\f$ cells in the sparse
direction.  Thus the worst-case computational cost of an orthogonal
range query is \f$\mathcal{O}(J \log(M^{1/K}) + \tilde{I})\f$.  Next we
determine the expected cost of a query.  Suppose that the cells are no
larger than the query range and that both are roughly cubical.  Let
\em R be the ratio of the length of a query range to the length of a
cell in a given coordinate.  In this case \f$J \lesssim (R + 1)^K\f$.  We will
have to perform about \f$(R + 1)^{K - 1}\f$ binary searches to access these
cells.  Each binary search will be performed on no more than
\f$\mathcal{O}(M^{1/K})\f$ cells.  Thus the cost of the binary searches is
\f$\mathcal{O}((R + 1)^{K - 1} \log (M^{1/K}))\f$.  Excluding the
binary searches, the cost of accessing cells is \f$\mathcal{O}((R +
1)^K)\f$.  The number of records in the overlapping cells will be about
\f$\tilde{I} \lesssim (1 + 1/R)^K I\f$, where \em I is the number of records in the
query range.  For the interior cells, records are simply returned.
For the boundary cells, which partially overlap the query range, the
records must be tested for inclusion.  These operations add a cost of
\f$\mathcal{O}((1 + 1/R)^K I)\f$:

\f[
\mathrm{Preprocess} = \mathcal{O}(M^{1 - 1/K} + N),
\quad
\mathrm{Reprocess} = \mathcal{O}(M^{1 - 1/K} + N),
\f]
\f[
\mathrm{Storage} = \mathcal{O}(M^{1 - 1/K} + N),
\quad
\mathrm{Query} = \mathcal{O}(J \log(M) / K + \tilde{I}),
\f]
\f[
\mathrm{AverageQuery} 
  = \mathcal{O}\left( (R + 1)^{K - 1} \log(M) / K 
  + (R + 1)^K + (1 + 1/R)^K I \right)
\f]



The figure below shows the
execution times and storage requirements for the chair
problem. Again the best execution times are obtained when \em R is
between 1/4 and 1/2.  The performance of the sparse cell arrays is
very close to that of dense cell arrays.  The execution times are a
little higher than those for dense cell arrays for medium to large
cell sizes.  This reflects the overhead of the binary search to access
cells.  The execution times are lower than those for dense cell arrays
for small cell sizes.  This is due to removing the overhead of
accessing empty cells.

\image html orq/SparseCellArrayCellSizeChairTime.jpg "The effect of leaf size on the performance of the sparse cell array for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the dense cell array is shown for comparison."
\image latex orq/SparseCellArrayCellSizeChairTime.pdf "The effect of leaf size on the performance of the sparse cell array for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the dense cell array is shown for comparison." width=0.5\textwidth

\image html orq/SparseCellArrayCellSizeChairMemory.jpg "The effect of leaf size on the performance of the sparse cell array for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the dense cell array is shown for comparison." 
\image latex orq/SparseCellArrayCellSizeChairMemory.pdf "The effect of leaf size on the performance of the sparse cell array for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the dense cell array is shown for comparison." width=0.5\textwidth



The figure below shows the execution
times and storage requirements for the random points problem.
The execution times are very close to those for the dense cell array.

\image html orq/SparseCellArrayCellSizeRandomTime.jpg "The effect of leaf size on the performance of the sparse cell array for the random points problem.  The plot shows the execution time in seconds versus \em R. The performance of the dense cell array is shown for comparison."
\image latex orq/SparseCellArrayCellSizeRandomTime.pdf "The effect of leaf size on the performance of the sparse cell array for the random points problem.  The plot shows the execution time in seconds versus \em R. The performance of the dense cell array is shown for comparison." width=0.5\textwidth

\image html orq/SparseCellArrayCellSizeRandomMemory.jpg "The effect of leaf size on the performance of the sparse cell array for the random points problem. The plot shows the memory usage in megabytes versus \em R.  The performance of the dense cell array is shown for comparison."
\image latex orq/SparseCellArrayCellSizeRandomMemory.pdf "The effect of leaf size on the performance of the sparse cell array for the random points problem. The plot shows the memory usage in megabytes versus \em R.  The performance of the dense cell array is shown for comparison." width=0.5\textwidth




In the figure below we show
the best cell size versus the query range size for the random points
problem.  We see that the best cell size is correlated to the query
size.  The results are very similar to those for dense cell arrays.
For sparse cell arrays, the best cell size is slightly larger due to
the higher overhead of accessing a cell.


\image html orq/SparseCellArrayCellSizeRandomBestSize.jpg "The best cell size versus the query range size for the sparse cell array on the random points problem."
\image latex orq/SparseCellArrayCellSizeRandomBestSize.pdf "The best cell size versus the query range size for the sparse cell array on the random points problem." width=0.5\textwidth

\image html orq/SparseCellArrayCellSizeRandomBestSizeRatio.jpg "The ratio of the cell size to the query range size."
\image latex orq/SparseCellArrayCellSizeRandomBestSizeRatio.pdf "The ratio of the cell size to the query range size." width=0.5\textwidth

\image html orq/SparseCellArrayCellSizeRandomBestRecordRatio.jpg "The average number of records in a cell versus the average number of records returned by an orthogonal range query as a ratio."
\image latex orq/SparseCellArrayCellSizeRandomBestRecordRatio.pdf "The average number of records in a cell versus the average number of records returned by an orthogonal range query as a ratio." width=0.5\textwidth










<!---------------------------------------------------------------------------->
\section geom_orq_orq_binary Cells Coupled with a Binary Search


One can couple the cell method with other search data structures.  For
instance, one could sort the records in each of the cells or store
those records in a search tree. Most such combinations of data
structures do not offer any advantages.  However, there are some that
do.  Based on the success of the sparse cell method, we couple a
binary search to a cell array.  For the sparse cell method previously
presented, there is a dense cell array which spans \em K - 1 dimensions
and a sparse cell structure in the final dimension.  Instead of
storing sparse cells in the final dimension, we store records sorted
in that direction.  We can access a record with array indexing in the
first \em K - 1 dimensions and a binary search in the final dimension.
See the figure below for a depiction of a 
cell array with binary searching.

\image html orq/cell_binary_search.jpg "First we depict a cell array with binary search in 2-D.  There are 8 cells which contain records sorted in the \em x direction.  Next we show an orthogonal range query.  The query range is shown as a rectangle with thick lines.  There are three overlapping cells."
\image latex orq/cell_binary_search.pdf "First we depict a cell array with binary search in 2-D.  There are 8 cells which contain records sorted in the \em x direction.  Next we show an orthogonal range query.  The query range is shown as a rectangle with thick lines.  There are three overlapping cells." width=\textwidth


We construct the data structure by cell sorting the records and then
sorting the records within each cell.  Let the data structure have the
attribute \em min which returns the minimum multikey in the
domain and the attribute \em delta which returns the size of a
cell.  Let \em cells be the cell array.  Below is the method
for constructing the data structure.

<pre>
multikey_to_cell_index(multikey):
  <b>for</b> k \f$\in\f$ [0..K-1):
    index[k] \f$= \lfloor\f$(multikey[k] - min[k]) / delta[k]\f$\rfloor\f$
  <b>return</b> index
</pre>

<pre>
construct(file):
  <b>for</b> record <b>in</b> file:
    cells[multikey_to_cell_index(record.multikey)] += record
  <b>for</b> cell <b>in</b> cells:
    sort_by_last_key(cell)
  <b>return</b>
</pre>


The orthogonal range query consists of accessing cells that overlap
the domain and then doing a binary search followed by a sequential
scan on the sorted records in each of these cells.  Below is the
orthogonal range query method.


<pre>
ORQ_cell_binary_search(range):
  included_records = \f$\emptyset\f$
  min_index = multikey_to_index(range.min)
  max_index = multikey_to_index(range.max)
  <b>for</b> index <b>in</b> [min_index..max_index]:
    iter = binary_search_lower_bound(cells[index].begin, cells[index].end, range.min[K-1])
    <b>while</b> (*iter).multikey[K-1] \f$\leq\f$ range.max[K-1]:
      <b>if</b> *iter \f$\in\f$ range:
        included_records += iter
  <b>return</b> included_records
</pre>



As with sparse cell arrays, the query performance depends on the size
of the cells and the query range.  The same results carry over. The
only differences are that the binary search on records is more costly
than the search on cells, but we do not have the computational cost of
accessing cells in the sparse data structure or the storage overhead
of those cells.  Let \em M be the number of cells used with the dense
cell method.  The cell with binary search method has an array of cells
which contain sorted records.  This array spans \em K - 1 dimensions and
thus has size \f$\mathcal{O}(M^{1 - 1/K})\f$.  Thus the storage
requirement is \f$\mathcal{O}(M^{1 - 1/K} + N)\f$.  The data structure is
built by cell sorting the records and then sorting the records within
each cell.  We will assume that the records are approximately
uniformly distributed.  Thus each cell contains \f$\mathcal{O}(N / M^{1
- 1/K})\f$ records.  Thus each sort of the records in a cell costs
\f$\mathcal{O}((N / M^{1 - 1/K}) \log (N / M^{1 - 1/K}) )\f$.  The
preprocessing time is \f$\mathcal{O}( M^{1 - 1/K} + N + N \log(N / M^{1
- 1/K}) )\f$.  We can use insertion sort for reprocessing, so its cost
is \f$\mathcal{O}(M^{1 - 1/K} + N)\f$.


Let \em J be the number of cells which overlap the query range and
\f$\tilde{I}\f$ be the number of records which are reported or checked for
inclusion in the overlapping cells.  There will be \em J binary searches
to find the starting record in each cell.  Thus the worst-case
computational complexity of an orthogonal range query is
\f$\mathcal{O}(J \log(N / M^{1 - 1/K}) + \tilde{I})\f$.  Next we determine
the expected cost of a query.  Suppose that the cells are no larger
than the query range and that both are roughly cubical (except in the
binary search direction).  Let \em R be the ratio of the length of a
query range to the length of a cell in a given coordinate.  In this
case \f$J \lesssim (R + 1)^{K-1}\f$.  Thus the cost of the binary searches
is \f$\mathcal{O}( (R + 1)^{K - 1} \log(N / M^{1 - 1/K}) )\f$.  The number
of records in the overlapping cells that are checked for inclusion is
about \f$\tilde{I} \lesssim (1 + 1/R)^{K-1} I\f$, where \em I is the number
of records in the query range.  These operations add a cost of
\f$\mathcal{O}((1 + 1/R)^{K-1} I)\f$:


\f[
\mathrm{Preprocess} = \mathcal{O}( M^{1 - 1/K} + N + N \log(N / M^{1 - 1/K}) ),
\quad
\mathrm{Reprocess} = \mathcal{O}(M^{1 - 1/K} + N),
\f]
\f[
\mathrm{Storage} = \mathcal{O}(M^{1 - 1/K} + N),
\quad
\mathrm{Query} = \mathcal{O}(J \log(N / M^{1 - 1/K})  + \tilde{I}),
\f]
\f[
\mathrm{AverageQuery} 
= \mathcal{O}\left( (R + 1)^{K - 1} \log \left( N / M^{1 - 1/K} \right) 
  + (1 + 1/R)^{K-1} I \right)
\f]



The figures below show the
execution times and storage requirements for the chair problem. Like
the sparse cell array, the best execution times are obtained when \em R
is between 1/4 and 1/2.  The performance of the cell array coupled
with a binary search is comparable to that of the sparse cell array.
However, it is less sensitive to the size of the cells.  Also,
compared to sparse cell arrays, there is less memory overhead for
small cells.

\image html orq/CellXYBinarySearchZCellSizeChairTime.jpg "The effect of leaf size on the performance of the cell array coupled with binary searches for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the sparse cell array is shown for comparison."
\image latex orq/CellXYBinarySearchZCellSizeChairTime.pdf "The effect of leaf size on the performance of the cell array coupled with binary searches for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the sparse cell array is shown for comparison." width=0.5\textwidth

\image html orq/CellXYBinarySearchZCellSizeChairMemory.jpg "The effect of leaf size on the performance of the cell array coupled with binary searches for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the sparse cell array is shown for comparison."
\image latex orq/CellXYBinarySearchZCellSizeChairMemory.pdf "The effect of leaf size on the performance of the cell array coupled with binary searches for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the sparse cell array is shown for comparison." width=0.5\textwidth


The figures below show the
execution times and storage requirements for the random points
problem.  The execution times are better than those for the sparse
cell array and are less dependent on the cell size.  Again, there is
less memory overhead for small cells.

\image html orq/CellXYBinarySearchZCellSizeRandomTime.jpg "The effect of leaf size on the performance of the cell array coupled with binary searches for the random points problem.  The plot shows the execution time in seconds versus R.  The performance of the sparse cell array is shown for comparison."
\image latex orq/CellXYBinarySearchZCellSizeRandomTime.pdf "The effect of leaf size on the performance of the cell array coupled with binary searches for the random points problem.  The plot shows the execution time in seconds versus R.  The performance of the sparse cell array is shown for comparison." width=0.5\textwidth

\image html orq/CellXYBinarySearchZCellSizeRandomMemory.jpg "The effect of leaf size on the performance of the cell array coupled with binary searches for the random points problem.  The plot shows the memory usage in megabytes versus R.  The performance of the sparse cell array is shown for comparison."
\image latex orq/CellXYBinarySearchZCellSizeRandomMemory.pdf "The effect of leaf size on the performance of the cell array coupled with binary searches for the random points problem.  The plot shows the memory usage in megabytes versus R.  The performance of the sparse cell array is shown for comparison." width=0.5\textwidth




In the figures below 
we show the best cell size versus the query range size for the random
points problem.  Surprisingly, we see that the best cell size for this
problem is not correlated to the query size.


\image html orq/CellXYBinarySearchZCellSizeRandomBestSize.jpg "The best cell size versus the query range size for the cell array coupled with binary searches on the random points problem."
\image latex orq/CellXYBinarySearchZCellSizeRandomBestSize.pdf "The best cell size versus the query range size for the cell array coupled with binary searches on the random points problem." width=0.5\textwidth

\image html orq/CellXYBinarySearchZCellSizeRandomBestSizeRatio.jpg "The ratio of the cell size to the query range size."
\image latex orq/CellXYBinarySearchZCellSizeRandomBestSizeRatio.pdf "The ratio of the cell size to the query range size." width=0.5\textwidth
*/


















//=============================================================================
//=============================================================================
/*!
\page geom_orq_querySizes Performance Tests over a Range of Query Sizes


By choosing the leaf size or cell size, tree methods and cell methods 
can be tuned to perform well for a given fixed query size.  In this section
we well consider how the various methods for doing single orthogonal range 
queries perform over a range of query sizes.  For each test there are 
one million records.  The records are points in 3-D space.  The query ranges
are cubes.



<!--------------------------------------------------------------------------->
\section geom_orq_querySizes_randomly Randomly Distributed Points in a Cube


The records for this test are uniform randomly distributed points in
the unit cube, \f$[0..1]^3\f$.  The query sizes are \f$\{ \sqrt[3]{1/2^n} : n
\in [1 .. 20] \}\f$ and range in size from about 0.01 to about 0.8.
The smallest query range contains about a single record on average.
The largest query range contains about half the records.  See
the figure below.

\image html orq/RandomCubeQuerySize.jpg "Log-log plot of the average number of records in the query versus the query size for the randomly distributed points in a cube problem."
\image latex orq/RandomCubeQuerySize.pdf "Log-log plot of the average number of records in the query versus the query size for the randomly distributed points in a cube problem." width=0.5\textwidth





<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_randomly_sequential Sequential Scan


The figure below shows the
performance of the sequential scan method.  The performance is roughly
constant for small and medium sized queries.  The execution time is
higher for large query sizes for two reasons.  Firstly, more records
are returned.  Secondly, for large query sizes the inclusion test is
unpredictable.  (If a branching statement is predictable, modern
CPU's will predict the answer to save time.  If they guess
incorrectly, there is a roll-back penalty.)  Still, the performance is
only weakly dependent on the query size.  There is only a factor of 2
difference between the smallest and largest query.  We will see that
the sequential scan algorithm performs poorly except for the largest
query sizes.


\image html orq/SequentialScanQuerySizeRandomCube.jpg "Log-log plot of execution time versus query size for the sequential scan method with the randomly distributed points in a cube problem."
\image latex orq/SequentialScanQuerySizeRandomCube.pdf "Log-log plot of execution time versus query size for the sequential scan method with the randomly distributed points in a cube problem." width=0.5\textwidth





<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_randomly_projection Projection Methods


The performance of the projection method and the related point-in-box 
method are shown in the figure below.  They 
perform much better than the sequential scan method, but we will see that 
they are not competitive with tree or cell methods.  The projection method
has slightly lower execution times than the point-in-box method.  The benefit
of doing integer comparisons is outweighed by the additional storage and
complexity of the point-in-box method.

\image html orq/ProjectQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the projection method and the point-in-box method with the randomly distributed points in a cube problem.  The performance of the sequential scan method is shown for comparison."
\image latex orq/ProjectQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the projection method and the point-in-box method with the randomly distributed points in a cube problem.  The performance of the sequential scan method is shown for comparison." width=0.6\textwidth











<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_randomly_tree Tree Methods



The figures below show the performance of the 
kd-tree data structure.  The execution time is moderately sensitive to the
leaf size for small queries and mildly sensitive for large queries.  As 
expected, small leaf sizes give good performance for small queries and
large leaf sizes give good performance for large queries.  The test with
a leaf size of 8 has the best overall performance.


\image html orq/KDTreeQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the kd-tree without domain checking on the randomly distributed points in a cube problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/KDTreeQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the kd-tree without domain checking on the randomly distributed points in a cube problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/KDTreeQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/KDTreeQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth







The figures below show the
performance of the kd-tree data structure with domain checking.  The
performance is similar to the kd-tree without domain checking, but is
less sensitive to leaf size for small queries.  Again, the test with a
leaf size of 8 has the best overall performance.



\image html orq/KDTreeDomainQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the kd-tree with domain checking on the randomly distributed points in a cube problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/KDTreeDomainQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the kd-tree with domain checking on the randomly distributed points in a cube problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/KDTreeDomainQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/KDTreeDomainQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth











The figures below show the performance of
the octree data structure.  In terms of leaf size dependence, the
performance is similar to the kd-tree with domain checking, however
the execution times are higher.  The test with a leaf size of 16 has
the best overall performance.


\image html orq/OctreeQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the octree on the randomly distributed points in a cube problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/OctreeQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the octree on the randomly distributed points in a cube problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/OctreeQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/OctreeQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth






We compare the performance of the tree methods in
the figures below.   For small queries, the 
kd-tree method without domain checking gives the best performance.  For
large queries, domain checking becomes profitable.  The kd-tree data structure
with domain checking during the query appears to give the best overall
performance.
 

\image html orq/TreeQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the tree methods on the randomly distributed points in a cube problem.  The key indicates the data structure.  We show the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 8 and the octree with a leaf size of 16.  The performance of the sequential scan method is shown for comparison."
\image latex orq/TreeQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the tree methods on the randomly distributed points in a cube problem.  The key indicates the data structure.  We show the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 8 and the octree with a leaf size of 16.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/TreeQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/TreeQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth












<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_randomly_cell Cell Methods





The figures below show the
performance of the cell array data structure.  The execution time is
highly sensitive to the cell size for small queries and moderately
sensitive for large queries.  Small cell sizes give good performance
for small queries.  For large queries, the best cell size is still
quite small.  The test with a cell size of 0.02 has the best overall
performance.


\image html orq/CellArrayQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the cell array on the randomly distributed points in a cube problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/CellArrayQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the cell array on the randomly distributed points in a cube problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/CellArrayQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/CellArrayQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth







The figures below show the
performance of the sparse cell array data structure.  The performance
characteristics are similar to those of the dense cell array, however
the execution time is less sensitive to cell size for large queries.
The test with a cell size of 0.02 has the best overall performance.

\image html orq/SparseCellArrayQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the sparse cell array on the randomly distributed points in a cube problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/SparseCellArrayQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the sparse cell array on the randomly distributed points in a cube problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/SparseCellArrayQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/SparseCellArrayQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth







The figures below show
the performance of using a cell array with binary searching.  The
performance characteristics are similar to those of the dense and
sparse cell arrays, however the execution time is less sensitive to
cell size.  The test with a cell size of 0.01414 has the best
overall performance.


\image html orq/CellXYBinarySearchZQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the cell array with binary searching on the randomly distributed points in a cube problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/CellXYBinarySearchZQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the cell array with binary searching on the randomly distributed points in a cube problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/CellXYBinarySearchZQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/CellXYBinarySearchZQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth







We compare the performance of the cell methods in
the figures below.   For small queries, the 
execution times of the three methods are very close.  For large queries,
the dense cell array has a little better performance.  Thus dense cell 
arrays give the best overall performance for this test.


\image html orq/CellQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the cell methods on the randomly distributed points in a cube problem.  The key indicates the data structure.  We show the dense cell array with a cell size of 0.02, the sparse cell array with a cell size of 0.02 and the cell array with binary searching with a cell size of 0.01414.  The performance of the sequential scan method is shown for comparison."
\image latex orq/CellQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the cell methods on the randomly distributed points in a cube problem.  The key indicates the data structure.  We show the dense cell array with a cell size of 0.02, the sparse cell array with a cell size of 0.02 and the cell array with binary searching with a cell size of 0.01414.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/CellQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/CellQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth













<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_randomly_comparison Comparison





We compare the performance of the orthogonal range query methods in
the figures below.  We plot the execution
times of the best performers from each family of methods.  The
projection method has relatively high execution times, especially for
small query sizes.  The kd-tree without domain checking has good
performance for small queries.  The kd-tree with domain checking
performs well for large queries and has pretty good execution times
for small queries as well.  The dense cell array method has lower
execution times than each of the other methods for all query sizes.
It gives the best overall performance for this test.


\image html orq/CompareQuerySizeRandomCubeTotal.jpg "Log-log plot of execution time versus query size for the orthogonal range query methods on the randomly distributed points in a cube problem.  The key indicates the data structure.  We show the sequential scan method, the projection method, the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 8 and the cell array with a cell size of 0.02."
\image latex orq/CompareQuerySizeRandomCubeTotal.pdf "Log-log plot of execution time versus query size for the orthogonal range query methods on the randomly distributed points in a cube problem.  The key indicates the data structure.  We show the sequential scan method, the projection method, the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 8 and the cell array with a cell size of 0.02." width=0.9\textwidth

\image html orq/CompareQuerySizeRandomCubeScaled.jpg "The execution time per reported record."
\image latex orq/CompareQuerySizeRandomCubeScaled.pdf "The execution time per reported record." width=0.9\textwidth














<!--------------------------------------------------------------------------->
\section geom_orq_querySizes_sphere Randomly Distributed Points on a Sphere



In the test of the previous section the records were distributed throughout
the 3-D domain.  Now we do a test in which the records lie on a 2-D surface.
The records for this test are uniform randomly distributed points on
the surface of a sphere with unit radius.  The query sizes are \f$\{
\sqrt{1/2^n} : n \in [-2 .. 19] \}\f$ and range in size from about 0.001
to 2.  The query ranges are centered about points on the
surface of the sphere.  The smallest query range contains about a
single record on average.  The largest query range contains about
40% of the records.  See the figure below.


\image html orq/RandomSphereQuerySize.jpg "Log-log plot of the average number of records in the query versus the query size for the randomly distributed points on a sphere problem."
\image latex orq/RandomSphereQuerySize.pdf "Log-log plot of the average number of records in the query versus the query size for the randomly distributed points on a sphere problem." width=0.5\textwidth




<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_sphere_sequential Sequential Scan



The figure below shows the
performance of the sequential scan method.  As before, the performance
is roughly constant for small and medium sized queries but is higher
for large query sizes.

\image html orq/SequentialScanQuerySizeRandomSphere.jpg "Log-log plot of execution time versus query size for the sequential scan method on the randomly distributed points on a sphere problem."
\image latex orq/SequentialScanQuerySizeRandomSphere.pdf "Log-log plot of execution time versus query size for the sequential scan method on the randomly distributed points on a sphere problem." width=0.5\textwidth







<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_sphere_projection Projection Methods

The performance of the projection method and the point-in-box method
are shown in the figure below.  Again the
projection method has slightly lower execution times than the
point-in-box method.


\image html orq/ProjectQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the projection method and the point-in-box method on the randomly distributed points on a sphere problem.  The performance of the sequential scan method is shown for comparison."
\image latex orq/ProjectQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the projection method and the point-in-box method on the randomly distributed points on a sphere problem.  The performance of the sequential scan method is shown for comparison." width=0.6\textwidth





<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_sphere_tree Tree Methods


The figures below show the
performance of the kd-tree data structure.  The execution time is
moderately sensitive to the leaf size for small queries and mildly
sensitive for large queries.  As before, small leaf sizes give better
performance for small queries and large leaf sizes give better
performance for large queries.  The test with a leaf size of 8 has the
best overall performance.


\image html orq/KDTreeQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the kd-tree without domain checking data structure on the randomly distributed points on a sphere problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/KDTreeQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the kd-tree without domain checking data structure on the randomly distributed points on a sphere problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/KDTreeQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/KDTreeQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth





The figures below show the
performance of the kd-tree data structure with domain checking.  The
performance characteristics are similar to the kd-tree without domain
checking.  The test with a leaf size of 16 has the best overall
performance.


\image html orq/KDTreeDomainQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the kd-tree with domain checking data structure on the randomly distributed points on a sphere problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/KDTreeDomainQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the kd-tree with domain checking data structure on the randomly distributed points on a sphere problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/KDTreeDomainQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/KDTreeDomainQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth







The figures below show the performance of
the octree data structure.  The test with a leaf size of 16 has
the best overall performance.


\image html orq/OctreeQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the octree data structure on the randomly distributed points on a sphere problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/OctreeQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the octree data structure on the randomly distributed points on a sphere problem.  The key shows the leaf size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/OctreeQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/OctreeQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth




We compare the performance of the tree methods in the
figures below.  For small queries, the
kd-tree method without domain checking gives the best performance.
For large queries, domain checking becomes profitable.  For medium
sized queries, the different methods perform similarly.  The kd-tree
data structure without domain checking during the query appears to
give the best overall performance.


\image html orq/TreeQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the tree methods on the randomly distributed points on a sphere problem.  The key indicates the data structure.  We show the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 16 and the octree with a leaf size of 16.  The performance of the sequential scan method is shown for comparison."
\image latex orq/TreeQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the tree methods on the randomly distributed points on a sphere problem.  The key indicates the data structure.  We show the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 16 and the octree with a leaf size of 16.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/TreeQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/TreeQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth






<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_sphere_cell Cell Methods



The figures below show the
performance of the cell array data structure.  The execution time is
highly sensitive to the cell size.  Small cell sizes give good
performance for small queries.  Medium to large cell sizes give good
performance for large queries.  The test with a cell size of 0.02
has the best overall performance.


\image html orq/CellArrayQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the cell array data structure on the randomly distributed points on a sphere problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/CellArrayQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the cell array data structure on the randomly distributed points on a sphere problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/CellArrayQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/CellArrayQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth








The figures below show the
performance of the sparse cell array data structure.  The performance
is similar to that of the dense cell array for small queries. 
The execution time is less sensitive to cell size for large queries.
In fact, the performance hardly varies with cell size.
The test with a cell size of 0.02 has the best overall performance.


\image html orq/SparseCellArrayQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the sparse cell array data structure on the randomly distributed points on a sphere problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/SparseCellArrayQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the sparse cell array data structure on the randomly distributed points on a sphere problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/SparseCellArrayQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/SparseCellArrayQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth








The figures below show
the performance of using a cell array with binary searching.  The
execution time is moderately sensitive to cell size for small query
sizes and mildly sensitive for large queries.  The test with a cell
size of 0.02 has the best overall performance.


\image html orq/CellXYBinarySearchZQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the cell array with binary searching data structure on the randomly distributed points on a sphere problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison."
\image latex orq/CellXYBinarySearchZQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the cell array with binary searching data structure on the randomly distributed points on a sphere problem.  The key shows the cell size.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/CellXYBinarySearchZQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/CellXYBinarySearchZQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth







We compare the performance of the cell methods in
the figures below.  
Each method has a cell size of 0.02.  For small queries, the cell
array with binary searching has the best performance.  For large queries,
the dense cell array method is a little faster.


\image html orq/CellQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the cell methods on the randomly distributed points on a sphere problem.  The key indicates the data structure.  We show the dense cell array, the sparse cell array and the cell array with binary searching, each with a cell size of 0.02.  The performance of the sequential scan method is shown for comparison."
\image latex orq/CellQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the cell methods on the randomly distributed points on a sphere problem.  The key indicates the data structure.  We show the dense cell array, the sparse cell array and the cell array with binary searching, each with a cell size of 0.02.  The performance of the sequential scan method is shown for comparison." width=0.9\textwidth

\image html orq/CellQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/CellQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth









<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
\subsection geom_orq_querySizes_sphere_comparison Comparison



We compare the performance of the orthogonal range query methods in
the figures below.  We plot the execution
times of the best overall performers from each family of methods.  The
projection method has relatively high execution times, especially for
small query sizes.  The kd-tree without domain checking has good
performance for small queries.  There is a performance penalty
for domain checking for small queries and a performance boost
for large queries.  The cell array with binary searching performs
better than the dense cell array for small queries.  This is because the 
size of a cell in the dense cell array is much larger than the query size
so time is wasted with inclusion tests.  The dense cell
array has the edge for large queries because the query range spans many 
cells.

For small query sizes, the cell array with binary searching and the kd-tree
without domain checking both perform well.  For large query sizes the 
dense cell array performs best, followed by the cell array with binary 
searching.  The cell array with binary searching has the best overall 
performance for this test.


\image html orq/CompareQuerySizeRandomSphereTotal.jpg "Log-log plot of execution time versus query size for the orthogonal range query methods on the randomly distributed points on a sphere problem.  The key indicates the data structure.  We show the sequential scan method, the projection method, the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 16, the cell array with a cell size of 0.02 and the cell array with binary searching with a cell size of 0.02."
\image latex orq/CompareQuerySizeRandomSphereTotal.pdf "Log-log plot of execution time versus query size for the orthogonal range query methods on the randomly distributed points on a sphere problem.  The key indicates the data structure.  We show the sequential scan method, the projection method, the kd-tree with a leaf size of 8, the kd-tree with domain checking with a leaf size of 16, the cell array with a cell size of 0.02 and the cell array with binary searching with a cell size of 0.02." width=0.9\textwidth

\image html orq/CompareQuerySizeRandomSphereScaled.jpg "The execution time per reported record."
\image latex orq/CompareQuerySizeRandomSphereScaled.pdf "The execution time per reported record." width=0.9\textwidth
*/



















//=============================================================================
//=============================================================================
/*!
\page geom_orq_mrq Multiple Range Queries





<!--------------------------------------------------------------------------->
\section geom_orq_mrq_versus Single versus Multiple Queries


To the best of my knowledge, there has not been any previously
published work addressing the issue of doing a set of orthogonal range
queries.  There has been a great deal of work on doing single queries.
However, there are no previously introduced algorithms that can
perform a set of \em Q queries in less time than the product of \em Q and
the time of a single query.  In this section we will introduce an
algorithm for doing multiple 1-D range queries.  In the 
following section we will extend the algorithm to higher dimensions.








<!---------------------------------------------------------------------------->
\section geom_orq_mrq_sorted Sorted Key and Sorted Ranges with Forward Searching


\ref geom_orq_rq_bsosd "Previously" we sorted the file by its keys.  The
purpose of this was to enable us to use a binary search to find a
record.  The binary search requires a random access iterator to the
records.  That is, it can access any record of the file in constant
time.  Typically this means that the records are stored in an array.
Now we introduce a data structure that stores its data in sorted
order, but need only provide a forward iterator.  A container of this
type must provide the attribute \em begin, which points to the first
element and the attribute \em end, which points to one past the last
element.  A forward iterator must support the following operations.
<DL>
<DT>dereference</DT>
<DD>*iter returns the element pointed to by iter.</DD>
<DT>increment</DT>
<DD>++iter moves iter to the next element.</DD>
</DL>
In addition, the forward iterator supports assignment and equality tests.
All of the containers in the C++ STL library satisfy these criteria.
(See \ref geom_orq_austern_1999 "Generic programming and the STL: using and extending the C++ Standard Template Library"
for a description of containers, iterators and the C++ STL library.)

We will store pointers to the records in such a container, sorted by
key.  Likewise for the ranges, sorted by the lower end of each range.
Below is the algorithm for doing a set of range queries.  The
MRQ prefix stands for multiple range queries.

<pre>
MRQ_sorted_key_sorted_range(records, ranges):
  initialize(records)
  range_iter = ranges.begin
  <b>while</b> range_iter \f$\neq\f$ ranges.end:
    included_records = RQ_forward_search(records, *iter)
    // Do something with included_records
    ++range_iter
</pre>

<pre>
initialize(container):
  container.first_in_range = container.begin
</pre>

<pre>
RQ_forward_search(records, range):
1  included_records = \f$\emptyset\f$
2  <b>while</b> (records.first_in_range \f$\neq\f$ records.end <b>and</b> (*records.first_in_range).key < range.min):
3    ++records.first_in_range
4  iter = records.first_in_range
5  <b>while</b> (*iter).key \f$\leq\f$ range.max:
6    included_records += iter
7    ++iter
8  <b>return</b> included_records
</pre>

Let there be \em N elements and \em Q queries.  Let \em T be the total number of 
records in query ranges, counting multiplicities.  The computational complexity
of doing a set of range queries is \f$\mathcal{O}(N + Q + T)\f$.  
Iterating over the query ranges introduces the \f$\mathcal{O}(R)\f$ term.
Searching for the beginning of each range accounts for \f$\mathcal{O}(N)\f$.  
This occurs on lines 2 and 3 of RQ_forward_search().  Finally,
collecting the included records (lines 4-7) accounts for the 
\f$\mathcal{O}(T)\f$ term.  To match the previous notation, let \em I be the 
average number of records in a query.  The average cost of a 
single query is \f$\mathcal{O}(N/Q + I)\f$

The preprocessing time for making the data structure is 
\f$\mathcal{O}(N \log N + Q \log Q)\f$ because the records and query ranges
must be sorted.  If the records and query ranges change by small amounts,
the reprocessing time is \f$\mathcal{O}(N + Q)\f$ because the records and 
query ranges can be resorted with an insertion sort.  The storage 
requirement is linear in the number of records and query ranges.

\f[
\mathrm{Preprocess} = \mathcal{O}(N \log N + Q \log Q),
\quad
\mathrm{Reprocess} = \mathcal{O}(N + Q),
\f]
\f[
\mathrm{Storage} = \mathcal{O}(N + Q),
\quad
\mathrm{Query} = \mathcal{O}(N/Q + I)
\f]
*/



















//=============================================================================
//=============================================================================
/*!
\page geom_orq_morq Multiple Orthogonal Range Queries


<!--------------------------------------------------------------------------->
\section geom_orq_morq_forward Cells Coupled with Forward Searching


In this section we extend the algorithm for doing multiple
1-D range queries to higher dimensions.  Note that one can
couple the cell method with other search data structures.  For
instance, one could sort the records in each of the cells or store
those records in a search tree. Most such combinations of data
structures do not offer any advantages.  However, there are some that
do.  For example, we 
\ref geom_orq_orq_binary "coupled a binary search to a cell array."  
For this data structure, we can access a record with array indexing in the
first \em K - 1 dimensions and a binary search in the final dimension.

Coupling the forward search
of the previous section, which was designed for multiple queries, with 
a cell array makes sense.  The data structure is little changed.  Again
there is a dense cell array which spans \em K - 1 dimensions.  
In each cell, we store records sorted in the remaining dimension.
However, now each cell has the first_in_range attribute.
In performing range queries, we access records with array indexing in
\em K - 1 dimensions and a forward search in the final dimension.

We construct the data structure by cell sorting the records and then
sorting the records within each cell.  Then we sort the query ranges 
by their minimum key in the final dimension.
Let the data structure have the
attribute \em min which returns the minimum multikey in the
domain and the attribute \em delta which returns the size of a
cell.  Let \em cells be the cell array.  Below are the functions
for constructing and initializing the data structure.

<pre>
construct(file):
  <b>for</b> record <b>in</b> file:
    cells[multikey_to_cell_index(record.multikey)] += record
  <b>for</b> cell <b>in</b> cells:
    sort_by_last_key(cell)
  sort_by_last_key(queries)
  <b>return</b>
</pre>

<pre>
multikey_to_cell_index(multikey):
  <b>for</b> k \f$\in\f$ [0..K-1):
    index[k] \f$= \lfloor\f$(multikey[k] - min[k]) / delta[k]\f$\rfloor\f$
  <b>return</b> index
</pre>

<pre>
initialize():
  <b>for</b> cell <b>in</b> cells:
    cell.first_in_range = cell.begin
</pre>


Each orthogonal range query consists of accessing cells that overlap
the domain and then doing a forward search followed by a sequential
scan on the sorted records in each of these cells.  Below is the
orthogonal range query method.

<pre>
MORQ_cell_forward_search(range):
  included_records = \f$\emptyset\f$
  min_index = multikey_to_index(range.min)
  max_index = multikey_to_index(range.max)
  <b>for</b> index <b>in</b> [min_index..max_index]:
    cell = cells[index]
    <b>while</b> (cell.first_in_range \f$\neq\f$ cell.end <b>and</b> (*cell.first_in_range).multikey[K-1] < range.min[K-1]):
      ++cell.first_in_range
    iter = cell.first_in_range
    <b>while</b> (*iter).multikey[K-1] \f$\leq\f$ range.max[K-1]:
      <b>if</b> *iter \f$\in\f$ range:
        included_records += iter
      ++iter
  <b>return</b> included_records
</pre>



As with cell arrays with binary searching, the query performance
depends on the size of the cells and the query range.  The same
results carry over. The only difference is that the binary search on
records is replaced with a forward search.  If the total number of
records in ranges, \em T, is at least as large as the number of records
\em N, then we expect the forward searching to be less costly.  The
preprocessing time for the cell array with binary search is
\f$\mathcal{O}( M^{1 - 1/K} + N + N \log(N / M^{1- 1/K}) )\f$.  For the
forward search method we must also sort the \em Q queries, so the
preprocessing complexity is \f$\mathcal{O}( M^{1 - 1/K} + N + N \log(N /
M^{1- 1/K}) + Q \log Q )\f$.  The reprocessing time for the cell array
with binary search is \f$\mathcal{O}(M^{1 - 1/K} + N)\f$.  If the records
and query ranges change by small amounts, we can use insertion sort
(combined with cell sort for the records) to resort them.  Thus the
reprocessing complexity for the forward search method is
\f$\mathcal{O}(M^{1 - 1/K} + N + Q)\f$.  
The forward search method requires that we store the sorted query 
ranges.  This the storage requirement is
\f$\mathcal{O} \left( M^{1 - 1/K} + N + Q \right)\f$.


We will determine the expected cost of a query.  
Let \em I the number of records in a single query range.
Let \em J be the number of cells which overlap the query range and
\f$\tilde{I}\f$ be the number of records which are reported or checked for
inclusion in the overlapping cells.  There will be \em J forward searches
to find the starting record in each cell.  
The total cost of the forward searches for all the queries (excluding
the cost of starting the search in each cell) is 
\f$\mathcal{O}(N)\f$.  Thus the average cost of the forward searching
per query is \f$\mathcal{O}(J + N/Q)\f$.
As with the binary search method, we suppose that the cells are no 
larger than the query
range and that both are roughly cubical (except in the forward search
direction).  Let \em R be the ratio of the
length of a query range to the length of a cell in a given coordinate.
In this case \f$J \lesssim (R + 1)^{K-1}\f$.  
The number of records in the overlapping cells that are checked for
inclusion is about \f$\tilde{I} \lesssim (1 + 1/R)^{K-1} I\f$.  
The inclusion tests add a cost of \f$\mathcal{O}((1 + 1/R)^{K-1} I)\f$.
Thus the average cost of a single query is 
\f$\mathcal{O}( (R + 1)^{K-1} + N/Q + (1 + 1/R)^{K-1} I )\f$.


\f[
  \mathrm{Preprocess} = \mathcal{O} \left( M^{1 - 1/K} + N 
    + N \log(N / M^{1- 1/K}) + Q \log Q \right),
\f]
\f[
  \mathrm{Reprocess} = \mathcal{O} \left( M^{1 - 1/K} + N + Q \right),
  \quad
  \mathrm{Storage} = \mathcal{O} \left( M^{1 - 1/K} + N + Q \right),
\f]
\f[
  \mathrm{AverageQuery} 
  = \mathcal{O} \left( (R + 1)^{K-1} + N/Q + (1 + 1/R)^{K-1} I \right),
\f]
\f[
  \mathrm{TotalQueries}
  = \mathcal{O} \left( Q (R + 1)^{K-1} + N + (1 + 1/R)^{K-1} T \right),
\f]




The figures below show the
execution times and storage requirements for the chair problem. 
The performance is similar to the cell array coupled with binary searching.
As expected, the execution times are lower, and the memory usage is
higher.  Because the forward searching is less costly than 
the binary searching, the forward search method is less sensitive to 
cell size.  It has a larger "sweet spot."
For this test, the best execution times are obtained when \em R
is between 1/8 and 1/2.


\image html orq/CellXYForwardSearchZCellSizeChairTime.jpg "The effect of leaf size on the performance of the cell array coupled with forward searches for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison."
\image latex orq/CellXYForwardSearchZCellSizeChairTime.pdf "The effect of leaf size on the performance of the cell array coupled with forward searches for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison." width=0.5\textwidth

\image html orq/CellXYForwardSearchZCellSizeChairMemory.jpg "The effect of leaf size on the performance of the cell array coupled with forward searches for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison."
\image latex orq/CellXYForwardSearchZCellSizeChairMemory.pdf "The effect of leaf size on the performance of the cell array coupled with forward searches for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison." width=0.5\textwidth





The figures below show the
execution times and storage requirements for the random points
problem.  The execution times are better than those for the cell array
with binary searching.  This improvement is significant near the sweet
spot, but diminishes when the cell size is not tuned to the query size.
There is an increase in memory usage from the binary search method.


\image html orq/CellXYForwardSearchZCellSizeRandomTime.jpg "The effect of leaf size on the performance of the cell array coupled with forward searches for the random points problem.  The plot shows the execution time in seconds versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison."
\image latex orq/CellXYForwardSearchZCellSizeRandomTime.pdf "The effect of leaf size on the performance of the cell array coupled with forward searches for the random points problem.  The plot shows the execution time in seconds versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison." width=0.5\textwidth

\image html orq/CellXYForwardSearchZCellSizeRandomMemory.jpg "The effect of leaf size on the performance of the cell array coupled with forward searches for the random points problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison."
\image latex orq/CellXYForwardSearchZCellSizeRandomMemory.pdf "The effect of leaf size on the performance of the cell array coupled with forward searches for the random points problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the cell array coupled with binary searches is shown for comparison." width=0.5\textwidth





In the figures below
we show the best cell size versus the query range size for the random
points problem.  As with the cell array with binary searching, we see 
that the best cell size for this problem is not correlated to the query size.


\image html orq/CellXYForwardSearchZCellSizeRandomBestSize.jpg "The best cell size versus the query range size for the cell array coupled with forward searches on the random points problem."
\image latex orq/CellXYForwardSearchZCellSizeRandomBestSize.pdf "The best cell size versus the query range size for the cell array coupled with forward searches on the random points problem." width=0.5\textwidth

\image html orq/CellXYForwardSearchZCellSizeRandomBestSizeRatio.jpg "The ratio of the cell size to the query range size."
\image latex orq/CellXYForwardSearchZCellSizeRandomBestSizeRatio.pdf "The ratio of the cell size to the query range size." width=0.5\textwidth




<!--------------------------------------------------------------------------->
\section geom_orq_morq_keys Storing the Keys




The most expensive part of doing the orthogonal range queries with the 
cell array coupled with forward searching is actually just accessing the 
records and their keys.  This is because the search method is very efficient
and the forward search needs to access the records.  To cut the cost of 
accessing the records and their keys, we can store the keys in the data
structure.  This will reduce the cost of the forward searches and 
the inclusion tests.  However, there will be a substantial increase in 
memory usage.  We will examine the effect of storing the keys.



The figures below show
the execution times and storage requirements for the chair
problem. The performance has the same characteristics as the data
structure that does not store the keys.  By storing the keys, we
roughly cut the execution time in half.  However, there is a fairly
large storage overhead.  Instead of just storing a pointer to the
record, we store the pointer and three keys.  In this example, the
keys are double precision numbers.  Thus the memory usage goes up by
about a factor of seven.  If the keys had been integers or single
precision floats, the increase would have been smaller.


\image html orq/CellXYForwardSearchKeyZCellSizeChairTime.jpg "The effect of leaf size on the performance of the cell array that stores keys and uses forward searches for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the data structure that does not store the keys is shown for comparison."
\image latex orq/CellXYForwardSearchKeyZCellSizeChairTime.pdf "The effect of leaf size on the performance of the cell array that stores keys and uses forward searches for the chair problem.  The plot shows the execution time in seconds versus \em R.  The performance of the data structure that does not store the keys is shown for comparison." width=0.5\textwidth

\image html orq/CellXYForwardSearchKeyZCellSizeChairMemory.jpg "The effect of leaf size on the performance of the cell array that stores keys and uses forward searches for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the data structure that does not store the keys is shown for comparison."
\image latex orq/CellXYForwardSearchKeyZCellSizeChairMemory.pdf "The effect of leaf size on the performance of the cell array that stores keys and uses forward searches for the chair problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the data structure that does not store the keys is shown for comparison." width=0.5\textwidth




The figures below show the
execution times and storage requirements for the random points
problem.  Again we see that storing the keys improves the execution time at
the price of increased memory usage.


\image html orq/CellXYForwardSearchKeyZCellSizeRandomTime.jpg "The effect of leaf size on the performance of the cell array coupled with forward searches that stores the keys for the random points problem.  The plot shows the execution time in seconds versus \em R.  The performance of the data structure that does not store the keys is shown for comparison."
\image latex orq/CellXYForwardSearchKeyZCellSizeRandomTime.pdf "The effect of leaf size on the performance of the cell array coupled with forward searches that stores the keys for the random points problem.  The plot shows the execution time in seconds versus \em R.  The performance of the data structure that does not store the keys is shown for comparison." width=0.5\textwidth

\image html orq/CellXYForwardSearchKeyZCellSizeRandomMemory.jpg "The effect of leaf size on the performance of the cell array coupled with forward searches that stores the keys for the random points problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the data structure that does not store the keys is shown for comparison."
\image latex orq/CellXYForwardSearchKeyZCellSizeRandomMemory.pdf "The effect of leaf size on the performance of the cell array coupled with forward searches that stores the keys for the random points problem.  The plot shows the memory usage in megabytes versus \em R.  The performance of the data structure that does not store the keys is shown for comparison." width=0.5\textwidth
*/

















//=============================================================================
//=============================================================================
/*!
\page geom_orq_cc Computational Complexity Comparison

The table below gives labels to the orthogonal range
query methods that we will compare.  It also gives a reference to the 
section in which the method is introduced.  Related methods are grouped 
together.  

<table>
<tr> 
<th> label
<th> description
<th> reference
<tr> 
<td> seq. scan
<td> sequential scan
<td> \ref geom_orq_orq_sequential "section"
<tr> 
<td> projection
<td> projection
<td> \ref geom_orq_orq_projection "section"
<tr> 
<td> pt-in-box
<td> point-in-box
<td> \ref geom_orq_orq_point "section"
<tr> 
<td> kd-tree
<td> kd-tree
<td> \ref geom_orq_orq_kdtree "section"
<tr> 
<td> kd-tree d.
<td> kd-tree with domain checking
<td> \ref geom_orq_orq_kdtree "section"
<tr> 
<td> octree
<td> octree
<td> \ref geom_orq_orq_octree "section"
<tr> 
<td> cell
<td> cell array
<td> \ref geom_orq_orq_cell "section"
<tr> 
<td> sparse cell
<td> sparse cell array
<td> \ref geom_orq_orq_sparse "section"
<tr> 
<td> cell b. s.
<td> cell array with binary search
<td> \ref geom_orq_orq_binary "section"
<tr> 
<td> cell f. s.
<td> cell array with forward search
<td> \ref geom_orq_morq_forward "section"
<tr> 
<td> cell f. s. k.
<td> cell array with forward search on keys
<td> \ref geom_orq_morq_keys "section"
</table>
Labels and references for the orthogonal range query methods.




The table below lists the 
expected computational complexity for an orthogonal range query 
and the storage requirements of the data structure for each of
the presented methods.  We consider the case that the query range is
small and cubical.  To review the notation:
There are \em N records in <em>K</em>-dimensional space.
There are \em I records in the query range.
There are \em M cells in the dense cell array method.  \em R is the ratio of 
the length of a query range to the length of a
cell in a given coordinate.
The depth of the octree is \em D.
For multiple query methods, \em Q is the number of queries.

<table>
<tr>
<th> Method
<th> Expected Complexity of ORQ
<th> Storage
<tr>
<td> seq. scan
<td> \em N
<td> \em N
<tr>
<td> projection
<td> \f$K \log N + N^{1 - 1 / K}\f$
<td> \em K \em N
<tr>
<td> pt-in-box
<td> \f$K \log N + N^{1 - 1 / K}\f$
<td> \f$(2 K + 1) N\f$
<tr>
<td> kd-tree
<td> \f$\log N + I\f$
<td> \em N
<tr>
<td> kd-tree d.
<td> \f$\log N + I\f$
<td> \em N
<tr>
<td> octree
<td> \f$\log N + I\f$
<td> \f$(D + 1) N\f$
<tr>
<td> cell
<td> \f$(R + 1)^K + (1 + 1/R)^K I\f$
<td> \em M + \em N
<tr>
<td> sparse cell
<td> \f$(R + 1)^{K - 1} \log \left( M^{1/K} \right) + (R + 1)^K + (1 + 1/R)^K I\f$
<td> \f$M^{1 - 1/K} + N\f$
<tr>
<td> cell b. s.
<td> \f$(R + 1)^{K - 1} \log \left( N / M^{1 - 1/K} \right) + (1 + 1/R)^{K-1} I\f$
<td> \f$M^{1 - 1/K} + N\f$
<tr>
<td> cell f. s.
<td> \f$(R + 1)^{K-1} + N/Q + (1 + 1/R)^{K-1} I\f$
<td> \f$M^{1 - 1/K} + N\f$
<tr>
<td> cell f. s. k.
<td> \f$(R + 1)^{K-1} + N/Q + (1 + 1/R)^{K-1} I\f$
<td> \f$M^{1 - 1/K} + N\f$
</table>
Computational complexity and storage requirements for the orthogonal 
range query methods.



The sequential scan is the brute force method and is rarely practical.

For the projection and point-in-box methods the \f$N^{1 - 1 / K}\f$ term, which is
the expected number of records in a slice, typically dominates the 
query time.  This strong dependence on \em N means that the projection methods
are usually suitable when the number of records is small.  The 
projection methods also have a fairly high storage overhead, storing
either \em K or \f$2 K + 1\f$ arrays of length \em N.

Although the leaf size does not appear in the computational complexity
or storage complexity, choosing a good leaf size is fairly important
in getting good performance from tree methods.
<!--CONTINUE: do the octree work first.-->


Dense cell array methods are attractive when the distribution of records 
is such that the memory overhead of the cells is not too high.  A 
uniform distribution of records would be the best case.  If one can 
afford the memory overhead of cells, then one can often choose a cell size that
balances the cost of cell accesses, \f$\mathcal{O}((R + 1)^K)\f$, and 
inclusion tests, \f$\mathcal{O}((1 + 1/R)^K I)\f$, to obtain a method that 
is close to linear complexity in the number of included records.


If the dense cell array method requires too much memory, then a 
sparse cell array or a cell array coupled with a binary search on records
may give good performance.  Both use a binary search;
the former to access cells and the latter to access records.  Often 
the cost of the binary searches, 
(\f$\mathcal{O}((R + 1)^{K - 1} \log \left( M^{1/K} \right))\f$
and \f$\mathcal{O}((R + 1)^{K - 1} \log \left( N / M^{1 - 1/K} \right))\f$, respectively), 
is small compared to the costs of the inclusion tests.  This is because 
the number of cells, \f$\mathcal{O}(M^{1/K})\f$, or the number of records, 
\f$\mathcal{O}(N / M^{1 - 1/K})\f$, in the search is small.


For multiple query problems, if the total number \em T of records in 
query ranges
is at least as large as the number of records \em N then a cell array
coupled with forward searching will likely give good performance.
In this case the cost of the searching, \f$\mathcal{O}(N/Q)\f$, will be small
and the cell size can be chosen to balance the cost of accessing cells,
\f$\mathcal{O}((R + 1)^{K-1})\f$, with the cost of inclusion tests,
\f$\mathcal{O}((1 + 1/R)^{K-1} I)\f$.
*/












//=============================================================================
//=============================================================================
/*!
\page geom_orq_fileSizes Performance Tests for Multiple Queries over a Range of File Sizes





<!--------------------------------------------------------------------------->
\section geom_orq_fileSizes_chair Points on the Surface of a Chair


We consider the chair data set, introduced  
\ref geom_orq_orq_test "previously".  
By refining the surface mesh of the chair, we vary
the number of records from 1,782 to 1,864,200.  There is unit
spacing between adjacent records.  We perform a cubical orthogonal
range query of size 4 around each record.  
The table below 
shows the total number of records returned for the six tests.

The next table shows the execution times for 
the chair problems.  The leaf sizes and cell sizes are chosen to minimize 
the execution time.  The memory usage is shown in the following table.
An entry of "o.t." indicates that the test exceeded the time limit.
An entry of "o.m." means out of memory.  (Note that the kd-tree method with
domain checking has the same memory usage as the kd-tree method
without domain checking as the data structure is the same.)

<table>
<tr>
<th> # records
<td> 1,782
<td> 7,200
<td> 28,968
<td> 116,232
<td> 465,672
<td> 1,864,200
<tr>
<th> # returned
<td> 65,412
<td> 265,104
<td> 864,296
<td> 3,192,056
<td> 12,220,376
<td> 47,768,216
</table>
The total number of records returned by the orthogonal range 
queries for the chair problems.




<table>
<tr>
<th> # records
<td> 1,782
<td> 7,200
<td> 28,968
<td> 116,232
<td> 465,672
<td> 1,864,200
<tr>
<th> seq. scan
<td> 0.195 <td> 3.453 <td> 98.72 <td> o.t. <td> o.t. <td> o.t. 
<tr>
<th> projection
<td> 0.061 <td> 0.480 <td> 3.86 <td> 33.01 <td> 322.0 <td> o.t. 
<tr>
<th> pt-in-box
<td> 0.045 <td> 0.366 <td> 2.94 <td> 25.78 <td> 205.9 <td> o.t. 
<tr>
<th> kd-tree
<td> 0.081 <td> 0.383 <td> 1.94 <td> 9.58 <td> 46.7 <td> o.t. 
<tr>
<th> kd-tree d.
<td> 0.101 <td> 0.475 <td> 2.50 <td> 12.66 <td> 63.0 <td> o.t. 
<tr>
<th> octree
<td> 0.035 <td> 0.164 <td> 0.78 <td> 3.12 <td> 13.4 <td> 56
<tr>
<th> cell
<td> 0.024 <td> 0.102 <td> 0.37 <td> 1.41 <td> 5.6 <td>  o.m.
<tr>
<th> sparse cell
<td> 0.025 <td> 0.108 <td> 0.40 <td> 1.50 <td> 5.9 <td> 25 
<tr>
<th> cell b. s.
<td> 0.028 <td> 0.121 <td> 0.49 <td> 1.89 <td> 8.1 <td> 34
<tr>
<th> cell f. s.
<td> 0.019 <td> 0.081 <td> 0.31 <td> 1.24 <td> 5.0 <td> 21
<tr>
<th> cell f. s. k.
<td> 0.013 <td> 0.055 <td> 0.23 <td> 0.93 <td> 3.8 <td> 17
</table>
The total execution time for the orthogonal range queries for the
chair problem with a query size of 4.




<table>
<tr>
<th> # records
<td> 1,782
<td> 7,200
<td> 28,968
<td> 116,232
<td> 465,672
<td> 1,864,200
<tr>
<th> seq. scan
<td> 7,140 <td> 28,812 <td> 115,884 <td> o.t. <td> o.t. <td> o.t. 
<tr>
<th> projection
<td> 21,420 <td> 86,436 <td> 347,652 <td> 1,394,820 <td> 5,588,100 <td> o.t. 
<tr>
<th> pt-in-box
<td> 49,980 <td> 201,684 <td> 811,188 <td> 3,254,580 <td> 13,038,900 <td> o.t. 
<tr>
<th> kd-tree
<td> 17,416 <td> 69,808 <td> 279,760 <td> 1,120,336 <td> 4,484,176 <td> o.t. 
<tr>
<th> octree
<td> 62,708 <td> 272,212 <td> 1,160,492 <td> 4,520,452 <td> 17,979,204 <td> 72,681,460
<tr>
<th> cell
<td> 32,328 <td> 206,412 <td> 1,446,540 <td> 10,760,556 <td> 82,850,988 <td> o.m.
<tr>
<th> sparse cell
<td>14,360 <td> 63,088 <td> 252,976 <td> 1,013,680 <td> 4,058,800 <td> 16,243,888
<tr>
<th> cell b. s.
<td> 8,836 <td> 34,684 <td> 137,884 <td> 550,300 <td> 2,199,196 <td> 8,793,244
<tr>
<th> cell f. s.
<td> 16,764 <td> 66,372 <td> 264,708 <td> 1,057,860 <td> 4,230,084 <td> 16,918,212
<tr>
<th> cell f. s. k.
<td> 63,132 <td> 252,168 <td> 1,009,224 <td> 4,039,272 <td> 16,163,112 <td> 64,665,768
</table>
The memory usage of the data structures for the chair problem.


\image html orq/CompareFileSizeChairTime.jpg "Log-log plots of the execution times versus the number of reported records and the memory usage versus the number of records in the file for each of the orthogonal range query methods on the chair problems.  The execution time is shown in microseconds per returned record."
\image latex orq/CompareFileSizeChairTime.pdf "Log-log plots of the execution times versus the number of reported records and the memory usage versus the number of records in the file for each of the orthogonal range query methods on the chair problems.  The execution time is shown in microseconds per returned record." width=0.9\textwidth

\image html orq/CompareFileSizeChairMemory.jpg "The memory usage is shown in bytes per record."
\image latex orq/CompareFileSizeChairMemory.pdf "The memory usage is shown in bytes per record." width=0.9\textwidth



The figures above show the execution times
and memory usage for the various methods.  First consider the tree
methods.  The octree has significantly lower execution times than the
kd-tree methods.  It performs the orthogonal range queries in less
than half the time of the kd-tree methods.  This is because the
records are regularly spaced.  Having regularly spaced records avoids
the primary problem with using octrees: the octree may be much deeper
than a kd-tree.  However, memory usage is a different story.  The
octree uses about four times the memory of the kd-tree methods.

Next consider the cell methods.  The dense cell array has reasonable
execution times, but the the memory usage increases rapidly with the
problem size.  Recall that the cell size is chosen to minimize
execution time.  Following this criterion, the dense cell array runs
out of memory for the largest test case.  The execution times of the
sparse cell array are close to those of the dense cell array.
However, it uses much less memory.  Like the rest of the cell methods,
the memory usage is proportional to the problem size.  The cell array
with binary searches has higher execution times than the other cell
methods but has the lowest memory requirements.  The cell array with
forward searching has lower execution times than than above methods.
Its memory usage is about the same as the sparse cell array.  Finally,
storing the keys with the cell array coupled with forward searching
gives the lowest execution times at the price of a higher memory
overhead.

Among the tree methods, the octree offered the lowest execution times.
For cell methods, the sparse cell array and the cell array with
forward searching have good performance.  These two cell methods
perform significantly better than the octree.  The cell methods do the
queries in about half the time of the octree method and use one
quarter of the memory.  The cell array with forward searching has the
lower execution times of the two cell methods.

















<!--------------------------------------------------------------------------->
\section geom_orq_fileSizes_cube Randomly Distributed Points in a Cube


We consider the \ref geom_orq_orq_test "random points data set". 
The number of records varies from 100 to 1,000,000. 
We perform cubical orthogonal range queries around each record.  
The query size is chosen to contain an average of about 10 records.
The table below
shows the total number of records returned for the five tests.

The next table shows the execution times for 
the chair problems.  Again, the leaf sizes and cell sizes are chosen to 
minimize the execution time.  The memory usage is shown in 
the following table.
An entry of "o.t." indicates that the test exceeded the time limit.
(The kd-tree method with domain checking has the same memory usage as
the kd-tree method without domain checking as the data structure is
the same.)

<table>
<tr>
<th> # records
<td> 100
<td> 1,000
<td> 10,000
<td> 100,000
<td> 1,000,000
<tr>
<th> # returned
<td> 886
<td> 9,382
<td> 102,836
<td> 1,063,446
<td> 10,842,624
</table>
The total number of records returned by the orthogonal range 
queries for the random points problems.





<table>
<tr>
<th> # records
<td> 100
<td> 1,000
<td> 10,000
<td> 100,000
<td> 1,000,000
<tr>
<th> seq. scan
<td> 0.00071 <td> 0.0683 <td> 8.173 <td> o.t. <td> o.t.
<tr>
<th> projection
<td> 0.00077 <td> 0.0281 <td> 1.323 <td> 104.15 <td> o.t.
<tr>
<th> pt-in-box
<td> 0.00061 <td> 0.0254 <td> 1.304 <td> 112.61 <td> o.t.
<tr>
<th> kd-tree
<td> 0.00095 <td> 0.0154 <td> 0.205 <td> 3.56 <td> 44
<tr>
<th> kd-tree d.
<td> 0.00114 <td> 0.0187 <td> 0.268 <td> 4.31 <td> 52
<tr>
<th> octree
<td> 0.00079 <td> 0.0212 <td> 0.363 <td> 7.08 <td> 92
<tr>
<th> cell
<td> 0.00091 <td> 0.0135 <td> 0.173 <td> 3.03 <td> 36
<tr>
<th> sparse cell
<td> 0.00101 <td> 0.0146 <td> 0.201 <td> 3.24 <td> 39
<tr>
<th> cell b. s.
<td> 0.00088 <td> 0.0109 <td> 0.140 <td> 1.74 <td> 27
<tr>
<th> cell f. s.
<td> 0.00068 <td> 0.0082 <td> 0.109 <td> 1.15 <td> 16
<tr>
<th> cell f. s. k.
<td> 0.00050 <td> 0.0054 <td> 0.067 <td> 0.77 <td> 11
</table>
The total execution time for the orthogonal range queries for
the random points problem.



<table>
<tr>
<th> # records
<td> 100
<td> 1,000
<td> 10,000
<td> 100,000
<td> 1,000,000
<tr>
<th> seq. scan
<td> 412 <td> 4,012 <td> 40,012 <td> o.t. <td> o.t.
<tr>
<th> projection
<td> 1,236 <td> 12,036 <td> 120,036 <td> 1,200,036 <td> o.t.
<tr>
<th> pt-in-box
<td> 2,884 <td> 28,084 <td> 280,084 <td> 2,800,084 <td> o.t.
<tr>
<th> kd-tree
<td> 1,088 <td> 9,168 <td> 121,968 <td> 1,055,408 <td> 9,242,928
<tr>
<th> octree
<td> 4,628 <td> 46,280 <td> 398,488 <td> 3,425,956 <td> 30,199,580
<tr>
<th> cell
<td> 724 <td> 5,500 <td> 52,000 <td> 527,776 <td> 5,245,876
<tr>
<th> sparse cell
<td> 928 <td> 6,400 <td> 57,600 <td> 578,112 <td> 5,696,368
<tr>
<th> cell b. s.
<td> 652 <td> 4,508 <td> 41,708 <td> 407,852 <td> 4,035,452
<tr>
<th> cell f. s.
<td> 1,124 <td> 8,708 <td> 82,508 <td> 811,724 <td> 8,053,124
<tr>
<th> cell f. s. k.
<td> 3,848 <td> 33,608 <td> 326,108 <td> 3,229,148 <td> 32,132,648
</table>
The memory usage of the data structures for the random points problem.




\image html orq/CompareFileSizeRandomTime.jpg "Log-log plots of the execution times versus the number of reported records and the memory usage versus the number of records in the file for each of the orthogonal range query methods on the random points problems.  The execution time is shown in microseconds per returned record."
\image latex orq/CompareFileSizeRandomTime.pdf "Log-log plots of the execution times versus the number of reported records and the memory usage versus the number of records in the file for each of the orthogonal range query methods on the random points problems.  The execution time is shown in microseconds per returned record." width=0.9\textwidth

\image html orq/CompareFileSizeRandomMemory.jpg "The memory usage is shown in bytes per record."
\image latex orq/CompareFileSizeRandomMemory.pdf "The memory usage is shown in bytes per record." width=0.9\textwidth


The figures above show the execution times
and memory usage for the various methods.  First consider the tree
methods.  The kd-tree methods have lower execution times than the
octree method.  This result differs from the chair problems, because
now the records are not regularly spaced.  Where records are close to
each other, the octree does more subdivision.  Because of the small
query size, it is advantageous to not do domain checking in the
kd-tree algorithm.  Finally, we note that the kd-tree methods use about a
third of the memory of the octree.

Next consider the cell methods.  The dense and sparse cell arrays
have the highest execution times,  but they have fairly low
memory requirements.  Since the records are distributed throughout the 
domain, there are few empty cells, and there is nothing to be gained by
using a sparse array over a dense array.  However, the penalty for using
the sparse array (in terms of increased cell access time and increased
memory usage) is small.  The cell array with binary searching outperforms
the dense and sparse cell arrays.  It has lower execution times and 
uses less memory.  The execution times of the cell array with forward 
searching are lower still.  However, storing the sorted queries increases
the memory usage.  Finally, by storing the keys one can obtain the best
execution times at the price of higher memory usage.


Among the tree methods, the kd-tree without domain checking offered
the best performance.  For cell methods, the cell array with binary
searching and the cell array with forward searching have low execution
times.  These two cell methods outperform the kd-tree method.  The
cell array with binary searching does the queries in about half the
time of the kd-tree while using less than the half the memory.  The
cell array with forward searching has significantly lower execution
times than the binary search method, but uses about twice the memory.
*/













//=============================================================================
//=============================================================================
/*!
\page geom_orq_conclusions Conclusions



The performance of orthogonal range query methods depends on many
parameters: the dimension of the record's multikey, the number of records
and their distribution and the query range.  The performance may also
depend on additional parameters associated with the data structure,
like leaf size for tree methods or cell size for cell methods.  Also
important are the implementation of the algorithms and the computer
architecture.  There is no single best method for doing orthogonal
range queries.  Some methods perform well over a wide range of
parameters.  Others perform well for only a small class of problems.
In this section we will compare the methods previously presented.  We
will base our conclusions on the numerical experiments.  Thus we
restrict our attention to records with 3-D multikeys and
cubic query ranges.  Also, only a few distributions of the records
were tested.  Finally, note that all codes were implemented in C++ and
executed on a 450 MHz i686 processor with 256 MB of memory.




\section geom_orq_conclusions_projection Projection Methods

The projection methods have the advantage that they are relatively
easy to implement.  The fundamental operations in these methods are
sorting an array and doing binary searches on that array.  These
operations are in many standard libraries.  (The C++ STL library
provides this functionality.)  Also, projection methods are easily
adaptable to dynamic problems.  When the records change, one merely
resorts the arrays.  However, the projection method and the related
point-in-box method usually perform poorly.  The execution time does
not scale well with the file size, so the methods are only practical
for small files.  The memory usage of the projection method is
moderate.  It is typically not much higher than a kd-tree.  The
point-in-box method has a high memory requirement.  Storing the rank
arrays in order to do integer comparisons more than doubles the memory
usage.  Also, on x86 processors, doing integer instead of floating
point comparisons usually increases execution time.  So the
"optimization" of integer comparisons becomes a performance penalty.
The projection method typically outperforms the point-in-box method,
but neither is recommended for time critical applications.



\section geom_orq_conclusions_tree Tree Methods


Kd-trees usually outperform octrees by a moderate factor.  The octree
typically has higher execution times and uses several times the memory
of a kd-tree.  This is because the octree partitions space while the
kd-tree partitions the records.  The high memory usage of the octree
is also a result of storing the domain of each sub-tree.  The
exception to this rule is when the records are regularly spaced, as in
the chair problem.  Then the octree may have lower execution times
than the kd-tree.  The octree is also more easily adapted to dynamic
problems than the kd-tree.

For kd-trees, it is advantageous to do domain checking during the
orthogonal range query only when the query range is large.  For small
queries, it is best to not do domain checking and thus get faster access
to the leaves.  For a given problem, kd-trees are typically not the
best method for doing orthogonal range queries; there is usually a
cell method that has better execution times and uses less memory.
However, kd-trees perform pretty well in a wide range of problems and
the performance is only moderately sensitive to leaf size.




\section geom_orq_conclusions_cell Cell Methods


The dense cell array method performs very well on problems for which it is
well suited.  The structure of tree methods amortizes the cost of
accessing leaves.  The cell array offers constant time
access to any cell.  If the cell array with cell size chosen to
optimize execution time will fit in memory, then the
cell array will usually have lower execution times than any tree
method.  Depending on the number and distribution of the records, the
memory usage may be quite low or very high.  The performance of cell
arrays is fairly sensitive to the cell size.

It has been reported that cell arrays are
applicable only in situations when the query size is fixed and that in this
case the cell size should be chosen to match the query size.  
(See \ref geom_orq_bentley_1979 "Data Structures for Range Searching.")
This was
not the case in my experiments.  For the tests in 
\ref geom_orq_querySizes 
the cell array with a fixed cell size
performed well over a wide range of query sizes.  Choosing the best
cell size has more to do with the distribution of the records than
with the size of the query range.


Sparse cell arrays usually have execution times that are almost as low
as dense cell arrays.  The binary search to access cells has little
effect on performance.  If the dense cell array has many empty cells,
then the sparse cell array may offer significant savings in memory.
However, if the records are distributed throughout the domain, using
the sparse array structure only increases the memory usage and access
time to a cell.  Like the dense cell array, the performance of the
sparse cell array is fairly sensitive to the cell size.


Cell arrays coupled with binary searching are similar in structure to
sparse cell arrays.  The former searches on records while the latter
searches on cells.  The execution times are usually comparable with
the other cell methods.  However, the memory usage is typically lower
and the performance is less sensitive to cell size.  In fact, the
memory usage is often little more than the sequential scan method
which stores only a single pointer for each record.  It is interesting
to note that like the projection methods, there is a binary search on
records.  However, the projection methods perform this search on the
entire file, while the cell array coupled with binary searching
searches only within a single cell.  The combination of low execution
times, low memory usage and insensitivity to cell size make this an
attractive method for many problems.






\section geom_orq_conclusions_multiple Multiple Queries

For multiple query problems where the total number of records inside query
ranges is at least as large as the number records, the cell array
coupled with forward searching typically performs very well.  To 
store the records, it uses almost the same data structure as the cell array
coupled with binary searching.  However, its method of sweeping through
the records is more efficient than the binary search.  Having to store
the queries so that they can be processed in order increases the memory
usage from light to moderate.


One can moderately decrease the execution time (a factor of 1/3 was
common in the tests) by storing the multikeys to avoid accessing the 
records.  However, this does increase the memory usage.  Storing the 
keys is a useful optimization in situations when execution time is 
critical and available memory is sufficient.
*/





//=============================================================================
//=============================================================================
/*!
\page geom_orq_usage Usage

Each of the ORQ classes is templated on the following:
- N: The space dimension.
- _Record: The record type.
- _MultiKey: An N-tuple of keys.
- _Key: The number type for keys.
- _MultiKeyAccessor: A functor that takes a record as an argument and returns
  its multikey.  (If possible, a const reference to its multikey.)
.
For example, the dense cell array is declared as:
\code
template<int N,
	 typename _Record,
	 typename _MultiKey = typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class CellArray;
\endcode

Note that the KDTree and the Octree have an additional template parameter:
a record output iterator.  Also, the Octree is not templated on the 
space dimension.

The last three template parameters have default values.  These are
useful in the case that the record type is a handle to a multikey.
("Handle" is a generic term which includes pointers and iterators.)
Then the multikey type can be obtained by taking the value type of
this handle.  The key type can be obtained from the multikey if the
multikey is an STL-style container.  By default the multikey accessor
is just a functor which dereferences the record.

<b>Simple scenario.</b>


Consider the following usage scenario: You are performing a simulation with
particles.  You use orthogonal range queries to determine the particles
which are in a neighborhood of a given particle.  A particle has position,
velocity, and possibly other fields.  You store each of these fields in
separate containers, say \c std::vector .

\code
typedef ads::FixedArray<3, double> Point;
std::vector<Point> positions, velocities;
\endcode

Here is how to use a cell array to perform orthogonal range queries.

\code
// A record is a const iterator into the container of points.
typedef std::vector<Point>::const_iterator Record;

// Define the class we will use for orthogonal range queries.  It can deduce
// the multi-key type from the record type by dereferencing.  Since 
// ads::FixedArray is an STL-compliant container, the number type can be 
// deduced from the multi-key type.  Finally, the multi-key accessor is the
// default functor, which just dereferences the record.
typedef CellArray<3, Record> Orq;
// Define some geometric types.
typedef Orq::SemiOpenIntervalType SemiOpenIntervalType;
typedef Orq::BBox BBox;
typedef Orq::Point Point;

// Build the ORQ class.  The suggested cell size is 0.1 x 0.1 x 0.1.  The 
// domain is the semi-open interval [-1..1) x[-1..1) x[-1..1).  We insert
// all of the records in the positions vector.
Orq orq(Point(0.1, 0.1, 0.1), SemiOpenInterval(Point(-1, -1, -1), Point(1, 1, 1)), positions.begin(), positions.end());
\endcode

Now we are ready to perform window queries.
\code
// We want to get the records in the window [0..0.5] x [0..0.5] x [0..0.5].
BBox window(Point(0, 0, 0), Point(0.5, 0.5, 0.5));
std::vector<Record> inside;
orq.computeWindowQuery(std::back_inserter(inside), window);
std::cout << "Positions and velocities of the records in the window.\n";
for (std::vector<Record>::const_iterator i = inside.begin(); i != inside.end(); ++i) {
  // i is an iterator to a record.  *i is a record.  **i is multi-key, 
  // which is a Cartesian point.  We can take the difference of the record
  // *i and the record positions.begin() to get the index of the record in
  // our container: positions.  This index lets us access the velocity.
  std::cout << "Position = " << **i << ", Velocity = " << velocities[*i - positions.begin()] << "\n";
}
inside.clear();
\endcode





<b>More sophisticated scenario.</b>

Now suppose that instead of storing each attribute of a particle in a separate 
container, you use a class to represent a particle.

\code
class Particle {
public:
  typedef ads::FixedArray<3, double> Point;

private:
  Point _position;
  Point _velocity;
  ...

public:
  const Point&
  getPosition() const {
    return _position;
  }
  ...
};
\endcode

Suppose that you store the particles in a \c std::vector.
\code
std::vector<Particle> particles;
\endcode

Here is how to use a cell array to perform orthogonal range queries.

\code
// A record is a const iterator into the container of particles.
typedef std::vector<Particle>::const_iterator Record;
// The particle point type is the multikey.
typedef Particle::Point MultiKey;

// The functor to access the multikey.
struct ParticleLocationAccessor : 
  public std::unary_function<Record,MultiKey> {
  const MultiKey&
  operator()(const Record& record) const {
    return record->getPosition();
  }
}

// Define the class we will use for orthogonal range queries.
typedef CellArray<3, Record, MultiKey, double, ParticleLocationAccessor> Orq;
typedef Orq::SemiOpenIntervalType SemiOpenIntervalType;
typedef Orq::BBox BBox;
typedef Orq::Point Point;

// Build the ORQ class.
Orq orq(Point(0.1, 0.1, 0.1), SemiOpenInterval(Point(-1, -1, -1), Point(1, 1, 1)));
for (Record i = particles.begin(), i != particles.end(); ++i) {
  orq.insert(i);
}
\endcode

Now we are ready to perform window queries.
\code
BBox window(Point(0, 0, 0), Point(0.5, 0.5, 0.5));
// Just for variety, we'll store the records in a std::list.
std::list<Record> inside;
orq.computeWindowQuery(std::back_inserter(inside), window);
Particle::Point position;
for (std::list<Record>::const_iterator i = inside.begin(); i != inside.end(); ++i) {
  position = (*i)->getLocation();
  ...
}
inside.clear();
\endcode
*/






//=============================================================================
//=============================================================================
/*!
\page geom_orq_bibliography Bibliography

\anchor geom_orq_laursen_1991
<pre>
Laursen, T. A. and Simo, J. C.,
Springer-Verlag, Berlin,
On the formulation and numerical treatment of finite deformation frictional contact problems.,
Nonlinear Computational Mechanics - State of the Art, 716-736, 1987
</pre>


\anchor geom_orq_attaway_1998
<pre>
S. W. Attaway, B. A. Hendrickson, S. J. Plimpton, D. R. Gardner, C. T. Vaughan, K. H. Brown, and M. W. Heinstein,
A parallel contact detection algorithm for transient solid dynamics simulations using PRONTO3D,
Computational Mechanics, 22, 2, 143-159, August 1998
</pre>


\anchor geom_orq_heinstein_1993
<pre>
M. W. Heinstein, S. W. Attaway, F. J. Mello, J. W. Swegle,
A general-purpose contact detection algorithm for nonlinear structural analysis codes,
Sandia National Laboratories, Albuquerque, NM,
1993
</pre>


\anchor geom_orq_bentley_1979
<pre>
J. L. Bentley and J. H. Friedman,
Data Structures for Range Searching,
Computing Surveys, 11, 4, 397-409, December 1979
</pre>


\anchor geom_orq_austern_1999
<pre>
M. H. Austern,
Generic programming and the STL: using and extending the C++ Standard Template Library,
Addison Wesley Longman, Inc., Reading, Massachusetts, 1999
</pre>


\anchor geom_orq_cormen_2001
<pre>
Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein,
Introduction to Algorithms, Second Edition,
The MIT Press, Cambridge, Massachusetts, 2001
</pre>
*/

#if !defined(__geom_orq_h__)
#define __geom_orq_h__

#include "orq/CellArray.h"
#include "orq/CellBinarySearch.h"
#include "orq/CellForwardSearch.h"
#include "orq/CellForwardSearchKey.h"
#include "orq/KDTree.h"
#include "orq/Placebo.h"
#include "orq/PlaceboCheck.h"
#include "orq/SequentialScan.h"
#include "orq/SortFirst.h"
#include "orq/SortFirstDynamic.h"
#include "orq/SortProject.h"
#include "orq/SortRankProject.h"
#include "orq/SparseCellArray.h"

#endif
