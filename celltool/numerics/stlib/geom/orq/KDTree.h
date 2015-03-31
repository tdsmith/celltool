// -*- C++ -*-

/*! 
  \file geom/orq/KDTree.h
  \brief A class for a kd-tree in N-D.
*/

#if !defined(__KDTree_h__)
#define __KDTree_h__

#include "ORQ.h"

#include "../../ads/functor/composite_compare.h"
#include "../../ads/functor/Dereference.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_KDTree)
#define DEBUG_KDTree
#endif

BEGIN_NAMESPACE_GEOM

//
//---------------------------KDTreeNode----------------------------------
//

//! Abstract base class for nodes in a KDTree.
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
class KDTreeNode {
public:
    
  //
  // Public types.
  //

  //! A pointer to the record type.
  typedef _Record Record;
  //! The multi-key type.
  typedef _MultiKey MultiKey;
  //! The key type.
  typedef _Key Key;
  //! The multi-key accessor functor.
  typedef _MultiKeyAccessor MultiKeyAccessor;
  //! The output iterator for records.
  typedef _RecordOutputIterator RecordOutputIterator;

  //! The size type.
  typedef int SizeType;
  //! Bounding box.
  typedef BBox<N,Key> BBox;

private:

  //
  // Static member data.
  //

  //! The multi-key accessor.
  static MultiKeyAccessor _multiKeyAccessor;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Virtual destructor.  We need this because we have other virtual functions.
  virtual
  ~KDTreeNode()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Multikey access.
  //@{

  //! Get the multi-key of a record.
  const MultiKey&
  getMultiKey(const Record record) const {
    return _multiKeyAccessor(record);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Window queries.
  //@{

  //! Get the records in the node and children.  Return the # of records.
  virtual 
  SizeType
  report(RecordOutputIterator iter) const = 0;

  //! Get the records in the window.  Return the # of records inside.
  virtual 
  SizeType
  computeWindowQuery(RecordOutputIterator iter, const BBox& window) const = 0;

  //! Get the records in the window.  Return the # of records inside.
  virtual 
  SizeType
  computeWindowQuery(RecordOutputIterator iter, BBox* domain, 
		     const BBox& window) const = 0;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Print the records.
  virtual 
  void 
  put(std::ostream& out) const = 0;

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
  //! \name Validity.
  //@{

  // Check the validity of the node.
  virtual 
  bool
  isValid(const BBox& window) const = 0;

  //@}
};

//! Static member variable.
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
_MultiKeyAccessor KDTreeNode<N, _Record, _MultiKey, _Key, _MultiKeyAccessor, 
			     _RecordOutputIterator>::_multiKeyAccessor;

//! Write to a file stream.
/*! \relates KDTreeNode */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
inline
std::ostream& 
operator<<(std::ostream& out, 
	   const KDTreeNode<N, _Record, _MultiKey, _Key, _MultiKeyAccessor, 
	   _RecordOutputIterator>& node) {
  node.put(out);
  return out;
}


//
//---------------------------KDTreeLeaf----------------------------------
//

//! Class for a leaf in a KDTree.
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
class KDTreeLeaf : 
  public KDTreeNode<N, _Record, _MultiKey, _Key, _MultiKeyAccessor, 
		    _RecordOutputIterator> {
private:

  typedef KDTreeNode<N, _Record, _MultiKey, _Key, _MultiKeyAccessor, 
		     _RecordOutputIterator> Node;

public:

  //
  // Public types.
  //

  //! A pointer to the record type.
  typedef _Record Record;
  //! The multi-key type.
  typedef _MultiKey MultiKey;
  //! The key type.
  typedef _Key Key;
  //! The multi-key accessor functor.
  typedef _MultiKeyAccessor MultiKeyAccessor;
  //! The output iterator for records.
  typedef _RecordOutputIterator RecordOutputIterator;

  //! The size type.
  typedef typename Node::SizeType SizeType;
  //! A Cartesian point.
  typedef ads::FixedArray<N,Key> Point;
  //! Bounding box.
  typedef BBox<N,Key> BBox;

private:

  //
  // Private types.
  //

  typedef std::vector<Record> Container;
  typedef typename Container::iterator Iterator;
  typedef typename Container::const_iterator ConstIterator;

private:

  //
  // Member data
  //
    
  //! The records
  Container _records;

  //
  // Not implemented
  //

  //! Copy constructor not implemented
  KDTreeLeaf(const KDTreeLeaf&);

  //! Assignment operator not implemented
  KDTreeLeaf& 
  operator=(const KDTreeLeaf&);


public:
    
  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Construct from a vector of records.
  KDTreeLeaf(const std::vector<Record>& records) :
    _records(records)
  {}

  //! Trivual destructor.
  virtual 
  ~KDTreeLeaf()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Window queries.
  //@{

  // Get the records.  Return the # of records.
  SizeType
  report(RecordOutputIterator iter) const {
    for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
      *(iter++) = (*i);
    }
    return _records.size();
  }

