// -*- C++ -*-

#ifndef __numerical_integer_print_ipp__
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_NUMERICAL

// The version for signed integers is intentionally not implemented.

template<typename _Integer>
inline
void
printBitsUnsigned(std::ostream& out, const _Integer x,
		  const int indexBeginning, const int indexEnd) {
#ifdef DEBUG_numerical_integer_print
  assert(0 <= indexBeginning && 
	 indexBeginning <= indexEnd &&
	 indexEnd <= std::numeric_limits<_Integer>::digits);
#endif
  _Integer mask = 1 << (indexEnd - 1);
  for (int i = indexBeginning; i != indexEnd; ++i) {
    out << bool(x & mask);
    mask >>= 1;
  }
}

template<typename _Integer>
inline
void
printBits(std::ostream& out, const _Integer x, 
	  const int indexBeginning, const int indexEnd,
	  Loki::Int2Type<false> /*unsigned*/) {
  printBitsUnsigned(out, x, indexBeginning, indexEnd);
}


// Interface functions.

template<typename _Integer>
inline
void
printBits(std::ostream& out, const _Integer x) {
  printBits(out, x, 0, std::numeric_limits<_Integer>::digits);
}


template<typename _Integer>
inline
void
printBits(std::ostream& out, const _Integer x,
	  const int indexBeginning, const int indexEnd) {
  printBits(out, x, indexBeginning, indexEnd,
	    Loki::Int2Type<std::numeric_limits<_Integer>::is_signed>());
}

END_NAMESPACE_NUMERICAL

// End of file.
