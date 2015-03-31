// -*- C++ -*-

/*! 
  \file IntSetSparse.h
  \brief A set of integers.
*/

#if !defined(__ads_IntSetSparse_h__)
#define __ads_IntSetSparse_h__

#include "../defs.h"

#include "../algorithm/is_sorted.h"

#include <algorithm>
#include <iosfwd>
#include <iterator>
#include <set>

#include <cassert>

#if defined(DEBUG_ads) && !defined(DEBUG_IntSetSparse)
#define DEBUG_IntSetSparse
#endif

BEGIN_NAMESPACE_ADS

//! A set of integers.
template <typename T = int>
class IntSetSparse :
  public std::set<T> {
  //
  // Private types.
  //

private:

  //! The base type.
  typedef std::set<T> base_type;

  //
  // Public types.
  //

public:

  //! An element iterator.
  typedef typename base_type::iterator iterator;
  //! A const iterator on the elements.
  typedef typename base_type::const_iterator const_iterator;
  //! The value type.
  typedef typename base_type::value_type value_type;
  //! The size type.
  typedef int size_type;

  //
  // Data
  //

private:

  // Upper bound on the elements.
  size_type _ub;

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Default constructor.  Empty set.
  IntSetSparse() :
    base_type(),
    _ub(0)
  {}

  //! Construct from the element upper bound.
  IntSetSparse(const value_type upper_bound) :
    base_type(),
    _ub(upper_bound)
  {}

  //! Construct from the element upper bound and a range of elements.
  template <typename IntInIter>
  IntSetSparse(IntInIter start, IntInIter finish, 
		const value_type upper_bound) :
    base_type(start, finish),
    _ub(upper_bound)
  {}

  //! Copy constructor.
  IntSetSparse(const IntSetSparse& x) :
    base_type(x),
    _ub(x._ub)
  {}

  //! Assignment operator.
  IntSetSparse& 
  operator=(const IntSetSparse& x) {
    if (this != &x) {
      base_type::operator=(x);
      _ub = x._ub;
    }
    return *this;
  }
  
  //! Destructor.
  ~IntSetSparse()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the upper bound on the elements.
  value_type
  upper_bound() const {
    return _ub;
  }

  //! Return the number of elements.
  size_type
  size() const {
    return base_type::size();
  }

  //! Return true if the size() is zero.
  bool 
  empty() const { 
    return base_type::empty();
  }

  //! Return a const iterator to the first element.
  const_iterator 
  begin() const { 
    return base_type::begin();
  }

  //! Return a const iterator to one past the last element.
  const_iterator 
  end() const { 
    return base_type::end();
  }

  //! Return true if \c x is in the set.
  bool
  is_in(const value_type x) const {
    return base_type::count(x);
  }

  //! Return true if \c x is a subset of this set.
  bool
  subset(const IntSetSparse& x) const {
    return std::includes(begin(), end(), x.begin(), x.end());
  }

  //! Return true if the set is valid.
  /*!
    The elements must be in the range [ 0..upper_bound()).
  */
  bool
  is_valid() const {
    // Check that the elements are in the proper range.
    for (const_iterator i = begin(); i != end(); ++i) {
      if (*i < 0 || *i >= upper_bound()) {
	return false;
      }
    }
    return true;
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Return an iterator to the first element.
  iterator 
  begin() { 
    return base_type::begin();
  }

  //! Return an iterator to one past the last element.
  iterator 
  end() { 
    return base_type::end();
  }

  //! Set the upper bound.
  void
  set_upper_bound(const value_type upper_bound) {
    _ub = upper_bound;
  }

  //! Insert an element.
  std::pair<iterator,bool>
  insert(const value_type x) {
#ifdef DEBUG_IntSetSparse
    assert(0 <= x && x < upper_bound());
#endif
    return base_type::insert(x);
  }

  //! Insert an element using the \c position as a hint to where it will be inserted.
  iterator
  insert(const iterator position, const value_type x) {
#ifdef DEBUG_IntSetSparse
    assert(0 <= x && x < upper_bound());
#endif
    return base_type::insert(position, x);
  }

  //! Insert a range of elements.
  /*!
    \c IntInIter is an input iterator for the value type.
  */
  template <typename IntInIter>
  void
  insert(IntInIter start, IntInIter finish) {
    base_type::insert(start, finish);
  }

  //! Return an insert iterator.
  std::insert_iterator<IntSetSparse>
  inserter() {
    return std::inserter(*this, end());
  }

  //! Erase the element to which \c i points.
  void
  erase(const iterator i) {
    base_type::erase(i);
  }

  //! Erase the specified element.  
  /* 
     Return true if the element was in the set.
  */
  bool
  erase(const value_type x) {
    return base_type::erase(x);
  }

  //! Clear set.
  void
  clear() {
    base_type::clear();
  }

  //! Swap with another set.
  void
  swap(IntSetSparse& x) {
    base_type::swap(x);
    std::swap(_ub, x._ub);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{

  //! Return true if the sets are the same.
  bool
  operator==(const IntSetSparse<T>& x) const {
    return (upper_bound() == x.upper_bound() && 
	     static_cast<const base_type&>(*this) == 
	     static_cast<const base_type&>(x));
  }

  //! Return true if the sets are not the same.
  bool
  operator!=(const IntSetSparse& x) const {
    return ! operator==(x);
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Write the size and the elements.
  void
  put(std::ostream& out) const {
    out << _ub;
    for (const iterator i = begin(); i != end(); ++i) {
      out << *i << "\n";
    }
  }

  //@}
};

//
// File output.
//

//! Write the size and the elements.
/*!
  \relates IntSetSparse
*/
template <typename T>
inline
std::ostream&
operator<<(std::ostream& out, const IntSetSparse<T>& x) {
  x.put(out);
  return out;
}

//
// Set operations.
//

//! Form the union of the two sets.
/*!
  \relates IntSetSparse
*/
template <typename T>
inline
void
set_union(const IntSetSparse<T>& a, const IntSetSparse<T>& b, 
	   IntSetSparse<T>& c) {
  assert(a.upper_bound() <= c.upper_bound() && 
	  b.upper_bound() <= c.upper_bound());
  c.clear();
  std::set_union(a.begin(), a.end(), b.begin(), b.end(), 
		  std::inserter(c, c.end()));
}

//! Form the intersection of the two sets.
/*!
  \relates IntSetSparse
*/
template <typename T>
inline
void
set_intersection(const IntSetSparse<T>& a, const IntSetSparse<T>& b, 
		  IntSetSparse<T>& c) {
  assert(a.upper_bound() <= c.upper_bound() && 
	  b.upper_bound() <= c.upper_bound());
  c.clear();
  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), 
			 std::inserter(c, c.end()));
}

//! Form the difference of the two sets.
/*!
  \relates IntSetSparse
*/
template <typename T>
inline
void
set_difference(const IntSetSparse<T>& a, const IntSetSparse<T>& b, 
		IntSetSparse<T>& c) {
  assert(a.upper_bound() <= c.upper_bound());
  c.clear();
  std::set_difference(a.begin(), a.end(), b.begin(), b.end(), 
		       std::inserter(c, c.end()));
}

//! Form the complement of the set.
/*!
  \relates IntSetSparse
*/
template <typename T>
inline
void
set_complement(const IntSetSparse<T>& a, IntSetSparse<T>& b) {
  assert(a.upper_bound() == b.upper_bound());
#ifdef DEBUG_IntSetSparse
  assert(a.is_valid());
#endif
    
  b.clear();
  typename IntSetSparse<T>::const_iterator i = a.begin();
  // Loop over all integers in the range.
  for (int n = 0; n != a.upper_bound(); ++n) {
    // If the element is in the set.
    if (i != a.end() && *i == n) {
      // Skip the element.
      ++i;
    }
    // If the element is not in the set.
    else {
      // Add it to b.
      b.insert(b.end(), n);
    }
  }
  assert(i == a.end());
}

END_NAMESPACE_ADS

#endif
