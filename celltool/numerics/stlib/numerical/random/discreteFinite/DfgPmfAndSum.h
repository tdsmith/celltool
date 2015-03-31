// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfAndSum.h
  \brief PMF and its sum.
*/

#if !defined(__numerical_DfgPmfAndSum_h__)
#define __numerical_DfgPmfAndSum_h__

#include "DfgPmfWithGuard.h"
#include "DfgRepair.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfAndSum)
#define DEBUG_numerical_DfgPmfAndSum
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Whether to use immediate updating.
template<bool _UseImmediateUpdate>
class TraitsForImmediateUpdate {
public:
  //! Whether to use branching.
  static const bool UseImmediateUpdate = _UseImmediateUpdate;
};


//! PMF and its sum.
/*!
  \param Pmf is the policy class that handles the probability mass function.
  By default it is DfgPmfWithGuard<> .
  The \c Number type is inherited from this class.
  Because the different policies have different template parameters, this
  is a concrete class, and not a template template.
  \param Traits Specifies the policy for updating the sum
  when setting the PMF.

  If immediate updating is used, the sum of the PMF is updated each time 
  the PMF is modified with setPmf() .
*/
template<class Pmf = DfgPmfWithGuard<>,
	 class Traits = TraitsForImmediateUpdate<true> >
class DfgPmfAndSum :
  public Pmf, DfgRepairCounter<Traits::UseImmediateUpdate> {
  //
  // Private types.
  //
private:

  //! The interface for the probability mass function.
  typedef Pmf PmfBase;
  //! The interface for repairing the data structure.
  typedef DfgRepairCounter<Traits::UseImmediateUpdate> RepairBase;
  
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

  //! The sum of the PMF.
  Number _pmfSum;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmfAndSum() :
    PmfBase(),
    RepairBase(),
    // Invalid value for the sum.
    _pmfSum(-1)
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DfgPmfAndSum(ForwardIterator begin, ForwardIterator end) :
    PmfBase(),
    RepairBase(),
    // Invalid value for the sum.
    _pmfSum(-1) {
    initialize(begin, end);
  }

  //! Copy constructor.
  DfgPmfAndSum(const DfgPmfAndSum& other) :
    PmfBase(other),
    RepairBase(other),
    _pmfSum(other._pmfSum)
  {}

  //! Assignment operator.
  DfgPmfAndSum&
  operator=(const DfgPmfAndSum& other) {
    if (this != &other) {
      PmfBase::operator=(other);
      RepairBase::operator=(other);
      _pmfSum = other._pmfSum;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfAndSum()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
protected:

  //! Return a discrete, finite deviate.
  using PmfBase::operator();

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

protected:

  //! Get the beginning of the probabilities in the PMF.
  using PmfBase::getPmfBeginning;

  //! Get the end of the probabilities in the PMF.
  using PmfBase::getPmfEnd;

  //! Get the index of the specified element.
  using PmfBase::getIndex;

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgPmfAndSum& other) const {
    return PmfBase::operator==(other) && _pmfSum == other._pmfSum;
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
    repair();
  }

  //! Set the probability mass function with the specified index.
  /*!
    Update the sum of the PMF using the difference between the new and 
    old values.
  */
  void
  setPmf(const int index, const Number value) {
    _setPmf(index, value, Loki::Int2Type<Traits::UseImmediateUpdate>());
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  setPmf(_RandomAccessIterator iterator) {
    _setPmf(iterator, Loki::Int2Type<Traits::UseImmediateUpdate>());
  }

  //! Recompute the sum of the PMF.
  void
  updatePmf() {
    _updatePmf(Loki::Int2Type<Traits::UseImmediateUpdate>());
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
    Update the sum of the PMF using the difference between the new and 
    old values.
  */
  void
  _setPmf(const int index, const Number value, Loki::Int2Type<true> /*dummy*/) {
    // If the PMF has become zero.  (It was nonzero before.)
    if (value == 0 && getPmf(index) != 0) {
      PmfBase::setPmf(index, 0);
      // We need to recompute the sum of the PMF.  It may have become zero.
      repair();
    }
    // The standard case.
    else {
      _pmfSum += value - getPmf(index);
      PmfBase::setPmf(index, value);
      RepairBase::decrementRepairCounter();
    }
  }
  
  //! Set the probability mass function with the specified index.
  /*!
    Do not update the sum of the PMF.
  */
  void
  _setPmf(const int index, const Number value, 
	  Loki::Int2Type<false> /*dummy*/) {
    PmfBase::setPmf(index, value);
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  _setPmf(_RandomAccessIterator iterator, Loki::Int2Type<true> /*dummy*/) {
    for (int i = 0; i != getSize(); ++i) {
      setPmf(i, iterator[i]);
    }
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  _setPmf(_RandomAccessIterator iterator, Loki::Int2Type<false> /*dummy*/) {
    for (int i = 0; i != getSize(); ++i) {
      PmfBase::setPmf(i, iterator[i]);
    }
  }

  //! Check if the data structure needs repair.
  /*! 
    This data structure continuously updates the sum of the PMF so we don't 
    need to do that here.
  */
  void
  _updatePmf(Loki::Int2Type<true> /*dummy*/) {
    PmfBase::updatePmf();
    if (RepairBase::shouldRepair()) {
      repair();
    }
  }

  //! Recompute the sum of the PMF.
  void
  _updatePmf(Loki::Int2Type<false> /*dummy*/) {
    PmfBase::updatePmf();
    repair();
  }

  //! Repair the data structure.
  /*!
    Recompute the sum of the PMF.
  */
  void
  repair() {
    _pmfSum = std::accumulate(PmfBase::getPmfBeginning(), PmfBase::getPmfEnd(),
			      0.0);
    RepairBase::resetRepairCounter();
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
    RepairBase::print(out);
    out << "PMF sum = " << _pmfSum << "\n";
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
