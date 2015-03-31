// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgRepair.h
  \brief The repairing interface.
*/

#if !defined(__numerical_DfgRepair_h__)
#define __numerical_DfgRepair_h__

#include "../../defs.h"

#include "../../../ads/counter/CounterWithReset.h"

#include <iostream>
#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DfgRepair)
#define DEBUG_DfgRepair
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Counter for repairing the discrete, finite generator data structure.
/*!
  This is a base class for discrete, finite generators.  It provides the 
  interface functions for the repair counter.
*/
template<bool IsUsed = true>
class DfgRepairCounter;


//! Counter for repairing the discrete, finite generator data structure.
/*!
  This is a base class for discrete, finite generators.  It provides the 
  interface functions for the repair counter.
*/
template<>
class DfgRepairCounter<true> {
  //
  // Member data.
  //
private:

  //! The number of times you can set the PMF between repairs.
  ads::CounterWithReset<int> _counter;
  
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Construct from the number of steps between repairs.
  /*!
    By default set a suitable number of steps between repairs by assuming
    32-bit random integers and double precision real numbers.  Each floating
    point operation can introduce a relative round-off error of
    \f$\epsilon =\f$ std::numeric_limits<double>::epsilon().  
    If we assume that the errors accumulate in the worst way, \f$n\f$ 
    operations introduces a relative error of \f$n \epsilon\f$.  
    Since we need 32 bits of precision in our calculation, we can take
    \f$ 2^{-32} / \epsilon\f$ steps before repairing.
  */
  explicit
  DfgRepairCounter(const int stepsBetweenRepairs = 
		   int(1.0 / std::numeric_limits<unsigned>::max() /
			std::numeric_limits<double>::epsilon())) :
    _counter(stepsBetweenRepairs)
  {}

  //! Copy constructor.
  DfgRepairCounter(const DfgRepairCounter& other) :
    _counter(other._counter)
  {}

  //! Assignment operator.
  DfgRepairCounter&
  operator=(const DfgRepairCounter& other) {
    if (this != &other) {
      _counter = other._counter;
    }
    return *this;
  }

  //! Destructor.
  ~DfgRepairCounter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
protected:

  //! Return true if the data structure should be repaired.
  bool
  shouldRepair() const {
    return _counter() <= 0;
  }

public:

  //! Get the number of steps between repairs.
  int
  getStepsBetweenRepairs() const {
    return _counter.getReset();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgRepairCounter& other) const {
    return _counter == other._counter;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
protected:

  //! Reset the repair counter.
  void
  resetRepairCounter() {
    _counter.reset();
  }

  //! Decrement the repair counter.
  void
  decrementRepairCounter() {
    --_counter;
  }

  //! Decrement the repair counter by the specified amount.
  void
  decrementRepair(const int n) {
    _counter -= n;
  }

public:

  //! Set the number of steps between repairs.
  void
  setStepsBetweenRepairs(const int n) {
    assert(n > 0);
    _counter.setReset(n);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    out << "Steps between repairs = " << _counter.getReset() << "\n"
	<< "Steps until next repair = " << _counter() << "\n";
  }

  //@}
};




//! Counter for a discrete, finite generator that never needs repairing.
/*!
  This is a base class for discrete, finite generators.  It provides the 
  interface functions for the repair counter.
*/
template<>
class DfgRepairCounter<false> {
  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgRepairCounter()
  {}

  //! Construct from the number of steps between repairs.
  explicit
  DfgRepairCounter(const int stepsBetweenRepairs)
  {}

  //! Copy constructor.
  DfgRepairCounter(const DfgRepairCounter& other)
  {}

  //! Assignment operator.
  DfgRepairCounter&
  operator=(const DfgRepairCounter& other) {
    return *this;
  }

  //! Destructor.
  ~DfgRepairCounter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
protected:

  //! Return true if the data structure should be repaired.
  bool
  shouldRepair() const {
    return false;
  }

public:

  //! Get the number of steps between repairs.
  int
  getStepsBetweenRepairs() const {
    return std::numeric_limits<int>::max();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgRepairCounter& other) const {
    return true;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
protected:

  //! Reset the repair counter.
  void
  resetRepairCounter() {
  }

  //! Decrement the repair counter.
  void
  decrementRepairCounter() {
  }

  //! Decrement the repair counter by the specified amount.
  void
  decrementRepairCounter(const int n) {
  }

public:

  //! Set the number of steps between repairs.
  void
  setStepsBetweenRepairs(const int n) {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    out << "This data structure is never repaired.\n";
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
