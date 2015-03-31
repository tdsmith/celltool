// -*- C++ -*-

/*! 
  \file geom/spatialIndexing/OrthtreeMap.h
  \brief An orthant tree that uses std::map .
*/

#if !defined(__geom_spatialIndexing_OrthtreeMap_h__)
#define __geom_spatialIndexing_OrthtreeMap_h__

#include "SpatialIndex.h"
#include "Split.h"
#include "Merge.h"
#include "OrthtreeElementNull.h"

#include "../../ads/array/FixedArray.h"
#include "../../ads/functor/constant.h"
#include "../../ads/iterator/TrivialOutputIterator.h"
#include "../../numerical/partition.h"
#include "../../third-party/loki/static_check.h"

#include <iterator>
#include <map>
#include <set>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_geom_spatialIndexing_OrthtreeMap)
#define DEBUG_geom_spatialIndexing_OrthtreeMap
#endif

BEGIN_NAMESPACE_GEOM

//! An orthant tree that uses std::map .
/*!
  \param _Dimension The dimension of the space.
  \param _MaximumLevel The maximum level in the tree.
  \param _Element The data held in each node.
  \param _AutomaticBalancing If automatic balancing is enabled, then coarsening
  and refinement operations will leave the tree in a balanced state.
  This limits the groups of leaves that can be coarsened.  Also, refinement
  in a node may force refinement in adjacent nodes.
  \param _Split The functor which determines the element values in the 
  child nodes when a node is split.  The default functor leaves the elements
  in their default constructed states.
  \param _Merge The functor which determines the element value in the 
  parent node when $\f$2^N\f$ nodes are merged.  The default functor leaves 
  the element in its default constructed state.
  \param _Refine Predicate that determines if an element should be
  refined.
  \param _Coarsen Predicate that determines if a group of \f$2^N\f$ 
  elements should be merged to coarsen the tree.
  \param _Action The default action applied to nodes.
  \param _SpatialIndex The spatial index data structure.  This determines
  the level and position of the node.  It also holds a key for storing the
  node in the \c std::map data structure.

  In N-D space, an \e orthant is a region of space obtained by constraining each
  coordinate to be either positive or negative.  Thus there are \f$2^N\f$
  orthants.  In 2-D and 3-D one typically uses the more familiar terms 
  quadrant and octant.

  A quadtree divides space into quadrants; an octree divides space into 
  octants.  Since this data structure divides N-D space into orthants,
  it must be an \e orthtree.
*/
template<int _Dimension, int _MaximumLevel,
	 typename _Element = OrthtreeElementNull<_Dimension>,
	 bool _AutomaticBalancing = false,
	 class _Split = SplitNull,
	 class _Merge = MergeNull,
	 class _Refine = ads::GeneratorConstant<bool>,
	 class _Coarsen = ads::GeneratorConstant<bool>,
	 class _Action = ads::GeneratorConstant<void>,
	 template<int, int> class _Key = SpatialIndex>
