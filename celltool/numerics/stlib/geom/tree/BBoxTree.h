// -*- C++ -*-

/*! 
  \file BBoxTree.h
  \brief A class for a bounding box tree in N-D.
*/

#if !defined(__BBoxTree_h__)
#define __BBoxTree_h__

#include "../kernel/BBox.h"
#include "../kernel/distance.h"

#include "../../ads/functor/compose.h"
#include "../../ads/functor/composite_compare.h"
#include "../../ads/functor/Dereference.h"

#include "../../numerical/random/uniform/ContinuousUniformGenerator.h"
#include "../../numerical/random/uniform/DiscreteUniformGeneratorMc32.h"

#include <vector>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_BBoxTree)
#define DEBUG_BBoxTree
#endif

BEGIN_NAMESPACE_GEOM

//
//---------------------------BBoxTreeTypes----------------------------------
//

//! Define types for a BBoxTree.
/*!
  \param N is the space dimension.
  \param T is the number type.
 */
template<int N, typename T>
class BBoxTreeTypes {
  //
  // Public types.
  //
      
public:

  //! The number type.
  typedef T Number;
  //! The Cartesian point type.
  typedef ads::FixedArray<N,T> Point;
  //! A bounding box.
  typedef BBox<N,Number> BBox;
  //! The size type.
  typedef int SizeType;

  //! An index container.
  typedef std::vector<int> IndexContainer;
  //! An iterator in an index container.
  typedef typename IndexContainer::iterator IndexIterator;
  //! A const iterator in an index container.
  typedef typename IndexContainer::const_iterator IndexConstIterator;
};


//
//---------------------------BBoxTreeNode----------------------------------
//

// Forward declaration.
template<int N, typename T>
class BBoxTreeLeaf;

//! Abstract base class for nodes in a BBoxTree.
template<int N, typename T>
class BBoxTreeNode {
  //
  // Private types.
  //

private:

  typedef BBoxTreeTypes<N,T> Types;
  typedef BBoxTreeLeaf<N,T> Leaf;


  //
  // Public types.
  //

public:
    
  //! The number type.
  typedef typename Types::Number Number;
  //! The Cartesian point type.
  typedef typename Types::Point Point;
  //! A bounding box.
  typedef typename Types::BBox BBox;
  //! The size type.
  typedef typename Types::SizeType SizeType;

  //--------------------------------------------------------------------------
  //! \name Destructor.
  //@{

  //! Virtual destructor does nothing.
  virtual
  ~BBoxTreeNode()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Queries.
  //@{

  //! Get the leaves containing bounding boxes that might contain the point.
  virtual
  void
  computePointQuery(std::vector<const Leaf*>& leaves, 
		    const Point& x) const = 0;

  //! Get the leaves containing bounding boxes that might overlap the window.
  virtual
  void
  computeWindowQuery(std::vector<const Leaf*>& leaves, 
		const BBox& window) const = 0;

  //! Get the indices of the bounding boxes that might contain objects of minimum distance.
  virtual
  void
  computeMinimumDistanceQuery(std::vector<const Leaf*>& leaves, 
			      const Point& x, Number* upperBound) const 
    = 0;
  
  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the domain of this node.
  virtual
  const BBox&
  getDomain() const = 0;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Compute the domain for this node.
  virtual
  void
  computeDomain(const std::vector<BBox>& boxes) = 0;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Print the node information.
  virtual 
  void 
  printAscii(std::ostream& out) const = 0;

  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{

  //! Return the memory usage of this node and its children.
  virtual 
  SizeType
  getMemoryUsage() const = 0;

  //@}
  //--------------------------------------------------------------------------
  //! \name Validity check.
  //@{

  //! Check the validity of this node.
  virtual
  void 
  checkValidity(const std::vector<BBox>& boxes) const = 0;

  //@}
};


//! Write to a file stream.
/*! \relates BBoxTreeNode */
template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const BBoxTreeNode<N,T>& node) {
  node.printAscii(out);
  return out;
}



//
//---------------------------BBoxTreeLeaf----------------------------------
//

