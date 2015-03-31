// -*- C++ -*-

/*! 
  \file CounterWithReset.h
  \brief An integer counter with a reset value.
*/

#if !defined(__ads_counter_CounterWithReset_h__)
#define __ads_counter_CounterWithReset_h__

#include "Counter.h"

BEGIN_NAMESPACE_ADS

//! An integer counter with a reset value.
/*!
  The default integer type is \c int .

  This class inherits functions to access and manipulate the counter value from 
  Counter .
*/
template<typename _Integer = int>
class CounterWithReset :
  public Counter<_Integer> {
  //
  // Private types.
  //
private:

  //! The base counter.
  typedef Counter<_Integer> Base;

  //
  // Public types.
  //
public:

  //! The integer type.
  typedef typename Base::Integer Integer;

  //
  // Member data.
  //
private:

  Integer _reset;

  //--------------------------------------------------------------------------
  //! \name Constructors, etc.
  //@{
public:

  //! Construct from the reset value.
  CounterWithReset(const Integer resetValue = 0) :
    Base(resetValue),
    _reset(resetValue)
  {}

  //! Copy constructor.
  CounterWithReset(const CounterWithReset& other) :
    Base(other),
    _reset(other._reset)
  {}

  //! Assignment operator.
  const CounterWithReset& 
  operator=(const CounterWithReset& other) {
    // Avoid assignment to self
    if (&other != this) {
      Base::operator=(other);
      _reset = other._reset;
    }
    // Return *this so assignments can chain
    return *this;
  }

  //! Destructor.
  ~CounterWithReset()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the reset value.
  Integer
  getReset() const {
    return _reset;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the reset value and reset the counter.
  void
  setReset(const Integer resetValue) {
    _reset = resetValue;
    reset();
  }

  //! Reset the counter.
  void
  reset() {
    Base::operator=(_reset);
  }

  //@}
};

//! Return true if the counters and reset values are equal.
/*! \relates CounterWithReset */
template<typename _Integer>
inline
bool
operator==(const CounterWithReset<_Integer>& x, 
	   const CounterWithReset<_Integer>& y) {
  return static_cast<const Counter<_Integer>&>(x) ==
    static_cast<const Counter<_Integer>&>(y) && 
    x.getReset() == y.getReset();
}

//! Return true if the counters and reset values are not equal.
/*! \relates CounterWithReset */
template<typename _Integer>
inline
bool
operator!=(const CounterWithReset<_Integer>& x, 
	   const CounterWithReset<_Integer>& y) {
  return !(x() == y());
}

END_NAMESPACE_ADS

#endif
