// -*- C++ -*-

/*!
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorRejectionBinsSplitting.h
  \brief Discrete, finite deviate.  Rejection with bins and splitting.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorRejectionBinsSplitting_h__)
#define __numerical_DiscreteFiniteGeneratorRejectionBinsSplitting_h__

#include "DfgBinConstants.h"
#include "DfgRepair.h"
#include "DfgRebuild.h"

#include "../uniform/Default.h"

#include "../../../ads/array/Array.h"
#include "../../../ads/algorithm/sort.h"
#include "../../../ads/functor/compare_handle.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorRejectionBinsSplitting)
#define DEBUG_DiscreteFiniteGeneratorRejectionBinsSplitting
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Discrete, finite deviate.  Bins with splitting.
/*!
  Static specialization.

  \param IsDynamic A boolean value that indicates if the class has 
  dynamic capabilities, i.e. you can change the PMF values. 
  \param ExactlyBalance A boolean value that indicates if the PMF should be
  exactly balanced when when distributing it over the bins.  Exact balancing
  minimized the maximum bin height.  You will usually get a little better
  performance by using exact balancing.  If memory usage is critical, you 
  can go with approximate balancing.
  \param BinConstants A policy class that handles the number of bins and 
  related constants.
  \param Generator The discrete uniform generator.
*/
template<bool IsDynamic = true,
	 bool ExactlyBalance = true,
	 class BinConstants = DfgBinConstantsDynamic<>,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class DiscreteFiniteGeneratorRejectionBinsSplitting;

