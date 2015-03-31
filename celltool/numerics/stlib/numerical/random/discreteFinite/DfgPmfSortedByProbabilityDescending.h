// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfSortedByProbabilityDescending.h
  \brief Sorted probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmfSortedByProbabilityDescending_h__)
#define __numerical_DfgPmfSortedByProbabilityDescending_h__

#include "DfgPmfSorted.h"
#include "DfgPmfWithGuard.h"
#include "DfgRebuild.h"

// For sortTogether.
#include "../../../ads/algorithm/sort.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfSortedByProbabilityDescending)
#define DEBUG_numerical_DfgPmfSortedByProbabilityDescending
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Sorted probability mass function for a discrete, finite generator.
/*!
  \param Pmf The policy class for the PMF.
  \param RebuildCounter The policy class for rebuilding (sorting) the
  probabilities.
  \param T The number type.  By default it is double.

  Manage the probability mass function.  The PMF is sorted by rebuilding.
*/
template<class Pmf = DfgPmfWithGuard<>, 
	 class RebuildCounter = DfgRebuildCounter<true> >
class DfgPmfSortedByProbabilityDescending :
  public DfgPmfSorted<Pmf, RebuildCounter> {
  //
  // Private types.
  //
private:

  //! The base type.
  typedef DfgPmfSorted<Pmf, RebuildCounter> Base;
  
  //
  // Public types.
  //
public:

  //! The number type.
  typedef typename Base::Number Number;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmfSortedByProbabilityDescending() :
    Base()
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DfgPmfSortedByProbabilityDescending(ForwardIterator begin, 
				      ForwardIterator end) :
    Base() {
    initialize(begin, end);
  }

  //! Copy constructor.
  DfgPmfSortedByProbabilityDescending
  (const DfgPmfSortedByProbabilityDescending& other) :
    Base(other)
  {}

  //! Assignment operator.
  DfgPmfSortedByProbabilityDescending&
  operator=(const DfgPmfSortedByProbabilityDescending& other) {
    if (this != &other) {
      Base::operator=(other);
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfSortedByProbabilityDescending()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
protected:

  //! Return a discrete, finite deviate.
  using Base::operator();

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  using Base::getPmf;

  //! Get the number of possible deviates.
  using Base::getSize;

  //! Get the number of steps between rebuilds.
  using Base::getStepsBetweenRebuilds;

protected:

  //! Get the beginning of the probabilities in the PMF.
  using Base::getPmfBeginning;

  //! Get the end of the probabilities in the PMF.
  using Base::getPmfEnd;

  //! Get the index of the specified element.
  using Base::getIndex;

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  using Base::operator==;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    Base::initialize(begin, end);
    // Sort.
    rebuild();
  }

  //! Set the number of steps between rebuilds.
  using Base::setStepsBetweenRebuilds;

protected:

  //! Set the probability mass function with the specified index.
  using Base::setPmf;

  //! Check if the data structure needs rebuilding.
  void
  updatePmf() {
    if (Base::shouldRebuild()) {
      rebuild();
    }
  }

private:

  //! Sort the probabilities.
  void
  rebuild() {
    // Sort in descending order.
    ads::sortTogether(Base::getPmfBeginning(), Base::getPmfEnd(), 
		      Base::getIndicesBeginning(), Base::getIndicesEnd(),
		      std::greater<Number>());
    Base::computeRanks();
    Base::resetRebuildCounter();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  using Base::print;

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
