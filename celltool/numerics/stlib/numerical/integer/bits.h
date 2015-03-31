// -*- C++ -*-

/*! 
  \file numerical/integer/bits.h
  \brief Select an integer type with at least the specified number of bits.
*/

#if !defined(__numerical_integer_bits_h__)
#define __numerical_integer_bits_h__

#include "../defs.h"

#include "../../ads/array/FixedArray.h"
#include "../../third-party/loki/static_check.h"

#include <boost/array.hpp>

#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_integer_bits)
#define DEBUG_numerical_integer_bits
#endif

BEGIN_NAMESPACE_NUMERICAL

namespace {

  template<int enumeration>
  struct
  IntegerTypes;

  template<>
  struct
  IntegerTypes<1> {
    typedef long Type;
  };

  template<>
  struct
  IntegerTypes<2> {
    typedef int Type;
  };

  template<>
  struct
  IntegerTypes<3> {
    typedef short Type;
  };

  template<>
  struct
  IntegerTypes<4> {
    typedef char Type;
  };

}

//! Select an integer type with at least the specified number of bits.
/*!
  The Boost library has a more sophisticated solution, but this is sufficient
  for my purposes.
*/
template<int Bits>
struct
Integer {
  typedef typename IntegerTypes<
    (Bits - 1 <= std::numeric_limits<long>::digits) +
      (Bits - 1 <= std::numeric_limits<int>::digits) +
      (Bits - 1 <= std::numeric_limits<short>::digits) +
      (Bits - 1 <= std::numeric_limits<char>::digits)>::Type Type;
};



namespace {

  template<int enumeration>
  struct
  UnsignedIntegerTypes;

  template<>
  struct
  UnsignedIntegerTypes<1> {
    typedef unsigned long Type;
  };

  template<>
  struct
  UnsignedIntegerTypes<2> {
    typedef unsigned int Type;
  };

  template<>
  struct
  UnsignedIntegerTypes<3> {
    typedef unsigned short Type;
  };

  template<>
  struct
  UnsignedIntegerTypes<4> {
    typedef unsigned char Type;
  };

}

//! Select an unsigned integer type with at least the specified number of bits.
/*!
  The Boost library has a more sophisticated solution, but this is sufficient
  for my purposes.
*/
template<int Bits>
struct
UnsignedInteger {
  typedef typename UnsignedIntegerTypes<
    (Bits - 1 <= std::numeric_limits<long>::digits) +
      (Bits - 1 <= std::numeric_limits<int>::digits) +
      (Bits - 1 <= std::numeric_limits<short>::digits) +
      (Bits - 1 <= std::numeric_limits<char>::digits)>::Type Type;
};


//! Extract bits, starting with the least significant.
template<typename _Integer, int _Size>
inline
void
getBits(_Integer n, ads::FixedArray<_Size, bool>* bits) {
  LOKI_STATIC_CHECK(_Size > 0, BadDimension);
  (*bits)[0] = n % 2;
  for (int i = 1; i != _Size; ++i) {
    n /= 2;
    (*bits)[i] = n % 2;
  }
}

//! Extract bits, starting with the least significant.
template<typename _Integer, template<typename, int> class _Array, int _Size>
inline
void
getBits(_Integer n, _Array<bool, _Size>* bits) {
  LOKI_STATIC_CHECK(_Size > 0, BadDimension);
  (*bits)[0] = n % 2;
  for (int i = 1; i != _Size; ++i) {
    n /= 2;
    (*bits)[i] = n % 2;
  }
}

//! Reverse the bits of an unsigned integer type.
template<typename _Integer>
inline
_Integer
reverseBits(_Integer source) {
  // Get the least significant bit.
  _Integer reversed = source;
  // The number of shifts we will make.
  int shift = std::numeric_limits<_Integer>::digits - 1;
  // Loop while there are non-zero bits left in the source.
  for (source >>= 1; source; source >>= 1) {
    reversed <<= 1;
    reversed |= source & 1;
    --shift;
  }
  // Do a shift when some of the source's most significant bits are zero.
  reversed <<= shift;

  return reversed;
}

//! Reverse the n least significant bits of an unsigned integer type.
/*!
  The more significant bits will be zero.
*/
template<typename _Integer>
inline
_Integer
reverseBits(_Integer source, int n) {
#ifdef DEBUG_numerical_integer_bits
  assert(n >= 0);
#endif
  _Integer reversed = 0;
  for (; n; --n) {
    reversed <<= 1;
    reversed |= source & 1;
    source >>= 1;
  }
  return reversed;
}

