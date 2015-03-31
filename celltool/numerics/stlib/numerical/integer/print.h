// -*- C++ -*-

/*!
  \file numerical/integer/print.h
  \brief Select an integer type with at least the specified number of print.
*/

#if !defined(__numerical_integer_print_h__)
#define __numerical_integer_print_h__

#include "../defs.h"

#include "../../third-party/loki/TypeManip.h"

#include <limits>
#include <iostream>

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_integer_print)
#define DEBUG_numerical_integer_print
#endif

BEGIN_NAMESPACE_NUMERICAL

//-----------------------------------------------------------------------------
//! \defgroup numerical_integer_print Print the bits of an integer.
//@{

//! Print the bits of the integer.
template<typename _Integer>
void
printBits(std::ostream& out, _Integer x);


//! Print the specified bits of the integer.
template<typename _Integer>
void
printBits(std::ostream& out, _Integer x, int indexBeginning, int indexEnd);

//\@}

END_NAMESPACE_NUMERICAL

#define __numerical_integer_print_ipp__
#include "print.ipp"
#undef __numerical_integer_print_ipp__

#endif
