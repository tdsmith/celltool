// -*- C++ -*-

/*! 
  \file Timer.h
  \brief Implements a class for a timer.
*/

#if !defined(__ads_Timer_h__)
#define __ads_Timer_h__

#include "../defs.h"

#include <ctime>
#include <cassert>
#include <limits>

BEGIN_NAMESPACE_ADS

//! A timer that measures ellapsed time in seconds.
class Timer {
public:

  //! The number type for seconds.
  typedef double Number;

private:

  clock_t _start;

public:

  //! Default constructor.
  Timer() :
    // Initialize with an invalid time.
    _start(std::numeric_limits<clock_t>::max())
  {}

  //! Copy constructor.
  Timer(const Timer& x) :
    _start(x._start)
  {}

  //! Assignment operator.
  const Timer& 
  operator=(const Timer& x) {
    // Avoid assignment to self
    if (&x != this) {
      _start = x._start;
    }
    // Return *this so assignments can chain
    return *this;
  }

  //! Trivial destructor.
  ~Timer()
  {}

  //! Start/reset the timer.
  void 
  tic() {
    _start = std::clock();
  }

  //! Return the time in seconds since the last tic.
  Number 
  toc() {
    clock_t end = std::clock();
    // Make sure tic() has been called and the call worked.
    assert(_start != clock_t(- 1));
    // Make sure the end time worked.
    assert(end != clock_t(-1));
    // Return the elapsed time in seconds.
    return Number(end - _start) / CLOCKS_PER_SEC;
  }

};

END_NAMESPACE_ADS

#endif
