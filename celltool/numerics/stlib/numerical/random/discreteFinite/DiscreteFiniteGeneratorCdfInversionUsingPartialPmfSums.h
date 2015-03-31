// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums.h
  \brief Discrete, finite deviate.  CDF inversion using a partial sums of the PMF.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums_h__)
#define __numerical_DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums_h__

#include "DfgPmfWithGuard.h"
#include "DfgRepair.h"

#include "../uniform/ContinuousUniformGenerator.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums)
#define DEBUG_DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums
#endif

BEGIN_NAMESPACE_NUMERICAL

//! The number of elements per partial sum.
template<int Value>
class DfgElementsPerPartialSum {
protected:
  //! The number of elements per partial sum.
  static const int _value = Value;

  //! Do nothing.  The value is set by the template parameter.
  void
  initialize(const int size) {
  }
};

//! The number of elements per partial sum.
/*! 
  Specialization implements default value.
*/
template<>
class DfgElementsPerPartialSum<0> {
protected:
  //! The number of elements per partial sum.
  int _value;

  //! Set the value to the square root of the size.
  void
  initialize(const int size) {
    _value = int(std::sqrt(double(size)));
  }
};

//! Discrete, finite deviate.  CDF inversion using a partial sums of the PMF.
/*!
  \param UseImmediateUpdate Specifies the policy for updating the data
  structure when setting the PMF.
  \param Pmf is the policy class that handles the probability mass function.
  By default it is DfgPmfWithGuard .
  The \c Number type is inherited from this class.
  Because the different policies have different template parameters, this
  is a concrete class, and not a template template.
  \param Generator is the discrete, uniform generator.

  If UseImmediateUpdate is true, the data structure is updated each time 
  the PMF is modified with setPmf() .
*/
template<int ElementsPerPartialSum = 0,
	 bool UseImmediateUpdate = true,
	 class Pmf = DfgPmfWithGuard<>,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums :
  public Pmf, DfgRepairCounter<UseImmediateUpdate>, 
  DfgElementsPerPartialSum<ElementsPerPartialSum> {
  //
  // Private types.
  //
private:

  //! The interface for the probability mass function.
  typedef Pmf PmfBase;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<UseImmediateUpdate> RepairBase;
  //! The interface for managing the number of elements per partial sum.
  typedef DfgElementsPerPartialSum<ElementsPerPartialSum> 
  ElementsPerPartialSumBase;

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
  typedef typename PmfBase::Number Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;

  //
  // Member data.
  //
protected:

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;
  //! The sum of the PMF.
  Number _pmfSum;
  //! Partial sums of the PMF.
  ads::Array<1, Number> _partialPmfSums;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums
  (DiscreteUniformGenerator* generator) :
    // The PMF array is empty.
    PmfBase(),
    RepairBase(),
    ElementsPerPartialSumBase(),
    // Make a continuous uniform generator using the discrete uniform generator.
    _continuousUniformGenerator(generator),
    // Invalid value for the sum.
    _pmfSum(-1),
    // Empty array.
    _partialPmfSums()
  {}

  //! Construct from the uniform generator and the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    PmfBase(),
    RepairBase(),
    ElementsPerPartialSumBase(),
    // Make a continuous uniform generator using the discrete uniform generator.
    _continuousUniformGenerator(generator),
    // Invalid value for the sum.
    _pmfSum(-1),
    _partialPmfSums() {
    // Allocate the arrays and initialize the data structure.
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums
  (const DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums& other) :
    PmfBase(other),
    RepairBase(other),
    ElementsPerPartialSumBase(other),
    _continuousUniformGenerator(other._continuousUniformGenerator),
    _pmfSum(other._pmfSum),
    _partialPmfSums(other._partialPmfSums)
  {}

  //! Assignment operator.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums&
  operator=
  (const DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums& other) {
    if (this != &other) {
      PmfBase::operator=(other);
      RepairBase::operator=(other);
      ElementsPerPartialSumBase::operator=(other);
      _continuousUniformGenerator = other._continuousUniformGenerator;
      _pmfSum = other._pmfSum;
      _partialPmfSums = other._partialPmfSums;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
public:

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _continuousUniformGenerator.seed(seedValue);
  }

  //! Return a discrete, finite deviate.
  result_type
  operator()() {
    // A random weighted probability.
    Number r = _continuousUniformGenerator() * _pmfSum;
    // Use the partial PMF sums to step forward.
    typename ads::Array<1, Number>::const_iterator i = _partialPmfSums.begin();
    while (r >= *i) {
      r -= *i;
      ++i;
    }
    // Use a linear search from the offset to finish the search.
    return PmfBase::operator()(r, ElementsPerPartialSumBase::_value * 
			       (i - _partialPmfSums.begin()));
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  using PmfBase::getPmf;

  //! Get the number of possible deviates.
  using PmfBase::getSize;

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

  //! Get the number of steps between repairs.
  using RepairBase::getStepsBetweenRepairs;

  //! Get the number of steps between rebuilds.
  /*!
    \note You can use this only if the PMF base class utilizes rebuilding.
  */
  int
  getStepsBetweenRebuilds() const {
    return PmfBase::getStepsBetweenRebuilds();
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
    PmfBase::initialize(begin, end);
    // Initialize the partial sum size.
    ElementsPerPartialSumBase::initialize(getSize());
    // Allocate the array.  Include the guard element.
    _partialPmfSums.resize(getSize() / ElementsPerPartialSumBase::_value + 1);
    // Compute the partial sums.
    repair();
  }

  //! Set the probability mass function with the specified index.
  void
  setPmf(int index, Number value) {
    setPmf(index, value, Loki::Int2Type<UseImmediateUpdate>());
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  setPmf(_RandomAccessIterator iterator) {
    for (int i = 0; i != getSize(); ++i) {
      setPmf(i, iterator[i]);
    }
  }

  //! Update the data structure after calls to setPmf() .
  void
  updatePmf() {
    updatePmf(Loki::Int2Type<UseImmediateUpdate>());
  }

  //! Set the number of steps between repairs.
  using RepairBase::setStepsBetweenRepairs;

  //! Set the number of steps between rebuilds.
  /*!
    \note You can use this only if the PMF base class utilizes rebuilding.
  */
  void
  setStepsBetweenRebuilds(const int n) {
    PmfBase::setStepsBetweenRebuilds(n);
  }

private:

  //! Set the probability mass function with the specified index.
  /*!
    Update the partial sums and the total sum of the PMF using the difference
    between the new and old values.
  */
  void
  setPmf(int index, Number value, Loki::Int2Type<true> /*dummy*/) {
    // If the PMF has become zero.  (It was nonzero before.)
    if (value == 0 && getPmf(index) != 0) {
      PmfBase::setPmf(index, 0);
      // We need to recompute the sum of the PMF.  It may have become zero.
      repair();
    }
    // The standard case.
    else {
      const Number difference = value - getPmf(index);
      PmfBase::setPmf(index, value);
      _partialPmfSums[index / ElementsPerPartialSumBase::_value] += difference;
      _pmfSum += difference;
      RepairBase::decrementRepairCounter();
    }
  }
  
  //! Set the probability mass function with the specified index.
  /*!
    Do not update the sum of the PMF.
  */
  void
  setPmf(int index, Number value, Loki::Int2Type<false> /*dummy*/) {
    PmfBase::setPmf(index, value);
  }

  //! Check if the data structure needs repair.
  /*! 
    This data structure continuously updates the sum of the PMF so we don't 
    need to do that here.
  */
  void
  updatePmf(Loki::Int2Type<true> /*dummy*/) {
    PmfBase::updatePmf();
    if (RepairBase::shouldRepair()) {
      repair();
    }
  }

  //! Recompute the sum of the PMF.
  void
  updatePmf(Loki::Int2Type<false> /*dummy*/) {
    PmfBase::updatePmf();
    repair();
  }

  //! Repair the data structure.
  /*!
    Recompute the sum of the PMF.
  */
  void
  repair() {
    // Recompute the partial sums.
    int pmfIndex = 0;
    for (int i = 0; i != _partialPmfSums.size() - 1; ++i) {
      _partialPmfSums[i] = 0;
      // The compiler can unroll this loop.
      for (int j = 0; j != ElementsPerPartialSumBase::_value; ++j) {
	_partialPmfSums[i] += getPmf(pmfIndex++);
      }
    }
    // The guard element for searching.
    *(_partialPmfSums.end() - 1) = 0.5 * std::numeric_limits<Number>::max();

    // Recompute the total sum.  Use the partial sums and the remaining 
    // elements.
    _pmfSum = std::accumulate(_partialPmfSums.begin(),
			      _partialPmfSums.end() - 1, Number(0)) +
      std::accumulate(PmfBase::getPmfBeginning() + pmfIndex, 
		      PmfBase::getPmfEnd(), Number(0));

    RepairBase::resetRepairCounter();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    PmfBase::print(out);
    RepairBase::print(out);
    out << "Elements per partial sum = " 
	<< ElementsPerPartialSumBase::_value << "\n"
	<< "PMF sum = " << _pmfSum << "\n"
	<< "Partial sums = \n" << _partialPmfSums;
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
