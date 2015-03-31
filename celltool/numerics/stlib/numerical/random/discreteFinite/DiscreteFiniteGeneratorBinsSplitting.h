// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorBinsSplitting.h
  \brief Discrete, finite deviate.  BinsSplittingStacking.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorBinsSplitting_h__)
#define __numerical_DiscreteFiniteGeneratorBinsSplitting_h__

#include "DfgBinConstants.h"
#include "DfgRepair.h"
#include "DfgRebuild.h"

#include "../uniform/Default.h"

#include "../../../ads/array/Array.h"
#include "../../../ads/algorithm/sort.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorBinsSplitting)
#define DEBUG_DiscreteFiniteGeneratorBinsSplitting
#endif

BEGIN_NAMESPACE_NUMERICAL

// CONTINUE: Consider writing a class in which PMF's of zero would not have a 
// bin.  This could be useful for the case that many reactions have zero
// propensity.

template<bool IsDynamic = true,
	 bool UseImmediateUpdate = true,
	 class BinConstants = DfgBinConstantsDynamic<>,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class DiscreteFiniteGeneratorBinsSplitting;

//! Discrete, finite deviate.  Bins with splitting.
/*!
  \param BinConstants A policy class that handles the number of bins and 
  related constants.
  \param Generator The discrete uniform generator.

  This generator stores the PMF in a bin array.  Note that the size of the 
  PMF array must be no greater than the number of bins.
  Each probability is split across a number of bins.  
  They are split so as to minimize the maximum bin height.  One can determine
  the splitting with a greedy algorithm:  Start by using a single bin 
  for each probability.  Repeatedly expand the probability with greatest
  bin height into one more bin until all of the bins are being used.
  The correctness of this approach is easily proved by using recursion on
  the number of bins.
  
  Note that even probabilities with value zero occupy a bin.  This enables
  one to efficiently modify any probability.  One could design a data structure
  in which zero probabilities do not occupy bins.
 
  The rejection method is used to generate deviates.  
  A portion of the bits are used for indexing into the array.
  The remaining bits are used for the rejection test.
  
  Consider the probabilities that are split across multiple bins.  The 
  associated bin heights differ by at most a factor of two.  (Otherwise the
  probability for the shortest bins could be stored in one fewer bin and still
  have bin height less than the maximum.  Then the probability with maximum
  height could be expanded across an additional bin, thus reducing the maximum
  bin height.)  For the probabilities which are stored in a single 
  bin, the bin heights are no greater than twice maximum height for those
  split across multiple bins.

  Let B be the number of bins, P be the number of 
  probabilities, and S be the sum of the probabilities. 
  If P = B, the maximum bin height is no greater than S.
  Otherwise, there are at most P - 1 bins that contain unsplit probabilities,
  and at least B - P + 1 bins that contain split probabilities.
  Otherwise, the maximum bin height is 4 S / (B - P + 1).
*/
template<bool UseImmediateUpdate, class BinConstants, class Generator>
class DiscreteFiniteGeneratorBinsSplitting<false, UseImmediateUpdate, BinConstants, Generator> :
  public BinConstants {
  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The number type.
  typedef typename BinConstants::Number Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;

  //
  // Private types.
  //
private:

  //! The bin constants policy class.
  typedef BinConstants BinConstantsBase;

  //
  // Member data.
  //

private:

  //! The discrete uniform generator.
  DiscreteUniformGenerator* _discreteUniformGenerator;

protected:

  //! An upper bound on the height of the bins.
  Number _heightUpperBound;
  //! The binned probability mass function.
  ads::Array<1, Number> _binnedPmf;
  //! The indices of the deviate in the bin.
  ads::Array<1, int> _deviateIndices;

  //! The sum of the PMF.
  Number _pmfSum;
  //! Probability mass function.  (This is scaled and may not sum to unity.)
  ads::Array<1, Number> _pmf;
  //! The index of the first bin containing the PMF.
  ads::Array<1, int> _binIndices;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorBinsSplitting();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorBinsSplitting(DiscreteUniformGenerator* generator) :
    BinConstantsBase(),
    _discreteUniformGenerator(generator),
    _heightUpperBound(-1),
    _binnedPmf(getNumberOfBins()),
    _deviateIndices(getNumberOfBins() + 1),
    _pmfSum(-1),
    _pmf(),
    _binIndices()
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorBinsSplitting(DiscreteUniformGenerator* generator,
				       ForwardIterator begin, 
				       ForwardIterator end) :
    BinConstantsBase(),
    _discreteUniformGenerator(generator),
    _heightUpperBound(-1),
    _binnedPmf(getNumberOfBins()),
    _deviateIndices(getNumberOfBins() + 1),
    _pmfSum(-1),
    _pmf(),
    _binIndices() {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplitting
  (const DiscreteFiniteGeneratorBinsSplitting& other) :
    BinConstantsBase(other),
    _discreteUniformGenerator(other._discreteUniformGenerator),
    _heightUpperBound(other._heightUpperBound),
    _binnedPmf(other._binnedPmf),
    _deviateIndices(other._deviateIndices),
    _pmfSum(other._pmfSum),
    _pmf(other._pmf),
    _binIndices(other._binIndices)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplitting&
  operator=(const DiscreteFiniteGeneratorBinsSplitting& other) {
    if (this != &other) {
      BinConstantsBase::operator=(other);
      _discreteUniformGenerator = other._discreteUniformGenerator;
      _heightUpperBound = other._heightUpperBound;
      _binnedPmf = other._binnedPmf;
      _deviateIndices = other._deviateIndices;
      _pmfSum = other._pmfSum;
      _pmf = other._pmf;
      _binIndices = other._binIndices;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorBinsSplitting()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
public:

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _discreteUniformGenerator->seed(seedValue);
  }

  //! Return a discrete, finite deviate.
  result_type
  operator()();

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  Number
  getPmf(const int index) const {
    return _pmf[index];
  }

  //! Get the number of possible deviates.
  int
  getSize() const {
    return _pmf.size();
  }

  //! Get the sum of the probability mass functions.
  Number
  getPmfSum() const {
    return _pmfSum;
  }

  //! Return true if the sum of the PMF is positive.
  bool
  isValid() const {
    return getPmfSum() > 0;
  }

  //! Compute the efficiency of the method.
  Number
  computeEfficiency() const {
    return _pmfSum / (_heightUpperBound * _binnedPmf.size());
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end);

  //! Set the number of index bits.
  /*!
    This functionality is available only if the bin constants policy class 
    is dynamic.

    \note You must call initialize() after this function to (re)build the
    data structure.
  */
  void
  setIndexBits(const int indexBits) {
    BinConstantsBase::setIndexBits(indexBits);
    _binnedPmf.resize(getNumberOfBins());
    _deviateIndices.resize(getNumberOfBins() + 1);
  }

protected:

  //! Rebuild the bins.
  void
  rebuild();

  //! Update the data structure by recomputing the sum of the PMF's.
  void
  computePmfSum() {
    _pmfSum = std::accumulate(_pmf.begin(), _pmf.end(), 0.0);
  }

  //! Pack the PMF's into bins.
  /*!
    \pre The PMF must be sorted in descending order.
  */
  void
  packIntoBins();

  //! Fix the bin.
  void
  fixBin(int binIndex);

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const;

  //@}
  //--------------------------------------------------------------------------
  // Inherited from the bin constants policy class.
private:

  //! The number of bits used for indexing the bins.
  using BinConstantsBase::getIndexBits;

  //! The number of bins. 2^IndexBits.
  using BinConstantsBase::getNumberOfBins;

  //! The mask for extracting the index.  
  using BinConstantsBase::getIndexMask;

  //! The inverse of the maximum height.  1 / (2^(32-_indexBits) - 1).
  using BinConstantsBase::getMaxHeightInverse;
};



// CONTINUE
//#define MODIFY

//! Discrete, finite deviate.  Bins with splitting.
/*!
  \param BinConstants A policy class that handles the number of bins and 
  related constants.
  \param Generator The discrete uniform generator.
*/
template<bool UseImmediateUpdate, class BinConstants, class Generator>
class DiscreteFiniteGeneratorBinsSplitting
<true, UseImmediateUpdate, BinConstants, Generator> :
  public DiscreteFiniteGeneratorBinsSplitting
<false, UseImmediateUpdate, BinConstants, Generator>, 
  DfgRepairCounter<true> {
  //
  // Private types.
  //
private:

  //! The static version of this data structure.
  typedef DiscreteFiniteGeneratorBinsSplitting
  <false, UseImmediateUpdate, BinConstants, Generator> Base;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<true> RepairBase;

  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef typename Base::DiscreteUniformGenerator DiscreteUniformGenerator;
  //! The number type.
  typedef typename Base::Number Number;
  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //
  // Member data.
  //

private:

  //! An upper bound on the height of the bins.
  using Base::_heightUpperBound;
  //! The binned probability mass function.
  using Base::_binnedPmf;
  //! The indices of the deviate in the bin.
  using Base::_deviateIndices;

  //! The sum of the PMF.
  using Base::_pmfSum;
  //! Probability mass function.  (This is scaled and may not sum to unity.)
  using Base::_pmf;
  //! The index of the first bin containing the PMF.
  using Base::_binIndices;

#ifdef MODIFY
  //! The index of the bin that will be modified when setting the PMF.
  ads::Array<1, int> _binsToModify;
#endif
  //! The minimum allowed efficiency.
  Number _minimumEfficiency;
  //! The minimum allowed efficiency is the initial efficiency times this factor.
  Number _minimumEfficiencyFactor;
  
  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorBinsSplitting();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorBinsSplitting(DiscreteUniformGenerator* generator) :
    Base(generator),
    RepairBase(),
#ifdef MODIFY
    _binsToModify(),
#endif
    _minimumEfficiency(-1),
    // CONTINUE
    _minimumEfficiencyFactor(0.75)
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorBinsSplitting(DiscreteUniformGenerator* generator,
				       ForwardIterator begin, 
				       ForwardIterator end) :
    Base(generator),
    RepairBase(),
#ifdef MODIFY
    _binsToModify(),
#endif
    _minimumEfficiency(-1),
    _minimumEfficiencyFactor(0.75) {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplitting
  (const DiscreteFiniteGeneratorBinsSplitting& other) :
    Base(other),
    RepairBase(other),
#ifdef MODIFY
    _binsToModify(other._binsToModify),
#endif
    _minimumEfficiency(other._minimumEfficiency),
    _minimumEfficiencyFactor(other._minimumEfficiencyFactor)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplitting&
  operator=(const DiscreteFiniteGeneratorBinsSplitting& other) {
    if (this != &other) {
      Base::operator=(other);
      RepairBase::operator=(other);
#ifdef MODIFY
      _binsToModify = other._binsToModify;
#endif
      _minimumEfficiency = other._minimumEfficiency;
      _minimumEfficiencyFactor = other._minimumEfficiencyFactor;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorBinsSplitting()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
public:

  //! Seed the uniform random number generator.
  using Base::seed;

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

  //! Get the sum of the probability mass functions.
  using Base::getPmfSum;

  //! Return true if the sum of the PMF is positive.
  using Base::isValid;

  //! Compute the efficiency of the method.
  using Base::computeEfficiency;

  //! Get the number of steps between repairs.
  using RepairBase::getStepsBetweenRepairs;

  //! Get the minimum allowed efficiency.
  Number
  getMinimumEfficiency() const {
    return _minimumEfficiency;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end);

  //! Set the number of index bits.
  using Base::setIndexBits;

  //! Set the probability mass function with the specified index.
  void
  setPmf(int index, Number value) {
    _setPmf(index, value, Loki::Int2Type<UseImmediateUpdate>());
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  setPmf(_RandomAccessIterator iterator) {
    for (int i = 0; i != _pmf.size(); ++i) {
      setPmf(i, iterator[i]);
    }
  }

  //! Update the data structure following calls to setPmf().
  void
  updatePmf() {
    updatePmf(Loki::Int2Type<UseImmediateUpdate>());
  }

  //! Set the number of steps between repairs.
  using RepairBase::setStepsBetweenRepairs;

  //! Set the minimum allowed efficiency factor.
  void
  setMinimumEfficiencyFactor(const Number efficiency) const {
    _minimumEfficiencyFactor = efficiency;
  }

private:

  //! Set the probability mass function with the specified index.
  /*!
    This will update the data structure.
  */
  void
  _setPmf(int index, Number value, Loki::Int2Type<true> /*dummy*/);

  //! Set the probability mass function with the specified index.
  /*!
    \note After calling this function, you must call updatePmf() before
    computing deviates.
  */
  void
  _setPmf(int index, Number value, Loki::Int2Type<false> /*dummy*/) {
    // Update the PMF array.
    _pmf[index] = value;
  }

  //! Update the data structure following calls to setPmf().
  void
  updatePmf(Loki::Int2Type<true> /*dummy*/);

  //! Update the data structure following calls to setPmf().
  void
  updatePmf(Loki::Int2Type<false> /*dummy*/);

  //! Repair the data structure.
  /*!
    Recompute the PMF data.
  */
  void
  repair();

  //! Rebuild the bins.
  void
  rebuild();

  //! Update the minimum allowed efficiency.
  void
  updateMinimumAllowedEfficiency() {
    // Set the minimum allowed efficiency.
    _minimumEfficiency = computeEfficiency() * _minimumEfficiencyFactor;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const;

  //@}
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteFiniteGeneratorBinsSplitting_ipp__
#include "DiscreteFiniteGeneratorBinsSplitting.ipp"
#undef __numerical_random_DiscreteFiniteGeneratorBinsSplitting_ipp__

#endif
