// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfSorted.h
  \brief Sorted probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmfSorted_h__)
#define __numerical_DfgPmfSorted_h__

#include "linearSearch.h"

#include "../../../ads/array/Array.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfSorted)
#define DEBUG_numerical_DfgPmfSorted
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Sorted probability mass function for a discrete, finite generator.
/*!
  \param Pmf The policy class for the PMF.
  \param RebuildCounter The policy class for rebuilding (sorting) the
  probabilities.

  This is a base class for a sorted PMF.  It manages the permutation and 
  ranks for the probabilities, but does not know how to sort them.
*/
template<class Pmf,
	 class RebuildCounter>
class DfgPmfSorted :
  public Pmf, RebuildCounter {
  //
  // Private types.
  //
private:

  //! The base type.
  typedef Pmf PmfBase;
  //! The interface for rebuilding the data structure.
  typedef RebuildCounter RebuildBase;
  
  //
  // Public types.
  //
public:

  //! The number type.
  typedef typename PmfBase::Number Number;

  //
  // Member data.
  //
private:

  //! The index of the element in the original probability mass function array.
  /*!
    This is useful when traversing the _pmf array.  We can efficiently go from
    the PMF value to its index.
  */
  ads::Array<1, int> _index;
  //! The rank of the elements in _pmf array.
  /*!
    This is useful for manipulating the _pmf array by index.  \c _pmf[rank[i]]
    is the i_th element in the original PMF array.
    
    The rank array is the inverse of the index array mapping.  That is,
    \c _rank[_index[i]]==i and \c _index[_rank[i]]==i .
  */
  ads::Array<1, int> _rank;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmfSorted() :
    PmfBase(),
    // By default, take 1000 steps between rebuilds.
    RebuildBase(1000L),
    // The arrays are empty.
    _index(),
    _rank()
  {}

  //! Copy constructor.
  DfgPmfSorted(const DfgPmfSorted& other) :
    PmfBase(other),
    RebuildBase(other),
    _index(other._index),
    _rank(other._rank)
  {}

  //! Assignment operator.
  DfgPmfSorted&
  operator=(const DfgPmfSorted& other) {
    if (this != &other) {
      PmfBase::operator=(other);
      RebuildBase::operator=(other);
      _index = other._index;
      _rank = other._rank;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfSorted()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
protected:

  //! Return a discrete, finite deviate.
  /*!
    Use the linear search of the base class.
  */
  int
  operator()(const Number r, const int offset = 0) const {
    return _index[PmfBase::operator()(r, offset)];
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  Number
  getPmf(const int index) const {
    return PmfBase::getPmf(_rank[index]);
  }

  //! Get the number of possible deviates.
  using PmfBase::getSize;

  //! Get the number of steps between rebuilds.
  using RebuildBase::getStepsBetweenRebuilds;

protected:

  //! Get the beginning of the probabilities in the PMF.
  using PmfBase::getPmfBeginning;

  //! Get the end of the probabilities in the PMF.
  using PmfBase::getPmfEnd;

  //! Get the index of the specified element.
  int
  getIndex(const int n) const {
    return _index[n];
  }

  //! Return true if the data structure should be rebuilt.
  using RebuildBase::shouldRebuild;

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgPmfSorted& other) const {
    return PmfBase::operator==(other) && RebuildBase::operator==(other) &&
      _index == other._index && _rank == other._rank;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    // Initialize the PMF array.
    PmfBase::initialize(begin, end);

    _index.resize(getSize());
    _rank.resize(getSize());
    // Initialize the index array.
    for (int i = 0; i != _index.size(); ++i) {
      _index[i] = i;
    }
  }

  //! Set the number of steps between rebuilds.
  using RebuildBase::setStepsBetweenRebuilds;

protected:

  //! Set the probability mass function with the specified index.
  void
  setPmf(int index, Number value) {
    PmfBase::setPmf(_rank[index], value);
    RebuildBase::decrementRebuildCounter();
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  setPmf(_RandomAccessIterator iterator) {
    for (int i = 0; i != getSize(); ++i) {
      PmfBase::setPmf(_rank[i], iterator[i]);
    }
    RebuildBase::decrementRebuildCounter(getSize());
  }

  //! Reset the rebuild counter.
  using RebuildBase::resetRebuildCounter;

  //! Compute the ranks.
  void
  computeRanks() {
    for (int i = 0; i != _index.size(); ++i) {
      _rank[_index[i]] = i;
    }
  }

  //! Return the beginning of the indices.
  ads::Array<1, int>::iterator
  getIndicesBeginning() {
    return _index.begin();
  }

  //! Return the end of the indices.
  ads::Array<1, int>::iterator
  getIndicesEnd() {
    return _index.end();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    PmfBase::print(out);
    out << "Index = \n" << _index << "\n"
	<< "Rank = \n" << _rank << "\n";
    RebuildBase::print(out);
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
