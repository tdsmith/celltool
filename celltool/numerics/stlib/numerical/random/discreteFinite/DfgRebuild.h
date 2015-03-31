// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgRebuild.h
  \brief The rebuilding interface.
*/

#if !defined(__numerical_DfgRebuild_h__)
#define __numerical_DfgRebuild_h__

#include "../../defs.h"

#include "../../../ads/counter/CounterWithReset.h"

#include <iostream>
#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DfgRebuild)
#define DEBUG_DfgRebuild
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Counter for rebuilding the discrete, finite generator data structure.
/*!
  This is a base class for discrete, finite generators.  It provides the 
  interface functions for the rebuild counter.
*/
template<bool IsUsed = true>
class DfgRebuildCounter;


//! Counter for rebuilding the discrete, finite generator data structure.
/*!
  This is a base class for discrete, finite generators.  It provides the 
  interface functions for the rebuild counter.
*/
template<>
class DfgRebuildCounter<true> {
  //
  // Member data.
  //
private:

  //! The number of times you can set the PMF between rebuilds.
  ads::CounterWithReset<int> _counter;
  
  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DfgRebuildCounter();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Construct from the number of steps between rebuilds.
  explicit
  DfgRebuildCounter(const int stepsBetweenRebuilds) :
    _counter(stepsBetweenRebuilds)
  {}

  //! Copy constructor.
  DfgRebuildCounter(const DfgRebuildCounter& other) :
    _counter(other._counter)
  {}

  //! Assignment operator.
  DfgRebuildCounter&
  operator=(const DfgRebuildCounter& other) {
    if (this != &other) {
      _counter = other._counter;
    }
    return *this;
  }

  //! Destructor.
  ~DfgRebuildCounter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
protected:

  //! Return true if the data structure should be rebuilt.
  bool
  shouldRebuild() const {
    return _counter() <= 0;
  }

public:

  //! Get the number of steps between rebuilds.
  int
  getStepsBetweenRebuilds() const {
    return _counter.getReset();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgRebuildCounter& other) const {
    return _counter == other._counter;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
protected:

  //! Reset the rebuild counter.
  void
  resetRebuildCounter() {
    _counter.reset();
  }

  //! Decrement the rebuild counter.
  void
  decrementRebuildCounter() {
    --_counter;
  }

  //! Decrement the rebuild counter by the specified amount.
  void
  decrementRebuildCounter(const int n) {
    _counter -= n;
  }

public:

  //! Set the number of steps between rebuilds.
  void
  setStepsBetweenRebuilds(const int n) {
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
    out << "Steps between rebuilds = " << _counter.getReset() << "\n"
	<< "Steps until next rebuild = " << _counter() << "\n";
  }

  //@}
};






//! Counter for a discrete, finite generator that never needs rebuilding.
/*!
  This is a base class for discrete, finite generators.  It provides the 
  interface functions for the rebuild counter.
*/
template<>
class DfgRebuildCounter<false> {
  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DfgRebuildCounter();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Construct from the number of steps between rebuilds.
  explicit
  DfgRebuildCounter(const int stepsBetweenRebuilds)
  {}

  //! Copy constructor.
  DfgRebuildCounter(const DfgRebuildCounter& other)
  {}

  //! Assignment operator.
  DfgRebuildCounter&
  operator=(const DfgRebuildCounter& other) {
    return *this;
  }

  //! Destructor.
  ~DfgRebuildCounter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
protected:

  //! Return true if the data structure should be rebuilt.
  bool
  shouldRebuild() const {
    return false;
  }

public:

  //! Get the number of steps between rebuilds.
  int
  getStepsBetweenRebuilds() const {
    return std::numeric_limits<int>::max();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgRebuildCounter& other) const {
    return true;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
protected:

  //! Reset the rebuild counter.
  void
  resetRebuildCounter() {
  }

  //! Decrement the rebuild counter.
  void
  decrementRebuildCounter() {
  }

  //! Decrement the rebuild counter by the specified amount.
  void
  decrementRebuildCounter(const int n) {
  }

public:

  //! Set the number of steps between rebuilds.
  void
  setStepsBetweenRebuilds(const int n) {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    out << "This data structure is never rebuilt.\n";
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