//! Class for a leaf in a BBoxTree.
template<int N, typename T>
class BBoxTreeLeaf : 
  public BBoxTreeNode<N,T> {
  //
  // Private types.
  //

private:
  
  typedef BBoxTreeTypes<N,T> Types;
  typedef BBoxTreeLeaf Leaf;
  //! An index container.
  typedef typename Types::IndexContainer IndexContainer;
  //! An iterator in an index container.
  typedef typename Types::IndexIterator IndexIterator;
  
  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename Types::Number Number;
  //! The Cartesian point type.
  typedef typename Types::Point Point;
  //! A bounding box.
  typedef typename Types::BBox BBox;
  //! The size type.
  typedef typename Types::SizeType SizeType;

  //! A const iterator in an index container.
  typedef typename Types::IndexConstIterator IndexConstIterator;

  //
  // Member data
  //
    
private:

  //! The domain that contains all the bounding boxes in this leaf.
  BBox _domain;
  //! The bounding box indices.
  IndexContainer _indices;

  //
  // Not implemented
  //

private:

  //! Copy constructor not implemented
  BBoxTreeLeaf(const BBoxTreeLeaf&);

  //! Assignment operator not implemented
  BBoxTreeLeaf& 
  operator=(const BBoxTreeLeaf&);


public:
    
  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Construct from a range of iterators to objects.
  /*!
    \param base is the beggining of storage in the container of objects.
    \param begin is the beggining of the range of objects whose indices
    will be stored in this leaf.
    \param end is the end of the range of objects whose indices
    will be stored in this leaf.
    
    This function subtracts the base iterator to make indices.
  */
  template<typename ObjectIter, typename ObjectIterIter>
  BBoxTreeLeaf(ObjectIter base, ObjectIterIter begin, ObjectIterIter end);
  
  //! Trivual destructor.
  virtual 
  ~BBoxTreeLeaf()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the domain that contains all the bounding boxes in this leaf.
  const BBox& 
  getDomain() const {
    return _domain;
  }

  //! Return the beginning of the bounding box indices.
  IndexConstIterator
  getIndicesBeginning() const {
    return _indices.begin();
  }
  
  //! Return the end of the bounding box indices.
  IndexConstIterator
  getIndicesEnd() const {
    return _indices.end();
  }
  
  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Compute the domain for this leaf.
  void
  computeDomain(const std::vector<BBox>& boxes);

  //@}
  //--------------------------------------------------------------------------
  //! \name Queries.
  //@{

  //! Get the leaves containing bounding boxes that might contain the point.
  void
  computePointQuery(std::vector<const Leaf*>& leaves, const Point& x) const {
    if (_domain.isIn(x)) {
      leaves.push_back(this);
    }
  }

  //! Get the leaves containing bounding boxes that might overlap the window.
  void
  computeWindowQuery(std::vector<const Leaf*>& leaves, 
		     const BBox& window) const {
    if (doOverlap(window, _domain)) {
      leaves.push_back(this);
    }
  }

  //! Get the indices of the bounding boxes that might contain objects of minimum distance.
  void
  computeMinimumDistanceQuery(std::vector<const Leaf*>& leaves, 
			      const Point& x, Number* upperBound) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  // Print the domain and indices.
  void 
  printAscii(std::ostream& out) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{

  //! Return the memory usage of this leaf.
  SizeType
  getMemoryUsage() const {
    return int(sizeof(BBoxTreeLeaf) + _indices.size() * sizeof(int));
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Validity check.
  //@{

  //! Check the validity of this leaf.
  /*!
    There should be a non-zero number of indices.  The corresponding bounding
    boxes should be in the domain of this leaf.
  */
  void 
  checkValidity(const std::vector<BBox>& boxes) const;

  //@}
};




//
//-------------------------BBoxTreeBranch------------------------------
//

//! Class for an internal node in a BBoxTree.
template<int N, typename T>
class BBoxTreeBranch :
  public BBoxTreeNode<N,T> {
  //
  // Private types.
  //

private:
  
  typedef BBoxTreeTypes<N,T> Types;
  typedef BBoxTreeNode<N,T> Node;
  typedef BBoxTreeLeaf<N,T> Leaf;
  
  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename Types::Number Number;
  //! The Cartesian point type.
  typedef typename Types::Point Point;
  //! A bounding box.
  typedef typename Types::BBox BBox;
  //! The size type.
  typedef typename Types::SizeType SizeType;

  //
  // Member data
  //

private:

  //! The domain that contains all the bounding boxes in this branch.
  BBox _domain;
  //! The left sub-tree.
  Node* _left;
  //! The right sub-tree.
  Node* _right;

  //
  // Not implemented
  //
    
private:

  // Default constructor not implemented.
  BBoxTreeBranch();

  // Copy constructor not implemented
  BBoxTreeBranch(const BBoxTreeBranch&);

  // Assignment operator not implemented
  BBoxTreeBranch& 
  operator=(const BBoxTreeBranch&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructor and destructor.
  //@{

  //! Construct from sorted midpoints of bounding boxes.
  BBoxTreeBranch(const Point* base,
		 const ads::FixedArray<N, std::vector<const Point*> >& sorted,
		 SizeType leafSize);

  //! Destructor.  Delete this and the left and right branches.
  virtual 
  ~BBoxTreeBranch() {
    delete _left;
    delete _right;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Return the domain that contains all the bounding boxes in this branch.
  const BBox& 
  getDomain() const {
    return _domain;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Compute the domain for this branch.
  void
  computeDomain(const std::vector<BBox>& boxes);

  //@}
  //--------------------------------------------------------------------------
  //! \name Queries.
  //@{

  //! Get the leaves containing bounding boxes that might contain the point.
  void
  computePointQuery(std::vector<const Leaf*>& leaves, const Point& x) const;

  //! Get the leaves containing bounding boxes that might overlap the window.
  void
  computeWindowQuery(std::vector<const Leaf*>& leaves, 
		     const BBox& window) const;

  //! Get the indices of the bounding boxes that might contain objects of minimum distance.
  void
  computeMinimumDistanceQuery(std::vector<const Leaf*>& leaves, 
			      const Point& x, Number* upperBound) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{

  //! Return the memory usage of this branch and its children.
  SizeType
  getMemoryUsage() const {
    return (sizeof(BBoxTreeBranch) + _left->getMemoryUsage() 
	    + _right->getMemoryUsage());
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Validity check.
  //@{

  //! Check for validity.
  void 
  checkValidity(const std::vector<BBox>& boxes) const;
    
  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  // Print the domain and the leaves.
  void 
  printAscii(std::ostream& out) const;

  //@}
};




//-------------------------------------------------------------------------
//---------------------------BBoxTree--------------------------------------
//-------------------------------------------------------------------------

//! A bounding box tree in N-D.
/*!
  A binary tree in N-D holding bounding boxes.
*/
template<int N, typename T = double>
class BBoxTree {
  //
  // Private types.
  //

private:

  typedef BBoxTreeTypes<N,T> Types;

  //
  // Public types.
  //

public:

  //! The number type.
  typedef typename Types::Number Number;
  //! The Cartesian point type.
  typedef typename Types::Point Point;
  //! A bounding box.
  typedef typename Types::BBox BBox;
  //! The size type.
  typedef typename Types::SizeType SizeType;


  //
  // Private types.
  //

private:

  typedef BBoxTreeNode<N,T> Node;
  typedef BBoxTreeBranch<N,T> Branch;
  typedef BBoxTreeLeaf<N,T> Leaf;
  //! A const iterator in an index container.
  typedef typename Types::IndexConstIterator IndexConstIterator;

  //
  // Member data
  //

private:

  //! The root of the tree.
  Node* _root;

  //! The set of bounding boxes for the objects.
  std::vector<BBox> _boxes;

  //! A leaf container used in the queries.
  mutable std::vector<const Leaf*> _leaves;

private:

  //
  // Not implemented
  //

private:

  //! Copy constructor not implemented
  BBoxTree(const BBoxTree&);

  //! Assignment operator not implemented
  BBoxTree& 
  operator=(const BBoxTree&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Default constructor.  Empty tree.
  BBoxTree() :
    _root(0),
    _boxes(),
    _leaves()
  {}

  //! Construct from a range of bounding boxes.
  /*!
    \param begin is the beginning of the range of bounding boxes.
    \param end is the end of the range of bounding boxes.
    \param leafSize is the maximum number of objects that are stored in a
    leaf.  The default value is 8.
  */
  template<class BBoxInputIter>
  BBoxTree(BBoxInputIter begin, BBoxInputIter end, 
	   const SizeType leafSize = 8);

  //! Build from a range of bounding boxes.
  /*!
    \param begin is the beginning of the range of bounding boxes.
    \param end is the end of the range of bounding boxes.
    \param leafSize is the maximum number of objects that are stored in a
    leaf.  The default value is 8.
  */
  template<class BBoxInputIter>
  void
  build(BBoxInputIter begin, BBoxInputIter end, 
	const SizeType leafSize = 8);

  //! Destructor.  Delete the tree.
  ~BBoxTree() {
    if (_root) {
      delete _root;
    }
  }
  
  //! Delete all the data structures.
  void
  destroy();
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the number of objects in the tree.
  SizeType 
  getSize() const { 
    return SizeType(_boxes.size());
  }

  //! Return true if the tree is empty.
  bool 
  isEmpty() const { 
    return _boxes.empty();
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Queries.
  // @{

  //! Get the indices of the bounding boxes that contain the point.
  template<typename IntegerOutputIter>
  void
  computePointQuery(IntegerOutputIter iter, const Point& x) const;

  //! Get the indices of the bounding boxes that overlap the window.
  template<typename IntegerOutputIter>
  void
  computeWindowQuery(IntegerOutputIter iter, const BBox& window) const;

  //! Get the indices of the bounding boxes that might contain objects of minimum distance.
  template<typename IntegerOutputIter>
  void
  computeMinimumDistanceQuery(IntegerOutputIter iter, const Point& x) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the records.
  void 
  printAscii(std::ostream& out) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  // @{

  //! Return the memory usage of the tree.
  SizeType
  getMemoryUsage() const {
    if (_root) {
      return (sizeof(BBoxTree) + _root->getMemoryUsage());
    }
    return sizeof(BBoxTree);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Validity check.
  // @{

  //! Check the validity of the kd-tree.
  void 
  checkValidity() const {
    if (_root) {
      _root->checkValidity(_boxes);
    }
  }

  // @}

  //
  // Private member functions.
  //

private:

  //! Build the tree data structure.
  void
  build(SizeType leafSize);
};


//! Write to a file stream.
/*! \relates BBoxTree */
template<int N, typename T>
inline
std::ostream& 
operator<<(std::ostream& out, const BBoxTree<N,T>& x) {
  x.printAscii(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_BBoxTree_ipp__
#include "BBoxTree.ipp"
#undef __geom_BBoxTree_ipp__

#endif
