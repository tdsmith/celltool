// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/linearSearch.h
  \brief Probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_random_discreteFinite_linearSearch_h__)
#define __numerical_random_discreteFinite_linearSearch_h__

#include "../../defs.h"

#include "../../../third-party/loki/static_check.h"
#include "../../../third-party/loki/TypeManip.h"

// CONTINUE
#include <iostream>

#include <iterator>
#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_random_discreteFinite_linearSearch)
#define DEBUG_numerical_random_discreteFinite_linearSearch
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Linear search with the chop-down method.  Do not use branching.
template<typename InputConstIterator, typename T>
int
linearSearchChopDownNoBranching(InputConstIterator begin,
				InputConstIterator end, T r);

//! Unguarded linear search with the chop-down method.
template<typename RandomAccessConstIterator, typename T>
int
linearSearchChopDownUnguarded(RandomAccessConstIterator begin,
			      RandomAccessConstIterator end, T r);


//! Guarded linear search with the chop-down method.
template<typename RandomAccessConstIterator, typename T>
int
linearSearchChopDownGuarded(RandomAccessConstIterator begin,
			    RandomAccessConstIterator end, T r);


//! Linear search with the chop-down method.
template<int N, typename Iterator>
class LinearSearchChopDown {
public:
  //! The number type.
  typedef typename std::iterator_traits<Iterator>::value_type Number;

  //! Linear search without branching.
  static
  Number
  result(Iterator i, Number r, Loki::Int2Type<false> /*UseBranching*/) {
    r -= *i;
    return (r >= 0) + LinearSearchChopDown<N-1, Iterator>::result
      (i + 1, r, Loki::Int2Type<false>());
#if 0
    return (r >= *i) + LinearSearchChopDown<N-1, Iterator>::result
      (i + 1, r - *i, Loki::Int2Type<false>());
#endif
  }

  //! Linear search with branching.
  static
  Number
  result(Iterator i, Number r, Loki::Int2Type<true> /*UseBranching*/) {
    if (r < *i) {
      return 0;
    }
    return 1 + LinearSearchChopDown<N-1, Iterator>::result
      (i + 1, r - *i, Loki::Int2Type<true>());
  }

#if 0
  // Build-up version.
  static
  Number
  result(Iterator i, Number r, Number s) {
    return (r >= s + *i) + 
      LinearSearchChopDown<N-1, Iterator>::result(i + 1, r, s + *i);
  }
#endif
};

//! Linear search. Specialization for one element.
template<typename Iterator>
class LinearSearchChopDown<1, Iterator> {
public:
  //! The number type.
  typedef typename std::iterator_traits<Iterator>::value_type Number;

  //! Perform the linear search.
  template<bool UseBranching>
  static
  Number
  result(Iterator i, Number r, Loki::Int2Type<UseBranching> /*dummy*/) {
    return 0;
  }

#if 0
  // Build-up version.
  static
  Number
  result(Iterator i, Number r, Number s) {
    return 0;
  }
#endif
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_discreteFinite_linearSearch_ipp__
#include "linearSearch.ipp"
#undef __numerical_random_discreteFinite_linearSearch_ipp__

#endif
