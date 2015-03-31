// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorBinarySearch.h
  \brief Discrete, finite deviate.  Binary search.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorBinarySearch_h__)
#define __numerical_DiscreteFiniteGeneratorBinarySearch_h__

#include "DfgPmfSortedByMaxInfluencingProbabilityAscending.h"
#include "DfgRepair.h"

#include "../uniform/ContinuousUniformGenerator.h"

#include "../../../ads/array/Array.h"

#include "../../../third-party/loki/TypeManip.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorBinarySearch)
#define DEBUG_DiscreteFiniteGeneratorBinarySearch
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Discrete, finite deviate.  Binary search.
/*!
  \param T The number type.  By default it is double.

  CONTINUE.
*/
template<bool UseReactionInfluence = false,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 typename T = double>
class DiscreteFiniteGeneratorBinarySearch;



//! Discrete, finite deviate.  Binary search.
/*!
  \param T The number type.  By default it is double.

  \note This class stores a pointer to the influence array.  It is the 
  user's responsibility to hold that array until after this class is 
  destructed.

  CONTINUE.
*/
template<class Generator, typename T>
class DiscreteFiniteGeneratorBinarySearch<true, Generator, T> :
  public DfgPmfSortedByMaxInfluencingProbabilityAscending
<DfgPmf<TraitsForBranching<true>, T>, DfgRebuildCounter<true> >,
   DfgRepairCounter<false> {
  //
  // Private types.
  //
private:

  typedef DfgPmfSortedByMaxInfluencingProbabilityAscending
  <DfgPmf<TraitsForBranching<true>, T>, DfgRebuildCounter<true> > Base;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<false> RepairBase;

  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The continuous uniform generator.
  typedef ContinuousUniformGeneratorClosed<DiscreteUniformGenerator>
  ContinuousUniformGenerator;
  //! The number type.
  typedef typename Base::Number Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;

  //
  // Member data.
  //
private:

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;
  //! Cumulative distribution function.  (This is scaled and may not approach unity.)
  ads::Array<1, Number> _cdf;
  //! The modified probability with the lowest index.
  int _firstModifiedProbability;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorBinarySearch();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorBinarySearch(DiscreteUniformGenerator* generator) :
    Base(),
    RepairBase(),
    _continuousUniformGenerator(generator),
    _cdf(),
    _firstModifiedProbability(getSize())
  {}

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinarySearch
  (const DiscreteFiniteGeneratorBinarySearch& other) :
    Base(other),
    RepairBase(other),
    _continuousUniformGenerator(other._continuousUniformGenerator),
    _cdf(other._cdf),
    _firstModifiedProbability(other._firstModifiedProbability)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinarySearch&
  operator=(const DiscreteFiniteGeneratorBinarySearch& other) {
    if (this != &other) {
      Base::operator=(other);
      RepairBase::operator=(other);
      _continuousUniformGenerator = other._continuousUniformGenerator;
      _cdf = other._cdf;
      _firstModifiedProbability = other._firstModifiedProbability;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorBinarySearch()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _continuousUniformGenerator.seed(seedValue);
  }

  //! Return a discrete, finite deviate.
  /*!
    Use a binary search on the CDF.
  */
  result_type
  operator()() {
    return std::lower_bound
      (_cdf.begin(), _cdf.end(), 
       _continuousUniformGenerator() * *(_cdf.end() - 1)) - _cdf.begin();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Get the probability mass function with the specified index.
  using Base::getPmf;

  //! Get the number of possible deviates.
  using Base::getSize;

  //! Get the sum of the probability mass functions.
  /*!
    \pre The CDF must be computed before calling this function.
  */
  Number
  getPmfSum() const {
    return *(_cdf.end() - 1);
  }

  //! Return true if the sum of the PMF is positive.
  bool
  isValid() const {
    return getPmfSum() > 0;
  }

  //! Get the number of steps between repairs.
  using RepairBase::getStepsBetweenRepairs;

  //! Get the number of steps between rebuilds.
  using Base::getStepsBetweenRebuilds;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    Base::initialize(begin, end);
    _cdf.resize(getSize());
    _firstModifiedProbability = 0;
    updatePmf();
  }

  //! Set the probability mass function with the specified index.
  /*!
    \note After calling this function, you must call updatePmf() before
    computing deviates.
  */
  void
  setPmf(int index, Number value) {
    Base::setPmf(index, value);
    if (index <  _firstModifiedProbability) {
      _firstModifiedProbability = index;
    }
  }

  //! Update the data structure following calls to setPmfWithoutUpdating() .
  /*!
    This will update the CDF.
  */
  void
  updatePmf();

  //! Set the number of steps between repairs.
  using RepairBase::setStepsBetweenRepairs;

  //! Set the number of steps between rebuilds.
  using Base::setStepsBetweenRebuilds;

  //! Set the influence array.
  using Base::setInfluence;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    Base::print(out);
    out << "CDF = \n" << _cdf << "\n"
	<< "First modified probability = " << _firstModifiedProbability << "\n";
  }

  //@}
};