class
OrthtreeMap :
  public std::map<_Key<_Dimension, _MaximumLevel>, _Element> {
  //
  // Private types.
  //
private:

  typedef std::map<_Key<_Dimension, _MaximumLevel>, _Element> Base;

  //
  // Public types.
  //
public:

  //! The key type is the spatial index.
  typedef typename Base::key_type Key;
  //! The element type.
  typedef typename Base::mapped_type Element;

  //
  // Enumerations.
  //
public:

  //! Give enumeration values for the appropriate template parameters and the number of orthants.
  enum {Dimension = _Dimension, MaximumLevel = _MaximumLevel,
	AutomaticBalancing = _AutomaticBalancing,
	NumberOfOrthants = Key::NumberOfOrthants};

  //
  // More public types.
  //
public:

  //! The number type.
  typedef double Number;
  //! A Cartesian point.
  typedef ads::FixedArray<Dimension, Number> Point;
  //! The refinement functor.
  typedef _Split Split;
  //! The coarsening functor.
  typedef _Merge Merge;
  //! The default refinement predicate.
  typedef _Refine Refine;
  //! The default coarsening predicate.
  typedef _Coarsen Coarsen;
  //! The default action on elements.
  typedef _Action Action;

  //! The value type for the map.
  typedef typename Base::value_type value_type;
  //! An iterator in the map.
  typedef typename Base::iterator iterator;
  //! A const iterator in the map.
  typedef typename Base::const_iterator const_iterator;

  //
  // Protected types.
  //
protected:

  typedef typename Key::Level Level;
  typedef typename Key::Coordinate Coordinate;
  typedef typename Key::Code Code;

  //
  // Member data.
  //
private:

  Point _lowerCorner;
  ads::FixedArray<MaximumLevel + 1, Point> _extents;
  Split _split;
  Merge _merge;
  Refine _refine;
  Coarsen _coarsen;
  Action _action;

  //
  // Nested classes.
  //
private:

  //! Compare iterators by their codes.
  struct CompareIterator {
    bool
    operator()(const iterator i, const iterator j) const {
      return i->first < j->first;
    }
  };

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Make an empty tree.
  OrthtreeMap(const Point& lowerCorner, const Point& extents,
	      Split split = Split(),
	      Merge merge = Merge(),
	      Refine refine = Refine(),
	      Coarsen coarsen = Coarsen(),
	      Action action = Action());

  //! Copy constructor.
  OrthtreeMap(const OrthtreeMap& other);

  //! Assignment operator.
  OrthtreeMap&
  operator=(const OrthtreeMap& other);

  //! Destructor.
  ~OrthtreeMap() {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Operations on all nodes.
  //@{
public:

  //! Apply the default function to the element of each node.
  void
  apply() {
    apply(_action);
  }

  //! Apply the specified function to the element of each node.
  template<typename _Function>
  void
  apply(_Function f);

  //! Perform refinement with the default criterion.
  /*!
    \return The number of refinement operations.
  */
  int
  refine(const bool areBalancing = AutomaticBalancing) {
    return refine(_refine, areBalancing);
  }

  //! Perform refinement with the supplied criterion.
  /*!
    \return The number of refinement operations.
  */
  template<typename _Function>
  int
  refine(_Function refinePredicate, bool areBalancing = AutomaticBalancing);

  //! Perform coarsening with the supplied criterion until no more nodes can be coarsened.
  /*!
    \return The number of coarsening operations.
  */
  int
  coarsen(const bool areBalancing = AutomaticBalancing) {
    return coarsen(_coarsen, areBalancing);
  }

  //! Perform coarsening with the supplied criterion until no more nodes can be coarsened.
  /*!
    \return The number of coarsening operations.
  */
  template<typename _Function>
  int
  coarsen(_Function coarsenPredicate, bool areBalancing = AutomaticBalancing);

  //! Perform refinement to balance the tree.
  /*!
    \return The number of refinement operations.
  */
  int
  balance();

private:

  //! Perform a single sweep of coarsening with the supplied criterion.
  /*!
    \return The number of coarsening operations.
  */
  template<typename _Function>
  int
  coarsenSweep(_Function coarsenPredicate, bool areBalancing);

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor accessors.
  //@{
public:

  //! Get a const reference to the default splitting functor.
  const Split&
  getSplit() const {
    return _split;
  }

  //! Get a const reference to the default merging functor.
  const Merge&
  getMerge() const {
    return _merge;
  }

  //! Get a const reference to the default refinement predicate.
  const Refine&
  getRefine() const {
    return _refine;
  }

  //! Get a const reference to the default coarsening predicate.
  const Coarsen&
  getCoarsen() const {
    return _coarsen;
  }

  //! Get a const reference to the default action.
  const Action&
  getAction() const {
    return _action;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor manipulators.
  //@{
public:

  //! Get a reference to the default splitting functor.
  Split&
  getSplit() {
    return _split;
  }

  //! Get a reference to the default merging functor.
  Merge&
  getMerge() {
    return _merge;
  }

  //! Get a reference to the default refinement predicate.
  Refine&
  getRefine() {
    return _refine;
  }

  //! Get a reference to the default coarsening predicate.
  Coarsen&
  getCoarsen() {
    return _coarsen;
  }

  //! Get a reference to the default action.
  Action&
  getAction() {
    return _action;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  using Base::begin;
  using Base::end;
  using Base::size;
  using Base::max_size;
  using Base::empty;

  //! Compute the lower corner of the leaf.
  void
  computeLowerCorner(const Key& key, Point* lowerCorner) const;

  //! Compute the lower corner of the leaf.
  Point
  computeLowerCorner(const Key& key) const {
    Point lowerCorner;
    computeLowerCorner(key, &lowerCorner);
    return lowerCorner;
  }

  //! Get the Cartesian extents of the leaf.
  const Point&
  getExtents(const Key& key) const {
    return _extents[key.getLevel()];
  }

  //! Return true if the element can be refined.
  bool
  canBeRefined(const const_iterator element) const {
    return element->first.canBeRefined();
  }
  
  //! Get the keys that are parents of 2^Dimension leaves.
  template<typename _OutputIterator>
  void
  getParentKeys(_OutputIterator parentKeys) const;

  //! Get the keys that are parents of 2^Dimension leaves and would result in a balanced tree under merging.
  template<typename _OutputIterator>
  void
  getParentKeysBalanced(_OutputIterator parentKeys) const;

  //! Return true if the tree is balanced.
  bool
  isBalanced() const;

  //! Get the adjacent neighbors in the specified direction in a balanced tree.
  /*!
    Since the tree is balanced, the level of the adjacent neighbor(s) is 
    within one level of the specified node.

    Write a \c const_iterator to each adjacent neighbor to the output iterator.
  */
  template<typename _OutputIterator>
  void
  getBalancedNeighbors(const const_iterator node, _OutputIterator output)
    const;

  //! Get the adjacent neighbors in the specified direction in a balanced tree.
  /*!
    Since the tree is balanced, the level of the adjacent neighbor(s) is 
    within one level of the specified node.

    Write a \c const_iterator to each adjacent neighbor to the output iterator.
  */
  template<typename _OutputIterator>
  void
  getBalancedNeighbors(const const_iterator node, const int neighborDirection,
		       _OutputIterator output) const;

private:

  //! Partition the nodes.
  void
  partition(iterator* start, iterator* finish) {
#ifdef _OPENMP
    int a, b;
    numerical::partitionRange(size(), omp_get_num_threads(),
			      omp_get_thread_num(), &a, &b);
    *start = begin();
    std::advance(*start, a);
    *finish = *start;
    std::advance(*finish, b - a);
#else
    // Serial behavior.  The range that contains all nodes.
    *start = begin();
    *finish = end();
#endif
  }

  //! Partition the nodes.
  void
  partition(ads::Array<1, iterator>* delimiters) {
#ifdef DEBUG_geom_spatialIndexing_OrthtreeMap
#ifdef _OPENMP
    assert(! omp_in_parallel());
    assert(omp_get_max_threads() + 1 == delimiters->size());
#endif
#endif
    // Set the first and last delimiter.
    *delimiters->begin() = begin();
    *(delimiters->end() - 1) = end();

    const int threads = delimiters->size() - 1;
    // Provide this exit so we don't compute the size for the serial case.
    // (Computing the size has linear complexity.)
    if (threads == 1) {
      return;
    }

    const int numberOfNodes = size();
    for (int i = 0; i != threads - 1; ++i) {
      // The beginning of the partition.
      (*delimiters)[i + 1] = (*delimiters)[i];
      // Advance to the end of the partition.
      std::advance((*delimiters)[i + 1], 
		   numerical::partition(numberOfNodes, threads, i));
    }
  }

  //! Partition the nodes with the constraint that mergeable block do not cross partitions.
  void
  partitionMergeable(ads::Array<1, iterator>* delimiters) {
    // First partition.
    partition(delimiters);
    // Then move the delimiters forward to avoid breaking up mergeable blocks.
    for (int n = 1; n != delimiters->size() - 1; ++n) {
      iterator& i = (*delimiters)[n];
      while (i != end() && ! isLowerCorner(i->first)) {
	++i;
      }
    }
  }

  //! Return true if a node with the given key needs refinement in order to balance the tree.
  /*!
    A node needs refinement if it has an adjacent neighbor that is more than
    one level higher than itself.
  */
  bool
  needsRefinementToBalance(const Key& key) const;

  //! Find the node that matches the code.  If the node is not in the tree, find its ancestor.
  const_iterator
  findAncestor(const Key& key) const;

  //! Return true if the node has a higher level neighbor in the specified direction.
  bool
  hasHigherNeighbor(const const_iterator node, const int direction) const;

  //! Return true if the node has a higher level neighbor.
  bool
  hasHigherNeighbor(const const_iterator node) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  //! Return true if the data structures are equal.
  bool
  operator==(const OrthtreeMap& other) {
    return static_cast<const Base&>(*this) == static_cast<const Base&>(other) &&
      _lowerCorner == other._lowerCorner &&
      _extents == other._extents;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Operations on a single node.
  //@{
public:

  //! Call the function on the specified node.
  template<typename _Function>
  typename _Function::result_type
  evaluate(_Function f, const const_iterator i) const {
    return evaluate(&_Function::operator(), f, i);
  }

  //! Call the function on the specified node.
  template<typename _Function>
  typename _Function::result_type
  evaluate(_Function f, const iterator i) {
    return evaluate(&_Function::operator(), f, i);
  }

private:

  //
  // Evaluate a generator.
  //

  //! result_type () const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)() const,
	   _Function f, const_iterator i) const {
    return f();
  }

  //! result_type () const
  template<typename _Function>
  const typename _Function::result_type&
  evaluate(const typename _Function::result_type& (_Function::*/*dummy*/)() const,
	   _Function f, const_iterator i) const {
    return f();
  }

  //! result_type () const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)() const,
	   _Function f, iterator i) {
    return f();
  }

  //! const result_type& () const
  template<typename _Function>
  const typename _Function::result_type&
  evaluate(const typename _Function::result_type& (_Function::*/*dummy*/)() const,
	   _Function f, iterator i) {
    return f();
  }

  //
  // Evaluate using const_iterator.
  //

  //! result_type (const_iterator) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const_iterator) const,
	   _Function f, const_iterator i) const {
    return f(i);
  }

  //! result_type (const OrthtreeMap&, const_iterator) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const OrthtreeMap&, const_iterator) const,
	   _Function f, const_iterator i) const {
    return f(*this, i);
  }

  //
  // Evaluate using a pointer to the element.
  //

  //! result_type (Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (Element*) const,
	   _Function f, iterator i) {
    return f(&i->second);
  }

  //! result_type (const Key&, Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const Key&, Element*) const,
	   _Function f, iterator i) {
    return f(i->first, &i->second);
  }

  //! result_type (const OrthtreeMap&, Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const OrthtreeMap&, Element*) const,
	   _Function f, iterator i) {
    return f(*this, &i->second);
  }

  //! result_type (const OrthtreeMap&, const Key&, Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const OrthtreeMap&, const Key&, Element*) const,
	   _Function f, iterator i) {
    return f(*this, i->first, &i->second);
  }

  //
  // Evaluate using a const reference to the element.
  //

  //! result_type (const Element&) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const Element&) const,
	   _Function f, iterator i) {
    return f(i->second);
  }

  //! result_type (const Key&, Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const Key&, const Element&) const,
	   _Function f, iterator i) {
    return f(i->first, i->second);
  }

  //! result_type (const OrthtreeMap&, Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const OrthtreeMap&, const Element&) const,
	   _Function f, iterator i) {
    return f(*this, i->second);
  }

  //! result_type (const OrthtreeMap&, const Key&, Element*) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const OrthtreeMap&, const Key&, const Element&) const,
	   _Function f, iterator i) {
    return f(*this, i->first, i->second);
  }

  //
  // The node (through const_iterator) is the lower corner in a mergeable group.
  //

  //! result_type (const ads::FixedArray<NumberOfOrthants, const Element*>&) const
  template<typename _Function>
  typename _Function::result_type
  evaluate(typename _Function::result_type (_Function::*/*dummy*/)
	   (const ads::FixedArray<NumberOfOrthants, const Element*>&) const,
	   _Function f, const_iterator i) const {
    ads::FixedArray<NumberOfOrthants, const Element*> elements;
    for (int n = 0; n != NumberOfOrthants; ++n, ++i) {
      elements[n] = &i->second;
    }
    return f(elements);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Evaluate split functors.
  //@{
private:

  //
  // Dispatch.
  //

  //! Call the default split function on the node.
  void
  evaluateSplit(const Element& parent, const int n, const Key& key,
		Element* element) {
    evaluateSplit(_split, parent, n, key, element);
  }

  //! Call the specified split function on the node.
  template<typename _Function>
  void
  evaluateSplit(_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    evaluateSplit(&_Function::operator(), f, parent, n, key, element);
  }

  //
  // Evaluate for generators.
  //

  //! void operator()() const
  template<typename _Function>
  void
  evaluateSplit(void (_Function::*/*dummy*/)() const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
  }

  //! Element operator()() const
  template<typename _Function>
  void
  evaluateSplit(Element (_Function::*/*dummy*/)() const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    *element = f();
  }

  //! const Element& operator()() const
  template<typename _Function>
  void
  evaluateSplit(const Element& (_Function::*/*dummy*/)() const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    *element = f();
  }

  //! void operator()(Element*) const
  template<typename _Function>
  void
  evaluateSplit(void (_Function::*/*dummy*/)(Element*) const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    f(element);
  }

  //
  // Evaluate for functions of a const reference to the center point.
  //

  //! Element operator()(const Point&) const
  template<typename _Function>
  void
  evaluateSplit(Element (_Function::*/*dummy*/)(const Point&) const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    // Use the key to compute the center.
    Point center = getExtents(key);
    center *= 0.5;
    center += computeLowerCorner(key);
    // Call the split function.
    *element = f(center);
  }

  //! const Element& operator()(const Point&) const
  template<typename _Function>
  void
  evaluateSplit(const Element& (_Function::*/*dummy*/)(const Point&) const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    // Use the key to compute the center.
    Point center = getExtents(key);
    center *= 0.5;
    center += computeLowerCorner(key);
    // Call the split function.
    *element = f(center);
  }

  //! void operator()(const Point&, Element*) const
  template<typename _Function>
  void
  evaluateSplit(void (_Function::*/*dummy*/)(const Point&, Element*) const,
		_Function f, const Element& parent, const int n, const Key& key,
		Element* element) {
    // Use the key to compute the center.
    Point center = getExtents(key);
    center *= 0.5;
    center += computeLowerCorner(key);
    // Call the split function.
    f(center, element);
  }

  //
  // parent, orthant, lowerCorner, extents
  //

  //! Element operator()(const Element&, int, const Point&, const Point&) const
  template<typename _Function>
  void
  evaluateSplit(Element (_Function::*/*dummy*/)
		(const Element&, int, const Point&, const Point&) const,
		_Function f, const Element& parent, const int n,
		const Key& key,	Element* element) {
    // Call the split function.
    *element = f(parent, n, computeLowerCorner(key), getExtents(key));
  }

  //! void operator()(const Element&, int, const Point&, const Point&, Element*) const
  template<typename _Function>
  void
  evaluateSplit(void (_Function::*/*dummy*/)
		(const Element&, int, const Point&, const Point&, 
		 Element*) const,
		_Function f, const Element& parent, const int n,
		const Key& key,	Element* element) {
    // Call the split function.
    f(parent, n, computeLowerCorner(key), getExtents(key), element);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Evaluate merge functors.
  //@{
private:

  // CONTINUE Perhaps get rid of the parentKey argument.

  //
  // Dispatch.
  //

  //! Call the default merge function on the group of nodes.
  void
  evaluateMerge(const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    evaluateMerge(_merge, firstChild, parentKey, parentElement);
  }

  //! Call the specified merge function on the group of nodes.
  template<typename _Function>
  void
  evaluateMerge(_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    evaluateMerge(&_Function::operator(), f, firstChild, parentKey, 
		  parentElement);
  }

  //
  // Evaluate for generators.
  //

  //! void operator()() const
  template<typename _Function>
  void
  evaluateMerge(void (_Function::*/*dummy*/)() const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
  }

  //! Element operator()() const
  template<typename _Function>
  void
  evaluateMerge(Element (_Function::*/*dummy*/)() const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    *parentElement = f();
  }

  //! const Element& operator()() const
  template<typename _Function>
  void
  evaluateMerge(const Element& (_Function::*/*dummy*/)() const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    *parentElement = f();
  }

  //! void operator()(Element*) const
  template<typename _Function>
  void
  evaluateMerge(void (_Function::*/*dummy*/)(Element*) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    f(parentElement);
  }

  //
  // Evaluate for functions of OrthtreeMap and a const_iterator.
  //

  //! Element operator()(const OrthtreeMap&, const_iterator) const
  template<typename _Function>
  void
  evaluateMerge(Element (_Function::*/*dummy*/)
		(const OrthtreeMap&, const_iterator) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    *parentElement = f(*this, firstChild);
  }

  //! const Element& operator()(const OrthtreeMap&, const_iterator) const
  template<typename _Function>
  void
  evaluateMerge(const Element& (_Function::*/*dummy*/)
		(const OrthtreeMap&, const_iterator) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    *parentElement = f(*this, firstChild);
  }

  //! void operator()(const OrthtreeMap&, const_iterator, Element*) const
  template<typename _Function>
  void
  evaluateMerge(void (_Function::*/*dummy*/)
		(const OrthtreeMap&, const_iterator, Element*) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    f(*this, firstChild, parentElement);
  }

  //
  // Evaluate for functions of a const reference to the center point.
  //

  //! Element operator()(const Point&) const
  template<typename _Function>
  void
  evaluateMerge(Element (_Function::*/*dummy*/)(const Point&) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    // Use the first child (lower corner) to compute the center of the parent.
    Point center;
    computeLowerCorner(firstChild->first, &center);
    center += getExtents(firstChild->first);
    // Call the merge function.
    *parentElement = f(center);
  }

  //! const Element& operator()(const Point&) const
  template<typename _Function>
  void
  evaluateMerge(const Element& (_Function::*/*dummy*/)(const Point&) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    // Use the first child (lower corner) to compute the center of the parent.
    Point center;
    computeLowerCorner(firstChild->first, &center);
    center += getExtents(firstChild->first);
    // Call the merge function.
    *parentElement = f(center);
  }

  //! void operator()(const Point&, Element*) const
  template<typename _Function>
  void
  evaluateMerge(void (_Function::*/*dummy*/)(const Point&, Element*) const,
		_Function f, const iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    // Use the first child (lower corner) to compute the center of the parent.
    Point center;
    computeLowerCorner(firstChild->first, &center);
    center += getExtents(firstChild->first);
    // Call the merge function.
    f(center, parentElement);
  }

  //
  // Function of an array of elements.
  //

  //! Element operator()(const ads::FixedArray<NumberOfOrthants, const Element*>&) const
  template<typename _Function>
  void
  evaluateMerge(Element (_Function::*/*dummy*/)
		(const ads::FixedArray<NumberOfOrthants, const Element*>&) const,
		_Function f, iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    // Make an array of const pointers to the elements.
    ads::FixedArray<NumberOfOrthants, const Element*> elements;
    for (int n = 0; n != NumberOfOrthants; ++n, ++firstChild) {
      elements[n] = &firstChild->second;
    }
    *parentElement = f(elements);
  }

  //! const Element& operator()(const ads::FixedArray<NumberOfOrthants, const Element*>&) const
  template<typename _Function>
  void
  evaluateMerge(const Element& (_Function::*/*dummy*/)
		(const ads::FixedArray<NumberOfOrthants, const Element*>&) const,
		_Function f, iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    // Make an array of const pointers to the elements.
    ads::FixedArray<NumberOfOrthants, const Element*> elements;
    for (int n = 0; n != NumberOfOrthants; ++n, ++firstChild) {
      elements[n] = &firstChild->second;
    }
    *parentElement = f(elements);
  }

  //! void operator()(const ads::FixedArray<NumberOfOrthants, const Element*>&, Element*) const
  template<typename _Function>
  void
  evaluateMerge(void (_Function::*/*dummy*/)
		(const ads::FixedArray<NumberOfOrthants, const Element*>&,
		 Element*) const,
		_Function f, iterator firstChild, const Key& parentKey,
		Element* parentElement) {
    // Make an array of const pointers to the elements.
    ads::FixedArray<NumberOfOrthants, const Element*> elements;
    for (int n = 0; n != NumberOfOrthants; ++n, ++firstChild) {
      elements[n] = &firstChild->second;
    }
    f(elements, parentElement);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Apply functors.
  //@{
private:

  //
  // Dispatch.
  //

  //! Apply the default action to the node.
  void
  _apply(const iterator node) {
    _apply(_action, node);
  }

  //! Apply the specified action to the node.
  template<typename _Function>
  void
  _apply(_Function f, const iterator node) {
    _apply(&_Function::operator(), f, node);
  }

  //
  // Evaluate for generators.
  //

  //! void operator()() const
  template<typename _Function>
  void
  _apply(void (_Function::*/*dummy*/)() const,
	 _Function f, const iterator node) {
  }

  //! Element operator()() const
  template<typename _Function>
  void
  _apply(Element (_Function::*/*dummy*/)() const,
	 _Function f, const iterator node) {
    node->second = f();
  }

  //! const Element& operator()() const
  template<typename _Function>
  void
  _apply(const Element& (_Function::*/*dummy*/)() const,
	 _Function f, const iterator node) {
    node->second = f();
  }

  //! void operator()(Element*) const
  template<typename _Function>
  void
  _apply(void (_Function::*/*dummy*/)(Element*) const,
	 _Function f, const iterator node) {
    f(&node->second);
  }

  //
  // Apply functions of a const reference to the center point.
  //

  //! Element operator()(const Point&) const
  template<typename _Function>
  void
  _apply(Element (_Function::*/*dummy*/)(const Point&) const,
	 _Function f, const iterator node) {
    // Use the key to compute the center.
    Point center = getExtents(node->first);
    center *= 0.5;
    center += computeLowerCorner(node->first);
    // Call the function.
    node->second = f(center);
  }

  //! const Element& operator()(const Point&) const
  template<typename _Function>
  void
  _apply(const Element& (_Function::*/*dummy*/)(const Point&) const,
	 _Function f, const iterator node) {
    // Use the key to compute the center.
    Point center = getExtents(node->first);
    center *= 0.5;
    center += computeLowerCorner(node->first);
    // Call the function.
    node->second = f(center);
  }

  //! void operator()(const Point&, Element*) const
  template<typename _Function>
  void
  _apply(void (_Function::*/*dummy*/)(const Point&, Element*) const,
	 _Function f, const iterator node) {
    // Use the key to compute the center.
    Point center = getExtents(node->first);
    center *= 0.5;
    center += computeLowerCorner(node->first);
    // Call the function.
    f(center, &node->second);
  }

  //
  // lowerCorner, extents
  //

  //! Element operator()(const Point&, const Point&) const
  template<typename _Function>
  void
  _apply(Element (_Function::*/*dummy*/)(const Point&, const Point&) const,
	 _Function f, const iterator node) {
    // Call the function.
    node->second = f(computeLowerCorner(node->first), getExtents(node->first));
  }

  //! void operator()(const Point&, const Point&, Element*) const
  template<typename _Function>
  void
  _apply(void (_Function::*/*dummy*/)
	 (const Point&, const Point&, Element*) const,
	 _Function f, const iterator node) {
    // Call the function.
    f(computeLowerCorner(node->first), getExtents(node->first), &node->second);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Insert a value type.
  iterator
  insert(const value_type& x);

  // This produces a warning about using element uninitialized.
  //! Insert a key-element pair.
  iterator
  insert(const Key& key, const Element& element = Element()) {
    return insert(value_type(key, element));
  }

  //! Insert a range of values.
  template<typename InputIterator>
  void
  insert(InputIterator begin, InputIterator end) {
    while (begin != end) {
      insert(*begin++);
    }
  }

  //! Erase a value.
  using Base::erase;

  //! Split a node.
  void
  split(const iterator parent) {
    split(parent, ads::constructTrivialOutputIterator());
  }

  //! Split a node.  Get the children.
  template<typename _OutputIterator>
  void
  split(iterator parent, _OutputIterator children) {
    split(_split, parent, children);
  }

  //! Split a node with the specified functor.  Get the children.
  template<typename _Function, typename _OutputIterator>
  void
  split(_Function splitFunctor, iterator parent, _OutputIterator children);

  //! Merge the nodes given the first child.  Return the merged node.
  /*!
    \pre All of the children must be present.
  */
  iterator
  merge(iterator firstChild) {
    return merge(_merge, firstChild);
  }

  //! Merge the nodes given the first child and using the specified merge function.  Return the merged node.
  /*!
    \pre All of the children must be present.
  */
  template<typename _Function>
  iterator
  merge(_Function mergeFunctor, iterator firstChild);

  //! Get the adjacent neighbors which have lower levels.
  template<typename _OutputIterator>
  void
  getLowerNeighbors(const iterator node, _OutputIterator i);

private:
  
  //! Find the node that matches the code.  If the node is not in the tree, find its ancestor.
  iterator
  findAncestor(const Key& key);

  //! Get the mergeable groups of 2^Dimension nodes.
  template<typename _OutputIterator>
  void
  getMergeableGroups(_OutputIterator lowerCornerNodes) {
    getMergeableGroups(lowerCornerNodes, begin(), end());
  }

  //! Get the mergeable groups of 2^Dimension nodes from the range of nodes.
  template<typename _OutputIterator>
  void
  getMergeableGroups(_OutputIterator lowerCornerNodes, iterator start,
		     iterator finish);

  //! Get the mergeable groups of 2^Dimension nodes whose merging would result in a balanced tree.
  template<typename _OutputIterator>
  void
  getMergeableGroupsBalanced(_OutputIterator lowerCornerNodes) {
    getMergeableGroupsBalanced(lowerCornerNodes, begin(), end());
  }

  //! Get the mergeable groups of 2^Dimension nodes from the range of nodes whose merging would result in a balanced tree.
  template<typename _OutputIterator>
  void
  getMergeableGroupsBalanced(_OutputIterator lowerCornerNodes, iterator start,
			     iterator finish);

  //@}
  //--------------------------------------------------------------------------
  //! \name Search.
  //@{
public:

  //! Find a value.
  using Base::find;

  //! Count the occurences of the value.
  /*!
    We need to overide the base class function because different keys
    may have the same code.  We need to check the level as well.
  */
  bool
  count(const Key& key) const {
    const_iterator i = find(key);
    return i != end() && i->first == key;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print the keys and the elements.
  void
  print(std::ostream& out) const;

  //@}
};


//! Compute the sum of the elements.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
_Element
accumulate
(const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x,
 _Element initial) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> 
    OrthtreeMap;
  
  for (typename OrthtreeMap::const_iterator i = x.begin(); i != x.end(); ++i) {
    initial += i->second;
  }
  return initial;
}

//! Accumulate the elements using a specified function.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key,
	 typename _T, typename _BinaryFunction>
inline
_T
accumulate
(const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x,
 _T initial, _BinaryFunction f) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> 
    OrthtreeMap;
 
  for (typename OrthtreeMap::const_iterator i = x.begin(); i != x.end(); ++i) {
    initial = f(initial, i->second);
  }
  return initial;
}

//! Sum the results of a function applied to each node.
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key,
	 typename _Function>
inline
typename _Function::result_type
accumulateFunction
(const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x,
 typename _Function::result_type initial, _Function f) {
  typedef OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
    _Split, _Merge, _Refine, _Coarsen, _Action, _Key> 
    OrthtreeMap;
 
  for (typename OrthtreeMap::const_iterator i = x.begin(); i != x.end(); ++i) {
    initial += x.evaluate(f, i);
  }
  return initial;
}

//! Print the orthtree.
/*!
  \relates OrthtreeMap
*/
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
inline
std::ostream&
operator<<
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x) {
  x.print(out);
  return out;
}


//! Print the bounding boxes for the leaves in VTK format.
/*!
  \relates OrthtreeMap
*/
template<int _Dimension, int _MaximumLevel, typename _Element,
	 bool _AutomaticBalancing, class _Split, class _Merge,
	 class _Refine, class _Coarsen, class _Action,
	 template<int, int> class _Key>
void
printVtkUnstructuredGrid
(std::ostream& out, 
 const OrthtreeMap<_Dimension, _MaximumLevel, _Element, _AutomaticBalancing,
 _Split, _Merge, _Refine, _Coarsen, _Action, _Key>& x);


END_NAMESPACE_GEOM

#define __geom_spatialIndexing_OrthtreeMap_ipp__
#include "OrthtreeMap.ipp"
#undef __geom_spatialIndexing_OrthtreeMap_ipp__

#endif