  // Get the records in the window.  Return the # of records inside.
  SizeType 
  computeWindowQuery(RecordOutputIterator iter, const BBox& window) const;

  // Get the records in the window.  Return the # of records inside.
  SizeType
  computeWindowQuery(RecordOutputIterator iter, BBox* domain, 
		     const BBox& window) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Multikey access.
  //@{

  //! Get the multi-key of a record.
  using Node::getMultiKey;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  // Print the records.
  void 
  put(std::ostream& out) const {
    for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
      out << **i << '\n';
    }
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{

  //! Return the memory usage of this leaf.
  SizeType
  getMemoryUsage() const {
    return (sizeof(KDTreeLeaf) + _records.size() * sizeof(Record));
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Validity check.
  //@{

  bool
  isValid(const BBox& domain) const {
    for (ConstIterator i = _records.begin(); i != _records.end(); ++i) {
      if (! domain.isIn(getMultiKey(*i))) {
	return false;
      }
    }
    return true;
  }

  //@}
};




//
//-------------------------KDTreeBranch------------------------------
//


//! Class for an internal node in a KDTree.
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor,
	 typename _RecordOutputIterator>
class KDTreeBranch : 
  public KDTreeNode<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		     _RecordOutputIterator> {
private:

  typedef KDTreeNode<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		     _RecordOutputIterator> Node;
  typedef KDTreeLeaf<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		     _RecordOutputIterator> Leaf;

public:

  //
  // Public types.
  //

  //! A pointer to the record type.
  typedef _Record Record;
  //! The multi-key type.
  typedef _MultiKey MultiKey;
  //! The key type.
  typedef _Key Key;
  //! The multi-key accessor functor.
  typedef _MultiKeyAccessor MultiKeyAccessor;
  //! The output iterator for records.
  typedef _RecordOutputIterator RecordOutputIterator;

  //! The size type.
  typedef typename Node::SizeType SizeType;
  //! A Cartesian point.
  typedef ads::FixedArray<N,Key> Point;
  //! Bounding box.
  typedef geom::BBox<N,Key> BBox;

private:

  //
  // Not implemented
  //
    
  // Default constructor not implemented.
  KDTreeBranch();

  // Copy constructor not implemented
  KDTreeBranch(const KDTreeBranch&);

  // Assignment operator not implemented
  KDTreeBranch& 
  operator=(const KDTreeBranch&);

protected:

  //
  // Member data
  //

  //! The left sub-tree.
  Node* _left;

  //! The right sub-tree.
  Node* _right;

  //! The splitting dimension
  int _splitDimension;

  //! The splitting value.
  Key _splitValue;
      
public:

  //--------------------------------------------------------------------------
  //! \name Constructor and destructor.
  //@{

  //! Construct from sorted records.
  KDTreeBranch(const ads::FixedArray<N, std::vector<Record> >& sorted,
	       const SizeType leafSize);

  //! Destructor.  Delete this and the left and right branches.
  virtual 
  ~KDTreeBranch() {
    delete _left;
    delete _right;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Window queries.
  //@{

  // Get the records.  Return the # of records.
  SizeType
  report(RecordOutputIterator iter) const { 
    return _left->report(iter) + _right->report(iter);
  }

  // Get the records in the window.  Return the # of records inside.
  SizeType
  computeWindowQuery(RecordOutputIterator iter, const BBox& window) const;

  // Get the records in the window.  Return the # of records inside.
  SizeType
  computeWindowQuery(RecordOutputIterator iter, BBox* domain, 
		     const BBox& window) const;

  //@}
  //--------------------------------------------------------------------------
  //! \name Multikey access.
  //@{

  //! Get the multi-key of a record.
  using Node::getMultiKey;

  //@}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  //@{

  //! Return the memory usage of this branch and its children.
  SizeType
  getMemoryUsage() const {
    return (sizeof(KDTreeBranch) + _left->getMemoryUsage() 
	     + _right->getMemoryUsage());
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Validity check.
  //@{

  // Check for validity.
  bool
  isValid(const BBox& window) const;
    
  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  // Print the records.
  void 
  put(std::ostream& out) const {
    _left->put(out);
    _right->put(out);
  }

  //@}
};




//
//---------------------------KDTree--------------------------------------
//

// CONTINUE: Maybe _root should never be null.  It should be an empty leaf 
// instead.  This would simplify some of the code.
//! A kd-tree in N-D.
/*!
  An kd-tree in N-D.

  RecordOutputIterator is the typename that you will use in the
  computeWindowQuery() function.  You have to specify it as a template
  parameter because of the way KDTree is implemented.  C++ does
  not allow templated virtual functions.
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record>,
	 typename _RecordOutputIterator = _Record*>
class KDTree :
  public ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> {
private:

  //
  // Private types.
  //

  typedef ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> Base;

public:

  //
  // Public types.
  //

  //! A pointer to the record type.
  typedef _Record Record;
  //! The multi-key type.
  typedef _MultiKey MultiKey;
  //! The key type.
  typedef _Key Key;
  //! The record output iterator.
  typedef _RecordOutputIterator RecordOutputIterator;

  //! The size type.
  typedef typename Base::SizeType SizeType;
  //! A Cartesian point.
  typedef typename Base::Point Point;
  //! Bounding box.
  typedef typename Base::BBox BBox;

  //! The node type.
  typedef KDTreeNode<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		     _RecordOutputIterator> Node;

private:

  //
  // Functors.
  //

  //! Less than composite comparison for records.
  class LessThanComposite :
    public std::binary_function<Record, Record, bool> {
  private:

    int _n;
    _MultiKeyAccessor _f;

  public:

    //! Default constructor.  The starting coordinate has an invalid value.
    LessThanComposite() :
      _n(-1)
    {}
  
    //! Set the starting coordinate.
    void
    set(const int n) {
      _n = n;
    }

    //! Less than composite comparison, starting with a specified coordinate.
    bool
    operator()(const Record x, const Record y) {
      return ads::less_composite_fcn<N>(_n, _f(x), _f(y));
    }
  };

  //
  // Private types.
  //

  typedef KDTreeBranch<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		       _RecordOutputIterator> Branch;
  typedef KDTreeLeaf<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
		     _RecordOutputIterator> Leaf;

  //
  // Member data
  //

  //! The root of the tree.
  Node* _root;

  //! The domain of the kd-tree.
  BBox _domain;

private:

  //
  // Not implemented
  //

  //! Default constructor not implemented.
  KDTree();

  //! Copy constructor not implemented
  KDTree(const KDTree&);

  //! Assignment operator not implemented
  KDTree& 
  operator=(const KDTree&);

public:

  //--------------------------------------------------------------------------
  //! \name Constructors and destructor.
  //@{

  //! Construct from a range of records.
  /*!
    \param first is the beginning of the range of records.
    \param last is the end of the range of records.
    \param leafSize is the maximum number of records that are stored in a
    leaf.  Choose this value to be about the number records that you
    expect a window query to return.  The default value is 8.
  */
  template<class InputIterator>
  KDTree(InputIterator first, InputIterator last, const SizeType leafSize = 8);

  //! Destructor.  Delete the tree.
  ~KDTree() {
    delete _root;
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //
  // Inherited.
  //

  //! Return the number of records.
  using Base::getSize;

  //! Return true if the grid is empty.
  using Base::isEmpty;

  //
  // New.
  //

  //! Return the domain containing the records.
  const BBox& 
  getDomain() const { 
    return _domain; 
  }
  
  // @}
  //--------------------------------------------------------------------------
  //! \name Window queries.
  // @{

  //! Get the records in the window.  Return the # of records inside.
  SizeType
  computeWindowQuery(RecordOutputIterator iter, const BBox& window) const {
    return _root->computeWindowQuery(iter, window);
  }

  //! Get the records in the window.  Return the # of records inside.
  /*!
    Use this version of window query only if the number of records 
    returned is much larger than the leaf size.

    This implementation of KDTree does not store the domain
    information at the branches and leaves.  This choice decreases
    the memory usage but incurs the computational cost of having to
    compute the domain as the window query progresses if you use this
    function.
  */
  SizeType
  computeWindowQueryUsingDomain(RecordOutputIterator iter, const BBox& window)
    const {
    BBox domain(_domain);
    return _root->computeWindowQuery(iter, &domain, window);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the records.
  void 
  put(std::ostream& out) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Memory usage.
  // @{

  //! Return the memory usage of the tree.
  SizeType
  getMemoryUsage() const {
    return (sizeof(KDTree) + _root->getMemoryUsage());
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Validity.
  // @{

  //! Check the validity of the kd-tree.
  bool
  isValid() const {
    return _root->isValid(_domain);
  }

  // @}

protected:

  //! Increment the number of records.
  using Base::incrementSize;

  //! Get the multi-key of a record.
  using Base::getMultiKey;

  //! Set the number of records.
  using Base::setSize;
};


//! Write to a file stream.
/*! \relates KDTree */
template<int N, typename _Record, typename _MultiKey, typename _Key,
	 typename _MultiKeyAccessor, typename _RecordOutputIterator>
inline
std::ostream& 
operator<<(std::ostream& out, 
	   const KDTree<N,_Record,_MultiKey,_Key,_MultiKeyAccessor,
	   _RecordOutputIterator>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_KDTree_ipp__
#include "KDTree.ipp"
#undef __geom_KDTree_ipp__

#endif