//! Interlace the n least significant bits.
/*!
  The return type must be specified explicitly.  It cannot be deduced from
  the arguments.
*/
template<typename _ResultInteger, int _N, typename _ArgumentInteger>
inline
_ResultInteger
interlaceBits(ads::FixedArray<_N, _ArgumentInteger> sources, const int n) {
#ifdef DEBUG_numerical_integer_bits
  assert(n >= 0);
#endif
  _ResultInteger reversed = 0;
  // Interlace in reverse order.
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != _N; ++j) {
      reversed <<= 1;
      reversed |= sources[j] & 1;
      sources[j] >>= 1;
    }
  }
  // Reverse the bits.
  return reverseBits(reversed, n * _N);
}

//! Interlace the n least significant bits.
/*!
  The return type must be specified explicitly.  It cannot be deduced from
  the arguments.
*/
template<typename _ResultInteger, template<typename, int> class _Array,
	 typename _ArgumentInteger, int _N>
inline
_ResultInteger
interlaceBits(_Array<_ArgumentInteger, _N> sources, const int n) {
#ifdef DEBUG_numerical_integer_bits
  assert(n >= 0);
#endif
  _ResultInteger reversed = 0;
  // Interlace in reverse order.
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != _N; ++j) {
      reversed <<= 1;
      reversed |= sources[j] & 1;
      sources[j] >>= 1;
    }
  }
  // Reverse the bits.
  return reverseBits(reversed, n * _N);
}

//! Unlace the n least significant bits in each coordinate.
/*!
  The return type must be specified explicitly.  It cannot be deduced from
  the arguments.
*/
template<typename _SourceInteger, int _N, typename _TargetInteger>
inline
void
unlaceBits(_SourceInteger source, const int n, 
	   ads::FixedArray<_N, _TargetInteger>* targets) {
#ifdef DEBUG_numerical_integer_bits
  assert(n >= 0);
  assert(std::numeric_limits<_SourceInteger>::digits >= n * _N);
#endif

  // Clear the target coordinates.
  *targets = _TargetInteger(0);
  // Unlace into reverse order targets.
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != _N; ++j) {
      (*targets)[j] <<= 1;
      (*targets)[j] |= source & 1;
      source >>= 1;
    }
  }
  // Reverse the bits.
  for (int j = 0; j != _N; ++j) {
    (*targets)[j] = reverseBits((*targets)[j], n);
  }
}

//! Unlace the n least significant bits in each coordinate.
/*!
  The return type must be specified explicitly.  It cannot be deduced from
  the arguments.
*/
template<typename _SourceInteger, typename _TargetInteger, std::size_t _N>
inline
void
unlaceBits(_SourceInteger source, const std::size_t n, 
	   boost::array<_TargetInteger, _N>* targets) {
#ifdef DEBUG_numerical_integer_bits
  assert(n >= 0);
  assert(std::numeric_limits<_SourceInteger>::digits >= n * _N);
#endif

  // Clear the target coordinates.
  targets->assign(0);
  // Unlace into reverse order targets.
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != _N; ++j) {
      (*targets)[j] <<= 1;
      (*targets)[j] |= source & 1;
      source >>= 1;
    }
  }
  // Reverse the bits.
  for (int j = 0; j != _N; ++j) {
    (*targets)[j] = reverseBits((*targets)[j], n);
  }
}

// CONTINUE
#if 0
//! Unlace the n least significant bits in each coordinate.
/*!
  The return type must be specified explicitly.  It cannot be deduced from
  the arguments.
*/
template<typename _SourceInteger, template<typename, int> class _Array,
	 typename _TargetInteger, int _N>
inline
void
unlaceBits(_SourceInteger source, const int n, 
	   _Array<_TargetInteger, _N>* targets) {
#ifdef DEBUG_numerical_integer_bits
  assert(n >= 0);
  assert(std::numeric_limits<_SourceInteger>::digits >= n * _N);
#endif

  // Clear the target coordinates.
  *targets = _TargetInteger(0);
  // Unlace into reverse order targets.
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != _N; ++j) {
      (*targets)[j] <<= 1;
      (*targets)[j] |= source & 1;
      source >>= 1;
    }
  }
  // Reverse the bits.
  for (int j = 0; j != _N; ++j) {
    (*targets)[j] = reverseBits((*targets)[j], n);
  }
}
#endif

END_NAMESPACE_NUMERICAL

#endif
