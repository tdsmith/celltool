// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfSortedByMaxInfluencingProbabilityAscending.h
  \brief Sorted probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmfSortedByMaxInfluencingProbabilityAscending_h__)
#define __numerical_DfgPmfSortedByMaxInfluencingProbabilityAscending_h__

#include "DfgPmf.h"
#include "DfgPmfSorted.h"
#include "DfgRebuild.h"

// For sortTogether.
#include "../../../ads/algorithm/sort.h"
#include "../../../ads/array/StaticArrayOfArrays.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfSortedByMaxInfluencingProbabilityAscending)
#define DEBUG_numerical_DfgPmfSortedByMaxInfluencingProbabilityAscending
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Sorted probability mass function for a discrete, finite generator.
/*!
  \param Pmf The policy class for the PMF.
  \param RebuildCounter The policy class for rebuilding (sorting) the
  probabilities.
  \param T The number type.  By default it is double.

  Manage the probability mass function.  The PMF is sorted by rebuilding.

  CONTINUE: Using the accumulated influencing probability would be better.
*/
template<class Pmf = DfgPmf<>, 
	 class RebuildCounter = DfgRebuildCounter<true> >
class DfgPmfSortedByMaxInfluencingProbabilityAscending :
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

  //
  // Member data.
  //
private:

  const ads::StaticArrayOfArrays<int>* _influence;
  //! The array of maximum influencing probabilities.
  ads::Array<1, Number> _maximumInfluencingProbabilities;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmfSortedByMaxInfluencingProbabilityAscending() :
    Base(),
    _influence(0),
    _maximumInfluencingProbabilities()
  {}

  //! Copy constructor.
  DfgPmfSortedByMaxInfluencingProbabilityAscending
  (const DfgPmfSortedByMaxInfluencingProbabilityAscending& other) :
    Base(other),
    _influence(other._influence),
    _maximumInfluencingProbabilities(other._maximumInfluencingProbabilities)
  {}

  //! Assignment operator.
  DfgPmfSortedByMaxInfluencingProbabilityAscending&
  operator=(const DfgPmfSortedByMaxInfluencingProbabilityAscending& other) {
    if (this != &other) {
      Base::operator=(other);
      _influence = other._influence;
      _maximumInfluencingProbabilities = other._maximumInfluencingProbabilities;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfSortedByMaxInfluencingProbabilityAscending()
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
  /*!
    \note You must call set the influence array with setInfluence() before 
    calling this function.
  */
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    assert(_influence != 0);
    Base::initialize(begin, end);
    //! The array of maximum influencing probabilities.
    _maximumInfluencingProbabilities.resize(getSize());
    // Sort.
    rebuild();
  }

  //! Set the number of steps between rebuilds.
  using Base::setStepsBetweenRebuilds;

  //! Set the influence array.
  void
  setInfluence(const ads::StaticArrayOfArrays<int>* influence) {
    _influence = influence;
  }

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
    // Build the array of maximum influencing probabilities.
    _maximumInfluencingProbabilities = 0;
    int k;
    // For each probability.
    for (int i = 0; i != getSize(); ++i) {
      // For each probability that in influenced by it.
      for (int j = 0; j != _influence->size(i); ++j) {
	// If the i_th probabilty changes, the k_th will be affected.
	k = (*_influence)(i, j);
	if (getPmfBeginning()[i] > _maximumInfluencingProbabilities[k]) {
	  _maximumInfluencingProbabilities[k] = getPmfBeginning()[i];
	}
      }
    }

    // Sort in ascending order.
    ads::sortTogether(_maximumInfluencingProbabilities.begin(),
		      _maximumInfluencingProbabilities.end(),
		      Base::getPmfBeginning(), Base::getPmfEnd(), 
		      Base::getIndicesBeginning(), Base::getIndicesEnd());
    Base::computeRanks();
    Base::resetRebuildCounter();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    Base::print(out);
    out << "Influence = \n" << *_influence
	<< "Max influencing probabilities = \n" 
	<< _maximumInfluencingProbabilities;
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
