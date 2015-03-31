// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorBinsSplittingStacking.h
  \brief Discrete, finite deviate.  BinsSplittingStacking.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorBinsSplittingStacking_h__)
#define __numerical_DiscreteFiniteGeneratorBinsSplittingStacking_h__

#include "DfgBinConstants.h"
#include "DfgRepair.h"
#include "DfgRebuild.h"

#include "../uniform/Default.h"

#include "../../../ads/array/Array.h"
#include "../../../ads/algorithm/sort.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorBinsSplittingStacking)
#define DEBUG_DiscreteFiniteGeneratorBinsSplittingStacking
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Discrete, finite deviate.  Bins with splitting and stacking.
/*!
  CONTINUE.
*/
template<bool IsDynamic = true,
	 bool UseImmediateUpdate = true,
	 class BinConstants = DfgBinConstantsDynamic<>,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class DiscreteFiniteGeneratorBinsSplittingStacking;



//! Discrete, finite deviate.  Bins with splitting and stacking.
/*!
  CONTINUE.
*/
template<bool UseImmediateUpdate, class BinConstants, class Generator>
class DiscreteFiniteGeneratorBinsSplittingStacking
<false, UseImmediateUpdate, BinConstants, Generator> :
  public BinConstants {
  //
  // Private types.
  //
private:

  //! The bin constants policy class.
  typedef BinConstants BinConstantsBase;

  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The number type.
  typedef typename BinConstantsBase::Number Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;

  //
  // More private types.
  //
private:

#if 0
  // CONTINUE: Using FixedArray's hurts performance.  I would not have 
  // expected that.
  typedef ads::FixedArray<NumberOfBins, Number> PmfBinContainer;
  typedef ads::FixedArray<NumberOfBins + 1, int> IndexBinContainer;
#else
  typedef ads::Array<1, Number> PmfBinContainer;
  typedef ads::Array<1, int> IndexBinContainer;
#endif

  //
  // Member data.
  //

protected:

  //! The discrete uniform generator.
  DiscreteUniformGenerator* _discreteUniformGenerator;

  //! An upper bound on the height of the bins.
  Number _heightUpperBound;
  //! The binned probability mass function.
  PmfBinContainer _binnedPmf;
  //! The indices of the first deviate in the bin.
  IndexBinContainer _deviateIndices;

  //! The sum of the PMF.
  Number _pmfSum;
  //! The end of the PMF's that are split across multiple bins.
  int _splittingEnd;
  //! Probability mass function.  (This is scaled and may not sum to unity.)
  ads::Array<1, Number> _pmf;
  //! The permutation of the probability mass function array.
  /*!
    This is useful when traversing the _pmf array.  We can efficiently go from
    the PMF value to its index.
  */
  ads::Array<1, int> _permutation;
  //! The rank of the elements in _pmf array.
  /*!
    This is useful for manipulating the _pmf array by index.  \c _pmf[rank[i]]
    is the i_th element in the original PMF array.
    
    The rank array is the inverse of the permutation array mapping.  That is,
    \c _rank[_permutation[i]]==i and \c _permutation[_rank[i]]==i .
  */
  ads::Array<1, int> _rank;
  //! The index of the first bin containing the PMF.
  ads::Array<1, int> _binIndices;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorBinsSplittingStacking();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorBinsSplittingStacking
  (DiscreteUniformGenerator* generator) :
    BinConstantsBase(),
    _discreteUniformGenerator(generator),
    _heightUpperBound(-1),
#if 0
    _binnedPmf(),
    _deviateIndices(),
#else
    _binnedPmf(getNumberOfBins()),
    _deviateIndices(getNumberOfBins() + 1),
#endif
    _pmfSum(-1),
    _splittingEnd(-1),
    _pmf(),
    _permutation(),
    _rank(),
    _binIndices()
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorBinsSplittingStacking
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    BinConstantsBase(),
    _discreteUniformGenerator(generator),
    _heightUpperBound(-1),
#if 0
    _binnedPmf(),
    _deviateIndices(),
#else
    _binnedPmf(getNumberOfBins()),
    _deviateIndices(getNumberOfBins() + 1),
#endif
    _pmfSum(-1),
    _splittingEnd(-1),
    _pmf(),
    _permutation(),
    _rank(),
    _binIndices() {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplittingStacking
  (const DiscreteFiniteGeneratorBinsSplittingStacking& other) :
    BinConstantsBase(other),
    _discreteUniformGenerator(other._discreteUniformGenerator),
    _heightUpperBound(other._heightUpperBound),
    _binnedPmf(other._binnedPmf),
    _deviateIndices(other._deviateIndices),
    _pmfSum(other._pmfSum),
    _splittingEnd(other._splittingEnd),
    _pmf(other._pmf),
    _permutation(other._permutation),
    _rank(other._rank),
    _binIndices(other._binIndices)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplittingStacking&
  operator=(const DiscreteFiniteGeneratorBinsSplittingStacking& other) {
    if (this != &other) {
      BinConstantsBase::operator=(other);
      _discreteUniformGenerator = other._discreteUniformGenerator;
      _heightUpperBound = other._heightUpperBound;
      _binnedPmf = other._binnedPmf;
      _deviateIndices = other._deviateIndices;
      _pmfSum = other._pmfSum;
      _splittingEnd = other._splittingEnd;
      _pmf = other._pmf;
      _permutation = other._permutation;
      _rank = other._rank;
      _binIndices = other._binIndices;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorBinsSplittingStacking()
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
    return _pmf[_rank[index]];
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

  //! Fix the PMF in the specified bin.
  void
  fixBin(int binIndex);

  //! Update the data structure by recomputing the sum of the PMF's.
  void
  computePmfSum() {
    _pmfSum = std::accumulate(_pmf.begin(), _pmf.end(), 0.0);
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

  //--------------------------------------------------------------------------
  // Private member functions.
private:

  //! Pack the PMF's into bins.
  /*!
    \pre The PMF must be sorted in descending order.
  */
  void
  packIntoBins();

  //--------------------------------------------------------------------------
  // Inherited from the bin constants policy class.

  //! The number of bits used for indexing the bins.
  using BinConstantsBase::getIndexBits;

  //! The number of bins. 2^IndexBits.
  using BinConstantsBase::getNumberOfBins;

  //! The mask for extracting the index.  
  using BinConstantsBase::getIndexMask;

  //! The inverse of the maximum height.  1 / (2^(32-_indexBits) - 1).
  using BinConstantsBase::getMaxHeightInverse;
};






//! Discrete, finite deviate.  Bins with splitting and stacking.
/*!
  CONTINUE.
*/
template<bool UseImmediateUpdate, class BinConstants, class Generator>
class DiscreteFiniteGeneratorBinsSplittingStacking
<true, UseImmediateUpdate, BinConstants, Generator> :
  public DiscreteFiniteGeneratorBinsSplittingStacking
<false, UseImmediateUpdate, BinConstants, Generator>, 
  DfgRepairCounter<true>, DfgRebuildCounter<true> {
  //
  // Private types.
  //
private:

  //! The bin constants policy class.
  typedef DiscreteFiniteGeneratorBinsSplittingStacking
  <false, UseImmediateUpdate, BinConstants, Generator> Base;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<true> RepairBase;
  //! The interface for rebuilding the data structure.
  typedef DfgRebuildCounter<true> RebuildBase;

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
  //! The indices of the first deviate in the bin.
  //using Base::_deviateIndices;

  //! The sum of the PMF.
  using Base::_pmfSum;
  //! The end of the PMF's that are split across multiple bins.
  using Base::_splittingEnd;
  //! Probability mass function.  (This is scaled and may not sum to unity.)
  using Base::_pmf;
  //! The permutation of the probability mass function array.
  //using Base::_permutation;
  //! The rank of the elements in _pmf array.
  using Base::_rank;
  //! The index of the first bin containing the PMF.
  using Base::_binIndices;

  //! The target efficiency when rebuilding the data structure.
  Number _targetEfficiency;
  //! The minimum allowed efficiency.
  Number _minimumEfficiency;
  
  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorBinsSplittingStacking();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorBinsSplittingStacking
  (DiscreteUniformGenerator* generator) :
    Base(generator),
    RepairBase(),
    // By default, take 1000 steps between rebuilds.
    RebuildBase(1000L),
    _targetEfficiency(0.75),
    _minimumEfficiency(0.25)
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorBinsSplittingStacking
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    Base(generator),
    RepairBase(),
    // By default, take 1000 steps between rebuilds.
    RebuildBase(1000L),
    _targetEfficiency(0.75),
    _minimumEfficiency(0.25) {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplittingStacking
  (const DiscreteFiniteGeneratorBinsSplittingStacking& other) :
    Base(other),
    RepairBase(other),
    RebuildBase(other),
    _targetEfficiency(other._targetEfficiency),
    _minimumEfficiency(other._minimumEfficiency)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinsSplittingStacking&
  operator=(const DiscreteFiniteGeneratorBinsSplittingStacking& other) {
    if (this != &other) {
      Base::operator=(other);
      RepairBase::operator=(other);
      RebuildBase::operator=(other);
      _targetEfficiency = other._targetEfficiency;
      _minimumEfficiency = other._minimumEfficiency;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorBinsSplittingStacking()
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

  //! Get the target efficiency.
  /*!
    Rebuilding is only performed if the efficiency falls below this threshhold.
  */
  Number
  getTargetEfficiency() const {
    return _targetEfficiency;
  }

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

  //! Set the target efficiency.
  /*!
    Rebuilding is only performed if the efficiency falls below this threshhold.
    Usually set it to a number between 0.5 and 1.
  */
  void
  setTargetEfficiency(const Number efficiency) const {
    _targetEfficiency = efficiency;
  }

  //! Set the minimum allowed efficiency.
  void
  setMinimumEfficiency(const Number efficiency) const {
    _minimumEfficiency = efficiency;
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
    _pmf[_rank[index]] = value;

    decrementRebuildCounter();
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

  //! Fix the PMF in the specified bin.
  using Base::fixBin;

  //! Rebuild the bins.
  void
  rebuild();

  //! Pack the PMF's into bins.
  /*!
    \pre The PMF must be sorted in descending order.
  */
  void
  packIntoBins();

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

#define __numerical_random_DiscreteFiniteGeneratorBinsSplittingStacking_ipp__
#include "DiscreteFiniteGeneratorBinsSplittingStacking.ipp"
#undef __numerical_random_DiscreteFiniteGeneratorBinsSplittingStacking_ipp__

#endif
