// -*- C++ -*-

/*! 
  \file sort.h
  \brief Contains the sorting functions.
*/

#if !defined(__ads_algorithm_sort_h__)
#define __ads_algorithm_sort_h__

#include "Triplet.h"

#include "../functor/compose.h"
#include "../functor/select.h"

#include <algorithm>
#include <vector>

#include <cassert>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup algorithm_sort Algorithm: Sorting functions */
// @{


//! Sort the two ranges together, using the first for comparisons.
/*!
  Use \c Compare for comparing elements.  This class must be specified 
  explicitly.
*/
template<typename RandomAccessIterator1, typename RandomAccessIterator2,
	 typename Compare>
void
sortTogether(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
	     RandomAccessIterator2 begin2, RandomAccessIterator2 end2,
	     Compare compare1);


//! Sort the two ranges together, using the first for comparisons.
template<typename RandomAccessIterator1, typename RandomAccessIterator2>
inline
void
sortTogether(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
	     RandomAccessIterator2 begin2, RandomAccessIterator2 end2) {
  sortTogether(begin1, end1, begin2, end2, 
	       std::less<typename std::iterator_traits<RandomAccessIterator1>::
	       value_type>());
}


//! Sort the three ranges together, using the first for comparisons.
/*!
  Use \c Compare for comparing elements.  This class must be specified 
  explicitly.
*/
template<typename RandomAccessIterator1, typename RandomAccessIterator2,
	 typename RandomAccessIterator3, typename Compare>
void
sortTogether(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
	     RandomAccessIterator2 begin2, RandomAccessIterator2 end2,
	     RandomAccessIterator3 begin3, RandomAccessIterator3 end3,
	     Compare compare1);


//! Sort the two ranges together, using the first for comparisons.
template<typename RandomAccessIterator1, typename RandomAccessIterator2,
	 typename RandomAccessIterator3>
inline
void
sortTogether(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
	     RandomAccessIterator2 begin2, RandomAccessIterator2 end2,
	     RandomAccessIterator3 begin3, RandomAccessIterator3 end3) {
  sortTogether(begin1, end1, begin2, end2, begin3, end3,
	       std::less<typename std::iterator_traits<RandomAccessIterator1>::
	       value_type>());
}


//! Compute the order for the elements.
template<typename InputIterator, typename IntOutputIterator>
void
computeOrder(InputIterator begin, InputIterator end, IntOutputIterator order);


//! Order the elements by rank.
template<typename RandomAccessIterator, typename IntInputIterator>
void
orderByRank(RandomAccessIterator begin, RandomAccessIterator end,
	    IntInputIterator ranks);


// @}

END_NAMESPACE_ADS

#define __ads_algorithm_sort_ipp__
#include "sort.ipp"
#undef __ads_algorithm_sort_ipp__

#endif
