// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf.h
  \brief Discrete, finite deviate.  Binary search.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf_h__)
#define __numerical_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf_h__

#include "DfgPmf.h"
#include "DfgRepair.h"

#include "../uniform/ContinuousUniformGenerator.h"

#include "../../../ads/array/Array.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf)
#define DEBUG_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
#endif

BEGIN_NAMESPACE_NUMERICAL

// CONTINUE: Use linear search for a certain number of bits.
//! Discrete, finite deviate generator.  CDF inversion using a partial, recursive CDF.
/*!
  \param Generator The discrete, uniform generator.
  \param T The number type.  By default it is double.

  CONTINUE.
*/
template<bool UseImmediateUpdate = true,
	 class Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT,
	 typename T = double>
class DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf :
  public DfgPmf<T>, DfgRepairCounter<UseImmediateUpdate> {
  //
  // Private types.
  //
private:

  //! The interface for the probability mass function.
  typedef DfgPmf<T> PmfBase;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<UseImmediateUpdate> RepairBase;

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
private:

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;
  //! Cumulative distribution function.  (This is scaled and may not approach unity.)
  ads::Array<1, Number> _partialRecursiveCdf;
  //! The number of bits needed to index into the _partialRecursiveCdf array.
  int _indexBits;
#if 0
  int _numberOfNonzeroProbabilities;
#endif

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
  (DiscreteUniformGenerator* generator) :
    PmfBase(),
    RepairBase(),
    _continuousUniformGenerator(generator),
    _partialRecursiveCdf(),
    _indexBits(-1)
#if 0
    ,_numberOfNonzeroProbabilities(0)
#endif
  {}

  //! Construct from the uniform generator and the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    PmfBase(),
    RepairBase(),
    _continuousUniformGenerator(generator),
    _partialRecursiveCdf(),
    _indexBits(-1)
#if 0
    ,_numberOfNonzeroProbabilities(0) 
#endif
  {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
  (const DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf& other) :
    PmfBase(other),
    RepairBase(other),
    _continuousUniformGenerator(other._continuousUniformGenerator),
    _partialRecursiveCdf(other._partialRecursiveCdf),
    _indexBits(other._indexBits)
#if 0
    ,_numberOfNonzeroProbabilities(other._numberOfNonzeroProbabilities)
#endif
  {}

  //! Assignment operator.
  /*!
    \note The discrete,uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf&
  operator=
  (const DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf& other) {
    if (this != &other) {
      PmfBase::operator=(other);
      RepairBase::operator=(other);
      _continuousUniformGenerator = other._continuousUniformGenerator;
      _partialRecursiveCdf = other._partialRecursiveCdf;
      _indexBits = other._indexBits;
#if 0
      _numberOfNonzeroProbabilities = other._numberOfNonzeroProbabilities;
#endif
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf()
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
  operator()();

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
  /*!
    \pre The CDF must be computed before calling this function.
  */
  Number
  getPmfSum() const {
    return *(_partialRecursiveCdf.end() - 1);
  }

  //! Return true if the sum of the PMF is positive.
  bool
  isValid() const {
    return getPmfSum() > 0;
  }

  //! Get the number of steps between repairs.
  using RepairBase::getStepsBetweenRepairs;

private:

  //! Get the beginning of the probabilities in the PMF.
  using PmfBase::getPmfBeginning;

  //! Get the end of the probabilities in the PMF.
  using PmfBase::getPmfEnd;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    // Initialize the PMF.
    PmfBase::initialize(begin, end);
    // Compute the array size for the partial, recursive CDF.
    int size = 1;
    _indexBits = 0;
    while (size < getSize()) {
      size *= 2;
      ++_indexBits;
    }
    _partialRecursiveCdf.resize(size);

#if 0
    // Count the number of nonzero probabilities.
    _numberOfNonzeroProbabilities = 0;
    for (typename PmfBase::ConstIterator i = getPmfBeginning(); 
	 i != getPmfEnd(); ++i) {
      _numberOfNonzeroProbabilities += (*i != 0);
    }
#endif

    repair();
  }

  //! Set the probability mass function with the specified index.
  /*!
    \note After calling this function, you must call updatePmf() before
    computing deviates.
  */
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

  //! Update the data structure following calls to setPmf() .
  /*!
    Check if the data structure needs repairing.
  */
  void
  updatePmf() {
    updatePmf(Loki::Int2Type<UseImmediateUpdate>());
  }

  //! Set the number of steps between repairs.
  using RepairBase::setStepsBetweenRepairs;

private:

  //! Set the probability mass function with the specified index.
  /*!
    Update the partial, recursive CDF using the difference between the new and 
    old values.
  */
  void
  setPmf(int index, Number value, Loki::Int2Type<true> /*dummy*/) {
    // If the PMF has become zero.  (It was nonzero before.)
    if (value == 0 && getPmf(index) != 0) {
#if 0
      const Number difference = - getPmf(index);
#endif
      PmfBase::setPmf(index, 0);
      // We need to recompute the sum of the PMF.  It may have become zero.
      repair();
#if 0
      --_numberOfNonzeroProbabilities;
      if (_numberOfNonzeroProbabilities == 0) {
	// We need to recompute the sum of the PMF.  It may have become zero.
	repair();
      }
      else {
	// Update the partial, recursive CDF.
	updateCdf(index, difference);
	RepairBase::decrementRepairCounter();
      }
#endif
    }
    // The standard case.
    else {
#if 0
      if (value != 0 && getPmf(index) == 0) {
	++_numberOfNonzeroProbabilities;
      }
#endif
      const Number difference = value - getPmf(index);
      PmfBase::setPmf(index, value);
      // Update the partial, recursive CDF.
      updateCdf(index, difference);
      RepairBase::decrementRepairCounter();
    }
  }
  
  //! Set the probability mass function with the specified index.
  /*!
    Do not update the partial, recursive CDF.
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

  //! Repair the partial, recursive CDF.
  void
  repair();

  //! Update the CDF.
  void
  updateCdf(int index, const Number difference);

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    PmfBase::print(out);
    out << "Partial, recursive CDF = \n" << _partialRecursiveCdf << "\n"
	<< "Index bits = " << _indexBits << "\n";
  }

  //@}
};



END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf_ipp__
#include "DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf.ipp"
#undef __numerical_random_DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf_ipp__

#endif
