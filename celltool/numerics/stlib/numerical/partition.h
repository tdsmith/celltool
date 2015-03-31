// -*- C++ -*-

/*! 
  \file partition.h
  \brief A fair partition of an integer.
*/

/*!
  \page partition Fair Partition Package.

  This package has the 
  numerical::partition(const int x,const int n,const int i)
  function for partitioning an integer.
*/

#if !defined(__numerical_partition_h__)
#define __numerical_partition_h__

#include "defs.h"

#include <algorithm>

#include <cassert>

#ifdef _OPENMP
#include <omp.h>
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Return the i_th fair partition of x into n parts.
/*!
  Partition \c x into \c n fair parts.  Return the i_th part.  The parts differ
  by at most one and are in non-increasing order.

  \param x is the number to partition.
  \param n is the number of partitions.
  \param i is the requested partition.

  \pre
  \c x is non-negative.
  \c n is positive.
  \c i is in the range [0..n).
*/
inline
int
getPartition(const int x, const int n, const int i) {
#ifdef DEBUG_numerical
  assert(x >= 0 && n > 0 && 0 <= i && i < n);
#endif
  int p = x / n;
  if (i < x % n) {
    ++p;
  }
  return p;
}

//! Return this thread's fair partition of x.
/*!
  \param x is the number to partition. It must be non-negative.

  \pre The function must be called from within a parallel region.
*/
inline
int
getPartition(const int x) {
#ifdef _OPENMP
#ifdef DEBUG_numerical
  assert(omp_in_parallel());
#endif
  // Partition with the number of threads and this thread's number.
  return partition(x, omp_get_num_threads(), omp_get_thread_num());
#else
  // Serial behavior.
  return x;
#endif
}

//! Compute the i_th fair partition range of x into n ranges.
/*!
  Partition \c x into \c n fair ranges.  Compute the i_th range.  The lengths
  of the ranges differ by at most one and are in non-increasing order.

  \param x is the number to partition.
  \param n is the number of partitions.
  \param i is the requested range.
  \param a is the begining of the range.
  \param b is the end of the open range, [a..b).

  \pre
  \c x is non-negative.
  \c n is positive.
  \c i is in the range [0..n).
*/
inline
void
getPartitionRange(const int x, const int n, const int i, int* a, int* b) {
  const int p = x / n;
  *a = p * i;
  *a += std::min(i, x % n);
  *b = *a + getPartition(x, n, i);
}

//! Compute this thread's fair partition range of x.
/*!
  \param x is the number to partition.
  \param a is the begining of the range.
  \param b is the end of the open range, [a..b).

  \pre The function must be called from within a parallel region.
*/
inline
void
getPartitionRange(const int x, int* a, int* b) {
#ifdef _OPENMP
#ifdef DEBUG_numerical
  assert(omp_in_parallel());
#endif
  // Partition with the number of threads and this thread's number.
  getPartitionRange(x, omp_get_num_threads(), omp_get_thread_num(), a, b);
#else
  // Serial behavior.
  *a = 0;
  *b = x;
#endif
}

//! Compute the fair partition ranges of x into n ranges.
/*!
  Partition \c x into \c n fair ranges.  Compute the delimiters for each range.
  The lengths of the ranges differ by at most one and are in non-increasing
  order.

  \param x is the number to partition.
  \param n is the number of partitions.
  \param delimiters is the output iterator for range delimiters. n + 1 integers
  will be written to this iterator.

  \pre
  \c x is non-negative.
  \c n is positive.
*/
template<typename _OutputIterator>
inline
void
computePartitions(const int x, const int n, _OutputIterator delimiters) {
  const int p = x / n;
  int d = 0;
  *delimiters++ = d;
  int i;
  for (i = 0; i < x % n; ++i) {
    *delimiters++ = d += p + 1;
  }
  for (; i != n - 1; ++i) {
    *delimiters++ = d += p;
  }
  *delimiters++ = x;
}

END_NAMESPACE_NUMERICAL

#endif