//! Discrete, finite deviate.  Bins with splitting.
/*!
  Static specialization.

  \param ExactlyBalance A boolean value that indicates if the PMF should be
  exactly balanced when when distributing it over the bins.  Exact balancing
  minimized the maximum bin height.  You will usually get a little better
  performance by using exact balancing.  If memory usage is critical, you 
  can go with approximate balancing.
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
  The maximum bin height is 4 S / (B - P + 1).
*/
template<bool ExactlyBalance, class BinConstants, class Generator>
class DiscreteFiniteGeneratorRejectionBinsSplitting
<false, ExactlyBalance, BinConstants, Generator> :
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

  // Bin data.

  //! An upper bound on the height of the bins.
  Number _heightUpperBound;
  //! The indices of the deviate in the bin.
  ads::Array<1, int> _deviateIndices;

  // PMF data.

  //! The sum of the PMF.
  Number _pmfSum;
  //! Probability mass function.  (This is scaled and may not sum to unity.)
  ads::Array<1, Number> _pmf;
  //! The inverse of the number of bins for each probability.
  ads::Array<1, Number> _inverseSizes;
  //! Sorted probability mass function.
  ads::Array<1, typename ads::Array<1, Number>::const_iterator> _sortedPmf;
  //! The indices of the bins containing the PMF.
  /*!
    We only use this if ExactlyBalance is true.
  */
  ads::Array<1, std::vector<int> > _binIndices;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorRejectionBinsSplitting();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorRejectionBinsSplitting
  (DiscreteUniformGenerator* generator) :
    BinConstantsBase(),
    _discreteUniformGenerator(generator),
    // Bin data.
    _heightUpperBound(-1),
    _deviateIndices(getNumberOfBins()),
    // PMF data.
    _pmfSum(-1),
    _pmf(),
    _inverseSizes(),
    _sortedPmf(),
    _binIndices() {
  }

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorRejectionBinsSplitting
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    BinConstantsBase(),
    _discreteUniformGenerator(generator),
    // Bin data.
    _heightUpperBound(-1),
    _deviateIndices(getNumberOfBins()),
    // PMF data.
    _pmfSum(-1),
    _pmf(),
    _inverseSizes(),
    _sortedPmf(),
    _binIndices() {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorRejectionBinsSplitting
  (const DiscreteFiniteGeneratorRejectionBinsSplitting& other) :
    BinConstantsBase(other),
    _discreteUniformGenerator(other._discreteUniformGenerator),
    // Bin data.
    _heightUpperBound(other._heightUpperBound),
    _deviateIndices(other._deviateIndices),
    // PMF data.
    _pmfSum(other._pmfSum),
    _pmf(other._pmf),
    _inverseSizes(other._inverseSizes),
    // We don't need to copy the data, but this makes the array the right size.
    _sortedPmf(other._sortedPmf),
    _binIndices(other._binIndices)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorRejectionBinsSplitting&
  operator=(const DiscreteFiniteGeneratorRejectionBinsSplitting& other) {
    if (this != &other) {
      BinConstantsBase::operator=(other);
      _discreteUniformGenerator = other._discreteUniformGenerator;
      // Bin data.
      _heightUpperBound = other._heightUpperBound;
      _deviateIndices = other._deviateIndices;
      // PMF data.
      _pmfSum = other._pmfSum;
      _pmf = other._pmf;
      _inverseSizes = other._inverseSizes;
      // We don't need to copy the data, but this makes the array the right 
      // size.
      _sortedPmf = other._sortedPmf;
      _binIndices = other._binIndices;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorRejectionBinsSplitting()
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
    return _pmfSum / (_heightUpperBound * getNumberOfBins());
  }

protected:

  //! Compute the bin height for the given probability.
  /*!
    This is the same for each associated bin.
  */
  Number
  computeBinHeight(const int index) const {
    return _pmf[index] * _inverseSizes[index];
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
    _deviateIndices.resize(getNumberOfBins());
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
    \pre _pmfSum has been computed.
  */
  void
  packIntoBins() {
    _packIntoBins(Loki::Int2Type<ExactlyBalance>());
  }

private:

  //! Compute an upper bound on the bin height.
  void
  computeUpperBound() {
    _heightUpperBound = 0;
    for (int i = 0; i != _pmf.size(); ++i) {
      _heightUpperBound = std::max(_heightUpperBound, computeBinHeight(i));
    }
  }

  //! Pack the PMF's into bins.  Fast, approximate balancing.
  void
  _packIntoBins(Loki::Int2Type<false> /*ExactlyBalance*/);

  //! Pack the PMF's into bins.  Slower, exact balancing.
  void
  _packIntoBins(Loki::Int2Type<true> /*ExactlyBalance*/);

  //! Make a sorted array of the PMF.
  void
  computeSortedPmf() {
    for (int i = 0; i != _sortedPmf.size(); ++i) {
      _sortedPmf[i] = _pmf.begin() + i;
    }
    std::sort(_sortedPmf.begin(), _sortedPmf.end(), 
	      ads::constructLessByHandle
	      <typename ads::Array<1, Number>::const_iterator>());
  }

  //! Compute the inverse of the number of bins for each probability.
  void
  computeInverseSizes();
  
  //! Balance to minimize the maximum bin height.
  void
  balance();
  
  //! Trade bins to reduce the maximum bin height.
  bool
  tradeBins();
  
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






//! Discrete, finite deviate.  Bins with splitting.
/*!
  Dynamic specialization.
*/
template<bool ExactlyBalance, class BinConstants, class Generator>
class DiscreteFiniteGeneratorRejectionBinsSplitting
<true, ExactlyBalance, BinConstants, Generator> :
  public DiscreteFiniteGeneratorRejectionBinsSplitting
<false, ExactlyBalance, BinConstants, Generator>, 
  DfgRepairCounter<true> {
  //
  // Private types.
  //
private:

  //! The static version of this data structure.
  typedef DiscreteFiniteGeneratorRejectionBinsSplitting
  <false, ExactlyBalance, BinConstants, Generator> Base;
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
  //! The indices of the deviate in the bin.
  using Base::_deviateIndices;

  //! The sum of the PMF.
  using Base::_pmfSum;
  //! Probability mass function.  (This is scaled and may not sum to unity.)
  using Base::_pmf;

  //! The minimum allowed efficiency.
  Number _minimumEfficiency;
  //! The minimum allowed efficiency is the initial efficiency times this factor.
  Number _minimumEfficiencyFactor;
  
  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorRejectionBinsSplitting();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorRejectionBinsSplitting
  (DiscreteUniformGenerator* generator) :
    Base(generator),
    RepairBase(),
    _minimumEfficiency(-1),
    // CONTINUE
    _minimumEfficiencyFactor(0.75)
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorRejectionBinsSplitting
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    Base(generator),
    RepairBase(),
    _minimumEfficiency(-1),
    _minimumEfficiencyFactor(0.75) {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorRejectionBinsSplitting
  (const DiscreteFiniteGeneratorRejectionBinsSplitting& other) :
    Base(other),
    RepairBase(other),
    _minimumEfficiency(other._minimumEfficiency),
    _minimumEfficiencyFactor(other._minimumEfficiencyFactor)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorRejectionBinsSplitting&
  operator=(const DiscreteFiniteGeneratorRejectionBinsSplitting& other) {
    if (this != &other) {
      Base::operator=(other);
      RepairBase::operator=(other);
      _minimumEfficiency = other._minimumEfficiency;
      _minimumEfficiencyFactor = other._minimumEfficiencyFactor;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorRejectionBinsSplitting()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
public:

  //! Seed the uniform random number generator.
  using Base::seed;

  //! Return a discrete, finite deviate.
  result_type
  operator()() {
    if (computeEfficiency() < getMinimumEfficiency()) {
      rebuild();
    }
    return Base::operator()();
  }

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

  //! Compute the bin height for the given probability.
  using Base::computeBinHeight;

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
  setPmf(int index, Number value);

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
  updatePmf();

  //! Set the number of steps between repairs.
  using RepairBase::setStepsBetweenRepairs;

  //! Set the minimum allowed efficiency factor.
  void
  setMinimumEfficiencyFactor(const Number efficiency) const {
    _minimumEfficiencyFactor = efficiency;
  }

private:

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

#define __numerical_random_DiscreteFiniteGeneratorRejectionBinsSplitting_ipp__
#include "DiscreteFiniteGeneratorRejectionBinsSplitting.ipp"
#undef __numerical_random_DiscreteFiniteGeneratorRejectionBinsSplitting_ipp__

#endif
