// -*- C++ -*-

/*! 
  \file SortFirstDynamic.h
  \brief A class for a vector of records in N-D sorted in each dimension.
*/

#if !defined(__geom_SortFirstDynamic_h__)
#define __geom_SortFirstDynamic_h__

#include "ORQ.h"

#include "../../ads/algorithm/sort.h"
#include "../../ads/functor/Dereference.h"

#include <iostream>
#include <map>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SortFirstDynamic)
#define DEBUG_SortFirstDynamic
#endif

BEGIN_NAMESPACE_GEOM

//! A sorted vector of records in N-D.
/*!
  Dimension sorted vectors of records.
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class SortFirstDynamic : 
  public ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> {
private:

  //
  // Private types.
  //

  typedef ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> Base;
  //! The container of record iterators.
  typedef std::multimap<_Key,_Record> Container;
  //! A const iterator.
  typedef typename Container::value_type Value;
  typedef typename Container::iterator Iterator;
  typedef typename Container::const_iterator ConstIterator;

  // CONTINUE: With the Intel compiler, private members are not accessible
  // in nested classes.
#ifdef __INTEL_COMPILER
public:
#else
private:
#endif

  typedef _MultiKeyAccessor MultiKeyAccessor;

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

  //! The size type.
  typedef typename Base::SizeType SizeType;
  //! A Cartesian point.
  typedef typename Base::Point Point;
  //! Bounding box.
  typedef typename Base::BBox BBox;

protected:

  //
  // Comparison functors.
  //

  //! Less than comparison in a specified coordinate.
  class LessThanCompare :
    public std::binary_function<Record, Record, bool> {
  private:

    MultiKeyAccessor _f;

  public:

    //! Less than comparison in the first coordinate.
    bool 
    operator()(const Record x, const Record y) const {
      return _f(x)[0] < _f(y)[0];
    }
  };

  //
  // Member data.
  //

private:

  //! Records sorted by the first coordinate.
  Container _records;
  mutable LessThanCompare _lessThanCompare;

public:

  //-------------------------------------------------------------------------
  //! \name Constructors.
  // @{

  //! Default constructor.
  SortFirstDynamic() :
    Base(),
    _records()
  {}

  //! Copy constructor.
  SortFirstDynamic(const SortFirstDynamic& other) :
    _records(other._records)
  {}

  //! Assignment operator.
  SortFirstDynamic& 
  operator=(const SortFirstDynamic& other) {
    if (&other != this) {
      _records = other._records;
    }
    return *this;
  }

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template<class InputIterator>
  SortFirstDynamic(InputIterator first, InputIterator last) :
    Base(),
    _records() {
    insert(first, last);
  }

  // Trivial destructor.
  ~SortFirstDynamic()
  {}

  // @}
  //--------------------------------------------------------------------------
  /*! \name Accesors.
    Functionality inherited from ORQ.
  */
  // @{

  //! Return the number of records.
  using Base::getSize;

  //! Return true if the grid is empty.
  using Base::isEmpty;

  // @}
  //-------------------------------------------------------------------------
  //! \name Insert records.
  // @{

  //! Insert a single record.
  void 
  insert(const Record record) {
    _records.insert(Value(getMultiKey(record)[0],record));
    incrementSize();
  }


  //! Insert a range of records.
  template<class InputIterator>
  void 
  insert(InputIterator first, InputIterator last) {
    while (first != last) {
      insert(first);
      ++first;
    }
  }
  
  //! Erase a record.
  /*!
    The record must exist.
  */
  void
  erase(const Record record) {
    const Key key = getMultiKey(record)[0];
    Iterator i = _records.lower_bound(key);
    assert(i != _records.end());
    // For each record with this key.
    for (; i->first == key; ++i) {
      if (i->second == record) {
	_records.erase(i);
	return;
      }
    }
    assert(false);
  }

  // @}
  //-------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Get the records in the window.  Return the # of records inside.
  template<typename OutputIterator>
  SizeType 
  computeWindowQuery(OutputIterator iter, const BBox& window) const;

  // @}
  //-------------------------------------------------------------------------
  //! \name File I/O.
  // @{

  //! Print the records.
  void 
  put(std::ostream& out) const;

  // @}

protected:

  //! Increment the number of records.
  using Base::incrementSize;

  //! Get the multi-key of a record.
  using Base::getMultiKey;
};


//! Write to a file stream.
/*! \relates SortFirstDynamic */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor>
inline
std::ostream& 
operator<<
  (std::ostream& out, 
   const SortFirstDynamic<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_SortFirstDynamic_ipp__
#include "SortFirstDynamic.ipp"
#undef __geom_SortFirstDynamic_ipp__

#endif
