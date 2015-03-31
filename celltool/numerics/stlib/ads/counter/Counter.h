// -*- C++ -*-

/*! 
  \file Counter.h
  \brief An integer counter.
*/

#if !defined(__ads_counter_Counter_h__)
#define __ads_counter_Counter_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! Base class for integer counters.
template<typename _Integer>
class Counter {
  //
  // Public types.
  //
public:

  //! The integer type.
  typedef _Integer Integer;

  //
  // Member data.
  //
private:

  Integer _counter;

  //--------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{
protected:

  //! Construct from the initial value.
  Counter(const Integer initialValue) :
    _counter(initialValue)
  {}

  //! Copy constructor.
  Counter(const Counter& other) :
    _counter(other._counter)
  {}

  //! Assignment operator.
  const Counter& 
  operator=(const Counter& other) {
    // Avoid assignment to self
    if (&other != this) {
      _counter = other._counter;
    }
    // Return *this so assignments can chain
    return *this;
  }

  //! Destructor.
  ~Counter()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Return the value of the counter.
  Integer
  operator()() const {
    return _counter;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the counter.
  Counter&
  operator=(const Integer value) {
    _counter = value;
    return *this;
  }

  //! Offset the counter.
  Counter&
  operator+=(const Integer value) {
    _counter += value;
    return *this;
  }

  //! Offset the counter.
  Counter&
  operator-=(const Integer value) {
    _counter -= value;
    return *this;
  }

  //! Increment the counter.
  Counter&
  operator++() {
    ++_counter;
    return *this;
  }

  //! Decrement the counter.
  Counter&
  operator--() {
    --_counter;
    return *this;
  }

  //@}
};

//! Return true if the counters are equal.
/*! \relates Counter */
template<typename _Integer>
inline
bool
operator==(const Counter<_Integer>& x, const Counter<_Integer>& y) {
  return x() == y();
}

//! Return true if the counters are not equal.
/*! \relates Counter */
template<typename _Integer>
inline
bool
operator!=(const Counter<_Integer>& x, const Counter<_Integer>& y) {
  return x() != y();
}

END_NAMESPACE_ADS

#endif