//! Discrete, finite deviate.  Binary search.
/*!
  \param T The number type.  By default it is double.

  CONTINUE.
*/
template<class Generator, typename T>
class DiscreteFiniteGeneratorBinarySearch<false, Generator, T> :
  public DfgPmf<T>, DfgRepairCounter<false> {
  //
  // Private types.
  //
private:

  typedef DfgPmf<T> Base;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<false> RepairBase;

  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef Generator DiscreteUniformGenerator;
  //! The continuous uniform generator.
  typedef ContinuousUniformGeneratorClosed<DiscreteUniformGenerator>
  ContinuousUniformGenerator;
  //! The number type.
  typedef typename Base::Number Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;

  //
  // Member data.
  //
private:

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;
  //! Cumulative distribution function.  (This is scaled and may not approach unity.)
  ads::Array<1, Number> _cdf;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorBinarySearch();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorBinarySearch(DiscreteUniformGenerator* generator) :
    Base(),
    _continuousUniformGenerator(generator),
    _cdf()
  {}

  //! Construct from the uniform generator and the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorBinarySearch(DiscreteUniformGenerator* generator,
				      ForwardIterator begin, 
				      ForwardIterator end) :
    Base(),
    _continuousUniformGenerator(generator),
    _cdf() {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinarySearch
  (const DiscreteFiniteGeneratorBinarySearch& other) :
    Base(other),
    _continuousUniformGenerator(other._continuousUniformGenerator),
    _cdf(other._cdf)
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorBinarySearch&
  operator=(const DiscreteFiniteGeneratorBinarySearch& other) {
    if (this != &other) {
      Base::operator=(other);
      _continuousUniformGenerator = other._continuousUniformGenerator;
      _cdf = other._cdf;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorBinarySearch()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _continuousUniformGenerator.seed(seedValue);
  }

  //! Return a discrete, finite deviate.
  result_type
  operator()() {
    return std::lower_bound
      (_cdf.begin(), _cdf.end(), 
       _continuousUniformGenerator() * *(_cdf.end() - 1)) - _cdf.begin();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{

  //! Get the probability mass function with the specified index.
  using Base::getPmf;

  //! Get the number of possible deviates.
  using Base::getSize;

  //! Get the sum of the probability mass functions.
  /*!
    \pre The CDF must be computed before calling this function.
  */
  Number
  getPmfSum() const {
    return *(_cdf.end() - 1);
  }

  //! Return true if the sum of the PMF is positive.
  bool
  isValid() const {
    return getPmfSum() > 0;
  }

  //! Get the number of steps between repairs.
  using RepairBase::getStepsBetweenRepairs;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    Base::initialize(begin, end);
    _cdf.resize(getSize());
    updatePmf();
  }

  //! Set the probability mass function with the specified index.
  /*!
    \note After calling this function, you must call updatePmf() before
    computing deviates.
  */
  using Base::setPmf;

  //! Update the data structure following calls to setPmf() .
  /*!
    This will update the CDF.
  */
  void
  updatePmf() {
    // Compute the cumulative distribution function.
    std::partial_sum(Base::getPmfBeginning(), Base::getPmfEnd(), _cdf.begin());
  }

  //! Set the number of steps between repairs.
  using RepairBase::setStepsBetweenRepairs;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    Base::print(out);
    out << "CDF = " << _cdf << "\n";
  }

  //@}
};



END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteFiniteGeneratorBinarySearch_ipp__
#include "DiscreteFiniteGeneratorBinarySearch.ipp"
#undef __numerical_random_DiscreteFiniteGeneratorBinarySearch_ipp__

#endif
