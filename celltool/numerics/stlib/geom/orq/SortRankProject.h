// -*- C++ -*-

/*! 
  \file SortRankProject.h
  \brief A class for a coordinate sorted and ranked vector of grid elements 
  in N-D.  The class implements the point-in-box method.
*/

#if !defined(__geom_SortRankProject_h__)
#define __geom_SortRankProject_h__

#include "ORQ.h"

#include "../../ads/functor/Dereference.h"

#include <vector>
#include <algorithm>

// If we are debugging the whole geom package.
#if defined(DEBUG_geom) && !defined(DEBUG_SortRankProject)
#define DEBUG_SortRankProject
#endif

BEGIN_NAMESPACE_GEOM

//! A sorted and ranked vector of grid elements in N-D.
/*!
  Coordinate sorted and ranked vector of records.

  This class implements the point-in-box method developed by Swegle, 
  et. al.  
*/
template<int N,
	 typename _Record,
	 typename _MultiKey = 
	 typename std::iterator_traits<_Record>::value_type,
	 typename _Key = typename _MultiKey::value_type,
	 typename _MultiKeyAccessor = ads::Dereference<_Record> >
class SortRankProject : 
  public ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> {
private:

  //
  // Private types.
  //

  typedef ORQ<N,_Record,_MultiKey,_Key,_MultiKeyAccessor> Base;

  // CONTINUE: With the Intel compiler, private members are not accessible
  // in nested classes.
#ifdef __INTEL_COMPILER
public:
#else
private:
#endif

  typedef _MultiKeyAccessor MultiKeyAccessor;
  typedef typename std::vector<_Record>::const_pointer const_pointer;

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

private:

  //
  // Comparison functors.
  //

  //! Less than comparison in a specified coordinate for pointers to records.
  class LessThanCompare :
    public std::binary_function<const_pointer, const_pointer, bool> {
  private:

    int _n;
    MultiKeyAccessor _f;

  public:

    //! Default constructor.  The coordinate to compare has an invalid value.
    LessThanCompare() :
      _n(-1)
    {}
  
    //! Set the coordinate to compare.
    void
    set(const int n) {
      _n = n;
    }

    //! Less than comparison in a specified coordinate.
    bool 
    operator()(const const_pointer x, const const_pointer y) const {
      return _f(*x)[_n] < _f(*y)[_n];
    }
  };


  //! Less than comparison in a specified coordinate for a pointer to a record and a multi-key.
  class LessThanComparePointerAndMultiKey :
    public std::binary_function<const_pointer, MultiKey, bool> {
  private:

    int _n;
    MultiKeyAccessor _f;

  public:

    //! Default constructor.  The coordinate to compare has an invalid value.
    LessThanComparePointerAndMultiKey() :
      _n(-1)
    {}
  
    //! Set the coordinate to compare.
    void
    set(const int n) {
      _n = n;
    }

    //! Less than comparison in a specified coordinate.
    bool 
    operator()(const const_pointer recordPointer, 
	       const MultiKey& multiKey) const {
      return _f(*recordPointer)[_n] < multiKey[_n];
    }
  };


  //! Less than comparison in a specified coordinate for a multi-key and a pointer to record iterator.
  class LessThanCompareMultiKeyAndPointer :
    public std::binary_function<MultiKey, const_pointer, bool> {
  private:

    int _n;
    MultiKeyAccessor _f;

  public:

    //! Default constructor.  The coordinate to compare has an invalid value.
    LessThanCompareMultiKeyAndPointer() :
      _n(-1)
    {}
  
    //! Set the coordinate to compare.
    void
    set(const int n) {
      _n = n;
    }

    //! Less than comparison in a specified coordinate.
    bool 
    operator()(const MultiKey& multiKey, 
	       const const_pointer recordPointer) const {
      return  multiKey[_n] < _f(*recordPointer)[_n];
    }
  };

  //
  // Member data.
  //

  //! The vector of pointers to records.
  std::vector<Record> _records;

  //! Pointers to elements sorted by each coordinate.
  ads::FixedArray<N, std::vector<const_pointer> > _sorted;

  //! The rank of the records in each coordinate.
  ads::FixedArray<N, std::vector<int> > _rank;

private:

  //
  // Not implemented
  //

  //! Copy constructor not implemented
  SortRankProject(const SortRankProject&);

  //! Assignment operator not implemented
  SortRankProject& 
  operator=(const SortRankProject&);

public:

  //-------------------------------------------------------------------------
  //! \name Constructors.
  // @{

  //! Default constructor.
  SortRankProject();

  //! Reserve storage for \c size records.
  explicit 
  SortRankProject(const SizeType size);

  //! Construct from a range of records.
  /*!
    \param first the beginning of a range of records.
    \param last the end of a range of records.
  */
  template <typename InputIterator>
  SortRankProject(InputIterator first, InputIterator last);

  // Destructor.
  ~SortRankProject()
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
    _records.push_back(record);
    incrementSize();
  }

  //! Insert a range of records.
  template<typename InputIterator>
  void 
  insert(InputIterator first, InputIterator last) {
    while (first != last) {
      insert(first);
      ++first;
    }
  }
  
  // @}
  //-------------------------------------------------------------------------
  //! \name Memory usage.
  // @{
  
  //! Return the total memory usage.
  SizeType 
  getMemoryUsage() const;
  
  // @}
  //-------------------------------------------------------------------------
  //! \name Window Queries.
  // @{

  //! Sort and rank the records in each coordinate.
  void 
  sortAndRank();

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
  //-------------------------------------------------------------------------
  //! \name Validity check.
  // @{

  //! Check the validity of the data structure.
  bool
  isValid() const;

  //@}

protected:

  //! Increment the number of records.
  using Base::incrementSize;

  //! Get the multi-key of a record.
  using Base::getMultiKey;
};

//! Write to a file stream.
/*! \relates SortRankProject */
template<int N,
	 typename _Record,
	 typename _MultiKey,
	 typename _Key,
	 typename _MultiKeyAccessor>
inline
std::ostream& 
operator<<
  (std::ostream& out, 
   const SortRankProject<N,_Record,_MultiKey,_Key,_MultiKeyAccessor>& x) {
  x.put(out);
  return out;
}


END_NAMESPACE_GEOM

#define __geom_SortRankProject_ipp__
#include "SortRankProject.ipp"
#undef __geom_SortRankProject_ipp__

#endif
